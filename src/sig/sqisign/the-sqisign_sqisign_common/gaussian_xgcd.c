#include <lll.h>
#include <mp_ct.h>
#include "lll_config.h"
#include <assert.h>
#include <stdint.h>

// Complex bigint
typedef struct
{
    ibz_t re;
    ibz_t im;
} ct_complex_t;

// Complex 32-bit integer
typedef struct
{
    int32_t re;
    int32_t im;
} i32_complex_t;

// Complex 64-bit integer
typedef struct
{
    int64_t re;
    int64_t im;
} i64_complex_t;

static inline void
ct_complex_mul_si(ct_complex_t *res, ct_complex_t *A, i32_complex_t scalar, int wbits)
{
    ibz_t t1 = { 0 };
    ibz_t t2 = { 0 };
    ibz_t res_re = { 0 };
    ibz_mul_by_int_and_set_bound(&t1, &A->re, scalar.re, wbits);
    ibz_mul_by_int_and_set_bound(&t2, &A->im, -scalar.im, wbits);
    ibz_add_and_set_bound(&res_re, &t1, &t2, wbits);
    ibz_mul_by_int_and_set_bound(&t1, &A->re, scalar.im, wbits);
    ibz_mul_by_int_and_set_bound(&t2, &A->im, scalar.re, wbits);
    ibz_add_and_set_bound(&res->im, &t1, &t2, wbits);
    res->re = res_re;
}

static inline void
ct_complex_shift_left(ct_complex_t *res, ct_complex_t *A, int32_t extra_shift, int32_t max_shift, int wbits)
{
    ibz_ct_shl(&res->re, &A->re, extra_shift, max_shift, wbits);
    ibz_ct_shl(&res->im, &A->im, extra_shift, max_shift, wbits);
}

static inline void
ct_complex_add(ct_complex_t *res, ct_complex_t *A, ct_complex_t *B, int vbits)
{
    ibz_add_and_set_bound(&res->re, &A->re, &B->re, vbits);
    ibz_add_and_set_bound(&res->im, &A->im, &B->im, vbits);
}

static inline uint32_t
ct_complex_sizeinbase_2(const ct_complex_t *A)
{
    uint32_t bits_re = (uint32_t)ibz_bitsize_ct(&A->re);
    uint32_t bits_im = (uint32_t)ibz_bitsize_ct(&A->im);
    return ct_max_u32(bits_re, bits_im);
}

static inline void
ct_complex_cond_swap(digit_t mask, ct_complex_t *A, ct_complex_t *B)
{
    ibz_cswap(&A->re, &B->re, mask);
    ibz_cswap(&A->im, &B->im, mask);
}

// Return Norm(a) // 2^64
static inline uint64_t
ct_norm64_approx(i64_complex_t a)
{
    return ct_sqr_high(a.re) + ct_sqr_high(a.im);
}

static inline void
ct_swap_i32_complex(uint32_t mask, i32_complex_t *a, i32_complex_t *b)
{
    ct_swap_i32(mask, &a->re, &b->re);
    ct_swap_i32(mask, &a->im, &b->im);
}

static inline void
ct_swap_i64_complex(uint64_t mask, i64_complex_t *a, i64_complex_t *b)
{
    ct_swap_i64(mask, &a->re, &b->re);
    ct_swap_i64(mask, &a->im, &b->im);
}

static inline i32_complex_t
ct_mul_i32_complex(i32_complex_t a, i32_complex_t b)
{
    return (i32_complex_t){ a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re };
}

static inline i64_complex_t
ct_shift_i64_complex(i64_complex_t a, int k)
{
    return (i64_complex_t){ (int64_t)((uint64_t)a.re << k), (int64_t)((uint64_t)a.im << k) };
}

static inline i32_complex_t
ct_shift_i32_complex(i32_complex_t a, int k)
{
    return (i32_complex_t){ (int32_t)((uint32_t)a.re << k), (int32_t)((uint32_t)a.im << k) };
}

static void
ct_apply_matrix_gaussian(ct_complex_t *A,
                         ct_complex_t *B,
                         i32_complex_t M[2][2],
                         int32_t extra_shift,
                         ct_complex_t *T1,
                         ct_complex_t *T2,
                         int in_bits)
{
    const int wbits = DIM2_WORK_BITS(in_bits);
    const int vbits = DIM2_VAL_BITS(in_bits);

    ct_complex_mul_si(T1, A, M[1][0], wbits);
    ct_complex_shift_left(T2, B, extra_shift, DIM2I_SHIFT_MAX(in_bits), wbits);
    ct_complex_mul_si(T2, T2, M[0][1], wbits);
    ct_complex_mul_si(A, A, M[0][0], wbits);
    ct_complex_add(A, A, T2, vbits);
    ct_complex_mul_si(B, B, M[1][1], wbits);
    ct_complex_add(B, B, T1, vbits);
}

// Returns ceil(log2(a) - log2(b)).
// Intended for a > 0, b > 0; a zero argument is treated as 1
static int
ceil_log2_diff_u64(uint64_t a, uint64_t b)
{
    int clz_a = sqisign_clz64(a | 1);
    int clz_b = sqisign_clz64(b | 1);
    int d = clz_b - clz_a;
    uint32_t sign_bit = (uint32_t)d >> 31;
    uint32_t mask_neg = ct_barrier_u32(0U - sign_bit);
    uint32_t shift_b = (uint32_t)d & ~mask_neg;
    uint32_t shift_a = (uint32_t)(-d) & mask_neg;
    uint64_t shifted_b = b << shift_b;
    uint64_t shifted_a = a << shift_a;
    int cond = (int)((shifted_b - shifted_a) >> 63);
    return d + cond;
}

// Computes round(A / B) in constant time.
// Requires: A/B in [-2, 2], B > 0, and both |A| and B < 2^62.
static int32_t
round_ratio_62bit_ct(int64_t A, uint64_t B)
{
    uint64_t max_abs = ct_abs64(A);

    // Thresholds for rounding to nearest integer
    uint64_t T1 = B >> 1; // 0.5 B
    uint64_t T2 = B + T1; // 1.5 B
    uint32_t is_1 = (max_abs > T1);
    uint32_t is_2 = (max_abs > T2);
    uint32_t res_abs = is_1 + is_2;
    uint32_t sign_mask = (A >> 63);
    uint32_t res = (res_abs ^ sign_mask) - sign_mask;
    return (int32_t)res;
}

static int
LG_gaussian_inner(i64_complex_t a,
                  i64_complex_t b,
                  i64_complex_t c,
                  i64_complex_t d,
                  i32_complex_t matrix[2][2],
                  uint64_t swap,
                  int32_t bits_from_v1,
                  int32_t bits_from_v2)
{
    i32_complex_t u00 = { 1, 0 }, u01 = { 0, 0 };
    i32_complex_t u10 = { 0, 0 }, u11 = { 1, 0 };

    uint64_t valid_mask = 0xFFFFFFFFFFFFFFFFULL;
    int valid_steps = 0;

    uint64_t global_swap_mask = ct_mask(swap);
    for (int i = 0; i < DIM2I_INNER_ITS; i++) {
        // 1. Local stopping condition
        uint32_t clz_a = sqisign_clz64(ct_abs64(a.re) | ct_abs64(a.im) | 1);
        uint32_t clz_b = sqisign_clz64(ct_abs64(b.re) | ct_abs64(b.im) | 1);
        uint32_t clz_c = sqisign_clz64(ct_abs64(c.re) | ct_abs64(c.im) | 1);
        uint32_t clz_d = sqisign_clz64(ct_abs64(d.re) | ct_abs64(d.im) | 1);
        int32_t base_shift_v1 = ct_max0((int32_t)ct_min_u32(clz_a, clz_c) - DIM2I_UP_SHIFT);
        int32_t base_shift_v2 = ct_max0((int32_t)ct_min_u32(clz_b, clz_d) - DIM2I_UP_SHIFT);
        uint64_t stop_v1 = ct_mask(base_shift_v1 > (int32_t)(DIM2I_THRESHOLD + (64 - DIM2I_UP_SHIFT - DIM2I_W))) &
                           ct_mask(bits_from_v1 > 0);
        uint64_t stop_v2 = ct_mask(base_shift_v2 > (int32_t)(DIM2I_THRESHOLD + (64 - DIM2I_UP_SHIFT - DIM2I_W))) &
                           ct_mask(bits_from_v2 > 0);
        valid_mask = ct_barrier_u64(valid_mask & ~(stop_v1 | stop_v2));

        // 2. Base shift calculation to get most accuracy for the computations
        i64_complex_t a_base = ct_shift_i64_complex(a, base_shift_v1);
        i64_complex_t b_base = ct_shift_i64_complex(b, base_shift_v2);
        i64_complex_t c_base = ct_shift_i64_complex(c, base_shift_v1);
        i64_complex_t d_base = ct_shift_i64_complex(d, base_shift_v2);
        uint64_t na = ct_norm64_approx(a_base) + ct_norm64_approx(c_base); // n(v1)
        uint64_t nb = ct_norm64_approx(b_base) + ct_norm64_approx(d_base); // n(v2)

        // 3. Conditional swap based on norms
        int32_t base_shift_rel = (int32_t)base_shift_v2 - (int32_t)base_shift_v1;
        uint32_t v1_right = ct_select32(base_shift_rel < 0, -2 * base_shift_rel, 0);
        uint32_t v2_right = ct_select32(base_shift_rel > 0, 2 * base_shift_rel, 0);
        uint64_t lt_mask = -(uint64_t)((ct_shr_sat64(na, v1_right) - ct_shr_sat64(nb, v2_right)) >> 63); // na < nb
        uint64_t swap_mask = ct_barrier_u64(lt_mask & valid_mask);
        valid_mask = ct_barrier_u64(valid_mask & (~swap_mask | global_swap_mask));
        uint64_t actual_swap = ct_barrier_u64(swap_mask & global_swap_mask);
        ct_swap_u64(actual_swap, &na, &nb);
        ct_swap_i64_complex(actual_swap, &a, &b);
        ct_swap_i64_complex(actual_swap, &c, &d);
        ct_swap_i32_complex(actual_swap, &u00, &u10);
        ct_swap_i32_complex(actual_swap, &u01, &u11);
        ct_swap_i64_complex(actual_swap, &a_base, &b_base);
        ct_swap_i64_complex(actual_swap, &c_base, &d_base);
        ct_swap_i32((uint32_t)actual_swap, &base_shift_v1, &base_shift_v2);
        ct_swap_i32((uint32_t)actual_swap, &bits_from_v1, &bits_from_v2);

        // 4. Compute X+i*Y = <v_1, v_2> (up to scaling)
        base_shift_rel = (int32_t)base_shift_v2 - (int32_t)base_shift_v1;
        int64_t X = ct_mul_high(a_base.re, b_base.re) + ct_mul_high(a_base.im, b_base.im) +
                    ct_mul_high(c_base.re, d_base.re) + ct_mul_high(c_base.im, d_base.im);
        int64_t Y = ct_mul_high(a_base.im, b_base.re) - ct_mul_high(a_base.re, b_base.im) +
                    ct_mul_high(c_base.im, d_base.re) - ct_mul_high(c_base.re, d_base.im);
        uint64_t X_abs = ct_abs64(X);
        uint64_t Y_abs = ct_abs64(Y);

        // 5. Normalize such that (X+Y*i)/norm(v2_shifted) in [-2,2]x[-2,2]*i
        int32_t s = ceil_log2_diff_u64(ct_max_u64(X_abs, Y_abs), nb) - 1;
        uint32_t k = ct_select_u32(base_shift_rel + s < 0, 0, (uint32_t)(base_shift_rel + s));
        i64_complex_t b_shifted = ct_shift_i64_complex(b, k);
        i64_complex_t d_shifted = ct_shift_i64_complex(d, k);
        int32_t shift_diff = base_shift_rel - (int32_t)k;
        uint32_t shift_left_X = (uint32_t)(shift_diff & ~(shift_diff >> 31));
        uint32_t shift_left_nb = (uint32_t)((-shift_diff) & ~((-shift_diff) >> 31));
        int64_t X_adj = (int64_t)((uint64_t)X << shift_left_X);
        int64_t Y_adj = (int64_t)((uint64_t)Y << shift_left_X);
        uint64_t nb_adj = nb << shift_left_nb;

        // 6. Compute rounding
        int32_t Xround = round_ratio_62bit_ct(X_adj, nb_adj); // -2 to 2
        int32_t Yround = round_ratio_62bit_ct(Y_adj, nb_adj); // -2 to 2

        // 7. Global stopping condition (q=0)
        uint64_t zero_reduction = ct_mask(Xround == 0) & ct_mask(Yround == 0);
        valid_mask = ct_barrier_u64(valid_mask & ~zero_reduction);

        // 8. Update remainder and matrix
        i64_complex_t b_final = { Xround * b_shifted.re - Yround * b_shifted.im,
                                  Xround * b_shifted.im + Yround * b_shifted.re };
        i64_complex_t d_final = { Xround * d_shifted.re - Yround * d_shifted.im,
                                  Xround * d_shifted.im + Yround * d_shifted.re };
        a.re -= b_final.re & valid_mask;
        a.im -= b_final.im & valid_mask;
        c.re -= d_final.re & valid_mask;
        c.im -= d_final.im & valid_mask;
        i32_complex_t u10_shift = ct_shift_i32_complex(u10, k);
        i32_complex_t u11_shift = ct_shift_i32_complex(u11, k);
        u10_shift = ct_mul_i32_complex((i32_complex_t){ Xround, Yround }, u10_shift);
        u11_shift = ct_mul_i32_complex((i32_complex_t){ Xround, Yround }, u11_shift);
        u00.re -= u10_shift.re & ((uint32_t)valid_mask);
        u00.im -= u10_shift.im & ((uint32_t)valid_mask);
        u01.re -= u11_shift.re & ((uint32_t)valid_mask);
        u01.im -= u11_shift.im & ((uint32_t)valid_mask);

        // Statistics
        valid_steps += (int)(valid_mask & 1);
    }

    matrix[0][0] = u00;
    matrix[0][1] = u01;
    matrix[1][0] = u10;
    matrix[1][1] = u11;
    return valid_steps;
}

// Multiply (key, X, Y, Z) by the Z[i] unit (one of 1, -1, i, -i) that makes Re(key), Im(key) >= 0.
// X, Y, Z are OPTIONAL further coordinates carried along by the same unit (pass NULL).
// The key decides canonicity: a vector's own coordinate is not invariant over an ideal's
// equivalence class, a Hermitian inner product (ct_zi_hermitian) is.
static void
orient_canonical(ct_complex_t *key, ct_complex_t *X, ct_complex_t *Y, ct_complex_t *Z)
{
    ct_complex_t *tgt[4] = { key, X, Y, Z };

    digit_t neg_re = ~ibz_is_positive(&key->re);
    digit_t neg_im = ~ibz_is_positive(&key->im);
    digit_t swap_mask = neg_re ^ neg_im;

    for (int t = 0; t < 4; t++) {
        if (tgt[t] == NULL)
            continue;
        ibz_cswap(&tgt[t]->re, &tgt[t]->im, swap_mask);
        ibz_cneg(&tgt[t]->re, &tgt[t]->re, swap_mask);
    }

    digit_t neg_mask = ~ibz_is_positive(&key->re) | ~ibz_is_positive(&key->im);
    for (int t = 0; t < 4; t++) {
        if (tgt[t] == NULL)
            continue;
        ibz_cneg(&tgt[t]->re, &tgt[t]->re, neg_mask);
        ibz_cneg(&tgt[t]->im, &tgt[t]->im, neg_mask);
    }
}

// Lagrange-Gauss reduce the basis ((AA,CC),(BB,DD)) over Z[i], in place.
//
// (EE, FF) is an optional extra coordinate pair used for tracking scaled (BB, DD)
//
// Out: LG-reduced basis ((AA,CC),(BB,DD)),
// AA.re, AA.im, BB.re, BB.im >= 0 and ordered by norm ||(AA,CC)|| <= ||(BB,DD)||
static void
lehmer_gaussian(ct_complex_t *AA,
                ct_complex_t *BB,
                ct_complex_t *CC,
                ct_complex_t *DD,
                ct_complex_t *EE,
                ct_complex_t *FF,
                int outer_iters,
                int in_bits)
{
    i32_complex_t M[2][2];
    ct_complex_t TT1 = { 0 };
    ct_complex_t TT2 = { 0 };

    for (int i = 0; i < outer_iters; i++) {
        // 1. Rough swap based on bitsize
        int32_t bits_a = (int32_t)ct_complex_sizeinbase_2(AA);
        int32_t bits_b = (int32_t)ct_complex_sizeinbase_2(BB);
        int32_t bits_c = (int32_t)ct_complex_sizeinbase_2(CC);
        int32_t bits_d = (int32_t)ct_complex_sizeinbase_2(DD);
        int32_t bits_v1 = ct_max(bits_a, bits_c);
        int32_t bits_v2 = ct_max(bits_b, bits_d);
        digit_t swap_mask = ct_mask(bits_v1 < bits_v2);
        ct_complex_cond_swap(swap_mask, AA, BB);
        ct_complex_cond_swap(swap_mask, CC, DD);
        if (EE != NULL)
            ct_complex_cond_swap(swap_mask, EE, FF);
        ct_swap_i32((uint32_t)swap_mask, &bits_v1, &bits_v2);

        // 2. Extract MSB window
        int32_t rho = bits_v1 - bits_v2 + 1; // size difference
        int32_t bits_from_v1 = ct_select32(bits_v1 >= DIM2I_W, bits_v1 - DIM2I_W, 0);
        int32_t large_diff = (rho >= DIM2I_THRESHOLD);
        int32_t bfv2_ideal = bits_v2 - (DIM2I_W - DIM2I_THRESHOLD + 1);
        bfv2_ideal = ct_max0(bfv2_ideal);
        int32_t bits_from_v2 = ct_select32(large_diff, bfv2_ideal, bits_from_v1);
        int32_t extra_shift = ct_select32(large_diff, rho - DIM2I_THRESHOLD, 0);

        i64_complex_t pa = { ibz_extract_i64(&AA->re, bits_from_v1), ibz_extract_i64(&AA->im, bits_from_v1) };
        i64_complex_t pb = { ibz_extract_i64(&BB->re, bits_from_v2), ibz_extract_i64(&BB->im, bits_from_v2) };
        i64_complex_t pc = { ibz_extract_i64(&CC->re, bits_from_v1), ibz_extract_i64(&CC->im, bits_from_v1) };
        i64_complex_t pd = { ibz_extract_i64(&DD->re, bits_from_v2), ibz_extract_i64(&DD->im, bits_from_v2) };

        // 3. Run inner LG reduction.
        (void)LG_gaussian_inner(pa, pb, pc, pd, M, !large_diff, bits_from_v1, bits_from_v2);

        ct_apply_matrix_gaussian(AA, BB, M, extra_shift, &TT1, &TT2, in_bits);
        ct_apply_matrix_gaussian(CC, DD, M, extra_shift, &TT1, &TT2, in_bits);
        if (EE != NULL)
            ct_apply_matrix_gaussian(EE, FF, M, extra_shift, &TT1, &TT2, in_bits);
    }

    // Unconditional, not a norm comparison: the kernel's last effective act leaves the LARGER
    // vector in v1, so the pre-swap order is known rather than measured.
    ct_complex_cond_swap(~(digit_t)0, AA, BB);
    ct_complex_cond_swap(~(digit_t)0, CC, DD);
    if (EE != NULL)
        ct_complex_cond_swap(~(digit_t)0, EE, FF);

    // Make signs canonical
    orient_canonical(AA, CC, EE, NULL);
    orient_canonical(BB, DD, FF, NULL);
}

// Exact Z[i]-Hermitian inner product H(v, w) = conj(v1)*w1 + p*conj(v2)*w2
static void
ct_zi_hermitian(ct_complex_t *h,
                const ct_complex_t *v1,
                const ct_complex_t *v2,
                const ct_complex_t *w1,
                const ct_complex_t *w2,
                const ibz_t *p,
                int hbits)
{
    ibz_t t1 = { 0 }, t2 = { 0 }, t3 = { 0 }, acc = { 0 };

    // Re: (v1.re*w1.re + v1.im*w1.im) + p*(v2.re*w2.re + v2.im*w2.im)
    ibz_mul(&t1, &v2->re, &w2->re);
    ibz_set_bound_ct(&t1, hbits);
    ibz_mul(&t2, &v2->im, &w2->im);
    ibz_set_bound_ct(&t2, hbits);
    ibz_add_and_set_bound(&t3, &t1, &t2, hbits);
    ibz_mul(&acc, &t3, p);
    ibz_set_bound_ct(&acc, hbits);
    ibz_mul(&t1, &v1->re, &w1->re);
    ibz_set_bound_ct(&t1, hbits);
    ibz_mul(&t2, &v1->im, &w1->im);
    ibz_set_bound_ct(&t2, hbits);
    ibz_add_and_set_bound(&t3, &t1, &t2, hbits);
    ibz_add_and_set_bound(&h->re, &t3, &acc, hbits);

    // Im: (v1.re*w1.im - v1.im*w1.re) + p*(v2.re*w2.im - v2.im*w2.re)
    ibz_mul(&t1, &v2->re, &w2->im);
    ibz_set_bound_ct(&t1, hbits);
    ibz_mul(&t2, &v2->im, &w2->re);
    ibz_set_bound_ct(&t2, hbits);
    ibz_sub(&t3, &t1, &t2);
    ibz_set_bound_ct(&t3, hbits);
    ibz_mul(&acc, &t3, p);
    ibz_set_bound_ct(&acc, hbits);
    ibz_mul(&t1, &v1->re, &w1->im);
    ibz_set_bound_ct(&t1, hbits);
    ibz_mul(&t2, &v1->im, &w1->re);
    ibz_set_bound_ct(&t2, hbits);
    ibz_sub(&t3, &t1, &t2);
    ibz_set_bound_ct(&t3, hbits);
    ibz_add_and_set_bound(&h->im, &t3, &acc, hbits);
}

// ==== The public ideal-level entry point

static void
ct_complex_from_rows(ct_complex_t *out, const ibz_t *re, const ibz_t *im, int wbits)
{
    ibz_copy(&out->re, re);
    ibz_set_bound_ct(&out->re, wbits);
    ibz_copy(&out->im, im);
    ibz_set_bound_ct(&out->im, wbits);
}

static void
ct_complex_set_bound(ct_complex_t *a, int wbits)
{
    ibz_set_bound_ct(&a->re, wbits);
    ibz_set_bound_ct(&a->im, wbits);
}

// Write the Z[i] vector (z1, z2) into column c, and i*(z1, z2) into column c+1.
static void
quat_write_zi_column_pair(ibz_mat_4x4_t *basis, int c, const ct_complex_t *z1, const ct_complex_t *z2, int wbits)
{
    ibz_copy(&basis->m[0][c], &z1->re);
    ibz_copy(&basis->m[1][c], &z1->im);
    ibz_copy(&basis->m[2][c], &z2->re);
    ibz_copy(&basis->m[3][c], &z2->im);

    ibz_neg(&basis->m[0][c + 1], &z1->im);
    ibz_copy(&basis->m[1][c + 1], &z1->re);
    ibz_neg(&basis->m[2][c + 1], &z2->im);
    ibz_copy(&basis->m[3][c + 1], &z2->re);

    for (int r = 0; r < 4; r++) {
        ibz_set_bound_ct(&basis->m[r][c], wbits);
        ibz_set_bound_ct(&basis->m[r][c + 1], wbits);
    }
}

// The quaternion norm on the Z[i]-coordinates (z1, z2) is |z1|^2 + p*|z2|^2,
// not the unweighted form the kernel minimises, so the second coordinate is scaled by isqrt(p)
// for the reduction while the unscaled one rides along in lehmer_gaussian's tracked pair.
void
quat_lll_reduce_O0_ideal(quat_lattice_t *reduced, const quat_lattice_t *hnf, const quat_alg_t *alg, int bound)
{
    // 1. the sqrt(p) weight
    ibz_t s = { 0 };
    ibz_set_from_str(&s, QUAT_SQRT_P_HEX, 16);
    ibz_set_bound_ct(&s, QUAT_SQRT_P_BITS + 1);
#ifndef NDEBUG
    {
        // s^2 <= p < (s+1)^2.
        ibz_t sq = { 0 }, s1 = { 0 }, sq1 = { 0 };
        ibz_mul(&sq, &s, &s);
        ibz_add_int_and_set_bound(&s1, &s, 1, QUAT_SQRT_P_BITS + 2);
        ibz_mul(&sq1, &s1, &s1);
        assert(ibz_cmp(&sq, &alg->p) <= 0);
        assert(ibz_cmp(&alg->p, &sq1) < 0);
    }
#endif

    // 2. widths and trip counts, all PUBLIC
    int in_bits = MAXB(bound, QUAT_SQRT_P_BITS + 1);
    const int wbits = DIM2_VAL_BITS(in_bits);
    const int hbits = DIM2I_HERM_BITS(in_bits);
    assert(DIM2_WORK_BITS(in_bits) + 1 <= IBZ_MAX_BITS);
    assert(hbits <= IBZ_MAX_BITS);

    // 3. the rank-2 Z[i] basis. Columns 1 and 3 of the inert HNF are i times columns 0 and 2
    // modulo the Z[i]-span of column 0, so read 0 and 2 generically as the generating pair.
    ct_complex_t AA = { 0 }, BB = { 0 }, CC = { 0 }, DD = { 0 }, EE = { 0 }, FF = { 0 };
    ct_complex_from_rows(&AA, &hnf->basis.m[0][0], &hnf->basis.m[1][0], wbits);
    ct_complex_from_rows(&EE, &hnf->basis.m[2][0], &hnf->basis.m[3][0], wbits);
    ct_complex_from_rows(&BB, &hnf->basis.m[0][2], &hnf->basis.m[1][2], wbits);
    ct_complex_from_rows(&FF, &hnf->basis.m[2][2], &hnf->basis.m[3][2], wbits);

    // (CC, DD): the SCALED second coordinate, so |z1|^2 + s^2*|z2|^2 stands in for the norm.
    ibz_mul(&CC.re, &EE.re, &s);
    ibz_set_bound_ct(&CC.re, wbits);
    ibz_mul(&CC.im, &EE.im, &s);
    ibz_set_bound_ct(&CC.im, wbits);
    ibz_mul(&DD.re, &FF.re, &s);
    ibz_set_bound_ct(&DD.re, wbits);
    ibz_mul(&DD.im, &FF.im, &s);
    ibz_set_bound_ct(&DD.im, wbits);

    // 4. reduce
    lehmer_gaussian(&AA, &BB, &CC, &DD, &EE, &FF, DIM2I_OUTER_ITS(in_bits), in_bits);

    // 4b. Canonize v2 bases on Hermitian inner product.
    ct_complex_t H = { 0 };
    ct_zi_hermitian(&H, &AA, &EE, &BB, &FF, &alg->p, hbits);
    orient_canonical(&H, &BB, &DD, &FF);

#ifndef NDEBUG
    {
        // Recomputed from the final vectors, so this checks H(v1, mu*v2) = mu*H(v1, v2).
        ct_complex_t chk = { 0 };
        ct_zi_hermitian(&chk, &AA, &EE, &BB, &FF, &alg->p, hbits);
        assert(ibz_is_positive(&chk.re) && ibz_is_positive(&chk.im));
    }
    assert(ibz_bitsize_ct(&AA.re) <= wbits && ibz_bitsize_ct(&AA.im) <= wbits);
    assert(ibz_bitsize_ct(&BB.re) <= wbits && ibz_bitsize_ct(&BB.im) <= wbits);
    assert(ibz_bitsize_ct(&EE.re) <= wbits && ibz_bitsize_ct(&EE.im) <= wbits);
    assert(ibz_bitsize_ct(&FF.re) <= wbits && ibz_bitsize_ct(&FF.im) <= wbits);
#endif

    // Set output bounds. Could heuristically be much lower.
    ct_complex_set_bound(&AA, wbits);
    ct_complex_set_bound(&BB, wbits);
    ct_complex_set_bound(&CC, wbits);
    ct_complex_set_bound(&DD, wbits);
    ct_complex_set_bound(&EE, wbits);
    ct_complex_set_bound(&FF, wbits);

    // 5. write the four columns v1, i*v1, v2, i*v2
    quat_write_zi_column_pair(&reduced->basis, 0, &AA, &EE, wbits);
    quat_write_zi_column_pair(&reduced->basis, 2, &BB, &FF, wbits);
    ibz_copy(&reduced->denom, &hnf->denom);
}
