/** @file
 *
 * @brief The Lehmer Lagrange-Gauss kernel on a 2x2 Gram matrix, and the LDL block it consumes.
 */

#ifndef LLL_LG2_H
#define LLL_LG2_H

#include <stdint.h>
#include "mp.h"

/** @internal
 * @ingroup quat_helpers
 * @defgroup lll_lg2 The dimension-2 Gram kernel used by the dimension-4 driver
 * @{
 */

// Positive-definite binary form [[a,b],[b,c]], scaled by 2^-e
typedef struct
{
    ibz_t a, b, c;
    int32_t e;
} quat_lll_gram2_t;

/**
 * @brief Compute the local 2x2 Gram matrix from an LDL decomposition.
 *
 * (a, b, c) = (D_k, mu*D_k, D_{k+1} + mu^2*D_k), truncated to a common SECRET scale 2^-e
 * (returned in out->e), with e chosen so all three fit GRAM_TOT_BITS.
 *
 * @param out  Output: the form, and the scale e it was truncated to
 * @param Dk   D_k, bound FP_E_BITS
 * @param Dkp1 D_{k+1}, bound FP_E_BITS
 * @param mu   Fixed-point at scale 2^P, bound FP_L_BITS > P. Precondition: |mu| <= 1/2 at that
 *             scale, so a > 0, c > 0 and det = a*c - b^2 > 0.
 * @param P    The fixed-point scale of mu, PUBLIC
 */
void quat_lll_materialise_block(quat_lll_gram2_t *out, const ibz_t *Dk, const ibz_t *Dkp1, const ibz_t *mu, int P);

/**
 * @brief Lagrange-Gauss reduce a positive-definite binary form, returning the transform.
 *
 * @param g           In/out: the form, reduced in place. g->e is left untouched.
 * @param U           Output: the unimodular transform, bound FP_U_BITS. COLUMN convention --
 *                    b_k' = u00*b_k + u10*b_{k+1} and b_{k+1}' = u01*b_k + u11*b_{k+1}.
 * @param outer_iters PUBLIC trip count; a function of the round index alone, never of the data.
 *                    See lg_outer_its_for_round in lll_config.h.
 */
void quat_lll_gram_lehmer(quat_lll_gram2_t *g, ibz_t U[2][2], int outer_iters);

/** @}
 */

#endif /* LLL_LG2_H */
