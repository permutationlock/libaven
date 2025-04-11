#ifndef AVEN_HASH_H
#define AVEN_HASH_H

#include "aven.h"

#include "wyhash.h"

// wyhash source ends

typedef struct {
    uint64_t secret[4];
} AvenHashCtx;

static inline AvenHashCtx aven_hash_init(uint64_t seed) {
    AvenHashCtx ctx = { 0 };
    aven_wyhash_make_secret(seed, ctx.secret);
    return ctx;
}

static inline uint64_t aven_hash_seeded(
    AvenHashCtx *ctx,
    uint64_t seed,
    ByteSlice bytes
) {
    return aven_wyhash(bytes.ptr, bytes.len, seed, ctx->secret);
}

static inline uint64_t aven_hash(
    AvenHashCtx *ctx,
    ByteSlice bytes
) {
    return aven_hash_seeded(ctx, 0, bytes);
}

#endif // AVEN_HASH_H
