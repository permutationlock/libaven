#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
    #define _POSIX_C_SOURCE 200112L
#endif
#define AVEN_IMPLEMENTATION
#include <aven.h>
#include <aven/arena.h>
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
    {
        .name =  aven_str_init("-o"),
        .description = aven_str_init("Output source file"),
        .optional = true,
        .type = AVEN_ARG_TYPE_STRING,
    },
    {
        .name =  aven_str_init("-io"),
        .description = aven_str_init("Format source file in-place"),
        .optional = true,
        .type = AVEN_ARG_TYPE_STRING,
    },
};

// 1GB virtual memory reserve handles pathological files up to ~10MB, and
// for normal looking source files this limit should never be exceeded
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
        aven_str("USAGE: fmt [-i input] [-o output] [-io in_place]")
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
    Optional(AvenIoFd) in_fd = { 0 };
    Optional(AvenStr) in_file = { 0 };
    if (aven_arg_has_arg(args, "-i")) {
        if (aven_arg_has_arg(args, "-io")) {
            aven_io_perr(
                "error: cannot specify both -io and -i arguments\n"
            );
        }
        in_file.valid = true;
        in_file.value = aven_arg_get_str(args, "-i");
    }
    if (!in_file.valid and aven_arg_has_arg(args, "-io")) {
        in_file.valid = true;
        in_file.value = aven_arg_get_str(args, "-io");
    }
    if (in_file.valid) {
        AvenIoOpenResult in_res = aven_io_open(
            in_file.value,
            AVEN_IO_OPEN_MODE_READ,
            arena
        );
        if (in_res.error != AVEN_IO_OPEN_ERROR_NONE) {
            aven_io_perrf(
                "error: opening \"{}\" failed with code {}\n",
                aven_fmt_str(in_file.value),
                aven_fmt_int((int)in_res.error)
            );
        }
        in_fd.valid = true;
        in_fd.value = in_res.payload;
        reader = aven_io_reader_init_fd(in_fd.value);
    }
    size_t block_size = 8192;
    List(char) input = aven_arena_create_list(
        char,
        &arena,
        block_size
    );
    for (;;) {
        AvenStr rem = slice_list_free(input);
        if (rem.len == 0) {
            aven_arena_resize_list(&arena, input, input.len + block_size);
            continue;
        }
        AvenIoResult res = aven_io_reader_pop(&reader, slice_as_bytes(rem));
        if (res.error != 0) {
            aven_io_perrf(
                "error: reader failed with code {}\n",
                aven_fmt_int(res.error)
            );
            return 1;
        }
        if (res.payload == 0) {
            break;
        }
        input.len += res.payload;
    }
    if (in_fd.valid) {
        aven_io_close(in_fd.value);
    }
    AvenStr src = aven_arena_commit_list_to_slice(AvenStr, &arena, input);

    AvenIoWriter writer;
    Optional(AvenStr) out_file = { 0 };
    if (aven_arg_has_arg(args, "-o")) {
        if (aven_arg_has_arg(args, "-io")) {
            aven_io_perr(
                "error: cannot specify both -io and -i arguments\n"
            );
        }
        out_file.valid = true;
        out_file.value = aven_arg_get_str(args, "-o");
    }
    if (!out_file.valid and aven_arg_has_arg(args, "-io")) {
        out_file.valid = true;
        out_file.value = aven_arg_get_str(args, "-io");
    }
    if (out_file.valid) {
        // Max file size of 100MB for in-place formatting
        ByteSlice bytes = aven_arena_create_slice(
            unsigned char,
            &arena,
            (size_t)1024 * (size_t)1024 * (size_t)100
        );
        writer = aven_io_writer_init_bytes(bytes);
    } else {
        writer = aven_io_writer_init_stdout_buffered(block_size, &arena);
    }
    AvenCFmtResult fmt_res = aven_c_fmt(
        src,
        &writer,
        &arena
    );
    aven_io_writer_flush(&writer);
    if (fmt_res.error != AVEN_C_FMT_ERROR_NONE) {
        aven_io_perrf(
            "error: {}\n",
            aven_fmt_str(fmt_res.msg)
        );
        return 1;
    }

    Optional(AvenIoFd) out_fd = { 0 };
    if (out_file.valid) {
        AvenIoOpenResult out_res = aven_io_open(
            out_file.value,
            AVEN_IO_OPEN_MODE_WRITE,
            arena
        );
        if (out_res.error != AVEN_IO_OPEN_ERROR_NONE) {
            aven_io_perrf(
                "error: opening \"{}\" failed with code {}\n",
                aven_fmt_str(out_file.value),
                aven_fmt_int((int)out_res.error)
            );
        }
        out_fd.valid = true;
        out_fd.value = out_res.payload;
    }
    if (out_fd.valid) {
        AvenIoWriter file_writer = aven_io_writer_init_fd(unwrap(out_fd));
        ByteSlice rem = slice_head(writer.buffer, writer.index);
        for (;;) {
            if (rem.len == 0) {
                break;
            }
            AvenIoResult res = aven_io_writer_push(
                &file_writer,
                slice_as_bytes(rem)
            );
            if (res.error != 0) {
                aven_io_perrf(
                    "error: writing to '{}' failed with code {}\n",
                    aven_fmt_str(unwrap(out_file)),
                    aven_fmt_int(res.error)
                );
                return 1;
            }
            if (res.payload == 0) {
                aven_io_perrf(
                    "error: writing to '{}' with no space remaining\n",
                    aven_fmt_str(unwrap(out_file))
                );
                return 1;
            }
            rem = (ByteSlice)slice_tail(rem, res.payload);
        }
    }

    return 0;
}
