#include <assert.h>
#include <stdio.h>
#include <mp.h>
#include <ec.h>

void
ec_point_init(ec_point_t *P)
{ // Initialize point as identity element (1:0)
    fp2_set_one(&(P->x));
    fp2_set_zero(&(P->z));
}

void
ec_curve_init(ec_curve_t *E)
{ // Initialize the curve struct
    // Initialize the constants
    fp2_set_zero(&(E->A));
    fp2_set_one(&(E->C));

    // Initialize the point (A+2 : 4C)
    ec_point_init(&(E->A24));

    // Set the bool to be false by default
    E->is_A24_computed_and_normalized = false;
}

void
select_point(ec_point_t *Q, const ec_point_t *P1, const ec_point_t *P2, const digit_t option)
{ // Select points in constant time
  // If option = 0 then Q <- P1, else if option = 0xFF...FF then Q <- P2
    fp2_select(&(Q->x), &(P1->x), &(P2->x), option);
    fp2_select(&(Q->z), &(P1->z), &(P2->z), option);
}

void
cswap_points(ec_point_t *P, ec_point_t *Q, const digit_t option)
{ // Swap points in constant time
  // If option = 0 then P <- P and Q <- Q, else if option = 0xFF...FF then P <- Q and Q <- P
    fp2_cswap(&(P->x), &(Q->x), option);
    fp2_cswap(&(P->z), &(Q->z), option);
}

void
ec_normalize_point(ec_point_t *P)
{
    fp2_inv(&P->z);
    fp2_mul(&P->x, &P->x, &P->z);
    fp2_set_one(&(P->z));
}

void
ec_normalize_curve(ec_curve_t *E)
{
    fp2_inv(&E->C);
    fp2_mul(&E->A, &E->A, &E->C);
    fp2_set_one(&E->C);
}

void
ec_curve_normalize_A24(ec_curve_t *E)
{
    if (!E->is_A24_computed_and_normalized) {
        AC_to_A24(&E->A24, E);
        ec_normalize_point(&E->A24);
        E->is_A24_computed_and_normalized = true;
    }
    assert(fp2_is_one(&E->A24.z));
}

void
ec_normalize_curve_and_A24(ec_curve_t *E)
{ // Neither the curve or A24 are guaranteed to be normalized.
  // First we normalize (A/C : 1) and conditionally compute
    if (!fp2_is_one(&E->C)) {
        ec_normalize_curve(E);
    }

    if (!E->is_A24_computed_and_normalized) {
        // Now compute A24 = ((A + 2) / 4 : 1)
        fp2_add_one(&E->A24.x, &E->A);     // re(A24.x) = re(A) + 1
        fp2_add_one(&E->A24.x, &E->A24.x); // re(A24.x) = re(A) + 2
        fp_copy(&E->A24.x.im, &E->A.im);   // im(A24.x) = im(A)

        fp2_half(&E->A24.x, &E->A24.x); // (A + 2) / 2
        fp2_half(&E->A24.x, &E->A24.x); // (A + 2) / 4
        fp2_set_one(&E->A24.z);

        E->is_A24_computed_and_normalized = true;
    }
}

uint32_t
ec_is_zero(const ec_point_t *P)
{
    return fp2_is_zero(&P->z);
}

uint32_t
ec_has_zero_coordinate(const ec_point_t *P)
{
    return fp2_is_zero(&P->x) | fp2_is_zero(&P->z);
}

uint32_t
ec_is_equal(const ec_point_t *P, const ec_point_t *Q)
{ // Evaluate if two points in Montgomery coordinates (X:Z) are equal
  // Returns 0xFFFFFFFF (true) if P=Q, 0 (false) otherwise
    fp2_t t0, t1;

    // Check if P, Q are the points at infinity
    uint32_t l_zero = ec_is_zero(P);
    uint32_t r_zero = ec_is_zero(Q);

    // Check if PX * QZ = QX * PZ
    fp2_mul(&t0, &P->x, &Q->z);
    fp2_mul(&t1, &P->z, &Q->x);
    uint32_t lr_equal = fp2_is_equal(&t0, &t1);

    // Points are equal if
    // - Both are zero, or
    // - neither are zero AND PX * QZ = QX * PZ
    return (l_zero & r_zero) | (~l_zero & ~r_zero * lr_equal);
}

uint32_t
ec_is_two_torsion(const ec_point_t *P, const ec_curve_t *E)
{
    if (ec_is_zero(P))
        return 0;

    uint32_t x_is_zero, tmp_is_zero;
    fp2_t t0, t1, t2;
    fp2_add(&t0, &P->x, &P->z);
    fp2_sqr(&t0, &t0);
    fp2_sub(&t1, &P->x, &P->z);
    fp2_sqr(&t1, &t1);
    fp2_sub(&t2, &t0, &t1);
    fp2_add(&t1, &t0, &t1);
    fp2_mul(&t2, &t2, &E->A);
    fp2_mul(&t1, &t1, &E->C);
    fp2_add(&t1, &t1, &t1);
    fp2_add(&t0, &t1, &t2); // 4 (CX^2+CZ^2+AXZ)

    x_is_zero = fp2_is_zero(&P->x);
    tmp_is_zero = fp2_is_zero(&t0);

    // two torsion if x or x^2 + Ax + 1 is zero
    return x_is_zero | tmp_is_zero;
}

uint32_t
ec_is_four_torsion(const ec_point_t *P, const ec_curve_t *E)
{
    ec_point_t test;
    xDBL_A24(&test, P, &E->A24, E->is_A24_computed_and_normalized);
    return ec_is_two_torsion(&test, E);
}

uint32_t
ec_is_basis_four_torsion(const ec_basis_t *B, const ec_curve_t *E)
{ // Check if basis points (P, Q) form a full 2^t-basis
    ec_point_t P2, Q2;
    xDBL_A24(&P2, &B->P, &E->A24, E->is_A24_computed_and_normalized);
    xDBL_A24(&Q2, &B->Q, &E->A24, E->is_A24_computed_and_normalized);
    return (ec_is_two_torsion(&P2, E) & ec_is_two_torsion(&Q2, E) & ~ec_is_equal(&P2, &Q2));
}

int
ec_curve_verify_A(const fp2_t *A)
{ // Verify the Montgomery coefficient A is valid (A^2-4 \ne 0)
  // Return 1 if curve is valid, 0 otherwise
    fp2_t t;
    fp2_set_one(&t);
    fp_add(&t.re, &t.re, &t.re); // t=2
    if (fp2_is_equal(A, &t))
        return 0;
    fp_neg(&t.re, &t.re); // t=-2
    if (fp2_is_equal(A, &t))
        return 0;
    return 1;
}

int
ec_curve_init_from_A(ec_curve_t *E, const fp2_t *A)
{ // Initialize the curve from the A coefficient and check it is valid
  // Return 1 if curve is valid, 0 otherwise
    ec_curve_init(E);
    fp2_copy(&E->A, A); // Set A
    return ec_curve_verify_A(A);
}

void
ec_j_inv(fp2_t *j_inv, const ec_curve_t *curve)
{ // j-invariant computation for Montgommery coefficient A2=(A+2C:4C)
    fp2_t t0, t1;

    fp2_sqr(&t1, &curve->C);
    fp2_sqr(j_inv, &curve->A);
    fp2_add(&t0, &t1, &t1);
    fp2_sub(&t0, j_inv, &t0);
    fp2_sub(&t0, &t0, &t1);
    fp2_sub(j_inv, &t0, &t1);
    fp2_sqr(&t1, &t1);
    fp2_mul(j_inv, j_inv, &t1);
    fp2_add(&t0, &t0, &t0);
    fp2_add(&t0, &t0, &t0);
    fp2_sqr(&t1, &t0);
    fp2_mul(&t0, &t0, &t1);
    fp2_add(&t0, &t0, &t0);
    fp2_add(&t0, &t0, &t0);
    fp2_inv(j_inv);
    fp2_mul(j_inv, &t0, j_inv);
}

void
xDBL_E0(ec_point_t *Q, const ec_point_t *P)
{ // Doubling of a Montgomery point in projective coordinates (X:Z) on the curve E0 with (A:C) = (0:1).
  // Input: projective Montgomery x-coordinates P = (XP:ZP), where xP=XP/ZP, and Montgomery curve constants (A:C) = (0:1). 
  // Output: projective Montgomery x-coordinates Q <- 2*P = (XQ:ZQ) such that x(2P)=XQ/ZQ.
    fp2_t t0, t1, t2;

    fp2_add(&t0, &P->x, &P->z);
    fp2_sqr(&t0, &t0);
    fp2_sub(&t1, &P->x, &P->z);
    fp2_sqr(&t1, &t1);
    fp2_sub(&t2, &t0, &t1);
    fp2_add(&t1, &t1, &t1);
    fp2_mul(&Q->x, &t0, &t1);
    fp2_add(&Q->z, &t1, &t2);
    fp2_mul(&Q->z, &Q->z, &t2);
}

void
xDBL(ec_point_t *Q, const ec_point_t *P, const ec_point_t *AC)
{ // Doubling of a Montgomery point in projective coordinates (X:Z). Computation of coefficient values A+2C and 4C
  // on-the-fly. 
  // Input: projective Montgomery x-coordinates P = (XP:ZP), where xP=XP/ZP, and Montgomery curve constants (A:C). 
  // Output: projective Montgomery x-coordinates Q <- 2*P = (XQ:ZQ) such that x(2P)=XQ/ZQ.
    fp2_t t0, t1, t2, t3;

    fp2_add(&t0, &P->x, &P->z);
    fp2_sqr(&t0, &t0);
    fp2_sub(&t1, &P->x, &P->z);
    fp2_sqr(&t1, &t1);
    fp2_sub(&t2, &t0, &t1);
    fp2_add(&t3, &AC->z, &AC->z);
    fp2_mul(&t1, &t1, &t3);
    fp2_add(&t1, &t1, &t1);
    fp2_mul(&Q->x, &t0, &t1);
    fp2_add(&t0, &t3, &AC->x);
    fp2_mul(&t0, &t0, &t2);
    fp2_add(&t0, &t0, &t1);
    fp2_mul(&Q->z, &t0, &t2);
}

void
xDBL_A24(ec_point_t *Q, const ec_point_t *P, const ec_point_t *A24, const bool A24_normalized)
{ // Doubling of a Montgomery point in projective coordinates (X:Z).
  // Input: projective Montgomery x-coordinates P = (XP:ZP), where xP=XP/ZP, and
  //        the Montgomery curve constants A24 = (A+2C:4C) (or A24 = (A+2C/4C:1) if normalized).
  // Output: projective Montgomery x-coordinates Q <- 2*P = (XQ:ZQ) such that x(2P)=XQ/ZQ.
    fp2_t t0, t1, t2;

    fp2_add(&t0, &P->x, &P->z);
    fp2_sqr(&t0, &t0);
    fp2_sub(&t1, &P->x, &P->z);
    fp2_sqr(&t1, &t1);
    fp2_sub(&t2, &t0, &t1);
    if (!A24_normalized)
        fp2_mul(&t1, &t1, &A24->z);
    fp2_mul(&Q->x, &t0, &t1);
    fp2_mul(&t0, &t2, &A24->x);
    fp2_add(&t0, &t0, &t1);
    fp2_mul(&Q->z, &t0, &t2);
}

void
xADD(ec_point_t *R, const ec_point_t *P, const ec_point_t *Q, const ec_point_t *PQ)
{ // Differential addition of Montgomery points in projective coordinates (X:Z).
  // Input: projective Montgomery points P=(XP:ZP) and Q=(XQ:ZQ) such that xP=XP/ZP and xQ=XQ/ZQ, and difference
  //        PQ=P-Q=(XPQ:ZPQ).
  // Output: projective Montgomery point R <- P+Q = (XR:ZR) such that x(P+Q)=XR/ZR.
    fp2_t t0, t1, t2, t3;

    fp2_add(&t0, &P->x, &P->z);
    fp2_sub(&t1, &P->x, &P->z);
    fp2_add(&t2, &Q->x, &Q->z);
    fp2_sub(&t3, &Q->x, &Q->z);
    fp2_mul(&t0, &t0, &t3);
    fp2_mul(&t1, &t1, &t2);
    fp2_add(&t2, &t0, &t1);
    fp2_sub(&t3, &t0, &t1);
    fp2_sqr(&t2, &t2);
    fp2_sqr(&t3, &t3);
    fp2_mul(&t2, &PQ->z, &t2);
    fp2_mul(&R->z, &PQ->x, &t3);
    fp2_copy(&R->x, &t2);
}

void
xDBLADD(ec_point_t *R,
        ec_point_t *S,
        const ec_point_t *P,
        const ec_point_t *Q,
        const ec_point_t *PQ,
        const ec_point_t *A24,
        const bool A24_normalized)
{ // Simultaneous doubling and differential addition.
  // Input:  projective Montgomery points P=(XP:ZP) and Q=(XQ:ZQ) such that xP=XP/ZP and xQ=XQ/ZQ, the difference
  //         PQ=P-Q=(XPQ:ZPQ), and the Montgomery curve constants A24 = (A+2C:4C) (or A24 = (A+2C/4C:1) if normalized).
  // Output: projective Montgomery points R <- 2*P = (XR:ZR) such that x(2P)=XR/ZR, and S <- P+Q = (XS:ZS) such that =
  //         x(Q+P)=XS/ZS.
    fp2_t t0, t1, t2;

    fp2_add(&t0, &P->x, &P->z);
    fp2_sub(&t1, &P->x, &P->z);
    fp2_sqr(&R->x, &t0);
    fp2_sub(&t2, &Q->x, &Q->z);
    fp2_add(&S->x, &Q->x, &Q->z);
    fp2_mul(&t0, &t0, &t2);
    fp2_sqr(&R->z, &t1);
    fp2_mul(&t1, &t1, &S->x);
    fp2_sub(&t2, &R->x, &R->z);
    if (!A24_normalized)
        fp2_mul(&R->z, &R->z, &A24->z);
    fp2_mul(&R->x, &R->x, &R->z);
    fp2_mul(&S->x, &A24->x, &t2);
    fp2_sub(&S->z, &t0, &t1);
    fp2_add(&R->z, &R->z, &S->x);
    fp2_add(&S->x, &t0, &t1);
    fp2_mul(&R->z, &R->z, &t2);
    fp2_sqr(&S->z, &S->z);
    fp2_sqr(&S->x, &S->x);
    fp2_mul(&S->z, &S->z, &PQ->x);
    fp2_mul(&S->x, &S->x, &PQ->z);
}

void
xMUL(ec_point_t *Q, const ec_point_t *P, const digit_t *k, const int kbits, const ec_curve_t *curve)
{ // The Montgomery ladder
  // Input: projective Montgomery point P=(XP:ZP) such that xP=XP/ZP, a scalar k of bitlength kbits, and
  //        the Montgomery curve constants (A:C) (or A24 = (A+2C/4C:1) if normalized).
  // Output: projective Montgomery points Q <- k*P = (XQ:ZQ) such that x(k*P)=XQ/ZQ.
    ec_point_t R0, R1, A24;
    digit_t mask;
    unsigned int bit, prevbit = 0, swap;

    if (!curve->is_A24_computed_and_normalized) {
        // Computation of A24=(A+2C:4C)
        fp2_add(&A24.x, &curve->C, &curve->C);
        fp2_add(&A24.z, &A24.x, &A24.x);
        fp2_add(&A24.x, &A24.x, &curve->A);
    } else {
        fp2_copy(&A24.x, &curve->A24.x);
        fp2_copy(&A24.z, &curve->A24.z);
        // Assert A24 has been normalised
        assert(fp2_is_one(&A24.z));
    }

    // R0 <- (1:0), R1 <- P
    ec_point_init(&R0);
    fp2_copy(&R1.x, &P->x);
    fp2_copy(&R1.z, &P->z);

    // Main loop
    for (int i = kbits - 1; i >= 0; i--) {
        bit = (k[i >> LOG2RADIX] >> (i & (RADIX - 1))) & 1;
        swap = bit ^ prevbit;
        prevbit = bit;
        mask = 0 - (digit_t)swap;

        cswap_points(&R0, &R1, mask);
        xDBLADD(&R0, &R1, &R0, &R1, P, &A24, true);
    }
    swap = 0 ^ prevbit;
    mask = 0 - (digit_t)swap;
    cswap_points(&R0, &R1, mask);

    fp2_copy(&Q->x, &R0.x);
    fp2_copy(&Q->z, &R0.z);
}

int
xDBLMUL(ec_point_t *S,
        const ec_point_t *P,
        const digit_t *k,
        const ec_point_t *Q,
        const digit_t *l,
        const ec_point_t *PQ,
        const int kbits,
        const ec_curve_t *curve)
{ // The Montgomery biladder
  // Input:  projective Montgomery points P=(XP:ZP) and Q=(XQ:ZQ) such that xP=XP/ZP and xQ=XQ/ZQ, scalars k and l of
  //         bitlength kbits, the difference PQ=P-Q=(XPQ:ZPQ), and the Montgomery curve constants (A:C).
  // Output: projective Montgomery point S <- k*P + l*Q = (XS:ZS) such that x(k*P + l*Q)=XS/ZS.

    int i, A_is_zero;
    digit_t evens, mevens, bitk0, bitl0, maskk, maskl, temp, bs1_ip1, bs2_ip1, bs1_i, bs2_i, h;
    digit_t sigma[2] = { 0 }, pre_sigma = 0;
    digit_t k_t[NWORDS_ORDER], l_t[NWORDS_ORDER], one[NWORDS_ORDER] = { 0 }, r[2 * BITS] = { 0 };
    ec_point_t DIFF1a, DIFF1b, DIFF2a, DIFF2b, R[3] = { 0 }, T[3];

    // differential additions formulas are invalid in this case
    if (ec_has_zero_coordinate(P) | ec_has_zero_coordinate(Q) | ec_has_zero_coordinate(PQ))
        return 0;

    // Derive sigma according to parity
    bitk0 = (k[0] & 1);
    bitl0 = (l[0] & 1);
    maskk = 0 - bitk0; // Parity masks: 0 if even, otherwise 1...1
    maskl = 0 - bitl0;
    sigma[0] = (bitk0 ^ 1);
    sigma[1] = (bitl0 ^ 1);
    evens = sigma[0] + sigma[1]; // Count number of even scalars
    mevens = 0 - (evens & 1);    // Mask mevens <- 0 if # even of scalars = 0 or 2, otherwise mevens = 1...1

    // If k and l are both even or both odd, pick sigma = (0,1)
    sigma[0] = (sigma[0] & mevens);
    sigma[1] = (sigma[1] & mevens) | (1 & ~mevens);

    // Convert even scalars to odd
    one[0] = 1;
    mp_sub(k_t, k, one, NWORDS_ORDER);
    mp_sub(l_t, l, one, NWORDS_ORDER);
    select_ct(k_t, k_t, k, maskk, NWORDS_ORDER);
    select_ct(l_t, l_t, l, maskl, NWORDS_ORDER);

    // Scalar recoding
    for (i = 0; i < kbits; i++) {
        // If sigma[0] = 1 swap k_t and l_t
        maskk = 0 - (sigma[0] ^ pre_sigma);
        swap_ct(k_t, l_t, maskk, NWORDS_ORDER);

        if (i == kbits - 1) {
            bs1_ip1 = 0;
            bs2_ip1 = 0;
        } else {
            bs1_ip1 = mp_shiftr(k_t, 1, NWORDS_ORDER);
            bs2_ip1 = mp_shiftr(l_t, 1, NWORDS_ORDER);
        }
        bs1_i = k_t[0] & 1;
        bs2_i = l_t[0] & 1;

        r[2 * i] = bs1_i ^ bs1_ip1;
        r[2 * i + 1] = bs2_i ^ bs2_ip1;

        // Revert sigma if second bit, r_(2i+1), is 1
        pre_sigma = sigma[0];
        maskk = 0 - r[2 * i + 1];
        select_ct(&temp, &sigma[0], &sigma[1], maskk, 1);
        select_ct(&sigma[1], &sigma[1], &sigma[0], maskk, 1);
        sigma[0] = temp;
    }

    // Point initialization
    ec_point_init(&R[0]);
    maskk = 0 - sigma[0];
    select_point(&R[1], P, Q, maskk);
    select_point(&R[2], Q, P, maskk);

    fp2_copy(&DIFF1a.x, &R[1].x);
    fp2_copy(&DIFF1a.z, &R[1].z);
    fp2_copy(&DIFF1b.x, &R[2].x);
    fp2_copy(&DIFF1b.z, &R[2].z);

    // Initialize DIFF2a <- P+Q, DIFF2b <- P-Q
    xADD(&R[2], &R[1], &R[2], PQ);
    if (ec_has_zero_coordinate(&R[2]))
        return 0; // non valid formulas

    fp2_copy(&DIFF2a.x, &R[2].x);
    fp2_copy(&DIFF2a.z, &R[2].z);
    fp2_copy(&DIFF2b.x, &PQ->x);
    fp2_copy(&DIFF2b.z, &PQ->z);

    A_is_zero = fp2_is_zero(&curve->A);

    // Main loop
    for (i = kbits - 1; i >= 0; i--) {
        h = r[2 * i] + r[2 * i + 1]; // in {0, 1, 2}
        maskk = 0 - (h & 1);
        select_point(&T[0], &R[0], &R[1], maskk);
        maskk = 0 - (h >> 1);
        select_point(&T[0], &T[0], &R[2], maskk);
        if (A_is_zero) {
            xDBL_E0(&T[0], &T[0]);
        } else {
            assert(fp2_is_one(&curve->A24.z));
            xDBL_A24(&T[0], &T[0], &curve->A24, true);
        }

        maskk = 0 - r[2 * i + 1]; // in {0, 1}
        select_point(&T[1], &R[0], &R[1], maskk);
        select_point(&T[2], &R[1], &R[2], maskk);

        cswap_points(&DIFF1a, &DIFF1b, maskk);
        xADD(&T[1], &T[1], &T[2], &DIFF1a);
        xADD(&T[2], &R[0], &R[2], &DIFF2a);

        // If hw (mod 2) = 1 then swap DIFF2a and DIFF2b
        maskk = 0 - (h & 1);
        cswap_points(&DIFF2a, &DIFF2b, maskk);

        // R <- T
        copy_point(&R[0], &T[0]);
        copy_point(&R[1], &T[1]);
        copy_point(&R[2], &T[2]);
    }

    // Output R[evens]
    select_point(S, &R[0], &R[1], mevens);

    maskk = 0 - (bitk0 & bitl0);
    select_point(S, S, &R[2], maskk);
    return 1;
}

int
ec_ladder3pt(ec_point_t *R,
             const digit_t *m,
             const ec_point_t *P,
             const ec_point_t *Q,
             const ec_point_t *PQ,
             const ec_curve_t *E)
{ // The 3-point Montgomery ladder
  // Input:  projective Montgomery points P=(XP:ZP) and Q=(XQ:ZQ) such that xP=XP/ZP and xQ=XQ/ZQ, a scalar k of
  //         bitlength kbits, the difference PQ=P-Q=(XPQ:ZPQ), and the Montgomery curve constants A24 = (A+2C/4C:1).
  // Output: projective Montgomery point R <- P + m*Q = (XR:ZR) such that x(P + m*Q)=XR/ZR.
    assert(E->is_A24_computed_and_normalized);
    if (!fp2_is_one(&E->A24.z)) {
        return 0;
    }
    // Formulas are not valid in that case
    if (ec_has_zero_coordinate(PQ)) {
        return 0;
    }

    ec_point_t X0, X1, X2;
    copy_point(&X0, Q);
    copy_point(&X1, P);
    copy_point(&X2, PQ);

    int i, j;
    digit_t t;
    for (i = 0; i < NWORDS_ORDER; i++) {
        t = 1;
        for (j = 0; j < RADIX; j++) {
            cswap_points(&X1, &X2, -((t & m[i]) == 0));
            xDBLADD(&X0, &X1, &X0, &X1, &X2, &E->A24, true);
            cswap_points(&X1, &X2, -((t & m[i]) == 0));
            t <<= 1;
        };
    };
    copy_point(R, &X1);
    return 1;
}

// WRAPPERS to export

void
ec_dbl(ec_point_t *res, const ec_point_t *P, const ec_curve_t *curve)
{
    // If A24 = ((A+2)/4 : 1) we save multiplications
    if (curve->is_A24_computed_and_normalized) {
        assert(fp2_is_one(&curve->A24.z));
        xDBL_A24(res, P, &curve->A24, true);
    } else {
        // Otherwise we compute A24 on the fly for doubling
        xDBL(res, P, (const ec_point_t *)curve);
    }
}

void
ec_dbl_iter(ec_point_t *res, int n, const ec_point_t *P, ec_curve_t *curve)
{
    if (n == 0) {
        copy_point(res, P);
        return;
    }

    // When the chain is long enough, we should normalise A24
    if (n > 50) {
        ec_curve_normalize_A24(curve);
    }

    // When A24 is normalized we can save some multiplications
    if (curve->is_A24_computed_and_normalized) {
        assert(fp2_is_one(&curve->A24.z));
        xDBL_A24(res, P, &curve->A24, true);
        for (int i = 0; i < n - 1; i++) {
            assert(fp2_is_one(&curve->A24.z));
            xDBL_A24(res, res, &curve->A24, true);
        }
    } else {
        // Otherwise we do normal doubling
        xDBL(res, P, (const ec_point_t *)curve);
        for (int i = 0; i < n - 1; i++) {
            xDBL(res, res, (const ec_point_t *)curve);
        }
    }
}

void
ec_dbl_iter_basis(ec_basis_t *res, int n, const ec_basis_t *B, ec_curve_t *curve)
{
    ec_dbl_iter(&res->P, n, &B->P, curve);
    ec_dbl_iter(&res->Q, n, &B->Q, curve);
    ec_dbl_iter(&res->PmQ, n, &B->PmQ, curve);
}

void
ec_mul(ec_point_t *res, const digit_t *scalar, const int kbits, const ec_point_t *P, ec_curve_t *curve)
{
    // For large scalars it's worth normalising anyway
    if (kbits > 50) {
        ec_curve_normalize_A24(curve);
    }

    // When A24 is computed and normalized we save some Fp2 multiplications
    xMUL(res, P, scalar, kbits, curve);
}

int
ec_biscalar_mul(ec_point_t *res,
                const digit_t *scalarP,
                const digit_t *scalarQ,
                const int kbits,
                const ec_basis_t *PQ,
                const ec_curve_t *curve)
{
    if (fp2_is_zero(&PQ->PmQ.z))
        return 0;

    /* Differential additions behave badly when PmQ = (0:1), so we need to
     * treat this case specifically. Since we assume P, Q are a basis, this
     * can happen only if kbits==1 */
    if (kbits == 1) {
        // Sanity check: our basis should be given by 2-torsion points
        if (!ec_is_two_torsion(&PQ->P, curve) || !ec_is_two_torsion(&PQ->Q, curve) ||
            !ec_is_two_torsion(&PQ->PmQ, curve))
            return 0;
        digit_t bP, bQ;
        bP = (scalarP[0] & 1);
        bQ = (scalarQ[0] & 1);
        if (bP == 0 && bQ == 0)
            ec_point_init(res); //(1: 0)
        else if (bP == 1 && bQ == 0)
            copy_point(res, &PQ->P);
        else if (bP == 0 && bQ == 1)
            copy_point(res, &PQ->Q);
        else if (bP == 1 && bQ == 1)
            copy_point(res, &PQ->PmQ);
        else // should never happen
            assert(0);
        return 1;
    } else {
        ec_curve_t E;
        copy_curve(&E, curve);

        if (!fp2_is_zero(&curve->A)) { // If A is not zero normalize
            ec_curve_normalize_A24(&E);
        }
        return xDBLMUL(res, &PQ->P, scalarP, &PQ->Q, scalarQ, &PQ->PmQ, kbits, (const ec_curve_t *)&E);
    }
}
