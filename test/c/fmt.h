#ifndef AVEN_FMT_H
    #define AVEN_FMT_H

    #include "../aven.h"
    #include "arena.h"
    #include "str.h"

    #define aven_fmt_uint(i) (AvenFmtArg){ \
            .type = AVEN_FMT_ARG_TYPE_UINT, \
            .data = { \
                .arg_uint = (AvenFmtArgUInt){ \
                    .val = (i), \
                    .digits = aven_fmt_uint_decimal_digits(i), \
                }, \
            }, \
        }
    #define aven_fmt_int(i) (AvenFmtArg){ \
            .type = AVEN_FMT_ARG_TYPE_INT, \
            .data = { \
                .arg_int = (AvenFmtArgInt){ \
                    .val = (i), \
                    .digits = aven_fmt_int_decimal_digits(i), \
                }, \
            }, \
        }
    #define aven_fmt_str(s) (AvenFmtArg){ \
            .type = AVEN_FMT_ARG_TYPE_STR, \
            .data = { .arg_str = (s) }, \
        }
    #define aven_fmt(a, f, ...) aven_fmt_args( \
            aven_str(f), \
            (AvenFmtArgSlice)slice_array((AvenFmtArg[]){ __VA_ARGS__ }), \
            a \
        )

    typedef enum {
        AVEN_FMT_ARG_TYPE_UINT,
        AVEN_FMT_ARG_TYPE_INT,
        AVEN_FMT_ARG_TYPE_STR,
    } AvenFmtArgType;

    typedef struct {
        uint64_t val;
        size_t digits;
    } AvenFmtArgUInt;

    typedef struct {
        int64_t val;
        size_t digits;
    } AvenFmtArgInt;

    typedef union {
        AvenFmtArgUInt arg_uint;
        AvenFmtArgInt arg_int;
        AvenStr arg_str;
    } AvenFmtArgData;

    typedef struct {
        AvenFmtArgType type;
        AvenFmtArgData data;
    } AvenFmtArg;
    typedef Slice(AvenFmtArg) AvenFmtArgSlice;

    static inline size_t aven_fmt_uint_decimal_digits(uint64_t num) {
        uint64_t digits = 1;
        uint64_t coeff = 10;
        while (coeff <= num) {
            uint64_t next_coeff = coeff * 10;
            digits += 1;
            if (next_coeff < coeff) {
                break;
            }
            coeff = next_coeff;
        }
        return (size_t)digits;
    }

    static inline size_t aven_fmt_int_decimal_digits(int64_t num) {
        uint64_t abs_num = (uint64_t)((num < 0) ? -num : num);
        return aven_fmt_uint_decimal_digits(abs_num);
    }

    static inline size_t aven_fmt_uint_decimal_buffer(AvenStr str, uint64_t num) {
        size_t digits = aven_fmt_uint_decimal_digits(num);
        size_t len = min(str.len, digits);

        do {
            digits -= 1;
            if (digits < str.len) {
                get(str, digits) = '0' + (char)(num % 10);
            }
            num /= 10;
        } while (digits > 0);

        return len;
    }

    static inline size_t aven_fmt_int_decimal_buffer(AvenStr str, int64_t num) {
        if (num < 0) {
            get(str, 0) = '-';
            str = aven_str_tail(str, 1);
        } else {
            get(str, 0) = '+';
            str = aven_str_tail(str, 1);
        }

        uint64_t abs_num = (uint64_t)((num < 0) ? -num : num);
        return 1 + aven_fmt_uint_decimal_buffer(str, abs_num);
    }

    typedef enum {
        AVEN_FMT_PARSE_INT_ERROR_NONE = 0,
        AVEN_FMT_PARSE_INT_ERROR_BADCHAR,
    } AvenFmtParseIntError;
    typedef Result(int64_t, AvenFmtParseIntError) AvenFmtParseIntResult;

    static inline AvenFmtParseIntResult aven_fmt_parse_int_decimal(AvenStr str) {
        int64_t sign = 1;
        if (get(str, 0) == '-') {
            sign = -1;
            str = aven_str_tail(str, 1);
        } else if (get(str, 0) == '+') {
            str = aven_str_tail(str, 1);
        }

        int64_t mag = 0;
        for (size_t i = 0; i < str.len; i += 1) {
            char c = get(str, i);
            if (c < '0' or c > '9') {
                return (AvenFmtParseIntResult){
                    .error = AVEN_FMT_PARSE_INT_ERROR_BADCHAR,
                };
            }
            int64_t digit = c - '0';
            mag = (10 * mag) + digit;
        }

        return (AvenFmtParseIntResult){ .payload = sign * mag };
    }

    static inline AvenStr aven_fmt_args(
        AvenStr fmt,
        AvenFmtArgSlice args,
        AvenArena *arena
    ) {
        size_t max_len = fmt.len;
        for (size_t i = 0; i < args.len; i += 1) {
            AvenFmtArg arg = get(args, i);
            switch (arg.type) {
                case AVEN_FMT_ARG_TYPE_UINT: {
                    AvenFmtArgUInt arg_uint = arg.data.arg_uint;
                    assert(arg_uint.digits > 0);
                    max_len += arg_uint.digits;
                    max_len -= 2;
                    break;
                }
                case AVEN_FMT_ARG_TYPE_INT: {
                    AvenFmtArgInt arg_int = arg.data.arg_int;
                    assert(arg_int.digits > 0);
                    max_len += 1 + arg_int.digits;
                    max_len -= 2;
                    break;
                }
                case AVEN_FMT_ARG_TYPE_STR: {
                    AvenStr arg_str = arg.data.arg_str;
                    max_len += arg_str.len;
                    max_len -= 2;
                    break;
                }
            }
        }

        AvenStr str = { .len = max_len };
        str.ptr = aven_arena_alloc(arena, str.len, 1, 1);

        AvenStr rem = str;
        size_t arg_index = 0;
        for (size_t i = 0; i < fmt.len; i += 1) {
            char c = get(fmt, i);
            if (c == '{' and (i + 1) < fmt.len and get(fmt, i + 1) == '}') {
                AvenFmtArg arg = get(args, arg_index);
                size_t written = 0;
                switch (arg.type) {
                    case AVEN_FMT_ARG_TYPE_UINT: {
                        AvenFmtArgUInt arg_uint = arg.data.arg_uint;
                        AvenStr space = aven_str_head(rem, arg_uint.digits);
                        written = aven_fmt_uint_decimal_buffer(
                            space,
                            arg_uint.val
                        );
                        break;
                    }
                    case AVEN_FMT_ARG_TYPE_INT: {
                        AvenFmtArgInt arg_int = arg.data.arg_int;
                        AvenStr space = aven_str_head(rem, 1 + arg_int.digits);
                        written = aven_fmt_int_decimal_buffer(
                            space,
                            arg_int.val
                        );
                        break;
                    }
                    case AVEN_FMT_ARG_TYPE_STR: {
                        AvenStr arg_str = arg.data.arg_str;
                        slice_copy(rem, arg_str);
                        written = arg_str.len;
                        break;
                    }
                }
                rem = aven_str_tail(rem, written);
                i += 1;
                arg_index += 1;
            } else {
                get(rem, 0) = c;
                rem = aven_str_tail(rem, 1);
            }
        }
        assert(arg_index == args.len);

        AvenStr written = aven_str_head(str, str.len - rem.len);
        aven_arena_resize(arena, str.ptr, str.len, written.len, 1);

        return written;
    }
#endif
// AVEN_FMT_H
