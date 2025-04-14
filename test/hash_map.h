#ifndef TEST_HASH_MAP_H
#define TEST_HASH_MAP_H

#include <aven.h>
#include <aven/arena.h>
#include <aven/fmt.h>
#include <aven/hash.h>
#include <aven/str.h>
#include <aven/test.h>

typedef struct {
    ByteSlice key;
    ByteSlice value;
} TestHashMapFlatKVPair;

typedef struct {
    Slice(TestHashMapFlatKVPair) kv_pairs;
    uint64_t seed;
    uint32_t exp;
} TestAvenHashMapFlatArgs;

AvenTestResult test_aven_hash_map_flat(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *opaque_args
) {
    TestAvenHashMapFlatArgs *args = opaque_args;

    AvenHashMapFlat(ByteSlice) map = aven_hash_map_flat_init(
        ByteSlice,
        args->seed,
        args->exp,
        &arena
    );

    for (size_t i = 0; i < args->kv_pairs.len; i += 1) {
        TestHashMapFlatKVPair kv_pair = get(args->kv_pairs, i);
        aven_hash_map_flat_get_or_put(map, kv_pair.key) = kv_pair.value;
    }

    size_t valid_entries = 0;
    for (size_t i = 0; i < args->kv_pairs.len; i += 1) {
        TestHashMapFlatKVPair kv_pair = get(args->kv_pairs, i);
        ByteSlice actual = aven_hash_map_flat_get(map, kv_pair.key);
        if (bytes_equal(actual, kv_pair.value)) {
            valid_entries += 1;
        }
    }

    if (valid_entries != args->kv_pairs.len) {
        return (AvenTestResult){
            .error = 1,
            .message = aven_fmt(
                emsg_arena,
                "expected {} valid entries, found {}",
                aven_fmt_uint(args->kv_pairs.len),
                aven_fmt_uint(valid_entries)
            ),
        };
    }

    return (AvenTestResult){ 0 };
}

static int test_hash_map(AvenArena arena) {
    AvenTestCase tcase_data[] = {
        {
            .desc = aven_str("aven_hash_map_flat_init"),
            .fn = test_aven_hash_map_flat,
            .args = &(TestAvenHashMapFlatArgs){
                .exp = 2,
                .seed = 0xdead,
                .kv_pairs = { 0 },
            },
        },
        {
            .desc = aven_str("aven_hash_map_flat_get insert and retrieve 1 item"),
            .fn = test_aven_hash_map_flat,
            .args = &(TestAvenHashMapFlatArgs){
                .exp = 2,
                .seed = 0xdead,
                .kv_pairs = slice_array(
                    (TestHashMapFlatKVPair[]){
                        {
                            .key = slice_as_bytes(aven_str("hello")),
                            .value = slice_as_bytes(aven_str("world")),
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_hash_map_flat_get insert and retrieve 3 items"),
            .fn = test_aven_hash_map_flat,
            .args = &(TestAvenHashMapFlatArgs){
                .exp = 4,
                .seed = 0xbeef,
                .kv_pairs = slice_array(
                    (TestHashMapFlatKVPair[]){
                        {
                            .key = slice_as_bytes(aven_str("a")),
                            .value = slice_as_bytes(aven_str("1")),
                        },
                        {
                            .key = slice_as_bytes(aven_str("b")),
                            .value = slice_as_bytes(aven_str("2")),
                        },
                        {
                            .key = slice_as_bytes(aven_str("c")),
                            .value = slice_as_bytes(aven_str("3")),
                        },
                    }
                ),
            },
        },
    };
    AvenTestCaseSlice tcases = slice_array(tcase_data);

    aven_test(tcases, arena);

    return 0;
}

#endif // TEST_HASH_MAP_H
