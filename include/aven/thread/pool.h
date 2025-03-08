#ifndef AVEN_THREAD_POOL_H
#define AVEN_THREAD_POOL_H

#include "../../aven.h"
#include "../arena.h"
#include "../thread.h"

typedef void (*AvenThreadPoolJobFn)(void *);

typedef struct {
    AvenThreadPoolJobFn fn;
    void *args;
} AvenThreadPoolJob;
typedef Slice(AvenThreadPoolJob) AvenThreadPoolJobSlice;

typedef struct {
    Queue(AvenThreadPoolJob) job_queue;
    Slice(AvenThread) workers;
    AvenThreadCnd job_cond;
    AvenThreadCnd done_cond;
    AvenThreadMtx lock;
    size_t jobs_in_progress;
    size_t active_threads;
    bool done;
} AvenThreadPool;

static int aven_thread_pool_worker_internal_fn(void *args) {
    AvenThreadPool *thread_pool = args;

    aven_thread_mtx_lock(&thread_pool->lock);
    thread_pool->active_threads += 1;
    aven_thread_mtx_unlock(&thread_pool->lock);

    for (;;) {
        aven_thread_mtx_lock(&thread_pool->lock);
        while (thread_pool->job_queue.used == 0 and !thread_pool->done) {
            aven_thread_cnd_wait(&thread_pool->job_cond, &thread_pool->lock);
        }
        if (thread_pool->done) {
            break;
        }
        thread_pool->jobs_in_progress += 1;
        AvenThreadPoolJob job = queue_pop(thread_pool->job_queue);
        aven_thread_mtx_unlock(&thread_pool->lock);

        job.fn(job.args);

        aven_thread_mtx_lock(&thread_pool->lock);
        thread_pool->jobs_in_progress -= 1;
        if (
            !thread_pool->done and
            thread_pool->jobs_in_progress == 0 and
            thread_pool->job_queue.used == 0
        ) {
            aven_thread_cnd_signal(&thread_pool->done_cond);
        }
        aven_thread_mtx_unlock(&thread_pool->lock);
    }

    thread_pool->active_threads -= 1;
    aven_thread_cnd_signal(&thread_pool->done_cond);
    aven_thread_mtx_unlock(&thread_pool->lock);

    return 0;
}

static inline AvenThreadPool aven_thread_pool_init(
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
        AvenThread,
        arena,
        thread_pool.workers.len
    );

    aven_thread_mtx_init(&thread_pool.lock);
    aven_thread_cnd_init(&thread_pool.job_cond);
    aven_thread_cnd_init(&thread_pool.done_cond);

    return thread_pool;
}

static inline void aven_thread_pool_run(AvenThreadPool *thread_pool) {
    for (size_t i = 0; i < thread_pool->workers.len; i += 1) {
        aven_thread_create(
            &get(thread_pool->workers, i),
            aven_thread_pool_worker_internal_fn,
            thread_pool
        );
    }
}

static inline void aven_thread_pool_submit(
    AvenThreadPool *thread_pool,
    AvenThreadPoolJobFn fn,
    void *args
) {
    aven_thread_mtx_lock(&thread_pool->lock);
    queue_push(thread_pool->job_queue) = (AvenThreadPoolJob){
        .fn = fn,
        .args = args,
    };
    aven_thread_cnd_broadcast(&thread_pool->job_cond);
    aven_thread_mtx_unlock(&thread_pool->lock);
}

static inline void aven_thread_pool_submit_slice(
    AvenThreadPool *thread_pool,
    AvenThreadPoolJobSlice jobs
) {
    aven_thread_mtx_lock(&thread_pool->lock);
    for (size_t i = 0; i < jobs.len; i += 1) {
        queue_push(thread_pool->job_queue) = get(jobs, i);
    }
    aven_thread_cnd_broadcast(&thread_pool->job_cond);
    aven_thread_mtx_unlock(&thread_pool->lock);
}

static inline void aven_thread_pool_wait(AvenThreadPool *thread_pool) {
    aven_thread_mtx_lock(&thread_pool->lock);
    while (
        thread_pool->job_queue.used > 0 or
        (!thread_pool->done and thread_pool->jobs_in_progress > 0) or
        (thread_pool->done and thread_pool->active_threads > 0)
    ) {
        aven_thread_cnd_wait(&thread_pool->done_cond, &thread_pool->lock);
    }
    aven_thread_mtx_unlock(&thread_pool->lock);
}

static inline void aven_thread_pool_halt_and_destroy(
    AvenThreadPool *thread_pool
) {
    aven_thread_mtx_lock(&thread_pool->lock);
    thread_pool->done = true;
    queue_clear(thread_pool->job_queue);
    thread_pool->job_queue.ptr = NULL;
    thread_pool->job_queue.cap = 0;
    aven_thread_cnd_broadcast(&thread_pool->job_cond);
    aven_thread_mtx_unlock(&thread_pool->lock);

    aven_thread_pool_wait(thread_pool);

    for (size_t i = 0; i < thread_pool->workers.len; i += 1) {
        aven_thread_join(get(thread_pool->workers, i));
    }

    aven_thread_mtx_destroy(&thread_pool->lock);
    aven_thread_cnd_destroy(&thread_pool->job_cond);
    aven_thread_cnd_destroy(&thread_pool->done_cond);
}

#endif // AVEN_THREAD_POOL_H
