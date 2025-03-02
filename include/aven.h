#ifndef AVEN_H
#define AVEN_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define or ||
#define and &&

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define max(a, b) (((a) > (b)) ? (a) : (b))

#ifndef __has_attribute
    #define __has_attribute(unused) 0
#endif

#ifndef __has_builtin
    #define __has_builtin(unused) 0
#endif

#if !defined(AVEN_USE_STD_ASSERT) and __has_builtin(__builtin_unreachable)
    #define assert(c) while (!(c)) { __builtin_unreachable(); }
#else
    #ifndef AVEN_USE_STD_ASSERT
        #define AVEN_USE_STD_ASSERT
    #endif
    #include <assert.h>
#endif

#if defined(__STDC_VERSION__) and __STDC_VERSION__ >= 202311L
    #define AVEN_NORETURN noreturn
#elif defined(__STDC_VERSION__) and __STDC_VERSION__ >= 201112L
    #define AVEN_NORETURN _Noreturn
#elif defined(__STDC_VERSION__) and __STDC_VERSION__ >= 199901L
    #define AVEN_NORETURN
#else
    #error "C99 or later is required"
#endif

#define countof(array) (sizeof(array) / sizeof(*array))

#define Optional(t) struct { t value; bool valid; }
#define OptPtr(t) union { t *value; t *valid; }
#define Result(t) struct { t payload; int error; }
#define Slice(t) struct { t *ptr; size_t len; }
#define List(t) struct { t *ptr; size_t len; size_t cap; }
#define Queue(t) struct { \
        t *ptr; \
        size_t cap; \
        size_t front; \
        size_t back; \
        size_t used; \
    }

#define PoolEntry(t) union { t data; size_t parent; }
#define PoolExplicit(e) struct { \
        e *ptr; \
        size_t len; \
        size_t cap; \
        size_t free; \
        size_t used; \
    }
#define Pool(t) PoolExplicit(PoolEntry(t))

typedef Slice(unsigned char) ByteSlice;

#if !defined(AVEN_USE_STD_ASSERT) or !defined(NDEBUG)
    static inline size_t aven_assert_lt_internal_fn(size_t a, size_t b) {
        assert(a < b);
        return a;
    }

    #define aven_assert_lt_internal(a, b) aven_assert_lt_internal_fn(a, b)
#else
    #define aven_assert_lt_internal(a, b) a
#endif

static inline size_t aven_queue_push_internal(
    size_t *used,
    size_t *back,
    size_t cap
) {
    assert(*used < cap);
    *used += 1;
    size_t index = *back;
    *back = index + 1;
    if (*back >= cap) {
        *back -= cap;
    }
    return index;
}

static inline size_t aven_queue_pop_internal(
    size_t *used,
    size_t *front,
    size_t cap
) {
    assert(*used > 0);
    *used -= 1;
    size_t index = *front;
    *front = index + 1;
    if (*front >= cap) {
        *front -= cap;
    }
    return index;
}

static inline size_t aven_pool_next_internal(
    size_t *used,
    size_t *len,
    size_t cap
) {
    assert(*len < cap);
    *used += 1;
    size_t index = *len;
    *len += 1;
    return index;
}

static inline size_t aven_pool_pop_free_internal(
    size_t *used,
    size_t *free,
    size_t parent
) {
    *used += 1;

    size_t index = *free;
    *free = parent;
    return index - 1;
}

static inline void aven_pool_push_free_internal(
    size_t *used,
    size_t *free,
    size_t *parent,
    size_t index
) {
    assert(*used > 0);

    *used -= 1;
    *parent = *free;
    *free = index + 1;
}

#define get(s, i) (s).ptr[aven_assert_lt_internal(i, (s).len)]
#define list_get(l, i) get(l, i)
#define list_front(l) get(l, 0)
#define list_back(l) get(l, (l).len - 1)
#define list_pop(l) (l).ptr[aven_assert_lt_internal(--(l).len, (l).cap)]
#define list_push(l) (l).ptr[aven_assert_lt_internal((l).len++, (l).cap)]
#define list_clear(l) do { (l).len = 0; } while (0)
#define queue_front(q) (q).ptr[(q).front + aven_assert_lt_internal(0, (q).used)]
#define queue_back(q) (q).ptr[(q).back + aven_assert_lt_internal(0, (q).used)]
#define queue_pop(q) (q).ptr[ \
        aven_queue_pop_internal(&(q).used, &(q).front, (q).cap) \
    ]
#define queue_push(q) (q).ptr[ \
        aven_queue_push_internal(&(q).used, &(q).back, (q).cap) \
    ]
#define queue_clear(q) do { \
        (q).used = 0; \
        (q).front = 0; \
        (q).back = 0; \
    } while (0)
#define pool_get(p, i) get(p, i).data
#define pool_create(p) (((p).free == 0) ? \
        aven_pool_next_internal(&(p).used, &(p).len, (p).cap) : \
        aven_pool_pop_free_internal( \
            &(p).used, \
            &(p).free, \
            get(p, (p).free - 1).parent \
        ) \
    )
#define pool_delete(p, i) aven_pool_push_free_internal( \
        &(p).used, \
        &(p).free, \
        &get(p, i).parent, \
        i \
    )
#define pool_clear(p) do { \
        (p).used = 0; \
        (p).free = 0; \
        (p).len = 0; \
    } while (0)

#define slice_array(a) { .ptr = a, .len = countof(a) }
#define slice_list(l) { .ptr = (l).ptr, .len = (l).len }
#define slice_head(s, i) { \
        .ptr = (s).ptr, \
        .len = aven_assert_lt_internal(i, (s).len + 1) \
    }
#define slice_tail(s, i) { \
        .ptr = (s).ptr + i, \
        .len = (s).len - aven_assert_lt_internal(i, (s).len + 1) \
    }
#define slice_range(s, i, j) { \
        .ptr = (s).ptr + i, \
        .len = aven_assert_lt_internal(j, (s).len + 1) - \
            aven_assert_lt_internal(i, j + 1) \
    }
#define list_array(a) { .ptr = a, .cap = countof(a) }
#define pool_array(a) { .ptr = a, .cap = countof(a) }

#define as_bytes(ref) (ByteSlice){ \
        .ptr = (unsigned char *)ref, \
        .len = sizeof(*ref) \
    }
#define array_as_bytes(arr) (ByteSlice){ \
        .ptr = (unsigned char *)arr, \
        .len = sizeof(arr)\
    }
#define slice_as_bytes(s) (ByteSlice){ \
        .ptr = (unsigned char *)(s).ptr, \
        .len = (s).len * sizeof(*(s).ptr), \
    }

#if defined(_WIN32) and defined(_MSC_VER)
    void *memcpy(void *s1, const void *s2, size_t n);
#else
    void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
#endif

void *memset(void *ptr, int value, size_t num);

#define slice_copy(d, s) memcpy( \
        (d).ptr, \
        (s).ptr, \
        aven_assert_lt_internal( \
            (s).len * sizeof(*(s).ptr), \
            (d).len * sizeof(*(d).ptr) + 1 \
        ) \
    )

static inline AVEN_NORETURN void aven_panic_internal_fn(
    const char *msg,
    size_t len
) {
    AVEN_NORETURN void _Exit(int status);

#ifdef _WIN32
    int _write(int fd, const void *buffer, unsigned int count);
    _write(2, msg, (unsigned int)len);
#else
    long write(int fd, const void *buffer, size_t count);
    write(2, msg, len);
#endif

    _Exit(1);
}

#define aven_panic_internal_s(x) #x
#define aven_panic_internal_sfy(x) aven_panic_internal_s(x)
#define aven_panic_internal_fmt(msg) \
    "panic at " \
    __FILE__ \
    ":" \
    aven_panic_internal_sfy(__LINE__) \
    "\n    error: " \
    msg \
    "\n"
#define aven_panic_internal_ex(msg) aven_panic_internal_fn(msg, sizeof(msg) - 1)
#define aven_panic(msg) aven_panic_internal_ex( \
        aven_panic_internal_fmt(msg) \
    )

#if defined(AVEN_IMPLEMENTATION) and !defined(AVEN_IMPLEMENTATION_SEPARATE_TU)
    #define AVEN_FN static inline
#else
    #define AVEN_FN
#endif

#ifdef _WIN32
    #define AVEN_WIN32_FN(t) __declspec(dllimport) t __stdcall
#endif

#endif // AVEN_H
