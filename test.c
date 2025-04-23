#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
    #define _POSIX_C_SOURCE 200112L
#endif

#define AVEN_IMPLEMENTATION

#include <aven.h>
#include <aven/arena.h>
#include <aven/fs.h>
#include <aven/path.h>
#include <aven/str.h>
#include <aven/test.h>

#include <stdlib.h>

#include "test/path.h"
#include "test/io.h"
#include "test/hash_map.h"

#define ARENA_SIZE (4096 * 32)

int main(void) {
    aven_fs_utf8_mode();
    void *mem = malloc(ARENA_SIZE);
    AvenArena test_arena = aven_arena_init(mem, ARENA_SIZE);

    test_path(test_arena);
    test_io(test_arena);
    test_hash_map(test_arena);

    return 0;
}
