#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
    #define _POSIX_C_SOURCE 200112L
#endif
#define AVEN_IMPLEMENTATION
#include <aven.h>
#include <aven/c.h>
#include <aven/fmt.h>
#include <aven/io.h>

#include <stdlib.h>

// Enough to handle pathological files up to ~10MB, and much larger real files
#define ARENA_SIZE (4096 * 250000)

int main(void) {
    void *mem = malloc(ARENA_SIZE);
    if (mem == NULL) {
        aven_panic("malloc failed\n");
    }
    AvenArena arena = aven_arena_init(mem, ARENA_SIZE);
    AvenIoWriter stdout = aven_io_writer_init_stdout_buffered(8192, &arena);
    AvenCFmtResult fmt_res = aven_c_fmt(
        &aven_io_stdin,
        &stdout,
        &arena
    );
    aven_io_writer_flush(&stdout);
    if (fmt_res.error != AVEN_C_FMT_ERROR_NONE) {
        aven_io_perrf(
            "error: {}\n",
            aven_fmt_str(fmt_res.msg)
        );
        return 1;
    }
    return 0;
}
