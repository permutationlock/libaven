#ifndef AVEN_TIME_H
#define AVEN_TIME_H

#include "../aven.h"

#ifdef _WIN32
    typedef struct {
        int64_t tv_sec;
        long tv_nsec;
    } AvenTimeInst;
#else
    #include <time.h>
    typedef struct timespec AvenTimeInst;
#endif

#define AVEN_TIME_NSEC_PER_SEC (1000L * 1000L * 1000L)
#define AVEN_TIME_USEC_PER_SEC (1000L * 1000L)
#define AVEN_TIME_MSEC_PER_SEC (1000L)
#define AVEN_TIME_NSEC_PER_MSEC (1000L * 1000L)
#define AVEN_TIME_NSEC_PER_USEC (1000L)
#define AVEN_TIME_USEC_PER_MSEC (1000L)

AVEN_FN AvenTimeInst aven_time_now(void);
AVEN_FN int64_t aven_time_since(AvenTimeInst end, AvenTimeInst start);
AVEN_FN void aven_time_sleep_ms(uint32_t wait_ms);

#ifdef AVEN_IMPLEMENTATION

#ifdef _WIN32
    AVEN_FN AvenTimeInst aven_time_now(void) {
        AVEN_WIN32_FN(int) QueryPerformanceFrequency(int64_t *freq);
        AVEN_WIN32_FN(int) QueryPerformanceCounter(int64_t *count);

        int64_t freq;
        int success = QueryPerformanceFrequency(&freq);
        assert(success != 0);

        int64_t count;
        success = QueryPerformanceCounter(&count);
        assert(success != 0);

        AvenTimeInst now = {
            .tv_sec = count / freq,
            .tv_nsec = (int)(
                ((count % freq) * AVEN_TIME_NSEC_PER_SEC + (freq >> 1)) / freq
            ),
        };
        if (now.tv_nsec >= AVEN_TIME_NSEC_PER_SEC) {
            now.tv_sec += 1;
            now.tv_nsec -= AVEN_TIME_NSEC_PER_SEC;
        }

        return now;
    }

    AVEN_FN void aven_time_sleep_ms(uint32_t wait_ms) {
        AVEN_WIN32_FN(void) Sleep(uint32_t);
        Sleep(wait_ms);
    } 
#else // !defined(_WIN32)
    #include <errno.h>

    #if !defined(_POSIX_C_SOURCE) or _POSIX_C_SOURCE < 199309L
        #error "clock_gettime requires _POSIX_C_SOURCE >= 199309L"
    #endif

    AVEN_FN AvenTimeInst aven_time_now(void) {
        AvenTimeInst now;
        int error = clock_gettime(CLOCK_MONOTONIC, &now);
        assert(error == 0);
        return now;
    }

    #if defined(__EMSCRIPTEN__)
        AVEN_FN void aven_time_sleep_ms(uint32_t wait_ms) {
            void emscripten_sleep(unsigned int);

            emscripten_sleep((unsigned int)wait_ms);
        }
    #else // !defined(__EMSCRIPTEN__)
        AVEN_FN void aven_time_sleep_ms(uint32_t wait_ms) {
            time_t wait_sec = 0;
            if (wait_ms > AVEN_TIME_MSEC_PER_SEC) {
                wait_sec = (time_t)(wait_ms / AVEN_TIME_MSEC_PER_SEC);
                wait_ms = wait_ms % AVEN_TIME_MSEC_PER_SEC;
            }
            AvenTimeInst remaining_time = {
                .tv_sec = wait_sec,
                .tv_nsec = (int64_t)wait_ms * (int64_t)AVEN_TIME_NSEC_PER_MSEC,
            };

            int error = 0;
            do {
                AvenTimeInst wait_time = remaining_time;
                error = nanosleep(&wait_time, &remaining_time);
            } while (error < 0 and errno == EINTR);
            assert(error == 0);
        }
    #endif // !defined(__EMSCRIPTEN__)

#endif // !defined(_WIN32)

AVEN_FN int64_t aven_time_since(AvenTimeInst end, AvenTimeInst start) {
    int64_t seconds = (int64_t)end.tv_sec - (int64_t)start.tv_sec;
    int64_t sec_diff = seconds * AVEN_TIME_NSEC_PER_SEC;
    int64_t nsec_diff = (int64_t)end.tv_nsec - (int64_t)start.tv_nsec;
    return sec_diff + nsec_diff;
}

#endif // AVEN_IMPLEMENTATION

#endif // AVEN_TIME_H
