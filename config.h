// aven cfmt columns: 100
// config.h can be used to defines custom defaults for flags

#if !defined(_WIN32) && defined(__GNUC__)
    #ifndef AVEN_BUILD_COMMON_DEFAULT_CFLAGS
        #define AVEN_BUILD_COMMON_DEFAULT_CFLAGS "-std=c11"
    #endif
    #ifndef AVEN_BUILD_COMMON_DEFAULT_CCFLAGS
        #define AVEN_BUILD_COMMON_DEFAULT_CCFLAGS \
                    "-pedantic -fstrict-aliasing -O0 -g3 -Werror -Wall -Wextra " \
                    "-Wshadow -Wconversion -Wdouble-promotion -Winit-self " \
                    "-Wcast-align -Wstrict-prototypes -Wold-style-definition " \
                    "-fsanitize-trap -fsanitize=unreachable -fsanitize=undefined"
    #endif
#endif

