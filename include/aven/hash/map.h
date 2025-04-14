#ifndef AVEN_HASH_SET_H
#define AVEN_HASH_SET_H

#include "../../aven.h"
#include "../arena.h"
#include "../hash.h"

#define AvenHashMap(t) struct { \
        AvenHashSet set; \
        Slice(t) values; \
    }

#define aven_hash_map_init(t, s, e, a) { \
        .set = aven_hash_set_init(s, e, a), \
        .values = aven_arena_create_slice(t, a, ((size_t)1) << e), \
    }
#define aven_hash_map_set(m, k) get( \
        (m).values, \
        aven_hash_set_insert(&(m).set, k) - 1 \
    )
#define aven_hash_map_get(m, k) get( \
        (m).values, \
        aven_hash_set_contains(&(m).set, k) - 1 \
    )

typedef struct {
    Slice(ByteSlice) keys;
    uint32_t exp;
    uint32_t mask;
    AvenHashCtx hash_ctx;
} AvenHashSet;

static unsigned char aven_hash_set_tombstone[] = {
    0xa7, 0xe2, 0x4a, 0x54, 0x5e, 0x77, 0x08, 0x85, 0x70, 0x4e
};

static inline AvenHashSet aven_hash_set_init(
    uint64_t seed,
    uint32_t exp,
    AvenArena *arena
) {
    assert(exp < 32);
    AvenHashSet set = {
        .keys = aven_arena_create_slice(ByteSlice, arena, ((size_t)1) << exp),
        .exp = exp,
        .mask = (((uint32_t)1) << exp) - 1,
        .hash_ctx = aven_hash_init(seed),
    };
    for (size_t i = 0; i < set.keys.len; i += 1) {
        get(set.keys, i) = (ByteSlice){ 0 };
    }
    return set;
}

static inline uint32_t aven_hash_set_insert(AvenHashSet *set, ByteSlice key) {
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

static inline uint32_t aven_hash_set_contains(AvenHashSet *set, ByteSlice key) {
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

#endif // AVEN_HASH_SET_H
