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

    static void aven_arg_print_type(AvenArgType arg_type) {
        switch (arg_type) {
            case AVEN_ARG_TYPE_UINT: {
                aven_io_perr(" n");
                break;
            }
            case AVEN_ARG_TYPE_INT: {
                aven_io_perr(" [+|-]n");
                break;
            }
            case AVEN_ARG_TYPE_STRING: {
                aven_io_perr(" \"str\"");
                break;
            }
            case AVEN_ARG_TYPE_BOOL:
            default: {
                break;
            }
        }
    }

    static void aven_arg_print_value(AvenArgValue value) {
        switch (value.type) {
            case AVEN_ARG_TYPE_BOOL: {
                if (value.data.arg_bool) {
                    aven_io_perr("true");
                } else {
                    aven_io_perr("false");
                }
                break;
            }
            case AVEN_ARG_TYPE_INT: {
                aven_io_perrf("{}", aven_fmt_int(value.data.arg_int));
                break;
            }
            case AVEN_ARG_TYPE_UINT: {
                aven_io_perrf("{}", aven_fmt_uint(value.data.arg_uint));
                break;
            }
            case AVEN_ARG_TYPE_STRING: {
                aven_io_perrf("\"{}\"", aven_fmt_str(value.data.arg_str));
                break;
            }
            default: {
                break;
            }
        }
    }

    static void aven_arg_print(AvenArg arg) {
        if (arg.name.len == 0) {
            return;
        }
        aven_io_perrf("    {}", aven_fmt_str(arg.name));

        aven_arg_print_type(arg.type);

        aven_io_perrf("  --  {}", aven_fmt_str(arg.description));

        if (arg.type == arg.value.type) {
            if (arg.type != AVEN_ARG_TYPE_BOOL or arg.value.data.arg_bool) {
                aven_io_perr(" (default=");
                aven_arg_print_value(arg.value);
                aven_io_perr(")");
            }
        } else if (arg.optional) {
            aven_io_perr(" (optional)");
        }

        aven_io_perr("\n");
    }

    static void aven_arg_help(
        AvenArgSlice args,
        AvenStr overview,
        AvenStr usage
    ) {
        aven_io_perrf("overview: {}\n", aven_fmt_str(overview));
        aven_io_perrf("usage: {}\n", aven_fmt_str(usage));
        aven_io_perr("options:\n");
        aven_io_perr("    help, -h, -help, --help -- Show this message\n");
        for (size_t i = 0; i < args.len; i += 1) {
            aven_arg_print(get(args, i));
        }
    }

    static inline AvenArgError aven_arg_parse(
        AvenArgSlice args,
        char **argv,
        int argc,
        AvenStr overview,
        AvenStr usage
    ) {
        bool found_generic = false;
        for (int i = 1; i < argc; i += 1) {
            AvenStr arg_str = aven_str_cstr(argv[i]);
            if (
                aven_str_equals(arg_str, aven_str("help")) or
                aven_str_equals(arg_str, aven_str("-h")) or
                aven_str_equals(arg_str, aven_str("-help")) or
                aven_str_equals(arg_str, aven_str("--help"))
            ) {
                aven_arg_help(args, overview, usage);
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
                            aven_arg_print(*arg);
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
                            aven_arg_print(*arg);
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
                            aven_arg_print(*arg);
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
                            aven_arg_print(*arg);
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
                            aven_arg_print(*arg);
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
                aven_arg_help(args, overview, usage);
                return AVEN_ARG_ERROR_UNKNOWN;
            }
        }

        AvenArgError error = 0;
        for (size_t j = 0; j < args.len; j += 1) {
            AvenArg arg = get(args, j);
            if (!arg.optional and arg.value.type != arg.type) {
                aven_io_perr("missing required argument:\n");
                aven_arg_print(arg);
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
// AVEN_ARG_H
