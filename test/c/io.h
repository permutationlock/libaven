#ifndef AVEN_IO_H
    #define AVEN_IO_H

    #include "../aven.h"
    #include "arena.h"
    #include "fmt.h"
    #include "str.h"

    typedef int AvenIoFd;

    typedef enum {
        AVEN_IO_ERROR_NONE = 0,
        AVEN_IO_ERROR_BADF,
        AVEN_IO_ERROR_OTHER,
        AVEN_IO_ERROR_NOSPACE,
        AVEN_IO_ERROR_FINGERPRINT,
        AVEN_IO_ERROR_MISMATCH,
    } AvenIoError;
    typedef Result(size_t, int) AvenIoResult;

    typedef enum {
        AVEN_IO_OPEN_MODE_READ = 0,
        AVEN_IO_OPEN_MODE_WRITE,
        AVEN_IO_OPEN_MODE_APPEND,
    } AvenIoOpenMode;

    typedef enum {
        AVEN_IO_OPEN_ERROR_NONE = 0,
        AVEN_IO_OPEN_ERROR_ACCESS,
        AVEN_IO_OPEN_ERROR_BADPATH,
        AVEN_IO_OPEN_ERROR_OTHER,
    } AvenIoOpenError;
    typedef Result(AvenIoFd, AvenIoOpenError) AvenIoOpenResult;

    AVEN_FN AvenIoOpenResult aven_io_open(
        AvenStr file_path,
        AvenIoOpenMode mode,
        AvenArena temp_arena
    );

    typedef enum {
        AVEN_IO_READ_ERROR_NONE = AVEN_IO_ERROR_NONE,
        AVEN_IO_READ_ERROR_BADF = AVEN_IO_ERROR_BADF,
        AVEN_IO_READ_ERROR_OTHER = AVEN_IO_ERROR_OTHER,
    } AvenIoReadError;
    typedef Result(size_t, AvenIoReadError) AvenIoReadResult;

    AVEN_FN AvenIoReadResult aven_io_read(AvenIoFd fd, ByteSlice dest);

    typedef enum {
        AVEN_IO_WRITE_ERROR_NONE = AVEN_IO_ERROR_NONE,
        AVEN_IO_WRITE_ERROR_BADF = AVEN_IO_ERROR_BADF,
        AVEN_IO_WRITE_ERROR_OTHER = AVEN_IO_ERROR_OTHER,
    } AvenIoWriteError;
    typedef Result(size_t, AvenIoWriteError) AvenIoWriteResult;

    AVEN_FN AvenIoWriteResult aven_io_write(AvenIoFd fd, ByteSlice src);

    AVEN_FN void aven_io_close(AvenIoFd fd);

    typedef union {
        void *ptr;
        AvenIoFd fd;
    } AvenIoCtx;

    typedef AvenIoResult AvenIoFn(AvenIoCtx ctx, ByteSlice bytes);

    typedef struct {
        List(uint8_t) buffer;
        size_t index;
        AvenIoFn *read;
        AvenIoCtx ctx;
    } AvenIoReader;

    typedef struct {
        ByteSlice buffer;
        size_t index;
        AvenIoFn *write;
        AvenIoCtx ctx;
    } AvenIoWriter;

    static AvenIoResult aven_io_sink_stub(AvenIoCtx ctx, ByteSlice dest) {
        (void)ctx;
        (void)dest;
        return (AvenIoResult){ .payload = dest.len };
    }

    static inline AvenIoWriter aven_io_writer_init_sink(void) {
        return (AvenIoWriter){ .write = aven_io_sink_stub };
    }

    static AvenIoResult aven_io_null_stub(AvenIoCtx ctx, ByteSlice dest) {
        (void)ctx;
        (void)dest;
        return (AvenIoResult){ 0 };
    }

    static AvenIoResult aven_io_fd_read_stub(AvenIoCtx ctx, ByteSlice dest) {
        AvenIoReadResult res = aven_io_read(ctx.fd, dest);
        return (AvenIoResult){ .payload = res.payload, .error = (int)res.error };
    }

    static AvenIoResult aven_io_fd_write_stub(AvenIoCtx ctx, ByteSlice dest) {
        AvenIoWriteResult res = aven_io_write(ctx.fd, dest);
        return (AvenIoResult){ .payload = res.payload, .error = (int)res.error };
    }

    static inline AvenIoReader aven_io_reader_init_fd(AvenIoFd fd) {
        return (AvenIoReader){
            .read = aven_io_fd_read_stub,
            .ctx = { .fd = fd },
        };
    }

    static inline AvenIoReader aven_io_reader_init_fd_buffered(
        AvenIoFd fd,
        size_t size,
        AvenArena *arena
    ) {
        return (AvenIoReader){
            .buffer = aven_arena_create_list(unsigned char, arena, size),
            .ctx = { .fd = fd },
            .read = aven_io_fd_read_stub,
        };
    }

    static inline AvenIoReader aven_io_reader_init_bytes(ByteSlice bytes) {
        return (AvenIoReader){
            .buffer = { .ptr = bytes.ptr, .len = bytes.len, .cap = bytes.len },
            .read = aven_io_null_stub,
        };
    }

    #if __has_attribute(unused)
        __attribute__((unused))
    #endif
    static AvenIoReader aven_io_stdin = {
        .read = aven_io_fd_read_stub,
        .ctx = { .fd = 0 },
    };

    static inline AvenIoReader aven_io_reader_init_stdin_buffered(
        size_t size,
        AvenArena *arena
    ) {
        return aven_io_reader_init_fd_buffered(0, size, arena);
    }

    static inline AvenIoResult aven_io_reader_pop(
        AvenIoReader *reader,
        ByteSlice dest
    ) {
        ByteSlice dest_rem = dest;

        if (reader->index < reader->buffer.len) {
            ByteSlice buffer = slice_list(reader->buffer);
            ByteSlice src_rem = slice_tail(buffer, reader->index);
            ByteSlice fit = slice_head(src_rem, min(src_rem.len, dest_rem.len));
            if (fit.len > 0) {
                slice_copy(dest_rem, fit);
                dest_rem = (ByteSlice)slice_tail(dest_rem, fit.len);
                reader->index += fit.len;
            }
        }

        if (dest_rem.len > 0) {
            assert(reader->index == reader->buffer.len);
            if (dest_rem.len > reader->buffer.cap) {
                AvenIoResult res = reader->read(reader->ctx, dest_rem);
                dest_rem = (ByteSlice)slice_tail(dest_rem, res.payload);
                if (res.error != 0) {
                    return (AvenIoResult){
                        .payload = dest.len - dest_rem.len,
                        .error = res.error,
                    };
                }
            } else {
                ByteSlice buffer_cap = {
                    .ptr = reader->buffer.ptr,
                    .len = reader->buffer.cap,
                };
                AvenIoResult res = reader->read(reader->ctx, buffer_cap);
                reader->index = 0;
                reader->buffer.len = res.payload;

                ByteSlice src_rem = slice_list(reader->buffer);
                ByteSlice fit = slice_head(
                    src_rem,
                    min(src_rem.len, dest_rem.len)
                );
                slice_copy(dest_rem, fit);
                dest_rem = (ByteSlice)slice_tail(dest_rem, fit.len);
                reader->index += fit.len;

                if (res.error != 0 or res.payload == 0) {
                    return (AvenIoResult){
                        .payload = dest.len - dest_rem.len,
                        .error = res.error,
                    };
                }
            }
        }

        return (AvenIoResult){ .payload = dest.len - dest_rem.len };
    }

    static inline AvenIoWriter aven_io_writer_init_fd(AvenIoFd fd) {
        return (AvenIoWriter){
            .write = aven_io_fd_write_stub,
            .ctx = { .fd = fd },
        };
    }

    static inline AvenIoWriter aven_io_writer_init_fd_buffered(
        AvenIoFd fd,
        size_t size,
        AvenArena *arena
    ) {
        return (AvenIoWriter){
            .buffer = aven_arena_create_slice(unsigned char, arena, size),
            .index = 0,
            .ctx = { .fd = fd },
            .write = aven_io_fd_write_stub,
        };
    }

    static inline AvenIoWriter aven_io_writer_init_bytes(ByteSlice bytes) {
        return (AvenIoWriter){
            .buffer = { .ptr = bytes.ptr, .len = bytes.len },
            .write = aven_io_null_stub,
        };
    }

    #if __has_attribute(unused)
        __attribute__((unused))
    #endif
    static AvenIoWriter aven_io_stdout = {
        .write = aven_io_fd_write_stub,
        .ctx = { .fd = 1 },
    };
    #if __has_attribute(unused)
        __attribute__((unused))
    #endif
    static AvenIoWriter aven_io_stderr = {
        .write = aven_io_fd_write_stub,
        .ctx = { .fd = 2 },
    };

    static inline AvenIoWriter aven_io_writer_init_stdout_buffered(
        size_t size,
        AvenArena *arena
    ) {
        return aven_io_writer_init_fd_buffered(1, size, arena);
    }

    static inline AvenIoWriter aven_io_writer_init_stderr_buffered(
        size_t size,
        AvenArena *arena
    ) {
        return aven_io_writer_init_fd_buffered(2, size, arena);
    }

    static int aven_io_writer_flush(AvenIoWriter *writer) {
        ByteSlice written = slice_head(writer->buffer, writer->index);
        if (written.len == 0) {
            return AVEN_IO_ERROR_NONE;
        }
        AvenIoResult res = writer->write(writer->ctx, written);
        if (res.payload == written.len) {
            writer->index = 0;
        } else if (res.payload > 0) {
            ByteSlice dest_rem = slice_tail(written, res.payload);
            if (dest_rem.len > written.len / 2) {
                for (size_t i = 0; i < dest_rem.len; i += 1) {
                    get(writer->buffer, i) = get(dest_rem, i);
                }
            } else {
                ByteSlice buffer_head = slice_head(writer->buffer, dest_rem.len);
                slice_copy(buffer_head, dest_rem);
            }
            writer->index = dest_rem.len;
        }
        if (res.error != 0) {
            return res.error;
        }

        return AVEN_IO_ERROR_NONE;
    }

    static inline void aven_io_writer_clear_buffer(AvenIoWriter *writer) {
        writer->index = 0;
    }

    static inline AvenIoResult aven_io_writer_push(
        AvenIoWriter *writer,
        ByteSlice src
    ) {
        ByteSlice src_rem = src;

        if (writer->index > 0) {
            ByteSlice dest_rem = slice_tail(writer->buffer, writer->index);
            ByteSlice fit = slice_head(src_rem, min(src_rem.len, dest_rem.len));
            if (fit.len > 0) {
                slice_copy(dest_rem, fit);
                src_rem = (ByteSlice)slice_tail(src_rem, fit.len);
                writer->index += fit.len;
            }
        }

        if (writer->index == writer->buffer.len) {
            int error = aven_io_writer_flush(writer);
            if (error != 0) {
                return (AvenIoResult){
                    .payload = src.len - src_rem.len,
                    .error = error,
                };
            }
        }

        if (writer->index == 0 and src_rem.len > writer->buffer.len) {
            AvenIoResult res = writer->write(writer->ctx, src_rem);
            src_rem = (ByteSlice)slice_tail(src_rem, res.payload);
            if (res.error != 0) {
                return (AvenIoResult){
                    .payload = src.len - src_rem.len,
                    .error = res.error,
                };
            }
        } else {
            ByteSlice dest_rem = slice_tail(writer->buffer, writer->index);
            ByteSlice fit = slice_head(src_rem, min(src_rem.len, dest_rem.len));
            if (fit.len > 0) {
                slice_copy(dest_rem, fit);
                src_rem = (ByteSlice)slice_tail(src_rem, fit.len);
                writer->index += fit.len;
            }
        }

        return (AvenIoResult){ .payload = src.len - src_rem.len };
    }

    #define aven_io_writer_printf_ex(w, a, f, ...) \
            aven_io_writer_push_fmt_args_ex( \
                w, \
                aven_str(f), \
                (AvenFmtArgSlice)slice_array((AvenFmtArg[]){ __VA_ARGS__ }), \
                a \
            )
    #define aven_io_printf_ex(a, f, ...) aven_io_writer_printf_ex( \
            &aven_io_stdout, \
            a, \
            f, \
            __VA_ARGS__ \
        )
    #define aven_io_perrf_ex(a, f, ...) aven_io_writer_printf_ex( \
            &aven_io_stderr, \
            a, \
            f, \
            __VA_ARGS__ \
        )
    #define aven_io_writer_printf(w, f, ...) aven_io_writer_push_fmt_args( \
            w, \
            aven_str(f), \
            (AvenFmtArgSlice)slice_array((AvenFmtArg[]){ __VA_ARGS__ }) \
        )
    #define aven_io_printf(f, ...) aven_io_writer_printf( \
            &aven_io_stdout, \
            f, \
            __VA_ARGS__ \
        )
    #define aven_io_perrf(f, ...) aven_io_writer_printf( \
            &aven_io_stderr, \
            f, \
            __VA_ARGS__ \
        )
    #define aven_io_writer_print(w, s) aven_io_writer_push_str(w, aven_str(s))
    #define aven_io_print(s) aven_io_writer_print(&aven_io_stdout, s)
    #define aven_io_perr(s) aven_io_writer_print(&aven_io_stderr, s)

    static inline AvenIoResult aven_io_writer_push_str(
        AvenIoWriter *writer,
        AvenStr str
    ) {
        ByteSlice str_bytes = slice_as_bytes(str);
        return aven_io_writer_push(writer, str_bytes);
    }

    static inline AvenIoResult aven_io_writer_push_fmt_args_ex(
        AvenIoWriter *writer,
        AvenStr fmt,
        AvenFmtArgSlice args,
        AvenArena temp_arena
    ) {
        AvenStr str = aven_fmt_args(fmt, args, &temp_arena);
        ByteSlice str_bytes = slice_as_bytes(str);
        return aven_io_writer_push(writer, str_bytes);
    }

    #define AVEN_IO_PRINT_BUFFER_SIZE (8192)

    static inline AvenIoResult aven_io_writer_push_fmt_args(
        AvenIoWriter *writer,
        AvenStr fmt,
        AvenFmtArgSlice args
    ) {
        char buffer[AVEN_IO_PRINT_BUFFER_SIZE];
        AvenArena arena = aven_arena_init(buffer, sizeof(buffer));

        return aven_io_writer_push_fmt_args_ex(writer, fmt, args, arena);
    }

    typedef Result(ByteSlice, int) AvenIoPopAllResult;

    static inline AvenIoPopAllResult aven_io_reader_pop_all(
        AvenIoReader *reader,
        size_t block_size,
        AvenArena *arena
    ) {
        List(unsigned char) input = aven_arena_create_list(
            unsigned char,
            arena,
            block_size
        );
        for (;;) {
            ByteSlice rem = slice_list_free(input);
            if (rem.len == 0) {
                aven_arena_resize_list(arena, input, input.len + block_size);
                continue;
            }
            AvenIoResult res = aven_io_reader_pop(reader, rem);
            if (res.error != 0) {
                return (AvenIoPopAllResult){ .error = res.error };
            }
            if (res.payload == 0) {
                break;
            }
            input.len += res.payload;
        }
        list_push(input) = 0;
        return (AvenIoPopAllResult){
            .payload = aven_arena_commit_list_to_slice(ByteSlice, arena, input),
        };
    }

    #define aven_io_writer_push_struct(w, s) \
            aven_io_writer_push_struct_internal(w, as_bytes(s))
    #define aven_io_reader_pop_struct(w, s) aven_io_reader_pop_struct_internal( \
            w, \
            as_bytes(s) \
        )

    static inline int aven_io_reader_pop_struct_internal(
        AvenIoReader *reader,
        ByteSlice dest
    ) {
        AvenIoResult res = aven_io_reader_pop(reader, dest);
        if (res.error != 0) {
            return res.error;
        }
        if (res.payload < dest.len) {
            return AVEN_IO_ERROR_NOSPACE;
        }

        return AVEN_IO_ERROR_NONE;
    }

    static inline int aven_io_writer_push_struct_internal(
        AvenIoWriter *writer,
        ByteSlice src
    ) {
        AvenIoResult res = aven_io_writer_push(writer, src);
        if (res.error != 0) {
            return res.error;
        }
        if (res.payload < src.len) {
            return AVEN_IO_ERROR_NOSPACE;
        }

        return AVEN_IO_ERROR_NONE;
    }

    #define aven_io_writer_push_slice(w, s) aven_io_writer_push_slice_internal( \
            w, \
            slice_as_bytes(s), \
            sizeof(*(s).ptr), \
            (s).len \
        )
    #define aven_io_reader_pop_slice(t, r, a) \
            aven_io_reader_pop_slice_internal( \
                r, \
                sizeof(t), \
                aven_arena_alignof(t), \
                a \
            )
    #define aven_io_slice(t, g) { \
            .ptr = (assert(sizeof(t) == (size_t)(g).slice.size), (t *)(g).ptr), \
            .len = (size_t)(g).slice.len, \
        }
    #define aven_io_slice_size(s) ( \
            sizeof(AvenIoSliceHeader) + (s).len * sizeof(*(s).ptr) \
        )
    #define AVEN_IO_SLICE_FINGERPRINT ((uint64_t)0x571ce04eade2)

    typedef struct {
        uint64_t size;
        uint64_t len;
    } AvenIoSlice;

    typedef struct {
        void *ptr;
        AvenIoSlice slice;
    } AvenIoSliceGeneric;

    typedef struct {
        uint64_t fp;
        AvenIoSlice slice;
    } AvenIoSliceHeader;

    typedef Result(AvenIoSliceGeneric, int) AvenIoSliceResult;

    static inline AvenIoSliceResult aven_io_reader_pop_slice_internal(
        AvenIoReader *reader,
        size_t size,
        size_t align,
        AvenArena *arena
    ) {
        AvenIoSliceHeader header = { 0 };
        int hd_error = aven_io_reader_pop_struct(reader, &header);
        if (hd_error != 0) {
            return (AvenIoSliceResult){ .error = hd_error };
        }
        if (header.fp != AVEN_IO_SLICE_FINGERPRINT) {
            return (AvenIoSliceResult){ .error = AVEN_IO_ERROR_FINGERPRINT };
        }
        if ((size_t)header.slice.size != size) {
            return (AvenIoSliceResult){ .error = AVEN_IO_ERROR_MISMATCH };
        }

        AvenArena temp_arena = *arena;
        ByteSlice slice_bytes = {
            .ptr = aven_arena_alloc(
                &temp_arena,
                header.slice.len,
                align,
                (size_t)header.slice.size
            ),
            .len = (size_t)(header.slice.len * header.slice.size),
        };
        AvenIoResult sl_res = aven_io_reader_pop(reader, slice_bytes);
        if (sl_res.error != 0) {
            return (AvenIoSliceResult){ .error = sl_res.error };
        }
        if (sl_res.payload < slice_bytes.len) {
            return (AvenIoSliceResult){ .error = AVEN_IO_ERROR_NOSPACE };
        }

        *arena = temp_arena;

        return (AvenIoSliceResult){
            .payload = { .ptr = slice_bytes.ptr, .slice = header.slice },
        };
    }

    static inline int aven_io_writer_push_slice_internal(
        AvenIoWriter *writer,
        ByteSlice bytes,
        size_t size,
        size_t len
    ) {
        AvenIoSliceHeader header = {
            .fp = AVEN_IO_SLICE_FINGERPRINT,
            .slice = { .size = size, .len = len },
        };
        int hd_error = aven_io_writer_push_struct(writer, &header);
        if (hd_error != 0) {
            return hd_error;
        }

        AvenIoResult sl_res = aven_io_writer_push(writer, bytes);
        if (sl_res.error != 0) {
            return sl_res.error;
        }
        if (sl_res.payload < bytes.len) {
            return AVEN_IO_ERROR_NOSPACE;
        }
        return AVEN_IO_ERROR_NONE;
    }

    #define aven_io_writer_push_list(w, l) aven_io_writer_push_list_internal( \
            w, \
            list_as_bytes(l), \
            sizeof(*(l).ptr), \
            (l).len, \
            (l).cap \
        )
    #define aven_io_reader_pop_list(t, r, a) aven_io_reader_pop_list_internal( \
            r, \
            sizeof(t), \
            aven_arena_alignof(t), \
            a \
        )
    #define aven_io_list(t, g) { \
            .ptr = (assert(sizeof(t) == (size_t)(g).list.size), (t *)(g).ptr), \
            .cap = (size_t)(g).list.cap, \
            .len = (size_t)(g).list.len, \
        }
    #define aven_io_list_size(s) ( \
            sizeof(AvenIoListHeader) + (s).len * sizeof(*(s).ptr) \
        )
    #define AVEN_IO_LIST_FINGERPRINT ((uint64_t)0x715704eade2)

    typedef struct {
        uint64_t size;
        uint64_t len;
        uint64_t cap;
    } AvenIoList;

    typedef struct {
        void *ptr;
        AvenIoList list;
    } AvenIoListGeneric;

    typedef struct {
        uint64_t fp;
        AvenIoList list;
    } AvenIoListHeader;

    typedef Result(AvenIoListGeneric, int) AvenIoListResult;

    static inline AvenIoListResult aven_io_reader_pop_list_internal(
        AvenIoReader *reader,
        size_t size,
        size_t align,
        AvenArena *arena
    ) {
        AvenIoListHeader header = { 0 };
        int hd_error = aven_io_reader_pop_struct(reader, &header);
        if (hd_error != 0) {
            return (AvenIoListResult){ .error = hd_error };
        }
        if (header.fp != AVEN_IO_LIST_FINGERPRINT) {
            return (AvenIoListResult){ .error = AVEN_IO_ERROR_FINGERPRINT };
        }
        if ((size_t)header.list.size != size) {
            return (AvenIoListResult){ .error = AVEN_IO_ERROR_MISMATCH };
        }
        if (header.list.len > header.list.cap) {
            return (AvenIoListResult){ .error = AVEN_IO_ERROR_MISMATCH };
        }

        AvenArena temp_arena = *arena;

        ByteSlice list_bytes = {
            .ptr = aven_arena_alloc(
                arena,
                header.list.cap,
                align,
                (size_t)header.list.size
            ),
            .len = (size_t)(header.list.cap * header.list.size),
        };
        ByteSlice used_list_bytes = slice_head(
            list_bytes,
            header.list.len * header.list.size
        );
        AvenIoResult sl_res = aven_io_reader_pop(reader, used_list_bytes);
        if (sl_res.error != 0) {
            return (AvenIoListResult){ .error = sl_res.error };
        }
        if (sl_res.payload < used_list_bytes.len) {
            return (AvenIoListResult){ .error = AVEN_IO_ERROR_NOSPACE };
        }

        *arena = temp_arena;

        return (AvenIoListResult){
            .payload = { .ptr = list_bytes.ptr, .list = header.list },
        };
    }

    static inline int aven_io_writer_push_list_internal(
        AvenIoWriter *writer,
        ByteSlice bytes,
        size_t size,
        size_t len,
        size_t cap
    ) {
        AvenIoListHeader header = {
            .fp = AVEN_IO_LIST_FINGERPRINT,
            .list = { .size = size, .len = len, .cap = cap },
        };
        int hd_error = aven_io_writer_push_struct(writer, &header);
        if (hd_error != 0) {
            return hd_error;
        }

        AvenIoResult lt_res = aven_io_writer_push(writer, bytes);
        if (lt_res.error != 0) {
            return lt_res.error;
        }
        if (lt_res.payload < bytes.len) {
            return AVEN_IO_ERROR_NOSPACE;
        }

        return AVEN_IO_ERROR_NONE;
    }

    #define aven_io_writer_push_queue(w, q) aven_io_writer_push_queue_internal( \
            w, \
            queue_front_as_bytes(q), \
            queue_back_as_bytes(q), \
            sizeof(*(q).ptr), \
            (q).used, \
            (q).cap \
        )
    #define aven_io_reader_pop_queue(t, r, a) \
            aven_io_reader_pop_queue_internal( \
                r, \
                sizeof(t), \
                aven_arena_alignof(t), \
                a \
            )
    #define aven_io_queue(t, g) { \
            .ptr = (assert(sizeof(t) == (size_t)(g).queue.size), (t *)(g).ptr), \
            .cap = (size_t)(g).queue.cap, \
            .front = 0, \
            .back = (size_t)(g).queue.used, \
            .used = (size_t)(g).queue.used, \
        }
    #define aven_io_queue_size(s) ( \
            sizeof(AvenIoQueueHeader) + (s).used * sizeof(*(s).ptr) \
        )
    #define AVEN_IO_QUEUE_FINGERPRINT ((uint64_t)0x98e8e04eade2)

    typedef struct {
        uint64_t size;
        uint64_t used;
        uint64_t cap;
    } AvenIoQueue;

    typedef struct {
        void *ptr;
        AvenIoQueue queue;
    } AvenIoQueueGeneric;

    typedef struct {
        uint64_t fp;
        AvenIoQueue queue;
    } AvenIoQueueHeader;

    typedef Result(AvenIoQueueGeneric, int) AvenIoQueueResult;

    static inline AvenIoQueueResult aven_io_reader_pop_queue_internal(
        AvenIoReader *reader,
        size_t size,
        size_t align,
        AvenArena *arena
    ) {
        AvenIoQueueHeader header = { 0 };
        int hd_error = aven_io_reader_pop_struct(reader, &header);
        if (hd_error != 0) {
            return (AvenIoQueueResult){ .error = hd_error };
        }
        if (header.fp != AVEN_IO_QUEUE_FINGERPRINT) {
            return (AvenIoQueueResult){ .error = AVEN_IO_ERROR_FINGERPRINT };
        }
        if ((size_t)header.queue.size != size) {
            return (AvenIoQueueResult){ .error = AVEN_IO_ERROR_MISMATCH };
        }
        if (header.queue.used > header.queue.cap) {
            return (AvenIoQueueResult){ .error = AVEN_IO_ERROR_MISMATCH };
        }

        AvenArena temp_arena = *arena;

        ByteSlice queue_bytes = {
            .ptr = aven_arena_alloc(
                arena,
                header.queue.cap,
                align,
                (size_t)header.queue.size
            ),
            .len = (size_t)(header.queue.cap * header.queue.size),
        };
        ByteSlice used_queue_bytes = slice_head(
            queue_bytes,
            header.queue.used * header.queue.size
        );
        AvenIoResult sl_res = aven_io_reader_pop(reader, used_queue_bytes);
        if (sl_res.error != 0) {
            return (AvenIoQueueResult){ .error = sl_res.error };
        }
        if (sl_res.payload < used_queue_bytes.len) {
            return (AvenIoQueueResult){ .error = AVEN_IO_ERROR_NOSPACE };
        }

        *arena = temp_arena;

        return (AvenIoQueueResult){
            .payload = { .ptr = queue_bytes.ptr, .queue = header.queue },
        };
    }

    static inline int aven_io_writer_push_queue_internal(
        AvenIoWriter *writer,
        ByteSlice front_bytes,
        ByteSlice back_bytes,
        size_t size,
        size_t used,
        size_t cap
    ) {
        AvenIoQueueHeader header = {
            .fp = AVEN_IO_QUEUE_FINGERPRINT,
            .queue = { .size = size, .used = used, .cap = cap },
        };
        int hd_error = aven_io_writer_push_struct(writer, &header);
        if (hd_error != 0) {
            return hd_error;
        }

        AvenIoResult ft_res = aven_io_writer_push(writer, front_bytes);
        if (ft_res.error != 0) {
            return ft_res.error;
        }
        if (ft_res.payload < front_bytes.len) {
            return AVEN_IO_ERROR_NOSPACE;
        }
        AvenIoResult bk_res = aven_io_writer_push(writer, back_bytes);
        if (bk_res.error != 0) {
            return bk_res.error;
        }
        if (bk_res.payload < back_bytes.len) {
            return AVEN_IO_ERROR_NOSPACE;
        }

        return AVEN_IO_ERROR_NONE;
    }

    #define aven_io_writer_push_pool(w, p) aven_io_writer_push_pool_internal( \
            w, \
            pool_as_bytes(p), \
            sizeof(*(p).ptr), \
            (p).free, \
            (p).used, \
            (p).len, \
            (p).cap \
        )
    #define aven_io_reader_pop_pool(t, r, a) aven_io_reader_pop_pool_internal( \
            r, \
            sizeof(PoolEntry(t)), \
            aven_arena_alignof(PoolEntry(t)), \
            a \
        )
    #define aven_io_pool(t, g) { \
            .ptr = ( \
                assert(sizeof(PoolEntry(t)) == (size_t)(g).pool.size), \
                (void *)(g).ptr \
            ), \
            .free = (size_t)(g).pool.free, \
            .used = (size_t)(g).pool.used, \
            .len = (size_t)(g).pool.len, \
            .cap = (size_t)(g).pool.cap, \
        }
    #define aven_io_pool_size(s) ( \
            sizeof(AvenIoPoolHeader) + (s).len * sizeof(*(s).ptr) \
        )
    #define AVEN_IO_POOL_FINGERPRINT ((uint64_t)0x700704eade2)

    typedef struct {
        uint64_t size;
        uint64_t free;
        uint64_t used;
        uint64_t len;
        uint64_t cap;
    } AvenIoPool;

    typedef struct {
        void *ptr;
        AvenIoPool pool;
    } AvenIoPoolGeneric;

    typedef struct {
        uint64_t fp;
        AvenIoPool pool;
    } AvenIoPoolHeader;

    typedef Result(AvenIoPoolGeneric, int) AvenIoPoolResult;

    static inline AvenIoPoolResult aven_io_reader_pop_pool_internal(
        AvenIoReader *reader,
        size_t size,
        size_t align,
        AvenArena *arena
    ) {
        AvenIoPoolHeader header = { 0 };
        int hd_error = aven_io_reader_pop_struct(reader, &header);
        if (hd_error != 0) {
            return (AvenIoPoolResult){ .error = hd_error };
        }
        if (header.fp != AVEN_IO_POOL_FINGERPRINT) {
            return (AvenIoPoolResult){ .error = AVEN_IO_ERROR_FINGERPRINT };
        }
        if ((size_t)header.pool.size != size) {
            return (AvenIoPoolResult){ .error = AVEN_IO_ERROR_MISMATCH };
        }
        if (header.pool.len > header.pool.cap) {
            return (AvenIoPoolResult){ .error = AVEN_IO_ERROR_MISMATCH };
        }
        if (header.pool.free > header.pool.len) {
            return (AvenIoPoolResult){ .error = AVEN_IO_ERROR_MISMATCH };
        }
        if (header.pool.used > header.pool.len) {
            return (AvenIoPoolResult){ .error = AVEN_IO_ERROR_MISMATCH };
        }
        if (header.pool.used < header.pool.len and header.pool.free == 0) {
            return (AvenIoPoolResult){ .error = AVEN_IO_ERROR_MISMATCH };
        }
        if (header.pool.used == header.pool.len and header.pool.free != 0) {
            return (AvenIoPoolResult){ .error = AVEN_IO_ERROR_MISMATCH };
        }

        AvenArena temp_arena = *arena;

        ByteSlice pool_bytes = {
            .ptr = aven_arena_alloc(
                arena,
                header.pool.cap,
                align,
                (size_t)header.pool.size
            ),
            .len = (size_t)(header.pool.cap * header.pool.size),
        };
        ByteSlice used_pool_bytes = slice_head(
            pool_bytes,
            header.pool.len * header.pool.size
        );
        AvenIoResult sl_res = aven_io_reader_pop(reader, used_pool_bytes);
        if (sl_res.error != 0) {
            return (AvenIoPoolResult){ .error = sl_res.error };
        }
        if (sl_res.payload < used_pool_bytes.len) {
            return (AvenIoPoolResult){ .error = AVEN_IO_ERROR_NOSPACE };
        }

        *arena = temp_arena;

        return (AvenIoPoolResult){
            .payload = { .ptr = pool_bytes.ptr, .pool = header.pool },
        };
    }

    static inline int aven_io_writer_push_pool_internal(
        AvenIoWriter *writer,
        ByteSlice bytes,
        size_t size,
        size_t free,
        size_t used,
        size_t len,
        size_t cap
    ) {
        AvenIoPoolHeader header = {
            .fp = AVEN_IO_POOL_FINGERPRINT,
            .pool = {
                .size = size,
                .free = free,
                .used = used,
                .len = len,
                .cap = cap,
            },
        };
        int hd_error = aven_io_writer_push_struct(writer, &header);
        if (hd_error != 0) {
            return hd_error;
        }

        AvenIoResult pl_res = aven_io_writer_push(writer, bytes);
        if (pl_res.error != 0) {
            return pl_res.error;
        }
        if (pl_res.payload < bytes.len) {
            return AVEN_IO_ERROR_NOSPACE;
        }

        return AVEN_IO_ERROR_NONE;
    }

    #ifdef AVEN_IMPLEMENTATION

        #include <errno.h>

        #ifdef _WIN32
            #if defined(_MSC_VER) and defined(__clang__)
                #pragma clang diagnostic push
                #pragma clang diagnostic ignored "-Wdeprecated-declarations"
            #endif
            #include <fcntl.h>
            #include <io.h>
            #include <sys/stat.h>
        #else
            #if defined(__linux__) and defined(NOLIBC)
                #include <sys.h>
            #else
                #include <fcntl.h>
                #include <sys/stat.h>
            #endif
            #include <unistd.h>
        #endif

        AVEN_FN AvenIoOpenResult aven_io_open(
            AvenStr file_path,
            AvenIoOpenMode mode,
            AvenArena temp_arena
        ) {
        #ifdef _WIN32
            unsigned int oflag = 0;
            switch (mode) {
                case AVEN_IO_OPEN_MODE_WRITE:
                    oflag = _O_CREAT | _O_TRUNC | _O_WRONLY | _O_BINARY;
                    break;
                case AVEN_IO_OPEN_MODE_APPEND:
                    oflag = _O_CREAT | _O_APPEND | _O_WRONLY | _O_BINARY;
                    break;
                case AVEN_IO_OPEN_MODE_READ:
                    oflag = _O_RDONLY | _O_BINARY;
                    break;
                default:
                    assert(false);
                    break;
            }
            AvenIoFd fd = _open(
                aven_str_to_cstr(file_path, &temp_arena),
                oflag,
                _S_IREAD | _S_IWRITE
            );
            if (fd < 0) {
                switch (errno) {
                    case EACCES:
                        return (AvenIoOpenResult){
                            .error = AVEN_IO_OPEN_ERROR_ACCESS,
                        };
                    case ENOENT:
                        return (AvenIoOpenResult){
                            .error = AVEN_IO_OPEN_ERROR_BADPATH,
                        };
                    default:
                        return (AvenIoOpenResult){
                            .error = AVEN_IO_OPEN_ERROR_OTHER,
                        };
                }
            }

            return (AvenIoOpenResult){ .payload = fd };
        #else
            AvenIoFd fd = -1;
            int oflag = 0;
            switch (mode) {
                case AVEN_IO_OPEN_MODE_WRITE:
                    oflag = O_CREAT | O_TRUNC | O_WRONLY;
                    break;
                case AVEN_IO_OPEN_MODE_APPEND:
                    oflag = O_CREAT | O_APPEND | O_WRONLY;
                    break;
                case AVEN_IO_OPEN_MODE_READ:
                    oflag = O_RDONLY;
                    break;
                default:
                    assert(false);
                    break;
            }
            do {
                fd = open(
                    aven_str_to_cstr(file_path, &temp_arena),
                    oflag,
                    S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH
                );
            } while (fd < 0 and errno == EINTR);
            if (fd < 0) {
                switch (errno) {
                    case EACCES:
                        return (AvenIoOpenResult){
                            .error = AVEN_IO_OPEN_ERROR_ACCESS,
                        };
                    case ENOENT:
                    case ENOTDIR:
                    case EISDIR:
                        return (AvenIoOpenResult){
                            .error = AVEN_IO_OPEN_ERROR_BADPATH,
                        };
                    default:
                        return (AvenIoOpenResult){
                            .error = AVEN_IO_OPEN_ERROR_OTHER,
                        };
                }
            }

            return (AvenIoOpenResult){ .payload = fd };
        #endif
        }

        AVEN_FN AvenIoReadResult aven_io_read(AvenIoFd fd, ByteSlice dest) {
        #ifdef _WIN32
            ByteSlice dest_rem = dest;
            while (dest_rem.len > 0) {
                int result = _read(fd, dest_rem.ptr, (unsigned int)dest_rem.len);
                if (result > 0) {
                    dest_rem = (ByteSlice)slice_tail(dest_rem, (size_t)result);
                } else if (result == 0) {
                    break;
                } else {
                    switch (errno) {
                        case EBADF:
                            return (AvenIoReadResult){
                                .error = AVEN_IO_READ_ERROR_BADF,
                                .payload = dest.len - dest_rem.len,
                            };
                        default:
                            return (AvenIoReadResult){
                                .error = AVEN_IO_READ_ERROR_OTHER,
                                .payload = dest.len - dest_rem.len,
                            };
                    }
                }
            }

            return (AvenIoReadResult){ .payload = dest.len - dest_rem.len };
        #else
            ByteSlice dest_rem = dest;
            while (dest_rem.len > 0) {
                ssize_t result = read(fd, dest_rem.ptr, dest_rem.len);
                if (result > 0) {
                    dest_rem = (ByteSlice)slice_tail(dest_rem, (size_t)result);
                } else if (result == 0) {
                    break;
                } else if (errno != EINTR) {
                    switch (errno) {
                        case EBADF:
                            return (AvenIoReadResult){
                                .error = AVEN_IO_READ_ERROR_BADF,
                                .payload = dest.len - dest_rem.len,
                            };
                        default:
                            return (AvenIoReadResult){
                                .error = AVEN_IO_READ_ERROR_OTHER,
                                .payload = dest.len - dest_rem.len,
                            };
                    }
                }
            }

            return (AvenIoReadResult){ .payload = dest.len - dest_rem.len };
        #endif
        }

        AVEN_FN AvenIoWriteResult aven_io_write(AvenIoFd fd, ByteSlice src) {
        #ifdef _WIN32
            ByteSlice src_rem = src;
            while (src_rem.len > 0) {
                int result = _write(fd, src_rem.ptr, (unsigned int)src_rem.len);
                if (result > 0) {
                    src_rem = (ByteSlice)slice_tail(src_rem, (size_t)result);
                } else if (result == 0) {
                    break;
                } else {
                    switch (errno) {
                        case EBADF:
                            return (AvenIoWriteResult){
                                .error = AVEN_IO_WRITE_ERROR_BADF,
                                .payload = src.len - src_rem.len,
                            };
                        default:
                            return (AvenIoWriteResult){
                                .error = AVEN_IO_WRITE_ERROR_OTHER,
                                .payload = src.len - src_rem.len,
                            };
                    }
                }
            }

            return (AvenIoWriteResult){ .payload = src.len - src_rem.len };
        #else
            ByteSlice src_rem = src;
            while (src_rem.len > 0) {
                ssize_t result = write(fd, src_rem.ptr, src_rem.len);
                if (result > 0) {
                    src_rem = (ByteSlice)slice_tail(src_rem, (size_t)result);
                } else if (result == 0) {
                    break;
                } else if (errno != EINTR) {
                    switch (errno) {
                        case EBADF:
                            return (AvenIoWriteResult){
                                .error = AVEN_IO_WRITE_ERROR_BADF,
                                .payload = src.len - src_rem.len,
                            };
                        default:
                            return (AvenIoWriteResult){
                                .error = AVEN_IO_WRITE_ERROR_OTHER,
                                .payload = src.len - src_rem.len,
                            };
                    }
                }
            }

            return (AvenIoWriteResult){ .payload = src.len - src_rem.len };
        #endif
        }

        AVEN_FN void aven_io_close(AvenIoFd fd) {
        #ifdef _WIN32
            _close(fd);
        #else
            int error = 0;
            do {
                error = close(fd);
            } while (error != 0 and errno == EINTR);
        #endif
        }
        #if defined(_WIN32) and defined(_MSC_VER) and defined(__clang__)
            #pragma clang diagnostic pop
        #endif

    #endif
    // AVEN_IMPLEMENTATION

#endif
// AVEN_IO_H
