#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
    #define _POSIX_C_SOURCE 200112L
#endif
#define AVEN_IMPLEMENTATION
#include <aven.h>
#include <aven/arg.h>
#include <aven/c.h>
#include <aven/fmt.h>
#include <aven/io.h>
#include <aven/str.h>

#include <stdlib.h>

static AvenArg arg_data[] = {
    {
        .name =  aven_str_init("-i"),
        .description = aven_str_init("Input source file"),
        .optional = true,
        .type = AVEN_ARG_TYPE_STRING,
    },
};

// Enough to handle pathological files up to ~10MB, and much larger real files
#define ARENA_SIZE (4096 * 250000)

int main(int argc, char **argv) {
    void *mem = malloc(ARENA_SIZE);
    if (mem == NULL) {
        aven_panic("malloc failed\n");
    }
    AvenArena arena = aven_arena_init(mem, ARENA_SIZE);

    AvenArgSlice args = slice_array(arg_data);
    AvenArgError parse_error =  aven_arg_parse(
        args,
        argv,
        argc,
        aven_str("OVERVIEW: Aven C Formatter"),
        aven_str("USAGE: fmt [-i src_file] [-o out_file]")
    );
    switch (parse_error) {
        case AVEN_ARG_ERROR_NONE: {
            break;
        }
        case AVEN_ARG_ERROR_HELP: {
            return 0;
        }
        default: {
            return 1;
        }
    }
    AvenIoReader reader = aven_io_stdin;
    if (aven_arg_has_arg(args, "-i")) {
        AvenStr in_file = aven_arg_get_str(args, "-i");
        AvenIoOpenResult in_res = aven_io_open(
            in_file,
            AVEN_IO_OPEN_MODE_READ,
            arena
        );
        if (in_res.error != AVEN_IO_OPEN_ERROR_NONE) {
            aven_io_perrf(
                "error reading \"{}\": {}",
                aven_fmt_str(in_file),
                aven_fmt_int((int)in_res.error)
            );
        }
        reader = aven_io_reader_init_fd_buffered(in_res.payload, 8192, &arena);
    }
    
    AvenIoWriter stdout = aven_io_writer_init_stdout_buffered(8192, &arena);
    AvenCFmtResult fmt_res = aven_c_fmt(
        &reader,
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
