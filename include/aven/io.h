#ifndef AVEN_IO_H
#define AVEN_IO_H

#include "../aven.h"
#include "arena.h"
#include "str.h"

typedef enum {
    AVEN_IO_MODE_BIN = 0,
    AVEN_IO_MODE_TEXT,
} AvenIoMode;

typedef enum {
    AVEN_IO_READ_ERROR_NONE = 0,
    AVEN_IO_READ_ERROR_OPEN,
    AVEN_IO_READ_ERROR_SEEK,
    AVEN_IO_READ_ERROR_TELL,
    AVEN_IO_READ_ERROR_READ,
} AvenIoReadError;
typedef Result(ByteSlice, AvenIoReadError) AvenIoReadResult;

AVEN_FN AvenIoReadResult aven_io_read(
    AvenStr file_path,
    AvenIoMode mode,
    AvenArena *arena
);

typedef enum {
    AVEN_IO_WRITE_ERROR_NONE = 0,
    AVEN_IO_WRITE_ERROR_OPEN,
    AVEN_IO_WRITE_ERROR_WRITE,
} AvenIoWriteError;

AVEN_FN AvenIoWriteError aven_io_write(
    AvenStr file_path,
    AvenIoMode mode,
    ByteSlice bytes,
    AvenArena temp_arena
);

#ifdef AVEN_IMPLEMENTATION

#include <stdio.h>

AVEN_FN AvenIoReadResult aven_io_read(
    AvenStr file_path,
    AvenIoMode mode,
    AvenArena *arena
) {
    AvenArena cstr_arena = *arena;
#ifdef _WIN32
    FILE *file = NULL;
    const char *mstr = mode == (AVEN_IO_MODE_BIN) ? "rb" : "r";
    if (fopen_s(&file, aven_str_to_cstr(file_path, &cstr_arena), mstr) != 0) {
        file = NULL;
    }
#else
    (void)mode;
    FILE *file = fopen(aven_str_to_cstr(file_path, &cstr_arena), "r");
#endif
    if (file == NULL) {
        return (AvenIoReadResult){ .error = AVEN_IO_READ_ERROR_OPEN };
    }

    int error = fseek(file, 0L, SEEK_END);
    if (error != 0) {
        fclose(file);
        return (AvenIoReadResult){ .error = AVEN_IO_READ_ERROR_SEEK };
    }

    long len = ftell(file);
    if (len < 0) {
        fclose(file);
        return (AvenIoReadResult){ .error = AVEN_IO_READ_ERROR_TELL };
    }

    if (len == 0) {
        fclose(file);
        return (AvenIoReadResult){ 0 };
    }

    rewind(file);

    AvenArena temp_arena = *arena;

    ByteSlice bytes = { .len = (size_t)len };
    bytes.ptr = aven_arena_alloc(
        &temp_arena,
        ((bytes.len - 1) / AVEN_ARENA_BIGGEST_ALIGNMENT) + 1,
        AVEN_ARENA_BIGGEST_ALIGNMENT,
        AVEN_ARENA_BIGGEST_ALIGNMENT
    );
    size_t bytes_read = fread((char *)bytes.ptr, 1, bytes.len, file);
    fclose(file);

#ifdef _WIN32
    if (mode == AVEN_IO_MODE_BIN) {
        if (bytes_read != bytes.len) {
            return (AvenIoReadResult){ .error = AVEN_IO_READ_ERROR_READ };
        }
    } else {
        // Text on Windows translates \r\n -> \n, so bytes_read <= bytes.len
        if (bytes_read == 0) {
            return (AvenIoReadResult){ .error = AVEN_IO_READ_ERROR_READ };
        }
        bytes.len = bytes_read;
    }
#else
    if (bytes_read != bytes.len) {
        return (AvenIoReadResult){ .error = AVEN_IO_READ_ERROR_READ };
    }
#endif

    *arena = temp_arena;

    return (AvenIoReadResult){ .payload = bytes };
}

AVEN_FN AvenIoWriteError aven_io_write(
    AvenStr file_path,
    AvenIoMode mode,
    ByteSlice bytes,
    AvenArena temp_arena
) {
#ifdef _WIN32
    FILE *file = NULL;
    const char *mstr = mode == (AVEN_IO_MODE_BIN) ? "wb" : "w";
    if (fopen_s(&file, aven_str_to_cstr(file_path, &temp_arena), mstr) != 0) {
        file = NULL;
    }
#else
    (void)mode;
    FILE *file = fopen(aven_str_to_cstr(file_path, &temp_arena), "w");
#endif
    if (file == NULL) {
        return AVEN_IO_WRITE_ERROR_OPEN;
    }

    size_t bytes_written = fwrite(bytes.ptr, 1, bytes.len, file);
    fclose(file);

#ifdef _WIN32
    if (mode == AVEN_IO_MODE_BIN) {
        if (bytes_written != bytes.len) {
            return AVEN_IO_WRITE_ERROR_WRITE;
        }
    } else {
        // Text on Windows translates \r\n -> \n, so bytes_read <= bytes.len
        if (bytes.len > 0 and bytes_written == 0) {
            return AVEN_IO_WRITE_ERROR_WRITE;
        }
    }
#else
    if (bytes_written != bytes.len) {
        return AVEN_IO_WRITE_ERROR_WRITE;
    }
#endif

    return AVEN_IO_WRITE_ERROR_NONE;
}

#endif // AVEN_IMPLEMENTATION

#endif // AVEN_IO_H
