#ifndef AVEN_TS_H
    #define AVEN_TS_H

    #define AVEN_TS_PLAIN ""
    #define AVEN_TS_BRIGHT "1"
    #define AVEN_TS_DIM "2"
    #define AVEN_TS_ITALIC "3"
    #define AVEN_TS_UNDERLINE "4"
    #define AVEN_TS_REVERSE "7"

    #define aven_ts_no(s) "2" s

    #define AVEN_TS_FG "3"
    #define AVEN_TS_FGB "9"
    #define AVEN_TS_BG "4"
    #define AVEN_TS_BGB "10"

    #define AVEN_TS_BLACK "0"
    #define AVEN_TS_RED "1"
    #define AVEN_TS_GREEN "2"
    #define AVEN_TS_YELLOW "3"
    #define AVEN_TS_BLUE "4"
    #define AVEN_TS_MAGENTA "5"
    #define AVEN_TS_CYAN "6"
    #define AVEN_TS_WHITE "7"

    #define aven_ts_fg(c) "" AVEN_TS_FG c
    #define aven_ts_fgb(c) "" AVEN_TS_FGB c
    #define aven_ts_bg(c) "" AVEN_TS_BG c
    #define aven_ts_bgb(c) "" AVEN_TS_BGB c

    #define aven_ts_join1(s1) s1
    #define aven_ts_join2(s1, s2) "" s1 ";" s2
    #define aven_ts_join3(s1, ...) "" s1 ";" aven_ts_join2(__VA_ARGS__)
    #define aven_ts_join4(s1, ...) "" s1 ";" aven_ts_join3(__VA_ARGS__)
    #define aven_ts_join5(s1, ...) "" s1 ";" aven_ts_join4(__VA_ARGS__)
    #define aven_ts_join6(s1, ...) "" s1 ";" aven_ts_join5(__VA_ARGS__)
    #define aven_ts_join7(s1, ...) "" s1 ";" aven_ts_join6(__VA_ARGS__)
    #define aven_ts_join_dispatch(n) aven_ts_join##n
    #define aven_ts_apply(m, ...) m(__VA_ARGS__)
    #define aven_ts_cntn(_1, _2, _3, _4, _5, _6, _7, n, ...) n
    #define aven_ts_cnt(...) aven_ts_cntn(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1, 0)
    #define aven_ts_join(...) aven_ts_apply( \
            aven_ts_join_dispatch, \
            aven_ts_cnt(__VA_ARGS__) \
        )(__VA_ARGS__)
    #define aven_ts(...) "\x1b[" aven_ts_join(__VA_ARGS__) "m"
#endif
