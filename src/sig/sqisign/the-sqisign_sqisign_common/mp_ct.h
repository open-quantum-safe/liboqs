/** @file
 *
 * @brief Branch-free machine-word helpers shared by mp's constant-time entry points.
 * Constant-time behaviour is claimed at RADIX_64; RADIX_32 is supported for correctness, and in
 * particular ct_limb_bit_length's 64-bit clz may lower to a branch or a libgcc call on a 32-bit
 * target.
 */

#ifndef MP_CT_H
#define MP_CT_H

#include <stdint.h>
#include <stddef.h>
#include <tutil.h>

// --- optimisation barriers
// The point of a barrier is to stop the compiler proving something about a value and turning a
// masked select back into a branch.
#if defined(__GNUC__) || defined(__clang__)
#define CT_BARRIER_BODY(T, x)                                                                                          \
    do {                                                                                                               \
        __asm__ volatile("" : "+r"(x));                                                                                \
    } while (0)
#else
// more portable version
#define CT_BARRIER_BODY(T, x)                                                                                          \
    do {                                                                                                               \
        volatile T ct_barrier_tmp_ = (x);                                                                              \
        (x) = ct_barrier_tmp_;                                                                                         \
    } while (0)
#endif

static inline uint64_t
ct_barrier_u64(uint64_t x)
{
    CT_BARRIER_BODY(uint64_t, x);
    return x;
}

static inline uint32_t
ct_barrier_u32(uint32_t x)
{
    CT_BARRIER_BODY(uint32_t, x);
    return x;
}

static inline int32_t
ct_barrier_i32(int32_t x)
{
    CT_BARRIER_BODY(int32_t, x);
    return x;
}

static inline digit_t
ct_barrier_digit(digit_t x)
{
    CT_BARRIER_BODY(digit_t, x);
    return x;
}

// --- digit predicates

static inline unsigned int
ct_is_digit_nonzero(digit_t x)
{ // Is x != 0?
    return (unsigned int)((x | (0 - x)) >> (RADIX - 1));
}

static inline unsigned int
ct_is_digit_zero(digit_t x)
{ // Is x == 0?
    return (unsigned int)(1 ^ ct_is_digit_nonzero(x));
}

static inline unsigned int
ct_is_digit_lessthan(digit_t x, digit_t y)
{ // Is x < y?
    return (unsigned int)((x ^ ((x ^ y) | ((x - y) ^ y))) >> (RADIX - 1));
}

// bitlen(x), 0 for x == 0.
static inline size_t
ct_limb_bit_length(digit_t x)
{
    unsigned long long safe_x = (unsigned long long)x | 1;
    size_t leading_zeros = sqisign_clz64(safe_x);
    size_t len = 64 - leading_zeros;
    digit_t nonzero_mask = ct_barrier_digit((digit_t)(-((digit_t)(x != 0))));
    return len & nonzero_mask;
}

static inline unsigned int
ct_is_u64_lessthan(uint64_t x, uint64_t y)
{ // Is x < y? Fixed at 64 bits regardless of RADIX.
    return (unsigned int)((x ^ ((x ^ y) | ((x - y) ^ y))) >> 63);
}

// c <- a if mask == 0, c <- b if mask == ~0. Selects over nwords limbs, branch-free.
static inline void
ct_select_words(digit_t *c, const digit_t *a, const digit_t *b, const digit_t mask, const int nwords)
{
    for (int i = 0; i < nwords; i++) {
        c[i] = ((a[i] ^ b[i]) & mask) ^ a[i];
    }
}

// --- machine-word CT helpers for the constant-time lattice reduction
// Branch-free min/max/select/swap, saturating shifts, integer sqrt and the wide-multiply
// high-half primitives. Word-level only; the ibz_t layer is ibz_ct_* in mp.h.

// Branchless max(e, 0).
static inline int32_t
ct_max0(int32_t e)
{
    uint32_t sign_bit = (uint32_t)e >> 31;
    uint32_t mask_neg = 0U - sign_bit;
    return (int32_t)((uint32_t)e & ~mask_neg);
}

static inline uint64_t
ct_mask(uint64_t condition)
{
    return ct_barrier_u64(-((condition | -condition) >> 63));
}

static inline uint64_t
ct_abs64(int64_t x)
{
    uint64_t ux = (uint64_t)x;
    uint64_t mask = (uint64_t)(x >> 63);
    return (ux ^ mask) - mask;
}

// x >> s, saturating to 0 for s >= 64. A plain `x >> s` is undefined at s >= 64, and clamping s
// to 63 would return x's top bit instead of the 0 the caller means. Branch-free.
static inline uint64_t
ct_shr_sat64(uint64_t x, uint32_t s)
{
    uint64_t in_range = ct_mask((uint64_t)(s < 64));
    return (x >> (s & 63)) & in_range;
}

static inline void
ct_negate_mask_i32_inplace(uint32_t mask, int32_t *x)
{
    uint32_t m = ct_barrier_u32(mask);
    uint32_t ux = (uint32_t)*x;
    *x = (int32_t)((ux ^ m) - m);
}

static inline void
ct_swap_u64(uint64_t mask, uint64_t *a, uint64_t *b)
{
    uint64_t t = (*a ^ *b) & ct_barrier_u64(mask);
    *a ^= t;
    *b ^= t;
}

static inline void
ct_swap_i32(uint32_t mask, int32_t *a, int32_t *b)
{
    int32_t t = (*a ^ *b) & ct_barrier_i32((int32_t)mask);
    *a ^= t;
    *b ^= t;
}

static inline int32_t
ct_select32(int32_t condition, int32_t a, int32_t b)
{
    int32_t mask = ct_barrier_i32(-condition);
    return (a & mask) | (b & ~mask);
}

static inline int32_t
ct_max(int32_t a, int32_t b)
{
    return ct_select32(a > b, a, b);
}

// --- machine-word CT helpers used by the dimension-2 reductions
//
// Constant-time only for RADIX 64.
// Shifts on uint64_t would have to be replaced by helper functions for RADIX 32.

// Branchless min, the sibling of ct_max above.
static inline int32_t
ct_min(int32_t a, int32_t b)
{
    return ct_select32(a < b, a, b);
}

static inline uint32_t
ct_select_u32(uint32_t condition, uint32_t a, uint32_t b)
{
    uint32_t mask = ct_barrier_u32(-condition);
    return (a & mask) | (b & ~mask);
}

static inline uint64_t
ct_select_u64(uint64_t condition, uint64_t a, uint64_t b)
{
    uint64_t mask = ct_barrier_u64(-condition);
    return (a & mask) | (b & ~mask);
}

static inline uint32_t
ct_max_u32(uint32_t a, uint32_t b)
{
    return ct_select_u32(a > b, a, b);
}

static inline uint32_t
ct_min_u32(uint32_t a, uint32_t b)
{
    return ct_select_u32(a < b, a, b);
}

static inline uint64_t
ct_max_u64(uint64_t a, uint64_t b)
{
    return ct_select_u64(a > b, a, b);
}

static inline void
ct_swap_i64(uint64_t mask, int64_t *a, int64_t *b)
{
    int64_t t = (*a ^ *b) & (int64_t)ct_barrier_u64(mask);
    *a ^= t;
    *b ^= t;
}

// *abs_val = |scalar|, *sign = 1 iff scalar < 0. Machine word only.
static inline void
ct_abs_sign_i64(int64_t scalar, uint64_t *abs_val, uint64_t *sign)
{
    uint64_t u = (uint64_t)scalar;
    uint64_t mask = (uint64_t)(scalar >> 63);
    *abs_val = (u ^ mask) - mask;
    *sign = mask & 1;
}

// floor(sqrt(n)) for a 32-bit n, by a fixed 16-step masked binary search.
static inline uint64_t
ct_isqrt_32(uint32_t n)
{
    uint32_t res = 0;
    for (int bit = 15; bit >= 0; bit--) {
        uint32_t temp = res | (1ULL << bit);
        uint32_t mask = ct_barrier_u32(-((uint32_t)(temp * temp <= n)));
        res = (mask & temp) | (~mask & res);
    }
    return res;
}

// High 64 bits of the signed 128-bit product a*b, from four 32x32 cross-products.
static inline int64_t
ct_mul_high_halves(int64_t a, int64_t b)
{
    uint64_t ua = (uint64_t)a;
    uint64_t ub = (uint64_t)b;

    uint32_t ah = (uint32_t)(ua >> 32);
    uint32_t al = (uint32_t)ua;
    uint32_t bh = (uint32_t)(ub >> 32);
    uint32_t bl = (uint32_t)ub;

    uint64_t p00 = (uint64_t)al * bl;
    uint64_t p01 = (uint64_t)al * bh;
    uint64_t p10 = (uint64_t)ah * bl;
    uint64_t p11 = (uint64_t)ah * bh;

    // Cannot overflow 64 bits.
    uint64_t mid_sum = (p00 >> 32) + (uint64_t)(uint32_t)p01 + (uint64_t)(uint32_t)p10;
    uint64_t res_high = p11 + (p01 >> 32) + (p10 >> 32) + (mid_sum >> 32);

    // Signed correction: subtract the other operand once per negative operand.
    uint64_t mask_a = -(ua >> 63);
    uint64_t mask_b = -(ub >> 63);
    res_high -= (mask_a & ub);
    res_high -= (mask_b & ua);

    return (int64_t)res_high;
}

// High 64 bits of a*a. Same construction as ct_mul_high_halves with the cross-terms folded.
static inline uint64_t
ct_sqr_high_halves(int64_t a)
{
    uint64_t ua = (uint64_t)a;
    uint32_t ah = (uint32_t)(ua >> 32);
    uint32_t al = (uint32_t)ua;

    uint64_t p00 = (uint64_t)al * al;
    uint64_t p01 = (uint64_t)ah * al;
    uint64_t p11 = (uint64_t)ah * ah;
    uint64_t mid_sum = (p00 >> 32) + ((uint64_t)(uint32_t)p01 << 1);
    uint64_t res_high = p11 + ((p01 >> 32) << 1) + (mid_sum >> 32);
    uint64_t mask_a = -(ua >> 63);
    res_high -= (mask_a & (ua << 1));

    return res_high;
}

// Following sqisign choices: RADIX_64 is only ever built by a compiler with __int128,
// and RADIX_32 only ever for a 32-bit target
#if defined(RADIX_64)
__extension__ typedef __int128 ct_int128_t;
#endif

static inline int64_t
ct_mul_high(int64_t a, int64_t b)
{
#if defined(RADIX_64)
    return (int64_t)(((ct_int128_t)a * (ct_int128_t)b) >> 64);
#else
    return ct_mul_high_halves(a, b);
#endif
}

static inline uint64_t
ct_sqr_high(int64_t a)
{
#if defined(RADIX_64)
    return (uint64_t)(int64_t)(((ct_int128_t)a * (ct_int128_t)a) >> 64);
#else
    return ct_sqr_high_halves(a);
#endif
}

// floor(X / Y), Y != 0. Restoring division; the borrow formula is exact over the full 64-bit
// range of Y, so no 65th bit is needed. The 128/64 sibling is mp.c's ct_div128_by64.
static inline uint64_t
ct_div_unsigned_nonzero(uint64_t X, uint64_t Y)
{
    uint64_t Q = 0;
    uint64_t R = 0;
    for (int i = 63; i >= 0; i--) {
        R = (R << 1) | ((X >> i) & 1);
        uint64_t diff = R - Y;
        uint64_t borrow = ((~R & Y) | ((~R | Y) & diff)) >> 63;
        uint64_t mask = -(1 - borrow);
        R = (diff & mask) | (R & ~mask);
        Q = (Q << 1) | (1 & mask);
    }
    return Q;
}

#endif /* MP_CT_H */
