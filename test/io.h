#ifndef TEST_IO_H
    #define TEST_IO_H

    #include <aven.h>
    #include <aven/arena.h>
    #include <aven/fs.h>
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
            return (AvenTestResult){
                .error = (int)fd_res.error,
                .message = aven_fmt(
                    emsg_arena,
                    "error opening file \"{}\"",
                    aven_fmt_str(io_args->fpath)
                ),
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
            return (AvenTestResult){
                .error = (int)rd_res.error,
                .message = aven_fmt(
                    emsg_arena,
                    "error reading file \"{}\"",
                    aven_fmt_str(io_args->fpath)
                ),
            };
        }

        if (rd_res.payload != io_args->expected.len) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected {} bytes, found {} bytes",
                    aven_fmt_uint(io_args->expected.len),
                    aven_fmt_uint(rd_res.payload)
                ),
            };
        }

        size_t diff = 0;
        for (size_t i = 0; i < actual.len; i += 1) {
            if (get(actual, i) != get(io_args->expected, i)) {
                diff += 1;
            }
        }

        if (diff > 0) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "file contents differed by {} bytes",
                    aven_fmt_uint(diff)
                ),
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
        int error = (int)aven_fs_mkdir(tmp_dir_path, arena);
        if (error != 0) {
            return (AvenTestResult){
                .error = error,
                .message = aven_fmt(
                    emsg_arena,
                    "error creating directory \"{}\"",
                    aven_fmt_str(tmp_dir_path)
                ),
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
            return (AvenTestResult){
                .error = (int)fd_res.error,
                .message = aven_fmt(
                    emsg_arena,
                    "error opening file\"{}\"",
                    aven_fmt_str(tmp_path)
                ),
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
            return (AvenTestResult){
                .error = (int)wr_res.error,
                .message = aven_fmt(
                    emsg_arena,
                    "error writing to file\"{}\"",
                    aven_fmt_str(tmp_path)
                ),
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
            return (AvenTestResult){
                .error = (int)fd_res.error,
                .message = aven_fmt(
                    emsg_arena,
                    "error opening file\"{}\"",
                    aven_fmt_str(io_args->fpath)
                ),
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
            return (AvenTestResult){
                .error = rd_res.error,
                .message = aven_fmt(
                    emsg_arena,
                    "error reading from file\"{}\"",
                    aven_fmt_str(io_args->fpath)
                ),
            };
        }

        if (rd_res.payload != io_args->expected.len) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected {} bytes, found {} bytes",
                    aven_fmt_uint(io_args->expected.len),
                    aven_fmt_uint(rd_res.payload)
                ),
            };
        }

        size_t diff = 0;
        for (size_t i = 0; i < actual.len; i += 1) {
            if (get(actual, i) != get(io_args->expected, i)) {
                diff += 1;
            }
        }

        if (diff > 0) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "file contents differed by {} bytes",
                    aven_fmt_uint(diff)
                ),
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
        int error = (int)aven_fs_mkdir(tmp_dir_path, arena);
        if (error != 0) {
            return (AvenTestResult){
                .error = error,
                .message = aven_fmt(
                    emsg_arena,
                    "error creating directory \"{}\"",
                    aven_fmt_str(tmp_dir_path)
                ),
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
            return (AvenTestResult){
                .error = (int)fd_res.error,
                .message = aven_fmt(
                    emsg_arena,
                    "error opening file \"{}\"",
                    aven_fmt_str(tmp_path)
                ),
            };
        }

        AvenIoWriter writer = aven_io_writer_init_fd_buffered(
            fd_res.payload,
            8,
            &arena
        );
        AvenIoResult wr_res = aven_io_writer_push(&writer, io_args->contents);
        AvenIoWriteError fl_error = (AvenIoWriteError)aven_io_writer_flush(
            &writer
        );
        aven_io_close(fd_res.payload);

        if (wr_res.error != 0 or fl_error != 0) {
            aven_fs_rm(tmp_path, arena);
            aven_fs_rmdir(tmp_dir_path, arena);
            return (AvenTestResult){
                .error = (wr_res.error == 0 ? (int)wr_res.error : (int)fl_error),
                .message = aven_fmt(
                    emsg_arena,
                    "error writing to file\"{}\"",
                    aven_fmt_str(tmp_path)
                ),
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
    typedef List(TestAvenIoStruct) TestAvenIoStructList;
    typedef Queue(TestAvenIoStruct) TestAvenIoStructQueue;
    typedef Pool(TestAvenIoStruct) TestAvenIoStructPool;

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
                .message = aven_str("error writing slice"),
            };
        }

        if (writer.index != space.len) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_str("written slice too small"),
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
                .message = aven_str("error reading slice"),
            };
        }

        TestAvenIoStructSlice read_slice = aven_io_slice(
            TestAvenIoStruct,
            rd_res.payload
        );

        if (read_slice.len != io_args->slice.len) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected slice len {}, found {}",
                    aven_fmt_uint(io_args->slice.len),
                    aven_fmt_uint(read_slice.len)
                ),
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
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "read slice and written slice differed in {} / {} entries",
                    aven_fmt_uint(io_args->slice.len - entries_equal),
                    aven_fmt_uint(io_args->slice.len)
                ),
            };
        }

        return (AvenTestResult){ 0 };
    }

    typedef struct {
        TestAvenIoStructList list;
    } TestAvenIoWriterListArgs;

    AvenTestResult test_aven_io_writer_list(
        AvenArena *emsg_arena,
        AvenArena arena,
        void *args
    ) {
        TestAvenIoWriterListArgs *io_args = args;

        ByteSlice space = aven_arena_create_slice(
            unsigned char,
            &arena,
            aven_io_list_size(io_args->list)
        );

        AvenIoWriter writer = aven_io_writer_init_bytes(space);
        int error = aven_io_writer_push_list(&writer, io_args->list);

        if (error != 0) {
            return (AvenTestResult){
                .error = error,
                .message = aven_str("error writing list"),
            };
        }

        if (writer.index != space.len) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_str("written list too small"),
            };
        }

        AvenIoReader reader = aven_io_reader_init_bytes(space);
        AvenIoListResult rd_res = aven_io_reader_pop_list(
            TestAvenIoStruct,
            &reader,
            &arena
        );

        if (rd_res.error != 0) {
            return (AvenTestResult){
                .error = rd_res.error,
                .message = aven_str("error reading list"),
            };
        }

        TestAvenIoStructList read_list = aven_io_list(
            TestAvenIoStruct,
            rd_res.payload
        );

        if (read_list.cap != io_args->list.cap) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected list cap {}, found {}",
                    aven_fmt_uint(io_args->list.cap),
                    aven_fmt_uint(read_list.cap)
                ),
            };
        }

        if (read_list.len != io_args->list.len) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected list len {}, found {}",
                    aven_fmt_uint(io_args->list.len),
                    aven_fmt_uint(read_list.len)
                ),
            };
        }

        size_t entries_equal = 0;
        for (size_t i = 0; i < read_list.len; i += 1) {
            TestAvenIoStruct actual = get(read_list, i);
            TestAvenIoStruct expected = get(io_args->list, i);
            if (
                actual.a == expected.a and
                actual.b == expected.b and
                actual.c == expected.c
            ) {
                entries_equal += 1;
            }
        }

        if (entries_equal != io_args->list.len) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "read list and written list differed in {} / {} entries",
                    aven_fmt_uint(io_args->list.len - entries_equal),
                    aven_fmt_uint(io_args->list.len)
                ),
            };
        }

        return (AvenTestResult){ 0 };
    }

    typedef struct {
        TestAvenIoStructQueue queue;
    } TestAvenIoWriterQueueArgs;

    AvenTestResult test_aven_io_writer_queue(
        AvenArena *emsg_arena,
        AvenArena arena,
        void *args
    ) {
        TestAvenIoWriterQueueArgs *io_args = args;

        ByteSlice space = aven_arena_create_slice(
            unsigned char,
            &arena,
            aven_io_queue_size(io_args->queue)
        );

        AvenIoWriter writer = aven_io_writer_init_bytes(space);
        int error = aven_io_writer_push_queue(&writer, io_args->queue);

        if (error != 0) {
            return (AvenTestResult){
                .error = error,
                .message = aven_str("error writing queue"),
            };
        }

        if (writer.index != space.len) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_str("written queue too small"),
            };
        }

        AvenIoReader reader = aven_io_reader_init_bytes(space);
        AvenIoQueueResult rd_res = aven_io_reader_pop_queue(
            TestAvenIoStruct,
            &reader,
            &arena
        );

        if (rd_res.error != 0) {
            return (AvenTestResult){
                .error = rd_res.error,
                .message = aven_str("error reading queue"),
            };
        }

        TestAvenIoStructQueue read_queue = aven_io_queue(
            TestAvenIoStruct,
            rd_res.payload
        );

        if (read_queue.cap != io_args->queue.cap) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected queue cap {}, found {}",
                    aven_fmt_uint(io_args->queue.cap),
                    aven_fmt_uint(read_queue.cap)
                ),
            };
        }

        if (read_queue.used != io_args->queue.used) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected queue used {}, found {}",
                    aven_fmt_uint(io_args->queue.used),
                    aven_fmt_uint(read_queue.used)
                ),
            };
        }

        size_t entries_equal = 0;
        for (size_t i = 0; i < read_queue.used; i += 1) {
            TestAvenIoStruct actual = queue_get(read_queue, i);
            TestAvenIoStruct expected = queue_get(io_args->queue, i);
            if (
                actual.a == expected.a and
                actual.b == expected.b and
                actual.c == expected.c
            ) {
                entries_equal += 1;
            }
        }

        if (entries_equal != io_args->queue.used) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "read queue and written queue differed in {} / {} entries",
                    aven_fmt_uint(io_args->queue.used - entries_equal),
                    aven_fmt_uint(io_args->queue.used)
                ),
            };
        }

        return (AvenTestResult){ 0 };
    }

    typedef Slice(uint32_t) TestAvenIoPoolIndexSlice;
    typedef struct {
        uint32_t size;
        TestAvenIoStructSlice inserts;
        TestAvenIoPoolIndexSlice deletes;
    } TestAvenIoWriterPoolArgs;

    AvenTestResult test_aven_io_writer_pool(
        AvenArena *emsg_arena,
        AvenArena arena,
        void *args
    ) {
        TestAvenIoWriterPoolArgs *io_args = args;

        TestAvenIoStructPool pool = aven_arena_create_pool(
            TestAvenIoStruct,
            &arena,
            io_args->size
        );
        Slice(bool) valid_entries = aven_arena_create_slice(
            bool,
            &arena,
            io_args->inserts.len
        );
        for (uint32_t i = 0; i < io_args->inserts.len; i += 1) {
            Idx idx = pool_create(pool);
            pool_get(pool, idx) = get(io_args->inserts, i);
            get(valid_entries, idx_unwrap(idx)) = true;
        }
        for (uint32_t i = 0; i < io_args->deletes.len; i += 1) {
            Idx idx = idx_wrap(get(io_args->deletes, i));
            pool_delete(pool, idx);
            get(valid_entries, idx_unwrap(idx)) = false;
        }

        ByteSlice space = aven_arena_create_slice(
            unsigned char,
            &arena,
            aven_io_pool_size(pool)
        );

        AvenIoWriter writer = aven_io_writer_init_bytes(space);
        int error = aven_io_writer_push_pool(&writer, pool);

        if (error != 0) {
            return (AvenTestResult){
                .error = error,
                .message = aven_str("error writing pool"),
            };
        }

        if (writer.index != space.len) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_str("written pool too small"),
            };
        }

        AvenIoReader reader = aven_io_reader_init_bytes(space);
        AvenIoPoolResult rd_res = aven_io_reader_pop_pool(
            TestAvenIoStruct,
            &reader,
            &arena
        );

        if (rd_res.error != 0) {
            return (AvenTestResult){
                .error = rd_res.error,
                .message = aven_str("error reading pool"),
            };
        }

        TestAvenIoStructPool read_pool = aven_io_pool(
            TestAvenIoStruct,
            rd_res.payload
        );

        if (read_pool.cap != pool.cap) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected pool cap {}, found {}",
                    aven_fmt_uint(pool.cap),
                    aven_fmt_uint(read_pool.cap)
                ),
            };
        }

        if (read_pool.len != pool.len) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected pool len {}, found {}",
                    aven_fmt_uint(pool.len),
                    aven_fmt_uint(read_pool.len)
                ),
            };
        }

        if (read_pool.free.index != pool.free.index) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected pool free {}, found {}",
                    aven_fmt_uint(pool.free.index),
                    aven_fmt_uint(read_pool.free.index)
                ),
            };
        }

        if (read_pool.used != pool.used) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected pool used {}, found {}",
                    aven_fmt_uint(pool.used),
                    aven_fmt_uint(read_pool.used)
                ),
            };
        }

        bool valid = true;
        uint32_t count = 0;
        Idx free = pool_next_free(read_pool, (Idx){ 0 });
        while (
            idx_valid(free) != 0 and
            idx_unwrap(free) < read_pool.len and
            count < read_pool.len
        ) {
            if (get(valid_entries, idx_unwrap(free))) {
                valid = false;
                break;
            }
            free = pool_next_free(read_pool, free);
            count += 1;
        }

        if (!valid) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_str("valid pool entry in read pool's free list"),
            };
        }
        if (count != pool.len - pool.used) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected {} element(s) in free list, found {}",
                    aven_fmt_uint(pool.len - pool.used),
                    aven_fmt_uint(count)
                ),
            };
        }

        uint32_t entries_equal = 0;
        for (uint32_t i = 0; i < read_pool.len; i += 1) {
            if (!get(valid_entries, i)) {
                continue;
            }
            TestAvenIoStruct actual = pool_get(read_pool, idx_wrap(i));
            TestAvenIoStruct expected = pool_get(pool, idx_wrap(i));
            if (
                actual.a == expected.a and
                actual.b == expected.b and
                actual.c == expected.c
            ) {
                entries_equal += 1;
            }
        }

        if (entries_equal != pool.used) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "read pool and written pool differed in {} / {} entries",
                    aven_fmt_uint(pool.len - entries_equal),
                    aven_fmt_uint(pool.len)
                ),
            };
        }

        return (AvenTestResult){ 0 };
    }

    static int test_io(AvenArena arena) {
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
                .desc = aven_str("aven_io_read empty file"),
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
                .desc = aven_str("aven_io_read text file"),
                .fn = test_aven_io_read,
                .args = &(TestAvenIoReadArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("io"),
                        aven_str("hello.txt")
                    ),
                    .expected = slice_as_bytes(aven_str("Hello, World!")),
                },
            },
            {
                .desc = aven_str("aven_io_write empty file"),
                .fn = test_aven_io_write_read,
                .args = &(TestAvenIoWriteArgs){
                    .exe_dir_path = exe_dir_path,
                    .contents = { 0 },
                },
            },
            {
                .desc = aven_str("aven_io_write text file"),
                .fn = test_aven_io_write_read,
                .args = &(TestAvenIoWriteArgs){
                    .exe_dir_path = exe_dir_path,
                    .contents = slice_as_bytes(aven_str("Hello, World!")),
                },
            },
            {
                .desc = aven_str("aven_io_write bin file"),
                .fn = test_aven_io_write_read,
                .args = &(TestAvenIoWriteArgs){
                    .exe_dir_path = exe_dir_path,
                    .contents = slice_array(
                        (unsigned char[]){ 0xde, 0xad, 0xbe, 0xef }
                    ),
                },
            },
            {
                .desc = aven_str("aven_io_reader_pop empty file"),
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
                .desc = aven_str("aven_io_reader_pop text file"),
                .fn = test_aven_io_reader,
                .args = &(TestAvenIoReadArgs){
                    .fpath = aven_path(
                        &arena,
                        test_root_path,
                        aven_str("io"),
                        aven_str("hello.txt")
                    ),
                    .expected = slice_as_bytes(aven_str("Hello, World!")),
                },
            },
            {
                .desc = aven_str("aven_io_writer_push empty file"),
                .fn = test_aven_io_writer,
                .args = &(TestAvenIoWriteArgs){
                    .exe_dir_path = exe_dir_path,
                    .contents = { 0 },
                },
            },
            {
                .desc = aven_str("aven_io_writer_push text file"),
                .fn = test_aven_io_writer,
                .args = &(TestAvenIoWriteArgs){
                    .exe_dir_path = exe_dir_path,
                    .contents = slice_as_bytes(aven_str("Hello, World!")),
                },
            },
            {
                .desc = aven_str("aven_io_writer_push bin file"),
                .fn = test_aven_io_writer,
                .args = &(TestAvenIoWriteArgs){
                    .exe_dir_path = exe_dir_path,
                    .contents = slice_array(
                        (unsigned char[]){ 0xde, 0xad, 0xbe, 0xef }
                    ),
                },
            },
            {
                .desc = aven_str("aven_io_writer_push_slice empty"),
                .fn = test_aven_io_writer_slice,
                .args = &(TestAvenIoWriterSliceArgs){ .slice = { 0 } },
            },
            {
                .desc = aven_str("aven_io_writer_push_slice 1 element"),
                .fn = test_aven_io_writer_slice,
                .args = &(TestAvenIoWriterSliceArgs){
                    .slice = slice_array(
                        (TestAvenIoStruct[]){ { .a = 1, .b = 2, .c = 3 } }
                    ),
                },
            },
            {
                .desc = aven_str("aven_io_writer_push_slice 3 elements"),
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
            {
                .desc = aven_str("aven_io_writer_push_list empty"),
                .fn = test_aven_io_writer_list,
                .args = &(TestAvenIoWriterListArgs){ .list = { 0 } },
            },
            {
                .desc = aven_str("aven_io_writer_push_list 1 element empty"),
                .fn = test_aven_io_writer_list,
                .args = &(TestAvenIoWriterListArgs){
                    .list = {
                        .ptr = (TestAvenIoStruct[]){
                            { .a = 1, .b = 2, .c = 3 },
                        },
                        .cap = 1,
                        .len = 0,
                    },
                },
            },
            {
                .desc = aven_str("aven_io_writer_push_list 1 element len"),
                .fn = test_aven_io_writer_list,
                .args = &(TestAvenIoWriterListArgs){
                    .list = {
                        .ptr = (TestAvenIoStruct[]){
                            { .a = 1, .b = 2, .c = 3 },
                        },
                        .cap = 1,
                        .len = 1,
                    },
                },
            },
            {
                .desc = aven_str("aven_io_writer_push_list 3 elements empty"),
                .fn = test_aven_io_writer_list,
                .args = &(TestAvenIoWriterListArgs){
                    .list = {
                        .ptr = (TestAvenIoStruct[]){
                            { .a = 1, .b = 2, .c = 3 },
                            { .a = 11, .b = 22, .c = 33 },
                            { .a = 111, .b = 222, .c = 333 },
                        },
                        .cap = 3,
                        .len = 0,
                    },
                },
            },
            {
                .desc = aven_str("aven_io_writer_push_list 3 elements 2 used"),
                .fn = test_aven_io_writer_list,
                .args = &(TestAvenIoWriterListArgs){
                    .list = {
                        .ptr = (TestAvenIoStruct[]){
                            { .a = 1, .b = 2, .c = 3 },
                            { .a = 11, .b = 22, .c = 33 },
                            { .a = 111, .b = 222, .c = 333 },
                        },
                        .cap = 3,
                        .len = 2,
                    },
                },
            },
            {
                .desc = aven_str("aven_io_writer_push_list 3 elements full"),
                .fn = test_aven_io_writer_list,
                .args = &(TestAvenIoWriterListArgs){
                    .list = {
                        .ptr = (TestAvenIoStruct[]){
                            { .a = 1, .b = 2, .c = 3 },
                            { .a = 11, .b = 22, .c = 33 },
                            { .a = 111, .b = 222, .c = 333 },
                        },
                        .cap = 3,
                        .len = 3,
                    },
                },
            },
            {
                .desc = aven_str("aven_io_writer_push_queue empty"),
                .fn = test_aven_io_writer_queue,
                .args = &(TestAvenIoWriterQueueArgs){ .queue = { 0 } },
            },
            {
                .desc = aven_str("aven_io_writer_push_queue 1 element empty"),
                .fn = test_aven_io_writer_queue,
                .args = &(TestAvenIoWriterQueueArgs){
                    .queue = {
                        .ptr = (TestAvenIoStruct[]){
                            { .a = 1, .b = 2, .c = 3 },
                        },
                        .cap = 1,
                        .used = 0,
                        .front = 0,
                        .back = 0,
                    },
                },
            },
            {
                .desc = aven_str("aven_io_writer_push_queue 1 element used"),
                .fn = test_aven_io_writer_queue,
                .args = &(TestAvenIoWriterQueueArgs){
                    .queue = {
                        .ptr = (TestAvenIoStruct[]){
                            { .a = 1, .b = 2, .c = 3 },
                        },
                        .cap = 1,
                        .used = 1,
                        .front = 0,
                        .back = 0,
                    },
                },
            },
            {
                .desc = aven_str("aven_io_writer_push_queue 3 elements empty"),
                .fn = test_aven_io_writer_queue,
                .args = &(TestAvenIoWriterQueueArgs){
                    .queue = {
                        .ptr = (TestAvenIoStruct[]){
                            { .a = 1, .b = 2, .c = 3 },
                            { .a = 11, .b = 22, .c = 33 },
                            { .a = 111, .b = 222, .c = 333 },
                        },
                        .cap = 3,
                        .used = 0,
                        .front = 0,
                        .back = 0,
                    },
                },
            },
            {
                .desc = aven_str("aven_io_writer_push_queue 3 elements 2 used"),
                .fn = test_aven_io_writer_queue,
                .args = &(TestAvenIoWriterQueueArgs){
                    .queue = {
                        .ptr = (TestAvenIoStruct[]){
                            { .a = 1, .b = 2, .c = 3 },
                            { .a = 11, .b = 22, .c = 33 },
                            { .a = 111, .b = 222, .c = 333 },
                        },
                        .cap = 3,
                        .used = 2,
                        .front = 1,
                        .back = 0,
                    },
                },
            },
            {
                .desc = aven_str("aven_io_writer_push_queue 3 elements full"),
                .fn = test_aven_io_writer_queue,
                .args = &(TestAvenIoWriterQueueArgs){
                    .queue = {
                        .ptr = (TestAvenIoStruct[]){
                            { .a = 1, .b = 2, .c = 3 },
                            { .a = 11, .b = 22, .c = 33 },
                            { .a = 111, .b = 222, .c = 333 },
                        },
                        .cap = 3,
                        .used = 3,
                        .front = 2,
                        .back = 2,
                    },
                },
            },
            {
                .desc = aven_str("aven_io_writer_push_pool empty"),
                .fn = test_aven_io_writer_pool,
                .args = &(TestAvenIoWriterPoolArgs){
                    .size = 0,
                    .inserts = { 0 },
                    .deletes = { 0 },
                },
            },
            {
                .desc = aven_str("aven_io_writer_push_pool 1 element inserted"),
                .fn = test_aven_io_writer_pool,
                .args = &(TestAvenIoWriterPoolArgs){
                    .size = 1,
                    .inserts = slice_array(
                        (TestAvenIoStruct[]){ { .a = 1, .b = 1, .c = 1 } }
                    ),
                    .deletes = { 0 },
                },
            },
            {
                .desc = aven_str(
                    "aven_io_writer_push_pool 1 element inserted then deleted"
                ),
                .fn = test_aven_io_writer_pool,
                .args = &(TestAvenIoWriterPoolArgs){
                    .size = 1,
                    .inserts = slice_array(
                        (TestAvenIoStruct[]){ { .a = 1, .b = 1, .c = 1 } }
                    ),
                    .deletes = slice_array((uint32_t[]){ 0 }),
                },
            },
            {
                .desc = aven_str("aven_io_writer_push_pool 2 elements inserted"),
                .fn = test_aven_io_writer_pool,
                .args = &(TestAvenIoWriterPoolArgs){
                    .size = 3,
                    .inserts = slice_array(
                        (TestAvenIoStruct[]){
                            { .a = 1, .b = 1, .c = 1 },
                            { .a = 2, .b = 2, .c = 2 },
                        }
                    ),
                    .deletes = { 0 },
                },
            },
            {
                .desc = aven_str(
                    "aven_io_writer_push_pool 2 elements inserted, 1 deleted"
                ),
                .fn = test_aven_io_writer_pool,
                .args = &(TestAvenIoWriterPoolArgs){
                    .size = 3,
                    .inserts = slice_array(
                        (TestAvenIoStruct[]){
                            { .a = 1, .b = 1, .c = 1 },
                            { .a = 2, .b = 2, .c = 2 },
                        }
                    ),
                    .deletes = slice_array((uint32_t[]){ 0 }),
                },
            },
            {
                .desc = aven_str("aven_io_writer_push_pool 5 elements inserted"),
                .fn = test_aven_io_writer_pool,
                .args = &(TestAvenIoWriterPoolArgs){
                    .size = 7,
                    .inserts = slice_array(
                        (TestAvenIoStruct[]){
                            { .a = 1, .b = 1, .c = 1 },
                            { .a = 2, .b = 2, .c = 2 },
                            { .a = 3, .b = 3, .c = 3 },
                            { .a = 4, .b = 4, .c = 4 },
                            { .a = 5, .b = 5, .c = 5 },
                        }
                    ),
                    .deletes = { 0 },
                },
            },
            {
                .desc = aven_str(
                    "aven_io_writer_push_pool 5 elements inserted, 2 deleted"
                ),
                .fn = test_aven_io_writer_pool,
                .args = &(TestAvenIoWriterPoolArgs){
                    .size = 7,
                    .inserts = slice_array(
                        (TestAvenIoStruct[]){
                            { .a = 1, .b = 1, .c = 1 },
                            { .a = 2, .b = 2, .c = 2 },
                            { .a = 3, .b = 3, .c = 3 },
                            { .a = 4, .b = 4, .c = 4 },
                            { .a = 5, .b = 5, .c = 5 },
                        }
                    ),
                    .deletes = slice_array((uint32_t[]){ 1, 3 }),
                },
            },
        };
        AvenTestCaseSlice tcases = slice_array(tcase_data);

        aven_test(tcases, arena);

        return 0;
    }
#endif
// TEST_IO_H
