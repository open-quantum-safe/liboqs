#ifndef gf27500_h__
#define gf27500_h__

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
     * A gf27500 instance represents an integer modulo q.
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
        // representation, with R = 2^512. Only partial reduction is
        // done internally to ensure the value is below 2^505
        struct
        {
            uint64_t v0;
            uint64_t v1;
            uint64_t v2;
            uint64_t v3;
            uint64_t v4;
            uint64_t v5;
            uint64_t v6;
            uint64_t v7;
        };
        digit_t arr[8];
    } gf27500;

    /*
     * Constant zero (in the field).
     */
    extern const gf27500 ZERO;

    /*
     * Constant one (in the field).
     */
    extern const gf27500 ONE;

    /*
     * Constant -1 (in the field).
     */
    extern const gf27500 gf27500_MINUS_ONE;

    /*
     * API RULES:
     * ==========
     *
     * Elementary operations on field elements are implemented by functions
     * which take as parameter pointers to the operands. The first parameter
     * is the pointer to the destination. Thus:
     *    gf27500 a = ...;
     *    gf27500 b = ...;
     *    gf27500 d;
     *    gf27500_sub(&d, &a, &b)
     * sets field element d to a - b (implicitly modulo q).
     *
     * Operands may be used several times: it is always valid to use as
     * output a gf27500 structure which is also used as input.
     *
     * Boolean values are represented by 32-bit integer (uint32_t) which have
     * value exactly 0xFFFFFFFF (for "true") or 0x00000000 (for "false"). This
     * convention minimizes the risk that a "smart" compiler breaks the
     * constant-time property of the code through unfortunated optimizations.
     * When a function expects such a Boolean, the caller MUST take care never
     * to provide any value other than 0x00000000 or 0xFFFFFFFF.
     *
     * Values are encoded into exactly 64 bytes: value x modulo q is mapped to
     * its unique integer representant in the [0..q-1] range, which is then
     * encoded over 64 bytes with little-endian convention. Encoding is canonical
     * and checked: when decoding (with gf27500_decode()), the input value is
     * verified to be in the [0..q-1] range; for an out-of-range value,
     * gf27500_decode() fills the output structure with zero, and returns
     * 0x00000000.
     *
     * For most operations, the implementation is an inline function, defined
     * below; the compiler can thus efficiently include it in the calling code.
     * A few expensive operations (e.g. divisions) use non-inline functions,
     * declared below but defined in gf27500.c
     *
     * All functions and macro whose name starts with "inner_gf27500_" are
     * internal to this implementation and visible here only in order to
     * support the API inline functions; they MUST NOT be used directly.
     */

#if (defined _MSC_VER && defined _M_X64) || (defined __x86_64__ && (defined __GNUC__ || defined __clang__)) || defined(C_PEDANTIC_MODE)
#include <immintrin.h>
#define inner_gf27500_adc(cc, a, b, d) _addcarry_u64(cc, a, b, (unsigned long long *)(void *)d)
#define inner_gf27500_sbb(cc, a, b, d) _subborrow_u64(cc, a, b, (unsigned long long *)(void *)d)
#else
static inline unsigned char
inner_gf27500_adc(unsigned char cc, uint64_t a, uint64_t b, uint64_t *d)
{
    unsigned __int128 t = (unsigned __int128)a + (unsigned __int128)b + cc;
    *d = (uint64_t)t;
    return (unsigned char)(t >> 64);
}
static inline unsigned char
inner_gf27500_sbb(unsigned char cc, uint64_t a, uint64_t b, uint64_t *d)
{
    unsigned __int128 t = (unsigned __int128)a - (unsigned __int128)b - cc;
    *d = (uint64_t)t;
    return (unsigned char)(-(uint64_t)(t >> 64));
}
#endif

#if defined _MSC_VER || defined(C_PEDANTIC_MODE)
#if defined _MSC_VER 
#define inner_gf27500_umul(lo, hi, x, y)                                                                               \
    do {                                                                                                               \
        uint64_t umul_hi;                                                                                              \
        (lo) = _umul128((x), (y), &umul_hi);                                                                           \
        (hi) = umul_hi;                                                                                                \
    } while (0)
#else
#define inner_gf27500_umul(lo, hi, a, b)                                                                                \
    do {                                                                                                               \
        register uint64_t al, ah, bl, bh, temp; \
        uint64_t albl, albh, ahbl, ahbh, res1, res2, res3, carry; \
        uint64_t mask_low = (uint64_t)(-1) >> (sizeof(uint64_t) * 4), mask_high = (uint64_t)(-1) << (sizeof(uint64_t) * 4); \
        al = a & mask_low; \
        ah = a >> (sizeof(uint64_t) * 4); \
        bl = b & mask_low; \
        bh = b >> (sizeof(uint64_t) * 4); \
        albl = al * bl; \
        albh = al * bh; \
        ahbl = ah * bl; \
        ahbh = ah * bh; \
        (lo) = albl & mask_low; \
        res1 = albl >> (sizeof(uint64_t) * 4); \
        res2 = ahbl & mask_low; \
        res3 = albh & mask_low; \
        temp = res1 + res2 + res3 ; \
        carry = temp >> (sizeof(uint64_t) * 4); \
        (lo) ^= temp << (sizeof(uint64_t) * 4); \
        res1 = ahbl >> (sizeof(uint64_t) * 4); \
        res2 = albh >> (sizeof(uint64_t) * 4); \
        res3 = ahbh & mask_low; \
        temp = res1 + res2 + res3 + carry; \
        (hi) = temp & mask_low; \
        carry = temp & mask_high; \
        (hi) ^= (ahbh & mask_high) + carry; \
    } while (0)
#endif
#define inner_gf27500_umul_add(lo, hi, x, y, z)                                                                        \
    do {                                                                                                               \
        uint64_t umul_lo, umul_hi;                                                                                     \
        inner_gf27500_umul(umul_lo, umul_hi, (x), (y));                                                                \
        unsigned char umul_cc;                                                                                         \
        umul_cc = inner_gf27500_adc(0, umul_lo, (z), &umul_lo);                                                        \
        (void)inner_gf27500_adc(umul_cc, umul_hi, 0, &umul_hi);                                                        \
        (lo) = umul_lo;                                                                                                \
        (hi) = umul_hi;                                                                                                \
    } while (0)
#define inner_gf27500_umul_x2(lo, hi, x1, y1, x2, y2)                                                                  \
    do {                                                                                                               \
        uint64_t umul_lo, umul_hi;                                                                                     \
        inner_gf27500_umul(umul_lo, umul_hi, (x1), (y1));                                                              \
        uint64_t umul_lo2, umul_hi2;                                                                                   \
        inner_gf27500_umul(umul_lo2, umul_hi2, (x2), (y2));                                                            \
        unsigned char umul_cc;                                                                                         \
        umul_cc = inner_gf27500_adc(0, umul_lo, umul_lo2, &umul_lo);                                                   \
        (void)inner_gf27500_adc(umul_cc, umul_hi, umul_hi2, &umul_hi);                                                 \
        (lo) = umul_lo;                                                                                                \
        (hi) = umul_hi;                                                                                                \
    } while (0)
#define inner_gf27500_umul_x2_add(lo, hi, x1, y1, x2, y2, z)                                                           \
    do {                                                                                                               \
        uint64_t umul_lo, umul_hi;                                                                                     \
        inner_gf27500_umul(umul_lo, umul_hi, (x1), (y1));                                                              \
        uint64_t umul_lo2, umul_hi2;                                                                                   \
        inner_gf27500_umul(umul_lo2, umul_hi2, (x2), (y2));                                                            \
        unsigned char umul_cc;                                                                                         \
        umul_cc = inner_gf27500_adc(0, umul_lo, umul_lo2, &umul_lo);                                                   \
        (void)inner_gf27500_adc(umul_cc, umul_hi, umul_hi2, &umul_hi);                                                 \
        umul_cc = inner_gf27500_adc(0, umul_lo, (z), &umul_lo);                                                        \
        (void)inner_gf27500_adc(umul_cc, umul_hi, 0, &umul_hi);                                                        \
        (lo) = umul_lo;                                                                                                \
        (hi) = umul_hi;                                                                                                \
    } while (0)
#else
#define inner_gf27500_umul(lo, hi, x, y)                                                                               \
    do {                                                                                                               \
        unsigned __int128 umul_tmp;                                                                                    \
        umul_tmp = (unsigned __int128)(x) * (unsigned __int128)(y);                                                    \
        (lo) = (uint64_t)umul_tmp;                                                                                     \
        (hi) = (uint64_t)(umul_tmp >> 64);                                                                             \
    } while (0)
#define inner_gf27500_umul_add(lo, hi, x, y, z)                                                                        \
    do {                                                                                                               \
        unsigned __int128 umul_tmp;                                                                                    \
        umul_tmp = (unsigned __int128)(x) * (unsigned __int128)(y) + (unsigned __int128)(uint64_t)(z);                 \
        (lo) = (uint64_t)umul_tmp;                                                                                     \
        (hi) = (uint64_t)(umul_tmp >> 64);                                                                             \
    } while (0)
#define inner_gf27500_umul_x2(lo, hi, x1, y1, x2, y2)                                                                  \
    do {                                                                                                               \
        unsigned __int128 umul_tmp;                                                                                    \
        umul_tmp =                                                                                                     \
            (unsigned __int128)(x1) * (unsigned __int128)(y1) + (unsigned __int128)(x2) * (unsigned __int128)(y2);     \
        (lo) = (uint64_t)umul_tmp;                                                                                     \
        (hi) = (uint64_t)(umul_tmp >> 64);                                                                             \
    } while (0)
#define inner_gf27500_umul_x2_add(lo, hi, x1, y1, x2, y2, z)                                                           \
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
    gf27500_add(gf27500 *d, const gf27500 *a, const gf27500 *b)
    {
        uint64_t d0, d1, d2, d3, d4, d5, d6, d7, f;
        unsigned char cc;

        // Raw addition.
        cc = inner_gf27500_adc(0, a->v0, b->v0, &d0);
        cc = inner_gf27500_adc(cc, a->v1, b->v1, &d1);
        cc = inner_gf27500_adc(cc, a->v2, b->v2, &d2);
        cc = inner_gf27500_adc(cc, a->v3, b->v3, &d3);
        cc = inner_gf27500_adc(cc, a->v4, b->v4, &d4);
        cc = inner_gf27500_adc(cc, a->v5, b->v5, &d5);
        cc = inner_gf27500_adc(cc, a->v6, b->v6, &d6);
        (void)inner_gf27500_adc(cc, a->v7, b->v7, &d7);

        // Sum is up to 2^506 - 2. Subtract q if the value is not lower
        // than 2^505 (we subtract q by adding -q).
        // Note: 0xE5 = (-27) % 256, 52 = 500 - 7*64
        f = d7 >> 57;
        cc = inner_gf27500_adc(0, d0, f, &d0);
        cc = inner_gf27500_adc(cc, d1, 0, &d1);
        cc = inner_gf27500_adc(cc, d2, 0, &d2);
        cc = inner_gf27500_adc(cc, d3, 0, &d3);
        cc = inner_gf27500_adc(cc, d4, 0, &d4);
        cc = inner_gf27500_adc(cc, d5, 0, &d5);
        cc = inner_gf27500_adc(cc, d6, 0, &d6);
        (void)inner_gf27500_adc(cc, d7, ((uint64_t)0xFE5 << 52) & -f, &d7);

        // One subtraction of q might not be enough.
        f = d7 >> 57;
        cc = inner_gf27500_adc(0, d0, f, &d0);
        cc = inner_gf27500_adc(cc, d1, 0, &d1);
        cc = inner_gf27500_adc(cc, d2, 0, &d2);
        cc = inner_gf27500_adc(cc, d3, 0, &d3);
        cc = inner_gf27500_adc(cc, d4, 0, &d4);
        cc = inner_gf27500_adc(cc, d5, 0, &d5);
        cc = inner_gf27500_adc(cc, d6, 0, &d6);
        (void)inner_gf27500_adc(cc, d7, ((uint64_t)0xFE5 << 52) & -f, &d7);

        d->v0 = d0;
        d->v1 = d1;
        d->v2 = d2;
        d->v3 = d3;
        d->v4 = d4;
        d->v5 = d5;
        d->v6 = d6;
        d->v7 = d7;
    }

    /*
     * d <- a - b
     */
    static inline void
    gf27500_sub(gf27500 *d, const gf27500 *a, const gf27500 *b)
    {
        uint64_t d0, d1, d2, d3, d4, d5, d6, d7, m, f;
        unsigned char cc;

        // Raw subtraction.
        cc = inner_gf27500_sbb(0, a->v0, b->v0, &d0);
        cc = inner_gf27500_sbb(cc, a->v1, b->v1, &d1);
        cc = inner_gf27500_sbb(cc, a->v2, b->v2, &d2);
        cc = inner_gf27500_sbb(cc, a->v3, b->v3, &d3);
        cc = inner_gf27500_sbb(cc, a->v4, b->v4, &d4);
        cc = inner_gf27500_sbb(cc, a->v5, b->v5, &d5);
        cc = inner_gf27500_sbb(cc, a->v6, b->v6, &d6);
        cc = inner_gf27500_sbb(cc, a->v7, b->v7, &d7);

        // Add 2*q if the result is negative.
        // Note: 0xCA = (-2*27) % 256, 52 = 500 - 7*64
        (void)inner_gf27500_sbb(cc, 0, 0, &m);
        cc = inner_gf27500_sbb(0, d0, m & 2, &d0);
        cc = inner_gf27500_sbb(cc, d1, 0, &d1);
        cc = inner_gf27500_sbb(cc, d2, 0, &d2);
        cc = inner_gf27500_sbb(cc, d3, 0, &d3);
        cc = inner_gf27500_sbb(cc, d4, 0, &d4);
        cc = inner_gf27500_sbb(cc, d5, 0, &d5);
        cc = inner_gf27500_sbb(cc, d6, 0, &d6);
        (void)inner_gf27500_sbb(cc, d7, ((uint64_t)0xFCA << 52) & m, &d7);

        // We might have overdone it; subtract q if necessary.
        // Note: 0xE5 = (-27) % 256, 52 = 500 - 7*64
        f = d7 >> 57;
        cc = inner_gf27500_adc(0, d0, f, &d0);
        cc = inner_gf27500_adc(cc, d1, 0, &d1);
        cc = inner_gf27500_adc(cc, d2, 0, &d2);
        cc = inner_gf27500_adc(cc, d3, 0, &d3);
        cc = inner_gf27500_adc(cc, d4, 0, &d4);
        cc = inner_gf27500_adc(cc, d5, 0, &d5);
        cc = inner_gf27500_adc(cc, d6, 0, &d6);
        (void)inner_gf27500_adc(cc, d7, ((uint64_t)0xFE5 << 52) & -f, &d7);

        d->v0 = d0;
        d->v1 = d1;
        d->v2 = d2;
        d->v3 = d3;
        d->v4 = d4;
        d->v5 = d5;
        d->v6 = d6;
        d->v7 = d7;
    }

    /*
     * d <- -a
     */
    static inline void
    gf27500_neg(gf27500 *d, const gf27500 *a)
    {
        uint64_t d0, d1, d2, d3, d4, d5, d6, d7, f;
        unsigned char cc;

        // 2*q - a
        cc = inner_gf27500_sbb(0, (uint64_t)0xFFFFFFFFFFFFFFFE, a->v0, &d0);
        cc = inner_gf27500_sbb(cc, (uint64_t)0xFFFFFFFFFFFFFFFF, a->v1, &d1);
        cc = inner_gf27500_sbb(cc, (uint64_t)0xFFFFFFFFFFFFFFFF, a->v2, &d2);
        cc = inner_gf27500_sbb(cc, (uint64_t)0xFFFFFFFFFFFFFFFF, a->v3, &d3);
        cc = inner_gf27500_sbb(cc, (uint64_t)0xFFFFFFFFFFFFFFFF, a->v4, &d4);
        cc = inner_gf27500_sbb(cc, (uint64_t)0xFFFFFFFFFFFFFFFF, a->v5, &d5);
        cc = inner_gf27500_sbb(cc, (uint64_t)0xFFFFFFFFFFFFFFFF, a->v6, &d6);
        (void)inner_gf27500_sbb(cc, (uint64_t)0x035FFFFFFFFFFFFF, a->v7, &d7);

        // Subtract q if the value is not lower than 2^505.
        f = d7 >> 57;
        cc = inner_gf27500_adc(0, d0, f, &d0);
        cc = inner_gf27500_adc(cc, d1, 0, &d1);
        cc = inner_gf27500_adc(cc, d2, 0, &d2);
        cc = inner_gf27500_adc(cc, d3, 0, &d3);
        cc = inner_gf27500_adc(cc, d4, 0, &d4);
        cc = inner_gf27500_adc(cc, d5, 0, &d5);
        cc = inner_gf27500_adc(cc, d6, 0, &d6);
        (void)inner_gf27500_adc(cc, d7, ((uint64_t)0xFE5 << 52) & -f, &d7);

        d->v0 = d0;
        d->v1 = d1;
        d->v2 = d2;
        d->v3 = d3;
        d->v4 = d4;
        d->v5 = d5;
        d->v6 = d6;
        d->v7 = d7;
    }

    /*
     * If ctl == 0x00000000, then *a0 is copied into *d.
     * If ctl == 0xFFFFFFFF, then *a1 is copied into *d.
     * ctl MUST be either 0x00000000 or 0xFFFFFFFF.
     */
    static inline void
    gf27500_select(gf27500 *d, const gf27500 *a0, const gf27500 *a1, uint32_t ctl)
    {
        uint64_t cw = (uint64_t)*(int32_t *)&ctl;
        d->v0 = a0->v0 ^ (cw & (a0->v0 ^ a1->v0));
        d->v1 = a0->v1 ^ (cw & (a0->v1 ^ a1->v1));
        d->v2 = a0->v2 ^ (cw & (a0->v2 ^ a1->v2));
        d->v3 = a0->v3 ^ (cw & (a0->v3 ^ a1->v3));
        d->v4 = a0->v4 ^ (cw & (a0->v4 ^ a1->v4));
        d->v5 = a0->v5 ^ (cw & (a0->v5 ^ a1->v5));
        d->v6 = a0->v6 ^ (cw & (a0->v6 ^ a1->v6));
        d->v7 = a0->v7 ^ (cw & (a0->v7 ^ a1->v7));
    }

    /*
     * If ctl == 0x00000000, then *a and *b are unchanged.
     * If ctl == 0xFFFFFFFF, then the contents of *a and *b are swapped.
     * ctl MUST be either 0x00000000 or 0xFFFFFFFF.
     */
    static inline void
    gf27500_cswap(gf27500 *a, gf27500 *b, uint32_t ctl)
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
        t = cw & (a->v6 ^ b->v6);
        a->v6 ^= t;
        b->v6 ^= t;
        t = cw & (a->v7 ^ b->v7);
        a->v7 ^= t;
        b->v7 ^= t;
    }

    /*
     * d <- a/2
     */
    static inline void
    gf27500_half(gf27500 *d, const gf27500 *a)
    {
        uint64_t d0, d1, d2, d3, d4, d5, d6, d7;

        d0 = (a->v0 >> 1) | (a->v1 << 63);
        d1 = (a->v1 >> 1) | (a->v2 << 63);
        d2 = (a->v2 >> 1) | (a->v3 << 63);
        d3 = (a->v3 >> 1) | (a->v4 << 63);
        d4 = (a->v4 >> 1) | (a->v5 << 63);
        d5 = (a->v5 >> 1) | (a->v6 << 63);
        d6 = (a->v6 >> 1) | (a->v7 << 63);
        d7 = a->v7 >> 1;
        d7 += ((uint64_t)27 << 51) & -(a->v0 & 1);
        d->v0 = d0;
        d->v1 = d1;
        d->v2 = d2;
        d->v3 = d3;
        d->v4 = d4;
        d->v5 = d5;
        d->v6 = d6;
        d->v7 = d7;
    }

    // Inner function: 512-bit to 505-bit reduction
    static inline void
    inner_gf27500_partial_reduce(gf27500 *d,
                                 uint64_t a0,
                                 uint64_t a1,
                                 uint64_t a2,
                                 uint64_t a3,
                                 uint64_t a4,
                                 uint64_t a5,
                                 uint64_t a6,
                                 uint64_t a7)
    {
        uint64_t d0, d1, d2, d3, d4, d5, d6, d7, h, quo, rem;
        unsigned char cc;

        // Split value in high (12 bits) and low (500 bits) parts.
        h = a7 >> 52;
        a7 &= 0x000FFFFFFFFFFFFF;

        // 27*2^500 = 1 mod q; hence, we add floor(h/27) + (h mod 27)*2^500
        // to the low part.
        quo = (0x12F7 * h) >> 17;
        rem = h - (27 * quo);

        cc = inner_gf27500_adc(0, a0, quo, &d0);
        cc = inner_gf27500_adc(cc, a1, 0, &d1);
        cc = inner_gf27500_adc(cc, a2, 0, &d2);
        cc = inner_gf27500_adc(cc, a3, 0, &d3);
        cc = inner_gf27500_adc(cc, a4, 0, &d4);
        cc = inner_gf27500_adc(cc, a5, 0, &d5);
        cc = inner_gf27500_adc(cc, a6, 0, &d6);
        (void)inner_gf27500_adc(cc, a7, rem << 52, &d7);

        d->v0 = d0;
        d->v1 = d1;
        d->v2 = d2;
        d->v3 = d3;
        d->v4 = d4;
        d->v5 = d5;
        d->v6 = d6;
        d->v7 = d7;
    }

    // Inner function: Normalize value *a into *d.
    static inline void
    inner_gf27500_normalize(gf27500 *d, const gf27500 *a)
    {
        uint64_t d0, d1, d2, d3, d4, d5, d6, d7, m;
        unsigned char cc;

        // Subtract q.
        cc = inner_gf27500_sbb(0, a->v0, 0xFFFFFFFFFFFFFFFF, &d0);
        cc = inner_gf27500_sbb(cc, a->v1, 0xFFFFFFFFFFFFFFFF, &d1);
        cc = inner_gf27500_sbb(cc, a->v2, 0xFFFFFFFFFFFFFFFF, &d2);
        cc = inner_gf27500_sbb(cc, a->v3, 0xFFFFFFFFFFFFFFFF, &d3);
        cc = inner_gf27500_sbb(cc, a->v4, 0xFFFFFFFFFFFFFFFF, &d4);
        cc = inner_gf27500_sbb(cc, a->v5, 0xFFFFFFFFFFFFFFFF, &d5);
        cc = inner_gf27500_sbb(cc, a->v6, 0xFFFFFFFFFFFFFFFF, &d6);
        cc = inner_gf27500_sbb(cc, a->v7, 0x01AFFFFFFFFFFFFF, &d7);

        // Add back q if the result is negative.
        (void)inner_gf27500_sbb(cc, 0, 0, &m);
        cc = inner_gf27500_adc(0, d0, m, &d0);
        cc = inner_gf27500_adc(cc, d1, m, &d1);
        cc = inner_gf27500_adc(cc, d2, m, &d2);
        cc = inner_gf27500_adc(cc, d3, m, &d3);
        cc = inner_gf27500_adc(cc, d4, m, &d4);
        cc = inner_gf27500_adc(cc, d5, m, &d5);
        cc = inner_gf27500_adc(cc, d6, m, &d6);
        (void)inner_gf27500_adc(cc, d7, m & 0x01AFFFFFFFFFFFFF, &d7);

        d->v0 = d0;
        d->v1 = d1;
        d->v2 = d2;
        d->v3 = d3;
        d->v4 = d4;
        d->v5 = d5;
        d->v6 = d6;
        d->v7 = d7;
    }

    /*
     * d <- 2*a
     */
    static inline void
    gf27500_mul2(gf27500 *d, const gf27500 *a)
    {
        gf27500_add(d, a, a);
    }

    /*
     * d <- a*x
     * (multiplication by a 32-bit integer)
     */
    static inline void
    gf27500_mul_small(gf27500 *d, const gf27500 *a, uint32_t x)
    {
        uint64_t d0, d1, d2, d3, d4, d5, d6, d7, d8;
        uint64_t lo, hi, b, h, quo, rem;
        unsigned char cc;

        // Product over the integers. Top output word (d6) is at most 31 bits.
        b = (uint64_t)x;
        inner_gf27500_umul(d0, d1, a->v0, b);
        inner_gf27500_umul(d2, d3, a->v2, b);
        inner_gf27500_umul(d4, d5, a->v4, b);
        inner_gf27500_umul(d6, d7, a->v6, b);

        inner_gf27500_umul(lo, hi, a->v1, b);
        cc = inner_gf27500_adc(0, d1, lo, &d1);
        cc = inner_gf27500_adc(cc, d2, hi, &d2);
        inner_gf27500_umul(lo, hi, a->v3, b);
        cc = inner_gf27500_adc(cc, d3, lo, &d3);
        cc = inner_gf27500_adc(cc, d4, hi, &d4);
        inner_gf27500_umul(lo, hi, a->v5, b);
        cc = inner_gf27500_adc(cc, d5, lo, &d5);
        cc = inner_gf27500_adc(cc, d6, hi, &d6);
        inner_gf27500_umul(lo, d8, a->v7, b);
        cc = inner_gf27500_adc(cc, d7, lo, &d7);
        (void)inner_gf27500_adc(cc, d8, 0, &d8);

        // Extract low 500-bit part, and the high part (at most 35 bits).
        h = (d8 << 12) | (d7 >> 52);
        d7 &= 0x000FFFFFFFFFFFFF;

        // Fold h by adding floor(h/65) + (h mod 65)*2^376 to the low part.
        inner_gf27500_umul(lo, hi, h, 0x97B425ED097B425F);
        quo = hi >> 4;
        rem = h - (27 * quo);

        cc = inner_gf27500_adc(cc, d0, quo, &d0);
        cc = inner_gf27500_adc(cc, d1, 0, &d1);
        cc = inner_gf27500_adc(cc, d2, 0, &d2);
        cc = inner_gf27500_adc(cc, d3, 0, &d3);
        cc = inner_gf27500_adc(cc, d4, 0, &d4);
        cc = inner_gf27500_adc(cc, d5, 0, &d5);
        cc = inner_gf27500_adc(cc, d6, 0, &d6);
        (void)inner_gf27500_adc(cc, d7, rem << 52, &d7);

        d->v0 = d0;
        d->v1 = d1;
        d->v2 = d2;
        d->v3 = d3;
        d->v4 = d4;
        d->v5 = d5;
        d->v6 = d6;
        d->v7 = d7;
    }

    /*
     * d <- x
     * Input value x (32-bit integer) is converted to field element x mod q.
     */
    static inline void
    gf27500_set_small(gf27500 *d, uint32_t x)
    {
        // We want Montgomery representation, i.e. x*2^512 mod q.
        // We set h = x*2^12; then:
        //   x*2^512 = h*2^500
        //           = (h mod 27)*2^500 + floor(h/27)*27*2^500
        //           = (h mod 27)*2^500 + floor(h/27)  mod q
        // by using the fact that 27*2^500 = 1 mod q.
        uint64_t h, lo, hi, quo, rem;
        h = (uint64_t)x << 12;
        inner_gf27500_umul(lo, hi, h, 0x97B425ED097B425F);
        (void)lo;
        quo = hi >> 4;
        rem = h - (27 * quo);

        d->v0 = quo;
        d->v1 = 0;
        d->v2 = 0;
        d->v3 = 0;
        d->v4 = 0;
        d->v5 = 0;
        d->v6 = 0;
        d->v7 = rem << 52;
    }

    // Inner function: d <- a/2^512, with normalization to [0..q-1].
    static inline void
    inner_gf27500_montgomery_reduce(gf27500 *d, const gf27500 *a)
    {
        uint64_t x0, x1, x2, x3, x4, x5, x6, x7;
        uint64_t f0, f1, f2, f3, f4, f5, f6, f7;
        uint64_t g0, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15;
        uint64_t d0, d1, d2, d3, d4, d5, d6, d7;
        uint64_t hi, t, w;
        unsigned char cc;

        // Let m = -1/q mod 2^512 = 27*2^500 + 1
        // For input x, we compute f = x*m mod 2^512, then
        // h = x + f*q, which is a multiple of 2^512. The output
        // is then h/2^512.
        // Since x < 2^512, we have:
        //    h <= 2^512 - 1 + (2^512 - 1)*q
        //    h <= q*2^512 + 2^512 - q - 1
        // Since h = 0 mod 2^512, this implies that h <= q*2^512.
        // The output h/2^512 is therefore between 0 and q (inclusive).

        x0 = a->v0;
        x1 = a->v1;
        x2 = a->v2;
        x3 = a->v3;
        x4 = a->v4;
        x5 = a->v5;
        x6 = a->v6;
        x7 = a->v7;

        // f = x*(-1/q) mod 2^500
        f0 = x0;
        f1 = x1;
        f2 = x2;
        f3 = x3;
        f4 = x4;
        f5 = x5;
        f6 = x6;
        f7 = x7 + ((x0 * 27) << 52);

        // g = f*q
        inner_gf27500_umul(g7, hi, f0, (uint64_t)27 << 52);
        inner_gf27500_umul_add(g8, hi, f1, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g9, hi, f2, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g10, hi, f3, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g11, hi, f4, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g12, hi, f5, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g13, hi, f6, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g14, g15, f7, (uint64_t)27 << 52, hi);

        cc = inner_gf27500_sbb(0, 0, f0, &g0);
        cc = inner_gf27500_sbb(cc, 0, f1, &g1);
        cc = inner_gf27500_sbb(cc, 0, f2, &g2);
        cc = inner_gf27500_sbb(cc, 0, f3, &g3);
        cc = inner_gf27500_sbb(cc, 0, f4, &g4);
        cc = inner_gf27500_sbb(cc, 0, f5, &g5);
        cc = inner_gf27500_sbb(cc, 0, f6, &g6);
        cc = inner_gf27500_sbb(cc, g7, f7, &g7);
        cc = inner_gf27500_sbb(cc, g8, 0, &g8);
        cc = inner_gf27500_sbb(cc, g9, 0, &g9);
        cc = inner_gf27500_sbb(cc, g10, 0, &g10);
        cc = inner_gf27500_sbb(cc, g11, 0, &g11);
        cc = inner_gf27500_sbb(cc, g12, 0, &g12);
        cc = inner_gf27500_sbb(cc, g13, 0, &g13);
        cc = inner_gf27500_sbb(cc, g14, 0, &g14);
        (void)inner_gf27500_sbb(cc, g15, 0, &g15);

        // h = x + f*q  (we drop the low 512 bits).
        cc = inner_gf27500_adc(0, g0, x0, &x0);
        cc = inner_gf27500_adc(cc, g1, x1, &x1);
        cc = inner_gf27500_adc(cc, g2, x2, &x2);
        cc = inner_gf27500_adc(cc, g3, x3, &x3);
        cc = inner_gf27500_adc(cc, g4, x4, &x4);
        cc = inner_gf27500_adc(cc, g5, x5, &x5);
        cc = inner_gf27500_adc(cc, g6, x6, &x6);
        cc = inner_gf27500_adc(cc, g7, x7, &x7);
        cc = inner_gf27500_adc(cc, g8, 0, &d0);
        cc = inner_gf27500_adc(cc, g9, 0, &d1);
        cc = inner_gf27500_adc(cc, g10, 0, &d2);
        cc = inner_gf27500_adc(cc, g11, 0, &d3);
        cc = inner_gf27500_adc(cc, g12, 0, &d4);
        cc = inner_gf27500_adc(cc, g13, 0, &d5);
        cc = inner_gf27500_adc(cc, g14, 0, &d6);
        (void)inner_gf27500_adc(cc, g15, 0, &d7);

        // Normalize: if h = q, replace it with zero.
        t = d0 & d1 & d2 & d3 & d4 & d5 & d6 & (d7 ^ ~(uint64_t)0x01AFFFFFFFFFFFFF);
        cc = inner_gf27500_adc(0, t, 1, &t);
        (void)inner_gf27500_sbb(cc, 0, 0, &w);
        w = ~w;
        d->v0 = d0 & w;
        d->v1 = d1 & w;
        d->v2 = d2 & w;
        d->v3 = d3 & w;
        d->v4 = d4 & w;
        d->v5 = d5 & w;
        d->v6 = d6 & w;
        d->v7 = d7 & w;
    }

    /*
     * d <- a*b
     */
    static inline void
    gf27500_mul(gf27500 *d, const gf27500 *a, const gf27500 *b)
    {
        uint64_t e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15;
        uint64_t f0, f1, f2, f3, f4, f5, f6, f7;
        uint64_t lo, hi, lo2, hi2;
        uint64_t g0, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15;
        unsigned char cc;

        // Multiplication over integers.
        // 8 mul
        inner_gf27500_umul(e0, e1, a->v0, b->v0);
        inner_gf27500_umul(e2, e3, a->v1, b->v1);
        inner_gf27500_umul(e4, e5, a->v2, b->v2);
        inner_gf27500_umul(e6, e7, a->v3, b->v3);
        inner_gf27500_umul(e8, e9, a->v4, b->v4);
        inner_gf27500_umul(e10, e11, a->v5, b->v5);
        inner_gf27500_umul(e12, e13, a->v6, b->v6);
        inner_gf27500_umul(e14, e15, a->v7, b->v7);

        // + 7 mul = 15
        inner_gf27500_umul(lo, hi, a->v0, b->v1);
        cc = inner_gf27500_adc(0, e1, lo, &e1);
        cc = inner_gf27500_adc(cc, e2, hi, &e2);
        inner_gf27500_umul(lo, hi, a->v0, b->v3);
        cc = inner_gf27500_adc(cc, e3, lo, &e3);
        cc = inner_gf27500_adc(cc, e4, hi, &e4);
        inner_gf27500_umul(lo, hi, a->v0, b->v5);
        cc = inner_gf27500_adc(cc, e5, lo, &e5);
        cc = inner_gf27500_adc(cc, e6, hi, &e6);
        inner_gf27500_umul(lo, hi, a->v0, b->v7);
        cc = inner_gf27500_adc(cc, e7, lo, &e7);
        cc = inner_gf27500_adc(cc, e8, hi, &e8);
        inner_gf27500_umul(lo, hi, a->v2, b->v7);
        cc = inner_gf27500_adc(cc, e9, lo, &e9);
        cc = inner_gf27500_adc(cc, e10, hi, &e10);
        inner_gf27500_umul(lo, hi, a->v4, b->v7);
        cc = inner_gf27500_adc(cc, e11, lo, &e11);
        cc = inner_gf27500_adc(cc, e12, hi, &e12);
        inner_gf27500_umul(lo, hi, a->v6, b->v7);
        cc = inner_gf27500_adc(cc, e13, lo, &e13);
        cc = inner_gf27500_adc(cc, e14, hi, &e14);
        cc = inner_gf27500_adc(cc, e15, 0, &e15);
        assert(cc == 0);

        // + 7 mul = 22
        inner_gf27500_umul(lo, hi, a->v1, b->v0);
        cc = inner_gf27500_adc(0, e1, lo, &e1);
        cc = inner_gf27500_adc(cc, e2, hi, &e2);
        inner_gf27500_umul(lo, hi, a->v3, b->v0);
        cc = inner_gf27500_adc(cc, e3, lo, &e3);
        cc = inner_gf27500_adc(cc, e4, hi, &e4);
        inner_gf27500_umul(lo, hi, a->v5, b->v0);
        cc = inner_gf27500_adc(cc, e5, lo, &e5);
        cc = inner_gf27500_adc(cc, e6, hi, &e6);
        inner_gf27500_umul(lo, hi, a->v7, b->v0);
        cc = inner_gf27500_adc(cc, e7, lo, &e7);
        cc = inner_gf27500_adc(cc, e8, hi, &e8);
        inner_gf27500_umul(lo, hi, a->v7, b->v2);
        cc = inner_gf27500_adc(cc, e9, lo, &e9);
        cc = inner_gf27500_adc(cc, e10, hi, &e10);
        inner_gf27500_umul(lo, hi, a->v7, b->v4);
        cc = inner_gf27500_adc(cc, e11, lo, &e11);
        cc = inner_gf27500_adc(cc, e12, hi, &e12);
        inner_gf27500_umul(lo, hi, a->v7, b->v6);
        cc = inner_gf27500_adc(cc, e13, lo, &e13);
        cc = inner_gf27500_adc(cc, e14, hi, &e14);
        cc = inner_gf27500_adc(cc, e15, 0, &e15);
        assert(cc == 0);

        // + 6 mul = 28
        inner_gf27500_umul(lo, hi, a->v0, b->v2);
        cc = inner_gf27500_adc(0, e2, lo, &e2);
        cc = inner_gf27500_adc(cc, e3, hi, &e3);
        inner_gf27500_umul(lo, hi, a->v0, b->v4);
        cc = inner_gf27500_adc(cc, e4, lo, &e4);
        cc = inner_gf27500_adc(cc, e5, hi, &e5);
        inner_gf27500_umul(lo, hi, a->v0, b->v6);
        cc = inner_gf27500_adc(cc, e6, lo, &e6);
        cc = inner_gf27500_adc(cc, e7, hi, &e7);
        inner_gf27500_umul(lo, hi, a->v1, b->v7);
        cc = inner_gf27500_adc(cc, e8, lo, &e8);
        cc = inner_gf27500_adc(cc, e9, hi, &e9);
        inner_gf27500_umul(lo, hi, a->v3, b->v7);
        cc = inner_gf27500_adc(cc, e10, lo, &e10);
        cc = inner_gf27500_adc(cc, e11, hi, &e11);
        inner_gf27500_umul(lo, hi, a->v5, b->v7);
        cc = inner_gf27500_adc(cc, e12, lo, &e12);
        cc = inner_gf27500_adc(cc, e13, hi, &e13);
        cc = inner_gf27500_adc(cc, e14, 0, &e14);
        cc = inner_gf27500_adc(cc, e15, 0, &e15);
        assert(cc == 0);

        // + 6 mul = 34
        inner_gf27500_umul(lo, hi, a->v2, b->v0);
        cc = inner_gf27500_adc(0, e2, lo, &e2);
        cc = inner_gf27500_adc(cc, e3, hi, &e3);
        inner_gf27500_umul(lo, hi, a->v4, b->v0);
        cc = inner_gf27500_adc(cc, e4, lo, &e4);
        cc = inner_gf27500_adc(cc, e5, hi, &e5);
        inner_gf27500_umul(lo, hi, a->v6, b->v0);
        cc = inner_gf27500_adc(cc, e6, lo, &e6);
        cc = inner_gf27500_adc(cc, e7, hi, &e7);
        inner_gf27500_umul(lo, hi, a->v7, b->v1);
        cc = inner_gf27500_adc(cc, e8, lo, &e8);
        cc = inner_gf27500_adc(cc, e9, hi, &e9);
        inner_gf27500_umul(lo, hi, a->v7, b->v3);
        cc = inner_gf27500_adc(cc, e10, lo, &e10);
        cc = inner_gf27500_adc(cc, e11, hi, &e11);
        inner_gf27500_umul(lo, hi, a->v7, b->v5);
        cc = inner_gf27500_adc(cc, e12, lo, &e12);
        cc = inner_gf27500_adc(cc, e13, hi, &e13);
        cc = inner_gf27500_adc(cc, e14, 0, &e14);
        cc = inner_gf27500_adc(cc, e15, 0, &e15);
        assert(cc == 0);

        // + 5 mul = 39
        inner_gf27500_umul(lo, hi, a->v1, b->v2);
        cc = inner_gf27500_adc(cc, e3, lo, &e3);
        cc = inner_gf27500_adc(cc, e4, hi, &e4);
        inner_gf27500_umul(lo, hi, a->v1, b->v4);
        cc = inner_gf27500_adc(cc, e5, lo, &e5);
        cc = inner_gf27500_adc(cc, e6, hi, &e6);
        inner_gf27500_umul(lo, hi, a->v1, b->v6);
        cc = inner_gf27500_adc(cc, e7, lo, &e7);
        cc = inner_gf27500_adc(cc, e8, hi, &e8);
        inner_gf27500_umul(lo, hi, a->v3, b->v6);
        cc = inner_gf27500_adc(cc, e9, lo, &e9);
        cc = inner_gf27500_adc(cc, e10, hi, &e10);
        inner_gf27500_umul(lo, hi, a->v5, b->v6);
        cc = inner_gf27500_adc(cc, e11, lo, &e11);
        cc = inner_gf27500_adc(cc, e12, hi, &e12);
        cc = inner_gf27500_adc(cc, e13, 0, &e13);
        cc = inner_gf27500_adc(cc, e14, 0, &e14);
        cc = inner_gf27500_adc(cc, e15, 0, &e15);
        assert(cc == 0);

        // + 5 mul = 44
        inner_gf27500_umul(lo, hi, a->v2, b->v1);
        cc = inner_gf27500_adc(cc, e3, lo, &e3);
        cc = inner_gf27500_adc(cc, e4, hi, &e4);
        inner_gf27500_umul(lo, hi, a->v4, b->v1);
        cc = inner_gf27500_adc(cc, e5, lo, &e5);
        cc = inner_gf27500_adc(cc, e6, hi, &e6);
        inner_gf27500_umul(lo, hi, a->v6, b->v1);
        cc = inner_gf27500_adc(cc, e7, lo, &e7);
        cc = inner_gf27500_adc(cc, e8, hi, &e8);
        inner_gf27500_umul(lo, hi, a->v6, b->v3);
        cc = inner_gf27500_adc(cc, e9, lo, &e9);
        cc = inner_gf27500_adc(cc, e10, hi, &e10);
        inner_gf27500_umul(lo, hi, a->v6, b->v5);
        cc = inner_gf27500_adc(cc, e11, lo, &e11);
        cc = inner_gf27500_adc(cc, e12, hi, &e12);
        cc = inner_gf27500_adc(cc, e13, 0, &e13);
        cc = inner_gf27500_adc(cc, e14, 0, &e14);
        cc = inner_gf27500_adc(cc, e15, 0, &e15);
        assert(cc == 0);

        // + 4 mul = 48
        inner_gf27500_umul(lo, hi, a->v1, b->v3);
        cc = inner_gf27500_adc(cc, e4, lo, &e4);
        cc = inner_gf27500_adc(cc, e5, hi, &e5);
        inner_gf27500_umul(lo, hi, a->v1, b->v5);
        cc = inner_gf27500_adc(cc, e6, lo, &e6);
        cc = inner_gf27500_adc(cc, e7, hi, &e7);
        inner_gf27500_umul(lo, hi, a->v3, b->v5);
        cc = inner_gf27500_adc(cc, e8, lo, &e8);
        cc = inner_gf27500_adc(cc, e9, hi, &e9);
        inner_gf27500_umul(lo, hi, a->v4, b->v6);
        cc = inner_gf27500_adc(cc, e10, lo, &e10);
        cc = inner_gf27500_adc(cc, e11, hi, &e11);
        cc = inner_gf27500_adc(cc, e12, 0, &e12);
        cc = inner_gf27500_adc(cc, e13, 0, &e13);
        cc = inner_gf27500_adc(cc, e14, 0, &e14);
        cc = inner_gf27500_adc(cc, e15, 0, &e15);
        assert(cc == 0);

        // + 4 mul = 52
        inner_gf27500_umul(lo, hi, a->v3, b->v1);
        cc = inner_gf27500_adc(cc, e4, lo, &e4);
        cc = inner_gf27500_adc(cc, e5, hi, &e5);
        inner_gf27500_umul(lo, hi, a->v5, b->v1);
        cc = inner_gf27500_adc(cc, e6, lo, &e6);
        cc = inner_gf27500_adc(cc, e7, hi, &e7);
        inner_gf27500_umul(lo, hi, a->v5, b->v3);
        cc = inner_gf27500_adc(cc, e8, lo, &e8);
        cc = inner_gf27500_adc(cc, e9, hi, &e9);
        inner_gf27500_umul(lo, hi, a->v6, b->v4);
        cc = inner_gf27500_adc(cc, e10, lo, &e10);
        cc = inner_gf27500_adc(cc, e11, hi, &e11);
        cc = inner_gf27500_adc(cc, e12, 0, &e12);
        cc = inner_gf27500_adc(cc, e13, 0, &e13);
        cc = inner_gf27500_adc(cc, e14, 0, &e14);
        cc = inner_gf27500_adc(cc, e15, 0, &e15);
        assert(cc == 0);

        // + 3 mul = 55
        inner_gf27500_umul(lo, hi, a->v2, b->v3);
        cc = inner_gf27500_adc(cc, e5, lo, &e5);
        cc = inner_gf27500_adc(cc, e6, hi, &e6);
        inner_gf27500_umul(lo, hi, a->v2, b->v5);
        cc = inner_gf27500_adc(cc, e7, lo, &e7);
        cc = inner_gf27500_adc(cc, e8, hi, &e8);
        inner_gf27500_umul(lo, hi, a->v4, b->v5);
        cc = inner_gf27500_adc(cc, e9, lo, &e9);
        cc = inner_gf27500_adc(cc, e10, hi, &e10);
        cc = inner_gf27500_adc(cc, e11, 0, &e11);
        cc = inner_gf27500_adc(cc, e12, 0, &e12);
        cc = inner_gf27500_adc(cc, e13, 0, &e13);
        cc = inner_gf27500_adc(cc, e14, 0, &e14);
        cc = inner_gf27500_adc(cc, e15, 0, &e15);
        assert(cc == 0);

        // + 3 mul = 58
        inner_gf27500_umul(lo, hi, a->v3, b->v2);
        cc = inner_gf27500_adc(cc, e5, lo, &e5);
        cc = inner_gf27500_adc(cc, e6, hi, &e6);
        inner_gf27500_umul(lo, hi, a->v5, b->v2);
        cc = inner_gf27500_adc(cc, e7, lo, &e7);
        cc = inner_gf27500_adc(cc, e8, hi, &e8);
        inner_gf27500_umul(lo, hi, a->v5, b->v4);
        cc = inner_gf27500_adc(cc, e9, lo, &e9);
        cc = inner_gf27500_adc(cc, e10, hi, &e10);
        cc = inner_gf27500_adc(cc, e11, 0, &e11);
        cc = inner_gf27500_adc(cc, e12, 0, &e12);
        cc = inner_gf27500_adc(cc, e13, 0, &e13);
        cc = inner_gf27500_adc(cc, e14, 0, &e14);
        cc = inner_gf27500_adc(cc, e15, 0, &e15);
        assert(cc == 0);

        // + 2 mul = 60
        inner_gf27500_umul(lo, hi, a->v2, b->v4);
        cc = inner_gf27500_adc(cc, e6, lo, &e6);
        cc = inner_gf27500_adc(cc, e7, hi, &e7);
        inner_gf27500_umul(lo, hi, a->v2, b->v6);
        cc = inner_gf27500_adc(cc, e8, lo, &e8);
        cc = inner_gf27500_adc(cc, e9, hi, &e9);
        cc = inner_gf27500_adc(cc, e10, 0, &e10);
        cc = inner_gf27500_adc(cc, e11, 0, &e11);
        cc = inner_gf27500_adc(cc, e12, 0, &e12);
        cc = inner_gf27500_adc(cc, e13, 0, &e13);
        cc = inner_gf27500_adc(cc, e14, 0, &e14);
        cc = inner_gf27500_adc(cc, e15, 0, &e15);
        assert(cc == 0);

        // + 2 mul = 62
        inner_gf27500_umul(lo, hi, a->v4, b->v2);
        cc = inner_gf27500_adc(cc, e6, lo, &e6);
        cc = inner_gf27500_adc(cc, e7, hi, &e7);
        inner_gf27500_umul(lo, hi, a->v6, b->v2);
        cc = inner_gf27500_adc(cc, e8, lo, &e8);
        cc = inner_gf27500_adc(cc, e9, hi, &e9);
        cc = inner_gf27500_adc(cc, e10, 0, &e10);
        cc = inner_gf27500_adc(cc, e11, 0, &e11);
        cc = inner_gf27500_adc(cc, e12, 0, &e12);
        cc = inner_gf27500_adc(cc, e13, 0, &e13);
        cc = inner_gf27500_adc(cc, e14, 0, &e14);
        cc = inner_gf27500_adc(cc, e15, 0, &e15);
        assert(cc == 0);

        // + 2 mul = 64
        inner_gf27500_umul(lo, hi, a->v3, b->v4);
        inner_gf27500_umul(lo2, hi2, a->v4, b->v3);
        cc = inner_gf27500_adc(0, lo, lo2, &lo);
        cc = inner_gf27500_adc(cc, hi, hi2, &hi);
        cc = inner_gf27500_adc(cc, 0, 0, &hi2);
        assert(cc == 0);
        cc = inner_gf27500_adc(0, e7, lo, &e7);
        cc = inner_gf27500_adc(cc, e8, hi, &e8);
        cc = inner_gf27500_adc(cc, e9, hi2, &e9);
        cc = inner_gf27500_adc(cc, e10, 0, &e10);
        cc = inner_gf27500_adc(cc, e11, 0, &e11);
        cc = inner_gf27500_adc(cc, e12, 0, &e12);
        cc = inner_gf27500_adc(cc, e13, 0, &e13);
        cc = inner_gf27500_adc(cc, e14, 0, &e14);
        cc = inner_gf27500_adc(cc, e15, 0, &e15);
        assert(cc == 0);

        // Montgomery reduction.
        //
        // Low part is lo(e) = e0..e7 (512 bits).
        // Let m = -1/q mod 2^512; we add (lo(e)*m mod 2^512)*q to the
        // high part g = e8..e15 (498 bits).
        //
        // We have m = 27*2^500 + 1.
        f0 = e0;
        f1 = e1;
        f2 = e2;
        f3 = e3;
        f4 = e4;
        f5 = e5;
        f6 = e6;
        f7 = e7 + ((e0 * 27) << 52);

        // g = f*q
        inner_gf27500_umul(g7, hi, f0, (uint64_t)27 << 52);
        inner_gf27500_umul_add(g8, hi, f1, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g9, hi, f2, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g10, hi, f3, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g11, hi, f4, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g12, hi, f5, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g13, hi, f6, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g14, g15, f7, (uint64_t)27 << 52, hi);

        cc = inner_gf27500_sbb(0, 0, f0, &g0);
        cc = inner_gf27500_sbb(cc, 0, f1, &g1);
        cc = inner_gf27500_sbb(cc, 0, f2, &g2);
        cc = inner_gf27500_sbb(cc, 0, f3, &g3);
        cc = inner_gf27500_sbb(cc, 0, f4, &g4);
        cc = inner_gf27500_sbb(cc, 0, f5, &g5);
        cc = inner_gf27500_sbb(cc, 0, f6, &g6);
        cc = inner_gf27500_sbb(cc, g7, f7, &g7);
        cc = inner_gf27500_sbb(cc, g8, 0, &g8);
        cc = inner_gf27500_sbb(cc, g9, 0, &g9);
        cc = inner_gf27500_sbb(cc, g10, 0, &g10);
        cc = inner_gf27500_sbb(cc, g11, 0, &g11);
        cc = inner_gf27500_sbb(cc, g12, 0, &g12);
        cc = inner_gf27500_sbb(cc, g13, 0, &g13);
        cc = inner_gf27500_sbb(cc, g14, 0, &g14);
        (void)inner_gf27500_sbb(cc, g15, 0, &g15);

        // Add g = f*q to e0..e11.
        // Since e0..e15 < 2^1010 and f < 2^512, we know that the result
        // is less than 2^1010 + 2^512*27*2^500, which is less than 2^1017.
        // This is also a multiple of 2^512. We divide by 2^512 by simply
        // dropping the low 512 bits (which are all equal to zero), and
        // the result is less than 2**505
        cc = inner_gf27500_adc(0, g0, e0, &e0);
        cc = inner_gf27500_adc(cc, g1, e1, &e1);
        cc = inner_gf27500_adc(cc, g2, e2, &e2);
        cc = inner_gf27500_adc(cc, g3, e3, &e3);
        cc = inner_gf27500_adc(cc, g4, e4, &e4);
        cc = inner_gf27500_adc(cc, g5, e5, &e5);
        cc = inner_gf27500_adc(cc, g6, e6, &e6);
        cc = inner_gf27500_adc(cc, g7, e7, &e7);
        cc = inner_gf27500_adc(cc, g8, e8, &e8);
        cc = inner_gf27500_adc(cc, g9, e9, &e9);
        cc = inner_gf27500_adc(cc, g10, e10, &e10);
        cc = inner_gf27500_adc(cc, g11, e11, &e11);
        cc = inner_gf27500_adc(cc, g12, e12, &e12);
        cc = inner_gf27500_adc(cc, g13, e13, &e13);
        cc = inner_gf27500_adc(cc, g14, e14, &e14);
        cc = inner_gf27500_adc(cc, g15, e15, &e15);
        assert(cc == 0);

        d->v0 = e8;
        d->v1 = e9;
        d->v2 = e10;
        d->v3 = e11;
        d->v4 = e12;
        d->v5 = e13;
        d->v6 = e14;
        d->v7 = e15;
    }

    /*
     * d <- a^2
     */
    static inline void
    gf27500_square(gf27500 *d, const gf27500 *a)
    {
        uint64_t e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15;
        uint64_t f0, f1, f2, f3, f4, f5, f6, f7;
        uint64_t lo, hi;
        uint64_t g0, g1, g2, g3, g4, g5, g6, g7, g8, g9, g10, g11, g12, g13, g14, g15;
        unsigned char cc;

        // Squaring over integers.
        // 7 mul
        inner_gf27500_umul(e1, e2, a->v0, a->v1);
        inner_gf27500_umul(e3, e4, a->v0, a->v3);
        inner_gf27500_umul(e5, e6, a->v0, a->v5);
        inner_gf27500_umul(e7, e8, a->v0, a->v7);
        inner_gf27500_umul(e9, e10, a->v2, a->v7);
        inner_gf27500_umul(e11, e12, a->v4, a->v7);
        inner_gf27500_umul(e13, e14, a->v6, a->v7);

        // + 6 = 13 mul
        inner_gf27500_umul(lo, hi, a->v0, a->v2);
        cc = inner_gf27500_adc(0, e2, lo, &e2);
        cc = inner_gf27500_adc(cc, e3, hi, &e3);
        inner_gf27500_umul(lo, hi, a->v0, a->v4);
        cc = inner_gf27500_adc(cc, e4, lo, &e4);
        cc = inner_gf27500_adc(cc, e5, hi, &e5);
        inner_gf27500_umul(lo, hi, a->v0, a->v6);
        cc = inner_gf27500_adc(cc, e6, lo, &e6);
        cc = inner_gf27500_adc(cc, e7, hi, &e7);
        inner_gf27500_umul(lo, hi, a->v1, a->v7);
        cc = inner_gf27500_adc(cc, e8, lo, &e8);
        cc = inner_gf27500_adc(cc, e9, hi, &e9);
        inner_gf27500_umul(lo, hi, a->v3, a->v7);
        cc = inner_gf27500_adc(cc, e10, lo, &e10);
        cc = inner_gf27500_adc(cc, e11, hi, &e11);
        inner_gf27500_umul(lo, hi, a->v5, a->v7);
        cc = inner_gf27500_adc(cc, e12, lo, &e12);
        cc = inner_gf27500_adc(cc, e13, hi, &e13);
        (void)inner_gf27500_adc(cc, e14, 0, &e14);

        // + 5 = 18 mul
        inner_gf27500_umul(lo, hi, a->v1, a->v2);
        cc = inner_gf27500_adc(0, e3, lo, &e3);
        cc = inner_gf27500_adc(cc, e4, hi, &e4);
        inner_gf27500_umul(lo, hi, a->v1, a->v4);
        cc = inner_gf27500_adc(cc, e5, lo, &e5);
        cc = inner_gf27500_adc(cc, e6, hi, &e6);
        inner_gf27500_umul(lo, hi, a->v1, a->v6);
        cc = inner_gf27500_adc(cc, e7, lo, &e7);
        cc = inner_gf27500_adc(cc, e8, hi, &e8);
        inner_gf27500_umul(lo, hi, a->v3, a->v6);
        cc = inner_gf27500_adc(cc, e9, lo, &e9);
        cc = inner_gf27500_adc(cc, e10, hi, &e10);
        inner_gf27500_umul(lo, hi, a->v5, a->v6);
        cc = inner_gf27500_adc(cc, e11, lo, &e11);
        cc = inner_gf27500_adc(cc, e12, hi, &e12);
        (void)inner_gf27500_adc(cc, e13, 0, &e13);

        // + 4 = 22 mul
        inner_gf27500_umul(lo, hi, a->v1, a->v3);
        cc = inner_gf27500_adc(0, e4, lo, &e4);
        cc = inner_gf27500_adc(cc, e5, hi, &e5);
        inner_gf27500_umul(lo, hi, a->v1, a->v5);
        cc = inner_gf27500_adc(cc, e6, lo, &e6);
        cc = inner_gf27500_adc(cc, e7, hi, &e7);
        inner_gf27500_umul(lo, hi, a->v2, a->v6);
        cc = inner_gf27500_adc(cc, e8, lo, &e8);
        cc = inner_gf27500_adc(cc, e9, hi, &e9);
        inner_gf27500_umul(lo, hi, a->v4, a->v6);
        cc = inner_gf27500_adc(cc, e10, lo, &e10);
        cc = inner_gf27500_adc(cc, e11, hi, &e11);
        (void)inner_gf27500_adc(cc, e12, 0, &e12);

        // + 3 = 25 mul
        inner_gf27500_umul(lo, hi, a->v2, a->v3);
        cc = inner_gf27500_adc(0, e5, lo, &e5);
        cc = inner_gf27500_adc(cc, e6, hi, &e6);
        inner_gf27500_umul(lo, hi, a->v2, a->v5);
        cc = inner_gf27500_adc(cc, e7, lo, &e7);
        cc = inner_gf27500_adc(cc, e8, hi, &e8);
        inner_gf27500_umul(lo, hi, a->v4, a->v5);
        cc = inner_gf27500_adc(cc, e9, lo, &e9);
        cc = inner_gf27500_adc(cc, e10, hi, &e10);
        (void)inner_gf27500_adc(cc, e11, 0, &e11);

        // + 2 = 27 mul
        inner_gf27500_umul(lo, hi, a->v2, a->v4);
        cc = inner_gf27500_adc(0, e6, lo, &e6);
        cc = inner_gf27500_adc(cc, e7, hi, &e7);
        inner_gf27500_umul(lo, hi, a->v3, a->v5);
        cc = inner_gf27500_adc(cc, e8, lo, &e8);
        cc = inner_gf27500_adc(cc, e9, hi, &e9);
        (void)inner_gf27500_adc(cc, e10, 0, &e10);

        // + 1 = 28 mul
        inner_gf27500_umul(lo, hi, a->v3, a->v4);
        cc = inner_gf27500_adc(0, e7, lo, &e7);
        cc = inner_gf27500_adc(cc, e8, hi, &e8);
        (void)inner_gf27500_adc(cc, e9, 0, &e9);

        // Multiplication by two for all off diagonal
        // terms
        e15 = e14 >> 63;
        e14 = (e14 << 1) | (e13 >> 63);
        e13 = (e13 << 1) | (e12 >> 63);
        e12 = (e12 << 1) | (e11 >> 63);
        e11 = (e11 << 1) | (e10 >> 63);
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

        // + 8 = 36 mul (diagonal terms)
        inner_gf27500_umul(e0, hi, a->v0, a->v0);
        cc = inner_gf27500_adc(0, e1, hi, &e1);
        inner_gf27500_umul(lo, hi, a->v1, a->v1);
        cc = inner_gf27500_adc(cc, e2, lo, &e2);
        cc = inner_gf27500_adc(cc, e3, hi, &e3);
        inner_gf27500_umul(lo, hi, a->v2, a->v2);
        cc = inner_gf27500_adc(cc, e4, lo, &e4);
        cc = inner_gf27500_adc(cc, e5, hi, &e5);
        inner_gf27500_umul(lo, hi, a->v3, a->v3);
        cc = inner_gf27500_adc(cc, e6, lo, &e6);
        cc = inner_gf27500_adc(cc, e7, hi, &e7);
        inner_gf27500_umul(lo, hi, a->v4, a->v4);
        cc = inner_gf27500_adc(cc, e8, lo, &e8);
        cc = inner_gf27500_adc(cc, e9, hi, &e9);
        inner_gf27500_umul(lo, hi, a->v5, a->v5);
        cc = inner_gf27500_adc(cc, e10, lo, &e10);
        cc = inner_gf27500_adc(cc, e11, hi, &e11);
        inner_gf27500_umul(lo, hi, a->v6, a->v6);
        cc = inner_gf27500_adc(cc, e12, lo, &e12);
        cc = inner_gf27500_adc(cc, e13, hi, &e13);
        inner_gf27500_umul(lo, hi, a->v7, a->v7);
        cc = inner_gf27500_adc(cc, e14, lo, &e14);
        (void)inner_gf27500_adc(cc, e15, hi, &e15);

        // Montgomery reduction.
        //
        // Low part is lo(e) = e0..e7 (512 bits).
        // Let m = -1/q mod 2^512; we add (lo(e)*m mod 2^512)*q to the
        // high part g = e8..e15 (498 bits).
        //
        // We have m = 27*2^500 + 1.
        f0 = e0;
        f1 = e1;
        f2 = e2;
        f3 = e3;
        f4 = e4;
        f5 = e5;
        f6 = e6;
        f7 = e7 + ((e0 * 27) << 52);

        // g = f*q
        inner_gf27500_umul(g7, hi, f0, (uint64_t)27 << 52);
        inner_gf27500_umul_add(g8, hi, f1, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g9, hi, f2, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g10, hi, f3, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g11, hi, f4, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g12, hi, f5, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g13, hi, f6, (uint64_t)27 << 52, hi);
        inner_gf27500_umul_add(g14, g15, f7, (uint64_t)27 << 52, hi);

        cc = inner_gf27500_sbb(0, 0, f0, &g0);
        cc = inner_gf27500_sbb(cc, 0, f1, &g1);
        cc = inner_gf27500_sbb(cc, 0, f2, &g2);
        cc = inner_gf27500_sbb(cc, 0, f3, &g3);
        cc = inner_gf27500_sbb(cc, 0, f4, &g4);
        cc = inner_gf27500_sbb(cc, 0, f5, &g5);
        cc = inner_gf27500_sbb(cc, 0, f6, &g6);
        cc = inner_gf27500_sbb(cc, g7, f7, &g7);
        cc = inner_gf27500_sbb(cc, g8, 0, &g8);
        cc = inner_gf27500_sbb(cc, g9, 0, &g9);
        cc = inner_gf27500_sbb(cc, g10, 0, &g10);
        cc = inner_gf27500_sbb(cc, g11, 0, &g11);
        cc = inner_gf27500_sbb(cc, g12, 0, &g12);
        cc = inner_gf27500_sbb(cc, g13, 0, &g13);
        cc = inner_gf27500_sbb(cc, g14, 0, &g14);
        (void)inner_gf27500_sbb(cc, g15, 0, &g15);

        // Add g = f*q to e0..e11.
        // Since e0..e15 < 2^1010 and f < 2^512, we know that the result
        // is less than 2^1010 + 2^512*27*2^500, which is less than 2^1017.
        // This is also a multiple of 2^512. We divide by 2^512 by simply
        // dropping the low 512 bits (which are all equal to zero), and
        // the result is less than 2**505
        cc = inner_gf27500_adc(0, g0, e0, &e0);
        cc = inner_gf27500_adc(cc, g1, e1, &e1);
        cc = inner_gf27500_adc(cc, g2, e2, &e2);
        cc = inner_gf27500_adc(cc, g3, e3, &e3);
        cc = inner_gf27500_adc(cc, g4, e4, &e4);
        cc = inner_gf27500_adc(cc, g5, e5, &e5);
        cc = inner_gf27500_adc(cc, g6, e6, &e6);
        cc = inner_gf27500_adc(cc, g7, e7, &e7);
        cc = inner_gf27500_adc(cc, g8, e8, &e8);
        cc = inner_gf27500_adc(cc, g9, e9, &e9);
        cc = inner_gf27500_adc(cc, g10, e10, &e10);
        cc = inner_gf27500_adc(cc, g11, e11, &e11);
        cc = inner_gf27500_adc(cc, g12, e12, &e12);
        cc = inner_gf27500_adc(cc, g13, e13, &e13);
        cc = inner_gf27500_adc(cc, g14, e14, &e14);
        cc = inner_gf27500_adc(cc, g15, e15, &e15);
        assert(cc == 0);

        d->v0 = e8;
        d->v1 = e9;
        d->v2 = e10;
        d->v3 = e11;
        d->v4 = e12;
        d->v5 = e13;
        d->v6 = e14;
        d->v7 = e15;
    }

    /*
     * d <- a^(2^n)
     * This computes n successive squarings of value a, with result in d.
     * n == 0 is a valid input (in that case, *a is copied into *d).
     * This function is not constant-time with regard to n: the number of
     * successive squarings may be observable through timing-based side channels.
     */
    static inline void
    gf27500_xsquare(gf27500 *d, const gf27500 *a, unsigned n)
    {
        if (n == 0) {
            *d = *a;
            return;
        }
        gf27500_square(d, a);
        while (n-- > 1) {
            gf27500_square(d, d);
        }
    }

    /*
     * Returns 0xFFFFFFFF if *a is zero; otherwise, 0x00000000 is returned.
     */
    static inline uint32_t
    gf27500_iszero(const gf27500 *a)
    {
        uint64_t a0, a1, a2, a3, a4, a5, a6, a7, t0, t1, r;

        // Zero can be represented by 0 or by q.
        a0 = a->v0;
        a1 = a->v1;
        a2 = a->v2;
        a3 = a->v3;
        a4 = a->v4;
        a5 = a->v5;
        a6 = a->v6;
        a7 = a->v7;

        t0 = a0 | a1 | a2 | a3 | a4 | a5 | a6 | a7;
        t1 = ~a0 | ~a1 | ~a2 | ~a3 | ~a4 | ~a5 | ~a6 | (a7 ^ 0x01AFFFFFFFFFFFFF);

        // Top bit of r is 0 if and only if one of t0 or t1 is zero.
        r = (t0 | -t0) & (t1 | -t1);
        return (uint32_t)(r >> 63) - 1;
    }

    /*
     * Returns 0xFFFFFFFF if *a and *b represent the same field element;
     * otherwise, 0x00000000 is returned.
     */
    static inline uint32_t
    gf27500_equals(const gf27500 *a, const gf27500 *b)
    {
        gf27500 d;
        gf27500_sub(&d, a, b);
        return gf27500_iszero(&d);
    }

    /*
     * d <- 1/a
     * If *a is not zero, then the inverse is well-defined and written into *d,
     * and the function returns 0xFFFFFFFF. If *a is zero, then this function
     * sets *d to zero and returns 0x00000000.
     */
    uint32_t gf27500_invert(gf27500 *d, const gf27500 *a);

    /*
     * d <- a/b
     * If *b is not zero, then this functions writes a/b into *d, and returns
     * 0xFFFFFFFF. If *b is zero, then this function sets *d to zero (regardless
     * of the value of *a) and returns 0x00000000.
     */
    uint32_t gf27500_div(gf27500 *d, const gf27500 *a, const gf27500 *b);

    /*
     * d <- a/3
     * Divides by 3 in the field by implementing the algorithm proposed in
     * "Efficient Multiplication in Finite Field Extensions of Degree 5"
     * by El Mrabet, Guillevic and Ionica at ASIACRYPT 2011.
     */
    void gf27500_div3(gf27500 *out, const gf27500 *a);

    /*
     * Get the Legendre symbol of *a (0 for zero, +1 for a non-zero square,
     * -1 for a non-square).
     */
    int32_t gf27500_legendre(const gf27500 *a);

    /*
     * If *a is a square, then this function sets *d to a square root of a,
     * and returns 0xFFFFFFFF. If *a is not a square, then this function
     * sets *d to a square root of -a, and returns 0x00000000.
     * In all cases, the value written into *d is such that the least significant
     * bit of its integer representation (in [0..q-1]) is zero.
     */
    uint32_t gf27500_sqrt(gf27500 *d, const gf27500 *a);

    /*
     * Encode field element *a into buffer dst (exactly 64 bytes are written).
     */
    void gf27500_encode(void *dst, const gf27500 *a);

    /*
     * Decode source buffer src (exactly 64 bytes) into a field element *d.
     * If the source value is not a valid canonical encoding, then *d is zero
     * and the function returns 0x00000000; otherwise, the function returns
     * 0xFFFFFFFF.
     */
    uint32_t gf27500_decode(gf27500 *d, const void *src);

    /*
     * Interpret the source buffer (of size len bytes) as an unsigned integer
     * (little-endian convention) and reduce it modulo q, yielding a field
     * element which is written into *d. Since reduction is applied, this
     * function cannot fail.
     */
    void gf27500_decode_reduce(gf27500 *d, const void *src, size_t len);

#ifdef __cplusplus
}
#endif

#endif
