#ifndef AVEN_PATH_H
#define AVEN_PATH_H

#include "../aven.h"
#include "arena.h"
#include "str.h"

#define AVEN_PATH_MAX_ARGS 64
#define AVEN_PATH_MAX_LEN 4096

#ifdef _WIN32
    #define AVEN_PATH_SEP '\\'
#else
    #define AVEN_PATH_SEP '/'
#endif

#define aven_path(a, ...) aven_path_internal( \
        a, \
        __VA_ARGS__, \
        (AvenStr){ 0 } \
    )

AVEN_FN AvenStr aven_path_internal(AvenArena *arena, AvenStr part_str, ...);
AVEN_FN AvenStr aven_path_rel_dir(AvenStr path, AvenArena *arena);
AVEN_FN AvenStr aven_path_fname(AvenStr path, AvenArena *arena);
AVEN_FN bool aven_path_is_abs(AvenStr path);
AVEN_FN AvenStr aven_path_rel_intersect(
    AvenStr path1,
    AvenStr path2,
    AvenArena *arena
);
AVEN_FN AvenStr aven_path_rel_diff(
    AvenStr path1,
    AvenStr path2,
    AvenArena *arena
);

typedef Result(AvenStr) AvenPathResult;
typedef enum {
    AVEN_PATH_EXE_ERROR_NONE = 0,
    AVEN_PATH_EXE_ERROR_FAIL,
    AVEN_PATH_EXE_ERROR_UNSUPPORTED,
} AvenPathExeError;

AVEN_FN AvenPathResult aven_path_exe(AvenArena *arena);

#ifdef AVEN_IMPLEMENTATION

#include <stdarg.h>

#ifdef __linux__
    #if !defined(_POSIX_C_SOURCE) or _POSIX_C_SOURCE < 200112L
        #error "readlink requires _POSIX_C_SOURCE >= 200112L"
    #endif
    #include <unistd.h>
#endif

AVEN_FN AvenStr aven_path_internal(AvenArena *arena, AvenStr part_str, ...) {
    AvenStr part_data[AVEN_PATH_MAX_ARGS];
    List(AvenStr) part_list = list_array(part_data);

    list_push(part_list) = part_str;

    va_list args;
    va_start(args, part_str);
    for (
        AvenStr str = va_arg(args, AvenStr);
        str.len != 0;
        str = va_arg(args, AvenStr)
    ) {
        list_push(part_list) = str;
    }
    va_end(args);

    AvenStrSlice part_slice = slice_list(part_list);

    return aven_str_join(
        part_slice,
        AVEN_PATH_SEP,
        arena
    );
}

AVEN_FN AvenStr aven_path_fname(AvenStr path, AvenArena *arena) {
    size_t i;
    for (i = path.len; i > 0; i -= 1) {
        if (get(path, i - 1) == AVEN_PATH_SEP) {
            break;
        }
    }
    if (i == 0) {
        return path;
    }
    if (i == path.len) {
        return aven_str("");
    }
    AvenStr fname = { .len = path.len - i };
    fname.ptr = aven_arena_alloc(arena, fname.len, 1, 1);

    path.ptr += i;
    path.len -= i;
    slice_copy(fname, path);

    return fname;
}

AVEN_FN AvenStr aven_path_rel_dir(AvenStr path, AvenArena *arena) {
    while (path.len > 0 and get(path, 0) == '.') {
        if (path.len > 1) {
            if (get(path, 1) == '/') {
                path = aven_str_tail(path, 2);
            } else {
                break;
            }
        } else {
            path = aven_str("");
        }
    }
    size_t i;
    for (i = path.len; i > 0; i -= 1) {
        if (get(path, i - 1) == AVEN_PATH_SEP) {
            break;
        }
    }
    if (i == 0) {
        return aven_str(".");
    }
    if (i == path.len) {
        return aven_str_head(path, path.len - 1);
    }
    AvenStr dir = { .len = i + 1 };
    dir.ptr = aven_arena_alloc(arena, dir.len, 1, 1),

    get(dir, 0) = '.';
    get(dir, 1) = AVEN_PATH_SEP;
    slice_copy(aven_str_tail(dir, 2), aven_str_head(path, dir.len - 2));

    return dir;
}

AVEN_FN bool aven_path_is_abs(AvenStr path) {
#ifdef _WIN32
    if (path.len <= 1) {
        return false;
    }

    if (get(path, 1) == ':') {
        return true;
    }

    return get(path, 0) == AVEN_PATH_SEP and
        get(path, 1) == AVEN_PATH_SEP;
#else
    return get(path, 0) == AVEN_PATH_SEP;
#endif
}

AVEN_FN AvenStr aven_path_rel_intersect(
    AvenStr path1,
    AvenStr path2,
    AvenArena *arena
) {
    assert(!aven_path_is_abs(path1));
    assert(!aven_path_is_abs(path2));

    while (path1.len > 1 and get(path1, 0) == '.' and get(path1, 1) == '/') {
        path1 = aven_str_tail(path1, 2);
    }
    while (path2.len > 1 and get(path2, 0) == '.' and get(path2, 1) == '/') {
        path2 = aven_str_tail(path2, 2);
    }
    if (path1.len == 0 or path2.len == 0) {
        return aven_str(".");
    }
    if (get(path1, 0) != get(path2, 0)) {
        return aven_str(".");
    }

    ByteSlice join_mem = aven_arena_create_slice(
        unsigned char,
        arena,
        2 + min(path1.len, path2.len)
    );
    AvenArena join_arena = aven_arena_init(join_mem.ptr, join_mem.len);

    AvenArena temp_arena = *arena;

    AvenStrSlice path1_parts = aven_str_split(
        path1,
        AVEN_PATH_SEP,
        &temp_arena
    );
    AvenStrSlice path2_parts = aven_str_split(
        path2,
        AVEN_PATH_SEP,
        &temp_arena
    );

    size_t len = min(path1_parts.len, path2_parts.len);
    size_t same_index = 0;
    for (; same_index < len; same_index += 1) {
        bool match = aven_str_compare(
            get(path1_parts, same_index),
            get(path2_parts, same_index)
        );
        if (!match) {
            break;
        }
    }

    List(AvenStr) intersect_list = aven_arena_create_list(
        AvenStr,
        &temp_arena,
        same_index + 1
    );
    list_push(intersect_list) = aven_str(".");
    for (size_t i = 0; i < same_index; i += 1) {
        list_push(intersect_list) = get(path1_parts, i);
    }
    AvenStrSlice intersect_parts = slice_list(intersect_list);

    AvenStr intersect = aven_str_join(
        intersect_parts,
        AVEN_PATH_SEP,
        &join_arena
    );

    size_t used_mem = (size_t)(join_arena.base - join_mem.ptr);
    (void)aven_arena_shrink_array(
        unsigned char,
        arena,
        join_mem.ptr,
        join_mem.len,
        used_mem
    );

    return intersect;
}

AVEN_FN AvenStr aven_path_rel_diff(
    AvenStr path1,
    AvenStr path2,
    AvenArena *arena
) {
    assert(!aven_path_is_abs(path1));
    assert(!aven_path_is_abs(path2));

    ByteSlice join_mem = aven_arena_create_slice(
        unsigned char,
        arena,
        2 * path1.len + 2 * path2.len + 3
    );
    AvenArena join_arena = aven_arena_init(join_mem.ptr, join_mem.len);

    AvenArena temp_arena = *arena;

    AvenStrSlice path1_parts = aven_str_split(
        path1,
        AVEN_PATH_SEP,
        &temp_arena
    );
    if (aven_str_compare(get(path1_parts, 0), aven_str("."))) {
        path1_parts.ptr += 1;
        path1_parts.len -= 1;
    }

    AvenStrSlice path2_parts = aven_str_split(
        path2,
        AVEN_PATH_SEP,
        &temp_arena
    );
    if (
        path2_parts.len > 0 and
        aven_str_compare(get(path2_parts, 0), aven_str("."))
    ) {
        path2_parts.ptr += 1;
        path2_parts.len -= 1;
    }

    size_t len = min(path1_parts.len, path2_parts.len);
    size_t same_index = 0;
    for (; same_index < len; same_index += 1) {
        bool match = aven_str_compare(
            get(path1_parts, same_index),
            get(path2_parts, same_index)
        );
        if (!match) {
            break;
        }
    }

    List(AvenStr) diff_list = aven_arena_create_list(
        AvenStr,
        &temp_arena,
        1 + path1_parts.len + path2_parts.len - 2 * same_index
    );
    list_push(diff_list) = aven_str(".");
    for (size_t i = same_index; i < path2_parts.len; i += 1) {
        list_push(diff_list) = aven_str("..");
    }
    for (size_t i = same_index; i < path1_parts.len; i += 1) {
        list_push(diff_list) = get(path1_parts, i);
    }
    AvenStrSlice diff_parts = slice_list(diff_list);

    AvenStr diff = aven_str_join(diff_parts, AVEN_PATH_SEP, &join_arena);
    size_t used_mem = (size_t)(join_arena.base - join_mem.ptr);
    (void)aven_arena_shrink_array(
        unsigned char,
        arena,
        join_mem.ptr,
        join_mem.len,
        used_mem
    );

    return diff;
}

AVEN_FN AvenPathResult aven_path_exe(AvenArena *arena) {
#ifdef _WIN32
    AVEN_WIN32_FN(uint32_t) GetModuleFileNameA(
        void *mod,
        char *buffer,
        uint32_t buffer_len
    );

    char buffer[AVEN_PATH_MAX_LEN];
    uint32_t len = GetModuleFileNameA(NULL, buffer, countof(buffer));
    if (len <= 0 or len == countof(buffer)) {
        return (AvenPathResult){ .error = AVEN_PATH_EXE_ERROR_FAIL };
    }

    AvenStr path = { .len = len };
    path.ptr = aven_arena_alloc(arena, path.len, 1, 1);
    memcpy(path.ptr, buffer, path.len);

    return (AvenPathResult){ .payload = path };
#elif defined(__linux__)
    char buffer[AVEN_PATH_MAX_LEN];
    ssize_t len = readlink("/proc/self/exe", buffer, countof(buffer));
    if (len <= 0 or len == countof(buffer)) {
        return (AvenPathResult){ .error = AVEN_PATH_EXE_ERROR_FAIL };
    }

    AvenStr path = { .len = (size_t)len };
    path.ptr = aven_arena_alloc(arena, path.len, 1, 1);

    memcpy(path.ptr, buffer, path.len);

    return (AvenPathResult){ .payload = path };
#else
    (void)arena;
    return (AvenPathResult){ .error = AVEN_PATH_EXE_ERROR_UNSUPPORTED };
#endif
}

#endif // AVEN_IMPLEMENTATION

#endif // AVEN_PATH_H
