/** @file
 *
 * @authors Luca De Feo, Francisco RH
 *
 * @brief Elliptic curve stuff
 */

#ifndef EC_H
#define EC_H
#include <sqisign_namespace.h>
#include <ec_params.h>
#include <fp2.h>
#include <tools.h>
#include <stdio.h>

/** @defgroup ec Elliptic curves
 * @{
 */

/** @defgroup ec_t Data structures
 * @{
 */

/** @brief Projective point on the Kummer line E/pm 1 in Montgomery coordinates
 *
 * @typedef ec_point_t
 *
 * @struct ec_point_t
 *
 * A projective point in (X:Z) or (X:Y:Z) coordinates (tbd).
 */
typedef struct ec_point_t
{
    fp2_t x;
    fp2_t z;
} ec_point_t;

/** @brief Projective point in Montgomery coordinates
 *
 * @typedef jac_point_t
 *
 * @struct jac_point_t
 *
 * A projective point in (X:Y:Z) coordinates
 */
typedef struct jac_point_t
{
    fp2_t x;
    fp2_t y;
    fp2_t z;
} jac_point_t;

/** @brief Addition components
 *
 * @typedef add_components_t
 *
 * @struct add_components_t
 *
 * 3 components u,v,w that define the (X:Z) coordinates of both
 * addition and substraction of two distinct points with
 * P+Q =(u-v:w) and P-Q = (u+v=w)
 */
typedef struct add_components_t
{
    fp2_t u;
    fp2_t v;
    fp2_t w;
} add_components_t;

/** @brief A basis of a torsion subgroup
 *
 * @typedef ec_basis_t
 *
 * @struct ec_basis_t
 *
 * A pair of points (or a triplet, tbd) forming a basis of a torsion subgroup.
 */
typedef struct ec_basis_t
{
    ec_point_t P;
    ec_point_t Q;
    ec_point_t PmQ;
} ec_basis_t;

/** @brief An elliptic curve
 *
 * @typedef ec_curve_t
 *
 * @struct ec_curve_t
 *
 * An elliptic curve in projective Montgomery form
 */
typedef struct ec_curve_t
{
    fp2_t A;
    fp2_t C;                             ///< cannot be 0
    ec_point_t A24;                      // the point (A+2 : 4C)
    bool is_A24_computed_and_normalized; // says if A24 has been computed and normalized
} ec_curve_t;

/** @brief An isogeny of degree a power of 2
 *
 * @typedef ec_isog_even_t
 *
 * @struct ec_isog_even_t
 */
typedef struct ec_isog_even_t
{
    ec_curve_t curve;  ///< The domain curve
    ec_point_t kernel; ///< A kernel generator
    unsigned length;   ///< The length as a 2-isogeny walk
} ec_isog_even_t;

/** @brief Isomorphism of Montgomery curves
 *
 * @typedef ec_isom_t
 *
 * @struct ec_isom_t
 *
 * The isomorphism is given by the map maps (X:Z) ↦ ( (Nx X + Nz Z) : (D Z) )
 */
typedef struct ec_isom_t
{
    fp2_t Nx;
    fp2_t Nz;
    fp2_t D;
} ec_isom_t;

// end ec_t
/** @}
 */

/** @defgroup ec_curve_t Curves and isomorphisms
 * @{
 */

// Initalisation for curves and points
void ec_curve_init(ec_curve_t *E);
void ec_point_init(ec_point_t *P);

/**
 * @brief Verify that a Montgomery coefficient is valid
 *
 * @param A an fp2_t
 *
 * @return 0  if curve is invalid, 1 otherwise
 */
int ec_curve_verify_A(const fp2_t *A);

/**
 * @brief Initialize an elliptic curve from a coefficient
 *
 * @param A an fp2_t
 * @param E the elliptic curve to initialize
 *
 * @return 0  if curve is invalid, 1 otherwise
 */
int ec_curve_init_from_A(ec_curve_t *E, const fp2_t *A);

// Copying points, bases and curves
static inline void
copy_point(ec_point_t *P, const ec_point_t *Q)
{
    fp2_copy(&P->x, &Q->x);
    fp2_copy(&P->z, &Q->z);
}

static inline void
copy_basis(ec_basis_t *B1, const ec_basis_t *B0)
{
    copy_point(&B1->P, &B0->P);
    copy_point(&B1->Q, &B0->Q);
    copy_point(&B1->PmQ, &B0->PmQ);
}

static inline void
copy_curve(ec_curve_t *E1, const ec_curve_t *E2)
{
    fp2_copy(&(E1->A), &(E2->A));
    fp2_copy(&(E1->C), &(E2->C));
    E1->is_A24_computed_and_normalized = E2->is_A24_computed_and_normalized;
    copy_point(&E1->A24, &E2->A24);
}

// Functions for working with the A24 point and normalisation

/**
 * @brief Reduce (A : C) to (A/C : 1) in place
 *
 * @param E a curve
 */
void ec_normalize_curve(ec_curve_t *E);

/**
 * @brief Reduce (A + 2 : 4C) to ((A+2)/4C : 1) in place
 *
 * @param E a curve
 */
void ec_curve_normalize_A24(ec_curve_t *E);

/**
 * @brief Normalise both (A : C) and (A + 2 : 4C) as above, in place
 *
 * @param E a curve
 */
void ec_normalize_curve_and_A24(ec_curve_t *E);

/**
 * @brief Given a curve E, compute (A+2 : 4C)
 *
 * @param A24 the value (A+2 : 4C) to return into
 * @param E a curve
 */
static inline void
AC_to_A24(ec_point_t *A24, const ec_curve_t *E)
{
    // Maybe we already have this computed
    if (E->is_A24_computed_and_normalized) {
        copy_point(A24, &E->A24);
        return;
    }

    // A24 = (A+2C : 4C)
    fp2_add(&A24->z, &E->C, &E->C);
    fp2_add(&A24->x, &E->A, &A24->z);
    fp2_add(&A24->z, &A24->z, &A24->z);
}

/**
 * @brief Given a curve the point (A+2 : 4C) compute the curve coefficients (A : C)
 *
 * @param E a curve to compute
 * @param A24 the value (A+2 : 4C)
 */
static inline void
A24_to_AC(ec_curve_t *E, const ec_point_t *A24)
{
    // (A:C) = ((A+2C)*2-4C : 4C)
    fp2_add(&E->A, &A24->x, &A24->x);
    fp2_sub(&E->A, &E->A, &A24->z);
    fp2_add(&E->A, &E->A, &E->A);
    fp2_copy(&E->C, &A24->z);
}

/**
 * @brief j-invariant.
 *
 * @param j_inv computed j_invariant
 * @param curve input curve
 */
void ec_j_inv(fp2_t *j_inv, const ec_curve_t *curve);

/**
 * @brief Isomorphism of elliptic curve
 * Takes as input two isomorphic Kummer lines in Montgomery form, and output an isomorphism between
 * them
 *
 * @param isom computed isomorphism
 * @param from domain curve
 * @param to image curve
 * @return 0xFFFFFFFF if there was an error during the computation, zero otherwise
 */
uint32_t ec_isomorphism(ec_isom_t *isom, const ec_curve_t *from, const ec_curve_t *to);

/**
 * @brief In-place evaluation of an isomorphism
 *
 * @param P a point
 * @param isom an isomorphism
 */
void ec_iso_eval(ec_point_t *P, ec_isom_t *isom);

/** @}
 */
/** @defgroup ec_point_t Point operations
 * @{
 */

/**
 * @brief Point equality
 *
 * @param P a point
 * @param Q a point
 * @return 0xFFFFFFFF if equal, zero otherwise
 */
uint32_t ec_is_equal(const ec_point_t *P, const ec_point_t *Q);

/**
 * @brief Point equality
 *
 * @param P a point
 * @return 0xFFFFFFFF if point at infinity, zero otherwise
 */
uint32_t ec_is_zero(const ec_point_t *P);

/**
 * @brief Two torsion test
 *
 * @param P a point
 * @param E the elliptic curve
 * @return 0xFFFFFFFF if P is 2-torsion but not zero, zero otherwise
 */
uint32_t ec_is_two_torsion(const ec_point_t *P, const ec_curve_t *E);

/**
 * @brief Four torsion test
 *
 * @param P a point
 * @param E the elliptic curve
 * @return 0xFFFFFFFF if P is 2-torsion but not zero, zero otherwise
 */
uint32_t ec_is_four_torsion(const ec_point_t *P, const ec_curve_t *E);

/**
 * @brief Reduce Z-coordinate of point in place
 *
 * @param P a point
 */
void ec_normalize_point(ec_point_t *P);

void xDBL_E0(ec_point_t *Q, const ec_point_t *P);
void xADD(ec_point_t *R, const ec_point_t *P, const ec_point_t *Q, const ec_point_t *PQ);
void xDBL_A24(ec_point_t *Q, const ec_point_t *P, const ec_point_t *A24, const bool A24_normalized);

/**
 * @brief Point doubling
 *
 * @param res computed double of P
 * @param P a point
 * @param curve an elliptic curve
 */
void ec_dbl(ec_point_t *res, const ec_point_t *P, const ec_curve_t *curve);

/**
 * @brief Point iterated doubling
 *
 * @param res computed double of P
 * @param P a point
 * @param n the number of double
 * @param curve the curve on which P lays
 */
void ec_dbl_iter(ec_point_t *res, int n, const ec_point_t *P, ec_curve_t *curve);

/**
 * @brief Iterated doubling for a basis P, Q, PmQ
 *
 * @param res the computed iterated double of basis B
 * @param n the number of doubles
 * @param B the basis to double
 * @param curve the parent curve of the basis
 */
void ec_dbl_iter_basis(ec_basis_t *res, int n, const ec_basis_t *B, ec_curve_t *curve);

/**
 * @brief Point multiplication
 *
 * @param res computed scalar * P
 * @param curve the curve
 * @param scalar an unsigned multi-precision integer
 * @param P a point
 * @param kbits numer of bits of the scalar
 */
void ec_mul(ec_point_t *res, const digit_t *scalar, const int kbits, const ec_point_t *P, ec_curve_t *curve);

/**
 * @brief Combination P+m*Q
 *
 * @param R computed P + m * Q
 * @param curve the curve
 * @param m an unsigned multi-precision integer
 * @param P a point
 * @param Q a point
 * @param PQ the difference P-Q
 * @return 0 if there was an error, 1 otherwise
 */
int ec_ladder3pt(ec_point_t *R,
                 const digit_t *m,
                 const ec_point_t *P,
                 const ec_point_t *Q,
                 const ec_point_t *PQ,
                 const ec_curve_t *curve);

/**
 * @brief Linear combination of points of a basis
 *
 * @param res computed scalarP * P + scalarQ * Q
 * @param scalarP an unsigned multi-precision integer
 * @param scalarQ an unsigned multi-precision integer
 * @param kbits number of bits of the scalars, or n for points of order 2^n
 * @param PQ a torsion basis consisting of points P and Q
 * @param curve the curve
 *
 * @return 0 if there was an error, 1 otherwise
 */
int ec_biscalar_mul(ec_point_t *res,
                    const digit_t *scalarP,
                    const digit_t *scalarQ,
                    const int kbits,
                    const ec_basis_t *PQ,
                    const ec_curve_t *curve);

// end point computations
/**
 * @}
 */

/** @defgroup ec_dlog_t Torsion basis computations
 * @{
 */

/**
 * @brief Generate a 2^f-torsion basis from a Montgomery curve along with a hint
 *
 * @param PQ2 an ec_basis_t
 * @param curve an ec_curve_t
 * @param f an integer
 *
 * @return A hint
 *
 * The algorithm is deterministc
 */
uint8_t ec_curve_to_basis_2f_to_hint(ec_basis_t *PQ2, ec_curve_t *curve, int f);

/**
 * @brief Generate a 2^f-torsion basis from a Montgomery curve and a given hint
 *
 * @param PQ2 an ec_basis_t
 * @param curve an ec_curve_t
 * @param f an integer
 * @param hint the hint
 *
 * @return 1 is the basis is valid, 0 otherwise
 *
 * The algorithm is deterministc
 */
int ec_curve_to_basis_2f_from_hint(ec_basis_t *PQ2, ec_curve_t *curve, int f, const uint8_t hint);
/** // end basis computations
 * @}
 */

/** @defgroup ec_isog_t Isogenies
 * @{
 */

/**
 * @brief Evaluate isogeny of even degree on list of points.
 * Returns 0 if successful and -1 if kernel has the wrong order or includes (0:1).
 *
 * @param image computed image curve
 * @param phi isogeny
 * @param points a list of points to evaluate the isogeny on, modified in place
 * @param len_points length of the list points
 *
 * @return 0 if there was no error, 0xFFFFFFFF otherwise
 */
uint32_t ec_eval_even(ec_curve_t *image, ec_isog_even_t *phi, ec_point_t *points, unsigned len_points);

/**
 * @brief Multiplicative strategy for a short isogeny chain. Returns 1 if successfull and -1
 * if kernel has the wrong order or includes (0:1) when special=false.
 *
 * @param curve domain curve, to be overwritten by the codomain curve.
 * @param kernel a kernel generator of order 2^len
 * @param len the length of t he 2-isogeny chain
 * @param points a list of points to evaluate the isogeny on, modified in place
 * @param len_points length of the list points
 * @param special if true, allow isogenies with (0:1) in the kernel
 *
 * @return 0 if there was no error, 0xFFFFFFFF otherwise
 */
uint32_t ec_eval_small_chain(ec_curve_t *curve,
                             const ec_point_t *kernel,
                             int len,
                             ec_point_t *points,
                             unsigned len_points,
                             bool special);

/**
 * @brief Recover Y-coordinate from X-coordinate and curve coefficients.
 *
 * @param y: a y-coordinate
 * @param Px: a x-coordinate
 * @param curve: the elliptic curve
 *
 * @return 0xFFFFFFFF if the point was on the curve, 0 otherwise
 */
uint32_t ec_recover_y(fp2_t *y, const fp2_t *Px, const ec_curve_t *curve);

// Jacobian point init and copying
void jac_init(jac_point_t *P);
void copy_jac_point(jac_point_t *P, const jac_point_t *Q);

/**
 * @brief Test if two Jacobian points are equal
 *
 * @param P: a point
 * @param Q: a point
 *
 * @return 0xFFFFFFFF if they are equal, 0 otherwise
 */
uint32_t jac_is_equal(const jac_point_t *P, const jac_point_t *Q);

// Convert from Jacobian to x-only (just drop the Y-coordinate)
void jac_to_xz(ec_point_t *P, const jac_point_t *xyP);
// Convert from Jacobian coordinates in Montgomery model to Weierstrass
void jac_to_ws(jac_point_t *P, fp2_t *t, fp2_t *ao3, const jac_point_t *Q, const ec_curve_t *curve);
void jac_from_ws(jac_point_t *Q, const jac_point_t *P, const fp2_t *ao3, const ec_curve_t *curve);

// Jacobian arithmetic
void jac_neg(jac_point_t *Q, const jac_point_t *P);
void ADD(jac_point_t *R, const jac_point_t *P, const jac_point_t *Q, const ec_curve_t *AC);
void DBL(jac_point_t *Q, const jac_point_t *P, const ec_curve_t *AC);
void DBLW(jac_point_t *Q, fp2_t *u, const jac_point_t *P, const fp2_t *t);
void jac_to_xz_add_components(add_components_t *uvw, const jac_point_t *P, const jac_point_t *Q, const ec_curve_t *AC);

/**
 * @brief Given a basis in x-only, lift to a pair of Jacobian points
 *
 * @param P: a point
 * @param Q: a point
 * @param B: a basis
 * @param E: an elliptic curve
 *
 * @return 0xFFFFFFFF if there was no error, 0 otherwise
 *
 *
 * Lifts a basis x(P), x(Q), x(P-Q) assuming the curve has (A/C : 1) and
 * the point P = (X/Z : 1). For generic implementation see lift_basis()
 */
uint32_t lift_basis_normalized(jac_point_t *P, jac_point_t *Q, ec_basis_t *B, ec_curve_t *E);

/**
 * @brief Given a basis in x-only, lift to a pair of Jacobian points
 *
 * @param P: a point
 * @param Q: a point
 * @param B: a basis
 * @param E: an elliptic curve
 *
 * @return 0xFFFFFFFF if there was no error, 0 otherwise
 */
uint32_t lift_basis(jac_point_t *P, jac_point_t *Q, ec_basis_t *B, ec_curve_t *E);

/**
 * @brief Check if basis points (P, Q) form a full 4-basis
 *
 * @param B: a basis
 * @param E: an elliptic curve
 *
 * @return 0xFFFFFFFF if they form a basis, 0 otherwise
 */
uint32_t ec_is_basis_four_torsion(const ec_basis_t *B, const ec_curve_t *E);

/*
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Test functions for printing and order checking, only used in debug mode
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

/**
 * @brief Check if a point (X : Z) has order exactly 2^t
 *
 * @param P: a point
 * @param E: an elliptic curve
 * @param t: an integer
 *
 * @return 0xFFFFFFFF if the order is correct, 0 otherwise
 */
static int
test_point_order_twof(const ec_point_t *P, const ec_curve_t *E, int t)
{
    ec_point_t test;
    ec_curve_t curve;
    test = *P;
    copy_curve(&curve, E);

    if (ec_is_zero(&test))
        return 0;
    // Scale point by 2^(t-1)
    ec_dbl_iter(&test, t - 1, &test, &curve);
    // If it's zero now, it doesnt have order 2^t
    if (ec_is_zero(&test))
        return 0;
    // Ensure [2^t] P = 0
    ec_dbl(&test, &test, &curve);
    return ec_is_zero(&test);
}

/**
 * @brief Check if basis points (P, Q, PmQ) all have order exactly 2^t
 *
 * @param B: a basis
 * @param E: an elliptic curve
 * @param t: an integer
 *
 * @return 0xFFFFFFFF if the order is correct, 0 otherwise
 */
static int
test_basis_order_twof(const ec_basis_t *B, const ec_curve_t *E, int t)
{
    int check_P = test_point_order_twof(&B->P, E, t);
    int check_Q = test_point_order_twof(&B->Q, E, t);
    int check_PmQ = test_point_order_twof(&B->PmQ, E, t);

    return check_P & check_Q & check_PmQ;
}

/**
 * @brief Check if a Jacobian point (X : Y : Z) has order exactly 2^f
 *
 * @param P: a point
 * @param E: an elliptic curve
 * @param t: an integer
 *
 * @return 0xFFFFFFFF if the order is correct, 0 otherwise
 */
static int
test_jac_order_twof(const jac_point_t *P, const ec_curve_t *E, int t)
{
    jac_point_t test;
    test = *P;
    if (fp2_is_zero(&test.z))
        return 0;
    for (int i = 0; i < t - 1; i++) {
        DBL(&test, &test, E);
    }
    if (fp2_is_zero(&test.z))
        return 0;
    DBL(&test, &test, E);
    return (fp2_is_zero(&test.z));
}

// Prints the x-coordinate of the point (X : 1)
static void
ec_point_print(const char *name, ec_point_t P)
{
    fp2_t a;
    if (fp2_is_zero(&P.z)) {
        printf("%s = INF\n", name);
    } else {
        fp2_copy(&a, &P.z);
        fp2_inv(&a);
        fp2_mul(&a, &a, &P.x);
        fp2_print(name, &a);
    }
}

// Prints the Montgomery coefficient A
static void
ec_curve_print(const char *name, ec_curve_t E)
{
    fp2_t a;
    fp2_copy(&a, &E.C);
    fp2_inv(&a);
    fp2_mul(&a, &a, &E.A);
    fp2_print(name, &a);
}

#endif
// end isogeny computations
/**
 * @}
 */

// end ec
/**
 * @}
 */
