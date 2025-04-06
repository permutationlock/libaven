#ifndef TEST_C_H
#define TEST_C_H

#include <aven.h>
#include <aven/arena.h>
#include <aven/c.h>
#include <aven/str.h>
#include <aven/test.h>

typedef struct {
    AvenStr val;
    AvenCPPTokenType type;
} TestAvenCPPToken;

typedef struct {
    AvenStr src;
    Slice(TestAvenCPPToken) expected;
} TestAvenCPPArgs;

static AvenTestResult test_aven_c_pp(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenCPPArgs *pp_args = args;

    AvenCPPTokenSet actual = aven_c_pp_lex(pp_args->src, &arena);
    if (actual.tokens.len != pp_args->expected.len) {
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
    for (size_t i = 0; i < actual.tokens.len; i += 1) {
        AvenCPPToken at = get(actual.tokens, i);
        TestAvenCPPToken et = get(pp_args->expected, i);

        AvenStr as = aven_str_range(actual.bytes, at.index, at.index + at.len);

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
                    aven_fmt_int(et.type),
                    aven_fmt_str(as),
                    aven_fmt_int(at.type)
                ),
            };
        }
    }

    return (AvenTestResult){ 0 };
}

static int test_c(AvenArena arena) {   
    AvenTestCase tcase_data[] = {
        {
            .desc = aven_str("aven_c_pp_lex empty file"),
            .fn = test_aven_c_pp,
            .args = &(TestAvenCPPArgs){
                .src = aven_str(""),
                .expected = { 0 },
            },
        },
        {
            .desc = aven_str("aven_c_pp_lex single declaration"),
            .fn = test_aven_c_pp,
            .args = &(TestAvenCPPArgs){
                .src = aven_str("int x;"),
                .expected = slice_array(
                    (TestAvenCPPToken[]){
                        {
                            .val = aven_str("int"),
                            .type = AVEN_C_PP_TOKEN_TYPE_ID,
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
            .desc = aven_str("aven_c_pp_lex header name with <>"),
            .fn = test_aven_c_pp,
            .args = &(TestAvenCPPArgs){
                .src = aven_str("#include <stdio.h>"),
                .expected = slice_array(
                    (TestAvenCPPToken[]){
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
            .desc = aven_str("aven_c_pp_lex header name with <>"),
            .fn = test_aven_c_pp,
            .args = &(TestAvenCPPArgs){
                .src = aven_str("#include \"helper.h\""),
                .expected = slice_array(
                    (TestAvenCPPToken[]){
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
            .desc = aven_str("aven_c_pp_lex small block"),
            .fn = test_aven_c_pp,
            .args = &(TestAvenCPPArgs){
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
                    (TestAvenCPPToken[]){
                        {
                            .val = aven_str("int"),
                            .type = AVEN_C_PP_TOKEN_TYPE_ID,
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
                            .type = AVEN_C_PP_TOKEN_TYPE_ID,
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
    };

    AvenTestCaseSlice tcases = slice_array(tcase_data);

    aven_test(tcases, arena);

    return 0;
}

#endif // TEST_C_H

