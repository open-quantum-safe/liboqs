#include <biextension.h>
#include <assert.h>
#include <inttypes.h>
#include <mp.h>

/*
 * We implement the biextension arithmetic by using the cubical torsor
 * representation. For now only implement the 2^e-ladder.
 *
 * Warning: cubicalADD is off by a factor x4 with respect to the correct
 * cubical arithmetic. This does not affect the Weil pairing or the Tate
 * pairing over F_{p^2} (due to the final exponentiation), but would give
 * the wrong result if we compute the Tate pairing over F_p.
 */

// this would be exactly like xADD if PQ was 'antinormalised' as (1,z)
// Cost: 3M + 2S + 3a + 3s
// Note: if needed, cubicalDBL is simply xDBL_A24 normalized and
// costs 3M + 2S + 2a + 2s

static void
cubicalADD(ec_point_t *R, const ec_point_t *P, const ec_point_t *Q, const fp2_t *ixPQ)
{
    fp2_t t0, t1, t2, t3;

    fp2_add(&t0, &P->x, &P->z);
    fp2_sub(&t1, &P->x, &P->z);
    fp2_add(&t2, &Q->x, &Q->z);
    fp2_sub(&t3, &Q->x, &Q->z);
    fp2_mul(&t0, &t0, &t3);
    fp2_mul(&t1, &t1, &t2);
    fp2_add(&t2, &t0, &t1);
    fp2_sub(&t3, &t0, &t1);
    fp2_sqr(&R->z, &t3);
    fp2_sqr(&t2, &t2);
    fp2_mul(&R->x, ixPQ, &t2);
}

// Given cubical reps of P, Q and x(P - Q) = (1 : ixPQ)
// compute P + Q, [2]Q
// Cost: 6M + 4S + 4a + 4s
static void
cubicalDBLADD(ec_point_t *PpQ,
              ec_point_t *QQ,
              const ec_point_t *P,
              const ec_point_t *Q,
              const fp2_t *ixPQ,
              const ec_point_t *A24)
{
    // A24 = (A+2C/4C: 1)
    assert(fp2_is_one(&A24->z));

    fp2_t t0, t1, t2, t3;

    fp2_add(&t0, &P->x, &P->z);
    fp2_sub(&t1, &P->x, &P->z);
    fp2_add(&PpQ->x, &Q->x, &Q->z);
    fp2_sub(&t3, &Q->x, &Q->z);
    fp2_sqr(&t2, &PpQ->x);
    fp2_sqr(&QQ->z, &t3);
    fp2_mul(&t0, &t0, &t3);
    fp2_mul(&t1, &t1, &PpQ->x);
    fp2_add(&PpQ->x, &t0, &t1);
    fp2_sub(&t3, &t0, &t1);
    fp2_sqr(&PpQ->z, &t3);
    fp2_sqr(&PpQ->x, &PpQ->x);
    fp2_mul(&PpQ->x, ixPQ, &PpQ->x);
    fp2_sub(&t3, &t2, &QQ->z);
    fp2_mul(&QQ->x, &t2, &QQ->z);
    fp2_mul(&t0, &t3, &A24->x);
    fp2_add(&t0, &t0, &QQ->z);
    fp2_mul(&QQ->z, &t0, &t3);
}

// iterative biextension doubling
static void
biext_ladder_2e(uint32_t e,
                ec_point_t *PnQ,
                ec_point_t *nQ,
                const ec_point_t *PQ,
                const ec_point_t *Q,
                const fp2_t *ixP,
                const ec_point_t *A24)
{
    copy_point(PnQ, PQ);
    copy_point(nQ, Q);
    for (uint32_t i = 0; i < e; i++) {
        cubicalDBLADD(PnQ, nQ, PnQ, nQ, ixP, A24);
    }
}

// Compute the monodromy ratio X/Z above as a (X:Z) point to avoid a division
// We implicitly use (1,0) as a cubical point above 0_E
static void
point_ratio(ec_point_t *R, const ec_point_t *PnQ, const ec_point_t *nQ, const ec_point_t *P)
{
    // Sanity tests
    assert(ec_is_zero(nQ));
    assert(ec_is_equal(PnQ, P));

    fp2_mul(&R->x, &nQ->x, &P->x);
    fp2_copy(&R->z, &PnQ->x);
}

// Compute the cubical translation of P by a point of 2-torsion T
static void
translate(ec_point_t *P, const ec_point_t *T)
{
    // When we translate, the following three things can happen:
    // T = (A : 0) then the translation of P should be P
    // T = (0 : B) then the translation of P = (X : Z) should be (Z : X)
    // Otherwise T = (A : B) and P translates to (AX - BZ : BX - AZ)
    // We compute this in constant time by computing the generic case
    // and then using constant time swaps.
    fp2_t PX_new, PZ_new;

    {
        fp2_t t0, t1;

        // PX_new = AX - BZ
        fp2_mul(&t0, &T->x, &P->x);
        fp2_mul(&t1, &T->z, &P->z);
        fp2_sub(&PX_new, &t0, &t1);

        // PZ_new = BX - AZ
        fp2_mul(&t0, &T->z, &P->x);
        fp2_mul(&t1, &T->x, &P->z);
        fp2_sub(&PZ_new, &t0, &t1);
    }

    // When we have A zero we should return (Z : X)
    uint32_t TA_is_zero = fp2_is_zero(&T->x);
    fp2_select(&PX_new, &PX_new, &P->z, TA_is_zero);
    fp2_select(&PZ_new, &PZ_new, &P->x, TA_is_zero);

    // When we have B zero we should return (X : Z)
    uint32_t TB_is_zero = fp2_is_zero(&T->z);
    fp2_select(&PX_new, &PX_new, &P->x, TB_is_zero);
    fp2_select(&PZ_new, &PZ_new, &P->z, TB_is_zero);

    // Set the point to the desired result
    fp2_copy(&P->x, &PX_new);
    fp2_copy(&P->z, &PZ_new);
}

// Compute the biextension monodromy g_P,Q^{2^g} (in level 1) via the
// cubical arithmetic of P+2^e Q.
// The suffix _i means that we are given 1/x(P) as parameter. Warning: to
// get meaningful result when using the monodromy to compute pairings, we
// need P, Q, PQ, A24 to be normalised (this is not strictly necessary, but
// care need to be taken when they are not normalised. Only handle the
// normalised case for now)
static void
monodromy_i(ec_point_t *R, const pairing_params_t *pairing_data, bool swap_PQ)
{
    fp2_t ixP;
    ec_point_t P, Q, PnQ, nQ;

    // When we compute the Weil pairing we need both P + [2^e]Q and
    // Q + [2^e]P which we can do easily with biext_ladder_2e() below
    // we use a bool to decide wether to use Q, ixP or P, ixQ in the
    // ladder and P or Q in translation.
    if (!swap_PQ) {
        copy_point(&P, &pairing_data->P);
        copy_point(&Q, &pairing_data->Q);
        fp2_copy(&ixP, &pairing_data->ixP);
    } else {
        copy_point(&P, &pairing_data->Q);
        copy_point(&Q, &pairing_data->P);
        fp2_copy(&ixP, &pairing_data->ixQ);
    }

    // Compute the biextension ladder P + [2^e]Q
    biext_ladder_2e(pairing_data->e - 1, &PnQ, &nQ, &pairing_data->PQ, &Q, &ixP, &pairing_data->A24);
    translate(&PnQ, &nQ);
    translate(&nQ, &nQ);
    point_ratio(R, &PnQ, &nQ, &P);
}

// Normalize the points and also store 1/x(P), 1/x(Q)
static void
cubical_normalization(pairing_params_t *pairing_data, const ec_point_t *P, const ec_point_t *Q)
{
    fp2_t t[4];
    fp2_copy(&t[0], &P->x);
    fp2_copy(&t[1], &P->z);
    fp2_copy(&t[2], &Q->x);
    fp2_copy(&t[3], &Q->z);
    fp2_batched_inv(t, 4);

    // Store PZ / PX and QZ / QX
    fp2_mul(&pairing_data->ixP, &P->z, &t[0]);
    fp2_mul(&pairing_data->ixQ, &Q->z, &t[2]);

    // Store x(P), x(Q) normalised to (X/Z : 1)
    fp2_mul(&pairing_data->P.x, &P->x, &t[1]);
    fp2_mul(&pairing_data->Q.x, &Q->x, &t[3]);
    fp2_set_one(&pairing_data->P.z);
    fp2_set_one(&pairing_data->Q.z);
}

// Weil pairing, PQ should be P+Q in (X:Z) coordinates
// We assume the points are normalised correctly
static void
weil_n(fp2_t *r, const pairing_params_t *pairing_data)
{
    ec_point_t R0, R1;
    monodromy_i(&R0, pairing_data, true);
    monodromy_i(&R1, pairing_data, false);

    fp2_mul(r, &R0.x, &R1.z);
    fp2_inv(r);
    fp2_mul(r, r, &R0.z);
    fp2_mul(r, r, &R1.x);
}

// Weil pairing, PQ should be P+Q in (X:Z) coordinates
// Normalise the points and call the code above
// The code will crash (division by 0) if either P or Q is (0:1)
void
weil(fp2_t *r, uint32_t e, const ec_point_t *P, const ec_point_t *Q, const ec_point_t *PQ, ec_curve_t *E)
{
    pairing_params_t pairing_data;
    // Construct the structure for the Weil pairing
    // Set (PX/PZ : 1), (QX : QZ : 1), PZ/PX and QZ/QX
    pairing_data.e = e;
    cubical_normalization(&pairing_data, P, Q);
    copy_point(&pairing_data.PQ, PQ);

    // Ensure the input curve has A24 normalised and store
    // in a struct
    ec_curve_normalize_A24(E);
    copy_point(&pairing_data.A24, &E->A24);

    // Compute the Weil pairing e_(2^n)(P, Q)
    weil_n(r, &pairing_data);
}

// two helper functions for reducing the tate pairing
// clear_cofac clears (p + 1) // 2^f for an Fp2 value
void
clear_cofac(fp2_t *r, const fp2_t *a)
{
    digit_t exp = *p_cofactor_for_2f;
    exp >>= 1;

    fp2_t x;
    fp2_copy(&x, a);
    fp2_copy(r, a);

    // removes cofac
    while (exp > 0) {
        fp2_sqr(r, r);
        if (exp & 1) {
            fp2_mul(r, r, &x);
        }
        exp >>= 1;
    }
}

// applies frobenius a + ib --> a - ib to an fp2 element
void
fp2_frob(fp2_t *out, const fp2_t *in)
{
    fp_copy(&(out->re), &(in->re));
    fp_neg(&(out->im), &(in->im));
}

// reduced Tate pairing, normalizes the points, assumes PQ is P+Q in (X:Z)
// coordinates. Computes 1/x(P) and 1/x(Q) for efficient cubical ladder
void
reduced_tate(fp2_t *r, uint32_t e, const ec_point_t *P, const ec_point_t *Q, const ec_point_t *PQ, ec_curve_t *E)
{
    uint32_t e_full = TORSION_EVEN_POWER;
    uint32_t e_diff = e_full - e;
    ec_point_t R;
    pairing_params_t pairing_data;

    // Construct the structure for the Weil pairing
    // Set (PX/PZ : 1), (QX : QZ : 1), PZ/PX and QZ/QX
    pairing_data.e = e;
    cubical_normalization(&pairing_data, P, Q);
    copy_point(&pairing_data.PQ, PQ);

    // Ensure the input curve has A24 normalised and store
    // in a struct
    ec_curve_normalize_A24(E);
    copy_point(&pairing_data.A24, &E->A24);

    monodromy_i(&R, &pairing_data, true);

    // we get unreduced tate as R.X, R.Z
    // reduced tate is -(R.Z/R.X)^((p^2 - 1) div 2^f)
    //  we reuse R.X and R.Z to split reduction step ^(p-1) into frobenius and ^-1
    fp2_t frob, tmp;
    fp2_copy(&tmp, &R.x);
    fp2_frob(&frob, &R.x);
    fp2_mul(&R.x, &R.z, &frob);
    fp2_frob(&frob, &R.z);
    fp2_mul(&R.z, &tmp, &frob);
    fp2_inv(&R.x);
    fp2_mul(r, &R.x, &R.z);

    clear_cofac(r, r);
    // clear remaining 2^e_diff
    for (uint32_t j = 0; j < e_diff; j++) {
        fp2_sqr(r, r);
    }
}

// Functions to compute discrete logs by computing the Weil pairing of points
// followed by computing the dlog in Fp^2
// (If we work with full order points, it would be faster to use the Tate
// pairings rather than the Weil pairings; this is not implemented yet)

// recursive dlog function
static bool
fp2_dlog_2e_rec(digit_t *a, long len, fp2_t *pows_f, fp2_t *pows_g, long stacklen)
{
    if (len == 0) {
        // *a = 0;
        for (int i = 0; i < NWORDS_ORDER; i++) {
            a[i] = 0;
        }
        return true;
    } else if (len == 1) {
        if (fp2_is_one(&pows_f[stacklen - 1])) {
            // a = 0;
            for (int i = 0; i < NWORDS_ORDER; i++) {
                a[i] = 0;
            }
            for (int i = 0; i < stacklen - 1; ++i) {
                fp2_sqr(&pows_g[i], &pows_g[i]); // new_g = g^2
            }
            return true;
        } else if (fp2_is_equal(&pows_f[stacklen - 1], &pows_g[stacklen - 1])) {
            // a = 1;
            a[0] = 1;
            for (int i = 1; i < NWORDS_ORDER; i++) {
                a[i] = 0;
            }
            for (int i = 0; i < stacklen - 1; ++i) {
                fp2_mul(&pows_f[i], &pows_f[i], &pows_g[i]); // new_f = f*g
                fp2_sqr(&pows_g[i], &pows_g[i]);             // new_g = g^2
            }
            return true;
        } else {
            return false;
        }
    } else {
        long right = (double)len * 0.5;
        long left = len - right;
        pows_f[stacklen] = pows_f[stacklen - 1];
        pows_g[stacklen] = pows_g[stacklen - 1];
        for (int i = 0; i < left; i++) {
            fp2_sqr(&pows_f[stacklen], &pows_f[stacklen]);
            fp2_sqr(&pows_g[stacklen], &pows_g[stacklen]);
        }
        // uint32_t dlp1 = 0, dlp2 = 0;
        digit_t dlp1[NWORDS_ORDER], dlp2[NWORDS_ORDER];
        bool ok;
        ok = fp2_dlog_2e_rec(dlp1, right, pows_f, pows_g, stacklen + 1);
        if (!ok)
            return false;
        ok = fp2_dlog_2e_rec(dlp2, left, pows_f, pows_g, stacklen);
        if (!ok)
            return false;
        // a = dlp1 + 2^right * dlp2
        multiple_mp_shiftl(dlp2, right, NWORDS_ORDER);
        mp_add(a, dlp2, dlp1, NWORDS_ORDER);

        return true;
    }
}

// compute DLP: compute scal such that f = g^scal with f, 1/g as input
static bool
fp2_dlog_2e(digit_t *scal, const fp2_t *f, const fp2_t *g_inverse, int e)
{
    long log, len = e;
    for (log = 0; len > 1; len >>= 1)
        log++;
    log += 1;

    fp2_t pows_f[log], pows_g[log];
    pows_f[0] = *f;
    pows_g[0] = *g_inverse;

    for (int i = 0; i < NWORDS_ORDER; i++) {
        scal[i] = 0;
    }

    bool ok = fp2_dlog_2e_rec(scal, e, pows_f, pows_g, 1);
    assert(ok);

    return ok;
}

// Normalize the bases (P, Q), (R, S) and store their inverse
// and additionally normalise the curve to (A/C : 1)
static void
cubical_normalization_dlog(pairing_dlog_params_t *pairing_dlog_data, ec_curve_t *curve)
{
    fp2_t t[11];
    ec_basis_t *PQ = &pairing_dlog_data->PQ;
    ec_basis_t *RS = &pairing_dlog_data->RS;
    fp2_copy(&t[0], &PQ->P.x);
    fp2_copy(&t[1], &PQ->P.z);
    fp2_copy(&t[2], &PQ->Q.x);
    fp2_copy(&t[3], &PQ->Q.z);
    fp2_copy(&t[4], &PQ->PmQ.x);
    fp2_copy(&t[5], &PQ->PmQ.z);
    fp2_copy(&t[6], &RS->P.x);
    fp2_copy(&t[7], &RS->P.z);
    fp2_copy(&t[8], &RS->Q.x);
    fp2_copy(&t[9], &RS->Q.z);
    fp2_copy(&t[10], &curve->C);

    fp2_batched_inv(t, 11);

    fp2_mul(&pairing_dlog_data->ixP, &PQ->P.z, &t[0]);
    fp2_mul(&PQ->P.x, &PQ->P.x, &t[1]);
    fp2_set_one(&PQ->P.z);

    fp2_mul(&pairing_dlog_data->ixQ, &PQ->Q.z, &t[2]);
    fp2_mul(&PQ->Q.x, &PQ->Q.x, &t[3]);
    fp2_set_one(&PQ->Q.z);

    fp2_mul(&PQ->PmQ.x, &PQ->PmQ.x, &t[5]);
    fp2_set_one(&PQ->PmQ.z);

    fp2_mul(&pairing_dlog_data->ixR, &RS->P.z, &t[6]);
    fp2_mul(&RS->P.x, &RS->P.x, &t[7]);
    fp2_set_one(&RS->P.z);

    fp2_mul(&pairing_dlog_data->ixS, &RS->Q.z, &t[8]);
    fp2_mul(&RS->Q.x, &RS->Q.x, &t[9]);
    fp2_set_one(&RS->Q.z);

    fp2_mul(&curve->A, &curve->A, &t[10]);
    fp2_set_one(&curve->C);
}

// Given two bases <P, Q> and basis = <R, S> compute
// x(P - R), x(P - S), x(R - Q), x(S - Q)
static void
compute_difference_points(pairing_dlog_params_t *pairing_dlog_data, ec_curve_t *curve)
{
    jac_point_t xyP, xyQ, xyR, xyS, temp;

    // lifting the two basis points, assumes that x(P) and x(R)
    // and the curve itself are normalised to (X : 1)
    lift_basis_normalized(&xyP, &xyQ, &pairing_dlog_data->PQ, curve);
    lift_basis_normalized(&xyR, &xyS, &pairing_dlog_data->RS, curve);

    // computation of the differences
    // x(P - R)
    jac_neg(&temp, &xyR);
    ADD(&temp, &temp, &xyP, curve);
    jac_to_xz(&pairing_dlog_data->diff.PmR, &temp);

    // x(P - S)
    jac_neg(&temp, &xyS);
    ADD(&temp, &temp, &xyP, curve);
    jac_to_xz(&pairing_dlog_data->diff.PmS, &temp);

    // x(R - Q)
    jac_neg(&temp, &xyQ);
    ADD(&temp, &temp, &xyR, curve);
    jac_to_xz(&pairing_dlog_data->diff.RmQ, &temp);

    // x(S - Q)
    jac_neg(&temp, &xyQ);
    ADD(&temp, &temp, &xyS, curve);
    jac_to_xz(&pairing_dlog_data->diff.SmQ, &temp);
}

// Inline all the Weil pairing computations needed for ec_dlog_2_weil
static void
weil_dlog(digit_t *r1, digit_t *r2, digit_t *s1, digit_t *s2, pairing_dlog_params_t *pairing_dlog_data)
{

    ec_point_t nP, nQ, nR, nS, nPQ, PnQ, nPR, PnR, nPS, PnS, nRQ, RnQ, nSQ, SnQ;

    copy_point(&nP, &pairing_dlog_data->PQ.P);
    copy_point(&nQ, &pairing_dlog_data->PQ.Q);
    copy_point(&nR, &pairing_dlog_data->RS.P);
    copy_point(&nS, &pairing_dlog_data->RS.Q);
    copy_point(&nPQ, &pairing_dlog_data->PQ.PmQ);
    copy_point(&PnQ, &pairing_dlog_data->PQ.PmQ);
    copy_point(&nPR, &pairing_dlog_data->diff.PmR);
    copy_point(&nPS, &pairing_dlog_data->diff.PmS);
    copy_point(&PnR, &pairing_dlog_data->diff.PmR);
    copy_point(&PnS, &pairing_dlog_data->diff.PmS);
    copy_point(&nRQ, &pairing_dlog_data->diff.RmQ);
    copy_point(&nSQ, &pairing_dlog_data->diff.SmQ);
    copy_point(&RnQ, &pairing_dlog_data->diff.RmQ);
    copy_point(&SnQ, &pairing_dlog_data->diff.SmQ);

    for (uint32_t i = 0; i < pairing_dlog_data->e - 1; i++) {
        cubicalADD(&nPQ, &nPQ, &nP, &pairing_dlog_data->ixQ);
        cubicalADD(&nPR, &nPR, &nP, &pairing_dlog_data->ixR);
        cubicalDBLADD(&nPS, &nP, &nPS, &nP, &pairing_dlog_data->ixS, &pairing_dlog_data->A24);

        cubicalADD(&PnQ, &PnQ, &nQ, &pairing_dlog_data->ixP);
        cubicalADD(&RnQ, &RnQ, &nQ, &pairing_dlog_data->ixR);
        cubicalDBLADD(&SnQ, &nQ, &SnQ, &nQ, &pairing_dlog_data->ixS, &pairing_dlog_data->A24);

        cubicalADD(&PnR, &PnR, &nR, &pairing_dlog_data->ixP);
        cubicalDBLADD(&nRQ, &nR, &nRQ, &nR, &pairing_dlog_data->ixQ, &pairing_dlog_data->A24);

        cubicalADD(&PnS, &PnS, &nS, &pairing_dlog_data->ixP);
        cubicalDBLADD(&nSQ, &nS, &nSQ, &nS, &pairing_dlog_data->ixQ, &pairing_dlog_data->A24);
    }

    // weil(&w0,e,&PQ->P,&PQ->Q,&PQ->PmQ,&A24);
    translate(&nPQ, &nP);
    translate(&nPR, &nP);
    translate(&nPS, &nP);
    translate(&PnQ, &nQ);
    translate(&RnQ, &nQ);
    translate(&SnQ, &nQ);
    translate(&PnR, &nR);
    translate(&nRQ, &nR);
    translate(&PnS, &nS);
    translate(&nSQ, &nS);

    translate(&nP, &nP);
    translate(&nQ, &nQ);
    translate(&nR, &nR);
    translate(&nS, &nS);

    // computation of the reference weil pairing
    ec_point_t T0, T1;
    fp2_t w1[5], w2[5];

    // e(P, Q) = w0
    point_ratio(&T0, &nPQ, &nP, &pairing_dlog_data->PQ.Q);
    point_ratio(&T1, &PnQ, &nQ, &pairing_dlog_data->PQ.P);
    // For the first element we need it's inverse for
    // fp2_dlog_2e so we swap w1 and w2 here to save inversions
    fp2_mul(&w2[0], &T0.x, &T1.z);
    fp2_mul(&w1[0], &T1.x, &T0.z);

    // e(P,R) = w0^r2
    point_ratio(&T0, &nPR, &nP, &pairing_dlog_data->RS.P);
    point_ratio(&T1, &PnR, &nR, &pairing_dlog_data->PQ.P);
    fp2_mul(&w1[1], &T0.x, &T1.z);
    fp2_mul(&w2[1], &T1.x, &T0.z);

    // e(R,Q) = w0^r1
    point_ratio(&T0, &nRQ, &nR, &pairing_dlog_data->PQ.Q);
    point_ratio(&T1, &RnQ, &nQ, &pairing_dlog_data->RS.P);
    fp2_mul(&w1[2], &T0.x, &T1.z);
    fp2_mul(&w2[2], &T1.x, &T0.z);

    // e(P,S) = w0^s2
    point_ratio(&T0, &nPS, &nP, &pairing_dlog_data->RS.Q);
    point_ratio(&T1, &PnS, &nS, &pairing_dlog_data->PQ.P);
    fp2_mul(&w1[3], &T0.x, &T1.z);
    fp2_mul(&w2[3], &T1.x, &T0.z);

    // e(S,Q) = w0^s1
    point_ratio(&T0, &nSQ, &nS, &pairing_dlog_data->PQ.Q);
    point_ratio(&T1, &SnQ, &nQ, &pairing_dlog_data->RS.Q);
    fp2_mul(&w1[4], &T0.x, &T1.z);
    fp2_mul(&w2[4], &T1.x, &T0.z);

    fp2_batched_inv(w1, 5);
    for (int i = 0; i < 5; i++) {
        fp2_mul(&w1[i], &w1[i], &w2[i]);
    }

    fp2_dlog_2e(r2, &w1[1], &w1[0], pairing_dlog_data->e);
    fp2_dlog_2e(r1, &w1[2], &w1[0], pairing_dlog_data->e);
    fp2_dlog_2e(s2, &w1[3], &w1[0], pairing_dlog_data->e);
    fp2_dlog_2e(s1, &w1[4], &w1[0], pairing_dlog_data->e);
}

void
ec_dlog_2_weil(digit_t *r1,
               digit_t *r2,
               digit_t *s1,
               digit_t *s2,
               ec_basis_t *PQ,
               const ec_basis_t *RS,
               ec_curve_t *curve,
               int e)
{
    assert(test_point_order_twof(&PQ->Q, curve, e));

    // precomputing the correct curve data
    ec_curve_normalize_A24(curve);

    pairing_dlog_params_t pairing_dlog_data;
    pairing_dlog_data.e = e;
    pairing_dlog_data.PQ = *PQ;
    pairing_dlog_data.RS = *RS;
    pairing_dlog_data.A24 = curve->A24;

    cubical_normalization_dlog(&pairing_dlog_data, curve);
    compute_difference_points(&pairing_dlog_data, curve);

    weil_dlog(r1, r2, s1, s2, &pairing_dlog_data);

#ifndef NDEBUG
    ec_point_t test;
    ec_biscalar_mul(&test, r1, r2, e, PQ, curve);
    // R = [r1]P + [r2]Q
    assert(ec_is_equal(&test, &RS->P));
    ec_biscalar_mul(&test, s1, s2, e, PQ, curve);
    // S = [s1]P + [s2]Q
    assert(ec_is_equal(&test, &RS->Q));
#endif
}

// Inline all the Tate pairing computations needed for ec_dlog_2_weil
// including reduction, assumes a bases PQ of full E[2^e_full] torsion
// and a bases RS of smaller E[2^e] torsion
static void
tate_dlog_partial(digit_t *r1, digit_t *r2, digit_t *s1, digit_t *s2, pairing_dlog_params_t *pairing_dlog_data)
{

    uint32_t e_full = TORSION_EVEN_POWER;
    uint32_t e_diff = e_full - pairing_dlog_data->e;

    ec_point_t nP, nQ, nR, nS, nPQ, PnR, PnS, nRQ, nSQ;

    copy_point(&nP, &pairing_dlog_data->PQ.P);
    copy_point(&nQ, &pairing_dlog_data->PQ.Q);
    copy_point(&nR, &pairing_dlog_data->RS.P);
    copy_point(&nS, &pairing_dlog_data->RS.Q);
    copy_point(&nPQ, &pairing_dlog_data->PQ.PmQ);
    copy_point(&PnR, &pairing_dlog_data->diff.PmR);
    copy_point(&PnS, &pairing_dlog_data->diff.PmS);
    copy_point(&nRQ, &pairing_dlog_data->diff.RmQ);
    copy_point(&nSQ, &pairing_dlog_data->diff.SmQ);

    for (uint32_t i = 0; i < e_full - 1; i++) {
        cubicalDBLADD(&nPQ, &nP, &nPQ, &nP, &pairing_dlog_data->ixQ, &pairing_dlog_data->A24);
    }

    for (uint32_t i = 0; i < pairing_dlog_data->e - 1; i++) {
        cubicalADD(&PnR, &PnR, &nR, &pairing_dlog_data->ixP);
        cubicalDBLADD(&nRQ, &nR, &nRQ, &nR, &pairing_dlog_data->ixQ, &pairing_dlog_data->A24);

        cubicalADD(&PnS, &PnS, &nS, &pairing_dlog_data->ixP);
        cubicalDBLADD(&nSQ, &nS, &nSQ, &nS, &pairing_dlog_data->ixQ, &pairing_dlog_data->A24);
    }

    translate(&nPQ, &nP);
    translate(&PnR, &nR);
    translate(&nRQ, &nR);
    translate(&PnS, &nS);
    translate(&nSQ, &nS);

    translate(&nP, &nP);
    translate(&nQ, &nQ);
    translate(&nR, &nR);
    translate(&nS, &nS);

    // computation of the reference Tate pairing
    ec_point_t T0;
    fp2_t w1[5], w2[5];

    // t(P, Q)^(2^e_diff) = w0
    point_ratio(&T0, &nPQ, &nP, &pairing_dlog_data->PQ.Q);
    fp2_copy(&w1[0], &T0.x);
    fp2_copy(&w2[0], &T0.z);

    // t(R,P) = w0^r2
    point_ratio(&T0, &PnR, &nR, &pairing_dlog_data->PQ.P);
    fp2_copy(&w1[1], &T0.x);
    fp2_copy(&w2[1], &T0.z);

    // t(R,Q) = w0^r1
    point_ratio(&T0, &nRQ, &nR, &pairing_dlog_data->PQ.Q);
    fp2_copy(&w2[2], &T0.x);
    fp2_copy(&w1[2], &T0.z);

    // t(S,P) = w0^s2
    point_ratio(&T0, &PnS, &nS, &pairing_dlog_data->PQ.P);
    fp2_copy(&w1[3], &T0.x);
    fp2_copy(&w2[3], &T0.z);

    // t(S,Q) = w0^s1
    point_ratio(&T0, &nSQ, &nS, &pairing_dlog_data->PQ.Q);
    fp2_copy(&w2[4], &T0.x);
    fp2_copy(&w1[4], &T0.z);

    // batched reduction using projective representation
    for (int i = 0; i < 5; i++) {
        fp2_t frob, tmp;
        fp2_copy(&tmp, &w1[i]);
        // inline frobenius for ^p
        // multiply by inverse to get ^(p-1)
        fp2_frob(&frob, &w1[i]);
        fp2_mul(&w1[i], &w2[i], &frob);

        // repeat for denom
        fp2_frob(&frob, &w2[i]);
        fp2_mul(&w2[i], &tmp, &frob);
    }

    // batched normalization
    fp2_batched_inv(w2, 5);
    for (int i = 0; i < 5; i++) {
        fp2_mul(&w1[i], &w1[i], &w2[i]);
    }

    for (int i = 0; i < 5; i++) {
        clear_cofac(&w1[i], &w1[i]);

        // removes 2^e_diff
        for (uint32_t j = 0; j < e_diff; j++) {
            fp2_sqr(&w1[i], &w1[i]);
        }
    }

    fp2_dlog_2e(r2, &w1[1], &w1[0], pairing_dlog_data->e);
    fp2_dlog_2e(r1, &w1[2], &w1[0], pairing_dlog_data->e);
    fp2_dlog_2e(s2, &w1[3], &w1[0], pairing_dlog_data->e);
    fp2_dlog_2e(s1, &w1[4], &w1[0], pairing_dlog_data->e);
}

void
ec_dlog_2_tate(digit_t *r1,
               digit_t *r2,
               digit_t *s1,
               digit_t *s2,
               const ec_basis_t *PQ,
               const ec_basis_t *RS,
               ec_curve_t *curve,
               int e)
{
    // assume PQ is a full torsion basis
    // returns a, b, c, d such that R = [a]P + [b]Q, S = [c]P + [d]Q

#ifndef NDEBUG
    int e_full = TORSION_EVEN_POWER;
    int e_diff = e_full - e;
#endif
    assert(test_basis_order_twof(PQ, curve, e_full));

    // precomputing the correct curve data
    ec_curve_normalize_A24(curve);

    pairing_dlog_params_t pairing_dlog_data;
    pairing_dlog_data.e = e;
    pairing_dlog_data.PQ = *PQ;
    pairing_dlog_data.RS = *RS;
    pairing_dlog_data.A24 = curve->A24;

    cubical_normalization_dlog(&pairing_dlog_data, curve);
    compute_difference_points(&pairing_dlog_data, curve);
    tate_dlog_partial(r1, r2, s1, s2, &pairing_dlog_data);

#ifndef NDEBUG
    ec_point_t test;
    ec_biscalar_mul(&test, r1, r2, e, PQ, curve);
    ec_dbl_iter(&test, e_diff, &test, curve);
    // R = [r1]P + [r2]Q
    assert(ec_is_equal(&test, &RS->P));

    ec_biscalar_mul(&test, s1, s2, e, PQ, curve);
    ec_dbl_iter(&test, e_diff, &test, curve);
    // S = [s1]P + [s2]Q
    assert(ec_is_equal(&test, &RS->Q));
#endif
}
