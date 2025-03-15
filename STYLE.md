# The libaven C style guide

This document provides a short summary of my personal style of C programming.

## Debugging

I program for a debugger, i.e. I assume that I will always run debug builds with
a debugger. I use an unreachable assert macro and compile with `-fsanitize-trap`
and `-fsanitize=unreachable,undefined`.
```
#define assert(c) ((!(c)) ? __builtin_unreachable() : (void)0)
```
Any failed assertions will trap the debugger directly at the failure source
location and allow for program state inspection, backtraces, etc. Assertions
should be true assertions, that is, the programmer should truly believe that they
cannot fail.

If high memory safety is desired for cases that may have not come up during
debug tests, the `AVEN_USE_PANIC_ASSERT` macro may be defined.
Such a build will panic on all common memory errors.

## Errors

Recoverable errors should generally be handled immediately. If they cannot be
handled immediately, they may be be bubbled the stack up by returning an error
enum `E`.
If a value of type `T` will returned when no error occurs, then a `Result(T, E)`
struct is used as the return type.
```C
#define Result(t, e) struct { t payload; e error; }
```
```C
typedef enum {
    FOO_ERROR_NONE = 0,
    FOO_ERROR_SCARY_EXTERNAL_FAILED,
} FooError;
typedef Result(T, FooError) FooResult;

FooResult foo(void) {
    T t;
    int error = scary_external_get_t(&t);
    if (error != 0) {
        return (FooResult){ .error = FOO_ERROR_SCARY_EXTERNAL_FAILED };
    }

    return (FooResult){ .paylaod = t };
}
```

Non-recoverable errors should be handled by a call to `aven_panic(msg)`, which
will dump the source location and string literal `msg` to stderr and call
`_Exit(1)`.
```
if (!necessary_condition) {
    aven_panic("necessary condition failed");
}
```

## Core generic data structures

There are several simple "generic" data structures that I use frequently.

### Optional

An `Optional(T)` is a structure that may contain a valid `T`.
```C
#define Optional(t) struct { t value; bool valid; }
```
The `valid` member should be checked before accessing the `value` member.
```C
void foo(T);
Optional(T) opt = { 0 };
if (cond) {
    opt.value = my_value;
    opt.valid = true;
}
// ...
if (opt.valid) {
    foo(opt.value);
}
```
The `unwrap(opt)` macro asserts `opt.valid` and retrieves `opt.value`.
```C
void foo(T);
Optional(T) opt = { .value = my_value, .valid = true };
foo(unwrap(opt));
```

### OptPtr

The `OptPtr(T)` type may be used in the same way as `Optional(T *)`, but it only
takes the memory space of a `T *`.
```C
#define OptPtr(t) union { t *value; t *valid; }
```
This works because `if (ptr)` is false if and only if `ptr==NULL`.
It is essentially no different than a `T *`, but forces the programmer
to unwrap the pointer from the union and consider the `NULL` case.

### Slice

A slice is a multi-item pointer plus a length, e.g. it refers to a
contiguos section of an array in memory.
```C
#define Slice(t) struct { t *ptr; size_t len; }
```
The `slice_array` macro constructs a slice from an array.
```C
#define slice_array(a) { .ptr = a, .len = countof(a) }
```
Slice elements should be accessed with the bounds checked `get` macro.
```C
int backing_array[] = { 1, 1, 2, 3, 5, 8 };
Slice(int) fibo = slice_array(backing_array);
assert(get(fibo, 4) == 5);
```

### List

A list is a fixed capacity array list structure.
```C
#define List(t) struct { t *ptr; size_t len; size_t cap; }
```
The same `get` macro may be used to access elements of a list.
Elements can be pushed and
popped from the tail of the list like a stack data structure.
```C
int backing_array[16];
List(int) int_list = list_array(backing_array);
list_push(int_list) = 4;
list_push(int_list) = 5;
int back = list_pop(int_list);
assert(back == 5);
assert(int_list.len == 1);
```

### Queue

A queue is a FIFO queue implemented with a circular array.
```C
int backing_array[16];
Queue(int) int_queue = queue_array(backing_array);
queue_push(int_queue) = 4;
queue_push(int_queue) = 5;
int front = queue_pop(int_queue);
assert(front == 4);
assert(int_queu.len == 1);
```
A queue does not allow random access via the `get` macro, only access to
the front and back with `queue_front` and `queue_back`.

### Pool

A pool is a fixed size memory pool to allocate and re-use objects of
a fixed type.
```C
Pool(int) int_pool = arena_create_pool(int, &arena, 32);
size_t handle = pool_create(int_pool);
pool_get(int_pool, handle) = 14;
// use int_pool element corresponding to handle,
// then delete and release back to the pool
pool_delete(int_pool, handle);
```
A pool is an extension of a list of `PoolEntry(T)` union objects,
so the special `pool_get` macro must be used in lieu of `get` to extract
the underlying `T` member of the union.

## Pointers

All parameters and variables of type `T *` are assumed to be non-null,
single-item pointers. E.g. if a function has the signature `void foo(S *s)`,
it is never valid to call `foo(NULL)`.

If a multi-item pointer is desired, a slice struct `Slice(s)` should b used
instead.
```C
typedef Slice(S) SSlice;
void foo(SSlice s);

S arr[] = { s1, s2, s3 };
foo((SSlice){ .ptr = arr, .len = countof(arr) }); // Pass an array to foo
foo((SSlice)slice_array(arr)); // Pass array to foo with the slice_array macro
```

If an optional pointer is truly desired, then the `OptPtr(T)` union type should
be used instead. I have not yet found a need for `OptPtr` in my own code.
```C
typedef OptPtr(S) SOptPtr;
void foo(SOptPtr s);

foo((SOptPtr){ .valid = false }); // Pass a null OptPtr(S) to foo
foo((SOptPtr){ .value = NULL }); // Pass a null OptPtr(S) to foo
S s;
foo((SOptPtr){ .value = &s }); // Pass a valid OptPtr(S) to foo
```

Multi-item data structures should never store pointers. E.g., if we store a
`Slice(S)`, then the struct `S` should not contain member of type `T*`. Instead,
a corresponding `Slice(T)`, `List(T)`, or `Pool(T)` structure should be stored,
and `S` should contain an index into that structure. This usually allows for a
reduced memory footprint (often a 32-bit index will suffice), and allows data
structures to be copied from memory to disk or over a network and back without
modification.

## Memory

I use arenas for all memory allocation, except potentially for allocating
the arenas themselves. Arenas may be allocated using other arenas,
`malloc`, or the OS page allocator. Arenas are
backed by a single fixed size chunk of memory and are not expandable.
```C
typedef struct {
    unsigned char *base;
    unsigned char *top;
} AvenArena;
void *aven_arena_alloc(
    AvenArena *arena,
    size_t count,
    size_t align,
    size_t size
);
void *aven_arena_realloc(
    AvenArena *arena,
    void *ptr,
    size_t old_count,
    size_t new_count,
    size_t align,
    size_t size
);
```
The `aven_arena_alloc` and `aven_arena_realloc` functions will always
return a valid pointer, allocating more space than available triggers a panic.

The `aven_arena_realloc` function re-sizes in place if the given `ptr` was
the last allocation by the given arena. The `aven_arena_resize` macro
asserts that re-sizing must be possible.

A variety of helper macros are provided for the core data structures
described below, e.g. `aven_arena_create_slice` and `aven_arena_resize_list`.

A common pattern is to allocate a list with some maximum capacity, push elements
based on various conditions, then "commit" the list by shrinking the
list memory to fit the current length and converting a slice.
```C
typedef Slice(int) IntSlice;
List(int) int_list = aven_arena_create_list(int, arena, MAX_LIST_LEN);
list_push(int_list) = 1;
list_push(int_list) = 2;
// ...
if (cond_a) {
    list_push(int_list) = 3;
}
if (cond_b) {
    list_push(int_list) = 4;
}
// ...
// Resize list memory to current len and return a slice of the current contents
IntSlice int_slice = aven_arena_commit_list_to_slice(IntSlice, arena, int_list);
```
A more generic version of the same pattern is to allocate a child arena with some
maximum space for the result value, and then use the remaining arena space as temporary
work space. After allocating as much as needed from the child arena,
the memory can be resized to free up unused space.
```C
// A simple and flawed funciton to find the intersection of two relative file paths
AvenStr relative_path_intersect(AvenStr path1, AvenStr path2, AvenArena *arena) {
    AvenArenaChild join_child = aven_arena_child_init(arena, min(path1.len, path2.len));
    AvenArena temp_arena = *arena;

    AvenStrSlice path1_parts = aven_str_split(path1, '/', &temp_arena);
    AvenStrSlice path2_parts = aven_str_split(path2, '/', &temp_arena);

    size_t len = min(path1_parts.len, path2_parts.len);
    size_t same_index = 0;
    for (; same_index < len; same_index += 1) {
        bool match = aven_str_compare(
            get(path1_parts, same_index),
            get(path2_parts, same_index)
        );
        if (!match) {
            break;
        }
    }

    List(AvenStr) int_list = aven_arena_create_list(AvenStr, &temp_arena, same_index);
    for (size_t i = 0; i < same_index; i += 1) {
        list_push(int_list) = get(path1_parts, i);
    }
    AvenStrSlice int_parts = slice_list(int_list);

    AvenStr int_path = aven_str_join(int_parts, '/', &join_child.arena);
    aven_arena_child_commit(arena, &join_child);

    // Only the space taken by `int_path` has been allocated from `arena`
    return int_path;
}
```
