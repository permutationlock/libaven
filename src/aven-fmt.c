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
        .name = aven_str_init(""),
        .optional = true,
        .type = AVEN_ARG_TYPE_STRING,
    },
    {
        .name = aven_str_init("--out"),
        .description = aven_str_init("Output file"),
        .optional = true,
        .type = AVEN_ARG_TYPE_STRING,
    },
    {
        .name = aven_str_init("--stdin"),
        .description = aven_str_init("Read from stdin"),
        .type = AVEN_ARG_TYPE_BOOL,
        .value = { .type = AVEN_ARG_TYPE_BOOL, .data = { .arg_bool = false } },
    },
    {
        .name = aven_str_init("--in-place"),
        .description = aven_str_init("Format src_file in-place"),
        .type = AVEN_ARG_TYPE_BOOL,
        .value = { .type = AVEN_ARG_TYPE_BOOL, .data = { .arg_bool = false } },
    },
    {
        .name = aven_str_init("--columns"),
        .description = aven_str_init("Column width, 0 for no limit"),
        .value = { .type = AVEN_ARG_TYPE_INT, .data = { .arg_int = 80 } },
        .type = AVEN_ARG_TYPE_INT,
    },
    {
        .name = aven_str_init("--parse-depth"),
        .description = aven_str_init(
            "Parse recursive depth limit, 0 for infinite"
        ),
        .value = { .type = AVEN_ARG_TYPE_INT, .data = { .arg_int = 10 } },
        .type = AVEN_ARG_TYPE_INT,
    },
};

// 1GB virtual memory reserve handles pathological files up to ~10MB, and
// for normal looking source files this limit should never be exceeded
#define ARENA_SIZE (4096 * 250000)
#define MAX_RENDER_SIZE ((size_t)1024 * (size_t)1024 * (size_t)100)

int main(int argc, char **argv) {
    void *mem = malloc(ARENA_SIZE);
    if (mem == NULL) {
        aven_panic("malloc failed\n");
    }
    AvenArena arena = aven_arena_init(mem, ARENA_SIZE);

    AvenStr overview = aven_str("Aven C Formatter");
    AvenStr usage = aven_str("aven-fmt [src_file]");
    AvenArgSlice args = slice_array(arg_data);
    AvenArgError parse_error = aven_arg_parse(args, argv, argc, overview, usage);
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
    int64_t arg_cwidth = aven_arg_get_int(args, "--columns");
    int64_t arg_depth = aven_arg_get_int(args, "--parse-depth");
    size_t column_width = (size_t)arg_cwidth;
    size_t parse_depth = (size_t)arg_depth;
    AvenIoReader reader = aven_io_stdin;
    Optional(AvenIoFd) in_fd = { 0 };
    Optional(AvenStr) in_file = { 0 };
    if (aven_arg_has_arg(args, "")) {
        if (aven_arg_get_bool(args, "--stdin")) {
            aven_io_perr("error: cannot specify --stdin with src_file\n");
            return 1;
        }
        in_file.valid = true;
        in_file.value = aven_arg_get_str(args, "");
    } else if (!aven_arg_get_bool(args, "--stdin")) {
        aven_io_perr("error: specify src_file to format or use --stdin\n");
        aven_arg_help(args, overview, usage);
        return 1;
    }

    Optional(AvenStr) out_file = { 0 };
    bool in_place = aven_arg_get_bool(args, "--in-place");
    if (aven_arg_has_arg(args, "--out")) {
        if (in_place) {
            aven_io_perr("error: can't specify both --out and --in-place\n");
            aven_arg_help(args, overview, usage);
            return 1;
        }
        out_file.valid = true;
        out_file.value = aven_arg_get_str(args, "--out");
    }
    if (in_place) {
        if (!in_file.valid) {
            aven_io_perr("error: specify src_file to use --in-place\n");
            aven_arg_help(args, overview, usage);
            return 1;
        }
        assert(out_file.valid == false);
        out_file.valid = true;
        out_file.value = in_file.value;
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
    List(char) input = aven_arena_create_list(char, &arena, block_size);
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
    list_push(input) = 0;
    AvenStr src = aven_arena_commit_list_to_slice(AvenStr, &arena, input);

    // Max render size of 100MB
    ByteSlice bytes = aven_arena_create_slice(
        unsigned char,
        &arena,
        MAX_RENDER_SIZE
    );
    AvenIoWriter writer = aven_io_writer_init_bytes(bytes);
    AvenCFmtResult fmt_res = aven_c_fmt(
        src,
        &writer,
        column_width,
        parse_depth,
        &arena
    );
    aven_io_writer_flush(&writer);
    if (fmt_res.error != AVEN_C_FMT_ERROR_NONE) {
        aven_io_perrf("error: {}\n", aven_fmt_str(fmt_res.msg));
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
    AvenIoWriter file_writer;
    if (out_fd.valid) {
        file_writer = aven_io_writer_init_fd(unwrap(out_fd));
    } else {
        file_writer = aven_io_stdout;
    }
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
                "error: writing '{}' failed with code {}\n",
                aven_fmt_str(unwrap(out_file)),
                aven_fmt_int(res.error)
            );
            return 1;
        }
        if (res.payload == 0) {
            aven_io_perrf(
                "error: writing '{}' ran out of space\n",
                aven_fmt_str(unwrap(out_file))
            );
            return 1;
        }
        rem = (ByteSlice)slice_tail(rem, res.payload);
    }

    return 0;
}
