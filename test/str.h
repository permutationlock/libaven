#ifndef TEST_AVEN_STR_H
    #define TEST_AVEN_STR_H

    #include <aven.h>
    #include <aven/arena.h>
    #include <aven/fmt.h>
    #include <aven/str.h>
    #include <aven/test.h>

    typedef struct {
        AvenStr str;
        Optional(size_t) inv_index;
        size_t codepoints;
    } TestAvenStrCodepointsArgs;

    AvenTestResult test_aven_str_codepoints(
        AvenArena *emsg_arena,
        AvenArena arena,
        void *opaque_args
    ) {
        (void)arena;
        TestAvenStrCodepointsArgs *args = opaque_args;

        AvenStrCodepointsResult cpt_res = aven_str_codepoints(args->str);
        if (cpt_res.error != 0) {
            if (!args->inv_index.valid) {
                return (AvenTestResult){
                    .error = 1,
                    .message = aven_fmt(
                        emsg_arena,
                        "expected valid utf8, found invalid codepoint at {}",
                        aven_fmt_uint(cpt_res.payload)
                    ),
                };
            } else if (cpt_res.payload != unwrap(args->inv_index)) {
                return (AvenTestResult){
                    .error = 1,
                    .message = aven_fmt(
                        emsg_arena,
                        "expected invalid codepoint at {}, found at {}",
                        aven_fmt_uint(unwrap(args->inv_index)),
                        aven_fmt_uint(cpt_res.payload)
                    ),
                };
            }
            return (AvenTestResult){ 0 };
        }
        if (args->inv_index.valid) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected invalid codepoint at {}, found valid utf8",
                    aven_fmt_uint(unwrap(args->inv_index))
                ),
            };
        }

        if (cpt_res.payload != args->codepoints) {
            return (AvenTestResult){
                .error = 1,
                .message = aven_fmt(
                    emsg_arena,
                    "expected {} codepoints, found {}",
                    aven_fmt_uint(args->codepoints),
                    aven_fmt_uint(cpt_res.payload)
                ),
            };
        }

        return (AvenTestResult){ 0 };
    }

    static int test_str(AvenArena arena) {
        AvenTestCase tcase_data[] = {
            {
                .desc = aven_str("aven_str codepoints empty str"),
                .fn = test_aven_str_codepoints,
                .args = &(TestAvenStrCodepointsArgs){
                    .str = aven_str_init(""),
                    .codepoints = 0,
                },
            },
            {
                .desc = aven_str("aven_str codepoints ascii str"),
                .fn = test_aven_str_codepoints,
                .args = &(TestAvenStrCodepointsArgs){
                    .str = aven_str_init("Hello, World!"),
                    .codepoints = 13,
                },
            },
            {
                .desc = aven_str("aven_str codepoints poop str"),
                .fn = test_aven_str_codepoints,
                .args = &(TestAvenStrCodepointsArgs){
                    .str = aven_str_init("Hello 💩 World!"),
                    .codepoints = 14,
                },
            },
            {
                .desc = aven_str("aven_str invalid utf8 codepoint"),
                .fn = test_aven_str_codepoints,
                .args = &(TestAvenStrCodepointsArgs){
                    .str = slice_array(
                        (char[]){
                            'H',
                            'e',
                            'l',
                            'l',
                            'o',
                            ',',
                            (char)0xff,
                            (char)0xff,
                            (char)0xff,
                            (char)0xff,
                            (char)0xff,
                        }
                    ),
                    .inv_index = { .valid = true, .value = 6 },
                },
            },
        };
        AvenTestCaseSlice tcases = slice_array(tcase_data);

        aven_test(tcases, arena);

        return 0;
    }
#endif
