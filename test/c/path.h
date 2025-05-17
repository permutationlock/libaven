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

    #define aven_path(a, ...) aven_str_join( \
            (AvenStrSlice)slice_array((AvenStr[]){ __VA_ARGS__ }), \
            AVEN_PATH_SEP, \
            a \
        )

    static inline AvenStr aven_path_fname(AvenStr path, AvenArena *arena) {
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

    static inline AvenStr aven_path_containing_dir(AvenStr path) {
        if (path.len == 0) {
            return aven_str("..");
        } else if (path.len == 1) {
            if (get(path, 0) == AVEN_PATH_SEP) {
                static const char up_path[] = { AVEN_PATH_SEP, '.', '.', 0 };
                return (AvenStr){
                    .ptr = (char *)up_path,
                    .len = countof(up_path) - 1,
                };
            } else if (get(path, 0) == '.') {
                return aven_str("..");
            } else {
                return aven_str(".");
            }
        }

        size_t i;
        for (i = path.len - 1; i > 0; i -= 1) {
            if (get(path, i - 1) == AVEN_PATH_SEP) {
                break;
            }
        }
        if (i == 0) {
            return aven_str(".");
        }

        return aven_str_head(path, i - 1);
    }

    static inline bool aven_path_is_abs(AvenStr path) {
    #ifdef _WIN32
        if (path.len <= 1) {
            return false;
        }

        if (get(path, 1) == ':') {
            return true;
        }

        return get(path, 0) == AVEN_PATH_SEP and get(path, 1) == AVEN_PATH_SEP;
    #else
        return get(path, 0) == AVEN_PATH_SEP;
    #endif
    }

    static inline AvenStr aven_path_rel_intersect(
        AvenStr path1,
        AvenStr path2,
        AvenArena *arena
    ) {
        assert(!aven_path_is_abs(path1));
        assert(!aven_path_is_abs(path2));

        while (
            path1.len > 1 and
            get(path1, 0) == '.' and
            get(path1, 1) == AVEN_PATH_SEP
        ) {
            path1 = aven_str_tail(path1, 2);
        }
        while (
            path2.len > 1 and
            get(path2, 0) == '.' and
            get(path2, 1) == AVEN_PATH_SEP
        ) {
            path2 = aven_str_tail(path2, 2);
        }
        if (path1.len == 0 or path2.len == 0) {
            return aven_str(".");
        }
        if (get(path1, 0) != get(path2, 0)) {
            return aven_str(".");
        }

        AvenArenaChild join_child = aven_arena_child_init(
            arena,
            (size_t)2 + min(path1.len, path2.len)
        );
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
            bool match = aven_str_equals(
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
            &join_child.arena
        );
        aven_arena_child_commit(arena, &join_child);

        return intersect;
    }

    static inline AvenStr aven_path_rel_diff(
        AvenStr path1,
        AvenStr path2,
        AvenArena *arena
    ) {
        assert(!aven_path_is_abs(path1));
        assert(!aven_path_is_abs(path2));

        AvenArenaChild join_child = aven_arena_child_init(
            arena,
            (size_t)2 * path1.len + (size_t)2 * path2.len + 3
        );
        AvenArena temp_arena = *arena;

        AvenStrSlice path1_parts = aven_str_split(
            path1,
            AVEN_PATH_SEP,
            &temp_arena
        );
        if (aven_str_equals(get(path1_parts, 0), aven_str("."))) {
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
            aven_str_equals(get(path2_parts, 0), aven_str("."))
        ) {
            path2_parts.ptr += 1;
            path2_parts.len -= 1;
        }

        size_t len = min(path1_parts.len, path2_parts.len);
        size_t same_index = 0;
        for (; same_index < len; same_index += 1) {
            bool match = aven_str_equals(
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

        AvenStr diff = aven_str_join(
            diff_parts,
            AVEN_PATH_SEP,
            &join_child.arena
        );
        aven_arena_child_commit(arena, &join_child);

        return diff;
    }

    typedef enum {
        AVEN_PATH_EXE_ERROR_NONE = 0,
        AVEN_PATH_EXE_ERROR_FAIL,
        AVEN_PATH_EXE_ERROR_UNSUPPORTED,
    } AvenPathExeError;
    typedef Result(AvenStr, AvenPathExeError) AvenPathResult;

    AVEN_FN AvenPathResult aven_path_exe(AvenArena *arena);

    #ifdef AVEN_IMPLEMENTATION

        #if defined(__linux__) and !defined(NOLIBC)
            #if !defined(_POSIX_C_SOURCE) or _POSIX_C_SOURCE < 200112L
                #error "readlink requires _POSIX_C_SOURCE >= 200112L"
            #endif
            #include <unistd.h>
        #endif

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
        #elif defined(__linux__) and !defined(NOLIBC)
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
    #endif
    // AVEN_IMPLEMENTATION

#endif
// AVEN_PATH_H
