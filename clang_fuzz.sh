clang -fsanitize=undefined -fsanitize=unreachable -fsanitize-trap -fsanitize=fuzzer \
    -O2 -g3 -I ./include/ \
    fuzz/fmt.c -o fuzz_fmt
