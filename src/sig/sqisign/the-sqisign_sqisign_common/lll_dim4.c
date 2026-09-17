#include <lll.h>
#include "lll_lg2.h"
#include "lll_config.h"
#include "mp.h"
#include <mp_ct.h>
#include <assert.h>

typedef struct
{
    ibz_t D[4];       // raw,   bound FP_E_BITS
    ibz_t L[4][4];    // 2^P,   bound FP_L_BITS, L[j][j] = 1<<P
    ibz_t Ainv[4][4]; // exact, bound FP_U_BITS -- the accumulated inverse transform
} fp_ldl_t;

typedef struct
{
    int8_t T[4][4];         // selected candidates' t-coefficients (row i = candidate i)
    ibz_t Ainv_total[4][4]; // = Ainv * T^-1, bound QUAT_AINV_BITS (= FP_U_BITS + 32)
    ibz_t G[4][4];
} ct_mk_result_t;

// ==== Part 1: LDL state — dual init, block update, size reduction

// ---- closed-form dual init

static void fp_size_reduce_from(fp_ldl_t *st, int k, int P);

// L[i][j] := -X/(2N) at scale 2^P, via the shared reciprocal R
// (num<<P)/D==ibz_ct_highmul_s(num, W, -s, ...)).
static void
set_neg_ratio(ibz_t *out, const ibz_t *X, const ibz_t *R, int32_t s2N, int max_shift)
{
    ibz_t ratio = { 0 }, neg = { 0 };
    ibz_ct_highmul_s(&ratio, X, R, -s2N, max_shift, FP_E_BITS);
    ibz_neg(&neg, &ratio);
    ibz_set_bound_ct(&neg, FP_E_BITS);
    ibz_copy(out, &neg);
}

// Closed-form dual LDL from the HNF (2N, B, C, Dq, E, p): D = ((2N)^2, (2N)^2, p, p),
// and the dual L has only
//   L[2][0] = -E/(2N), L[2][1] = -Dq/(2N), L[3][0] = -C/(2N), L[3][1] = -B/(2N)
// (all other off-diagonal entries 0, diagonal 1<<P). Ainv is initialised to the identity.
static void
fp_ldl_init_dual(fp_ldl_t *st,
                 const ibz_t *twoN,
                 const ibz_t *B,
                 const ibz_t *C,
                 const ibz_t *Dq,
                 const ibz_t *E,
                 const ibz_t *p,
                 int P)
{
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_set(&st->L[i][j], 0, 2);
            ibz_set_bound_ct(&st->L[i][j], FP_L_BITS);
        }
    }
    for (int i = 0; i < 4; i++) {
        ibz_set(&st->L[i][i], 1, 2);
        ibz_mul_2exp(&st->L[i][i], &st->L[i][i], (uint32_t)P);
        ibz_set_bound_ct(&st->L[i][i], FP_L_BITS);
    }

    // FP_D_SHIFT: floor-right-shift D entries by the same public amount.
    // We don't require full precision
    ibz_t twoN_sq = { 0 };
    ibz_mul(&twoN_sq, twoN, twoN);
    ibz_ct_shift(&twoN_sq, &twoN_sq, -FP_D_SHIFT, FP_D_SHIFT, FP_E_BITS);
    ibz_copy(&st->D[0], &twoN_sq);
    ibz_copy(&st->D[1], &twoN_sq);
    ibz_t p_local = { 0 };
    ibz_copy(&p_local, p);
    ibz_ct_shift(&p_local, &p_local, -FP_D_SHIFT, FP_D_SHIFT, FP_E_BITS);
    ibz_copy(&st->D[2], &p_local);
    ibz_copy(&st->D[3], &p_local);

    // L has a 2x2 lower-triangular block [[-E/2N, -Dq/2N],[-C/2N], [-B/2N]].
    // Diagonal is 1 (2^P), rest is 0.
    // Compute fixed-point representation of 1/2N once.
    ibz_t R = { 0 };
    int32_t s2N = 0;
    ibz_ct_fp_recip(&R, &s2N, twoN, P, FP_NR_ROUNDS);
    int max_shift = ibz_get_bound(twoN);
    set_neg_ratio(&st->L[2][0], E, &R, s2N, max_shift);
    set_neg_ratio(&st->L[2][1], Dq, &R, s2N, max_shift);
    set_neg_ratio(&st->L[3][0], C, &R, s2N, max_shift);
    set_neg_ratio(&st->L[3][1], B, &R, s2N, max_shift);

    // initialize Ainv (inverse unimodular transform)
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_set(&st->Ainv[i][j], (i == j) ? 1 : 0, 2);
            ibz_set_bound_ct(&st->Ainv[i][j], FP_U_BITS);
        }
    }

    // The HNF off-diagonals are not required to be reduced mod 2N, so L may come out
    // of the closed form not size-reduced. Establish |L[r][j]| <= 2^(P-1) before the tours.
    fp_size_reduce_from(st, 0, P);
}

// ---- block update (STEP 1/2 closed-form Gram update; STEP 3/4 tail projections)

// Apply the 2x2 unimodular transform U (column convention, matching the LG kernel's output:
// b_k' = u00*b_k + u10*b_{k+1}, b_{k+1}' = u01*b_k + u11*b_{k+1}) to block (k, k+1) of the LDL
// state, updating D, L and Ainv. U entries are ibz_t, bound FP_U_BITS.
static void
fp_update_block(fp_ldl_t *st, int k, ibz_t U[2][2], int P)
{
    int kp1 = k + 1;

    // Capture old values before it is overwritten below.
    ibz_t mu_old = { 0 };
    ibz_copy(&mu_old, &st->L[kp1][k]);
    ibz_set_bound_ct(&mu_old, FP_L_BITS);

    // Set bounds
    ibz_t u00 = { 0 }, u01 = { 0 }, u10 = { 0 }, u11 = { 0 };
    ibz_copy(&u00, &U[0][0]);
    ibz_set_bound_ct(&u00, BLOCK_U_BITS);
    ibz_copy(&u01, &U[0][1]);
    ibz_set_bound_ct(&u01, BLOCK_U_BITS);
    ibz_copy(&u10, &U[1][0]);
    ibz_set_bound_ct(&u10, BLOCK_U_BITS);
    ibz_copy(&u11, &U[1][1]);
    ibz_set_bound_ct(&u11, BLOCK_U_BITS);

    ibz_t t1 = { 0 }, t2 = { 0 }, det = { 0 };
    ibz_mul(&t1, &u00, &u11);
    ibz_set_bound_ct(&t1, 2 * BLOCK_U_BITS);
    ibz_mul(&t2, &u01, &u10);
    ibz_set_bound_ct(&t2, 2 * BLOCK_U_BITS);
    ibz_sub(&det, &t1, &t2);
    ibz_set_bound_ct(&det, 32);
    assert(ibz_cmp_int32(&det, 1) == 0 || ibz_cmp_int32(&det, -1) == 0);
    digit_t negate_det = ibz_ct_lt_mask(&det, &ibz_const_zero); // det == -1

    // STEP 1: local 2x2 Gram in orthogonalised space.
    // eta0 = u00 * 2^P + u10 * mu_old
    // |eta0| <= 2^P guarantee from LG
    ibz_t s0 = { 0 }, m0 = { 0 }, eta0 = { 0 };
    ibz_mul_2exp(&s0, &u00, (uint32_t)P);
    ibz_mul(&m0, &mu_old, &u10);
    ibz_add(&eta0, &s0, &m0);
    assert(ibz_bitsize_ct(&eta0) <= P + 1);
    ibz_set_bound_ct(&eta0, P + 2);

    // eta1 = u01 * 2^P + u11 * mu_old
    ibz_t s1 = { 0 }, m1 = { 0 }, eta1 = { 0 };
    ibz_mul_2exp(&s1, &u01, (uint32_t)P);
    ibz_set_bound_ct(&s1, FP_L_BITS);
    ibz_mul(&m1, &mu_old, &u11);
    ibz_add(&eta1, &s1, &m1);
    ibz_set_bound_ct(&m1, FP_L_BITS);
    ibz_set_bound_ct(&eta1, FP_L_BITS);
    assert(ibz_bitsize_ct(&eta1) <= P + 1);

    // eta0_sq = eta0^2 / 2^P
    // eta0_sq <= 2^P
    ibz_t eta0_sq = { 0 };
    ibz_ct_highmul_p(&eta0_sq, &eta0, &eta0, P, P + 2);
    assert(ibz_bitsize_ct(&eta0_sq) <= P + 1);

    // g00 = eta0^2 * D[k] + u10^2 * D[k+1]
    ibz_t g00 = { 0 }, g01 = { 0 };
    {
        ibz_t term1 = { 0 }, term2 = { 0 };
        ibz_ct_highmul_p(&term1, &eta0_sq, &st->D[k], P, FP_E_BITS);
        ibz_mul(&term2, &st->D[kp1], &u10);
        ibz_set_bound_ct(&term2, FP_E_BITS);
        ibz_mul(&term2, &term2, &u10);
        ibz_set_bound_ct(&term2, FP_E_BITS);
        ibz_add(&g00, &term1, &term2);
        ibz_set_bound_ct(&g00, FP_E_BITS);
    }
    // g01 = eta0 * eta1 * D[k] + u10 * u11 * D[k+1]
    {
        ibz_t eta0_eta1 = { 0 }, term1 = { 0 }, term2 = { 0 };
        ibz_ct_highmul_p(&eta0_eta1, &eta0, &eta1, P, FP_E_BITS);
        ibz_ct_highmul_p(&term1, &eta0_eta1, &st->D[k], P, FP_E_BITS);
        ibz_mul(&term2, &st->D[kp1], &u10);
        ibz_set_bound_ct(&term2, FP_E_BITS);
        ibz_mul(&term2, &term2, &u11);
        ibz_set_bound_ct(&term2, FP_E_BITS);
        ibz_add(&g01, &term1, &term2);
        ibz_set_bound_ct(&g01, FP_E_BITS);
    }

    // STEP 2: update diagonal norms and the block coefficient.
    ibz_t inv_g00 = { 0 };
    int32_t s_g00 = 0;
    ibz_ct_fp_recip(&inv_g00, &s_g00, &g00, P, FP_NR_ROUNDS);

    // L[k][k+1] = g01*2^P/g00
    ibz_t new_mu = { 0 };
    ibz_ct_highmul_s(&new_mu, &g01, &inv_g00, -s_g00, FP_L_BITS, FP_L_BITS);

    // D[k+1] = (old_Dk * old_Dkp1) / g00
    ibz_t temp_inv = { 0 };
    ibz_ct_highmul_s(&temp_inv, &st->D[k + 1], &inv_g00, -s_g00, FP_E_BITS, FP_E_BITS);
    ibz_ct_highmul_p(&st->D[kp1], &temp_inv, &st->D[k], P, FP_E_BITS);

    ibz_copy(&st->D[k], &g00);
    ibz_set_bound_ct(&st->D[k], FP_E_BITS);
    assert(ibz_is_positive(&st->D[k]) && !ibz_is_zero(&st->D[k]));
    assert(ibz_is_positive(&st->D[kp1]) && !ibz_is_zero(&st->D[kp1]));
    ibz_copy(&st->L[kp1][k], &new_mu);
    ibz_set_bound_ct(&st->L[kp1][k], FP_L_BITS);

    // STEP 3: left-tail projections (j < k).
    for (int j = 0; j < k; j++) {
        // L[k][j]
        ibz_t a1 = { 0 }, a2 = { 0 }, newLk = { 0 };
        ibz_mul(&a1, &st->L[k][j], &u00);
        ibz_set_bound_ct(&a1, FP_L_BITS);
        ibz_mul(&a2, &st->L[kp1][j], &u10);
        ibz_set_bound_ct(&a2, FP_L_BITS);
        ibz_add(&newLk, &a1, &a2);
        ibz_set_bound_ct(&newLk, FP_L_BITS);

        // L[k+1][j]
        ibz_t b1 = { 0 }, b2 = { 0 }, newLkp1 = { 0 };
        ibz_mul(&b1, &st->L[k][j], &u01);
        ibz_set_bound_ct(&b1, FP_L_BITS);
        ibz_mul(&b2, &st->L[kp1][j], &u11);
        ibz_set_bound_ct(&b2, FP_L_BITS);
        ibz_add(&newLkp1, &b1, &b2);
        ibz_set_bound_ct(&newLkp1, FP_L_BITS);

        ibz_copy(&st->L[k][j], &newLk);
        ibz_set_bound_ct(&st->L[k][j], FP_L_BITS);
        ibz_copy(&st->L[kp1][j], &newLkp1);
        ibz_set_bound_ct(&st->L[kp1][j], FP_L_BITS);
    }

    // STEP 4: right-tail projections (i > k+1),
    // using the old mu captured above.
    for (int i = kp1 + 1; i < 4; i++) {
        ibz_t old_Lik = { 0 }, old_Likp1 = { 0 };
        ibz_copy(&old_Lik, &st->L[i][k]);
        ibz_copy(&old_Likp1, &st->L[i][kp1]);

        // In current kernel mu_old isn't always size-reduced
        // in edge case where lambda1 << lambda2 in local block.
        // low-probability event but could happen along with large U?
        ibz_t M_ikp1 = { 0 };
        ibz_ct_highmul_p(&M_ikp1, &mu_old, &old_Likp1, P, FP_L_BITS);

        ibz_t e0a = { 0 }, e0b = { 0 }, eta0_L = { 0 };
        ibz_mul(&e0a, &old_Likp1, &u00);
        ibz_set_bound_ct(&e0a, FP_L_BITS);
        // Could requite FP_L_BITS + mu_old integer bitsize
        ibz_mul(&e0b, &M_ikp1, &u10);
        assert(ibz_bitsize_ct(&e0b) < FP_L_BITS); // check if issue.
        ibz_set_bound_ct(&e0b, FP_L_BITS);
        ibz_add(&eta0_L, &e0a, &e0b);
        ibz_set_bound_ct(&eta0_L, FP_L_BITS);

        ibz_t e1a = { 0 }, e1b = { 0 }, eta1_L = { 0 };
        ibz_mul(&e1a, &old_Likp1, &u01);
        ibz_set_bound_ct(&e1a, FP_L_BITS);
        ibz_mul(&e1b, &M_ikp1, &u11);
        ibz_set_bound_ct(&e1b, FP_L_BITS);
        ibz_add(&eta1_L, &e1a, &e1b);
        ibz_set_bound_ct(&eta1_L, FP_L_BITS);

        ibz_t u10_Lik = { 0 }, term_kp1 = { 0 };
        ibz_mul(&u10_Lik, &old_Lik, &u10);
        ibz_set_bound_ct(&u10_Lik, FP_L_BITS);
        ibz_sub(&term_kp1, &eta0_L, &u10_Lik);
        ibz_set_bound_ct(&term_kp1, FP_L_BITS);

        ibz_t new_Likp1 = { 0 };
        ibz_cneg(&new_Likp1, &term_kp1, negate_det);

        ibz_t u11_Lik = { 0 }, term_k = { 0 };
        ibz_mul(&u11_Lik, &old_Lik, &u11);
        ibz_set_bound_ct(&u11_Lik, FP_L_BITS);
        ibz_sub(&term_k, &u11_Lik, &eta1_L);
        ibz_set_bound_ct(&term_k, FP_L_BITS);

        ibz_t T_0 = { 0 };
        ibz_cneg(&T_0, &term_k, negate_det);

        ibz_t hm = { 0 }, new_Lik = { 0 };
        ibz_ct_highmul_p(&hm, &new_mu, &new_Likp1, P, FP_L_BITS);
        ibz_add(&new_Lik, &T_0, &hm);
        ibz_set_bound_ct(&new_Lik, FP_L_BITS);

        ibz_copy(&st->L[i][k], &new_Lik);
        ibz_set_bound_ct(&st->L[i][k], FP_L_BITS);
        ibz_copy(&st->L[i][kp1], &new_Likp1);
        ibz_set_bound_ct(&st->L[i][kp1], FP_L_BITS);
    }

    // Ainv column update. d = det_U (+-1); for every row r:
    //   Ainv[r][k]   = d*( u11*x - u01*y )
    //   Ainv[r][k+1] = d*(-u10*x + u00*y )
    // where x = Ainv[r][k], y = Ainv[r][k+1] (the OLD values).
    for (int r = 0; r < 4; r++) {
        ibz_t x = { 0 }, y = { 0 };
        ibz_copy(&x, &st->Ainv[r][k]);
        ibz_copy(&y, &st->Ainv[r][kp1]);

        ibz_t p1 = { 0 }, p2 = { 0 }, newk = { 0 };
        ibz_mul(&p1, &u11, &x);
        ibz_set_bound_ct(&p1, FP_U_BITS + BLOCK_U_BITS);
        ibz_mul(&p2, &u01, &y);
        ibz_sub(&newk, &p1, &p2);
        ibz_set_bound_ct(&newk, FP_U_BITS);
        ibz_cneg(&newk, &newk, negate_det);

        ibz_t p3 = { 0 }, p4 = { 0 }, newkp1 = { 0 };
        ibz_mul(&p3, &u10, &x);
        ibz_mul(&p4, &u00, &y);
        ibz_sub(&newkp1, &p4, &p3); // -u10*x + u00*y
        ibz_set_bound_ct(&newkp1, FP_U_BITS);
        ibz_cneg(&newkp1, &newkp1, negate_det);

        ibz_copy(&st->Ainv[r][k], &newk);
        ibz_set_bound_ct(&st->Ainv[r][k], FP_U_BITS);
        ibz_copy(&st->Ainv[r][kp1], &newkp1);
        ibz_set_bound_ct(&st->Ainv[r][kp1], FP_U_BITS);
    }
}

// ---- size-reduction with the mirrored Ainv update

// Restore |L[r][j]| <= 2^(P-1) for rows r = k..3,
// columns j < r (ascending rows, descending columns).
// Every size-reduction step L[r] -= q*L[j] is mirrored
// on Ainv as column j += q*column r.
static void
fp_size_reduce_from(fp_ldl_t *st, int k, int P)
{
    for (int r = k; r < 4; r++) {
        for (int j = r - 1; j >= 0; j--) {
            ibz_t q = { 0 };
            ibz_ct_round_shift_limb(&q, &st->L[r][j], P, FP_HEAD);

            for (int m = 0; m < j; m++) {
                assert(ibz_bitsize_ct(&st->L[j][m]) <= P);
                ibz_t prod = { 0 }, newv = { 0 };
                ibz_set_bound_ct(&st->L[j][m], FP_P_BITS); // |L[j][m]| <= 2^(P-1)
                ibz_mul(&prod, &st->L[j][m], &q);
                ibz_set_bound_ct(&prod, FP_L_BITS);
                ibz_sub(&newv, &st->L[r][m], &prod);
                ibz_set_bound_ct(&newv, FP_L_BITS);
                ibz_copy(&st->L[r][m], &newv);
            }
            { // treat m=j separately
                ibz_t prod = { 0 }, newv = { 0 };
                ibz_mul_2exp(&prod, &q, (uint32_t)P); // L[j][j]=2^P
                ibz_set_bound_ct(&prod, FP_L_BITS);
                ibz_sub(&newv, &st->L[r][j], &prod);
                ibz_set_bound_ct(&newv, FP_L_BITS);
                ibz_copy(&st->L[r][j], &newv);
            }

            // Mirror on Ainv: column j += q * column r.
            for (int i = 0; i < 4; i++) {
                ibz_t prod = { 0 }, newv = { 0 };
                ibz_mul(&prod, &st->Ainv[i][r], &q);
                ibz_set_bound_ct(&prod, FP_U_BITS + FP_HEAD);
                ibz_add(&newv, &st->Ainv[i][j], &prod);
                ibz_set_bound_ct(&newv, FP_U_BITS);
                ibz_copy(&st->Ainv[i][j], &newv);
            }
        }
    }
}

// ==== Part 2: the LLL drivers

// One fixed-cost pass over block (k, k+1): materialise block Gram -> Lagrange-Gauss -> apply U
// -> size-reduce from k. Postcondition: L is size-reduced.
// `round` is this call's index in ct_lll_tour4's fixed sequence of 3*CT_LLL_TOURS block
// reductions; lg_outer_its_for_round turns it into the LG outer-pass count. Callers outside that
// sequence pass 0.
static void
ct_reduce_block(fp_ldl_t *st, int k, int P, int round)
{
    int kp1 = k + 1;
    quat_lll_gram2_t g = { 0 };
    quat_lll_materialise_block(&g, &st->D[k], &st->D[kp1], &st->L[kp1][k], P);

    ibz_t U[2][2] = { 0 };
    quat_lll_gram_lehmer(&g, U, lg_outer_its_for_round(round));

    fp_update_block(st, k, U, P);
    fp_size_reduce_from(st, k, P);
}

// CT_LLL_TOURS tours, each visiting block positions in the fixed order 1, 0, 2 (an odd-even sweep
// over the three adjacent pairs (0,1),(1,2),(2,3) of the dimension-4 basis).
static void
ct_lll_tour4(fp_ldl_t *st, int P)
{
    // Simple odd-even BKZ-2 reduction strategy
    for (int t = 0; t < CT_LLL_TOURS; t++) {
        ct_reduce_block(st, 1, P, 3 * t + 0);
        ct_reduce_block(st, 0, P, 3 * t + 1);
        ct_reduce_block(st, 2, P, 3 * t + 2);
    }
}

// ==== Part 3: constant-time Minkowski reduction

// Working bounds for this part's intermediates.
#define MK_WORK FP_L_BITS           // lambda_j / shift intermediates
#define MK_NORM_BITS QUAT_GRAM_BITS // (FP_E_BITS + 40): Sum_j D_j*lambda_j^2
#define MK_KEY_BITS MK_NORM_BITS
#define MK_T0_BITS (FP_L_BITS - FP_P_BITS + 16)
#define MK_ATOTAL_BITS QUAT_AINV_BITS // (FP_U_BITS + 32)

#define MK_N_REAL 27
#define MK_N_TOTAL 32

static int8_t
ct_sel8(int cond, int8_t a, int8_t b)
{
    int8_t mask = (int8_t)ct_barrier_i32(-(int32_t)(cond != 0));
    return (int8_t)((a & mask) | (b & ~mask));
}

static void
ct_swap_i8(digit_t mask, int8_t *a, int8_t *b)
{
    int8_t m = (int8_t)ct_barrier_digit(mask);
    int8_t t = (int8_t)((*a ^ *b) & m);
    *a = (int8_t)(*a ^ t);
    *b = (int8_t)(*b ^ t);
}

// mask ? -v : v, branch-free. |v| <= 2 here, so the negation cannot overflow int8_t.
static int8_t
ct_cneg_i8(digit_t mask, int8_t v)
{
    int8_t m = (int8_t)ct_barrier_digit(mask);
    return (int8_t)(((int8_t)(-v) & m) | (v & ~m));
}

// ---- Gram precompute + norm evaluation / t0 floor

// G[i][j] = Sum_{k=0}^{min(i,j)} D_k * L[i][k] * L[j][k].
// Only lower-triangular part is constructed.
static void
ct_mk_build_gram(ibz_t G[4][4], const fp_ldl_t *st, int P)
{
    ibz_t DL[4][4] = { 0 };
    for (int i = 0; i < 4; i++) {
        for (int k = 0; k < i; k++)
            ibz_ct_highmul_p(&DL[k][i], &st->D[k], &st->L[i][k], P, MK_NORM_BITS);
        ibz_copy(&DL[i][i], &st->D[i]); // L[i][i] = 2^P
        ibz_set_bound_ct(&DL[i][i], MK_NORM_BITS);
    }

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j <= i; j++) {
            ibz_t acc = { 0 };
            ibz_copy(&acc, &DL[j][i]); // the k == j term, exact
            ibz_set_bound_ct(&acc, MK_NORM_BITS);
            for (int k = 0; k < j; k++) {
                ibz_t p = { 0 };
                ibz_ct_highmul_p(&p, &DL[k][i], &st->L[j][k], P, MK_NORM_BITS);
                ibz_add(&acc, &acc, &p);
                ibz_set_bound_ct(&acc, MK_NORM_BITS);
            }
            ibz_copy(&G[i][j], &acc);
        }
    }
}

// norm(t) = t^T G t.
// callers cast: const 2D-array params reject non-const arrays before C23
static void
ct_mk_eval_norm(ibz_t *norm, const ibz_t G[4][4], const int8_t t[4])
{
    ibz_t total = { 0 };
    ibz_set(&total, 0, 2);
    ibz_set_bound_ct(&total, MK_NORM_BITS);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j <= i; j++) {
            ibz_t term = { 0 };
            int32_t coeff = (int32_t)t[i] * (int32_t)t[j] * ((i == j) ? 1 : 2);
            ibz_mul_by_int_and_set_bound(&term, &G[i][j], coeff, MK_NORM_BITS);
            ibz_add(&total, &total, &term);
            ibz_set_bound_ct(&total, MK_NORM_BITS);
        }
    }

    ibz_copy(norm, &total);
}

// <t, u>_G. G is lower-triangular only, so the (i,j) and (j,i) contributions are folded into the
// single stored entry G[i][j] -- ct_mk_eval_norm's (i==j) ? 1 : 2 shortcut is valid only for
// t == u and must not be reused here. Coefficients stay tiny (|t_0| <= 2, |t_{1..3}| <= 1, so
// |coeff| <= 8): big x small only, no ibz_mul.
static void
ct_mk_eval_inner(ibz_t *ip, const ibz_t G[4][4], const int8_t t[4], const int8_t u[4])
{
    ibz_t total = { 0 };
    ibz_set(&total, 0, 2);
    ibz_set_bound_ct(&total, MK_NORM_BITS);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j <= i; j++) {
            ibz_t term = { 0 };
            int32_t coeff = (i == j) ? ((int32_t)t[i] * (int32_t)u[i])
                                     : ((int32_t)t[i] * (int32_t)u[j] + (int32_t)t[j] * (int32_t)u[i]);
            ibz_mul_by_int_and_set_bound(&term, &G[i][j], coeff, MK_NORM_BITS);
            ibz_add(&total, &total, &term);
            ibz_set_bound_ct(&total, MK_NORM_BITS);
        }
    }

    ibz_copy(ip, &total);
}

// floor(-(t1*L[1][0] + t2*L[2][0] + t3*L[3][0]) / 2^P)
static int32_t
ct_mk_t0_floor(const fp_ldl_t *st, const int8_t t123[3], int P)
{
    ibz_t shift = { 0 };
    ibz_set(&shift, 0, 2);
    ibz_set_bound_ct(&shift, MK_WORK);
    for (int j = 1; j <= 3; j++) {
        ibz_t term = { 0 };
        ibz_mul_by_int_and_set_bound(&term, &st->L[j][0], t123[j - 1], MK_WORK);
        ibz_add(&shift, &shift, &term);
        ibz_set_bound_ct(&shift, MK_WORK);
    }
    ibz_t neg_shift = { 0 }, t0f = { 0 };
    ibz_neg(&neg_shift, &shift);
    ibz_set_bound_ct(&neg_shift, MK_WORK);
    ibz_ct_shift(&t0f, &neg_shift, -P, MK_WORK, MK_T0_BITS);

    int32_t t0 = ibz_get(&t0f);
    assert(t0 >= INT8_MIN && t0 < INT8_MAX);
    return t0;
}

// ---- 27-candidate generation

// The 13 unique (t1,t2,t3) triples: one representative per +-pair of the 26 nonzero points of
// {-1,0,1}^3 .
static const int8_t MK_T_SUB[13][3] = {
    { 0,  0,  1 },
    { 0,  1, -1 },
    { 0,  1,  0 },
    { 0,  1,  1 },
    { 1, -1, -1 },
    { 1, -1,  0 },
    { 1, -1,  1 },
    { 1,  0, -1 },
    { 1,  0,  0 },
    { 1,  0,  1 },
    { 1,  1, -1 },
    { 1,  1,  0 },
    { 1,  1,  1 },
};

static void
gen_candidates(ibz_t key[MK_N_TOTAL], int8_t tv[MK_N_TOTAL][4], ibz_t G[4][4], const fp_ldl_t *st, int P)
{
    int idx = 0;

    // Possible optimization: cut down precision to +-LAMBDA.
    ct_mk_build_gram(G, st, P);

    // candidate 0: pure b_1 = (1,0,0,0)
    {
        int8_t t[4] = { 1, 0, 0, 0 };
        ct_mk_eval_norm(&key[idx], (const ibz_t(*)[4])G, t);
        for (int c = 0; c < 4; c++)
            tv[idx][c] = t[c];
        idx++;
    }

    // remaining candidates: (t2,t3,t4) in {-1,0,1}^3, nonzero, up to sign
    // recover t1 by floor + ceil size-reduction (provably covers all relevant cases)
    for (int s = 0; s < 13; s++) {
        int8_t t123[3] = { MK_T_SUB[s][0], MK_T_SUB[s][1], MK_T_SUB[s][2] };
        int32_t t0 = ct_mk_t0_floor(st, t123, P);

        for (int which = 0; which < 2; which++) { // floor, then ceil
            int8_t t[4] = { (int8_t)(t0 + which), t123[0], t123[1], t123[2] };
            ct_mk_eval_norm(&key[idx], (const ibz_t(*)[4])G, t);
            for (int c = 0; c < 4; c++)
                tv[idx][c] = t[c];
            idx++;
        }
    }
    assert(idx == MK_N_REAL); // 1 + 13*2
    ibz_t sentinel = { 0 };
    ibz_set(&sentinel, 1, 2);
    ibz_set_bound_ct(&sentinel, MK_KEY_BITS);
    ibz_mul_2exp(&sentinel, &sentinel, (uint32_t)(MK_KEY_BITS - 4));
    ibz_set_bound_ct(&sentinel, MK_KEY_BITS);
    for (int i = 0; i < idx; i++)
        assert(ibz_bitsize_ct(&key[i]) < MK_KEY_BITS - 4);

    for (; idx < MK_N_TOTAL; idx++) {
        for (int c = 0; c < 4; c++)
            tv[idx][c] = 0;
        ibz_copy(&key[idx], &sentinel);
    }
}

// bitonic sort (32 elements, ascending by key, carrying the t[4] payload)
static void
cex(ibz_t key[MK_N_TOTAL], int8_t tv[MK_N_TOTAL][4], int i, int l, int ascending)
{
    digit_t lt = ibz_ct_lt_mask(&key[l], &key[i]); // key[l] < key[i]
    digit_t swap_mask = ascending ? lt : ~lt;
    assert(ibz_get_bound(&key[i]) == ibz_get_bound(&key[l]));
    ibz_cswap(&key[i], &key[l], swap_mask);
    for (int c = 0; c < 4; c++)
        ct_swap_i8(swap_mask, &tv[i][c], &tv[l][c]);
}

static void
bitonic_sort32(ibz_t key[MK_N_TOTAL], int8_t tv[MK_N_TOTAL][4])
{
    for (int idx = 0; idx < MK_N_TOTAL; idx++)
        ibz_set_bound_ct(&key[idx], MK_KEY_BITS);

    for (int k = 2; k <= MK_N_TOTAL; k <<= 1) {
        for (int j = k >> 1; j > 0; j >>= 1) {
            for (int i = 0; i < MK_N_TOTAL; i++) {
                int l = i ^ j;
                if (l > i) {
                    int ascending = ((i & k) == 0);
                    cex(key, tv, i, l, ascending);
                }
            }
        }
    }
}

// independence selection, small t-coordinates only
// Future optimization: 0-1 candidates are already guaranteed?
static void
select_basis(int8_t Tout[4][4], const int8_t sorted_t[MK_N_TOTAL][4])
{
    for (int i = 0; i < 4; i++)
        for (int c = 0; c < 4; c++)
            Tout[i][c] = 0;

    int32_t slot_count = 0;
    for (int idx = 0; idx < MK_N_TOTAL; idx++) {
        int32_t v0 = sorted_t[idx][0], v1 = sorted_t[idx][1], v2 = sorted_t[idx][2], v3 = sorted_t[idx][3];

        // 2x2 minors candidate + row 0
        int32_t m1_0 = Tout[0][0], m1_1 = Tout[0][1], m1_2 = Tout[0][2], m1_3 = Tout[0][3];
        int32_t T01 = m1_0 * v1 - m1_1 * v0, T02 = m1_0 * v2 - m1_2 * v0, T03 = m1_0 * v3 - m1_3 * v0;
        int32_t T12 = m1_1 * v2 - m1_2 * v1, T13 = m1_1 * v3 - m1_3 * v1, T23 = m1_2 * v3 - m1_3 * v2;
        int test1 = ((T01 | T02 | T03 | T12 | T13 | T23) != 0);

        // 3x3 minors candidate + rows 0-1
        int32_t m2_0 = Tout[1][0], m2_1 = Tout[1][1], m2_2 = Tout[1][2], m2_3 = Tout[1][3];
        int32_t M01 = m1_0 * m2_1 - m1_1 * m2_0, M02 = m1_0 * m2_2 - m1_2 * m2_0, M03 = m1_0 * m2_3 - m1_3 * m2_0;
        int32_t M12 = m1_1 * m2_2 - m1_2 * m2_1, M13 = m1_1 * m2_3 - m1_3 * m2_1, M23 = m1_2 * m2_3 - m1_3 * m2_2;
        int32_t S012 = v0 * M12 - v1 * M02 + v2 * M01;
        int32_t S013 = v0 * M13 - v1 * M03 + v3 * M01;
        int32_t S023 = v0 * M23 - v2 * M03 + v3 * M02;
        int32_t S123 = v1 * M23 - v2 * M13 + v3 * M12;
        int test2 = ((S012 | S013 | S023 | S123) != 0);

        // 4x4 minor = det candidate + rows 0-2
        int32_t m3_0 = Tout[2][0], m3_1 = Tout[2][1], m3_2 = Tout[2][2], m3_3 = Tout[2][3];
        int32_t C012 = m3_0 * M12 - m3_1 * M02 + m3_2 * M01;
        int32_t C013 = m3_0 * M13 - m3_1 * M03 + m3_3 * M01;
        int32_t C023 = m3_0 * M23 - m3_2 * M03 + m3_3 * M02;
        int32_t C123 = m3_1 * M23 - m3_2 * M13 + m3_3 * M12;
        int32_t det4 = v3 * C012 - v2 * C013 + v1 * C023 - v0 * C123;
        int test3 = (det4 != 0);

        int32_t c_eq0 = ct_barrier_i32(slot_count == 0);
        int32_t c_eq1 = ct_barrier_i32(slot_count == 1);
        int32_t c_eq2 = ct_barrier_i32(slot_count == 2);
        int32_t c_eq3 = ct_barrier_i32(slot_count == 3);
        int cur_test =
            ct_select32(c_eq0, 1, ct_select32(c_eq1, test1, ct_select32(c_eq2, test2, ct_select32(c_eq3, test3, 0))));
        int room = ct_barrier_i32(slot_count < 4);
        int accept = room & cur_test;

        for (int dest = 0; dest < 4; dest++) {
            int mask = ct_barrier_i32(accept & ct_barrier_i32(slot_count == dest));
            for (int c = 0; c < 4; c++)
                Tout[dest][c] = ct_sel8(mask, sorted_t[idx][c], Tout[dest][c]);
        }
        slot_count += accept;
    }
}

// ---- inverse_transpose_4x4 (int64_t)

// V := "inverse transpose" of T: V = (T^-1)^T = (T^T)^-1
// Only applied to |T|_inf <= 2, so no overflow.
static void
ct_mk_invert_transpose_int(int64_t V[4][4], const int8_t T[4][4])
{
    int64_t U[4][4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            U[i][j] = T[i][j];

    int64_t m01 = U[0][0] * U[1][1] - U[0][1] * U[1][0];
    int64_t m02 = U[0][0] * U[1][2] - U[0][2] * U[1][0];
    int64_t m03 = U[0][0] * U[1][3] - U[0][3] * U[1][0];
    int64_t m12 = U[0][1] * U[1][2] - U[0][2] * U[1][1];
    int64_t m13 = U[0][1] * U[1][3] - U[0][3] * U[1][1];
    int64_t m23 = U[0][2] * U[1][3] - U[0][3] * U[1][2];

    int64_t n01 = U[2][0] * U[3][1] - U[2][1] * U[3][0];
    int64_t n02 = U[2][0] * U[3][2] - U[2][2] * U[3][0];
    int64_t n03 = U[2][0] * U[3][3] - U[2][3] * U[3][0];
    int64_t n12 = U[2][1] * U[3][2] - U[2][2] * U[3][1];
    int64_t n13 = U[2][1] * U[3][3] - U[2][3] * U[3][1];
    int64_t n23 = U[2][2] * U[3][3] - U[2][3] * U[3][2];

    int64_t C[4][4];
    C[0][0] = (U[1][1] * n23 - U[1][2] * n13 + U[1][3] * n12);
    C[0][1] = -(U[1][0] * n23 - U[1][2] * n03 + U[1][3] * n02);
    C[0][2] = (U[1][0] * n13 - U[1][1] * n03 + U[1][3] * n01);
    C[0][3] = -(U[1][0] * n12 - U[1][1] * n02 + U[1][2] * n01);

    C[1][0] = -(U[0][1] * n23 - U[0][2] * n13 + U[0][3] * n12);
    C[1][1] = (U[0][0] * n23 - U[0][2] * n03 + U[0][3] * n02);
    C[1][2] = -(U[0][0] * n13 - U[0][1] * n03 + U[0][3] * n01);
    C[1][3] = (U[0][0] * n12 - U[0][1] * n02 + U[0][2] * n01);

    C[2][0] = (U[3][1] * m23 - U[3][2] * m13 + U[3][3] * m12);
    C[2][1] = -(U[3][0] * m23 - U[3][2] * m03 + U[3][3] * m02);
    C[2][2] = (U[3][0] * m13 - U[3][1] * m03 + U[3][3] * m01);
    C[2][3] = -(U[3][0] * m12 - U[3][1] * m02 + U[3][2] * m01);

    C[3][0] = -(U[2][1] * m23 - U[2][2] * m13 + U[2][3] * m12);
    C[3][1] = (U[2][0] * m23 - U[2][2] * m03 + U[2][3] * m02);
    C[3][2] = -(U[2][0] * m13 - U[2][1] * m03 + U[2][3] * m01);
    C[3][3] = (U[2][0] * m12 - U[2][1] * m02 + U[2][2] * m01);

    int64_t det_U = 0;
    for (int j = 0; j < 4; j++)
        det_U += U[0][j] * C[0][j];

    assert(det_U == 1 || det_U == -1);

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            V[i][j] = C[i][j] * det_U;
}

// Sign canonicalisation such that <T[0], T[i]> > 0 for i = 1,2,3.
// With the exception of boundary cases this makes the dual reduction
// canonical over the ideal equivalence class.
static void
ct_mk_canonicalize_T(int8_t T[4][4], const ibz_t G[4][4])
{
    for (int i = 1; i < 4; i++) {
        ibz_t ip = { 0 };
        ct_mk_eval_inner(&ip, G, T[0], T[i]);
        digit_t neg = ~(digit_t)ibz_is_positive(&ip); // all-ones iff < 0
        for (int c = 0; c < 4; c++)
            T[i][c] = ct_cneg_i8(neg, T[i][c]);
    }
}

// Minkowski-reduce an LLL-reduced state: generate the 27 candidates, sort by norm (a 15-stage
// bitonic network), select 4 independent ones via masked multi-way tests on the t-coordinates
// only, then compose out->Ainv_total = Ainv * T^-1 (the total inverse unimodular transform).
static void
ct_minkowski_reduce(ct_mk_result_t *out, const fp_ldl_t *st, int P)
{
    ibz_t key[MK_N_TOTAL] = { 0 };
    int8_t tv[MK_N_TOTAL][4];

    gen_candidates(key, tv, out->G, st, P);
    bitonic_sort32(key, tv);
    select_basis(out->T, (const int8_t(*)[4])tv);
    ct_mk_canonicalize_T(out->T, (const ibz_t(*)[4])out->G);

    int64_t V[4][4];
    ct_mk_invert_transpose_int(V, (const int8_t(*)[4])out->T);
    // T^-1 = V^T, i.e. T^-1[c][j] = V[j][c].

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            ibz_t acc = { 0 };
            ibz_set(&acc, 0, 2);
            ibz_set_bound_ct(&acc, MK_ATOTAL_BITS);
            for (int c = 0; c < 4; c++) {
                ibz_t term = { 0 };
                assert(V[j][c] >= INT32_MIN && V[j][c] <= INT32_MAX);
                ibz_mul_by_int_and_set_bound(&term, &st->Ainv[i][c], (int32_t)V[j][c], MK_ATOTAL_BITS);
                ibz_add(&acc, &acc, &term);
                ibz_set_bound_ct(&acc, MK_ATOTAL_BITS);
            }
            ibz_copy(&out->Ainv_total[i][j], &acc);
        }
    }
}

// ==== Part 4: the one-call entry point

// qlapoty HNF quadruple -> Minkowski-reduced result; `st` receives the post-LLL state.
//
// Preconditions (all PUBLIC): |B|, |C|, |Dq|, |E| < 2N * 2^(FP_HEAD-1), so no reduction mod 2N is
// needed; every input's declared ibz_t bound covers its value, twoN's being at least as wide as
// the other five.
//
// SCOPE: honest-generated inputs only, failure below 2^-LAMBDA. Adversarial input with
// lambda_i << lambda_{i+1} far outside the honest range can fail to reduce.
static void
lll_dim4_qlapoty_reduce(ct_mk_result_t *out,
                        fp_ldl_t *st,
                        const ibz_t *twoN,
                        const ibz_t *B,
                        const ibz_t *C,
                        const ibz_t *Dq,
                        const ibz_t *E,
                        const ibz_t *p,
                        int P)
{
    fp_ldl_init_dual(st, twoN, B, C, Dq, E, p, P);
    ct_lll_tour4(st, P);
    ct_minkowski_reduce(out, st, P);
}

// ==== Part 5: the public ideal-level entry point

/* See lll.h. Integer and precision bounds are hardcoded and derived per security level in lll_config.h. */
void
quat_lll_dual_reduce_ideal(quat_lattice_t *reduced,
                           ibz_t gram_diag[4],
                           ibz_mat_4x4_t *Ainv,
                           const quat_lattice_t *hnf,
                           const quat_alg_t *alg)
{
    const int P = FP_P_BITS;

    // 1. the qlapoty quadruple
    ibz_t twoN = { 0 }, B = { 0 }, C = { 0 }, Dq = { 0 }, E = { 0 }, p = { 0 };
    ibz_copy(&twoN, &hnf->basis.m[0][0]);
    ibz_set_bound_ct(&twoN, QUAT_HNF_BITS);
    ibz_copy(&p, &alg->p);
    ibz_set_bound_ct(&p, QUAT_HNF_BITS);
    assert(ibz_is_positive(&twoN) && !ibz_is_zero(&twoN));
    assert(ibz_cmp(&hnf->basis.m[1][1], &twoN) == 0);
    assert(ibz_bitsize(&hnf->basis.m[0][0]) < QUAT_HNF_BITS); // twoN
    assert(ibz_bitsize(&hnf->basis.m[0][2]) < QUAT_HNF_BITS); // E
    assert(ibz_bitsize(&hnf->basis.m[0][3]) < QUAT_HNF_BITS); // Dq
    assert(ibz_bitsize(&hnf->basis.m[1][2]) < QUAT_HNF_BITS); // C
    assert(ibz_bitsize(&hnf->basis.m[1][3]) < QUAT_HNF_BITS); // B

    // The transform that will be returned: P * Ainv_total, with the column order reversed.
    ibz_t Aout[4][4] = { 0 };

    fp_ldl_t st;
    ct_mk_result_t res = { 0 };

    {
        ibz_copy(&E, &hnf->basis.m[0][2]);
        ibz_set_bound_ct(&E, QUAT_HNF_BITS);
        ibz_copy(&Dq, &hnf->basis.m[0][3]);
        ibz_set_bound_ct(&Dq, QUAT_HNF_BITS);
        ibz_copy(&C, &hnf->basis.m[1][2]);
        ibz_set_bound_ct(&C, QUAT_HNF_BITS);
        ibz_copy(&B, &hnf->basis.m[1][3]);
        ibz_set_bound_ct(&B, QUAT_HNF_BITS);
    }

    // 2. reduce
    lll_dim4_qlapoty_reduce(&res, &st, &twoN, &B, &C, &Dq, &E, &p, P);

    // 3. Aout = P * Ainv_total with the column order reversed; P is the pair-swap (2,3,0,1)
    static const int QUAT_SIGMA[4] = { 2, 3, 0, 1 };
    for (int i = 0; i < 4; i++) {
        for (int c = 0; c < 4; c++) {
            ibz_copy(&Aout[i][3 - c], &res.Ainv_total[QUAT_SIGMA[i]][c]);
            ibz_set_bound_ct(&Aout[i][3 - c], QUAT_AINVOUT_BITS);
        }
    }

    // 4. materialise the primal basis: reduced->basis = hnf->basis * Aout, which is exactly
    // quat_lattice_t's basis_out = basis_in * Ainv_total convention
    ibz_t Bout[4][4] = { 0 };
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            ibz_t acc = { 0 };
            ibz_set(&acc, 0, 2);
            ibz_set_bound_ct(&acc, QUAT_BASIS_BITS);
            for (int m = 0; m < 4; m++) {
                ibz_t hm = { 0 }, term = { 0 };
                ibz_copy(&hm, &hnf->basis.m[r][m]);
                ibz_set_bound_ct(&hm, QUAT_HNF_BITS);
                ibz_mul(&term, &hm, &Aout[m][c]);
                ibz_add(&acc, &acc, &term);
                ibz_set_bound_ct(&acc, QUAT_BASIS_BITS);
            }
            ibz_copy(&Bout[r][c], &acc);
        }
    }

    // 5. Gram diagonals of the Minkowski-reduced DUAL, in the same (reversed) order
    {
        for (int i = 0; i < 4; i++) {
            ibz_t nrm = { 0 };
            ct_mk_eval_norm(&nrm, (const ibz_t(*)[4])res.G, res.T[i]);
            ibz_set_bound_ct(&nrm, QUAT_GRAM_BITS);
            ibz_mul_2exp(&nrm, &nrm, (uint32_t)QUAT_GRAM_SHIFT);
            ibz_copy(&gram_diag[3 - i], &nrm);
        }
    }

    // 6. write the outputs
    ibz_copy(&reduced->denom, &hnf->denom);
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 4; c++) {
            ibz_copy(&reduced->basis.m[r][c], &Bout[r][c]);
            ibz_copy(&Ainv->m[r][c], &Aout[r][c]);
        }
    }
}
