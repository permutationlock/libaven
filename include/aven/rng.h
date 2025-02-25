#ifndef AVEN_RNG_H
#define AVEN_RNG_H

#include "../aven.h"

typedef uint32_t (*aven_rng_rand_fn)(void *state);

typedef struct {
    aven_rng_rand_fn rand;
    void *state;
} AvenRng;

static inline uint32_t aven_rng_rand(AvenRng rng) {
    return rng.rand(rng.state);
}

static inline uint32_t aven_rng_rand_bounded(AvenRng rng, uint32_t bound) {
    uint32_t threshold = -bound % bound;

    uint32_t n;
    do {
        n = aven_rng_rand(rng);
    } while (n < threshold);

    return n % bound;
}

static inline float aven_rng_randf(AvenRng rng) {
    return (float)(aven_rng_rand(rng) >> 8) * (1.0f / (1UL << 24));
}

#endif // RNG_H
