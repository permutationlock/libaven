clang -fsanitize=undefined -fsanitize=unreachable -fsanitize-trap -fsanitize=fuzzer \
    -O2 -g3 -I ./include/ \
    fuzz/fmt.c -o fuzz_fmt
# ./fuzz_fmt -max_len=204800 -jobs=4 -dict=fuzz/cdict.txt -only_ascii=1 fuzz_corpus
