#ifndef _WIN32
    #error "attempted to build winpthreads for a non-Windows target"
#endif

#include "src/barrier.c"
#include "src/clock.c"
#include "src/cond.c"
#include "src/misc.c"
#include "src/mutex.c"
#include "src/nanosleep.c"
#include "src/rwlock.c"
#include "src/sched.c"
#include "src/sem.c"
#include "src/spinlock.c"
#include "src/thread.c"

