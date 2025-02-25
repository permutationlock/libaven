#ifndef AVEN_THREAD_POOL_H
#define AVEN_THREAD_POOL_H

#include <aven.h>
#include <aven/arena.h>

#if !defined(__STDC_VERSION__) or \
    __STDC_VERSION__ < 201112L or \
    (defined(_WIN32) and !defined(_MSC_VER))

    #define AVEN_THREAD_POOL_USE_PTHREADS
#endif

#ifndef AVEN_THREAD_POOL_USE_PTHREADS
    #include <threads.h>

    typedef void (*AvenThreadPoolJobFn)(void *);

    typedef struct {
        AvenThreadPoolJobFn fn;
        void *args;
    } AvenThreadPoolJob;

    typedef struct {
        Queue(AvenThreadPoolJob) job_queue;
        Slice(thrd_t) workers;
        cnd_t job_cond;
        cnd_t done_cond;
        mtx_t lock;
        size_t jobs_in_progress;
        size_t active_threads;
        bool done;
    } AvenThreadPool;

    static int aven_thread_pool_worker_internal_fn(void *args) {
        AvenThreadPool *thread_pool = args;

        mtx_lock(&thread_pool->lock);
        thread_pool->active_threads += 1;
        mtx_unlock(&thread_pool->lock);

        for (;;) {
            mtx_lock(&thread_pool->lock);
            while (thread_pool->job_queue.used == 0 and !thread_pool->done) {
                cnd_wait(&thread_pool->job_cond, &thread_pool->lock);
            }
            if (thread_pool->done) {
                break;
            }
            thread_pool->jobs_in_progress += 1;
            AvenThreadPoolJob job = queue_pop(thread_pool->job_queue);
            mtx_unlock(&thread_pool->lock);

            job.fn(job.args);

            mtx_lock(&thread_pool->lock);
            thread_pool->jobs_in_progress -= 1;
            if (
                !thread_pool->done and
                thread_pool->jobs_in_progress == 0 and
                thread_pool->job_queue.used == 0
            ) {
                cnd_signal(&thread_pool->done_cond);
            }
            mtx_unlock(&thread_pool->lock);
        }

        thread_pool->active_threads -= 1;
        cnd_signal(&thread_pool->done_cond);
        mtx_unlock(&thread_pool->lock);

        return 0;
    }

    static AvenThreadPool aven_thread_pool_init(
        size_t nthreads,
        size_t njobs,
        AvenArena *arena
    ) {
        AvenThreadPool thread_pool = {
            .job_queue = { .cap = njobs },
            .workers = { .len = nthreads },
        };

        thread_pool.job_queue.ptr = aven_arena_create_array(
            AvenThreadPoolJob,
            arena,
            thread_pool.job_queue.cap
        );
        thread_pool.workers.ptr = aven_arena_create_array(
            thrd_t,
            arena,
            thread_pool.workers.len
        );

        int error = mtx_init(&thread_pool.lock, 0);
        if (error != 0) {
            aven_panic("mtx_init failed");
        }

        error = cnd_init(&thread_pool.job_cond);
        if (error != 0) {
            aven_panic("cnd_init failed");
        }

        error = cnd_init(&thread_pool.done_cond);
        if (error != 0) {
            aven_panic("cnd_init failed");
        }

        return thread_pool;
    }

    static void aven_thread_pool_run(AvenThreadPool *thread_pool) {
        for (size_t i = 0; i < thread_pool->workers.len; i += 1) {
            int error = thrd_create(
                &get(thread_pool->workers, i),
                aven_thread_pool_worker_internal_fn,
                thread_pool
            );
            if (error != 0) {
                aven_panic("thrd_create failed");
            }
        }
    }

    static void aven_thread_pool_submit(
        AvenThreadPool *thread_pool,
        AvenThreadPoolJobFn fn,
        void *args
    ) {
        mtx_lock(&thread_pool->lock);
        queue_push(thread_pool->job_queue) = (AvenThreadPoolJob){
            .fn = fn,
            .args = args,
        };
        cnd_broadcast(&thread_pool->job_cond);
        mtx_unlock(&thread_pool->lock);
    }

    static void aven_thread_pool_wait(AvenThreadPool *thread_pool) {
        mtx_lock(&thread_pool->lock);
        while (
            thread_pool->job_queue.used > 0 or
            (!thread_pool->done and thread_pool->jobs_in_progress > 0) or
            (thread_pool->done and thread_pool->active_threads > 0)
        ) {
            cnd_wait(&thread_pool->done_cond, &thread_pool->lock);
        }
        mtx_unlock(&thread_pool->lock);
    }

    static void aven_thread_pool_halt_and_destroy(AvenThreadPool *thread_pool) {
        mtx_lock(&thread_pool->lock);
        thread_pool->done = true;
        queue_clear(thread_pool->job_queue);
        thread_pool->job_queue.ptr = NULL;
        thread_pool->job_queue.cap = 0;
        cnd_broadcast(&thread_pool->job_cond);
        mtx_unlock(&thread_pool->lock);

        aven_thread_pool_wait(thread_pool);

        for (size_t i = 0; i < thread_pool->workers.len; i += 1) {
            thrd_join(get(thread_pool->workers, i), NULL);
        }

        mtx_destroy(&thread_pool->lock);
        cnd_destroy(&thread_pool->job_cond);
        cnd_destroy(&thread_pool->done_cond);
    }
#else // defined(AVEN_THREAD_POOL_USE_PTHREADS)
    #ifdef _MSC_VER
        #error "pthreads not supported in MSVC"
    #endif

    #include <aven.h>
    #include <aven/arena.h>

    #include <pthread.h>

    typedef void (*AvenThreadPoolJobFn)(void *);

    typedef struct {
        AvenThreadPoolJobFn fn;
        void *args;
    } AvenThreadPoolJob;

    typedef struct {
        Queue(AvenThreadPoolJob) job_queue;
        Slice(pthread_t) workers;
        pthread_cond_t job_cond;
        pthread_cond_t done_cond;
        pthread_mutex_t lock;
        size_t jobs_in_progress;
        size_t active_threads;
        bool done;
    } AvenThreadPool;

    static void *aven_thread_pool_worker_internal_fn(void *args) {
        AvenThreadPool *thread_pool = args;

        pthread_mutex_lock(&thread_pool->lock);
        thread_pool->active_threads += 1;
        pthread_mutex_unlock(&thread_pool->lock);

        for (;;) {
            pthread_mutex_lock(&thread_pool->lock);
            while (thread_pool->job_queue.used == 0 and !thread_pool->done) {
                pthread_cond_wait(&thread_pool->job_cond, &thread_pool->lock);
            }
            if (thread_pool->done) {
                break;
            }
            thread_pool->jobs_in_progress += 1;
            AvenThreadPoolJob job = queue_pop(thread_pool->job_queue);
            pthread_mutex_unlock(&thread_pool->lock);

            job.fn(job.args);

            pthread_mutex_lock(&thread_pool->lock);
            thread_pool->jobs_in_progress -= 1;
            if (
                !thread_pool->done and
                thread_pool->jobs_in_progress == 0 and
                thread_pool->job_queue.used == 0
            ) {
                pthread_cond_signal(&thread_pool->done_cond);
            }
            pthread_mutex_unlock(&thread_pool->lock);
        }

        thread_pool->active_threads -= 1;
        pthread_cond_signal(&thread_pool->done_cond);
        pthread_mutex_unlock(&thread_pool->lock);
        return NULL;
    }

    static AvenThreadPool aven_thread_pool_init(
        size_t nthreads,
        size_t njobs,
        AvenArena *arena
    ) {

        AvenThreadPool thread_pool = {
            .job_queue = { .cap = njobs },
            .workers = { .len = nthreads },
        };

        thread_pool.job_queue.ptr = aven_arena_create_array(
            AvenThreadPoolJob,
            arena,
            thread_pool.job_queue.cap
        );
        thread_pool.workers.ptr = aven_arena_create_array(
            pthread_t,
            arena,
            thread_pool.workers.len
        );

        int error = pthread_mutex_init(&thread_pool.lock, NULL);
        if (error != 0) {
            aven_panic("pthread_mutex_init failed");
        }

        error = pthread_cond_init(&thread_pool.job_cond, NULL);
        if (error != 0) {
            aven_panic("pthread_cond_init failed");
        }

        error = pthread_cond_init(&thread_pool.done_cond, NULL);
        if (error != 0) {
            aven_panic("pthread_cond_init failed");
        }

        return thread_pool;
    }

    static void aven_thread_pool_run(AvenThreadPool *thread_pool) {
        for (size_t i = 0; i < thread_pool->workers.len; i += 1) {
            int error = pthread_create(
                &get(thread_pool->workers, i),
                NULL,
                aven_thread_pool_worker_internal_fn,
                thread_pool
            );
            if (error != 0) {
                aven_panic("pthread_create failed");
            }
        }
    }

    static void aven_thread_pool_submit(
        AvenThreadPool *thread_pool,
        AvenThreadPoolJobFn fn,
        void *args
    ) {
        pthread_mutex_lock(&thread_pool->lock);
        queue_push(thread_pool->job_queue) = (AvenThreadPoolJob){
            .fn = fn,
            .args = args,
        };
        pthread_cond_broadcast(&thread_pool->job_cond);
        pthread_mutex_unlock(&thread_pool->lock);
    }

    static void aven_thread_pool_wait(AvenThreadPool *thread_pool) {
        pthread_mutex_lock(&thread_pool->lock);
        while (
            thread_pool->job_queue.used > 0 or
            (!thread_pool->done and thread_pool->jobs_in_progress > 0) or
            (thread_pool->done and thread_pool->active_threads > 0)
        ) {
            pthread_cond_wait(&thread_pool->done_cond, &thread_pool->lock);
        }
        pthread_mutex_unlock(&thread_pool->lock);
    }

    static void aven_thread_pool_halt_and_destroy(AvenThreadPool *thread_pool) {
        pthread_mutex_lock(&thread_pool->lock);
        thread_pool->done = true;
        queue_clear(thread_pool->job_queue);
        thread_pool->job_queue.ptr = NULL;
        thread_pool->job_queue.cap = 0;
        pthread_cond_broadcast(&thread_pool->job_cond);
        pthread_mutex_unlock(&thread_pool->lock);

        aven_thread_pool_wait(thread_pool);

        for (size_t i = 0; i < thread_pool->workers.len; i += 1) {
            pthread_join(get(thread_pool->workers, i), NULL);
        }

        pthread_mutex_destroy(&thread_pool->lock);
        pthread_cond_destroy(&thread_pool->job_cond);
        pthread_cond_destroy(&thread_pool->done_cond);
    }
#endif // defined(AVEN_THREAD_POOL_USE_PTHREADS)

#endif // AVEN_THREAD_POOL_H
