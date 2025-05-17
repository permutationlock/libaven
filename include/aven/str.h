#ifndef AVEN_STR_H
    #define AVEN_STR_H

    #include "../aven.h"
    #include "arena.h"

    typedef Slice(char) AvenStr;
    typedef Slice(AvenStr) AvenStrSlice;

    #define aven_str_init(a) { .ptr = a, .len = countof(a) - 1 }
    #define aven_str(a) (AvenStr)aven_str_init(a)

    #define aven_str_head(s, i) (AvenStr)slice_head(s, i)
    #define aven_str_tail(s, i) (AvenStr)slice_tail(s, i)
    #define aven_str_range(s, i, j) (AvenStr)slice_range(s, i, j)

    static inline AvenStr aven_str_cstr(char *cstr) {
        size_t len = 0;
        for (char *c = cstr; *c != 0; c += 1) {
            len += 1;
        }
        return (AvenStr){ .ptr = cstr, .len = len };
    }

    static inline char *aven_str_to_cstr(AvenStr str, AvenArena *arena) {
        AvenStr cpy = { .len = str.len };
        cpy.ptr = aven_arena_alloc(arena, cpy.len + 1, 1, 1);
        slice_copy(cpy, str);
        cpy.ptr[cpy.len] = 0;
        return cpy.ptr;
    }

    static inline bool aven_str_equals(AvenStr s1, AvenStr s2) {
        if (s1.len != s2.len) {
            return false;
        }
        for (size_t i = 0; i < s1.len; i += 1) {
            if (get(s1, i) != get(s2, i)) {
                return false;
            }
        }
        return true;
    }

    static inline AvenStr aven_str_copy(AvenStr str, AvenArena *arena) {
        AvenStr cpy = { .len = str.len };
        cpy.ptr = aven_arena_alloc(arena, cpy.len, 1, 1);
        slice_copy(cpy, str);
        return cpy;
    }

    static inline AvenStrSlice aven_str_split(
        AvenStr str,
        char separator,
        AvenArena *arena
    ) {
        List(AvenStr) split_strs = aven_arena_create_list(
            AvenStr,
            arena,
            str.len / 2 + 1
        );

        size_t after_last_sep = 0;
        for (size_t i = 0; i <= str.len; i += 1) {
            if (i == str.len or get(str, i) == separator) {
                size_t len = i - after_last_sep;
                if (len > 0) {
                    list_push(split_strs) = (AvenStr){
                        .ptr = str.ptr + after_last_sep,
                        .len = len,
                    };
                }

                after_last_sep = i + 1;
            }
        }

        return aven_arena_commit_list_to_slice(AvenStrSlice, arena, split_strs);
    }

    static inline AvenStr aven_str_concat_slice(
        AvenStrSlice strs,
        AvenArena *arena
    ) {
        size_t total_len = 0;
        for (size_t i = 0; i < strs.len; i += 1) {
            total_len += get(strs, i).len;
        }

        AvenStr new_string = { .len = total_len };
        new_string.ptr = aven_arena_alloc(arena, total_len, 1, 1);

        AvenStr rest_str = new_string;
        for (size_t i = 0; i < strs.len; i += 1) {
            AvenStr cur_str = get(strs, i);
            slice_copy(rest_str, cur_str);
            rest_str = aven_str_tail(rest_str, cur_str.len);
        }

        return new_string;
    }

    static inline AvenStr aven_str_concat(
        AvenStr s1,
        AvenStr s2,
        AvenArena *arena
    ) {
        AvenStr str_data[] = { s1, s2 };
        AvenStrSlice strs = slice_array(str_data);
        return aven_str_concat_slice(strs, arena);
    }

    static inline AvenStr aven_str_join(
        AvenStrSlice strings,
        char separator,
        AvenArena *arena
    ) {
        size_t len = 0;
        for (size_t i = 0; i < strings.len; i += 1) {
            AvenStr cur_str = get(strings, i);
            if (cur_str.len == 0) {
                continue;
            }

            len += get(strings, i).len;
            if ((i + 1) < strings.len) {
                len += 1;
            }
        }

        char *str_mem = aven_arena_alloc(arena, len, 1, 1);

        AvenStr new_str = { .ptr = str_mem, .len = len };
        AvenStr rest_str = new_str;

        for (size_t i = 0; i < strings.len; i += 1) {
            AvenStr cur_str = get(strings, i);
            if (cur_str.len == 0) {
                continue;
            }

            slice_copy(rest_str, cur_str);
            rest_str = aven_str_tail(rest_str, cur_str.len);

            if ((i + 1) < strings.len) {
                get(rest_str, 0) = separator;
                rest_str = aven_str_tail(rest_str, 1);
            }
        }

        return new_str;
    }

    static inline AvenStr aven_str_escape(AvenStr str, AvenArena *arena) {
        List(char) esc_list = aven_arena_create_list(
            char,
            arena,
            (size_t)2 * str.len
        );

        for (size_t i = 0; i < str.len; i += 1) {
            char c = get(str, i);
            switch (c) {
                case '\\':
                case '\"':
                case '\'':
                    list_push(esc_list) = '\\';
                    break;
                default:
                    break;
            }
            list_push(esc_list) = c;
        }

        return aven_arena_commit_list_to_slice(AvenStr, arena, esc_list);
    }

    // This is slow, but it's simple and I'm not using it for anything critical
    static inline AvenStr aven_str_delete_in_place(
        AvenStr str,
        AvenStrSlice substrs
    ) {
        size_t fin = 0;
        size_t start = 0;
        size_t end = 0;
        while (end < str.len) {
            bool replaced = false;
            for (size_t i = 0; i < substrs.len; i += 1) {
                AvenStr substr = get(substrs, i);
                AvenStr head = aven_str_range(
                    str,
                    end,
                    min(str.len, end + substr.len)
                );
                if (aven_str_equals(head, substr)) {
                    if (start != end) {
                        AvenStr src = aven_str_range(str, start, end);
                        if (fin != start) {
                            AvenStr dest = aven_str_range(
                                str,
                                fin,
                                fin + src.len
                            );
                            slice_copy(dest, src);
                        }
                        fin += src.len;
                    }
                    end += substr.len;
                    start = end;
                    replaced = true;
                    break;
                }
            }
            if (!replaced) {
                end += 1;
            }
        }
        if (start != end) {
            AvenStr src = aven_str_range(str, start, end);
            if (fin != start) {
                AvenStr dest = aven_str_range(str, fin, fin + src.len);
                slice_copy(dest, src);
            }
            fin += src.len;
        }

        return aven_str_head(str, fin);
    }
#endif
// AVEN_STR_H
