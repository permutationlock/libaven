#ifndef AVEN_C_H
#define AVEN_C_H

#include "../aven.h"
#include "arena.h"
#include "str.h"

typedef enum {
    AVEN_C_PP_TOKEN_TYPE_NONE,
    AVEN_C_PP_TOKEN_TYPE_HDR,
    AVEN_C_PP_TOKEN_TYPE_ID,
    AVEN_C_PP_TOKEN_TYPE_NUM,
    AVEN_C_PP_TOKEN_TYPE_CHR,
    AVEN_C_PP_TOKEN_TYPE_STR,
    AVEN_C_PP_TOKEN_TYPE_PNC,
    AVEN_C_PP_TOKEN_TYPE_OTH,
    AVEN_C_PP_TOKEN_TYPE_ENL,
} AvenCPPTokenType;

typedef struct {
    AvenCPPTokenType type;
    uint32_t index;
    uint32_t len;
} AvenCPPToken;
typedef Slice(AvenCPPToken) AvenCPPTokenSlice;

typedef struct {
    AvenStr bytes;
    AvenCPPTokenSlice tokens;
} AvenCPPTokenSet;

typedef struct {
    AvenStr bytes;
    List(AvenCPPToken) tokens;
    AvenCPPToken cur;
} AvenCPPTokenCtx;

static inline AvenCPPTokenCtx aven_c_pp_lex_init(
    AvenStr bytes,
    AvenArena *arena
) {
    return (AvenCPPTokenCtx){
        .bytes = bytes,
        .tokens = aven_arena_create_list(AvenCPPToken, arena, bytes.len),
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

static inline bool aven_c_pp_lex_char(AvenCPPTokenCtx *ctx, char d) {
    uint32_t len = ctx->cur.len;
    if (ctx->cur.index + len >= ctx->bytes.len) {
        return false;
    }
    char c = get(ctx->bytes, ctx->cur.index + len);
    if (c == d) {
        ctx->cur.len += 1;
        return true;
    }

    return false;
}

static inline bool aven_c_pp_lex_str(AvenCPPTokenCtx *ctx, AvenStr str) {
    uint32_t len = ctx->cur.len;
    for (size_t i = 0; i < str.len; i += 1) {
        if (!aven_c_pp_lex_char(ctx, get(str, i))) {
            ctx->cur.len = len;
            return false;
        }
    }

    return true;
}

static inline bool aven_c_pp_lex_oct_digit(AvenCPPTokenCtx *ctx) {
    uint32_t len = ctx->cur.len;
    if (ctx->cur.index + len >= ctx->bytes.len) {
        return false;
    }
    char c = get(ctx->bytes, ctx->cur.index + len);
    if (aven_c_is_oct_digit(c)) {
        ctx->cur.len += 1;
        return true;
    }

    return false;
}

static inline bool aven_c_pp_lex_dec_digit(AvenCPPTokenCtx *ctx) {
    uint32_t len = ctx->cur.len;
    if (ctx->cur.index + len >= ctx->bytes.len) {
        return false;
    }
    char c = get(ctx->bytes, ctx->cur.index + len);
    if (aven_c_is_dec_digit(c)) {
        ctx->cur.len += 1;
        return true;
    }

    return false;
}

static inline bool aven_c_pp_lex_hex_digit(AvenCPPTokenCtx *ctx) {
    uint32_t len = ctx->cur.len;
    if (ctx->cur.index + len >= ctx->bytes.len) {
        return false;
    }
    char c = get(ctx->bytes, ctx->cur.index + len);
    if (aven_c_is_hex_digit(c)) {
        ctx->cur.len += 1;
        return true;
    }

    return false;
}

static inline bool aven_c_pp_lex_hex_str(AvenCPPTokenCtx *ctx, uint32_t digits) {
    uint32_t len = ctx->cur.len;
    for (uint32_t i = 0; i < digits; i += 1) {
        if (!aven_c_pp_lex_hex_digit(ctx)) {
            ctx->cur.len = len;
            return false;
        }
    }

    return true;
}

static inline bool aven_c_pp_lex_letter(AvenCPPTokenCtx *ctx) {
    uint32_t len = ctx->cur.len;
    if (ctx->cur.index + len >= ctx->bytes.len) {
        return false;
    }
    char c = get(ctx->bytes, ctx->cur.index + len);
    if (aven_c_is_letter(c)) {
        ctx->cur.len += 1;
        return true;
    }

    return false;    
}

static inline bool aven_c_pp_lex_id_nondigit(AvenCPPTokenCtx *ctx) {
    uint32_t len = ctx->cur.len;
    if (ctx->cur.index + len >= ctx->bytes.len) {
        return false;
    }
    if (aven_c_pp_lex_char(ctx, '\\') or aven_c_pp_lex_letter(ctx)) {
        return true;
    }
    if (aven_c_pp_lex_char(ctx, '\\')) {
        if (aven_c_pp_lex_char(ctx, 'u') and aven_c_pp_lex_hex_str(ctx, 4)) {
            return true;
        } else if (
            aven_c_pp_lex_char(ctx, 'U') and
            aven_c_pp_lex_hex_str(ctx, 8)
        ) {
            return true;
        }
    }

    ctx->cur.len = len;
    return false;
}

static inline bool aven_c_pp_lex_escape_seq(AvenCPPTokenCtx * ctx) {
    uint32_t len = ctx->cur.len;
    if (ctx->cur.index + len >= ctx->bytes.len) {
        return false;
    }
    if (!aven_c_pp_lex_char(ctx, '\\')) {
        return false;
    }

    char e = get(ctx->bytes, ctx->cur.index + ctx->cur.len);
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
            ctx->cur.len += 1;
            return true;
        default:
            break;
    }
    uint32_t oct_digits;
    for (oct_digits = 0; oct_digits < 3; oct_digits += 1) {
        if (!aven_c_pp_lex_oct_digit(ctx)) {
            break;
        }
    }
    if (oct_digits > 0) {
        return true;
    }

    if (e == 'x') {
        uint32_t hex_digits = 0;
        while (aven_c_pp_lex_hex_digit(ctx)) {
            hex_digits += 1;
        }
        if (hex_digits > 0) {
            return true;
        }
    }

    ctx->cur.len = len;
    return false;
}

static inline bool aven_c_pp_lex_header(AvenCPPTokenCtx *ctx) {
    uint32_t len = ctx->cur.len;
    if (ctx->cur.index + len >= ctx->bytes.len) {
        return false;
    }

    if (ctx->tokens.len < 2) {
        return false;
    }

    bool quoted = false;
    if (aven_c_pp_lex_char(ctx, '\"')) {
        quoted = true;
    }
    if (!quoted and !aven_c_pp_lex_char(ctx, '<')) {
        return false;
    }

    AvenCPPToken t1 = get(ctx->tokens, ctx->tokens.len - 2);
    AvenCPPToken t2 = get(ctx->tokens, ctx->tokens.len - 1);
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
        ctx->cur.len = len;
        return false;
    }

    bool finished = false;
    uint32_t interior = 0;
    while (!finished and ctx->cur.index + ctx->cur.len < ctx->bytes.len) {
        char c = get(ctx->bytes, ctx->cur.index + ctx->cur.len);
        if (c == '\n') {
            break;
        }
        ctx->cur.len += 1;
        if (quoted and c == '\"') {
            finished = true;
            break;
        }
        if (!quoted and c == '>') {
            finished = true;
            break;
        }
        interior += 1;
    }
    if (finished and interior > 0) {
        return true;
    }

    ctx->cur.len = len;
    return false;
}

static inline bool aven_c_pp_lex_identifier(AvenCPPTokenCtx *ctx) {
    uint32_t len = ctx->cur.len;
    if (ctx->cur.index + len >= ctx->bytes.len) {
        return false;
    }

    if (!aven_c_pp_lex_id_nondigit(ctx)) {
        return false;
    }

    while (
        aven_c_pp_lex_id_nondigit(ctx) or
        aven_c_pp_lex_dec_digit(ctx)
    ) {}

    return true;
}

static inline bool aven_c_pp_lex_number_exp(AvenCPPTokenCtx *ctx) {
    uint32_t len = ctx->cur.len;
    if (ctx->cur.index + len >= ctx->bytes.len) {
        return false;
    }

    if (
        !aven_c_pp_lex_char(ctx, 'e') and
        !aven_c_pp_lex_char(ctx, 'E') and
        !aven_c_pp_lex_char(ctx, 'p') and
        !aven_c_pp_lex_char(ctx, 'P')
    ) {
        return false;
    }

    if (aven_c_pp_lex_char(ctx, '+') or aven_c_pp_lex_char(ctx, '+')) {
        return true;
    }

    ctx->cur.len = len;
    return false;
}

static inline bool aven_c_pp_lex_number(AvenCPPTokenCtx *ctx) {
    uint32_t len = ctx->cur.len;
    if (ctx->cur.index + len >= ctx->bytes.len) {
        return false;
    }

    if (
        !aven_c_pp_lex_dec_digit(ctx) and
        !aven_c_pp_lex_char(ctx, '.')
    ) {
        return false;
    }

    while (
        aven_c_pp_lex_dec_digit(ctx) or
        aven_c_pp_lex_number_exp(ctx) or
        aven_c_pp_lex_id_nondigit(ctx) or
        aven_c_pp_lex_char(ctx, '.')
    ) {}

    return true;
}

static inline bool aven_c_pp_lex_character_const(AvenCPPTokenCtx *ctx) {
    uint32_t len = ctx->cur.len;
    if (ctx->cur.index + len >= ctx->bytes.len) {
        return false;
    }

    if (!aven_c_pp_lex_char(ctx, '\'')) {
        return false;
    }

    bool finished = false;
    uint32_t interior = 0;
    while (!finished and ctx->cur.index + ctx->cur.len < ctx->bytes.len) {
        char c = get(ctx->bytes, ctx->cur.index + ctx->cur.len);
        if (c == '\n') {
            break;
        }
        ctx->cur.len += 1;
        if (c == '\'') {
            finished = true;
            break;
        }
        interior += 1;
    }
    if (finished and interior > 0) {
        return true;
    }

    ctx->cur.len = len;
    return false;
}

static inline bool aven_c_pp_lex_string_lit(AvenCPPTokenCtx *ctx) {
    uint32_t len = ctx->cur.len;
    if (ctx->cur.index + len >= ctx->bytes.len) {
        return false;
    }

    if (!aven_c_pp_lex_char(ctx, '\"')) {
        return false;
    }

    bool finished = false;
    while (!finished and ctx->cur.index + ctx->cur.len < ctx->bytes.len) {
        char c = get(ctx->bytes, ctx->cur.index + ctx->cur.len);
        if (c == '\n') {
            break;
        }
        ctx->cur.len += 1;
        if (c == '\"') {
            finished = true;
            break;
        }
    }
    if (finished) {
        return true;
    }

    ctx->cur.len = len;
    return false;
}

static inline bool aven_c_pp_lex_punctuator(AvenCPPTokenCtx *ctx) {
    uint32_t len = ctx->cur.len;
    if (ctx->cur.index + len >= ctx->bytes.len) {
        return false;
    }

    char c = get(ctx->bytes, ctx->cur.index + ctx->cur.len);
    switch (c) {
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
            ctx->cur.len += 1;
            return true;
        }
        case '.': {
            ctx->cur.len += 1;
            if (aven_c_pp_lex_str(ctx, aven_str(".."))) {
                return true;
            }
            return true;
        }
        case '-': {
            ctx->cur.len += 1;
            if (
                aven_c_pp_lex_char(ctx, '-') or
                aven_c_pp_lex_char(ctx, '>') or
                aven_c_pp_lex_char(ctx, '=')
            ) {
                return true;
            }
            return true;
        }
        case '+': {
            ctx->cur.len += 1;
            if (
                aven_c_pp_lex_char(ctx, '+') or
                aven_c_pp_lex_char(ctx, '=')
            ) {
                return true;
            }
            return true;
        }
        case '&': {
            ctx->cur.len += 1;
            if (
                aven_c_pp_lex_char(ctx, '&') or
                aven_c_pp_lex_char(ctx, '=')
            ) {
                return true;
            }
            return true;
        }
        case '*': {
            ctx->cur.len += 1;
            if (aven_c_pp_lex_char(ctx, '=')) {
                return true;
            }
            return true;
        }
        case '!': {
            ctx->cur.len += 1;
            if (aven_c_pp_lex_char(ctx, '=')) {
                return true;
            }
            return true;
        }
        case '/': {
            ctx->cur.len += 1;
            if (aven_c_pp_lex_char(ctx, '=')) {
                return true;
            }
            return true;
        }
        case '%': {
            ctx->cur.len += 1;
            if (aven_c_pp_lex_char(ctx, ':')) {
                if (aven_c_pp_lex_str(ctx, aven_str("%:"))) {
                    return true;
                }
                return true;
            }
            if (
                aven_c_pp_lex_char(ctx, '>') or
                aven_c_pp_lex_char(ctx, '=')
            ) {
                return true;
            }
            return true;
        }
        case '<': {
            ctx->cur.len += 1;
            if (aven_c_pp_lex_char(ctx, '<')) {
                if (aven_c_pp_lex_char(ctx, '=')) {
                    return true;
                }
                return true;
            }
            if (
                aven_c_pp_lex_char(ctx, '=') or
                aven_c_pp_lex_char(ctx, ':') or
                aven_c_pp_lex_char(ctx, '%')
            ) {
                return true;
            }
            return true;
        }
        case '>': {
            ctx->cur.len += 1;
            if (aven_c_pp_lex_char(ctx, '>')) {
                if (aven_c_pp_lex_char(ctx, '=')) {
                    return true;
                }
                return true;
            }
            if (aven_c_pp_lex_char(ctx, '=')) {
                return true;
            }
            return true;
        }
        case '=': {
            ctx->cur.len += 1;
            if (aven_c_pp_lex_char(ctx, '=')) {
                return true;
            }
            return true;
        }
        case '^': {
            ctx->cur.len += 1;
            if (aven_c_pp_lex_char(ctx, '^')) {
                return true;
            }
            return true;
        }
        case '|': {
            ctx->cur.len += 1;
            if (aven_c_pp_lex_char(ctx, '|')) {
                return true;
            }
            if (aven_c_pp_lex_char(ctx, '=')) {
                return true;
            }
            return true;
        }
        case ':': {
            ctx->cur.len += 1;
            if (aven_c_pp_lex_char(ctx, '>')) {
                return true;
            }
            return true;
        }
        case '#': {
            ctx->cur.len += 1;
            if (aven_c_pp_lex_char(ctx, '#')) {
                return true;
            }
            return true;
        }
    }

    ctx->cur.len = len;
    return false;
}

static inline void aven_c_pp_lex_ignore(AvenCPPTokenCtx *ctx) {
    assert(ctx->cur.len == 0);
    bool done = false;
    while (!done and ctx->cur.index + ctx->cur.len < ctx->bytes.len) {
        char c = get(ctx->bytes, ctx->cur.index + ctx->cur.len);
        switch (c) {
            case ' ':
            case '\t':
            case '\r':
            case '\n': {
                ctx->cur.len += 1;
                break;
            }
            case '/': {
                if (aven_c_pp_lex_str(ctx, aven_str("//"))) {
                    while (
                        ctx->cur.index + ctx->cur.len < ctx->bytes.len and
                        get(ctx->bytes, ctx->cur.index + ctx->cur.len) != '\n'
                    ) {
                        ctx->cur.len += 1;
                    }
                } else if (aven_c_pp_lex_str(ctx, aven_str("/*"))) {
                    while (
                        ctx->cur.index + ctx->cur.len < ctx->bytes.len and
                        !aven_c_pp_lex_str(ctx, aven_str("*/"))
                    ) {
                        ctx->cur.len += 1;
                    }
                } else {
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

    ctx->cur.index += ctx->cur.len;
    ctx->cur.len = 0;
}

static inline bool aven_c_pp_lex_step(AvenCPPTokenCtx *ctx) {
    assert(ctx->cur.len == 0);
    aven_c_pp_lex_ignore(ctx);
    if (ctx->cur.index == ctx->bytes.len) {
        return true;
    }
    if (aven_c_pp_lex_str(ctx, aven_str("\\n"))) {
        // We treat escaped newlines as a token and don't allow them within
        // other tokens. This is nonstandard, but I have no interest in
        // code that uses escaped newlines that way
        ctx->cur.type = AVEN_C_PP_TOKEN_TYPE_ENL;
        list_push(ctx->tokens) = ctx->cur;
        ctx->cur = (AvenCPPToken){ .index = ctx->cur.index + ctx->cur.len };
        return false;
    }
    if (aven_c_pp_lex_header(ctx)) {
        ctx->cur.type = AVEN_C_PP_TOKEN_TYPE_HDR;
        list_push(ctx->tokens) = ctx->cur;
        ctx->cur = (AvenCPPToken){ .index = ctx->cur.index + ctx->cur.len };
        return false;
    }
    if (aven_c_pp_lex_identifier(ctx)) {
        ctx->cur.type = AVEN_C_PP_TOKEN_TYPE_ID;
        list_push(ctx->tokens) = ctx->cur;
        ctx->cur = (AvenCPPToken){ .index = ctx->cur.index + ctx->cur.len };
        return false;
    }
    if (aven_c_pp_lex_number(ctx)) {
        ctx->cur.type = AVEN_C_PP_TOKEN_TYPE_NUM;
        list_push(ctx->tokens) = ctx->cur;
        ctx->cur = (AvenCPPToken){ .index = ctx->cur.index + ctx->cur.len };
        return false;
    }
    if (aven_c_pp_lex_character_const(ctx)) {
        ctx->cur.type = AVEN_C_PP_TOKEN_TYPE_CHR;
        list_push(ctx->tokens) = ctx->cur;
        ctx->cur = (AvenCPPToken){ .index = ctx->cur.index + ctx->cur.len };
        return false;
    }
    if (aven_c_pp_lex_string_lit(ctx)) {
        ctx->cur.type = AVEN_C_PP_TOKEN_TYPE_STR;
        list_push(ctx->tokens) = ctx->cur;
        ctx->cur = (AvenCPPToken){ .index = ctx->cur.index + ctx->cur.len };
        return false;
    }
    if (aven_c_pp_lex_punctuator(ctx)) {
        ctx->cur.type = AVEN_C_PP_TOKEN_TYPE_PNC;
        list_push(ctx->tokens) = ctx->cur;
        ctx->cur = (AvenCPPToken){ .index = ctx->cur.index + ctx->cur.len };
        return false;
    }

    ctx->cur.len += 1;
    ctx->cur.type = AVEN_C_PP_TOKEN_TYPE_OTH;
    list_push(ctx->tokens) = ctx->cur;
    ctx->cur = (AvenCPPToken){ .index = ctx->cur.index + ctx->cur.len };
    return false;
}

static inline AvenCPPTokenSet aven_c_pp_lex(AvenStr bytes, AvenArena *arena) {
    if (bytes.len == 0) {
        return (AvenCPPTokenSet){ 0 };
    }

    AvenCPPTokenCtx ctx = aven_c_pp_lex_init(bytes, arena);

    while (!aven_c_pp_lex_step(&ctx)) {}

    return (AvenCPPTokenSet){
        .bytes = bytes,
        .tokens = aven_arena_commit_list_to_slice(
            AvenCPPTokenSlice,
            arena,
            ctx.tokens
        ),
    };
}

#endif // AVEN_C_H
