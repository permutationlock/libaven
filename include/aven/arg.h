#ifndef AVEN_ARG_H
    #define AVEN_ARG_H

    #include "../aven.h"
    #include "io.h"
    #include "str.h"

    typedef enum {
        AVEN_ARG_TYPE_BOOL = 0,
        AVEN_ARG_TYPE_INT,
        AVEN_ARG_TYPE_UINT,
        AVEN_ARG_TYPE_STRING,
        AVEN_ARG_TYPE_HELP,
    } AvenArgType;

    typedef struct {
        AvenArgType type;
        union {
            bool arg_bool;
            int64_t arg_int;
            uint64_t arg_uint;
            AvenStr arg_str;
        } data;
    } AvenArgValue;

    typedef struct {
        AvenStr name;
        AvenStr description;
        bool optional;
        AvenArgType type;
        AvenArgValue value;
    } AvenArg;

    typedef Optional(AvenArg) AvenArgOptional;
    typedef Slice(AvenArg) AvenArgSlice;

    typedef enum {
        AVEN_ARG_ERROR_NONE = 0,
        AVEN_ARG_ERROR_HELP,
        AVEN_ARG_ERROR_VALUE,
        AVEN_ARG_ERROR_MISSING,
        AVEN_ARG_ERROR_UNKNOWN,
    } AvenArgError;

    static const AvenStr aven_arg_type_data[] = {
        [AVEN_ARG_TYPE_BOOL] = aven_str_init(" [false]"),
        [AVEN_ARG_TYPE_UINT] = aven_str_init(" N"),
        [AVEN_ARG_TYPE_INT] = aven_str_init(" [+|-]N"),
        [AVEN_ARG_TYPE_STRING] = aven_str_init(" \"str\""),
        [AVEN_ARG_TYPE_HELP] = aven_str_init(""),
    };

    static const AvenStrSlice aven_arg_types = {
        .ptr = (AvenStr *)aven_arg_type_data,
        .len = countof(aven_arg_type_data),
    };

    static const AvenArg aven_arg_help_arg = {
        .name = aven_str_init("--help"),
        .description = aven_str_init("show this  message"),
        .type = AVEN_ARG_TYPE_HELP,
    };

    static size_t aven_arg_col_len(AvenArgSlice args) {
        size_t max_len = aven_arg_help_arg.name.len +
            get(aven_arg_types, aven_arg_help_arg.type).len;
        for (size_t i = 0; i < args.len; i += 1) {
            AvenArg arg = get(args, i);
            max_len = max(
                arg.name.len + get(aven_arg_types, arg.type).len,
                max_len
            );
        }
        return max_len;
    }

    static void aven_arg_print(AvenArg arg, size_t cols) {
        if (arg.name.len == 0) {
            return;
        }
        char buffer[1024];
        List(char) clist = list_array(buffer);
        slice_copy((AvenStr)slice_list_free(clist), arg.name);
        clist.len += arg.name.len;
        AvenStr type_str = get(aven_arg_types, arg.type);
        slice_copy((AvenStr)slice_list_free(clist), type_str);
        clist.len += type_str.len;
        while (clist.len < cols) {
            list_push(clist) = ' ';
        }
        AvenStr str = slice_list(clist);
        if (arg.type == arg.value.type and arg.type != AVEN_ARG_TYPE_HELP) {
            AvenFmtArg default_arg = { 0 };
            switch (arg.type) {
                case AVEN_ARG_TYPE_BOOL: {
                    default_arg = arg.value.data.arg_bool ?
                        aven_fmt_str(aven_str("true")) :
                        aven_fmt_str(aven_str("false"));
                    break;
                }
                case AVEN_ARG_TYPE_UINT: {
                    default_arg = aven_fmt_uint(arg.value.data.arg_uint);
                    break;
                }
                case AVEN_ARG_TYPE_INT: {
                    default_arg = aven_fmt_int(arg.value.data.arg_int);
                    break;
                }
                case AVEN_ARG_TYPE_STRING: {
                    default_arg = aven_fmt_str(arg.value.data.arg_str);
                    break;
                }
                case AVEN_ARG_TYPE_HELP: {
                    assert(false);
                    break;
                }
            }
            aven_io_perrf(
                "    {}    {} (default={})\n",
                aven_fmt_str(str),
                aven_fmt_str(arg.description),
                default_arg
            );
        } else if (arg.optional) {
            aven_io_perrf(
                "    {}    {} (optional)\n",
                aven_fmt_str(str),
                aven_fmt_str(arg.description)
            );
        } else {
            aven_io_perrf(
                "    {}    {}\n",
                aven_fmt_str(str),
                aven_fmt_str(arg.description)
            );
        }
    }

    static void aven_arg_help(
        AvenArgSlice args,
        AvenStr overview,
        AvenStr usage,
        size_t cols
    ) {
        aven_io_perrf("overview: {}\n", aven_fmt_str(overview));
        aven_io_perrf("usage: {}\n", aven_fmt_str(usage));
        aven_io_perr("options:\n");
        for (size_t i = 0; i < args.len; i += 1) {
            aven_arg_print(get(args, i), cols);
        }
        aven_arg_print(aven_arg_help_arg, cols);
    }

    static inline AvenArgError aven_arg_parse(
        AvenArgSlice args,
        char **argv,
        int argc,
        AvenStr overview,
        AvenStr usage
    ) {
        size_t cols = aven_arg_col_len(args);
        bool found_generic = false;
        for (int i = 1; i < argc; i += 1) {
            AvenStr arg_str = aven_str_cstr(argv[i]);
            if (aven_str_equals(arg_str, aven_str("--help"))) {
                aven_arg_help(args, overview, usage, cols);
                return AVEN_ARG_ERROR_HELP;
            }

            bool found = false;
            for (size_t j = 0; j < args.len; j += 1) {
                AvenArg *arg = &get(args, j);
                if (!aven_str_equals(arg_str, arg->name)) {
                    continue;
                }

                switch (arg->type) {
                    case AVEN_ARG_TYPE_BOOL: {
                        if (
                            i + 1 < argc and
                            aven_str_equals(
                                aven_str_cstr(argv[i + 1]),
                                aven_str("false")
                            )
                        ) {
                            arg->value.data.arg_bool = false;
                            i += 1;
                        } else if (
                            i + 1 < argc and
                            aven_str_equals(
                                aven_str_cstr(argv[i + 1]),
                                aven_str("true")
                            )
                        ) {
                            arg->value.data.arg_bool = true;
                            i += 1;
                        } else {
                            arg->value.data.arg_bool = true;
                        }
                        break;
                    }
                    case AVEN_ARG_TYPE_UINT: {
                        if (i + 1 >= argc) {
                            aven_io_perr("missing expected argument value:\n");
                            aven_arg_print(*arg, cols);
                            return AVEN_ARG_ERROR_VALUE;
                        }
                        AvenStr next_arg = aven_str_cstr(argv[i + 1]);
                        AvenFmtParseIntResult pr_res =
                            aven_fmt_parse_int_decimal(next_arg);
                        if (pr_res.error != 0 or pr_res.payload < 0) {
                            aven_io_perrf(
                                "expected unsigned integer argument, "
                                "found \"{}\"",
                                aven_fmt_str(next_arg)
                            );
                            aven_arg_print(*arg, cols);
                            return AVEN_ARG_ERROR_VALUE;
                        }
                        arg->value.data.arg_int = pr_res.payload;
                        arg->value.type = AVEN_ARG_TYPE_UINT;
                        i += 1;
                        break;
                    }
                    case AVEN_ARG_TYPE_INT: {
                        if (i + 1 >= argc) {
                            aven_io_perr("missing expected argument value:\n");
                            aven_arg_print(*arg, cols);
                            return AVEN_ARG_ERROR_VALUE;
                        }
                        AvenStr next_arg = aven_str_cstr(argv[i + 1]);
                        AvenFmtParseIntResult pr_res =
                            aven_fmt_parse_int_decimal(next_arg);
                        if (pr_res.error != 0) {
                            aven_io_perrf(
                                "expected integer argument, found \"{}\"",
                                aven_fmt_str(next_arg)
                            );
                            aven_arg_print(*arg, cols);
                            return AVEN_ARG_ERROR_VALUE;
                        }
                        arg->value.data.arg_int = pr_res.payload;
                        arg->value.type = AVEN_ARG_TYPE_INT;
                        i += 1;
                        break;
                    }
                    case AVEN_ARG_TYPE_STRING: {
                        if (i + 1 >= argc) {
                            aven_io_perr("missing expected argument value:\n");
                            aven_arg_print(*arg, cols);
                            return AVEN_ARG_ERROR_VALUE;
                        }
                        arg->value.data.arg_str = aven_str_cstr(argv[i + 1]);
                        arg->value.type = AVEN_ARG_TYPE_STRING;
                        i += 1;
                        break;
                    }
                    default: {
                        break;
                    }
                }

                found = true;
                break;
            }

            if (!found and !found_generic) {
                for (size_t j = 0; j < args.len; j += 1) {
                    AvenArg *arg = &get(args, j);
                    if (!aven_str_equals(arg->name, aven_str(""))) {
                        continue;
                    }
                    switch (arg->type) {
                        case AVEN_ARG_TYPE_BOOL: {
                            if (aven_str_equals(arg_str, aven_str("false"))) {
                                arg->value.data.arg_bool = false;
                                arg->value.type = AVEN_ARG_TYPE_BOOL;
                            } else if (
                                aven_str_equals(arg_str, aven_str("true"))
                            ) {
                                arg->value.data.arg_bool = true;
                                arg->value.type = AVEN_ARG_TYPE_BOOL;
                            } else {
                                aven_io_perrf(
                                    "expected boolean argument, found \"{}\"",
                                    aven_fmt_str(arg_str)
                                );
                                return AVEN_ARG_ERROR_VALUE;
                            }
                            found = true;
                            break;
                        }
                        case AVEN_ARG_TYPE_UINT: {
                            AvenFmtParseIntResult pr_res =
                                aven_fmt_parse_int_decimal(arg_str);
                            if (pr_res.error != 0 or pr_res.payload < 0) {
                                aven_io_perrf(
                                    "expected unsigned integer argument,"
                                    " found \"{}\"",
                                    aven_fmt_str(arg_str)
                                );
                                return AVEN_ARG_ERROR_VALUE;
                            }
                            arg->value.data.arg_int = pr_res.payload;
                            arg->value.type = AVEN_ARG_TYPE_UINT;
                            found = true;
                            break;
                        }
                        case AVEN_ARG_TYPE_INT: {
                            AvenFmtParseIntResult pr_res =
                                aven_fmt_parse_int_decimal(arg_str);
                            if (pr_res.error != 0) {
                                aven_io_perrf(
                                    "expected integer argument, found \"{}\"",
                                    aven_fmt_str(arg_str)
                                );
                                return AVEN_ARG_ERROR_VALUE;
                            }
                            arg->value.data.arg_int = pr_res.payload;
                            arg->value.type = AVEN_ARG_TYPE_INT;
                            found = true;
                            break;
                        }
                        case AVEN_ARG_TYPE_STRING: {
                            arg->value.data.arg_str = arg_str;
                            arg->value.type = AVEN_ARG_TYPE_STRING;
                            found = true;
                            break;
                        }
                        default: {
                            break;
                        }
                    }
                }
                found_generic = found;
            }

            if (!found) {
                aven_io_perrf("unknown option: {}\n", aven_fmt_str(arg_str));
                aven_arg_help(args, overview, usage, cols);
                return AVEN_ARG_ERROR_UNKNOWN;
            }
        }

        AvenArgError error = 0;
        for (size_t j = 0; j < args.len; j += 1) {
            AvenArg arg = get(args, j);
            if (!arg.optional and arg.value.type != arg.type) {
                aven_io_perr("missing required argument:\n");
                aven_arg_print(arg, cols);
                error = AVEN_ARG_ERROR_MISSING;
            }
        }

        return error;
    }

    static inline AvenArgOptional aven_arg_get(
        AvenArgSlice arg_slice,
        AvenStr argname
    ) {
        for (size_t i = 0; i < arg_slice.len; i += 1) {
            if (aven_str_equals(argname, get(arg_slice, i).name)) {
                return (AvenArgOptional){
                    .value = get(arg_slice, i),
                    .valid = true,
                };
            }
        }

        return (AvenArgOptional){ .valid = false };
    }

    #define aven_arg_has_arg(as, n) aven_arg_has_arg_internal(as, aven_str(n))
    #define aven_arg_get_bool(as, n) aven_arg_get_bool_internal( \
            as, \
            aven_str(n) \
        )
    #define aven_arg_get_int(as, n) aven_arg_get_int_internal(as, aven_str(n))
    #define aven_arg_get_uint(as, n) aven_arg_get_uint_internal( \
            as, \
            aven_str(n) \
        )
    #define aven_arg_get_str(as, n) aven_arg_get_str_internal(as, aven_str(n))

    static inline bool aven_arg_has_arg_internal(
        AvenArgSlice arg_slice,
        AvenStr argname
    ) {
        AvenArgOptional opt_arg = aven_arg_get(arg_slice, argname);
        return opt_arg.valid and
            (opt_arg.value.type == opt_arg.value.value.type);
    }

    static inline bool aven_arg_get_bool_internal(
        AvenArgSlice arg_slice,
        AvenStr argname
    ) {
        AvenArgOptional opt_arg = aven_arg_get(arg_slice, argname);
        AvenArg arg = unwrap(opt_arg);
        assert(arg.type == arg.value.type);
        assert(arg.type == AVEN_ARG_TYPE_BOOL);
        return arg.value.data.arg_bool;
    }

    static inline int64_t aven_arg_get_int_internal(
        AvenArgSlice arg_slice,
        AvenStr argname
    ) {
        AvenArgOptional opt_arg = aven_arg_get(arg_slice, argname);
        AvenArg arg = unwrap(opt_arg);
        assert(arg.type == arg.value.type);
        assert(arg.type == AVEN_ARG_TYPE_INT);
        return arg.value.data.arg_int;
    }

    static inline uint64_t aven_arg_get_uint_internal(
        AvenArgSlice arg_slice,
        AvenStr argname
    ) {
        AvenArgOptional opt_arg = aven_arg_get(arg_slice, argname);
        AvenArg arg = unwrap(opt_arg);
        assert(arg.type == arg.value.type);
        assert(arg.type == AVEN_ARG_TYPE_UINT);
        return arg.value.data.arg_uint;
    }

    static inline AvenStr aven_arg_get_str_internal(
        AvenArgSlice arg_slice,
        AvenStr argname
    ) {
        AvenArgOptional opt_arg = aven_arg_get(arg_slice, argname);
        AvenArg arg = unwrap(opt_arg);
        assert(arg.type == arg.value.type);
        assert(arg.type == AVEN_ARG_TYPE_STRING);
        return arg.value.data.arg_str;
    }
#endif

