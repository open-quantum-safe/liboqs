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

/** @internal
 * @ingroup hd_module
 * @defgroup hd_theta Functions for theta structures
 * @{
 */

/**
 * @brief Perform the hadamard transform on a theta point
 *
 * @param out Output: the theta_point
 * @param in a theta point*
 * in = (x,y,z,t)
 * out = (x+y+z+t, x-y+z-t, x+y-z-t, x-y-z+t)
 *
 */
static inline void
hadamard(theta_point_t *out, const theta_point_t *in)
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
 * @brief Square the coordinates and then perform the hadamard transform
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
    hadamard(out, out);
}

/**
 * @brief Perform the theta structure precomputation
 *
 * @param A Output: the theta_structure
 *
 * if A.null_point = (x,y,z,t)
 * if (xx,yy,zz,tt) = to_squared_theta(A.null_point)
 * Computes y0,z0,t0,Y0,Z0,T0 = x/y,x/z,x/t,XX/YY,XX/ZZ,XX/TT
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
void double_point(theta_point_t *out, theta_structure_t *A, const theta_point_t *in);

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
void double_iter(theta_point_t *out, theta_structure_t *A, const theta_point_t *in, int exp);

/*
 * @brief Check if a theta point is a product theta point
 *
 * @param P a theta point
 * @return 0xFFFFFFFF if true, zero otherwise
 */
uint32_t is_product_theta_point(const theta_point_t *P);

// end hd_theta
/**
 * @}
 */

#endif
