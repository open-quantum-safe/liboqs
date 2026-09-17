#ifndef ISOG_H
#define ISOG_H
#include <sqisign_namespace.h>
#include <ec.h>

/**
 * @ingroup ec
 * @defgroup iso Isogenies
 * @{
 */

/** @brief data struct consisting of constant required to evaluate points through isogenies of degree 4
 *
 * @typedef ec_kps4_t
 *
 * @struct ec_kps4_t
 */
typedef struct ec_kps4_t
{
    fp2_t K[3];
} ec_kps4_t;

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

/**
 * @brief Evaluate isogeny of even degree on list of points.
 * Returns 0 if successful and -1 if kernel has the wrong order or includes (0:1).
 *
 * @param image computed image curve
 * @param phi isogeny, conissting in points
 *
 * @return 0 if there was no error, 0xFFFFFFFF otherwise
 */
uint32_t iso_isogeny_2chain(ec_curve_t *image, ec_isog_even_t *phi);

/**
 * @brief Evaluate a point through a 4 isogeny
 *
 * @param R image point
 * @param Q input point
 * @param lenQ number of evalaution
 * @param kps precomputed data to evaluate the isogeny
 *
 */
void iso_xeval_4(ec_point_t *R, const ec_point_t *Q, const int lenQ, const ec_kps4_t *kps);

/**
 * @brief compute a 4 isogeny chain of kernel P
 *
 * @param kps   precomputed data
 * @param B     codomain (A:C) coefficient
 * @param P     kernel of isogeny
 *
 */
void iso_xisog_4(ec_kps4_t *kps, ec_point_t *B, const ec_point_t P);

/**
 * @}
 */

#endif
