#ifndef AVEN_IO_H
#define AVEN_IO_H

#include "../aven.h"
#include "arena.h"
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

typedef AvenIoResult (AvenIoFn)(void *ctx, ByteSlice bytes);

typedef struct {
    List(uint8_t) buffer;
    size_t index;
    AvenIoFn *read;
    void *ctx;
} AvenIoReader;

typedef struct {
    ByteSlice buffer;
    size_t index;
    AvenIoFn *write;
    void *ctx;
} AvenIoWriter;

static AvenIoResult aven_io_null_stub(void *ctx, ByteSlice dest) {
    (void)ctx;
    (void)dest;
    return (AvenIoResult){ 0 };
}

static AvenIoResult aven_io_fd_read_stub(void *ctx, ByteSlice dest) {
    AvenIoReadResult res = aven_io_read((AvenIoFd)((uintptr_t)ctx), dest);
    return (AvenIoResult){ .payload = res.payload, .error = (int)res.error };
}

static AvenIoResult aven_io_fd_write_stub(void *ctx, ByteSlice dest) {
    AvenIoWriteResult res = aven_io_write((AvenIoFd)((uintptr_t)ctx), dest);
    return (AvenIoResult){ .payload = res.payload, .error = (int)res.error };
}

static inline AvenIoReader aven_io_reader_init_fd(AvenIoFd fd) {
    return (AvenIoReader){
        .ctx = (void *)((uintptr_t)fd),
        .read = aven_io_fd_read_stub,
    };
}

static inline AvenIoReader aven_io_reader_init_fd_buffered(
    AvenIoFd fd,
    size_t size,
    AvenArena *arena
) {
    return (AvenIoReader){
        .buffer = aven_arena_create_list(unsigned char, arena, size),
        .ctx = (void *)((uintptr_t)fd),
        .read = aven_io_fd_read_stub,
    };
}

static inline AvenIoReader aven_io_reader_init_bytes(ByteSlice bytes) {
    return (AvenIoReader){
        .buffer = { .ptr = bytes.ptr, .len = bytes.len, .cap = bytes.len },
        .ctx = NULL,
        .read = aven_io_null_stub,
    };
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
                    .error = res.error,
                    .payload = dest.len - dest_rem.len,
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
            ByteSlice fit = slice_head(src_rem, min(src_rem.len, dest_rem.len));
            slice_copy(dest_rem, fit);
            dest_rem = (ByteSlice)slice_tail(dest_rem, fit.len);
            reader->index += fit.len;

            if (res.error != 0 or res.payload == 0) {
                return (AvenIoResult){
                    .error = res.error,
                    .payload = dest.len - dest_rem.len,
                };
            }
        }
    }

    return (AvenIoResult){ .payload = dest.len - dest_rem.len };
}

static inline AvenIoWriter aven_io_writer_init_fd(AvenIoFd fd) {
    return (AvenIoWriter){
        .ctx = (void *)((uintptr_t)fd),
        .write = aven_io_fd_write_stub,
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
        .ctx = (void *)((uintptr_t)fd),
        .write = aven_io_fd_write_stub,
    };
}

static inline AvenIoWriter aven_io_writer_init_bytes(ByteSlice bytes) {
    return (AvenIoWriter){
        .buffer = { .ptr = bytes.ptr, .len = bytes.len },
        .ctx = NULL,
        .write = aven_io_null_stub,
    };
}

static int aven_io_writer_flush(AvenIoWriter *writer) {
    AvenIoResult res = writer->write(writer->ctx, writer->buffer);
    if (res.payload == writer->buffer.len) {
        writer->index = 0;
    } else if (res.payload > 0) {
        ByteSlice dest_rem = slice_tail(writer->buffer, res.payload);
        if (dest_rem.len > writer->buffer.len / 2) {
            for (size_t i = 0; i < dest_rem.len; i += 1) {
                get(writer->buffer, i) = get(dest_rem, i);
            }
        } else {
            ByteSlice buffer_head = slice_head(
                writer->buffer,
                dest_rem.len
            );
            slice_copy(buffer_head, dest_rem);
        }
        writer->index = dest_rem.len;
    }
    if (res.error != 0) {
        return res.error;
    }

    return 0;
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
                .error = error,
                .payload = src.len - src_rem.len,
            };
        }
    }

    if (writer->index == 0 and src_rem.len > writer->buffer.len) {
        AvenIoResult res = writer->write(writer->ctx, src_rem);
        src_rem = (ByteSlice)slice_tail(src_rem, res.payload);
        if (res.error != 0) {
            return (AvenIoResult){
                .error = res.error,
                .payload = src.len - src_rem.len,
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
        .ptr = (assert(sizeof(t) == (g).slice.size), (t *)(g).ptr), \
        .len = (g).slice.count, \
    }
#define aven_io_slice_size(s) ( \
        sizeof(AvenIoSliceHeader) + \
        (s).len * sizeof(*(s).ptr) \
    )
#define AVEN_IO_SLICE_FINGERPRINT (0x571ce4ed)

typedef struct {
    size_t size;
    size_t count;
} AvenIoSlice;

typedef struct {
    void *ptr;
    AvenIoSlice slice;
} AvenIoSliceGeneric;

typedef struct {
    size_t fp;
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
    ByteSlice header_bytes = as_bytes(&header);

    AvenIoResult hd_res = aven_io_reader_pop(reader, header_bytes);
    if (hd_res.error != 0) {
        return (AvenIoSliceResult){ .error = hd_res.error };
    }
    if (hd_res.payload < header_bytes.len) {
        return (AvenIoSliceResult){ .error = AVEN_IO_ERROR_NOSPACE };
    }
    if (header.fp != AVEN_IO_SLICE_FINGERPRINT) {
        return (AvenIoSliceResult){ .error = AVEN_IO_ERROR_FINGERPRINT };
    }
    if (header.slice.size != size) {
        return (AvenIoSliceResult){ .error = AVEN_IO_ERROR_MISMATCH };
    }

    ByteSlice slice_bytes = {
        .ptr = aven_arena_alloc(
            arena,
            header.slice.count,
            align,
            header.slice.size
        ),
        .len = header.slice.count * header.slice.size,
    };
    AvenIoResult sl_res = aven_io_reader_pop(reader, slice_bytes);
    if (sl_res.error != 0) {
        return (AvenIoSliceResult){ .error = sl_res.error };
    }
    if (sl_res.payload < slice_bytes.len) {
        return (AvenIoSliceResult){ .error = AVEN_IO_ERROR_NOSPACE };
    }

    return (AvenIoSliceResult){
        .payload = {
            .ptr = slice_bytes.ptr,
            .slice = header.slice,
        },
    };
}

static inline int aven_io_writer_push_slice_internal(
    AvenIoWriter *writer,
    ByteSlice bytes,
    size_t size,
    size_t count
) {
    AvenIoSliceHeader header = {
        .fp = AVEN_IO_SLICE_FINGERPRINT,
        .slice = { .size = size, .count = count },
    };
    ByteSlice header_bytes = as_bytes(&header);

    AvenIoResult hd_res = aven_io_writer_push(writer, header_bytes);
    if (hd_res.error != 0) {
        return hd_res.error;
    }
    if (hd_res.payload < header_bytes.len) {
        return AVEN_IO_ERROR_NOSPACE;
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
    #include <fcntl.h>
    #include <sys/stat.h>
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
                return (AvenIoOpenResult){ .error = AVEN_IO_OPEN_ERROR_ACCESS };
            case ENOENT:
                return (AvenIoOpenResult){
                    .error = AVEN_IO_OPEN_ERROR_BADPATH,
                };
            default:
                return (AvenIoOpenResult){ .error = AVEN_IO_OPEN_ERROR_OTHER };
        }
    }

    return (AvenIoOpenResult){ .payload = fd };
#else
    AvenIoFd fd = -1;
    unsigned int oflag = 0;
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
                return (AvenIoOpenResult){ .error = AVEN_IO_OPEN_ERROR_ACCESS };
            case ENOENT:
            case ENOTDIR:
            case EISDIR:
                return (AvenIoOpenResult){
                    .error = AVEN_IO_OPEN_ERROR_BADPATH,
                };
            default:
                return (AvenIoOpenResult){ .error = AVEN_IO_OPEN_ERROR_OTHER };
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
        if (result >= 0) {
            dest_rem = (ByteSlice)slice_tail(dest_rem, (size_t)result);
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
        if (result >= 0) {
            src_rem = (ByteSlice)slice_tail(src_rem, (size_t)result);
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
    } while (error != 0 and errno != EINTR);
#endif
}

#if defined(_WIN32) and defined(_MSC_VER) and defined(__clang__)
    #pragma clang diagnostic pop
#endif

#endif // AVEN_IMPLEMENTATION

#endif // AVEN_IO_H
