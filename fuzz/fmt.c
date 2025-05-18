#define AVEN_IMPLEMENTATION
#include <aven.h>
#include <aven/arena.h>
#include <aven/io.h>
#include <aven/c.h>

#include <stdlib.h>

#define ARENA_SIZE ((size_t)4096 * (size_t)750000)

static Optional(AvenArena) arena = { 0 };

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    if (!arena.valid) {
        void *mem = malloc(ARENA_SIZE);
        if (mem == NULL) {
            aven_panic("malloc failed\n");
        }
        arena.valid = true;
        arena.value = aven_arena_init(mem, ARENA_SIZE);
    }
    AvenArena temp_arena = arena.value;
    AvenStr src = aven_arena_create_slice(char, &temp_arena, size + 1);
    memcpy(src.ptr, data, size);
    get(src, size) = 0;
    AvenIoWriter writer = aven_io_writer_init_sink();
    AvenCFmtResult fmt_res = aven_c_fmt(src, &writer, 128, 4, 12, &temp_arena);
    (void)fmt_res;
    // if (fmt_res.error == AVEN_C_FMT_ERROR_PARSE) {
    //     return -1;
    // }
    return 0;
}
