#include <aven.h>
#include <aven/arena.h>
#include <aven/io.h>
#include <aven/path.h>
#include <aven/str.h>
#include <aven/test.h>

typedef struct {
    AvenStr fpath;
    ByteSlice expected;
} TestAvenIoReadArgs;

AvenTestResult test_aven_io_read(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenIoReadArgs *io_args = args;

    AvenIoOpenResult fd_res = aven_io_open(
        io_args->fpath,
        AVEN_IO_OPEN_MODE_READ,
        arena
    );
    if (fd_res.error != 0) {
        char fmt[] = "error opening file \"%s\"";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) +
                io_args->fpath.len,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            aven_str_to_cstr(io_args->fpath, &arena)
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 1,
            .message = buffer,
        };
    }

    ByteSlice actual = aven_arena_create_slice(
        unsigned char,
        &arena,
        io_args->expected.len
    );
    AvenIoReadResult rd_res = aven_io_read(fd_res.payload, actual);
    aven_io_close(fd_res.payload);

    if (rd_res.error != 0) {
        char fmt[] = "error reading file \"%s\"";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) +
                io_args->fpath.len,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            aven_str_to_cstr(io_args->fpath, &arena)
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 2,
            .message = buffer,
        };
    }

    if (rd_res.payload != io_args->expected.len) {
        char fmt[] = "expected \"%d\" bytes, found \"%d\" bytes";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) + 16,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            (int)io_args->expected.len,
            (int)rd_res.payload
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 3,
            .message = buffer,
        };
    }

    size_t diff = 0;
    for (size_t i = 0; i < actual.len; i += 1) {
        if (get(actual, i) != get(io_args->expected, i)) {
            diff += 1;
        }
    }

    if (diff > 0) {
        char fmt[] = "file contents differed by \"%d\" bytes";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) + 16,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            (int)diff
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 4,
            .message = buffer,
        };
    }

    return (AvenTestResult){ 0 };
}

typedef struct {
    AvenStr exe_dir_path;
    ByteSlice contents;
} TestAvenIoWriteArgs;

AvenTestResult test_aven_io_write_read(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenIoWriteArgs *io_args = args;

    AvenStr tmp_dir_path = aven_path(
        &arena,
        io_args->exe_dir_path,
        aven_str("test_aven_io_write_read")
    );
    int error = aven_fs_mkdir(tmp_dir_path, arena);
    if (error != 0) {
        char fmt[] = "error creating directory \"%s\"";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) + tmp_dir_path.len,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            aven_str_to_cstr(tmp_dir_path, &arena)
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 5,
            .message = buffer,
        };
    }

    AvenStr fname = aven_str("tmp.bin");
    AvenStr tmp_path = aven_path(&arena, tmp_dir_path, fname);
    AvenIoOpenResult fd_res = aven_io_open(
        tmp_path,
        AVEN_IO_OPEN_MODE_WRITE,
        arena
    );
    if (fd_res.error != 0) {
        aven_fs_rm(tmp_path, arena);
        aven_fs_rmdir(tmp_dir_path, arena);

        char fmt[] = "error opening file \"%s\"";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) + tmp_path.len,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            aven_str_to_cstr(tmp_path, &arena)
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 6,
            .message = buffer,
        };
    }

    AvenIoWriteResult wr_res = aven_io_write(
         fd_res.payload,
         io_args->contents
    );
    aven_io_close(fd_res.payload);

    if (wr_res.error != 0) {
        aven_fs_rm(tmp_path, arena);
        aven_fs_rmdir(tmp_dir_path, arena);

        char fmt[] = "error writing to file \"%s\"";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) + tmp_path.len,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            aven_str_to_cstr(tmp_path, &arena)
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 7,
            .message = buffer,
        };
    }

    TestAvenIoReadArgs read_args = {
        .expected = io_args->contents,
        .fpath = tmp_path,
    };

    AvenTestResult result = test_aven_io_read(emsg_arena, arena, &read_args);
    aven_fs_rm(tmp_path, arena);
    aven_fs_rmdir(tmp_dir_path, arena);

    return result;
}

AvenTestResult test_aven_io_reader(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenIoReadArgs *io_args = args;

    AvenIoOpenResult fd_res = aven_io_open(
        io_args->fpath,
        AVEN_IO_OPEN_MODE_READ,
        arena
    );
    if (fd_res.error != 0) {
        char fmt[] = "error opening file \"%s\"";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) +
                io_args->fpath.len,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            aven_str_to_cstr(io_args->fpath, &arena)
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 1,
            .message = buffer,
        };
    }

    AvenIoReader reader = aven_io_reader_init_fd_buffered(
        fd_res.payload,
        8,
        &arena
    );
    ByteSlice actual = aven_arena_create_slice(
        unsigned char,
        &arena,
        io_args->expected.len
    );
    AvenIoResult rd_res = aven_io_reader_pop(&reader, actual);
    aven_io_close(fd_res.payload);

    if (rd_res.error != 0) {
        char fmt[] = "error reading file \"%s\"";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) +
                io_args->fpath.len,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            aven_str_to_cstr(io_args->fpath, &arena)
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 2,
            .message = buffer,
        };
    }

    if (rd_res.payload != io_args->expected.len) {
        char fmt[] = "expected \"%d\" bytes, found \"%d\" bytes";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) + 16,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            (int)io_args->expected.len,
            (int)rd_res.payload
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 3,
            .message = buffer,
        };
    }

    size_t diff = 0;
    for (size_t i = 0; i < actual.len; i += 1) {
        if (get(actual, i) != get(io_args->expected, i)) {
            diff += 1;
        }
    }

    if (diff > 0) {
        char fmt[] = "file contents differed by \"%d\" bytes";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) + 16,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            (int)diff
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 4,
            .message = buffer,
        };
    }

    return (AvenTestResult){ 0 };
}

AvenTestResult test_aven_io_writer(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenIoWriteArgs *io_args = args;

    AvenStr tmp_dir_path = aven_path(
        &arena,
        io_args->exe_dir_path,
        aven_str("test_aven_io_write_read")
    );
    int error = aven_fs_mkdir(tmp_dir_path, arena);
    if (error != 0) {
        char fmt[] = "error creating directory \"%s\"";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) + tmp_dir_path.len,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            aven_str_to_cstr(tmp_dir_path, &arena)
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 5,
            .message = buffer,
        };
    }

    AvenStr fname = aven_str("tmp.bin");
    AvenStr tmp_path = aven_path(&arena, tmp_dir_path, fname);
    AvenIoOpenResult fd_res = aven_io_open(
        tmp_path,
        AVEN_IO_OPEN_MODE_WRITE,
        arena
    );
    if (fd_res.error != 0) {
        aven_fs_rm(tmp_path, arena);
        aven_fs_rmdir(tmp_dir_path, arena);

        char fmt[] = "error opening file \"%s\"";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) + tmp_path.len,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            aven_str_to_cstr(tmp_path, &arena)
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 6,
            .message = buffer,
        };
    }

    AvenIoWriter writer = aven_io_writer_init_fd_buffered(
        fd_res.payload,
        8,
        &arena
    );
    AvenIoResult wr_res = aven_io_writer_push(
         &writer,
         io_args->contents
    );
    AvenIoWriteError fl_error = aven_io_writer_flush(&writer);
    aven_io_close(fd_res.payload);

    if (wr_res.error != 0 or fl_error != 0) {
        aven_fs_rm(tmp_path, arena);
        aven_fs_rmdir(tmp_dir_path, arena);

        char fmt[] = "error writing to file \"%s\"";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) + tmp_path.len,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            aven_str_to_cstr(tmp_path, &arena)
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 7,
            .message = buffer,
        };
    }

    TestAvenIoReadArgs read_args = {
        .expected = io_args->contents,
        .fpath = tmp_path,
    };

    AvenTestResult result = test_aven_io_read(emsg_arena, arena, &read_args);
    aven_fs_rm(tmp_path, arena);
    aven_fs_rmdir(tmp_dir_path, arena);

    return result;
}

typedef struct {
    uint32_t a;
    uint8_t b;
    uint64_t c;
} TestAvenIoStruct;
typedef Slice(TestAvenIoStruct) TestAvenIoStructSlice;

typedef struct {
    TestAvenIoStructSlice slice;
} TestAvenIoWriterSliceArgs;

AvenTestResult test_aven_io_writer_slice(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenIoWriterSliceArgs *io_args = args;

    ByteSlice space = aven_arena_create_slice(
        unsigned char,
        &arena,
        aven_io_slice_size(io_args->slice)
    );

    AvenIoWriter writer = aven_io_writer_init_bytes(space);
    int error = aven_io_writer_push_slice(&writer, io_args->slice);

    if (error != 0) {
        return (AvenTestResult){
            .error = error,
            .message = "error writing slice",
        };
    }

    AvenIoReader reader = aven_io_reader_init_bytes(space);
    AvenIoSliceResult rd_res = aven_io_reader_pop_slice(
        TestAvenIoStruct,
        &reader,
        &arena
    );

    if (rd_res.error != 0) {
        return (AvenTestResult){
            .error = rd_res.error,
            .message = "error reading slice",
        };
    }

    TestAvenIoStructSlice read_slice = aven_io_slice(
        TestAvenIoStruct,
        rd_res.payload
    );

    if (read_slice.len != io_args->slice.len) {
        char fmt[] = "expected slice len %ul, found %ul";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) + 8,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            (unsigned long)io_args->slice.len,
            (unsigned long)read_slice.len
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 1,
            .message = buffer,
        };
    }
    
    size_t entries_equal = 0;
    for (size_t i = 0; i < read_slice.len; i += 1) {
        TestAvenIoStruct actual = get(read_slice, i);
        TestAvenIoStruct expected = get(io_args->slice, i);
        if (
            actual.a == expected.a and
            actual.b == expected.b and
            actual.c == expected.c
        ) {
            entries_equal += 1;
        }
    }

    if (entries_equal != io_args->slice.len) {
        char fmt[] =
            "read slice and written slice differed in %ul / %ul entries";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) + 8,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            (unsigned long)(read_slice.len - entries_equal),
            (unsigned long)read_slice.len
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 1,
            .message = buffer,
        };
    }

    return (AvenTestResult){ 0 };
}

int test_io(AvenArena arena) {
    AvenPathResult exe_res = aven_path_exe(&arena);
    if (exe_res.error != 0) {
        printf(
            "unable to run tests for %s: could not find path to test exe",
            __FILE__
        );
        return 1;
    }

    AvenStr exe_path = exe_res.payload;
    AvenStr exe_dir_path = aven_path_containing_dir(exe_path);
    AvenStr test_root_path = aven_path(
        &arena,
        aven_path_containing_dir(exe_dir_path),
        aven_str("test")
    );

    AvenTestCase tcase_data[] = {
        {
            .desc = "aven_io_read empty file",
            .fn = test_aven_io_read,
            .args = &(TestAvenIoReadArgs){
                .fpath = aven_path(
                    &arena,
                    test_root_path,
                    aven_str("io"),
                    aven_str("empty.bin")
                ),
                .expected = { 0 },
            },
        },
        {
            .desc = "aven_io_read text file",
            .fn = test_aven_io_read,
            .args = &(TestAvenIoReadArgs){
                .fpath = aven_path(
                    &arena,
                    test_root_path,
                    aven_str("io"),
                    aven_str("hello.txt")
                ),
                .expected = {
                    .ptr = (unsigned char *)"Hello, World!",
                    .len = countof("Hello, World!") - 1,
                },
            },
        },
        {
            .desc = "aven_io_write empty file",
            .fn = test_aven_io_write_read,
            .args = &(TestAvenIoWriteArgs){
                .exe_dir_path = exe_dir_path,
                .contents = { 0 },
            },
        },
        {
            .desc = "aven_io_write text file",
            .fn = test_aven_io_write_read,
            .args = &(TestAvenIoWriteArgs){
                .exe_dir_path = exe_dir_path,
                .contents = {
                    .ptr = (unsigned char *)"Hello, World!",
                    .len = countof("Hello, World!") - 1,
                },
            },
        },
        {
            .desc = "aven_io_write bin file",
            .fn = test_aven_io_write_read,
            .args = &(TestAvenIoWriteArgs){
                .exe_dir_path = exe_dir_path,
                .contents = slice_array(
                    (unsigned char[]){ 0xde, 0xad, 0xbe, 0xef }
                ),
            },
        },
        {
            .desc = "aven_io_reader_pop empty file",
            .fn = test_aven_io_reader,
            .args = &(TestAvenIoReadArgs){
                .fpath = aven_path(
                    &arena,
                    test_root_path,
                    aven_str("io"),
                    aven_str("empty.bin")
                ),
                .expected = { 0 },
            },
        },
        {
            .desc = "aven_io_reader_pop text file",
            .fn = test_aven_io_reader,
            .args = &(TestAvenIoReadArgs){
                .fpath = aven_path(
                    &arena,
                    test_root_path,
                    aven_str("io"),
                    aven_str("hello.txt")
                ),
                .expected = {
                    .ptr = (unsigned char *)"Hello, World!",
                    .len = countof("Hello, World!") - 1,
                },
            },
        },
        {
            .desc = "aven_io_writer_push empty file",
            .fn = test_aven_io_writer,
            .args = &(TestAvenIoWriteArgs){
                .exe_dir_path = exe_dir_path,
                .contents = { 0 },
            },
        },
        {
            .desc = "aven_io_writer_push text file",
            .fn = test_aven_io_writer,
            .args = &(TestAvenIoWriteArgs){
                .exe_dir_path = exe_dir_path,
                .contents = {
                    .ptr = (unsigned char *)"Hello, World!",
                    .len = countof("Hello, World!") - 1,
                },
            },
        },
        {
            .desc = "aven_io_writer_push bin file",
            .fn = test_aven_io_writer,
            .args = &(TestAvenIoWriteArgs){
                .exe_dir_path = exe_dir_path,
                .contents = slice_array(
                    (unsigned char[]){ 0xde, 0xad, 0xbe, 0xef }
                ),
            },
        },
        {
            .desc = "aven_io_writer_push_slice empty",
            .fn = test_aven_io_writer_slice,
            .args = &(TestAvenIoWriterSliceArgs){
                .slice = { 0 },
            },
        },
        {
            .desc = "aven_io_writer_push_slice one element",
            .fn = test_aven_io_writer_slice,
            .args = &(TestAvenIoWriterSliceArgs){
                .slice = slice_array(
                    (TestAvenIoStruct[]){
                        { .a = 1, .b = 2, .c = 3 },
                    }
                ),
            },
        },
        {
            .desc = "aven_io_writer_push_slice three elements",
            .fn = test_aven_io_writer_slice,
            .args = &(TestAvenIoWriterSliceArgs){
                .slice = slice_array(
                    (TestAvenIoStruct[]){
                        { .a = 1, .b = 2, .c = 3 },
                        { .a = 11, .b = 22, .c = 33 },
                        { .a = 111, .b = 222, .c = 333 },
                    }
                ),
            },
        },
    };
    AvenTestCaseSlice tcases = {
        .ptr = tcase_data,
        .len = countof(tcase_data),
    };

    aven_test(tcases, __FILE__, arena);

    return 0;
}

