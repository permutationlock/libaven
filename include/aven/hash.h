#ifndef AVEN_HASH_H
    #define AVEN_HASH_H

    #include "../aven.h"
    #include "arena.h"
    #include "wyhash.h"

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

    static inline uint64_t aven_hash(AvenHashCtx *ctx, ByteSlice bytes) {
        return aven_hash_seeded(ctx, 0, bytes);
    }

    #define AvenHashMapFlat(t) struct { \
            AvenHashSetFlat set; \
            Slice(t) values; \
        }

    #define aven_hash_map_flat_init(t, s, e, a) { \
            .set = aven_hash_set_flat_init(s, e, a), \
            .values = aven_arena_create_slice(t, a, ((size_t)1) << e), \
        }
    #define aven_hash_map_flat_get_or_put(m, k) get( \
            (m).values, \
            aven_hash_set_flat_insert(&(m).set, k) - 1 \
        )
    #define aven_hash_map_flat_get(m, k) get( \
            (m).values, \
            aven_hash_set_flat_contains(&(m).set, k) - 1 \
        )
    #define aven_hash_map_flat_delete(m, k) aven_hash_set_remove(&(m).set, k)

    typedef struct {
        Slice(ByteSlice) keys;
        uint32_t exp;
        uint32_t mask;
        AvenHashCtx hash_ctx;
    } AvenHashSetFlat;

    static inline AvenHashSetFlat aven_hash_set_flat_init(
        uint64_t seed,
        uint32_t exp,
        AvenArena *arena
    ) {
        assert(exp < 32);
        AvenHashSetFlat set = {
            .keys = aven_arena_create_slice(
                ByteSlice,
                arena,
                ((size_t)1) << exp
            ),
            .exp = exp,
            .mask = (((uint32_t)1) << exp) - 1,
            .hash_ctx = aven_hash_init(seed),
        };
        for (size_t i = 0; i < set.keys.len; i += 1) {
            get(set.keys, i) = (ByteSlice){ 0 };
        }
        return set;
    }

    static inline uint32_t aven_hash_set_flat_insert(
        AvenHashSetFlat *set,
        ByteSlice key
    ) {
        uint64_t hash = aven_hash(&set->hash_ctx, key);
        uint32_t step = (uint32_t)(hash >> (64 - set->exp)) | 1;
        uint32_t i = (uint32_t)hash;
        for (size_t count = 0; count < set->keys.len; count += 1) {
            i = (i + step) & set->mask;
            if (get(set->keys, i).len == 0) {
                get(set->keys, i) = key;
                return i + 1;
            } else if (bytes_equal(key, get(set->keys, i))) {
                return i + 1;
            }
        }
        return 0;
    }

    static inline uint32_t aven_hash_set_flat_contains(
        AvenHashSetFlat *set,
        ByteSlice key
    ) {
        uint64_t hash = aven_hash(&set->hash_ctx, key);
        uint32_t step = (uint32_t)(hash >> (64 - set->exp)) | 1;
        uint32_t i = (uint32_t)hash;
        for (size_t count = 0; count < set->keys.len; count += 1) {
            i = (i + step) & set->mask;
            if (get(set->keys, i).len == 0) {
                return 0;
            } else if (bytes_equal(key, get(set->keys, i))) {
                return i + 1;
            }
        }
        return 0;
    }

    static inline void aven_hash_set_flat_remove(
        AvenHashSetFlat *set,
        ByteSlice key
    ) {
        uint64_t hash = aven_hash(&set->hash_ctx, key);
        uint32_t step = (uint32_t)(hash >> (64 - set->exp)) | 1;
        uint32_t i = (uint32_t)hash;
        for (size_t count = 0; count < set->keys.len; count += 1) {
            i = (i + step) & set->mask;
            if (get(set->keys, i).len == 0) {
                return;
            } else if (bytes_equal(key, get(set->keys, i))) {
                get(set->keys, i) = (ByteSlice){ 0 };
                return;
            }
        }
        return;
    }
#endif
// AVEN_HASH_H
