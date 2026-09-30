#include "lll_lg2.h"
#include <mp_ct.h>
#include "lll_config.h"
#include <assert.h>

// ==== Part 1: materialise the block Gram from the LDL state

// Local working bound for the untruncated (b_full, c_full) intermediates: D-entries are bound
// FP_E_BITS, |mu| <= ~1/2 at scale P on entry (driver invariant), so b_full ~ Dk/2 and
// c_full ~ Dkp1 + Dk/4 both stay comfortably inside FP_E_BITS with a small margin.
#define LG_LDL_WBITS (FP_E_BITS + 16)

void
quat_lll_materialise_block(quat_lll_gram2_t *out, const ibz_t *Dk, const ibz_t *Dkp1, const ibz_t *mu, int P)
{
    // b_full = mu * D_k
    ibz_t b_full = { 0 };
    ibz_ct_highmul_p(&b_full, mu, Dk, P, LG_LDL_WBITS);

    // c_full = D_{k+1} + mu^2 * D_k = D_{k+1} + mu * b_full
    ibz_t mu_bfull = { 0 }, c_full = { 0 };
    ibz_ct_highmul_p(&mu_bfull, mu, &b_full, P, LG_LDL_WBITS);
    ibz_add_and_set_bound(&c_full, Dkp1, &mu_bfull, LG_LDL_WBITS);

    ibz_t sum_ac = { 0 };
    ibz_add_and_set_bound(&sum_ac, Dk, &c_full, LG_LDL_WBITS + 1);
    int bs = ibz_bitsize_ct(&sum_ac);
    int32_t e = ct_max0((int32_t)(bs - (GRAM_TOT_BITS - 1)));

    const int32_t max_e = LG_LDL_WBITS + 1; // public upper bound on e: e <= sum_ac's bound

    ibz_ct_shift(&out->a, Dk, -e, max_e, GRAM_TOT_BITS);
    ibz_ct_shift(&out->b, &b_full, -e, max_e, GRAM_TOT_BITS);
    ibz_ct_shift(&out->c, &c_full, -e, max_e, GRAM_TOT_BITS);
    out->e = e;

#ifndef NDEBUG
    assert(ibz_is_positive(&out->a) && !ibz_is_zero(&out->a));
    assert(ibz_is_positive(&out->c) && !ibz_is_zero(&out->c));
#endif
}

// ==== Part 2: Lehmer LG Gram Kernel

enum
{
    THRESHOLD_PARAM = 29,                     // Bitsize threshold between Euclid or division step.
    EXTRACT_W_PARAM = 2 * THRESHOLD_PARAM + 3 // Bitsize of extraction window.
};

// PROVEN bound on extra_shift
#define LG_EXTRA_SHIFT_MAX ((GRAM_TOT_BITS - THRESHOLD_PARAM + 2) / 2)
_Static_assert(LG_EXTRA_SHIFT_MAX > 0, "LG_EXTRA_SHIFT_MAX must be positive");

static int
fast_log2_ratio(uint64_t a, uint64_t b, int clz_b)
{
    int clz_a = sqisign_clz64(a | 1);
    uint64_t norm_a = (a << clz_a) >> 2;
    uint64_t norm_b = (b << clz_b) >> 2;
    uint64_t three_b = 3 * norm_b;
    int inc = ((norm_a << 1) >= three_b); // True if a/b >= 1.5 * 2^k_base
    int dec = ((norm_a << 2) < three_b);  // True if a/b < 0.75 * 2^k_base
    return (clz_b - clz_a) + inc - dec;
}

/**
 * partial_gram_msb:
 * Constant-time MSB-based reduction on Gram matrix [[a,b],[b,c]]
 * When swap=0 no swaps are performed and thus acts purely as a size-reduction.
 * assumes bitsize(c)=EXTRACT_W_PARAM, and bitsize(a) > bitsize(c) - THRESHOLD_PARAM = THRESHOLD_PARAM+2
 * Out: M column-based unimodular transform
 */
static int
partial_gram_msb(uint64_t a, int64_t b, uint64_t c, int32_t matrix[2][2], uint64_t swap)
{
    int32_t u00 = 1, u01 = 0;
    int32_t u10 = 0, u11 = 1;
    uint64_t valid_mask = 0xFFFFFFFFFFFFFFFFULL;
    int valid_steps = 0;

    uint64_t global_swap_mask = ct_mask(swap);
    // THRESHOLD_PARAM+1/2 iterations probably sufficient
    for (int i = 0; i < THRESHOLD_PARAM / 2 + 2; i++) {
        // 1. Either is zero
        valid_mask &= ~ct_mask(!a | !c);

        // 2. Conditonal negation of b
        // after: b >= 0
        uint64_t negate_mask = ct_mask(b < 0) & valid_mask;
        uint64_t b_abs = ct_abs64(b);
        b = b_abs;
        ct_negate_mask_i32_inplace(negate_mask, &u00);
        ct_negate_mask_i32_inplace(negate_mask, &u10);

        // 2. Conditional swap on length
        // after: a <= c
        uint64_t swap_mask = ct_mask(c < a) & valid_mask;
        valid_mask &= ~swap_mask | global_swap_mask;
        uint64_t actual_swap = swap_mask & global_swap_mask;
        ct_swap_u64(actual_swap, &a, &c);
        ct_swap_i32(actual_swap, &u00, &u01);
        ct_swap_i32(actual_swap, &u10, &u11);

        // 3. Global stopping condition
        // could be wrong when large_diff as a,b are shifted by 2^2k, 2^k resp.
        valid_mask &= ~ct_mask(2 * b_abs <= a);

        // 4. Local stopping condition
        uint32_t clz_a = sqisign_clz64(a | 1);
        uint32_t clz_c = sqisign_clz64(c | 1);
        valid_mask &= ~ct_mask((clz_a >= (64 - EXTRACT_W_PARAM + THRESHOLD_PARAM)) |
                               (clz_c >= (64 - EXTRACT_W_PARAM + THRESHOLD_PARAM)));

        // 5. Shift calculation
        // k such that $|b/a - 2^k|$ is minimal.
        // prevent k < 0 in swap=0 edge case (for defined b << k).
        int32_t k = fast_log2_ratio(b_abs, a, clz_a);
        k = ct_max(0, k);

        // 6. Update remainder and matrix
        c -= ((int64_t)((b_abs << (k + 1)) - (int64_t)(a << (2 * k))) & valid_mask);
        b -= (a << k) & valid_mask;
        u01 -= (int32_t)(((uint32_t)u00 << k) & ((uint32_t)valid_mask));
        u11 -= (int32_t)(((uint32_t)u10 << k) & ((uint32_t)valid_mask));

        // Statistics
        valid_steps += (int)(valid_mask & 1);
    }

    matrix[0][0] = u00;
    matrix[0][1] = u01;
    matrix[1][0] = u10;
    matrix[1][1] = u11;
    return valid_steps;
}

static void
apply_matrix_2x2_shift(ibz_t *a, ibz_t *b, ibz_t *c, int32_t M[2][2], int32_t extra_shift, int wbits)
{
    int32_t m00 = M[0][0], m01 = M[0][1], m10 = M[1][0], m11 = M[1][1];

    int32_t c_a0 = m00 * m00;
    int32_t c_a1 = 2 * m00 * m10;
    int32_t c_a2 = m10 * m10;

    int32_t c_b0 = m00 * m01;
    int32_t c_b1 = m00 * m11 + m01 * m10;
    int32_t c_b2 = m10 * m11;

    int32_t c_c0 = m01 * m01;
    int32_t c_c1 = 2 * m01 * m11;
    int32_t c_c2 = m11 * m11;

    ibz_t na = { 0 }, nb = { 0 }, nc = { 0 }, t = { 0 };
    ibz_mul_by_int_and_set_bound(&na, a, c_a0, wbits);
    ibz_mul_by_int_and_set_bound(&t, b, c_a1, wbits);
    ibz_add_and_set_bound(&na, &na, &t, wbits);
    ibz_mul_by_int_and_set_bound(&t, c, c_a2, wbits);
    ibz_add_and_set_bound(&na, &na, &t, wbits);

    ibz_t a_shifted = { 0 };
    ibz_ct_shl(&a_shifted, a, extra_shift, LG_EXTRA_SHIFT_MAX, wbits);
    ibz_t b_shifted = { 0 };
    ibz_ct_shl(&b_shifted, b, extra_shift, LG_EXTRA_SHIFT_MAX, wbits);

    ibz_mul_by_int_and_set_bound(&nb, &a_shifted, c_b0, wbits);
    ibz_mul_by_int_and_set_bound(&t, b, c_b1, wbits);
    ibz_add_and_set_bound(&nb, &nb, &t, wbits);
    ibz_mul_by_int_and_set_bound(&t, c, c_b2, wbits);
    ibz_add_and_set_bound(&nb, &nb, &t, wbits);

    ibz_ct_shl(&a_shifted, a, 2 * extra_shift, 2 * LG_EXTRA_SHIFT_MAX, wbits);
    ibz_mul_by_int_and_set_bound(&nc, &a_shifted, c_c0, wbits);
    ibz_mul_by_int_and_set_bound(&t, &b_shifted, c_c1, wbits);
    ibz_add_and_set_bound(&nc, &nc, &t, wbits);
    ibz_mul_by_int_and_set_bound(&t, c, c_c2, wbits);
    ibz_add_and_set_bound(&nc, &nc, &t, wbits);

    ibz_copy(a, &na);
    ibz_copy(b, &nb);
    ibz_copy(c, &nc);
}

static void
apply_matrix_to_U_shift(ibz_t U[2][2], int32_t M[2][2], int32_t extra_shift)
{
    ibz_t nu00 = { 0 }, nu10 = { 0 }, nu01 = { 0 }, nu11 = { 0 }, t = { 0 };
    ibz_mul_by_int_and_set_bound(&nu00, &U[0][0], M[0][0], FP_U_BITS);
    ibz_mul_by_int_and_set_bound(&t, &U[0][1], M[1][0], FP_U_BITS);
    ibz_add_and_set_bound(&nu00, &nu00, &t, FP_U_BITS);

    ibz_mul_by_int_and_set_bound(&nu10, &U[1][0], M[0][0], FP_U_BITS);
    ibz_mul_by_int_and_set_bound(&t, &U[1][1], M[1][0], FP_U_BITS);
    ibz_add_and_set_bound(&nu10, &nu10, &t, FP_U_BITS);

    ibz_t u_shifted = { 0 };
    ibz_ct_shl(&u_shifted, &U[0][0], extra_shift, LG_EXTRA_SHIFT_MAX, FP_U_BITS);
    ibz_mul_by_int_and_set_bound(&nu01, &u_shifted, M[0][1], FP_U_BITS);
    ibz_mul_by_int_and_set_bound(&t, &U[0][1], M[1][1], FP_U_BITS);
    ibz_add_and_set_bound(&nu01, &nu01, &t, FP_U_BITS);

    ibz_ct_shl(&u_shifted, &U[1][0], extra_shift, LG_EXTRA_SHIFT_MAX, FP_U_BITS);
    ibz_mul_by_int_and_set_bound(&nu11, &u_shifted, M[0][1], FP_U_BITS);
    ibz_mul_by_int_and_set_bound(&t, &U[1][1], M[1][1], FP_U_BITS);
    ibz_add_and_set_bound(&nu11, &nu11, &t, FP_U_BITS);

    ibz_copy(&U[0][0], &nu00);
    ibz_copy(&U[1][0], &nu10);
    ibz_copy(&U[0][1], &nu01);
    ibz_copy(&U[1][1], &nu11);
}

void
quat_lll_gram_lehmer(quat_lll_gram2_t *g, ibz_t U[2][2], int outer_iters)
{
    int32_t M[2][2];

    ibz_set(&U[0][0], 1, 2);
    ibz_set(&U[0][1], 0, 2);
    ibz_set(&U[1][0], 0, 2);
    ibz_set(&U[1][1], 1, 2);
    ibz_set_bound_ct(&U[0][0], FP_U_BITS);
    ibz_set_bound_ct(&U[0][1], FP_U_BITS);
    ibz_set_bound_ct(&U[1][0], FP_U_BITS);
    ibz_set_bound_ct(&U[1][1], FP_U_BITS);

    ibz_set_bound_ct(&g->a, GRAM_WORK_BITS);
    ibz_set_bound_ct(&g->b, GRAM_WORK_BITS);
    ibz_set_bound_ct(&g->c, GRAM_WORK_BITS);

    // Gram Matrix [[A,B],[B,C]]
    for (int i = 0; i < outer_iters; i++) {
        // 1. Swap if C < A
        ibz_t diff;
        ibz_sub(&diff, &g->c, &g->a);
        digit_t borrow = ~ibz_is_positive(&diff);
        ibz_cswap(&g->a, &g->c, borrow);
        ibz_cswap(&U[0][0], &U[0][1], borrow);
        ibz_cswap(&U[1][0], &U[1][1], borrow);

        // 2. Determine shift/MSB extraction
        int32_t bits_a = (int32_t)ibz_bitsize_ct(&g->a);
        int32_t bits_c = (int32_t)ibz_bitsize_ct(&g->c);
        int32_t rho = bits_c - bits_a; // size difference
        int32_t large_diff = (rho >= THRESHOLD_PARAM);
        int32_t bits_from_c = ct_max(bits_c - EXTRACT_W_PARAM, 0);
        int32_t extra_shift = ct_select32(large_diff, (rho - THRESHOLD_PARAM + 2) / 2, 0);
        int32_t bits_from_a = ct_select32(large_diff, bits_from_c - 2 * extra_shift, bits_from_c);
        int32_t bits_from_b = ct_select32(large_diff, bits_from_c - extra_shift, bits_from_c);

        // 3. Extract MSB
        uint64_t pa = ibz_extract_u64(&g->a, bits_from_a);
        int64_t pb = ibz_extract_i64(&g->b, bits_from_b);
        uint64_t pc = ibz_extract_u64(&g->c, bits_from_c);

        // 5. Partial xgcd on MSB's.
        partial_gram_msb(pa, pb, pc, M, !large_diff);

        // 6. Apply transform globally
        apply_matrix_2x2_shift(&g->a, &g->b, &g->c, M, extra_shift, GRAM_WORK_BITS);
        apply_matrix_to_U_shift(U, M, extra_shift);
    }

    ibz_set_bound_ct(&U[0][0], FP_U_BITS);
    ibz_set_bound_ct(&U[0][1], FP_U_BITS);
    ibz_set_bound_ct(&U[1][0], FP_U_BITS);
    ibz_set_bound_ct(&U[1][1], FP_U_BITS);
}
