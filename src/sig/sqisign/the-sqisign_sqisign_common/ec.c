#include <assert.h>
#include <stdio.h>
#include <mp.h>
#include <ec.h>

#include <ct_testing.h>

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

static void
select_point(ec_point_t *Q, const ec_point_t *P1, const ec_point_t *P2, const digit_t option)
{ // Select points in constant time
  // If option = 0 then Q <- P1, else if option = 0xFF...FF then Q <- P2
    fp2_select(&(Q->x), &(P1->x), &(P2->x), option);
    fp2_select(&(Q->z), &(P1->z), &(P2->z), option);
}

static void
swap_points(ec_point_t *P, ec_point_t *Q)
{
    ec_point_t temp;
    ec_copy_point(&temp, P);
    ec_copy_point(P, Q);
    ec_copy_point(Q, &temp);
}

static void
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
        // A24 = (A+2C : 4C)
        fp2_add(&E->A24.z, &E->C, &E->C);
        fp2_add(&E->A24.x, &E->A, &E->A24.z);
        fp2_add(&E->A24.z, &E->A24.z, &E->A24.z);

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
    ec_xDBL_A24(&test, P, &E->A24, E->is_A24_computed_and_normalized);
    return ec_is_two_torsion(&test, E);
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
ec_xDBL_E0(ec_point_t *Q, const ec_point_t *P)
{ // Doubling of a Montgomery point in projective coordinates (X:Z) on the curve E0 with (A:C) = (0:1).
  // Input: projective Montgomery x-coordinates P = (XP:ZP), where xP=XP/ZP, and Montgomery curve constants (A:C) =
  // (0:1). Output: projective Montgomery x-coordinates Q <- 2*P = (XQ:ZQ) such that x(2P)=XQ/ZQ.
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
ec_xDBL(ec_point_t *Q, const ec_point_t *P, const ec_point_t *AC)
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
ec_xDBL_A24(ec_point_t *Q, const ec_point_t *P, const ec_point_t *A24, const bool A24_normalized)
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
ec_xADD(ec_point_t *R, const ec_point_t *P, const ec_point_t *Q, const ec_point_t *PQ)
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
ec_xDBLADD(ec_point_t *R,
           ec_point_t *S,
           const ec_point_t *P,
           const ec_point_t *Q,
           const ec_point_t *PQ,
           const ec_point_t *A24)
{ // Simultaneous doubling and differential addition.
  // Input:  projective Montgomery points P=(XP:ZP) and Q=(XQ:ZQ) such that xP=XP/ZP and xQ=XQ/ZQ, the difference
  //         PQ=P-Q=(XPQ:ZPQ), and the normalised Montgomery curve constants A24 = (A+2C/4C:1).
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

static void
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
        ec_xDBLADD(&R0, &R1, &R0, &R1, P, &A24);
    }
    swap = 0 ^ prevbit;
    mask = 0 - (digit_t)swap;
    cswap_points(&R0, &R1, mask);

    fp2_copy(&Q->x, &R0.x);
    fp2_copy(&Q->z, &R0.z);
}

static int
xDBLMUL(ec_point_t *S,
        const ec_point_t *P,
        const ibz_t *k,
        const ec_point_t *Q,
        const ibz_t *l,
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
    ibz_t k_t = { 0 }, l_t = { 0 }, tmp = { 0 };
    digit_t r[2 * BITS] = { 0 };
    ec_point_t DIFF1a, DIFF1b, DIFF2a, DIFF2b, R[3] = { 0 }, T[3];

    assert(kbits > 0);

    // differential additions formulas are invalid in this case
    uint32_t invalid_input = ec_has_zero_coordinate(P) | ec_has_zero_coordinate(Q) | ec_has_zero_coordinate(PQ);
    // Validity bit of the input points: the bases fed to the biladder during signing are published in or recomputable
    // from the signature, so this bit is public; the coordinates themselves stay tainted
    CT_TESTING_MAKE_PUBLIC(&invalid_input, sizeof(invalid_input));
    if (invalid_input)
        return 0;

    // Derive sigma according to parity
    bitk0 = 1 & ibz_is_odd(k);
    bitl0 = 1 & ibz_is_odd(l);
    maskk = 0 - bitk0; // Parity masks: 0 if even, otherwise 1...1
    maskl = 0 - bitl0;
    sigma[0] = (bitk0 ^ 1);
    sigma[1] = (bitl0 ^ 1);
    evens = sigma[0] + sigma[1]; // Count number of even scalars
    mevens = 0 - (evens & 1);    // Mask mevens <- 0 if # even of scalars = 0 or 2, otherwise mevens = 1...1

    // If k and l are both even or both odd, pick sigma = (0,1)
    sigma[0] = (sigma[0] & mevens);
    sigma[1] = (sigma[1] & mevens) | (1 & ~mevens);

    // Convert even scalars to odd. k - 1 (resp. l - 1) can go negative only when k
    // (resp. l) is 0, in which case we want the kbits-bit wraparound value 2^kbits - 1,
    // not the true (negative) integer -- ibz_div_2exp below rounds towards zero, so a
    // genuine -1 would divide down to 0 instead of staying congruent to -1 mod 2^kbits.
    ibz_sub(&k_t, k, &ibz_const_one);
    ibz_mod2exp(&k_t, &k_t, (uint32_t)kbits);
    ibz_sub(&l_t, l, &ibz_const_one);
    ibz_mod2exp(&l_t, &l_t, (uint32_t)kbits);

    ibz_copy(&tmp, k);
    ibz_cswap(&k_t, &tmp, maskk);
    ibz_copy(&tmp, l);
    ibz_cswap(&l_t, &tmp, maskl);

    // Scalar recoding
    for (i = 0; i < kbits; i++) {
        // If sigma[0] = 1 swap k_t and l_t
        maskk = 0 - (sigma[0] ^ pre_sigma);
        ibz_cswap(&k_t, &l_t, maskk);

        if (i == kbits - 1) {
            bs1_ip1 = 0;
            bs2_ip1 = 0;
        } else {
            bs1_ip1 = 1 & ibz_is_odd(&k_t);
            ibz_div_2exp(&k_t, &k_t, 1);
            ibz_set_bound(&k_t, NWORDS_ORDER * NUM_BITS_LIMB + 1);
            bs2_ip1 = 1 & ibz_is_odd(&l_t);
            ibz_div_2exp(&l_t, &l_t, 1);
            ibz_set_bound(&l_t, NWORDS_ORDER * NUM_BITS_LIMB + 1);
        }
        bs1_i = k_t.limbs[0] & 1;
        bs2_i = l_t.limbs[0] & 1;

        r[2 * i] = bs1_i ^ bs1_ip1;
        r[2 * i + 1] = bs2_i ^ bs2_ip1;

        // Revert sigma if second bit, r_(2i+1), is 1
        pre_sigma = sigma[0];
        maskk = 0 - r[2 * i + 1];
        temp = ((sigma[0] ^ sigma[1]) & maskk) ^ sigma[0];
        sigma[1] = ((sigma[1] ^ sigma[0]) & maskk) ^ sigma[1];
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
    ec_xADD(&R[2], &R[1], &R[2], PQ);
    uint32_t invalid_sum = ec_has_zero_coordinate(&R[2]);
    // Validity bit of P+Q, public for the same reason as the input-validity bit above
    CT_TESTING_MAKE_PUBLIC(&invalid_sum, sizeof(invalid_sum));
    if (invalid_sum)
        return 0; // non valid formulas

    fp2_copy(&DIFF2a.x, &R[2].x);
    fp2_copy(&DIFF2a.z, &R[2].z);
    fp2_copy(&DIFF2b.x, &PQ->x);
    fp2_copy(&DIFF2b.z, &PQ->z);

    A_is_zero = fp2_is_zero(&curve->A);
    // Whether A == 0 is a public property of the curve: the curves reaching the biladder during signing are published
    // in or recomputable from the signature; the curve itself stays tainted
    CT_TESTING_MAKE_PUBLIC(&A_is_zero, sizeof(A_is_zero));

    // Main loop
    for (i = kbits - 1; i >= 0; i--) {
        h = r[2 * i] + r[2 * i + 1]; // in {0, 1, 2}
        maskk = 0 - (h & 1);
        select_point(&T[0], &R[0], &R[1], maskk);
        maskk = 0 - (h >> 1);
        select_point(&T[0], &T[0], &R[2], maskk);
        if (A_is_zero) {
            ec_xDBL_E0(&T[0], &T[0]);
        } else {
            assert(fp2_is_one(&curve->A24.z));
            ec_xDBL_A24(&T[0], &T[0], &curve->A24, true);
        }

        maskk = 0 - r[2 * i + 1]; // in {0, 1}
        select_point(&T[1], &R[0], &R[1], maskk);
        select_point(&T[2], &R[1], &R[2], maskk);

        cswap_points(&DIFF1a, &DIFF1b, maskk);
        ec_xADD(&T[1], &T[1], &T[2], &DIFF1a);
        ec_xADD(&T[2], &R[0], &R[2], &DIFF2a);

        // If hw (mod 2) = 1 then swap DIFF2a and DIFF2b
        maskk = 0 - (h & 1);
        cswap_points(&DIFF2a, &DIFF2b, maskk);

        // R <- T
        ec_copy_point(&R[0], &T[0]);
        ec_copy_point(&R[1], &T[1]);
        ec_copy_point(&R[2], &T[2]);
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
    ec_copy_point(&X0, Q);
    ec_copy_point(&X1, P);
    ec_copy_point(&X2, PQ);

    int i, j;
    digit_t t;
    for (i = 0; i < NWORDS_ORDER; i++) {
        t = 1;
        for (j = 0; j < RADIX; j++) {
            cswap_points(&X1, &X2, -((t & m[i]) == 0));
            ec_xDBLADD(&X0, &X1, &X0, &X1, &X2, &E->A24);
            cswap_points(&X1, &X2, -((t & m[i]) == 0));
            t <<= 1;
        };
    };
    ec_copy_point(R, &X1);
    return 1;
}

// WRAPPERS to export

void
ec_dbl(ec_point_t *res, const ec_point_t *P, const ec_curve_t *curve)
{
    // If A24 = ((A+2)/4 : 1) we save multiplications
    if (curve->is_A24_computed_and_normalized) {
        assert(fp2_is_one(&curve->A24.z));
        ec_xDBL_A24(res, P, &curve->A24, true);
    } else {
        // Otherwise we compute A24 on the fly for doubling
        ec_xDBL(res, P, (const ec_point_t *)curve);
    }
}

void
ec_dbl_iter(ec_point_t *res, int n, const ec_point_t *P, ec_curve_t *curve)
{
    if (n == 0) {
        ec_copy_point(res, P);
        return;
    }

    // When the chain is long enough, we should normalise A24
    if (n > 50) {
        ec_curve_normalize_A24(curve);
    }

    // When A24 is normalized we can save some multiplications
    if (curve->is_A24_computed_and_normalized) {
        assert(fp2_is_one(&curve->A24.z));
        ec_xDBL_A24(res, P, &curve->A24, true);
        for (int i = 0; i < n - 1; i++) {
            assert(fp2_is_one(&curve->A24.z));
            ec_xDBL_A24(res, res, &curve->A24, true);
        }
    } else {
        // Otherwise we do normal doubling
        ec_xDBL(res, P, (const ec_point_t *)curve);
        for (int i = 0; i < n - 1; i++) {
            ec_xDBL(res, res, (const ec_point_t *)curve);
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
    uint32_t pmq_invalid = fp2_is_zero(&PQ->PmQ.z);
    // Validity bit of the basis: the bases used during signing are published in or recomputable from the signature, so
    // this bit is public; the coordinates themselves stay tainted
    CT_TESTING_MAKE_PUBLIC(&pmq_invalid, sizeof(pmq_invalid));
    if (pmq_invalid)
        return 0;

    /* Differential additions behave badly when PmQ = (0:1), so we need to treat this case specifically. Since we assume
     * P, Q are a basis, this can happen only if kbits==1 */
    if (kbits == 1) {
        // Sanity check: our basis should be given by 2-torsion points
        if (!ec_is_two_torsion(&PQ->P, curve) || !ec_is_two_torsion(&PQ->Q, curve) ||
            !ec_is_two_torsion(&PQ->PmQ, curve))
            return 0;
        // res <- (1:0), P, Q or PmQ according to the scalar parity bits, selected in constant time
        digit_t bP = scalarP[0] & 1;
        digit_t bQ = scalarQ[0] & 1;
        ec_point_t T0, T1;
        ec_point_init(&T0);
        select_point(&T0, &T0, &PQ->Q, 0 - bQ);      // bP = 0: (1:0) or Q
        select_point(&T1, &PQ->P, &PQ->PmQ, 0 - bQ); // bP = 1: P or PmQ
        select_point(res, &T0, &T1, 0 - bP);
        return 1;
    } else {
        ec_curve_t E;
        ec_copy_curve(&E, curve);
        ibz_t ibz_scalarP = { 0 }, ibz_scalarQ = { 0 };

        ibz_copy_bits(&ibz_scalarP, scalarP, kbits);
        ibz_copy_bits(&ibz_scalarQ, scalarQ, kbits);

        uint32_t a_is_zero = fp2_is_zero(&curve->A);
        // Whether A == 0 is a public property of the curve (published in or recomputable from the signature)
        CT_TESTING_MAKE_PUBLIC(&a_is_zero, sizeof(a_is_zero));
        if (!a_is_zero) { // If A is not zero normalize
            ec_curve_normalize_A24(&E);
        }
        return xDBLMUL(res, &PQ->P, &ibz_scalarP, &PQ->Q, &ibz_scalarQ, &PQ->PmQ, kbits, (const ec_curve_t *)&E);
    }
}

int
ec_biscalar_mul_verif(ec_point_t *res,
                      const digit_t *scalarP,
                      const digit_t *scalarQ,
                      const int kbits,
                      const ec_basis_t *PQ,
                      ec_curve_t *curve)
{
    // Adapted from Algorithm 9 of https://eprint.iacr.org/2017/212.pdf

    ibz_t s0 = { 0 }, s1 = { 0 }, t0 = { 0 };
    ec_point_t x0, x1, xdiff, xtmp;

    // Differential addition formulas are invalid when P, Q or P-Q have zero coordinates.
    if (ec_has_zero_coordinate(&PQ->P) | ec_has_zero_coordinate(&PQ->Q) | ec_has_zero_coordinate(&PQ->PmQ))
        return 0;

    ibz_copy_bits(&s0, scalarP, kbits);
    ibz_copy_bits(&s1, scalarQ, kbits);

    if (ibz_is_zero(&s0) && ibz_is_zero(&s1))
        return 0;

    ec_copy_point(&x0, &PQ->P);
    ec_copy_point(&x1, &PQ->Q);
    ec_copy_point(&xdiff, &PQ->PmQ);

    if (!fp2_is_zero(&curve->A)) { // If A is not zero normalize
        ec_curve_normalize_A24(curve);
    }

    while (!ibz_is_zero(&s0)) {
        if (ibz_cmp(&s1, &s0) < 0) {
            ibz_swap(&s0, &s1);
            swap_points(&x0, &x1);
        }
        ibz_copy(&t0, &s0);
        ibz_mul_2exp(&t0, &t0, 2); // t0 = 4*s0
        if (ibz_cmp(&s1, &t0) < 0) {
            ibz_sub(&s1, &s1, &s0);
            ec_copy_point(&xtmp, &x0);
            ec_xADD(&x0, &x1, &x0, &xdiff);
            ec_copy_point(&xdiff, &xtmp);
            if (ec_has_zero_coordinate(&x0)) // non valid formulas
                return 0;
        } else if (ibz_is_odd(&s1) == ibz_is_odd(&s0)) {
            ibz_sub(&s1, &s1, &s0);
            ibz_div_2exp(&s1, &s1, 1);
            ec_xADD(&x0, &x1, &x0, &xdiff);
            ec_xDBL_A24(&x1, &x1, &curve->A24, curve->is_A24_computed_and_normalized);
            if (ec_has_zero_coordinate(&x0) | ec_has_zero_coordinate(&x1)) // non valid formulas
                return 0;
        } else if (!ibz_is_odd(&s1)) {
            ibz_div_2exp(&s1, &s1, 1);
            ec_xADD(&xdiff, &x1, &xdiff, &x0);
            ec_xDBL_A24(&x1, &x1, &curve->A24, curve->is_A24_computed_and_normalized);
            if (ec_has_zero_coordinate(&xdiff) | ec_has_zero_coordinate(&x1)) // non valid formulas
                return 0;
        } else {
            ibz_div_2exp(&s0, &s0, 1);
            ec_xADD(&xdiff, &x0, &xdiff, &x1);
            ec_xDBL_A24(&x0, &x0, &curve->A24, curve->is_A24_computed_and_normalized);
            if (ec_has_zero_coordinate(&xdiff) | ec_has_zero_coordinate(&x0)) // non valid formulas
                return 0;
        }
    }

    while (!ibz_is_odd(&s1)) {
        ibz_div_2exp(&s1, &s1, 1);
        ec_xDBL_A24(&x1, &x1, &curve->A24, curve->is_A24_computed_and_normalized);
    }

    if (!ibz_is_one(&s1)) {
        ec_mul(&x1, s1.limbs, kbits, &x1, curve);
    }

    if (ec_has_zero_coordinate(&x1)) // non valid formulas
        return 0;

    ec_copy_point(res, &x1);
    return 1;
}

// Take P,Q and PmQ in xz-coordinates and outputs the barycentric coordinates.
void
ec_points_to_bary_coordinates(ec_bary_coordinates_t *uvw, ec_point_t *P, ec_point_t *Q, ec_point_t *PmQ)
{
    fp2_t t0, t1, t2, t3;

    fp2_add(&t0, &P->x, &P->z); // t0 = xP + zP
    fp2_sub(&t1, &P->x, &P->z); // t1 = xP - zP
    fp2_add(&t2, &Q->x, &Q->z); // t2 = xQ + zQ
    fp2_sub(&t3, &Q->x, &Q->z); // t3 = xQ - zQ
    fp2_mul(&t0, &t0, &t3);     // t0 = (xP + zP)(xQ - zQ)
    fp2_mul(&t1, &t1, &t2);     // t1 = (xP - zP)(xQ + zQ)
    fp2_add(&t2, &t1, &t0);     // t2 = (xP - zP)(xQ + zQ) + (xP + zP)(xQ - zQ) = lambda
    fp2_sub(&t3, &t1, &t0);     // t3 = (xP - zP)(xQ + zQ) - (xP + zP)(xQ - zQ)  = mu
    fp2_mul(&t0, &PmQ->z, &t2); // t0 = lamdba * z_{PmQ}
    fp2_sqr(&t0, &t0);          // t0 = (lamdba * z_{PmQ})^2
    fp2_mul(&t1, &t3, &PmQ->x); // t1 = mu * x_{PmQ}
    fp2_mul(&t2, &t3, &PmQ->z); // t2 = mu * z_{PmQ}
    fp2_sqr(&t3, &t1);          // t3 = (mu * x_{PmQ})^2

    fp2_add(&uvw->u, &t3, &t0);         // u =  (mu * x_{PmQ})^2 + (lamdba * z_{PmQ})^2
    fp2_sub(&uvw->v, &t3, &t0);         // v = (mu * x_{PmQ})^2 - (lamdba * z_{PmQ})^2
    fp2_mul(&uvw->w, &t1, &t2);         // w = mu^2 * z_{PmQ} * x_{PmQ}
    fp2_add(&uvw->w, &uvw->w, &uvw->w); // w = 2(mu^2 * z_{PmQ} * x_{PmQ})
}
