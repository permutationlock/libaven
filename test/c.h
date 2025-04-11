#ifndef TEST_C_H
#define TEST_C_H

#include <aven.h>
#include <aven/arena.h>
#include <aven/c.h>
#include <aven/str.h>
#include <aven/test.h>

typedef struct {
    AvenStr val;
    AvenCPpTokenType type;
} TestAvenCPpToken;

typedef struct {
    AvenStr src;
    Slice(TestAvenCPpToken) expected;
} TestAvenCLexPpArgs;

static AvenTestResult test_aven_c_lex_pp(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenCLexPpArgs *pp_args = args;

    AvenCPpTokenSet actual = aven_c_lex_pp(pp_args->src, &arena);
    if (actual.tokens.len != pp_args->expected.len + 1) {
        return (AvenTestResult){
            .error = 1,
            .message = aven_fmt(
                emsg_arena,
                "expected {} tokens, found {}",
                aven_fmt_uint(pp_args->expected.len + 1),
                aven_fmt_uint(actual.tokens.len)
            ),
        };
    }
    for (uint32_t i = 0; i < pp_args->expected.len; i += 1) {
        AvenCPpToken at = get(actual.tokens, i);
        TestAvenCPpToken et = get(pp_args->expected, i);

        AvenStr as = aven_c_pp_token_str(actual, i);

        if (
            at.type != et.type or
            !aven_str_equals(as, et.val)
        ) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected ({}, {}), found ({}, {})",
                    aven_fmt_str(et.val),
                    aven_fmt_str(aven_c_pp_token_type_str(et.type)),
                    aven_fmt_str(as),
                    aven_fmt_str(aven_c_pp_token_type_str(at.type))
                ),
            };
        }
    }
    if (
        get(actual.tokens, actual.tokens.len - 1).type !=
            AVEN_C_PP_TOKEN_TYPE_NONE
    ) {
        return (AvenTestResult){
            .error = 1,
            .message = aven_str("no terminating NONE token"),
        };
    }

    return (AvenTestResult){ 0 };
}

typedef struct {
    TestAvenCPpToken token;
    AvenCPpTokenLoc loc;
} TestAvenCPpTokenLoc;

typedef struct {
    AvenStr src;
    Slice(TestAvenCPpTokenLoc) expected;
} TestAvenCPpTokenLocArgs;

static AvenTestResult test_aven_c_pp_token_loc(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenCPpTokenLocArgs *pp_args = args;

    AvenCPpTokenSet actual = aven_c_lex_pp(pp_args->src, &arena);
    if (actual.tokens.len != pp_args->expected.len + 1) {
        return (AvenTestResult){
            .error = 1,
            .message = aven_fmt(
                emsg_arena,
                "expected {} tokens, found {}",
                aven_fmt_uint(pp_args->expected.len),
                aven_fmt_uint(actual.tokens.len)
            ),
        };
    }
    for (uint32_t i = 0; i < pp_args->expected.len; i += 1) {
        AvenCPpToken at = get(actual.tokens, i);
        AvenCPpTokenLoc al = aven_c_pp_token_loc(actual, i);
        TestAvenCPpTokenLoc et = get(pp_args->expected, i);

        AvenStr as = aven_c_pp_token_str(actual, i);

        if (
            at.type != et.token.type or
            !aven_str_equals(as, et.token.val)
        ) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected ({}, {}), found ({}, {})",
                    aven_fmt_str(et.token.val),
                    aven_fmt_str(aven_c_pp_token_type_str(et.token.type)),
                    aven_fmt_str(as),
                    aven_fmt_str(aven_c_pp_token_type_str(at.type))
                ),
            };
        }
        if (al.line != et.loc.line or al.col != et.loc.col) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected ({}, {}) at {}:{}, found at {}:{}",
                    aven_fmt_str(et.token.val),
                    aven_fmt_str(aven_c_pp_token_type_str(et.token.type)),
                    aven_fmt_uint(et.loc.line),
                    aven_fmt_uint(et.loc.col),
                    aven_fmt_uint(al.line),
                    aven_fmt_uint(al.col)
                ),
            };
        }
    }
    if (
        get(actual.tokens, actual.tokens.len - 1).type !=
            AVEN_C_PP_TOKEN_TYPE_NONE
    ) {
        return (AvenTestResult){
            .error = 1,
            .message = aven_str("no terminating NONE token"),
        };
    }

    return (AvenTestResult){ 0 };
}

static int test_c(AvenArena arena) {
    AvenTestCase tcase_data[] = {
        {
            .desc = aven_str("aven_c_lex_pp empty file"),
            .fn = test_aven_c_lex_pp,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str(""),
                .expected = { 0 },
            },
        },
        {
            .desc = aven_str("aven_c_lex_pp single declaration"),
            .fn = test_aven_c_lex_pp,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("int x;"),
                .expected = slice_array(
                    (TestAvenCPpToken[]){
                        {
                            .val = aven_str("int"),
                            .type = AVEN_C_PP_TOKEN_TYPE_KEY,
                        },
                        {
                            .val = aven_str("x"),
                            .type = AVEN_C_PP_TOKEN_TYPE_ID,
                        },
                        {
                            .val = aven_str(";"),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex_pp header name with <>"),
            .fn = test_aven_c_lex_pp,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("#include <stdio.h>"),
                .expected = slice_array(
                    (TestAvenCPpToken[]){
                        {
                            .val = aven_str("#"),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("include"),
                            .type = AVEN_C_PP_TOKEN_TYPE_ID,
                        },
                        {
                            .val = aven_str("<stdio.h>"),
                            .type = AVEN_C_PP_TOKEN_TYPE_HDR,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex_pp header name with <>"),
            .fn = test_aven_c_lex_pp,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("#include \"helper.h\""),
                .expected = slice_array(
                    (TestAvenCPpToken[]){
                        {
                            .val = aven_str("#"),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("include"),
                            .type = AVEN_C_PP_TOKEN_TYPE_ID,
                        },
                        {
                            .val = aven_str("\"helper.h\""),
                            .type = AVEN_C_PP_TOKEN_TYPE_HDR,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex_pp weird pp-number"),
            .fn = test_aven_c_lex_pp,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("int x = 0xE+12;\n"),
                .expected = slice_array(
                    (TestAvenCPpToken[]){
                        {
                            .val = aven_str("int"),
                            .type = AVEN_C_PP_TOKEN_TYPE_KEY,
                        },
                        {
                            .val = aven_str("x"),
                            .type = AVEN_C_PP_TOKEN_TYPE_ID,
                        },
                        {
                            .val = aven_str("="),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("0xE+12"),
                            .type = AVEN_C_PP_TOKEN_TYPE_NUM,
                        },
                        {
                            .val = aven_str(";"),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex_pp whitespace breaks pp-number"),
            .fn = test_aven_c_lex_pp,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("int x = 0xE +12;\n"),
                .expected = slice_array(
                    (TestAvenCPpToken[]){
                        {
                            .val = aven_str("int"),
                            .type = AVEN_C_PP_TOKEN_TYPE_KEY,
                        },
                        {
                            .val = aven_str("x"),
                            .type = AVEN_C_PP_TOKEN_TYPE_ID,
                        },
                        {
                            .val = aven_str("="),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("0xE"),
                            .type = AVEN_C_PP_TOKEN_TYPE_NUM,
                        },
                        {
                            .val = aven_str("+"),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("12"),
                            .type = AVEN_C_PP_TOKEN_TYPE_NUM,
                        },
                        {
                            .val = aven_str(";"),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex_pp simple escape sequence"),
            .fn = test_aven_c_lex_pp,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("\"Hello, World!\\n\""),
                .expected = slice_array(
                    (TestAvenCPpToken[]){
                        {
                            .val = aven_str("\"Hello, World!\\n\""),
                            .type = AVEN_C_PP_TOKEN_TYPE_STR,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex_pp octal escape sequence"),
            .fn = test_aven_c_lex_pp,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("\"\\17\""),
                .expected = slice_array(
                    (TestAvenCPpToken[]){
                        {
                            .val = aven_str("\"\\17\""),
                            .type = AVEN_C_PP_TOKEN_TYPE_STR,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex_pp hex escape sequence"),
            .fn = test_aven_c_lex_pp,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("\"color code: \\xFFAA22\""),
                .expected = slice_array(
                    (TestAvenCPpToken[]){
                        {
                            .val = aven_str("\"color code: \\xFFAA22\""),
                            .type = AVEN_C_PP_TOKEN_TYPE_STR,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex_pp invalid escape sequence"),
            .fn = test_aven_c_lex_pp,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("\"\\9\""),
                .expected = slice_array(
                    (TestAvenCPpToken[]){
                        {
                            .val = aven_str("\""),
                            .type = AVEN_C_PP_TOKEN_TYPE_OTH,
                        },
                        {
                            .val = aven_str("\\"),
                            .type = AVEN_C_PP_TOKEN_TYPE_OTH,
                        },
                        {
                            .val = aven_str("9"),
                            .type = AVEN_C_PP_TOKEN_TYPE_NUM,
                        },
                        {
                            .val = aven_str("\""),
                            .type = AVEN_C_PP_TOKEN_TYPE_OTH,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex_pp small block with comments"),
            .fn = test_aven_c_lex_pp,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str(
                    "int x = 0; // a comment with #\n"
                    "while (x < 2) {\n"
                    "    /*\n"
                    "     * here we are computing x + 1\n"
                    "     */\n"
                    "    x += 1;\n"
                    "}\n"
                ),
                .expected = slice_array(
                    (TestAvenCPpToken[]){
                        {
                            .val = aven_str("int"),
                            .type = AVEN_C_PP_TOKEN_TYPE_KEY,
                        },
                        {
                            .val = aven_str("x"),
                            .type = AVEN_C_PP_TOKEN_TYPE_ID,
                        },
                        {
                            .val = aven_str("="),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("0"),
                            .type = AVEN_C_PP_TOKEN_TYPE_NUM,
                        },
                        {
                            .val = aven_str(";"),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("while"),
                            .type = AVEN_C_PP_TOKEN_TYPE_KEY,
                        },
                        {
                            .val = aven_str("("),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("x"),
                            .type = AVEN_C_PP_TOKEN_TYPE_ID,
                        },
                        {
                            .val = aven_str("<"),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("2"),
                            .type = AVEN_C_PP_TOKEN_TYPE_NUM,
                        },
                        {
                            .val = aven_str(")"),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("{"),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("x"),
                            .type = AVEN_C_PP_TOKEN_TYPE_ID,
                        },
                        {
                            .val = aven_str("+="),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("1"),
                            .type = AVEN_C_PP_TOKEN_TYPE_NUM,
                        },
                        {
                            .val = aven_str(";"),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("}"),
                            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_pp_token_loc single declaration"),
            .fn = test_aven_c_pp_token_loc,
            .args = &(TestAvenCPpTokenLocArgs){
                .src = aven_str("int x;"),
                .expected = slice_array(
                    (TestAvenCPpTokenLoc[]){
                        {
                            .token = {
                                .val = aven_str("int"),
                                .type = AVEN_C_PP_TOKEN_TYPE_KEY,
                            },
                            .loc = {
                                .line = 1,
                                .col = 1,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("x"),
                                .type = AVEN_C_PP_TOKEN_TYPE_ID,
                            },
                            .loc = {
                                .line = 1,
                                .col = 5,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str(";"),
                                .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                            },
                            .loc = {
                                .line = 1,
                                .col = 6,
                            },
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex_pp invalid escape sequence"),
            .fn = test_aven_c_pp_token_loc,
            .args = &(TestAvenCPpTokenLocArgs){
                .src = aven_str("\"\\9\""),
                .expected = slice_array(
                    (TestAvenCPpTokenLoc[]){
                        {
                            .token = {
                                .val = aven_str("\""),
                                .type = AVEN_C_PP_TOKEN_TYPE_OTH,
                            },
                            .loc = {
                                .line = 1,
                                .col = 1,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("\\"),
                                .type = AVEN_C_PP_TOKEN_TYPE_OTH,
                            },
                            .loc = {
                                .line = 1,
                                .col = 2,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("9"),
                                .type = AVEN_C_PP_TOKEN_TYPE_NUM,
                            },
                            .loc = {
                                .line = 1,
                                .col = 3,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("\""),
                                .type = AVEN_C_PP_TOKEN_TYPE_OTH,
                            },
                            .loc = {
                                .line = 1,
                                .col = 4,
                            },
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex_pp invalid newline in string literal"),
            .fn = test_aven_c_pp_token_loc,
            .args = &(TestAvenCPpTokenLocArgs){
                .src = aven_str("\"Hello,\nWorld!\""),
                .expected = slice_array(
                    (TestAvenCPpTokenLoc[]){
                        {
                            .token = {
                                .val = aven_str("\""),
                                .type = AVEN_C_PP_TOKEN_TYPE_OTH,
                            },
                            .loc = {
                                .line = 1,
                                .col = 1,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("Hello"),
                                .type = AVEN_C_PP_TOKEN_TYPE_ID,
                            },
                            .loc = {
                                .line = 1,
                                .col = 2,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str(","),
                                .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                            },
                            .loc = {
                                .line = 1,
                                .col = 7,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("World"),
                                .type = AVEN_C_PP_TOKEN_TYPE_ID,
                            },
                            .loc = {
                                .line = 2,
                                .col = 1,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("!"),
                                .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                            },
                            .loc = {
                                .line = 2,
                                .col = 6,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("\""),
                                .type = AVEN_C_PP_TOKEN_TYPE_OTH,
                            },
                            .loc = {
                                .line = 2,
                                .col = 7,
                            },
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_pp_token_loc escaped newlines"),
            .fn = test_aven_c_pp_token_loc,
            .args = &(TestAvenCPpTokenLocArgs){
                .src = aven_str(
                    "#define Slice(T) struct {\\\n"
                    "        size_t len;\\\n"
                    "        T *ptr;\\\n"
                    "    }\n"
                ),
                .expected = slice_array(
                    (TestAvenCPpTokenLoc[]){
                        {
                            .token = {
                                .val = aven_str("#"),
                                .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                            },
                            .loc = {
                                .line = 1,
                                .col = 1,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("define"),
                                .type = AVEN_C_PP_TOKEN_TYPE_ID,
                            },
                            .loc = {
                                .line = 1,
                                .col = 2,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("Slice"),
                                .type = AVEN_C_PP_TOKEN_TYPE_ID,
                            },
                            .loc = {
                                .line = 1,
                                .col = 9,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("("),
                                .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                            },
                            .loc = {
                                .line = 1,
                                .col = 14,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("T"),
                                .type = AVEN_C_PP_TOKEN_TYPE_ID,
                            },
                            .loc = {
                                .line = 1,
                                .col = 15,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str(")"),
                                .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                            },
                            .loc = {
                                .line = 1,
                                .col = 16,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("struct"),
                                .type = AVEN_C_PP_TOKEN_TYPE_KEY,
                            },
                            .loc = {
                                .line = 1,
                                .col = 18,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("{"),
                                .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                            },
                            .loc = {
                                .line = 1,
                                .col = 25,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("size_t"),
                                .type = AVEN_C_PP_TOKEN_TYPE_ID,
                            },
                            .loc = {
                                .line = 2,
                                .col = 9,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("len"),
                                .type = AVEN_C_PP_TOKEN_TYPE_ID,
                            },
                            .loc = {
                                .line = 2,
                                .col = 16,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str(";"),
                                .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                            },
                            .loc = {
                                .line = 2,
                                .col = 19,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("T"),
                                .type = AVEN_C_PP_TOKEN_TYPE_ID,
                            },
                            .loc = {
                                .line = 3,
                                .col = 9,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("*"),
                                .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                            },
                            .loc = {
                                .line = 3,
                                .col = 11,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("ptr"),
                                .type = AVEN_C_PP_TOKEN_TYPE_ID,
                            },
                            .loc = {
                                .line = 3,
                                .col = 12,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str(";"),
                                .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                            },
                            .loc = {
                                .line = 3,
                                .col = 15,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("}"),
                                .type = AVEN_C_PP_TOKEN_TYPE_PNC,
                            },
                            .loc = {
                                .line = 4,
                                .col = 5,
                            },
                        },
                    }
                ),
            },
        },
    };

    AvenTestCaseSlice tcases = slice_array(tcase_data);

    aven_test(tcases, arena);

    return 0;
}

#endif // TEST_C_H

