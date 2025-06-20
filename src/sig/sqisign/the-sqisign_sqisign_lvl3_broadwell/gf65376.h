#ifndef gf65376_h__
#define gf65376_h__

#ifdef __cplusplus
extern "C"
{
#endif
#include <sqisign_namespace.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

    typedef uint64_t digit_t; // Datatype for representing field elements

    /*
     * A gf65376 instance represents an integer modulo q.
     * This is a structure; it can be copied with a simple assignment, and
     * passed around as a value (though exchanging pointers is possibly more
     * efficient).
     * The contents are opaque. No calling code should make any assumption
     * about the contents.
     */
    typedef union
    {
        // Contents are opaque.
        // Implementation note: this encodes the value in Montgomery
        // representation, with R = 2^384. Only partial reduction is
        // done internally to ensure the value is below 2^383
        struct
        {
            uint64_t v0;
            uint64_t v1;
            uint64_t v2;
            uint64_t v3;
            uint64_t v4;
            uint64_t v5;
        };
        digit_t arr[6];
    } gf65376;

    /*
     * Constant zero (in the field).
     */
    extern const gf65376 ZERO;

    /*
     * Constant one (in the field).
     */
    extern const gf65376 ONE;

    /*
     * Constant -1 (in the field).
     */
    extern const gf65376 gf65376_MINUS_ONE;

    /*
     * API RULES:
     * ==========
     *
     * Elementary operations on field elements are implemented by functions
     * which take as parameter pointers to the operands. The first parameter
     * is the pointer to the destination. Thus:
     *    gf65376 a = ...;
     *    gf65376 b = ...;
     *    gf65376 d;
     *    gf65376_sub(&d, &a, &b)
     * sets field element d to a - b (implicitly modulo q).
     *
     * Operands may be used several times: it is always valid to use as
     * output a gf65376 structure which is also used as input.
     *
     * Boolean values are represented by 32-bit integer (uint32_t) which have
     * value exactly 0xFFFFFFFF (for "true") or 0x00000000 (for "false"). This
     * convention minimizes the risk that a "smart" compiler breaks the
     * constant-time property of the code through unfortunated optimizations.
     * When a function expects such a Boolean, the caller MUST take care never
     * to provide any value other than 0x00000000 or 0xFFFFFFFF.
     *
     * Values are encoded into exactly 48 bytes: value x modulo q is mapped to
     * its unique integer representant in the [0..q-1] range, which is then
     * encoded over 48 bytes with little-endian convention. Encoding is canonical
     * and checked: when decoding (with gf65376_decode()), the input value is
     * verified to be in the [0..q-1] range; for an out-of-range value,
     * gf65376_decode() fills the output structure with zero, and returns
     * 0x00000000.
     *
     * For most operations, the implementation is an inline function, defined
     * below; the compiler can thus efficiently include it in the calling code.
     * A few expensive operations (e.g. divisions) use non-inline functions,
     * declared below but defined in gf65376.c
     *
     * All functions and macro whose name starts with "inner_gf65376_" are
     * internal to this implementation and visible here only in order to
     * support the API inline functions; they MUST NOT be used directly.
     */

#if (defined _MSC_VER && defined _M_X64) || (defined __x86_64__ && (defined __GNUC__ || defined __clang__))
#include <immintrin.h>
#define inner_gf65376_adc(cc, a, b, d) _addcarry_u64(cc, a, b, (unsigned long long *)(void *)d)
#define inner_gf65376_sbb(cc, a, b, d) _subborrow_u64(cc, a, b, (unsigned long long *)(void *)d)
#else
static inline unsigned char
inner_gf65376_adc(unsigned char cc, uint64_t a, uint64_t b, uint64_t *d)
{
    unsigned __int128 t = (unsigned __int128)a + (unsigned __int128)b + cc;
    *d = (uint64_t)t;
    return (unsigned char)(t >> 64);
}
static inline unsigned char
inner_gf65376_sbb(unsigned char cc, uint64_t a, uint64_t b, uint64_t *d)
{
    unsigned __int128 t = (unsigned __int128)a - (unsigned __int128)b - cc;
    *d = (uint64_t)t;
    return (unsigned char)(-(uint64_t)(t >> 64));
}
#endif

#if defined _MSC_VER
#define inner_gf65376_umul(lo, hi, x, y)                                                                               \
    do {                                                                                                               \
        uint64_t umul_hi;                                                                                              \
        (lo) = _umul128((x), (y), &umul_hi);                                                                           \
        (hi) = umul_hi;                                                                                                \
    } while (0)
#define inner_gf65376_umul_add(lo, hi, x, y, z)                                                                        \
    do {                                                                                                               \
        uint64_t umul_lo, umul_hi;                                                                                     \
        umul_lo = _umul128((x), (y), &umul_hi);                                                                        \
        unsigned char umul_cc;                                                                                         \
        umul_cc = inner_gf65376_adc(0, umul_lo, (z), &umul_lo);                                                        \
        (void)inner_gf65376_adc(umul_cc, umul_hi, 0, &umul_hi);                                                        \
        (lo) = umul_lo;                                                                                                \
        (hi) = umul_hi;                                                                                                \
    } while (0)
#define inner_gf65376_umul_x2(lo, hi, x1, y1, x2, y2)                                                                  \
    do {                                                                                                               \
        uint64_t umul_lo, umul_hi;                                                                                     \
        umul_lo = _umul128((x1), (y1), &umul_hi);                                                                      \
        uint64_t umul_lo2, umul_hi2;                                                                                   \
        umul_lo2 = _umul128((x2), (y2), &umul_hi2);                                                                    \
        unsigned char umul_cc;                                                                                         \
        umul_cc = inner_gf65376_adc(0, umul_lo, umul_lo2, &umul_lo);                                                   \
        (void)inner_gf65376_adc(umul_cc, umul_hi, umul_hi2, &umul_hi);                                                 \
        (lo) = umul_lo;                                                                                                \
        (hi) = umul_hi;                                                                                                \
    } while (0)
#define inner_gf65376_umul_x2_add(lo, hi, x1, y1, x2, y2, z)                                                           \
    do {                                                                                                               \
        uint64_t umul_lo, umul_hi;                                                                                     \
        umul_lo = _umul128((x1), (y1), &umul_hi);                                                                      \
        uint64_t umul_lo2, umul_hi2;                                                                                   \
        umul_lo2 = _umul128((x2), (y2), &umul_hi2);                                                                    \
        unsigned char umul_cc;                                                                                         \
        umul_cc = inner_gf65376_adc(0, umul_lo, umul_lo2, &umul_lo);                                                   \
        (void)inner_gf65376_adc(umul_cc, umul_hi, umul_hi2, &umul_hi);                                                 \
        umul_cc = inner_gf65376_adc(0, umul_lo, (z), &umul_lo);                                                        \
        (void)inner_gf65376_adc(umul_cc, umul_hi, 0, &umul_hi);                                                        \
        (lo) = umul_lo;                                                                                                \
        (hi) = umul_hi;                                                                                                \
    } while (0)
#else
#define inner_gf65376_umul(lo, hi, x, y)                                                                               \
    do {                                                                                                               \
        unsigned __int128 umul_tmp;                                                                                    \
        umul_tmp = (unsigned __int128)(x) * (unsigned __int128)(y);                                                    \
        (lo) = (uint64_t)umul_tmp;                                                                                     \
        (hi) = (uint64_t)(umul_tmp >> 64);                                                                             \
    } while (0)
#define inner_gf65376_umul_add(lo, hi, x, y, z)                                                                        \
    do {                                                                                                               \
        unsigned __int128 umul_tmp;                                                                                    \
        umul_tmp = (unsigned __int128)(x) * (unsigned __int128)(y) + (unsigned __int128)(uint64_t)(z);                 \
        (lo) = (uint64_t)umul_tmp;                                                                                     \
        (hi) = (uint64_t)(umul_tmp >> 64);                                                                             \
    } while (0)
#define inner_gf65376_umul_x2(lo, hi, x1, y1, x2, y2)                                                                  \
    do {                                                                                                               \
        unsigned __int128 umul_tmp;                                                                                    \
        umul_tmp =                                                                                                     \
            (unsigned __int128)(x1) * (unsigned __int128)(y1) + (unsigned __int128)(x2) * (unsigned __int128)(y2);     \
        (lo) = (uint64_t)umul_tmp;                                                                                     \
        (hi) = (uint64_t)(umul_tmp >> 64);                                                                             \
    } while (0)
#define inner_gf65376_umul_x2_add(lo, hi, x1, y1, x2, y2, z)                                                           \
    do {                                                                                                               \
        unsigned __int128 umul_tmp;                                                                                    \
        umul_tmp = (unsigned __int128)(x1) * (unsigned __int128)(y1) +                                                 \
                   (unsigned __int128)(x2) * (unsigned __int128)(y2) + (unsigned __int128)(uint64_t)(z);               \
        (lo) = (uint64_t)umul_tmp;                                                                                     \
        (hi) = (uint64_t)(umul_tmp >> 64);                                                                             \
    } while (0)
#endif

    /*
     * d <- a + b
     */
    static inline void
    gf65376_add(gf65376 *d, const gf65376 *a, const gf65376 *b)
    {
        uint64_t d0, d1, d2, d3, d4, d5, f;
        unsigned char cc;

        // Raw addition.
        cc = inner_gf65376_adc(0, a->v0, b->v0, &d0);
        cc = inner_gf65376_adc(cc, a->v1, b->v1, &d1);
        cc = inner_gf65376_adc(cc, a->v2, b->v2, &d2);
        cc = inner_gf65376_adc(cc, a->v3, b->v3, &d3);
        cc = inner_gf65376_adc(cc, a->v4, b->v4, &d4);
        (void)inner_gf65376_adc(cc, a->v5, b->v5, &d5);

        // Sum is up to 2^384 - 2. Subtract q if the value is not lower
        // than 2^383 (we subtract q by adding -q).
        // Note: 0xBF = (-65) % 256, 56 = 376 - 5*64
        f = d5 >> 63;
        cc = inner_gf65376_adc(0, d0, f, &d0);
        cc = inner_gf65376_adc(cc, d1, 0, &d1);
        cc = inner_gf65376_adc(cc, d2, 0, &d2);
        cc = inner_gf65376_adc(cc, d3, 0, &d3);
        cc = inner_gf65376_adc(cc, d4, 0, &d4);
        (void)inner_gf65376_adc(cc, d5, ((uint64_t)0xBF << 56) & -f, &d5);

        // One subtraction of q might not be enough.
        f = d5 >> 63;
        cc = inner_gf65376_adc(0, d0, f, &d0);
        cc = inner_gf65376_adc(cc, d1, 0, &d1);
        cc = inner_gf65376_adc(cc, d2, 0, &d2);
        cc = inner_gf65376_adc(cc, d3, 0, &d3);
        cc = inner_gf65376_adc(cc, d4, 0, &d4);
        (void)inner_gf65376_adc(cc, d5, ((uint64_t)0xBF << 56) & -f, &d5);

        d->v0 = d0;
        d->v1 = d1;
        d->v2 = d2;
        d->v3 = d3;
        d->v4 = d4;
        d->v5 = d5;
    }

    /*
     * d <- a - b
     */
    static inline void
    gf65376_sub(gf65376 *d, const gf65376 *a, const gf65376 *b)
    {
        uint64_t d0, d1, d2, d3, d4, d5, m, f;
        unsigned char cc;

        // Raw subtraction.
        cc = inner_gf65376_sbb(0, a->v0, b->v0, &d0);
        cc = inner_gf65376_sbb(cc, a->v1, b->v1, &d1);
        cc = inner_gf65376_sbb(cc, a->v2, b->v2, &d2);
        cc = inner_gf65376_sbb(cc, a->v3, b->v3, &d3);
        cc = inner_gf65376_sbb(cc, a->v4, b->v4, &d4);
        cc = inner_gf65376_sbb(cc, a->v5, b->v5, &d5);

        // Add 2*q if the result is negative.
        // Note: 0x7E = (-2*65) % 256, 56 = 376 - 5*64
        (void)inner_gf65376_sbb(cc, 0, 0, &m);
        cc = inner_gf65376_sbb(0, d0, m & 2, &d0);
        cc = inner_gf65376_sbb(cc, d1, 0, &d1);
        cc = inner_gf65376_sbb(cc, d2, 0, &d2);
        cc = inner_gf65376_sbb(cc, d3, 0, &d3);
        cc = inner_gf65376_sbb(cc, d4, 0, &d4);
        (void)inner_gf65376_sbb(cc, d5, ((uint64_t)0x7E << 56) & m, &d5);

        // We might have overdone it; subtract q if necessary.
        // Note: 0xBF = (-65) % 256, 56 = 376 - 5*64
        f = d5 >> 63;
        cc = inner_gf65376_adc(0, d0, f, &d0);
        cc = inner_gf65376_adc(cc, d1, 0, &d1);
        cc = inner_gf65376_adc(cc, d2, 0, &d2);
        cc = inner_gf65376_adc(cc, d3, 0, &d3);
        cc = inner_gf65376_adc(cc, d4, 0, &d4);
        (void)inner_gf65376_adc(cc, d5, ((uint64_t)0xBF << 56) & -f, &d5);

        d->v0 = d0;
        d->v1 = d1;
        d->v2 = d2;
        d->v3 = d3;
        d->v4 = d4;
        d->v5 = d5;
    }

    /*
     * d <- -a
     */
    static inline void
    gf65376_neg(gf65376 *d, const gf65376 *a)
    {
        uint64_t d0, d1, d2, d3, d4, d5, f;
        unsigned char cc;

        // 2*q - a
        cc = inner_gf65376_sbb(0, (uint64_t)0xFFFFFFFFFFFFFFFE, a->v0, &d0);
        cc = inner_gf65376_sbb(cc, (uint64_t)0xFFFFFFFFFFFFFFFF, a->v1, &d1);
        cc = inner_gf65376_sbb(cc, (uint64_t)0xFFFFFFFFFFFFFFFF, a->v2, &d2);
        cc = inner_gf65376_sbb(cc, (uint64_t)0xFFFFFFFFFFFFFFFF, a->v3, &d3);
        cc = inner_gf65376_sbb(cc, (uint64_t)0xFFFFFFFFFFFFFFFF, a->v4, &d4);
        (void)inner_gf65376_sbb(cc, (uint64_t)0x81FFFFFFFFFFFFFF, a->v5, &d5);

        // Subtract q if the value is not lower than 2^251.
        f = d5 >> 63;
        cc = inner_gf65376_adc(0, d0, f, &d0);
        cc = inner_gf65376_adc(cc, d1, 0, &d1);
        cc = inner_gf65376_adc(cc, d2, 0, &d2);
        cc = inner_gf65376_adc(cc, d3, 0, &d3);
        cc = inner_gf65376_adc(cc, d4, 0, &d4);
        (void)inner_gf65376_adc(cc, d5, ((uint64_t)0xBF << 56) & -f, &d5);

        d->v0 = d0;
        d->v1 = d1;
        d->v2 = d2;
        d->v3 = d3;
        d->v4 = d4;
        d->v5 = d5;
    }

    /*
     * If ctl == 0x00000000, then *a0 is copied into *d.
     * If ctl == 0xFFFFFFFF, then *a1 is copied into *d.
     * ctl MUST be either 0x00000000 or 0xFFFFFFFF.
     */
    static inline void
    gf65376_select(gf65376 *d, const gf65376 *a0, const gf65376 *a1, uint32_t ctl)
    {
        uint64_t cw = (uint64_t)*(int32_t *)&ctl;
        d->v0 = a0->v0 ^ (cw & (a0->v0 ^ a1->v0));
        d->v1 = a0->v1 ^ (cw & (a0->v1 ^ a1->v1));
        d->v2 = a0->v2 ^ (cw & (a0->v2 ^ a1->v2));
        d->v3 = a0->v3 ^ (cw & (a0->v3 ^ a1->v3));
        d->v4 = a0->v4 ^ (cw & (a0->v4 ^ a1->v4));
        d->v5 = a0->v5 ^ (cw & (a0->v5 ^ a1->v5));
    }

    /*
     * If ctl == 0x00000000, then *a and *b are unchanged.
     * If ctl == 0xFFFFFFFF, then the contents of *a and *b are swapped.
     * ctl MUST be either 0x00000000 or 0xFFFFFFFF.
     */
    static inline void
    gf65376_cswap(gf65376 *a, gf65376 *b, uint32_t ctl)
    {
        uint64_t cw = (uint64_t)*(int32_t *)&ctl;
        uint64_t t;
        t = cw & (a->v0 ^ b->v0);
        a->v0 ^= t;
        b->v0 ^= t;
        t = cw & (a->v1 ^ b->v1);
        a->v1 ^= t;
        b->v1 ^= t;
        t = cw & (a->v2 ^ b->v2);
        a->v2 ^= t;
        b->v2 ^= t;
        t = cw & (a->v3 ^ b->v3);
        a->v3 ^= t;
        b->v3 ^= t;
        t = cw & (a->v4 ^ b->v4);
        a->v4 ^= t;
        b->v4 ^= t;
        t = cw & (a->v5 ^ b->v5);
        a->v5 ^= t;
        b->v5 ^= t;
    }

    /*
     * d <- a/2
     */
    static inline void
    gf65376_half(gf65376 *d, const gf65376 *a)
    {
        uint64_t d0, d1, d2, d3, d4, d5;

        d0 = (a->v0 >> 1) | (a->v1 << 63);
        d1 = (a->v1 >> 1) | (a->v2 << 63);
        d2 = (a->v2 >> 1) | (a->v3 << 63);
        d3 = (a->v3 >> 1) | (a->v4 << 63);
        d4 = (a->v4 >> 1) | (a->v5 << 63);
        d5 = a->v5 >> 1;
        d5 += ((uint64_t)65 << 55) & -(a->v0 & 1);
        d->v0 = d0;
        d->v1 = d1;
        d->v2 = d2;
        d->v3 = d3;
        d->v4 = d4;
        d->v5 = d5;
    }

    // Inner function: 384-bit to 383-bit reduction
    static inline void
    inner_gf65376_partial_reduce(gf65376 *d,
                                 uint64_t a0,
                                 uint64_t a1,
                                 uint64_t a2,
                                 uint64_t a3,
                                 uint64_t a4,
                                 uint64_t a5)
    {
        uint64_t d0, d1, d2, d3, d4, d5, h, quo, rem;
        unsigned char cc;

        // Split value in high (8 bits) and low (376 bits) parts.
        h = a5 >> 56;
        a5 &= 0x00FFFFFFFFFFFFFF;

        // 65*2^376 = 1 mod q; hence, we add floor(h/65) + (h mod 65)*2^376
        // to the low part.
        quo = (h * 0xFC1) >> 18;
        rem = h - (65 * quo);
        cc = inner_gf65376_adc(0, a0, quo, &d0);
        cc = inner_gf65376_adc(cc, a1, 0, &d1);
        cc = inner_gf65376_adc(cc, a2, 0, &d2);
        cc = inner_gf65376_adc(cc, a3, 0, &d3);
        cc = inner_gf65376_adc(cc, a4, 0, &d4);
        (void)inner_gf65376_adc(cc, a5, rem << 56, &d5);

        d->v0 = d0;
        d->v1 = d1;
        d->v2 = d2;
        d->v3 = d3;
        d->v4 = d4;
        d->v5 = d5;
    }

    // Inner function: Normalize value *a into *d.
    static inline void
    inner_gf65376_normalize(gf65376 *d, const gf65376 *a)
    {
        uint64_t d0, d1, d2, d3, d4, d5, m;
        unsigned char cc;

        // Subtract q.
        cc = inner_gf65376_sbb(0, a->v0, 0xFFFFFFFFFFFFFFFF, &d0);
        cc = inner_gf65376_sbb(cc, a->v1, 0xFFFFFFFFFFFFFFFF, &d1);
        cc = inner_gf65376_sbb(cc, a->v2, 0xFFFFFFFFFFFFFFFF, &d2);
        cc = inner_gf65376_sbb(cc, a->v3, 0xFFFFFFFFFFFFFFFF, &d3);
        cc = inner_gf65376_sbb(cc, a->v4, 0xFFFFFFFFFFFFFFFF, &d4);
        cc = inner_gf65376_sbb(cc, a->v5, 0x40FFFFFFFFFFFFFF, &d5);

        // Add back q if the result is negative.
        (void)inner_gf65376_sbb(cc, 0, 0, &m);
        cc = inner_gf65376_adc(0, d0, m, &d0);
        cc = inner_gf65376_adc(cc, d1, m, &d1);
        cc = inner_gf65376_adc(cc, d2, m, &d2);
        cc = inner_gf65376_adc(cc, d3, m, &d3);
        cc = inner_gf65376_adc(cc, d4, m, &d4);
        (void)inner_gf65376_adc(cc, d5, m & 0x40FFFFFFFFFFFFFF, &d5);

        d->v0 = d0;
        d->v1 = d1;
        d->v2 = d2;
        d->v3 = d3;
        d->v4 = d4;
        d->v5 = d5;
    }

    /*
     * d <- 2*a
     */
    static inline void
    gf65376_mul2(gf65376 *d, const gf65376 *a)
    {
        gf65376_add(d, a, a);
    }

    /*
     * d <- a*x
     * (multiplication by a 32-bit integer)
     */
    static inline void
    gf65376_mul_small(gf65376 *d, const gf65376 *a, uint32_t x)
    {
        uint64_t d0, d1, d2, d3, d4, d5, d6, lo, hi, b, h, quo, rem;
        unsigned char cc;

        // Product over the integers. Top output word (d6) is at most 31 bits.
        b = (uint64_t)x;
        inner_gf65376_umul(d0, d1, a->v0, b);
        inner_gf65376_umul(d2, d3, a->v2, b);
        inner_gf65376_umul(d4, d5, a->v4, b);

        inner_gf65376_umul(lo, hi, a->v1, b);
        cc = inner_gf65376_adc(0, d1, lo, &d1);
        cc = inner_gf65376_adc(cc, d2, hi, &d2);
        inner_gf65376_umul(lo, hi, a->v3, b);
        cc = inner_gf65376_adc(cc, d3, lo, &d3);
        cc = inner_gf65376_adc(cc, d4, hi, &d4);
        inner_gf65376_umul(lo, d6, a->v5, b);
        cc = inner_gf65376_adc(cc, d5, lo, &d5);
        (void)inner_gf65376_adc(cc, d6, 0, &d6);

        // Extract low 248-bit part, and the high part (at most 35 bits).
        h = (d6 << 8) | (d5 >> 56);
        d5 &= 0x00FFFFFFFFFFFFFF;

        // Fold h by adding floor(h/65) + (h mod 65)*2^376 to the low part.
        inner_gf65376_umul(lo, hi, h, 0xFC0FC0FC0FC0FC1);
        quo = hi >> 2;
        rem = h - (65 * quo);
        cc = inner_gf65376_adc(cc, d0, quo, &d0);
        cc = inner_gf65376_adc(cc, d1, 0, &d1);
        cc = inner_gf65376_adc(cc, d2, 0, &d2);
        cc = inner_gf65376_adc(cc, d3, 0, &d3);
        cc = inner_gf65376_adc(cc, d4, 0, &d4);
        (void)inner_gf65376_adc(cc, d5, rem << 56, &d5);

        d->v0 = d0;
        d->v1 = d1;
        d->v2 = d2;
        d->v3 = d3;
        d->v4 = d4;
        d->v5 = d5;
    }

    /*
     * d <- x
     * Input value x (32-bit integer) is converted to field element x mod q.
     */
    static inline void
    gf65376_set_small(gf65376 *d, uint32_t x)
    {
        // We want Montgomery representation, i.e. x*2^384 mod q.
        // We set h = x*2^8; then:
        //   x*2^384 = h*2^376
        //           = (h mod 65)*2^376 + floor(h/65)*65*2^376
        //           = (h mod 65)*2^376 + floor(h/65)  mod q
        // by using the fact that 65*2^376 = 1 mod q.
        uint64_t h, lo, hi, quo, rem;

        // NOTE: 0xFC0FC0FC0FC0FC1 = 65^(-1) % 2^64
        h = (uint64_t)x << 8;
        inner_gf65376_umul(lo, hi, h, 0xFC0FC0FC0FC0FC1);
        (void)lo;
        quo = hi >> 2;
        rem = h - (65 * quo);

        d->v0 = quo;
        d->v1 = 0;
        d->v2 = 0;
        d->v3 = 0;
        d->v4 = 0;
        d->v5 = rem << 56;
    }

    // Inner function: d <- a/2^384, with normalization to [0..q-1].
    static inline void
    inner_gf65376_montgomery_reduce(gf65376 *d, const gf65376 *a)
    {
        uint64_t x0, x1, x2, x3, x4, x5, f0, f1, f2, f3, f4, f5;
        uint64_t g0, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11;
        uint64_t d0, d1, d2, d3, d4, d5;
        uint64_t hi, t, w;
        unsigned char cc;

        // Let m = -1/q mod 2^384 = 65*2^376 + 1
        // For input x, we compute f = x*m mod 2^384, then
        // h = x + f*q, which is a multiple of 2^384. The output
        // is then h/2^384.
        // Since x < 2^384, we have:
        //    h <= 2^384 - 1 + (2^384 - 1)*q
        //    h <= q*2^384 + 2^384 - q - 1
        // Since h = 0 mod 2^384, this implies that h <= q*2^384.
        // The output h/2^384 is therefore between 0 and q (inclusive).

        x0 = a->v0;
        x1 = a->v1;
        x2 = a->v2;
        x3 = a->v3;
        x4 = a->v4;
        x5 = a->v5;

        // f = x*(-1/q) mod 2^384
        f0 = x0;
        f1 = x1;
        f2 = x2;
        f3 = x3;
        f4 = x4;
        f5 = x5 + ((x0 * 65) << 56);

        // g = f*q
        inner_gf65376_umul(g5, hi, f0, (uint64_t)65 << 56);
        inner_gf65376_umul_add(g6, hi, f1, (uint64_t)65 << 56, hi);
        inner_gf65376_umul_add(g7, hi, f2, (uint64_t)65 << 56, hi);
        inner_gf65376_umul_add(g8, hi, f3, (uint64_t)65 << 56, hi);
        inner_gf65376_umul_add(g9, hi, f4, (uint64_t)65 << 56, hi);
        inner_gf65376_umul_add(g10, g11, f5, (uint64_t)65 << 56, hi);

        cc = inner_gf65376_sbb(0, 0, f0, &g0);
        cc = inner_gf65376_sbb(cc, 0, f1, &g1);
        cc = inner_gf65376_sbb(cc, 0, f2, &g2);
        cc = inner_gf65376_sbb(cc, 0, f3, &g3);
        cc = inner_gf65376_sbb(cc, 0, f4, &g4);
        cc = inner_gf65376_sbb(cc, g5, f5, &g5);
        cc = inner_gf65376_sbb(cc, g6, 0, &g6);
        cc = inner_gf65376_sbb(cc, g7, 0, &g7);
        cc = inner_gf65376_sbb(cc, g8, 0, &g8);
        cc = inner_gf65376_sbb(cc, g9, 0, &g9);
        cc = inner_gf65376_sbb(cc, g10, 0, &g10);
        (void)inner_gf65376_sbb(cc, g11, 0, &g11);

        // h = x + f*q  (we drop the low 384 bits).
        cc = inner_gf65376_adc(0, g0, x0, &x0);
        cc = inner_gf65376_adc(cc, g1, x1, &x1);
        cc = inner_gf65376_adc(cc, g2, x2, &x2);
        cc = inner_gf65376_adc(cc, g3, x3, &x3);
        cc = inner_gf65376_adc(cc, g4, x4, &x4);
        cc = inner_gf65376_adc(cc, g5, x5, &x5);
        cc = inner_gf65376_adc(cc, g6, 0, &d0);
        cc = inner_gf65376_adc(cc, g7, 0, &d1);
        cc = inner_gf65376_adc(cc, g8, 0, &d2);
        cc = inner_gf65376_adc(cc, g9, 0, &d3);
        cc = inner_gf65376_adc(cc, g10, 0, &d4);
        (void)inner_gf65376_adc(cc, g11, 0, &d5);

        // Normalize: if h = q, replace it with zero.
        t = d0 & d1 & d2 & d3 & d4 & (d5 ^ ~(uint64_t)0x40FFFFFFFFFFFFFF);
        cc = inner_gf65376_adc(0, t, 1, &t);
        (void)inner_gf65376_sbb(cc, 0, 0, &w);
        w = ~w;
        d->v0 = d0 & w;
        d->v1 = d1 & w;
        d->v2 = d2 & w;
        d->v3 = d3 & w;
        d->v4 = d4 & w;
        d->v5 = d5 & w;
    }

    /*
     * d <- a*b
     */
    static inline void
    gf65376_mul(gf65376 *d, const gf65376 *a, const gf65376 *b)
    {
        uint64_t e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11;
        uint64_t f0, f1, f2, f3, f4, f5, lo, hi, lo2, hi2;
        uint64_t g0, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11;
        unsigned char cc;

        // Multiplication over integers.
        // 6 mul
        inner_gf65376_umul(e0, e1, a->v0, b->v0);
        inner_gf65376_umul(e2, e3, a->v1, b->v1);
        inner_gf65376_umul(e4, e5, a->v2, b->v2);
        inner_gf65376_umul(e6, e7, a->v3, b->v3);
        inner_gf65376_umul(e8, e9, a->v4, b->v4);
        inner_gf65376_umul(e10, e11, a->v5, b->v5);

        // + 5 mul = 11
        inner_gf65376_umul(lo, hi, a->v0, b->v1);
        cc = inner_gf65376_adc(0, e1, lo, &e1);
        cc = inner_gf65376_adc(cc, e2, hi, &e2);
        inner_gf65376_umul(lo, hi, a->v0, b->v3);
        cc = inner_gf65376_adc(cc, e3, lo, &e3);
        cc = inner_gf65376_adc(cc, e4, hi, &e4);
        inner_gf65376_umul(lo, hi, a->v0, b->v5);
        cc = inner_gf65376_adc(cc, e5, lo, &e5);
        cc = inner_gf65376_adc(cc, e6, hi, &e6);
        inner_gf65376_umul(lo, hi, a->v2, b->v5);
        cc = inner_gf65376_adc(cc, e7, lo, &e7);
        cc = inner_gf65376_adc(cc, e8, hi, &e8);
        inner_gf65376_umul(lo, hi, a->v4, b->v5);
        cc = inner_gf65376_adc(cc, e9, lo, &e9);
        cc = inner_gf65376_adc(cc, e10, hi, &e10);
        cc = inner_gf65376_adc(cc, e11, 0, &e11);
        assert(cc == 0);

        // + 5 mul = 16
        inner_gf65376_umul(lo, hi, a->v1, b->v0);
        cc = inner_gf65376_adc(0, e1, lo, &e1);
        cc = inner_gf65376_adc(cc, e2, hi, &e2);
        inner_gf65376_umul(lo, hi, a->v3, b->v0);
        cc = inner_gf65376_adc(cc, e3, lo, &e3);
        cc = inner_gf65376_adc(cc, e4, hi, &e4);
        inner_gf65376_umul(lo, hi, a->v5, b->v0);
        cc = inner_gf65376_adc(cc, e5, lo, &e5);
        cc = inner_gf65376_adc(cc, e6, hi, &e6);
        inner_gf65376_umul(lo, hi, a->v5, b->v2);
        cc = inner_gf65376_adc(cc, e7, lo, &e7);
        cc = inner_gf65376_adc(cc, e8, hi, &e8);
        inner_gf65376_umul(lo, hi, a->v5, b->v4);
        cc = inner_gf65376_adc(cc, e9, lo, &e9);
        cc = inner_gf65376_adc(cc, e10, hi, &e10);
        cc = inner_gf65376_adc(cc, e11, 0, &e11);
        assert(cc == 0);

        // + 4 mul = 20
        inner_gf65376_umul(lo, hi, a->v0, b->v2);
        cc = inner_gf65376_adc(0, e2, lo, &e2);
        cc = inner_gf65376_adc(cc, e3, hi, &e3);
        inner_gf65376_umul(lo, hi, a->v0, b->v4);
        cc = inner_gf65376_adc(cc, e4, lo, &e4);
        cc = inner_gf65376_adc(cc, e5, hi, &e5);
        inner_gf65376_umul(lo, hi, a->v2, b->v4);
        cc = inner_gf65376_adc(cc, e6, lo, &e6);
        cc = inner_gf65376_adc(cc, e7, hi, &e7);
        inner_gf65376_umul(lo, hi, a->v3, b->v5);
        cc = inner_gf65376_adc(cc, e8, lo, &e8);
        cc = inner_gf65376_adc(cc, e9, hi, &e9);
        cc = inner_gf65376_adc(cc, e10, 0, &e10);
        cc = inner_gf65376_adc(cc, e11, 0, &e11);
        assert(cc == 0);

        // + 4 mul = 24
        inner_gf65376_umul(lo, hi, a->v2, b->v0);
        cc = inner_gf65376_adc(0, e2, lo, &e2);
        cc = inner_gf65376_adc(cc, e3, hi, &e3);
        inner_gf65376_umul(lo, hi, a->v4, b->v0);
        cc = inner_gf65376_adc(cc, e4, lo, &e4);
        cc = inner_gf65376_adc(cc, e5, hi, &e5);
        inner_gf65376_umul(lo, hi, a->v4, b->v2);
        cc = inner_gf65376_adc(cc, e6, lo, &e6);
        cc = inner_gf65376_adc(cc, e7, hi, &e7);
        inner_gf65376_umul(lo, hi, a->v5, b->v3);
        cc = inner_gf65376_adc(cc, e8, lo, &e8);
        cc = inner_gf65376_adc(cc, e9, hi, &e9);
        cc = inner_gf65376_adc(cc, e10, 0, &e10);
        cc = inner_gf65376_adc(cc, e11, 0, &e11);
        assert(cc == 0);

        // + 3 mul = 27
        inner_gf65376_umul(lo, hi, a->v1, b->v2);
        cc = inner_gf65376_adc(cc, e3, lo, &e3);
        cc = inner_gf65376_adc(cc, e4, hi, &e4);
        inner_gf65376_umul(lo, hi, a->v1, b->v4);
        cc = inner_gf65376_adc(cc, e5, lo, &e5);
        cc = inner_gf65376_adc(cc, e6, hi, &e6);
        inner_gf65376_umul(lo, hi, a->v3, b->v4);
        cc = inner_gf65376_adc(cc, e7, lo, &e7);
        cc = inner_gf65376_adc(cc, e8, hi, &e8);
        cc = inner_gf65376_adc(cc, e9, 0, &e9);
        cc = inner_gf65376_adc(cc, e10, 0, &e10);
        cc = inner_gf65376_adc(cc, e11, 0, &e11);
        assert(cc == 0);

        // + 3 mul = 30
        inner_gf65376_umul(lo, hi, a->v2, b->v1);
        cc = inner_gf65376_adc(cc, e3, lo, &e3);
        cc = inner_gf65376_adc(cc, e4, hi, &e4);
        inner_gf65376_umul(lo, hi, a->v4, b->v1);
        cc = inner_gf65376_adc(cc, e5, lo, &e5);
        cc = inner_gf65376_adc(cc, e6, hi, &e6);
        inner_gf65376_umul(lo, hi, a->v4, b->v3);
        cc = inner_gf65376_adc(cc, e7, lo, &e7);
        cc = inner_gf65376_adc(cc, e8, hi, &e8);
        cc = inner_gf65376_adc(cc, e9, 0, &e9);
        cc = inner_gf65376_adc(cc, e10, 0, &e10);
        cc = inner_gf65376_adc(cc, e11, 0, &e11);
        assert(cc == 0);

        // + 2 mul = 32
        inner_gf65376_umul(lo, hi, a->v1, b->v3);
        cc = inner_gf65376_adc(cc, e4, lo, &e4);
        cc = inner_gf65376_adc(cc, e5, hi, &e5);
        inner_gf65376_umul(lo, hi, a->v1, b->v5);
        cc = inner_gf65376_adc(cc, e6, lo, &e6);
        cc = inner_gf65376_adc(cc, e7, hi, &e7);
        cc = inner_gf65376_adc(cc, e8, 0, &e8);
        cc = inner_gf65376_adc(cc, e9, 0, &e9);
        cc = inner_gf65376_adc(cc, e10, 0, &e10);
        cc = inner_gf65376_adc(cc, e11, 0, &e11);
        assert(cc == 0);

        // + 2 mul = 34
        inner_gf65376_umul(lo, hi, a->v3, b->v1);
        cc = inner_gf65376_adc(cc, e4, lo, &e4);
        cc = inner_gf65376_adc(cc, e5, hi, &e5);
        inner_gf65376_umul(lo, hi, a->v5, b->v1);
        cc = inner_gf65376_adc(cc, e6, lo, &e6);
        cc = inner_gf65376_adc(cc, e7, hi, &e7);
        cc = inner_gf65376_adc(cc, e8, 0, &e8);
        cc = inner_gf65376_adc(cc, e9, 0, &e9);
        cc = inner_gf65376_adc(cc, e10, 0, &e10);
        cc = inner_gf65376_adc(cc, e11, 0, &e11);
        assert(cc == 0);

        // + 2 mul = 36
        inner_gf65376_umul(lo, hi, a->v2, b->v3);
        inner_gf65376_umul(lo2, hi2, a->v3, b->v2);
        cc = inner_gf65376_adc(0, lo, lo2, &lo);
        cc = inner_gf65376_adc(cc, hi, hi2, &hi);
        cc = inner_gf65376_adc(cc, 0, 0, &hi2);
        assert(cc == 0);
        cc = inner_gf65376_adc(0, e5, lo, &e5);
        cc = inner_gf65376_adc(cc, e6, hi, &e6);
        cc = inner_gf65376_adc(cc, e7, hi2, &e7);
        cc = inner_gf65376_adc(cc, e8, 0, &e8);
        cc = inner_gf65376_adc(cc, e9, 0, &e9);
        cc = inner_gf65376_adc(cc, e10, 0, &e10);
        cc = inner_gf65376_adc(cc, e11, 0, &e11);
        assert(cc == 0);

        // Montgomery reduction.
        //
        // Low part is lo(e) = e0..e5 (384 bits).
        // Let m = -1/q mod 2^384; we add (lo(e)*m mod 2^384)*q to the
        // high part g = e6..e11 (766 bits).
        //
        // We have m = 65*2^376 + 1.
        f0 = e0;
        f1 = e1;
        f2 = e2;
        f3 = e3;
        f4 = e4;
        f5 = e5 + ((e0 * 65) << 56);

        // g = f*q
        inner_gf65376_umul(g5, hi, f0, (uint64_t)65 << 56);
        inner_gf65376_umul_add(g6, hi, f1, (uint64_t)65 << 56, hi);
        inner_gf65376_umul_add(g7, hi, f2, (uint64_t)65 << 56, hi);
        inner_gf65376_umul_add(g8, hi, f3, (uint64_t)65 << 56, hi);
        inner_gf65376_umul_add(g9, hi, f4, (uint64_t)65 << 56, hi);
        inner_gf65376_umul_add(g10, g11, f5, (uint64_t)65 << 56, hi);

        cc = inner_gf65376_sbb(0, 0, f0, &g0);
        cc = inner_gf65376_sbb(cc, 0, f1, &g1);
        cc = inner_gf65376_sbb(cc, 0, f2, &g2);
        cc = inner_gf65376_sbb(cc, 0, f3, &g3);
        cc = inner_gf65376_sbb(cc, 0, f4, &g4);
        cc = inner_gf65376_sbb(cc, g5, f5, &g5);
        cc = inner_gf65376_sbb(cc, g6, 0, &g6);
        cc = inner_gf65376_sbb(cc, g7, 0, &g7);
        cc = inner_gf65376_sbb(cc, g8, 0, &g8);
        cc = inner_gf65376_sbb(cc, g9, 0, &g9);
        cc = inner_gf65376_sbb(cc, g10, 0, &g10);
        cc = inner_gf65376_sbb(cc, g11, 0, &g11);
        assert(cc == 0);

        // Add g = f*q to e0..e11.
        // Since e0..e11 < 2^766 and f < 2^384, we know that the result
        // is less than 2^766 + 2^384*65*2^376, which is less than 2^768.
        // This is also a multiple of 2^256. We divide by 2^256 by simply
        // dropping the low 256 bits (which are all equal to zero), and
        // the result is less than 2**384
        cc = inner_gf65376_adc(0, g0, e0, &e0);
        cc = inner_gf65376_adc(cc, g1, e1, &e1);
        cc = inner_gf65376_adc(cc, g2, e2, &e2);
        cc = inner_gf65376_adc(cc, g3, e3, &e3);
        cc = inner_gf65376_adc(cc, g4, e4, &e4);
        cc = inner_gf65376_adc(cc, g5, e5, &e5);
        cc = inner_gf65376_adc(cc, g6, e6, &e6);
        cc = inner_gf65376_adc(cc, g7, e7, &e7);
        cc = inner_gf65376_adc(cc, g8, e8, &e8);
        cc = inner_gf65376_adc(cc, g9, e9, &e9);
        cc = inner_gf65376_adc(cc, g10, e10, &e10);
        cc = inner_gf65376_adc(cc, g11, e11, &e11);
        assert(cc == 0);

        // To ensure the result is in the allowable range, we still need to
        // do a final reduction to ensure the value is smaller than 2^383
        inner_gf65376_partial_reduce(d, e6, e7, e8, e9, e10, e11);
    }

    /*
     * d <- a^2
     */
    static inline void
    gf65376_square(gf65376 *d, const gf65376 *a)
    {
        uint64_t e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11;
        uint64_t f0, f1, f2, f3, f4, f5, lo, hi;
        uint64_t g0, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11;
        unsigned char cc;

        // Squaring over integers.
        // 5 mul
        inner_gf65376_umul(e1, e2, a->v0, a->v1);
        inner_gf65376_umul(e3, e4, a->v0, a->v3);
        inner_gf65376_umul(e5, e6, a->v0, a->v5);
        inner_gf65376_umul(e7, e8, a->v2, a->v5);
        inner_gf65376_umul(e9, e10, a->v4, a->v5);

        inner_gf65376_umul(lo, hi, a->v0, a->v2);
        cc = inner_gf65376_adc(0, e2, lo, &e2);
        cc = inner_gf65376_adc(cc, e3, hi, &e3);
        inner_gf65376_umul(lo, hi, a->v0, a->v4);
        cc = inner_gf65376_adc(cc, e4, lo, &e4);
        cc = inner_gf65376_adc(cc, e5, hi, &e5);
        inner_gf65376_umul(lo, hi, a->v1, a->v5);
        cc = inner_gf65376_adc(cc, e6, lo, &e6);
        cc = inner_gf65376_adc(cc, e7, hi, &e7);
        inner_gf65376_umul(lo, hi, a->v3, a->v5);
        cc = inner_gf65376_adc(cc, e8, lo, &e8);
        cc = inner_gf65376_adc(cc, e9, hi, &e9);
        (void)inner_gf65376_adc(cc, e10, 0, &e10);

        inner_gf65376_umul(lo, hi, a->v1, a->v2);
        cc = inner_gf65376_adc(0, e3, lo, &e3);
        cc = inner_gf65376_adc(cc, e4, hi, &e4);
        inner_gf65376_umul(lo, hi, a->v1, a->v4);
        cc = inner_gf65376_adc(cc, e5, lo, &e5);
        cc = inner_gf65376_adc(cc, e6, hi, &e6);
        inner_gf65376_umul(lo, hi, a->v3, a->v4);
        cc = inner_gf65376_adc(cc, e7, lo, &e7);
        cc = inner_gf65376_adc(cc, e8, hi, &e8);
        (void)inner_gf65376_adc(cc, e9, 0, &e9);

        inner_gf65376_umul(lo, hi, a->v1, a->v3);
        cc = inner_gf65376_adc(0, e4, lo, &e4);
        cc = inner_gf65376_adc(cc, e5, hi, &e5);
        inner_gf65376_umul(lo, hi, a->v2, a->v4);
        cc = inner_gf65376_adc(cc, e6, lo, &e6);
        cc = inner_gf65376_adc(cc, e7, hi, &e7);
        (void)inner_gf65376_adc(cc, e8, 0, &e8);

        inner_gf65376_umul(lo, hi, a->v2, a->v3);
        cc = inner_gf65376_adc(0, e5, lo, &e5);
        cc = inner_gf65376_adc(cc, e6, hi, &e6);
        (void)inner_gf65376_adc(cc, e7, 0, &e7);

        e11 = e10 >> 63;
        e10 = (e10 << 1) | (e9 >> 63);
        e9 = (e9 << 1) | (e8 >> 63);
        e8 = (e8 << 1) | (e7 >> 63);
        e7 = (e7 << 1) | (e6 >> 63);
        e6 = (e6 << 1) | (e5 >> 63);
        e5 = (e5 << 1) | (e4 >> 63);
        e4 = (e4 << 1) | (e3 >> 63);
        e3 = (e3 << 1) | (e2 >> 63);
        e2 = (e2 << 1) | (e1 >> 63);
        e1 = e1 << 1;

        inner_gf65376_umul(e0, hi, a->v0, a->v0);
        cc = inner_gf65376_adc(0, e1, hi, &e1);
        inner_gf65376_umul(lo, hi, a->v1, a->v1);
        cc = inner_gf65376_adc(cc, e2, lo, &e2);
        cc = inner_gf65376_adc(cc, e3, hi, &e3);
        inner_gf65376_umul(lo, hi, a->v2, a->v2);
        cc = inner_gf65376_adc(cc, e4, lo, &e4);
        cc = inner_gf65376_adc(cc, e5, hi, &e5);
        inner_gf65376_umul(lo, hi, a->v3, a->v3);
        cc = inner_gf65376_adc(cc, e6, lo, &e6);
        cc = inner_gf65376_adc(cc, e7, hi, &e7);
        inner_gf65376_umul(lo, hi, a->v4, a->v4);
        cc = inner_gf65376_adc(cc, e8, lo, &e8);
        cc = inner_gf65376_adc(cc, e9, hi, &e9);
        inner_gf65376_umul(lo, hi, a->v5, a->v5);
        cc = inner_gf65376_adc(cc, e10, lo, &e10);
        (void)inner_gf65376_adc(cc, e11, hi, &e11);

        // Montgomery reduction.
        //
        // Low part is lo(e) = e0..e5 (384 bits).
        // Let m = -1/q mod 2^384; we add (lo(e)*m mod 2^384)*q to the
        // high part g = e6..e11 (766 bits).
        //
        // We have m = 65*2^376 + 1.
        f0 = e0;
        f1 = e1;
        f2 = e2;
        f3 = e3;
        f4 = e4;
        f5 = e5 + ((e0 * 65) << 56);

        // g = f*q
        inner_gf65376_umul(g5, hi, f0, (uint64_t)65 << 56);
        inner_gf65376_umul_add(g6, hi, f1, (uint64_t)65 << 56, hi);
        inner_gf65376_umul_add(g7, hi, f2, (uint64_t)65 << 56, hi);
        inner_gf65376_umul_add(g8, hi, f3, (uint64_t)65 << 56, hi);
        inner_gf65376_umul_add(g9, hi, f4, (uint64_t)65 << 56, hi);
        inner_gf65376_umul_add(g10, g11, f5, (uint64_t)65 << 56, hi);

        cc = inner_gf65376_sbb(0, 0, f0, &g0);
        cc = inner_gf65376_sbb(cc, 0, f1, &g1);
        cc = inner_gf65376_sbb(cc, 0, f2, &g2);
        cc = inner_gf65376_sbb(cc, 0, f3, &g3);
        cc = inner_gf65376_sbb(cc, 0, f4, &g4);
        cc = inner_gf65376_sbb(cc, g5, f5, &g5);
        cc = inner_gf65376_sbb(cc, g6, 0, &g6);
        cc = inner_gf65376_sbb(cc, g7, 0, &g7);
        cc = inner_gf65376_sbb(cc, g8, 0, &g8);
        cc = inner_gf65376_sbb(cc, g9, 0, &g9);
        cc = inner_gf65376_sbb(cc, g10, 0, &g10);
        cc = inner_gf65376_sbb(cc, g11, 0, &g11);
        assert(cc == 0);

        // Add g = f*q to e0..e11.
        // Since e0..e11 < 2^766 and f < 2^384, we know that the result
        // is less than 2^766 + 2^384*65*2^376, which is less than 2^768.
        // This is also a multiple of 2^256. We divide by 2^256 by simply
        // dropping the low 256 bits (which are all equal to zero), and
        // the result is less than 2**384
        cc = inner_gf65376_adc(0, g0, e0, &e0);
        cc = inner_gf65376_adc(cc, g1, e1, &e1);
        cc = inner_gf65376_adc(cc, g2, e2, &e2);
        cc = inner_gf65376_adc(cc, g3, e3, &e3);
        cc = inner_gf65376_adc(cc, g4, e4, &e4);
        cc = inner_gf65376_adc(cc, g5, e5, &e5);
        cc = inner_gf65376_adc(cc, g6, e6, &e6);
        cc = inner_gf65376_adc(cc, g7, e7, &e7);
        cc = inner_gf65376_adc(cc, g8, e8, &e8);
        cc = inner_gf65376_adc(cc, g9, e9, &e9);
        cc = inner_gf65376_adc(cc, g10, e10, &e10);
        cc = inner_gf65376_adc(cc, g11, e11, &e11);
        assert(cc == 0);

        // To ensure the result is in the allowable range, we still need to
        // do a final reduction to ensure the value is smaller than 2^383
        inner_gf65376_partial_reduce(d, e6, e7, e8, e9, e10, e11);
    }

    /*
     * d <- a^(2^n)
     * This computes n successive squarings of value a, with result in d.
     * n == 0 is a valid input (in that case, *a is copied into *d).
     * This function is not constant-time with regard to n: the number of
     * successive squarings may be observable through timing-based side channels.
     */
    static inline void
    gf65376_xsquare(gf65376 *d, const gf65376 *a, unsigned n)
    {
        if (n == 0) {
            *d = *a;
            return;
        }
        gf65376_square(d, a);
        while (n-- > 1) {
            gf65376_square(d, d);
        }
    }

    /*
     * Returns 0xFFFFFFFF if *a is zero; otherwise, 0x00000000 is returned.
     */
    static inline uint32_t
    gf65376_iszero(const gf65376 *a)
    {
        uint64_t a0, a1, a2, a3, a4, a5, t0, t1, r;

        // Zero can be represented by 0 or by q.
        a0 = a->v0;
        a1 = a->v1;
        a2 = a->v2;
        a3 = a->v3;
        a4 = a->v4;
        a5 = a->v5;
        t0 = a0 | a1 | a2 | a3 | a4 | a5;
        t1 = ~a0 | ~a1 | ~a2 | ~a3 | ~a4 | (a5 ^ 0x40FFFFFFFFFFFFFF);

        // Top bit of r is 0 if and only if one of t0 or t1 is zero.
        r = (t0 | -t0) & (t1 | -t1);
        return (uint32_t)(r >> 63) - 1;
    }

    /*
     * Returns 0xFFFFFFFF if *a and *b represent the same field element;
     * otherwise, 0x00000000 is returned.
     */
    static inline uint32_t
    gf65376_equals(const gf65376 *a, const gf65376 *b)
    {
        gf65376 d;
        gf65376_sub(&d, a, b);
        return gf65376_iszero(&d);
    }

    /*
     * d <- 1/a
     * If *a is not zero, then the inverse is well-defined and written into *d,
     * and the function returns 0xFFFFFFFF. If *a is zero, then this function
     * sets *d to zero and returns 0x00000000.
     */
    uint32_t gf65376_invert(gf65376 *d, const gf65376 *a);

    /*
     * d <- a/b
     * If *b is not zero, then this functions writes a/b into *d, and returns
     * 0xFFFFFFFF. If *b is zero, then this function sets *d to zero (regardless
     * of the value of *a) and returns 0x00000000.
     */
    uint32_t gf65376_div(gf65376 *d, const gf65376 *a, const gf65376 *b);

    /*
     * d <- a/3
     * Divides by 3 in the field by implementing the algorithm proposed in
     * "Efficient Multiplication in Finite Field Extensions of Degree 5"
     * by El Mrabet, Guillevic and Ionica at ASIACRYPT 2011.
     */
    void gf65376_div3(gf65376 *out, const gf65376 *a);

    /*
     * Get the Legendre symbol of *a (0 for zero, +1 for a non-zero square,
     * -1 for a non-square).
     */
    int32_t gf65376_legendre(const gf65376 *a);

    /*
     * If *a is a square, then this function sets *d to a square root of a,
     * and returns 0xFFFFFFFF. If *a is not a square, then this function
     * sets *d to a square root of -a, and returns 0x00000000.
     * In all cases, the value written into *d is such that the least significant
     * bit of its integer representation (in [0..q-1]) is zero.
     */
    uint32_t gf65376_sqrt(gf65376 *d, const gf65376 *a);

    /*
     * Encode field element *a into buffer dst (exactly 32 bytes are written).
     */
    void gf65376_encode(void *dst, const gf65376 *a);

    /*
     * Decode source buffer src (exactly 32 bytes) into a field element *d.
     * If the source value is not a valid canonical encoding, then *d is zero
     * and the function returns 0x00000000; otherwise, the function returns
     * 0xFFFFFFFF.
     */
    uint32_t gf65376_decode(gf65376 *d, const void *src);

    /*
     * Interpret the source buffer (of size len bytes) as an unsigned integer
     * (little-endian convention) and reduce it modulo q, yielding a field
     * element which is written into *d. Since reduction is applied, this
     * function cannot fail.
     */
    void gf65376_decode_reduce(gf65376 *d, const void *src, size_t len);

#ifdef __cplusplus
}
#endif

#endif
