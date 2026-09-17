/** @file
 *
 * @authors Damien Robert, Max Duparc
 *
 * @brief the splitting isogeny header
 */

#ifndef SPLITTING_ISOGENY_H
#define SPLITTING_ISOGENY_H

#include <sqisign_namespace.h>

#include <fp2.h>
#include <ec.h>
#include <hd.h>
#include <assert.h>

#include "theta_structure.h"

/**
 * @ingroup hd_module
 * @defgroup hd_split Splitting isogenies
 * @{
 */

/**
 * @brief Convert the codomain to a product of Montgomery elliptic curves
 * @param E12 The two curves
 * @param P12 The image of the points on both curves
 * @param dual_null_point The codomain null point in dual form.
 * @param pts The dim 2 theta points to convert
 * @param numP The number of points
 * @param index the mode we use the splitting in:
 *                 0: compute both E1 and E2, together with the associated points.
 *                 1: compute only E1, together with the associated points.
 *                 2: compute only E2, together with the associated points.
 *                -1: Same as 1, but also checking validity.
 *
 */
bool splitting_to_elliptic_product(theta_couple_curve_t *E12,
                                   theta_couple_point_t *P12,
                                   theta_point_t *dual_null_point,
                                   theta_point_t *pts,
                                   size_t numP,
                                   int index);

/**
 * @}
 */

#endif
