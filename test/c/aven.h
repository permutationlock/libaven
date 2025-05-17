#ifndef AVEN_H
    #define AVEN_H

    #include <stdbool.h>
    #include <stddef.h>
    #include <stdint.h>

    #define or ||
    #define and &&

    #ifndef _WIN32
        #ifdef AVEN_LINUX_NOLIBC
            #include <nolibc.h>
            #ifdef AVEN_IMPLEMENTATION
                #include <stackprotector.h>
                #include <crt.h>
            #endif
        #endif
        #if defined(__linux__) and defined(NOLIBC)
            #include <sys.h>
        #else
            #include <sys/types.h>
        #endif
    #endif

    #define min(a, b) (((a) < (b)) ? (a) : (b))
    #define max(a, b) (((a) > (b)) ? (a) : (b))

    #ifndef __has_attribute
        #define __has_attribute(unused) 0
    #endif

    #ifndef __has_builtin
        #define __has_builtin(unused) 0
    #endif

    #if defined(AVEN_USE_PANIC_ASSERT)
        #define assert(c) ( \
                (!(c)) ? aven_panic("assert(" #c ") failed") : (void)0 \
            )
    #elif !defined(AVEN_USE_STD_ASSERT) and \
            __has_builtin(__builtin_unreachable)
        #define assert(c) ((!(c)) ? __builtin_unreachable() : (void)0)
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

    #define countof(...) (sizeof(__VA_ARGS__) / sizeof(*(__VA_ARGS__)))

    #define Optional(t) struct { \
            t value; \
            uint8_t valid; \
        }
    #define OptPtr(t) union { \
            t *value; \
            t *valid; \
        }
    #define Result(t, e) struct { \
            t payload; \
            e error; \
        }
    #define Slice(t) struct { \
            t *ptr; \
            size_t len; \
        }
    #define List(t) struct { \
            t *ptr; \
            size_t len; \
            size_t cap; \
        }
    #define Queue(t) struct { \
            t *ptr; \
            size_t cap; \
            size_t front; \
            size_t back; \
            size_t used; \
        }

    #define PoolEntry(t) union { \
            t data; \
            uint64_t parent; \
        }
    #define PoolExplicit(e) struct { \
            e *ptr; \
            size_t len; \
            size_t cap; \
            size_t used; \
            size_t free; \
        }
    #define Pool(t) PoolExplicit(PoolEntry(t))

    typedef Slice(unsigned char) ByteSlice;

    static inline size_t aven_queue_push_internal(
        size_t *used,
        size_t *back,
        size_t cap
    ) {
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
        *used -= 1;
        size_t index = *front;
        *front = index + 1;
        if (*front >= cap) {
            *front -= cap;
        }
        return index;
    }

    static inline size_t aven_pool_next_internal(size_t *used, size_t *len) {
        *used += 1;
        size_t index = *len;
        *len += 1;
        return index;
    }

    static inline size_t aven_pool_pop_free_internal(
        size_t *used,
        size_t *free,
        uint64_t parent
    ) {
        *used += 1;

        size_t index = *free;
        *free = (size_t)parent;
        return index - 1;
    }

    static inline void aven_pool_push_free_internal(
        size_t *used,
        size_t *free,
        uint64_t *parent,
        size_t index
    ) {
        *used -= 1;
        *parent = *free;
        *free = index + 1;
    }

    #define unwrap(o) (assert((o).valid), (o).value)
    #define get(s, i) (s).ptr[(assert((i) < (s).len), i)]
    #define list_get(l, i) get(l, i)
    #define list_front(l) get(l, 0)
    #define list_back(l) get(l, (l).len - 1)
    #define list_delete(l, i) (assert(i <= (l).len), l.ptr[i] = list_pop(l))
    #define list_pop(l) (l).ptr[(assert((l).len > 0), --(l).len)]
    #define list_push(l) (l).ptr[(assert((l).len < (l).cap), (l).len++)]
    #define list_clear(l) do { \
            (l).len = 0; \
        } while (0)
    #define queue_get(q, i) (q).ptr[ \
            ( \
                assert((i) < (q).used), \
                ((q).front + (i) < (q).cap) ? \
                    (q).front + (i) : \
                    (q).front - (q).cap + (i) \
            ) \
        ]
    #define queue_front(q) (q).ptr[(assert((q).used > 0), (q).front)]
    #define queue_back(q) (q).ptr[(assert((q).used > 0), (q).back)]
    #define queue_pop(q) (q).ptr[ \
            ( \
                assert((q).used > 0), \
                aven_queue_pop_internal(&(q).used, &(q).front, (q).cap) \
            ) \
        ]
    #define queue_push(q) (q).ptr[ \
            ( \
                assert((q).used < (q).cap), \
                aven_queue_push_internal(&(q).used, &(q).back, (q).cap) \
            ) \
        ]
    #define queue_clear(q) do { \
            (q).used = 0; \
            (q).front = 0; \
            (q).back = 0; \
        } while (0)
    #define pool_get(p, i) get(p, i).data
    #define pool_create(p) ( \
            ((p).free == 0) ? \
                ( \
                    assert((p).len < (p).cap and (p).used == (p).len), \
                    aven_pool_next_internal(&(p).used, &(p).len) \
                ) : \
                ( \
                    assert((p).used < (p).cap), \
                    aven_pool_pop_free_internal( \
                        &(p).used, \
                        &(p).free, \
                        get(p, (p).free - 1).parent \
                    ) \
                ) \
        )
    #define pool_delete(p, i) ( \
            assert((p).used > 0), \
            aven_pool_push_free_internal( \
                &(p).used, \
                &(p).free, \
                &get(p, i).parent, \
                i \
            ) \
        )
    #define pool_clear(p) do { \
            (p).used = 0; \
            (p).free = 0; \
            (p).len = 0; \
        } while (0)

    #define slice_array(...) { \
            .ptr = (__VA_ARGS__), \
            .len = countof(__VA_ARGS__), \
        }
    #define slice_list(l) { .ptr = (l).ptr, .len = (l).len }
    #define slice_list_free(l) { \
            .ptr = (l).ptr + (l).len, \
            .len = (l).cap - (l).len, \
        }
    #define slice_queue_front(q) { \
            .ptr = ((q).used > 0) ? (q).ptr + (q).front : NULL, \
            .len = min((q).cap, (q).front + (q).used) - (q).front, \
        }
    #define slice_queue_back(q) { \
            .ptr = (q).ptr, \
            .len = ((q).front + (q).used <= (q).cap) ? 0 : (q).back, \
        }
    #define slice_head(s, i) { \
            .ptr = (s).ptr, \
            .len = (assert((i) <= (s).len), (i)), \
        }
    #define slice_tail(s, i) { \
            .ptr = ((s).len > 0) ? (s).ptr + (i) : NULL, \
            .len = (assert((i) <= (s).len), (s).len - (i)), \
        }
    #define slice_range(s, i, j) { \
            .ptr = ((s).len > 0) ? (s).ptr + (i) : NULL, \
            .len = (assert((j) <= (s).len), assert((i) <= (j)), ((j)) - (i)), \
        }
    #define list_array(...) { \
            .ptr = (__VA_ARGS__), \
            .cap = countof(__VA_ARGS__), \
        }
    #define queue_array(...) { \
            .ptr = (__VA_ARGS__), \
            .cap = countof(__VA_ARGS__), \
        }
    #define pool_array(...) { \
            .ptr = (__VA_ARGS__), \
            .cap = countof(__VA_ARGS__), \
        }

    #define as_bytes(ref) (ByteSlice){ \
            .ptr = (unsigned char *)ref, \
            .len = sizeof(*ref), \
        }
    #define array_as_bytes(arr) (ByteSlice){ \
            .ptr = (unsigned char *)arr, \
            .len = sizeof(arr), \
        }
    #define slice_as_bytes(s) (ByteSlice){ \
            .ptr = (unsigned char *)(s).ptr, \
            .len = (s).len * sizeof(*(s).ptr), \
        }
    #define list_as_bytes(l) slice_as_bytes(l)
    #define queue_front_as_bytes(q) (ByteSlice){ \
            .ptr = ((q).used > 0) ? \
                (unsigned char *)((q).ptr + (q).front) : \
                NULL, \
            .len = sizeof(*(q).ptr) * \
                (min((q).cap, (q).front + (q).used) - (q).front), \
        }
    #define queue_back_as_bytes(q) (ByteSlice){ \
            .ptr = (unsigned char *)(q).ptr, \
            .len = sizeof(*(q).ptr) * \
                (((q).front + (q).used <= (q).cap) ? 0 : (q).back), \
        }
    #define pool_as_bytes(p) slice_as_bytes(p)

    #if defined(_WIN32) and defined(_MSC_VER)
        void *memcpy(void *s1, const void *s2, size_t n);
    #else
        void *memcpy(void *restrict s1, const void *restrict s2, size_t n);
    #endif

    void *memset(void *ptr, int value, size_t num);
    int memcmp(const void *buffer1, const void *buffer2, size_t count);

    #define slice_copy(d, s) ( \
            (s).len > 0 ? \
                memcpy( \
                    (d).ptr, \
                    (s).ptr, \
                    ( \
                        assert( \
                            (s).len * sizeof(*(s).ptr) <= (d).len * \
                                sizeof(*(d).ptr) \
                        ), \
                        (s).len * sizeof(*(s).ptr) \
                    ) \
                ) : \
                (void *)0 \
        )

    static inline bool bytes_equal(ByteSlice b1, ByteSlice b2) {
        return (b1.len == b2.len) and (memcmp(b1.ptr, b2.ptr, b1.len) == 0);
    }

    static inline AVEN_NORETURN void aven_panic_internal_fn(
        const char *msg,
        size_t len
    ) {
        AVEN_NORETURN void exit(int status);

    #ifdef _WIN32
        int _write(int fd, const void *buffer, unsigned int count);
        _write(2, msg, (unsigned int)len);
    #elif defined(__linux__) and defined(NOLIBC)
        write(2, msg, len);
    #else
        ssize_t write(int fd, const void *buffer, size_t count);
        write(2, msg, len);
    #endif

    #if defined(__GNUC__) and __has_builtin(__builtin_trap)
        __builtin_trap();
    #endif
        exit(1);
    }
    #define aven_panic_internal_s(x) #x
    #define aven_panic_internal_sfy(x) aven_panic_internal_s(x)
    #define aven_panic_internal_fmt(msg) "panic at " \
            __FILE__ \
            ":" \
            aven_panic_internal_sfy(__LINE__) \
            "\n    error: " \
            msg \
            "\n"
    #define aven_panic_internal_ex(msg) aven_panic_internal_fn( \
            msg, \
            sizeof((msg)) - 1 \
        )
    #define aven_panic(msg) aven_panic_internal_ex( \
            aven_panic_internal_fmt(msg) \
        )

    #if defined(AVEN_IMPLEMENTATION) and \
            !defined(AVEN_IMPLEMENTATION_SEPARATE_TU)
        #define AVEN_FN static inline
    #else
        #define AVEN_FN
    #endif

    #ifdef _WIN32
        #define AVEN_WIN32_FN(t) __declspec(dllimport) t __stdcall
    #endif

#endif
// AVEN_H
