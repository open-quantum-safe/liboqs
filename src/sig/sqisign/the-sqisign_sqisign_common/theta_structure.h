/** @file
 *
 * @authors Antonin Leroux
 *
 * @brief the theta structure header
 */

#ifndef THETA_STRUCTURE_H
#define THETA_STRUCTURE_H

#include <ec.h>
#include <fp2.h>
#include <hd.h>

/**
 * @ingroup hd_module
 * @defgroup hd_theta Theta structures
 * @{
 */

/**
 * @brief Perform the theta_hadamard transform on a theta point
 *
 * @param out Output: the theta_point
 * @param in a theta point*
 * in = (x,y,z,t)
 * out = (x+y+z+t, x-y+z-t, x+y-z-t, x-y-z+t)
 *
 */
static inline void
theta_hadamard(theta_point_t *out, const theta_point_t *in)
{
    fp2_t t1, t2, t3, t4;

    // t1 = x + y
    fp2_add(&t1, &in->x, &in->y);
    // t2 = x - y
    fp2_sub(&t2, &in->x, &in->y);
    // t3 = z + t
    fp2_add(&t3, &in->z, &in->t);
    // t4 = z - t
    fp2_sub(&t4, &in->z, &in->t);

    fp2_add(&out->x, &t1, &t3);
    fp2_add(&out->y, &t2, &t4);
    fp2_sub(&out->z, &t1, &t3);
    fp2_sub(&out->t, &t2, &t4);
}

/**
 * @brief compute the projective inverse of a point. Assumes it has no zero null coefficient
 *
 * @param inv Output: the inverse theta_point
 * @param in a theta point*
 * in = (x,y,z,t)
 * out = (yzt, xzt, xyt, xyz)
 */
static inline void
theta_invert_point(theta_point_t *inv, const theta_point_t *in)
{
    fp2_t a, b, c, d;
    fp2_mul(&a, &in->x, &in->y);
    fp2_mul(&d, &in->z, &in->t);
    fp2_copy(&b, &in->x);
    fp2_copy(&c, &in->z);
    fp2_mul(&inv->x, &in->y, &d);
    fp2_mul(&inv->y, &b, &d);
    fp2_mul(&inv->z, &in->t, &a);
    fp2_mul(&inv->t, &c, &a);
}

/**
 * @brief Square the coordinates of a theta point
 * @param out Output: the theta_point
 * @param in a theta point*
 * in = (x,y,z,t)
 * out = (x^2, y^2, z^2, t^2)
 *
 */
static inline void
pointwise_square(theta_point_t *out, const theta_point_t *in)
{
    fp2_sqr(&out->x, &in->x);
    fp2_sqr(&out->y, &in->y);
    fp2_sqr(&out->z, &in->z);
    fp2_sqr(&out->t, &in->t);
}

/**
 * @brief Square the coordinates of a theta point
 * @param out Output: the dot-product of those points
 * @param P a theta point*
 * @param Q a theta point*
 * in = (x,y,z,t), (a,b,c,d)
 * out = (xa, yb, zc, td)
 *
 */
static inline void
pointwise_product(theta_point_t *out, const theta_point_t *P, const theta_point_t *Q)
{
    fp2_mul(&out->x, &P->x, &Q->x);
    fp2_mul(&out->y, &P->y, &Q->y);
    fp2_mul(&out->z, &P->z, &Q->z);
    fp2_mul(&out->t, &P->t, &Q->t);
}

/**
 * @brief Copy a theta point
 * @param out Output: the copy
 * @param in a theta point*
 *
 */
static inline void
theta_copy(theta_point_t *out, const theta_point_t *in)
{
    fp2_copy(&out->x, &in->x);
    fp2_copy(&out->y, &in->y);
    fp2_copy(&out->z, &in->z);
    fp2_copy(&out->t, &in->t);
}

/**
 * @brief Square the coordinates and then perform the theta_hadamard transform
 *
 * @param out Output: the theta_point
 * @param in a theta point*
 * in = (x,y,z,t)
 * out = (x^2+y^2+z^2+t^2, x^2-y^2+z^2-t^2, x^2+y^2-z^2-t^2, x^2-y^2-z^2+t^2)
 *
 */
static inline void
to_squared_theta(theta_point_t *out, const theta_point_t *in)
{
    pointwise_square(out, in);
    theta_hadamard(out, out);
}

/**
 * @brief Perform the theta structure precomputation
 *
 * @param A Output: the theta_structure
 *
 * if A.dual_null_point = (x,y,z,t)
 * let (XX,YY,ZZ,TT) = to_squared_theta(x,y,z,t)
 * Compute A.inv_sqr_null_point = (1, XX/YY,XX/ZZ,XX/TT)
 * using A.precomp.
 *
 */
void theta_precomputation(theta_structure_t *A);

/**
 * @brief Compute the double of the theta point in on the theta struc A
 *
 * @param out Output: the theta_point
 * @param A a theta structure
 * @param in a theta point in the theta structure A
 * in = (x,y,z,t)
 * out = [2] (x,y,z,t)
 * /!\ assumes that no coordinates is zero and that the precomputation of A has been done
 *
 */
void theta_DBL(theta_point_t *out, theta_structure_t *A, const theta_point_t *in);

/**
 * @brief Compute the iterated double of the theta point in on the theta struc A
 *
 * @param out Output: the theta_point
 * @param A a theta structure
 * @param in a theta point in the theta structure A
 * @param exp the exponent
 * in = (x,y,z,t)
 * out = [2^2] (x,y,z,t)
 * /!\ assumes that no coordinates is zero and that the precomputation of A has been done
 *
 */
void theta_DBL_iter(theta_point_t *out, theta_structure_t *A, const theta_point_t *in, uint16_t exp);

/**
 * @brief Check if a theta point is a product theta point
 *
 * @param P a theta point
 * @return 0xFFFFFFFF if true, zero otherwise
 */
uint32_t theta_is_product_theta_point(const theta_point_t *P);

/**
 * @brief compute (fast) the dual theta null point from a theta structure.
 *
 * @param dual_null_point a theta point
 * @param A a theta structure. it assumes dbl_data was computed.
 */
void theta_extract_dual_null_point(theta_point_t *dual_null_point, theta_structure_t *A);

/** @}
 */

#endif
