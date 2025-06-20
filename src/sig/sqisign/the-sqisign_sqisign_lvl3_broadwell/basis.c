#include "ec.h"
#include "fp2.h"
#include "e0_basis.h"
#include <assert.h>

uint32_t
ec_recover_y(fp2_t *y, const fp2_t *Px, const ec_curve_t *curve)
{ // Recover y-coordinate of a point on the Montgomery curve y^2 = x^3 + Ax^2 + x
    fp2_t t0;

    fp2_sqr(&t0, Px);
    fp2_mul(y, &t0, &curve->A); // Ax^2
    fp2_add(y, y, Px);          // Ax^2 + x
    fp2_mul(&t0, &t0, Px);
    fp2_add(y, y, &t0); // x^3 + Ax^2 + x
    // This is required, because we do not yet know that our curves are
    // supersingular so our points live on the twist with B = 1.
    return fp2_sqrt_verify(y);
}

static void
difference_point(ec_point_t *PQ, const ec_point_t *P, const ec_point_t *Q, const ec_curve_t *curve)
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

    // To ensure that the denominator is a fourth power in Fp, we normalize by
    // C*C_bar^2*(P.z)_bar^2*(Q.z)_bar^2
    fp_copy(&t0.re, &curve->C.re);
    fp_neg(&t0.im, &curve->C.im);
    fp2_sqr(&t0, &t0);
    fp2_mul(&t0, &t0, &curve->C);
    fp_copy(&t1.re, &P->z.re);
    fp_neg(&t1.im, &P->z.im);
    fp2_sqr(&t1, &t1);
    fp2_mul(&t0, &t0, &t1);
    fp_copy(&t1.re, &Q->z.re);
    fp_neg(&t1.im, &Q->z.im);
    fp2_sqr(&t1, &t1);
    fp2_mul(&t0, &t0, &t1);
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
    fp2_copy(&PQ->z, &Bzz);
}

// Lifts a basis x(P), x(Q), x(P-Q) assuming the curve has (A/C : 1) and the point
// P = (X/Z : 1). For generic implementation see lift_basis()
uint32_t
lift_basis_normalized(jac_point_t *P, jac_point_t *Q, ec_basis_t *B, ec_curve_t *E)
{
    assert(fp2_is_one(&B->P.z));
    assert(fp2_is_one(&E->C));

    fp2_copy(&P->x, &B->P.x);
    fp2_copy(&Q->x, &B->Q.x);
    fp2_copy(&Q->z, &B->Q.z);
    fp2_set_one(&P->z);
    uint32_t ret = ec_recover_y(&P->y, &P->x, E);

    // Algorithm of Okeya-Sakurai to recover y.Q in the montgomery model
    fp2_t v1, v2, v3, v4;
    fp2_mul(&v1, &P->x, &Q->z);
    fp2_add(&v2, &Q->x, &v1);
    fp2_sub(&v3, &Q->x, &v1);
    fp2_sqr(&v3, &v3);
    fp2_mul(&v3, &v3, &B->PmQ.x);
    fp2_add(&v1, &E->A, &E->A);
    fp2_mul(&v1, &v1, &Q->z);
    fp2_add(&v2, &v2, &v1);
    fp2_mul(&v4, &P->x, &Q->x);
    fp2_add(&v4, &v4, &Q->z);
    fp2_mul(&v2, &v2, &v4);
    fp2_mul(&v1, &v1, &Q->z);
    fp2_sub(&v2, &v2, &v1);
    fp2_mul(&v2, &v2, &B->PmQ.z);
    fp2_sub(&Q->y, &v3, &v2);
    fp2_add(&v1, &P->y, &P->y);
    fp2_mul(&v1, &v1, &Q->z);
    fp2_mul(&v1, &v1, &B->PmQ.z);
    fp2_mul(&Q->x, &Q->x, &v1);
    fp2_mul(&Q->z, &Q->z, &v1);

    // Transforming to a jacobian coordinate
    fp2_sqr(&v1, &Q->z);
    fp2_mul(&Q->y, &Q->y, &v1);
    fp2_mul(&Q->x, &Q->x, &Q->z);
    return ret;
}

uint32_t
lift_basis(jac_point_t *P, jac_point_t *Q, ec_basis_t *B, ec_curve_t *E)
{
    // Normalise the curve E such that (A : C) is (A/C : 1)
    // and the point x(P) = (X/Z : 1).
    fp2_t inverses[2];
    fp2_copy(&inverses[0], &B->P.z);
    fp2_copy(&inverses[1], &E->C);

    fp2_batched_inv(inverses, 2);
    fp2_set_one(&B->P.z);
    fp2_set_one(&E->C);

    fp2_mul(&B->P.x, &B->P.x, &inverses[0]);
    fp2_mul(&E->A, &E->A, &inverses[1]);

    // Lift the basis to Jacobian points P, Q
    return lift_basis_normalized(P, Q, B, E);
}

// Given an x-coordinate, determines if this is a valid
// point on the curve. Assumes C=1.
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

// Helper function which given a point of order k*2^n with n maximal
// and k odd, computes a point of order 2^f
static inline void
clear_cofactor_for_maximal_even_order(ec_point_t *P, ec_curve_t *curve, int f)
{
    // clear out the odd cofactor to get a point of order 2^n
    ec_mul(P, p_cofactor_for_2f, P_COFACTOR_FOR_2F_BITLENGTH, P, curve);

    // clear the power of two to get a point of order 2^f
    for (int i = 0; i < TORSION_EVEN_POWER - f; i++) {
        xDBL_A24(P, P, &curve->A24, curve->is_A24_computed_and_normalized);
    }
}

// Helper function which finds an NQR -1 / (1 + i*b) for entangled basis generation
static uint8_t
find_nqr_factor(fp2_t *x, ec_curve_t *curve, const uint8_t start)
{
    // factor = -1/(1 + i*b) for b in Fp will be NQR whenever 1 + b^2 is NQR
    // in Fp, so we find one of these and then invert (1 + i*b). We store b
    // as a u8 hint to save time in verification.

    // We return the hint as a u8, but use (uint16_t)n to give 2^16 - 1
    // to make failure cryptographically negligible, with a fallback when
    // n > 128 is required.
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

        // for Px := -A/(1 + i*b) to be on the curve
        // is equivalent to A^2*(z-1) - z^2 NQR for z = 1 + i*b
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

        qr_b = 1;
    } while (!found);

    // set Px to -A/(1 + i*b)
    fp2_copy(x, &z);
    fp2_inv(x);
    fp2_mul(x, x, &curve->A);
    fp2_neg(x, x);

    /*
     * With very low probability n will not fit in 7 bits.
     * We set hint = 0 which signals failure and the need
     * to generate a value on the fly during verification
     */
    hint = n <= 128 ? n - 1 : 0;

    return hint;
}

// Helper function which finds a point x(P) = n * A
static uint8_t
find_nA_x_coord(fp2_t *x, ec_curve_t *curve, const uint8_t start)
{
    assert(!fp2_is_square(&curve->A)); // Only to be called when A is a NQR

    // when A is NQR we allow x(P) to be a multiple n*A of A
    uint8_t n = start;
    if (n == 1) {
        fp2_copy(x, &curve->A);
    } else {
        fp2_mul_small(x, &curve->A, n);
    }

    while (!is_on_curve(x, curve)) {
        fp2_add(x, x, &curve->A);
        n++;
    }

    /*
     * With very low probability (1/2^128), n will not fit in 7 bits.
     * In this case, we set hint = 0 which signals failure and the need
     * to generate a value on the fly during verification
     */
    uint8_t hint = n < 128 ? n : 0;
    return hint;
}

// The entangled basis generation does not allow A = 0
// so we simply return the one we have already precomputed
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
        xDBL_E0(&P, &P);
        xDBL_E0(&Q, &Q);
    }

    // Set P, Q in the basis and compute x(P - Q)
    copy_point(&PQ2->P, &P);
    copy_point(&PQ2->Q, &Q);
    difference_point(&PQ2->PmQ, &P, &Q, curve);
}

// Computes a basis E[2^f] = <P, Q> where the point Q is above (0 : 0)
// and stores hints as an array for faster recomputation at a later point
uint8_t
ec_curve_to_basis_2f_to_hint(ec_basis_t *PQ2, ec_curve_t *curve, int f)
{
    // Normalise (A/C : 1) and ((A + 2)/4 : 1)
    ec_normalize_curve_and_A24(curve);

    if (fp2_is_zero(&curve->A)) {
        ec_basis_E0_2f(PQ2, curve, f);
        return 0;
    }

    uint8_t hint;
    bool hint_A = fp2_is_square(&curve->A);

    // Compute the points P, Q
    ec_point_t P, Q;

    if (!hint_A) {
        // when A is NQR we allow x(P) to be a multiple n*A of A
        hint = find_nA_x_coord(&P.x, curve, 1);
    } else {
        // when A is QR we instead have to find (1 + b^2) a NQR
        // such that x(P) = -A / (1 + i*b)
        hint = find_nqr_factor(&P.x, curve, 1);
    }

    fp2_set_one(&P.z);
    fp2_add(&Q.x, &curve->A, &P.x);
    fp2_neg(&Q.x, &Q.x);
    fp2_set_one(&Q.z);

    // clear out the odd cofactor to get a point of order 2^f
    clear_cofactor_for_maximal_even_order(&P, curve, f);
    clear_cofactor_for_maximal_even_order(&Q, curve, f);

    // compute PmQ, set PmQ to Q to ensure Q above (0,0)
    difference_point(&PQ2->Q, &P, &Q, curve);
    copy_point(&PQ2->P, &P);
    copy_point(&PQ2->PmQ, &Q);

    // Finally, we compress hint_A and hint into a single bytes.
    // We choose to set the LSB of hint to hint_A
    assert(hint < 128); // We expect hint to be 7-bits in size
    return (hint << 1) | hint_A;
}

// Computes a basis E[2^f] = <P, Q> where the point Q is above (0 : 0)
// given the hints as an array for faster basis computation
int
ec_curve_to_basis_2f_from_hint(ec_basis_t *PQ2, ec_curve_t *curve, int f, const uint8_t hint)
{
    // Normalise (A/C : 1) and ((A + 2)/4 : 1)
    ec_normalize_curve_and_A24(curve);

    if (fp2_is_zero(&curve->A)) {
        ec_basis_E0_2f(PQ2, curve, f);
        return 1;
    }

    // The LSB of hint encodes whether A is a QR
    // The remaining 7-bits are used to find a valid x(P)
    bool hint_A = hint & 1;
    uint8_t hint_P = hint >> 1;

    // Compute the points P, Q
    ec_point_t P, Q;

    if (!hint_P) {
        // When hint_P = 0 it means we did not find a point in 128 attempts
        // this is very rare and we almost never expect to need this fallback
        // In either case, we can start with b = 128 to skip testing the known
        // values which will not work
        if (!hint_A) {
            find_nA_x_coord(&P.x, curve, 128);
        } else {
            find_nqr_factor(&P.x, curve, 128);
        }
    } else {
        // Otherwise we use the hint to directly find x(P) based on hint_A
        if (!hint_A) {
            // when A is NQR, we have found n such that x(P) = n*A
            fp2_mul_small(&P.x, &curve->A, hint_P);
        } else {
            // when A is QR we have found b such that (1 + b^2) is a NQR in
            // Fp, so we must compute x(P) = -A / (1 + i*b)
            fp_set_one(&P.x.re);
            fp_set_small(&P.x.im, hint_P);
            fp2_inv(&P.x);
            fp2_mul(&P.x, &P.x, &curve->A);
            fp2_neg(&P.x, &P.x);
        }
    }
    fp2_set_one(&P.z);

#ifndef NDEBUG
    int passed = 1;
    passed = is_on_curve(&P.x, curve);
    passed &= !fp2_is_square(&P.x);

    if (!passed)
        return 0;
#endif

    // set xQ to -xP - A
    fp2_add(&Q.x, &curve->A, &P.x);
    fp2_neg(&Q.x, &Q.x);
    fp2_set_one(&Q.z);

    // clear out the odd cofactor to get a point of order 2^f
    clear_cofactor_for_maximal_even_order(&P, curve, f);
    clear_cofactor_for_maximal_even_order(&Q, curve, f);

    // compute PmQ, set PmQ to Q to ensure Q above (0,0)
    difference_point(&PQ2->Q, &P, &Q, curve);
    copy_point(&PQ2->P, &P);
    copy_point(&PQ2->PmQ, &Q);

#ifndef NDEBUG
    passed &= test_basis_order_twof(PQ2, curve, f);

    if (!passed)
        return 0;
#endif

    return 1;
}
