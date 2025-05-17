// wyhash source from: https://github.com/wangyi-fudan/wyhash.git
// modified April 2025 by Aven Bross to add namespace prefixes

// This is free and unencumbered software released into the public domain under The Unlicense (http://unlicense.org/)
// main repo: https://github.com/wangyi-fudan/wyhash
// author: 王一 Wang Yi <godspeed_china@yeah.net>
// contributors: Reini Urban, Dietrich Epp, Joshua Haberman, Tommy Ettinger, Daniel Lemire, Otmar Ertl, cocowalla, leo-yuriev, Diego Barrios Romero, paulie-g, dumblob, Yann Collet, ivte-ms, hyb, James Z.M. Gao, easyaspi314 (Devin), TheOneric

/* quick example:
string s="fjsakfdsjkf";
uint64_t hash=wyhash(s.c_str(), s.size(), 0, aven_wyhash_wyp_internal);
*/

#include "../aven.h"

#ifndef aven_wyhash_final_version_4_2
    #define aven_wyhash_final_version_4_2

    #ifndef AVEN_WYHASH_CONDOM
        //protections that produce different results:
        //1: normal valid behavior
        //2: extra protection against entropy loss (probability=2^-63), aka. "blind multiplication"
        #define AVEN_WYHASH_CONDOM 1
    #endif

    #ifndef AVEN_WYHASH_32BIT_MUM
        //0: normal version, slow on 32 bit systems
        //1: faster on 32 bit systems but produces different results, incompatible with aven_wyhash_wy2u0k function
        #define AVEN_WYHASH_32BIT_MUM 0
    #endif

    //includes
    #if defined(_MSC_VER) && defined(_M_X64)
        #include <intrin.h>
        #pragma intrinsic(_umul128)
    #endif

    //likely and unlikely macros
    #if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
        #define aven_wyhash_likely_internal(x) __builtin_expect(x, 1)
        #define aven_wyhash_unlikely_internal(x) __builtin_expect(x, 0)
    #else
        #define aven_wyhash_likely_internal(x) (x)
        #define aven_wyhash_unlikely_internal(x) (x)
    #endif

    //128bit multiply function
    static inline uint64_t aven_wyhash_wyrot_internal(uint64_t x) {
        return (x >> 32) | (x << 32);
    }
    static inline void aven_wyhash_wymum_internal(uint64_t *A, uint64_t *B) {
    #if (AVEN_WYHASH_32BIT_MUM)
        uint64_t hh = (*A >> 32) * (*B >> 32),
        hl = (*A >> 32) * (uint32_t)*B,
        lh = (uint32_t)*A * (*B >> 32),
        ll = (uint64_t)(uint32_t)*A * (uint32_t)*B;
    #if (AVEN_WYHASH_CONDOM > 1)
        *A ^= aven_wyhash_wyrot_internal(hl) ^ hh;
        *B ^= aven_wyhash_wyrot_internal(lh) ^ ll;
    #else
        *A = aven_wyhash_wyrot_internal(hl) ^ hh;
        *B = aven_wyhash_wyrot_internal(lh) ^ ll;
    #endif
    #elif defined(__SIZEOF_INT128__)
        __uint128_t r = *A;
        r *= *B;
    #if (AVEN_WYHASH_CONDOM > 1)
        *A ^= (uint64_t)r;
        *B ^= (uint64_t)(r >> 64);
    #else
        *A = (uint64_t)r;
        *B = (uint64_t)(r >> 64);
    #endif
    #elif defined(_MSC_VER) && defined(_M_X64)
    #if (AVEN_WYHASH_CONDOM > 1)
        uint64_t a, b;
        a = _umul128(*A, *B, &b);
        *A ^= a;
        *B ^= b;
    #else
        *A = _umul128(*A, *B, B);
    #endif
    #else
        uint64_t ha = *A >> 32,
        hb = *B >> 32,
        la = (uint32_t)*A,
        lb = (uint32_t)*B,
        hi,
        lo;
        uint64_t rh = ha * hb,
        rm0 = ha * lb,
        rm1 = hb * la,
        rl = la * lb,
        t = rl + (rm0 << 32),
        c = t < rl;
        lo = t + (rm1 << 32);
        c += lo < t;
        hi = rh + (rm0 >> 32) + (rm1 >> 32) + c;
    #if (AVEN_WYHASH_CONDOM > 1)
        *A ^= lo;
        *B ^= hi;
    #else
        *A = lo;
        *B = hi;
    #endif
    #endif
    }

    //multiply and xor mix function, aka MUM
    static inline uint64_t aven_wyhash_wymix_internal(uint64_t A, uint64_t B) {
        aven_wyhash_wymum_internal(&A, &B);
        return A ^ B;
    }

    //endian macros
    #ifndef AVEN_WYHASH_LITTLE_ENDIAN
        #if defined(_WIN32) || \
                defined(__LITTLE_ENDIAN__) || \
                ( \
                    defined(__BYTE_ORDER__) && \
                    __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__ \
                )
            #define AVEN_WYHASH_LITTLE_ENDIAN 1
        #elif defined(__BIG_ENDIAN__) || \
                ( \
                    defined(__BYTE_ORDER__) && \
                    __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__ \
                )
            #define AVEN_WYHASH_LITTLE_ENDIAN 0
        #else
            #warning "can't determine endianness, fall back to little endian"
            #define AVEN_WYHASH_LITTLE_ENDIAN 1
        #endif
    #endif

    //read functions
    #if (AVEN_WYHASH_LITTLE_ENDIAN)
        static inline uint64_t aven_wyhash_wyr8_internal(const uint8_t *p) {
            uint64_t v;
            memcpy(&v, p, 8);
            return v;
        }
        static inline uint64_t aven_wyhash_wyr4_internal(const uint8_t *p) {
            uint32_t v;
            memcpy(&v, p, 4);
            return v;
        }
    #elif defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
        static inline uint64_t aven_wyhash_wyr8_internal(const uint8_t *p) {
            uint64_t v;
            memcpy(&v, p, 8);
            return __builtin_bswap64(v);
        }
        static inline uint64_t aven_wyhash_wyr4_internal(const uint8_t *p) {
            uint32_t v;
            memcpy(&v, p, 4);
            return __builtin_bswap32(v);
        }
    #elif defined(_MSC_VER)
        static inline uint64_t aven_wyhash_wyr8_internal(const uint8_t *p) {
            uint64_t v;
            memcpy(&v, p, 8);
            return _byteswap_uint64(v);
        }
        static inline uint64_t aven_wyhash_wyr4_internal(const uint8_t *p) {
            uint32_t v;
            memcpy(&v, p, 4);
            return _byteswap_ulong(v);
        }
    #else
        static inline uint64_t aven_wyhash_wyr8_internal(const uint8_t *p) {
            uint64_t v;
            memcpy(&v, p, 8);
            return (
                ((v >> 56) & 0xff) | ((v >> 40) & 0xff00) | (
                    (v >> 24) & 0xff0000
                ) | ((v >> 8) & 0xff000000) | ((v << 8) & 0xff00000000) | (
                    (v << 24) & 0xff0000000000
                ) | ((v << 40) & 0xff000000000000) | (
                    (v << 56) & 0xff00000000000000
                )
            );
        }
        static inline uint64_t aven_wyhash_wyr4_internal(const uint8_t *p) {
            uint32_t v;
            memcpy(&v, p, 4);
            return (
                ((v >> 24) & 0xff) | ((v >> 8) & 0xff00) | ((v << 8) & 0xff0000) | (
                    (v << 24) & 0xff000000
                )
            );
        }
    #endif
    static inline uint64_t aven_wyhash_wyr3_internal(const uint8_t *p, size_t k) {
        return (((uint64_t)p[0]) << 16) | (((uint64_t)p[k >> 1]) << 8) | p[
            k - 1
        ];
    }
    //wyhash main function
    static inline uint64_t aven_wyhash(
        const void *key,
        size_t len,
        uint64_t seed,
        const uint64_t *secret
    ) {
        const uint8_t *p = (const uint8_t *)key;
        seed ^= aven_wyhash_wymix_internal(seed ^ secret[0], secret[1]);
        uint64_t a, b;
        if (aven_wyhash_likely_internal(len <= 16)) {
            if (aven_wyhash_likely_internal(len >= 4)) {
                a = (aven_wyhash_wyr4_internal(p) << 32) |
                    aven_wyhash_wyr4_internal(p + ((len >> 3) << 2));
                b = (aven_wyhash_wyr4_internal(p + len - 4) << 32) |
                    aven_wyhash_wyr4_internal(p + len - 4 - ((len >> 3) << 2));
            } else if (aven_wyhash_likely_internal(len > 0)) {
                a = aven_wyhash_wyr3_internal(p, len);
                b = 0;
            } else
                a = b = 0;
        } else {
            size_t i = len;
            if (aven_wyhash_unlikely_internal(i >= 48)) {
                uint64_t see1 = seed, see2 = seed;
                do {
                    seed = aven_wyhash_wymix_internal(
                        aven_wyhash_wyr8_internal(p) ^ secret[1],
                        aven_wyhash_wyr8_internal(p + 8) ^ seed
                    );
                    see1 = aven_wyhash_wymix_internal(
                        aven_wyhash_wyr8_internal(p + 16) ^ secret[2],
                        aven_wyhash_wyr8_internal(p + 24) ^ see1
                    );
                    see2 = aven_wyhash_wymix_internal(
                        aven_wyhash_wyr8_internal(p + 32) ^ secret[3],
                        aven_wyhash_wyr8_internal(p + 40) ^ see2
                    );
                    p += 48;
                    i -= 48;
                } while (aven_wyhash_likely_internal(i >= 48));
                seed ^= see1 ^ see2;
            }
            while (aven_wyhash_unlikely_internal(i > 16)) {
                seed = aven_wyhash_wymix_internal(
                    aven_wyhash_wyr8_internal(p) ^ secret[1],
                    aven_wyhash_wyr8_internal(p + 8) ^ seed
                );
                i -= 16;
                p += 16;
            }
            a = aven_wyhash_wyr8_internal(p + i - 16);
            b = aven_wyhash_wyr8_internal(p + i - 8);
        }
        a ^= secret[1];
        b ^= seed;
        aven_wyhash_wymum_internal(&a, &b);
        return aven_wyhash_wymix_internal(a ^ secret[0] ^ len, b ^ secret[1]);
    }

    //the default secret parameters
    static const uint64_t aven_wyhash_wyp_internal[4] = {
        0x2d358dccaa6c78a5ull,
        0x8bb84b93962eacc9ull,
        0x4b33a62ed433d4a3ull,
        0x4d5a2da51de1aa47ull,
    };

    //a useful 64bit-64bit mix function to produce deterministic pseudo random numbers that can pass BigCrush and PractRand
    static inline uint64_t aven_wyhash64(uint64_t A, uint64_t B) {
        A ^= 0x2d358dccaa6c78a5ull;
        B ^= 0x8bb84b93962eacc9ull;
        aven_wyhash_wymum_internal(&A, &B);
        return aven_wyhash_wymix_internal(
            A ^ 0x2d358dccaa6c78a5ull,
            B ^ 0x8bb84b93962eacc9ull
        );
    }

    //The aven_wyhahs_rand PRNG that pass BigCrush and PractRand
    static inline uint64_t aven_wyhahs_rand(uint64_t *seed) {
        *seed += 0x2d358dccaa6c78a5ull;
        return aven_wyhash_wymix_internal(*seed, *seed ^ 0x8bb84b93962eacc9ull);
    }

    //convert any 64 bit pseudo random numbers to uniform distribution [0,1). It can be combined with aven_wyhahs_rand, aven_wyhash64 or wyhash.
    static inline double aven_wyhash_wy2u01(uint64_t r) {
        const double _wynorm = 1.0 / (1ull << 52);
        return ((double)(r >> 12)) * _wynorm;
    }

    //convert any 64 bit pseudo random numbers to APPROXIMATE Gaussian distribution. It can be combined with aven_wyhahs_rand, aven_wyhash64 or wyhash.
    static inline double aven_wyhash_wy2gau(uint64_t r) {
        const double _wynorm = 1.0 / (1ull << 20);
        return (
                (double)(
                    (r & 0x1fffff) +
                    ((r >> 21) & 0x1fffff) +
                    ((r >> 42) & 0x1fffff)
                )
            ) *
            _wynorm -
            3.0;
    }

    #if (!AVEN_WYHASH_32BIT_MUM)
        //fast range integer random number generation on [0,k) credit to Daniel Lemire. May not work when AVEN_WYHASH_32BIT_MUM=1. It can be combined with aven_wyhahs_rand, aven_wyhash64 or wyhash.
        static inline uint64_t aven_wyhash_wy2u0k(uint64_t r, uint64_t k) {
            aven_wyhash_wymum_internal(&r, &k);
            return k;
        }
    #endif

    // modified from https://github.com/going-digital/Prime64 
    static inline unsigned long long aven_wyhash_mul_mod(
        unsigned long long a,
        unsigned long long b,
        unsigned long long m
    ) {
        unsigned long long r = 0;
        while (b) {
            if (b & 1) {
                unsigned long long r2 = r + a;
                if (r2 < r)
                    r2 -= m;
                r = r2 % m;
            }
            b >>= 1;
            if (b) {
                unsigned long long a2 = a + a;
                if (a2 < a)
                    a2 -= m;
                a = a2 % m;
            }
        }
        return r;
    }
    static inline unsigned long long aven_wyhash_pow_mod(
        unsigned long long a,
        unsigned long long b,
        unsigned long long m
    ) {
        unsigned long long r = 1;
        while (b) {
            if (b & 1)
                r = aven_wyhash_mul_mod(r, a, m);
            b >>= 1;
            if (b)
                a = aven_wyhash_mul_mod(a, a, m);
        }
        return r;
    }
    unsigned aven_wyhash_sprp(unsigned long long n, unsigned long long a) {
        unsigned long long d = n - 1;
        unsigned char s = 0;
        while (!(d & 0xff)) {
            d >>= 8;
            s += 8;
        }
        if (!(d & 0xf)) {
            d >>= 4;
            s += 4;
        }
        if (!(d & 0x3)) {
            d >>= 2;
            s += 2;
        }
        if (!(d & 0x1)) {
            d >>= 1;
            s += 1;
        }
        unsigned long long b = aven_wyhash_pow_mod(a, d, n);
        if ((b == 1) || (b == (n - 1)))
            return 1;
        unsigned char r;
        for (r = 1; r < s; r++) {
            b = aven_wyhash_mul_mod(b, b, n);
            if (b <= 1)
                return 0;
            if (b == (n - 1))
                return 1;
        }
        return 0;
    }
    unsigned aven_wyhash_is_prime(unsigned long long n) {
        if (n < 2 || !(n & 1))
            return 0;
        if (n < 4)
            return 1;
        if (!aven_wyhash_sprp(n, 2))
            return 0;
        if (n < 2047)
            return 1;
        if (!aven_wyhash_sprp(n, 3))
            return 0;
        if (!aven_wyhash_sprp(n, 5))
            return 0;
        if (!aven_wyhash_sprp(n, 7))
            return 0;
        if (!aven_wyhash_sprp(n, 11))
            return 0;
        if (!aven_wyhash_sprp(n, 13))
            return 0;
        if (!aven_wyhash_sprp(n, 17))
            return 0;
        if (!aven_wyhash_sprp(n, 19))
            return 0;
        if (!aven_wyhash_sprp(n, 23))
            return 0;
        if (!aven_wyhash_sprp(n, 29))
            return 0;
        if (!aven_wyhash_sprp(n, 31))
            return 0;
        if (!aven_wyhash_sprp(n, 37))
            return 0;
        return 1;
    }
    //make your own secret
    static inline void aven_wyhash_make_secret(uint64_t seed, uint64_t *secret) {
        uint8_t c[] = {
            15,
            23,
            27,
            29,
            30,
            39,
            43,
            45,
            46,
            51,
            53,
            54,
            57,
            58,
            60,
            71,
            75,
            77,
            78,
            83,
            85,
            86,
            89,
            90,
            92,
            99,
            101,
            102,
            105,
            106,
            108,
            113,
            114,
            116,
            120,
            135,
            139,
            141,
            142,
            147,
            149,
            150,
            153,
            154,
            156,
            163,
            165,
            166,
            169,
            170,
            172,
            177,
            178,
            180,
            184,
            195,
            197,
            198,
            201,
            202,
            204,
            209,
            210,
            212,
            216,
            225,
            226,
            228,
            232,
            240,
        };
        for (size_t i = 0; i < 4; i++) {
            uint8_t ok;
            do {
                ok = 1;
                secret[i] = 0;
                for (size_t j = 0; j < 64; j += 8)
                    secret[i] |= (
                        (uint64_t)c[aven_wyhahs_rand(&seed) % sizeof(c)]
                    ) << j;
                if (secret[i] % 2 == 0) {
                    ok = 0;
                    continue;
                }
                for (size_t j = 0; j < i; j++) {
    #if defined(__GNUC__) || defined(__INTEL_COMPILER) || defined(__clang__)
                    if (__builtin_popcountll(secret[j] ^ secret[i]) != 32) {
                        ok = 0;
                        break;
                    }
    #elif defined(_MSC_VER) && defined(_M_X64)
                    if (_mm_popcnt_u64(secret[j] ^ secret[i]) != 32) {
                        ok = 0;
                        break;
                    }
    #else
                    //manual popcount
                    uint64_t x = secret[j] ^ secret[i];
                    x -= (x >> 1) & 0x5555555555555555;
                    x = (x & 0x3333333333333333) +
                        ((x >> 2) & 0x3333333333333333);
                    x = (x + (x >> 4)) & 0x0f0f0f0f0f0f0f0f;
                    x = (x * 0x0101010101010101) >> 56;
                    if (x != 32) {
                        ok = 0;
                        break;
                    }
    #endif
                }
                if (ok && !aven_wyhash_is_prime(secret[i]))
                    ok = 0;
            } while (!ok);
        }
    }
#endif

/* The Unlicense
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org/>
*/
