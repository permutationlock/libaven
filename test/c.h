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
        AvenCTokenLoc al = aven_c_token_loc(at, pp_args->src);
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
    AvenCAst ast = aven_c_ast_parse(tset, &arena);
    ByteSlice out_buffer = aven_arena_create_slice(
        unsigned char,
        &arena,
        8 * fmt_args->expected.len
    );
    AvenIoWriter writer = aven_io_writer_init_bytes(out_buffer);
    int error =  aven_c_ast_render(
        &ast,
        &writer,
        fmt_args->line_len,
        aven_str("\n"),
        aven_str("    "),
        arena
    );
    if (error != 0) {
        return (AvenTestResult){
            .error = 1,
            .message = aven_fmt(
                emsg_arena,
                "encountered io error: {}",
                aven_fmt_int(error)
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
            .desc = aven_str("aven_c_ast_render expression"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("x = 2 + 2"),
                .expected = aven_str("x = 2 + 2"),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render expression split same op add"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("x = 2 + 2 * 4 - 7"),
                .expected = aven_str(
                    "x = 2 +\n"
                    "    2 * 4 -\n"
                    "    7"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render expression split same op mul"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("x = 10 - 2 * 2 * 4 - 7\n"),
                .expected = aven_str(
                    "x = 10 -\n"
                        "    2 * 2 * 4 -\n"
                        "    7"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render expression split op same indent"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("10 - 2 * 2 * 4 - 7\n"),
                .expected = aven_str(
                    "10 -\n"
                    "2 * 2 * 4 -\n"
                    "7"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render expression split same op mul"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("x = 10 - 2 * 2 * 4 - 7 = 32 + 7 + 14\n"),
                .expected = aven_str(
                    "x = 10 -\n"
                    "    2 * 2 * 4 -\n"
                    "    7 = 32 +\n"
                    "    7 +\n"
                    "    14"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render ternary expression"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("x = true ? 1 : 0\n"),
                .expected = aven_str(
                    "x = true ? 1 : 0"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render ternary expression"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("x = true ? 1 : 0\n"),
                .expected = aven_str(
                    "x = true ?\n"
                    "    1 :\n"
                    "    0"
                ),
                .line_len = 12,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render comma expression "),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("x = (10 - 2 * 2 * 4 - 7, 32 + 7 + 14)\n"),
                .expected = aven_str(
                    "x = (\n"
                    "        10 -\n"
                    "        2 * 2 * 4 -\n"
                    "        7,\n"
                    "        32 + 7 + 14\n"
                    "    )"
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
                    "x = 2 + 2\n"
                ),
                .expected = aven_str(
                    "// Hello World!\n"
                    "x = 2 + 2"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render trailing comment"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("x = 2 + 2 // Hello World!\n"),
                .expected = aven_str(
                    "x = 2 + 2\n"
                    "// Hello World!\n"
                ),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render internal block comment"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("x = 2 + /* add */ 2\n"),
                .expected = aven_str(
                    "x = 2 +\n"
                    "    /* add */\n"
                    "    2"
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
                .line_len = 24,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render pp define fn const expr short line"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("#define ADD1(n) (n + 1)"),
                .expected = aven_str("#define ADD1(n) \\\n    (n + 1)\n"),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render pp define fn const expr middle line"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("#define ADD1(n) (n + 1)"),
                .expected = aven_str("#define ADD1(n) ( \\\n        n + 1 \\\n    )\n"),
                .line_len = 18,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render string literal"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("\"Hello, World!\""),
                .expected = aven_str("\"Hello, World!\""),
                .line_len = 18,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render compound string literal"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("\"Hello,\" \" World!\""),
                .expected = aven_str("\"Hello,\" \" World!\""),
                .line_len = 20,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render compound string literal short line"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("\"Hello,\" \" World!\""),
                .expected = aven_str("\"Hello,\"\n    \" World!\""),
                .line_len = 16,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render compound string literal w/macro"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("MY_STR \" World!\""),
                .expected = aven_str("MY_STR \" World!\""),
                .line_len = 18,
            },
        },
        {
            .desc = aven_str("aven_c_ast_render compound string literal in parens"),
            .fn = test_aven_c_ast_render,
            .args = &(TestAvenCAstRenderArgs){
                .src = aven_str("(\"Hello there,\" \" World!\")"),
                .expected = aven_str("(\n    \"Hello there,\"\n        \" World!\"\n)"),
                .line_len = 20,
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
    };

    AvenTestCaseSlice tcases = slice_array(tcase_data);

    aven_test(tcases, arena);

    return 0;
}

#endif // TEST_C_H

