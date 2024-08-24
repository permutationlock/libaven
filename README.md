# libaven: a small portable C library for slices, results, arenas, and more

I love programming in C, but I almost always need slices, optionals, and
result types (a.k.a. "errors as values").
The `aven.h` header contains a few minimal and unintrusive definitions.
It also defines a debugger friendly `assert` macro and bounds checked slice
access macros.

The other prominent part of the library is the build system. I've long been
frustrated by how non-portable Makefiles are, and how complicated larger build
systems are. What I wanted out of a C build system was:

 - a portable way to describe how the standard build tools
   (`cc`, `ld`, and `ar`) should be used to produce build artifacts;
 - a way to specify exactly what executables and flags should be used for each
   of these tools, e.g. how the variables `CC` and `CFLAGS` are used in
   Makefiles;
 - a portable way to interact with the filesystem through build system steps[^2];
 - an easy way to build and use the artifacts from dependent projects within
   a parent project.

The current build system accomplishes all four bullet points to my
satisfaction while remaining extremely simple.

In total library currenlty includes:

 - slices, optionals, and results: `aven.h`
 - arena allocation: `aven/arena.h` ([inspired by this post][2])
 - slice based strings: `aven/string.h`
 - command line argument parsing: `aven/arg.h`
 - a C build system: `aven/build.h`, `aven/build/common.h`
 - a tiny SIMD linear algebra library: `aven/glm.h`
 - portable shared library loading: `aven/dl.h`
 - portable directory watching: `aven/watch.h`

Everything is cross-platform (POSIX[^1] and Windows w/[MinGW][3]). 
The coding style was inspired by the [nullprogram blog][1].

## Minimizing namespace polution

All identifiers for functions, variables, and macros will be in snake case
and begin with a prefix for the corresponding header path, except for those
defined in `aven.h`. E.g. the allocate
function defined in `aven/arena.h` is `aven_arena_alloc`.

When built as a separate static library using the build system (see below),
the headers will only include the minimal freestanding C standard headers:
`stddef.h`, `iso646.h`, `stdbool.h`, and `stdint.h`. If compiling for C11 then
`stdalign.h` and `stdnoreturn.h` are also included.

When used as a header only library (by defining the `AVEN_IMPLEMENTATION` macro)
a small number of C standard library and basic POSIX headers will be included.
For Windows builds bespoke definitions are used in lieu of including `windows.h`.

## Building the library

A static library can built using the contained build system.

### Building the build system

```shell
make
```

or simply

```shell
cc -o build build.c
```

### Showing the build system help message

```shell
./build help
```

### Building the library

```shell
./build
```

### Cleaning up the build artifacts

```shell
./build clean
```

[^1]: Finding the path to a running executable is not standard even for
    POSIX systems. Currently `aven_path_exe` is implemented
    for Windows and Linux only.

[^2]: If you have ever attempted to implement a `make clean` step that works
    on Linux and Windows, then you know what I am talking about :(

[1]: https://nullprogram.com
[2]: https://nullprogram.com/blog/2023/09/27/
[3]: https://www.mingw-w64.org/
