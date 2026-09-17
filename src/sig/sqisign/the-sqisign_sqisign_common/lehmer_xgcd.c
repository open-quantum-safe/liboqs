#include "lll_config.h"
#include <mp_ct.h>
#include <lll.h>
#include <assert.h>

/**
 * partial_xgcd_msb:
 * Constant-time MSB-based reduction
 */
static int
partial_xgcd_msb(uint64_t a, uint64_t b, uint64_t sqrta, int32_t matrix[2][2], uint64_t swap)
{
    int32_t u00 = 1, u01 = 0;
    int32_t u10 = 0, u11 = 1;
    uint64_t valid_mask = 0xFFFFFFFFFFFFFFFFULL;
    int valid_steps = 0;

    uint64_t global_swap_mask = ct_mask(swap);
    for (int i = 0; i < DIM2_THRESHOLD + 1; i++) {
        // 1. Either is zero
        valid_mask &= ~ct_mask(!a | !b);

        // 2. Conditional swap
        uint64_t swap_mask = ct_mask(a < b) & valid_mask;
        valid_mask &= ~swap_mask | global_swap_mask;
        uint64_t actual_swap = swap_mask & global_swap_mask;
        ct_swap_u64(actual_swap, &a, &b);
        ct_swap_i32((uint32_t)actual_swap, &u00, &u10);
        ct_swap_i32((uint32_t)actual_swap, &u01, &u11);

        // 3. Global stopping condition
        valid_mask &= ~ct_mask(b < sqrta);

        // 4. Local stopping condition
        uint32_t lza = __builtin_clzll(a | 1);
        uint32_t lzb = __builtin_clzll(b | 1);
        valid_mask &= ~ct_mask((lza > DIM2_THRESHOLD) | (lzb > DIM2_THRESHOLD));

        // 5. Shift calculation
        // k = floor(log_2(lza) - log_2(lzb)).
        // prevent k < 0 in swap=0 edge case (for defined b << k).
        uint32_t k = (lzb - lza) & (uint32_t)valid_mask;
        uint64_t b_shifted = b << k;
        k -= (uint32_t)(1 & ct_mask(b_shifted > a));
        k &= (uint32_t)valid_mask;

        // 6. Update remainder and matrix
        a -= (b << k) & valid_mask;
        u00 -= (int32_t)(((uint32_t)u10 << k) & ((uint32_t)valid_mask));
        u01 -= (int32_t)(((uint32_t)u11 << k) & ((uint32_t)valid_mask));

        // Statistics
        valid_steps += (int)(valid_mask & 1);
    }

    matrix[0][0] = u00;
    matrix[0][1] = u01;
    matrix[1][0] = u10;
    matrix[1][1] = u11;
    return valid_steps;
}

// window(x, off) == x / 2^off, for off of either sign: extract at max(off, 0),
// then shift up by max(-off, 0).
static uint64_t
lg_window_u64(const ibz_t *x, int32_t off)
{
    return ibz_extract_u64(x, ct_max0(off)) << ct_max0(-off);
}

static int64_t
lg_window_i64(const ibz_t *x, int32_t off)
{
    return (int64_t)((uint64_t)ibz_extract_i64(x, ct_max0(off)) << ct_max0(-off));
}

// Apply the int32 transform M to the pair (A, B), with an extra left shift of extra_shift on the
// off-diagonal term. extra_shift is SECRET (it is derived from ibz_bitsize_ct), hence the
// shift goes through ibz_ct_shl, never ibz_mul_2exp. det_bits is the PUBLIC
// state bound; see DIM2_VAL_BITS. T1, T2 are caller-provided scratch.
static void
ct_apply_matrix(ibz_t *A, ibz_t *B, int32_t M[2][2], int32_t extra_shift, ibz_t *T1, ibz_t *T2, int det_bits)
{
    const int wbits = DIM2_WORK_BITS(det_bits);
    const int vbits = DIM2_VAL_BITS(det_bits);

    ibz_mul_by_int_and_set_bound(T1, A, M[1][0], wbits);
    ibz_ct_shl(T2, B, extra_shift, DIM2_SHIFT_MAX(det_bits), wbits);
    ibz_mul_by_int_and_set_bound(T2, T2, M[0][1], wbits);
    ibz_mul_by_int_and_set_bound(A, A, M[0][0], wbits);
    ibz_add_and_set_bound(A, A, T2, vbits);
    ibz_mul_by_int_and_set_bound(B, B, M[1][1], wbits);
    ibz_add_and_set_bound(B, B, T1, vbits);
}

// A += mu * (B << extra_shift)
static void
ct_size_reduce(ibz_t *A, ibz_t *B, int32_t mu, int32_t extra_shift, ibz_t *T1, int det_bits)
{
    ibz_ct_shl(T1, B, extra_shift, DIM2_SHIFT_MAX(det_bits), DIM2_WORK_BITS(det_bits));
    ibz_mul_by_int_and_set_bound(T1, T1, mu, DIM2_WORK_BITS(det_bits));
    ibz_add_and_set_bound(A, A, T1, DIM2_VAL_BITS(det_bits));
}

// asserts HNF form, A >= 0, B >= 0, CC = 0, DD > 0, and det = AA*DD <= 2^det_bits
// handles [[A/g,0],[B,g]], where g may now be as large as itself).
static void
partial_xgcd_lehmer(ibz_t *AA, ibz_t *BB, ibz_t *CC, ibz_t *DD, int outer_iters, int det_bits)
{
    int32_t M[2][2];
    ibz_t TT1 = { 0 };
    ibz_t TT2 = { 0 };

    ibz_t det = { 0 };
    ibz_mul(&det, AA, DD);
    assert(ibz_bitsize_ct(&det) <= det_bits);
    ibz_set_bound_ct(&det, DIM2_VAL_BITS(det_bits));
    int32_t ba = (int32_t)ibz_bitsize_ct(&det);
    int32_t shift_2k = (ba - DIM2_THRESHOLD) & ~(int32_t)1; // round DOWN to even; may be < 0
    int32_t shift_k = shift_2k / 2;                         // exact: shift_2k is even
    uint32_t a_trunc = (uint32_t)lg_window_u64(&det, shift_2k);
    uint64_t sqrta_trunc = (uint64_t)ct_isqrt_32(a_trunc);

    for (int i = 0; i < outer_iters; i++) {
        // 1. Swap if A < B
        ibz_t diff = { 0 };
        ibz_sub(&diff, AA, BB);
        digit_t borrow = ~ibz_is_positive(&diff);
        ibz_cswap(AA, BB, borrow);
        ibz_cswap(CC, DD, borrow);

        // 2. Determine shift/MSB extraction
        int32_t bits_a = (int32_t)ibz_bitsize_ct(AA);
        int32_t bits_b = (int32_t)ibz_bitsize_ct(BB);
        int32_t rho = bits_a - bits_b + 1; // size difference
        int32_t bits_from_a = bits_a - DIM2_W;
        int32_t large_diff = (rho >= DIM2_THRESHOLD);
        int32_t bits_from_b = ct_select32(large_diff, bits_b - (DIM2_W - DIM2_THRESHOLD + 1), bits_from_a);
        int32_t extra_shift = ct_select32(large_diff, rho - DIM2_THRESHOLD, 0);

        // 3. Extract MSB
        uint64_t pa = 0, pb = 0;
        pa = lg_window_u64(AA, bits_from_a);
        pb = lg_window_u64(BB, bits_from_b);

        // 4. Compute approximation of sqrt(a) >> bits_from_b.
        int32_t diff_sh = bits_from_b - shift_k;
        int32_t shift_right = ct_min(16, ct_max0(diff_sh));
        int64_t psqrta = (sqrta_trunc >> shift_right);
        int32_t shift_left = ct_select32(diff_sh <= -48, 0, ct_max0(-diff_sh));
        psqrta <<= shift_left;
        psqrta |= ct_mask(diff_sh <= -48);

        // 5. Partial xgcd on MSB's.
        (void)partial_xgcd_msb(pa, pb, psqrta, M, !large_diff);

        // 6. Apply transform globally
        ct_apply_matrix(AA, BB, M, extra_shift, &TT1, &TT2, det_bits);
        ct_apply_matrix(CC, DD, M, extra_shift, &TT1, &TT2, det_bits);

        // 7. Bring back to normal form A >= 0, B >= 0.
        {
            digit_t negAmask = ~ibz_is_positive(AA);
            ibz_cneg(AA, AA, negAmask);
            ibz_cneg(CC, CC, negAmask);
        }
        {
            digit_t negBmask = ~ibz_is_positive(BB);
            ibz_cneg(BB, BB, negBmask);
            ibz_cneg(DD, DD, negBmask);
        }

#ifndef NDEBUG
        // Verify bitsize bounds
        assert(ibz_bitsize_ct(AA) < DIM2_VAL_BITS(det_bits));
        assert(ibz_bitsize_ct(BB) < DIM2_VAL_BITS(det_bits));
        assert(ibz_bitsize_ct(CC) < DIM2_VAL_BITS(det_bits));
        assert(ibz_bitsize_ct(DD) < DIM2_VAL_BITS(det_bits));
#endif
    }
}

// Size reduce v_2 = (a,c) by v_1=(b,d)
// Returns k such that v_2' = v_2 - k * v_1 is size-reduced
static int32_t
sizered_msb(uint64_t a, uint64_t b, int64_t c, int64_t d)
{
    int32_t v1_shift = ct_min((int32_t)__builtin_clzll(b | 1) - 1, (int32_t)__builtin_clrsbll(d | 1));
    int64_t b_shifted = (int64_t)(b << v1_shift);
    int64_t d_shifted = (int64_t)((uint64_t)d << v1_shift);
    int64_t num = (ct_mul_high(a, b_shifted) >> 1) + (ct_mul_high(c, d_shifted) >> 1);
    int64_t denom = (ct_mul_high(b, b_shifted) >> 1) + (ct_mul_high(d, d_shifted) >> 1);

    uint64_t num_abs;
    uint64_t num_sign;
    ct_abs_sign_i64(num, &num_abs, &num_sign);
    uint64_t k_abs = ct_div_unsigned_nonzero(2 * num_abs + denom, 2 * denom);
    uint64_t mask = -(uint64_t)(num_sign & 1);
    uint64_t signed_u = (k_abs ^ mask) + num_sign;
    int64_t k = signed_u;
    return -(int32_t)k;
}

// postprocessing function after partial_xgcd_lehmer
// need two of these calls to bring partial_xgcd approach to full LG reduced basis
static void
sizered_lehmer(ibz_t *AA, ibz_t *BB, ibz_t *CC, ibz_t *DD, int outer_iters, int det_bits)
{
    ibz_t TT1 = { 0 };

    for (int i = 0; i < outer_iters; i++) {
        // 1. Select correct shifts
        int32_t bits_a = (int32_t)ibz_bitsize_ct(AA);
        int32_t bits_b = (int32_t)ibz_bitsize_ct(BB);
        int32_t bits_c = (int32_t)ibz_bitsize_ct(CC);
        int32_t bits_d = (int32_t)ibz_bitsize_ct(DD);

        int32_t bits_v1 = ct_max(bits_b, bits_d);
        int32_t bits_v2 = ct_max(bits_a, bits_c);
        int32_t bits_max = ct_max(bits_v1, bits_v2);
        int32_t rho = bits_max - bits_v1 + 1; // size difference

        int32_t bits_from_v2 = bits_max - DIM2_W;
        int32_t large_diff = (rho >= DIM2_THRESHOLD);
        int32_t bits_from_v1 = ct_select32(large_diff, bits_v1 - (DIM2_W - DIM2_THRESHOLD + 1), bits_from_v2);
        int32_t extra_shift = ct_select32(large_diff, rho - DIM2_THRESHOLD, 0);

        // 2. Extract MSB's
        uint64_t pa = 0, pb = 0;
        int64_t pc = 0, pd = 0;
        pa = lg_window_u64(AA, bits_from_v2);
        pb = lg_window_u64(BB, bits_from_v1);
        pc = lg_window_i64(CC, bits_from_v2);
        pd = lg_window_i64(DD, bits_from_v1);

        // 3. Partial size-reduction on MSB's
        int32_t mu = sizered_msb(pa, pb, pc, pd);

        // 4. Apply to global values
        ct_size_reduce(AA, BB, mu, extra_shift, &TT1, det_bits);
        ct_size_reduce(CC, DD, mu, extra_shift, &TT1, det_bits);

        // 5. Normalize such that A >= 0 and B >= 0.
        {
            digit_t negAmask = ~ibz_is_positive(AA);
            ibz_cneg(AA, AA, negAmask);
            ibz_cneg(CC, CC, negAmask);
        }
        {
            digit_t negBmask = ~ibz_is_positive(BB);
            ibz_cneg(BB, BB, negBmask);
            ibz_cneg(DD, DD, negBmask);
        }

#ifndef NDEBUG
        // Verify bitsize bounds
        assert(ibz_bitsize_ct(AA) < DIM2_VAL_BITS(det_bits));
        assert(ibz_bitsize_ct(BB) < DIM2_VAL_BITS(det_bits));
        assert(ibz_bitsize_ct(CC) < DIM2_VAL_BITS(det_bits));
        assert(ibz_bitsize_ct(DD) < DIM2_VAL_BITS(det_bits));
#endif
    }
}

// LG reduces a 2x2 basis in HNF form [[A/g, 0], [B, g]], det = A <= 2^det_bits.
void
quat_lll_dim2_short_basis(ibz_mat_2x2_t *reduced, const ibz_mat_2x2_t *basis, int det_bits)
{
    assert(DIM2_WORK_BITS(det_bits) + 1 <= IBZ_MAX_BITS);

    // Number of outer iterations, currently set heuristically.
    // Could be tuned to provable regime. Not need in practice.
    int outer_iters = DIM2_OUTER_ITS(det_bits);

    // 1. Copy matrix to be reduced in-place
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            ibz_copy(&(reduced->m[i][j]), &(basis->m[i][j]));
            ibz_set_bound_ct(&(reduced->m[i][j]), DIM2_VAL_BITS(det_bits));
        }
    }

    // 2. Partial xgcd-based reduction
    // Two extra size-reductions are required to make the partial_xgcd approach fully size-reduced.
    // One potentially large size-red + one tiny one.
    // Using outer_iters on the large case is for some large corner-cases
    // Could be reduced significantly if needed, these cases don't appear in practice.
    partial_xgcd_lehmer(
        &(reduced->m[0][0]), &(reduced->m[0][1]), &(reduced->m[1][0]), &(reduced->m[1][1]), outer_iters, det_bits);
    sizered_lehmer(
        &(reduced->m[0][0]), &(reduced->m[0][1]), &(reduced->m[1][0]), &(reduced->m[1][1]), outer_iters, det_bits);
    sizered_lehmer(&(reduced->m[0][1]), &(reduced->m[0][0]), &(reduced->m[1][1]), &(reduced->m[1][0]), 1, det_bits);

    // 3. Swap based on norm
    int32_t bits_a = (int32_t)ibz_bitsize_ct(&(reduced->m[0][0]));
    int32_t bits_b = (int32_t)ibz_bitsize_ct(&(reduced->m[0][1]));
    int32_t bits_c = (int32_t)ibz_bitsize_ct(&(reduced->m[1][0]));
    int32_t bits_d = (int32_t)ibz_bitsize_ct(&(reduced->m[1][1]));
    int32_t bits_v1 = ct_max(bits_b, bits_d);
    int32_t bits_v2 = ct_max(bits_a, bits_c);
    int32_t bits_max = ct_max(bits_v1, bits_v2);
    int32_t bits_from_vecs = ct_select32(bits_max >= DIM2_W, bits_max - DIM2_W, 0);
    int64_t pa = ibz_extract_i64(&(reduced->m[0][0]), bits_from_vecs);
    int64_t pb = ibz_extract_i64(&(reduced->m[0][1]), bits_from_vecs);
    int64_t pc = ibz_extract_i64(&(reduced->m[1][0]), bits_from_vecs);
    int64_t pd = ibz_extract_i64(&(reduced->m[1][1]), bits_from_vecs);
    // Shift to prevent highmul issues with very small entries.
    // Not triggered in practice but for extra robustness.
    int32_t up = ct_max0(DIM2_W - 1 - bits_max);
    pa = (int64_t)((uint64_t)pa << up);
    pb = (int64_t)((uint64_t)pb << up);
    pc = (int64_t)((uint64_t)pc << up);
    pd = (int64_t)((uint64_t)pd << up);
    uint64_t norm_v1 = ct_sqr_high(pa) + ct_sqr_high(pc);
    uint64_t norm_v2 = ct_sqr_high(pb) + ct_sqr_high(pd);
    digit_t swap_mask = ct_mask(norm_v1 > norm_v2);
    ibz_cswap(&(reduced->m[0][0]), &(reduced->m[0][1]), swap_mask);
    ibz_cswap(&(reduced->m[1][0]), &(reduced->m[1][1]), swap_mask);
}

// Input prime p and 0 < r < p such that r^2 = -1 mod p.
// Output: 0 <= x,y < p such that x^2 + y^2 = p.
void
quat_lll_dim2_sumofsquares(ibz_t *x, ibz_t *y, const ibz_t *p, const ibz_t *r)
{
    // The shape here is [[p, 0], [r, 1]], so det == p
    int det_bits = MAXB(ibz_get_bound(p), ibz_get_bound(r));

    assert(DIM2_WORK_BITS(det_bits) + 1 <= IBZ_MAX_BITS);
    // Heuristic, see quat_lll_dim2_short_basis
    int outer_iters = DIM2_OUTER_ITS(det_bits);

    // 1. Setup matrix to be reduced
    ibz_mat_2x2_t basis = { 0 };
    ibz_copy(&basis.m[0][0], p);
    ibz_copy(&basis.m[0][1], r);
    ibz_set_bound_ct(&basis.m[0][0], DIM2_VAL_BITS(det_bits));
    ibz_set_bound_ct(&basis.m[0][1], DIM2_VAL_BITS(det_bits));
    ibz_set(&basis.m[1][0], 0, 2);
    ibz_set(&basis.m[1][1], 1, 2);
    ibz_set_bound_ct(&basis.m[1][0], DIM2_VAL_BITS(det_bits));
    ibz_set_bound_ct(&basis.m[1][1], DIM2_VAL_BITS(det_bits));

    // 2. Partial xgcd-based reduction
    partial_xgcd_lehmer(&basis.m[0][0], &basis.m[0][1], &basis.m[1][0], &basis.m[1][1], outer_iters, det_bits);

    ibz_abs(x, &basis.m[0][1]);
    ibz_abs(y, &basis.m[1][1]);
}
