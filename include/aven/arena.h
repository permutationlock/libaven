#ifndef AVEN_ARENA_H
#define AVEN_ARENA_H

#include "../aven.h"

#if defined(__STDC_VERSION__) and __STDC_VERSION__ >= 201112L
    #include <stdalign.h>
    #define aven_arena_alignof(t) alignof(t)
    #if defined(_WIN32) and defined(_MSC_VER)
        // Oh Microsoft...
        #define AVEN_ARENA_BIGGEST_ALIGNMENT 16
    #else
        #define AVEN_ARENA_BIGGEST_ALIGNMENT (alignof(max_align_t))
    #endif
#elif defined(__STDC_VERSION__) and __STDC_VERSION__ >= 199901L
    #ifndef __BIGGEST_ALIGNMENT__
        #error "__BIGGEST_ALIGNMENT__ must be the max required alignment"
    #endif
    #define aven_arena_alignof(t) __BIGGEST_ALIGNMENT__
    #define AVEN_ARENA_BIGGEST_ALIGNMENT __BIGGEST_ALIGNMENT__
#else
    #error "C99 or later is required"
#endif

// Inspired by and/or copied from Chris Wellons (https://nullprogram.com)

typedef struct {
    unsigned char *base;
    unsigned char *top;
} AvenArena;

static inline AvenArena aven_arena_init(void *mem, size_t size) {
    return (AvenArena){ .base = mem, .top = (unsigned char *)mem + size };
}

#if __has_attribute(malloc)
    __attribute__((malloc))
#endif
#if !defined(AVEN_IMPLEMENTATION)
    // These attributes cause issues when compiling as one translation unit
    #if __has_attribute(alloc_size)
        __attribute__((alloc_size(2, 4)))
    #endif
    #if __has_attribute(alloc_align)
        __attribute__((alloc_align(3)))
    #endif
#endif
AVEN_FN void *aven_arena_alloc(
    AvenArena *arena,
    size_t count,
    size_t align,
    size_t size
);
#if !defined(AVEN_IMPLEMENTATION)
    // These attributes cause issues when compiling as one translation unit
    #if __has_attribute(alloc_size)
        __attribute__((alloc_size(4, 6)))
    #endif
    #if __has_attribute(alloc_align)
        __attribute__((alloc_align(5)))
    #endif
#endif
AVEN_FN void *aven_arena_realloc(
    AvenArena *arena,
    void *ptr,
    size_t old_count,
    size_t new_count,
    size_t align,
    size_t size
);

#define aven_arena_create(t, a) (t *)aven_arena_alloc( \
        a, \
        1, \
        aven_arena_alignof(t), \
        sizeof(t) \
    )
#define aven_arena_create_array(t, a, n) (t *)aven_arena_alloc( \
        a, \
        n, \
        aven_arena_alignof(t), \
        sizeof(t) \
    )
#define aven_arena_resize_array(t, a, p, oc, nc) (t *)aven_arena_realloc( \
        a, \
        p, \
        oc, \
        nc, \
        aven_arena_alignof(t), \
        sizeof(t) \
    )

#define aven_arena_create_slice(t, a, n) { \
        .ptr = aven_arena_create_array(t, a, n), \
        .len = n, \
    }
#define aven_arena_create_list(t, a, n) { \
        .ptr = aven_arena_create_array(t, a, n), \
        .cap = n, \
    }
#define aven_arena_create_queue(t, a, n) { \
        .ptr = aven_arena_create_array(t, a, n), \
        .cap = n, \
    }
#define aven_arena_create_pool(t, a, n) { \
        .ptr = (void *)aven_arena_create_array(PoolEntry(t), a, n), \
        .cap = n, \
    }

#ifdef AVEN_IMPLEMENTATION

AVEN_FN void *aven_arena_alloc(
    AvenArena *arena,
    size_t count,
    size_t align,
    size_t size
) {
    assert((align & (align - 1)) == 0);

    ptrdiff_t padding = (ptrdiff_t)(-(uintptr_t)arena->base & (align - 1));
    ptrdiff_t available = arena->top - arena->base - padding;
    if (available < 0 || count > ((size_t)available / size)) {
        aven_panic("arena out of memory");
    }

    void *ptr = arena->base + padding;
    arena->base += (size_t)padding + size * count;
    return ptr;
}

AVEN_FN void *aven_arena_realloc(
    AvenArena *arena,
    void *ptr,
    size_t old_count,
    size_t new_count,
    size_t align,
    size_t size
) {
    if ((unsigned char *)ptr + size * old_count == arena->base) {
        arena->base = ptr;
        return aven_arena_alloc(arena, new_count, align, size);
    }

    if (new_count <= old_count) {
        return ptr;
    }

    void *new_ptr = aven_arena_alloc(arena, new_count, align, size);
    memcpy(new_ptr, ptr, old_count * size);
    return new_ptr;
}

#endif // AVEN_IMPLEMENTATION

#endif // AVEN_ARENA_H
