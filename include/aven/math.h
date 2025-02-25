#ifndef AVEN_MATH_H
#define AVEN_MATH_H

#include "../aven.h"

#if ( \
    !defined(AVEN_MATH_USE_STDMATH) and \
    defined(__GNUC__) and \
    __has_builtin(__builtin_sinf) and \
    __has_builtin(__builtin_cosf) and \
    __has_builtin(__builtin_tanf) and \
    __has_builtin(__builtin_asinf) and \
    __has_builtin(__builtin_acosf) and \
    __has_builtin(__builtin_atanf) and \
    __has_builtin(__builtin_atan2f) and \
    __has_builtin(__builtin_sqrtf) and \
    __has_builtin(__builtin_logf) and \
    __has_builtin(__builtin_expf) and \
    __has_builtin(__builtin_powf) and \
    __has_builtin(__builtin_ceilf) and \
    __has_builtin(__builtin_floorf) and \
    __has_builtin(__builtin_fabsf) \
)
    #define AVEN_MATH_USE_BUILTINS

    #define sinf __builtin_sinf
    #define cosf __builtin_cosf
    #define tanf __builtin_tanf
    #define asinf __builtin_asinf
    #define acosf __builtin_acosf
    #define atanf __builtin_atanf
    #define atan2f __builtin_atan2f
    #define sqrtf __builtin_sqrtf
    #define logf __builtin_logf
    #define expf __builtin_expf
    #define powf __builtin_powf
    #define ceilf __builtin_ceilf
    #define floorf __builtin_floorf
    #define fabsf __builtin_fabsf
#else
    #ifndef AVEN_MATH_USE_STDMATH
        #define AVEN_MATH_USE_STDMATH
    #endif

    #include <math.h>
#endif

#define AVEN_MATH_PI_D 3.14159265358979323846264338327950288
#define AVEN_MATH_PI_F 3.14159265358979323846264338327950288f
#define AVEN_MATH_SQRT2_D 1.41421356237309504880168872420969807
#define AVEN_MATH_SQRT2_F 1.41421356237309504880168872420969807f
#define AVEN_MATH_SQRT3_D 1.73205080756887729352744634150587236
#define AVEN_MATH_SQRT3_F 1.73205080756887729352744634150587236f

#if ( \
    !defined(AVEN_MATH_NO_SIMD) and \
    defined(__GNUC__) and \
    __has_attribute(vector_size) and \
    __has_attribute(aligned) and \
    __has_builtin(__builtin_shuffle) \
)
    #define AVEN_MATH_SIMD

    typedef float Vec2SIMD __attribute__((vector_size(8)));
    typedef float Vec4SIMD __attribute__((vector_size(16)));

    typedef int32_t Vec2SIMDMask __attribute__((vector_size(8)));
    typedef int32_t Vec4SIMDMask __attribute__((vector_size(16)));

    typedef float Vec2[2] __attribute__((aligned(8)));
    typedef float Vec4[4] __attribute__((aligned(16)));
    typedef Vec2 Mat2[2] __attribute__((aligned(16)));
    typedef Vec2 Aff2[4] __attribute__((aligned(16)));
#else
    #ifndef AVEN_MATH_NO_SIMD
        #define AVEN_MATH_NO_SIMD
    #endif

    typedef float Vec2[2];
    typedef float Vec4[4];
    typedef Vec2 Mat2[2];
    typedef Vec2 Aff2[3];
#endif

typedef float Vec3[3];
typedef Vec3 Mat3[3];
typedef Vec4 Mat4[4];

static inline void vec2_copy(Vec2 dst, Vec2 a) {
#ifdef AVEN_MATH_SIMD
    *(Vec2SIMD *)dst = *(Vec2SIMD *)a;
#else
    dst[0] = a[0];
    dst[1] = a[1];
#endif // AVEN_MATH_SIMD
}

static inline void vec2_scale(Vec2 dst, float s, Vec2 a) {
#ifdef AVEN_MATH_SIMD
    *(Vec2SIMD *)dst = *(Vec2SIMD *)a * s;
#else
    dst[0] = a[0] * s;
    dst[1] = a[1] * s;
#endif // AVEN_MATH_SIMD
}

static inline void vec2_add(Vec2 dst, Vec2 a, Vec2 b) {
#ifdef AVEN_MATH_SIMD
    *(Vec2SIMD *)dst = *(Vec2SIMD *)a + *(Vec2SIMD *)b;
#else
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
#endif // AVEN_MATH_SIMD
}

static inline void vec2_sub(Vec2 dst, Vec2 a, Vec2 b) {
#ifdef AVEN_MATH_SIMD
    *(Vec2SIMD *)dst = *(Vec2SIMD *)a - *(Vec2SIMD *)b;
#else
    dst[0] = a[0] - b[0];
    dst[1] = a[1] - b[1];
#endif // AVEN_MATH_SIMD
}

static inline void vec2_mul(Vec2 dst, Vec2 a, Vec2 b) {
#ifdef AVEN_MATH_SIMD
    *(Vec2SIMD *)dst = (*(Vec2SIMD *)a) * (*(Vec2SIMD *)b);
#else
    dst[0] = a[0] * b[0];
    dst[1] = a[1] * b[1];
#endif // AVEN_MATH_SIMD
}

static inline float vec2_dot(Vec2 a, Vec2 b) {
    Vec2 ab;
    vec2_mul(ab, a, b);
    return ab[0] + ab[1];
}

static inline float vec2_mag(Vec2 a) {
    return sqrtf(vec2_dot(a, a));
}

static inline float vec2_dist(Vec2 a, Vec2 b) {
    Vec2 ab;
    vec2_sub(ab, b, a);
    return vec2_mag(ab);
}

static inline void vec2_lerp(Vec2 dest, Vec2 a, Vec2 b, float r) {
    vec2_scale(dest, (1.0f - r), a);
    Vec2 bscale;
    vec2_scale(bscale, r, b);
    vec2_add(dest, dest, bscale);
}

static inline float vec2_angle(Vec2 a, Vec2 b) {
    Vec2 an;
    vec2_scale(an, 1.0f / vec2_mag(a), a);

    Vec2 bn;
    vec2_scale(bn, 1.0f / vec2_mag(b), b);

    return acosf(vec2_dot(an, bn));
}

static inline float vec2_angle_xaxis(Vec2 a) {
    float angle = vec2_angle(a, (Vec2){ 1.0f, 0.0f });
    if (a[1] < 0.0f) {
        angle = 2.0f * AVEN_MATH_PI_F - angle;
    }
    return angle;
}

static inline void vec2_midpoint(Vec2 dst, Vec2 a, Vec2 b) {
    vec2_add(dst, a, b);
    vec2_scale(dst, 0.5f, dst);
}

static inline float vec2_triangle_area(Vec2 p1, Vec2 p2, Vec2 p3) {
    return 0.5f * fabsf(
        p1[0] * (p2[1] - p3[1]) +
        p2[0] * (p3[1] - p1[1]) +
        p3[0] * (p1[1] - p2[1])
    );
}

static inline bool vec2_point_in_rect(
    Vec2 p1,
    Vec2 p2,
    Vec2 point
) {
    return (point[0] >= p1[0] and point[0] <= p2[0]) and
        (point[1] >= p1[1] and point[1] <= p2[1]);
}

static inline bool vec2_rect_intersect(
    Vec2 pos1,
    Vec2 dim1,
    Vec2 pos2,
    Vec2 dim2
) {
    Vec2 pdiff;
    vec2_sub(pdiff, pos1, pos2);

    Vec2 pos1_opp;
    vec2_add(pos1_opp, pos1, dim1);

    Vec2 pos2_opp;
    vec2_add(pos2_opp, pos2, dim2);

    bool x_overlap;
    if (pdiff[0] < 0) {
        x_overlap = pos1_opp[0] >= pos2[0];
    } else {
        x_overlap = pos2_opp[0] >= pos1[0];
    }

    bool y_overlap;
    if (pdiff[1] < 0) {
        y_overlap = pos1_opp[1] >= pos2[1];
    } else {
        y_overlap = pos2_opp[1] >= pos1[1];
    }

    return x_overlap and y_overlap;
}

static inline void mat2_copy(Mat2 dst, Mat2 m) {
#ifdef AVEN_MATH_SIMD
    *(Vec4SIMD *)dst = *(Vec4SIMD *)m;
#else
    dst[0][0] = m[0][0];
    dst[0][1] = m[0][1];
    dst[1][0] = m[1][0];
    dst[1][1] = m[1][1];
#endif // AVEN_MATH_SIMD
}

static inline void mat2_identity(Mat2 m) {
    Mat2 ident = {
        { 1.0f, 0.0f },
        { 0.0f, 1.0f },
    };
    mat2_copy(m, ident);
}

static inline void mat2_scale(Mat2 dst, float s, Mat2 m) {
#ifdef AVEN_MATH_SIMD
    *(Vec4SIMD *)dst = *(Vec4SIMD *)m * s;
#else
    dst[0][0] = m[0][0] * s;
    dst[0][1] = m[0][1] * s;
    dst[1][0] = m[1][0] * s;
    dst[1][1] = m[1][1] * s;
#endif // AVEN_MATH_SIMD
}

static inline void mat2_add(Mat2 dst, Mat2 m, Mat2 n) {
#ifdef AVEN_MATH_SIMD
    *(Vec4SIMD *)dst = *(Vec4SIMD *)m + *(Vec4SIMD *)n;
#else
    dst[0][0] = m[0][0] + n[0][0];
    dst[0][1] = m[0][1] + n[0][1];
    dst[1][0] = m[1][0] + n[1][0];
    dst[1][1] = m[1][1] + n[1][1];
#endif // AVEN_MATH_SIMD
}

static inline void mat2_mul_vec2(Vec2 dst, Mat2 m, Vec2 a) {
#ifdef AVEN_MATH_SIMD
    Vec4SIMD vm = *(Vec4SIMD *)m;
    Vec4SIMD va = { a[0], a[0], a[1], a[1] };
    Vec4SIMD vma = vm * va;
    Vec2SIMD vma_low = { vma[0], vma[1] };
    Vec2SIMD vma_high = { vma[2], vma[3] };
    *(Vec2SIMD *)dst = vma_low + vma_high;
#else
    Vec2 ta;
    vec2_copy(ta, a);
    dst[0] = m[0][0] * ta[0] + m[1][0] * ta[1];
    dst[1] = m[0][1] * ta[0] + m[1][1] * ta[1];
#endif // AVEN_MATH_SIMD
}

static inline void mat2_mul_mat2(Mat2 dst, Mat2 m, Mat2 n) {
#ifdef AVEN_MATH_SIMD
    Vec4SIMD vm = *(Vec4SIMD *)m;
    Vec4SIMD vn = *(Vec4SIMD *)n;

    Vec4SIMD vm0_ = __builtin_shuffle(vm, (Vec4SIMDMask){ 0, 1, 0, 1 });
    Vec4SIMD vm1_ = __builtin_shuffle(vm, (Vec4SIMDMask){ 2, 3, 2, 3 });
    Vec4SIMD vn_0 = __builtin_shuffle(vn, (Vec4SIMDMask){ 0, 0, 2, 2 });
    Vec4SIMD vn_1 = __builtin_shuffle(vn, (Vec4SIMDMask){ 1, 1, 3, 3 });
    *(Vec4SIMD *)dst = vm0_ * vn_0 + vm1_ * vn_1;
#else
    Mat2 tn;
    mat2_copy(tn, n);
    Mat2 tm;
    mat2_copy(tm, m);
    dst[0][0] = tm[0][0] * tn[0][0] + tm[1][0] * tn[0][1];
    dst[0][1] = tm[0][1] * tn[0][0] + tm[1][1] * tn[0][1];
    dst[1][0] = tm[0][0] * tn[1][0] + tm[1][0] * tn[1][1];
    dst[1][1] = tm[0][1] * tn[1][0] + tm[1][1] * tn[1][1];
#endif // AVEN_MATH_SIMD
}

static inline void mat2_rotate(Mat2 dst, Mat2 m, float theta) {
    float sin_theta = sinf(theta);
    float cos_theta = cosf(theta);
    Mat2 rot = {
        {  cos_theta, sin_theta },
        { -sin_theta, cos_theta },
    };
    mat2_mul_mat2(dst, m, rot);
}

static inline void mat2_stretch(
    Mat2 dst,
    Vec2 dim,
    Mat2 m
) {
    Mat2 s = {
        { dim[0],   0.0f },
        {   0.0f, dim[1] },
    };
    mat2_mul_mat2(dst, s, m);
}

static inline void aff2_copy(Aff2 dst, Aff2 t) {
    mat2_copy(dst, t);
    vec2_copy(dst[2], t[2]);
}

static inline void aff2_identity(Aff2 t) {
    mat2_identity(t);
    Vec2 zero = { 0.0f, 0.0f };
    vec2_copy(t[2], zero);
}

static inline void aff2_scale(Aff2 dst, float s, Aff2 t) {
    mat2_scale(dst, s, t);
    vec2_scale(dst[2], s, t[2]);
}

static inline void aff2_add_vec2(Aff2 dst, Aff2 t, Vec2 v) {
    mat2_copy(dst, t);
    vec2_add(dst[2], t[2], v);
}

static inline void aff2_sub_vec2(Aff2 dst, Aff2 t, Vec2 v) {
    mat2_copy(dst, t);
    vec2_sub(dst[2], t[2], v);
}

static inline void mat2_mul_aff2(Aff2 dst, Mat2 m, Aff2 t) {
    mat2_mul_mat2(dst, m, t);
    mat2_mul_vec2(dst[2], m, t[2]);
}

static inline void aff2_compose(Aff2 dst, Aff2 t, Aff2 w) {
    mat2_mul_aff2(dst, t, w);
    aff2_add_vec2(dst, dst, t[2]);
}

static inline void aff2_transform(Vec2 dst, Aff2 t, Vec2 v) {
    mat2_mul_vec2(dst, t, v);
    vec2_add(dst, t[2], dst);
}

static inline void aff2_stretch(Aff2 dst, Vec2 dim, Aff2 t) {
    Mat2 s;
    mat2_identity(s);
    mat2_stretch(s, dim, s);
    mat2_mul_aff2(dst, s, t);
}

static inline void aff2_rotate(Aff2 dst, Aff2 t, float theta) {
    Mat2 r;
    mat2_identity(r);
    mat2_rotate(r, r, theta);
    mat2_mul_aff2(dst, r, t);
}

static inline void aff2_position(
    Aff2 dst,
    Vec2 pos,
    Vec2 dim,
    float theta
) {
    aff2_identity(dst);
    aff2_stretch(dst, dim, dst);
    aff2_rotate(dst, dst, theta);
    aff2_add_vec2(dst, dst, pos);
}

static inline void aff2_camera_position(
    Aff2 dst,
    Vec2 pos,
    Vec2 dim,
    float theta
) {
    aff2_identity(dst);
    aff2_sub_vec2(dst, dst, pos);
    aff2_rotate(dst, dst, -theta);
    aff2_stretch(dst, (Vec2){ 1.0f / dim[0], 1.0f / dim[1] }, dst);
}

static inline void aff2_lerp(
    Aff2 dst,
    Aff2 t,
    Aff2 w,
    float r
) {
    Aff2 rm;
    aff2_scale(rm, r, t);
    aff2_scale(dst, 1.0f - r, w);
    mat2_add(dst, rm, dst);
    vec2_add(dst[2], rm[2], dst[2]);
}

static inline void vec3_copy(Vec3 dst, Vec3 a) {
    dst[0] = a[0];
    dst[1] = a[1];
    dst[2] = a[2];
}

static inline float vec3_dot(Vec3 a, Vec3 b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static inline void vec3_scale(Vec3 dst, float s, Vec3 a) {
    dst[0] = a[0] * s;
    dst[1] = a[1] * s;
    dst[2] = a[2] * s;
}

static inline void vec3_add(Vec3 dst, Vec3 a, Vec3 b) {
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
}

static inline void vec3_sub(Vec3 dst, Vec3 a, Vec3 b) {
    dst[0] = a[0] - b[0];
    dst[1] = a[1] - b[1];
    dst[2] = a[2] - b[2];
}

static inline void vec3_mul(Vec3 dst, Vec3 a, Vec3 b) {
    dst[0] = a[0] * b[0];
    dst[1] = a[1] * b[1];
    dst[2] = a[2] * b[2];
}

static inline void mat3_copy(Mat3 dst, Mat3 m) {
    dst[0][0] = m[0][0];
    dst[0][1] = m[0][1];
    dst[0][2] = m[0][2];
    dst[1][0] = m[1][0];
    dst[1][1] = m[1][1];
    dst[1][2] = m[1][2];
    dst[2][0] = m[2][0];
    dst[2][1] = m[2][1];
    dst[2][2] = m[2][2];
}

static inline void mat3_mul_vec3(Vec3 dst, Mat3 m, Vec3 a) {
    dst[0] = m[0][0] * a[0] + m[1][0] * a[1] + m[2][0] * a[2];
    dst[1] = m[0][1] * a[0] + m[1][1] * a[1] + m[2][1] * a[2];
    dst[2] = m[0][2] * a[0] + m[1][2] * a[1] + m[2][2] * a[2];
}

static inline void mat3_mul_mat3(Mat3 dst, Mat3 m, Mat3 n) {
    Mat3 tn;
    mat3_copy(tn, n);
    Mat3 tm;
    mat3_copy(tm, m);
    dst[0][0] = tm[0][0] * tn[0][0] + tm[1][0] * tn[0][1] + tm[2][0] * tn[0][2];
    dst[0][1] = tm[0][1] * tn[0][0] + tm[1][1] * tn[0][1] + tm[2][1] * tn[0][2];
    dst[0][2] = tm[0][2] * tn[0][0] + tm[1][2] * tn[0][1] + tm[2][2] * tn[0][2];

    dst[1][0] = tm[0][0] * tn[1][0] + tm[1][0] * tn[1][1] + tm[2][0] * tn[1][2];
    dst[1][1] = tm[0][1] * tn[1][0] + tm[1][1] * tn[1][1] + tm[2][1] * tn[1][2];
    dst[1][2] = tm[0][2] * tn[1][0] + tm[1][2] * tn[1][1] + tm[2][2] * tn[1][2];

    dst[2][0] = tm[0][0] * tn[2][0] + tm[1][0] * tn[2][1] + tm[2][0] * tn[2][2];
    dst[2][1] = tm[0][1] * tn[2][0] + tm[1][1] * tn[2][1] + tm[2][1] * tn[2][2];
    dst[2][2] = tm[0][2] * tn[2][0] + tm[1][2] * tn[2][1] + tm[2][2] * tn[2][2];
}

static inline void vec4_copy(Vec4 dst, Vec4 a) {
#ifdef AVEN_MATH_SIMD
    *(Vec4SIMD *)dst = *(Vec4SIMD *)a;
#else
    dst[0] = a[0];
    dst[1] = a[1];
    dst[2] = a[2];
    dst[3] = a[3];
#endif // AVEN_MATH_SIMD
}

static inline float vec4_dot(Vec4 a, Vec4 b) {
#ifdef AVEN_MATH_SIMD
    Vec4SIMD ab = *(Vec4SIMD *)a * *(Vec4SIMD *)b;
    return ab[0] + ab[1] + ab[2] + ab[3];
#else
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
#endif // AVEN_MATH_SIMD
}

static inline void vec4_scale(Vec4 dst, float s, Vec4 a) {
#ifdef AVEN_MATH_SIMD
    *(Vec4SIMD *)dst = *(Vec4SIMD *)a * s;
#else
    dst[0] = a[0] * s;
    dst[1] = a[1] * s;
    dst[2] = a[2] * s;
    dst[3] = a[3] * s;
#endif // AVEN_MATH_SIMD
}

static inline void vec4_add(Vec4 dst, Vec4 a, Vec4 b) {
#ifdef AVEN_MATH_SIMD
    *(Vec4SIMD *)dst = *(Vec4SIMD *)a + *(Vec4SIMD *)b;
#else
    dst[0] = a[0] + b[0];
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
    dst[3] = a[3] + b[3];
#endif // AVEN_MATH_SIMD
}

static inline void vec4_sub(Vec4 dst, Vec4 a, Vec4 b) {
#ifdef AVEN_MATH_SIMD
    *(Vec4SIMD *)dst = *(Vec4SIMD *)a - *(Vec4SIMD *)b;
#else
    dst[0] = a[0] - b[0];
    dst[1] = a[1] - b[1];
    dst[2] = a[2] - b[2];
    dst[3] = a[3] - b[3];
#endif // AVEN_MATH_SIMD
}

static inline void vec4_mul(Vec4 dst, Vec4 a, Vec4 b) {
#ifdef AVEN_MATH_SIMD
    *(Vec4SIMD *)dst = (*(Vec4SIMD *)a) * (*(Vec4SIMD *)b);
#else
    dst[0] = a[0] * b[0];
    dst[1] = a[1] * b[1];
    dst[2] = a[2] * b[2];
    dst[3] = a[3] * b[3];
#endif // AVEN_MATH_SIMD
}

static inline void mat4_copy(Mat4 dst, Mat4 m) {
#ifdef AVEN_MATH_SIMD
    *(Vec4SIMD *)dst[0] = *(Vec4SIMD *)m[0];
    *(Vec4SIMD *)dst[1] = *(Vec4SIMD *)m[1];
    *(Vec4SIMD *)dst[2] = *(Vec4SIMD *)m[2];
    *(Vec4SIMD *)dst[3] = *(Vec4SIMD *)m[3];
#else
    dst[0][0] = m[0][0];
    dst[0][1] = m[0][1];
    dst[0][2] = m[0][2];
    dst[0][3] = m[0][3];
    dst[1][0] = m[1][0];
    dst[1][1] = m[1][1];
    dst[1][2] = m[1][2];
    dst[1][3] = m[1][3];
    dst[2][0] = m[2][0];
    dst[2][1] = m[2][1];
    dst[2][2] = m[2][2];
    dst[2][3] = m[2][3];
    dst[3][0] = m[3][0];
    dst[3][1] = m[3][1];
    dst[3][2] = m[3][2];
    dst[3][3] = m[3][3];
#endif // AVEN_MATH_SIMD
}

static inline void mat4_identity(Mat4 m) {
    Mat4 ident = {
        { 1.0f, 0.0f, 0.0f, 0.0f, },
        { 0.0f, 1.0f, 0.0f, 0.0f, },
        { 0.0f, 0.0f, 1.0f, 0.0f, },
        { 0.0f, 0.0f, 0.0f, 1.0f, },
    };
    mat4_copy(m, ident);
}

static inline void mat4_mul_vec4(Vec4 dst, Mat4 m, Vec4 a) {
#ifdef AVEN_MATH_SIMD
    Vec4SIMD vm0 = *(Vec4SIMD *)m[0];
    Vec4SIMD vm1 = *(Vec4SIMD *)m[1];
    Vec4SIMD vm2 = *(Vec4SIMD *)m[2];
    Vec4SIMD vm3 = *(Vec4SIMD *)m[3];
    Vec4SIMD va = *(Vec4SIMD *)a;

    Vec4SIMD va0 = __builtin_shuffle(va, (Vec4SIMDMask){ 0, 0, 0, 0 });
    Vec4SIMD va1 = __builtin_shuffle(va, (Vec4SIMDMask){ 1, 1, 1, 1 });
    Vec4SIMD va2 = __builtin_shuffle(va, (Vec4SIMDMask){ 2, 2, 2, 2 });
    Vec4SIMD va3 = __builtin_shuffle(va, (Vec4SIMDMask){ 3, 3, 3, 3 });
    *(Vec4SIMD *)dst = vm0 * va0 + vm1 * va1 + vm2 * va2 + vm3 * va3;
#else
    Vec4 ta;
    vec4_copy(ta, a);
    dst[0] = m[0][0] * ta[0] + m[1][0] * ta[1] + m[2][0] * ta[2] +
        m[3][0] * ta[3];
    dst[1] = m[0][1] * ta[0] + m[1][1] * ta[1] + m[2][1] * ta[2] +
        m[3][1] * ta[3];
    dst[2] = m[0][2] * ta[0] + m[1][2] * ta[1] + m[2][2] * ta[2] +
        m[3][2] * ta[3];
    dst[3] = m[0][3] * ta[0] + m[1][3] * ta[1] + m[2][3] * ta[2] +
        m[3][3] * ta[3];
#endif // AVEN_MATH_SIMD
}

static inline void mat4_mul_mat4(Mat4 dst, Mat4 m, Mat4 n) {
#ifdef AVEN_MATH_SIMD
    Vec4SIMD vm0 = *(Vec4SIMD *)m[0];
    Vec4SIMD vm1 = *(Vec4SIMD *)m[1];
    Vec4SIMD vm2 = *(Vec4SIMD *)m[2];
    Vec4SIMD vm3 = *(Vec4SIMD *)m[3];

    Vec4SIMD vn0 = *(Vec4SIMD *)n[0];
    Vec4SIMD vn1 = *(Vec4SIMD *)n[1];
    Vec4SIMD vn2 = *(Vec4SIMD *)n[2];
    Vec4SIMD vn3 = *(Vec4SIMD *)n[3];

    Vec4SIMD vn00 = __builtin_shuffle(vn0, (Vec4SIMDMask){ 0, 0, 0, 0 });
    Vec4SIMD vn01 = __builtin_shuffle(vn0, (Vec4SIMDMask){ 1, 1, 1, 1 });
    Vec4SIMD vn02 = __builtin_shuffle(vn0, (Vec4SIMDMask){ 2, 2, 2, 2 });
    Vec4SIMD vn03 = __builtin_shuffle(vn0, (Vec4SIMDMask){ 3, 3, 3, 3 });

    Vec4SIMD vn10 = __builtin_shuffle(vn1, (Vec4SIMDMask){ 0, 0, 0, 0 });
    Vec4SIMD vn11 = __builtin_shuffle(vn1, (Vec4SIMDMask){ 1, 1, 1, 1 });
    Vec4SIMD vn12 = __builtin_shuffle(vn1, (Vec4SIMDMask){ 2, 2, 2, 2 });
    Vec4SIMD vn13 = __builtin_shuffle(vn1, (Vec4SIMDMask){ 3, 3, 3, 3 });

    Vec4SIMD vn20 = __builtin_shuffle(vn2, (Vec4SIMDMask){ 0, 0, 0, 0 });
    Vec4SIMD vn21 = __builtin_shuffle(vn2, (Vec4SIMDMask){ 1, 1, 1, 1 });
    Vec4SIMD vn22 = __builtin_shuffle(vn2, (Vec4SIMDMask){ 2, 2, 2, 2 });
    Vec4SIMD vn23 = __builtin_shuffle(vn2, (Vec4SIMDMask){ 3, 3, 3, 3 });

    Vec4SIMD vn30 = __builtin_shuffle(vn3, (Vec4SIMDMask){ 0, 0, 0, 0 });
    Vec4SIMD vn31 = __builtin_shuffle(vn3, (Vec4SIMDMask){ 1, 1, 1, 1 });
    Vec4SIMD vn32 = __builtin_shuffle(vn3, (Vec4SIMDMask){ 2, 2, 2, 2 });
    Vec4SIMD vn33 = __builtin_shuffle(vn3, (Vec4SIMDMask){ 3, 3, 3, 3 });

    *(Vec4SIMD *)dst[0] = vm0 * vn00 + vm1 * vn01 + vm2 * vn02 + vm3 * vn03;
    *(Vec4SIMD *)dst[1] = vm0 * vn10 + vm1 * vn11 + vm2 * vn12 + vm3 * vn13;
    *(Vec4SIMD *)dst[2] = vm0 * vn20 + vm1 * vn21 + vm2 * vn22 + vm3 * vn23;
    *(Vec4SIMD *)dst[3] = vm0 * vn30 + vm1 * vn31 + vm2 * vn32 + vm3 * vn33;
#else
    Mat4 tn;
    mat4_copy(tn, n);
    Mat4 tm;
    mat4_copy(tm, m);
    dst[0][0] = tm[0][0] * tn[0][0] + tm[1][0] * tn[0][1] + tm[2][0] * tn[0][2]
        + tm[3][0] * tn[0][3];
    dst[0][1] = tm[0][1] * tn[0][0] + tm[1][1] * tn[0][1] + tm[2][1] * tn[0][2]
        + tm[3][1] * tn[0][3];
    dst[0][2] = tm[0][2] * tn[0][0] + tm[1][2] * tn[0][1] + tm[2][2] * tn[0][2]
        + tm[3][2] * tn[0][3];
    dst[0][3] = tm[0][3] * tn[0][0] + tm[1][3] * tn[0][1] + tm[2][3] * tn[0][2]
        + tm[3][3] * tn[0][3];

    dst[1][0] = tm[0][0] * tn[1][0] + tm[1][0] * tn[1][1] + tm[2][0] * tn[1][2]
        + tm[3][0] * tn[1][3];
    dst[1][1] = tm[0][1] * tn[1][0] + tm[1][1] * tn[1][1] + tm[2][1] * tn[1][2]
        + tm[3][1] * tn[1][3];
    dst[1][2] = tm[0][2] * tn[1][0] + tm[1][2] * tn[1][1] + tm[2][2] * tn[1][2]
        + tm[3][2] * tn[1][3];
    dst[1][3] = tm[0][3] * tn[1][0] + tm[1][3] * tn[1][1] + tm[2][3] * tn[1][2]
        + tm[3][3] * tn[1][3];

    dst[2][0] = tm[0][0] * tn[2][0] + tm[1][0] * tn[2][1] + tm[2][0] * tn[2][2]
        + tm[3][0] * tn[2][3];
    dst[2][1] = tm[0][1] * tn[2][0] + tm[1][1] * tn[2][1] + tm[2][1] * tn[2][2]
        + tm[3][1] * tn[2][3];
    dst[2][2] = tm[0][2] * tn[2][0] + tm[1][2] * tn[2][1] + tm[2][2] * tn[2][2]
        + tm[3][2] * tn[2][3];
    dst[2][3] = tm[0][3] * tn[2][0] + tm[1][3] * tn[2][1] + tm[2][3] * tn[2][2]
        + tm[3][3] * tn[2][3];

    dst[3][0] = tm[0][0] * tn[3][0] + tm[1][0] * tn[3][1] + tm[2][0] * tn[3][2]
        + tm[3][0] * tn[3][3];
    dst[3][1] = tm[0][1] * tn[3][0] + tm[1][1] * tn[3][1] + tm[2][1] * tn[3][2]
        + tm[3][1] * tn[3][3];
    dst[3][2] = tm[0][2] * tn[3][0] + tm[1][2] * tn[3][1] + tm[2][2] * tn[3][2]
        + tm[3][2] * tn[3][3];
    dst[3][3] = tm[0][3] * tn[3][0] + tm[1][3] * tn[3][1] + tm[2][3] * tn[3][2]
        + tm[3][3] * tn[3][3];
#endif // AVEN_MATH_SIMD
}

static inline void mat4_rotate_z(Mat4 dst, Mat4 m, float theta) {
    float sin_theta = sinf(theta);
    float cos_theta = cosf(theta);
    Mat4 rot = {
        {  cos_theta, sin_theta, 0.f, 0.f},
        { -sin_theta, cos_theta, 0.f, 0.f},
        {        0.f,       0.f, 1.f, 0.f},
        {        0.f,       0.f, 0.f, 1.f}
    };
    mat4_mul_mat4(dst, m, rot);
}

static inline void mat4_ortho(
    Mat4 dst,
    float left,
    float right,
    float bot,
    float top,
    float near,
    float far
) {
    Mat4 m = {
        { 2.0f / (right - left), 0.0f, 0.0f, 0.0f },
        { 0.0f, 2.0f / (top - bot), 0.0f, 0.0f },
        { 0.0f, 0.0f, 2.0f / (far - near), 0.0f },
        {
            -(right + left) / (right - left),
            -(top + bot) / (top - bot),
            -(far + near) / (far - near),
            1.0f,
        },
    };
    mat4_copy(dst, m);
}

#endif // AVEN_MATH_H
