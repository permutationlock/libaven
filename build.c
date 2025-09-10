#if !defined(_WIN32) && !defined(_POSIX_C_SOURCE)
    #define _POSIX_C_SOURCE 200112L
#endif

#include "config.h"

#define AVEN_IMPLEMENTATION

#include "include/aven.h"
#include "include/aven/arena.h"
#include "include/aven/build.h"
#include "include/aven/build/common.h"
#include "include/aven/io.h"
#include "include/aven/str.h"

#include "build.h"

#include <stdlib.h>

#define ARENA_SIZE (4096 * 2000)

int main(int argc, char **argv) {
    void *mem = malloc(ARENA_SIZE);
    if (mem == NULL) {
        aven_panic("malloc failed\n");
    }

    AvenArena arena = aven_arena_init(mem, ARENA_SIZE);

    AvenArgSlice common_args = aven_build_common_args();
    AvenArgSlice libaven_args = libaven_build_args();

    List(AvenArg) arg_list = aven_arena_create_list(
        AvenArg,
        &arena,
        common_args.len + libaven_args.len
    );
    for (size_t i = 0; i < common_args.len; i += 1) {
        list_push(arg_list) = get(common_args, i);
    }
    for (size_t i = 0; i < libaven_args.len; i += 1) {
        list_push(arg_list) = get(libaven_args, i);
    }
    AvenArgSlice args = slice_list(arg_list);

    AvenArgError arg_error = aven_arg_parse(
        args,
        argv,
        argc,
        aven_build_common_overview(),
        aven_build_common_usage()
    );
    if (arg_error != 0) {
        if (arg_error != AVEN_ARG_ERROR_HELP) {
            return 1;
        }
        return 0;
    }

    // Directories

    AvenStr root_dir = aven_str(".");

    // Build the library

    AvenBuildStep out_dir_step = aven_build_step_mkdir(aven_str("build_out"));

    AvenBuildCommonOpts opts = aven_build_common_opts(args, &arena);
    LibAvenBuildOpts libaven_opts = libaven_build_opts(args, &arena);

    AvenBuildStep libaven_step = libaven_build_step(
        &opts,
        root_dir,
        &out_dir_step,
        false,
        &arena
    );

    AvenBuildStep root_step = aven_build_step_root();
    aven_build_step_add_dep(&root_step, &libaven_step, &arena);

    // Build and run tests

    AvenStr aven_include = libaven_build_include_path(aven_str("."), &arena);
    AvenBuildStep test_dir_step = aven_build_step_mkdir(aven_str("build_test"));

    Optional(AvenBuildStep) winutf8_obj_step = { .valid = libaven_opts.winutf8 };
    if (winutf8_obj_step.valid) {
        winutf8_obj_step.value = libaven_build_step_windres_manifest(
            &opts,
            root_dir,
            &out_dir_step,
            &arena
        );
    }

    Optional(AvenBuildStep) winpthreads_obj_step = {
        .valid = libaven_opts.winpthreads.local,
    };
    if (winpthreads_obj_step.valid) {
        winpthreads_obj_step.value = libaven_build_step_winpthreads(
            &opts,
            &libaven_opts,
            root_dir,
            &out_dir_step,
            false,
            &arena
        );
    }

    AvenBuildStep *test_obj_step_data[2];
    List(AvenBuildStep *) test_obj_list = list_array(test_obj_step_data);
    if (winutf8_obj_step.valid) {
        list_push(test_obj_list) = &winutf8_obj_step.value;
    }
    if (winpthreads_obj_step.valid) {
        list_push(test_obj_list) = &winpthreads_obj_step.value;
    }
    AvenBuildStepPtrSlice test_obj_steps = slice_list(test_obj_list);

    AvenBuildStep test_step = aven_build_common_step_cc_ld_run_exe_ex(
        &opts,
        (AvenStrSlice){ .ptr = &aven_include, .len = 1 },
        (AvenStrSlice){ 0 },
        (AvenStrSlice){ 0 },
        test_obj_steps,
        aven_str("test.c"),
        &test_dir_step,
        false,
        (AvenStrSlice){ 0 },
        &arena
    );

    AvenBuildStep test_root_step = aven_build_step_root();
    aven_build_step_add_dep(&test_root_step, &test_step, &arena);

    // Execute the chosen build step

    if (opts.clean) {
        aven_build_step_clean(&root_step, arena);
        aven_build_step_clean(&test_root_step, arena);
    } else if (opts.test) {
        if (opts.dry_run) {
            aven_build_step_dry_run(&test_root_step, arena);
        } else {
            AvenBuildStepRunError run_error = aven_build_step_run(
                &test_root_step,
                arena
            );
            if (run_error != 0) {
                aven_io_perrf("TEST FAILED: {}\n", aven_fmt_int(run_error));
                return 1;
            }
        }
    } else {
        if (opts.dry_run) {
            aven_build_step_dry_run(&root_step, arena);
        } else {
            AvenBuildStepRunError run_error = aven_build_step_run(
                &root_step,
                arena
            );
            if (run_error != 0) {
                aven_io_perrf("BUILD FAILED: {}\n", aven_fmt_int(run_error));
                return 1;
            }
        }
    }

    return 0;
}
