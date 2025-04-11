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

#define aven_hash_map_get(m, k) get( \
        (m).values, \
        aven_hash_set_lookup(&(m).set, k) \
    )

typedef struct {
    Slice(ByteSlice) keys;
    uint32_t exp;
    uint32_t mask;
    AvenHashCtx hash_ctx;
} AvenHashSet;

static inline AvenHashSet aven_hash_set_init(
    uint64_t seed,
    uint32_t exp,
    AvenArena *arena
) {
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

static inline uint32_t aven_hash_set_lookup(AvenHashSet *set, ByteSlice key) {
    uint64_t hash = aven_hash(&set->hash_ctx, key);
    uint32_t step = (uint32_t)(hash >> (((uint32_t)64) - set->exp)) | 1;
    uint32_t i = (uint32_t)hash;
    for (uint32_t count = 0; count < set->keys.len; count += 1) {
        i = (i + step) & set->mask;
        if (get(set->keys, i).len == 0) {
            get(set->keys, i) = key;
            return i;
        } else if (bytes_equal(key, get(set->keys, i))) {
            return i;
        }
    }
    aven_panic("hash map full");
    return 0xffffffff;
}

#endif // AVEN_HASH_SET_H
