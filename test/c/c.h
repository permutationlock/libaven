#ifndef AVEN_C_H
    #define AVEN_C_H

    #include "../aven.h"
    #include "arena.h"
    #include "aven/fmt.h"
    #include "io.h"
    #include "str.h"

    typedef enum {
        AVEN_C_KEYWORD_NONE = 0,
        AVEN_C_KEYWORD_0END,
        AVEN_C_KEYWORD_1END,
        AVEN_C_KEYWORD_DO,
        AVEN_C_KEYWORD_IF,
        AVEN_C_KEYWORD_OR,
        AVEN_C_KEYWORD_2END,
        AVEN_C_KEYWORD_AND,
        AVEN_C_KEYWORD_ASM1,
        AVEN_C_KEYWORD_FOR,
        AVEN_C_KEYWORD_INT,
        AVEN_C_KEYWORD_3END,
        AVEN_C_KEYWORD_AUTO,
        AVEN_C_KEYWORD_C11BOOL,
        AVEN_C_KEYWORD_CASE,
        AVEN_C_KEYWORD_CHAR,
        AVEN_C_KEYWORD_ELSE,
        AVEN_C_KEYWORD_ENUM,
        AVEN_C_KEYWORD_GOTO,
        AVEN_C_KEYWORD_LONG,
        AVEN_C_KEYWORD_VOID,
        AVEN_C_KEYWORD_4END,
        AVEN_C_KEYWORD_BOOL,
        AVEN_C_KEYWORD_BREAK,
        AVEN_C_KEYWORD_CONST,
        AVEN_C_KEYWORD_FLOAT,
        AVEN_C_KEYWORD_SHORT,
        AVEN_C_KEYWORD_UNION,
        AVEN_C_KEYWORD_WHILE,
        AVEN_C_KEYWORD_5END,
        AVEN_C_KEYWORD_DOUBLE,
        AVEN_C_KEYWORD_EXTERN,
        AVEN_C_KEYWORD_INLINE,
        AVEN_C_KEYWORD_RETURN,
        AVEN_C_KEYWORD_SIGNED,
        AVEN_C_KEYWORD_SIZEOF,
        AVEN_C_KEYWORD_STATIC,
        AVEN_C_KEYWORD_STRUCT,
        AVEN_C_KEYWORD_SWITCH,
        AVEN_C_KEYWORD_6END,
        AVEN_C_KEYWORD_ASM2,
        AVEN_C_KEYWORD_ATOMIC,
        AVEN_C_KEYWORD_DEFAULT,
        AVEN_C_KEYWORD_TYPEDEF,
        AVEN_C_KEYWORD_7END,
        AVEN_C_KEYWORD_ALIGNAS,
        AVEN_C_KEYWORD_ALIGNOF,
        AVEN_C_KEYWORD_COMPLEX,
        AVEN_C_KEYWORD_CONTINUE,
        AVEN_C_KEYWORD_REGISTER,
        AVEN_C_KEYWORD_RESTRICT,
        AVEN_C_KEYWORD_UNSIGNED,
        AVEN_C_KEYWORD_VOLATILE,
        AVEN_C_KEYWORD_8END,
        AVEN_C_KEYWORD_NORETURN,
        AVEN_C_KEYWORD_9END,
        AVEN_C_KEYWORD_IMAGINARY,
        AVEN_C_KEYWORD_10END,
        AVEN_C_KEYWORD_ATTRIBUTE1,
        AVEN_C_KEYWORD_11END,
        AVEN_C_KEYWORD_12END,
        AVEN_C_KEYWORD_ATTRIBUTE2,
        AVEN_C_KEYWORD_THREAD_LOCAL,
        AVEN_C_KEYWORD_13END,
        AVEN_C_KEYWORD_STATIC_ASSERT,
        AVEN_C_KEYWORD_14END,
    } AvenCKeyword;

    static const AvenCKeyword aven_c_keyword_indices_data[] = {
        AVEN_C_KEYWORD_1END,
        AVEN_C_KEYWORD_2END,
        AVEN_C_KEYWORD_3END,
        AVEN_C_KEYWORD_4END,
        AVEN_C_KEYWORD_5END,
        AVEN_C_KEYWORD_6END,
        AVEN_C_KEYWORD_7END,
        AVEN_C_KEYWORD_8END,
        AVEN_C_KEYWORD_9END,
        AVEN_C_KEYWORD_10END,
        AVEN_C_KEYWORD_11END,
        AVEN_C_KEYWORD_12END,
        AVEN_C_KEYWORD_13END,
        AVEN_C_KEYWORD_14END,
    };

    static const Slice(AvenCKeyword) aven_c_keyword_indices = {
        .ptr = (AvenCKeyword *)aven_c_keyword_indices_data,
        .len = countof(aven_c_keyword_indices_data),
    };

    static const AvenStr aven_c_keyword_data[] = {
        [AVEN_C_KEYWORD_DO] = aven_str_init("do"),
        [AVEN_C_KEYWORD_IF] = aven_str_init("if"),
        [AVEN_C_KEYWORD_OR] = aven_str_init("or"),
        [AVEN_C_KEYWORD_AND] = aven_str_init("and"),
        [AVEN_C_KEYWORD_ASM1] = aven_str_init("asm"),
        [AVEN_C_KEYWORD_FOR] = aven_str_init("for"),
        [AVEN_C_KEYWORD_INT] = aven_str_init("int"),
        [AVEN_C_KEYWORD_AUTO] = aven_str_init("auto"),
        [AVEN_C_KEYWORD_C11BOOL] = aven_str_init("bool"),
        [AVEN_C_KEYWORD_CASE] = aven_str_init("case"),
        [AVEN_C_KEYWORD_CHAR] = aven_str_init("char"),
        [AVEN_C_KEYWORD_ELSE] = aven_str_init("else"),
        [AVEN_C_KEYWORD_ENUM] = aven_str_init("enum"),
        [AVEN_C_KEYWORD_GOTO] = aven_str_init("goto"),
        [AVEN_C_KEYWORD_LONG] = aven_str_init("long"),
        [AVEN_C_KEYWORD_VOID] = aven_str_init("void"),
        [AVEN_C_KEYWORD_BOOL] = aven_str_init("_Bool"),
        [AVEN_C_KEYWORD_BREAK] = aven_str_init("break"),
        [AVEN_C_KEYWORD_CONST] = aven_str_init("const"),
        [AVEN_C_KEYWORD_FLOAT] = aven_str_init("float"),
        [AVEN_C_KEYWORD_SHORT] = aven_str_init("short"),
        [AVEN_C_KEYWORD_UNION] = aven_str_init("union"),
        [AVEN_C_KEYWORD_WHILE] = aven_str_init("while"),
        [AVEN_C_KEYWORD_DOUBLE] = aven_str_init("double"),
        [AVEN_C_KEYWORD_EXTERN] = aven_str_init("extern"),
        [AVEN_C_KEYWORD_INLINE] = aven_str_init("inline"),
        [AVEN_C_KEYWORD_RETURN] = aven_str_init("return"),
        [AVEN_C_KEYWORD_SIGNED] = aven_str_init("signed"),
        [AVEN_C_KEYWORD_SIZEOF] = aven_str_init("sizeof"),
        [AVEN_C_KEYWORD_STATIC] = aven_str_init("static"),
        [AVEN_C_KEYWORD_STRUCT] = aven_str_init("struct"),
        [AVEN_C_KEYWORD_SWITCH] = aven_str_init("switch"),
        [AVEN_C_KEYWORD_ASM2] = aven_str_init("__asm__"),
        [AVEN_C_KEYWORD_ATOMIC] = aven_str_init("_Atomic"),
        [AVEN_C_KEYWORD_DEFAULT] = aven_str_init("default"),
        [AVEN_C_KEYWORD_TYPEDEF] = aven_str_init("typedef"),
        [AVEN_C_KEYWORD_ALIGNOF] = aven_str_init("_Alignof"),
        [AVEN_C_KEYWORD_ALIGNAS] = aven_str_init("_Alignas"),
        [AVEN_C_KEYWORD_COMPLEX] = aven_str_init("_Complex"),
        [AVEN_C_KEYWORD_CONTINUE] = aven_str_init("continue"),
        [AVEN_C_KEYWORD_REGISTER] = aven_str_init("register"),
        [AVEN_C_KEYWORD_RESTRICT] = aven_str_init("restrict"),
        [AVEN_C_KEYWORD_UNSIGNED] = aven_str_init("unsigned"),
        [AVEN_C_KEYWORD_VOLATILE] = aven_str_init("volatile"),
        [AVEN_C_KEYWORD_NORETURN] = aven_str_init("_Noreturn"),
        [AVEN_C_KEYWORD_IMAGINARY] = aven_str_init("_Imaginary"),
        [AVEN_C_KEYWORD_ATTRIBUTE1] = aven_str_init("__attribute"),
        [AVEN_C_KEYWORD_ATTRIBUTE2] = aven_str_init("__attribute__"),
        [AVEN_C_KEYWORD_THREAD_LOCAL] = aven_str_init("_Thread_local"),
        [AVEN_C_KEYWORD_STATIC_ASSERT] = aven_str_init("_Static_assert"),
    };

    static const AvenStrSlice aven_c_keywords = {
        .ptr = (AvenStr *)aven_c_keyword_data,
        .len = countof(aven_c_keyword_data),
    };

    static inline AvenCKeyword aven_c_keyword(AvenStr str) {
        if (str.len < 2 or str.len > 14) {
            return AVEN_C_KEYWORD_NONE;
        }
        AvenCKeyword start = get(aven_c_keyword_indices, str.len - 2);
        AvenCKeyword end = get(aven_c_keyword_indices, str.len - 1);
        for (AvenCKeyword kw = start + 1; kw != end; kw += 1) {
            AvenStr kw_str = get(aven_c_keywords, kw);
            bool equal = true;
            for (size_t i = 0; i < str.len; i += 1) {
                if (get(str, i) != get(kw_str, i)) {
                    equal = false;
                    break;
                }
            }
            if (equal) {
                return kw;
            }
        }
        return AVEN_C_KEYWORD_NONE;
    }

    typedef enum {
        AVEN_C_PNC_SQBL,
        AVEN_C_PNC_SQBR,
        AVEN_C_PNC_PARL,
        AVEN_C_PNC_PARR,
        AVEN_C_PNC_CRBL,
        AVEN_C_PNC_CRBR,
        AVEN_C_PNC_DOT,
        AVEN_C_PNC_ARW,
        AVEN_C_PNC_PLS2,
        AVEN_C_PNC_MIN2,
        AVEN_C_PNC_AMP,
        AVEN_C_PNC_STR,
        AVEN_C_PNC_PLS,
        AVEN_C_PNC_MIN,
        AVEN_C_PNC_TLD,
        AVEN_C_PNC_EXC,
        AVEN_C_PNC_FSL,
        AVEN_C_PNC_PCT,
        AVEN_C_PNC_LT,
        AVEN_C_PNC_GT,
        AVEN_C_PNC_LT2,
        AVEN_C_PNC_GT2,
        AVEN_C_PNC_LTEQ,
        AVEN_C_PNC_GTEQ,
        AVEN_C_PNC_EQ2,
        AVEN_C_PNC_EXCEQ,
        AVEN_C_PNC_CAR,
        AVEN_C_PNC_BAR,
        AVEN_C_PNC_AMP2,
        AVEN_C_PNC_BAR2,
        AVEN_C_PNC_QST,
        AVEN_C_PNC_COL,
        AVEN_C_PNC_SCOL,
        AVEN_C_PNC_DOT3,
        AVEN_C_PNC_EQ,
        AVEN_C_PNC_STREQ,
        AVEN_C_PNC_FSLEQ,
        AVEN_C_PNC_PCTEQ,
        AVEN_C_PNC_PLSEQ,
        AVEN_C_PNC_MINEQ,
        AVEN_C_PNC_LT2EQ,
        AVEN_C_PNC_GT2EQ,
        AVEN_C_PNC_AMPEQ,
        AVEN_C_PNC_CAREQ,
        AVEN_C_PNC_BAREQ,
        AVEN_C_PNC_COM,
        AVEN_C_PNC_HSH,
        AVEN_C_PNC_HSH2,
        AVEN_C_PNC_LTCOL,
        AVEN_C_PNC_COLGT,
        AVEN_C_PNC_LTPCT,
        AVEN_C_PNC_PCTGT,
        AVEN_C_PNC_PCTCOL,
    } AvenCPnc;

    static const AvenStr aven_c_punctuator_data[] = {
        [AVEN_C_PNC_SQBL] = aven_str_init("["),
        [AVEN_C_PNC_SQBR] = aven_str_init("]"),
        [AVEN_C_PNC_PARL] = aven_str_init("("),
        [AVEN_C_PNC_PARR] = aven_str_init(")"),
        [AVEN_C_PNC_CRBL] = aven_str_init("{"),
        [AVEN_C_PNC_CRBR] = aven_str_init("}"),
        [AVEN_C_PNC_DOT] = aven_str_init("."),
        [AVEN_C_PNC_ARW] = aven_str_init("->"),
        [AVEN_C_PNC_PLS2] = aven_str_init("++"),
        [AVEN_C_PNC_MIN2] = aven_str_init("--"),
        [AVEN_C_PNC_AMP] = aven_str_init("&"),
        [AVEN_C_PNC_STR] = aven_str_init("*"),
        [AVEN_C_PNC_PLS] = aven_str_init("+"),
        [AVEN_C_PNC_MIN] = aven_str_init("-"),
        [AVEN_C_PNC_TLD] = aven_str_init("~"),
        [AVEN_C_PNC_EXC] = aven_str_init("!"),
        [AVEN_C_PNC_FSL] = aven_str_init("/"),
        [AVEN_C_PNC_PCT] = aven_str_init("%"),
        [AVEN_C_PNC_LT] = aven_str_init("<"),
        [AVEN_C_PNC_GT] = aven_str_init(">"),
        [AVEN_C_PNC_LT2] = aven_str_init("<<"),
        [AVEN_C_PNC_GT2] = aven_str_init(">>"),
        [AVEN_C_PNC_LTEQ] = aven_str_init("<="),
        [AVEN_C_PNC_GTEQ] = aven_str_init(">="),
        [AVEN_C_PNC_EQ2] = aven_str_init("=="),
        [AVEN_C_PNC_EXCEQ] = aven_str_init("!="),
        [AVEN_C_PNC_CAR] = aven_str_init("^"),
        [AVEN_C_PNC_BAR] = aven_str_init("|"),
        [AVEN_C_PNC_AMP2] = aven_str_init("&&"),
        [AVEN_C_PNC_BAR2] = aven_str_init("||"),
        [AVEN_C_PNC_QST] = aven_str_init("?"),
        [AVEN_C_PNC_COL] = aven_str_init(":"),
        [AVEN_C_PNC_SCOL] = aven_str_init(";"),
        [AVEN_C_PNC_DOT3] = aven_str_init("..."),
        [AVEN_C_PNC_EQ] = aven_str_init("="),
        [AVEN_C_PNC_STREQ] = aven_str_init("*="),
        [AVEN_C_PNC_FSLEQ] = aven_str_init("/="),
        [AVEN_C_PNC_PCTEQ] = aven_str_init("%="),
        [AVEN_C_PNC_PLSEQ] = aven_str_init("+="),
        [AVEN_C_PNC_MINEQ] = aven_str_init("-="),
        [AVEN_C_PNC_LT2EQ] = aven_str_init("<<="),
        [AVEN_C_PNC_GT2EQ] = aven_str_init(">>="),
        [AVEN_C_PNC_AMPEQ] = aven_str_init("&="),
        [AVEN_C_PNC_CAREQ] = aven_str_init("^="),
        [AVEN_C_PNC_BAREQ] = aven_str_init("|="),
        [AVEN_C_PNC_COM] = aven_str_init(","),
        [AVEN_C_PNC_HSH] = aven_str_init("#"),
        [AVEN_C_PNC_HSH2] = aven_str_init("##"),
        [AVEN_C_PNC_LTCOL] = aven_str_init("<:"),
        [AVEN_C_PNC_COLGT] = aven_str_init(":>"),
        [AVEN_C_PNC_LTPCT] = aven_str_init("<%"),
        [AVEN_C_PNC_PCTGT] = aven_str_init("%>"),
        [AVEN_C_PNC_PCTCOL] = aven_str_init("%:"),
    };

    static const AvenStrSlice aven_c_punctuators = {
        .ptr = (AvenStr *)aven_c_punctuator_data,
        .len = countof(aven_c_punctuator_data),
    };

    typedef enum {
        AVEN_C_TOKEN_TYPE_NONE = 0,
        AVEN_C_TOKEN_TYPE_ID,
        AVEN_C_TOKEN_TYPE_KEY,
        AVEN_C_TOKEN_TYPE_NUM,
        AVEN_C_TOKEN_TYPE_CHR,
        AVEN_C_TOKEN_TYPE_STR,
        AVEN_C_TOKEN_TYPE_PNC,
        AVEN_C_TOKEN_TYPE_PPD,
        AVEN_C_TOKEN_TYPE_CMT,
        AVEN_C_TOKEN_TYPE_INV,
    } AvenCTokenType;

    static const AvenStr aven_c_token_type_str_data[] = {
        [AVEN_C_TOKEN_TYPE_NONE] = aven_str_init("none"),
        [AVEN_C_TOKEN_TYPE_ID] = aven_str_init("identifier"),
        [AVEN_C_TOKEN_TYPE_KEY] = aven_str_init("keyword"),
        [AVEN_C_TOKEN_TYPE_NUM] = aven_str_init("pp-number"),
        [AVEN_C_TOKEN_TYPE_CHR] = aven_str_init("character-constant"),
        [AVEN_C_TOKEN_TYPE_STR] = aven_str_init("string-literal"),
        [AVEN_C_TOKEN_TYPE_PNC] = aven_str_init("punctuator"),
        [AVEN_C_TOKEN_TYPE_PPD] = aven_str_init("preprocessor-directive"),
        [AVEN_C_TOKEN_TYPE_CMT] = aven_str_init("comment"),
        [AVEN_C_TOKEN_TYPE_INV] = aven_str_init("invalid"),
    };

    static const AvenStrSlice aven_c_token_type_strs = {
        .ptr = (AvenStr *)aven_c_token_type_str_data,
        .len = countof(aven_c_token_type_str_data),
    };

    static inline AvenStr aven_c_token_type_str(AvenCTokenType token_type) {
        return get(aven_c_token_type_strs, (size_t)token_type);
    }

    typedef struct {
        AvenCTokenType type;
        uint32_t index;
        uint32_t end;
        uint32_t trailing_lines;
    } AvenCToken;
    typedef Slice(AvenCToken) AvenCTokenSlice;
    typedef List(AvenCToken) AvenCTokenList;

    typedef struct {
        AvenStr bytes;
        AvenCTokenSlice tokens;
    } AvenCTokenSet;

    static inline AvenStr aven_c_token_str(AvenCTokenSet tset, uint32_t index) {
        AvenCToken token = get(tset.tokens, index);
        if (token.type == AVEN_C_TOKEN_TYPE_NONE) {
            return (AvenStr){ 0 };
        } else if (token.type == AVEN_C_TOKEN_TYPE_KEY) {
            return get(aven_c_keywords, token.end);
        } else if (token.type == AVEN_C_TOKEN_TYPE_PNC) {
            return get(aven_c_punctuators, token.end);
        } else if (token.type == AVEN_C_TOKEN_TYPE_PPD) {
            // Grab entire text chunk for all tokens within directive
            AvenCToken t1 = get(tset.tokens, token.index);
            uint32_t last_index = token.index;
            AvenCToken tn = t1;
            while (tn.type != AVEN_C_TOKEN_TYPE_NONE) {
                last_index += 1;
                tn = get(tset.tokens, last_index);
            }
            return aven_str_range(tset.bytes, t1.index, tn.index);
        }
        return aven_str_range(tset.bytes, token.index, token.end);
    }

    typedef struct {
        uint32_t line;
        uint32_t col;
    } AvenCTokenLoc;

    static inline AvenCTokenLoc aven_c_token_loc(
        AvenCTokenSet tset,
        uint32_t index
    ) {
        AvenCToken token = get(tset.tokens, index);
        uint32_t src_index = (token.type == AVEN_C_TOKEN_TYPE_PPD) ?
            get(tset.tokens, token.index).index :
            token.index;
        uint32_t line = 1;
        uint32_t col = 1;
        for (uint32_t i = 0; i < src_index; i += 1) {
            if (get(tset.bytes, i) == '\n') {
                line += 1;
                col = 1;
            } else {
                col += 1;
            }
        }
        return (AvenCTokenLoc){ .line = line, .col = col };
    }

    typedef enum {
        AVEN_C_LEX_STATE_NONE = 0,
        AVEN_C_LEX_STATE_DONE,
        AVEN_C_LEX_STATE_INV,
        AVEN_C_LEX_STATE_ID,
        AVEN_C_LEX_STATE_NUM,
        AVEN_C_LEX_STATE_NUM_EXP,
        AVEN_C_LEX_STATE_STR,
        AVEN_C_LEX_STATE_CHAR,
        AVEN_C_LEX_STATE_PPD_NAME,
        AVEN_C_LEX_STATE_PPD_BODY,
        AVEN_C_LEX_STATE_PPD_BODY_ESC,
        AVEN_C_LEX_STATE_PPD_BODY_ESC_CR,
        AVEN_C_LEX_STATE_PPD_BODY_FSLASH,
        AVEN_C_LEX_STATE_PPD_STR,
        AVEN_C_LEX_STATE_PPD_STR_ESC,
        AVEN_C_LEX_STATE_PPD_CHAR,
        AVEN_C_LEX_STATE_PPD_CHAR_ESC,
        AVEN_C_LEX_STATE_COMMENT,
        AVEN_C_LEX_STATE_COMMENT_ESC,
        AVEN_C_LEX_STATE_COMMENT_CR,
        AVEN_C_LEX_STATE_MLCOMMENT,
        AVEN_C_LEX_STATE_MLCOMMENT_CR,
        AVEN_C_LEX_STATE_MLCOMMENT_SKIP,
        AVEN_C_LEX_STATE_MLCOMMENT_STAR,
        AVEN_C_LEX_STATE_STR_ESC,
        AVEN_C_LEX_STATE_CHAR_ESC,
        AVEN_C_LEX_STATE_HASH,
        AVEN_C_LEX_STATE_DOT,
        AVEN_C_LEX_STATE_DOT_DOT,
        AVEN_C_LEX_STATE_MINUS,
        AVEN_C_LEX_STATE_PLUS,
        AVEN_C_LEX_STATE_AMP,
        AVEN_C_LEX_STATE_STAR,
        AVEN_C_LEX_STATE_EXCL,
        AVEN_C_LEX_STATE_FSLASH,
        AVEN_C_LEX_STATE_BSLASH,
        AVEN_C_LEX_STATE_BSLASH_CR,
        AVEN_C_LEX_STATE_PERCT,
        AVEN_C_LEX_STATE_LT,
        AVEN_C_LEX_STATE_LTLT,
        AVEN_C_LEX_STATE_GT,
        AVEN_C_LEX_STATE_GTGT,
        AVEN_C_LEX_STATE_EQ,
        AVEN_C_LEX_STATE_XOR,
        AVEN_C_LEX_STATE_OR,
        AVEN_C_LEX_STATE_COLON,
    } AvenCLexState;

    typedef struct {
        AvenStr bytes;
        AvenCTokenList tokens;
        AvenCLexState state;
        uint32_t token_start;
        uint32_t index;
        uint32_t count;
        bool ppd;
        char next;
    } AvenCLexCtx;

    static inline bool aven_c_lex_step(AvenCLexCtx *ctx) {
        char c = get(ctx->bytes, ctx->index);
        switch (ctx->state) {
            case AVEN_C_LEX_STATE_NONE: {
                ctx->token_start = ctx->index;
                switch (c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_NUM;
                        break;
                    }
                    case '_':
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'E':
                    case 'F':
                    case 'G':
                    case 'H':
                    case 'I':
                    case 'J':
                    case 'K':
                    case 'L':
                    case 'M':
                    case 'N':
                    case 'O':
                    case 'P':
                    case 'Q':
                    case 'R':
                    case 'S':
                    case 'T':
                    case 'U':
                    case 'V':
                    case 'W':
                    case 'X':
                    case 'Y':
                    case 'Z':
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'e':
                    case 'f':
                    case 'g':
                    case 'h':
                    case 'i':
                    case 'j':
                    case 'k':
                    case 'l':
                    case 'm':
                    case 'n':
                    case 'o':
                    case 'p':
                    case 'q':
                    case 'r':
                    case 's':
                    case 't':
                    case 'u':
                    case 'v':
                    case 'w':
                    case 'x':
                    case 'y':
                    case 'z': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_ID;
                        break;
                    }
                    case '\"': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_STR;
                        break;
                    }
                    case '\'': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_CHAR;
                        break;
                    }
                    case '#': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_HASH;
                        break;
                    }
                    case '.': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_DOT;
                        break;
                    }
                    case '-': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_MINUS;
                        break;
                    }
                    case '+': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_PLUS;
                        break;
                    }
                    case '&': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_AMP;
                        break;
                    }
                    case '*': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_STAR;
                        break;
                    }
                    case '!': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_EXCL;
                        break;
                    }
                    case '/': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_FSLASH;
                        break;
                    }
                    case '\\': {
                        ctx->state = AVEN_C_LEX_STATE_BSLASH;
                        break;
                    }
                    case '%': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_PERCT;
                        break;
                    }
                    case '<': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_LT;
                        break;
                    }
                    case '>': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_GT;
                        break;
                    }
                    case '=': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_EQ;
                        break;
                    }
                    case '^': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_XOR;
                        break;
                    }
                    case '|': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_OR;
                        break;
                    }
                    case ':': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_COLON;
                        break;
                    }
                    case ';': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_SCOL,
                        };
                        break;
                    }
                    case '(': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_PARL,
                        };
                        break;
                    }
                    case ')': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_PARR,
                        };
                        break;
                    }
                    case '[': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_SQBL,
                        };
                        break;
                    }
                    case ']': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_SQBR,
                        };
                        break;
                    }
                    case '{': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_CRBL,
                        };
                        break;
                    }
                    case '}': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_CRBR,
                        };
                        break;
                    }
                    case ',': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_COM,
                        };
                        break;
                    }
                    case '~': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_TLD,
                        };
                        break;
                    }
                    case '\?': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_QST,
                        };
                        break;
                    }
                    case ' ':
                    case '\t':
                    case '\r': {
                        ctx->index += 1;
                        break;
                    }
                    case '\n': {
                        if (ctx->ppd) {
                            ctx->state = AVEN_C_LEX_STATE_DONE;
                        } else {
                            ctx->index += 1;
                            list_back(ctx->tokens).trailing_lines += 1;
                        }
                        break;
                    }
                    case 0: {
                        ctx->state = AVEN_C_LEX_STATE_DONE;
                        break;
                    }
                    default: {
                        ctx->state = AVEN_C_LEX_STATE_INV;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_INV: {
                list_push(ctx->tokens) = (AvenCToken){
                    .type = AVEN_C_TOKEN_TYPE_INV,
                    .index = ctx->token_start,
                    .end = ctx->index,
                };
                ctx->state = AVEN_C_LEX_STATE_DONE;
                ctx->token_start = ctx->index;
                break;
            }
            case AVEN_C_LEX_STATE_DONE: {
                break;
            }
            case AVEN_C_LEX_STATE_ID: {
                switch (c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                    case '_':
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'E':
                    case 'F':
                    case 'G':
                    case 'H':
                    case 'I':
                    case 'J':
                    case 'K':
                    case 'L':
                    case 'M':
                    case 'N':
                    case 'O':
                    case 'P':
                    case 'Q':
                    case 'R':
                    case 'S':
                    case 'T':
                    case 'U':
                    case 'V':
                    case 'W':
                    case 'X':
                    case 'Y':
                    case 'Z':
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'e':
                    case 'f':
                    case 'g':
                    case 'h':
                    case 'i':
                    case 'j':
                    case 'k':
                    case 'l':
                    case 'm':
                    case 'n':
                    case 'o':
                    case 'p':
                    case 'q':
                    case 'r':
                    case 's':
                    case 't':
                    case 'u':
                    case 'v':
                    case 'w':
                    case 'x':
                    case 'y':
                    case 'z': {
                        ctx->index += 1;
                        break;
                    }
                    default: {
                        AvenStr name = aven_str_range(
                            ctx->bytes,
                            ctx->token_start,
                            ctx->index
                        );
                        AvenCKeyword keyword = aven_c_keyword(name);
                        if (keyword == AVEN_C_KEYWORD_NONE) {
                            list_push(ctx->tokens) = (AvenCToken){
                                .index = ctx->token_start,
                                .end = ctx->index,
                                .type = AVEN_C_TOKEN_TYPE_ID,
                            };
                        } else {
                            list_push(ctx->tokens) = (AvenCToken){
                                .index = ctx->token_start,
                                .end = keyword,
                                .type = AVEN_C_TOKEN_TYPE_KEY,
                            };
                        }
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_NUM: {
                switch (c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                    case '_':
                    case '.':
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'F':
                    case 'G':
                    case 'H':
                    case 'I':
                    case 'J':
                    case 'K':
                    case 'L':
                    case 'M':
                    case 'N':
                    case 'O':
                    case 'Q':
                    case 'R':
                    case 'S':
                    case 'T':
                    case 'U':
                    case 'V':
                    case 'W':
                    case 'X':
                    case 'Y':
                    case 'Z':
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'f':
                    case 'g':
                    case 'h':
                    case 'i':
                    case 'j':
                    case 'k':
                    case 'l':
                    case 'm':
                    case 'n':
                    case 'o':
                    case 'q':
                    case 'r':
                    case 's':
                    case 't':
                    case 'u':
                    case 'v':
                    case 'w':
                    case 'x':
                    case 'y':
                    case 'z': {
                        ctx->index += 1;
                        break;
                    }
                    case 'E':
                    case 'e':
                    case 'P':
                    case 'p': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_NUM_EXP;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index,
                            .type = AVEN_C_TOKEN_TYPE_NUM,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_NUM_EXP: {
                switch (c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                    case '_':
                    case '.':
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'F':
                    case 'G':
                    case 'H':
                    case 'I':
                    case 'J':
                    case 'K':
                    case 'L':
                    case 'M':
                    case 'N':
                    case 'O':
                    case 'Q':
                    case 'R':
                    case 'S':
                    case 'T':
                    case 'U':
                    case 'V':
                    case 'W':
                    case 'X':
                    case 'Y':
                    case 'Z':
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'f':
                    case 'g':
                    case 'h':
                    case 'i':
                    case 'j':
                    case 'k':
                    case 'l':
                    case 'm':
                    case 'n':
                    case 'o':
                    case 'q':
                    case 'r':
                    case 's':
                    case 't':
                    case 'u':
                    case 'v':
                    case 'w':
                    case 'x':
                    case 'y':
                    case 'z': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_NUM;
                        break;
                    }
                    case 'E':
                    case 'e':
                    case 'P':
                    case 'p': {
                        ctx->index += 1;
                        break;
                    }
                    case '+':
                    case '-': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_NUM;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index,
                            .type = AVEN_C_TOKEN_TYPE_NUM,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_STR: {
                switch (c) {
                    case '\\': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_STR_ESC;
                        break;
                    }
                    case '\"': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index,
                            .type = AVEN_C_TOKEN_TYPE_STR,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    case 0:
                    case '\n': {
                        ctx->state = AVEN_C_LEX_STATE_INV;
                        break;
                    }
                    default: {
                        ctx->index += 1;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_CHAR: {
                switch (c) {
                    case '\\': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_CHAR_ESC;
                        break;
                    }
                    case '\'': {
                        ctx->index += 1;
                        uint32_t len = ctx->token_start - ctx->index;
                        if (len <= 1) {
                            ctx->state = AVEN_C_LEX_STATE_INV;
                            break;
                        }
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index,
                            .type = AVEN_C_TOKEN_TYPE_CHR,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    case 0:
                    case '\n': {
                        ctx->state = AVEN_C_LEX_STATE_INV;
                        break;
                    }
                    default: {
                        ctx->index += 1;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_PPD_NAME: {
                switch (c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                    case '_':
                    case 'A':
                    case 'B':
                    case 'C':
                    case 'D':
                    case 'E':
                    case 'F':
                    case 'G':
                    case 'H':
                    case 'I':
                    case 'J':
                    case 'K':
                    case 'L':
                    case 'M':
                    case 'N':
                    case 'O':
                    case 'P':
                    case 'Q':
                    case 'R':
                    case 'S':
                    case 'T':
                    case 'U':
                    case 'V':
                    case 'W':
                    case 'X':
                    case 'Y':
                    case 'Z':
                    case 'a':
                    case 'b':
                    case 'c':
                    case 'd':
                    case 'e':
                    case 'f':
                    case 'g':
                    case 'h':
                    case 'i':
                    case 'j':
                    case 'k':
                    case 'l':
                    case 'm':
                    case 'n':
                    case 'o':
                    case 'p':
                    case 'q':
                    case 'r':
                    case 's':
                    case 't':
                    case 'u':
                    case 'v':
                    case 'w':
                    case 'x':
                    case 'y':
                    case 'z': {
                        ctx->index += 1;
                        break;
                    }
                    default: {
                        AvenStr str = aven_str_range(
                            ctx->bytes,
                            ctx->token_start + 1,
                            ctx->index
                        );
                        if (str.len == 0) {
                            list_push(ctx->tokens) = (AvenCToken){
                                .index = ctx->token_start,
                                .end = AVEN_C_PNC_HSH,
                                .type = AVEN_C_TOKEN_TYPE_PNC,
                            };
                            ctx->state = AVEN_C_LEX_STATE_NONE;
                        } else if (
                            aven_str_equals(str, aven_str("pragma")) or
                            aven_str_equals(str, aven_str("error")) or
                            aven_str_equals(str, aven_str("warning"))
                        ) {
                            ctx->state = AVEN_C_LEX_STATE_COMMENT;
                        } else {
                            ctx->state = AVEN_C_LEX_STATE_PPD_BODY;
                        }
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_PPD_BODY: {
                switch (c) {
                    case '\\': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_PPD_BODY_ESC;
                        break;
                    }
                    case '/': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_PPD_BODY_FSLASH;
                        break;
                    }
                    case '\"': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_PPD_STR;
                        break;
                    }
                    case '\'': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_PPD_CHAR;
                        break;
                    }
                    case '\n': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index,
                            .type = AVEN_C_TOKEN_TYPE_PPD,
                            .trailing_lines = 1,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    case 0: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index + 1,
                            .type = AVEN_C_TOKEN_TYPE_PPD,
                        };
                        ctx->state = AVEN_C_LEX_STATE_DONE;
                        break;
                    }
                    default: {
                        ctx->index += 1;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_PPD_BODY_ESC: {
                switch (c) {
                    case '\\': {
                        ctx->index += 1;
                        break;
                    }
                    case '\r': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_PPD_BODY_ESC_CR;
                        break;
                    }
                    case 0: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index + 1,
                            .type = AVEN_C_TOKEN_TYPE_PPD,
                        };
                        ctx->state = AVEN_C_LEX_STATE_DONE;
                        break;
                    }
                    default: {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_PPD_BODY;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_PPD_BODY_ESC_CR: {
                switch (c) {
                    case '\n': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_PPD_BODY;
                        break;
                    }
                    default: {
                        ctx->state = AVEN_C_LEX_STATE_PPD_BODY;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_PPD_STR: {
                switch (c) {
                    case '\\': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_PPD_STR_ESC;
                        break;
                    }
                    case '\"': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_PPD_BODY;
                        break;
                    }
                    case 0:
                    case '\n': {
                        ctx->state = AVEN_C_LEX_STATE_INV;
                        break;
                    }
                    default: {
                        ctx->index += 1;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_PPD_STR_ESC: {
                switch (c) {
                    case 'a':
                    case 'b':
                    case 'f':
                    case 'n':
                    case 'r':
                    case 't':
                    case 'v':
                    case '\\':
                    case '\?':
                    case '\"':
                    case '\'': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_PPD_STR;
                        break;
                    }
                    default: {
                        ctx->state = AVEN_C_LEX_STATE_PPD_STR;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_PPD_CHAR: {
                switch (c) {
                    case '\\': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_PPD_CHAR_ESC;
                        break;
                    }
                    case '\'': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_PPD_BODY;
                        break;
                    }
                    case 0:
                    case '\n': {
                        ctx->state = AVEN_C_LEX_STATE_INV;
                        break;
                    }
                    default: {
                        ctx->index += 1;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_PPD_CHAR_ESC: {
                switch (c) {
                    case 'a':
                    case 'b':
                    case 'f':
                    case 'n':
                    case 'r':
                    case 't':
                    case 'v':
                    case '\\':
                    case '\?':
                    case '\"':
                    case '\'': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_PPD_CHAR;
                        break;
                    }
                    default: {
                        ctx->state = AVEN_C_LEX_STATE_PPD_CHAR;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_PPD_BODY_FSLASH: {
                switch (c) {
                    case '\\': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_PPD_BODY_ESC;
                        break;
                    }
                    case '/': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index,
                            .type = AVEN_C_TOKEN_TYPE_PPD,
                        };
                        ctx->token_start = ctx->index - 2;
                        ctx->state = AVEN_C_LEX_STATE_COMMENT;
                        break;
                    }
                    case '*': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index,
                            .type = AVEN_C_TOKEN_TYPE_PPD,
                        };
                        ctx->token_start = ctx->index - 2;
                        ctx->state = AVEN_C_LEX_STATE_MLCOMMENT;
                        break;
                    }
                    case 0: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index + 1,
                            .type = AVEN_C_TOKEN_TYPE_PPD,
                        };
                        ctx->state = AVEN_C_LEX_STATE_DONE;
                        break;
                    }
                    default: {
                        ctx->state = AVEN_C_LEX_STATE_PPD_BODY;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_BSLASH: {
                if (!ctx->ppd) {
                    ctx->state = AVEN_C_LEX_STATE_INV;
                    break;
                }
                switch (c) {
                    case '\\': {
                        ctx->index += 1;
                        break;
                    }
                    case '\r': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_BSLASH_CR;
                        break;
                    }
                    case '\n': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        list_back(ctx->tokens).trailing_lines += 1;
                        break;
                    }
                    default: {
                        ctx->state = AVEN_C_LEX_STATE_INV;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_BSLASH_CR: {
                switch (c) {
                    case '\n': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        list_back(ctx->tokens).trailing_lines += 1;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index - 1,
                            .type = AVEN_C_TOKEN_TYPE_CMT,
                        };
                        ctx->token_start = ctx->index - 1;
                        ctx->state = AVEN_C_LEX_STATE_INV;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_COMMENT: {
                switch (c) {
                    case '\\': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_COMMENT_ESC;
                        break;
                    }
                    case '\r': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_COMMENT_CR;
                        break;
                    }
                    case 0:
                    case '\n': {
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index,
                            .type = AVEN_C_TOKEN_TYPE_CMT,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        ctx->index += 1;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_COMMENT_CR: {
                switch (c) {
                    case '\n': {
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index - 1,
                            .type = AVEN_C_TOKEN_TYPE_CMT,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index - 1,
                            .type = AVEN_C_TOKEN_TYPE_CMT,
                        };
                        ctx->token_start = ctx->index - 1;
                        ctx->state = AVEN_C_LEX_STATE_INV;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_COMMENT_ESC: {
                switch (c) {
                    case '\\': {
                        ctx->index += 1;
                        break;
                    }
                    case 0:
                    case '\r':
                    case '\n': {
                        ctx->state = AVEN_C_LEX_STATE_INV;
                        break;
                    }
                    default: {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_COMMENT;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_MLCOMMENT: {
                switch (c) {
                    case '*': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_MLCOMMENT_STAR;
                        break;
                    }
                    case '\r': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_MLCOMMENT_CR;
                        break;
                    }
                    case '\n': {
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index,
                            .type = AVEN_C_TOKEN_TYPE_CMT,
                        };
                        ctx->state = AVEN_C_LEX_STATE_MLCOMMENT_SKIP;
                        break;
                    }
                    case 0: {
                        ctx->state = AVEN_C_LEX_STATE_DONE;
                        break;
                    }
                    default: {
                        ctx->index += 1;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_MLCOMMENT_CR: {
                switch (c) {
                    case '\n': {
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index - 1,
                            .type = AVEN_C_TOKEN_TYPE_CMT,
                        };
                        ctx->state = AVEN_C_LEX_STATE_MLCOMMENT_SKIP;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index - 1,
                            .type = AVEN_C_TOKEN_TYPE_CMT,
                        };
                        ctx->token_start = ctx->index - 1;
                        ctx->state = AVEN_C_LEX_STATE_INV;
                        break;
                    }
                }
                break;
            }

            case AVEN_C_LEX_STATE_MLCOMMENT_SKIP: {
                ctx->token_start = ctx->index;
                switch (c) {
                    case '\n': {
                        list_back(ctx->tokens).trailing_lines += 1;
                        ctx->index += 1;
                        ctx->token_start = ctx->index;
                        break;
                    }
                    case ' ':
                    case '\t':
                    case '\r': {
                        ctx->index += 1;
                        ctx->token_start = ctx->index;
                        break;
                    }
                    case 0: {
                        ctx->state = AVEN_C_LEX_STATE_DONE;
                        break;
                    }
                    default: {
                        ctx->state = AVEN_C_LEX_STATE_MLCOMMENT;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_MLCOMMENT_STAR: {
                switch (c) {
                    case '/': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .index = ctx->token_start,
                            .end = ctx->index,
                            .type = AVEN_C_TOKEN_TYPE_CMT,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    case '*': {
                        ctx->index += 1;
                        break;
                    }
                    case 0: {
                        ctx->state = AVEN_C_LEX_STATE_DONE;
                        break;
                    }
                    default: {
                        ctx->state = AVEN_C_LEX_STATE_MLCOMMENT;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_STR_ESC: {
                switch (c) {
                    case 'a':
                    case 'b':
                    case 'f':
                    case 'n':
                    case 'r':
                    case 't':
                    case 'v':
                    case '\\':
                    case '\?':
                    case '\"':
                    case '\'': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_STR;
                        break;
                    }
                    default: {
                        ctx->state = AVEN_C_LEX_STATE_STR;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_CHAR_ESC: {
                switch (c) {
                    case 'a':
                    case 'b':
                    case 'f':
                    case 'n':
                    case 'r':
                    case 't':
                    case 'v':
                    case '\\':
                    case '\?':
                    case '\"':
                    case '\'': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_CHAR;
                        break;
                    }
                    default: {
                        ctx->state = AVEN_C_LEX_STATE_CHAR;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_DOT: {
                switch (c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_NUM;
                        break;
                    }
                    case '.': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_DOT_DOT;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_DOT,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_DOT_DOT: {
                switch (c) {
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9': {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_DOT,
                        };
                        ctx->token_start = ctx->index - 1;
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_NUM;
                        break;
                    }
                    case '.': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_DOT3,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_DOT,
                        };
                        ctx->token_start = ctx->index - 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_DOT,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_HASH: {
                switch (c) {
                    case '#': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_HSH2,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        if (ctx->ppd) {
                            list_push(ctx->tokens) = (AvenCToken){
                                .type = AVEN_C_TOKEN_TYPE_PNC,
                                .index = ctx->token_start,
                                .end = AVEN_C_PNC_HSH,
                            };
                            ctx->state = AVEN_C_LEX_STATE_NONE;
                        } else {
                            ctx->state = AVEN_C_LEX_STATE_PPD_NAME;
                        }
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_MINUS: {
                switch (c) {
                    case '=': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_MINEQ,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    case '>': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_ARW,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    case '-': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_MIN2,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_MIN,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_PLUS: {
                switch (c) {
                    case '=': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_PLSEQ,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    case '+': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_PLS2,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_PLS,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_AMP: {
                switch (c) {
                    case '=': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_AMPEQ,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    case '&': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_AMP2,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_AMP,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_STAR: {
                switch (c) {
                    case '=': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_STREQ,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_STR,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_EXCL: {
                switch (c) {
                    case '=': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_EXCEQ,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_EXC,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_FSLASH: {
                switch (c) {
                    case '/': {
                        ctx->index += 1;
                        if (ctx->ppd) {
                            ctx->state = AVEN_C_LEX_STATE_DONE;
                        } else {
                            ctx->state = AVEN_C_LEX_STATE_COMMENT;
                        }
                        break;
                    }
                    case '*': {
                        ctx->index += 1;
                        if (ctx->ppd) {
                            ctx->state = AVEN_C_LEX_STATE_DONE;
                        } else {
                            ctx->state = AVEN_C_LEX_STATE_MLCOMMENT;
                        }
                        break;
                    }
                    case '=': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_FSLEQ,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_FSL,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_PERCT: {
                switch (c) {
                    case ':': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_PCTCOL,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    case '>': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_PCTGT,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    case '=': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_PCTEQ,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_PCT,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_LT: {
                switch (c) {
                    case '%': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_LTPCT,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    case ':': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_LTCOL,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    case '=': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_LTEQ,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    case '<': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_LTLT;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_LT,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_LTLT: {
                switch (c) {
                    case '=': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_LT2EQ,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_LT2,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_GT: {
                switch (c) {
                    case '=': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_GTEQ,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    case '>': {
                        ctx->index += 1;
                        ctx->state = AVEN_C_LEX_STATE_GTGT;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_GT,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_GTGT: {
                switch (c) {
                    case '=': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_GT2EQ,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_GT2,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_EQ: {
                switch (c) {
                    case '=': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_EQ2,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_EQ,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_XOR: {
                switch (c) {
                    case '=': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_CAREQ,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_CAR,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_OR: {
                switch (c) {
                    case '|': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_BAR2,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    case '=': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_BAREQ,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_BAR,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
            case AVEN_C_LEX_STATE_COLON: {
                switch (c) {
                    case '>': {
                        ctx->index += 1;
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_COLGT,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                    default: {
                        list_push(ctx->tokens) = (AvenCToken){
                            .type = AVEN_C_TOKEN_TYPE_PNC,
                            .index = ctx->token_start,
                            .end = AVEN_C_PNC_COL,
                        };
                        ctx->state = AVEN_C_LEX_STATE_NONE;
                        break;
                    }
                }
                break;
            }
        }
        return ctx->state == AVEN_C_LEX_STATE_DONE;
    }

    static inline AvenCTokenSet aven_c_lex(AvenStr bytes, AvenArena *arena) {
        assert(get(bytes, bytes.len - 1) == 0);
        AvenCLexCtx ctx = {
            .bytes = bytes,
            .tokens = aven_arena_create_list(
                AvenCToken,
                arena,
                2 * bytes.len + 2
            ),
        };
        list_push(ctx.tokens) = (AvenCToken){ .type = AVEN_C_TOKEN_TYPE_NONE };
        while (!aven_c_lex_step(&ctx)) {}
        list_push(ctx.tokens) = (AvenCToken){
            .type = AVEN_C_TOKEN_TYPE_NONE,
            .index = ctx.token_start,
            .end = ctx.token_start,
        };
        size_t len = ctx.tokens.len;
        for (size_t i = 0; i < len; i += 1) {
            AvenCToken *token = &get(ctx.tokens, i);
            if (token->type == AVEN_C_TOKEN_TYPE_PPD) {
                AvenStr str = aven_str_head(bytes, token->end);
                uint32_t start = (uint32_t)ctx.tokens.len;
                AvenCLexCtx ppd_ctx = {
                    .bytes = str,
                    .tokens = ctx.tokens,
                    .ppd = true,
                    .index = token->index,
                };
                while (!aven_c_lex_step(&ppd_ctx)) {}
                list_push(ppd_ctx.tokens) = (AvenCToken){
                    .type = AVEN_C_TOKEN_TYPE_NONE,
                    .index = ppd_ctx.token_start,
                    .end = ppd_ctx.token_start,
                };
                ctx.tokens = ppd_ctx.tokens;
                token->index = start;
                token->end = (uint32_t)ctx.tokens.len;
            }
        }
        return (AvenCTokenSet){
            .bytes = bytes,
            .tokens = aven_arena_commit_list_to_slice(
                AvenCTokenSlice,
                arena,
                ctx.tokens
            ),
        };
    }

    typedef enum {
        AVEN_C_AST_NODE_TYPE_NONE = 0,
        AVEN_C_AST_NODE_TYPE_ANYTOKEN,
        AVEN_C_AST_NODE_TYPE_STRING_LITERAL,
        AVEN_C_AST_NODE_TYPE_CONSTANT,
        AVEN_C_AST_NODE_TYPE_IDENTIFIER,
        AVEN_C_AST_NODE_TYPE_KEYWORD,
        AVEN_C_AST_NODE_TYPE_PUNCTUATOR,
        AVEN_C_AST_NODE_TYPE_STRING_CONSTANT,
        AVEN_C_AST_NODE_TYPE_INITIALIZER_LIST,
        AVEN_C_AST_NODE_TYPE_ARRAY_DESIGNATOR,
        AVEN_C_AST_NODE_TYPE_DOT_DESIGNATOR,
        AVEN_C_AST_NODE_TYPE_DESIGNATION,
        AVEN_C_AST_NODE_TYPE_EXPR,
        AVEN_C_AST_NODE_TYPE_PRIMARY_EXPR,
        AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_BRAC,
        AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_PAREN,
        AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_UOP,
        AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_BOP,
        AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_INITIALIZER,
        AVEN_C_AST_NODE_TYPE_UNARY_EXPR,
        AVEN_C_AST_NODE_TYPE_UNARY_EXPR_FN,
        AVEN_C_AST_NODE_TYPE_UNARY_EXPR_SIZEOF,
        AVEN_C_AST_NODE_TYPE_CAST_EXPR,
        AVEN_C_AST_NODE_TYPE_MULTIPLY_EXPR,
        AVEN_C_AST_NODE_TYPE_ADD_EXPR,
        AVEN_C_AST_NODE_TYPE_SHIFT_EXPR,
        AVEN_C_AST_NODE_TYPE_RELATE_EXPR,
        AVEN_C_AST_NODE_TYPE_EQUAL_EXPR,
        AVEN_C_AST_NODE_TYPE_AND_EXPR,
        AVEN_C_AST_NODE_TYPE_XOR_EXPR,
        AVEN_C_AST_NODE_TYPE_OR_EXPR,
        AVEN_C_AST_NODE_TYPE_LOGICAL_AND_EXPR,
        AVEN_C_AST_NODE_TYPE_LOGICAL_OR_EXPR,
        AVEN_C_AST_NODE_TYPE_CONDITIONAL_EXPR,
        AVEN_C_AST_NODE_TYPE_ASSIGN_EXPR,
        AVEN_C_AST_NODE_TYPE_ATOMIC_SPECIFIER,
        AVEN_C_AST_NODE_TYPE_TYPE_NAME,
        AVEN_C_AST_NODE_TYPE_TYPE_SPECIFIER,
        AVEN_C_AST_NODE_TYPE_TYPE_QUALIFIER,
        AVEN_C_AST_NODE_TYPE_STRUCT_SPECIFIER,
        AVEN_C_AST_NODE_TYPE_ENUM_SPECIFIER,
        AVEN_C_AST_NODE_TYPE_ENUMERATOR,
        AVEN_C_AST_NODE_TYPE_DECLARATION,
        AVEN_C_AST_NODE_TYPE_ALIGNMENT_SPECIFIER,
        AVEN_C_AST_NODE_TYPE_INIT_DECLARATOR,
        AVEN_C_AST_NODE_TYPE_ATTRIBUTE,
        AVEN_C_AST_NODE_TYPE_DECLARATOR,
        AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR,
        AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR_PARAM_TYPE_LIST,
        AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR_ATTRIBUTE,
        AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR_ID_LIST,
        AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR_BRAC,
        AVEN_C_AST_NODE_TYPE_ABS_DECLARATOR,
        AVEN_C_AST_NODE_TYPE_DIR_ABS_DECLARATOR,
        AVEN_C_AST_NODE_TYPE_DIR_ABS_DECLARATOR_PARAM_TYPE_LIST,
        AVEN_C_AST_NODE_TYPE_DIR_ABS_DECLARATOR_BRAC,
        AVEN_C_AST_NODE_TYPE_STATIC_ASSERT_DECLARATION,
        AVEN_C_AST_NODE_TYPE_STRUCT_DECLARATION,
        AVEN_C_AST_NODE_TYPE_STRUCT_DECLARATOR,
        AVEN_C_AST_NODE_TYPE_POINTER,
        AVEN_C_AST_NODE_TYPE_PARAMETER_DECLARATION,
        AVEN_C_AST_NODE_TYPE_LABELED_STATEMENT,
        AVEN_C_AST_NODE_TYPE_COMPOUND_STATEMENT,
        AVEN_C_AST_NODE_TYPE_IF_STATEMENT,
        AVEN_C_AST_NODE_TYPE_IF_ELSE_STATEMENT,
        AVEN_C_AST_NODE_TYPE_SWITCH_STATEMENT,
        AVEN_C_AST_NODE_TYPE_SWITCH_CASE,
        AVEN_C_AST_NODE_TYPE_WHILE_STATEMENT,
        AVEN_C_AST_NODE_TYPE_FOR_STATEMENT,
        AVEN_C_AST_NODE_TYPE_DO_STATEMENT,
        AVEN_C_AST_NODE_TYPE_GOTO_STATEMENT,
        AVEN_C_AST_NODE_TYPE_RETURN_STATEMENT,
        AVEN_C_AST_NODE_TYPE_ASM_STATEMENT,
        AVEN_C_AST_NODE_TYPE_FUNCTION_DEFINITION,
        AVEN_C_AST_NODE_TYPE_TRANSLATION_UNIT,
        AVEN_C_AST_NODE_TYPE_TERMINATED_LINE,
        AVEN_C_AST_NODE_TYPE_MACRO_INVOCATION,
        AVEN_C_AST_NODE_TYPE_PREPROCESSOR_PASTE,
        AVEN_C_AST_NODE_TYPE_PREPROCESSOR_MERGE,
        AVEN_C_AST_NODE_TYPE_PREPROCESSOR_HEADER,
        AVEN_C_AST_NODE_TYPE_PREPROCESSOR_SPEC_LIST,
        AVEN_C_AST_NODE_TYPE_PREPROCESSOR_DEFINE,
        AVEN_C_AST_NODE_TYPE_PREPROCESSOR_DEFINE_FN,
        AVEN_C_AST_NODE_TYPE_PREPROCESSOR_DIRECTIVE,
        AVEN_C_AST_NODE_TYPE_COMMENT,
    } AvenCAstNodeType;

    typedef struct {
        AvenCAstNodeType type;
        uint32_t token;
        uint32_t lhs;
        uint32_t rhs;
    } AvenCAstNode;
    typedef Optional(AvenCAstNode) AvenCAstNodeOpt;
    typedef Slice(AvenCAstNode) AvenCAstNodeSlice;

    typedef Slice(uint32_t) AvenCAstDataSlice;

    typedef struct {
        AvenCTokenSet tset;
        AvenCAstNodeSlice nodes;
        AvenCAstDataSlice data;
        AvenCAstDataSlice pp_nodes;
        uint32_t root;
    } AvenCAst;
    typedef enum {
        AVEN_C_AST_RESULT_TYPE_AST = 0,
        AVEN_C_AST_RESULT_TYPE_ERROR = 1,
    } AvenCAstResultType;
    typedef union {
        AvenStr error;
        AvenCAst ast;
    } AvenCAstResultData;
    typedef struct {
        AvenCAstResultType type;
        AvenCAstResultData data;
    } AvenCAstResult;

    static inline AvenCAstNode aven_c_ast_node(AvenCAst *ast, uint32_t index) {
        return get(ast->nodes, index - 1);
    }

    static inline uint32_t aven_c_ast_data(AvenCAst *ast, uint32_t index) {
        return get(ast->data, index - 1);
    }

    typedef struct {
        AvenCTokenType type;
        uint32_t token;
        uint32_t pp_token;
        AvenStr exp;
    } AvenCAstError;

    #ifndef AVEN_C_MAX_PARSE_DEPTH
        #define AVEN_C_MAX_PARSE_DEPTH 128
    #endif
    #ifndef AVEN_C_MIN_PARES_DEPTH
        #define AVEN_C_MIN_PARSE_DEPTH 8
    #endif

    typedef struct {
        AvenCTokenSet tset;
        List(AvenCAstNode) nodes;
        List(uint32_t) data;
        List(uint32_t) scratch;
        AvenCAstDataSlice pp_nodes;
        bool ppd;
        bool depth_exceeded;
        uint32_t max_depth;
        uint32_t depth;
        uint32_t spec_depth;
        uint32_t unary_expr_depth;
        uint32_t token_index;
        AvenCAstError error;
        Optional(AvenCAstError) ppd_error;
    } AvenCAstCtx;

    typedef Slice(uint32_t) AvenCIndexSlice;

    typedef struct {
        uint32_t nodes_len;
        uint32_t data_len;
        uint32_t scratch_len;
        uint32_t token_index;
        uint32_t depth;
    } AvenCAstCtxState;

    static inline AvenCAstCtx aven_c_ast_init(
        AvenCTokenSet tset,
        uint32_t max_depth,
        AvenArena *arena
    ) {
        size_t max_tokens = 1 + 3 * tset.tokens.len;
        AvenCAstCtx ctx = {
            .tset = tset,
            .nodes = { .cap = max_tokens },
            .pp_nodes = { .len = tset.tokens.len },
            .data = { .cap = max_tokens },
            .scratch = { .cap = max_tokens },
            .max_depth = max_depth,
        };
        ctx.nodes.ptr = aven_arena_create_array(
            AvenCAstNode,
            arena,
            ctx.nodes.cap
        );
        ctx.data.ptr = aven_arena_create_array(uint32_t, arena, ctx.data.cap);
        ctx.scratch.ptr = aven_arena_create_array(
            uint32_t,
            arena,
            ctx.scratch.cap
        );
        ctx.pp_nodes.ptr = aven_arena_create_array(
            uint32_t,
            arena,
            ctx.pp_nodes.len
        );
        for (uint32_t i = 0; i < ctx.pp_nodes.len; i += 1) {
            get(ctx.pp_nodes, i) = 0;
        }
        return ctx;
    }

    static inline AvenCAstCtx aven_c_ast_init_ppd(
        AvenCAstCtx *ctx,
        uint32_t token
    ) {
        AvenCToken ppd_token = get(ctx->tset.tokens, token);
        assert(ppd_token.type == AVEN_C_TOKEN_TYPE_PPD);
        return (AvenCAstCtx){
            .tset = ctx->tset,
            .nodes = ctx->nodes,
            .data = ctx->data,
            .scratch = ctx->scratch,
            .token_index = ppd_token.index,
            .max_depth = ctx->max_depth,
            .ppd = true,
        };
    }

    static inline void aven_c_ast_commit_ppd(
        AvenCAstCtx *ctx,
        AvenCAstCtx *ppd_ctx
    ) {
        assert(ctx->scratch.len == ppd_ctx->scratch.len);
        ctx->nodes.len = ppd_ctx->nodes.len;
        ctx->data.len = ppd_ctx->data.len;
    }

    static inline uint32_t aven_c_ast_push(
        AvenCAstCtx *ctx,
        AvenCAstNodeType type,
        uint32_t token,
        uint32_t lhs,
        uint32_t rhs
    ) {
        list_push(ctx->nodes) = (AvenCAstNode){
            .type = type,
            .token = token,
            .lhs = lhs,
            .rhs = rhs,
        };
        return (uint32_t)(ctx->nodes.len);
    }

    static inline uint32_t aven_c_ast_push_leaf(
        AvenCAstCtx *ctx,
        AvenCAstNodeType type,
        uint32_t token
    ) {
        return aven_c_ast_push(ctx, type, token, 0, 0);
    }

    static inline uint32_t aven_c_ast_parse_preprocessor_directive(
        AvenCAstCtx *ctx
    );

    static inline uint32_t aven_c_ast_next_index(AvenCAstCtx *ctx) {
        return ctx->token_index;
    }

    static inline AvenCToken aven_c_ast_next(AvenCAstCtx *ctx) {
        return get(ctx->tset.tokens, aven_c_ast_next_index(ctx));
    }

    static inline uint32_t aven_c_ast_inc_index(AvenCAstCtx *ctx) {
        uint32_t og_index = ctx->token_index;
        ctx->token_index += 1;
        AvenCToken token = get(ctx->tset.tokens, ctx->token_index);
        while (
            token.type == AVEN_C_TOKEN_TYPE_CMT or
            token.type == AVEN_C_TOKEN_TYPE_PPD
        ) {
            ctx->token_index += 1;
            token = get(ctx->tset.tokens, ctx->token_index);
        }
        return og_index;
    }

    static inline void aven_c_ast_trap(AvenCAstCtx *ctx) {
        get(ctx->tset.tokens, aven_c_ast_next_index(ctx)).type =
            AVEN_C_TOKEN_TYPE_NONE;
    }

    static inline void aven_c_ast_descend(AvenCAstCtx *ctx) {
        if (ctx->depth >= ctx->max_depth or ctx->depth_exceeded) {
            ctx->depth_exceeded = true;
            aven_c_ast_trap(ctx);
        }
        ctx->depth += 1;
    }

    static inline void aven_c_ast_ascend(AvenCAstCtx *ctx) {
        assert(ctx->depth > 0);
        ctx->depth -= 1;
    }

    static inline AvenCAstCtxState aven_c_ast_save(AvenCAstCtx *ctx) {
        return (AvenCAstCtxState){
            .nodes_len = (uint32_t)ctx->nodes.len,
            .data_len = (uint32_t)ctx->data.len,
            .scratch_len = (uint32_t)ctx->scratch.len,
            .token_index = ctx->token_index,
            .depth = ctx->depth,
        };
    }

    static inline void aven_c_ast_restore(
        AvenCAstCtx *ctx,
        AvenCAstCtxState state
    ) {
        ctx->nodes.len = state.nodes_len;
        ctx->data.len = state.data_len;
        ctx->scratch.len = state.scratch_len;
        ctx->token_index = state.token_index;
        ctx->depth = state.depth;
    }

    static inline void aven_c_ast_restore_trap(
        AvenCAstCtx *ctx,
        AvenCAstCtxState state
    ) {
        bool trap = aven_c_ast_next(ctx).type == AVEN_C_TOKEN_TYPE_NONE;
        aven_c_ast_restore(ctx, state);
        if (trap) {
            aven_c_ast_trap(ctx);
        }
    }

    static inline void aven_c_ast_error(
        AvenCAstCtx *ctx,
        AvenCAstCtxState state
    ) {
        aven_c_ast_restore(ctx, state);
        aven_c_ast_trap(ctx);
    }

    static inline uint32_t aven_c_ast_parse_ppd_tokens(AvenCAstCtx *ctx) {
        uint32_t i = 1;
        for (; i < ctx->tset.tokens.len; i += 1) {
            AvenCToken token = get(ctx->tset.tokens, i);
            if (token.type == AVEN_C_TOKEN_TYPE_NONE) {
                break;
            }
            if (token.type == AVEN_C_TOKEN_TYPE_PPD) {
                AvenCAstCtx ppd_ctx = aven_c_ast_init_ppd(ctx, i);
                uint32_t node = aven_c_ast_parse_preprocessor_directive(
                    &ppd_ctx
                );
                if (
                    node == 0 or
                    get(ppd_ctx.tset.tokens, aven_c_ast_next_index(&ppd_ctx))
                        .type != AVEN_C_TOKEN_TYPE_NONE
                ) {
                    ctx->ppd_error.valid = true;
                    ctx->ppd_error.value = (AvenCAstError){
                        .type = ppd_ctx.error.type,
                        .token = i,
                        .pp_token = ppd_ctx.error.token + 1,
                        .exp = ppd_ctx.error.exp,
                    };
                    if (ppd_ctx.depth_exceeded) {
                        ctx->depth_exceeded = true;
                    }
                    break;
                } else {
                    get(ctx->pp_nodes, i) = node;
                    aven_c_ast_commit_ppd(ctx, &ppd_ctx);
                }
            } else if (token.type == AVEN_C_TOKEN_TYPE_CMT) {
                get(ctx->pp_nodes, i) = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_COMMENT,
                    i
                );
            }
        }
        return i;
    }

    static inline uint32_t aven_c_ast_scratch_init(AvenCAstCtx *ctx) {
        uint32_t top = (uint32_t)ctx->scratch.len;
        list_push(ctx->scratch) = 0;
        return top;
    }

    static inline uint32_t aven_c_ast_scratch_commit(
        AvenCAstCtx *ctx,
        uint32_t scratch_top
    ) {
        Slice(uint32_t) scratch_slice = slice_tail(ctx->scratch, scratch_top);
        if (scratch_slice.len <= 1) {
            ctx->scratch.len = scratch_top;
            return 0;
        }

        get(scratch_slice, 0) = (uint32_t)(scratch_slice.len - 1);

        uint32_t data_top = (uint32_t)ctx->data.len;
        for (uint32_t i = 0; i < scratch_slice.len; i += 1) {
            list_push(ctx->data) = get(scratch_slice, i);
        }
        ctx->scratch.len = scratch_top;
        return data_top + 1;
    }

    static inline AvenCAstDataSlice aven_c_ast_data_get(
        AvenCAst *ast,
        uint32_t index
    ) {
        if (index == 0) {
            return (AvenCAstDataSlice){ 0 };
        }
        uint32_t len = get(ast->data, index - 1);
        return (AvenCAstDataSlice){
            .len = len,
            .ptr = len > 0 ? &get(ast->data, index) : NULL,
        };
    }

    static inline uint32_t aven_c_ast_parse_string_literal(AvenCAstCtx *ctx) {
        if (aven_c_ast_next(ctx).type == AVEN_C_TOKEN_TYPE_STR) {
            return aven_c_ast_push_leaf(
                ctx,
                AVEN_C_AST_NODE_TYPE_STRING_LITERAL,
                aven_c_ast_inc_index(ctx)
            );
        }
        if (aven_c_ast_next_index(ctx) >= ctx->error.token) {
            ctx->error = (AvenCAstError){
                .type = AVEN_C_TOKEN_TYPE_STR,
                .token = aven_c_ast_next_index(ctx),
            };
        }
        return 0;
    }

    static inline uint32_t aven_c_ast_parse_constant(AvenCAstCtx *ctx) {
        switch (aven_c_ast_next(ctx).type) {
            case AVEN_C_TOKEN_TYPE_NUM:
            case AVEN_C_TOKEN_TYPE_CHR: {
                return aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_CONSTANT,
                    aven_c_ast_inc_index(ctx)
                );
            }
            default: {
                break;
            }
        }
        if (aven_c_ast_next_index(ctx) >= ctx->error.token) {
            ctx->error = (AvenCAstError){
                .token = aven_c_ast_next_index(ctx),
                .type = AVEN_C_TOKEN_TYPE_NUM,
            };
        }
        return 0;
    }

    static inline bool aven_c_ast_match_punctuator(
        AvenCAstCtx *ctx,
        AvenCPnc punctuator
    ) {
        AvenCToken next = aven_c_ast_next(ctx);
        if (
            next.type == AVEN_C_TOKEN_TYPE_PNC and
            (AvenCPnc)next.end == punctuator
        ) {
            aven_c_ast_inc_index(ctx);
            return true;
        }
        if (aven_c_ast_next_index(ctx) >= ctx->error.token) {
            ctx->error = (AvenCAstError){
                .token = aven_c_ast_next_index(ctx),
                .type = AVEN_C_TOKEN_TYPE_PNC,
                .exp = get(aven_c_punctuators, punctuator),
            };
        }
        return false;
    }

    static inline uint32_t aven_c_ast_parse_identifier_token(AvenCAstCtx *ctx) {
        if (aven_c_ast_next(ctx).type == AVEN_C_TOKEN_TYPE_ID) {
            return aven_c_ast_push_leaf(
                ctx,
                AVEN_C_AST_NODE_TYPE_IDENTIFIER,
                aven_c_ast_inc_index(ctx)
            );
        }
        if (aven_c_ast_next_index(ctx) >= ctx->error.token) {
            ctx->error = (AvenCAstError){
                .type = AVEN_C_TOKEN_TYPE_ID,
                .token = aven_c_ast_next_index(ctx),
            };
        }
        return 0;
    }

    static inline uint32_t aven_c_ast_parse_identifier(AvenCAstCtx *ctx) {
        uint32_t id_node = aven_c_ast_parse_identifier_token(ctx);
        if (id_node == 0) {
            return 0;
        }
        if (ctx->ppd) {
            AvenCAstCtxState state = aven_c_ast_save(ctx);
            uint32_t main_token = aven_c_ast_next_index(ctx);
            if (aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_HSH2)) {
                uint32_t rhs = aven_c_ast_parse_identifier(ctx);
                if (rhs == 0) {
                    aven_c_ast_error(ctx, state);
                }
                return aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_PREPROCESSOR_MERGE,
                    main_token,
                    id_node,
                    rhs
                );
            }
        }
        return id_node;
    }

    static inline bool aven_c_ast_match_keyword(
        AvenCAstCtx *ctx,
        AvenCKeyword keyword
    ) {
        AvenCToken next = aven_c_ast_next(ctx);
        if (next.type == AVEN_C_TOKEN_TYPE_KEY and next.end == keyword) {
            aven_c_ast_inc_index(ctx);
            return true;
        }
        if (aven_c_ast_next_index(ctx) >= ctx->error.token) {
            ctx->error = (AvenCAstError){
                .token = aven_c_ast_next_index(ctx),
                .type = AVEN_C_TOKEN_TYPE_KEY,
                .exp = get(aven_c_keywords, keyword),
            };
        }
        return false;
    }

    static inline uint32_t aven_c_ast_parse_keyword(
        AvenCAstCtx *ctx,
        AvenCKeyword keyword
    ) {
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (aven_c_ast_match_keyword(ctx, keyword)) {
            return aven_c_ast_push_leaf(
                ctx,
                AVEN_C_AST_NODE_TYPE_KEYWORD,
                main_token
            );
        }
        return 0;
    }

    static inline uint32_t aven_c_ast_parse_punctuator(
        AvenCAstCtx *ctx,
        AvenCPnc punctuator
    ) {
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (aven_c_ast_match_punctuator(ctx, punctuator)) {
            return aven_c_ast_push_leaf(
                ctx,
                AVEN_C_AST_NODE_TYPE_PUNCTUATOR,
                main_token
            );
        }
        return 0;
    }

    static inline uint32_t aven_c_ast_parse_terminated_line(
        AvenCAstCtx *ctx,
        uint32_t lhs
    ) {
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_SCOL)) {
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_TERMINATED_LINE,
            main_token,
            lhs,
            0
        );
    }

    typedef struct {
        uint32_t decl_spec_list;
        bool abstract;
    } AvenCAstDsl;

    static inline uint32_t aven_c_ast_parse_type_name(AvenCAstCtx *ctx);
    static inline uint32_t aven_c_ast_parse_type_specifier(AvenCAstCtx *ctx);
    static inline uint32_t aven_c_ast_parse_type_qualifier(AvenCAstCtx *ctx);
    static inline uint32_t aven_c_ast_parse_primary_expr(AvenCAstCtx *ctx);
    static inline uint32_t aven_c_ast_parse_expr(AvenCAstCtx *ctx);
    static inline uint32_t aven_c_ast_parse_cast_expr(AvenCAstCtx *ctx);
    static inline uint32_t aven_c_ast_parse_assign_expr(AvenCAstCtx *ctx);
    static inline uint32_t aven_c_ast_parse_const_expr(AvenCAstCtx *ctx);
    static inline uint32_t aven_c_ast_parse_initializer_list(AvenCAstCtx *ctx);
    static inline uint32_t aven_c_ast_parse_declaration_specifier(
        AvenCAstCtx *ctx
    );
    static inline AvenCAstDsl aven_c_ast_parse_declaration_specifier_list(
        AvenCAstCtx *ctx,
        bool abstract,
        bool decl_opt
    );
    static inline uint32_t aven_c_ast_parse_declaration(AvenCAstCtx *ctx);
    static inline uint32_t aven_c_ast_parse_declarator(AvenCAstCtx *ctx);
    static inline uint32_t aven_c_ast_parse_parameter_type_list(
        AvenCAstCtx *ctx
    );
    static inline uint32_t aven_c_ast_parse_abstract_declarator(
        AvenCAstCtx *ctx
    );

    static inline uint32_t aven_c_ast_parse_macro_argument(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t expr = aven_c_ast_parse_assign_expr(ctx);
        if (expr != 0) {
            AvenCAstCtxState last = aven_c_ast_save(ctx);
            if (
                !aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_COM) and
                !aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)
            ) {
                expr = 0;
                aven_c_ast_restore_trap(ctx, state);
            } else {
                aven_c_ast_restore(ctx, last);
            }
        }
        if (expr == 0) {
            expr = aven_c_ast_parse_type_name(ctx);
        }
        return expr;
    }

    static inline uint32_t aven_c_ast_parse_macro_invocation(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t node = aven_c_ast_parse_identifier(ctx);
        if (node == 0) {
            return 0;
        }
        uint32_t open_token = aven_c_ast_next_index(ctx);
        if (aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
            aven_c_ast_descend(ctx);
            uint32_t arg_list = 0;
            {
                uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
                for (;;) {
                    uint32_t arg = aven_c_ast_parse_macro_argument(ctx);
                    if (arg == 0) {
                        break;
                    }
                    list_push(ctx->scratch) = arg;
                    if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_COM)) {
                        break;
                    }
                }
                arg_list = aven_c_ast_scratch_commit(ctx, scratch_top);
            }
            uint32_t close_token = aven_c_ast_next_index(ctx);
            if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
                aven_c_ast_restore_trap(ctx, state);
                return 0;
            }
            aven_c_ast_ascend(ctx);
            uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
            list_push(ctx->scratch) = open_token;
            list_push(ctx->scratch) = close_token;
            node = aven_c_ast_push(
                ctx,
                AVEN_C_AST_NODE_TYPE_MACRO_INVOCATION,
                aven_c_ast_scratch_commit(ctx, scratch_top),
                node,
                arg_list
            );
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_preprocessor_paste(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_HSH)) {
            return 0;
        }
        uint32_t lhs = aven_c_ast_parse_identifier(ctx);
        if (lhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_PREPROCESSOR_PASTE,
            main_token,
            lhs,
            0
        );
    }

    static inline uint32_t aven_c_ast_parse_string_constant(AvenCAstCtx *ctx) {
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t count = 0;
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        for (;;) {
            uint32_t part = aven_c_ast_parse_string_literal(ctx);
            if (count > 0 and part == 0) {
                part = aven_c_ast_parse_macro_invocation(ctx);
            }
            if (ctx->ppd and part == 0) {
                part = aven_c_ast_parse_preprocessor_paste(ctx);
            }
            if (part == 0) {
                break;
            }
            list_push(ctx->scratch) = part;
            count += 1;
        }
        if (count == 0) {
            uint32_t list = aven_c_ast_scratch_commit(ctx, scratch_top);
            assert(list == 0);
            return 0;
        }
        if (count == 1) {
            uint32_t node = list_pop(ctx->scratch);
            uint32_t list = aven_c_ast_scratch_commit(ctx, scratch_top);
            assert(list == 0);
            return node;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_STRING_CONSTANT,
            main_token,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            0
        );
    }

    static inline uint32_t aven_c_ast_parse_atomic_specifier(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_ATOMIC)) {
            return 0;
        }
        uint32_t start_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
            aven_c_ast_restore(ctx, state);
            return 0;
        }
        aven_c_ast_descend(ctx);
        uint32_t tname = aven_c_ast_parse_type_name(ctx);
        if (tname == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        uint32_t end_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_ascend(ctx);
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = main_token;
        list_push(ctx->scratch) = start_token;
        list_push(ctx->scratch) = end_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_ATOMIC_SPECIFIER,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            tname,
            0
        );
    }

    static inline uint32_t aven_c_ast_parse_enumerator(AvenCAstCtx *ctx) {
        uint32_t id_node = aven_c_ast_parse_identifier(ctx);
        if (id_node == 0) {
            return 0;
        }
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_EQ)) {
            aven_c_ast_restore_trap(ctx, state);
            return id_node;
        }
        uint32_t cexpr_node = aven_c_ast_parse_const_expr(ctx);
        if (cexpr_node == 0) {
            aven_c_ast_error(ctx, state);
            return id_node;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_ENUMERATOR,
            main_token,
            id_node,
            cexpr_node
        );
    }

    static inline uint32_t aven_c_ast_parse_enum_specifier(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_ENUM)) {
            return 0;
        }
        uint32_t id_node = aven_c_ast_parse_identifier(ctx);
        uint32_t enum_list = 0;
        uint32_t close_token;
        uint32_t open_token = aven_c_ast_next_index(ctx);
        if (aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_CRBL)) {
            aven_c_ast_descend(ctx);
            uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
            for (;;) {
                uint32_t enum_node = aven_c_ast_parse_enumerator(ctx);
                if (enum_node == 0) {
                    break;
                }
                list_push(ctx->scratch) = enum_node;
                if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_COM)) {
                    break;
                }
            }
            close_token = aven_c_ast_next_index(ctx);
            if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_CRBR)) {
                aven_c_ast_error(ctx, state);
                return 0;
            }
            aven_c_ast_ascend(ctx);
            enum_list = aven_c_ast_scratch_commit(ctx, scratch_top);
        }
        if (id_node == 0 and enum_list == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        uint32_t token = main_token;
        if (enum_list != 0) {
            uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
            list_push(ctx->scratch) = main_token;
            list_push(ctx->scratch) = open_token;
            list_push(ctx->scratch) = close_token;
            token = aven_c_ast_scratch_commit(ctx, scratch_top);
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_ENUM_SPECIFIER,
            token,
            id_node,
            enum_list
        );
    }

    static inline uint32_t aven_c_ast_parse_static_assert_declaration(
        AvenCAstCtx *ctx
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t lhs = aven_c_ast_parse_keyword(
            ctx,
            AVEN_C_KEYWORD_STATIC_ASSERT
        );
        if (lhs == 0) {
            return 0;
        }
        uint32_t open_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_descend(ctx);
        uint32_t cexpr = aven_c_ast_parse_const_expr(ctx);
        if (cexpr == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_COM)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        uint32_t str = aven_c_ast_parse_string_constant(ctx);
        if (str == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        uint32_t close_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_ascend(ctx);
        uint32_t rhs = 0;
        {
            uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
            list_push(ctx->scratch) = cexpr;
            list_push(ctx->scratch) = str;
            rhs = aven_c_ast_scratch_commit(ctx, scratch_top);
        }
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = open_token;
        list_push(ctx->scratch) = close_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_STATIC_ASSERT_DECLARATION,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_struct_declarator(AvenCAstCtx *ctx) {
        uint32_t decl_node = aven_c_ast_parse_declarator(ctx);
        if (decl_node == 0) {
            return 0;
        }
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_COL)) {
            return decl_node;
        }
        uint32_t cexpr_node = aven_c_ast_parse_const_expr(ctx);
        if (cexpr_node == 0) {
            aven_c_ast_error(ctx, state);
            return decl_node;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_STRUCT_DECLARATOR,
            main_token,
            decl_node,
            cexpr_node
        );
    }

    static inline uint32_t aven_c_ast_parse_struct_declarator_list(
        AvenCAstCtx *ctx
    ) {
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        for (;;) {
            uint32_t struct_decl = aven_c_ast_parse_struct_declarator(ctx);
            if (struct_decl == 0) {
                break;
            }
            list_push(ctx->scratch) = struct_decl;
            if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_COM)) {
                break;
            }
        }
        return aven_c_ast_scratch_commit(ctx, scratch_top);
    }

    static inline uint32_t aven_c_ast_parse_struct_declaration_unterminated(
        AvenCAstCtx *ctx
    ) {
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t static_ass_node = aven_c_ast_parse_static_assert_declaration(
            ctx
        );
        if (static_ass_node != 0) {
            return static_ass_node;
        }
        AvenCAstDsl dsl = aven_c_ast_parse_declaration_specifier_list(
            ctx,
            false,
            true
        );
        if (dsl.decl_spec_list == 0) {
            return aven_c_ast_parse_declaration_specifier(ctx);
        }
        uint32_t struct_decl_list = aven_c_ast_parse_struct_declarator_list(ctx);
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_STRUCT_DECLARATION,
            main_token,
            dsl.decl_spec_list,
            struct_decl_list
        );
    }

    static inline uint32_t aven_c_ast_parse_struct_declaration(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t unterm = aven_c_ast_parse_struct_declaration_unterminated(ctx);
        if (unterm == 0) {
            return 0;
        }
        uint32_t term = aven_c_ast_parse_terminated_line(ctx, unterm);
        if (term == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        return term;
    }

    static inline uint32_t aven_c_ast_parse_struct_specifier(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (
            !(
                aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_STRUCT) or
                aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_UNION)
            )
        ) {
            return 0;
        }
        uint32_t id_node = aven_c_ast_parse_identifier(ctx);
        uint32_t decl_list = 0;
        uint32_t close_token;
        uint32_t open_token = aven_c_ast_next_index(ctx);
        if (aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_CRBL)) {
            aven_c_ast_descend(ctx);
            uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
            for (;;) {
                uint32_t decl_node = aven_c_ast_parse_struct_declaration(ctx);
                if (decl_node == 0) {
                    break;
                }
                list_push(ctx->scratch) = decl_node;
            }
            close_token = aven_c_ast_next_index(ctx);
            if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_CRBR)) {
                aven_c_ast_error(ctx, state);
                return 0;
            }
            aven_c_ast_ascend(ctx);
            decl_list = aven_c_ast_scratch_commit(ctx, scratch_top);
        }
        if (id_node == 0 and decl_list == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        uint32_t token = main_token;
        if (decl_list != 0) {
            uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
            list_push(ctx->scratch) = main_token;
            list_push(ctx->scratch) = open_token;
            list_push(ctx->scratch) = close_token;
            token = aven_c_ast_scratch_commit(ctx, scratch_top);
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_STRUCT_SPECIFIER,
            token,
            id_node,
            decl_list
        );
    }

    static inline uint32_t aven_c_ast_parse_type_specifier(AvenCAstCtx *ctx) {
        AvenCToken next = aven_c_ast_next(ctx);
        if (next.type != AVEN_C_TOKEN_TYPE_KEY) {
            return 0;
        }
        uint32_t node = 0;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        AvenCKeyword keyword = (AvenCKeyword)next.end;
        switch (keyword) {
            case AVEN_C_KEYWORD_C11BOOL:
            case AVEN_C_KEYWORD_BOOL:
            case AVEN_C_KEYWORD_CHAR:
            case AVEN_C_KEYWORD_DOUBLE:
            case AVEN_C_KEYWORD_FLOAT:
            case AVEN_C_KEYWORD_INT:
            case AVEN_C_KEYWORD_LONG:
            case AVEN_C_KEYWORD_SHORT:
            case AVEN_C_KEYWORD_SIGNED:
            case AVEN_C_KEYWORD_UNSIGNED:
            case AVEN_C_KEYWORD_VOID:
            case AVEN_C_KEYWORD_COMPLEX: {
                aven_c_ast_inc_index(ctx);
                node = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_KEYWORD,
                    main_token
                );
                break;
            }
            case AVEN_C_KEYWORD_UNION:
            case AVEN_C_KEYWORD_STRUCT: {
                node = aven_c_ast_parse_struct_specifier(ctx);
                break;
            }
            case AVEN_C_KEYWORD_ENUM: {
                node = aven_c_ast_parse_enum_specifier(ctx);
                break;
            }
            case AVEN_C_KEYWORD_ATOMIC: {
                node = aven_c_ast_parse_atomic_specifier(ctx);
                break;
            }
            default: {
                if (main_token >= ctx->error.token) {
                    ctx->error = (AvenCAstError){
                        .token = aven_c_ast_next_index(ctx),
                        .type = AVEN_C_TOKEN_TYPE_KEY,
                        .exp = aven_str("<type-specifier>"),
                    };
                }
                break;
            }
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_type_qualifier(AvenCAstCtx *ctx) {
        if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_KEY) {
            return 0;
        }
        AvenCKeyword keyword = (AvenCKeyword)aven_c_ast_next(ctx).end;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t node = 0;
        switch (keyword) {
            case AVEN_C_KEYWORD_CONST:
            case AVEN_C_KEYWORD_RESTRICT:
            case AVEN_C_KEYWORD_VOLATILE:
            case AVEN_C_KEYWORD_ATOMIC: {
                aven_c_ast_inc_index(ctx);
                node = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_TYPE_QUALIFIER,
                    main_token
                );
                break;
            }
            default: {
                if (main_token >= ctx->error.token) {
                    ctx->error = (AvenCAstError){
                        .token = aven_c_ast_next_index(ctx),
                        .type = AVEN_C_TOKEN_TYPE_KEY,
                        .exp = aven_str("[const|restrict|volatile|_Atomic]"),
                    };
                }
                break;
            }
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_type_name(AvenCAstCtx *ctx) {
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        for (;;) {
            uint32_t node = aven_c_ast_parse_type_specifier(ctx);
            if (node == 0) {
                node = aven_c_ast_parse_type_qualifier(ctx);
            }
            if (node == 0) {
                node = aven_c_ast_parse_macro_invocation(ctx);
            }
            if (node == 0) {
                break;
            }
            list_push(ctx->scratch) = node;
        }
        uint32_t list_data = aven_c_ast_scratch_commit(ctx, scratch_top);
        if (list_data == 0) {
            return 0;
        }
        uint32_t rhs = aven_c_ast_parse_abstract_declarator(ctx);
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_TYPE_NAME,
            main_token,
            list_data,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_storage_class_specifier(
        AvenCAstCtx *ctx
    ) {
        AvenCToken next = aven_c_ast_next(ctx);
        if (next.type != AVEN_C_TOKEN_TYPE_KEY) {
            return 0;
        }
        uint32_t node = 0;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        AvenCKeyword keyword = (AvenCKeyword)next.end;
        switch (keyword) {
            case AVEN_C_KEYWORD_TYPEDEF:
            case AVEN_C_KEYWORD_EXTERN:
            case AVEN_C_KEYWORD_STATIC:
            case AVEN_C_KEYWORD_THREAD_LOCAL:
            case AVEN_C_KEYWORD_AUTO:
            case AVEN_C_KEYWORD_REGISTER: {
                aven_c_ast_inc_index(ctx);
                node = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_KEYWORD,
                    main_token
                );
                break;
            }
            default: {
                if (main_token >= ctx->error.token) {
                    ctx->error = (AvenCAstError){
                        .token = aven_c_ast_next_index(ctx),
                        .type = AVEN_C_TOKEN_TYPE_KEY,
                        .exp = aven_str("<storage-specifier>"),
                    };
                }
                break;
            }
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_alignment_specifier(
        AvenCAstCtx *ctx
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_ALIGNAS)) {
            return 0;
        }
        uint32_t start_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_descend(ctx);
        uint32_t node = aven_c_ast_parse_const_expr(ctx);
        if (node == 0) {
            node = aven_c_ast_parse_type_name(ctx);
        }
        if (node == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        uint32_t end_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_ascend(ctx);
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = main_token;
        list_push(ctx->scratch) = start_token;
        list_push(ctx->scratch) = end_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_ALIGNMENT_SPECIFIER,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            node,
            0
        );
    }

    static inline uint32_t aven_c_ast_parse_function_specifier(AvenCAstCtx *ctx) {
        AvenCToken next = aven_c_ast_next(ctx);
        if (next.type != AVEN_C_TOKEN_TYPE_KEY) {
            return 0;
        }
        uint32_t node = 0;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        AvenCKeyword keyword = (AvenCKeyword)next.end;
        switch (keyword) {
            case AVEN_C_KEYWORD_INLINE:
            case AVEN_C_KEYWORD_NORETURN: {
                aven_c_ast_inc_index(ctx);
                node = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_KEYWORD,
                    main_token
                );
                break;
            }
            default: {
                if (main_token >= ctx->error.token) {
                    ctx->error = (AvenCAstError){
                        .token = aven_c_ast_next_index(ctx),
                        .type = AVEN_C_TOKEN_TYPE_KEY,
                        .exp = aven_str("[inline|noreturn]"),
                    };
                }
                break;
            }
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_attribute_specifier(
        AvenCAstCtx *ctx
    ) {
        AvenCToken next = aven_c_ast_next(ctx);
        if (next.type != AVEN_C_TOKEN_TYPE_KEY) {
            return 0;
        }
        uint32_t node = 0;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        AvenCKeyword keyword = (AvenCKeyword)next.end;
        switch (keyword) {
            case AVEN_C_KEYWORD_ATTRIBUTE1:
            case AVEN_C_KEYWORD_ATTRIBUTE2: {
                aven_c_ast_inc_index(ctx);
                node = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_KEYWORD,
                    main_token
                );
                break;
            }
            default: {
                if (main_token >= ctx->error.token) {
                    ctx->error = (AvenCAstError){
                        .token = aven_c_ast_next_index(ctx),
                        .type = AVEN_C_TOKEN_TYPE_KEY,
                        .exp = aven_str("[__attribute|__attribute__]"),
                    };
                }
                break;
            }
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_attribute(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t lhs = aven_c_ast_parse_attribute_specifier(ctx);
        if (lhs == 0) {
            return 0;
        }
        uint32_t open_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_descend(ctx);
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        for (;;) {
            uint32_t arg = aven_c_ast_parse_macro_argument(ctx);
            if (arg == 0) {
                break;
            }
            list_push(ctx->scratch) = arg;
            if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_COM)) {
                break;
            }
        }
        uint32_t arg_list = aven_c_ast_scratch_commit(ctx, scratch_top);
        uint32_t close_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_ascend(ctx);
        scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = open_token;
        list_push(ctx->scratch) = close_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_ATTRIBUTE,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            lhs,
            arg_list
        );
    }

    static inline uint32_t aven_c_ast_parse_declaration_specifier(
        AvenCAstCtx *ctx
    ) {
        uint32_t node = aven_c_ast_parse_storage_class_specifier(ctx);
        if (node == 0) {
            node = aven_c_ast_parse_alignment_specifier(ctx);
        }
        if (node == 0) {
            node = aven_c_ast_parse_function_specifier(ctx);
        }
        if (node == 0) {
            node = aven_c_ast_parse_type_specifier(ctx);
        }
        if (node == 0) {
            node = aven_c_ast_parse_type_qualifier(ctx);
        }
        if (node == 0) {
            node = aven_c_ast_parse_macro_invocation(ctx);
        }
        return node;
    }

    static inline AvenCAstDsl aven_c_ast_parse_declaration_specifier_list(
        AvenCAstCtx *ctx,
        bool abstract,
        bool decl_opt
    ) {
        typedef enum {
            AVEN_C_AST_PARSE_DSL_OPT_NONE = 0,
            AVEN_C_AST_PARSE_DSL_OPT_DECL,
            AVEN_C_AST_PARSE_DSL_OPT_ABDECL,
            AVEN_C_AST_PARSE_DSL_OPT_ID_DECL,
            AVEN_C_AST_PARSE_DSL_OPT_ID_ABDECL,
            AVEN_C_AST_PARSE_DSL_OPT_SPEC_DECL,
            AVEN_C_AST_PARSE_DSL_OPT_SPEC_ABDECL,
        } AvenCAstParseDslOpt;
        uint32_t decl_spec_count = 0;
        uint32_t best_index = ctx->token_index;
        AvenCAstParseDslOpt best_option = AVEN_C_AST_PARSE_DSL_OPT_NONE;
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        AvenCAstCtxState cur = aven_c_ast_save(ctx);
        AvenCAstCtxState last = cur;
        // For my own sanity, I require attributes to be listed first
        for (;;) {
            uint32_t decl_spec = aven_c_ast_parse_attribute(ctx);
            if (decl_spec == 0) {
                break;
            }
            list_push(ctx->scratch) = decl_spec;
            last = cur;
            cur = aven_c_ast_save(ctx);
        }
        for (; decl_spec_count < ctx->max_depth; decl_spec_count += 1) {
            uint32_t decl_spec = aven_c_ast_parse_declaration_specifier(ctx);
            if (decl_spec == 0) {
                break;
            }
            list_push(ctx->scratch) = decl_spec;
            last = cur;
            cur = aven_c_ast_save(ctx);
        }
        if (decl_spec_count == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return (AvenCAstDsl){ 0 };
        }
        if (decl_spec_count == ctx->max_depth) {
            aven_c_ast_error(ctx, state);
            return (AvenCAstDsl){ 0 };
        }
        if (abstract) {
            uint32_t decl = aven_c_ast_parse_abstract_declarator(ctx);
            if ((decl_opt or decl != 0) and ctx->token_index > best_index) {
                best_index = ctx->token_index;
                best_option = AVEN_C_AST_PARSE_DSL_OPT_SPEC_ABDECL;
            }
            aven_c_ast_restore(ctx, cur);
        }
        {
            uint32_t decl = aven_c_ast_parse_declarator(ctx);
            if ((decl_opt or decl != 0) and ctx->token_index > best_index) {
                best_index = ctx->token_index;
                best_option = AVEN_C_AST_PARSE_DSL_OPT_SPEC_DECL;
            }
            aven_c_ast_restore(ctx, last);
        }
        if (abstract and decl_spec_count > 1) {
            uint32_t decl = aven_c_ast_parse_abstract_declarator(ctx);
            if (decl != 0 and ctx->token_index >= best_index) {
                best_index = ctx->token_index;
                best_option = AVEN_C_AST_PARSE_DSL_OPT_ABDECL;
            }
            aven_c_ast_restore(ctx, last);
        }
        if (decl_spec_count > 1) {
            uint32_t decl = aven_c_ast_parse_declarator(ctx);
            if (decl != 0 and ctx->token_index >= best_index) {
                best_index = ctx->token_index;
                best_option = AVEN_C_AST_PARSE_DSL_OPT_DECL;
            }
            aven_c_ast_restore(ctx, last);
        }
        if (abstract) {
            uint32_t id = aven_c_ast_parse_identifier(ctx);
            if (id != 0) {
                uint32_t decl = aven_c_ast_parse_abstract_declarator(ctx);
                if (decl != 0 and ctx->token_index >= best_index) {
                    best_index = ctx->token_index;
                    best_option = AVEN_C_AST_PARSE_DSL_OPT_ID_ABDECL;
                }
                aven_c_ast_restore(ctx, last);
            }
        }
        {
            uint32_t id = aven_c_ast_parse_identifier(ctx);
            if (id != 0) {
                uint32_t decl = aven_c_ast_parse_declarator(ctx);
                if (decl != 0 and ctx->token_index >= best_index) {
                    best_index = ctx->token_index;
                    best_option = AVEN_C_AST_PARSE_DSL_OPT_ID_DECL;
                }
                aven_c_ast_restore(ctx, last);
            }
        }
        bool next_abs = false;
        switch (best_option) {
            case AVEN_C_AST_PARSE_DSL_OPT_NONE: {
                aven_c_ast_restore_trap(ctx, state);
                return (AvenCAstDsl){ 0 };
            }
            case AVEN_C_AST_PARSE_DSL_OPT_DECL: {
                break;
            }
            case AVEN_C_AST_PARSE_DSL_OPT_ABDECL: {
                next_abs = true;
                break;
            }
            case AVEN_C_AST_PARSE_DSL_OPT_ID_DECL: {
                list_push(ctx->scratch) = aven_c_ast_parse_identifier(ctx);
                break;
            }
            case AVEN_C_AST_PARSE_DSL_OPT_ID_ABDECL: {
                list_push(ctx->scratch) = aven_c_ast_parse_identifier(ctx);
                next_abs = true;
                break;
            }
            case AVEN_C_AST_PARSE_DSL_OPT_SPEC_DECL: {
                list_push(ctx->scratch) = aven_c_ast_parse_declaration_specifier(
                    ctx
                );
                break;
            }
            case AVEN_C_AST_PARSE_DSL_OPT_SPEC_ABDECL: {
                list_push(ctx->scratch) = aven_c_ast_parse_declaration_specifier(
                    ctx
                );
                next_abs = true;
                break;
            }
        }
        uint32_t decl_spec_list = aven_c_ast_scratch_commit(ctx, scratch_top);
        return (AvenCAstDsl){
            .abstract = next_abs,
            .decl_spec_list = decl_spec_list,
        };
    }

    static inline uint32_t aven_c_ast_parse_parameter_declaration(
        AvenCAstCtx *ctx
    ) {
        uint32_t main_token = aven_c_ast_next_index(ctx);
        AvenCAstDsl dsl = aven_c_ast_parse_declaration_specifier_list(
            ctx,
            true,
            true
        );
        if (dsl.decl_spec_list == 0) {
            return 0;
        }
        uint32_t rhs = 0;
        if (dsl.abstract) {
            rhs = aven_c_ast_parse_abstract_declarator(ctx);
        } else {
            rhs = aven_c_ast_parse_declarator(ctx);
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_PARAMETER_DECLARATION,
            main_token,
            dsl.decl_spec_list,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_parameter_type_list(
        AvenCAstCtx *ctx
    ) {
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        for (;;) {
            uint32_t pdecl_node = aven_c_ast_parse_parameter_declaration(ctx);
            if (pdecl_node == 0) {
                pdecl_node = aven_c_ast_parse_punctuator(ctx, AVEN_C_PNC_DOT3);
            }
            if (pdecl_node == 0) {
                break;
            }
            list_push(ctx->scratch) = pdecl_node;
            if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_COM)) {
                break;
            }
        }
        uint32_t pdecl_list = aven_c_ast_scratch_commit(ctx, scratch_top);
        return pdecl_list;
    }

    static inline uint32_t aven_c_ast_parse_designator(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_SQBL)) {
            aven_c_ast_descend(ctx);
            uint32_t node = aven_c_ast_parse_const_expr(ctx);
            if (node == 0) {
                aven_c_ast_restore_trap(ctx, state);
                return 0;
            }
            uint32_t end_token = aven_c_ast_next_index(ctx);
            if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_SQBR)) {
                aven_c_ast_error(ctx, state);
                return 0;
            }
            aven_c_ast_ascend(ctx);
            uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
            list_push(ctx->scratch) = main_token;
            list_push(ctx->scratch) = end_token;
            return aven_c_ast_push(
                ctx,
                AVEN_C_AST_NODE_TYPE_ARRAY_DESIGNATOR,
                aven_c_ast_scratch_commit(ctx, scratch_top),
                node,
                0
            );
        }
        if (aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_DOT)) {
            uint32_t node = aven_c_ast_parse_identifier(ctx);
            if (node == 0) {
                aven_c_ast_error(ctx, state);
                return 0;
            }
            return aven_c_ast_push(
                ctx,
                AVEN_C_AST_NODE_TYPE_DOT_DESIGNATOR,
                main_token,
                node,
                0
            );
        }
        return 0;
    }

    static inline uint32_t aven_c_ast_parse_designation(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        for (;;) {
            uint32_t designator = aven_c_ast_parse_designator(ctx);
            if (designator == 0) {
                break;
            }
            list_push(ctx->scratch) = designator;
        }
        uint32_t designators = aven_c_ast_scratch_commit(ctx, scratch_top);
        if (designators == 0) {
            return 0;
        }
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_EQ)) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t rhs = aven_c_ast_parse_initializer_list(ctx);
        if (rhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_DESIGNATION,
            main_token,
            designators,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_initializer_list(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t open_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_CRBL)) {
            return aven_c_ast_parse_assign_expr(ctx);
        }
        aven_c_ast_descend(ctx);
        uint32_t list = 0;
        {
            uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
            for (;;) {
                uint32_t node = aven_c_ast_parse_designation(ctx);
                if (node == 0) {
                    node = aven_c_ast_parse_initializer_list(ctx);
                }
                if (node == 0) {
                    break;
                }
                list_push(ctx->scratch) = node;
                if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_COM)) {
                    break;
                }
            }
            list = aven_c_ast_scratch_commit(ctx, scratch_top);
        }
        if (list == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t close_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_CRBR)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_ascend(ctx);
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = open_token;
        list_push(ctx->scratch) = close_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_INITIALIZER_LIST,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            list,
            0
        );
    }

    static inline uint32_t aven_c_ast_parse_pointer(AvenCAstCtx *ctx) {
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_STR)) {
            return 0;
        }
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        for (;;) {
            uint32_t qual = aven_c_ast_parse_type_qualifier(ctx);
            if (qual == 0) {
                break;
            }
            list_push(ctx->scratch) = qual;
        }
        uint32_t lhs = aven_c_ast_scratch_commit(ctx, scratch_top);
        uint32_t rhs = aven_c_ast_parse_pointer(ctx);
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_POINTER,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_direct_declarator_op(
        AvenCAstCtx *ctx,
        uint32_t parent,
        bool allow_paren
    ) {
        AvenCToken next_token = aven_c_ast_next(ctx);
        if (next_token.type != AVEN_C_TOKEN_TYPE_PNC) {
            return 0;
        }
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        AvenStr tstr = aven_c_token_str(ctx->tset, main_token);
        uint32_t node = 0;
        switch (tstr.ptr[0]) {
            case '[': {
                if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_SQBL)) {
                    break;
                }
                aven_c_ast_descend(ctx);
                uint32_t arg_list = 0;
                {
                    bool stat = false;
                    bool ptr = false;
                    bool expr = false;
                    uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
                    {
                        uint32_t stat_node = aven_c_ast_parse_keyword(
                            ctx,
                            AVEN_C_KEYWORD_STATIC
                        );
                        if (stat_node != 0) {
                            stat = true;
                            list_push(ctx->scratch) = stat_node;
                        }
                    }
                    for (;;) {
                        uint32_t qual = aven_c_ast_parse_type_qualifier(ctx);
                        if (qual == 0) {
                            break;
                        }
                        list_push(ctx->scratch) = qual;
                    }
                    if (!stat) {
                        uint32_t stat_node = aven_c_ast_parse_keyword(
                            ctx,
                            AVEN_C_KEYWORD_STATIC
                        );
                        if (stat_node != 0) {
                            stat = true;
                            list_push(ctx->scratch) = stat_node;
                        }
                    }
                    if (!stat) {
                        uint32_t ptr_node = aven_c_ast_parse_punctuator(
                            ctx,
                            AVEN_C_PNC_STR
                        );
                        if (ptr_node != 0) {
                            ptr = true;
                            list_push(ctx->scratch) = ptr_node;
                        }
                    }
                    if (!ptr) {
                        uint32_t expr_node = aven_c_ast_parse_assign_expr(ctx);
                        if (expr_node != 0) {
                            expr = true;
                            list_push(ctx->scratch) = expr_node;
                        }
                    }
                    if (stat and !expr) {
                        aven_c_ast_restore_trap(ctx, state);
                        break;
                    }
                    arg_list = aven_c_ast_scratch_commit(ctx, scratch_top);
                }
                uint32_t end_token = aven_c_ast_next_index(ctx);
                if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_SQBR)) {
                    aven_c_ast_restore_trap(ctx, state);
                    break;
                }
                aven_c_ast_ascend(ctx);
                uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
                list_push(ctx->scratch) = main_token;
                list_push(ctx->scratch) = end_token;
                node = aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR_BRAC,
                    aven_c_ast_scratch_commit(ctx, scratch_top),
                    parent,
                    arg_list
                );
                break;
            }
            case '(': {
                if (!allow_paren) {
                    break;
                }
                if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
                    break;
                }
                aven_c_ast_descend(ctx);
                uint32_t plist = aven_c_ast_parse_parameter_type_list(ctx);
                if (plist != 0) {
                    uint32_t end_token = aven_c_ast_next_index(ctx);
                    if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
                        aven_c_ast_restore_trap(ctx, state);
                        break;
                    }
                    aven_c_ast_ascend(ctx);
                    uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
                    list_push(ctx->scratch) = main_token;
                    list_push(ctx->scratch) = end_token;
                    node = aven_c_ast_push(
                        ctx,
                        AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR_PARAM_TYPE_LIST,
                        aven_c_ast_scratch_commit(ctx, scratch_top),
                        parent,
                        plist
                    );
                    break;
                }
                uint32_t id_list;
                {
                    uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
                    for (;;) {
                        uint32_t id_node = aven_c_ast_parse_identifier(ctx);
                        if (id_node == 0) {
                            break;
                        }
                        list_push(ctx->scratch) = id_node;
                    }
                    id_list = aven_c_ast_scratch_commit(ctx, scratch_top);
                }
                uint32_t end_token = aven_c_ast_next_index(ctx);
                if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
                    aven_c_ast_restore_trap(ctx, state);
                    break;
                }
                aven_c_ast_ascend(ctx);
                uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
                list_push(ctx->scratch) = main_token;
                list_push(ctx->scratch) = end_token;
                node = aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR_ID_LIST,
                    aven_c_ast_scratch_commit(ctx, scratch_top),
                    parent,
                    id_list
                );
                break;
            }
            case '_':
            case 'a': {
                uint32_t attribute = aven_c_ast_parse_attribute(ctx);
                if (attribute == 0) {
                    break;
                }
                node = aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR_ATTRIBUTE,
                    main_token,
                    parent,
                    attribute
                );
                break;
            }
            default: {
                break;
            }
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_direct_declarator_attribute(
        AvenCAstCtx *ctx,
        uint32_t parent
    ) {
        AvenCToken next_token = aven_c_ast_next(ctx);
        if (next_token.type != AVEN_C_TOKEN_TYPE_KEY) {
            return 0;
        }
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t node = 0;
        switch ((AvenCKeyword)next_token.end) {
            case AVEN_C_KEYWORD_ATTRIBUTE1:
            case AVEN_C_KEYWORD_ATTRIBUTE2: {
                uint32_t attribute = aven_c_ast_parse_attribute(ctx);
                if (attribute == 0) {
                    break;
                }
                node = aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR_ATTRIBUTE,
                    main_token,
                    parent,
                    attribute
                );
                break;
            }
            default: {
                break;
            }
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_direct_declarator(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t node = 0;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        bool paren = true;
        if (aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
            aven_c_ast_descend(ctx);
            uint32_t decl = aven_c_ast_parse_declarator(ctx);
            if (decl == 0) {
                aven_c_ast_restore_trap(ctx, state);
                return false;
            }
            uint32_t end_token = aven_c_ast_next_index(ctx);
            if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
                aven_c_ast_restore_trap(ctx, state);
                return false;
            }
            aven_c_ast_ascend(ctx);
            uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
            list_push(ctx->scratch) = main_token;
            list_push(ctx->scratch) = end_token;
            node = aven_c_ast_push(
                ctx,
                AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR,
                aven_c_ast_scratch_commit(ctx, scratch_top),
                decl,
                0
            );
            // We only allow parenthesis in declarators with postfix op
            //     int (*x);      -> not ok, why do this?
            //     int (*x)(int); -> ok, declare function pointer 
            uint32_t suffix_node = aven_c_ast_parse_direct_declarator_op(
                ctx,
                node,
                paren
            );
            paren = false;
            if (suffix_node == 0) {
                aven_c_ast_restore_trap(ctx, state);
                return false;
            }
            node = suffix_node;
        }
        if (node == 0) {
            node = aven_c_ast_parse_identifier(ctx);
        }
        if (node == 0) {
            return 0;
        }
        for (;;) {
            uint32_t suffix_node = aven_c_ast_parse_direct_declarator_op(
                ctx,
                node,
                paren
            );
            paren = false;
            if (suffix_node == 0) {
                break;
            }
            node = suffix_node;
        }
        for (;;) {
            uint32_t suffix_node = aven_c_ast_parse_direct_declarator_attribute(
                ctx,
                node
            );
            if (suffix_node == 0) {
                break;
            }
            node = suffix_node;
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_declarator(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t lhs = aven_c_ast_parse_pointer(ctx);
        uint32_t rhs = aven_c_ast_parse_direct_declarator(ctx);
        if (rhs == 0) {
            if (lhs != 0) {
                aven_c_ast_restore_trap(ctx, state);
            }
            return 0;
        }
        if (lhs == 0) {
            return rhs;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_DECLARATOR,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_direct_abstract_declarator_op(
        AvenCAstCtx *ctx,
        uint32_t parent,
        bool allow_paren
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        AvenCToken next_token = aven_c_ast_next(ctx);
        if (next_token.type != AVEN_C_TOKEN_TYPE_PNC) {
            return 0;
        }
        AvenStr tstr = aven_c_token_str(ctx->tset, main_token);
        if (tstr.len != 1) {
            return 0;
        }
        uint32_t node = 0;
        switch (tstr.ptr[0]) {
            case '[': {
                aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_SQBL);
                aven_c_ast_descend(ctx);
                bool stat = false;
                bool ptr = false;
                bool expr = false;
                uint32_t arg_list = 0;
                {
                    uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
                    {
                        uint32_t ptr_node = aven_c_ast_parse_punctuator(
                            ctx,
                            AVEN_C_PNC_STR
                        );
                        if (ptr_node != 0) {
                            ptr = true;
                            list_push(ctx->scratch) = ptr_node;
                        }
                    }
                    if (!ptr) {
                        {
                            uint32_t stat_node = aven_c_ast_parse_keyword(
                                ctx,
                                AVEN_C_KEYWORD_STATIC
                            );
                            if (stat_node != 0) {
                                stat = true;
                                list_push(ctx->scratch) = stat_node;
                            }
                        }
                        for (;;) {
                            uint32_t qual = aven_c_ast_parse_type_qualifier(ctx);
                            if (qual == 0) {
                                break;
                            }
                            list_push(ctx->scratch) = qual;
                        }
                        if (!stat) {
                            uint32_t stat_node = aven_c_ast_parse_keyword(
                                ctx,
                                AVEN_C_KEYWORD_STATIC
                            );
                            if (stat_node != 0) {
                                stat = true;
                                list_push(ctx->scratch) = stat_node;
                            }
                        }
                        if (stat and !expr) {
                            aven_c_ast_restore_trap(ctx, state);
                            break;
                        }
                    }
                    arg_list = aven_c_ast_scratch_commit(ctx, scratch_top);
                }
                uint32_t end_token = aven_c_ast_next_index(ctx);
                if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_SQBR)) {
                    aven_c_ast_restore_trap(ctx, state);
                    break;
                }
                aven_c_ast_ascend(ctx);
                uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
                list_push(ctx->scratch) = main_token;
                list_push(ctx->scratch) = end_token;
                node = aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_DIR_ABS_DECLARATOR_BRAC,
                    aven_c_ast_scratch_commit(ctx, scratch_top),
                    parent,
                    arg_list
                );
                break;
            }
            case '(': {
                if (!allow_paren) {
                    break;
                }
                aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL);
                aven_c_ast_descend(ctx);
                uint32_t plist = aven_c_ast_parse_parameter_type_list(ctx);
                uint32_t end_token = aven_c_ast_next_index(ctx);
                if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
                    aven_c_ast_restore_trap(ctx, state);
                    break;
                }
                aven_c_ast_ascend(ctx);
                uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
                list_push(ctx->scratch) = main_token;
                list_push(ctx->scratch) = end_token;
                node = aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_DIR_ABS_DECLARATOR_PARAM_TYPE_LIST,
                    aven_c_ast_scratch_commit(ctx, scratch_top),
                    parent,
                    plist
                );
                break;
            }
            default: {
                break;
            }
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_direct_abstract_declarator(
        AvenCAstCtx *ctx
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t node = 0;
        bool paren = true;
        if (aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
            aven_c_ast_descend(ctx);
            uint32_t decl = aven_c_ast_parse_abstract_declarator(ctx);
            if (decl == 0) {
                aven_c_ast_restore_trap(ctx, state);
                return false;
            }
            uint32_t end_token = aven_c_ast_next_index(ctx);
            if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
                aven_c_ast_restore_trap(ctx, state);
                return false;
            }
            aven_c_ast_ascend(ctx);
            uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
            list_push(ctx->scratch) = main_token;
            list_push(ctx->scratch) = end_token;
            node = aven_c_ast_push(
                ctx,
                AVEN_C_AST_NODE_TYPE_DIR_ABS_DECLARATOR,
                aven_c_ast_scratch_commit(ctx, scratch_top),
                decl,
                0
            );
            // We only allow parenthesis in declarators with postfix op
            //     int (*)      -> not ok, why do this?
            //     int (*)(int) -> ok, a function pointer 
            uint32_t suffix_node =
                aven_c_ast_parse_direct_abstract_declarator_op(ctx, node, paren);
            paren = false;
            if (suffix_node == 0) {
                aven_c_ast_error(ctx, state);
                return 0;
            }
            node = suffix_node;
        }
        for (;;) {
            uint32_t suffix_node =
                aven_c_ast_parse_direct_abstract_declarator_op(ctx, node, paren);
            paren = false;
            if (suffix_node == 0) {
                break;
            }
            node = suffix_node;
        }

        if (node == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }

        return node;
    }

    static inline uint32_t aven_c_ast_parse_abstract_declarator(
        AvenCAstCtx *ctx
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t lhs = aven_c_ast_parse_pointer(ctx);
        uint32_t rhs = aven_c_ast_parse_direct_abstract_declarator(ctx);
        if (lhs == 0 and rhs == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        if (lhs == 0) {
            return rhs;
        }
        if (rhs == 0) {
            return lhs;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_ABS_DECLARATOR,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_init_declarator(AvenCAstCtx *ctx) {
        uint32_t decl_node = aven_c_ast_parse_declarator(ctx);
        if (decl_node == 0) {
            return 0;
        }
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_EQ)) {
            return decl_node;
        }
        uint32_t init_node = aven_c_ast_parse_initializer_list(ctx);
        if (init_node == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_INIT_DECLARATOR,
            main_token,
            decl_node,
            init_node
        );
    }

    static inline uint32_t aven_c_ast_parse_init_declarator_list(
        AvenCAstCtx *ctx
    ) {
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        for (;;) {
            uint32_t init_decl = aven_c_ast_parse_init_declarator(ctx);
            if (init_decl == 0) {
                break;
            }
            list_push(ctx->scratch) = init_decl;
            if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_COM)) {
                break;
            }
        }
        return aven_c_ast_scratch_commit(ctx, scratch_top);
    }

    static inline uint32_t aven_c_ast_parse_declaration_unterminated(
        AvenCAstCtx *ctx
    ) {
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t static_ass_node = aven_c_ast_parse_static_assert_declaration(
            ctx
        );
        if (static_ass_node != 0) {
            return static_ass_node;
        }
        AvenCAstDsl dsl = aven_c_ast_parse_declaration_specifier_list(
            ctx,
            false,
            true
        );
        if (dsl.decl_spec_list == 0) {
            return aven_c_ast_parse_declaration_specifier(ctx);
        }
        uint32_t init_decl_list = aven_c_ast_parse_init_declarator_list(ctx);
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_DECLARATION,
            main_token,
            dsl.decl_spec_list,
            init_decl_list
        );
    }

    static inline uint32_t aven_c_ast_parse_declaration(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t unterm = aven_c_ast_parse_declaration_unterminated(ctx);
        if (unterm == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t term = aven_c_ast_parse_terminated_line(ctx, unterm);
        if (term == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        return term;
    }

    static inline uint32_t aven_c_ast_parse_primary_expr(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t exp_node = aven_c_ast_parse_string_constant(ctx);
        if (exp_node != 0) {
            return exp_node;
        }
        exp_node = aven_c_ast_parse_constant(ctx);
        if (exp_node != 0) {
            return exp_node;
        }
        exp_node = aven_c_ast_parse_identifier(ctx);
        if (exp_node != 0) {
            return exp_node;
        }

        uint32_t open_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
            return 0;
        }
        aven_c_ast_descend(ctx);
        exp_node = aven_c_ast_parse_expr(ctx);
        uint32_t close_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        aven_c_ast_ascend(ctx);
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = open_token;
        list_push(ctx->scratch) = close_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_PRIMARY_EXPR,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            exp_node,
            0
        );
    }

    static inline uint32_t aven_c_ast_parse_postfix_expr_initializer(
        AvenCAstCtx *ctx
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
            return 0;
        }
        aven_c_ast_descend(ctx);
        uint32_t type_node = aven_c_ast_parse_type_name(ctx);
        if (type_node == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t end_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        aven_c_ast_ascend(ctx);
        uint32_t initializer_list = aven_c_ast_parse_initializer_list(ctx);
        if (initializer_list == 0) {
            aven_c_ast_restore(ctx, state);
            return 0;
        }
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = main_token;
        list_push(ctx->scratch) = end_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_INITIALIZER,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            type_node,
            initializer_list
        );
    }

    static inline uint32_t aven_c_ast_parse_postfix_op(
        AvenCAstCtx *ctx,
        uint32_t parent
    ) {
        assert(parent != 0);
        if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_PNC) {
            return 0;
        }
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        AvenCPnc pnc = (AvenCPnc)aven_c_ast_next(ctx).end;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t node = 0;
        switch (pnc) {
            case AVEN_C_PNC_SQBL: {
                aven_c_ast_inc_index(ctx);
                aven_c_ast_descend(ctx);
                uint32_t arg = aven_c_ast_parse_expr(ctx);
                if (arg == 0) {
                    aven_c_ast_restore_trap(ctx, state);
                    break;
                }
                uint32_t end_token = aven_c_ast_next_index(ctx);
                if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_SQBR)) {
                    aven_c_ast_error(ctx, state);
                    break;
                }
                aven_c_ast_ascend(ctx);
                uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
                list_push(ctx->scratch) = main_token;
                list_push(ctx->scratch) = end_token;
                node = aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_BRAC,
                    aven_c_ast_scratch_commit(ctx, scratch_top),
                    parent,
                    arg
                );
                break;
            }
            case AVEN_C_PNC_PARL: {
                aven_c_ast_inc_index(ctx);
                aven_c_ast_descend(ctx);
                uint32_t arg_list = 0;
                {
                    uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
                    for (;;) {
                        uint32_t arg = aven_c_ast_parse_macro_argument(ctx);
                        if (arg == 0) {
                            break;
                        }
                        list_push(ctx->scratch) = arg;
                        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_COM)) {
                            break;
                        }
                    }
                    arg_list = aven_c_ast_scratch_commit(ctx, scratch_top);
                }
                uint32_t end_token = aven_c_ast_next_index(ctx);
                if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
                    aven_c_ast_restore_trap(ctx, state);
                    break;
                }
                aven_c_ast_ascend(ctx);
                uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
                list_push(ctx->scratch) = main_token;
                list_push(ctx->scratch) = end_token;
                node = aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_PAREN,
                    aven_c_ast_scratch_commit(ctx, scratch_top),
                    parent,
                    arg_list
                );
                break;
            }
            case AVEN_C_PNC_DOT: {
                aven_c_ast_inc_index(ctx);
                uint32_t id_node = aven_c_ast_parse_identifier(ctx);
                if (id_node == 0) {
                    aven_c_ast_error(ctx, state);
                    break;
                }
                node = aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_BOP,
                    main_token,
                    parent,
                    id_node
                );
                break;
            }
            case AVEN_C_PNC_ARW: {
                aven_c_ast_inc_index(ctx);
                uint32_t id_node = aven_c_ast_parse_identifier(ctx);
                if (id_node == 0) {
                    aven_c_ast_error(ctx, state);
                    break;
                }
                node = aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_BOP,
                    main_token,
                    parent,
                    id_node
                );
                break;
            }
            case AVEN_C_PNC_MIN2: {
                aven_c_ast_inc_index(ctx);
                node = aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_UOP,
                    main_token,
                    parent,
                    0
                );
                break;
            }
            case AVEN_C_PNC_PLS2: {
                aven_c_ast_inc_index(ctx);
                node = aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_UOP,
                    main_token,
                    parent,
                    0
                );
                break;
            }
            default: {
                break;
            }
        }

        return node;
    }

    static inline uint32_t aven_c_ast_parse_postfix_expr(AvenCAstCtx *ctx) {
        uint32_t node = aven_c_ast_parse_postfix_expr_initializer(ctx);
        if (node == 0) {
            node = aven_c_ast_parse_primary_expr(ctx);
        }
        if (node == 0) {
            return 0;
        }
        for (;;) {
            uint32_t suffix_node = aven_c_ast_parse_postfix_op(ctx, node);
            if (suffix_node == 0) {
                break;
            }
            node = suffix_node;
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_unary_expr(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        if (ctx->unary_expr_depth > ctx->max_depth) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        uint32_t node = 0;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        AvenCTokenType token_type = aven_c_ast_next(ctx).type;
        ctx->unary_expr_depth += 1;
        switch (token_type) {
            case AVEN_C_TOKEN_TYPE_KEY: {
                bool sizeof_op = aven_c_ast_match_keyword(
                    ctx,
                    AVEN_C_KEYWORD_SIZEOF
                );
                bool alignof_op = !sizeof_op and
                    aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_ALIGNOF);
                if (!(sizeof_op or alignof_op)) {
                    break;
                }
                uint32_t open_token = aven_c_ast_next_index(ctx);
                uint32_t child = aven_c_ast_parse_unary_expr(ctx);
                if (child != 0) {
                    node = aven_c_ast_push(
                        ctx,
                        AVEN_C_AST_NODE_TYPE_UNARY_EXPR_SIZEOF,
                        main_token,
                        child,
                        0
                    );
                    break;
                }
                if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
                    aven_c_ast_error(ctx, state);
                    break;
                }
                aven_c_ast_descend(ctx);
                child = aven_c_ast_parse_type_name(ctx);
                if (child == 0) {
                    aven_c_ast_error(ctx, state);
                    break;
                }
                uint32_t close_token = aven_c_ast_next_index(ctx);
                if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
                    aven_c_ast_error(ctx, state);
                    break;
                }
                aven_c_ast_ascend(ctx);
                uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
                list_push(ctx->scratch) = main_token;
                list_push(ctx->scratch) = open_token;
                list_push(ctx->scratch) = close_token;
                node = aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_UNARY_EXPR_FN,
                    aven_c_ast_scratch_commit(ctx, scratch_top),
                    child,
                    0
                );
                break;
            }
            case AVEN_C_TOKEN_TYPE_PNC: {
                AvenStr token_str = aven_c_token_str(ctx->tset, main_token);
                if (token_str.len != 1) {
                    if (
                        !(
                            aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PLS2) or
                            aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_MIN2)
                        )
                    ) {
                        break;
                    }
                    uint32_t child = aven_c_ast_parse_unary_expr(ctx);
                    if (child == 0) {
                        aven_c_ast_error(ctx, state);
                        break;
                    }
                    node = aven_c_ast_push(
                        ctx,
                        AVEN_C_AST_NODE_TYPE_UNARY_EXPR,
                        main_token,
                        child,
                        0
                    );
                } else {
                    switch (get(token_str, 0)) {
                        case '&':
                        case '*':
                        case '+':
                        case '-':
                        case '~':
                        case '!': {
                            aven_c_ast_inc_index(ctx);
                            uint32_t child = aven_c_ast_parse_cast_expr(ctx);
                            if (child == 0) {
                                aven_c_ast_restore_trap(ctx, state);
                                break;
                            }
                            node = aven_c_ast_push(
                                ctx,
                                AVEN_C_AST_NODE_TYPE_UNARY_EXPR,
                                main_token,
                                child,
                                0
                            );
                            break;
                        }
                    }
                }
                break;
            }
            default: {
                break;
            }
        }
        if (node == 0) {
            node = aven_c_ast_parse_postfix_expr(ctx);
        }
        ctx->unary_expr_depth -= 1;
        return node;
    }

    static inline uint32_t aven_c_ast_parse_cast_expr(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
            return aven_c_ast_parse_unary_expr(ctx);
        }
        aven_c_ast_descend(ctx);
        uint32_t lhs = aven_c_ast_parse_type_name(ctx);
        if (lhs == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return aven_c_ast_parse_unary_expr(ctx);
        }
        uint32_t end_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
            aven_c_ast_restore_trap(ctx, state);
            return aven_c_ast_parse_unary_expr(ctx);
        }
        aven_c_ast_ascend(ctx);
        uint32_t rhs = aven_c_ast_parse_cast_expr(ctx);
        if (rhs == 0) {
            aven_c_ast_restore(ctx, state);
            return aven_c_ast_parse_unary_expr(ctx);
        }
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = main_token;
        list_push(ctx->scratch) = end_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_CAST_EXPR,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_multiply_op(
        AvenCAstCtx *ctx,
        uint32_t lhs
    ) {
        AvenCToken next_token = aven_c_ast_next(ctx);
        if (next_token.type != AVEN_C_TOKEN_TYPE_PNC) {
            return 0;
        }
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        AvenCPnc pnc = (AvenCPnc)aven_c_ast_next(ctx).end;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t rhs = 0;
        switch (pnc) {
            case AVEN_C_PNC_STR:
            case AVEN_C_PNC_FSL:
            case AVEN_C_PNC_PCT: {
                aven_c_ast_inc_index(ctx);
                rhs = aven_c_ast_parse_cast_expr(ctx);
                break;
            }
            default: {
                break;
            }
        }
        if (rhs == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_MULTIPLY_EXPR,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_multiply_expr(AvenCAstCtx *ctx) {
        uint32_t node = aven_c_ast_parse_cast_expr(ctx);
        if (node == 0) {
            return 0;
        }
        for (;;) {
            uint32_t rhs = aven_c_ast_parse_multiply_op(ctx, node);
            if (rhs == 0) {
                break;
            }
            node = rhs;
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_add_op(
        AvenCAstCtx *ctx,
        uint32_t lhs
    ) {
        AvenCToken next_token = aven_c_ast_next(ctx);
        if (next_token.type != AVEN_C_TOKEN_TYPE_PNC) {
            return 0;
        }
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        AvenCPnc pnc = (AvenCPnc)aven_c_ast_next(ctx).end;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t rhs = 0;
        switch (pnc) {
            case AVEN_C_PNC_PLS:
            case AVEN_C_PNC_MIN: {
                aven_c_ast_inc_index(ctx);
                rhs = aven_c_ast_parse_multiply_expr(ctx);
                break;
            }
            default: {
                break;
            }
        }
        if (rhs == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_ADD_EXPR,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_add_expr(AvenCAstCtx *ctx) {
        uint32_t node = aven_c_ast_parse_multiply_expr(ctx);
        if (node == 0) {
            return 0;
        }
        for (;;) {
            uint32_t rhs = aven_c_ast_parse_add_op(ctx, node);
            if (rhs == 0) {
                break;
            }
            node = rhs;
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_shift_op(
        AvenCAstCtx *ctx,
        uint32_t lhs
    ) {
        if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_PNC) {
            return 0;
        }
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        AvenCPnc pnc = (AvenCPnc)aven_c_ast_next(ctx).end;
        uint32_t rhs = 0;
        switch (pnc) {
            case AVEN_C_PNC_GT2:
            case AVEN_C_PNC_LT2: {
                aven_c_ast_inc_index(ctx);
                rhs = aven_c_ast_parse_add_expr(ctx);
                if (rhs == 0) {
                    aven_c_ast_error(ctx, state);
                }
                break;
            }
            default: {
                break;
            }
        }
        if (rhs == 0) {
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_SHIFT_EXPR,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_shift_expr(AvenCAstCtx *ctx) {
        uint32_t node = aven_c_ast_parse_add_expr(ctx);
        if (node == 0) {
            return 0;
        }
        for (;;) {
            uint32_t rhs = aven_c_ast_parse_shift_op(ctx, node);
            if (rhs == 0) {
                break;
            }
            node = rhs;
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_relate_op(
        AvenCAstCtx *ctx,
        uint32_t lhs
    ) {
        if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_PNC) {
            return 0;
        }
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        AvenCPnc pnc = (AvenCPnc)aven_c_ast_next(ctx).end;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t rhs = 0;
        switch (pnc) {
            case AVEN_C_PNC_LT:
            case AVEN_C_PNC_LTEQ:
            case AVEN_C_PNC_GT:
            case AVEN_C_PNC_GTEQ: {
                aven_c_ast_inc_index(ctx);
                rhs = aven_c_ast_parse_shift_expr(ctx);
                if (rhs == 0) {
                    aven_c_ast_error(ctx, state);
                }
                break;
            }
            default: {
                break;
            }
        }
        if (rhs == 0) {
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_RELATE_EXPR,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_relate_expr(AvenCAstCtx *ctx) {
        uint32_t node = aven_c_ast_parse_shift_expr(ctx);
        if (node == 0) {
            return 0;
        }
        for (;;) {
            uint32_t rhs = aven_c_ast_parse_relate_op(ctx, node);
            if (rhs == 0) {
                break;
            }
            node = rhs;
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_equal_op(
        AvenCAstCtx *ctx,
        uint32_t lhs
    ) {
        if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_PNC) {
            return 0;
        }
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        AvenCPnc pnc = (AvenCPnc)aven_c_ast_next(ctx).end;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t rhs = 0;
        switch (pnc) {
            case AVEN_C_PNC_EQ2:
            case AVEN_C_PNC_EXCEQ: {
                aven_c_ast_inc_index(ctx);
                rhs = aven_c_ast_parse_relate_expr(ctx);
                if (rhs == 0) {
                    aven_c_ast_error(ctx, state);
                }
                break;
            }
            default: {
                break;
            }
        }
        if (rhs == 0) {
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_EQUAL_EXPR,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_equal_expr(AvenCAstCtx *ctx) {
        uint32_t node = aven_c_ast_parse_relate_expr(ctx);
        if (node == 0) {
            return 0;
        }
        for (;;) {
            uint32_t rhs = aven_c_ast_parse_equal_op(ctx, node);
            if (rhs == 0) {
                break;
            }
            node = rhs;
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_and_op(
        AvenCAstCtx *ctx,
        uint32_t lhs
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_AMP)) {
            return 0;
        }
        uint32_t rhs = aven_c_ast_parse_equal_expr(ctx);
        if (rhs == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_AND_EXPR,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_and_expr(AvenCAstCtx *ctx) {
        uint32_t node = aven_c_ast_parse_equal_expr(ctx);
        if (node == 0) {
            return 0;
        }
        for (;;) {
            uint32_t rhs = aven_c_ast_parse_and_op(ctx, node);
            if (rhs == 0) {
                break;
            }
            node = rhs;
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_xor_op(
        AvenCAstCtx *ctx,
        uint32_t lhs
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_CAR)) {
            return 0;
        }
        uint32_t rhs = aven_c_ast_parse_and_expr(ctx);
        if (rhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_XOR_EXPR,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_xor_expr(AvenCAstCtx *ctx) {
        uint32_t node = aven_c_ast_parse_and_expr(ctx);
        if (node == 0) {
            return 0;
        }
        for (;;) {
            uint32_t rhs = aven_c_ast_parse_xor_op(ctx, node);
            if (rhs == 0) {
                break;
            }
            node = rhs;
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_or_op(
        AvenCAstCtx *ctx,
        uint32_t lhs
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_BAR)) {
            return 0;
        }
        uint32_t rhs = aven_c_ast_parse_xor_expr(ctx);
        if (rhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_OR_EXPR,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_or_expr(AvenCAstCtx *ctx) {
        uint32_t node = aven_c_ast_parse_xor_expr(ctx);
        if (node == 0) {
            return 0;
        }
        for (;;) {
            uint32_t rhs = aven_c_ast_parse_or_op(ctx, node);
            if (rhs == 0) {
                break;
            }
            node = rhs;
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_logical_and_op(
        AvenCAstCtx *ctx,
        uint32_t lhs
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (
            !(
                aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_AMP2) or
                aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_AND)
            )
        ) {
            return 0;
        }
        uint32_t rhs = aven_c_ast_parse_or_expr(ctx);
        if (rhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_LOGICAL_AND_EXPR,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_logical_and_expr(AvenCAstCtx *ctx) {
        uint32_t node = aven_c_ast_parse_or_expr(ctx);
        if (node == 0) {
            return 0;
        }
        for (;;) {
            uint32_t rhs = aven_c_ast_parse_logical_and_op(ctx, node);
            if (rhs == 0) {
                break;
            }
            node = rhs;
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_logical_or_op(
        AvenCAstCtx *ctx,
        uint32_t lhs
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (
            !(
                aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_BAR2) or
                aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_OR)
            )
        ) {
            return 0;
        }
        uint32_t rhs = aven_c_ast_parse_logical_and_expr(ctx);
        if (rhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_LOGICAL_OR_EXPR,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_logical_or_expr(AvenCAstCtx *ctx) {
        uint32_t node = aven_c_ast_parse_logical_and_expr(ctx);
        if (node == 0) {
            return 0;
        }
        for (;;) {
            uint32_t rhs = aven_c_ast_parse_logical_or_op(ctx, node);
            if (rhs == 0) {
                break;
            }
            node = rhs;
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_conditional_op(
        AvenCAstCtx *ctx,
        uint32_t lhs
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t colo_token = 0;
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_QST)) {
            return 0;
        }
        uint32_t nodes = 0;
        {
            uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
            uint32_t middle = aven_c_ast_parse_expr(ctx);
            if (middle == 0) {
                aven_c_ast_error(ctx, state);
                return 0;
            }
            list_push(ctx->scratch) = middle;
            colo_token = aven_c_ast_next_index(ctx);
            if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_COL)) {
                aven_c_ast_error(ctx, state);
                return 0;
            }
            uint32_t rhs = aven_c_ast_parse_logical_or_expr(ctx);
            if (rhs == 0) {
                aven_c_ast_error(ctx, state);
                return 0;
            }
            list_push(ctx->scratch) = rhs;
            nodes = aven_c_ast_scratch_commit(ctx, scratch_top);
        }
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = main_token;
        list_push(ctx->scratch) = colo_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_CONDITIONAL_EXPR,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            lhs,
            nodes
        );
    }

    static inline uint32_t aven_c_ast_parse_conditional_expr(AvenCAstCtx *ctx) {
        uint32_t node = aven_c_ast_parse_logical_or_expr(ctx);
        if (node == 0) {
            return 0;
        }
        for (;;) {
            uint32_t rhs = aven_c_ast_parse_conditional_op(ctx, node);
            if (rhs == 0) {
                break;
            }
            node = rhs;
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_assign_op(
        AvenCAstCtx *ctx,
        uint32_t lhs
    ) {
        if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_PNC) {
            return 0;
        }
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        AvenCPnc pnc = (AvenCPnc)aven_c_ast_next(ctx).end;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t rhs = 0;
        switch (pnc) {
            case AVEN_C_PNC_EQ:
            case AVEN_C_PNC_STREQ:
            case AVEN_C_PNC_FSLEQ:
            case AVEN_C_PNC_PLSEQ:
            case AVEN_C_PNC_MINEQ:
            case AVEN_C_PNC_PCTEQ:
            case AVEN_C_PNC_LT2EQ:
            case AVEN_C_PNC_GT2EQ:
            case AVEN_C_PNC_AMPEQ:
            case AVEN_C_PNC_CAREQ:
            case AVEN_C_PNC_BAREQ: {
                aven_c_ast_inc_index(ctx);
                rhs = aven_c_ast_parse_conditional_expr(ctx);
                if (rhs == 0) {
                    aven_c_ast_error(ctx, state);
                }
                break;
            }
            default: {
                break;
            }
        }
        if (rhs == 0) {
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_ASSIGN_EXPR,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_assign_expr(AvenCAstCtx *ctx) {
        uint32_t node = aven_c_ast_parse_conditional_expr(ctx);
        if (node == 0) {
            return 0;
        }
        for (;;) {
            uint32_t rhs = aven_c_ast_parse_assign_op(ctx, node);
            if (rhs == 0) {
                break;
            }
            node = rhs;
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_expr(AvenCAstCtx *ctx) {
        uint32_t node = aven_c_ast_parse_assign_expr(ctx);
        if (node == 0) {
            return false;
        }
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = node;
        uint32_t count = 0;
        for (;;) {
            if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_COM)) {
                break;
            }
            uint32_t next = aven_c_ast_parse_assign_expr(ctx);
            if (next == 0) {
                aven_c_ast_error(ctx, state);
                return node;
            }
            list_push(ctx->scratch) = next;
            count += 1;
        }
        uint32_t list = aven_c_ast_scratch_commit(ctx, scratch_top);
        if (count == 0) {
            aven_c_ast_restore(ctx, state);
            return node;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_EXPR,
            main_token,
            list,
            0
        );
    }

    static inline uint32_t aven_c_ast_parse_const_expr(AvenCAstCtx *ctx) {
        return aven_c_ast_parse_conditional_expr(ctx);
    }

    static inline uint32_t aven_c_ast_parse_statement(AvenCAstCtx *ctx);

    static inline uint32_t aven_c_ast_parse_switch_case(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_CASE)) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t expr_node = aven_c_ast_parse_const_expr(ctx);
        if (expr_node == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_SWITCH_CASE,
            main_token,
            expr_node,
            0
        );
    }

    static inline uint32_t aven_c_ast_parse_labeled_statement(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t lhs = aven_c_ast_parse_keyword(ctx, AVEN_C_KEYWORD_DEFAULT);
        if (lhs == 0) {
            lhs = aven_c_ast_parse_switch_case(ctx);
        }
        if (lhs == 0) {
            lhs = aven_c_ast_parse_identifier(ctx);
        }
        if (lhs == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_COL)) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t rhs = aven_c_ast_parse_statement(ctx);
        if (rhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_LABELED_STATEMENT,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_compound_statement(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_CRBL)) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        aven_c_ast_descend(ctx);
        uint32_t block_items = 0;
        {
            uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
            for (;;) {
                uint32_t block_item_node = aven_c_ast_parse_declaration(ctx);
                if (block_item_node == 0) {
                    block_item_node = aven_c_ast_parse_statement(ctx);
                }
                if (block_item_node == 0) {
                    break;
                }
                list_push(ctx->scratch) = block_item_node;
            }
            block_items = aven_c_ast_scratch_commit(ctx, scratch_top);
        }
        uint32_t end_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_CRBR)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_ascend(ctx);
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = main_token;
        list_push(ctx->scratch) = end_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_COMPOUND_STATEMENT,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            block_items,
            0
        );
    }

    static inline uint32_t aven_c_ast_parse_if_else_statement(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_ELSE)) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t lhs = aven_c_ast_parse_statement(ctx);
        if (lhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_IF_ELSE_STATEMENT,
            main_token,
            lhs,
            0
        );
    }

    static inline uint32_t aven_c_ast_parse_if_statement(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_IF)) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t start_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_descend(ctx);
        uint32_t lhs = aven_c_ast_parse_expr(ctx);
        if (lhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        uint32_t end_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_ascend(ctx);
        uint32_t rhs = 0;
        {
            uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
            uint32_t statement = aven_c_ast_parse_statement(ctx);
            if (statement == 0) {
                aven_c_ast_error(ctx, state);
                return 0;
            }
            list_push(ctx->scratch) = statement;
            uint32_t else_node = aven_c_ast_parse_if_else_statement(ctx);
            if (else_node != 0) {
                list_push(ctx->scratch) = else_node;
            }
            rhs = aven_c_ast_scratch_commit(ctx, scratch_top);
        }
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = main_token;
        list_push(ctx->scratch) = start_token;
        list_push(ctx->scratch) = end_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_IF_STATEMENT,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_switch_statement(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_SWITCH)) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t start_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_descend(ctx);
        uint32_t lhs = aven_c_ast_parse_expr(ctx);
        if (lhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        uint32_t end_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_ascend(ctx);
        uint32_t rhs = aven_c_ast_parse_statement(ctx);
        if (rhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = main_token;
        list_push(ctx->scratch) = start_token;
        list_push(ctx->scratch) = end_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_SWITCH_STATEMENT,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_selection_statement(
        AvenCAstCtx *ctx
    ) {
        AvenCToken next = aven_c_ast_next(ctx);
        if (next.type != AVEN_C_TOKEN_TYPE_KEY) {
            return 0;
        }
        uint32_t node = 0;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        AvenCKeyword keyword = (AvenCKeyword)next.end;
        switch (keyword) {
            case AVEN_C_KEYWORD_IF: {
                node = aven_c_ast_parse_if_statement(ctx);
                break;
            }
            case AVEN_C_KEYWORD_SWITCH: {
                node = aven_c_ast_parse_switch_statement(ctx);
                break;
            }
            default: {
                if (main_token >= ctx->error.token) {
                    ctx->error = (AvenCAstError){
                        .token = aven_c_ast_next_index(ctx),
                        .type = AVEN_C_TOKEN_TYPE_KEY,
                        .exp = aven_str("[if|switch]"),
                    };
                }
                break;
            }
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_while_statement(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_WHILE)) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t start_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_descend(ctx);
        uint32_t lhs = aven_c_ast_parse_expr(ctx);
        if (lhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        uint32_t end_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_ascend(ctx);
        uint32_t rhs = aven_c_ast_parse_statement(ctx);
        if (rhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = main_token;
        list_push(ctx->scratch) = start_token;
        list_push(ctx->scratch) = end_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_WHILE_STATEMENT,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_do_statement_unterminated(
        AvenCAstCtx *ctx
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_DO)) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t lhs = aven_c_ast_parse_statement(ctx);
        if (lhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        uint32_t while_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_WHILE)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        uint32_t start_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_descend(ctx);
        uint32_t rhs = aven_c_ast_parse_expr(ctx);
        if (rhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        uint32_t end_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_ascend(ctx);
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = main_token;
        list_push(ctx->scratch) = while_token;
        list_push(ctx->scratch) = start_token;
        list_push(ctx->scratch) = end_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_DO_STATEMENT,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_do_statement(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t unterm = aven_c_ast_parse_do_statement_unterminated(ctx);
        if (unterm == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t term = aven_c_ast_parse_terminated_line(ctx, unterm);
        if (term == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        return term;
    }

    static inline uint32_t aven_c_ast_parse_for_statement(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_FOR)) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t open_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_descend(ctx);
        uint32_t lhs = 0;
        {
            uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
            uint32_t count = 0;
            AvenCAstCtxState pre_decl_state = aven_c_ast_save(ctx);
            uint32_t decl_node = aven_c_ast_parse_declaration_unterminated(ctx);
            if (decl_node != 0) {
                if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_SCOL)) {
                    aven_c_ast_restore(ctx, pre_decl_state);
                } else {
                    list_push(ctx->scratch) = decl_node;
                    count += 1;
                }
            }
            for (; count < 2; count += 1) {
                uint32_t expr_node = aven_c_ast_parse_expr(ctx);
                list_push(ctx->scratch) = expr_node;
                if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_SCOL)) {
                    aven_c_ast_error(ctx, state);
                    return 0;
                }
            }
            uint32_t expr_node = aven_c_ast_parse_expr(ctx);
            list_push(ctx->scratch) = expr_node;
            lhs = aven_c_ast_scratch_commit(ctx, scratch_top);
        }
        uint32_t close_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_ascend(ctx);
        uint32_t rhs = aven_c_ast_parse_statement(ctx);
        if (rhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = main_token;
        list_push(ctx->scratch) = open_token;
        list_push(ctx->scratch) = close_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_FOR_STATEMENT,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_iteration_statement(
        AvenCAstCtx *ctx
    ) {
        uint32_t node = aven_c_ast_parse_while_statement(ctx);
        if (node == 0) {
            node = aven_c_ast_parse_do_statement(ctx);
        }
        if (node == 0) {
            node = aven_c_ast_parse_for_statement(ctx);
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_goto_statement(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t lhs = aven_c_ast_parse_keyword(ctx, AVEN_C_KEYWORD_GOTO);
        if (lhs == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t rhs = aven_c_ast_parse_identifier(ctx);
        if (rhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_GOTO_STATEMENT,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_return_statement(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t lhs = aven_c_ast_parse_keyword(ctx, AVEN_C_KEYWORD_RETURN);
        if (lhs == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t rhs = aven_c_ast_parse_expr(ctx);
        if (rhs == 0) {
            return lhs;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_RETURN_STATEMENT,
            main_token,
            lhs,
            rhs
        );
    }

    static inline uint32_t aven_c_ast_parse_jump_statement_unterminated(
        AvenCAstCtx *ctx
    ) {
        AvenCToken next = aven_c_ast_next(ctx);
        if (next.type != AVEN_C_TOKEN_TYPE_KEY) {
            return 0;
        }
        uint32_t node = 0;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        AvenCKeyword keyword = (AvenCKeyword)next.end;
        switch (keyword) {
            case AVEN_C_KEYWORD_BREAK:
            case AVEN_C_KEYWORD_CONTINUE: {
                aven_c_ast_inc_index(ctx);
                node = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_KEYWORD,
                    main_token
                );
                break;
            }
            case AVEN_C_KEYWORD_GOTO: {
                node = aven_c_ast_parse_goto_statement(ctx);
                break;
            }
            case AVEN_C_KEYWORD_RETURN: {
                node = aven_c_ast_parse_return_statement(ctx);
                break;
            }
            default: {
                if (main_token >= ctx->error.token) {
                    ctx->error = (AvenCAstError){
                        .token = aven_c_ast_next_index(ctx),
                        .type = AVEN_C_TOKEN_TYPE_KEY,
                        .exp = aven_str("[break|continue|goto|return]"),
                    };
                }
                break;
            }
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_jump_statement(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t unterm = aven_c_ast_parse_jump_statement_unterminated(ctx);
        if (unterm == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t term = aven_c_ast_parse_terminated_line(ctx, unterm);
        if (term == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        return term;
    }

    static inline uint32_t aven_c_ast_parse_expr_statement(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t unterm = aven_c_ast_parse_expr(ctx);
        uint32_t term = aven_c_ast_parse_terminated_line(ctx, unterm);
        if (term == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        return term;
    }

    static inline uint32_t aven_c_ast_parse_asm_specifier(AvenCAstCtx *ctx) {
        AvenCToken next = aven_c_ast_next(ctx);
        if (next.type != AVEN_C_TOKEN_TYPE_KEY) {
            return 0;
        }
        uint32_t node = 0;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        AvenCKeyword keyword = (AvenCKeyword)next.end;
        switch (keyword) {
            case AVEN_C_KEYWORD_ASM1:
            case AVEN_C_KEYWORD_ASM2: {
                aven_c_ast_inc_index(ctx);
                node = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_KEYWORD,
                    main_token
                );
                break;
            }
            default: {
                if (main_token >= ctx->error.token) {
                    ctx->error = (AvenCAstError){
                        .token = aven_c_ast_next_index(ctx),
                        .type = AVEN_C_TOKEN_TYPE_KEY,
                        .exp = aven_str("[asm|__asm__]"),
                    };
                }
                break;
            }
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_asm_qualifier(AvenCAstCtx *ctx) {
        AvenCToken next = aven_c_ast_next(ctx);
        if (next.type != AVEN_C_TOKEN_TYPE_KEY) {
            return 0;
        }
        uint32_t node = 0;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        AvenCKeyword keyword = (AvenCKeyword)next.end;
        switch (keyword) {
            case AVEN_C_KEYWORD_VOLATILE:
            case AVEN_C_KEYWORD_GOTO:
            case AVEN_C_KEYWORD_INLINE: {
                aven_c_ast_inc_index(ctx);
                node = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_KEYWORD,
                    main_token
                );
                break;
            }
            default: {
                if (main_token >= ctx->error.token) {
                    ctx->error = (AvenCAstError){
                        .token = aven_c_ast_next_index(ctx),
                        .type = AVEN_C_TOKEN_TYPE_KEY,
                        .exp = aven_str("[volatile|goto|inline]"),
                    };
                }
                break;
            }
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_asm_statement_unterminated(
        AvenCAstCtx *ctx
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        for (;;) {
            uint32_t attrib = aven_c_ast_parse_attribute(ctx);
            if (attrib == 0) {
                break;
            }
            list_push(ctx->scratch) = attrib;
        }
        uint32_t asm_node = aven_c_ast_parse_asm_specifier(ctx);
        if (asm_node == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        list_push(ctx->scratch) = asm_node;
        for (;;) {
            uint32_t qual = aven_c_ast_parse_asm_qualifier(ctx);
            if (qual == 0) {
                break;
            }
            list_push(ctx->scratch) = qual;
        }
        uint32_t open_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_descend(ctx);
        uint32_t qual_list = aven_c_ast_scratch_commit(ctx, scratch_top);
        scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = aven_c_ast_parse_string_constant(ctx);
        for (;;) {
            if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_COL)) {
                break;
            }
            list_push(ctx->scratch) = aven_c_ast_parse_string_constant(ctx);
        }
        uint32_t close_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_ascend(ctx);
        uint32_t asm_list = aven_c_ast_scratch_commit(ctx, scratch_top);
        scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = open_token;
        list_push(ctx->scratch) = close_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_ASM_STATEMENT,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            qual_list,
            asm_list
        );
    }

    static inline uint32_t aven_c_ast_parse_asm_statement(AvenCAstCtx *ctx) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t unterm = aven_c_ast_parse_asm_statement_unterminated(ctx);
        if (unterm == 0) {
            return 0;
        }
        uint32_t term = aven_c_ast_parse_terminated_line(ctx, unterm);
        if (term == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        return term;
    }

    static inline uint32_t aven_c_ast_parse_statement(AvenCAstCtx *ctx) {
        uint32_t node = aven_c_ast_parse_compound_statement(ctx);
        if (node == 0) {
            node = aven_c_ast_parse_iteration_statement(ctx);
        }
        if (node == 0) {
            node = aven_c_ast_parse_asm_statement(ctx);
        }
        if (node == 0) {
            node = aven_c_ast_parse_selection_statement(ctx);
        }
        if (node == 0) {
            node = aven_c_ast_parse_jump_statement(ctx);
        }
        if (node == 0) {
            node = aven_c_ast_parse_labeled_statement(ctx);
        }
        if (node == 0) {
            node = aven_c_ast_parse_expr_statement(ctx);
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_function_definition(
        AvenCAstCtx *ctx
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        AvenCAstDsl dsl = aven_c_ast_parse_declaration_specifier_list(
            ctx,
            false,
            false
        );
        if (dsl.decl_spec_list == 0) {
            return 0;
        }
        uint32_t declarator = aven_c_ast_parse_declarator(ctx);
        if (declarator == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t statement = aven_c_ast_parse_compound_statement(ctx);
        if (statement == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = declarator;
        list_push(ctx->scratch) = statement;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_FUNCTION_DEFINITION,
            main_token,
            dsl.decl_spec_list,
            aven_c_ast_scratch_commit(ctx, scratch_top)
        );
    }

    static inline uint32_t aven_c_ast_parse_translation_unit(AvenCAstCtx *ctx) {
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        for (;;) {
            uint32_t extern_decl = aven_c_ast_parse_function_definition(ctx);
            if (extern_decl == 0) {
                extern_decl = aven_c_ast_parse_declaration(ctx);
            }
            if (extern_decl == 0) {
                break;
            }
            list_push(ctx->scratch) = extern_decl;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_TRANSLATION_UNIT,
            main_token,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            0
        );
    }

    static inline AvenCAstResult aven_c_ast_parse(
        AvenCTokenSet tset,
        uint32_t max_depth,
        AvenArena *arena
    ) {
        AvenArena temp_arena = *arena;
        AvenCAstCtx ctx = aven_c_ast_init(tset, max_depth, &temp_arena);
        uint32_t last_index = aven_c_ast_parse_ppd_tokens(&ctx);
        aven_c_ast_inc_index(&ctx);
        uint32_t root = 0;
        if (!ctx.depth_exceeded) {
            root = aven_c_ast_parse_translation_unit(&ctx);
        }
        bool normal_error = aven_c_ast_next_index(&ctx) != last_index or
            root == 0;
        if (normal_error or ctx.depth_exceeded or ctx.ppd_error.valid) {
            AvenCToken token = get(ctx.tset.tokens, ctx.error.token);
            AvenCTokenLoc eloc = aven_c_token_loc(ctx.tset, ctx.error.token);
            uint32_t token_index = token.index;
            AvenCTokenType exp_token_type = ctx.error.type;
            AvenStr exp_token_str = ctx.error.exp;
            if (
                !normal_error or
                (
                    ctx.ppd_error.valid and
                    ctx.ppd_error.value.token < ctx.error.token
                )
            ) {
                AvenCAstError ppd_error = ctx.ppd_error.value;
                eloc = aven_c_token_loc(ctx.tset, ppd_error.pp_token - 1);
                token = get(ctx.tset.tokens, ppd_error.pp_token - 1);
                token_index = token.index;
                exp_token_type = ppd_error.type;
                exp_token_str = ppd_error.exp;
            }
            AvenStr exp_type = exp_token_type == AVEN_C_TOKEN_TYPE_NUM ?
                aven_str("constant") :
                aven_c_token_type_str(exp_token_type);
            Optional(AvenStr) exp_str = { 0 };
            if (
                exp_token_type == AVEN_C_TOKEN_TYPE_PNC or
                exp_token_type == AVEN_C_TOKEN_TYPE_KEY
            ) {
                exp_str.valid = true;
                exp_str.value = exp_token_str;
            }
            size_t back_offset = 64;
            size_t start = token_index >= back_offset ?
                token_index - back_offset :
                0;
            for (size_t i = start; i < (size_t)token_index; i += 1) {
                if (get(ctx.tset.bytes, i) == '\n') {
                    start = i + 1;
                }
            }
            size_t end = min(start + 100, ctx.tset.bytes.len);
            for (uint32_t i = token_index; i < end; i += 1) {
                if (get(ctx.tset.bytes, i) == '\n') {
                    end = i;
                    break;
                }
            }
            AvenStr line = aven_str_range(ctx.tset.bytes, start, end);
            char arrow_buffer[165];
            AvenStr arrow_str = slice_array(arrow_buffer);
            size_t arrow_len = 0;
            size_t start_col = eloc.col - (token_index - start);
            size_t ln_digits = aven_fmt_uint_decimal_digits(eloc.line);
            ln_digits += aven_fmt_uint_decimal_digits(start_col);
            assert(ln_digits < 64);
            for (size_t i = 0; i < ln_digits + 1; i += 1) {
                get(arrow_str, arrow_len) = ' ';
                arrow_len += 1;
            }
            for (size_t i = start; i < (size_t)token_index; i += 1) {
                get(arrow_str, arrow_len) = ' ';
                arrow_len += 1;
            }
            get(arrow_str, arrow_len) = '^';
            arrow_len += 1;
            arrow_str = aven_str_head(arrow_str, arrow_len);
            AvenStr error_str = exp_str.valid ?
                aven_fmt(
                    &temp_arena,
                    "error at {}:{}: expected {} '{}', found:\n"
                    "{}:{}: {}\n  {}"
                    ,
                    aven_fmt_uint(eloc.line),
                    aven_fmt_uint(eloc.col),
                    aven_fmt_str(exp_type),
                    aven_fmt_str(exp_str.value),
                    aven_fmt_uint(eloc.line),
                    aven_fmt_uint(start_col),
                    aven_fmt_str(line),
                    aven_fmt_str(arrow_str)
                ) :
                aven_fmt(
                    &temp_arena,
                    "error at {}:{}: expected {}, found:\n" "{}:{}: {}\n  {}",
                    aven_fmt_uint(eloc.line),
                    aven_fmt_uint(eloc.col),
                    aven_fmt_str(exp_type),
                    aven_fmt_uint(eloc.line),
                    aven_fmt_uint(start_col),
                    aven_fmt_str(line),
                    aven_fmt_str(arrow_str)
                );
            AvenStr final_str = ctx.depth_exceeded ?
                aven_fmt(
                    arena,
                    "parse depth of {} exceeded\n{}",
                    aven_fmt_uint(ctx.max_depth),
                    aven_fmt_str(error_str)
                ) :
                aven_fmt(arena, "{}", aven_fmt_str(error_str));
            return (AvenCAstResult){
                .type = AVEN_C_AST_RESULT_TYPE_ERROR,
                .data = { .error = final_str },
            };
        }
        AvenCAstNodeSlice nodes = aven_arena_create_slice(
            AvenCAstNode,
            arena,
            ctx.nodes.len
        );
        AvenCAstDataSlice data = aven_arena_create_slice(
            uint32_t,
            arena,
            ctx.data.len
        );
        AvenCAstDataSlice pp_nodes = aven_arena_create_slice(
            uint32_t,
            arena,
            ctx.pp_nodes.len
        );
        for (size_t i = 0; i < nodes.len; i += 1) {
            get(nodes, i) = get(ctx.nodes, i);
        }
        for (size_t i = 0; i < data.len; i += 1) {
            get(data, i) = get(ctx.data, i);
        }
        for (size_t i = 0; i < pp_nodes.len; i += 1) {
            get(pp_nodes, i) = get(ctx.pp_nodes, i);
        }
        return (AvenCAstResult){
            .type = AVEN_C_AST_RESULT_TYPE_AST,
            .data = {
                .ast = {
                    .tset = tset,
                    .nodes = nodes,
                    .data = data,
                    .pp_nodes = pp_nodes,
                    .root = root,
                },
            },
        };
    }

    static inline uint32_t aven_c_ast_parse_preprocessor_header(
        AvenCAstCtx *ctx
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_LT)) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        aven_c_ast_descend(ctx);
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = main_token;
        for (;;) {
            uint32_t end_token = aven_c_ast_next_index(ctx);
            if (aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_GT)) {
                list_push(ctx->scratch) = end_token;
                break;
            }
            if (aven_c_ast_next(ctx).type == AVEN_C_TOKEN_TYPE_NONE) {
                aven_c_ast_error(ctx, state);
                return 0;
            }
            list_push(ctx->scratch) = aven_c_ast_inc_index(ctx);
        }
        aven_c_ast_ascend(ctx);
        return aven_c_ast_push_leaf(
            ctx,
            AVEN_C_AST_NODE_TYPE_PREPROCESSOR_HEADER,
            aven_c_ast_scratch_commit(ctx, scratch_top)
        );
    }

    static inline uint32_t aven_c_ast_parse_preprocessor_define(
        AvenCAstCtx *ctx
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        AvenCToken id_token = aven_c_ast_next(ctx);
        if (id_token.type != AVEN_C_TOKEN_TYPE_ID) {
            return 0;
        }
        AvenStr id_token_str = aven_c_token_str(ctx->tset, main_token);
        if (!aven_str_equals(id_token_str, aven_str("define"))) {
            return 0;
        }
        aven_c_ast_inc_index(ctx);
        AvenCToken name_token = aven_c_ast_next(ctx);
        uint32_t name_node = aven_c_ast_parse_identifier(ctx);
        if (
            name_node == 0 and
            aven_c_ast_next(ctx).type == AVEN_C_TOKEN_TYPE_KEY
        ) {
            name_node = aven_c_ast_push_leaf(
                ctx,
                AVEN_C_AST_NODE_TYPE_KEYWORD,
                aven_c_ast_inc_index(ctx)
            );
        }
        if (name_node == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        // no space between name and '(' for function macro
        AvenCToken paren_token = aven_c_ast_next(ctx);
        uint32_t open_token = aven_c_ast_next_index(ctx);
        if (
            paren_token.index != name_token.end or
            !aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARL)
        ) {
            return aven_c_ast_push(
                ctx,
                AVEN_C_AST_NODE_TYPE_PREPROCESSOR_DEFINE,
                main_token,
                name_node,
                0
            );
        }
        aven_c_ast_descend(ctx);
        uint32_t arg_list = 0;
        {
            uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
            for (;;) {
                uint32_t arg = aven_c_ast_parse_const_expr(ctx);
                if (arg == 0) {
                    arg = aven_c_ast_parse_punctuator(ctx, AVEN_C_PNC_DOT3);
                }
                if (arg == 0) {
                    break;
                }
                list_push(ctx->scratch) = arg;
                if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_COM)) {
                    break;
                }
            }
            arg_list = aven_c_ast_scratch_commit(ctx, scratch_top);
        }
        uint32_t close_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_PARR)) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        aven_c_ast_ascend(ctx);
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = main_token;
        list_push(ctx->scratch) = open_token;
        list_push(ctx->scratch) = close_token;
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_PREPROCESSOR_DEFINE_FN,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            name_node,
            arg_list
        );
    }

    static inline uint32_t aven_c_ast_parse_preprocessor_spec_list(
        AvenCAstCtx *ctx
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        uint32_t count = 0;
        for (;;) {
            uint32_t attr = aven_c_ast_parse_attribute(ctx);
            if (attr == 0) {
                break;
            }
            list_push(ctx->scratch) = attr;
            count += 1;
        }
        for (;;) {
            uint32_t attr = aven_c_ast_parse_declaration_specifier(ctx);
            if (attr == 0) {
                break;
            }
            list_push(ctx->scratch) = attr;
            count += 1;
        }
        if (count == 0) {
            aven_c_ast_restore_trap(ctx, state);
            return 0;
        }
        if (count == 1) {
            uint32_t node = list_pop(ctx->scratch);
            uint32_t list = aven_c_ast_scratch_commit(ctx, scratch_top);
            assert(list == 0);
            return node;
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_PREPROCESSOR_SPEC_LIST,
            main_token,
            aven_c_ast_scratch_commit(ctx, scratch_top),
            0
        );
    }

    static inline uint32_t aven_c_ast_parse_if_else_specifier(AvenCAstCtx *ctx) {
        AvenCToken next = aven_c_ast_next(ctx);
        if (next.type != AVEN_C_TOKEN_TYPE_KEY) {
            return 0;
        }
        uint32_t node = 0;
        uint32_t main_token = aven_c_ast_next_index(ctx);
        AvenCKeyword keyword = (AvenCKeyword)next.end;
        switch (keyword) {
            case AVEN_C_KEYWORD_IF:
            case AVEN_C_KEYWORD_ELSE: {
                aven_c_ast_inc_index(ctx);
                node = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_KEYWORD,
                    main_token
                );
                break;
            }
            default: {
                if (main_token >= ctx->error.token) {
                    ctx->error = (AvenCAstError){
                        .token = aven_c_ast_next_index(ctx),
                        .type = AVEN_C_TOKEN_TYPE_KEY,
                        .exp = aven_str("[if|else]"),
                    };
                }
                break;
            }
        }
        return node;
    }

    static inline uint32_t aven_c_ast_parse_preprocessor_directive(
        AvenCAstCtx *ctx
    ) {
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        uint32_t main_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, AVEN_C_PNC_HSH)) {
            return false;
        }
        uint32_t lhs = aven_c_ast_parse_preprocessor_define(ctx);
        if (lhs == 0) {
            lhs = aven_c_ast_parse_identifier(ctx);
        }
        if (lhs == 0) {
            lhs = aven_c_ast_parse_if_else_specifier(ctx);
        }
        if (lhs == 0) {
            aven_c_ast_error(ctx, state);
            return 0;
        }
        AvenCAstCtxState rhs_state = aven_c_ast_save(ctx);
        if (aven_c_ast_next(ctx).type == AVEN_C_TOKEN_TYPE_NONE) {
            return aven_c_ast_push(
                ctx,
                AVEN_C_AST_NODE_TYPE_PREPROCESSOR_DIRECTIVE,
                main_token,
                lhs,
                0
            );
        }
        uint32_t rhs = aven_c_ast_push_leaf(
            ctx,
            AVEN_C_AST_NODE_TYPE_ANYTOKEN,
            aven_c_ast_inc_index(ctx)
        );
        if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_NONE) {
            rhs = 0;
            aven_c_ast_restore(ctx, rhs_state);
        }
        if (rhs == 0) {
            rhs = aven_c_ast_parse_preprocessor_header(ctx);
            if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_NONE) {
                rhs = 0;
                aven_c_ast_restore(ctx, rhs_state);
            }
        }
        if (rhs == 0) {
            rhs = aven_c_ast_parse_jump_statement_unterminated(ctx);
            if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_NONE) {
                rhs = 0;
                aven_c_ast_restore(ctx, rhs_state);
            }
        }
        if (rhs == 0) {
            rhs = aven_c_ast_parse_do_statement_unterminated(ctx);
            if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_NONE) {
                rhs = 0;
                aven_c_ast_restore(ctx, rhs_state);
            }
        }
        if (rhs == 0) {
            rhs = aven_c_ast_parse_initializer_list(ctx);
            if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_NONE) {
                rhs = 0;
                aven_c_ast_restore(ctx, rhs_state);
            }
        }
        if (rhs == 0) {
            rhs = aven_c_ast_parse_type_name(ctx);
            if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_NONE) {
                rhs = 0;
                aven_c_ast_restore(ctx, rhs_state);
            }
        }
        if (rhs == 0) {
            rhs = aven_c_ast_parse_preprocessor_spec_list(ctx);
            if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_NONE) {
                rhs = 0;
                aven_c_ast_restore(ctx, rhs_state);
            }
        }
        if (rhs == 0) {
            rhs = aven_c_ast_parse_asm_statement_unterminated(ctx);
            if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_NONE) {
                rhs = 0;
                aven_c_ast_restore(ctx, rhs_state);
            }
        }
        if (rhs == 0) {
            rhs = aven_c_ast_parse_expr(ctx);
            if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_NONE) {
                rhs = 0;
                aven_c_ast_restore(ctx, rhs_state);
            }
        }
        if (rhs == 0) {
            rhs = aven_c_ast_parse_declaration_unterminated(ctx);
            if (aven_c_ast_next(ctx).type == AVEN_C_TOKEN_TYPE_NONE) {
                aven_c_ast_restore(ctx, state);
                return 0;
            }
        }
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_PREPROCESSOR_DIRECTIVE,
            main_token,
            lhs,
            rhs
        );
    }

    typedef enum {
        AVEN_C_AST_RENDER_ERROR_NONE = 0,
        AVEN_C_AST_RENDER_ERROR_FMT = 1,
        AVEN_C_AST_RENDER_ERROR_IO = 1,
    } AvenCAstRenderError;
    typedef struct {
        AvenCAstRenderError error;
        int io_error;
        AvenStr msg;
    } AvenCAstRenderResult;

    typedef struct {
        AvenCAst *ast;
        AvenIoWriter *writer;
        AvenStr line;
        AvenStr newline_str;
        AvenStr indent_str;
        size_t line_len;
        int io_error;
        uint32_t pp_cursor;
        uint32_t lines_written;
        uint32_t cursor;
        uint32_t indent;
        uint32_t pp_indent;
        uint32_t trailing_lines;
        bool ppd;
    } AvenCAstRenderCtx;

    typedef struct {
        uint32_t cursor;
        uint32_t indent;
        bool ppd;
    } AvenCAstRenderCtxState;

    static inline AvenCAstRenderCtxState aven_c_ast_render_save(
        AvenCAstRenderCtx *ctx
    ) {
        return (AvenCAstRenderCtxState){
            .cursor = ctx->cursor,
            .indent = ctx->indent,
            .ppd = ctx->ppd,
        };
    }

    static inline void aven_c_ast_render_restore(
        AvenCAstRenderCtx *ctx,
        AvenCAstRenderCtxState state
    ) {
        ctx->cursor = state.cursor;
        ctx->indent = state.indent;
        ctx->ppd = state.ppd;
    }

    static inline bool aven_c_ast_render_write(
        AvenCAstRenderCtx *ctx,
        AvenStr str,
        bool force_fit
    ) {
        if (str.len == 0) {
            return true;
        }
        size_t cap = ctx->line_len;
        if (force_fit) {
            cap = ctx->line.len - ctx->newline_str.len;
        }
        if (ctx->ppd) {
            cap -= 2;
        }
        AvenStr rem = aven_str_range(
            ctx->line,
            ctx->cursor,
            max(cap, ctx->cursor)
        );
        if (ctx->cursor == 0) {
            for (uint32_t i = 0; i < ctx->indent + ctx->pp_indent; i += 1) {
                if (rem.len < ctx->indent_str.len) {
                    return false;
                }
                slice_copy(rem, ctx->indent_str);
                ctx->cursor += (uint32_t)ctx->indent_str.len;
                rem = aven_str_tail(rem, ctx->indent_str.len);
            }
        }
        if (rem.len < str.len) {
            return false;
        }
        slice_copy(rem, str);
        ctx->cursor += (uint32_t)str.len;
        return true;
    }

    static inline bool aven_c_ast_render_flush_line(AvenCAstRenderCtx *ctx) {
        while (ctx->cursor > 0 and get(ctx->line, ctx->cursor - 1) == ' ') {
            ctx->cursor -= 1;
        }
        if (ctx->cursor == 0) {
            return true;
        }
        if (ctx->ppd) {
            if (get(ctx->line, ctx->cursor - 1) != ' ') {
                get(ctx->line, ctx->cursor) = ' ';
                ctx->cursor += 1;
            }
            get(ctx->line, ctx->cursor) = '\\';
            ctx->cursor += 1;
        }
        slice_copy(aven_str_tail(ctx->line, ctx->cursor), ctx->newline_str);
        ctx->cursor += (uint32_t)ctx->newline_str.len;

        AvenStr line = aven_str_head(ctx->line, ctx->cursor);
        AvenIoResult res = aven_io_writer_push(
            ctx->writer,
            slice_as_bytes(line)
        );
        if (res.error != 0) {
            ctx->io_error = res.error;
            return false;
        }
        if (res.payload != line.len) {
            ctx->io_error = -1;
            return false;
        }
        ctx->lines_written += 1;
        ctx->cursor = 0;
        return true;
    }

    #define aven_c_ast_render_token_try(c, i, s, l) do { \
            if (!aven_c_ast_render_token_try_internal(c, i, s, false)) { \
                aven_c_ast_render_restore(c, l); \
                return false; \
            } \
        } while (0)
    #define aven_c_ast_render_token_force_try(c, i, s, l) do { \
            if (!aven_c_ast_render_token_try_internal(c, i, s, true)) { \
                aven_c_ast_render_restore(c, l); \
                return false; \
            } \
        } while (0)
    #define aven_c_ast_render_space_try(c, s, l) do { \
            if (s) { \
                if (!aven_c_ast_render_flush_line(c)) { \
                    return false; \
                } \
            } else if (!aven_c_ast_render_write(ctx, aven_str(" "), true)) { \
                aven_c_ast_render_restore(c, l); \
                return false; \
            } \
        } while (0)

    static inline bool aven_c_ast_render_node(
        AvenCAstRenderCtx *ctx,
        AvenCAstNodeType parent_type,
        uint32_t index,
        bool split
    );
    static inline bool aven_c_ast_render_pp_nodes(
        AvenCAstRenderCtx *ctx,
        uint32_t token_index,
        bool split
    );

    static inline bool aven_c_ast_render_token_try_internal(
        AvenCAstRenderCtx *ctx,
        uint32_t token,
        bool split,
        bool force
    ) {
        if (!aven_c_ast_render_pp_nodes(ctx, token, split)) {
            return false;
        }
        AvenStr token_str = aven_c_token_str(ctx->ast->tset, token);
        if (!aven_c_ast_render_write(ctx, token_str, force)) {
            return false;
        }
        if (!ctx->ppd) {
            ctx->trailing_lines = get(ctx->ast->tset.tokens, token)
                .trailing_lines;
        }
        return true;
    }

    static inline bool aven_c_ast_render_node_try_internal(
        AvenCAstRenderCtx *ctx,
        AvenCAstNodeType parent_type,
        uint32_t index,
        bool split_same,
        bool split
    ) {
        if (index == 0) {
            return true;
        }
        AvenCAstNode node = aven_c_ast_node(ctx->ast, index);
        if (
            (split_same and node.type == parent_type) or
            !aven_c_ast_render_node(ctx, parent_type, index, false)
        ) {
            if (!split) {
                return false;
            }
            uint32_t lines_written = ctx->lines_written;
            if (!aven_c_ast_render_node(ctx, parent_type, index, true)) {
                if (ctx->cursor == 0 or lines_written != ctx->lines_written) {
                    return false;
                }
                if (!aven_c_ast_render_flush_line(ctx)) {
                    return false;
                }
                ctx->indent += 1;
                if (!aven_c_ast_render_node(ctx, parent_type, index, false)) {
                    if (!aven_c_ast_render_node(ctx, parent_type, index, true)) {
                        return false;
                    }
                }
                ctx->indent -= 1;
            }
        }
        return true;
    }

    static inline bool aven_c_ast_render_node_surround_try_internal(
        AvenCAstRenderCtx *ctx,
        AvenCAstNodeType parent_type,
        uint32_t index,
        uint32_t open_token,
        uint32_t close_token,
        bool split_same,
        bool split
    ) {
        if (index == 0) {
            return true;
        }
        AvenCAstNode node = aven_c_ast_node(ctx->ast, index);
        if (
            !aven_c_ast_render_token_try_internal(ctx, open_token, split, split)
        ) {
            return false;
        }
        AvenCAstRenderCtxState state = aven_c_ast_render_save(ctx);
        if (
            (split_same and node.type == parent_type) or
            !aven_c_ast_render_node(ctx, parent_type, index, false) or
            !aven_c_ast_render_token_try_internal(
                ctx,
                close_token,
                false,
                false
            )
        ) {
            if (!split) {
                return false;
            }
            aven_c_ast_render_restore(ctx, state);
            ctx->indent += 1;
            if (!aven_c_ast_render_flush_line(ctx)) {
                return false;
            }
            if (!aven_c_ast_render_node(ctx, parent_type, index, false)) {
                if (!aven_c_ast_render_node(ctx, parent_type, index, true)) {
                    return false;
                }
            }
            if (!aven_c_ast_render_flush_line(ctx)) {
                return false;
            }
            if (!aven_c_ast_render_pp_nodes(ctx, close_token, split)) {
                return false;
            }
            ctx->indent -= 1;
            if (
                !aven_c_ast_render_token_try_internal(
                    ctx,
                    close_token,
                    false,
                    false
                )
            ) {
                return false;
            }
        }
        return true;
    }

    #define aven_c_ast_render_node_try(c, t, n, p, s, l) do { \
            if (!aven_c_ast_render_node_try_internal(c, t, n, p, s)) { \
                aven_c_ast_render_restore(c, l); \
                return false; \
            } \
        } while (0)
    #define aven_c_ast_render_node_surround_try(c, t, n, e, f, p, s, l) do { \
            if ( \
                !aven_c_ast_render_node_surround_try_internal( \
                    c, \
                    t, \
                    n, \
                    e, \
                    f, \
                    p, \
                    s \
                ) \
            ) { \
                aven_c_ast_render_restore(c, l); \
                return false; \
            } \
        } while (0)

    static inline bool aven_c_ast_render_data(
        AvenCAstRenderCtx *ctx,
        AvenCAstNodeType parent_type,
        uint32_t index,
        AvenStr sep,
        bool pre_space,
        bool trailing_sep,
        bool split
    );

    static inline bool aven_c_ast_render_data_try_internal(
        AvenCAstRenderCtx *ctx,
        AvenCAstNodeType parent_type,
        uint32_t index,
        AvenStr sep,
        bool trailing_sep,
        bool split
    ) {
        if (
            !aven_c_ast_render_data(
                ctx,
                parent_type,
                index,
                sep,
                false,
                trailing_sep,
                false
            )
        ) {
            if (!split) {
                return false;
            }
            uint32_t lines_written = ctx->lines_written;
            if (
                !aven_c_ast_render_data(
                    ctx,
                    parent_type,
                    index,
                    sep,
                    false,
                    trailing_sep,
                    true
                )
            ) {
                if (ctx->cursor == 0 or lines_written != ctx->lines_written) {
                    return false;
                }
                if (!aven_c_ast_render_flush_line(ctx)) {
                    return false;
                }
                ctx->indent += 1;
                if (
                    !aven_c_ast_render_data(
                        ctx,
                        parent_type,
                        index,
                        sep,
                        false,
                        trailing_sep,
                        false
                    )
                ) {
                    if (
                        !aven_c_ast_render_data(
                            ctx,
                            parent_type,
                            index,
                            sep,
                            false,
                            trailing_sep,
                            true
                        )
                    ) {
                        return false;
                    }
                }
                ctx->indent -= 1;
            }
        }
        return true;
    }

    #define aven_c_ast_render_data_try(c, t, n, p, b, s, l) do { \
            if (!aven_c_ast_render_data_try_internal(c, t, n, p, b, s)) { \
                aven_c_ast_render_restore(c, l); \
                return false; \
            } \
        } while (0)
    #define aven_c_ast_render_data_try_seq(c, t, n, s, l) do { \
            if (!aven_c_ast_render_data_seq(c, t, n, s)) { \
                aven_c_ast_render_restore(c, l); \
                return false; \
            } \
        } while (0)

    static inline bool aven_c_ast_render_whitespace(AvenCAstRenderCtx *ctx) {
        if (ctx->ppd) {
            return true;
        }
        if (ctx->trailing_lines > 1) {
            if (!aven_c_ast_render_flush_line(ctx)) {
                return false;
            }
            aven_io_writer_push(ctx->writer, slice_as_bytes(ctx->newline_str));
        }
        return true;
    }

    static inline bool aven_c_ast_render_pp_nodes(
        AvenCAstRenderCtx *ctx,
        uint32_t token_index,
        bool split
    ) {
        if (ctx->io_error != 0) {
            return false;
        }
        if (ctx->ppd) {
            return true;
        }
        uint32_t may_split = split or ctx->cursor == 0;
        for (uint32_t i = ctx->pp_cursor; i < token_index; i += 1) {
            AvenCToken token = get(ctx->ast->tset.tokens, i);
            if (
                token.type != AVEN_C_TOKEN_TYPE_CMT and
                token.type != AVEN_C_TOKEN_TYPE_PPD
            ) {
                continue;
            }
            if (!may_split) {
                return false;
            }
            ctx->pp_cursor = token_index;
            aven_c_ast_render_flush_line(ctx);
            if (
                !aven_c_ast_render_node(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_NONE,
                    get(ctx->ast->pp_nodes, i),
                    true
                )
            ) {
                return false;
            }
            ctx->trailing_lines = token.trailing_lines;
            if (!aven_c_ast_render_flush_line(ctx)) {
                return false;
            }
            if (!aven_c_ast_render_whitespace(ctx)) {
                return false;
            }
        }
        ctx->pp_cursor = token_index;
        return true;
    }

    static inline bool aven_c_ast_render_data(
        AvenCAstRenderCtx *ctx,
        AvenCAstNodeType parent_type,
        uint32_t index,
        AvenStr sep,
        bool pre_space,
        bool trailing_sep,
        bool split
    ) {
        if (ctx->io_error != 0) {
            return false;
        }
        if (index == 0) {
            return true;
        }
        AvenCAstRenderCtxState state = aven_c_ast_render_save(ctx);
        AvenCAstDataSlice data = aven_c_ast_data_get(ctx->ast, index);
        if (data.len == 0) {
            return true;
        }
        for (uint32_t i = 0; i < data.len; i += 1) {
            uint32_t node = get(data, i);
            aven_c_ast_render_node_try(
                ctx,
                parent_type,
                node,
                false,
                split,
                state
            );
            if (i + 1 < data.len) {
                if (!split and pre_space and node != 0) {
                    aven_c_ast_render_space_try(ctx, false, state);
                }
                if (!aven_c_ast_render_write(ctx, sep, split)) {
                    aven_c_ast_render_restore(ctx, state);
                    return false;
                }
                if (get(data, i + 1) != 0) {
                    aven_c_ast_render_space_try(ctx, split, state);
                    if (split) {
                        if (!aven_c_ast_render_whitespace(ctx)) {
                            return false;
                        }
                    }
                }
            } else if (split and trailing_sep) {
                if (!aven_c_ast_render_write(ctx, sep, split)) {
                    aven_c_ast_render_restore(ctx, state);
                    return false;
                }
            }
        }
        return true;
    }

    static inline bool aven_c_ast_render_data_seq(
        AvenCAstRenderCtx *ctx,
        AvenCAstNodeType parent_type,
        uint32_t index,
        bool split
    ) {
        if (ctx->io_error != 0) {
            return false;
        }
        if (index == 0) {
            return true;
        }
        AvenCAstRenderCtxState state = aven_c_ast_render_save(ctx);
        AvenCAstDataSlice data = aven_c_ast_data_get(ctx->ast, index);
        if (data.len == 0) {
            return true;
        }
        for (uint32_t i = 0; i < data.len; i += 1) {
            uint32_t entry = get(data, i);
            if (!aven_c_ast_render_node(ctx, parent_type, entry, false)) {
                if (!split) {
                    return false;
                }
                uint32_t lines_written = ctx->lines_written;
                if (!aven_c_ast_render_node(ctx, parent_type, entry, true)) {
                    if (ctx->cursor == 0 or lines_written != ctx->lines_written) {
                        return false;
                    }
                    if (!aven_c_ast_render_flush_line(ctx)) {
                        return false;
                    }
                    if (!aven_c_ast_render_node(ctx, parent_type, entry, false)) {
                        if (
                            !aven_c_ast_render_node(
                                ctx,
                                parent_type,
                                entry,
                                true
                            )
                        ) {
                            return false;
                        }
                    }
                }
            }
            if (i + 1 < data.len) {
                aven_c_ast_render_space_try(ctx, false, state);
            }
        }
        return true;
    }

    #define aven_c_ast_render_data_surround_try( \
            ctx, \
            parent_type, \
            index, \
            open_token, \
            close_token, \
            sep, \
            pre_space, \
            spaces, \
            trailing_sep, \
            split, \
            force_split, \
            state \
        ) do { \
            if ( \
                !aven_c_ast_render_data_surround_try_internal( \
                    ctx, \
                    parent_type, \
                    index, \
                    open_token, \
                    close_token, \
                    sep, \
                    pre_space, \
                    spaces, \
                    trailing_sep, \
                    split, \
                    force_split \
                ) \
            ) { \
                aven_c_ast_render_restore(ctx, state); \
                return false; \
            } \
        } while (0)

    static inline bool aven_c_ast_render_data_surround_try_internal(
        AvenCAstRenderCtx *ctx,
        AvenCAstNodeType parent_type,
        uint32_t index,
        uint32_t open_token,
        uint32_t close_token,
        AvenStr sep,
        bool pre_space,
        bool spaces,
        bool trailing_sep,
        bool split,
        bool force_split
    ) {
        if (ctx->io_error != 0) {
            return false;
        }
        AvenCAstRenderCtxState state = aven_c_ast_render_save(ctx);
        if (
            force_split or
            !aven_c_ast_render_token_try_internal(ctx, open_token, false, false) or
            (spaces and !aven_c_ast_render_write(ctx, aven_str(" "), true)) or
            !aven_c_ast_render_data(
                ctx,
                parent_type,
                index,
                sep,
                pre_space,
                trailing_sep,
                false
            ) or
            (spaces and !aven_c_ast_render_write(ctx, aven_str(" "), true)) or
            !aven_c_ast_render_token_try_internal(
                ctx,
                close_token,
                false,
                false
            )
        ) {
            aven_c_ast_render_restore(ctx, state);
            if (!split) {
                return false;
            }
            aven_c_ast_render_token_force_try(ctx, open_token, true, state);
            if (!aven_c_ast_render_flush_line(ctx)) {
                return false;
            }
            ctx->indent += 1;
            if (
                !aven_c_ast_render_data(
                    ctx,
                    parent_type,
                    index,
                    sep,
                    pre_space,
                    trailing_sep,
                    true
                )
            ) {
                return false;
            }
            if (!aven_c_ast_render_pp_nodes(ctx, close_token, split)) {
                aven_c_ast_render_restore(ctx, state);
                return false;
            }
            ctx->indent -= 1;
            if (!aven_c_ast_render_flush_line(ctx)) {
                return false;
            }
            aven_c_ast_render_token_try(ctx, close_token, true, state);
        }
        return true;
    }

    static inline bool aven_c_ast_render_statement_list(
        AvenCAstRenderCtx *ctx,
        AvenCAstNodeType parent_type,
        uint32_t index,
        AvenCAstRenderCtxState state
    ) {
        AvenCAstDataSlice data = aven_c_ast_data_get(ctx->ast, index);
        for (uint32_t i = 0; i < data.len; i += 1) {
            aven_c_ast_render_node_try(
                ctx,
                parent_type,
                get(data, i),
                false,
                true,
                state
            );
            if (!aven_c_ast_render_flush_line(ctx)) {
                return false;
            }
            if (i + 1 < data.len) {
                if (!aven_c_ast_render_whitespace(ctx)) {
                    return false;
                }
            }
        }
        return true;
    }

    static inline bool aven_c_ast_render_node(
        AvenCAstRenderCtx *ctx,
        AvenCAstNodeType parent_type,
        uint32_t index,
        bool split
    ) {
        if (ctx->io_error != 0) {
            return false;
        }
        if (index == 0) {
            return true;
        }
        AvenCAstRenderCtxState state = aven_c_ast_render_save(ctx);
        AvenCAstNode node = aven_c_ast_node(ctx->ast, index);
        switch (node.type) {
            case AVEN_C_AST_NODE_TYPE_PREPROCESSOR_PASTE: {
                aven_c_ast_render_token_try(ctx, node.token, split, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_PREPROCESSOR_MERGE: {
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_token_force_try(ctx, node.token, split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_PREPROCESSOR_HEADER: {
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                for (uint32_t i = 0; i < tokens.len; i += 1) {
                    aven_c_ast_render_token_try(
                        ctx,
                        get(tokens, i),
                        split,
                        state
                    );
                }
                break;
            }
            case AVEN_C_AST_NODE_TYPE_PREPROCESSOR_DEFINE_FN: {
                uint32_t lines_written = ctx->lines_written;
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_token_try(ctx, get(tokens, 0), split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                bool indent = lines_written != ctx->lines_written;
                if (indent) {
                    ctx->indent += 1;
                }
                aven_c_ast_render_data_surround_try(
                    ctx,
                    node.type,
                    node.rhs,
                    get(tokens, 1),
                    get(tokens, 2),
                    aven_str(","),
                    false,
                    false,
                    false,
                    split,
                    false,
                    state
                );
                if (indent) {
                    ctx->indent -= 1;
                }
                break;
            }
            case AVEN_C_AST_NODE_TYPE_PREPROCESSOR_DEFINE: {
                aven_c_ast_render_token_try(ctx, node.token, split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_PREPROCESSOR_DIRECTIVE: {
                ctx->ppd = true;
                uint32_t indent = ctx->indent;
                ctx->indent = 0;
                AvenStr pp_str = aven_str("");
                AvenCAstNode dir_node = aven_c_ast_node(ctx->ast, node.lhs);
                if (
                    dir_node.type == AVEN_C_AST_NODE_TYPE_IDENTIFIER or
                    dir_node.type == AVEN_C_AST_NODE_TYPE_KEYWORD
                ) {
                    pp_str = aven_c_token_str(ctx->ast->tset, dir_node.token);
                }
                bool elif = false;
                if (
                    aven_str_equals(pp_str, aven_str("elif")) or
                    aven_str_equals(pp_str, aven_str("else"))
                ) {
                    if (indent == 0 and ctx->pp_indent > 0) {
                        elif = true;
                    }
                } else if (aven_str_equals(pp_str, aven_str("endif"))) {
                    if (indent == 0 and ctx->pp_indent > 0) {
                        ctx->pp_indent -= 1;
                    }
                }
                if (elif) {
                    ctx->pp_indent -= 1;
                }
                aven_c_ast_render_token_try(ctx, node.token, split, state);
                ctx->indent += 1;
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                ctx->indent -= 1;
                if (elif) {
                    ctx->pp_indent += 1;
                } else if (
                    aven_str_equals(pp_str, aven_str("if")) or
                    aven_str_equals(pp_str, aven_str("ifdef")) or
                    aven_str_equals(pp_str, aven_str("ifndef"))
                ) {
                    if (indent == 0) {
                        ctx->pp_indent += 1;
                    }
                }
                ctx->indent = indent;
                ctx->ppd = false;
                break;
            }
            case AVEN_C_AST_NODE_TYPE_COMMENT: {
                for (uint32_t j = 0; j < ctx->indent + ctx->pp_indent; j += 1) {
                    AvenIoResult res = aven_io_writer_push(
                        ctx->writer,
                        slice_as_bytes(ctx->indent_str)
                    );
                    if (res.error != 0) {
                        ctx->io_error = res.error;
                        return false;
                    }
                    if (res.payload != ctx->indent_str.len) {
                        return false;
                    }
                }
                AvenStr tstr = aven_c_token_str(ctx->ast->tset, node.token);
                AvenIoResult res = aven_io_writer_push(
                    ctx->writer,
                    slice_as_bytes(tstr)
                );
                if (res.error != 0) {
                    ctx->io_error = res.error;
                    return false;
                }
                if (res.payload != tstr.len) {
                    return false;
                }
                res = aven_io_writer_push(
                    ctx->writer,
                    slice_as_bytes(ctx->newline_str)
                );
                if (res.error != 0) {
                    ctx->io_error = res.error;
                    return false;
                }
                if (res.payload != ctx->newline_str.len) {
                    return false;
                }
                break;
            }
            case AVEN_C_AST_NODE_TYPE_ASSIGN_EXPR: {
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_token_force_try(ctx, node.token, split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_EXPR: {
                aven_c_ast_render_data_try(
                    ctx,
                    node.type,
                    node.lhs,
                    aven_str(","),
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_SHIFT_EXPR:
            case AVEN_C_AST_NODE_TYPE_EQUAL_EXPR:
            case AVEN_C_AST_NODE_TYPE_RELATE_EXPR: {
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    split,
                    split,
                    state
                );
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_token_force_try(ctx, node.token, split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    split,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_LOGICAL_OR_EXPR:
            case AVEN_C_AST_NODE_TYPE_LOGICAL_AND_EXPR:
            case AVEN_C_AST_NODE_TYPE_OR_EXPR:
            case AVEN_C_AST_NODE_TYPE_XOR_EXPR:
            case AVEN_C_AST_NODE_TYPE_AND_EXPR:
            case AVEN_C_AST_NODE_TYPE_ADD_EXPR:
            case AVEN_C_AST_NODE_TYPE_MULTIPLY_EXPR: {
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    split,
                    split,
                    state
                );
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_token_force_try(ctx, node.token, split, state);
                aven_c_ast_render_space_try(ctx, split, state);
                if (split) {
                    ctx->indent += 1;
                }
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    split,
                    split,
                    state
                );
                if (split) {
                    ctx->indent -= 1;
                }
                break;
            }
            case AVEN_C_AST_NODE_TYPE_ANYTOKEN:
            case AVEN_C_AST_NODE_TYPE_TYPE_QUALIFIER:
            case AVEN_C_AST_NODE_TYPE_TYPE_SPECIFIER:
            case AVEN_C_AST_NODE_TYPE_STRING_LITERAL:
            case AVEN_C_AST_NODE_TYPE_CONSTANT:
            case AVEN_C_AST_NODE_TYPE_PUNCTUATOR:
            case AVEN_C_AST_NODE_TYPE_KEYWORD:
            case AVEN_C_AST_NODE_TYPE_IDENTIFIER: {
                aven_c_ast_render_token_try(ctx, node.token, split, state);
                break;
            }
            case AVEN_C_AST_NODE_TYPE_PRIMARY_EXPR: {
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_node_surround_try(
                    ctx,
                    node.type,
                    node.lhs,
                    get(tokens, 0),
                    get(tokens, 1),
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_STATIC_ASSERT_DECLARATION:
            case AVEN_C_AST_NODE_TYPE_DIR_ABS_DECLARATOR_BRAC:
            case AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR_BRAC:
            case AVEN_C_AST_NODE_TYPE_DIR_ABS_DECLARATOR_PARAM_TYPE_LIST:
            case AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR_ID_LIST:
            case AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR_PARAM_TYPE_LIST:
            case AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_PAREN:
            case AVEN_C_AST_NODE_TYPE_ATTRIBUTE:
            case AVEN_C_AST_NODE_TYPE_MACRO_INVOCATION: {
                uint32_t lines_written = ctx->lines_written;
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                bool indent = lines_written != ctx->lines_written;
                if (indent) {
                    ctx->indent += 1;
                }
                aven_c_ast_render_data_surround_try(
                    ctx,
                    node.type,
                    node.rhs,
                    get(tokens, 0),
                    get(tokens, 1),
                    aven_str(","),
                    false,
                    false,
                    false,
                    split,
                    false,
                    state
                );
                if (indent) {
                    ctx->indent -= 1;
                }
                break;
            }
            case AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR_ATTRIBUTE: {
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_STRING_CONSTANT: {
                uint32_t lines_written = ctx->lines_written;
                bool indent = (
                    parent_type == AVEN_C_AST_NODE_TYPE_ASSIGN_EXPR or
                    parent_type == AVEN_C_AST_NODE_TYPE_INIT_DECLARATOR or
                    parent_type == AVEN_C_AST_NODE_TYPE_DESIGNATION or
                    parent_type == AVEN_C_AST_NODE_TYPE_ENUMERATOR or
                    parent_type == AVEN_C_AST_NODE_TYPE_RETURN_STATEMENT
                );
                if (split and indent) {
                    ctx->indent += 1;
                }
                aven_c_ast_render_data_try(
                    ctx,
                    node.type,
                    node.lhs,
                    aven_str(""),
                    false,
                    split,
                    state
                );
                if (split and indent) {
                    ctx->indent -= 1;
                }
                bool trailing_nl = lines_written != ctx->lines_written and
                    (
                        parent_type == AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_PAREN or
                        parent_type == AVEN_C_AST_NODE_TYPE_INITIALIZER_LIST
                    );
                if (trailing_nl) {
                    if (!aven_c_ast_render_flush_line(ctx)) {
                        return false;
                    }
                }
                break;
            }
            case AVEN_C_AST_NODE_TYPE_INITIALIZER_LIST: {
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_data_surround_try(
                    ctx,
                    node.type,
                    node.lhs,
                    get(tokens, 0),
                    get(tokens, 1),
                    aven_str(","),
                    false,
                    true,
                    true,
                    split,
                    false,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_DOT_DESIGNATOR: {
                aven_c_ast_render_token_force_try(ctx, node.token, split, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_ARRAY_DESIGNATOR: {
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_node_surround_try(
                    ctx,
                    node.type,
                    node.lhs,
                    get(tokens, 0),
                    get(tokens, 1),
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_DESIGNATION: {
                aven_c_ast_render_data_try_seq(
                    ctx,
                    node.type,
                    node.lhs,
                    split,
                    state
                );
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_token_force_try(ctx, node.token, split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_TYPE_NAME: {
                aven_c_ast_render_data_try_seq(
                    ctx,
                    node.type,
                    node.lhs,
                    split,
                    state
                );
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_BRAC: {
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_node_surround_try(
                    ctx,
                    node.type,
                    node.rhs,
                    get(tokens, 0),
                    get(tokens, 1),
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_BOP: {
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                aven_c_ast_render_token_force_try(ctx, node.token, split, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_UOP: {
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                aven_c_ast_render_token_force_try(ctx, node.token, split, state);
                break;
            }
            case AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_INITIALIZER: {
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_node_surround_try(
                    ctx,
                    node.type,
                    node.lhs,
                    get(tokens, 0),
                    get(tokens, 1),
                    false,
                    split,
                    state
                );
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_UNARY_EXPR: {
                aven_c_ast_render_token_try(ctx, node.token, split, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_UNARY_EXPR_SIZEOF: {
                aven_c_ast_render_token_try(ctx, node.token, split, state);
                AvenCAstNode child = aven_c_ast_node(ctx->ast, node.lhs);
                if (child.type != AVEN_C_AST_NODE_TYPE_PRIMARY_EXPR) {
                    aven_c_ast_render_space_try(ctx, false, state);
                }
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_UNARY_EXPR_FN: {
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_token_try(ctx, get(tokens, 0), split, state);
                aven_c_ast_render_node_surround_try(
                    ctx,
                    node.type,
                    node.lhs,
                    get(tokens, 1),
                    get(tokens, 2),
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_CAST_EXPR: {
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_node_surround_try(
                    ctx,
                    node.type,
                    node.lhs,
                    get(tokens, 0),
                    get(tokens, 1),
                    false,
                    split,
                    state
                );
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_CONDITIONAL_EXPR: {
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                AvenCAstDataSlice data = aven_c_ast_data_get(ctx->ast, node.rhs);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_token_force_try(
                    ctx,
                    get(tokens, 0),
                    split,
                    state
                );
                ctx->indent += 1;
                aven_c_ast_render_space_try(ctx, split, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    get(data, 0),
                    false,
                    split,
                    state
                );
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_token_force_try(
                    ctx,
                    get(tokens, 1),
                    split,
                    state
                );
                aven_c_ast_render_space_try(ctx, split, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    get(data, 1),
                    false,
                    split,
                    state
                );
                ctx->indent -= 1;
                break;
            }
            case AVEN_C_AST_NODE_TYPE_STRUCT_DECLARATION:
            case AVEN_C_AST_NODE_TYPE_DECLARATION: {
                aven_c_ast_render_data_try_seq(
                    ctx,
                    node.type,
                    node.lhs,
                    split,
                    state
                );
                if (node.rhs != 0) {
                    aven_c_ast_render_space_try(ctx, false, state);
                    aven_c_ast_render_data_try(
                        ctx,
                        node.type,
                        node.rhs,
                        aven_str(","),
                        false,
                        split,
                        state
                    );
                }
                break;
            }
            case AVEN_C_AST_NODE_TYPE_INIT_DECLARATOR: {
                uint32_t lines_written = ctx->lines_written;
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                bool indent = lines_written != ctx->lines_written;
                if (indent) {
                    ctx->indent += 1;
                }
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_token_force_try(ctx, node.token, split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                if (indent) {
                    ctx->indent -= 1;
                }
                break;
            }
            case AVEN_C_AST_NODE_TYPE_DIR_ABS_DECLARATOR:
            case AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR: {
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_node_surround_try(
                    ctx,
                    node.type,
                    node.lhs,
                    get(tokens, 0),
                    get(tokens, 1),
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_POINTER: {
                if (
                    ctx->cursor != 0 and
                    get(ctx->line, ctx->cursor - 1) != '*' and
                    get(ctx->line, ctx->cursor - 1) != ' ' and
                    get(ctx->line, ctx->cursor - 1) != '('
                ) {
                    aven_c_ast_render_space_try(ctx, false, state);
                }
                aven_c_ast_render_token_try(ctx, node.token, split, state);
                aven_c_ast_render_data_try_seq(
                    ctx,
                    node.type,
                    node.lhs,
                    split,
                    state
                );
                if (node.rhs == 0) {
                    break;
                }
                if (
                    ctx->cursor != 0 and
                    get(ctx->line, ctx->cursor - 1) != '*' and
                    get(ctx->line, ctx->cursor - 1) != ' '
                ) {
                    aven_c_ast_render_space_try(ctx, false, state);
                }
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_PREPROCESSOR_SPEC_LIST:
            case AVEN_C_AST_NODE_TYPE_PARAMETER_DECLARATION: {
                aven_c_ast_render_data_try_seq(
                    ctx,
                    node.type,
                    node.lhs,
                    split,
                    state
                );
                if (node.rhs != 0) {
                    aven_c_ast_render_space_try(ctx, false, state);
                    aven_c_ast_render_node_try(
                        ctx,
                        node.type,
                        node.rhs,
                        false,
                        split,
                        state
                    );
                }
                break;
            }
            case AVEN_C_AST_NODE_TYPE_DECLARATOR:
            case AVEN_C_AST_NODE_TYPE_ABS_DECLARATOR: {
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                if (ctx->cursor != 0 and get(ctx->line, ctx->cursor - 1) != '*') {
                    aven_c_ast_render_space_try(ctx, false, state);
                }
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_STRUCT_DECLARATOR: {
                if (node.lhs != 0) {
                    aven_c_ast_render_node_try(
                        ctx,
                        node.type,
                        node.lhs,
                        false,
                        split,
                        state
                    );
                    aven_c_ast_render_space_try(ctx, false, state);
                }
                aven_c_ast_render_token_force_try(ctx, node.token, split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_ATOMIC_SPECIFIER:
            case AVEN_C_AST_NODE_TYPE_ALIGNMENT_SPECIFIER: {
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_token_try(ctx, get(tokens, 0), split, state);
                aven_c_ast_render_node_surround_try(
                    ctx,
                    node.type,
                    node.lhs,
                    get(tokens, 1),
                    get(tokens, 2),
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_STRUCT_SPECIFIER: {
                if (node.rhs == 0) {
                    aven_c_ast_render_token_try(ctx, node.token, split, state);
                    aven_c_ast_render_space_try(ctx, false, state);
                    aven_c_ast_render_node_try(
                        ctx,
                        node.type,
                        node.lhs,
                        false,
                        split,
                        state
                    );
                    break;
                }
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_token_try(ctx, get(tokens, 0), split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                if (node.lhs != 0) {
                    aven_c_ast_render_node_try(
                        ctx,
                        node.type,
                        node.lhs,
                        false,
                        split,
                        state
                    );
                    aven_c_ast_render_space_try(ctx, false, state);
                }
                aven_c_ast_render_data_surround_try(
                    ctx,
                    node.type,
                    node.rhs,
                    get(tokens, 1),
                    get(tokens, 2),
                    aven_str(""),
                    false,
                    true,
                    true,
                    split,
                    false,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_ENUMERATOR: {
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_token_force_try(ctx, node.token, split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_ENUM_SPECIFIER: {
                if (node.rhs == 0) {
                    aven_c_ast_render_token_try(ctx, node.token, split, state);
                    aven_c_ast_render_space_try(ctx, false, state);
                    aven_c_ast_render_node_try(
                        ctx,
                        node.type,
                        node.lhs,
                        false,
                        split,
                        state
                    );
                    break;
                }
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_token_try(ctx, get(tokens, 0), split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                if (node.lhs != 0) {
                    aven_c_ast_render_node_try(
                        ctx,
                        node.type,
                        node.lhs,
                        false,
                        split,
                        state
                    );
                    aven_c_ast_render_space_try(ctx, false, state);
                }
                aven_c_ast_render_data_surround_try(
                    ctx,
                    node.type,
                    node.rhs,
                    get(tokens, 1),
                    get(tokens, 2),
                    aven_str(","),
                    false,
                    true,
                    true,
                    split,
                    true,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_IF_STATEMENT: {
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_token_try(ctx, get(tokens, 0), split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_surround_try(
                    ctx,
                    node.type,
                    node.lhs,
                    get(tokens, 1),
                    get(tokens, 2),
                    false,
                    split,
                    state
                );
                AvenCAstDataSlice data = aven_c_ast_data_get(ctx->ast, node.rhs);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    get(data, 0),
                    false,
                    split,
                    state
                );
                if (data.len > 1) {
                    aven_c_ast_render_space_try(ctx, false, state);
                    aven_c_ast_render_node_try(
                        ctx,
                        node.type,
                        get(data, 1),
                        false,
                        split,
                        state
                    );
                } else {
                    if (!aven_c_ast_render_flush_line(ctx)) {
                        return false;
                    }
                }
                break;
            }
            case AVEN_C_AST_NODE_TYPE_IF_ELSE_STATEMENT: {
                aven_c_ast_render_token_try(ctx, node.token, split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_WHILE_STATEMENT:
            case AVEN_C_AST_NODE_TYPE_SWITCH_STATEMENT: {
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_token_try(ctx, get(tokens, 0), split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_surround_try(
                    ctx,
                    node.type,
                    node.lhs,
                    get(tokens, 1),
                    get(tokens, 2),
                    false,
                    split,
                    state
                );
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_SWITCH_CASE: {
                aven_c_ast_render_token_try(ctx, node.token, split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_DO_STATEMENT: {
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_token_try(ctx, get(tokens, 0), split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_token_try(ctx, get(tokens, 1), split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_surround_try(
                    ctx,
                    node.type,
                    node.rhs,
                    get(tokens, 2),
                    get(tokens, 3),
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_FOR_STATEMENT: {
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_token_try(ctx, get(tokens, 0), split, state);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_data_surround_try(
                    ctx,
                    node.type,
                    node.lhs,
                    get(tokens, 1),
                    get(tokens, 2),
                    aven_str(";"),
                    false,
                    false,
                    false,
                    split,
                    false,
                    state
                );
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_RETURN_STATEMENT:
            case AVEN_C_AST_NODE_TYPE_GOTO_STATEMENT: {
                aven_c_ast_render_token_try(ctx, node.token, split, state);
                if (node.rhs == 0) {
                    break;
                }
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_LABELED_STATEMENT: {
                if (!split) {
                    return false;
                }
                if (!aven_c_ast_render_flush_line(ctx)) {
                    return false;
                }
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                aven_c_ast_render_token_force_try(ctx, node.token, split, state);
                if (node.rhs == 0) {
                    break;
                }
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.rhs,
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_ASM_STATEMENT: {
                aven_c_ast_render_data_try_seq(
                    ctx,
                    node.type,
                    node.lhs,
                    split,
                    state
                );
                aven_c_ast_render_space_try(ctx, false, state);
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_data_surround_try(
                    ctx,
                    node.type,
                    node.rhs,
                    get(tokens, 0),
                    get(tokens, 1),
                    aven_str(":"),
                    true,
                    false,
                    false,
                    split,
                    false,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_COMPOUND_STATEMENT: {
                AvenCAstDataSlice tokens = aven_c_ast_data_get(
                    ctx->ast,
                    node.token
                );
                aven_c_ast_render_token_force_try(
                    ctx,
                    get(tokens, 0),
                    split,
                    state
                );
                if (node.lhs != 0) {
                    ctx->indent += 1;
                    if (!split) {
                        aven_c_ast_render_restore(ctx, state);
                        return false;
                    }
                    if (!aven_c_ast_render_flush_line(ctx)) {
                        return false;
                    }
                    if (
                        !aven_c_ast_render_statement_list(
                            ctx,
                            node.type,
                            node.lhs,
                            state
                        )
                    ) {
                        return false;
                    }
                    if (!aven_c_ast_render_flush_line(ctx)) {
                        return false;
                    }
                    // render pp tokens between last line and bracket w/indent
                    if (!aven_c_ast_render_pp_nodes(ctx, get(tokens, 1), split)) {
                        aven_c_ast_render_restore(ctx, state);
                        return false;
                    }
                    ctx->indent -= 1;
                }
                aven_c_ast_render_token_try(ctx, get(tokens, 1), split, state);
                if (
                    parent_type == AVEN_C_AST_NODE_TYPE_IF_STATEMENT or
                    parent_type == AVEN_C_AST_NODE_TYPE_DO_STATEMENT
                ) {
                    break;
                }
                if (!aven_c_ast_render_flush_line(ctx)) {
                    return false;
                }
                break;
            }
            case AVEN_C_AST_NODE_TYPE_FUNCTION_DEFINITION: {
                aven_c_ast_render_data_try_seq(
                    ctx,
                    node.type,
                    node.lhs,
                    split,
                    state
                );
                AvenCAstDataSlice data = aven_c_ast_data_get(ctx->ast, node.rhs);
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    get(data, 0),
                    false,
                    split,
                    state
                );
                aven_c_ast_render_space_try(ctx, false, state);
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    get(data, 1),
                    false,
                    split,
                    state
                );
                break;
            }
            case AVEN_C_AST_NODE_TYPE_TERMINATED_LINE: {
                aven_c_ast_render_node_try(
                    ctx,
                    node.type,
                    node.lhs,
                    false,
                    split,
                    state
                );
                aven_c_ast_render_token_force_try(ctx, node.token, split, state);
                break;
            }
            case AVEN_C_AST_NODE_TYPE_TRANSLATION_UNIT: {
                if (!split) {
                    aven_c_ast_render_restore(ctx, state);
                    return false;
                }
                if (!aven_c_ast_render_flush_line(ctx)) {
                    return false;
                }
                if (
                    !aven_c_ast_render_statement_list(
                        ctx,
                        node.type,
                        node.lhs,
                        state
                    )
                ) {
                    return false;
                }
                break;
            }
            case AVEN_C_AST_NODE_TYPE_NONE: {
                break;
            }
        }

        return true;
    }

    static inline AvenCAstRenderResult aven_c_ast_render_error(
        AvenCAstRenderCtx *ctx,
        AvenArena *arena
    ) {
        AvenCTokenLoc loc = aven_c_token_loc(ctx->ast->tset, ctx->pp_cursor);
        if (ctx->io_error == 0) {
            return (AvenCAstRenderResult){
                .error = AVEN_C_AST_RENDER_ERROR_FMT,
                .io_error = 0,
                .msg = aven_fmt(
                    arena,
                    "src line {} must exceed {} char limit",
                    aven_fmt_uint(loc.line),
                    aven_fmt_uint(ctx->line_len)
                ),
            };
        }
        return (AvenCAstRenderResult){
            .error = AVEN_C_AST_RENDER_ERROR_IO,
            .io_error = ctx->io_error,
            .msg = aven_fmt(
                arena,
                "writer error code {} while formatting src line {}",
                aven_fmt_int(ctx->io_error),
                aven_fmt_uint(loc.line)
            ),
        };
    }

    static inline AvenCAstRenderResult aven_c_ast_render(
        AvenCAst *ast,
        AvenIoWriter *writer,
        size_t line_len,
        AvenStr newline_str,
        AvenStr indent_str,
        AvenArena *arena
    ) {
        assert(line_len > 1);
        AvenArena temp_arena = *arena;
        AvenCAstRenderCtx ctx = {
            .ast = ast,
            .writer = writer,
            .line = aven_arena_create_slice(
                char,
                &temp_arena,
                line_len + newline_str.len + 8
            ),
            .line_len = line_len,
            .newline_str = newline_str,
            .indent_str = indent_str,
        };
        if (
            !aven_c_ast_render_node(
                &ctx,
                AVEN_C_AST_NODE_TYPE_NONE,
                ctx.ast->root,
                false
            ) and
            !aven_c_ast_render_node(
                &ctx,
                AVEN_C_AST_NODE_TYPE_NONE,
                ctx.ast->root,
                true
            )
        ) {
            return aven_c_ast_render_error(&ctx, arena);
        }
        if (!aven_c_ast_render_flush_line(&ctx)) {
            return aven_c_ast_render_error(&ctx, arena);
        }
        if (
            !aven_c_ast_render_pp_nodes(
                &ctx,
                (uint32_t)(ast->tset.tokens.len - 1),
                true
            )
        ) {
            return aven_c_ast_render_error(&ctx, arena);
        }
        return (AvenCAstRenderResult){ 0 };
    }

    typedef Optional(uint32_t) AvenCConfigOpt;

    static inline AvenCConfigOpt aven_c_parse_config_comment(
        AvenStr cmt,
        AvenArena temp_arena
    ) {
        if (cmt.len < 10) {
            return (AvenCConfigOpt){ 0 };
        }
        AvenStr sig_cmt = aven_str_tail(cmt, 2);
        AvenStr src = aven_arena_create_slice(
            char,
            &temp_arena,
            sig_cmt.len + 1
        );
        slice_copy(src, sig_cmt);
        get(src, sig_cmt.len) = 0;
        AvenCTokenSet tset = aven_c_lex(src, &temp_arena);
        uint32_t i = 1;
        AvenCToken token = { 0 };
        for (; i < tset.tokens.len; i += 1) {
            token = get(tset.tokens, i);
            if (token.type == AVEN_C_TOKEN_TYPE_NONE) {
                break;
            }
            if (token.type == AVEN_C_TOKEN_TYPE_ID) {
                break;
            }
        }
        if (token.type != AVEN_C_TOKEN_TYPE_ID) {
            return (AvenCConfigOpt){ 0 };
        }
        AvenStr str = aven_c_token_str(tset, i);
        if (!aven_str_equals(str, aven_str("aven"))) {
            return (AvenCConfigOpt){ 0 };
        }
        i += 1;
        token = get(tset.tokens, i);
        if (token.type != AVEN_C_TOKEN_TYPE_ID) {
            return (AvenCConfigOpt){ 0 };
        }
        str = aven_c_token_str(tset, i);
        if (!aven_str_equals(str, aven_str("fmt"))) {
            return (AvenCConfigOpt){ 0 };
        }
        i += 1;
        token = get(tset.tokens, i);

        if (token.type != AVEN_C_TOKEN_TYPE_ID) {
            return (AvenCConfigOpt){ 0 };
        }
        str = aven_c_token_str(tset, i);
        if (!aven_str_equals(str, aven_str("columns"))) {
            return (AvenCConfigOpt){ 0 };
        }
        i += 1;
        token = get(tset.tokens, i);
        if (token.type != AVEN_C_TOKEN_TYPE_PNC) {
            return (AvenCConfigOpt){ 0 };
        }
        str = aven_c_token_str(tset, i);
        if (!aven_str_equals(str, aven_str(":"))) {
            return (AvenCConfigOpt){ 0 };
        }
        i += 1;
        token = get(tset.tokens, i);
        if (token.type != AVEN_C_TOKEN_TYPE_NUM) {
            return (AvenCConfigOpt){ 0 };
        }
        str = aven_c_token_str(tset, i);
        AvenFmtParseIntResult ires = aven_fmt_parse_int_decimal(str);
        if (ires.error != 0 or ires.payload < 0) {
            return (AvenCConfigOpt){ 0 };
        }
        return (AvenCConfigOpt){
            .value = (uint32_t)ires.payload,
            .valid = true,
        };
    }

    #define AVEN_C_MAX_COLUMN_WIDTH ((size_t)1024 * (size_t)1024)

    typedef enum {
        AVEN_C_FMT_ERROR_NONE = 0,
        AVEN_C_FMT_ERROR_WRITE,
        AVEN_C_FMT_ERROR_PARSE,
        AVEN_C_FMT_ERROR_RENDER,
    } AvenCFmtError;
    typedef struct {
        AvenCFmtError error;
        int io_error;
        AvenStr msg;
    } AvenCFmtResult;

    static inline AvenCFmtResult aven_c_fmt(
        AvenStr src,
        AvenIoWriter *writer,
        size_t column_width,
        size_t depth,
        AvenArena *arena
    ) {
        if (get(src, src.len - 1) != 0) {
            return (AvenCFmtResult){
                .error = AVEN_C_FMT_ERROR_PARSE,
                .msg = aven_str("source string missing null terminator"),
            };
        }
        AvenArena temp_arena = *arena;
        AvenCTokenSet tset = aven_c_lex(src, &temp_arena);
        for (uint32_t i = 1; i < tset.tokens.len; i += 1) {
            AvenCToken token = get(tset.tokens, i);
            if (token.type == AVEN_C_TOKEN_TYPE_NONE) {
                break;
            }
            if (token.type != AVEN_C_TOKEN_TYPE_CMT) {
                continue;
            }
            AvenStr token_str = aven_c_token_str(tset, i);
            AvenCConfigOpt cfg = aven_c_parse_config_comment(
                token_str,
                temp_arena
            );
            if (cfg.valid) {
                column_width = cfg.value;
                break;
            }
        }
        if (depth == 0) {
            depth = AVEN_C_MAX_PARSE_DEPTH;
        }
        uint32_t max_depth = (uint32_t)min(
            (size_t)AVEN_C_MAX_PARSE_DEPTH,
            max((size_t)AVEN_C_MIN_PARSE_DEPTH, depth)
        );
        AvenCAstResult ast_res = aven_c_ast_parse(
            tset,
            (uint32_t)max_depth,
            &temp_arena
        );
        if (ast_res.type == AVEN_C_AST_RESULT_TYPE_ERROR) {
            return (AvenCFmtResult){
                .error = AVEN_C_FMT_ERROR_PARSE,
                .msg = aven_str_copy(ast_res.data.error, arena),
            };
        }
        if (
            column_width <= 0 or
            column_width > (int64_t)(AVEN_C_MAX_COLUMN_WIDTH)
        ) {
            column_width = (int64_t)AVEN_C_MAX_COLUMN_WIDTH;
        }
        size_t min_column_width = 32;
        if (column_width < min_column_width) {
            return (AvenCFmtResult){
                .error = AVEN_C_FMT_ERROR_RENDER,
                .msg = aven_fmt(
                    arena,
                    "column width {} is less than the minimum of {}",
                    aven_fmt_uint(column_width),
                    aven_fmt_uint(min_column_width)
                ),
            };
        }
        AvenCAst ast = ast_res.data.ast;
        AvenStr newline = aven_str("\n");
        AvenCAstRenderResult ren_res = aven_c_ast_render(
            &ast,
            writer,
            column_width,
            newline,
            aven_str("    "),
            &temp_arena
        );
        if (ren_res.error != AVEN_C_AST_RENDER_ERROR_NONE) {
            return (AvenCFmtResult){
                .error = ren_res.error == AVEN_C_AST_RENDER_ERROR_IO ?
                    AVEN_C_FMT_ERROR_WRITE :
                    AVEN_C_FMT_ERROR_RENDER,
                .io_error = ren_res.io_error,
                .msg = aven_str_copy(ren_res.msg, arena),
            };
        }
        return (AvenCFmtResult){ 0 };
    }
#endif
// AVEN_C_H
