#ifndef AVEN_BUILD_H
    #define AVEN_BUILD_H

    #include "../aven.h"
    #include "arena.h"
    #include "fs.h"
    #include "hash.h"
    #include "proc.h"
    #include "str.h"
    #include "io.h"

    typedef enum {
        AVEN_BUILD_STEP_STATE_NONE = 0,
        AVEN_BUILD_STEP_STATE_RUNNING,
        AVEN_BUILD_STEP_STATE_DONE,
    } AvenBuildStepState;

    typedef enum {
        AVEN_BUILD_STEP_TYPE_ROOT = 0,
        AVEN_BUILD_STEP_TYPE_PATH,
        AVEN_BUILD_STEP_TYPE_CMD,
        AVEN_BUILD_STEP_TYPE_COPY,
        AVEN_BUILD_STEP_TYPE_RM,
        AVEN_BUILD_STEP_TYPE_RMDIR,
        AVEN_BUILD_STEP_TYPE_MKDIR,
        AVEN_BUILD_STEP_TYPE_TRUNC,
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
        AvenBuildOptionalPath out_path;
        AvenBuildStepData data;
        AvenBuildStepState state;
        AvenProcId pid;
        AvenBuildStepType type;
        bool cached;
        bool always_run;
    } AvenBuildStep;

    struct AvenBuildStepNode {
        AvenBuildStepNode *next;
        AvenBuildStep *step;
    };

    typedef Optional(AvenBuildStep) AvenBuildStepOptional;
    typedef Optional(AvenBuildStep *) AvenBuildStepPtrOptional;
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

    static inline bool aven_build_step_cache_able(AvenBuildStep *step) {
        switch (step->type) {
            case AVEN_BUILD_STEP_TYPE_PATH:
            case AVEN_BUILD_STEP_TYPE_CMD:
            case AVEN_BUILD_STEP_TYPE_COPY:
                return step->out_path.valid;
            default:
                break;
        }
        return false;
    }

    static inline AvenStr aven_build_step_cache_path(
        AvenBuildStep *step,
        AvenArena *arena
    ) {
        AvenStr out_path = unwrap(step->out_path);
        AvenStr dir_path = aven_path_containing_dir(out_path);
        AvenStr fname = aven_path_fname(out_path);
        AvenStr cache_fname = aven_str_concat(aven_str(".cache."), fname, arena);

        return aven_path(arena, dir_path, cache_fname);
    }

    typedef Optional(uint64_t) AvenBuildStepHashOpt;

    static inline AvenBuildStepHashOpt aven_build_step_hash(
        AvenBuildStep *step,
        AvenArena arena
    ) {
        assert(aven_build_step_cache_able(step));
        AvenIoOpenResult opr_res = aven_io_open(
            unwrap(step->out_path),
            AVEN_IO_OPEN_MODE_READ,
            arena
        );
        if (opr_res.error != 0) {
            return (AvenBuildStepHashOpt){ 0 };
        }
        AvenIoReader reader = aven_io_reader_init_fd(opr_res.payload);
        AvenIoBytesResult rd_res = aven_io_reader_pop_all(&reader, 8096, &arena);
        aven_io_close(opr_res.payload);
        if (rd_res.error != 0) {
            return (AvenBuildStepHashOpt){ 0 };
        }
        AvenHashCtx hctx = aven_hash_init(0);
        return (AvenBuildStepHashOpt){
            .valid = true,
            .value = aven_hash(&hctx, rd_res.payload),
        };
    }

    static inline bool aven_build_step_cache_validate(
        AvenBuildStep *step,
        AvenArena arena
    ) {
        if (!aven_build_step_cache_able(step)) {
            return false;
        }

        AvenBuildStepHashOpt hash_opt = aven_build_step_hash(step, arena);
        if (!hash_opt.valid) {
            return false;
        }
        uint64_t hash = unwrap(hash_opt);

        AvenStr path = aven_build_step_cache_path(step, &arena);
        AvenIoOpenResult op_res = aven_io_open(
            path,
            AVEN_IO_OPEN_MODE_READ,
            arena
        );
        if (op_res.error != 0) {
            return false;
        }
        uint64_t old_hash;
        ByteSlice old_hash_bytes = as_bytes(&old_hash);
        AvenIoReadResult rd_res = aven_io_read(op_res.payload, old_hash_bytes);
        aven_io_close(op_res.payload);
        if (rd_res.error != 0 or rd_res.payload != old_hash_bytes.len) {
            return false;
        }
        return hash == old_hash;
    }

    static inline void aven_build_step_cache_update(
        AvenBuildStep *step,
        AvenArena arena
    ) {
        if (!aven_build_step_cache_able(step)) {
            return;
        }

        AvenBuildStepHashOpt hash_opt = aven_build_step_hash(step, arena);
        if (!hash_opt.valid) {
            return;
        }
        uint64_t hash = unwrap(hash_opt);

        AvenStr path = aven_build_step_cache_path(step, &arena);
        AvenIoOpenResult opw_res = aven_io_open(
            path,
            AVEN_IO_OPEN_MODE_WRITE,
            arena
        );
        if (opw_res.error != 0) {
            return;
        }

        aven_io_write(opw_res.payload, as_bytes(&hash));
        aven_io_close(opw_res.payload);
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

    static int aven_build_step_wait(AvenBuildStep *step, AvenArena arena) {
        if (step->state != AVEN_BUILD_STEP_STATE_RUNNING) {
            return 0;
        }

        AvenProcWaitResult result = aven_proc_wait(step->pid);
        step->state = AVEN_BUILD_STEP_STATE_DONE;
        if (result.error != 0) {
            return 1;
        }

        if (result.payload == 0) {
            if (!step->cached) {
                step->cached = aven_build_step_cache_validate(step, arena);
            }

            if (!step->cached) {
                aven_build_step_cache_update(step, arena);
            }
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
            AvenBuildStepRunError error = aven_build_step_run(dep->step, arena);
            if (error != 0) {
                return AVEN_BUILD_STEP_RUN_ERROR_DEPRUN;
            }
        }

        bool deps_cached = true;
        for (AvenBuildStepNode *dep = step->dep; dep != NULL; dep = dep->next) {
            int error = aven_build_step_wait(dep->step, arena);
            if (error != 0) {
                return AVEN_BUILD_STEP_RUN_ERROR_DEPWAIT;
            }
            deps_cached = deps_cached && dep->step->cached;
        }

        if (
            deps_cached &&
            !step->always_run &&
            aven_build_step_cache_validate(step, arena)
        ) {
            step->cached = true;
            step->state = AVEN_BUILD_STEP_STATE_DONE;
            return AVEN_BUILD_STEP_RUN_ERROR_NONE;
        }

        step->state = AVEN_BUILD_STEP_STATE_RUNNING;

        int error = 0;
        AvenProcCmdResult result;
        switch (step->type) {
            case AVEN_BUILD_STEP_TYPE_ROOT:
            case AVEN_BUILD_STEP_TYPE_PATH:
                step->state = AVEN_BUILD_STEP_STATE_DONE;
                step->cached = true;
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
                error = (int)aven_fs_rm(step->data.rm, arena);
                if (error != 0) {
                    return AVEN_BUILD_STEP_RUN_ERROR_RM;
                }
                step->state = AVEN_BUILD_STEP_STATE_DONE;
                break;
            case AVEN_BUILD_STEP_TYPE_RMDIR:
    #ifndef AVEN_SUPPRESS_LOGS
                aven_io_printf("rmdir {}\n", aven_fmt_str(step->data.rmdir));
    #endif
                error = (int)aven_fs_rmdir(step->data.rmdir, arena);
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
                error = (int)aven_fs_trunc(step->out_path.value, arena);
                if (error != 0) {
                    return AVEN_BUILD_STEP_RUN_ERROR_TRUNC;
                }
                step->state = AVEN_BUILD_STEP_STATE_DONE;
                break;
            case AVEN_BUILD_STEP_TYPE_MKDIR:
                if (!step->out_path.valid) {
                    return AVEN_BUILD_STEP_RUN_ERROR_OUTPATH;
                }
                error = (int)aven_fs_mkdir(step->out_path.value, arena);
                if (error != 0) {
                    if (error != AVEN_FS_MKDIR_ERROR_EXIST) {
                        return AVEN_BUILD_STEP_RUN_ERROR_MKDIR;
                    }
                    step->cached = true;
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
                error = (int)aven_fs_copy(
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

        if (step->state == AVEN_BUILD_STEP_STATE_DONE) {
            if (!step->cached) {
                step->cached = aven_build_step_cache_validate(step, arena);
            }

            if (!step->cached) {
                aven_build_step_cache_update(step, arena);
            }
        }

        return AVEN_BUILD_STEP_RUN_ERROR_NONE;
    }

    static inline void aven_build_step_dry_run(
        AvenBuildStep *step,
        AvenArena arena
    ) {
        if (step->state != AVEN_BUILD_STEP_STATE_NONE) {
            return;
        }
        for (AvenBuildStepNode *dep = step->dep; dep != NULL; dep = dep->next) {
            aven_build_step_dry_run(dep->step, arena);
        }

        switch (step->type) {
            case AVEN_BUILD_STEP_TYPE_ROOT:
            case AVEN_BUILD_STEP_TYPE_PATH:
                break;
            case AVEN_BUILD_STEP_TYPE_CMD:
                aven_io_printf(
                    "{}\n",
                    aven_fmt_str(aven_str_join(step->data.cmd, ' ', &arena))
                );
                break;
            case AVEN_BUILD_STEP_TYPE_RM:
                aven_io_printf("rm {}\n", aven_fmt_str(step->data.rm));
                break;
            case AVEN_BUILD_STEP_TYPE_RMDIR:
                aven_io_printf("rmdir {}\n", aven_fmt_str(step->data.rmdir));
                break;
            case AVEN_BUILD_STEP_TYPE_TRUNC:
                if (!step->out_path.valid) {
                    break;
                }
                aven_io_printf(
                    "truncate -s 0 {}\n",
                    aven_fmt_str(step->out_path.value)
                );
                break;
            case AVEN_BUILD_STEP_TYPE_MKDIR:
                if (!step->out_path.valid) {
                    break;
                }
                aven_io_printf("mkdir {}\n", aven_fmt_str(step->out_path.value));
                break;
            case AVEN_BUILD_STEP_TYPE_COPY:
                if (!step->out_path.valid) {
                    break;
                }
                aven_io_printf(
                    "cp {} {}\n",
                    aven_fmt_str(step->data.copy),
                    aven_fmt_str(step->out_path.value)
                );
                break;
            default:
                break;
        }

        step->state = AVEN_BUILD_STEP_STATE_DONE;
    }

    static inline void aven_build_step_clean(
        AvenBuildStep *step,
        AvenArena arena
    ) {
        if (step->out_path.valid) {
            aven_fs_rm(step->out_path.value, arena);
            aven_fs_rmdir(step->out_path.value, arena);
            if (aven_build_step_cache_able(step)) {
                aven_fs_rm(aven_build_step_cache_path(step, &arena), arena);
            }
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
        step->cached = false;

        for (AvenBuildStepNode *dep = step->dep; dep != NULL; dep = dep->next) {
            aven_build_step_reset(dep->step, arena);
        }
    }
#endif

