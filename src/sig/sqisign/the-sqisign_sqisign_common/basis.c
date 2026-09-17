#include "ec.h"
#include "fp2.h"
#include "e0_basis.h"
#include <assert.h>

#include <ct_testing.h>

static void
difference_point(ec_point_t *PQ, ec_point_t *PmQ, ec_point_t *P, ec_point_t *Q, ec_curve_t *curve)
{
    // Given P,Q in projective x-only, computes a deterministic choice for (P-Q)
    // Based on Proposition 3 of https://eprint.iacr.org/2017/518.pdf

    fp2_t Bxx, Bxz, Bzz, t0, t1;

    fp2_mul(&t0, &P->x, &Q->x);
    fp2_mul(&t1, &P->z, &Q->z);
    fp2_sub(&Bxx, &t0, &t1);
    fp2_sqr(&Bxx, &Bxx);
    fp2_mul(&Bxx, &Bxx, &curve->C); // C*(P.x*Q.x-P.z*Q.z)^2
    fp2_add(&Bxz, &t0, &t1);
    fp2_mul(&t0, &P->x, &Q->z);
    fp2_mul(&t1, &P->z, &Q->x);
    fp2_add(&Bzz, &t0, &t1);
    fp2_mul(&Bxz, &Bxz, &Bzz); // (P.x*Q.x+P.z*Q.z)(P.x*Q.z+P.z*Q.x)
    fp2_sub(&Bzz, &t0, &t1);
    fp2_sqr(&Bzz, &Bzz);
    fp2_mul(&Bzz, &Bzz, &curve->C); // C*(P.x*Q.z-P.z*Q.x)^2
    fp2_mul(&Bxz, &Bxz, &curve->C); // C*(P.x*Q.x+P.z*Q.z)(P.x*Q.z+P.z*Q.x)
    fp2_mul(&t0, &t0, &t1);
    fp2_mul(&t0, &t0, &curve->A);
    fp2_add(&t0, &t0, &t0);
    fp2_add(&Bxz, &Bxz, &t0); // C*(P.x*Q.x+P.z*Q.z)(P.x*Q.z+P.z*Q.x) + 2*A*P.x*Q.z*P.z*Q.x

    // To ensure that the denominator is a fourth power in Fp, we normalize by C*C_bar^2*(P.x*Q.z-P.z*Q.x)bar^2
    fp_copy(&t0.re, &curve->C.re);
    fp_neg(&t0.im, &curve->C.im);
    fp2_mul(&t0, &t0, &curve->C); // C * C_bar

    fp_copy(&t1.re, &Bzz.re);
    fp_neg(&t1.im, &Bzz.im);
    fp2_mul(&t0, &t0, &t1); // C*C_bar^2*(P.x*Q.z-P.z*Q.x)bar^2

    fp2_mul(&Bxx, &Bxx, &t0);
    fp2_mul(&Bxz, &Bxz, &t0);
    fp2_mul(&Bzz, &Bzz, &t0);

    // Solving quadratic equation
    fp2_sqr(&t0, &Bxz);
    fp2_mul(&t1, &Bxx, &Bzz);
    fp2_sub(&t0, &t0, &t1);
    // No need to check if t0 is square, as per the entangled basis algorithm.
    fp2_sqrt(&t0);
    fp2_add(&PQ->x, &Bxz, &t0);
    fp2_sub(&PmQ->x, &Bxz, &t0);
    fp2_copy(&PQ->z, &Bzz);
    fp2_copy(&PmQ->z, &Bzz);
}

// Given an x-coordinate, determines if this is a valid point on the curve. Assumes C=1.
static uint32_t
is_on_curve(const fp2_t *x, const ec_curve_t *curve)
{
    assert(fp2_is_one(&curve->C));
    fp2_t t0;

    fp2_add(&t0, x, &curve->A); // x + (A/C)
    fp2_mul(&t0, &t0, x);       // x^2 + (A/C)*x
    fp2_add_one(&t0, &t0);      // x^2 + (A/C)*x + 1
    fp2_mul(&t0, &t0, x);       // x^3 + (A/C)*x^2 + x

    return fp2_is_square(&t0);
}

// Helper function which given a point of order k*2^n with n maximal and k odd, computes a point of order 2^f
static inline void
clear_cofactor_for_maximal_even_order(ec_point_t *P, ec_curve_t *curve, int f)
{
    // clear out the odd cofactor to get a point of order 2^n
    ec_mul(P, p_cofactor_for_2f, P_COFACTOR_FOR_2F_BITLENGTH, P, curve);

    // clear the power of two to get a point of order 2^f
    for (int i = 0; i < TORSION_EVEN_POWER - f; i++) {
        ec_xDBL_A24(P, P, &curve->A24, curve->is_A24_computed_and_normalized);
    }
}

// Helper function which finds an NQR -1 / (1 + i*b) for entangled basis generation
static uint8_t
find_nqr_factor(ec_point_t *P, ec_curve_t *curve, const uint8_t start)
{
    // factor = -1/(1 + i*b) for b in Fp will be NQR whenever 1 + b^2 is NQR in Fp, so we find one of these and then
    // invert (1 + i*b). We store b as a u8 hint to save time in verification.

    // We return the hint as a u8, but use (uint16_t)n to give 2^16 - 1 to make failure cryptographically negligible,
    // with a fallback when n > 128 is required.
    uint8_t hint;
    uint32_t found = 0;
    uint16_t n = start;

    bool qr_b = 1;
    fp_t b, tmp;
    fp2_t z, t0, t1;

    do {
        while (qr_b) {
            // find b with 1 + b^2 a non-quadratic residue
            fp_set_small(&tmp, (uint32_t)n * n + 1);
            qr_b = fp_is_square(&tmp);
            n++; // keeps track of b = n - 1
        }

        // for Px := -A/(1 + i*b) to be on the curve is equivalent to A^2*(z-1) - z^2 NQR for z = 1 + i*b
        // thus prevents unnecessary inversion pre-check

        // t0 = z - 1 = i*b
        // t1 = z = 1 + i*b
        fp_set_small(&b, (uint32_t)n - 1);
        fp2_set_zero(&t0);
        fp2_set_one(&z);
        fp_copy(&z.im, &b);
        fp_copy(&t0.im, &b);

        // A^2*(z-1) - z^2
        fp2_sqr(&t1, &curve->A);
        fp2_mul(&t0, &t0, &t1); // A^2 * (z - 1)
        fp2_sqr(&t1, &z);
        fp2_sub(&t0, &t0, &t1); // A^2 * (z - 1) - z^2
        found = !fp2_is_square(&t0);
        // The search trajectory (and the accepted b) is the published basis hint, so the found bit is public;
        // the curve itself stays tainted
        CT_TESTING_MAKE_PUBLIC(&found, sizeof(found));

        qr_b = 1;
    } while (!found);

    // set P to -A : (1 + i*b)
    fp2_copy(&P->x, &curve->A);
    fp2_neg(&P->x, &P->x);
    fp2_copy(&P->z, &z);

    /*
     * With very low probability n will not fit in 7 bits.
     * We set hint = 0 which signals failure and the need
     * to generate a value on the fly during verification
     */
    hint = n <= 128 ? n - 1 : 0;

    return hint;
}

// Helper function which sets P, Q using that A is a nqr
static uint8_t
PQ_from_nqr(ec_point_t *P, ec_point_t *Q, ec_curve_t *curve, const uint8_t start)
{
    uint8_t hint = find_nqr_factor(P, curve, start);

    // Q has x-coordinate -(x(P) + A) which we set projectively as Q = (-(xP + A*zP) : zP)
    fp2_mul(&Q->x, &curve->A, &P->z);
    fp2_add(&Q->x, &Q->x, &P->x);
    fp2_neg(&Q->x, &Q->x);
    fp2_copy(&Q->z, &P->z);

    return hint;
}

// Helper function which finds a point x(P) = n * A
static uint8_t
find_nA_x_coord(fp2_t *x, ec_curve_t *curve, const uint8_t start)
{
    // The hint claims A is a NQR but is untrusted; a wrong claim is rejected later.
#ifndef NDEBUG
    if (fp2_is_square(&curve->A))
        debug_print("A is a square, so the basis hint is inconsistent");
#endif

    // when A is NQR we allow x(P) to be a multiple n*A of A
    uint8_t n = start;
    if (n == 1) {
        fp2_copy(x, &curve->A);
    } else {
        fp2_mul_small(x, &curve->A, n);
    }

    while (1) {
        uint32_t on_curve = is_on_curve(x, curve);
        // The search trajectory (and the accepted n) is the published basis hint, so this bit is public;
        // the curve itself stays tainted
        CT_TESTING_MAKE_PUBLIC(&on_curve, sizeof(on_curve));
        if (on_curve)
            break;
        fp2_add(x, x, &curve->A);
        n++;
    }

    /*
     * With very low probability (1/2^128), n will not fit in 7 bits. In this case, we set hint = 0 which signals
     * failure and the need to generate a value on the fly during verification
     */
    uint8_t hint = n < 128 ? n : 0;
    return hint;
}

// Helper function which sets P, Q using that A is a qr
static uint8_t
PQ_from_qr(ec_point_t *P, ec_point_t *Q, ec_curve_t *curve, const uint8_t start)
{
    uint8_t hint = find_nA_x_coord(&P->x, curve, start);
    fp2_set_one(&P->z);

    // Q = (-(A + xP) : 1)
    fp2_add(&Q->x, &curve->A, &P->x);
    fp2_neg(&Q->x, &Q->x);
    fp2_set_one(&Q->z);

    return hint;
}

// The entangled basis generation does not allow A = 0 so we simply return the one we have already precomputed
static void
ec_basis_E0_2f(ec_basis_t *PQ2, ec_curve_t *curve, int f)
{
    assert(fp2_is_zero(&curve->A));
    ec_point_t P, Q;

    // Set P, Q to precomputed (X : 1) values
    fp2_copy(&P.x, &BASIS_E0_PX);
    fp2_copy(&Q.x, &BASIS_E0_QX);
    fp2_set_one(&P.z);
    fp2_set_one(&Q.z);

    // clear the power of two to get a point of order 2^f
    for (int i = 0; i < TORSION_EVEN_POWER - f; i++) {
        ec_xDBL_E0(&P, &P);
        ec_xDBL_E0(&Q, &Q);
    }

    // Set P, Q in the basis and compute x(P - Q)
    ec_copy_point(&PQ2->P, &P);
    ec_copy_point(&PQ2->Q, &Q);
    difference_point(&P, &PQ2->PmQ, &PQ2->P, &PQ2->Q, curve);
}

// Computes a basis E[2^f] = <P, Q> where the point Q is above (0 : 0) and
// stores hints as an array for faster recomputation at a later point.
// The parameter e, if not 0, is here if the recomputation in
// ec_curve_to_basis_2f_from_hint only needs points of order 2^e rather
// than 2^f. In that case, to ensure we get the same point, we need to
// normalize the points of order 2^e in this function (in a way compatible
// with difference_point which will be called by
// ec_curve_to_basis_2f_from_hint)
uint8_t
ec_curve_to_basis_2f_to_hint(ec_basis_t *PQ2, ec_curve_t *curve, int f, int e)
{
    // Normalise (A/C : 1) and ((A + 2)/4 : 1)
    ec_normalize_curve_and_A24(curve);

    uint32_t a_is_zero = fp2_is_zero(&curve->A);
    // Whether A == 0 and whether A is a QR are public properties of the curve (the latter is the LSB of the published
    // hint); the curve itself stays tainted
    CT_TESTING_MAKE_PUBLIC(&a_is_zero, sizeof(a_is_zero));
    if (a_is_zero) {
        ec_basis_E0_2f(PQ2, curve, f);
        return 0;
    }

    uint8_t hint;
    bool hint_A = fp2_is_square(&curve->A);
    CT_TESTING_MAKE_PUBLIC(&hint_A, sizeof(hint_A));

    // Compute the points P, Q
    ec_point_t P, Q;

    if (!hint_A) {
        // when A is NQR we allow x(P) to be a multiple n*A of A
        hint = PQ_from_qr(&P, &Q, curve, 1);

    } else {
        // when A is QR we instead have to find (1 + b^2) a NQR such that P = (-A : (1 + i*b))
        hint = PQ_from_nqr(&P, &Q, curve, 1);
    }

    // clear out the odd cofactor to get a point of order 2^f
    clear_cofactor_for_maximal_even_order(&P, curve, f);
    clear_cofactor_for_maximal_even_order(&Q, curve, f);

    // compute PmQ, set PmQ to Q to ensure Q above (0,0)
    ec_copy_point(&PQ2->P, &P);
    ec_copy_point(&PQ2->PmQ, &Q);
    difference_point(&P, &PQ2->Q, &PQ2->P, &PQ2->PmQ, curve);

    if (e > 0) {
        assert(e < f);
        // warning: Q and PmQ are permuted above...
        ec_point_t Q2;

        // &PQ2->Q store our P-Q candidate, and we stored P+Q in P above
        // Q stores 2^{e-f} (P-Q), and Q2 2^{e-f} (P+Q)
        ec_dbl_iter(&Q, f - e, &PQ2->Q, curve);
        ec_dbl_iter(&Q2, f - e, &P, curve);

        fp2_t t0, t1, t2;

        // Compute (Q.x Q2.z - Q2.x Q.x) Qbar.z Q2bar.z N(Q.z) N(Q2.z)$
        fp2_mul(&t0, &Q.x, &Q2.z);
        fp2_mul(&t1, &Q2.x, &Q.z);
        fp2_sub(&t0, &t0, &t1); // Q.x Q2.z - Q2.x Q.x

        fp2_mul(&t1, &Q.z, &Q2.z);
        fp_copy(&t2.re, &t1.re);
        fp_neg(&t2.im, &t1.im);
        fp2_sqr(&t2, &t2);
        fp2_mul(&t1, &t1, &t2); // Qbar.z Q2bar.z N(Q.z) N(Q2.z)$
        fp2_mul(&t0, &t0, &t1);

        fp2_sqr(&t1, &t0);
        fp2_sqrt(&t1);
        fp2_select(&PQ2->Q.x, &PQ2->Q.x, &P.x, fp2_is_equal(&t0, &t1));
    }

    // Finally, we compress hint_A and hint into a single bytes. We choose to set the LSB of hint to hint_A
    assert(hint < 128); // We expect hint to be 7-bits in size
    return (hint << 1) | hint_A;
}

// Computes a basis E[2^f] = <P, Q> where the point Q is above (0 : 0)
// given the hints as an array for faster basis computation
// The bool normalize should be set to true if we want to use
// difference_point_normalized rather than difference_point.
// This is needed when this function is called as
//    ec_curve_to_basis_2f_from_hint(&B, E, e, 1);
// while the hint function was called as
//    ec_curve_to_basis_2f_to_hint(&B, E, f, e);
// with e<f.
int
ec_curve_to_basis_2f_from_hint(ec_basis_t *PQ2, ec_curve_t *curve, int f, const uint8_t hint)
{
    // Normalise (A/C : 1) and ((A + 2)/4 : 1)
    ec_normalize_curve_and_A24(curve);

    uint32_t a_is_zero = fp2_is_zero(&curve->A);
    // Whether A == 0 is a public property of the curve, and the hint is a published signature/public-key field;
    // the curve itself stays tainted
    CT_TESTING_MAKE_PUBLIC(&a_is_zero, sizeof(a_is_zero));
    if (a_is_zero) {
        ec_basis_E0_2f(PQ2, curve, f);
        return 1;
    }

    // The LSB of hint encodes whether A is a QR
    // The remaining 7-bits are used to find a valid x(P)
    bool hint_A = hint & 1;
    uint8_t hint_P = hint >> 1;
    CT_TESTING_MAKE_PUBLIC(&hint_A, sizeof(hint_A));
    CT_TESTING_MAKE_PUBLIC(&hint_P, sizeof(hint_P));

    // Compute the points P, Q
    ec_point_t P, Q;

    if (!hint_P) {
        // When hint_P = 0 it means we did not find a point in 128 attempts
        // this is very rare and we almost never expect to need this fallback
        // In either case, we can start with b = 128 to skip testing the known
        // values which will not work
        if (!hint_A) {
            PQ_from_qr(&P, &Q, curve, 128);

        } else {
            PQ_from_nqr(&P, &Q, curve, 128);
        }
    } else {
        // Otherwise we use the hint to directly find x(P) based on hint_A
        if (!hint_A) {
            // when A is NQR, we have found n such that x(P) = n*A
            fp2_mul_small(&P.x, &curve->A, hint_P);
            fp2_set_one(&P.z);

            // set xQ to -(xP + A)
            fp2_add(&Q.x, &curve->A, &P.x);
            fp2_neg(&Q.x, &Q.x);
            fp2_set_one(&Q.z);
        } else {
            // when A is QR we have found b such that (1 + b^2) is a NQR in Fp, so we must compute x(P) = -A / (1 + i*b)
            fp_set_one(&P.z.re);
            fp_set_small(&P.z.im, hint_P);

            fp2_copy(&P.x, &curve->A);
            fp2_neg(&P.x, &P.x);

            fp2_mul(&Q.x, &curve->A, &P.z);
            fp2_add(&Q.x, &Q.x, &P.x);
            fp2_neg(&Q.x, &Q.x);
            fp2_copy(&Q.z, &P.z);
        }
    }

#ifndef NDEBUG
    int passed = 1;
    // is_on_curve expects a normalised x-coordinate
    fp2_t x_test;
    fp2_copy(&x_test, &P.z);
    fp2_inv(&x_test);
    fp2_mul(&x_test, &x_test, &P.x);
    passed = is_on_curve(&x_test, curve);
    passed &= !fp2_is_square(&x_test);

    if (!passed)
        return 0;
#endif

    // clear out the odd cofactor to get a point of order 2^f
    clear_cofactor_for_maximal_even_order(&P, curve, f);
    clear_cofactor_for_maximal_even_order(&Q, curve, f);

    // compute PmQ, set PmQ to Q to ensure Q above (0,0)
    ec_copy_point(&PQ2->P, &P);
    ec_copy_point(&PQ2->PmQ, &Q);
    difference_point(&P, &PQ2->Q, &PQ2->P, &PQ2->PmQ, curve);

#ifndef NDEBUG
    passed &= test_basis_order_twof(PQ2, curve, f);

    if (!passed)
        return 0;
#endif

    return 1;
}
