#ifndef AVEN_THREAD_SPINLOCK_H
#define AVEN_THREAD_SPINLOCK_H

#include "../../aven.h"

#if !defined(__STDC_VERSION__) or __STDC_VERSION__ < 201112L
    #error "C11 or later is required"
#endif

#include <stdatomic.h>

typedef atomic_bool AvenThreadSpinlock;

static inline void aven_thread_spinlock_init(AvenThreadSpinlock *lock) {
    atomic_init(lock, false);
}

static inline void aven_thread_spinlock_lock(AvenThreadSpinlock *lock) {
    for (;;) {
        if (
            !atomic_exchange_explicit(
                lock,
                true,
                memory_order_acquire
            )
        ) {
            return;
        }
        while (atomic_load_explicit(lock, memory_order_relaxed)) {
#if __has_builtin(__builtin_ia32_pause)
            __builtin_ia32_pause();
#endif
        }
    }
}

static inline void aven_thread_spinlock_unlock(AvenThreadSpinlock *lock) {
    atomic_store_explicit(lock, false, memory_order_release);
}

#endif // AVEN_THREAD_SPINLOCK_H
