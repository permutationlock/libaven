# The libaven C style guide

This document provides a short summary of my personal style of C programming.

## Debugging

I program for a debugger, e.g. I assume that I will always run debug builds with
a debugger. I use an unreachable assert macro and compile with `-fsanitize-trap`
and `-fsanitize=unreachable,undefined`.
```
#define assert(c) ((!(c)) ? __builtin_unreachable() : (void)0)
```
Any failed assertions will trap the debugger directly at the failure source
location and allow for program state inspection, backtraces, etc. Assertions
should be true assertions, that is, the programmer should truly believe that they
cannot fail.

## Errors

Recoverable errors should be handled by returning an error enum `E`. If a value
of type `T` may also be returned, a `Result(T, E)` type is returned instead.
```C
#define Result(t, e) struct { t payload; e error; }
```

Non-recoverable errors should be handled by a call to `aven_panic(msg)`, which
will dump the source location and string literal `msg` to stderr and call
`_Exit(1)`.

## Memory

I use arenas for all memory allocation except for allocating arenas. Arenas are
allocated using either `malloc` or the OS page allocator. Arenas are backed by a
single fixed size chunk of memory and are not expandable.
```C
typedef struct {
    unsigned char *base;
    unsigned char *top;
} AvenArena;
AVEN_FN void *aven_arena_alloc(
    AvenArena *arena,
    size_t count,
    size_t align,
    size_t size
);
AVEN_FN void *aven_arena_realloc(
    AvenArena *arena,
    void *ptr,
    size_t old_count,
    size_t new_count,
    size_t align,
    size_t size
);
```
The `alloc` and `realloc` functions will never return `NULL`,
allocating more space than available triggers a panic.

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

### Slice

A slice is a multi-item pointer plus a length, e.g. it refers to part of
an array in memory.
```C
#define Slice(t) struct { t *ptr; size_t len; }
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

## Pointers

All pointer parameters and variables are assumed to be non-null, single-item
pointers. E.g. if a function has the signature `void foo(S *s)`,
then it is never valid to call `foo(NULL)`.

If a multi-item pointer is desired, a slice struct `Slice(s)` should b used
instead.
```C
typedef Slice(S) SSlice;
void foo(SSlice s);

S arr[] = { s1, s2, s3 };
foo((SSlice){ .ptr = arr, .len = countof(arr) }); // Pass an array to foo
foo((SSlice)slice_array(arr)); // Pass an array to foo using helper macro
```

If an optional pointer is truly desired, then the `OptPtr(T)` union type should
be used instaed. I have not yet found a need for `OptPtr` in my own code.
```C
typedef OptPtr(S) SOptPtr;
void foo(SOptPtr s);

foo((SOptPtr){ 0 }); // Pass a null OptPtr(S) to foo
```

Multi-item data structures should not store pointers. E.g., if we store a
`Slice(S)`, then the struct `S` should not contain member of type `T*`. Instead,
a corresponding `Slice(T)`, `List(T)`, or `Pool(T)` structure should be stored,
and `S` should contain an index into that structure. This allows for smaller
storage (often a 32-bit index suffices), and allows data structures to be copied
from memory to disk and back without modification.
