#ifndef AVEN_DL_H
    #define AVEN_DL_H

    #include "../aven.h"
    #include "arena.h"
    #include "str.h"

    #define AVEN_DL_MAX_PATH_LEN 4096

    AVEN_FN void *aven_dl_open(AvenStr fname, AvenArena temp_arena);
    AVEN_FN void *aven_dl_sym(
        void *handle,
        AvenStr symbol,
        AvenArena temp_arena
    );
    AVEN_FN void aven_dl_close(void *handle);

    #ifdef AVEN_IMPLEMENTATION

        #ifdef _WIN32
            AVEN_FN void *aven_dl_open(AvenStr fname, AvenArena temp_arena) {
                AVEN_WIN32_FN(void *) LoadLibraryA(const char *fname);
                AVEN_WIN32_FN(int) CopyFileA(
                    const char *fname,
                    const char *copy_fname,
                    int fail_exists
                );

                if (fname.len < 5) {
                    return NULL;
                }

                int dot_index = (int)fname.len - 1;
                for (; dot_index > 0; dot_index -= 1) {
                    if (get(fname, (size_t)dot_index) == '.') {
                        break;
                    }
                }

                if (dot_index == 0) {
                    return NULL;
                }

                char aven_dl_suffix[] = "_aven_dl_loaded.dll";
                char *temp_buffer = aven_arena_alloc(
                    &temp_arena,
                    dot_index + sizeof(aven_dl_suffix),
                    1,
                    1
                );
                memcpy(temp_buffer, fname.ptr, (size_t)dot_index);
                memcpy(
                    &temp_buffer[dot_index],
                    aven_dl_suffix,
                    sizeof(aven_dl_suffix)
                );

                int success = CopyFileA(
                    aven_str_to_cstr(fname, &temp_arena),
                    temp_buffer,
                    false
                );
                if (success == 0) {
                    return NULL;
                }

                return LoadLibraryA(temp_buffer);
            }

            AVEN_FN void *aven_dl_sym(
                void *handle,
                AvenStr symbol,
                AvenArena temp_arena
            ) {
                AVEN_WIN32_FN(void *) GetProcAddress(
                    void *handle,
                    const char *symbol
                );

                return GetProcAddress(
                    handle,
                    aven_str_to_cstr(symbol, &temp_arena)
                );
            }

            AVEN_FN void aven_dl_close(void *handle) {
                AVEN_WIN32_FN(int) FreeLibrary(void *handle);

                int success = FreeLibrary(handle);
                assert(success != 0);
            }
        #else
            #include <dlfcn.h>

            AVEN_FN void *aven_dl_open(AvenStr fname, AvenArena temp_arena) {
                return dlopen(aven_str_to_cstr(fname, &temp_arena), RTLD_LAZY);
            }

            AVEN_FN void *aven_dl_sym(
                void *handle,
                AvenStr symbol,
                AvenArena temp_arena
            ) {
                return dlsym(handle, aven_str_to_cstr(symbol, &temp_arena));
            }

            AVEN_FN void aven_dl_close(void *handle) {
                int error = dlclose(handle);
                assert(error == 0);
            }
        #endif

    #endif
    // AVEN_IMPLEMENTATION

#endif
// AVEN_DL_H
