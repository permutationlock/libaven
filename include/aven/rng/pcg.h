/*
 * PCG Random Number Generation for C.
 *
 * Copyright 2014 Melissa O'Neill <oneill@pcg-random.org>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * For additional information about the PCG random number generation scheme,
 * including its license and other licensing options, visit
 *
 *       http://www.pcg-random.org
 */

/*
 * This code is derived from the full C implementation, which is in turn
 * derived from the canonical C++ PCG implementation. The C++ version
 * has many additional features and is preferable if you can use C++ in
 * your project.
 */

// Modified to fit in libaven by Aven Bross in 2024

#ifndef AVEN_RNG_PCG_H
#define AVEN_RNG_PCG_H

#include "../../aven.h"
#include "../rng.h"

typedef struct {
    uint64_t state;
    uint64_t inc;
} AvenRngPcg;

static inline uint32_t aven_rng_pcg_rand(AvenRngPcg *pcg) {
    uint64_t oldstate = pcg->state;
    pcg->state = oldstate * 6364136223846793005ULL + pcg->inc;
    uint32_t xorshifted = (uint32_t)(((oldstate >> 18U) ^ oldstate) >> 27U);
    uint32_t rot = (uint32_t)(oldstate >> 59U);
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

static uint32_t aven_rng_pcg_rand_stub(void *pcg) {
    return aven_rng_pcg_rand(pcg);
}

static inline AvenRngPcg aven_rng_pcg_seed(
    uint64_t initstate,
    uint64_t initseq
) {
    AvenRngPcg pcg = {
        .state = 0U,
        .inc = (initseq << 1U) | 1U,
    };
    aven_rng_pcg_rand(&pcg);
    pcg.state += initstate;
    aven_rng_pcg_rand(&pcg);

    return pcg;
}

static inline AvenRng aven_rng_pcg(AvenRngPcg *rng) {
    return (AvenRng){
        .rand = aven_rng_pcg_rand_stub,
        .state = (void *)rng
    };
}

#endif // PCG_RNG_H
