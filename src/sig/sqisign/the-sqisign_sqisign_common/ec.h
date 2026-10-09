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

/** @brief barycentric coordinates
 *
 * @typedef ec_bary_coordinates_t
 *
 * @struct ec_bary_coordinates_t
 *
 * 3 components u,v,w that define the (X:Z) coordinates of both addition and substraction of two distinct points with
 * P+Q =(u-v:w) and P-Q = (u+v:w)
 */
typedef struct ec_bary_coordinates_t
{
    fp2_t u;
    fp2_t v;
    fp2_t w;
} ec_bary_coordinates_t;

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

// end ec_t
/** @}
 */

/** @defgroup ec_curve_t Curves
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
ec_copy_point(ec_point_t *P, const ec_point_t *Q)
{
    fp2_copy(&P->x, &Q->x);
    fp2_copy(&P->z, &Q->z);
}

static inline void
ec_copy_basis(ec_basis_t *B1, const ec_basis_t *B0)
{
    ec_copy_point(&B1->P, &B0->P);
    ec_copy_point(&B1->Q, &B0->Q);
    ec_copy_point(&B1->PmQ, &B0->PmQ);
}

static inline void
ec_copy_curve(ec_curve_t *E1, const ec_curve_t *E2)
{
    fp2_copy(&(E1->A), &(E2->A));
    fp2_copy(&(E1->C), &(E2->C));
    E1->is_A24_computed_and_normalized = E2->is_A24_computed_and_normalized;
    ec_copy_point(&E1->A24, &E2->A24);
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

void ec_xADD(ec_point_t *R, const ec_point_t *P, const ec_point_t *Q, const ec_point_t *PQ);
void ec_xDBL(ec_point_t *Q, const ec_point_t *P, const ec_point_t *AC);
void ec_xDBL_E0(ec_point_t *Q, const ec_point_t *P);
void ec_xDBL_A24(ec_point_t *Q, const ec_point_t *P, const ec_point_t *A24, const bool A24_normalized);
void ec_xDBLADD(ec_point_t *R,
                ec_point_t *S,
                const ec_point_t *P,
                const ec_point_t *Q,
                const ec_point_t *PQ,
                const ec_point_t *A24);

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

/**
 * @brief Linear combination of points of a basis, done faster but in non-constant time
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
int ec_biscalar_mul_verif(ec_point_t *res,
                          const digit_t *scalarP,
                          const digit_t *scalarQ,
                          const int kbits,
                          const ec_basis_t *PQ,
                          ec_curve_t *curve);

/**
 * @brief Given P, Q, P-Q, compute u,v,w s.t. P+Q = (u-v:w) and P-Q = (u+v:w)
 *
 * @param uvw output: the barycentric coordinates
 * @param P: a elliptic curve point in xz coordinates.
 * @param Q: a elliptic curve point in xz coordinates.
 * @param PmQ: P-Q in xz coordinates.
 */
void ec_points_to_bary_coordinates(ec_bary_coordinates_t *uvw, ec_point_t *P, ec_point_t *Q, ec_point_t *PmQ);

// end point computations
/**
 * @}
 */

/** @defgroup ec_dlog_t Torsion basis
 * @{
 */

/**
 * @brief Generate a 2^f-torsion basis from a Montgomery curve along with a hint
 *
 * @param PQ2 an ec_basis_t
 * @param curve an ec_curve_t
 * @param f an integer
 * @param e an integer, which, if non zero normalize the 2^f-basis to be compatible with a call of
 * `ec_curve_to_basis_2f_from_hint(&B, E, e)` later
 *
 * @return A hint
 *
 * The algorithm is deterministc
 */
uint8_t ec_curve_to_basis_2f_to_hint(ec_basis_t *PQ2, ec_curve_t *curve, int f, int e);

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

/** @defgroup ec_isom Isomorphisms
 * @{
 */

/** @brief A 2x2 matrix used for dim 1 basis change of coordinates
 * @typedef ec_change_coord_matrix_t
 *
 * @struct ec_change_coord_matrix
 */
typedef struct ec_change_coord_matrix
{
    fp2_t a;
    fp2_t b;
    fp2_t c;
    fp2_t d;
} ec_change_coord_matrix_t;

/**
 * @brief Apply the change of coordinate matrix M to a point P.
 *
 * @param P: (input/output) a xz-point on an eleptic curve
 * @param M: an change of coordinate matrix
 *
 */
void ec_apply_isomorphism(ec_point_t *P, const ec_change_coord_matrix_t *M);

// Normalize the Montgomery coefficient of a curve

/**
 * @brief Transform a curve E with basis B into its normalised form (with A biggest A coefficient)
 *
 * @param E: (input/output) a Montgomery curve E
 * @param four_torsion: (input/output) a 4 torsion point of E, not above (0,0).
 * @param B: (input/output) A basis of E
 *
 * @return 0 if not valid.
 */
bool ec_normalize_montgomery(ec_curve_t *E, const ec_point_t *four_torsion, ec_basis_t *B);

//
/**
 * @brief Convert a level 2 theta null point to a Montgomery curve
 *
 * @param E: (output) a Montgomery curve E
 * @param M: an change of coordinate matrix
 * @param th: an theta null point representing E
 *
 * @return 0 if not valid.
 */

bool ec_theta_to_montgomery(ec_curve_t *E, ec_change_coord_matrix_t *M, const ec_point_t *th);

#ifndef NDEBUG
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
static inline int
test_point_order_twof(const ec_point_t *P, const ec_curve_t *E, int t)
{
    ec_point_t test;
    ec_curve_t curve;
    test = *P;
    ec_copy_curve(&curve, E);

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
static inline int
test_basis_order_twof(const ec_basis_t *B, const ec_curve_t *E, int t)
{
    int check_P = test_point_order_twof(&B->P, E, t);
    int check_Q = test_point_order_twof(&B->Q, E, t);
    int check_PmQ = test_point_order_twof(&B->PmQ, E, t);

    return check_P & check_Q & check_PmQ;
}

// unused
#if 0
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
#endif

#endif
// end isogeny computations
/**
 * @}
 */

// end ec
/**
 * @}
 */
