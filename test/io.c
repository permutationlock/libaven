#include <aven.h>
#include <aven/arena.h>
#include <aven/io.h>
#include <aven/path.h>
#include <aven/str.h>
#include <aven/test.h>

typedef struct {
    AvenStr fpath;
    ByteSlice expected;
    AvenIoMode mode;
} TestAvenIoReadArgs;

AvenTestResult test_aven_io_read(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenIoReadArgs *io_args = args;

    AvenIoReadResult result = aven_io_read(
        io_args->fpath,
        AVEN_IO_MODE_TEXT,
        &arena
    );
    if (result.error != 0) {
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

    ByteSlice actual = result.payload;

    if (actual.len != io_args->expected.len) {
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
            (int)actual.len
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 2,
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
            .error = 3,
            .message = buffer,
        };
    }

    return (AvenTestResult){ 0 };
}

typedef struct {
    AvenStr exe_dir_path;
    ByteSlice contents;
    AvenIoMode mode;
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
            .error = 4,
            .message = buffer,
        };
    }

    AvenStr fname = (io_args->mode == AVEN_IO_MODE_BIN) ?
        aven_str("tmp.bin") :
        aven_str("tmp.txt");
    AvenStr tmp_path = aven_path(&arena, tmp_dir_path, fname);
    error = aven_io_write(
        tmp_path,
        io_args->mode,
        io_args->contents,
        arena
    );
    if (error != 0) {
        aven_fs_rm(tmp_path, arena);
        aven_fs_rmdir(tmp_dir_path, arena);

        char fmt[] = "error writing file \"%s\"";
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
            .error = 5,
            .message = buffer,
        };
    }

    TestAvenIoReadArgs read_args = {
        .expected = io_args->contents,
        .fpath = tmp_path,
        .mode = io_args->mode,
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

    AvenIoReaderResult result = aven_io_reader_init(
        io_args->fpath,
        AVEN_IO_MODE_TEXT,
        &arena
    );
    if (result.error != 0) {
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

    ByteSlice space = aven_arena_create_slice(
        unsigned char,
        &arena,
        io_args->expected.len
    );
    ByteSlice actual = aven_io_reader_pop(&result.payload, space);

    if (actual.len != io_args->expected.len) {
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
            (int)actual.len
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 2,
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
            .error = 3,
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
        aven_str("test_aven_io_writer")
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
            .error = 4,
            .message = buffer,
        };
    }

    AvenStr fname = (io_args->mode == AVEN_IO_MODE_BIN) ?
        aven_str("tmp.bin") :
        aven_str("tmp.txt");
    AvenStr tmp_path = aven_path(&arena, tmp_dir_path, fname);
    AvenIoWriter writer = aven_io_writer_init(io_args->contents.len, &arena);
    ByteSlice rem = aven_io_writer_push(&writer, io_args->contents);
    if (rem.len != 0) {
        aven_fs_rm(tmp_path, arena);
        aven_fs_rmdir(tmp_dir_path, arena);

        char fmt[] = "error %lu bytes didn't fit in writer";
        char *buffer = aven_arena_alloc(
            emsg_arena,
            sizeof(fmt) + 4,
            1,
            1
        );

        int len = sprintf(
            buffer,
            fmt,
            (unsigned long)rem.len
        );
        assert(len > 0);

        return (AvenTestResult){
            .error = 5,
            .message = buffer,
        };
    }
    error = aven_io_writer_commit(
        &writer,
        tmp_path,
        io_args->mode,
        arena
    );
    if (error != 0) {
        aven_fs_rm(tmp_path, arena);
        aven_fs_rmdir(tmp_dir_path, arena);

        char fmt[] = "error writing file \"%s\"";
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

    TestAvenIoReadArgs read_args = {
        .expected = io_args->contents,
        .fpath = tmp_path,
        .mode = io_args->mode,
    };

    AvenTestResult result = test_aven_io_read(emsg_arena, arena, &read_args);
    aven_fs_rm(tmp_path, arena);
    aven_fs_rmdir(tmp_dir_path, arena);

    return result;
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
            .desc = "aven_io_read empty file as bin",
            .fn = test_aven_io_read,
            .args = &(TestAvenIoReadArgs){
                .fpath = aven_path(
                    &arena,
                    test_root_path,
                    aven_str("io"),
                    aven_str("empty.bin")
                ),
                .expected = { 0 },
                .mode = AVEN_IO_MODE_BIN,
            },
        },
        {
            .desc = "aven_io_read empty file as text",
            .fn = test_aven_io_read,
            .args = &(TestAvenIoReadArgs){
                .fpath = aven_path(
                    &arena,
                    test_root_path,
                    aven_str("io"),
                    aven_str("empty.bin")
                ),
                .expected = { 0 },
                .mode = AVEN_IO_MODE_TEXT,
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
                    .ptr = (unsigned char *)"Hello, World!\n",
                    .len = countof("Hello, World!\n") - 1,
                },
                .mode = AVEN_IO_MODE_TEXT,
            },
        },
        {
            .desc = "aven_io_write empty file as bin",
            .fn = test_aven_io_write_read,
            .args = &(TestAvenIoWriteArgs){
                .exe_dir_path = exe_dir_path,
                .contents = { 0 },
                .mode = AVEN_IO_MODE_BIN,
            },
        },
        {
            .desc = "aven_io_write empty file as text",
            .fn = test_aven_io_write_read,
            .args = &(TestAvenIoWriteArgs){
                .exe_dir_path = exe_dir_path,
                .contents = { 0 },
                .mode = AVEN_IO_MODE_TEXT,
            },
        },
        {
            .desc = "aven_io_write text file",
            .fn = test_aven_io_write_read,
            .args = &(TestAvenIoWriteArgs){
                .exe_dir_path = exe_dir_path,
                .contents = {
                    .ptr = (unsigned char *)"Hello, World!\n",
                    .len = countof("Hello, World!\n") - 1,
                },
                .mode = AVEN_IO_MODE_TEXT,
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
                .mode = AVEN_IO_MODE_BIN,
            },
        },
        {
            .desc = "aven_io_reader read empty file as bin",
            .fn = test_aven_io_reader,
            .args = &(TestAvenIoReadArgs){
                .fpath = aven_path(
                    &arena,
                    test_root_path,
                    aven_str("io"),
                    aven_str("empty.bin")
                ),
                .expected = { 0 },
                .mode = AVEN_IO_MODE_BIN,
            },
        },
        {
            .desc = "aven_io_reader read empty file as text",
            .fn = test_aven_io_reader,
            .args = &(TestAvenIoReadArgs){
                .fpath = aven_path(
                    &arena,
                    test_root_path,
                    aven_str("io"),
                    aven_str("empty.bin")
                ),
                .expected = { 0 },
                .mode = AVEN_IO_MODE_TEXT,
            },
        },
        {
            .desc = "aven_io_reader read text file",
            .fn = test_aven_io_reader,
            .args = &(TestAvenIoReadArgs){
                .fpath = aven_path(
                    &arena,
                    test_root_path,
                    aven_str("io"),
                    aven_str("hello.txt")
                ),
                .expected = {
                    .ptr = (unsigned char *)"Hello, World!\n",
                    .len = countof("Hello, World!\n") - 1,
                },
                .mode = AVEN_IO_MODE_TEXT,
            },
        },
        {
            .desc = "aven_io_writer write empty file as bin",
            .fn = test_aven_io_writer,
            .args = &(TestAvenIoWriteArgs){
                .exe_dir_path = exe_dir_path,
                .contents = { 0 },
                .mode = AVEN_IO_MODE_BIN,
            },
        },
        {
            .desc = "aven_io_writer write empty file as text",
            .fn = test_aven_io_writer,
            .args = &(TestAvenIoWriteArgs){
                .exe_dir_path = exe_dir_path,
                .contents = { 0 },
                .mode = AVEN_IO_MODE_TEXT,
            },
        },
        {
            .desc = "aven_io_writer write text file",
            .fn = test_aven_io_writer,
            .args = &(TestAvenIoWriteArgs){
                .exe_dir_path = exe_dir_path,
                .contents = {
                    .ptr = (unsigned char *)"Hello, World!\n",
                    .len = countof("Hello, World!\n") - 1,
                },
                .mode = AVEN_IO_MODE_TEXT,
            },
        },
        {
            .desc = "aven_io_writer write bin file",
            .fn = test_aven_io_writer,
            .args = &(TestAvenIoWriteArgs){
                .exe_dir_path = exe_dir_path,
                .contents = slice_array(
                    (unsigned char[]){ 0xde, 0xad, 0xbe, 0xef }
                ),
                .mode = AVEN_IO_MODE_BIN,
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

