#ifndef TEST_PATH_H
#define TEST_PATH_H

#include <aven.h>
#include <aven/arena.h>
#include <aven/fmt.h>
#include <aven/path.h>
#include <aven/str.h>
#include <aven/test.h>

typedef struct {
    char *expected;
    char *parts[8];
    size_t nparts;
} TestAvenPathArgs;

AvenTestResult test_aven_path(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenPathArgs *pargs = args;

    AvenStr path = { 0 };
    switch (pargs->nparts) {
        case 0:
            break;
        case 1:
            path = aven_path(&arena, aven_str_cstr(pargs->parts[0]));
            break;
        case 2:
            path = aven_path(
                &arena,
                aven_str_cstr(pargs->parts[0]),
                aven_str_cstr(pargs->parts[1])
            );
            break;
        case 3:
            path = aven_path(
                &arena,
                aven_str_cstr(pargs->parts[0]),
                aven_str_cstr(pargs->parts[1]),
                aven_str_cstr(pargs->parts[2])
            );
            break;
        default:
            path = aven_path(
                &arena,
                aven_str_cstr(pargs->parts[0]),
                aven_str_cstr(pargs->parts[1]),
                aven_str_cstr(pargs->parts[2]),
                aven_str_cstr(pargs->parts[3])
            );
            break;
    }

    AvenStr expected_path = aven_str_cstr(pargs->expected);
    bool match = aven_str_compare(path, expected_path);

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
    char *expected;
    char *path;
} TestAvenPathDirArgs;

AvenTestResult test_aven_path_containing_dir(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    (void)arena;
    TestAvenPathDirArgs *pargs = args;

    AvenStr path = aven_path_containing_dir(aven_str_cstr(pargs->path));
    AvenStr expected_path = aven_str_cstr(pargs->expected);
    bool match = aven_str_compare(path, expected_path);

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
    char *expected;
    char *path1;
    char *path2;
} TestAvenPathDiffArgs;

AvenTestResult test_aven_path_rel_diff(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenPathDiffArgs *pargs = args;

    AvenStr path = aven_path_rel_diff(
        aven_str_cstr(pargs->path1),
        aven_str_cstr(pargs->path2),
        &arena
    );
    AvenStr expected_path = aven_str_cstr(pargs->expected);
    bool match = aven_str_compare(path, expected_path);

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
    char *expected;
    char *path1;
    char *path2;
} TestAvenPathIntersectArgs;

AvenTestResult test_aven_path_rel_intersect(
    AvenArena *emsg_arena,
    AvenArena arena,
    void *args
) {
    TestAvenPathIntersectArgs *pargs = args;

    AvenStr path = aven_path_rel_intersect(
        aven_str_cstr(pargs->path1),
        aven_str_cstr(pargs->path2),
        &arena
    );
    AvenStr expected_path = aven_str_cstr(pargs->expected);
    bool match = aven_str_compare(path, expected_path);

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

int test_path(AvenArena arena) {
    AvenTestCase tcase_data[] = {
        {
            .desc = aven_str("aven_path empty path"),
            .fn = test_aven_path,
            .args = &(TestAvenPathArgs){
                .expected = "",
                .nparts = 1,
                .parts = { "" },
            },
        },
        {
            .desc = aven_str("aven_path 1 level path"),
            .fn = test_aven_path,
            .args = &(TestAvenPathArgs){
                .expected = "dir",
                .nparts = 1,
                .parts = { "dir" },
            },
        },
        {
            .desc = aven_str("aven_path 2 level path"),
            .fn = test_aven_path,
            .args = &(TestAvenPathArgs){
#ifdef _WIN32
                .expected = "dir\\a",
#else
                .expected = "dir/a",
#endif
                .nparts = 2,
                .parts = { "dir", "a" },
            },
        },
        {
            .desc = aven_str("aven_path 3 level path"),
            .fn = test_aven_path,
            .args = &(TestAvenPathArgs){
#ifdef _WIN32
                .expected = "learn\\you\\for",
#else
                .expected = "learn/you/for",
#endif
                .nparts = 3,
                .parts = { "learn", "you", "for" },
            },
        },
        {
            .desc = aven_str("aven_path 4 level path"),
            .fn = test_aven_path,
            .args = &(TestAvenPathArgs){
#ifdef _WIN32
                .expected = "purely\\functional\\data\\structures",
#else
                .expected = "purely/functional/data/structures",
#endif
                .nparts = 4,
                .parts = { "purely", "functional", "data", "structures" },
            },
        },
        {
            .desc = aven_str("aven_path_containing_dir 1 level relative path"),
            .fn = test_aven_path_containing_dir,
            .args = &(TestAvenPathDirArgs){
                .expected = ".",
                .path = "dir",
            },
        },
        {
            .desc = aven_str("aven_path_containing_dir current dir"),
            .fn = test_aven_path_containing_dir,
            .args = &(TestAvenPathDirArgs){
#ifdef _WIN32
                .expected = "..",
                .path = ".",
#else
                .expected = "..",
                .path = ".",
#endif
            },
        },
#ifndef _WIN32
        {
            .desc = aven_str("aven_path_containing_dir root dir"),
            .fn = test_aven_path_containing_dir,
            .args = &(TestAvenPathDirArgs){
                .expected = "/..",
                .path = "/",
            },
        },
#endif
        {
            .desc = aven_str("aven_path_containing_dir 2 level relative path"),
            .fn = test_aven_path_containing_dir,
            .args = &(TestAvenPathDirArgs){
#ifdef _WIN32
                .expected = "a",
                .path = "a\\b",
#else
                .expected = "a",
                .path = "a/b",
#endif
            },
        },
        {
            .desc = aven_str("aven_path_containing_dir 2 level absolute path"),
            .fn = test_aven_path_containing_dir,
            .args = &(TestAvenPathDirArgs){
#ifdef _WIN32
                .expected = "C:\\a",
                .path = "C:\\a\\b",
#else
                .expected = "/a",
                .path = "/a/b",
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
                .expected = ".",
                .path = ".hidden",
            },
        },
        {
            .desc = aven_str(
                "aven_path_containing_dir 2 level dirname starting with \'.\'"
            ),
            .fn = test_aven_path_containing_dir,
            .args = &(TestAvenPathDirArgs){
                .expected = ".hidden",
                .path = ".hidden/.file",
            },
        },
#endif
        {
            .desc = aven_str("aven_path_rel_diff same dir relative path"),
            .fn = test_aven_path_rel_diff,
            .args = &(TestAvenPathDiffArgs){
#ifdef _WIN32
                .expected = ".",
                .path1 = "dir",
                .path2 = "dir",
#else
                .expected = ".",
                .path1 = "dir",
                .path2 = "dir",
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
                .expected = ".",
                .path1 = ".\\dir",
                .path2 = "dir",
#else
                .expected = ".",
                .path1 = "./dir",
                .path2 = "dir",
#endif
            },
        },
        {
            .desc = aven_str("aven_path_rel_diff neighbor relative path"),
            .fn = test_aven_path_rel_diff,
            .args = &(TestAvenPathDiffArgs){
#ifdef _WIN32
                .expected = ".\\..\\a",
                .path1 = "a",
                .path2 = "b",
#else
                .expected = "./../a",
                .path1 = "a",
                .path2 = "b",
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
                .expected = ".\\..\\a",
                .path1 = "a",
                .path2 = ".\\b",
#else
                .expected = "./../a",
                .path1 = "a",
                .path2 = "./b",
#endif
            },
        },
        {
            .desc = aven_str("aven_path_rel_diff subdir relative path"),
            .fn = test_aven_path_rel_diff,
            .args = &(TestAvenPathDiffArgs){
#ifdef _WIN32
                .expected = ".\\..",
                .path1 = "a",
                .path2 = "a\\b",
#else
                .expected = "./..",
                .path1 = "a",
                .path2 = "a/b",
#endif
            },
        },
        {
            .desc = aven_str("aven_path_rel_diff superdir relative path"),
            .fn = test_aven_path_rel_diff,
            .args = &(TestAvenPathDiffArgs){
#ifdef _WIN32
                .expected = ".\\b",
                .path1 = "a\\b",
                .path2 = "a",
#else
                .expected = "./b",
                .path1 = "a/b",
                .path2 = "a",
#endif
            },
        },
#ifndef _WIN32
        {
            .desc = aven_str("aven_path_rel_diff superdir w/\'.\' in name"),
            .fn = test_aven_path_rel_diff,
            .args = &(TestAvenPathIntersectArgs){
                .expected = "./.b",
                .path1 = ".a/.b",
                .path2 = ".a",
            },
        },
#endif
        {
            .desc = aven_str("aven_path_rel_intersect same dir relative path"),
            .fn = test_aven_path_rel_intersect,
            .args = &(TestAvenPathIntersectArgs){
#ifdef _WIN32
                .expected = ".\\dir",
                .path1 = "dir",
                .path2 = "dir",
#else
                .expected = "./dir",
                .path1 = "dir",
                .path2 = "dir",
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
                .expected = ".\\dir",
                .path1 = ".\\dir",
                .path2 = "dir",
#else
                .expected = "./dir",
                .path1 = "./dir",
                .path2 = "dir",
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
                .expected = ".\\dir",
                .path1 = "dir",
                .path2 = ".\\dir",
#else
                .expected = "./dir",
                .path1 = "dir",
                .path2 = "./dir",
#endif
            },
        },
        {
            .desc = aven_str("aven_path_rel_intersect neighbor relative path"),
            .fn = test_aven_path_rel_intersect,
            .args = &(TestAvenPathIntersectArgs){
#ifdef _WIN32
                .expected = ".",
                .path1 = "a",
                .path2 = "b",
#else
                .expected = ".",
                .path1 = "a",
                .path2 = "b",
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
                .expected = ".",
                .path1 = "a",
                .path2 = ".\\b",
#else
                .expected = ".",
                .path1 = "a",
                .path2 = "./b",
#endif
            },
        },
        {
            .desc = aven_str("aven_path_rel_intersect subdir relative path"),
            .fn = test_aven_path_rel_intersect,
            .args = &(TestAvenPathIntersectArgs){
#ifdef _WIN32
                .expected = ".\\a",
                .path1 = "a",
                .path2 = "a\\b",
#else
                .expected = "./a",
                .path1 = "a",
                .path2 = "a/b",
#endif
            },
        },
        {
            .desc = aven_str("aven_path_rel_intersect superdir relative path"),
            .fn = test_aven_path_rel_intersect,
            .args = &(TestAvenPathIntersectArgs){
#ifdef _WIN32
                .expected = ".\\a",
                .path1 = "a\\b",
                .path2 = "a",
#else
                .expected = "./a",
                .path1 = "a/b",
                .path2 = "a",
#endif
            },
        },
#ifndef _WIN32
        {
            .desc = aven_str("aven_path_rel_intersect superdir w/\'.\' in name"),
            .fn = test_aven_path_rel_intersect,
            .args = &(TestAvenPathIntersectArgs){
                .expected = "./.a",
                .path1 = ".a/.b",
                .path2 = ".a",
            },
        },
#endif
    };
    AvenTestCaseSlice tcases = slice_array(tcase_data);

    aven_test(tcases, arena);

    return 0;
}

#endif // TEST_PATH_H
