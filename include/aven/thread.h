#ifndef AVEN_THREAD_H
#define AVEN_THREAD_H

#include <aven.h>
#include <aven/arena.h>

#if !defined(__STDC_VERSION__) or \
    __STDC_VERSION__ < 201112L or \
    (defined(_WIN32) and !defined(_MSC_VER))

    #define AVEN_THREAD_USE_PTHREADS
#endif

typedef int (AvenThreadFn)(void *);

#ifndef AVEN_THREAD_USE_PTHREADS
    #include <threads.h>

    typedef thrd_t AvenThread;
    typedef cnd_t AvenThreadCnd;
    typedef mtx_t AvenThreadMtx;

    typedef int (AvenThreadFn)(void *);

    static inline void aven_thread_create(
        AvenThread *thread,
        AvenThreadFn *fn,
        void *arg
    ) {
        int error = thrd_create(thread, fn, arg);
        if (error != thrd_success) {
            aven_panic("thrd_create failed");
        }
    }

    static inline void aven_thread_join(
        AvenThread thread
    ) {
        int error = thrd_join(thread, NULL);
        if (error != thrd_success) {
            aven_panic("thrd_join failed");
        }
    }

    static inline void aven_thread_mtx_init(AvenThreadMtx *mtx) {
        int error = mtx_init(mtx, 0);
        if (error != thrd_success) {
            aven_panic("mtx_init failed");
        }        
    }

    static inline void aven_thread_mtx_lock(AvenThreadMtx *mtx) {
        int error = mtx_lock(mtx);
        assert(error == thrd_success);
    }

    static inline void aven_thread_mtx_unlock(AvenThreadMtx *mtx) {
        int error = mtx_unlock(mtx);
        assert(error == thrd_success);
    }

    static inline void aven_thread_mtx_destroy(AvenThreadMtx *mtx) {
        mtx_destroy(mtx);
    }

    static inline void aven_thread_cnd_init(AvenThreadCnd *cnd) {
        int error = cnd_init(cnd);
        if (error != thrd_success) {
            aven_panic("cnd_init failed");
        }
    }

    static inline void aven_thread_cnd_signal(AvenThreadCnd *cnd) {
        int error = cnd_signal(cnd);
        assert(error == thrd_success);
    }

    static inline void aven_thread_cnd_broadcast(AvenThreadCnd *cnd) {
        int error = cnd_broadcast(cnd);
        assert(error == thrd_success);
    }

    static inline void aven_thread_cnd_destroy(AvenThreadCnd *cnd) {
        cnd_destroy(cnd);
    }

    static inline void aven_thread_cnd_wait(
        AvenThreadCnd *cnd,
        AvenThreadMtx *mtx
    ) {
        int error = cnd_wait(cnd, mtx);
        assert(error == thrd_success);
    }
#else // defined(AVEN_THREAD_USE_PTHREADS)
    #ifdef _MSC_VER
        #error "pthreads not supported in MSVC"
    #endif

    #include <pthread.h>

    typedef pthread_t AvenThread;
    typedef pthread_cond_t AvenThreadCnd;
    typedef pthread_mutex_t AvenThreadMtx;

    static inline void aven_thread_create(
        AvenThread *thread,
        AvenThreadFn *fn,
        void *arg
    ) {
        int error = pthread_create(thread, NULL, (void *(*)(void *))fn, arg);
        if (error != 0) {
            aven_panic("pthread_create failed");
        }
    }

    static inline void aven_thread_join(
        AvenThread thread
    ) {
        int error = pthread_join(thread, NULL);
        if (error != 0) {
            aven_panic("pthread_join failed");
        }
    }

    static inline void aven_thread_mtx_init(AvenThreadMtx *mtx) {
        int error = pthread_mutex_init(mtx, NULL);
        if (error != 0) {
            aven_panic("pthread_mutx_init failed");
        }        
    }

    static inline void aven_thread_mtx_lock(AvenThreadMtx *mtx) {
        int error = pthread_mutex_lock(mtx);
        assert(error == 0);
    }

    static inline void aven_thread_mtx_unlock(AvenThreadMtx *mtx) {
        int error = pthread_mutex_unlock(mtx);
        assert(error == 0);
    }

    static inline void aven_thread_mtx_destroy(AvenThreadMtx *mtx) {
        pthread_mutex_destroy(mtx);
    }

    static inline void aven_thread_cnd_init(AvenThreadCnd *cnd) {
        int error = pthread_cond_init(cnd, NULL);
        if (error != 0) {
            aven_panic("pthread_cond_init failed");
        }
    }

    static inline void aven_thread_cnd_signal(AvenThreadCnd *cnd) {
        int error = pthread_cond_signal(cnd);
        assert(error == 0);
    }

    static inline void aven_thread_cnd_broadcast(AvenThreadCnd *cnd) {
        int error = pthread_cond_broadcast(cnd);
        assert(error == 0);
    }

    static inline void aven_thread_cnd_destroy(AvenThreadCnd *cnd) {
        pthread_cond_destroy(cnd);
    }

    static inline void aven_thread_cnd_wait(
        AvenThreadCnd *cnd,
        AvenThreadMtx *mtx
    ) {
        int error = pthread_cond_wait(cnd, mtx);
        assert(error == 0);
    }
#endif // defined(AVEN_THREAD_USE_PTHREADS)

#endif // AVEN_THREAD_H
