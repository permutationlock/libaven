#ifndef AVEN_PROCESS_H
    #define AVEN_PROCESS_H

    #include "../aven.h"
    #include "arena.h"
    #include "path.h"
    #include "str.h"

    #ifdef _WIN32
        typedef void *AvenProcId;
    #else
        typedef int AvenProcId;
    #endif

    typedef enum {
        AVEN_PROC_CMD_ERROR_NONE = 0,
        AVEN_PROC_CMD_ERROR_FORK,
    } AvenProcCmdError;
    typedef Result(AvenProcId, AvenProcCmdError) AvenProcCmdResult;

    AVEN_FN AvenProcCmdResult aven_proc_cmd(AvenStrSlice cmd, AvenArena arena);

    typedef enum {
        AVEN_PROC_WAIT_ERROR_NONE = 0,
        AVEN_PROC_WAIT_ERROR_WAIT,
        AVEN_PROC_WAIT_ERROR_GETCODE,
        AVEN_PROC_WAIT_ERROR_PROCESS,
        AVEN_PROC_WAIT_ERROR_SIGNAL,
        AVEN_PROC_WAIT_ERROR_TIMEOUT,
    } AvenProcWaitError;
    typedef Result(int, AvenProcWaitError) AvenProcWaitResult;

    AVEN_FN AvenProcWaitResult aven_proc_check(AvenProcId pid);
    AVEN_FN AvenProcWaitResult aven_proc_wait(AvenProcId pid);

    typedef enum {
        AVEN_PROC_KILL_ERROR_NONE = 0,
        AVEN_PROC_KILL_ERROR_KILL,
        AVEN_PROC_KILL_ERROR_OTHER,
    } AvenProcKillError;

    AVEN_FN AvenProcKillError aven_proc_kill(AvenProcId pid);

    #ifdef AVEN_IMPLEMENTATION

        #ifndef AVEN_SUPPRESS_LOGS
            #include "io.h"
        #endif

        #ifndef _WIN32
            #ifndef _POSIX_C_SOURCE
                #error "kill requires _POSIX_C_SOURCE"
            #endif
            #include <errno.h>
            #include <signal.h>
            #include <stdlib.h>

            #if defined(__linux__) and defined(NOLIBC)
                #include <sys.h>
            #else
                #include <sys/wait.h>
            #endif
            #include <unistd.h>
        #endif

        AVEN_FN AvenProcCmdResult aven_proc_cmd(
            AvenStrSlice cmd,
            AvenArena arena
        ) {
            AvenStr cmd_str = aven_str_join(cmd, ' ', &arena);
        #ifndef AVEN_SUPPRESS_LOGS
            aven_io_printf("{}\n", aven_fmt_str(cmd_str));
        #endif
            // defined(AVEN_SUPPRESS_LOGS)
        #ifdef _WIN32
            typedef struct {
                uint32_t len;
                void *security_descriptor;
                int inherit_handle;
            } AvenWinSecurityAttr;

            typedef struct {
                uint32_t cb;
                char *reserved;
                char *desktop;
                char *title;
                uint32_t x;
                uint32_t y;
                uint32_t x_size;
                uint32_t y_size;
                uint32_t x_count_chars;
                uint32_t y_count_chars;
                uint32_t fill_attribute;
                uint32_t flags;
                uint16_t show_window;
                uint16_t breserved2;
                unsigned char *preserved2;
                void *stdinput;
                void *stdoutput;
                void *stderror;
            } AvenWinStartupInfo;

            typedef struct {
                void *process;
                void *thread;
                uint32_t process_id;
                uint32_t thread_id;
            } AvenWinProcessInfo;

            AVEN_WIN32_FN(void *) GetStdHandle(uint32_t std_handle);
            AVEN_WIN32_FN(int) CloseHandle(void *handle);
            AVEN_WIN32_FN(int) CreateProcessA(
                const char *application_name,
                char *command_line,
                AvenWinSecurityAttr *process_attr,
                AvenWinSecurityAttr *thread_attr,
                int inherit_handles,
                uint32_t creation_flags,
                void *environment,
                const char *current_directory,
                AvenWinStartupInfo *startup_info,
                AvenWinProcessInfo *process_info
            );

            AvenWinStartupInfo startup_info = {
                .cb = sizeof(AvenWinStartupInfo),
                .stdinput = GetStdHandle(((uint32_t)-10)),
                .stdoutput = GetStdHandle(((uint32_t)-11)),
                .stderror = GetStdHandle(((uint32_t)-12)),
                .flags = 0x00000100,
                /* STARTF_USESTDHANDLES */
            };
            AvenWinProcessInfo process_info = { 0 };

            int success = CreateProcessA(
                NULL,
                aven_str_to_cstr(cmd_str, &arena),
                NULL,
                NULL,
                true,
                0,
                NULL,
                NULL,
                &startup_info,
                &process_info
            );
            if (success == 0) {
                return (AvenProcCmdResult){ .error = AVEN_PROC_CMD_ERROR_FORK };
            }

            CloseHandle(process_info.thread);

            return (AvenProcCmdResult){ .payload = process_info.process };
        #else
            // !defined(_WIN32)
            AvenProcId cmd_pid = fork();
            if (cmd_pid < 0) {
                return (AvenProcCmdResult){ .error = AVEN_PROC_CMD_ERROR_FORK };
            }

            if (cmd_pid == 0) {
                char **args = aven_arena_alloc(
                    &arena,
                    cmd.len + 1,
                    16,
                    sizeof(*args)
                );

                for (size_t i = 0; i < cmd.len; i += 1) {
                    args[i] = aven_str_to_cstr(get(cmd, i), &arena);
                }
                args[cmd.len] = NULL;

        #if defined(__linux__) and defined(NOLIBC)
                char *path_cstr = getenv("PATH");
                AvenStr path = (path_cstr == NULL) ?
                    aven_str("/usr/local/bin:/bin:/usr/bin") :
                    aven_str_cstr(path_cstr);
                AvenStrSlice dirs = aven_str_split(path, ':', &arena);
                int error = 0;
                for (size_t i = 0; i < dirs.len; i += 1) {
                    AvenArena temp_arena = arena;
                    char *exe_path_cstr = aven_str_to_cstr(
                        aven_path(&temp_arena, get(dirs, i), get(cmd, 0)),
                        &temp_arena
                    );
                    error = execve(exe_path_cstr, args, NULL);
                    if (error != 0) {
                        switch (errno) {
                            case EACCES:
                            case ENOENT:
                            case ENOTDIR:
                                break;
                            default:
        #ifndef AVEN_SUPPRESS_LOGS
                                aven_io_perrf(
                                    "execve failed: {}\n",
                                    aven_fmt_str(cmd_str)
                                );
        #endif
                                // defined(AVEN_SUPPRESS_LOGS)
                                exit(errno);
                                break;
                        }
                    }
                }
                char *exe_path_cstr = aven_str_to_cstr(get(cmd, 0), &arena);
                error = execve(exe_path_cstr, args, environ);
                if (error != 0) {
                    switch (errno) {
                        case EACCES:
                        case ENOENT:
                        case ENOTDIR:
                            break;
                        default:
        #ifndef AVEN_SUPPRESS_LOGS
                            aven_io_perrf(
                                "execve failed: {}\n",
                                aven_fmt_str(cmd_str)
                            );
        #endif
                            // defined(AVEN_SUPPRESS_LOGS)
                            exit(errno);
                            break;
                    }
                }
        #else
                // !defined(__linux__) or !defined(NOLIBC)
                int error = execvp(args[0], args);
                if (error != 0) {
        #ifndef AVEN_SUPPRESS_LOGS
                    aven_io_perrf("execvp failed: {}\n", aven_fmt_str(cmd_str));
        #endif
                    // defined(AVEN_SUPPRESS_LOGS)
                    exit(errno);
                }
        #endif
                // !defined(__linux__) or !defined(NOLIBC)
            }

            return (AvenProcCmdResult){ .payload = cmd_pid };
        #endif
            // !defined(_WIN32)
        }

        static AvenProcWaitResult aven_proc_status(AvenProcId pid, bool wait) {
        #ifdef _WIN32
            AVEN_WIN32_FN(uint32_t) WaitForSingleObject(
                void *handle,
                uint32_t timeout_ms
            );
            AVEN_WIN32_FN(int) GetExitCodeProcess(
                void *handle,
                uint32_t *exit_code
            );

            uint32_t result = WaitForSingleObject(
                pid,
                wait ?
                    0xffffffff
                    /* INFINITE */
                    :
                    0x0
            );
            if (
                result == 0x00000102L
                /* TIMEOUT */
            ) {
                return (AvenProcWaitResult){
                    .error = AVEN_PROC_WAIT_ERROR_TIMEOUT,
                };
            } else if (result != 0) {
                return (AvenProcWaitResult){
                    .error = AVEN_PROC_WAIT_ERROR_WAIT,
                };
            }

            uint32_t exit_code = 0;
            int success = GetExitCodeProcess(pid, &exit_code);
            if (success == 0) {
                return (AvenProcWaitResult){
                    .error = AVEN_PROC_WAIT_ERROR_GETCODE,
                };
            }

            return (AvenProcWaitResult){ .payload = (int)exit_code };
        #else
            int exit_status = 0;
            if (!wait) {
                int wstatus = 0;
                int res_pid = waitpid(pid, &wstatus, WNOHANG);
                if (res_pid < 0) {
                    return (AvenProcWaitResult){
                        .error = AVEN_PROC_WAIT_ERROR_WAIT,
                    };
                }
                if (res_pid == 0) {
                    return (AvenProcWaitResult){
                        .error = AVEN_PROC_WAIT_ERROR_TIMEOUT,
                    };
                }
                if (!WIFEXITED(wstatus)) {
                    exit_status = WEXITSTATUS(wstatus);
                }
            }
            for (;;) {
                int wstatus = 0;
                int res_pid = waitpid(pid, &wstatus, 0);
                if (res_pid < 0) {
                    if (errno == EINTR) {
                        continue;
                    }
                    return (AvenProcWaitResult){
                        .error = AVEN_PROC_WAIT_ERROR_WAIT,
                    };
                }

                if (WIFEXITED(wstatus)) {
                    exit_status = WEXITSTATUS(wstatus);
                    break;
                }

                if (WIFSIGNALED(wstatus)) {
                    return (AvenProcWaitResult){
                        .error = AVEN_PROC_WAIT_ERROR_SIGNAL,
                    };
                }
            }

            return (AvenProcWaitResult){ .payload = exit_status };
        #endif
        }

        AVEN_FN AvenProcWaitResult aven_proc_check(AvenProcId pid) {
            return aven_proc_status(pid, false);
        }

        AVEN_FN AvenProcWaitResult aven_proc_wait(AvenProcId pid) {
            return aven_proc_status(pid, true);
        }

        AVEN_FN AvenProcKillError aven_proc_kill(AvenProcId pid) {
        #ifdef _WIN32
            AVEN_WIN32_FN(int) TerminateProcess(
                AvenProcId pid,
                unsigned int error_code
            );
            AVEN_WIN32_FN(int) CloseHandle(void *handle);

            int success = TerminateProcess(pid, 1);
            if (success == 0) {
                CloseHandle(pid);
                return AVEN_PROC_KILL_ERROR_KILL;
            }
            CloseHandle(pid);
            return 0;
        #else
            int error = kill(pid, SIGTERM);
            if (error < 0) {
                switch (errno) {
                    case EPERM:
                    case ESRCH:
                        return AVEN_PROC_KILL_ERROR_KILL;
                    default:
                        return AVEN_PROC_KILL_ERROR_OTHER;
                }
            }

            return 0;
        #endif
        }
    #endif
    // AVEN_IMPLEMENTATION

#endif
// AVEN_PROCESS_H
