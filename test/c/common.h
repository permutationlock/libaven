#ifndef AVEN_BUILD_COMMON_H
    #define AVEN_BUILD_COMMON_H

    #include "../../aven.h"
    #include "../arena.h"
    #include "../arg.h"
    #include "../build.h"
    #include "../path.h"
    #include "../str.h"

    typedef struct {
        AvenStr compiler;
        AvenStr objflag;
        AvenStr outflag;
        AvenStr incflag;
        AvenStr defflag;
        AvenStrSlice flags;
        bool flagsep;
    } AvenBuildCommonCOpts;

    typedef struct {
        AvenStr linker;
        AvenStr outflag;
        AvenStr libflag;
        AvenStr conflag;
        AvenStr winflag;
        AvenStr shrflag;
        AvenStrSlice flags;
        bool flagsep;
    } AvenBuildCommonLDOpts;

    typedef struct {
        AvenStr archiver;
        AvenStr outflag;
        AvenStrSlice flags;
        bool flagsep;
    } AvenBuildCommonAROpts;

    typedef struct {
        Optional(AvenStr) compiler;
        AvenStrSlice flags;
        AvenStr outflag;
    } AvenBuildCommonWindresOpts;

    typedef struct {
        AvenBuildCommonCOpts cc;
        AvenBuildCommonLDOpts ld;
        AvenBuildCommonAROpts ar;
        AvenBuildCommonWindresOpts windres;
        AvenStrSlice obexts;
        AvenStrSlice exexts;
        AvenStrSlice soexts;
        AvenStrSlice arexts;
        AvenStrSlice wrexts;
        bool clean;
        bool test;
    } AvenBuildCommonOpts;

    static inline AvenStr aven_build_common_overview(void) {
        return aven_str("Aven C build system");
    }

    static inline AvenStr aven_build_common_usage(void) {
        return aven_str("./build [options]");
    }

    static AvenArg aven_build_common_args_data[] = {
        {
            .name = aven_str_init("test"),
            .description = aven_str_init("Build and  run tests"),
            .type = AVEN_ARG_TYPE_BOOL,
        },
        {
            .name = aven_str_init("clean"),
            .description = aven_str_init("Remove all build artifacts"),
            .type = AVEN_ARG_TYPE_BOOL,
        },
        {
            .name = aven_str_init("-cc"),
            .description = aven_str_init("C compiler exe"),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_CC)
                .data = {
                    .arg_str = aven_str_init(AVEN_BUILD_COMMON_DEFAULT_CC),
                },
    #elif defined(_WIN32)
    #if defined(__clang__)
                .data = { .arg_str = aven_str_init("clang.exe") },
    #elif defined(__GNUC__)
                .data = { .arg_str = aven_str_init("gcc.exe") },
    #elif defined(_MSC_VER)
                .data = { .arg_str = aven_str_init("cl.exe") },
    #elif defined(__TINYC__)
                .data = { .arg_str = aven_str_init("tcc.exe") },
    #else
                .data = { .arg_str = aven_str_init("") },
    #endif
    #else
    #if defined(__clang__)
                .data = { .arg_str = aven_str_init("clang") },
    #elif defined(__GNUC__)
                .data = { .arg_str = aven_str_init("gcc") },
    #elif defined(__TINYC__)
                .data = { .arg_str = aven_str_init("tcc") },
    #else
                .data = { .arg_str = aven_str_init("") },
    #endif
    #endif
            },
        },
        {
            .name = aven_str_init("-ld"),
            .description = aven_str_init(
                "Linker exe to use instead of C compiler"
            ),
            .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_LD)
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
                .data = {
                    .arg_str = aven_str_init(AVEN_BUILD_COMMON_DEFAULT_LD),
                },
            },
    #elif defined(_WIN32) and defined(_MSC_VER) and !defined(__clang__)
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
                .data = { .arg_str = aven_str_init("link.exe") },
            },
    #else
            .optional = true,
    #endif
        },
        {
            .name = aven_str_init("-ar"),
            .description = aven_str_init(
                "Archiver exe to create static libraries"
            ),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_AR)
                .data = {
                    .arg_str = aven_str_init(AVEN_BUILD_COMMON_DEFAULT_AR),
                },
    #elif defined(_WIN32)
    #if defined(__clang__)
                .data = { .arg_str = aven_str_init("llvm-ar.exe") },
    #elif defined(_MSC_VER)
                .data = { .arg_str = aven_str_init("lib.exe") },
    #elif defined(__GNUC__)
                .data = { .arg_str = aven_str_init("ar.exe") },
    #elif defined(__TINYC__)
                .data = { .arg_str = aven_str_init("tcc.exe") },
    #else
                .data = { .arg_str = aven_str_init("") },
    #endif
    #else
    #if defined(__clang__)
                .data = { .arg_str = aven_str_init("llvm-ar") },
    #elif defined(__GNUC__)
                .data = { .arg_str = aven_str_init("ar") },
    #elif defined(__TINYC__)
                .data = { .arg_str = aven_str_init("tcc") },
    #else
                .data = { .arg_str = aven_str_init("") },
    #endif
    #endif
            },
        },
        {
            .name = aven_str_init("-windres"),
            .description = aven_str_init("Windows resource compiler"),
            .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_WINDRES)
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
                .data = {
                    .arg_str = aven_str_init(AVEN_BUILD_COMMON_DEFAULT_WINDRES),
                },
            },
    #elif defined(_WIN32)
    #if defined(__clang__)
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
                .data = { .arg_str = aven_str_init("llvm-windres.exe") },
            },
    #elif defined(_MSC_VER)
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
                .data = { .arg_str = aven_str_init("rc.exe") },
            },
    #elif defined(__GNUC__)
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
                .data = { .arg_str = aven_str_init("windres.exe") },
            },
    #else
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
                .data = { .arg_str = aven_str_init("") },
            },
    #endif
    #else
            .optional = true,
    #endif
        },
        {
            .name = aven_str_init("-ccflags"),
            .description = aven_str_init("C compiler common flags"),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_CCFLAGS)
                .data = {
                    .arg_str = aven_str_init(AVEN_BUILD_COMMON_DEFAULT_CCFLAGS),
                },
    #elif defined(_WIN32) and defined(_MSC_VER)
    #if defined(__clang__)
                .data = { .arg_str = aven_str_init("-std=c11") },
    #else
                .data = {
                    .arg_str = aven_str_init(
                        "/std:c11 /experimental:c11atomics"
                    ),
                },
    #endif
    #elif defined(__TINYC__)
                .data = { .arg_str = aven_str_init("-std=c11") },
    #else
                .data = { .arg_str = aven_str_init("") },
    #endif
            },
        },
        {
            .name = aven_str_init("-ldflags"),
            .description = aven_str_init("Linker common flags"),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_LDFLAGS)
                .data = {
                    .arg_str = aven_str_init(AVEN_BUILD_COMMON_DEFAULT_LDFLAGS),
                },
    #else
                .data = { .arg_str = aven_str_init("") },
    #endif
            },
        },
        {
            .name = aven_str_init("-arflags"),
            .description = aven_str_init("Archiver common flags"),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_ARFLAGS)
                .data = {
                    .arg_str = aven_str_init(AVEN_BUILD_COMMON_DEFAULT_ARFLAGS),
                },
    #elif defined(_WIN32) and defined(_MSC_VER) and !defined(__clang__)
                .data = { .arg_str = aven_str_init("") },
    #elif defined(__TINYC__)
                .data = { .arg_str = aven_str_init("-ar -rcs") },
    #else
                .data = { .arg_str = aven_str_init("-rcs") },
    #endif
            },
        },
        {
            .name = aven_str_init("-windresflags"),
            .description = aven_str_init(
                "Windows resource compiler common flags"
            ),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_WINDRESFLAGS)
                .data = {
                    .arg_str = aven_str_init(
                        AVEN_BUILD_COMMON_DEFAULT_WINDRESFLAGS
                    ),
                },
    #else
                .data = { .arg_str = aven_str_init("") },
    #endif
            },
        },
        {
            .name = aven_str_init("-obext"),
            .description = aven_str_init("File extension(s) for object files"),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_OBEXT)
                .data = {
                    .arg_str = aven_str_init(AVEN_BUILD_COMMON_DEFAULT_OBEXT),
                },
    #elif defined(_WIN32)
    #if defined(_MSC_VER)
                .data = { .arg_str = aven_str_init(".obj .exp .pdb") },
    #else
                .data = { .arg_str = aven_str_init(".o") },
    #endif
    #else
                .data = { .arg_str = aven_str_init(".o") },
    #endif
            },
        },
        {
            .name = aven_str_init("-exext"),
            .description = aven_str_init("File extension(s) for executables"),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_EXEXT)
                .data = {
                    .arg_str = aven_str_init(AVEN_BUILD_COMMON_DEFAULT_EXEXT),
                },
    #elif defined(_WIN32)
                .data = { .arg_str = aven_str_init(".exe .pdb .lib .exp") },
    #else
                .data = { .arg_str = aven_str_init("") },
    #endif
            },
        },
        {
            .name = aven_str_init("-soext"),
            .description = aven_str_init(
                "File extension(s) for shared library files"
            ),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_SOEXT)
                .data = {
                    .arg_str = aven_str_init(AVEN_BUILD_COMMON_DEFAULT_SOEXT),
                },
    #elif defined(_WIN32)
                .data = {
                    .arg_str = aven_str_init(
                        ".dll .lib .pdb .exp _aven_dl_loaded.dll"
                    ),
                },
    #else
                .data = { .arg_str = aven_str_init(".so") },
    #endif
            },
        },
        {
            .name = aven_str_init("-arext"),
            .description = aven_str_init(
                "File extension(s) for static libraries"
            ),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_AREXT)
                .data = {
                    .arg_str = aven_str_init(AVEN_BUILD_COMMON_DEFAULT_AREXT),
                },
    #elif defined(_WIN32)
    #if defined(_MSC_VER)
                .data = { .arg_str = aven_str_init(".lib .exp .pdb") },
    #else
                .data = { .arg_str = aven_str_init(".a") },
    #endif
    #else
                .data = { .arg_str = aven_str_init(".a") },
    #endif
            },
        },
        {
            .name = aven_str_init("-wrext"),
            .description = aven_str_init(
                "File extension(s) for windows resource files"
            ),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_AREXT)
                .data = {
                    .arg_str = aven_str_init(AVEN_BUILD_COMMON_DEFAULT_AREXT),
                },
    #elif defined(_WIN32)
    #if defined(_MSC_VER)
                .data = { .arg_str = aven_str_init(".res") },
    #else
                .data = { .arg_str = aven_str_init(".o") },
    #endif
    #else
                .data = { .arg_str = aven_str_init(".o") },
    #endif
            },
        },
        {
            .name = aven_str_init("-ccincflag"),
            .description = aven_str_init("C compiler flag to add include path"),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_CCINCFLAG)
                .data = {
                    .arg_str = aven_str_init(
                        AVEN_BUILD_COMMON_DEFAULT_CCINCFLAGEXT
                    ),
                },
    #elif defined(_WIN32) and defined(_MSC_VER) and !defined(__clang__)
                .data = { .arg_str = aven_str_init("/I") },
    #else
                .data = { .arg_str = aven_str_init("-I") },
    #endif
            },
        },
        {
            .name = aven_str_init("-ccdefflag"),
            .description = aven_str_init("C compiler flag to define macro"),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_CCDEFFLAG)
                .data = {
                    .arg_str = aven_str_init(
                        AVEN_BUILD_COMMON_DEFAULT_CCDEFFLAGEXT
                    ),
                },
    #elif defined(_WIN32) and defined(_MSC_VER) and !defined(__clang__)
                .data = { .arg_str = aven_str_init("/D") },
    #else
                .data = { .arg_str = aven_str_init("-D") },
    #endif
            },
        },
        {
            .name = aven_str_init("-ccobjflag"),
            .description = aven_str_init("C compiler flag to compile object"),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_CCOBJFLAG)
                .data = {
                    .arg_str = aven_str_init(
                        AVEN_BUILD_COMMON_DEFAULT_COBJFLAGEXT
                    ),
                },
    #elif defined(_WIN32) and defined(_MSC_VER) and !defined(__clang__)
                .data = { .arg_str = aven_str_init("/c") },
    #else
                .data = { .arg_str = aven_str_init("-c") },
    #endif
            },
        },
        {
            .name = aven_str_init("-ccoutflag"),
            .description = aven_str_init(
                "C compiler flag to specify output file"
            ),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_CCOUTFLAG)
                .data = {
                    .arg_str = aven_str_init(
                        AVEN_BUILD_COMMON_DEFAULT_COUTFLAGEXT
                    ),
                },
    #elif defined(_WIN32) and defined(_MSC_VER) and !defined(__clang__)
                .data = { .arg_str = aven_str_init("/Fo:") },
    #else
                .data = { .arg_str = aven_str_init("-o") },
    #endif
            },
        },
        {
            .name = aven_str_init("-ldlibflag"),
            .description = aven_str_init("Linker flag to link library"),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_LDLIBFLAG)
                .data = {
                    .arg_str = aven_str_init(
                        AVEN_BUILD_COMMON_DEFAULT_LDLIBFLAG
                    ),
                },
    #elif defined(_WIN32) and defined(_MSC_VER) and !defined(__clang__)
                .data = { .arg_str = aven_str_init("") },
    #else
                .data = { .arg_str = aven_str_init("-l") },
    #endif
            },
        },
        {
            .name = aven_str_init("-ldshrflag"),
            .description = aven_str_init("Linker flag to emit shared library"),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_LDSHRFLAG)
                .data = {
                    .arg_str = aven_str_init(
                        AVEN_BUILD_COMMON_DEFAULT_LDSHRFLAG
                    ),
                },
    #elif defined(_WIN32) and defined(_MSC_VER) and !defined(__clang__)
                .data = { .arg_str = aven_str_init("/DLL") },
    #else
                .data = { .arg_str = aven_str_init("-shared") },
    #endif
            },
        },
        {
            .name = aven_str_init("-ldconflag"),
            .description = aven_str_init(
                "Linker flag to link a console application"
            ),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_LDCONFLAG)
                .data = {
                    .arg_str = aven_str_init(
                        AVEN_BUILD_COMMON_DEFAULT_LDCONFLAG
                    ),
                },
    #elif defined(_WIN32)
    #if defined(__clang__)
    #if defined(_MSC_VER)
                .data = { .arg_str = aven_str_init("-Wl,/SUBSYSTEM:CONSOLE") },
    #else
                .data = { .arg_str = aven_str_init("-Wl,--subsystem,console") },
    #endif
    #elif defined(_MSC_VER)
                .data = { .arg_str = aven_str_init("/SUBSYSTEM:CONSOLE") },
    #elif defined(__GNUC__)
                .data = { .arg_str = aven_str_init("-mconsole") },
    #else
                .data = { .arg_str = aven_str_init("") },
    #endif
    #else
                .data = { .arg_str = aven_str_init("") },
    #endif
            },
        },
        {
            .name = aven_str_init("-ldwinflag"),
            .description = aven_str_init(
                "Linker flag to link a graphical window application"
            ),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_LDWINFLAG)
                .data = {
                    .arg_str = aven_str_init(
                        AVEN_BUILD_COMMON_DEFAULT_LDWINFLAG
                    ),
                },
    #elif defined(_WIN32)
    #if defined(__clang__)
    #if defined(_MSC_VER)
                .data = { .arg_str = aven_str_init("-Wl,/SUBSYSTEM:WINDOWS") },
    #else
                .data = { .arg_str = aven_str_init("-Wl,--subsystem,windows") },
    #endif
    #elif defined(_MSC_VER)
                .data = { .arg_str = aven_str_init("/SUBSYSTEM:WINDOWS") },
    #elif defined(__GNUC__)
                .data = { .arg_str = aven_str_init("-mwindows") },
    #else
                .data = { .arg_str = aven_str_init("") },
    #endif
    #else
                .data = { .arg_str = aven_str_init("") },
    #endif
            },
        },
        {
            .name = aven_str_init("-ldoutflag"),
            .description = aven_str_init("Linker flag to specify output file"),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_LDOUTFLAG)
                .data = {
                    .arg_str = aven_str_init(
                        AVEN_BUILD_COMMON_DEFAULT_LDOUTFLAG
                    ),
                },
    #elif defined(_WIN32) and defined(_MSC_VER) and !defined(__clang__)
                .data = { .arg_str = aven_str_init("/OUT:") },
    #else
                .data = { .arg_str = aven_str_init("-o") },
    #endif
            },
        },
        {
            .name = aven_str_init("-aroutflag"),
            .description = aven_str_init("Archiver flag to specify output file"),
            .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_AROUTFLAG)
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
                .data = {
                    .arg_str = aven_str_init(
                        AVEN_BUILD_COMMON_DEFAULT_AROUTFLAG
                    ),
                },
            },
    #elif defined(_WIN32) and defined(_MSC_VER) and !defined(__clang__)
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
                .data = { .arg_str = aven_str_init("/OUT:") },
            },
    #else
            .optional = true,
    #endif
        },
        {
            .name = aven_str_init("-windresoutflag"),
            .description = aven_str_init(
                "Windows res compiler flag to specify output file"
            ),
            .type = AVEN_ARG_TYPE_STRING,
            .value = {
                .type = AVEN_ARG_TYPE_STRING,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_WINDRESOUTFLAG)
                .data = {
                    .arg_str = aven_str_init(
                        AVEN_BUILD_COMMON_DEFAULT_WINDRESOUTFLAG
                    ),
                },
    #elif defined(_WIN32) and defined(_MSC_VER) and !defined(__clang__)
                .data = { .arg_str = aven_str_init("/fo") },
    #else
                .data = { .arg_str = aven_str_init("-o") },
    #endif
            },
        },
        {
            .name = aven_str_init("-ccflagsep"),
            .description = aven_str_init(
                "C compiler add space between flag and argument"
            ),
            .type = AVEN_ARG_TYPE_BOOL,
            .value = {
                .type = AVEN_ARG_TYPE_BOOL,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_CCFLAGSPACES)
                .data = { .arg_bool = AVEN_BUILD_COMMON_DEFAULT_CCFLAGSPACES },
    #elif defined(_WIN32) and defined(_MSC_VER) and !defined(__clang__)
                .data = { .arg_bool = false },
    #else
                .data = { .arg_bool = true },
    #endif
            },
        },
        {
            .name = aven_str_init("-ldflagsep"),
            .description = aven_str_init(
                "Linker add space between flag and argument"
            ),
            .type = AVEN_ARG_TYPE_BOOL,
            .value = {
                .type = AVEN_ARG_TYPE_BOOL,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_LDFLAGSPACES)
                .data = { .arg_bool = AVEN_BUILD_COMMON_DEFAULT_LDFLAGSPACES },
    #elif defined(_WIN32) and defined(_MSC_VER) and !defined(__clang__)
                .data = { .arg_bool = false },
    #else
                .data = { .arg_bool = true },
    #endif
            },
        },
        {
            .name = aven_str_init("-arflagsep"),
            .description = aven_str_init(
                "Archiver add space between flag and argument"
            ),
            .type = AVEN_ARG_TYPE_BOOL,
            .value = {
                .type = AVEN_ARG_TYPE_BOOL,
    #if defined(AVEN_BUILD_COMMON_DEFAULT_ARFLAGSPACES)
                .data = { .arg_bool = AVEN_BUILD_COMMON_DEFAULT_ARFLAGSPACES },
    #elif defined(_WIN32) and defined(_MSC_VER) and !defined(__clang__)
                .data = { .arg_bool = false },
    #else
                .data = { .arg_bool = true },
    #endif
            },
        },
    };

    static inline AvenArgSlice aven_build_common_args(void) {
        AvenArgSlice args = slice_array(aven_build_common_args_data);
        return args;
    }

    static inline AvenBuildCommonOpts aven_build_common_opts(
        AvenArgSlice arg_slice,
        AvenArena *arena
    ) {
        AvenBuildCommonOpts opts = { 0 };

        opts.test = aven_arg_get_bool(arg_slice, "test");
        opts.clean = aven_arg_get_bool(arg_slice, "clean");

        opts.cc.compiler = aven_arg_get_str(arg_slice, "-cc");
        opts.cc.incflag = aven_arg_get_str(arg_slice, "-ccincflag");
        opts.cc.defflag = aven_arg_get_str(arg_slice, "-ccdefflag");
        opts.cc.objflag = aven_arg_get_str(arg_slice, "-ccobjflag");
        opts.cc.outflag = aven_arg_get_str(arg_slice, "-ccoutflag");
        opts.cc.flagsep = aven_arg_get_bool(arg_slice, "-ccflagsep");
        opts.cc.flags = aven_str_split(
            aven_arg_get_str(arg_slice, "-ccflags"),
            ' ',
            arena
        );

        if (aven_arg_has_arg(arg_slice, "-ld")) {
            opts.ld.linker = aven_arg_get_str(arg_slice, "-ld");
        } else {
            opts.ld.linker = opts.cc.compiler;
        }
        opts.ld.outflag = aven_arg_get_str(arg_slice, "-ldoutflag");
        opts.ld.libflag = aven_arg_get_str(arg_slice, "-ldlibflag");
        opts.ld.conflag = aven_arg_get_str(arg_slice, "-ldconflag");
        opts.ld.winflag = aven_arg_get_str(arg_slice, "-ldwinflag");
        opts.ld.shrflag = aven_arg_get_str(arg_slice, "-ldshrflag");
        opts.ld.flagsep = aven_arg_get_bool(arg_slice, "-ldflagsep");
        opts.ld.flags = aven_str_split(
            aven_arg_get_str(arg_slice, "-ldflags"),
            ' ',
            arena
        );

        opts.ar.archiver = aven_arg_get_str(arg_slice, "-ar");
        if (aven_arg_has_arg(arg_slice, "-aroutflag")) {
            opts.ar.outflag = aven_arg_get_str(arg_slice, "-aroutflag");
        } else {
            opts.ar.outflag = aven_str("");
        }
        opts.ar.flagsep = aven_arg_get_bool(arg_slice, "-arflagsep");
        opts.ar.flags = aven_str_split(
            aven_arg_get_str(arg_slice, "-arflags"),
            ' ',
            arena
        );

        if (aven_arg_has_arg(arg_slice, "-windres")) {
            opts.windres.compiler.valid = true;
            opts.windres.compiler.value = aven_arg_get_str(
                arg_slice,
                "-windres"
            );
            opts.windres.flags = aven_str_split(
                aven_arg_get_str(arg_slice, "-windresflags"),
                ' ',
                arena
            );
            opts.windres.outflag = aven_arg_get_str(
                arg_slice,
                "-windresoutflag"
            );
        }

        opts.obexts = aven_str_split(
            aven_arg_get_str(arg_slice, "-obext"),
            ' ',
            arena
        );
        opts.exexts = aven_str_split(
            aven_arg_get_str(arg_slice, "-exext"),
            ' ',
            arena
        );
        opts.soexts = aven_str_split(
            aven_arg_get_str(arg_slice, "-soext"),
            ' ',
            arena
        );
        opts.arexts = aven_str_split(
            aven_arg_get_str(arg_slice, "-arext"),
            ' ',
            arena
        );
        opts.wrexts = aven_str_split(
            aven_arg_get_str(arg_slice, "-wrext"),
            ' ',
            arena
        );

        return opts;
    }

    static inline AvenBuildStep aven_build_common_step_subdir(
        AvenBuildStep *dir_step,
        AvenStr subdir_name,
        AvenArena *arena
    ) {
        AvenStr dir_path = unwrap(dir_step->out_path);

        AvenBuildStep subdir_step = aven_build_step_mkdir(
            aven_path(arena, dir_path, subdir_name)
        );
        aven_build_step_add_dep(&subdir_step, dir_step, arena);
        return subdir_step;
    }

    static inline void aven_build_common_step_add_path_deps(
        AvenBuildStep *cmd_step,
        AvenBuildStep *dir_step,
        AvenStr fname,
        AvenStrSlice exts,
        AvenArena *arena
    ) {
        AvenStr dir_path = unwrap(dir_step->out_path);

        for (size_t i = 0; i < exts.len; i += 1) {
            AvenBuildStep *path_step = aven_arena_create(AvenBuildStep, arena);
            *path_step = aven_build_step_path(
                aven_path(
                    arena,
                    dir_path,
                    aven_str_concat(fname, get(exts, i), arena)
                )
            );
            aven_build_step_add_dep(path_step, dir_step, arena);
            aven_build_step_add_dep(cmd_step, path_step, arena);
        }
    }

    static inline AvenBuildStep aven_build_common_step_cc_ex(
        AvenBuildCommonOpts *opts,
        AvenStrSlice includes,
        AvenStrSlice macros,
        AvenStr src_path,
        AvenBuildStep *out_dir_step,
        AvenArena *arena
    ) {
        AvenStr out_dir_path = unwrap(out_dir_step->out_path);

        AvenStr src_fname = aven_path_fname(src_path, arena);
        AvenStrSlice ext_split = aven_str_split(src_fname, '.', arena);
        AvenStr ext_free_fname = get(ext_split, 0);

        AvenStr out_fname = ext_free_fname;
        if (opts->obexts.len > 0) {
            out_fname = aven_str_concat(out_fname, get(opts->obexts, 0), arena);
        }
        AvenStr target_path = aven_path(arena, out_dir_path, out_fname);

        List(AvenStr) cmd_list = aven_arena_create_list(
            AvenStr,
            arena,
            5 + opts->cc.flags.len + 2 * includes.len + 2 * macros.len
        );

        list_push(cmd_list) = opts->cc.compiler;

        for (size_t j = 0; j < opts->cc.flags.len; j += 1) {
            list_push(cmd_list) = get(opts->cc.flags, j);
        }

        for (size_t j = 0; j < includes.len; j += 1) {
            if (opts->cc.flagsep) {
                list_push(cmd_list) = opts->cc.incflag;
                list_push(cmd_list) = get(includes, j);
            } else {
                list_push(cmd_list) = aven_str_concat(
                    opts->cc.incflag,
                    get(includes, j),
                    arena
                );
            }
        }

        for (size_t j = 0; j < macros.len; j += 1) {
            if (opts->cc.flagsep) {
                list_push(cmd_list) = opts->cc.defflag;
                list_push(cmd_list) = get(macros, j);
            } else {
                list_push(cmd_list) = aven_str_concat(
                    opts->cc.defflag,
                    get(macros, j),
                    arena
                );
            }
        }

        list_push(cmd_list) = opts->cc.objflag;

        if (opts->cc.flagsep) {
            list_push(cmd_list) = opts->cc.outflag;
            list_push(cmd_list) = target_path;
        } else {
            list_push(cmd_list) = aven_str_concat(
                opts->cc.outflag,
                target_path,
                arena
            );
        }
        list_push(cmd_list) = src_path;

        AvenStrSlice cmd_slice = slice_list(cmd_list);

        AvenBuildOptionalPath out_path = { .value = target_path, .valid = true };
        AvenBuildStep cc_step = aven_build_step_cmd(out_path, cmd_slice);
        aven_build_step_add_dep(&cc_step, out_dir_step, arena);

        if (opts->obexts.len > 1) {
            AvenStrSlice extra_exts = slice_tail(opts->obexts, 1);
            aven_build_common_step_add_path_deps(
                &cc_step,
                out_dir_step,
                ext_free_fname,
                extra_exts,
                arena
            );
        }

        return cc_step;
    }

    static inline AvenBuildStep aven_build_common_step_cc(
        AvenBuildCommonOpts *opts,
        AvenStr src_path,
        AvenBuildStep *out_dir_step,
        AvenArena *arena
    ) {
        return aven_build_common_step_cc_ex(
            opts,
            (AvenStrSlice){ 0 },
            (AvenStrSlice){ 0 },
            src_path,
            out_dir_step,
            arena
        );
    }

    typedef enum {
        AVEN_BUILD_COMMON_BIN_TYPE_CONSOLE,
        AVEN_BUILD_COMMON_BIN_TYPE_WINDOW,
        AVEN_BUILD_COMMON_BIN_TYPE_DLL,
    } AvenBuildCommonBinType;

    static AvenBuildStep aven_build_common_step_ld(
        AvenBuildCommonOpts *opts,
        AvenStrSlice linked_libs,
        AvenBuildStepPtrSlice obj_steps,
        AvenBuildStep *out_dir_step,
        AvenStr out_fname,
        AvenBuildCommonBinType bin_type,
        AvenArena *arena
    ) {
        AvenStr out_dir_path = unwrap(out_dir_step->out_path);

        AvenStrSlice exts = opts->exexts;
        if (bin_type == AVEN_BUILD_COMMON_BIN_TYPE_DLL) {
            exts = opts->soexts;
        }

        AvenStr ext_free_fname = out_fname;
        if (exts.len > 0) {
            out_fname = aven_str_concat(out_fname, get(exts, 0), arena);
        }
        AvenStr target_path = aven_path(arena, out_dir_path, out_fname);

        List(AvenStr) cmd_list = aven_arena_create_list(
            AvenStr,
            arena,
            4 + opts->ld.flags.len + obj_steps.len + 2 * linked_libs.len
        );

        list_push(cmd_list) = opts->ld.linker;

        for (size_t j = 0; j < opts->ld.flags.len; j += 1) {
            list_push(cmd_list) = get(opts->ld.flags, j);
        }

        switch (bin_type) {
            case AVEN_BUILD_COMMON_BIN_TYPE_WINDOW:
                if (opts->ld.winflag.len > 0) {
                    list_push(cmd_list) = opts->ld.winflag;
                }
                break;
            case AVEN_BUILD_COMMON_BIN_TYPE_DLL:
                if (opts->ld.shrflag.len > 0) {
                    list_push(cmd_list) = opts->ld.shrflag;
                }
                break;
            case AVEN_BUILD_COMMON_BIN_TYPE_CONSOLE:
                if (opts->ld.conflag.len > 0) {
                    list_push(cmd_list) = opts->ld.conflag;
                }
                break;
        }

        if (opts->ld.flagsep) {
            list_push(cmd_list) = opts->ld.outflag;
            list_push(cmd_list) = target_path;
        } else {
            list_push(cmd_list) = aven_str_concat(
                opts->ld.outflag,
                target_path,
                arena
            );
        }

        for (size_t j = 0; j < obj_steps.len; j += 1) {
            AvenBuildStep *obj_step = get(obj_steps, j);
            list_push(cmd_list) = unwrap(obj_step->out_path);
        }

        for (size_t j = 0; j < linked_libs.len; j += 1) {
            if (opts->ld.flagsep) {
                list_push(cmd_list) = opts->ld.libflag;
                list_push(cmd_list) = get(linked_libs, j);
            } else {
                list_push(cmd_list) = aven_str_concat(
                    opts->ld.libflag,
                    get(linked_libs, j),
                    arena
                );
            }
        }

        AvenStrSlice cmd_slice = slice_list(cmd_list);

        AvenBuildOptionalPath out_path = { .value = target_path, .valid = true };
        AvenBuildStep link_step = aven_build_step_cmd(out_path, cmd_slice);

        for (size_t j = 0; j < obj_steps.len; j += 1) {
            aven_build_step_add_dep(&link_step, get(obj_steps, j), arena);
        }
        aven_build_step_add_dep(&link_step, out_dir_step, arena);

        if (exts.len > 1) {
            AvenStrSlice extra_exts = slice_tail(exts, 1);
            aven_build_common_step_add_path_deps(
                &link_step,
                out_dir_step,
                ext_free_fname,
                extra_exts,
                arena
            );
        }

        return link_step;
    }

    static inline AvenBuildStep aven_build_common_step_ld_exe_ex(
        AvenBuildCommonOpts *opts,
        AvenStrSlice linked_libs,
        AvenBuildStepPtrSlice obj_steps,
        AvenBuildStep *out_dir_step,
        AvenStr out_fname,
        bool graphical,
        AvenArena *arena
    ) {
        return aven_build_common_step_ld(
            opts,
            linked_libs,
            obj_steps,
            out_dir_step,
            out_fname,
            graphical ?
                AVEN_BUILD_COMMON_BIN_TYPE_WINDOW :
                AVEN_BUILD_COMMON_BIN_TYPE_CONSOLE,
            arena
        );
    }

    static inline AvenBuildStep aven_build_common_step_ld_so_ex(
        AvenBuildCommonOpts *opts,
        AvenStrSlice linked_libs,
        AvenBuildStepPtrSlice obj_steps,
        AvenBuildStep *out_dir_step,
        AvenStr out_fname,
        AvenArena *arena
    ) {
        return aven_build_common_step_ld(
            opts,
            linked_libs,
            obj_steps,
            out_dir_step,
            out_fname,
            AVEN_BUILD_COMMON_BIN_TYPE_DLL,
            arena
        );
    }

    static inline AvenBuildStep aven_build_common_step_ld_exe(
        AvenBuildCommonOpts *opts,
        AvenBuildStepPtrSlice obj_steps,
        AvenBuildStep *out_dir_step,
        AvenStr out_fname,
        bool graphical,
        AvenArena *arena
    ) {
        return aven_build_common_step_ld_exe_ex(
            opts,
            (AvenStrSlice){ 0 },
            obj_steps,
            out_dir_step,
            out_fname,
            graphical,
            arena
        );
    }

    static inline AvenBuildStep aven_build_common_step_ld_so(
        AvenBuildCommonOpts *opts,
        AvenBuildStepPtrSlice obj_steps,
        AvenBuildStep *out_dir_step,
        AvenStr out_fname,
        AvenArena *arena
    ) {
        return aven_build_common_step_ld_so_ex(
            opts,
            (AvenStrSlice){ 0 },
            obj_steps,
            out_dir_step,
            out_fname,
            arena
        );
    }

    static inline AvenBuildStep aven_build_common_step_ar(
        AvenBuildCommonOpts *opts,
        AvenBuildStepPtrSlice obj_steps,
        AvenBuildStep *out_dir_step,
        AvenStr out_fname,
        AvenArena *arena
    ) {
        AvenStr out_dir_path = unwrap(out_dir_step->out_path);

        AvenStr ext_free_fname = out_fname;
        if (opts->arexts.len > 0) {
            out_fname = aven_str_concat(out_fname, get(opts->arexts, 0), arena);
        }
        AvenStr target_path = aven_path(arena, out_dir_path, out_fname);

        List(AvenStr) cmd_list = aven_arena_create_list(
            AvenStr,
            arena,
            3 + opts->ar.flags.len + obj_steps.len
        );

        list_push(cmd_list) = opts->ar.archiver;

        for (size_t j = 0; j < opts->ar.flags.len; j += 1) {
            list_push(cmd_list) = get(opts->ar.flags, j);
        }

        if (opts->ar.outflag.len != 0) {
            if (opts->ar.flagsep) {
                list_push(cmd_list) = opts->ar.outflag;
                list_push(cmd_list) = target_path;
            } else {
                list_push(cmd_list) = aven_str_concat(
                    opts->ar.outflag,
                    target_path,
                    arena
                );
            }
        } else {
            list_push(cmd_list) = target_path;
        }

        for (size_t j = 0; j < obj_steps.len; j += 1) {
            AvenBuildStep *obj_step = get(obj_steps, j);
            list_push(cmd_list) = unwrap(obj_step->out_path);
        }

        AvenStrSlice cmd_slice = slice_list(cmd_list);

        AvenBuildOptionalPath out_path = { .value = target_path, .valid = true };
        AvenBuildStep ar_step = aven_build_step_cmd(out_path, cmd_slice);

        for (size_t j = 0; j < obj_steps.len; j += 1) {
            aven_build_step_add_dep(&ar_step, get(obj_steps, j), arena);
        }
        aven_build_step_add_dep(&ar_step, out_dir_step, arena);

        if (opts->arexts.len > 1) {
            AvenStrSlice extra_exts = slice_tail(opts->arexts, 1);
            aven_build_common_step_add_path_deps(
                &ar_step,
                out_dir_step,
                ext_free_fname,
                extra_exts,
                arena
            );
        }

        return ar_step;
    }

    static inline AvenBuildStep aven_build_common_step_windres(
        AvenBuildCommonOpts *opts,
        AvenStr src_path,
        AvenBuildStep *out_dir_step,
        AvenArena *arena
    ) {
        AvenStr out_dir_path = unwrap(out_dir_step->out_path);

        AvenStr src_fname = aven_path_fname(src_path, arena);
        AvenStrSlice ext_split = aven_str_split(src_fname, '.', arena);
        AvenStr ext_free_fname = get(ext_split, 0);

        AvenStr out_fname = ext_free_fname;
        if (opts->wrexts.len > 0) {
            out_fname = aven_str_concat(out_fname, get(opts->wrexts, 0), arena);
        }
        AvenStr target_path = aven_path(arena, out_dir_path, out_fname);

        List(AvenStr) cmd_list = aven_arena_create_list(
            AvenStr,
            arena,
            4 + opts->windres.flags.len
        );

        list_push(cmd_list) = unwrap(opts->windres.compiler);

        for (size_t j = 0; j < opts->windres.flags.len; j += 1) {
            list_push(cmd_list) = get(opts->windres.flags, j);
        }

        list_push(cmd_list) = opts->windres.outflag;
        list_push(cmd_list) = target_path;
        list_push(cmd_list) = src_path;

        AvenStrSlice cmd_slice = slice_list(cmd_list);

        AvenBuildOptionalPath out_path = { .value = target_path, .valid = true };
        AvenBuildStep windres_step = aven_build_step_cmd(out_path, cmd_slice);
        aven_build_step_add_dep(&windres_step, out_dir_step, arena);

        if (opts->wrexts.len > 1) {
            AvenStrSlice extra_exts = slice_tail(opts->wrexts, 1);
            aven_build_common_step_add_path_deps(
                &windres_step,
                out_dir_step,
                ext_free_fname,
                extra_exts,
                arena
            );
        }

        return windres_step;
    }

    static AvenBuildStep aven_build_common_step_cc_ld(
        AvenBuildCommonOpts *opts,
        AvenStrSlice includes,
        AvenStrSlice macros,
        AvenStrSlice linked_libs,
        AvenBuildStepPtrSlice obj_steps,
        AvenStr src_path,
        AvenBuildStep *out_dir_step,
        AvenBuildCommonBinType bin_type,
        AvenArena *arena
    ) {
        AvenBuildStep *obj_step = aven_arena_create(AvenBuildStep, arena);
        *obj_step = aven_build_common_step_cc_ex(
            opts,
            includes,
            macros,
            src_path,
            out_dir_step,
            arena
        );

        AvenBuildStepPtrSlice exe_obj_steps = { .len = 1 + obj_steps.len };
        exe_obj_steps.ptr = aven_arena_create_array(
            AvenBuildStep *,
            arena,
            exe_obj_steps.len
        );

        size_t i = 0;
        get(exe_obj_steps, i) = obj_step;
        i += 1;
        for (size_t j = 0; j < obj_steps.len; j += 1) {
            get(exe_obj_steps, i) = get(obj_steps, j);
            i += 1;
        }

        AvenStr obj_fname = aven_path_fname(unwrap(obj_step->out_path), arena);
        if (opts->obexts.len > 0) {
            obj_fname.len -= get(opts->obexts, 0).len;
        }
        AvenStr exe_fname = aven_str_copy(obj_fname, arena);

        AvenBuildStep *bin_step = aven_arena_create(AvenBuildStep, arena);
        *bin_step = aven_build_common_step_ld(
            opts,
            linked_libs,
            exe_obj_steps,
            out_dir_step,
            exe_fname,
            bin_type,
            arena
        );

        AvenBuildStep rm_obj_step = aven_build_step_rm(obj_step->out_path.value);
        aven_build_step_add_dep(&rm_obj_step, bin_step, arena);

        rm_obj_step.out_path = bin_step->out_path;
        bin_step->out_path.valid = false;
        return rm_obj_step;
    }

    static inline AvenBuildStep aven_build_common_step_cc_ld_so_ex(
        AvenBuildCommonOpts *opts,
        AvenStrSlice includes,
        AvenStrSlice macros,
        AvenStrSlice linked_libs,
        AvenBuildStepPtrSlice obj_steps,
        AvenStr src_path,
        AvenBuildStep *out_dir_step,
        AvenArena *arena
    ) {
        return aven_build_common_step_cc_ld(
            opts,
            includes,
            macros,
            linked_libs,
            obj_steps,
            src_path,
            out_dir_step,
            AVEN_BUILD_COMMON_BIN_TYPE_DLL,
            arena
        );
    }

    static inline AvenBuildStep aven_build_common_step_cc_ld_exe_ex(
        AvenBuildCommonOpts *opts,
        AvenStrSlice includes,
        AvenStrSlice macros,
        AvenStrSlice linked_libs,
        AvenBuildStepPtrSlice obj_steps,
        AvenStr src_path,
        AvenBuildStep *out_dir_step,
        bool graphical,
        AvenArena *arena
    ) {
        return aven_build_common_step_cc_ld(
            opts,
            includes,
            macros,
            linked_libs,
            obj_steps,
            src_path,
            out_dir_step,
            graphical ?
                AVEN_BUILD_COMMON_BIN_TYPE_WINDOW :
                AVEN_BUILD_COMMON_BIN_TYPE_CONSOLE,
            arena
        );
    }

    static inline AvenBuildStep aven_build_common_step_cc_ld_so(
        AvenBuildCommonOpts *opts,
        AvenStr src_path,
        AvenBuildStep *out_dir_step,
        AvenArena *arena
    ) {
        return aven_build_common_step_cc_ld_so_ex(
            opts,
            (AvenStrSlice){ 0 },
            (AvenStrSlice){ 0 },
            (AvenStrSlice){ 0 },
            (AvenBuildStepPtrSlice){ 0 },
            src_path,
            out_dir_step,
            arena
        );
    }

    static inline AvenBuildStep aven_build_common_step_cc_ld_exe(
        AvenBuildCommonOpts *opts,
        AvenStr src_path,
        AvenBuildStep *out_dir_step,
        bool graphical,
        AvenArena *arena
    ) {
        return aven_build_common_step_cc_ld_exe_ex(
            opts,
            (AvenStrSlice){ 0 },
            (AvenStrSlice){ 0 },
            (AvenStrSlice){ 0 },
            (AvenBuildStepPtrSlice){ 0 },
            src_path,
            out_dir_step,
            graphical,
            arena
        );
    }

    static inline AvenBuildStep aven_build_common_step_run_exe(
        AvenBuildStep *exe_step,
        AvenStrSlice args,
        AvenArena *arena
    ) {
        List(AvenStr) cmd_list = aven_arena_create_list(
            AvenStr,
            arena,
            1 + args.len
        );

        list_push(cmd_list) = unwrap(exe_step->out_path);

        for (size_t j = 0; j < args.len; j += 1) {
            list_push(cmd_list) = get(args, j);
        }

        AvenStrSlice cmd_slice = slice_list(cmd_list);

        AvenBuildOptionalPath out_path = { 0 };
        AvenBuildStep run_step = aven_build_step_cmd(out_path, cmd_slice);
        aven_build_step_add_dep(&run_step, exe_step, arena);
        return run_step;
    }

    static inline AvenBuildStep aven_build_common_step_cc_ld_run_exe_ex(
        AvenBuildCommonOpts *opts,
        AvenStrSlice includes,
        AvenStrSlice macros,
        AvenStrSlice linked_libs,
        AvenBuildStepPtrSlice obj_steps,
        AvenStr src_path,
        AvenBuildStep *work_dir_step,
        bool graphical,
        AvenStrSlice args,
        AvenArena *arena
    ) {
        AvenBuildStep *exe_step = aven_arena_create(AvenBuildStep, arena);
        *exe_step = aven_build_common_step_cc_ld_exe_ex(
            opts,
            includes,
            macros,
            linked_libs,
            obj_steps,
            src_path,
            work_dir_step,
            graphical,
            arena
        );

        return aven_build_common_step_run_exe(exe_step, args, arena);
    }

    static inline AvenBuildStep aven_build_common_step_cc_ld_run_exe(
        AvenBuildCommonOpts *opts,
        AvenStr src_path,
        AvenBuildStep *work_dir_step,
        bool graphical,
        AvenStrSlice args,
        AvenArena *arena
    ) {
        return aven_build_common_step_cc_ld_run_exe_ex(
            opts,
            (AvenStrSlice){ 0 },
            (AvenStrSlice){ 0 },
            (AvenStrSlice){ 0 },
            (AvenBuildStepPtrSlice){ 0 },
            src_path,
            work_dir_step,
            graphical,
            args,
            arena
        );
    }
#endif
// AVEN_BUILD_COMMON_H
