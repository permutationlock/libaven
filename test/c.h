// aven fmt columns: 100
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

    static AvenTestResult test_aven_c_lex(AvenArena *emsg_arena, AvenArena arena, void *args) {
        TestAvenCLexPpArgs *pp_args = args;

        AvenCTokenSet tset = aven_c_lex(pp_args->src, &arena);
        AvenCTokenSlice actual = tset.tokens;
        if (get(actual, 0).type != AVEN_C_TOKEN_TYPE_NONE) {
            return (AvenTestResult){ .error = 1, .message = aven_str("no initial NONE token") };
        }
        if (actual.len != pp_args->expected.len + 2) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected {} tokens, found {}",
                    aven_fmt_uint(pp_args->expected.len + 2),
                    aven_fmt_uint(actual.len)
                ),
            };
        }
        for (uint32_t i = 0; i < pp_args->expected.len; i += 1) {
            AvenCToken at = get(actual, i + 1);
            TestAvenCToken et = get(pp_args->expected, i);

            AvenStr as = aven_c_token_str(tset, i + 1);

            if (at.type != et.type or !aven_str_equals(as, et.val)) {
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
            return (AvenTestResult){ .error = 1, .message = aven_str("no terminating NONE token") };
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

    static AvenTestResult test_aven_c_token_loc(AvenArena *emsg_arena, AvenArena arena, void *args) {
        TestAvenCTokenLocArgs *pp_args = args;

        AvenCTokenSet tset = aven_c_lex(pp_args->src, &arena);
        AvenCTokenSlice actual = tset.tokens;
        if (actual.len != pp_args->expected.len + 2) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected {} tokens, found {}",
                    aven_fmt_uint(pp_args->expected.len + 2),
                    aven_fmt_uint(actual.len)
                ),
            };
        }
        if (get(actual, 0).type != AVEN_C_TOKEN_TYPE_NONE) {
            return (AvenTestResult){ .error = 1, .message = aven_str("no initial NONE token") };
        }
        for (uint32_t i = 0; i < pp_args->expected.len; i += 1) {
            AvenCToken at = get(actual, i + 1);
            AvenCTokenLoc al = aven_c_token_loc(tset, i + 1);
            TestAvenCTokenLoc et = get(pp_args->expected, i);

            AvenStr as = aven_c_token_str(tset, i + 1);
            if (at.type != et.token.type or !aven_str_equals(as, et.token.val)) {
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
            return (AvenTestResult){ .error = 1, .message = aven_str("no terminating NONE token") };
        }

        return (AvenTestResult){ 0 };
    }

    typedef struct {
        AvenStr src;
        AvenStr expected;
        size_t line_len;
    } TestAvenCAstRenderArgs;

    static AvenTestResult test_aven_c_ast_render(AvenArena *emsg_arena, AvenArena arena, void *args) {
        TestAvenCAstRenderArgs *fmt_args = args;

        AvenCTokenSet tset = aven_c_lex(fmt_args->src, &arena);
        AvenCAstResult ast_res = aven_c_ast_parse(tset, 12, &arena);
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
            8 * fmt_args->src.len + fmt_args->expected.len
        );
        AvenIoWriter writer = aven_io_writer_init_bytes(out_buffer);
        AvenCAstRenderResult ren_res = aven_c_ast_render(
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
                .message = aven_fmt(emsg_arena, "rendering error: {}", aven_fmt_str(ren_res.msg)),
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
                .args = &(TestAvenCLexPpArgs){ .src = slice_array(""), .expected = { 0 } },
            },
            {
                .desc = aven_str("aven_c_lex single declaration"),
                .fn = test_aven_c_lex,
                .args = &(TestAvenCLexPpArgs){
                    .src = slice_array("int x;"),
                    .expected = slice_array(
                        (TestAvenCToken[]){
                            { .val = aven_str("int"), .type = AVEN_C_TOKEN_TYPE_KEY },
                            { .val = aven_str("x"), .type = AVEN_C_TOKEN_TYPE_ID },
                            { .val = aven_str(";"), .type = AVEN_C_TOKEN_TYPE_PNC },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_lex include directive"),
                .fn = test_aven_c_lex,
                .args = &(TestAvenCLexPpArgs){
                    .src = slice_array("#include <stdio.h>"),
                    .expected = slice_array(
                        (TestAvenCToken[]){
                            { .val = aven_str("#include <stdio.h>"), .type = AVEN_C_TOKEN_TYPE_HDR },
                            { .val = aven_str(""), .type = AVEN_C_TOKEN_TYPE_NONE },
                            { .val = aven_str("#"), .type = AVEN_C_TOKEN_TYPE_PNC },
                            { .val = aven_str("include"), .type = AVEN_C_TOKEN_TYPE_ID },
                            { .val = aven_str("<"), .type = AVEN_C_TOKEN_TYPE_PNC },
                            { .val = aven_str("stdio.h"), .type = AVEN_C_TOKEN_TYPE_ID },
                            { .val = aven_str(">"), .type = AVEN_C_TOKEN_TYPE_PNC },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_lex weird pp-number"),
                .fn = test_aven_c_lex,
                .args = &(TestAvenCLexPpArgs){
                    .src = slice_array("int x = 0xE+12;\n"),
                    .expected = slice_array(
                        (TestAvenCToken[]){
                            { .val = aven_str("int"), .type = AVEN_C_TOKEN_TYPE_KEY },
                            { .val = aven_str("x"), .type = AVEN_C_TOKEN_TYPE_ID },
                            { .val = aven_str("="), .type = AVEN_C_TOKEN_TYPE_PNC },
                            { .val = aven_str("0xE+12"), .type = AVEN_C_TOKEN_TYPE_NUM },
                            { .val = aven_str(";"), .type = AVEN_C_TOKEN_TYPE_PNC },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_lex whitespace breaks pp-number"),
                .fn = test_aven_c_lex,
                .args = &(TestAvenCLexPpArgs){
                    .src = slice_array("int x = 0xE +12;\n"),
                    .expected = slice_array(
                        (TestAvenCToken[]){
                            { .val = aven_str("int"), .type = AVEN_C_TOKEN_TYPE_KEY },
                            { .val = aven_str("x"), .type = AVEN_C_TOKEN_TYPE_ID },
                            { .val = aven_str("="), .type = AVEN_C_TOKEN_TYPE_PNC },
                            { .val = aven_str("0xE"), .type = AVEN_C_TOKEN_TYPE_NUM },
                            { .val = aven_str("+"), .type = AVEN_C_TOKEN_TYPE_PNC },
                            { .val = aven_str("12"), .type = AVEN_C_TOKEN_TYPE_NUM },
                            { .val = aven_str(";"), .type = AVEN_C_TOKEN_TYPE_PNC },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_lex simple escape sequence"),
                .fn = test_aven_c_lex,
                .args = &(TestAvenCLexPpArgs){
                    .src = slice_array("\"Hello, World!\\n\""),
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
                    .src = slice_array("\"\\17\""),
                    .expected = slice_array(
                        (TestAvenCToken[]){
                            { .val = aven_str("\"\\17\""), .type = AVEN_C_TOKEN_TYPE_STR },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_lex hex escape sequence"),
                .fn = test_aven_c_lex,
                .args = &(TestAvenCLexPpArgs){
                    .src = slice_array("\"color code: \\xFFAA22\""),
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
                    .src = slice_array("\"\\9\""),
                    .expected = slice_array(
                        (TestAvenCToken[]){
                            { .val = aven_str("\"\\9\""), .type = AVEN_C_TOKEN_TYPE_STR },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_lex small block with comments"),
                .fn = test_aven_c_lex,
                .args = &(TestAvenCLexPpArgs){
                    .src = slice_array(
                        "int x = 0; // a comment with #\n"
                        "while (x < 2) {\n"
                        "    /*\n"
                        "    ** here we are computing x + 1\n"
                        "    **/\n"
                        "    x += 1;\n"
                        "}\n"
                    ),
                    .expected = slice_array(
                        (TestAvenCToken[]){
                            { .val = aven_str("int"), .type = AVEN_C_TOKEN_TYPE_KEY },
                            { .val = aven_str("x"), .type = AVEN_C_TOKEN_TYPE_ID },
                            { .val = aven_str("="), .type = AVEN_C_TOKEN_TYPE_PNC },
                            { .val = aven_str("0"), .type = AVEN_C_TOKEN_TYPE_NUM },
                            { .val = aven_str(";"), .type = AVEN_C_TOKEN_TYPE_PNC },
                            {
                                .val = aven_str("// a comment with #"),
                                .type = AVEN_C_TOKEN_TYPE_CMT,
                            },
                            { .val = aven_str("while"), .type = AVEN_C_TOKEN_TYPE_KEY },
                            { .val = aven_str("("), .type = AVEN_C_TOKEN_TYPE_PNC },
                            { .val = aven_str("x"), .type = AVEN_C_TOKEN_TYPE_ID },
                            { .val = aven_str("<"), .type = AVEN_C_TOKEN_TYPE_PNC },
                            { .val = aven_str("2"), .type = AVEN_C_TOKEN_TYPE_NUM },
                            { .val = aven_str(")"), .type = AVEN_C_TOKEN_TYPE_PNC },
                            { .val = aven_str("{"), .type = AVEN_C_TOKEN_TYPE_PNC },
                            { .val = aven_str("/*"), .type = AVEN_C_TOKEN_TYPE_CMT },
                            {
                                .val = aven_str("** here we are computing x + 1"),
                                .type = AVEN_C_TOKEN_TYPE_CMT,
                            },
                            { .val = aven_str("**/"), .type = AVEN_C_TOKEN_TYPE_CMT },
                            { .val = aven_str("x"), .type = AVEN_C_TOKEN_TYPE_ID },
                            { .val = aven_str("+="), .type = AVEN_C_TOKEN_TYPE_PNC },
                            { .val = aven_str("1"), .type = AVEN_C_TOKEN_TYPE_NUM },
                            { .val = aven_str(";"), .type = AVEN_C_TOKEN_TYPE_PNC },
                            { .val = aven_str("}"), .type = AVEN_C_TOKEN_TYPE_PNC },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_lex carriage return in whitespace"),
                .fn = test_aven_c_lex,
                .args = &(TestAvenCLexPpArgs){
                    .src = slice_array("\r\"Hello, World!\\n\""),
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
                .desc = aven_str("aven_c_lex carriage return to end comment"),
                .fn = test_aven_c_lex,
                .args = &(TestAvenCLexPpArgs){
                    .src = slice_array("// a comment\r\n"),
                    .expected = slice_array(
                        (TestAvenCToken[]){
                            { .val = aven_str("// a comment"), .type = AVEN_C_TOKEN_TYPE_CMT },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_lex carriage return invalid within comment"),
                .fn = test_aven_c_lex,
                .args = &(TestAvenCLexPpArgs){
                    .src = slice_array("// a\rcomment\n"),
                    .expected = slice_array(
                        (TestAvenCToken[]){
                            { .val = aven_str("// a"), .type = AVEN_C_TOKEN_TYPE_CMT },
                            { .val = aven_str("\r"), .type = AVEN_C_TOKEN_TYPE_INV },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_lex carriage return invalid within ml comment"),
                .fn = test_aven_c_lex,
                .args = &(TestAvenCLexPpArgs){
                    .src = slice_array("/* a\rcomment */"),
                    .expected = slice_array(
                        (TestAvenCToken[]){
                            { .val = aven_str("/* a"), .type = AVEN_C_TOKEN_TYPE_CMT },
                            { .val = aven_str("\r"), .type = AVEN_C_TOKEN_TYPE_INV },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_lex carriage return at end of ppd"),
                .fn = test_aven_c_lex,
                .args = &(TestAvenCLexPpArgs){
                    .src = slice_array("#define A 10\r\n"),
                    .expected = slice_array(
                        (TestAvenCToken[]){
                            { .val = aven_str("#define A 10\r"), .type = AVEN_C_TOKEN_TYPE_PPD },
                            { .type = AVEN_C_TOKEN_TYPE_NONE },
                            { .val = aven_str("#"), .type = AVEN_C_TOKEN_TYPE_PNC },
                            { .val = aven_str("define"), .type = AVEN_C_TOKEN_TYPE_ID },
                            { .val = aven_str("A"), .type = AVEN_C_TOKEN_TYPE_ID },
                            { .val = aven_str("10"), .type = AVEN_C_TOKEN_TYPE_NUM },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_lex carriage return newline escaped in ppd"),
                .fn = test_aven_c_lex,
                .args = &(TestAvenCLexPpArgs){
                    .src = slice_array("#define A\\\r\n10\r\n"),
                    .expected = slice_array(
                        (TestAvenCToken[]){
                            {
                                .val = aven_str("#define A\\\r\n10\r"),
                                .type = AVEN_C_TOKEN_TYPE_PPD,
                            },
                            { .type = AVEN_C_TOKEN_TYPE_NONE },
                            { .val = aven_str("#"), .type = AVEN_C_TOKEN_TYPE_PNC },
                            { .val = aven_str("define"), .type = AVEN_C_TOKEN_TYPE_ID },
                            { .val = aven_str("A"), .type = AVEN_C_TOKEN_TYPE_ID },
                            { .val = aven_str("10"), .type = AVEN_C_TOKEN_TYPE_NUM },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_lex carriage return within ppd"),
                .fn = test_aven_c_lex,
                .args = &(TestAvenCLexPpArgs){
                    .src = slice_array("#define A\r10\n"),
                    .expected = slice_array(
                        (TestAvenCToken[]){
                            { .val = aven_str("#define A\r10"), .type = AVEN_C_TOKEN_TYPE_PPD },
                            { .type = AVEN_C_TOKEN_TYPE_NONE },
                            { .val = aven_str("#"), .type = AVEN_C_TOKEN_TYPE_PNC },
                            { .val = aven_str("define"), .type = AVEN_C_TOKEN_TYPE_ID },
                            { .val = aven_str("A"), .type = AVEN_C_TOKEN_TYPE_ID },
                            { .val = aven_str("10"), .type = AVEN_C_TOKEN_TYPE_NUM },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_token_loc single declaration"),
                .fn = test_aven_c_token_loc,
                .args = &(TestAvenCTokenLocArgs){
                    .src = slice_array("int x;"),
                    .expected = slice_array(
                        (TestAvenCTokenLoc[]){
                            {
                                .token = { .val = aven_str("int"), .type = AVEN_C_TOKEN_TYPE_KEY },
                                .loc = { .line = 1, .col = 1 },
                            },
                            {
                                .token = { .val = aven_str("x"), .type = AVEN_C_TOKEN_TYPE_ID },
                                .loc = { .line = 1, .col = 5 },
                            },
                            {
                                .token = { .val = aven_str(";"), .type = AVEN_C_TOKEN_TYPE_PNC },
                                .loc = { .line = 1, .col = 6 },
                            },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_lex invalid newline in string literal"),
                .fn = test_aven_c_token_loc,
                .args = &(TestAvenCTokenLocArgs){
                    .src = slice_array("\"Hello,\nWorld!\""),
                    .expected = slice_array(
                        (TestAvenCTokenLoc[]){
                            {
                                .token = {
                                    .val = aven_str("\"Hello,"),
                                    .type = AVEN_C_TOKEN_TYPE_INV,
                                },
                                .loc = { .line = 1, .col = 1 },
                            },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_lex invalid carriage return in comment"),
                .fn = test_aven_c_token_loc,
                .args = &(TestAvenCTokenLocArgs){
                    .src = slice_array("// a\r comment"),
                    .expected = slice_array(
                        (TestAvenCTokenLoc[]){
                            {
                                .token = { .val = aven_str("// a"), .type = AVEN_C_TOKEN_TYPE_CMT },
                                .loc = { .line = 1, .col = 1 },
                            },
                            {
                                .token = { .val = aven_str("\r"), .type = AVEN_C_TOKEN_TYPE_INV },
                                .loc = { .line = 1, .col = 5 },
                            },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_token_loc escaped newlines"),
                .fn = test_aven_c_token_loc,
                .args = &(TestAvenCTokenLocArgs){
                    .src = slice_array(
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
                                .loc = { .line = 1, .col = 1 },
                            },
                            {
                                .token = { .type = AVEN_C_TOKEN_TYPE_NONE },
                                .loc = { .line = 5, .col = 1 },
                            },
                            {
                                .token = { .val = aven_str("#"), .type = AVEN_C_TOKEN_TYPE_PNC },
                                .loc = { .line = 1, .col = 1 },
                            },
                            {
                                .token = { .val = aven_str("define"), .type = AVEN_C_TOKEN_TYPE_ID },
                                .loc = { .line = 1, .col = 2 },
                            },
                            {
                                .token = { .val = aven_str("Slice"), .type = AVEN_C_TOKEN_TYPE_ID },
                                .loc = { .line = 1, .col = 9 },
                            },
                            {
                                .token = { .val = aven_str("("), .type = AVEN_C_TOKEN_TYPE_PNC },
                                .loc = { .line = 1, .col = 14 },
                            },
                            {
                                .token = { .val = aven_str("T"), .type = AVEN_C_TOKEN_TYPE_ID },
                                .loc = { .line = 1, .col = 15 },
                            },
                            {
                                .token = { .val = aven_str(")"), .type = AVEN_C_TOKEN_TYPE_PNC },
                                .loc = { .line = 1, .col = 16 },
                            },
                            {
                                .token = {
                                    .val = aven_str("struct"),
                                    .type = AVEN_C_TOKEN_TYPE_KEY,
                                },
                                .loc = { .line = 1, .col = 18 },
                            },
                            {
                                .token = { .val = aven_str("{"), .type = AVEN_C_TOKEN_TYPE_PNC },
                                .loc = { .line = 1, .col = 25 },
                            },
                            {
                                .token = { .val = aven_str("size_t"), .type = AVEN_C_TOKEN_TYPE_ID },
                                .loc = { .line = 2, .col = 9 },
                            },
                            {
                                .token = { .val = aven_str("len"), .type = AVEN_C_TOKEN_TYPE_ID },
                                .loc = { .line = 2, .col = 16 },
                            },
                            {
                                .token = { .val = aven_str(";"), .type = AVEN_C_TOKEN_TYPE_PNC },
                                .loc = { .line = 2, .col = 19 },
                            },
                            {
                                .token = { .val = aven_str("T"), .type = AVEN_C_TOKEN_TYPE_ID },
                                .loc = { .line = 3, .col = 9 },
                            },
                            {
                                .token = { .val = aven_str("*"), .type = AVEN_C_TOKEN_TYPE_PNC },
                                .loc = { .line = 3, .col = 11 },
                            },
                            {
                                .token = { .val = aven_str("ptr"), .type = AVEN_C_TOKEN_TYPE_ID },
                                .loc = { .line = 3, .col = 12 },
                            },
                            {
                                .token = { .val = aven_str(";"), .type = AVEN_C_TOKEN_TYPE_PNC },
                                .loc = { .line = 3, .col = 15 },
                            },
                            {
                                .token = { .val = aven_str("}"), .type = AVEN_C_TOKEN_TYPE_PNC },
                                .loc = { .line = 4, .col = 5 },
                            },
                        }
                    ),
                },
            },
            {
                .desc = aven_str("aven_c_ast_render int declaration"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x;"),
                    .expected = aven_str("int x;\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render int declaration list"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x, y;"),
                    .expected = aven_str("int x, y;\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render int declaration init expr"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x = 2 + 2;\n"),
                    .expected = aven_str("int x = 2 + 2;\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render int declaration init expr list"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x = 0, y = 0;\n"),
                    .expected = aven_str("int x = 0, y = 0;\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render char declaration init expr"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("char c = '\\n';\n"),
                    .expected = aven_str("char c = '\\n';\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render expression split same op add"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("long x = 2 + 2 * 4 - 7;"),
                    .expected = aven_str("long x = 2 +\n" "    2 * 4 -\n" "    7;\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render expression split same op mul"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x = 10 + 2 * 2 * 4 * 7;\n"),
                    .expected = aven_str(
                        "int x = 10 +\n" "    2 *\n" "    2 *\n" "    4 *\n" "    7;\n"
                    ),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render expression split op same indent"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x = 10 - 2 * 2 * 4 - 7;\n"),
                    .expected = aven_str("int x = 10 -\n" "    2 * 2 * 4 -\n" "    7;\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render expression split assignment"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x = 10 - 2 * 2 * 4 - 7 = 32 + 7 + 14;\n"),
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
                    .src = slice_array("bool x = true ? 1 : 0;\n"),
                    .expected = aven_str("bool x = true ? 1 : 0;\n"),
                    .line_len = 24,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render ternary expression split"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("bool x = true ? 1 : 0;\n"),
                    .expected = aven_str("bool x = true ?\n" "    1 :\n" "    0;\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render postfix '.'"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int error = result.error;\n"),
                    .expected = aven_str("int error = result.error;\n"),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render postfix []"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int error = arr[2];\n"),
                    .expected = aven_str("int error = arr[2];\n"),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render postfix [] and ()"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int error = arr[2](x,y);\n"),
                    .expected = aven_str("int error = arr[2](x, y);\n"),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render comma expression"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x = (10 - 2 * 2 * 4 - 7, 32 + 7 + 14);\n"),
                    .expected = aven_str("int x = (10 - 2 * 2 * 4 - 7, 32 + 7 + 14);\n"),
                    .line_len = 48,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render comma expression split"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x = (10 - 2 * 2 * 4 - 7, 32 + 7 + 14);\n"),
                    .expected = aven_str(
                        "int x = (\n"
                        "    10 -\n"
                        "    2 * 2 * 4 -\n"
                        "    7,\n"
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
                    .src = slice_array(
                        "int x = (10 - 2 * 2 * 4 - 7, 32 + 7 + 14) = 1 + 2 + 3 + 4 + 5;\n"
                    ),
                    .expected = aven_str(
                        "int x = (\n"
                        "    10 -\n"
                        "    2 * 2 * 4 -\n"
                        "    7,\n"
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
                .desc = aven_str("aven_c_ast_render sizeof operator postfix expr"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("const size_t x = 2 + sizeof foo(x);\n"),
                    .expected = aven_str("const size_t x = 2 +\n" "    sizeof foo(x);\n"),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render sizeof operator paren expr"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("const size_t x = 2 + sizeof(*x);\n"),
                    .expected = aven_str("const size_t x = 2 + sizeof(*x);\n"),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render sizeof operator unambiguous postfix expr"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("const size_t x = 2 + sizeof(*x)->y;\n"),
                    .expected = aven_str("const size_t x = 2 + sizeof (*x)->y;\n"),
                    .line_len = 38,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render sizeof operator prefer type to expr"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("const size_t x = 2 + sizeof (x)-y;\n"),
                    .expected = aven_str("const size_t x = 2 + sizeof(x) - y;\n"),
                    .line_len = 38,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render sizeof operator typename"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("const size_t x = 2 + sizeof(int);\n"),
                    .expected = aven_str("const size_t x = 2 + sizeof(int);\n"),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render sizeof operator"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("const size_t x = 2 + _Alignof(int);\n"),
                    .expected = aven_str("const size_t x = 2 +\n" "    _Alignof(int);\n"),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render _Atomic specifier"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("_Atomic(size_t) x;\n"),
                    .expected = aven_str("_Atomic(size_t) x;\n"),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render _Alignas specifier"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("_Alignas(1) uint64_t x;\n"),
                    .expected = aven_str("_Alignas(1) uint64_t x;\n"),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render static assert"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("_Static_assert(x == 2, \"OHNO\");\n"),
                    .expected = aven_str("_Static_assert(x == 2, \"OHNO\");\n"),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render struct initializer postfix"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x = (struct T){ .x = 7 };\n"),
                    .expected = aven_str("int x = (struct T){ .x = 7 };\n"),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render struct initializer postfix short"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x = (struct T){ .x = 7 };\n"),
                    .expected = aven_str("int x = (struct T){\n" "    .x = 7,\n" "};\n"),
                    .line_len = 24,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render leading comment"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("// Hello World!\n" "int x = 2 + 2;\n"),
                    .expected = aven_str("// Hello World!\n" "int x = 2 + 2;\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render leading block comment"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("/* Hello World! */ int x = 2 + 2;\n"),
                    .expected = aven_str("/* Hello World! */\n" "int x = 2 + 2;\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render trailing comment"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x = 2 + 2; // Hello World!\n"),
                    .expected = aven_str("int x = 2 + 2; // Hello World!\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render trailing block comment"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x = 2 + 2; /* Hello World! */\n"),
                    .expected = aven_str("int x = 2 + 2; /* Hello World! */\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render internal block comment"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x = 2 + /* add */ 2;\n"),
                    .expected = aven_str("int x = 2 + /* add */\n" "    2;\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render empty line after comment"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("// Hello World!\n\n" "int x = 2 + 2;\n"),
                    .expected = aven_str("// Hello World!\n\n" "int x = 2 + 2;\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render empty line before comment"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x = 2 + 2;\n\n" "// Hello World!\n" "int y = x;\n"),
                    .expected = aven_str("int x = 2 + 2;\n\n" "// Hello World!\n" "int y = x;\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render empty line within multi-line comment"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "int x = 2 + 2;\n" "/* Hello World!\n" "\n" "*/ int y = x;\n"
                    ),
                    .expected = aven_str(
                        "int x = 2 + 2;\n" "/* Hello World!\n" "\n" "*/\n" "int y = x;\n"
                    ),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render pp define simple const expr"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("#define NUM 1"),
                    .expected = aven_str("#define NUM 1\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render pp define fn const expr"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("#define ADD1(n) (n + 1)"),
                    .expected = aven_str("#define ADD1(n) (n + 1)\n"),
                    .line_len = 25,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render pp define fn const expr wrap body"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("#define ADD1(n) (n + 1)"),
                    .expected = aven_str("#define ADD1(n) ( \\\n        n + 1 \\\n    )\n"),
                    .line_len = 18,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render pp define fn const expr vrap def"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("#define ADD1(n) (n + 1)"),
                    .expected = aven_str("#define ADD1( \\\n        n \\\n    ) (n + 1)\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render comment in pp directive"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("#define NUM 1 // number of entries\n"),
                    .expected = aven_str("#define NUM 1 // number of entries\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render multi-line comment in pp directive"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "#define NUM 1 /* number of\n" "              ** entries */\n"
                    ),
                    .expected = aven_str("#define NUM 1 /* number of\n" "** entries */\n"),
                    .line_len = 16,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render pathological cast expr"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x = (a) - (b);"),
                    .expected = aven_str("int x = (a)-(b);\n"),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render pathological cast expr \"fixed\""),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int x = ((a)) - (b);"),
                    .expected = aven_str("int x = ((a)) - (b);\n"),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render string literal"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("char str[] = \"Hello, World!\";\n"),
                    .expected = aven_str("char str[] = \"Hello, World!\";\n"),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render compound string literal"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("char str[] = \"Hello,\" \" World!\";\n"),
                    .expected = aven_str("char str[] = \"Hello,\" \" World!\";\n"),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render compound string literal short line"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("char str[] = \"Hello,\" \" World!\";\n"),
                    .expected = aven_str("char str[] = \"Hello,\"\n    \" World!\";\n"),
                    .line_len = 24,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render compound string literal param"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("void *p = make(\"Hello,\" \" World!\");\n"),
                    .expected = aven_str("void *p = make(\"Hello,\" \" World!\");\n"),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render compound string literal param short line"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("void *p = make(\"Hello,\" \" World!\");\n"),
                    .expected = aven_str(
                        "void *p = make(\n" "    \"Hello,\"\n" "    \" World!\"\n" ");\n"
                    ),
                    .line_len = 20,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render compound string literal w/macro"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("const char *str = \"Hello, \" MY_STR;"),
                    .expected = aven_str("const char *str = \"Hello, \" MY_STR;\n"),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render compound string literal w/macro short"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("const char *str = \" World!\" MY_STR;"),
                    .expected = aven_str("const char *str = \" World!\"\n    MY_STR;\n"),
                    .line_len = 28,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render compound string literal return"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void foo(void) {\n" "    return \"Hello, \" \"World!\";\n" "}\n"
                    ),
                    .expected = aven_str(
                        "void foo(void) {\n" "    return \"Hello, \" \"World!\";\n" "}\n"
                    ),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render compound string literal return short"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void foo(void) {\n" "    return \"Hello, \" \"World!\";\n" "}\n"
                    ),
                    .expected = aven_str(
                        "void foo(void) {\n"
                        "    return \"Hello, \"\n"
                        "        \"World!\";\n"
                        "}\n"
                    ),
                    .line_len = 24,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render type pointer render as decl not expr"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("MyStruct *s;\n"),
                    .expected = aven_str("MyStruct *s;\n"),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render function typedef"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "typedef AvenIoResult AvenIoFn(AvenIoCtx ctx, ByteSlice bytes);\n"
                    ),
                    .expected = aven_str(
                        "typedef AvenIoResult AvenIoFn(\n"
                        "    AvenIoCtx ctx,\n"
                        "    ByteSlice bytes\n"
                        ");\n"
                    ),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render function pointer typedef"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("typedef size_t (*ReadFn)(char *, size_t);\n"),
                    .expected = aven_str(
                        "typedef size_t (*ReadFn)(\n" "    char *,\n" "    size_t\n" ");\n"
                    ),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render function pointer cast"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int foo(void *fn) { (void *(*)(void *))fn(); }\n"),
                    .expected = aven_str(
                        "int foo(void *fn) {\n" "    (void *(*)(void *))fn();\n" "}\n"
                    ),
                    .line_len = 48,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render function pointer struct declaration"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "typedef struct {\n"
                        "    const size_t (*ReadFn)(char *, size_t);\n"
                        "    void *ctx;\n"
                        "} Reader;\n"
                    ),
                    .expected = aven_str(
                        "typedef struct {\n"
                        "    const size_t (*ReadFn)(char *, size_t);\n"
                        "    void *ctx;\n"
                        "} Reader;\n"
                    ),
                    .line_len = 48,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render struct declaration bitfield"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "typedef struct Flags {\n"
                        "    unsigned char a : 1;\n"
                        "    unsigned char b : 1;\n"
                        "    unsigned char c : 1;\n"
                        "    unsigned char d : 1;\n"
                        "};\n"
                    ),
                    .expected = aven_str(
                        "typedef struct Flags {\n"
                        "    unsigned char a : 1;\n"
                        "    unsigned char b : 1;\n"
                        "    unsigned char c : 1;\n"
                        "    unsigned char d : 1;\n"
                        "};\n"
                    ),
                    .line_len = 48,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render struct declaration list bitfields"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "typedef struct Flags {\n"
                        "    unsigned char a : 1, b : 1, c : 1, d : 1;\n"
                        "};\n"
                    ),
                    .expected = aven_str(
                        "typedef struct Flags {\n"
                        "    unsigned char a : 1, b : 1, c : 1, d : 1;\n"
                        "};\n"
                    ),
                    .line_len = 48,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render postfix attribute"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("typedef float Vec2SIMD __attribute__((vector_size(8)));\n"),
                    .expected = aven_str(
                        "typedef float Vec2SIMD __attribute__(\n" "    (vector_size(8))\n" ");\n"
                    ),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render prefix attribute"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "__attribute__((unused))\n"
                        "static AvenIoReader aven_io_stdin = {\n"
                        "    .read = aven_io_fd_read_stub,\n"
                        "    .ctx = { .fd = 0 },\n"
                        "};\n"
                    ),
                    .expected = aven_str(
                        "__attribute__((unused)) static AvenIoReader\n"
                        "    aven_io_stdin = {\n"
                        "        .read = aven_io_fd_read_stub,\n"
                        "        .ctx = { .fd = 0 },\n"
                        "    };\n"
                    ),
                    .line_len = 48,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render function attributes"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "#if __has_attribute(malloc)\n"
                        "    __attribute__((malloc))\n"
                        "#endif\n"
                        "#if !defined(AVEN_IMPLEMENTATION)\n"
                        "    #if __has_attribute(alloc_size)\n"
                        "        __attribute__((alloc_size(2, 4)))\n"
                        "    #endif\n"
                        "    #if __has_attribute(alloc_align)\n"
                        "        __attribute__((alloc_align(3)))\n"
                        "    #endif\n"
                        "#endif\n"
                        "AVEN_FN void *aven_arena_alloc(\n"
                        "    AvenArena *arena,\n"
                        "    size_t count,\n"
                        "    size_t align,\n"
                        "    size_t size\n"
                        ");\n"
                    ),
                    .expected = aven_str(
                        "#if __has_attribute(malloc)\n"
                        "    __attribute__((malloc))\n"
                        "#endif\n"
                        "#if !defined(AVEN_IMPLEMENTATION)\n"
                        "    #if __has_attribute(alloc_size)\n"
                        "        __attribute__((alloc_size(2, 4)))\n"
                        "    #endif\n"
                        "    #if __has_attribute(alloc_align)\n"
                        "        __attribute__((alloc_align(3)))\n"
                        "    #endif\n"
                        "#endif\n"
                        "AVEN_FN void *aven_arena_alloc(\n"
                        "    AvenArena *arena,\n"
                        "    size_t count,\n"
                        "    size_t align,\n"
                        "    size_t size\n"
                        ");\n"
                    ),
                    .line_len = 80,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render initializer list w/pointer cast"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
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
                    .src = slice_array(
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
                .desc = aven_str("aven_c_ast_render if statement non-compound"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void bar(int n) {\n"
                        "    if (n % 2 == 0) return n / 2;\n"
                        "    return 3 * n + 1;\n"
                        "}\n"
                    ),
                    .expected = aven_str(
                        "void bar(int n) {\n"
                        "    if (n % 2 == 0)\n"
                        "        return n / 2;\n"
                        "    return 3 * n + 1;\n"
                        "}\n"
                    ),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render if else statement non-compound"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void bar(int n) {\n"
                        "    if (n % 2 == 0) return n / 2;"
                        "    else return 3 * n + 1;\n"
                        "}\n"
                    ),
                    .expected = aven_str(
                        "void bar(int n) {\n"
                        "    if (n % 2 == 0)\n"
                        "        return n / 2;\n"
                        "    else\n"
                        "        return 3 * n + 1;\n"
                        "}\n"
                    ),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render if else if else statement non-compound"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void bar(int n) {\n"
                        "    if (n % 2 == 0) return n / 2;"
                        "    else if (n % 5 == 0) return n / 5;"
                        "    else return 3 * n + 1;\n"
                        "}\n"
                    ),
                    .expected = aven_str(
                        "void bar(int n) {\n"
                        "    if (n % 2 == 0)\n"
                        "        return n / 2;\n"
                        "    else if (n % 5 == 0)\n"
                        "        return n / 5;\n"
                        "    else\n"
                        "        return 3 * n + 1;\n"
                        "}\n"
                    ),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render if statement compound"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void bar(int n) {\n"
                        "    if (n % 2 == 0) { return n / 2; }\n"
                        "    return 3 * n + 1;\n"
                        "}\n"
                    ),
                    .expected = aven_str(
                        "void bar(int n) {\n"
                        "    if (n % 2 == 0) {\n"
                        "        return n / 2;\n"
                        "    }\n"
                        "    return 3 * n + 1;\n"
                        "}\n"
                    ),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render if else statement compound"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void bar(int n) {\n"
                        "    if (n % 2 == 0) { return n / 2; }"
                        "    else { return 3 * n + 1; }\n"
                        "}\n"
                    ),
                    .expected = aven_str(
                        "void bar(int n) {\n"
                        "    if (n % 2 == 0) {\n"
                        "        return n / 2;\n"
                        "    } else {\n"
                        "        return 3 * n + 1;\n"
                        "    }\n"
                        "}\n"
                    ),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render if else if else statement compound"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void bar(int n) {\n"
                        "    if (n % 2 == 0) { return n / 2; }"
                        "    else if (n % 5 == 0) { return n / 5; }"
                        "    else { return 3 * n + 1; }\n"
                        "}\n"
                    ),
                    .expected = aven_str(
                        "void bar(int n) {\n"
                        "    if (n % 2 == 0) {\n"
                        "        return n / 2;\n"
                        "    } else if (n % 5 == 0) {\n"
                        "        return n / 5;\n"
                        "    } else {\n"
                        "        return 3 * n + 1;\n"
                        "    }\n"
                        "}\n"
                    ),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render if else statement compound empty"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void bar(int n) {\n"
                        "    if (n % 2 == 0) {  }\n"
                        "    else { return 3 * n + 1; }\n"
                        "}\n"
                    ),
                    .expected = aven_str(
                        "void bar(int n) {\n"
                        "    if (n % 2 == 0) {} else {\n"
                        "        return 3 * n + 1;\n"
                        "    }\n"
                        "}\n"
                    ),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render if statement short line"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void bar(int n) {\n"
                        "    if (n % 2 == 0) return n / 2;\n"
                        "    return 3 * n + 1;\n"
                        "}\n"
                    ),
                    .expected = aven_str(
                        "void bar(int n) {\n"
                        "    if (n % 2 == 0)\n"
                        "        return n / 2;\n"
                        "    return 3 * n + 1;\n"
                        "}\n"
                    ),
                    .line_len = 24,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render for statement non-compound"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void bar(int n) {\n" "    for (int x = 0; x < n; x++) foo(x);\n" "}\n"
                    ),
                    .expected = aven_str(
                        "void bar(int n) {\n"
                        "    for (int x = 0; x < n; x++)\n"
                        "        foo(x);\n"
                        "}\n"
                    ),
                    .line_len = 80,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render for statement if statement non-compound"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void bar(int n) {\n"
                        "    for (int x = 0; x < n; x++) "
                        "if (x%2 == 0) foo(x);\n"
                        "else bar(x);"
                        "}\n"
                    ),
                    .expected = aven_str(
                        "void bar(int n) {\n"
                        "    for (int x = 0; x < n; x++)\n"
                        "        if (x % 2 == 0)\n"
                        "            foo(x);\n"
                        "        else\n"
                        "            bar(x);\n"
                        "}\n"
                    ),
                    .line_len = 80,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render for statement compound"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
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
                .desc = aven_str("aven_c_ast_render for statement empty expr"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void bar(int n) {\n"
                        "    int x = 0;\n"
                        "    for (; x < n; x++) {\n"
                        "        foo(x);\n"
                        "    }\n"
                        "}\n"
                    ),
                    .expected = aven_str(
                        "void bar(int n) {\n"
                        "    int x = 0;\n"
                        "    for (; x < n; x++) {\n"
                        "        foo(x);\n"
                        "    }\n"
                        "}\n"
                    ),
                    .line_len = 80,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render for statement infinite"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void bar(int n) {\n" "    for (;;) {\n" "        foo(x);\n" "    }\n" "}\n"
                    ),
                    .expected = aven_str(
                        "void bar(int n) {\n" "    for (;;) {\n" "        foo(x);\n" "    }\n" "}\n"
                    ),
                    .line_len = 80,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render for statement split"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void bar(int n) {\n"
                        "    for (int x = 0; x < n; x++) {\n"
                        "        foo(x);\n"
                        "    }\n"
                        "}\n"
                    ),
                    .expected = aven_str(
                        "void bar(int n) {\n"
                        "    for (\n"
                        "        int x = 0;\n"
                        "        x < n;\n"
                        "        x++\n"
                        "    ) {\n"
                        "        foo(x);\n"
                        "    }\n"
                        "}\n"
                    ),
                    .line_len = 24,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render array designators"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
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
                    .src = slice_array(
                        "#define loop(f, a) do {\\\n" "        a = f;\\\n" "    } while (a == 0)\n"
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
                    .src = slice_array(
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
                    .src = slice_array(
                        "#define assert(c) ((!(c)) ? "
                        "aven_panic(\"assert(\" #c \") failed\") : "
                        "(void)0)\n"
                    ),
                    .expected = aven_str(
                        "#define assert(c) ((!(c)) ? "
                        "aven_panic(\"assert(\" #c \") failed\") : "
                        "(void)0)\n"
                    ),
                    .line_len = 80,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render pp directive do statement w/return"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
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
                    .src = slice_array("void main(void) { printf(\"Hello, World!\"); }\n"),
                    .expected = aven_str(
                        "void main(void) {\n" "    printf(\"Hello, World!\");\n" "}\n"
                    ),
                    .line_len = 40,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render function definition w/args"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
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
                .desc = aven_str("aven_c_ast_render function definition w/comment"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void main(int argc, const char **argv) {"
                        " /* main */ printf(\"Hello, World!\"); }\n"
                    ),
                    .expected = aven_str(
                        "void main(\n"
                        "    int argc,\n"
                        "    const char **argv\n"
                        ") { /* main */\n"
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
                    .src = slice_array(
                        "AVEN_FN void *aven_arena_alloc(\n"
                        "    AvenArena *arena,\n"
                        "    size_t count, size_t align,\n"
                        "\n"
                        "    size_t size\n"
                        ") {\n"
                        "\n"
                        "    assert((align & (align - 1)) == 0);\n"
                        "\n"
                        "    ptrdiff_t pad = (ptrdiff_t)(-(uintptr_t)arena->base & (align - 1));\n"
                        "    ptrdiff_t rem = arena->top - arena->base - pad;\n"
                        "\n"
                        "\n"
                        "\n"
                        "    if (rem < 0 || count > ((size_t)rem / size)) {\n"
                        "            // OOM unrecoverable, panic\n"
                        "            aven_panic(\"arena out of memory\");\n"
                        "    }\n"
                        "\n"
                        "\n"
                        "    void *ptr = arena->base + pad;\n"
                        "    arena->base += (size_t)pad + size * count;\n\n"
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
                        "    ptrdiff_t pad = (ptrdiff_t)(-(uintptr_t)arena->base & (align - 1));\n"
                        "    ptrdiff_t rem = arena->top - arena->base - pad;\n"
                        "\n"
                        "    if (rem < 0 || count > ((size_t)rem / size)) {\n"
                        "        // OOM unrecoverable, panic\n"
                        "        aven_panic(\"arena out of memory\");\n"
                        "    }\n"
                        "\n"
                        "    void *ptr = arena->base + pad;\n"
                        "    arena->base += (size_t)pad + size * count;\n\n"
                        "    return ptr;\n"
                        "}\n"
                    ),
                    .line_len = 80,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render function definition w/switch non-compound"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void collatz(int n) {\n"
                        "    switch (n % 2) {\n"
                        "        case 0:\n"
                        "            return n / 2;\n"
                        "            break;\n"
                        "        default:\n"
                        "            return 3 * n + 1;\n"
                        "            break;\n"
                        "    }\n"
                        "}\n"
                    ),
                    .expected = aven_str(
                        "void collatz(int n) {\n"
                        "    switch (n % 2) {\n"
                        "        case 0:\n"
                        "            return n / 2;\n"
                        "            break;\n"
                        "        default:\n"
                        "            return 3 * n + 1;\n"
                        "            break;\n"
                        "    }\n"
                        "}\n"
                    ),
                    .line_len = 48,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render function definition w/switch compound"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
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
                .desc = aven_str("aven_c_ast_render function definition w/while non-compound"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void main(void) {\n"
                        "    char c = 0;\n"
                        "    while (c != '\\n')\n"
                        "        c = getc();\n"
                        "}\n"
                    ),
                    .expected = aven_str(
                        "void main(void) {\n"
                        "    char c = 0;\n"
                        "    while (c != '\\n')\n"
                        "        c = getc();\n"
                        "}\n"
                    ),
                    .line_len = 48,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render function definition w/while compound"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
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
                    .src = slice_array(
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
                    .src = slice_array(
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
                    .src = slice_array("#include \"aven.h\"\n"),
                    .expected = aven_str("#include \"aven.h\"\n"),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render system include directive"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("#include <aven.h>\n"),
                    .expected = aven_str("#include <aven.h>\n"),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render init decl initializer list"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
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
                    .src = slice_array(
                        "static inline Str token_str(TokenSet tset, uint32_t index) {\n"
                        "    // Grab entire text chunk for all tokens within directive\n"
                        "    Token t1 = get(tset.ppd_tokens, token.index);\n"
                        "    Token tn = get(tset.ppd_tokens, token.index + (token.len - 1));\n"
                        "    return str_range(tset.bytes, t1.index, tn.index + tn.len);\n"
                        "}\n"
                    ),
                    .expected = aven_str(
                        "static inline Str token_str(TokenSet tset, uint32_t index) {\n"
                        "    // Grab entire text chunk for all tokens within directive\n"
                        "    Token t1 = get(tset.ppd_tokens, token.index);\n"
                        "    Token tn = get(tset.ppd_tokens, token.index + (token.len - 1));\n"
                        "    return str_range(tset.bytes, t1.index, tn.index + tn.len);\n"
                        "}\n"
                    ),
                    .line_len = 80,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render system include directive"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
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
                        "// Enough to handle pathological files up to ~10MB\n"
                        "#define ARENA_SIZE (4096 * 250000)\n"
                        "\n"
                        "int main(void) {\n"
                        "    void *mem = malloc(ARENA_SIZE);\n"
                        "    if (mem == NULL) {\n"
                        "        aven_panic(\"malloc failed\\n\");\n"
                        "    }\n"
                        "    AvenArena arena = aven_arena_init(mem, ARENA_SIZE);\n"
                        "    AvenIoWriter w = aven_io_writer_init_stdout_buffered(8192, &arena);\n"
                        "    AvenCFmtResult fmt_res = aven_c_fmt(\n"
                        "            &aven_io_stdin,\n"
                        "            &w,\n"
                        "#ifdef A\n"
                        "            &arena,\n"
                        "#else\n"
                        "            &arena\n"
                        "#endif\n"
                        "        );\n"
                        "    aven_io_writer_flush(&w);\n"
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
                        "// Enough to handle pathological files up to ~10MB\n"
                        "#define ARENA_SIZE (4096 * 250000)\n"
                        "\n"
                        "int main(void) {\n"
                        "    void *mem = malloc(ARENA_SIZE);\n"
                        "    if (mem == NULL) {\n"
                        "        aven_panic(\"malloc failed\\n\");\n"
                        "    }\n"
                        "    AvenArena arena = aven_arena_init(mem, ARENA_SIZE);\n"
                        "    AvenIoWriter w = aven_io_writer_init_stdout_buffered(8192, &arena);\n"
                        "    AvenCFmtResult fmt_res = aven_c_fmt(\n"
                        "        &aven_io_stdin,\n"
                        "        &w,\n"
                        "#ifdef A\n"
                        "        &arena,\n"
                        "#else\n"
                        "        &arena\n"
                        "#endif\n"
                        "    );\n"
                        "    aven_io_writer_flush(&w);\n"
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
            {
                .desc = aven_str("aven_c_ast_render gcc warn pragma"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("#pragma GCC diagnostic ignored \"-Wunused-function\"\n"),
                    .expected = aven_str("#pragma GCC diagnostic ignored \"-Wunused-function\"\n"),
                    .line_len = 80,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render msvc warn pragma"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("#pragma warning(disable : 4267)\n"),
                    .expected = aven_str("#pragma warning(disable : 4267)\n"),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render macro, postfix, sizeof expression"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void *x = RL_MALLOC(anim[a].num_frames*sizeof(Transform *));\n"
                    ),
                    .expected = aven_str(
                        "void *x = RL_MALLOC(\n"
                        "    anim[a].num_frames *\n"
                        "    sizeof(Transform *)\n"
                        ");\n"
                    ),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render ## merge operator"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("#define merge(x,y) x##y\n"),
                    .expected = aven_str("#define merge(x, y) x ## y\n"),
                    .line_len = 36,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render macro define attribute"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("#define API __attribute__((visibility(\"default\")))\n"),
                    .expected = aven_str("#define API __attribute__((visibility(\"default\")))\n"),
                    .line_len = 80,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render postfix binary op bound to rhs"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void foo(void) {\n"
                        "if (\n"
                        "    node == 0 or\n"
                        "        get(\n"
                        "            ppd_ctx.tset.tokens,\n"
                        "            aven_c_ast_next_index(&ppd_ctx)\n"
                        "        ).type !=\n"
                        "            AVEN_C_TOKEN_TYPE_NONE\n"
                        ") {\n"
                        "    break;\n"
                        "}\n"
                        "}\n"
                    ),
                    .expected = aven_str(
                        "void foo(void) {\n"
                        "    if (\n"
                        "        node == 0 or\n"
                        "        get(ppd_ctx.tset.tokens, aven_c_ast_next_index(&ppd_ctx))\n"
                        "            .type != AVEN_C_TOKEN_TYPE_NONE\n"
                        "    ) {\n"
                        "        break;\n"
                        "    }\n"
                        "}\n"
                    ),
                    .line_len = 68,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render asm statement"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void foo(void) {\n" "    asm volatile(\"\":::\"memory\");\n" "}\n"
                    ),
                    .expected = aven_str(
                        "void foo(void) {\n" "    asm volatile (\"\" ::: \"memory\");\n" "}\n"
                    ),
                    .line_len = 48,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render __asm__ statement"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "void foo(void) {\n"
                        "    __asm__ volatile(\"\":\"\":\"\":\"memory\");\n"
                        "}\n"
                    ),
                    .expected = aven_str(
                        "void foo(void) {\n"
                        "    __asm__ volatile (\n"
                        "        \"\":\n"
                        "        \"\":\n"
                        "        \"\":\n"
                        "        \"memory\"\n"
                        "    );\n"
                        "}\n"
                    ),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render newlines between struct members"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "struct Gap{\n" "    int member1;\n" "\n" "    int member2;\n" "\n" "};\n"
                    ),
                    .expected = aven_str(
                        "struct Gap {\n" "    int member1;\n" "\n" "    int member2;\n" "};\n"
                    ),
                    .line_len = 24,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render always split struct members"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "struct Gap{\n" "    int member1;\n" "    int member2;\n" "\n" "};\n"
                    ),
                    .expected = aven_str(
                        "struct Gap {\n" "    int member1;\n" "    int member2;\n" "};\n"
                    ),
                    .line_len = 80,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render pp def parenthesised expresson"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "#define AVEN_ARENA_BIGGEST_ALIGNMENT (alignof(max_align_t))\n"
                    ),
                    .expected = aven_str(
                        "#define AVEN_ARENA_BIGGEST_ALIGNMENT (alignof(max_align_t))\n"
                    ),
                    .line_len = 80,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render pp warning long line"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("#warning \"a long str warning!\"\n"),
                    .expected = aven_str("#warning \"a long str warning!\"\n"),
                    .line_len = 31,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render inline comment in initializer list"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("int arr[] = { a, /* a */ b, /* b */ c /* c */ };"),
                    .expected = aven_str(
                        "int arr[] = {\n"
                        "    a, /* a */\n"
                        "    b, /* b */\n"
                        "    c, /* c */\n"
                        "};\n"
                    ),
                    .line_len = 31,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render weird inline comment in initializer list"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "int arr[] /* y */ =\n"
                        "{ /* z */ a /* a */, /* b */ b, /* c */ /* d */ c /* e */ };"
                    ),
                    .expected = aven_str(
                        "int arr[] /* y */\n"
                        "= { /* z */\n"
                        "    a, /* a */\n"
                        "    /* b */\n"
                        "    b, /* c */\n"
                        "    /* d */\n"
                        "    c, /* e */\n"
                        "};\n"
                    ),
                    .line_len = 31,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render inline comments in compound literal"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "char arr[] =\n"
                        "\"line1\\n\" /* a */ \"line2\\n\" /* b */ \"line3\\n\"; /* c */"
                    ),
                    .expected = aven_str(
                        "char arr[] = \"line1\\n\" /* a */\n"
                        "    \"line2\\n\" /* b */\n"
                        "    \"line3\\n\"; /* c */\n"
                    ),
                    .line_len = 31,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render inline comments in indented compound literal"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array(
                        "char very_long_name[] =\n"
                        "\"line1\\n\" /* a */ \"line2\\n\" /* b */ \"line3\\n\"; /* c */"
                    ),
                    .expected = aven_str(
                        "char very_long_name[] =\n"
                        "        \"line1\\n\" /* a */\n"
                        "        \"line2\\n\" /* b */\n"
                        "        \"line3\\n\"; /* c */\n"
                    ),
                    .line_len = 24,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render ppd with whitespace between # and define"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("#   define  A   24\n"),
                    .expected = aven_str("#define A 24\n"),
                    .line_len = 32,
                },
            },
            {
                .desc = aven_str("aven_c_ast_render ppd with whitespace between # and include"),
                .fn = test_aven_c_ast_render,
                .args = &(TestAvenCAstRenderArgs){
                    .src = slice_array("#   include  <  path / to /   header .   h>\n"),
                    .expected = aven_str("#include <path/to/header.h>\n"),
                    .line_len = 32,
                },
            },
        };

        AvenTestCaseSlice tcases = slice_array(tcase_data);

        aven_test(tcases, arena);

        return 0;
    }
#endif

