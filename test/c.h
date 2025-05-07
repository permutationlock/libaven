#ifndef TEST_C_H
#define TEST_C_H

#include <aven.h>
#include <aven/arena.h>
#include <aven/c.h>
#include <aven/str.h>
#include <aven/test.h>

typedef struct {
    AvenStr val;
    AvenCTokenType type;
} TestAvenCToken;

typedef struct {
    AvenStr src;
    Slice(TestAvenCToken) expected;
} TestAvenCLexPpArgs;

static AvenTestResult test_aven_c_lex(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenCLexPpArgs *pp_args = args;

    AvenCTokenSet tset = aven_c_lex(pp_args->src, &arena);
    AvenCTokenSlice actual = tset.tokens;
    if (actual.len != pp_args->expected.len + 1) {
        return (AvenTestResult){
            .error = 1,
            .message = aven_fmt(
                emsg_arena,
                "expected {} tokens, found {}",
                aven_fmt_uint(pp_args->expected.len + 1),
                aven_fmt_uint(actual.len)
            ),
        };
    }
    for (uint32_t i = 0; i < pp_args->expected.len; i += 1) {
        AvenCToken at = get(actual, i);
        TestAvenCToken et = get(pp_args->expected, i);

        AvenStr as = aven_c_token_str(tset, i);

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
                    aven_fmt_str(aven_c_token_type_str(et.type)),
                    aven_fmt_str(as),
                    aven_fmt_str(aven_c_token_type_str(at.type))
                ),
            };
        }
    }
    if (get(actual, actual.len - 1).type != AVEN_C_TOKEN_TYPE_NONE) {
        return (AvenTestResult){
            .error = 1,
            .message = aven_str("no terminating NONE token"),
        };
    }

    return (AvenTestResult){ 0 };
}

typedef struct {
    TestAvenCToken token;
    AvenCTokenLoc loc;
} TestAvenCTokenLoc;

typedef struct {
    AvenStr src;
    Slice(TestAvenCTokenLoc) expected;
} TestAvenCTokenLocArgs;

static AvenTestResult test_aven_c_token_loc(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenCTokenLocArgs *pp_args = args;

    AvenCTokenSet tset = aven_c_lex(pp_args->src, &arena);
    AvenCTokenSlice actual = tset.tokens;
    if (actual.len != pp_args->expected.len + 1) {
        return (AvenTestResult){
            .error = 1,
            .message = aven_fmt(
                emsg_arena,
                "expected {} tokens, found {}",
                aven_fmt_uint(pp_args->expected.len),
                aven_fmt_uint(actual.len)
            ),
        };
    }
    for (uint32_t i = 0; i < pp_args->expected.len; i += 1) {
        AvenCToken at = get(actual, i);
        AvenCTokenLoc al = aven_c_token_loc(tset, i);
        TestAvenCTokenLoc et = get(pp_args->expected, i);

        AvenStr as = aven_c_token_str(tset, i);
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
                    aven_fmt_str(aven_c_token_type_str(et.token.type)),
                    aven_fmt_str(as),
                    aven_fmt_str(aven_c_token_type_str(at.type))
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
                    aven_fmt_str(aven_c_token_type_str(et.token.type)),
                    aven_fmt_uint(et.loc.line),
                    aven_fmt_uint(et.loc.col),
                    aven_fmt_uint(al.line),
                    aven_fmt_uint(al.col)
                ),
            };
        }
    }
    if (get(actual, actual.len - 1).type != AVEN_C_TOKEN_TYPE_NONE) {
        return (AvenTestResult){
            .error = 1,
            .message = aven_str("no terminating NONE token"),
        };
    }

    return (AvenTestResult){ 0 };
}

typedef struct {
    AvenStr src;
    AvenStr expected;
    size_t line_len;
} TestAvenCAstRenderArgs;

static AvenTestResult test_aven_c_ast_render(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenCAstRenderArgs *fmt_args = args;

    AvenCTokenSet tset = aven_c_lex(fmt_args->src, &arena);
    AvenCAstResult ast_res = aven_c_ast_parse(tset, &arena);
    if (ast_res.type == AVEN_C_AST_RESULT_TYPE_ERROR) {
        return (AvenTestResult){
            .error = 1,
            .message = aven_fmt(
                emsg_arena,
                "encountered parse error: \"{}\"",
                aven_fmt_str(ast_res.data.error)
            ),
        };
    }
    AvenCAst ast = ast_res.data.ast;
    ByteSlice out_buffer = aven_arena_create_slice(
        unsigned char,
        &arena,
        8 * fmt_args->expected.len
    );
    AvenIoWriter writer = aven_io_writer_init_bytes(out_buffer);
    AvenCAstRenderResult ren_res =  aven_c_ast_render(
        &ast,
        &writer,
        fmt_args->line_len,
        aven_str("\n"),
        aven_str("    "),
        &arena
    );
    if (ren_res.error != AVEN_C_AST_RENDER_ERROR_NONE) {
        return (AvenTestResult){
            .error = 1,
            .message = aven_fmt(
                emsg_arena,
                "rendering error: {}",
                aven_fmt_str(ren_res.msg)
            ),
        };
    }
    ByteSlice written = slice_head(writer.buffer, writer.index);
    AvenStr actual = { .ptr = (char *)written.ptr, .len = written.len };
    if (!aven_str_equals(actual, fmt_args->expected)) {
        return (AvenTestResult){
            .error = 1,
            .message = aven_fmt(
                emsg_arena,
                "expected \"{}\", found \"{}\"",
                aven_fmt_str(fmt_args->expected),
                aven_fmt_str(actual)
            ),
        };
    }
    return (AvenTestResult){ 0 };
}

static int test_c(AvenArena arena) {
    AvenTestCase tcase_data[] = {
        {
            .desc = aven_str("aven_c_lex empty file"),
            .fn = test_aven_c_lex,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str(""),
                .expected = { 0 },
            },
        },
        {
            .desc = aven_str("aven_c_lex single declaration"),
            .fn = test_aven_c_lex,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("int x;"),
                .expected = slice_array(
                    (TestAvenCToken[]){
                        {
                            .val = aven_str("int"),
                            .type = AVEN_C_TOKEN_TYPE_KEY,
                        },
                        {
                            .val = aven_str("x"),
                            .type = AVEN_C_TOKEN_TYPE_ID,
                        },
                        {
                            .val = aven_str(";"),
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex preprocessor directive"),
            .fn = test_aven_c_lex,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("#include <stdio.h>"),
                .expected = slice_array(
                    (TestAvenCToken[]){
                        {
                            .val = aven_str("#include <stdio.h>"),
                            .type = AVEN_C_TOKEN_TYPE_PPD,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex weird pp-number"),
            .fn = test_aven_c_lex,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("int x = 0xE+12;\n"),
                .expected = slice_array(
                    (TestAvenCToken[]){
                        {
                            .val = aven_str("int"),
                            .type = AVEN_C_TOKEN_TYPE_KEY,
                        },
                        {
                            .val = aven_str("x"),
                            .type = AVEN_C_TOKEN_TYPE_ID,
                        },
                        {
                            .val = aven_str("="),
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("0xE+12"),
                            .type = AVEN_C_TOKEN_TYPE_NUM,
                        },
                        {
                            .val = aven_str(";"),
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex whitespace breaks pp-number"),
            .fn = test_aven_c_lex,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("int x = 0xE +12;\n"),
                .expected = slice_array(
                    (TestAvenCToken[]){
                        {
                            .val = aven_str("int"),
                            .type = AVEN_C_TOKEN_TYPE_KEY,
                        },
                        {
                            .val = aven_str("x"),
                            .type = AVEN_C_TOKEN_TYPE_ID,
                        },
                        {
                            .val = aven_str("="),
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("0xE"),
                            .type = AVEN_C_TOKEN_TYPE_NUM,
                        },
                        {
                            .val = aven_str("+"),
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("12"),
                            .type = AVEN_C_TOKEN_TYPE_NUM,
                        },
                        {
                            .val = aven_str(";"),
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex simple escape sequence"),
            .fn = test_aven_c_lex,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("\"Hello, World!\\n\""),
                .expected = slice_array(
                    (TestAvenCToken[]){
                        {
                            .val = aven_str("\"Hello, World!\\n\""),
                            .type = AVEN_C_TOKEN_TYPE_STR,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex octal escape sequence"),
            .fn = test_aven_c_lex,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("\"\\17\""),
                .expected = slice_array(
                    (TestAvenCToken[]){
                        {
                            .val = aven_str("\"\\17\""),
                            .type = AVEN_C_TOKEN_TYPE_STR,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex hex escape sequence"),
            .fn = test_aven_c_lex,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("\"color code: \\xFFAA22\""),
                .expected = slice_array(
                    (TestAvenCToken[]){
                        {
                            .val = aven_str("\"color code: \\xFFAA22\""),
                            .type = AVEN_C_TOKEN_TYPE_STR,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex invalid escape sequence"),
            .fn = test_aven_c_lex,
            .args = &(TestAvenCLexPpArgs){
                .src = aven_str("\"\\9\""),
                .expected = slice_array(
                    (TestAvenCToken[]){
                        {
                            .val = aven_str("\""),
                            .type = AVEN_C_TOKEN_TYPE_OTH,
                        },
                        {
                            .val = aven_str("\\"),
                            .type = AVEN_C_TOKEN_TYPE_OTH,
                        },
                        {
                            .val = aven_str("9"),
                            .type = AVEN_C_TOKEN_TYPE_NUM,
                        },
                        {
                            .val = aven_str("\""),
                            .type = AVEN_C_TOKEN_TYPE_OTH,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_lex small block with comments"),
            .fn = test_aven_c_lex,
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
                    (TestAvenCToken[]){
                        {
                            .val = aven_str("int"),
                            .type = AVEN_C_TOKEN_TYPE_KEY,
                        },
                        {
                            .val = aven_str("x"),
                            .type = AVEN_C_TOKEN_TYPE_ID,
                        },
                        {
                            .val = aven_str("="),
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("0"),
                            .type = AVEN_C_TOKEN_TYPE_NUM,
                        },
                        {
                            .val = aven_str(";"),
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("// a comment with #"),
                            .type = AVEN_C_TOKEN_TYPE_CMT,
                        },
                        {
                            .val = aven_str("while"),
                            .type = AVEN_C_TOKEN_TYPE_KEY,
                        },
                        {
                            .val = aven_str("("),
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("x"),
                            .type = AVEN_C_TOKEN_TYPE_ID,
                        },
                        {
                            .val = aven_str("<"),
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("2"),
                            .type = AVEN_C_TOKEN_TYPE_NUM,
                        },
                        {
                            .val = aven_str(")"),
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("{"),
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str(
                                "/*\n"
                                "     * here we are computing x + 1\n"
                                "     */"
                            ),
                            .type = AVEN_C_TOKEN_TYPE_CMT,
                        },
                        {
                            .val = aven_str("x"),
                            .type = AVEN_C_TOKEN_TYPE_ID,
                        },
                        {
                            .val = aven_str("+="),
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("1"),
                            .type = AVEN_C_TOKEN_TYPE_NUM,
                        },
                        {
                            .val = aven_str(";"),
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                        },
                        {
                            .val = aven_str("}"),
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_token_loc single declaration"),
            .fn = test_aven_c_token_loc,
            .args = &(TestAvenCTokenLocArgs){
                .src = aven_str("int x;"),
                .expected = slice_array(
                    (TestAvenCTokenLoc[]){
                        {
                            .token = {
                                .val = aven_str("int"),
                                .type = AVEN_C_TOKEN_TYPE_KEY,
                            },
                            .loc = {
                                .line = 1,
                                .col = 1,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("x"),
                                .type = AVEN_C_TOKEN_TYPE_ID,
                            },
                            .loc = {
                                .line = 1,
                                .col = 5,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str(";"),
                                .type = AVEN_C_TOKEN_TYPE_PNC,
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
            .desc = aven_str("aven_c_lex invalid escape sequence"),
            .fn = test_aven_c_token_loc,
            .args = &(TestAvenCTokenLocArgs){
                .src = aven_str("\"\\9\""),
                .expected = slice_array(
                    (TestAvenCTokenLoc[]){
                        {
                            .token = {
                                .val = aven_str("\""),
                                .type = AVEN_C_TOKEN_TYPE_OTH,
                            },
                            .loc = {
                                .line = 1,
                                .col = 1,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("\\"),
                                .type = AVEN_C_TOKEN_TYPE_OTH,
                            },
                            .loc = {
                                .line = 1,
                                .col = 2,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("9"),
                                .type = AVEN_C_TOKEN_TYPE_NUM,
                            },
                            .loc = {
                                .line = 1,
                                .col = 3,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("\""),
                                .type = AVEN_C_TOKEN_TYPE_OTH,
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
            .desc = aven_str("aven_c_lex invalid newline in string literal"),
            .fn = test_aven_c_token_loc,
            .args = &(TestAvenCTokenLocArgs){
                .src = aven_str("\"Hello,\nWorld!\""),
                .expected = slice_array(
                    (TestAvenCTokenLoc[]){
                        {
                            .token = {
                                .val = aven_str("\""),
                                .type = AVEN_C_TOKEN_TYPE_OTH,
                            },
                            .loc = {
                                .line = 1,
                                .col = 1,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("Hello"),
                                .type = AVEN_C_TOKEN_TYPE_ID,
                            },
                            .loc = {
                                .line = 1,
                                .col = 2,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str(","),
                                .type = AVEN_C_TOKEN_TYPE_PNC,
                            },
                            .loc = {
                                .line = 1,
                                .col = 7,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("World"),
                                .type = AVEN_C_TOKEN_TYPE_ID,
                            },
                            .loc = {
                                .line = 2,
                                .col = 1,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("!"),
                                .type = AVEN_C_TOKEN_TYPE_PNC,
                            },
                            .loc = {
                                .line = 2,
                                .col = 6,
                            },
                        },
                        {
                            .token = {
                                .val = aven_str("\""),
                                .type = AVEN_C_TOKEN_TYPE_OTH,
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
            .desc = aven_str("aven_c_token_loc escaped newlines"),
            .fn = test_aven_c_token_loc,
            .args = &(TestAvenCTokenLocArgs){
                .src = aven_str(
                    "#define Slice(T) struct {\\\n"
                    "        size_t len;\\\n"
                    "        T *ptr;\\\n"
                    "    }\n"
                ),
                .expected = slice_array(
                    (TestAvenCTokenLoc[]){
                        {
                            .token = {
                                .val = aven_str(
                                    "#define Slice(T) struct {\\\n"
                                    "        size_t len;\\\n"
                                    "        T *ptr;\\\n"
                                    "    }"
                                ),
                                .type = AVEN_C_TOKEN_TYPE_PPD,
                            },
                            .loc = {
                                .line = 1,
                                .col = 1,
                            },
                        },
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_c_ast_render int declaration"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int x;"),
                .expected = aven_str("int x;\n"),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render int declaration init expr"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int x = 2 + 2;\n"),
                .expected = aven_str("int x = 2 + 2;\n"),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render char declaration init expr"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("char c = '\\n';\n"),
                .expected = aven_str("char c = '\\n';\n"),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render expression split same op add"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("long x = 2 + 2 * 4 - 7;"),
                .expected = aven_str(
                    "long x = 2 +\n"
                    "    2 * 4 -\n"
                    "    7;\n"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render expression split same op mul"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int x = 10 - 2 * 2 * 4 - 7;\n"),
                .expected = aven_str(
                    "int x = 10 -\n"
                    "    2 * 2 * 4 -\n"
                    "    7;\n"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render expression split op same indent"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int x = 10 - 2 * 2 * 4 - 7;\n"),
                .expected = aven_str(
                    "int x = 10 -\n"
                    "    2 * 2 * 4 -\n"
                    "    7;\n"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render expression split same op mul"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int x = 10 - 2 * 2 * 4 - 7 = 32 + 7 + 14;\n"),
                .expected = aven_str(
                    "int x = 10 -\n"
                    "    2 * 2 * 4 -\n"
                    "    7 = 32 +\n"
                    "    7 +\n"
                    "    14;\n"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render ternary expression"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("bool x = true ? 1 : 0;\n"),
                .expected = aven_str(
                    "bool x = true ? 1 : 0;\n"
                ),
                .line_len = 24,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render ternary expression split"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("bool x = true ? 1 : 0;\n"),
                .expected = aven_str(
                    "bool x = true ?\n"
                    "    1 :\n"
                    "    0;\n"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render postfix '.'"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int error = result.error;\n"),
                .expected = aven_str("int error = result.error;\n"),
                .line_len = 36,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render postfix []"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int error = arr[2];\n"),
                .expected = aven_str("int error = arr[2];\n"),
                .line_len = 36,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render postfix [] and ()"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int error = arr[2](x,y);\n"),
                .expected = aven_str("int error = arr[2](x, y);\n"),
                .line_len = 36,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render comma expression"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int x = (10 - 2 * 2 * 4 - 7, 32 + 7 + 14);\n"),
                .expected = aven_str("int x = (10 - 2 * 2 * 4 - 7, 32 + 7 + 14);\n"),
                .line_len = 48,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render comma expression split"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int x = (10 - 2 * 2 * 4 - 7, 32 + 7 + 14);\n"),
                .expected = aven_str(
                    "int x = (\n"
                    "    10 -\n"
                    "        2 * 2 * 4 -\n"
                    "        7,\n"
                    "    32 + 7 + 14\n"
                    ");\n"
                ),
                .line_len = 19,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render complex paren assign expression split"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int x = (10 - 2 * 2 * 4 - 7, 32 + 7 + 14) = 1 + 2 + 3 + 4 + 5;\n"),
                .expected = aven_str(
                    "int x = (\n"
                    "    10 -\n"
                    "        2 * 2 * 4 -\n"
                    "        7,\n"
                    "    32 + 7 + 14\n"
                    ") = 1 +\n"
                    "    2 +\n"
                    "    3 +\n"
                    "    4 +\n"
                    "    5;\n"
                ),
                .line_len = 19,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render sizeof operator"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "const size_t x = 2 + sizeof(int);\n"
                ),
                .expected = aven_str(
                    "const size_t x = 2 + sizeof(int);\n"
                ),
                .line_len = 32,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render _Atomic specifier"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "_Atomic(size_t) x;\n"
                ),
                .expected = aven_str(
                    "_Atomic(size_t) x;\n"
                ),
                .line_len = 32,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render _Alignas specifier"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "_Alignas(1) uint64_t x;\n"
                ),
                .expected = aven_str(
                    "_Alignas(1) uint64_t x;\n"
                ),
                .line_len = 32,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render static assert"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "_Static_assert(x == 2, \"OHNO\");\n"
                ),
                .expected = aven_str(
                    "_Static_assert(x == 2, \"OHNO\");\n"
                ),
                .line_len = 32,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render struct initializer postfix"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int x = (struct T){ .x = 7 };\n"),
                .expected = aven_str("int x = (struct T){ .x = 7 };\n"),
                .line_len = 32,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render struct initializer postfix short"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int x = (struct T){ .x = 7 };\n"),
                .expected = aven_str(
                    "int x = (struct T){\n"
                    "    .x = 7,\n"
                    "};\n"
                ),
                .line_len = 24,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render leading comment"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "// Hello World!\n"
                    "int x = 2 + 2;\n"
                ),
                .expected = aven_str(
                    "// Hello World!\n"
                    "int x = 2 + 2;\n"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render trailing comment"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int x = 2 + 2; // Hello World!\n"),
                .expected = aven_str(
                    "int x = 2 + 2;\n"
                    "// Hello World!\n"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render internal block comment"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int x = 2 + /* add */ 2;\n"),
                .expected = aven_str(
                    "int x = 2 +\n"
                    "    /* add */\n"
                    "    2;\n"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render empty line after comment"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "// Hello World!\n\n"
                    "int x = 2 + 2;\n"
                ),
                .expected = aven_str(
                    "// Hello World!\n\n"
                    "int x = 2 + 2;\n"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render empty line before comment"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "int x = 2 + 2;\n\n"
                    "// Hello World!\n"
                    "int y = x;\n"
                ),
                .expected = aven_str(
                    "int x = 2 + 2;\n\n"
                    "// Hello World!\n"
                    "int y = x;\n"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render pp define simple const expr"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("#define NUM 1"),
                .expected = aven_str("#define NUM 1\n"),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render pp define fn const expr"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("#define ADD1(n) (n + 1)"),
                .expected = aven_str("#define ADD1(n) (n + 1)\n"),
                .line_len = 25,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render pp define fn const expr wrap body"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("#define ADD1(n) (n + 1)"),
                .expected = aven_str("#define ADD1(n) ( \\\n        n + 1 \\\n    )\n"),
                .line_len = 18,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render pp define fn const expr vrap def"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("#define ADD1(n) (n + 1)"),
                .expected = aven_str("#define ADD1( \\\n        n \\\n    ) (n + 1)\n"),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render comment in pp directive"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("#define NUM 1 // number of entries\n"),
                .expected = aven_str("#define NUM 1\n// number of entries\n"),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render multi-line comment in pp directive"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "#define NUM 1 /* number of\n"
                    "                 entries */\n"
                ),
                .expected = aven_str(
                    "#define NUM 1\n"
                    "/* number of\n"
                    "                 entries */\n"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render pathological cast expr"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int x = (a) - (b);"),
                .expected = aven_str("int x = (a)-(b);\n"),
                .line_len = 36,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render pathological cast expr \"fixed\""),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("int x = ((a)) - (b);"),
                .expected = aven_str("int x = ((a)) - (b);\n"),
                .line_len = 36,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render string literal"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("char str[] = \"Hello, World!\";\n"),
                .expected = aven_str("char str[] = \"Hello, World!\";\n"),
                .line_len = 32,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render compound string literal"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("char str[] = \"Hello,\" \" World!\";\n"),
                .expected = aven_str("char str[] = \"Hello,\" \" World!\";\n"),
                .line_len = 36,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render compound string literal short line"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("char str[] = \"Hello,\" \" World!\";\n"),
                .expected = aven_str("char str[] = \"Hello,\"\n    \" World!\";\n"),
                .line_len = 24,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render compound string literal param"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("void *p = make(\"Hello,\" \" World!\");\n"),
                .expected = aven_str("void *p = make(\"Hello,\" \" World!\");\n"),
                .line_len = 36,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render compound string literal param short line"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("void *p = make(\"Hello,\" \" World!\");\n"),
                .expected = aven_str(
                    "void *p = make(\n"
                    "    \"Hello,\"\n"
                    "    \" World!\"\n"
                    ");\n"
                ),
                .line_len = 20,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render compound string literal w/macro"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("const char *str = MY_STR \" World!\";"),
                .expected = aven_str("const char *str = MY_STR \" World!\";\n"),
                .line_len = 36,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render compound string literal w/macro short"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("const char *str = MY_STR \" World!\";"),
                .expected = aven_str("const char *str = MY_STR\n    \" World!\";\n"),
                .line_len = 28,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render type pointer render as decl not expr"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("MyStruct *s;\n"),
                .expected = aven_str("MyStruct *s;\n"),
                .line_len = 36,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render initializer list w/pointer cast"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "static const AvenStrSlice aven_c_keywords = {\n"
                    "    .ptr = (AvenStr *)aven_c_keyword_data,\n"
                    "    .len = countof(aven_c_keyword_data),\n"
                    "};\n"
                ),
                .expected = aven_str(
                    "static const AvenStrSlice aven_c_keywords = {\n"
                    "    .ptr = (AvenStr *)aven_c_keyword_data,\n"
                    "    .len = countof(aven_c_keyword_data),\n"
                    "};\n"
                ),
                .line_len = 80,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render enum typedef"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "typedef enum {\n"
                    "    AVEN_ARG_TYPE_BOOL = 0,\n"
                    "    AVEN_ARG_TYPE_INT,\n"
                    "    AVEN_ARG_TYPE_STRING\n"
                    "} AvenArgType;\n"
                ),
                .expected = aven_str(
                    "typedef enum {\n"
                    "    AVEN_ARG_TYPE_BOOL = 0,\n"
                    "    AVEN_ARG_TYPE_INT,\n"
                    "    AVEN_ARG_TYPE_STRING,\n"
                    "} AvenArgType;\n"
                ),
                .line_len = 80,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render for statement"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "void bar(int n) {\n"
                    "    for (int x = 0; x < n; x++) {\n"
                    "        foo(x);\n"
                    "    }\n"
                    "}\n"
                ),
                .expected = aven_str(
                    "void bar(int n) {\n"
                    "    for (int x = 0; x < n; x++) {\n"
                    "        foo(x);\n"
                    "    }\n"
                    "}\n"
                ),
                .line_len = 80,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render array designators"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "static const AvenStr aven_c_keyword_data[] = {\n"
                    "    [AVEN_C_KEYWORD_AUTO] = aven_str_init(\"auto\"),\n"
                    "    [AVEN_C_KEYWORD_BREAK] = aven_str_init(\"break\"),\n"
                    "    [AVEN_C_KEYWORD_CASE] = aven_str_init(\"case\"),\n"
                    "    [AVEN_C_KEYWORD_CHAR] = aven_str_init(\"char\"),\n"
                    "    [AVEN_C_KEYWORD_CONST] = aven_str_init(\"const\")\n"
                    "};\n"
                ),
                .expected = aven_str(
                    "static const AvenStr aven_c_keyword_data[] = {\n"
                    "    [AVEN_C_KEYWORD_AUTO] = aven_str_init(\"auto\"),\n"
                    "    [AVEN_C_KEYWORD_BREAK] = aven_str_init(\"break\"),\n"
                    "    [AVEN_C_KEYWORD_CASE] = aven_str_init(\"case\"),\n"
                    "    [AVEN_C_KEYWORD_CHAR] = aven_str_init(\"char\"),\n"
                    "    [AVEN_C_KEYWORD_CONST] = aven_str_init(\"const\"),\n"
                    "};\n"
                ),
                .line_len = 80,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render multi-line do pp directive"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "#define loop(f, a) do {\\\n"
                    "        a = f;\\\n"
                    "    } while (a == 0)\n"
                ),
                .expected = aven_str(
                    "#define loop(f, a) do { \\\n"
                    "        a = f; \\\n"
                    "    } while (a == 0)\n"
                ),
                .line_len = 28,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render multi-line pp directive"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "#define Slice(T) struct {\\\n"
                    "        size_t len;\\\n"
                    "        T *ptr;\\\n"
                    "    }\n"
                ),
                .expected = aven_str(
                    "#define Slice(T) struct { \\\n"
                    "        size_t len; \\\n"
                    "        T *ptr; \\\n"
                    "    }\n"
                ),
                .line_len = 28,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render pp directive with stringify"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "#define assert(c) ((!(c)) ? aven_panic(\"assert(\" #c \") failed\") : (void)0)\n"
                ),
                .expected = aven_str(
                    "#define assert(c) ((!(c)) ? aven_panic(\"assert(\" #c \") failed\") : (void)0)\n"
                ),
                .line_len = 80,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render pp directive do statement w/return"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "#define aven_c_lex_next(c, i) do { \\\n"
                    "        if (!aven_c_lex_next_internal(c)) { \\\n"
                    "            c->index = i; \\\n"
                    "            return false; \\\n"
                    "        } \\\n"
                    "    } while (0)\n"
                ),
                .expected = aven_str(
                    "#define aven_c_lex_next(c, i) do { \\\n"
                    "        if (!aven_c_lex_next_internal(c)) { \\\n"
                    "            c->index = i; \\\n"
                    "            return false; \\\n"
                    "        } \\\n"
                    "    } while (0)\n"
                ),
                .line_len = 80,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render function definition"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "void main(void) { printf(\"Hello, World!\"); }\n"
                ),
                .expected = aven_str(
                    "void main(void) {\n"
                    "    printf(\"Hello, World!\");\n"
                    "}\n"
                ),
                .line_len = 40,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render function definition w/args"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "void main(int argc, const char **argv) { printf(\"Hello, World!\"); }\n"
                ),
                .expected = aven_str(
                    "void main(\n"
                    "    int argc,\n"
                    "    const char **argv\n"
                    ") {\n"
                    "    printf(\"Hello, World!\");\n"
                    "}\n"
                ),
                .line_len = 36,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render complex function definition"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "AVEN_FN void *aven_arena_alloc(\n"
                    "    AvenArena *arena,\n"
                    "    size_t count, size_t align,\n"
                    "\n"
                    "    size_t size\n"
                    ") {\n"
                    "\n"
                    "    assert((align & (align - 1)) == 0);\n"
                    "\n"
                    "    ptrdiff_t padding = (ptrdiff_t)(-(uintptr_t)arena->base & (align - 1));\n"
                    "    ptrdiff_t available = arena->top - arena->base - padding;\n"
                    "\n"
                    "\n"
                    "\n"
                    "    if (available < 0 || count > ((size_t)available / size)) {\n"
                    "            // OOM unrecoverable, panic\n"
                    "            aven_panic(\"arena out of memory\");\n"
                    "    }\n"
                    "\n"
                    "\n"
                    "    void *ptr = arena->base + padding;\n"
                    "    arena->base += (size_t)padding + size * count;\n\n"
                    "    return ptr;\n\n"
                    "}\n"
                ),
                .expected = aven_str(
                    "AVEN_FN void *aven_arena_alloc(\n"
                    "    AvenArena *arena,\n"
                    "    size_t count,\n"
                    "    size_t align,\n"
                    "    size_t size\n"
                    ") {\n"
                    "    assert((align & (align - 1)) == 0);\n"
                    "\n"
                    "    ptrdiff_t padding = (ptrdiff_t)(-(uintptr_t)arena->base & (align - 1));\n"
                    "    ptrdiff_t available = arena->top - arena->base - padding;\n"
                    "\n"
                    "    if (available < 0 || count > ((size_t)available / size)) {\n"
                    "        // OOM unrecoverable, panic\n"
                    "        aven_panic(\"arena out of memory\");\n"
                    "    }\n"
                    "\n"
                    "    void *ptr = arena->base + padding;\n"
                    "    arena->base += (size_t)padding + size * count;\n\n"
                    "    return ptr;\n"
                    "}\n"
                ),
                .line_len = 80,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render function definition w/switch"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "void collatz(int n) {\n"
                    "    switch (n % 2) {\n"
                    "        case 0: {\n"
                    "            return n / 2;\n"
                    "            break;\n"
                    "        }\n"
                    "        default: {\n"
                    "            return 3 * n + 1;\n"
                    "            break;\n"
                    "        }\n"
                    "    }\n"
                    "}\n"
                ),
                .expected = aven_str(
                    "void collatz(int n) {\n"
                    "    switch (n % 2) {\n"
                    "        case 0: {\n"
                    "            return n / 2;\n"
                    "            break;\n"
                    "        }\n"
                    "        default: {\n"
                    "            return 3 * n + 1;\n"
                    "            break;\n"
                    "        }\n"
                    "    }\n"
                    "}\n"
                ),
                .line_len = 48,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render function definition w/while"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "void main(void) {\n"
                    "    char c = 0;\n"
                    "    while (c != '\\n') {\n"
                    "        c = getc();\n"
                    "    }\n"
                    "}\n"
                ),
                .expected = aven_str(
                    "void main(void) {\n"
                    "    char c = 0;\n"
                    "    while (c != '\\n') {\n"
                    "        c = getc();\n"
                    "    }\n"
                    "}\n"
                ),
                .line_len = 48,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render function definition inside ifdef"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "#ifndef A\n"
                    "#define A\n"
                    "\n"
                    "void main(int argc, const char **argv) {\n"
                    "#if (X > 0)\n"
                    "    printf(\"Hello, World!\");\n"
                    "#endif\n"
                    "}\n"
                    "#endif\n"
                    "void main(\n"
                    "    int argc,\n"
                    "    const char **argv\n"
                    ") {\n"
                    "#ifdef A\n"
                    "    printf(\"Hello, World!\");\n"
                    "#endif\n"
                    "}\n"
                ),
                .expected = aven_str(
                    "#ifndef A\n"
                    "    #define A\n"
                    "\n"
                    "    void main(\n"
                    "        int argc,\n"
                    "        const char **argv\n"
                    "    ) {\n"
                    "    #if (X > 0)\n"
                    "        printf(\"Hello, World!\");\n"
                    "    #endif\n"
                    "    }\n"
                    "#endif\n"
                    "void main(\n"
                    "    int argc,\n"
                    "    const char **argv\n"
                    ") {\n"
                    "#ifdef A\n"
                    "    printf(\"Hello, World!\");\n"
                    "#endif\n"
                    "}\n"
                ),
                .line_len = 36,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render function definition w/args"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "int foo(\n"
                    "    int argc,\n"
                    "#ifndef A\n"
                    "    const char **argv,\n"
                    "#else\n"
                    "    const long **argv,\n"
                    "#endif\n"
                    "    bool split\n"
                    ") {\n"
                    "    printf(\"Hello, World!\");\n"
                    "}\n"
                ),
                .expected = aven_str(
                    "int foo(\n"
                    "    int argc,\n"
                    "#ifndef A\n"
                    "    const char **argv,\n"
                    "#else\n"
                    "    const long **argv,\n"
                    "#endif\n"
                    "    bool split\n"
                    ") {\n"
                    "    printf(\"Hello, World!\");\n"
                    "}\n"
                ),
                .line_len = 36,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render local include directive"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "#include \"aven.h\"\n"
                ),
                .expected = aven_str(
                    "#include \"aven.h\"\n"
                ),
                .line_len = 36,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render system include directive"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "#include <aven.h>\n"
                ),
                .expected = aven_str(
                    "#include <aven.h>\n"
                ),
                .line_len = 36,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render init decl initializer list"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "AvenCAstRenderCtx ctx = {\n"
                    "    .ast = ast,\n"
                    "    .writer = writer,\n"
                    "    .line = aven_arena_create_slice(\n"
                    "        char,\n"
                    "        &temp_arena,\n"
                    "        line_len + 2\n"
                    "    ),\n"
                    "    .newline_str = newline_str,\n"
                    "    .indent_str = indent_str,\n"
                    "};\n"
                ),
                .expected = aven_str(
                    "AvenCAstRenderCtx ctx = {\n"
                    "    .ast = ast,\n"
                    "    .writer = writer,\n"
                    "    .line =\n"
                    "        aven_arena_create_slice(\n"
                    "            char,\n"
                    "            &temp_arena,\n"
                    "            line_len + 2\n"
                    "        ),\n"
                    "    .newline_str = newline_str,\n"
                    "    .indent_str = indent_str,\n"
                    "};\n"
                ),
                .line_len = 34,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render fn w/comment at start of block"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "static inline AvenStr aven_c_token_str(AvenCTokenSet tset, uint32_t index) {\n"
                    "    // Grab entire text chunk for all tokens within directive\n"
                    "    AvenCToken t1 = get(tset.ppd_tokens, token.index);\n"
                    "    AvenCToken tn = get(tset.ppd_tokens, token.index + (token.len - 1));\n"
                    "    return aven_str_range(tset.bytes, t1.index, tn.index + tn.len);\n"
                    "}\n"
                ),
                .expected = aven_str(
                    "static inline AvenStr aven_c_token_str(AvenCTokenSet tset, uint32_t index) {\n"
                    "    // Grab entire text chunk for all tokens within directive\n"
                    "    AvenCToken t1 = get(tset.ppd_tokens, token.index);\n"
                    "    AvenCToken tn = get(tset.ppd_tokens, token.index + (token.len - 1));\n"
                    "    return aven_str_range(tset.bytes, t1.index, tn.index + tn.len);\n"
                    "}\n"
                ),
                .line_len = 80,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render system include directive"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str(
                    "#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)\n"
                    "    #define _POSIX_C_SOURCE 200112L\n"
                    "#endif\n"
                    "#define AVEN_IMPLEMENTATION\n"
                    "#include <aven.h>\n"
                    "#include <aven/c.h>\n"
                    "#include <aven/fmt.h>\n"
                    "#include <aven/io.h>\n"
                    "\n"
                    "#include <stdlib.h>\n"
                    "\n"
                    "// Enough to handle pathological files up to ~10MB, and much larger real files\n"
                    "#define ARENA_SIZE (4096 * 250000)\n"
                    "\n"
                    "int main(void) {\n"
                    "    void *mem = malloc(ARENA_SIZE);\n"
                    "    if (mem == NULL) {\n"
                    "        aven_panic(\"malloc failed\\n\");\n"
                    "    }\n"
                    "    AvenArena arena = aven_arena_init(mem, ARENA_SIZE);\n"
                    "    AvenIoWriter stdout = aven_io_writer_init_stdout_buffered(8192, &arena);\n"
                    "    AvenCFmtResult fmt_res = aven_c_fmt(\n"
                    "            &aven_io_stdin,\n"
                    "            &stdout,\n"
                    "#ifdef A\n"
                    "            &arena,\n"
                    "#else\n"
                    "            &arena\n"
                    "#endif\n"
                    "        );\n"
                    "    aven_io_writer_flush(&stdout);\n"
                    "    if (fmt_res.error != AVEN_C_FMT_ERROR_NONE) {\n"
                    "        aven_io_perrf(\n"
                    "                \"error: {}\\n\",\n"
                    "                aven_fmt_str(fmt_res.msg)\n"
                    "            );\n"
                    "        return 1;\n"
                    "    }\n"
                    "    return 0;\n"
                    "}\n"
                ),
                .expected = aven_str(
                    "#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)\n"
                    "    #define _POSIX_C_SOURCE 200112L\n"
                    "#endif\n"
                    "#define AVEN_IMPLEMENTATION\n"
                    "#include <aven.h>\n"
                    "#include <aven/c.h>\n"
                    "#include <aven/fmt.h>\n"
                    "#include <aven/io.h>\n"
                    "\n"
                    "#include <stdlib.h>\n"
                    "\n"
                    "// Enough to handle pathological files up to ~10MB, and much larger real files\n"
                    "#define ARENA_SIZE (4096 * 250000)\n"
                    "\n"
                    "int main(void) {\n"
                    "    void *mem = malloc(ARENA_SIZE);\n"
                    "    if (mem == NULL) {\n"
                    "        aven_panic(\"malloc failed\\n\");\n"
                    "    }\n"
                    "    AvenArena arena = aven_arena_init(mem, ARENA_SIZE);\n"
                    "    AvenIoWriter stdout = aven_io_writer_init_stdout_buffered(8192, &arena);\n"
                    "    AvenCFmtResult fmt_res = aven_c_fmt(\n"
                    "        &aven_io_stdin,\n"
                    "        &stdout,\n"
                    "#ifdef A\n"
                    "        &arena,\n"
                    "#else\n"
                    "        &arena\n"
                    "#endif\n"
                    "    );\n"
                    "    aven_io_writer_flush(&stdout);\n"
                    "    if (fmt_res.error != AVEN_C_FMT_ERROR_NONE) {\n"
                    "        aven_io_perrf(\"error: {}\\n\", aven_fmt_str(fmt_res.msg));\n"
                    "        return 1;\n"
                    "    }\n"
                    "    return 0;\n"
                    "}\n"
                ),
                .line_len = 80,
            },
        },
    };

    AvenTestCaseSlice tcases = slice_array(tcase_data);

    aven_test(tcases, arena);

    return 0;
}

#endif // TEST_C_H

