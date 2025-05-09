#ifndef AVEN_TEST_H
    #define AVEN_TEST_H

    #include "../aven.h"
    #include "arena.h"
    #include "io.h"

    typedef struct {
        int error;
        AvenStr message;
    } AvenTestResult;

    typedef AvenTestResult AvenTestFn(
        AvenArena *emsg_arena,
        AvenArena arena,
        void *args
    );

    typedef struct {
        AvenStr desc;
        void *args;
        AvenTestFn *fn;
    } AvenTestCase;

    typedef Slice(AvenTestCase) AvenTestCaseSlice;

    #define aven_test(cs, a) aven_test_internal(cs, aven_str(__FILE__), a)

    static inline void aven_test_internal(
        AvenTestCaseSlice tcases,
        AvenStr fname,
        AvenArena arena
    ) {
        char emessage_buffer[4096];

        aven_io_printf(
            "running {} test(s) for {}:",
            aven_fmt_uint(tcases.len),
            aven_fmt_str(fname)
        );
        size_t passed = 0;
        for (size_t i = 0; i < tcases.len; i += 1) {
            AvenArena emsg_arena = aven_arena_init(
                emessage_buffer,
                countof(emessage_buffer)
            );
            AvenTestCase *tcase = &get(tcases, i);
            AvenTestResult result = tcase->fn(&emsg_arena, arena, tcase->args);
            if (result.error != 0) {
                aven_io_printf(
                    "\n"
                    "    test \"{}\" failed:\n"
                    "        \"{}\"\n"
                    "        code: {}",
                    aven_fmt_str(tcase->desc),
                    aven_fmt_str(result.message),
                    aven_fmt_int(result.error)
                );
            } else {
                passed += 1;
            }
        }

        if (passed == tcases.len) {
            aven_io_print(" all tests passed\n");
        } else {
            aven_io_printf(
                "\ncompleted tests for {}: {} passed, {} failed\n",
                aven_fmt_str(fname),
                aven_fmt_uint(passed),
                aven_fmt_uint(tcases.len - passed)
            );
        }
    }
#endif
// TEST_H
