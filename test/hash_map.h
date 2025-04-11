#ifndef TEST_HASH_MAP_H
#define TEST_HASH_MAP_H

#include <aven.h>
#include <aven/arena.h>
#include <aven/fmt.h>
#include <aven/hash/map.h>
#include <aven/str.h>
#include <aven/test.h>

typedef struct {
    ByteSlice key;
    ByteSlice value;
} TestHashMapKVPair;

typedef struct {
    Slice(TestHashMapKVPair) kv_pairs;
    uint64_t seed;
    uint32_t exp;
} TestAvenHashMapArgs;

AvenTestResult test_aven_hash_map(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *opaque_args
) {
    TestAvenHashMapArgs *args = opaque_args;

    AvenHashMap(ByteSlice) map = aven_hash_map_init(
        ByteSlice,
        args->seed,
        args->exp,
        &arena
    );

    for (size_t i = 0; i < args->kv_pairs.len; i += 1) {
        TestHashMapKVPair kv_pair = get(args->kv_pairs, i);
        aven_hash_map_get(map, kv_pair.key) = kv_pair.value;
    }

    size_t valid_entries = 0;
    for (size_t i = 0; i < args->kv_pairs.len; i += 1) {
        TestHashMapKVPair kv_pair = get(args->kv_pairs, i);
        ByteSlice actual = aven_hash_map_get(map, kv_pair.key);
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
            .desc = aven_str("aven_hash_map_init"),
            .fn = test_aven_hash_map,
            .args = &(TestAvenHashMapArgs){
                .exp = 2,
                .seed = 0xdead,
                .kv_pairs = { 0 },
            },
        },
        {
            .desc = aven_str("aven_hash_map_get insert and retrieve 1 item"),
            .fn = test_aven_hash_map,
            .args = &(TestAvenHashMapArgs){
                .exp = 2,
                .seed = 0xdead,
                .kv_pairs = slice_array(
                    (TestHashMapKVPair[]){
                        {
                            .key = slice_as_bytes(aven_str("hello")),
                            .value = slice_as_bytes(aven_str("world")),
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_hash_map_get insert and retrieve 3 items"),
            .fn = test_aven_hash_map,
            .args = &(TestAvenHashMapArgs){
                .exp = 4,
                .seed = 0xbeef,
                .kv_pairs = slice_array(
                    (TestHashMapKVPair[]){
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
