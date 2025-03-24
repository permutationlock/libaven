#ifndef TEST_PATH_H
#define TEST_PATH_H

#include <aven.h>
#include <aven/arena.h>
#include <aven/fmt.h>
#include <aven/path.h>
#include <aven/str.h>
#include <aven/test.h>

typedef struct {
    AvenStr expected;
    AvenStrSlice parts;
} TestAvenPathArgs;

AvenTestResult test_aven_path(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenPathArgs *pargs = args;

    AvenStr path = { 0 };
    switch (pargs->parts.len) {
        case 0:
            break;
        case 1:
            path = aven_path(&arena, get(pargs->parts, 0));
            break;
        case 2:
            path = aven_path(
                &arena,
                get(pargs->parts, 0),
                get(pargs->parts, 1)
            );
            break;
        case 3:
            path = aven_path(
                &arena,
                get(pargs->parts, 0),
                get(pargs->parts, 1),
                get(pargs->parts, 2)
            );
            break;
        default:
            path = aven_path(
                &arena,
                get(pargs->parts, 0),
                get(pargs->parts, 1),
                get(pargs->parts, 2),
                get(pargs->parts, 3)
            );
            break;
    }

    AvenStr expected_path = pargs->expected;
    bool match = aven_str_equals(path, expected_path);

    if (!match) {
        return (AvenTestResult){
            .error = 2,
            .message = aven_fmt(
                emsg_arena,
                "expected \"{}\", found \"{}\"",
                aven_fmt_str(expected_path),
                aven_fmt_str(path)
            ),
        };
    }

    return (AvenTestResult){ 0 };
}

typedef struct {
    AvenStr expected;
    AvenStr path;
} TestAvenPathDirArgs;

AvenTestResult test_aven_path_containing_dir(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    (void)arena;
    TestAvenPathDirArgs *pargs = args;

    AvenStr path = aven_path_containing_dir(pargs->path);
    AvenStr expected_path = pargs->expected;
    bool match = aven_str_equals(path, expected_path);

    if (!match) {
        return (AvenTestResult){
            .error = 1,
            .message = aven_fmt(
                emsg_arena,
                "expected \"{}\", found \"{}\"",
                aven_fmt_str(expected_path),
                aven_fmt_str(path)
            ),
        };
    }

    return (AvenTestResult){ 0 };
}

typedef struct {
    AvenStr expected;
    AvenStr path1;
    AvenStr path2;
} TestAvenPathDiffArgs;

AvenTestResult test_aven_path_rel_diff(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenPathDiffArgs *pargs = args;

    AvenStr path = aven_path_rel_diff(
        pargs->path1,
        pargs->path2,
        &arena
    );
    AvenStr expected_path = pargs->expected;
    bool match = aven_str_equals(path, expected_path);

    if (!match) {
        return (AvenTestResult){
            .error = 1,
            .message = aven_fmt(
                emsg_arena,
                "expected \"{}\", found \"{}\"",
                aven_fmt_str(expected_path),
                aven_fmt_str(path)
            ),
        };
    }

    return (AvenTestResult){ 0 };
}

typedef struct {
    AvenStr expected;
    AvenStr path1;
    AvenStr path2;
} TestAvenPathIntersectArgs;

AvenTestResult test_aven_path_rel_intersect(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenPathIntersectArgs *pargs = args;

    AvenStr path = aven_path_rel_intersect(
        pargs->path1,
        pargs->path2,
        &arena
    );
    AvenStr expected_path = pargs->expected;
    bool match = aven_str_equals(path, expected_path);

    if (!match) {
        return (AvenTestResult){
            .error = 1,
            .message = aven_fmt(
                emsg_arena,
                "expected \"{}\", found \"{}\"",
                aven_fmt_str(expected_path),
                aven_fmt_str(path)
            ),
        };
    }

    return (AvenTestResult){ 0 };
}

static int test_path(AvenArena arena) {
    AvenTestCase tcase_data[] = {
        {
            .desc = aven_str("aven_path empty path"),
            .fn = test_aven_path,
            .args = &(TestAvenPathArgs){
                .expected = aven_str(""),
                .parts = slice_array((AvenStr[]){ aven_str("") }),
            },
        },
        {
            .desc = aven_str("aven_path 1 level path"),
            .fn = test_aven_path,
            .args = &(TestAvenPathArgs){
                .expected = aven_str("dir"),
                .parts = slice_array((AvenStr[]){ aven_str("dir") }),
            },
        },
        {
            .desc = aven_str("aven_path 2 level path"),
            .fn = test_aven_path,
            .args = &(TestAvenPathArgs){
#ifdef _WIN32
                .expected = aven_str("dir\\a"),
#else
                .expected = aven_str("dir/a"),
#endif
                .parts = slice_array(
                    (AvenStr[]){
                        aven_str("dir"),
                        aven_str("a")
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_path 3 level path"),
            .fn = test_aven_path,
            .args = &(TestAvenPathArgs){
#ifdef _WIN32
                .expected = aven_str("learn\\you\\for"),
#else
                .expected = aven_str("learn/you/for"),
#endif
                .parts = slice_array(
                    (AvenStr[]){
                        aven_str("learn"),
                        aven_str("you"),
                        aven_str("for"),
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_path 4 level path"),
            .fn = test_aven_path,
            .args = &(TestAvenPathArgs){
#ifdef _WIN32
                .expected = aven_str("purely\\functional\\data\\structures"),
#else
                .expected = aven_str("purely/functional/data/structures"),
#endif
                .parts = slice_array(
                    (AvenStr[]){
                        aven_str("purely"),
                        aven_str("functional"),
                        aven_str("data"),
                        aven_str("structures"),
                    }
                ),
            },
        },
        {
            .desc = aven_str("aven_path_containing_dir 1 level relative path"),
            .fn = test_aven_path_containing_dir,
            .args = &(TestAvenPathDirArgs){
                .expected = aven_str("."),
                .path = aven_str("dir"),
            },
        },
        {
            .desc = aven_str("aven_path_containing_dir current dir"),
            .fn = test_aven_path_containing_dir,
            .args = &(TestAvenPathDirArgs){
#ifdef _WIN32
                .expected = aven_str(".."),
                .path = aven_str("."),
#else
                .expected = aven_str(".."),
                .path = aven_str("."),
#endif
            },
        },
#ifndef _WIN32
        {
            .desc = aven_str("aven_path_containing_dir root dir"),
            .fn = test_aven_path_containing_dir,
            .args = &(TestAvenPathDirArgs){
                .expected = aven_str("/.."),
                .path = aven_str("/"),
            },
        },
#endif
        {
            .desc = aven_str("aven_path_containing_dir 2 level relative path"),
            .fn = test_aven_path_containing_dir,
            .args = &(TestAvenPathDirArgs){
#ifdef _WIN32
                .expected = aven_str("a"),
                .path = aven_str("a\\b"),
#else
                .expected = aven_str("a"),
                .path = aven_str("a/b"),
#endif
            },
        },
        {
            .desc = aven_str("aven_path_containing_dir 2 level absolute path"),
            .fn = test_aven_path_containing_dir,
            .args = &(TestAvenPathDirArgs){
#ifdef _WIN32
                .expected = aven_str("C:\\a"),
                .path = aven_str("C:\\a\\b"),
#else
                .expected = aven_str("/a"),
                .path = aven_str("/a/b"),
#endif
            },
        },
#ifndef _WIN32
        {
            .desc = aven_str(
                "aven_path_containing_dir filename starting with \'.\'"
            ),
            .fn = test_aven_path_containing_dir,
            .args = &(TestAvenPathDirArgs){
                .expected = aven_str("."),
                .path = aven_str(".hidden"),
            },
        },
        {
            .desc = aven_str(
                "aven_path_containing_dir 2 level dirname starting with \'.\'"
            ),
            .fn = test_aven_path_containing_dir,
            .args = &(TestAvenPathDirArgs){
                .expected = aven_str(".hidden"),
                .path = aven_str(".hidden/.file"),
            },
        },
#endif
        {
            .desc = aven_str("aven_path_rel_diff same dir relative path"),
            .fn = test_aven_path_rel_diff,
            .args = &(TestAvenPathDiffArgs){
#ifdef _WIN32
                .expected = aven_str("."),
                .path1 = aven_str("dir"),
                .path2 = aven_str("dir"),
#else
                .expected = aven_str("."),
                .path1 = aven_str("dir"),
                .path2 = aven_str("dir"),
#endif
            },
        },
        {
            .desc = aven_str(
                "aven_path_rel_diff same dir relative path w/ '.' prefix"
            ),
            .fn = test_aven_path_rel_diff,
            .args = &(TestAvenPathDiffArgs){
#ifdef _WIN32
                .expected = aven_str("."),
                .path1 = aven_str(".\\dir"),
                .path2 = aven_str("dir"),
#else
                .expected = aven_str("."),
                .path1 = aven_str("./dir"),
                .path2 = aven_str("dir"),
#endif
            },
        },
        {
            .desc = aven_str("aven_path_rel_diff neighbor relative path"),
            .fn = test_aven_path_rel_diff,
            .args = &(TestAvenPathDiffArgs){
#ifdef _WIN32
                .expected = aven_str(".\\..\\a"),
                .path1 = aven_str("a"),
                .path2 = aven_str("b"),
#else
                .expected = aven_str("./../a"),
                .path1 = aven_str("a"),
                .path2 = aven_str("b"),
#endif
            },
        },
        {
            .desc = aven_str(
                "aven_path_rel_diff neighbor relative path w/ '.' prefix"
            ),
            .fn = test_aven_path_rel_diff,
            .args = &(TestAvenPathDiffArgs){
#ifdef _WIN32
                .expected = aven_str(".\\..\\a"),
                .path1 = aven_str("a"),
                .path2 = aven_str(".\\b"),
#else
                .expected = aven_str("./../a"),
                .path1 = aven_str("a"),
                .path2 = aven_str("./b"),
#endif
            },
        },
        {
            .desc = aven_str("aven_path_rel_diff subdir relative path"),
            .fn = test_aven_path_rel_diff,
            .args = &(TestAvenPathDiffArgs){
#ifdef _WIN32
                .expected = aven_str(".\\.."),
                .path1 = aven_str("a"),
                .path2 = aven_str("a\\b"),
#else
                .expected = aven_str("./.."),
                .path1 = aven_str("a"),
                .path2 = aven_str("a/b"),
#endif
            },
        },
        {
            .desc = aven_str("aven_path_rel_diff superdir relative path"),
            .fn = test_aven_path_rel_diff,
            .args = &(TestAvenPathDiffArgs){
#ifdef _WIN32
                .expected = aven_str(".\\b"),
                .path1 = aven_str("a\\b"),
                .path2 = aven_str("a"),
#else
                .expected = aven_str("./b"),
                .path1 = aven_str("a/b"),
                .path2 = aven_str("a"),
#endif
            },
        },
#ifndef _WIN32
        {
            .desc = aven_str("aven_path_rel_diff superdir w/\'.\' in name"),
            .fn = test_aven_path_rel_diff,
            .args = &(TestAvenPathIntersectArgs){
                .expected = aven_str("./.b"),
                .path1 = aven_str(".a/.b"),
                .path2 = aven_str(".a"),
            },
        },
#endif
        {
            .desc = aven_str("aven_path_rel_intersect same dir relative path"),
            .fn = test_aven_path_rel_intersect,
            .args = &(TestAvenPathIntersectArgs){
#ifdef _WIN32
                .expected = aven_str(".\\dir"),
                .path1 = aven_str("dir"),
                .path2 = aven_str("dir"),
#else
                .expected = aven_str("./dir"),
                .path1 = aven_str("dir"),
                .path2 = aven_str("dir"),
#endif
            },
        },
        {
            .desc = aven_str(
                "aven_path_rel_intersect same dir relative path w/ '.' prefix"
            ),
            .fn = test_aven_path_rel_intersect,
            .args = &(TestAvenPathIntersectArgs){
#ifdef _WIN32
                .expected = aven_str(".\\dir"),
                .path1 = aven_str(".\\dir"),
                .path2 = aven_str("dir"),
#else
                .expected = aven_str("./dir"),
                .path1 = aven_str("./dir"),
                .path2 = aven_str("dir"),
#endif
            },
        },
        {
            .desc = aven_str(
                "aven_path_rel_intersect same dir relative path w/ '.' prefix"
            ),
            .fn = test_aven_path_rel_intersect,
            .args = &(TestAvenPathIntersectArgs){
#ifdef _WIN32
                .expected = aven_str(".\\dir"),
                .path1 = aven_str("dir"),
                .path2 = aven_str(".\\dir"),
#else
                .expected = aven_str("./dir"),
                .path1 = aven_str("dir"),
                .path2 = aven_str("./dir"),
#endif
            },
        },
        {
            .desc = aven_str("aven_path_rel_intersect neighbor relative path"),
            .fn = test_aven_path_rel_intersect,
            .args = &(TestAvenPathIntersectArgs){
#ifdef _WIN32
                .expected = aven_str("."),
                .path1 = aven_str("a"),
                .path2 = aven_str("b"),
#else
                .expected = aven_str("."),
                .path1 = aven_str("a"),
                .path2 = aven_str("b"),
#endif
            },
        },
        {
            .desc = aven_str(
                "aven_path_rel_intersect neighbor relative path w/ '.' prefix"
            ),
            .fn = test_aven_path_rel_intersect,
            .args = &(TestAvenPathIntersectArgs){
#ifdef _WIN32
                .expected = aven_str("."),
                .path1 = aven_str("a"),
                .path2 = aven_str(".\\b"),
#else
                .expected = aven_str("."),
                .path1 = aven_str("a"),
                .path2 = aven_str("./b"),
#endif
            },
        },
        {
            .desc = aven_str("aven_path_rel_intersect subdir relative path"),
            .fn = test_aven_path_rel_intersect,
            .args = &(TestAvenPathIntersectArgs){
#ifdef _WIN32
                .expected = aven_str(".\\a"),
                .path1 = aven_str("a"),
                .path2 = aven_str("a\\b"),
#else
                .expected = aven_str("./a"),
                .path1 = aven_str("a"),
                .path2 = aven_str("a/b"),
#endif
            },
        },
        {
            .desc = aven_str("aven_path_rel_intersect superdir relative path"),
            .fn = test_aven_path_rel_intersect,
            .args = &(TestAvenPathIntersectArgs){
#ifdef _WIN32
                .expected = aven_str(".\\a"),
                .path1 = aven_str("a\\b"),
                .path2 = aven_str("a"),
#else
                .expected = aven_str("./a"),
                .path1 = aven_str("a/b"),
                .path2 = aven_str("a"),
#endif
            },
        },
#ifndef _WIN32
        {
            .desc = aven_str("aven_path_rel_intersect superdir w/\'.\' in name"),
            .fn = test_aven_path_rel_intersect,
            .args = &(TestAvenPathIntersectArgs){
                .expected = aven_str("./.a"),
                .path1 = aven_str(".a/.b"),
                .path2 = aven_str(".a"),
            },
        },
#endif
    };
    AvenTestCaseSlice tcases = slice_array(tcase_data);

    aven_test(tcases, arena);

    return 0;
}

#endif // TEST_PATH_H
