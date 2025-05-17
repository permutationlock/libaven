#ifndef AVEN_BUILD_H
    #define AVEN_BUILD_H

    #include "../aven.h"
    #include "arena.h"
    #include "fs.h"
    #include "proc.h"
    #include "str.h"

    #ifndef AVEN_SUPPRESS_LOGS
        #include "io.h"
    #endif

    typedef enum {
        AVEN_BUILD_STEP_STATE_NONE = 0,
        AVEN_BUILD_STEP_STATE_RUNNING,
        AVEN_BUILD_STEP_STATE_DONE,
    } AvenBuildStepState;

    typedef enum {
        AVEN_BUILD_STEP_TYPE_ROOT = 0,
        AVEN_BUILD_STEP_TYPE_PATH,
        AVEN_BUILD_STEP_TYPE_CMD,
        AVEN_BUILD_STEP_TYPE_RM,
        AVEN_BUILD_STEP_TYPE_RMDIR,
        AVEN_BUILD_STEP_TYPE_MKDIR,
        AVEN_BUILD_STEP_TYPE_TRUNC,
        AVEN_BUILD_STEP_TYPE_COPY,
    } AvenBuildStepType;

    typedef union {
        AvenStrSlice cmd;
        AvenStr rm;
        AvenStr rmdir;
        AvenStr copy;
    } AvenBuildStepData;

    typedef Optional(AvenStr) AvenBuildOptionalPath;
    typedef struct AvenBuildStepNode AvenBuildStepNode;

    typedef struct AvenBuildStep {
        AvenBuildStepNode *dep;
        AvenBuildStepState state;
        AvenProcId pid;
        AvenBuildStepType type;
        AvenBuildStepData data;
        AvenBuildOptionalPath out_path;
    } AvenBuildStep;

    struct AvenBuildStepNode {
        AvenBuildStepNode *next;
        AvenBuildStep *step;
    };

    typedef Slice(AvenBuildStep) AvenBuildStepSlice;
    typedef Slice(AvenBuildStep *) AvenBuildStepPtrSlice;

    static inline AvenBuildStep aven_build_step_cmd(
        AvenBuildOptionalPath out_path,
        AvenStrSlice cmd_slice
    ) {
        return (AvenBuildStep){
            .type = AVEN_BUILD_STEP_TYPE_CMD,
            .data = { .cmd = cmd_slice },
            .out_path = out_path,
        };
    }

    static inline AvenBuildStep aven_build_step_root(void) {
        return (AvenBuildStep){ .type = AVEN_BUILD_STEP_TYPE_ROOT };
    }

    static inline AvenBuildStep aven_build_step_path(AvenStr path) {
        return (AvenBuildStep){
            .type = AVEN_BUILD_STEP_TYPE_PATH,
            .out_path = { .valid = true, .value = path },
        };
    }

    static inline AvenBuildStep aven_build_step_mkdir(AvenStr dir_path) {
        return (AvenBuildStep){
            .type = AVEN_BUILD_STEP_TYPE_MKDIR,
            .out_path = { .valid = true, .value = dir_path },
        };
    }

    static inline AvenBuildStep aven_build_step_rm(AvenStr file_path) {
        return (AvenBuildStep){
            .type = AVEN_BUILD_STEP_TYPE_RM,
            .data = { .rm = file_path },
        };
    }

    static inline AvenBuildStep aven_build_step_rmdir(AvenStr dir_path) {
        return (AvenBuildStep){
            .type = AVEN_BUILD_STEP_TYPE_RMDIR,
            .data = { .rmdir = dir_path },
        };
    }

    static inline AvenBuildStep aven_build_step_trunc(AvenStr file_path) {
        return (AvenBuildStep){
            .type = AVEN_BUILD_STEP_TYPE_TRUNC,
            .out_path = { .valid = true, .value = file_path },
        };
    }

    static inline AvenBuildStep aven_build_step_copy(
        AvenStr in_file_path,
        AvenStr out_file_path
    ) {
        return (AvenBuildStep){
            .type = AVEN_BUILD_STEP_TYPE_COPY,
            .data = { .copy = in_file_path },
            .out_path = { .valid = true, .value = out_file_path },
        };
    }

    static inline void aven_build_step_add_dep(
        AvenBuildStep *step,
        AvenBuildStep *dep,
        AvenArena *arena
    ) {
        AvenBuildStepNode *node = aven_arena_create(AvenBuildStepNode, arena);
        *node = (AvenBuildStepNode){ .next = step->dep, .step = dep };
        step->dep = node;
    }

    typedef enum {
        AVEN_BUILD_STEP_RUN_ERROR_NONE = 0,
        AVEN_BUILD_STEP_RUN_ERROR_DEPRUN,
        AVEN_BUILD_STEP_RUN_ERROR_DEPWAIT,
        AVEN_BUILD_STEP_RUN_ERROR_CMD,
        AVEN_BUILD_STEP_RUN_ERROR_RM,
        AVEN_BUILD_STEP_RUN_ERROR_RMDIR,
        AVEN_BUILD_STEP_RUN_ERROR_MKDIR,
        AVEN_BUILD_STEP_RUN_ERROR_TRUNC,
        AVEN_BUILD_STEP_RUN_ERROR_COPY,
        AVEN_BUILD_STEP_RUN_ERROR_OUTPATH,
        AVEN_BUILD_STEP_RUN_ERROR_BADTYPE,
    } AvenBuildStepRunError;

    static int aven_build_step_wait(AvenBuildStep *step) {
        if (step->state != AVEN_BUILD_STEP_STATE_RUNNING) {
            return 0;
        }

        AvenProcWaitResult result = aven_proc_wait(step->pid);
        step->state = AVEN_BUILD_STEP_STATE_DONE;
        if (result.error != 0) {
            return 1;
        }
        return result.payload;
    }

    static inline AvenBuildStepRunError aven_build_step_run(
        AvenBuildStep *step,
        AvenArena arena
    ) {
        if (step->state != AVEN_BUILD_STEP_STATE_NONE) {
            return AVEN_BUILD_STEP_RUN_ERROR_NONE;
        }

        for (AvenBuildStepNode *dep = step->dep; dep != NULL; dep = dep->next) {
            int error = aven_build_step_run(dep->step, arena);
            if (error != 0) {
                return AVEN_BUILD_STEP_RUN_ERROR_DEPRUN;
            }
        }

        for (AvenBuildStepNode *dep = step->dep; dep != NULL; dep = dep->next) {
            int error = aven_build_step_wait(dep->step);
            if (error != 0) {
                return AVEN_BUILD_STEP_RUN_ERROR_DEPWAIT;
            }
        }

        step->state = AVEN_BUILD_STEP_STATE_RUNNING;

        int error = 0;
        ;
        AvenProcCmdResult result;
        switch (step->type) {
            case AVEN_BUILD_STEP_TYPE_ROOT:
            case AVEN_BUILD_STEP_TYPE_PATH:
                step->state = AVEN_BUILD_STEP_STATE_DONE;
                break;
            case AVEN_BUILD_STEP_TYPE_CMD:
                result = aven_proc_cmd(step->data.cmd, arena);
                if (result.error != 0) {
                    return AVEN_BUILD_STEP_RUN_ERROR_CMD;
                }

                step->pid = result.payload;
                break;
            case AVEN_BUILD_STEP_TYPE_RM:
    #ifndef AVEN_SUPPRESS_LOGS
                aven_io_printf("rm {}\n", aven_fmt_str(step->data.rm));
    #endif
                error = aven_fs_rm(step->data.rm, arena);
                if (error != 0) {
                    return AVEN_BUILD_STEP_RUN_ERROR_RM;
                }
                step->state = AVEN_BUILD_STEP_STATE_DONE;
                break;
            case AVEN_BUILD_STEP_TYPE_RMDIR:
    #ifndef AVEN_SUPPRESS_LOGS
                aven_io_printf("rmdir {}\n", aven_fmt_str(step->data.rmdir));
    #endif
                error = aven_fs_rmdir(step->data.rmdir, arena);
                if (error != 0) {
                    return AVEN_BUILD_STEP_RUN_ERROR_RMDIR;
                }
                step->state = AVEN_BUILD_STEP_STATE_DONE;
                break;
            case AVEN_BUILD_STEP_TYPE_TRUNC:
                if (!step->out_path.valid) {
                    return AVEN_BUILD_STEP_RUN_ERROR_OUTPATH;
                }
    #ifndef AVEN_SUPPRESS_LOGS
                aven_io_printf(
                    "truncate -s 0 {}\n",
                    aven_fmt_str(step->out_path.value)
                );
    #endif
                error = aven_fs_trunc(step->out_path.value, arena);
                if (error != 0) {
                    return AVEN_BUILD_STEP_RUN_ERROR_TRUNC;
                }
                step->state = AVEN_BUILD_STEP_STATE_DONE;
                break;
            case AVEN_BUILD_STEP_TYPE_MKDIR:
                if (!step->out_path.valid) {
                    return AVEN_BUILD_STEP_RUN_ERROR_OUTPATH;
                }
                error = aven_fs_mkdir(step->out_path.value, arena);
                if (error != 0) {
                    if (error != AVEN_FS_MKDIR_ERROR_EXIST) {
                        return AVEN_BUILD_STEP_RUN_ERROR_MKDIR;
                    }
                } else {
    #ifndef AVEN_SUPPRESS_LOGS
                    aven_io_printf(
                        "mkdir {}\n",
                        aven_fmt_str(step->out_path.value)
                    );
    #endif
                }
                step->state = AVEN_BUILD_STEP_STATE_DONE;
                break;
            case AVEN_BUILD_STEP_TYPE_COPY:
                if (!step->out_path.valid) {
                    return AVEN_BUILD_STEP_RUN_ERROR_OUTPATH;
                }
                error = aven_fs_copy(
                    step->data.copy,
                    step->out_path.value,
                    arena
                );
                if (error != 0) {
                    return AVEN_BUILD_STEP_RUN_ERROR_COPY;
                }
    #ifndef AVEN_SUPPRESS_LOGS
                aven_io_printf(
                    "cp {} {}\n",
                    aven_fmt_str(step->data.copy),
                    aven_fmt_str(step->out_path.value)
                );
    #endif
                step->state = AVEN_BUILD_STEP_STATE_DONE;
                break;
            default:
                return AVEN_BUILD_STEP_RUN_ERROR_BADTYPE;
        }

        return AVEN_BUILD_STEP_RUN_ERROR_NONE;
    }

    static inline void aven_build_step_clean(
        AvenBuildStep *step,
        AvenArena arena
    ) {
        if (step->out_path.valid) {
            aven_fs_rm(step->out_path.value, arena);
            aven_fs_rmdir(step->out_path.value, arena);
        }
        step->state = AVEN_BUILD_STEP_STATE_NONE;

        for (AvenBuildStepNode *dep = step->dep; dep != NULL; dep = dep->next) {
            aven_build_step_clean(dep->step, arena);
        }
    }

    static inline void aven_build_step_reset(
        AvenBuildStep *step,
        AvenArena arena
    ) {
        step->state = AVEN_BUILD_STEP_STATE_NONE;

        for (AvenBuildStepNode *dep = step->dep; dep != NULL; dep = dep->next) {
            aven_build_step_reset(dep->step, arena);
        }
    }
#endif
// AVEN_BUILD_H
