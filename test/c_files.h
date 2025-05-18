#ifndef TEST_C_FILES_H
    #define TEST_C_FILES_H

    #include <aven.h>
    #include <aven/arena.h>
    #include <aven/c.h>
    #include <aven/io.h>
    #include <aven/path.h>
    #include <aven/str.h>
    #include <aven/test.h>

    typedef struct {
        AvenStr fpath;
    } TestAvenCFileArgs;

    AvenTestResult test_aven_c_file(
        AvenArena *emsg_arena,
        AvenArena arena,
        void *args
    ) {
        TestAvenCFileArgs *fargs = args;
        AvenIoOpenResult ores = aven_io_open(
            fargs->fpath,
            AVEN_IO_OPEN_MODE_READ,
            arena
        );
        AvenIoReader reader = aven_io_reader_init_fd(ores.payload);
        if (ores.error != 0) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "error opening {}",
                    aven_fmt_str(fargs->fpath)
                ),
            };
        }
        size_t block_size = 8192;
        AvenIoBytesResult rd_res = aven_io_reader_pop_all(
            &reader,
            block_size,
            &arena
        );
        if (rd_res.error != 0) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "error reading {}",
                    aven_fmt_str(fargs->fpath)
                ),
            };
        }
        AvenStr src = {
            .ptr = (char *)rd_res.payload.ptr,
            .len = rd_res.payload.len,
        };
        ByteSlice bytes = aven_arena_create_slice(unsigned char, &arena, 512000);
        AvenIoWriter writer = aven_io_writer_init_bytes(bytes);
        AvenCFmtResult fmt_res = aven_c_fmt(src, &writer, 80, 4, 12, &arena);
        if (fmt_res.error != 0) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "format error: {}",
                    aven_fmt_str(fmt_res.msg)
                ),
            };
        }
        AvenStr act = { .ptr = (char *)writer.buffer.ptr, .len = writer.index };
        AvenIoReader exp_reader = aven_io_reader_init_bytes(slice_as_bytes(src));
        AvenIoReader act_reader = aven_io_reader_init_bytes(slice_as_bytes(act));
        for (;;) {
            AvenArena temp_arena = arena;
            AvenIoBytesResult exp_res = aven_io_reader_pop_line(
                &exp_reader,
                4096,
                &temp_arena
            );
            AvenIoBytesResult act_res = aven_io_reader_pop_line(
                &act_reader,
                4096,
                &temp_arena
            );
            assert(exp_res.error == 0 and act_res.error == 0);
            AvenStr exp_line = {
                .ptr = (char *)exp_res.payload.ptr,
                .len = exp_res.payload.len,
            };
            AvenStr act_line = {
                .ptr = (char *)act_res.payload.ptr,
                .len = act_res.payload.len,
            };
            if (exp_line.len == 0 and act_line.len == 0) {
                break;
            }
            if (exp_line.len != 0) {
                exp_line = aven_str_head(exp_line, exp_line.len - 1);
            }
            if (act_line.len != 0) {
                act_line = aven_str_head(act_line, act_line.len - 1);
            }
            if (exp_line.len != 0 and get(exp_line, exp_line.len - 1) == '\r') {
                exp_line = aven_str_head(exp_line, exp_line.len - 1);
            }
            if (act_line.len != 0 and get(act_line, act_line.len - 1) == '\r') {
                act_line = aven_str_head(act_line, act_line.len - 1);
            }
            size_t i = 0;
            for (; i < min(exp_line.len, act_line.len); i += 1) {
                if (get(exp_line, i) != get(act_line, i)) {
                    break;
                }
            }
            if (i != exp_line.len) {
                size_t start = i > 100 ? i - 100 : 0;
                size_t exp_end = (i + 32) < exp_line.len ? i + 32 : exp_line.len;
                size_t act_end = (i + 32) < act_line.len ? i + 32 : act_line.len;
                AvenStr exp = aven_str_range(exp_line, start, exp_end);
                AvenStr found = aven_str_range(act_line, start, act_end);
                return (AvenTestResult){
                    .error = 1,
                    .message = aven_fmt(
                        emsg_arena,
                        "expected:\n    \"{}\"\nfound:\n    \"{}\"\n",
                        aven_fmt_str(exp),
                        aven_fmt_str(found)
                    ),
                };
            }
        }
        return (AvenTestResult){ 0 };
    }

    static int test_c_files(AvenArena arena) {
        AvenPathResult exe_res = aven_path_exe(&arena);
        if (exe_res.error != 0) {
            aven_io_printf(
                "unable to run tests for {}: could not find path to test exe\n",
                aven_fmt_str(aven_str(__FILE__))
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
                .desc = aven_str("aven_c_fmt aven.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("aven.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt arg.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("arg.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt arena.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("arena.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt build.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("build.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt c.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("c.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt common.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("common.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt dl.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("dl.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt fmt.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("fmt.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt hash.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("hash.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt io.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("io.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt math.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("math.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt path.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("path.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt pool.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("pool.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt proc.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("proc.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt proc.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("proc.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt rng.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("rng.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt spinlock.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("spinlock.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt str.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("str.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt test.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("test.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt thread.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("thread.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt time.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("time.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt watch.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("watch.h")
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_fmt wyhash.h"),
                .fn = test_aven_c_file,
                .args = &(TestAvenCFileArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("c"),
                        aven_str("wyhash.h")
                    ),
                },
            },
        };
        AvenTestCaseSlice tcases = slice_array(tcase_data);

        aven_test(tcases, arena);

        return 0;
    }
#endif
