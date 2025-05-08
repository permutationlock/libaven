#ifndef AVEN_C_H
#define AVEN_C_H

#include "../aven.h"
#include "arena.h"
#include "aven/fmt.h"
#include "io.h"
#include "str.h"

typedef enum {
    AVEN_C_KEYWORD_NONE = 0,
    AVEN_C_KEYWORD_AUTO,
    AVEN_C_KEYWORD_BREAK,
    AVEN_C_KEYWORD_CASE,
    AVEN_C_KEYWORD_CHAR,
    AVEN_C_KEYWORD_CONST,
    AVEN_C_KEYWORD_CONTINUE,
    AVEN_C_KEYWORD_DEFAULT,
    AVEN_C_KEYWORD_DO,
    AVEN_C_KEYWORD_DOUBLE,
    AVEN_C_KEYWORD_ELSE,
    AVEN_C_KEYWORD_ENUM,
    AVEN_C_KEYWORD_EXTERN,
    AVEN_C_KEYWORD_FLOAT,
    AVEN_C_KEYWORD_FOR,
    AVEN_C_KEYWORD_GOTO,
    AVEN_C_KEYWORD_IF,
    AVEN_C_KEYWORD_INLINE,
    AVEN_C_KEYWORD_INT,
    AVEN_C_KEYWORD_LONG,
    AVEN_C_KEYWORD_REGISTER,
    AVEN_C_KEYWORD_RESTRICT,
    AVEN_C_KEYWORD_RETURN,
    AVEN_C_KEYWORD_SHORT,
    AVEN_C_KEYWORD_SIGNED,
    AVEN_C_KEYWORD_SIZEOF,
    AVEN_C_KEYWORD_ALIGNOF,
    AVEN_C_KEYWORD_STATIC,
    AVEN_C_KEYWORD_STRUCT,
    AVEN_C_KEYWORD_SWITCH,
    AVEN_C_KEYWORD_TYPEDEF,
    AVEN_C_KEYWORD_UNION,
    AVEN_C_KEYWORD_UNSIGNED,
    AVEN_C_KEYWORD_VOID,
    AVEN_C_KEYWORD_VOLATILE,
    AVEN_C_KEYWORD_WHILE,
    AVEN_C_KEYWORD_BOOL,
    AVEN_C_KEYWORD_C11BOOL,
    AVEN_C_KEYWORD_ALIGNAS,
    AVEN_C_KEYWORD_NORETURN,
    AVEN_C_KEYWORD_STATIC_ASSERT,
    AVEN_C_KEYWORD_ATOMIC,
    AVEN_C_KEYWORD_COMPLEX,
    AVEN_C_KEYWORD_IMAGINARY,
    AVEN_C_KEYWORD_THREAD_LOCAL,
    AVEN_C_KEYWORD_OR,
    AVEN_C_KEYWORD_AND,
} AvenCKeyword;

static const AvenStr aven_c_keyword_data[] = {
    [AVEN_C_KEYWORD_AUTO] = aven_str_init("auto"),
    [AVEN_C_KEYWORD_BREAK] = aven_str_init("break"),
    [AVEN_C_KEYWORD_CASE] = aven_str_init("case"),
    [AVEN_C_KEYWORD_CHAR] = aven_str_init("char"),
    [AVEN_C_KEYWORD_CONST] = aven_str_init("const"),
    [AVEN_C_KEYWORD_CONTINUE] = aven_str_init("continue"),
    [AVEN_C_KEYWORD_DEFAULT] = aven_str_init("default"),
    [AVEN_C_KEYWORD_DO] = aven_str_init("do"),
    [AVEN_C_KEYWORD_DOUBLE] = aven_str_init("double"),
    [AVEN_C_KEYWORD_ELSE] = aven_str_init("else"),
    [AVEN_C_KEYWORD_ENUM] = aven_str_init("enum"),
    [AVEN_C_KEYWORD_EXTERN] = aven_str_init("extern"),
    [AVEN_C_KEYWORD_FLOAT] = aven_str_init("float"),
    [AVEN_C_KEYWORD_FOR] = aven_str_init("for"),
    [AVEN_C_KEYWORD_GOTO] = aven_str_init("goto"),
    [AVEN_C_KEYWORD_IF] = aven_str_init("if"),
    [AVEN_C_KEYWORD_INLINE] = aven_str_init("inline"),
    [AVEN_C_KEYWORD_INT] = aven_str_init("int"),
    [AVEN_C_KEYWORD_LONG] = aven_str_init("long"),
    [AVEN_C_KEYWORD_REGISTER] = aven_str_init("register"),
    [AVEN_C_KEYWORD_RESTRICT] = aven_str_init("restrict"),
    [AVEN_C_KEYWORD_RETURN] = aven_str_init("return"),
    [AVEN_C_KEYWORD_SHORT] = aven_str_init("short"),
    [AVEN_C_KEYWORD_SIGNED] = aven_str_init("signed"),
    [AVEN_C_KEYWORD_SIZEOF] = aven_str_init("sizeof"),
    [AVEN_C_KEYWORD_STATIC] = aven_str_init("static"),
    [AVEN_C_KEYWORD_STRUCT] = aven_str_init("struct"),
    [AVEN_C_KEYWORD_SWITCH] = aven_str_init("switch"),
    [AVEN_C_KEYWORD_TYPEDEF] = aven_str_init("typedef"),
    [AVEN_C_KEYWORD_UNION] = aven_str_init("union"),
    [AVEN_C_KEYWORD_UNSIGNED] = aven_str_init("unsigned"),
    [AVEN_C_KEYWORD_VOID] = aven_str_init("void"),
    [AVEN_C_KEYWORD_VOLATILE] = aven_str_init("volatile"),
    [AVEN_C_KEYWORD_WHILE] = aven_str_init("while"),
    [AVEN_C_KEYWORD_BOOL] = aven_str_init("_Bool"),
    [AVEN_C_KEYWORD_C11BOOL] = aven_str_init("bool"),
    [AVEN_C_KEYWORD_ALIGNOF] = aven_str_init("_Alignof"),
    [AVEN_C_KEYWORD_ALIGNAS] = aven_str_init("_Alignas"),
    [AVEN_C_KEYWORD_NORETURN] = aven_str_init("_Noreturn"),
    [AVEN_C_KEYWORD_STATIC_ASSERT] = aven_str_init("_Static_assert"),
    [AVEN_C_KEYWORD_ATOMIC] = aven_str_init("_Atomic"),
    [AVEN_C_KEYWORD_COMPLEX] = aven_str_init("_Complex"),
    [AVEN_C_KEYWORD_IMAGINARY] = aven_str_init("_Imaginary"),
    [AVEN_C_KEYWORD_THREAD_LOCAL] = aven_str_init("_Thread_local"),
    [AVEN_C_KEYWORD_OR] = aven_str_init("or"),
    [AVEN_C_KEYWORD_AND] = aven_str_init("and"),
};

static const AvenStrSlice aven_c_keywords = {
    .ptr = (AvenStr *)aven_c_keyword_data,
    .len = countof(aven_c_keyword_data),
};

static inline AvenCKeyword aven_c_keyword(AvenStr str) {
    for (
        AvenCKeyword i = AVEN_C_KEYWORD_NONE + 1;
        i < aven_c_keywords.len;
        i += 1
    ) {
        if (aven_str_equals(str, get(aven_c_keywords, i))) {
            return i;
        }
    }

    return AVEN_C_KEYWORD_NONE;
}

static inline AvenStr aven_c_keyword_str(AvenCKeyword kwd) {
    if (kwd == AVEN_C_KEYWORD_NONE) {
        return (AvenStr){ 0 };
    }
    return get(aven_c_keywords, kwd);
}

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
    AVEN_C_TOKEN_TYPE_OTH,
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
    [AVEN_C_TOKEN_TYPE_OTH] = aven_str_init("other"),
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
    uint32_t len;
    uint32_t trailing_lines;
} AvenCToken;
typedef Slice(AvenCToken) AvenCTokenSlice;
typedef List(AvenCToken) AvenCTokenList;

typedef struct {
    AvenStr bytes;
    AvenCTokenSlice tokens;
    AvenCTokenSlice ppd_tokens;
} AvenCTokenSet;

static inline AvenStr aven_c_token_str(
    AvenCTokenSet tset,
    uint32_t index
) {
    AvenCToken token = get(tset.tokens, index);
    if (token.type == AVEN_C_TOKEN_TYPE_NONE) {
        return (AvenStr){ 0 };
    } else if (token.type == AVEN_C_TOKEN_TYPE_KEY) {
        return aven_c_keyword_str((AvenCKeyword)token.len);
    } else if (token.type == AVEN_C_TOKEN_TYPE_PPD) {
        // Grab entire text chunk for all tokens within directive
        AvenCToken t1 = get(tset.ppd_tokens, token.index);
        AvenCToken tn = get(tset.ppd_tokens, token.index + (token.len - 1));
        return aven_str_range(
            tset.bytes,
            t1.index,
            tn.index + tn.len
        );
    }
    return aven_str_range(
        tset.bytes,
        token.index,
        token.index + token.len
    );
}

static inline AvenStr aven_c_ppd_token_str(
    AvenCTokenSet tset,
    uint32_t index
) {
    AvenCToken token = get(tset.ppd_tokens, index);
    if (token.type == AVEN_C_TOKEN_TYPE_NONE) {
        return (AvenStr){ 0 };
    } else if (token.type == AVEN_C_TOKEN_TYPE_KEY) {
        return aven_c_keyword_str((AvenCKeyword)token.len);
    }
    return aven_str_range(
        tset.bytes,
        token.index,
        token.index + token.len
    );
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
        get(tset.ppd_tokens, token.index).index :
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

static inline AvenCTokenLoc aven_c_ppd_token_loc(
    AvenCTokenSet tset,
    uint32_t index
) {
    AvenCToken token = get(tset.ppd_tokens, index);
    assert(token.type != AVEN_C_TOKEN_TYPE_PPD);
    uint32_t line = 1;
    uint32_t col = 1;
    for (uint32_t i = 0; i < token.index; i += 1) {
        if (get(tset.bytes, i) == '\n') {
            line += 1;
            col = 1;
        } else {
            col += 1;
        }
    }
    return (AvenCTokenLoc){ .line = line, .col = col };
}

typedef struct {
    AvenStr bytes;
    AvenCTokenList tokens;
    AvenCTokenList child_tokens;
    uint32_t token_start;
    uint32_t index;
    bool child;
    char next;
} AvenCLexCtx;

static inline AvenCLexCtx aven_c_lex_init(
    AvenStr bytes,
    AvenArena *arena
) {
    AvenCLexCtx ctx = {
        .bytes = bytes,
        .tokens = { .cap = bytes.len + 1 },
        .child_tokens = { .cap = bytes.len + 1 },
    };
    ctx.tokens.ptr = aven_arena_create_array(
        AvenCToken,
        arena,
        ctx.tokens.cap
    );
    ctx.child_tokens.ptr = aven_arena_create_array(
        AvenCToken,
        arena,
        ctx.child_tokens.cap
    );
    return ctx;
}

static inline AvenCLexCtx aven_c_lex_init_child(
    AvenStr bytes,
    uint32_t index,
    AvenCTokenList tokens
) {
    return (AvenCLexCtx){
        .bytes = bytes,
        .index = index,
        .token_start = index,
        .tokens = tokens,
        .child = true,
    };
}

static inline bool aven_c_is_whitespace(char c) {
    return c == ' ' or c == '\t' or c == '\r' or c == '\n';
}

static inline bool aven_c_is_oct_digit(char c) {
    return c >= '0' and c <= '7';
}

static inline bool aven_c_is_dec_digit(char c) {
    return c >= '0' and c <= '9';
}

static inline bool aven_c_is_hex_digit(char c) {
    return aven_c_is_dec_digit(c) or
        (c >= 'a' and c <= 'f') or
        (c >= 'A' and c <= 'F');
}

static inline bool aven_c_is_letter(char c) {
    return (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z');
}

#define aven_c_lex_next(c, i) do { \
        if (!aven_c_lex_next_internal(c)) { \
            c->index = i; \
            return false; \
        } \
    } while (0)

static inline bool aven_c_lex_next_internal(AvenCLexCtx *ctx) {
    if (ctx->index >= ctx->bytes.len) {
        return false;
    }
    ctx->next = get(ctx->bytes, ctx->index);
    ctx->index += 1;
    return true;
}

static inline bool aven_c_lex_char(AvenCLexCtx *ctx, char d) {
    uint32_t init_index = ctx->index;
    aven_c_lex_next(ctx, init_index);
    if (ctx->next == d) {
        return true;
    }
    ctx->index = init_index;
    return false;
}

static inline bool aven_c_lex_str(AvenCLexCtx *ctx, AvenStr str) {
    uint32_t init_index = ctx->index;
    for (size_t i = 0; i < str.len; i += 1) {
        if (!aven_c_lex_char(ctx, get(str, i))) {
            ctx->index = init_index;
            return false;
        }
    }
    return true;
}

static inline bool aven_c_lex_oct_digit(AvenCLexCtx *ctx) {
    uint32_t init_index = ctx->index;
    aven_c_lex_next(ctx, init_index);
    if (aven_c_is_oct_digit(ctx->next)) {
        return true;
    }
    ctx->index = init_index;
    return false;
}

static inline bool aven_c_lex_dec_digit(AvenCLexCtx *ctx) {
    uint32_t init_index = ctx->index;
    aven_c_lex_next(ctx, ctx->index);
    if (aven_c_is_dec_digit(ctx->next)) {
        return true;
    }
    ctx->index = init_index;
    return false;
}

static inline bool aven_c_lex_hex_digit(AvenCLexCtx *ctx) {
    uint32_t init_index = ctx->index;
    aven_c_lex_next(ctx, ctx->index);
    if (aven_c_is_hex_digit(ctx->next)) {
        return true;
    }
    ctx->index = init_index;
    return false;
}

static inline bool aven_c_lex_hex_str(AvenCLexCtx *ctx, uint32_t digits) {
    uint32_t init_index = ctx->index;
    for (uint32_t i = 0; i < digits; i += 1) {
        if (!aven_c_lex_hex_digit(ctx)) {
            ctx->index = init_index;
            return false;
        }
    }
    return true;
}

static inline bool aven_c_lex_letter(AvenCLexCtx *ctx) {
    uint32_t init_index = ctx->index;
    aven_c_lex_next(ctx, ctx->index);
    if (aven_c_is_letter(ctx->next)) {
        return true;
    }
    ctx->index = init_index;
    return false;
}

static inline bool aven_c_lex_id_nondigit(AvenCLexCtx *ctx) {
    uint32_t init_index = ctx->index;
    aven_c_lex_next(ctx, init_index);
    char c = ctx->next;
    switch (c) {
        case '_': {
            return true;
        }
        case '\\': {
            if (aven_c_lex_char(ctx, 'u') and aven_c_lex_hex_str(ctx, 4)) {
                return true;
            } else if (
                aven_c_lex_char(ctx, 'U') and
                aven_c_lex_hex_str(ctx, 8)
            ) {
                return true;
            }
            ctx->index = init_index;
            break;
        }
        default: {
            if (aven_c_is_letter(c)) {
                return true;
            }
            ctx->index -= 1;
            break;
        }
    }
    return false;
}

static inline bool aven_c_lex_escape_seq(AvenCLexCtx * ctx) {
    uint32_t init_index = ctx->index;
    if (!aven_c_lex_char(ctx, '\\')) {
        return false;
    }

    aven_c_lex_next(ctx, init_index);
    char e = ctx->next;
    switch (e) {
        case 'a':
        case 'b':
        case 'f':
        case 'n':
        case 'r':
        case 't':
        case 'v':
        case '\'':
        case '\"':
        case '\\':
        case '?': {
            return true;
        }
        default: {
            ctx->index -= 1;
            break;
        }
    }
    uint32_t oct_digits;
    for (oct_digits = 0; oct_digits < 3; oct_digits += 1) {
        if (!aven_c_lex_oct_digit(ctx)) {
            break;
        }
    }
    if (oct_digits > 0) {
        return true;
    }

    if (e == 'x') {
        ctx->index += 1;
        uint32_t hex_digits = 0;
        while (aven_c_lex_hex_digit(ctx)) {
            hex_digits += 1;
        }
        if (hex_digits > 0) {
            return true;
        }
    }

    ctx->index = init_index;
    return false;
}

static inline bool aven_c_lex_identifier(AvenCLexCtx *ctx) {
    if (!aven_c_lex_id_nondigit(ctx)) {
        return false;
    }

    while (
        aven_c_lex_id_nondigit(ctx) or
        aven_c_lex_dec_digit(ctx)
    ) {}

    return true;
}

static inline bool aven_c_lex_number_exp(AvenCLexCtx *ctx) {
    uint32_t init_index = ctx->index;
    if (
        !aven_c_lex_char(ctx, 'e') and
        !aven_c_lex_char(ctx, 'E') and
        !aven_c_lex_char(ctx, 'p') and
        !aven_c_lex_char(ctx, 'P')
    ) {
        return false;
    }

    if (aven_c_lex_char(ctx, '+') or aven_c_lex_char(ctx, '+')) {
        return true;
    }

    ctx->index = init_index;
    return false;
}

static inline bool aven_c_lex_number(AvenCLexCtx *ctx) {
    uint32_t init_index = ctx->index;
    if (
        !aven_c_lex_dec_digit(ctx) and
        !(aven_c_lex_char(ctx, '.') and aven_c_lex_dec_digit(ctx))
    ) {
        ctx->index = init_index;
        return false;
    }

    while (
        aven_c_lex_dec_digit(ctx) or
        aven_c_lex_number_exp(ctx) or
        aven_c_lex_id_nondigit(ctx) or
        aven_c_lex_char(ctx, '.')
    ) {}

    return true;
}

static inline bool aven_c_lex_character_const(AvenCLexCtx *ctx) {
    uint32_t init_index = ctx->index;
    if (!aven_c_lex_char(ctx, '\'')) {
        return false;
    }

    bool finished = false;
    uint32_t interior = 0;
    while (!finished) {
        while (aven_c_lex_escape_seq(ctx)) {
            interior += 1;
        }
        aven_c_lex_next(ctx, init_index);
        if (ctx->next == '\n' or ctx->next == '\\') {
            break;
        }
        if (ctx->next == '\'') {
            finished = true;
            break;
        }
        interior += 1;
    }
    if (finished and interior > 0) {
        return true;
    }

    ctx->index = init_index;
    return false;
}

static inline bool aven_c_lex_string_lit(AvenCLexCtx *ctx) {
    uint32_t init_index = ctx->index;
    // not doing C11 UTF8 encoding prefixes
    if (!aven_c_lex_char(ctx, '\"')) {
        return false;
    }

    bool finished = false;
    while (!finished) {
        while (aven_c_lex_escape_seq(ctx)) {}
        aven_c_lex_next(ctx, init_index);
        if (ctx->next == '\n' or ctx->next == '\\') {
            break;
        }
        if (ctx->next == '\"') {
            finished = true;
            break;
        }
    }
    if (finished) {
        return true;
    }

    ctx->index = init_index;
    return false;
}

static inline bool aven_c_lex_punctuator(AvenCLexCtx *ctx) {
    uint32_t init_index = ctx->index;
    aven_c_lex_next(ctx, init_index);
    switch (ctx->next) {
        case '[':
        case ']':
        case '(':
        case ')':
        case '{':
        case '}':
        case '?':
        case ',':
        case ';':
        case '~': {
            return true;
        }
        case '.': {
            if (aven_c_lex_str(ctx, aven_str(".."))) {
                return true;
            }
            return true;
        }
        case '-': {
            if (
                aven_c_lex_char(ctx, '-') or
                aven_c_lex_char(ctx, '>') or
                aven_c_lex_char(ctx, '=')
            ) {
                return true;
            }
            return true;
        }
        case '+': {
            if (
                aven_c_lex_char(ctx, '+') or
                aven_c_lex_char(ctx, '=')
            ) {
                return true;
            }
            return true;
        }
        case '&': {
            if (
                aven_c_lex_char(ctx, '&') or
                aven_c_lex_char(ctx, '=')
            ) {
                return true;
            }
            return true;
        }
        case '*': {
            if (aven_c_lex_char(ctx, '=')) {
                return true;
            }
            return true;
        }
        case '!': {
            if (aven_c_lex_char(ctx, '=')) {
                return true;
            }
            return true;
        }
        case '/': {
            if (aven_c_lex_char(ctx, '=')) {
                return true;
            }
            return true;
        }
        case '%': {
            if (aven_c_lex_char(ctx, ':')) {
                if (aven_c_lex_str(ctx, aven_str("%:"))) {
                    return true;
                }
                return true;
            }
            if (
                aven_c_lex_char(ctx, '>') or
                aven_c_lex_char(ctx, '=')
            ) {
                return true;
            }
            return true;
        }
        case '<': {
            if (aven_c_lex_char(ctx, '<')) {
                if (aven_c_lex_char(ctx, '=')) {
                    return true;
                }
                return true;
            }
            if (
                aven_c_lex_char(ctx, '=') or
                aven_c_lex_char(ctx, ':') or
                aven_c_lex_char(ctx, '%')
            ) {
                return true;
            }
            return true;
        }
        case '>': {
            if (aven_c_lex_char(ctx, '>')) {
                if (aven_c_lex_char(ctx, '=')) {
                    return true;
                }
                return true;
            }
            if (aven_c_lex_char(ctx, '=')) {
                return true;
            }
            return true;
        }
        case '=': {
            if (aven_c_lex_char(ctx, '=')) {
                return true;
            }
            return true;
        }
        case '^': {
            if (aven_c_lex_char(ctx, '=')) {
                return true;
            }
            return true;
        }
        case '|': {
            if (aven_c_lex_char(ctx, '|')) {
                return true;
            }
            if (aven_c_lex_char(ctx, '=')) {
                return true;
            }
            return true;
        }
        case ':': {
            if (aven_c_lex_char(ctx, '>')) {
                return true;
            }
            return true;
        }
        case '#': {
            if (aven_c_lex_char(ctx, '#')) {
                return true;
            }
            return true;
        }
        default: {
            break;
        }
    }

    ctx->index = init_index;
    return false;
}

static inline bool aven_c_lex_ignore(AvenCLexCtx *ctx) {
    assert(ctx->index == ctx->token_start);
    bool done = false;
    while (!done) {
        ctx->token_start = ctx->index;
        aven_c_lex_next(ctx, ctx->index);
        switch (ctx->next) {
            case ' ':
            case '\t':
            case '\r': {
                break;
            }
            case '\n': {
                if (ctx->tokens.len > 0) {
                    list_back(ctx->tokens).trailing_lines += 1;
                }
                break;
            }
            case '\\': {
                if (
                    !(
                        aven_c_lex_char(ctx, '\n') or
                        aven_c_lex_str(ctx, aven_str("\r\n"))
                    )
                ) {
                    done = true;
                }
                break;
            }
            default: {
                done = true;
                break;
            }
        }
    }
    // the last character was not whitespace, so backtrack
    ctx->index -= 1;
    return false;
}

static inline bool aven_c_lex_step(AvenCLexCtx *ctx);

static inline void aven_c_lex_line(AvenCLexCtx * ctx) {
    for (; ctx->index < ctx->bytes.len; ctx->index += 1) {
        if (get(ctx->bytes, ctx->index) == '\n') {
            if (get(ctx->bytes, ctx->index - 1) == '\\') {
                continue;
            }
            if (
                ctx->index > 1 and
                get(ctx->bytes, ctx->index - 1) == '\r' and
                get(ctx->bytes, ctx->index - 2) == '\\'
            ) {
                continue;
            }
            break;
        }
    }
}

static inline void aven_c_lex_pp_line(AvenCLexCtx * ctx) {
    for (; ctx->index < ctx->bytes.len; ctx->index += 1) {
        bool done = false;
        switch (get(ctx->bytes, ctx->index)) {
            case '\n': {
                if (get(ctx->bytes, ctx->index - 1) == '\\') {
                    break;
                }
                if (
                    ctx->index > 1 and
                    get(ctx->bytes, ctx->index - 1) == '\r' and
                    get(ctx->bytes, ctx->index - 2) == '\\'
                ) {
                    break;
                }
                done = true;
                break;
            }
            case '/': {
                if (
                    ctx->index + 1 < ctx->bytes.len and (
                        get(ctx->bytes, ctx->index + 1) == '/' or
                        get(ctx->bytes, ctx->index + 1) == '*'
                    )
                ) {
                    done = true;
                }
                break;
            }
        }
        if (done) {
            break;
        }
    }
}

static inline bool aven_c_lex_multiline_comment_line(AvenCLexCtx * ctx) {
    for (; ctx->index < ctx->bytes.len; ctx->index += 1) {
        if (get(ctx->bytes, ctx->index) == '\n') {
            return false;
        } else if (get(ctx->bytes, ctx->index) == '/') {
            if (get(ctx->bytes, ctx->index - 1) == '*') {
                ctx->index += 1;
                return true;
            }
        }
    }
    return true;
}

static inline bool aven_c_lex_ppd(AvenCLexCtx *ctx) {
    assert(ctx->index == ctx->token_start);
    uint32_t init_index = ctx->index;
    if (!aven_c_lex_char(ctx, '#')) {
        return false;
    }
    uint32_t start = ctx->index;
    aven_c_lex_pp_line(ctx);
    uint32_t first = (uint32_t)ctx->child_tokens.len;
    list_push(ctx->child_tokens) = (AvenCToken){
        .type = AVEN_C_TOKEN_TYPE_PNC,
        .index = init_index,
        .len = 1,
    };
    AvenCLexCtx pp_ctx = aven_c_lex_init_child(
        aven_str_head(ctx->bytes, ctx->index),
        start,
        ctx->child_tokens
    );
    while (!aven_c_lex_step(&pp_ctx)) {}
    ctx->child_tokens = pp_ctx.tokens;
    uint32_t last = (uint32_t)ctx->child_tokens.len;
    list_push(ctx->tokens) = (AvenCToken){
        .type = AVEN_C_TOKEN_TYPE_PPD,
        .index = first,
        .len = last - first,
    };
    ctx->token_start = ctx->index;

    return true;
}

static inline bool aven_c_lex_comment(AvenCLexCtx *ctx) {
    if (aven_c_lex_str(ctx, aven_str("//"))) {
        aven_c_lex_line(ctx);
        return true;
    }
    return false;
}

static inline bool aven_c_lex_multiline_comment(AvenCLexCtx *ctx) {
    bool found = false;
    if (aven_c_lex_str(ctx, aven_str("/*"))) {
        found = true;
        if (ctx->index < ctx->bytes.len) {
            if (get(ctx->bytes, ctx->index) == '\n') {
                list_push(ctx->tokens) = (AvenCToken){
                    .type = AVEN_C_TOKEN_TYPE_CMT,
                    .index = ctx->token_start,
                    .len = ctx->index - ctx->token_start,
                };
                ctx->token_start = ctx->index;
                aven_c_lex_ignore(ctx);
            } else {
                ctx->index += 1;
            }
        }
        while (!aven_c_lex_multiline_comment_line(ctx)) {
            list_push(ctx->tokens) = (AvenCToken){
                .type = AVEN_C_TOKEN_TYPE_CMT,
                .index = ctx->token_start,
                .len = ctx->index - ctx->token_start,
            };
            ctx->token_start = ctx->index;
            aven_c_lex_ignore(ctx);
        }
    }
    return found;
}

static inline bool aven_c_lex_other(AvenCLexCtx *ctx) {
    aven_c_lex_next(ctx, ctx->index);
    return true;
}

static inline bool aven_c_lex_step(AvenCLexCtx *ctx) {
    assert(ctx->index == ctx->token_start);
    aven_c_lex_ignore(ctx);
    if (!ctx->child and aven_c_lex_ppd(ctx)) {
        return false;
    }
    if (aven_c_lex_multiline_comment(ctx)) {
        list_push(ctx->tokens) = (AvenCToken){
            .type = AVEN_C_TOKEN_TYPE_CMT,
            .index = ctx->token_start,
            .len = ctx->index - ctx->token_start,
        };
        ctx->token_start = ctx->index;
        return false;
    }
    if (aven_c_lex_comment(ctx)) {
        list_push(ctx->tokens) = (AvenCToken){
            .type = AVEN_C_TOKEN_TYPE_CMT,
            .index = ctx->token_start,
            .len = ctx->index - ctx->token_start,
        };
        ctx->token_start = ctx->index;
        return false;
    }
    if (aven_c_lex_identifier(ctx)) {
        AvenStr name = aven_str_range(ctx->bytes, ctx->token_start, ctx->index);
        AvenCKeyword keyword = aven_c_keyword(name);
        if (keyword == AVEN_C_KEYWORD_NONE) {
            list_push(ctx->tokens) = (AvenCToken){
                .type = AVEN_C_TOKEN_TYPE_ID,
                .index = ctx->token_start,
                .len = ctx->index - ctx->token_start,
            };
        } else {
            list_push(ctx->tokens) = (AvenCToken){
                .type = AVEN_C_TOKEN_TYPE_KEY,
                .index = ctx->token_start,
                .len = (uint32_t)keyword,
            };
        }
        ctx->token_start = ctx->index;
        return false;
    }
    if (aven_c_lex_number(ctx)) {
        list_push(ctx->tokens) = (AvenCToken){
            .type = AVEN_C_TOKEN_TYPE_NUM,
            .index = ctx->token_start,
            .len = ctx->index - ctx->token_start,
        };
        ctx->token_start = ctx->index;
        return false;
    }
    if (aven_c_lex_character_const(ctx)) {
        list_push(ctx->tokens) = (AvenCToken){
            .type = AVEN_C_TOKEN_TYPE_CHR,
            .index = ctx->token_start,
            .len = ctx->index - ctx->token_start,
        };
        ctx->token_start = ctx->index;
        return false;
    }
    if (aven_c_lex_string_lit(ctx)) {
        list_push(ctx->tokens) = (AvenCToken){
            .type = AVEN_C_TOKEN_TYPE_STR,
            .index = ctx->token_start,
            .len = ctx->index - ctx->token_start,
        };
        ctx->token_start = ctx->index;
        return false;
    }
    if (aven_c_lex_punctuator(ctx)) {
        list_push(ctx->tokens) = (AvenCToken){
            .type = AVEN_C_TOKEN_TYPE_PNC,
            .index = ctx->token_start,
            .len = ctx->index - ctx->token_start,
        };
        ctx->token_start = ctx->index;
        return false;
    }
    if (aven_c_lex_other(ctx)) {
        list_push(ctx->tokens) = (AvenCToken){
            .type = AVEN_C_TOKEN_TYPE_OTH,
            .index = ctx->token_start,
            .len = ctx->index - ctx->token_start,
        };
        ctx->token_start = ctx->index;
        return false;
    }
    // We NONE-terminate the token list
    list_push(ctx->tokens) = (AvenCToken){
        .type = AVEN_C_TOKEN_TYPE_NONE,
        .index = ctx->token_start,
        .len = ctx->index - ctx->token_start,
    };
    return true;
}

static inline AvenCTokenSet aven_c_lex(AvenStr bytes, AvenArena *arena) {
    AvenArena temp_arena = *arena;
    AvenCLexCtx ctx = aven_c_lex_init(bytes, &temp_arena);

    while (!aven_c_lex_step(&ctx)) {}

    AvenCTokenSlice tokens = aven_arena_create_slice(
        AvenCToken,
        arena,
        ctx.tokens.len
    );
    AvenCTokenSlice child_tokens = aven_arena_create_slice(
        AvenCToken,
        arena,
        ctx.child_tokens.len
    );
    for (size_t i = 0; i < tokens.len; i += 1) {
        get(tokens, i) = get(ctx.tokens, i);
    }
    for (size_t i = 0; i < child_tokens.len; i += 1) {
        get(child_tokens, i) = get(ctx.child_tokens, i);
    }
    return (AvenCTokenSet){
        .bytes = bytes,
        .tokens = tokens,
        .ppd_tokens = child_tokens,
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
    AVEN_C_AST_NODE_TYPE_DECLARATOR,
    AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR,
    AVEN_C_AST_NODE_TYPE_DIR_DECLARATOR_PARAM_TYPE_LIST,
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
    AVEN_C_AST_NODE_TYPE_FUNCTION_DEFINITION,
    AVEN_C_AST_NODE_TYPE_TRANSLATION_UNIT,
    AVEN_C_AST_NODE_TYPE_TERMINATED_LINE,
    AVEN_C_AST_NODE_TYPE_MACRO_INVOCATION,
    AVEN_C_AST_NODE_TYPE_PREPROCESSOR_PASTE,
    AVEN_C_AST_NODE_TYPE_PREPROCESSOR_HEADER,
    AVEN_C_AST_NODE_TYPE_PREPROCESSOR_DEFINE,
    AVEN_C_AST_NODE_TYPE_PREPROCESSOR_DEFINE_FN,
    AVEN_C_AST_NODE_TYPE_PREPROCESSOR_DIRECTIVE,
    AVEN_C_AST_NODE_TYPE_PREPROCESSOR_MSVC_WARN_SPECIFIER,
    AVEN_C_AST_NODE_TYPE_PREPROCESSOR_MSVC_JUSTIFICATION,
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

typedef struct {
    AvenCTokenSet tset;
    List(AvenCAstNode) nodes;
    List(uint32_t) data;
    List(uint32_t) scratch;
    AvenCAstDataSlice pp_nodes;
    bool ppd;
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
} AvenCAstCtxState;

static inline AvenCAstCtx aven_c_ast_init(
    AvenCTokenSet tset,
    AvenArena *arena
) {
    size_t max_tokens = 1 + 3 * tset.tokens.len + 3 * tset.ppd_tokens.len;
    AvenCAstCtx ctx = {
        .tset = tset,
        .nodes = { .cap = max_tokens },
        .pp_nodes = { .len = tset.tokens.len },
        .data =  { .cap = max_tokens },
        .scratch = { .cap = max_tokens },
    };
    ctx.nodes.ptr = aven_arena_create_array(AvenCAstNode, arena, ctx.nodes.cap);
    ctx.data.ptr = aven_arena_create_array(uint32_t, arena, ctx.data.cap);
    ctx.scratch.ptr = aven_arena_create_array(uint32_t, arena, ctx.scratch.cap);
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
        .tset = {
            .bytes = ctx->tset.bytes,
            .tokens = slice_head(
                ctx->tset.ppd_tokens,
                ppd_token.index + ppd_token.len
            ),
        },
        .nodes = ctx->nodes,
        .data = ctx->data,
        .scratch = ctx->scratch,
        .token_index = ppd_token.index,
        .ppd = true,
    };
}

static inline void aven_c_ast_commit_ppd(
    AvenCAstCtx *ctx,
    AvenCAstCtx* ppd_ctx
) {
    assert(ctx->scratch.len == ppd_ctx->scratch.len);
    ctx->nodes.len = ppd_ctx->nodes.len;
    ctx->data.len = ppd_ctx->data.len;
}

static inline AvenCAstCtxState aven_c_ast_save(AvenCAstCtx *ctx) {
    return (AvenCAstCtxState){
        .nodes_len = (uint32_t)ctx->nodes.len,
        .data_len = (uint32_t)ctx->data.len,
        .scratch_len = (uint32_t)ctx->scratch.len,
        .token_index = ctx->token_index,
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
    return ctx->token_index - 1;
}

static inline uint32_t aven_c_ast_inc_index(AvenCAstCtx *ctx) {
    uint32_t og_index = ctx->token_index;
    ctx->token_index += 1;
    AvenCToken token = get(ctx->tset.tokens, ctx->token_index - 1);
    while (
        token.type == AVEN_C_TOKEN_TYPE_CMT or
        token.type == AVEN_C_TOKEN_TYPE_PPD
    ) {
        ctx->token_index += 1;
        token = get(ctx->tset.tokens, ctx->token_index - 1);
    }
    return og_index - 1;
}

static inline void aven_c_ast_parse_ppd_tokens(AvenCAstCtx *ctx) {
    for (uint32_t i = 0; i < ctx->tset.tokens.len; i += 1) {
        AvenCToken token = get(ctx->tset.tokens, i);
        if (token.type == AVEN_C_TOKEN_TYPE_PPD) {
            AvenCAstCtx ppd_ctx = aven_c_ast_init_ppd(ctx, i);
            aven_c_ast_inc_index(&ppd_ctx);
            uint32_t node = aven_c_ast_parse_preprocessor_directive(&ppd_ctx);
            if (
                node == 0 or
                ppd_ctx.token_index <= ppd_ctx.tset.tokens.len - 1
            ) {
                ctx->ppd_error.valid = true;
                ctx->ppd_error.value = (AvenCAstError){
                    .type = ppd_ctx.error.type,
                    .token = i,
                    .pp_token = ppd_ctx.error.token + 1,
                    .exp = ppd_ctx.error.exp,
                };
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
}

static inline AvenCToken aven_c_ast_next(AvenCAstCtx *ctx) {
    return get(ctx->tset.tokens, aven_c_ast_next_index(ctx));
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

static inline uint32_t aven_c_ast_parse_identifier(AvenCAstCtx *ctx) {
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
    AvenStr str
) {
    if (
        aven_c_ast_next(ctx).type == AVEN_C_TOKEN_TYPE_PNC and
        aven_str_equals(
            aven_c_token_str(ctx->tset, aven_c_ast_next_index(ctx)),
            str
        )
    ) {
        aven_c_ast_inc_index(ctx);
        return true;
    }
    if (aven_c_ast_next_index(ctx) >= ctx->error.token) {
        ctx->error = (AvenCAstError){
            .token = aven_c_ast_next_index(ctx),
            .type = AVEN_C_TOKEN_TYPE_PNC,
            .exp = str,
        };
    }
    return false;
}

static inline bool aven_c_ast_match_keyword(
    AvenCAstCtx *ctx,
    AvenCKeyword keyword
) {
    AvenStr keyword_str = aven_c_keyword_str(keyword);
    if (
        aven_c_ast_next(ctx).type == AVEN_C_TOKEN_TYPE_KEY and
        aven_str_equals(
            aven_c_token_str(ctx->tset, aven_c_ast_next_index(ctx)),
            keyword_str
        )
    ) {
        aven_c_ast_inc_index(ctx);
        return true;
    }
    if (aven_c_ast_next_index(ctx) >= ctx->error.token) {
        ctx->error = (AvenCAstError){
            .token = aven_c_ast_next_index(ctx),
            .type = AVEN_C_TOKEN_TYPE_KEY,
            .exp = keyword_str,
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
    AvenStr pstr    
) {
    uint32_t main_token = aven_c_ast_next_index(ctx);
    if (aven_c_ast_match_punctuator(ctx, pstr)) {
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
    if (!aven_c_ast_match_punctuator(ctx, aven_str(";"))) {
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

static inline uint32_t aven_c_ast_parse_type_name(AvenCAstCtx *ctx);
static inline uint32_t aven_c_ast_parse_type_specifier(AvenCAstCtx *ctx);
static inline uint32_t aven_c_ast_parse_type_qualifier(AvenCAstCtx *ctx);
static inline uint32_t aven_c_ast_parse_primary_expr(AvenCAstCtx *ctx);
static inline uint32_t aven_c_ast_parse_expr(AvenCAstCtx *ctx);
static inline uint32_t aven_c_ast_parse_cast_expr(AvenCAstCtx *ctx);
static inline uint32_t aven_c_ast_parse_assign_expr(AvenCAstCtx *ctx);
static inline uint32_t aven_c_ast_parse_const_expr(AvenCAstCtx *ctx);
static inline uint32_t aven_c_ast_parse_initializer_list(AvenCAstCtx *ctx);
static inline uint32_t aven_c_ast_parse_declaration(AvenCAstCtx *ctx);
static inline uint32_t aven_c_ast_parse_declarator(AvenCAstCtx *ctx);
static inline uint32_t aven_c_ast_parse_parameter_type_list(AvenCAstCtx *ctx);
static inline uint32_t aven_c_ast_parse_abstract_declarator(AvenCAstCtx *ctx);

static inline uint32_t aven_c_ast_parse_preprocessor_msvc_warn_arg(
    AvenCAstCtx * ctx
) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t id_node = aven_c_ast_parse_identifier(ctx);
    if (id_node == 0) {
        return 0;
    }
    uint32_t main_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(":"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t rhs = aven_c_ast_parse_string_literal(ctx);
    if (rhs != 0) {
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_PREPROCESSOR_MSVC_JUSTIFICATION,
            main_token,
            id_node,
            rhs
        );
    }
    uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
    for (;;) {
        uint32_t num = aven_c_ast_parse_constant(ctx);
        if (num == 0) {
            break;
        }
        list_push(ctx->scratch) = num;
    }
    rhs = aven_c_ast_scratch_commit(ctx, scratch_top);
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    return aven_c_ast_push(
        ctx,
        AVEN_C_AST_NODE_TYPE_PREPROCESSOR_MSVC_WARN_SPECIFIER,
        main_token,
        id_node,
        rhs
    );
}

static inline uint32_t aven_c_ast_parse_macro_argument(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    if (ctx->ppd) {
        uint32_t msvc = aven_c_ast_parse_preprocessor_msvc_warn_arg(ctx);
        if (msvc != 0) {
            return msvc;
        }
    }
    uint32_t expr_index = 0;
    uint32_t expr = aven_c_ast_parse_assign_expr(ctx);
    if (expr != 0) {
        expr_index = ctx->token_index;
        aven_c_ast_restore(ctx, state);
    }
    uint32_t type_name_index = 0;
    uint32_t type_name = aven_c_ast_parse_type_name(ctx);
    if (type_name != 0) {
        type_name_index = ctx->token_index;
        aven_c_ast_restore(ctx, state);
    }
    if (type_name_index > expr_index) {
        return aven_c_ast_parse_type_name(ctx);
    }
    if (expr_index > 0) {
        return aven_c_ast_parse_assign_expr(ctx);
    }
    return 0;
}

static inline uint32_t aven_c_ast_parse_macro_invocation(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t node = aven_c_ast_parse_identifier(ctx);
    if (node == 0) {
        return 0;
    }
    uint32_t open_token = aven_c_ast_next_index(ctx);
    if (aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        uint32_t arg_list = 0;
        {
            uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
            for (;;) {
                uint32_t arg = aven_c_ast_parse_macro_argument(ctx);
                if (arg == 0) {
                    break;
                }
                list_push(ctx->scratch) = arg;
                if (!aven_c_ast_match_punctuator(ctx, aven_str(","))) {
                    break;
                }
            }
            arg_list = aven_c_ast_scratch_commit(ctx, scratch_top);
        }
        uint32_t close_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
            aven_c_ast_restore(ctx, state);
            return 0;
        }
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
    if (!aven_c_ast_match_punctuator(ctx, aven_str("#"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t lhs = aven_c_ast_parse_identifier(ctx);
    if (lhs == 0) {
        aven_c_ast_restore(ctx, state);
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
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    uint32_t count = 0;
    uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
    for (;;) {
        uint32_t part = aven_c_ast_parse_string_literal(ctx);
        if (part == 0) {
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
        aven_c_ast_restore(ctx, state);
        return 0;
    } 
    if (count == 1) {
        aven_c_ast_restore(ctx, state);
        uint32_t node = aven_c_ast_parse_string_literal(ctx);
        if (node == 0) {
            node = aven_c_ast_parse_macro_invocation(ctx);
        }
        if (ctx->ppd and node == 0) {
            node = aven_c_ast_parse_preprocessor_paste(ctx);
        }
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
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t start_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t tname = aven_c_ast_parse_type_name(ctx);
    uint32_t end_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
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
    if (!aven_c_ast_match_punctuator(ctx, aven_str("="))) {
        aven_c_ast_restore(ctx, state);
        return id_node;
    }
    uint32_t cexpr_node = aven_c_ast_parse_const_expr(ctx);
    if (cexpr_node == 0) {
        aven_c_ast_restore(ctx, state);
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
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t id_node = aven_c_ast_parse_identifier(ctx);
    uint32_t enum_list = 0;
    uint32_t close_token;
    uint32_t open_token = aven_c_ast_next_index(ctx);
    if (aven_c_ast_match_punctuator(ctx, aven_str("{"))) {
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        for (;;) {
            uint32_t enum_node = aven_c_ast_parse_enumerator(ctx);
            if (enum_node == 0) {
                break;
            }
            list_push(ctx->scratch) = enum_node;
            if (!aven_c_ast_match_punctuator(ctx, aven_str(","))) {
                break;
            }
        }
        close_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, aven_str("}"))) {
            aven_c_ast_restore(ctx, state);
            return 0;
        }
        enum_list = aven_c_ast_scratch_commit(ctx, scratch_top);
    }
    if (id_node == 0 and enum_list == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t token = main_token;
    if (enum_list != 0) {
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = main_token;
        list_push(ctx->scratch) = open_token;
        list_push(ctx->scratch) = close_token;
        token = aven_c_ast_scratch_commit(ctx,scratch_top);
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
    uint32_t lhs = aven_c_ast_parse_keyword(ctx, AVEN_C_KEYWORD_STATIC_ASSERT);
    if (lhs == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t open_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t cexpr = aven_c_ast_parse_const_expr(ctx);
    if (cexpr == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    if (!aven_c_ast_match_punctuator(ctx, aven_str(","))) {
        aven_c_ast_restore(ctx, state);
        return 0;        
    }
    uint32_t str = aven_c_ast_parse_string_constant(ctx);
    if (str == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t close_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
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
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(":"))) {
        return decl_node;
    }
    uint32_t cexpr_node = aven_c_ast_parse_const_expr(ctx);
    if (cexpr_node == 0) {
        aven_c_ast_restore(ctx, state);
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

static inline uint32_t aven_c_ast_parse_struct_declarator_list(AvenCAstCtx *ctx) {
    uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
    for (;;) {
        uint32_t struct_decl = aven_c_ast_parse_struct_declarator(ctx);
        if (struct_decl == 0) {
            break;
        }
        list_push(ctx->scratch) = struct_decl;
        if (!aven_c_ast_match_punctuator(ctx, aven_str(","))) {
            break;
        }
    }
    return aven_c_ast_scratch_commit(ctx, scratch_top);
}

static inline uint32_t aven_c_ast_parse_struct_declaration_specifier(
    AvenCAstCtx *ctx
) {
    uint32_t node = aven_c_ast_parse_type_specifier(ctx);
    if (node == 0) {
        node = aven_c_ast_parse_type_qualifier(ctx);
    }
    if (node == 0) {
        // parse an identifier over macro invocation only if it allows
        // for a subsequent declarator to be parsed
        AvenCAstCtxState state = aven_c_ast_save(ctx);
        node = aven_c_ast_parse_identifier(ctx);
        if (node != 0) {
            AvenCAstCtxState last = aven_c_ast_save(ctx);
            uint32_t declarator = aven_c_ast_parse_struct_declarator(ctx);
            aven_c_ast_restore(ctx, last);
            if (declarator == 0) {
                node = 0;
                aven_c_ast_restore(ctx, state);
            }
        }
    }
    if (node == 0) {
        node = aven_c_ast_parse_macro_invocation(ctx);
    }
    return node;
}

static inline uint32_t aven_c_ast_parse_struct_declaration_unterminated(
    AvenCAstCtx *ctx
) {
    uint32_t main_token = aven_c_ast_next_index(ctx);
    uint32_t sassert_node = aven_c_ast_parse_static_assert_declaration(ctx);
    if (sassert_node != 0) {
        return sassert_node;
    }
    uint32_t best_index = ctx->token_index;
    uint32_t best_decl_spec_count = 0;
    uint32_t decl_spec_count = 0;
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    for (;;) {
        uint32_t tnode = aven_c_ast_parse_struct_declaration_specifier(ctx);
        if (tnode == 0) {
            break;
        }
        decl_spec_count += 1;
        AvenCAstCtxState last = aven_c_ast_save(ctx);
        aven_c_ast_parse_struct_declarator_list(ctx);
        if (ctx->token_index >= best_index) {
            best_index = ctx->token_index;
            best_decl_spec_count = decl_spec_count;
        }
        aven_c_ast_restore(ctx, last);
    }
    aven_c_ast_restore(ctx, state);
    if (best_decl_spec_count == 0) {
        return 0;
    }
    uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
    for (uint32_t i = 0; i < best_decl_spec_count; i += 1) {
        list_push(ctx->scratch) = aven_c_ast_parse_struct_declaration_specifier(
            ctx
        );
    }
    uint32_t decl_spec_list = aven_c_ast_scratch_commit(ctx, scratch_top);
    uint32_t struct_decl_list = aven_c_ast_parse_struct_declarator_list(ctx);
    return aven_c_ast_push(
        ctx,
        AVEN_C_AST_NODE_TYPE_STRUCT_DECLARATION,
        main_token,
        decl_spec_list,
        struct_decl_list
    );
}

static inline uint32_t aven_c_ast_parse_struct_declaration(
    AvenCAstCtx *ctx
) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t unterm = aven_c_ast_parse_struct_declaration_unterminated(ctx);
    if (unterm == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t term = aven_c_ast_parse_terminated_line(ctx, unterm);
    if (term == 0) {
        aven_c_ast_restore(ctx, state);
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
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t id_node = aven_c_ast_parse_identifier(ctx);
    uint32_t decl_list = 0;
    uint32_t close_token;
    uint32_t open_token = aven_c_ast_next_index(ctx);
    if (aven_c_ast_match_punctuator(ctx, aven_str("{"))) {
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        for (;;) {
            uint32_t decl_node = aven_c_ast_parse_struct_declaration(ctx);
            if (decl_node == 0) {
                break;
            }
            list_push(ctx->scratch) = decl_node;
        }
        close_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, aven_str("}"))) {
            aven_c_ast_restore(ctx, state);
            return 0;
        }
        decl_list = aven_c_ast_scratch_commit(ctx,scratch_top);
    }
    if (id_node == 0 and decl_list == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t token = main_token;
    if (decl_list != 0) {
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        list_push(ctx->scratch) = main_token;
        list_push(ctx->scratch) = open_token;
        list_push(ctx->scratch) = close_token;
        token = aven_c_ast_scratch_commit(ctx,scratch_top);
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
    if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_KEY) {
        return 0;
    }
    uint32_t node = 0;
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    switch (nchar) {
        case 'b': {
            node = aven_c_ast_parse_keyword(
                ctx,
                AVEN_C_KEYWORD_C11BOOL
            );
            break;
        }
        case 'c': {
            node = aven_c_ast_parse_keyword(
                ctx,
                AVEN_C_KEYWORD_CHAR
            );
            break;
        }
        case 'd': {
            node = aven_c_ast_parse_keyword(
                ctx,
                AVEN_C_KEYWORD_DOUBLE
            );
            break;
        }
        case 'e': {
            node = aven_c_ast_parse_enum_specifier(ctx);
            break;
        }
        case 'f': {
            node = aven_c_ast_parse_keyword(
                ctx,
                AVEN_C_KEYWORD_FLOAT
            );
            break;
        }
        case 'i': {
            node = aven_c_ast_parse_keyword(
                ctx,
                AVEN_C_KEYWORD_INT
            );
            break;
        }
        case 'l': {
            node = aven_c_ast_parse_keyword(
                ctx,
                AVEN_C_KEYWORD_LONG
            );
            break;
        }
        case 's': {
            node = aven_c_ast_parse_keyword(
                ctx,
                AVEN_C_KEYWORD_SHORT
            );
            if (node == 0) {
                node = aven_c_ast_parse_keyword(
                    ctx,
                    AVEN_C_KEYWORD_SIGNED
                );
            }
            if (node == 0) {
                node = aven_c_ast_parse_struct_specifier(ctx);
            }
            break;
        }
        case 'u': {
            node = aven_c_ast_parse_keyword(
                ctx,
                AVEN_C_KEYWORD_UNSIGNED
            );
            if (node == 0) {
                node = aven_c_ast_parse_struct_specifier(ctx);
            }
            break;
        }
        case 'v': {
            node = aven_c_ast_parse_keyword(
                ctx,
                AVEN_C_KEYWORD_VOID
            );
            break;
        }
        case '_': {
            node = aven_c_ast_parse_keyword(
                ctx,
                AVEN_C_KEYWORD_BOOL
            );
            if (node == 0) {
                node = aven_c_ast_parse_keyword(
                    ctx,
                    AVEN_C_KEYWORD_COMPLEX
                );
            }
            if (node == 0) {
                node = aven_c_ast_parse_atomic_specifier(ctx);
            }
            break;
        }
        default: {
            break;
        }
    }
    return node;
}

static inline uint32_t aven_c_ast_parse_type_qualifier(AvenCAstCtx *ctx) {
    if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_KEY) {
        return 0;
    }
    uint32_t main_token = aven_c_ast_next_index(ctx);
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    uint32_t node = 0;
    switch (nchar) {
        case 'c': {
            if (aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_CONST)) {
                node = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_TYPE_QUALIFIER,
                    main_token
                );
            }
            break;
        }
        case 'r': {
            if (aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_RESTRICT)) {
                node = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_TYPE_QUALIFIER,
                    main_token
                );
            }
            break;
        }
        case 'v': {
            if (aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_VOLATILE)) {
                node = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_TYPE_QUALIFIER,
                    main_token
                );
            }
            break;
        }
        case '_': {
            if (aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_ATOMIC)) {
                node = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_TYPE_QUALIFIER,
                    main_token
                );
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
    if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_KEY) {
        return 0;
    }
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    switch (nchar) {
        case 't': {
            return aven_c_ast_parse_keyword(ctx, AVEN_C_KEYWORD_TYPEDEF);
        }
        case 'e': {
            return aven_c_ast_parse_keyword(ctx, AVEN_C_KEYWORD_EXTERN);
        }
        case 's': {
            return aven_c_ast_parse_keyword(ctx, AVEN_C_KEYWORD_STATIC);
        }
        case '_': {
            return aven_c_ast_parse_keyword(ctx, AVEN_C_KEYWORD_THREAD_LOCAL);
        }
        case 'a': {
            return aven_c_ast_parse_keyword(ctx, AVEN_C_KEYWORD_AUTO);
        }
        case 'r': {
            return aven_c_ast_parse_keyword(ctx, AVEN_C_KEYWORD_REGISTER);
        }
        default: {
            break;
        }
    }
    return 0;
}

static inline uint32_t aven_c_ast_parse_alignment_specifier(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_ALIGNAS)) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t start_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t node = aven_c_ast_parse_const_expr(ctx);
    if (node == 0) {
        node = aven_c_ast_parse_type_name(ctx);
    }
    if (node == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t end_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
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

static inline uint32_t aven_c_ast_parse_function_specifier(
    AvenCAstCtx *ctx
) {
    if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_KEY) {
        return 0;
    }
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    switch (nchar) {
        case 'i': {
            return aven_c_ast_parse_keyword(ctx, AVEN_C_KEYWORD_INLINE);
        }
        case '_': {
            return aven_c_ast_parse_keyword(ctx, AVEN_C_KEYWORD_NORETURN);
        }
        default: {
            break;
        }
    }
    return 0;
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
    // if (node == 0) {
    //     // parse an identifier over macro invocation only if it allows
    //     // for a subsequent declarator to be parsed
    //     AvenCAstCtxState state = aven_c_ast_save(ctx);
    //     node = aven_c_ast_parse_identifier(ctx);
    //     if (node != 0) {
    //         AvenCAstCtxState last = aven_c_ast_save(ctx);
    //         uint32_t declarator = aven_c_ast_parse_declarator(ctx);
    //         aven_c_ast_restore(ctx, last);
    //         if (abstract and declarator == 0) {
    //             declarator = aven_c_ast_parse_abstract_declarator(ctx);
    //             aven_c_ast_restore(ctx, last);
    //         }
    //         if (declarator == 0) {
    //             node = 0;
    //             aven_c_ast_restore(ctx, state);
    //         }
    //     }
    // }
    if (node == 0) {
        node = aven_c_ast_parse_macro_invocation(ctx);
    }
    return node;
}

static inline uint32_t aven_c_ast_count_declaration_specifiers(
    AvenCAstCtx *ctx
) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t count = 0;
    for (;;) {
        uint32_t decl_spec = aven_c_ast_parse_declaration_specifier(
            ctx
        );
        if (decl_spec == 0) {
            break;
        }
        count += 1;
    }
    aven_c_ast_restore(ctx, state);
    return count;
}

typedef struct {
    uint32_t decl_spec_list;
    bool abstract;
} AvenCAstDsl;

static inline AvenCAstDsl aven_c_ast_parse_declaration_specifier_list(
    AvenCAstCtx *ctx,
    bool abstract
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
    for (;;) {
        uint32_t decl_spec = aven_c_ast_parse_declaration_specifier(ctx);
        if (decl_spec == 0) {
            break;
        }
        list_push(ctx->scratch) = decl_spec;
        last = cur;
        cur = aven_c_ast_save(ctx);
        decl_spec_count += 1;
    }
    if (decl_spec_count == 0) {
        aven_c_ast_restore(ctx, state);
        return (AvenCAstDsl){ 0 };
    }
    if (abstract) {
        aven_c_ast_parse_abstract_declarator(ctx);
        if (ctx->token_index > best_index) {
            best_index = ctx->token_index;
            best_option = AVEN_C_AST_PARSE_DSL_OPT_SPEC_ABDECL;
        }
        aven_c_ast_restore(ctx, cur);
    }
    {
        uint32_t decl = aven_c_ast_parse_declarator(ctx);
        if (decl != 0 and ctx->token_index > best_index) {
            best_index = ctx->token_index;
            best_option = AVEN_C_AST_PARSE_DSL_OPT_SPEC_DECL;
        }
        aven_c_ast_restore(ctx, last);
    }
    if (abstract and decl_spec_count > 1) {
        aven_c_ast_parse_abstract_declarator(ctx);
        if (ctx->token_index > best_index) {
            best_index = ctx->token_index;
            best_option = AVEN_C_AST_PARSE_DSL_OPT_ABDECL;
        }
        aven_c_ast_restore(ctx, last);
    }
    if (decl_spec_count > 1) {
        uint32_t decl = aven_c_ast_parse_declarator(ctx);
        if (decl != 0 and ctx->token_index > best_index) {
            best_index = ctx->token_index;
            best_option = AVEN_C_AST_PARSE_DSL_OPT_DECL;
        }
        aven_c_ast_restore(ctx, last);
    }
    if (abstract) {
        uint32_t id = aven_c_ast_parse_identifier(ctx);
        if (id != 0) {
            aven_c_ast_parse_abstract_declarator(ctx);
            if (ctx->token_index > best_index) {
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
            if (decl != 0 and ctx->token_index > best_index) {
                best_index = ctx->token_index;
                best_option = AVEN_C_AST_PARSE_DSL_OPT_ID_DECL;
            }
            aven_c_ast_restore(ctx, last);
        }
    }
    bool next_abs = false;
    switch (best_option) {
        case AVEN_C_AST_PARSE_DSL_OPT_NONE: {
            aven_c_ast_restore(ctx, state);
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

static inline uint32_t aven_c_ast_parse_parameter_type_list(AvenCAstCtx *ctx) {
    uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
    for (;;) {
        uint32_t pdecl_node = aven_c_ast_parse_parameter_declaration(ctx);
        if (pdecl_node == 0) {
            pdecl_node = aven_c_ast_parse_punctuator(ctx, aven_str("..."));
        }
        if (pdecl_node == 0) {
            break;
        }
        list_push(ctx->scratch) = pdecl_node;
        if (!aven_c_ast_match_punctuator(ctx, aven_str(","))) {
            break;
        }
    }
    uint32_t pdecl_list = aven_c_ast_scratch_commit(ctx, scratch_top);
    return pdecl_list;
}

static inline uint32_t aven_c_ast_parse_designator(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    if (aven_c_ast_match_punctuator(ctx, aven_str("["))) {
        uint32_t node = aven_c_ast_parse_const_expr(ctx);
        if (node == 0) {
            aven_c_ast_restore(ctx, state);
            return false;
        }
        uint32_t end_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, aven_str("]"))) {
            node = 0;
        }
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
    if (aven_c_ast_match_punctuator(ctx, aven_str("."))) {
        uint32_t node = aven_c_ast_parse_identifier(ctx);
        if (node == 0) {
            aven_c_ast_restore(ctx, state);
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
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t main_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("="))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t rhs = aven_c_ast_parse_initializer_list(ctx);
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
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
    if (!aven_c_ast_match_punctuator(ctx, aven_str("{"))) {
        return aven_c_ast_parse_assign_expr(ctx);
    }
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
            if (!aven_c_ast_match_punctuator(ctx, aven_str(","))) {
                break;
            }
        }
        list = aven_c_ast_scratch_commit(ctx, scratch_top);
    }
    if (list == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t close_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("}"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
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
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("*"))) {
        aven_c_ast_restore(ctx, state);
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
    uint32_t parent
) {
    AvenCToken next_token = aven_c_ast_next(ctx);
    if (next_token.type != AVEN_C_TOKEN_TYPE_PNC or next_token.len != 1) {
        return 0;
    }
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    AvenStr tstr = aven_c_token_str(ctx->tset, main_token);
    uint32_t node = 0;
    switch (tstr.ptr[0]) {
        case '[': {
            aven_c_ast_inc_index(ctx);
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
                        aven_str("*")
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
                    break;
                }
                arg_list = aven_c_ast_scratch_commit(ctx, scratch_top);
            }
            uint32_t end_token = aven_c_ast_next_index(ctx);
            if (!aven_c_ast_match_punctuator(ctx, aven_str("]"))) {
                break;
            }
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
            aven_c_ast_inc_index(ctx);
            uint32_t plist = aven_c_ast_parse_parameter_type_list(ctx);
            if (plist != 0) {
                uint32_t end_token = aven_c_ast_next_index(ctx);
                if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
                    break;
                }
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
            if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
                break;
            }
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
        default: {
            break;
        }
    }
    if (node == 0) {
        aven_c_ast_restore(ctx, state);
    }
    return node;
}

static inline uint32_t aven_c_ast_parse_direct_declarator(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t node = 0;
    uint32_t main_token = aven_c_ast_next_index(ctx);
    if (aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        uint32_t decl = aven_c_ast_parse_declarator(ctx);
        if (decl == 0) {
            aven_c_ast_restore(ctx, state);
            return false;
        }
        uint32_t end_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
            aven_c_ast_restore(ctx, state);
            return false;
        }
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
            node
        );
        if (suffix_node == 0) {
            aven_c_ast_restore(ctx, state);
            return false;
        }
        node = suffix_node;
    }
    if (node == 0) {
        node = aven_c_ast_parse_identifier(ctx);
    }
    if (node == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    for (;;) {
        uint32_t suffix_node = aven_c_ast_parse_direct_declarator_op(
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
        aven_c_ast_restore(ctx, state);
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
    uint32_t parent
) {
    AvenCToken next_token = aven_c_ast_next(ctx);
    if (next_token.type != AVEN_C_TOKEN_TYPE_PNC or next_token.len != 1) {
        return 0;
    }
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    AvenStr tstr = aven_c_token_str(ctx->tset, main_token);
    uint32_t node = 0;
    switch (tstr.ptr[0]) {
        case '[': {
            aven_c_ast_inc_index(ctx);
            bool stat = false;
            bool ptr = false;
            bool expr = false;
            uint32_t arg_list = 0;
            {
                uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
                {
                    uint32_t ptr_node = aven_c_ast_parse_punctuator(
                        ctx,
                        aven_str("*")
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
                        break;
                    }
                }
                arg_list = aven_c_ast_scratch_commit(ctx, scratch_top);
            }
            uint32_t end_token = aven_c_ast_next_index(ctx);
            if (!aven_c_ast_match_punctuator(ctx, aven_str("]"))) {
                break;
            }
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
            aven_c_ast_inc_index(ctx);
            uint32_t plist = aven_c_ast_parse_parameter_type_list(ctx);
            uint32_t end_token = aven_c_ast_next_index(ctx);
            if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
                break;
            }
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
    if (node == 0) {
        aven_c_ast_restore(ctx, state);
    }
    return node;
}

static inline uint32_t aven_c_ast_parse_direct_abstract_declarator(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    uint32_t node = 0;
    if (aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        uint32_t decl = aven_c_ast_parse_abstract_declarator(ctx);
        if (decl == 0) {
            aven_c_ast_restore(ctx, state);
            return false;
        }
        uint32_t end_token = aven_c_ast_next_index(ctx);
        if (aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
            aven_c_ast_restore(ctx, state);
            return false;
        }
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
        uint32_t suffix_node = aven_c_ast_parse_direct_abstract_declarator_op(
            ctx,
            node
        );
        if (suffix_node == 0) {
            aven_c_ast_restore(ctx, state);
            return false;
        }
        node = suffix_node;
    }
    for (;;) {
        uint32_t suffix_node = aven_c_ast_parse_direct_abstract_declarator_op(
            ctx,
            node
        );
        if (suffix_node == 0) {
            break;
        }
        node = suffix_node;
    }

    if (node == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }

    return node;
}

static inline uint32_t aven_c_ast_parse_abstract_declarator(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    uint32_t lhs = aven_c_ast_parse_pointer(ctx);
    uint32_t rhs = aven_c_ast_parse_direct_abstract_declarator(ctx);
    if (lhs == 0 and rhs == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
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
    if (!aven_c_ast_match_punctuator(ctx, aven_str("="))) {
        return decl_node;
    }
    uint32_t init_node = aven_c_ast_parse_initializer_list(ctx);
    if (init_node == 0) {
        aven_c_ast_restore(ctx, state);
        return decl_node;        
    }
    return aven_c_ast_push(
        ctx,
        AVEN_C_AST_NODE_TYPE_INIT_DECLARATOR,
        main_token,
        decl_node,
        init_node
    );
}

static inline uint32_t aven_c_ast_parse_init_declarator_list(AvenCAstCtx *ctx) {
    uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
    for (;;) {
        uint32_t init_decl = aven_c_ast_parse_init_declarator(ctx);
        if (init_decl == 0) {
            break;
        }
        list_push(ctx->scratch) = init_decl;
        if (!aven_c_ast_match_punctuator(ctx, aven_str(","))) {
            break;
        }
    }
    return aven_c_ast_scratch_commit(ctx, scratch_top);
}

static inline uint32_t aven_c_ast_parse_declaration_unterminated(AvenCAstCtx *ctx) {
    uint32_t main_token = aven_c_ast_next_index(ctx);
    uint32_t static_ass_node = aven_c_ast_parse_static_assert_declaration(ctx);
    if (static_ass_node != 0) {
        return static_ass_node;
    }
    AvenCAstDsl dsl = aven_c_ast_parse_declaration_specifier_list(
        ctx,
        false
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

static inline uint32_t aven_c_ast_parse_declaration(
    AvenCAstCtx *ctx
) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t unterm = aven_c_ast_parse_declaration_unterminated(ctx);
    if (unterm == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t term = aven_c_ast_parse_terminated_line(ctx, unterm);
    if (term == 0) {
        aven_c_ast_restore(ctx, state);
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

    uint32_t open_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        return 0;
    }
    exp_node = aven_c_ast_parse_expr(ctx);
    uint32_t close_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
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
    if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t type_node = aven_c_ast_parse_type_name(ctx) ;
    if (type_node == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t end_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
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
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    uint32_t node = 0;
    switch (nchar) {
        case '[': {
            if (!aven_c_ast_match_punctuator(ctx, aven_str("["))) {
                break;
            }
            uint32_t arg = aven_c_ast_parse_expr(ctx);
            uint32_t end_token = aven_c_ast_next_index(ctx);
            if (!aven_c_ast_match_punctuator(ctx, aven_str("]"))) {
                arg = 0;
            }
            if (arg == 0) {
                aven_c_ast_restore(ctx, state);
                break;
            }
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
        case '(': {
            if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
                break;
            }
            uint32_t arg_list = 0;
            {
                uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
                for (;;) {
                    uint32_t arg = aven_c_ast_parse_assign_expr(ctx);
                    if (arg == 0) {
                        break;
                    }
                    list_push(ctx->scratch) = arg;
                    if (!aven_c_ast_match_punctuator(ctx, aven_str(","))) {
                        break;
                    }
                }
                arg_list = aven_c_ast_scratch_commit(ctx, scratch_top);
            }
            uint32_t end_token = aven_c_ast_next_index(ctx);
            if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
                aven_c_ast_restore(ctx, state);
                break;
            }
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
        case '.': {
            if (!aven_c_ast_match_punctuator(ctx, aven_str("."))) {
                break;
            }
            uint32_t id_node = aven_c_ast_parse_identifier(ctx);
            if (id_node == 0) {
                aven_c_ast_restore(ctx, state);
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
        case '-': {
            if (aven_c_ast_match_punctuator(ctx, aven_str("->"))) {
                uint32_t id_node = aven_c_ast_parse_identifier(ctx);
                if (id_node == 0) {
                    aven_c_ast_restore(ctx, state);
                    break;
                }
                node = aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_BOP,
                    main_token,
                    parent,
                    id_node
                );
            } else if (aven_c_ast_match_punctuator(ctx, aven_str("--"))) {
                node = aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_UOP,
                    main_token,
                    parent,
                    0
                );
            }
            break;
        }
        case '+': {
            if (!aven_c_ast_match_punctuator(ctx, aven_str("++"))) {
                break;
            }
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
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t node = aven_c_ast_parse_postfix_expr_initializer(ctx);
    if (node == 0) {
        node = aven_c_ast_parse_primary_expr(ctx);
    }
    if (node == 0) {
        aven_c_ast_restore(ctx, state);
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
    uint32_t node = 0;
    uint32_t main_token = aven_c_ast_next_index(ctx);
    AvenCTokenType token_type = aven_c_ast_next(ctx).type;
    AvenStr token_str = aven_c_token_str(ctx->tset, main_token);
    switch (token_type) {
        case AVEN_C_TOKEN_TYPE_KEY: {
            bool sizeof_op = aven_c_ast_match_keyword(
                ctx,
                AVEN_C_KEYWORD_SIZEOF
            );
            bool alignof_op = !sizeof_op and aven_c_ast_match_keyword(
                ctx,
                AVEN_C_KEYWORD_ALIGNOF
            );
            if (!(sizeof_op or alignof_op)) {
                aven_c_ast_restore(ctx, state);
                break;
            }
            uint32_t open_token = aven_c_ast_next_index(ctx);
            if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
                aven_c_ast_restore(ctx, state);
                break;
            }
            uint32_t child = aven_c_ast_parse_expr(ctx);
            if (child == 0) {
                child = aven_c_ast_parse_type_name(ctx);
            }
            if (child == 0) {
                aven_c_ast_restore(ctx, state);
                break;
            }
            uint32_t close_token = aven_c_ast_next_index(ctx);
            if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
                aven_c_ast_restore(ctx, state);
                break;
            }
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
            if (token_str.len != 1) {
                if (
                    !(
                        aven_c_ast_match_punctuator(ctx, aven_str("++")) or
                        aven_c_ast_match_punctuator(ctx, aven_str("--"))
                    )
                ) {
                    aven_c_ast_restore(ctx, state);
                    break;
                }
                uint32_t child = aven_c_ast_parse_unary_expr(ctx);
                if (child == 0) {
                    aven_c_ast_restore(ctx, state);
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
                            aven_c_ast_restore(ctx, state);
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
    if (node == 0) {
        aven_c_ast_restore(ctx, state);
    }
    return node;
}

static inline uint32_t aven_c_ast_parse_cast_expr(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        return aven_c_ast_parse_unary_expr(ctx);
    }
    uint32_t lhs = aven_c_ast_parse_type_name(ctx);
    if (lhs == 0) {
        aven_c_ast_restore(ctx, state);
        return aven_c_ast_parse_unary_expr(ctx);
    }
    uint32_t end_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
        aven_c_ast_restore(ctx, state);
        return aven_c_ast_parse_unary_expr(ctx);
    }
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
    if (aven_c_ast_next(ctx).len != 1) {
        return 0;
    }
    if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_PNC) {
        return 0;
    }
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    uint32_t rhs = 0;
    switch (nchar) {
        case '*':
        case '/':
        case '%': {
            aven_c_ast_inc_index(ctx);
            rhs = aven_c_ast_parse_cast_expr(ctx);
            break;
        }
        default: {
            break;
        }
    }
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
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
    if (aven_c_ast_next(ctx).len != 1) {
        return 0;
    }
    if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_PNC) {
        return 0;
    }
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    uint32_t rhs = 0;
    switch (nchar) {
        case '+':
        case '-': {
            aven_c_ast_inc_index(ctx);
            rhs = aven_c_ast_parse_multiply_expr(ctx);
            break;
        }
        default: {
            break;
        }
    }
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
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
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    uint32_t rhs = 0;
    switch (nchar) {
        case '<': {
            if (aven_c_ast_match_punctuator(ctx, aven_str("<<"))) {
                rhs = aven_c_ast_parse_add_expr(ctx);
            }
            break;
        }
        case '>': {
            if (aven_c_ast_match_punctuator(ctx, aven_str(">>"))) {
                rhs = aven_c_ast_parse_add_expr(ctx);
            }
            break;
        }
        default: {
            break;
        }
    }
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
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
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    uint32_t rhs = 0;
    switch (nchar) {
        case '>': {
            if (
                aven_c_ast_match_punctuator(ctx, aven_str(">=")) or
                aven_c_ast_match_punctuator(ctx, aven_str(">"))
            ) {
                rhs = aven_c_ast_parse_shift_expr(ctx);
            }
            break;
        }
        case '<': {
            if (
                aven_c_ast_match_punctuator(ctx, aven_str("<=")) or
                aven_c_ast_match_punctuator(ctx, aven_str("<"))
            ) {
                rhs = aven_c_ast_parse_shift_expr(ctx);
            }
            break;
        }
        default: {
            break;
        }
    }
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
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
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    uint32_t rhs = 0;
    switch (nchar) {
        case '=': {
            if (aven_c_ast_match_punctuator(ctx, aven_str("=="))) {
                rhs = aven_c_ast_parse_relate_expr(ctx);
            }
            break;
        }
        case '!': {
            if (aven_c_ast_match_punctuator(ctx, aven_str("!="))) {
                rhs = aven_c_ast_parse_relate_expr(ctx);
            }
            break;
        }
        default: {
            break;
        }
    }
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
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
    if (!aven_c_ast_match_punctuator(ctx, aven_str("&"))) {
        return 0;
    }
    uint32_t rhs = aven_c_ast_parse_equal_expr(ctx);
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
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
    if (!aven_c_ast_match_punctuator(ctx, aven_str("^"))) {
        return 0;
    }
    uint32_t rhs = aven_c_ast_parse_and_expr(ctx);
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
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
    if (!aven_c_ast_match_punctuator(ctx, aven_str("|"))) {
        return 0;
    }
    uint32_t rhs = aven_c_ast_parse_xor_expr(ctx);
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
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
            aven_c_ast_match_punctuator(ctx, aven_str("&&")) or
            aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_AND)
        )
    ) {
        return 0;
    }
    uint32_t rhs = aven_c_ast_parse_or_expr(ctx);
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
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
            aven_c_ast_match_punctuator(ctx, aven_str("||")) or
            aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_OR)
        )
    ) {
        return 0;
    }
    uint32_t rhs = aven_c_ast_parse_logical_and_expr(ctx);
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
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
    if (!aven_c_ast_match_punctuator(ctx, aven_str("?"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t nodes = 0;
    {
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        uint32_t middle = aven_c_ast_parse_expr(ctx);
        if (middle == 0) {
            aven_c_ast_restore(ctx, state);
            return 0;
        }
        list_push(ctx->scratch) = middle;
        colo_token = aven_c_ast_next_index(ctx);
        if (!aven_c_ast_match_punctuator(ctx, aven_str(":"))) {
            aven_c_ast_restore(ctx, state);
            return 0;
        }
        uint32_t rhs = aven_c_ast_parse_logical_or_expr(ctx);
        if (rhs == 0) {
            aven_c_ast_restore(ctx, state);
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
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    uint32_t rhs = 0;
    switch (nchar) {
        case '=': {
            if (aven_c_ast_match_punctuator(ctx, aven_str("="))) {
                rhs = aven_c_ast_parse_conditional_expr(ctx);
            }
            break;
        }
        case '*': {
            if (aven_c_ast_match_punctuator(ctx, aven_str("*="))) {
                rhs = aven_c_ast_parse_conditional_expr(ctx);
            }
            break;
        }
        case '/': {
            if (aven_c_ast_match_punctuator(ctx, aven_str("/="))) {
                rhs = aven_c_ast_parse_conditional_expr(ctx);
            }
            break;
        }
        case '%': {
            if (aven_c_ast_match_punctuator(ctx, aven_str("%="))) {
                rhs = aven_c_ast_parse_conditional_expr(ctx);
            }
            break;
        }
        case '+': {
            if (aven_c_ast_match_punctuator(ctx, aven_str("+="))) {
                rhs = aven_c_ast_parse_conditional_expr(ctx);
            }
            break;
        }
        case '-': {
            if (aven_c_ast_match_punctuator(ctx, aven_str("-="))) {
                rhs = aven_c_ast_parse_conditional_expr(ctx);
            }
            break;
        }
        case '<': {
            if (aven_c_ast_match_punctuator(ctx, aven_str("<<="))) {
                rhs = aven_c_ast_parse_conditional_expr(ctx);
            }
            break;
        }
        case '>': {
            if (aven_c_ast_match_punctuator(ctx, aven_str(">>="))) {
                rhs = aven_c_ast_parse_conditional_expr(ctx);
            }
            break;
        }
        case '&': {
            if (aven_c_ast_match_punctuator(ctx, aven_str("&="))) {
                rhs = aven_c_ast_parse_conditional_expr(ctx);
            }
            break;
        }
        case '^': {
            if (aven_c_ast_match_punctuator(ctx, aven_str("^="))) {
                rhs = aven_c_ast_parse_conditional_expr(ctx);
            }
            break;
        }
        case '|': {
            if (aven_c_ast_match_punctuator(ctx, aven_str("|="))) {
                rhs = aven_c_ast_parse_conditional_expr(ctx);
            }
            break;
        }
        default: {
            break;
        }
    }
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
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

static inline uint32_t aven_c_ast_parse_comma_op(
    AvenCAstCtx *ctx,
    uint32_t lhs
) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(","))) {
        return 0;
    }
    uint32_t rhs = aven_c_ast_parse_assign_expr(ctx);
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    return aven_c_ast_push(
        ctx,
        AVEN_C_AST_NODE_TYPE_EXPR,
        main_token,
        lhs,
        rhs
    );
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
        if (!aven_c_ast_match_punctuator(ctx, aven_str(","))) {
            break;
        }
        uint32_t next = aven_c_ast_parse_assign_expr(ctx);
        if (next == 0) {
            aven_c_ast_restore(ctx, state);
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
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t expr_node = aven_c_ast_parse_const_expr(ctx);
    if (expr_node == 0) {
        aven_c_ast_restore(ctx, state);
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
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t main_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(":"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t rhs = aven_c_ast_parse_statement(ctx);
    if (rhs == 0) {
        rhs = aven_c_ast_parse_statement(ctx);
    }
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
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
    if (!aven_c_ast_match_punctuator(ctx, aven_str("{"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
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
    if (!aven_c_ast_match_punctuator(ctx, aven_str("}"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
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
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t lhs = aven_c_ast_parse_statement(ctx);
    if (lhs == 0) {
        aven_c_ast_restore(ctx, state);
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
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t start_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t lhs = aven_c_ast_parse_expr(ctx);
    if (lhs == 0) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t end_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t rhs = 0;
    {
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        uint32_t statement = aven_c_ast_parse_statement(ctx);
        if (statement == 0) {
            aven_c_ast_restore(ctx, state);
            return false;
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
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t start_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t lhs = aven_c_ast_parse_expr(ctx);
    if (lhs == 0) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t end_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t rhs = aven_c_ast_parse_statement(ctx);
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
        return false;
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

static inline uint32_t aven_c_ast_parse_selection_statement(AvenCAstCtx *ctx) {
    uint32_t node = aven_c_ast_parse_if_statement(ctx);
    if (node == 0) {
        node = aven_c_ast_parse_switch_statement(ctx);
    }
    return node;
}

static inline uint32_t aven_c_ast_parse_while_statement(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_WHILE)) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t start_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t lhs = aven_c_ast_parse_expr(ctx);
    if (lhs == 0) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t end_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t rhs = aven_c_ast_parse_statement(ctx);
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
        return false;
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
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t lhs = aven_c_ast_parse_statement(ctx);
    if (lhs == 0) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t while_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_WHILE)) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t start_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t rhs = aven_c_ast_parse_expr(ctx);
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t end_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
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

static inline uint32_t aven_c_ast_parse_do_statement(
    AvenCAstCtx *ctx
) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t unterm = aven_c_ast_parse_do_statement_unterminated(ctx);
    if (unterm == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t term = aven_c_ast_parse_terminated_line(ctx, unterm);
    if (term == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    return term;
}

static inline uint32_t aven_c_ast_parse_for_statement(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_keyword(ctx, AVEN_C_KEYWORD_FOR)) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t open_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t lhs = 0;
    {
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        uint32_t count = 0;
        AvenCAstCtxState pre_decl_state = aven_c_ast_save(ctx);
        uint32_t decl_node = aven_c_ast_parse_declaration_unterminated(ctx);
        if (decl_node != 0) {
            if (!aven_c_ast_match_punctuator(ctx, aven_str(";"))) {
                aven_c_ast_restore(ctx, pre_decl_state);
            } else {
                list_push(ctx->scratch) = decl_node;
                count += 1;
            }
        }
        for (; count < 2; count += 1) {
            uint32_t expr_node = aven_c_ast_parse_expr(ctx);
            list_push(ctx->scratch) = expr_node;
            if (!aven_c_ast_match_punctuator(ctx, aven_str(";"))) {
                aven_c_ast_restore(ctx, state);
                return false;
            }
        }
        uint32_t expr_node = aven_c_ast_parse_expr(ctx);
        list_push(ctx->scratch) = expr_node;
        lhs = aven_c_ast_scratch_commit(ctx, scratch_top);
    }
    uint32_t close_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t rhs = aven_c_ast_parse_statement(ctx);
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
        return false;
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

static inline uint32_t aven_c_ast_parse_iteration_statement(AvenCAstCtx *ctx) {
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
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t rhs = aven_c_ast_parse_identifier(ctx);
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
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
        aven_c_ast_restore(ctx, state);
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

static inline uint32_t aven_c_ast_parse_jump_statement_unterminated(AvenCAstCtx *ctx) {
    uint32_t node = aven_c_ast_parse_keyword(ctx, AVEN_C_KEYWORD_CONTINUE);
    if (node == 0) {
        node = aven_c_ast_parse_keyword(ctx, AVEN_C_KEYWORD_BREAK);
    }
    if (node == 0) {
        node = aven_c_ast_parse_goto_statement(ctx);
    }
    if (node == 0) {
        node = aven_c_ast_parse_return_statement(ctx);
    }
    return node;
}

static inline uint32_t aven_c_ast_parse_jump_statement(
    AvenCAstCtx *ctx
) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t unterm = aven_c_ast_parse_jump_statement_unterminated(ctx);
    if (unterm == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t term = aven_c_ast_parse_terminated_line(ctx, unterm);
    if (term == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    return term;
}

static inline uint32_t aven_c_ast_parse_expr_statement(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t unterm = aven_c_ast_parse_expr(ctx);
    uint32_t term = aven_c_ast_parse_terminated_line(ctx, unterm);
    if (term == 0) {
        aven_c_ast_restore(ctx, state);
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

static inline uint32_t aven_c_ast_parse_function_definition(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    AvenCAstDsl dsl = aven_c_ast_parse_declaration_specifier_list(
        ctx,
        false
    );
    if (dsl.decl_spec_list == 0) {
        return 0;
    }
    uint32_t declarator = aven_c_ast_parse_declarator(ctx);
    if (declarator == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t statement = aven_c_ast_parse_compound_statement(ctx);
    if (statement == 0) {
        aven_c_ast_restore(ctx, state);
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

static inline AvenCAstResult aven_c_ast_parse(AvenCTokenSet tset, AvenArena *arena) {
    AvenArena temp_arena = *arena;
    AvenCAstCtx ctx = aven_c_ast_init(tset, &temp_arena);
    aven_c_ast_parse_ppd_tokens(&ctx);
    aven_c_ast_inc_index(&ctx);
    uint32_t root = aven_c_ast_parse_translation_unit(&ctx);
    bool normal_error = root == 0 or ctx.token_index <= ctx.tset.tokens.len - 1;
    if (normal_error or ctx.ppd_error.valid) {
        AvenCToken token = get(ctx.tset.tokens, ctx.error.token);
        AvenCTokenLoc eloc = aven_c_token_loc(ctx.tset, ctx.error.token);
        AvenStr token_str = aven_c_token_str(tset, ctx.error.token);
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
            exp_token_type = ppd_error.type;
            exp_token_str = ppd_error.exp;
            eloc = aven_c_ppd_token_loc(
                ctx.tset,
                ppd_error.pp_token - 1
            );
            token_str = aven_c_ppd_token_str(tset, ppd_error.pp_token - 1);
            token = get(ctx.tset.ppd_tokens, ppd_error.pp_token - 1);
            token_index = token.index;
            if (token.type == AVEN_C_TOKEN_TYPE_NONE) {
                token = get(ctx.tset.ppd_tokens, ppd_error.pp_token - 2);
                token_str = aven_c_ppd_token_str(tset, ppd_error.pp_token - 2);
                eloc = aven_c_ppd_token_loc(ctx.tset, ppd_error.pp_token - 2);
                eloc.col += token.len;
                token_index = token.index + token.len;
            }
        } else if (token.type == AVEN_C_TOKEN_TYPE_NONE) {
            token = get(ctx.tset.tokens, ctx.error.token - 1);
            token_str = aven_c_token_str(tset, ctx.error.token - 1);
            eloc = aven_c_token_loc(ctx.tset, ctx.error.token - 1);
            eloc.col += token.len;
            token_index = token.index + token.len;
        }
        AvenStr exp_type = exp_token_type == AVEN_C_TOKEN_TYPE_NUM ?
            aven_str("constant") :
            aven_c_token_type_str(exp_token_type);
        AvenStr act_type = aven_c_token_type_str(token.type);
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
        char arrow_buffer[132];
        AvenStr arrow_str = slice_array(arrow_buffer);
        size_t arrow_len = 0;
        size_t ln_digits = aven_fmt_uint_decimal_digits(eloc.line);
        assert(ln_digits < 32);
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
                arena,
                "error at {}:{}: expected {} '{}', found {} '{}'\n"
                "{}:    {}\n    {}",
                aven_fmt_uint(eloc.line),
                aven_fmt_uint(eloc.col),
                aven_fmt_str(exp_type),
                aven_fmt_str(exp_str.value),
                aven_fmt_str(act_type),
                aven_fmt_str(token_str),
                aven_fmt_uint(eloc.line),
                aven_fmt_str(line),
                aven_fmt_str(arrow_str)
            ) :
            aven_fmt(
                arena,
                "error at {}:{}: expected {}, found {} '{}'\n"
                "{}:    {}\n    {}",
                aven_fmt_uint(eloc.line),
                aven_fmt_uint(eloc.col),
                aven_fmt_str(exp_type),
                aven_fmt_str(act_type),
                aven_fmt_str(token_str),
                aven_fmt_uint(eloc.line),
                aven_fmt_str(line),
                aven_fmt_str(arrow_str)
            );
        return (AvenCAstResult) {
            .type = AVEN_C_AST_RESULT_TYPE_ERROR,
            .data = {
                .error = error_str
            },
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

static inline uint32_t aven_c_ast_parse_preprocessor_header(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("<"))) {
        aven_c_ast_restore(ctx, state);
        return false;
    }
    uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
    list_push(ctx->scratch) = main_token;
    for (;;) {
        uint32_t end_token = aven_c_ast_next_index(ctx);
        if (aven_c_ast_match_punctuator(ctx, aven_str(">"))) {
            list_push(ctx->scratch) = end_token;
            break;
        };
        if (aven_c_ast_next(ctx).type == AVEN_C_TOKEN_TYPE_NONE) {
            aven_c_ast_restore(ctx, state);
            return false;
        }
        list_push(ctx->scratch) = aven_c_ast_inc_index(ctx);
    }
    return aven_c_ast_push_leaf(
        ctx,
        AVEN_C_AST_NODE_TYPE_PREPROCESSOR_HEADER,
        aven_c_ast_scratch_commit(ctx, scratch_top)
    );
}

static inline uint32_t aven_c_ast_parse_preprocessor_define(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    AvenStr id_token_str = aven_c_token_str(ctx->tset, main_token);
    AvenCToken id_token = aven_c_ast_next(ctx);
    if (
        id_token.type != AVEN_C_TOKEN_TYPE_ID or
        !aven_str_equals(id_token_str, aven_str("define"))
    ) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    aven_c_ast_inc_index(ctx);
    AvenCToken name_token = aven_c_ast_next(ctx);
    uint32_t name_node = aven_c_ast_parse_identifier(ctx);
    if (name_node == 0 and aven_c_ast_next(ctx).type == AVEN_C_TOKEN_TYPE_KEY) {
        name_node = aven_c_ast_push_leaf(
            ctx,
            AVEN_C_AST_NODE_TYPE_KEYWORD,
            aven_c_ast_inc_index(ctx)
        );
    }
    if (name_node == 0) {
        aven_c_ast_restore(ctx, state);
        return 0; 
    }
    // no space between name and '(' for function macro
    AvenCToken paren_token = aven_c_ast_next(ctx);
    uint32_t open_token = aven_c_ast_next_index(ctx);
    if (
        paren_token.index != name_token.index + name_token.len or
        !aven_c_ast_match_punctuator(ctx, aven_str("("))
    ) {
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_PREPROCESSOR_DEFINE,
            main_token,
            name_node,
            0
        );
    }
    uint32_t arg_list = 0;
    {
        uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
        for (;;) {
            uint32_t arg = aven_c_ast_parse_const_expr(ctx);
            if (arg == 0) {
                arg = aven_c_ast_parse_punctuator(ctx, aven_str("..."));
            }
            if (arg == 0) {
                break;
            }
            list_push(ctx->scratch) = arg;
            if (!aven_c_ast_match_punctuator(ctx, aven_str(","))) {
                break;
            }
        }
        arg_list = aven_c_ast_scratch_commit(ctx, scratch_top);
    }
    uint32_t close_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
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

static inline uint32_t aven_c_ast_parse_preprocessor_directive(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t main_token = aven_c_ast_next_index(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("#"))) {
        return false;
    }
    uint32_t lhs = aven_c_ast_parse_preprocessor_define(ctx);
    if (lhs == 0) {
        lhs = aven_c_ast_parse_identifier(ctx);
    }
    if (lhs == 0) {
        lhs = aven_c_ast_parse_keyword(ctx, AVEN_C_KEYWORD_IF);
    }
    if (lhs == 0) {
        lhs = aven_c_ast_parse_keyword(ctx, AVEN_C_KEYWORD_ELSE);
    }
    if (lhs == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    AvenCAstCtxState rhs_state = aven_c_ast_save(ctx);
    uint32_t rhs = aven_c_ast_parse_declaration_unterminated(ctx);
    if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_NONE) {
        rhs = 0;
        aven_c_ast_restore(ctx, rhs_state);
    }
    if (rhs == 0) {
        rhs = aven_c_ast_parse_expr(ctx);
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
        rhs = aven_c_ast_parse_preprocessor_header(ctx);
        if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_NONE) {
            rhs = 0;
            aven_c_ast_restore(ctx, rhs_state);
        }
    }
    if (rhs == 0 and aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_NONE) {
        rhs = aven_c_ast_push_leaf(
            ctx,
            AVEN_C_AST_NODE_TYPE_ANYTOKEN,
            aven_c_ast_inc_index(ctx)
        );
        if (aven_c_ast_next(ctx).type != AVEN_C_TOKEN_TYPE_NONE) {
            rhs = 0;
            aven_c_ast_restore(ctx, rhs_state);
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
    AvenCAstRenderCtx* ctx
) {
    return (AvenCAstRenderCtxState){
        .cursor = ctx->cursor,
        .indent = ctx->indent,
        .ppd = ctx->ppd,
    };
}

static inline void aven_c_ast_render_restore(
    AvenCAstRenderCtx* ctx,
    AvenCAstRenderCtxState state
) {
    ctx->cursor = state.cursor;
    ctx->indent = state.indent;
    ctx->ppd = state.ppd;
}

static inline bool aven_c_ast_render_write(
    AvenCAstRenderCtx* ctx,
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
    AvenIoResult res = aven_io_writer_push(ctx->writer, slice_as_bytes(line));
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

#define aven_c_ast_render_token_try(c, i, s, l) \
    do { \
        if (!aven_c_ast_render_token_try_internal(c, i, s, false)) { \
            aven_c_ast_render_restore(c, l); \
            return false; \
        } \
    } while (0)
#define aven_c_ast_render_token_force_try(c, i, s, l) \
    do { \
        if (!aven_c_ast_render_token_try_internal(c, i, s, true)) { \
            aven_c_ast_render_restore(c, l); \
            return false; \
        } \
    } while (0)
#define aven_c_ast_render_space_try(c, s, l) \
    do { \
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
    bool split_same,
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
    AvenStr token_str = ctx->ppd ?
        aven_c_ppd_token_str(ctx->ast->tset, token) :
        aven_c_token_str(ctx->ast->tset, token);
    if (!aven_c_ast_render_write(ctx, token_str, force)) {
        return false;
    }
    if (!ctx->ppd) {
        ctx->trailing_lines = get(
            ctx->ast->tset.tokens,
            token
        ).trailing_lines;
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
    if (!aven_c_ast_render_node(ctx, parent_type, index, split_same, false)) {
        if (!split) {
            return false;
        }
        uint32_t lines_written = ctx->lines_written;
        if (
            !aven_c_ast_render_node(
                ctx,
                parent_type,
                index,
                split_same,
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
                !aven_c_ast_render_node(
                    ctx,
                    parent_type,
                    index,
                    split_same,
                    false
                )
            ) {
                if (
                    !aven_c_ast_render_node(
                        ctx,
                        parent_type,
                        index,
                        split_same,
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

static inline bool aven_c_ast_render_node_surround_try_internal(
    AvenCAstRenderCtx *ctx,
    AvenCAstNodeType parent_type,
    uint32_t index,
    uint32_t open_token,
    uint32_t close_token,
    bool split_same,
    bool split
) {
    if (!aven_c_ast_render_token_try_internal(ctx, open_token, split, split)) {
        return false;
    }
    AvenCAstRenderCtxState state = aven_c_ast_render_save(ctx);
    if (
        !aven_c_ast_render_node(ctx, parent_type, index, split_same, false) or
        !aven_c_ast_render_token_try_internal(ctx, close_token, false, false)
    ) {
        if (!split) {
            return false;
        }
        aven_c_ast_render_restore(ctx, state);
        ctx->indent += 1;
        if (!aven_c_ast_render_flush_line(ctx)) {
            return false;
        }
        if (
            !aven_c_ast_render_node(
                ctx,
                parent_type,
                index,
                split_same,
                false
            )
        ) {
            if (
                !aven_c_ast_render_node(
                    ctx,
                    parent_type,
                    index,
                    split_same,
                    true
                )
            ) {
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

#define aven_c_ast_render_node_try(c, t, n, p, s, l) \
    do { \
        if (!aven_c_ast_render_node_try_internal(c, t, n, p, s)) { \
            aven_c_ast_render_restore(c, l); \
            return false; \
        } \
    } while (0)
#define aven_c_ast_render_node_surround_try(c, t, n, e, f, p, s, l) \
    do { \
        if ( \
            !aven_c_ast_render_node_surround_try_internal(c, t, n, e, f, p, s) \
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

#define aven_c_ast_render_data_try(c, t, n, p, b, s, l) \
    do { \
        if (!aven_c_ast_render_data_try_internal(c, t, n, p, b, s)) { \
            aven_c_ast_render_restore(c, l); \
            return false; \
        } \
    } while (0)
#define aven_c_ast_render_data_try_seq(c, t, n, s, l) \
    do { \
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
        aven_io_writer_push(
            ctx->writer,
            slice_as_bytes(ctx->newline_str)
        );
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
        if (!aven_c_ast_render_node(
            ctx,
            AVEN_C_AST_NODE_TYPE_NONE,
            get(ctx->ast->pp_nodes, i),
            false,
            true
            )
        ) {
            return false;
        }
        ctx->trailing_lines = token.trailing_lines;
        aven_c_ast_render_flush_line(ctx);
        aven_c_ast_render_whitespace(ctx);
    }
    ctx->pp_cursor = token_index;
    return true;
}

static inline bool aven_c_ast_render_data(
    AvenCAstRenderCtx *ctx,
    AvenCAstNodeType parent_type,
    uint32_t index,
    AvenStr sep,
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
        uint32_t cursor = ctx->cursor;
        uint32_t lines_written = ctx->lines_written;
        aven_c_ast_render_node_try(
            ctx,
            parent_type,
            get(data, i),
            false,
            split,
            state
        );
        bool written = cursor != ctx->cursor or
            lines_written != ctx->lines_written;
        if (i + 1 < data.len) {
            if (!aven_c_ast_render_write(ctx, sep, split)) {
                aven_c_ast_render_restore(ctx, state);
                return false;
            }
            if (written) {
                aven_c_ast_render_space_try(ctx, split, state);
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
        if (
            !aven_c_ast_render_node(
                ctx,
                parent_type,
                entry,
                false,
                false
            )
        ) {
            if (!split) {
                return false;
            }
            uint32_t lines_written = ctx->lines_written;
            if (
                !aven_c_ast_render_node(
                    ctx,
                    parent_type,
                    entry,
                    false,
                    true
                )
            ) {
                if (ctx->cursor == 0 or lines_written != ctx->lines_written) {
                    return false;
                }
                if (!aven_c_ast_render_flush_line(ctx)) {
                    return false;
                }
                if (
                    !aven_c_ast_render_node(
                        ctx,
                        parent_type,
                        entry,
                        false,
                        false
                    )
                ) {
                    if (
                        !aven_c_ast_render_node(
                            ctx,
                            parent_type,
                            entry,
                            false,
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
            trailing_sep,
            false
        ) or
        (spaces and !aven_c_ast_render_write(ctx, aven_str(" "), true)) or
        !aven_c_ast_render_token_try_internal(ctx, close_token, false, false)
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
    bool split_same,
    bool split_all
) {
    if (ctx->io_error != 0) {
        return false;
    }
    if (index == 0) {
        return true;
    }
    AvenCAstRenderCtxState state = aven_c_ast_render_save(ctx);
    AvenCAstNode node = aven_c_ast_node(ctx->ast, index);
    bool split = split_all or (split_same and node.type == parent_type);
    switch (node.type) {
        case AVEN_C_AST_NODE_TYPE_PREPROCESSOR_MSVC_JUSTIFICATION: {
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
        case AVEN_C_AST_NODE_TYPE_PREPROCESSOR_MSVC_WARN_SPECIFIER: {
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
            aven_c_ast_render_data_try(
                ctx,
                node.type,
                node.rhs,
                aven_str(""),
                false,
                split,
                state
            );
            break;
        }
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
        case AVEN_C_AST_NODE_TYPE_PREPROCESSOR_HEADER: {
            AvenCAstDataSlice tokens = aven_c_ast_data_get(
                ctx->ast,
                node.token
            );
            for (uint32_t i = 0; i < tokens.len; i += 1) {
                aven_c_ast_render_token_try(ctx, get(tokens, i), split, state);
            }
            break;
        }
        case AVEN_C_AST_NODE_TYPE_PREPROCESSOR_DEFINE_FN: {
            uint32_t lines_written = ctx->lines_written;
            AvenCAstDataSlice tokens = aven_c_ast_data_get(ctx->ast, node.token);
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
                pp_str = aven_c_ppd_token_str(ctx->ast->tset, dir_node.token);
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
                true,
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
            aven_c_ast_render_token_force_try(
                ctx,
                node.token,
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
        case AVEN_C_AST_NODE_TYPE_LOGICAL_OR_EXPR:
        case AVEN_C_AST_NODE_TYPE_LOGICAL_AND_EXPR:
        case AVEN_C_AST_NODE_TYPE_OR_EXPR:
        case AVEN_C_AST_NODE_TYPE_XOR_EXPR:
        case AVEN_C_AST_NODE_TYPE_AND_EXPR:
        case AVEN_C_AST_NODE_TYPE_EQUAL_EXPR:
        case AVEN_C_AST_NODE_TYPE_RELATE_EXPR:
        case AVEN_C_AST_NODE_TYPE_SHIFT_EXPR:
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
            aven_c_ast_render_token_force_try(
                ctx,
                node.token,
                split,
                state
            );
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
            AvenCAstDataSlice tokens = aven_c_ast_data_get(ctx->ast, node.token);
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
        case AVEN_C_AST_NODE_TYPE_MACRO_INVOCATION: {
            uint32_t lines_written = ctx->lines_written;
            AvenCAstDataSlice tokens = aven_c_ast_data_get(ctx->ast, node.token);
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
                split,
                false,
                state
            );
            if (indent) {
                ctx->indent -= 1;
            }
            break;
        }
        case AVEN_C_AST_NODE_TYPE_STRING_CONSTANT: {
            uint32_t lines_written = ctx->lines_written;
            bool indent = (
                parent_type == AVEN_C_AST_NODE_TYPE_ASSIGN_EXPR or
                parent_type == AVEN_C_AST_NODE_TYPE_INIT_DECLARATOR or
                parent_type == AVEN_C_AST_NODE_TYPE_DESIGNATION or
                parent_type == AVEN_C_AST_NODE_TYPE_ENUMERATOR
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
            bool trailing_nl = lines_written != ctx->lines_written and (
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
            AvenCAstDataSlice tokens = aven_c_ast_data_get(ctx->ast, node.token);
            aven_c_ast_render_data_surround_try(
                ctx,
                node.type,
                node.lhs,
                get(tokens, 0),
                get(tokens, 1),
                aven_str(","),
                true,
                true,
                split,
                false,
                state
            );
            break;
        }
        case AVEN_C_AST_NODE_TYPE_DOT_DESIGNATOR: {
            aven_c_ast_render_token_force_try(
                ctx,
                node.token,
                split,
                state
            );
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
            aven_c_ast_render_token_force_try(
                ctx,
                node.token,
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
            aven_c_ast_render_token_force_try(
                ctx,
                node.token,
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
        case AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_UOP: {
            aven_c_ast_render_node_try(
                ctx,
                node.type,
                node.lhs,
                false,
                split,
                state
            );
            aven_c_ast_render_token_force_try(
                ctx,
                node.token,
                split,
                state
            );
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
            aven_c_ast_render_token_try(
                ctx,
                node.token,
                split,
                state
            );
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
            aven_c_ast_render_token_try(
                ctx,
                get(tokens, 0),
                split,
                state
            );
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
                node.token,
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
            if (node.lhs != 0 and node.rhs != 0) {
                if (
                    ctx->cursor != 0 and get(ctx->line, ctx->cursor - 1) != '*'
                ) {
                    aven_c_ast_render_space_try(ctx, false, state);
                }
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
            aven_c_ast_render_token_force_try(
                ctx,
                node.token,
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
            aven_c_ast_render_token_force_try(ctx, get(tokens, 0), split, state);
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
                true,
                true,
                split,
                true,
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
            aven_c_ast_render_token_force_try(
                ctx,
                node.token,
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
            aven_c_ast_render_token_force_try(
                ctx,
                node.token,
                split,
                state
            );
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
            if (!split) {
                aven_c_ast_render_restore(ctx, state);
                return false;
            }
            // if (!aven_c_ast_render_flush_line(ctx)) {
            //     return false;
            // }
            aven_c_ast_render_node_try(
                ctx,
                node.type,
                node.lhs,
                false,
                split,
                state
            );
            aven_c_ast_render_token_force_try(
                ctx,
                node.token,
                split,
                state
            );
            if (
                parent_type == AVEN_C_AST_NODE_TYPE_IF_STATEMENT or
                parent_type == AVEN_C_AST_NODE_TYPE_DO_STATEMENT
            ) {
                break;
            }
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
            false,
            false
        ) and
        !aven_c_ast_render_node(
            &ctx,
            AVEN_C_AST_NODE_TYPE_NONE,
            ctx.ast->root,
            false,
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
    AvenArena *arena
) {
    AvenArena temp_arena = *arena;
    AvenCTokenSet tset = aven_c_lex(src, &temp_arena);
    AvenCAstResult ast_res = aven_c_ast_parse(tset, &temp_arena);
    if (ast_res.type == AVEN_C_AST_RESULT_TYPE_ERROR) {
        return (AvenCFmtResult){
            .error = AVEN_C_FMT_ERROR_PARSE,
            .msg = aven_str_copy(ast_res.data.error, arena),
        };
    }
    size_t min_column_width = 16;
    if (column_width < min_column_width) {
        return (AvenCFmtResult){
            .error = AVEN_C_FMT_ERROR_PARSE,
            .msg = aven_fmt(
                arena,
                "column width {} is less than the minimum of {}",
                aven_fmt_uint(column_width),
                aven_fmt_uint(min_column_width)
            ),
        };
    }
    AvenCAst ast = ast_res.data.ast;
    AvenCAstRenderResult ren_res = aven_c_ast_render(
        &ast,
        writer,
        column_width,
        aven_str("\n"),
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

#endif // AVEN_C_H
