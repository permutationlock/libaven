#ifndef AVEN_C_H
#define AVEN_C_H

#include "../aven.h"
#include "arena.h"
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
    AVEN_C_KEYWORD_COMPLEX,
    AVEN_C_KEYWORD_IMAGINARY,
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
    [AVEN_C_KEYWORD_BOOL] = aven_str_init("bool"),
    [AVEN_C_KEYWORD_COMPLEX] = aven_str_init("complex"),
    [AVEN_C_KEYWORD_IMAGINARY] = aven_str_init("imaginary"),
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
    return get(aven_c_keywords, kwd);
}

typedef enum {
    AVEN_C_PP_TOKEN_TYPE_NONE = 0,
    AVEN_C_PP_TOKEN_TYPE_HDR,
    AVEN_C_PP_TOKEN_TYPE_ID,
    AVEN_C_PP_TOKEN_TYPE_KEY,
    AVEN_C_PP_TOKEN_TYPE_NUM,
    AVEN_C_PP_TOKEN_TYPE_CHR,
    AVEN_C_PP_TOKEN_TYPE_STR,
    AVEN_C_PP_TOKEN_TYPE_PNC,
    AVEN_C_PP_TOKEN_TYPE_OTH,
} AvenCPpTokenType;

static const AvenStr aven_c_pp_token_type_str_data[] = {
    [AVEN_C_PP_TOKEN_TYPE_NONE] = aven_str_init("none"),
    [AVEN_C_PP_TOKEN_TYPE_HDR] = aven_str_init("header-name"),
    [AVEN_C_PP_TOKEN_TYPE_ID] = aven_str_init("identifier"),
    [AVEN_C_PP_TOKEN_TYPE_KEY] = aven_str_init("keyword"),
    [AVEN_C_PP_TOKEN_TYPE_NUM] = aven_str_init("pp-number"),
    [AVEN_C_PP_TOKEN_TYPE_CHR] = aven_str_init("character-constant"),
    [AVEN_C_PP_TOKEN_TYPE_STR] = aven_str_init("string-literal"),
    [AVEN_C_PP_TOKEN_TYPE_PNC] = aven_str_init("punctuator"),
    [AVEN_C_PP_TOKEN_TYPE_OTH] = aven_str_init("other"),
};

static const AvenStrSlice aven_c_pp_token_type_strs = {
    .ptr = (AvenStr *)aven_c_pp_token_type_str_data,
    .len = countof(aven_c_pp_token_type_str_data),
};

static inline AvenStr aven_c_pp_token_type_str(AvenCPpTokenType token_type) {
    return get(aven_c_pp_token_type_strs, (size_t)token_type);
}

typedef struct {
    AvenCPpTokenType type;
    uint32_t index;
    uint32_t len;
} AvenCPpToken;
typedef Slice(AvenCPpToken) AvenCPpTokenSlice;

typedef struct {
    AvenStr bytes;
    AvenCPpTokenSlice tokens;
} AvenCPpTokenSet;

static inline AvenStr aven_c_pp_token_str(
    AvenCPpTokenSet tset,
    uint32_t index
) {
    AvenCPpToken token = get(tset.tokens, index);
    if (token.type == AVEN_C_PP_TOKEN_TYPE_KEY) {
        return aven_c_keyword_str((AvenCKeyword)token.len);
    }

    return aven_str_range(tset.bytes, token.index, token.index + token.len);
}

typedef struct {
    uint32_t line;
    uint32_t col;
} AvenCPpTokenLoc;

static inline AvenCPpTokenLoc aven_c_pp_token_loc(
    AvenCPpTokenSet tset,
    uint32_t tindex
) {
    AvenCPpToken token = get(tset.tokens, tindex);
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
    return (AvenCPpTokenLoc){ .line = line, .col = col };
}

typedef struct {
    AvenStr bytes;
    List(AvenCPpToken) tokens;
    uint32_t token_start;
    uint32_t index;
    char next;
} AvenCLexPpCtx;

static inline AvenCLexPpCtx aven_c_lex_pp_init(
    AvenStr bytes,
    AvenArena *arena
) {
    return (AvenCLexPpCtx){
        .bytes = bytes,
        .tokens = aven_arena_create_list(AvenCPpToken, arena, bytes.len + 1),
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

#define aven_c_lex_next(c, i) if (!aven_c_lex_next_internal(c)) { \
        c->index = i; \
        return false; \
    }

static inline bool aven_c_lex_next_internal(AvenCLexPpCtx *ctx) {
    if (ctx->index >= ctx->bytes.len) {
        return false;
    }
    ctx->next = get(ctx->bytes, ctx->index);
    ctx->index += 1;
    return true;
}

static inline bool aven_c_lex_char(AvenCLexPpCtx *ctx, char d) {
    uint32_t init_index = ctx->index;
    aven_c_lex_next(ctx, init_index);
    if (ctx->next == d) {
        return true;
    }
    ctx->index = init_index;
    return false;
}

static inline bool aven_c_lex_str(AvenCLexPpCtx *ctx, AvenStr str) {
    uint32_t init_index = ctx->index;
    for (size_t i = 0; i < str.len; i += 1) {
        if (!aven_c_lex_char(ctx, get(str, i))) {
            ctx->index = init_index;
            return false;
        }
    }
    return true;
}

static inline bool aven_c_lex_oct_digit(AvenCLexPpCtx *ctx) {
    uint32_t init_index = ctx->index;
    aven_c_lex_next(ctx, init_index);
    if (aven_c_is_oct_digit(ctx->next)) {
        return true;
    }
    ctx->index = init_index;
    return false;
}

static inline bool aven_c_lex_dec_digit(AvenCLexPpCtx *ctx) {
    uint32_t init_index = ctx->index;
    aven_c_lex_next(ctx, ctx->index);
    if (aven_c_is_dec_digit(ctx->next)) {
        return true;
    }
    ctx->index = init_index;
    return false;
}

static inline bool aven_c_lex_hex_digit(AvenCLexPpCtx *ctx) {
    uint32_t init_index = ctx->index;
    aven_c_lex_next(ctx, ctx->index);
    if (aven_c_is_hex_digit(ctx->next)) {
        return true;
    }
    ctx->index = init_index;
    return false;
}

static inline bool aven_c_lex_hex_str(AvenCLexPpCtx *ctx, uint32_t digits) {
    uint32_t init_index = ctx->index;
    for (uint32_t i = 0; i < digits; i += 1) {
        if (!aven_c_lex_hex_digit(ctx)) {
            ctx->index = init_index;
            return false;
        }
    }
    return true;
}

static inline bool aven_c_lex_letter(AvenCLexPpCtx *ctx) {
    uint32_t init_index = ctx->index;
    aven_c_lex_next(ctx, ctx->index);
    if (aven_c_is_letter(ctx->next)) {
        return true;
    }
    ctx->index = init_index;
    return false;
}

static inline bool aven_c_lex_id_nondigit(AvenCLexPpCtx *ctx) {
    uint32_t init_index = ctx->index;
    if (aven_c_lex_char(ctx, '_') or aven_c_lex_letter(ctx)) {
        return true;
    }
    if (aven_c_lex_char(ctx, '\\')) {
        if (aven_c_lex_char(ctx, 'u') and aven_c_lex_hex_str(ctx, 4)) {
            return true;
        } else if (
            aven_c_lex_char(ctx, 'U') and
            aven_c_lex_hex_str(ctx, 8)
        ) {
            return true;
        }
    }
    ctx->index = init_index;
    return false;
}

static inline bool aven_c_lex_escape_seq(AvenCLexPpCtx * ctx) {
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
        case '?':
            return true;
        default:
            ctx->index -= 1;
            break;
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

static inline bool aven_c_lex_pp_header(AvenCLexPpCtx *ctx) {
    uint32_t init_index = ctx->index;

    if (ctx->tokens.len < 2) {
        return false;
    }

    bool quoted = false;
    if (aven_c_lex_char(ctx, '\"')) {
        quoted = true;
    }
    if (!quoted and !aven_c_lex_char(ctx, '<')) {
        return false;
    }

    AvenCPpToken t1 = get(ctx->tokens, ctx->tokens.len - 2);
    AvenCPpToken t2 = get(ctx->tokens, ctx->tokens.len - 1);
    if (
        t1.type != AVEN_C_PP_TOKEN_TYPE_PNC or
        t1.len != 1 or
        t2.type != AVEN_C_PP_TOKEN_TYPE_ID or
        t2.len != aven_str("include").len or
        get(ctx->bytes, t1.index) != '#' or
        !aven_str_equals(
            aven_str_range(
                ctx->bytes,
                t2.index,
                t2.index + t2.len
            ),
            aven_str("include")
        )
    ) {
        ctx->index = init_index;
        return false;
    }

    bool finished = false;
    uint32_t interior = 0;
    while (!finished) {
        aven_c_lex_next(ctx, init_index);
        if (ctx->next == '\n') {
            break;
        }
        if (quoted and ctx->next == '\"') {
            finished = true;
            break;
        }
        if (!quoted and ctx->next == '>') {
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

static inline bool aven_c_lex_identifier(AvenCLexPpCtx *ctx) {
    if (!aven_c_lex_id_nondigit(ctx)) {
        return false;
    }

    while (
        aven_c_lex_id_nondigit(ctx) or
        aven_c_lex_dec_digit(ctx)
    ) {}

    return true;
}

static inline bool aven_c_lex_pp_number_exp(AvenCLexPpCtx *ctx) {
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

static inline bool aven_c_lex_pp_number(AvenCLexPpCtx *ctx) {
    if (
        !aven_c_lex_dec_digit(ctx) and
        !aven_c_lex_char(ctx, '.')
    ) {
        return false;
    }

    while (
        aven_c_lex_dec_digit(ctx) or
        aven_c_lex_pp_number_exp(ctx) or
        aven_c_lex_id_nondigit(ctx) or
        aven_c_lex_char(ctx, '.')
    ) {}

    return true;
}

static inline bool aven_c_lex_character_const(AvenCLexPpCtx *ctx) {
    uint32_t init_index = ctx->index;
    if (!aven_c_lex_char(ctx, '\'')) {
        return false;
    }

    bool finished = false;
    uint32_t interior = 0;
    while (!finished) {
        aven_c_lex_next(ctx, init_index);
        if (ctx->next == '\n') {
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

static inline bool aven_c_lex_string_lit(AvenCLexPpCtx *ctx) {
    uint32_t init_index = ctx->index;
    if (
        !(
            aven_c_lex_char(ctx, '\"') or
            aven_c_lex_str(ctx, aven_str("u8\"")) or
            aven_c_lex_str(ctx, aven_str("u\"")) or
            aven_c_lex_str(ctx, aven_str("U\"")) or
            aven_c_lex_str(ctx, aven_str("L\""))
        )
    ) {
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

static inline bool aven_c_lex_punctuator(AvenCLexPpCtx *ctx) {
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
            if (aven_c_lex_char(ctx, '^')) {
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

static inline bool aven_c_lex_ignore(AvenCLexPpCtx *ctx) {
    assert(ctx->index == ctx->token_start);
    bool done = false;
    while (!done) {
        ctx->token_start = ctx->index;
        aven_c_lex_next(ctx, ctx->index);
        switch (ctx->next) {
            case ' ':
            case '\t':
            case '\r':
            case '\n': {
                break;
            }
            case '/': {
                if (aven_c_lex_char(ctx, '/')) {
                    while (!aven_c_lex_char(ctx, '\n')) {
                        ctx->token_start = ctx->index;
                        aven_c_lex_next(ctx, ctx->index);
                    }
                } else if (aven_c_lex_char(ctx, '*')) {
                    while (!aven_c_lex_str(ctx, aven_str("*/"))) {
                        ctx->token_start = ctx->index;
                        aven_c_lex_next(ctx, ctx->index);
                    }
                } else {
                    done = true;
                }
                break;
            }
            // We only accept and ignore escaped newlines between tokens
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

static inline bool aven_c_lex_pp_other(AvenCLexPpCtx *ctx) {
    aven_c_lex_next(ctx, ctx->index);
    return true;
}

static inline bool aven_c_lex_pp_step(AvenCLexPpCtx *ctx) {
    assert(ctx->index == ctx->token_start);
    aven_c_lex_ignore(ctx);
    if (aven_c_lex_pp_header(ctx)) {
        list_push(ctx->tokens) = (AvenCPpToken){
            .type = AVEN_C_PP_TOKEN_TYPE_HDR,
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
            list_push(ctx->tokens) = (AvenCPpToken){
                .type = AVEN_C_PP_TOKEN_TYPE_ID,
                .index = ctx->token_start,
                .len = ctx->index - ctx->token_start,
            };
        } else {
            list_push(ctx->tokens) = (AvenCPpToken){
                .type = AVEN_C_PP_TOKEN_TYPE_KEY,
                .index = ctx->token_start,
                .len = (uint32_t)keyword,
            };
        }
        ctx->token_start = ctx->index;
        return false;
    }
    if (aven_c_lex_pp_number(ctx)) {
        list_push(ctx->tokens) = (AvenCPpToken){
            .type = AVEN_C_PP_TOKEN_TYPE_NUM,
            .index = ctx->token_start,
            .len = ctx->index - ctx->token_start,
        };
        ctx->token_start = ctx->index;
        return false;
    }
    if (aven_c_lex_character_const(ctx)) {
        list_push(ctx->tokens) = (AvenCPpToken){
            .type = AVEN_C_PP_TOKEN_TYPE_CHR,
            .index = ctx->token_start,
            .len = ctx->index - ctx->token_start,
        };
        ctx->token_start = ctx->index;
        return false;
    }
    if (aven_c_lex_string_lit(ctx)) {
        list_push(ctx->tokens) = (AvenCPpToken){
            .type = AVEN_C_PP_TOKEN_TYPE_STR,
            .index = ctx->token_start,
            .len = ctx->index - ctx->token_start,
        };
        ctx->token_start = ctx->index;
        return false;
    }
    if (aven_c_lex_punctuator(ctx)) {
        list_push(ctx->tokens) = (AvenCPpToken){
            .type = AVEN_C_PP_TOKEN_TYPE_PNC,
            .index = ctx->token_start,
            .len = ctx->index - ctx->token_start,
        };
        ctx->token_start = ctx->index;
        return false;
    }

    if (aven_c_lex_pp_other(ctx)) {
        list_push(ctx->tokens) = (AvenCPpToken){
            .type = AVEN_C_PP_TOKEN_TYPE_OTH,
            .index = ctx->token_start,
            .len = ctx->index - ctx->token_start,
        };
        ctx->token_start = ctx->index;
        return false;
    }

    // We NONE-terminate the token list
    list_push(ctx->tokens) = (AvenCPpToken){
        .type = AVEN_C_PP_TOKEN_TYPE_NONE,
        .index = ctx->token_start,
        .len = ctx->index - ctx->token_start,
    };

    return true;
}

static const AvenCPpToken aven_c_token_empty = { 0 };
static const AvenCPpTokenSet aven_c_token_set_empty = {
    .tokens = {
        .ptr = (AvenCPpToken *)&aven_c_token_empty,
        .len = 1,
    },
    .bytes = { 0 },
};

static inline AvenCPpTokenSet aven_c_lex_pp(AvenStr bytes, AvenArena *arena) {
    if (bytes.len == 0) {
        return aven_c_token_set_empty;
    }

    AvenCLexPpCtx ctx = aven_c_lex_pp_init(bytes, arena);

    while (!aven_c_lex_pp_step(&ctx)) {}

    return (AvenCPpTokenSet){
        .bytes = bytes,
        .tokens = aven_arena_commit_list_to_slice(
            AvenCPpTokenSlice,
            arena,
            ctx.tokens
        ),
    };
}

typedef enum {
    AVEN_C_AST_NODE_TYPE_NONE = 0,
    AVEN_C_AST_NODE_TYPE_PUNCTUATOR,
    AVEN_C_AST_NODE_TYPE_STRING_LITERAL,
    AVEN_C_AST_NODE_TYPE_CONSTANT,
    AVEN_C_AST_NODE_TYPE_IDENTIFIER,
    AVEN_C_AST_NODE_TYPE_EXPR,
    AVEN_C_AST_NODE_TYPE_PRIMARY_EXPR,
    AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR,
    AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_INITIALIZER,
    AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_INITIALIZER_COMMA,
    AVEN_C_AST_NODE_TYPE_UNARY_EXPR,
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
    AVEN_C_AST_NODE_TYPE_TYPE_NAME,
    AVEN_C_AST_NODE_TYPE_TYPE_SPECIFIER,
    AVEN_C_AST_NODE_TYPE_TYPE_QUALIFIER,
} AvenCAstNodeType;

typedef struct {
    AvenCAstNodeType type;
    uint32_t token;
    uint32_t lhs;
    uint32_t rhs;
} AvenCAstNode;
typedef Optional(AvenCAstNode) AvenCAstNodeOpt;

typedef struct {
    AvenCPpTokenSet tset;
    List(AvenCAstNode) nodes;
    List(uint32_t) data;
    List(uint32_t) scratch;
    uint32_t token_index;
    uint32_t root;
} AvenCAstCtx;

typedef Slice(uint32_t) AvenCIndexSlice;

typedef struct {
    uint32_t nodes_len;
    uint32_t data_len;
    uint32_t scratch_len;
    uint32_t token_index;
} AvenCAstCtxState;

static inline AvenCAstCtx aven_c_ast_init(
    AvenCPpTokenSet tset,
    AvenArena *arena
) {
    AvenCAstCtx ctx = {
        .tset = tset,
        .nodes = { .len = tset.tokens.len },
        .data =  { .len = tset.tokens.len },
        .scratch = { .len = tset.tokens.len },
    };
    ctx.nodes.ptr = aven_arena_create_array(AvenCAstNode, arena, ctx.nodes.len);
    ctx.data.ptr = aven_arena_create_array(uint32_t, arena, ctx.data.len);
    ctx.scratch.ptr = aven_arena_create_array(uint32_t, arena, ctx.scratch.len);

    return ctx;
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

static inline AvenCPpToken aven_c_ast_next(AvenCAstCtx *ctx) {
    return get(ctx->tset.tokens, ctx->token_index);
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

static inline AvenCAstNodeOpt aven_c_ast_get(AvenCAstCtx *ctx, uint32_t index) {
    if (index == 0) {
        return (AvenCAstNodeOpt){ 0 };
    }
    return (AvenCAstNodeOpt){
        .valid = true,
        .value = get(ctx->nodes, index - 1),
    };
}

static inline uint32_t aven_c_ast_push_leaf(
    AvenCAstCtx *ctx,
    AvenCAstNodeType type,
    uint32_t token
) {
    return aven_c_ast_push(ctx, type, token, 0, 0);
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
    return data_top;
}

static inline uint32_t aven_c_ast_parse_identifier(AvenCAstCtx *ctx) {
    if (aven_c_ast_next(ctx).type == AVEN_C_PP_TOKEN_TYPE_ID) {
        return aven_c_ast_push_leaf(
            ctx,
            AVEN_C_AST_NODE_TYPE_IDENTIFIER,
            ctx->token_index++
        );
    }
    return 0;
}

static inline uint32_t aven_c_ast_parse_string_literal(AvenCAstCtx *ctx) {
    if (aven_c_ast_next(ctx).type == AVEN_C_PP_TOKEN_TYPE_STR) {
        return aven_c_ast_push_leaf(
            ctx,
            AVEN_C_AST_NODE_TYPE_STRING_LITERAL,
            ctx->token_index++
        );
    }
    return 0;
}

static inline uint32_t aven_c_ast_parse_constant(AvenCAstCtx *ctx) {
    switch (aven_c_ast_next(ctx).type) {
        case AVEN_C_PP_TOKEN_TYPE_NUM:
        case AVEN_C_PP_TOKEN_TYPE_CHR: {
            return aven_c_ast_push_leaf(
                ctx,
                AVEN_C_AST_NODE_TYPE_CONSTANT,
                ctx->token_index++
            );
        }
        default: {
            break;
        }
    }

    return 0;
}

static inline bool aven_c_ast_match_punctuator(
    AvenCAstCtx *ctx,
    AvenStr str
) {
    if (
        aven_c_ast_next(ctx).type == AVEN_C_PP_TOKEN_TYPE_PNC and
        aven_str_equals(
            aven_c_pp_token_str(ctx->tset, ctx->token_index),
            str
        )
    ) {
        ctx->token_index += 1;
        return true;
    }
    return false;
}

static inline bool aven_c_ast_match_keyword(
    AvenCAstCtx *ctx,
    AvenStr str
) {
    if (
        aven_c_ast_next(ctx).type == AVEN_C_PP_TOKEN_TYPE_KEY and
        aven_str_equals(
            aven_c_pp_token_str(ctx->tset, ctx->token_index),
            str
        )
    ) {
        ctx->token_index += 1;
        return true;
    }
    return false;
}

static inline uint32_t aven_c_ast_parse_expr(AvenCAstCtx *ctx);
static inline uint32_t aven_c_ast_parse_assign_expr(AvenCAstCtx *ctx);
static inline uint32_t aven_c_ast_parse_initializer_list(AvenCAstCtx *ctx);

static inline uint32_t aven_c_ast_parse_atomic_specifier(AvenCAstCtx *ctx) {
    (void)ctx;
    return 0;
}

static inline uint32_t aven_c_ast_parse_enum_specifier(AvenCAstCtx *ctx) {
    (void)ctx;
    return 0;
}

static inline uint32_t aven_c_ast_parse_struct_specifier(AvenCAstCtx *ctx) {
    (void)ctx;
    return 0;
}

static inline uint32_t aven_c_ast_parse_type_specifier(AvenCAstCtx *ctx) {
    uint32_t main_token = ctx->token_index;
    uint32_t node = 0;
    switch (aven_c_ast_next(ctx).type) {
        case AVEN_C_PP_TOKEN_TYPE_KEY: {
            char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
            switch (nchar) {
                case 'c': {
                    if (aven_c_ast_match_keyword(ctx, aven_str("char"))) {
                        node = aven_c_ast_push_leaf(
                            ctx,
                            AVEN_C_AST_NODE_TYPE_TYPE_NAME,
                            main_token
                        );
                    }
                    break;
                }
                case 'd': {
                    if (aven_c_ast_match_keyword(ctx, aven_str("double"))) {
                        node = aven_c_ast_push_leaf(
                            ctx,
                            AVEN_C_AST_NODE_TYPE_TYPE_NAME,
                            main_token
                        );
                    }
                    break;
                }
                case 'e': {
                    node = aven_c_ast_parse_enum_specifier(ctx);
                    break;
                }
                case 'f': {
                    if (aven_c_ast_match_keyword(ctx, aven_str("float"))) {
                        node = aven_c_ast_push_leaf(
                            ctx,
                            AVEN_C_AST_NODE_TYPE_TYPE_NAME,
                            main_token
                        );
                    }
                    break;
                }
                case 'i': {
                    if (aven_c_ast_match_keyword(ctx, aven_str("int"))) {
                        node = aven_c_ast_push_leaf(
                            ctx,
                            AVEN_C_AST_NODE_TYPE_TYPE_NAME,
                            main_token
                        );
                    }
                    break;
                }
                case 'l': {
                    if (aven_c_ast_match_keyword(ctx, aven_str("long"))) {
                        node = aven_c_ast_push_leaf(
                            ctx,
                            AVEN_C_AST_NODE_TYPE_TYPE_NAME,
                            main_token
                        );
                    }
                    break;
                }
                case 's': {
                    if (
                        aven_c_ast_match_keyword(ctx, aven_str("short")) or
                        aven_c_ast_match_keyword(ctx, aven_str("signed"))
                    ) {
                        node = aven_c_ast_push_leaf(
                            ctx,
                            AVEN_C_AST_NODE_TYPE_TYPE_NAME,
                            main_token
                        );
                    } else {
                        node = aven_c_ast_parse_struct_specifier(ctx);
                    }
                    break;
                }
                case 'u': {
                    if (aven_c_ast_match_keyword(ctx, aven_str("unsigned"))) {
                        node = aven_c_ast_push_leaf(
                            ctx,
                            AVEN_C_AST_NODE_TYPE_TYPE_NAME,
                            main_token
                        );
                    } else {
                        node = aven_c_ast_parse_struct_specifier(ctx);
                    }
                    break;
                }
                case 'v': {
                    if (aven_c_ast_match_keyword(ctx, aven_str("void"))) {
                        node = aven_c_ast_push_leaf(
                            ctx,
                            AVEN_C_AST_NODE_TYPE_TYPE_NAME,
                            main_token
                        );
                    }
                    break;
                }
                case '_': {
                    if (
                        aven_c_ast_match_keyword(ctx, aven_str("_Bool")) or
                        aven_c_ast_match_keyword(ctx, aven_str("_Complex"))
                    ) {
                        node = aven_c_ast_push_leaf(
                            ctx,
                            AVEN_C_AST_NODE_TYPE_TYPE_NAME,
                            main_token
                        );
                    } else {
                        node = aven_c_ast_parse_atomic_specifier(ctx);
                    }
                    break;
                }
            }
        }
        case AVEN_C_PP_TOKEN_TYPE_ID: {
            ctx->token_index += 1;
            node = aven_c_ast_push_leaf(
                ctx,
                AVEN_C_AST_NODE_TYPE_TYPE_SPECIFIER,
                main_token
            );
            break;
        }
        default: {
            break;
        }
    }
    return node;
}

static inline uint32_t aven_c_ast_parse_type_qualifier(AvenCAstCtx *ctx) {
    if (aven_c_ast_next(ctx).type != AVEN_C_PP_TOKEN_TYPE_KEY) {
        return 0;
    }
    uint32_t main_token = ctx->token_index;
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    uint32_t node = 0;
    switch (nchar) {
        case 'c': {
            if (aven_c_ast_match_keyword(ctx, aven_str("const"))) {
                node = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_TYPE_QUALIFIER,
                    main_token
                );
            }
            break;
        }
        case 'r': {
            if (aven_c_ast_match_keyword(ctx, aven_str("restrict"))) {
                node = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_TYPE_QUALIFIER,
                    main_token
                );
            }
            break;
        }
        case 'v': {
            if (aven_c_ast_match_keyword(ctx, aven_str("volatile"))) {
                node = aven_c_ast_push_leaf(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_TYPE_QUALIFIER,
                    main_token
                );
            }
            break;
        }
        case '_': {
            if (aven_c_ast_match_keyword(ctx, aven_str("_Atomic"))) {
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
    uint32_t main_token = ctx->token_index;
    for (;;) {
        uint32_t node = aven_c_ast_parse_type_specifier(ctx);
        if (node == 0) {
            node = aven_c_ast_parse_type_qualifier(ctx);
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
    return aven_c_ast_push(
        ctx,
        AVEN_C_AST_NODE_TYPE_TYPE_NAME,
        main_token,
        list_data,
        0
    );
}

static inline uint32_t aven_c_ast_parse_initializer_list(AvenCAstCtx *ctx) {
    (void)ctx;
    return 0;
}

static inline uint32_t aven_c_ast_parse_primary_expr(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    uint32_t exp_node = aven_c_ast_parse_identifier(ctx);
    if (exp_node != 0) {
        return exp_node;
    }
    exp_node = aven_c_ast_parse_constant(ctx);
    if (exp_node != 0) {
        return exp_node;
    }

    uint32_t token = ctx->token_index;
    if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        return 0;
    }
    exp_node = aven_c_ast_parse_expr(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    return aven_c_ast_push(
        ctx,
        AVEN_C_AST_NODE_TYPE_PRIMARY_EXPR,
        token,
        exp_node,
        0
    );
}

static inline uint32_t aven_c_ast_parse_postfix_expr_initializer(
    AvenCAstCtx *ctx
) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t type_node = aven_c_ast_parse_type_name(ctx) ;
    if (type_node == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t main_token = ctx->token_index;
    if (!aven_c_ast_match_punctuator(ctx, aven_str("{"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t initializer_list_data = aven_c_ast_parse_initializer_list(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("}"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    if (aven_c_ast_match_punctuator(ctx, aven_str(","))) {
        return aven_c_ast_push(
            ctx,
            AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_INITIALIZER_COMMA,
            type_node,
            main_token,
            initializer_list_data
        );
    }
    return aven_c_ast_push(
        ctx,
        AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR_INITIALIZER,
        type_node,
        main_token,
        initializer_list_data
    );
}

static inline uint32_t aven_c_ast_parse_postfix_op(
    AvenCAstCtx *ctx,
    uint32_t parent
) {
    assert(parent != 0);
    if (aven_c_ast_next(ctx).type != AVEN_C_PP_TOKEN_TYPE_PNC) {
        return 0;
    }
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    uint32_t main_token = ctx->token_index;
    uint32_t node = 0;
    switch (nchar) {
        case '[': {
            if (!aven_c_ast_match_punctuator(ctx, aven_str("["))) {
                break;
            }
            node = aven_c_ast_parse_expr(ctx);
            if (!aven_c_ast_match_punctuator(ctx, aven_str("]"))) {
                node = 0;
            }
            if (node == 0) {
                aven_c_ast_restore(ctx, state);
            }
            break;
        }
        case '(': {
            if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
                break;
            }
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
            if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
                aven_c_ast_restore(ctx, state);
                break;
            }
            node = aven_c_ast_push(
                ctx,
                AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR,
                main_token,
                parent,
                aven_c_ast_scratch_commit(ctx, scratch_top)
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
                AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR,
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
                    AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR,
                    main_token,
                    parent,
                    id_node
                );
            } else if (aven_c_ast_match_punctuator(ctx, aven_str("--"))) {
                node = aven_c_ast_push(
                    ctx,
                    AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR,
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
                AVEN_C_AST_NODE_TYPE_POSTFIX_EXPR,
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
    uint32_t node = aven_c_ast_parse_primary_expr(ctx);
    if (node == 0) {
        if (aven_c_ast_match_punctuator(ctx, aven_str("{"))) {
            node = aven_c_ast_parse_type_name(ctx);
            if (!aven_c_ast_match_punctuator(ctx, aven_str("{"))) {
                node = 0;
            }
        }
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
    uint32_t main_token = ctx->token_index;
    AvenCPpTokenType token_type = aven_c_ast_next(ctx).type;
    AvenStr token_str = aven_c_pp_token_str(ctx->tset, main_token);
    switch (token_type) {
        case AVEN_C_PP_TOKEN_TYPE_KEY: {
            bool sizeof_op = aven_str_equals(token_str, aven_str("sizeof"));
            bool alignof_op = !sizeof_op and
                aven_str_equals(token_str, aven_str("_Alignof"));
            if (!(sizeof_op or alignof_op)) {
                aven_c_ast_restore(ctx, state);
                break;
            }
            ctx->token_index += 1;
            bool parens = alignof_op or aven_str_equals(
                aven_c_pp_token_str(ctx->tset, ctx->token_index),
                aven_str("(")
            );
            if (parens and !aven_c_ast_match_punctuator(ctx, aven_str("("))) {
                aven_c_ast_restore(ctx, state);
                break;
            }
            uint32_t child = aven_c_ast_parse_unary_expr(ctx);
            if (child == 0) {
                aven_c_ast_restore(ctx, state);
                break;
            }
            if (parens and !aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
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
        case AVEN_C_PP_TOKEN_TYPE_PNC: {
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
                        ctx->token_index += 1;
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
        node = aven_c_ast_parse_primary_expr(ctx);
    }
    if (node == 0) {
        aven_c_ast_restore(ctx, state);
    }
    return node;
}

static inline uint32_t aven_c_ast_parse_cast_expr(AvenCAstCtx *ctx) {
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("("))) {
        return aven_c_ast_parse_unary_expr(ctx);
    }
    uint32_t lhs = aven_c_ast_parse_type_name(ctx);
    if (lhs == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t main_token = ctx->token_index;
    if (!aven_c_ast_match_punctuator(ctx, aven_str(")"))) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    uint32_t rhs = aven_c_ast_parse_cast_expr(ctx);
    if (rhs == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    return aven_c_ast_push(
        ctx,
        AVEN_C_AST_NODE_TYPE_CAST_EXPR,
        main_token,
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
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    uint32_t main_token = ctx->token_index;
    uint32_t rhs = 0;
    switch (nchar) {
        case '*':
        case '/':
        case '%': {
            ctx->token_index += 1;
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
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    uint32_t main_token = ctx->token_index;
    uint32_t rhs = 0;
    switch (nchar) {
        case '+':
        case '-': {
            ctx->token_index += 1;
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
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    uint32_t main_token = ctx->token_index;
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
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    uint32_t main_token = ctx->token_index;
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
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    uint32_t main_token = ctx->token_index;
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
    uint32_t main_token = ctx->token_index;
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
    uint32_t main_token = ctx->token_index;
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
    uint32_t main_token = ctx->token_index;
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
    uint32_t main_token = ctx->token_index;
    if (!aven_c_ast_match_punctuator(ctx, aven_str("&&"))) {
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
    uint32_t main_token = ctx->token_index;
    if (aven_c_ast_match_punctuator(ctx, aven_str("||"))) {
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
    uint32_t main_token = ctx->token_index;
    uint32_t scratch_top = aven_c_ast_scratch_init(ctx);
    if (!aven_c_ast_match_punctuator(ctx, aven_str("?"))) {
        return 0;
    }
    uint32_t middle = aven_c_ast_parse_expr(ctx);
    if (middle == 0) {
        aven_c_ast_restore(ctx, state);
        return 0;
    }
    list_push(ctx->scratch) = middle;
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
    return aven_c_ast_push(
        ctx,
        AVEN_C_AST_NODE_TYPE_CONDITIONAL_EXPR,
        main_token,
        lhs,
        aven_c_ast_scratch_commit(ctx, scratch_top)
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
    AvenCAstCtxState state = aven_c_ast_save(ctx);
    char nchar = get(ctx->tset.bytes, aven_c_ast_next(ctx).index);
    uint32_t main_token = ctx->token_index;
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
    uint32_t main_token = ctx->token_index;
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
        return 0;
    }
    for (;;) {
        uint32_t rhs = aven_c_ast_parse_comma_op(ctx, node);
        if (rhs == 0) {
            break;
        }
        node = rhs;
    }
    return node;
}

static inline uint32_t aven_c_ast_parse_const_expr(AvenCAstCtx *ctx) {
    return aven_c_ast_parse_conditional_expr(ctx);
}

static inline bool aven_c_ast_step(AvenCAstCtx *ctx) {
    ctx->root = aven_c_ast_parse_expr(ctx);
    return true;
}

#endif // AVEN_C_H
