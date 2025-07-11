/** @file
 *
 * @authors Sina Schaeffler
 *
 * @brief Declarations for functions internal to the HNF computation and its tests
 */

#ifndef QUAT_HNF_HELPERS_H
#define QUAT_HNF_HELPERS_H

#include <quaternion.h>

/** @internal
 * @ingroup quat_helpers
 * @defgroup quat_hnf_helpers Internal functions for the HNF computation and tests
 */

/** @internal
 * @ingroup quat_hnf_helpers
 * @defgroup quat_hnf_helpers_ibz Internal renamed GMP functions for the HNF computation
 */

/**
 * @brief GCD and Bézout coefficients u, v such that ua + bv = gcd
 *
 * @param gcd Output: Set to the gcd of a and b
 * @param u Output: integer such that ua+bv=gcd
 * @param v Output: Integer such that ua+bv=gcd
 * @param a
 * @param b
 */
void ibz_xgcd(ibz_t *gcd,
              ibz_t *u,
              ibz_t *v,
              const ibz_t *a,
              const ibz_t *b); // integers, dim4, test/integers, test/dim4

/** @}
 */

/** @internal
 * @ingroup quat_hnf_helpers
 * @defgroup quat_hnf_integer_helpers Integer functions internal to the HNF computation and tests
 * @{
 */

/** @brief x mod mod, with x in [1,mod]
 *
 * @param res Output: res = x [mod] and 0<res<mod+1
 * @param x
 * @param mod integer > 0
 */
void ibz_mod_not_zero(ibz_t *res, const ibz_t *x, const ibz_t *mod);

/** @brief x mod mod, with x in ]-mod/2,mod/2]
 *
 * Centered and rather positive then negative.
 *
 * @param remainder Output: remainder = x [mod] and -mod/2<remainder<=mod/2
 * @param a
 * @param mod integer > 0
 */
void ibz_centered_mod(ibz_t *remainder, const ibz_t *a, const ibz_t *mod);

/** @brief if c then x else y
 *
 * @param res Output: if c, res = x, else res = y
 * @param x
 * @param y
 * @param c condition: must be 0 or 1
 */
void ibz_conditional_assign(ibz_t *res, const ibz_t *x, const ibz_t *y, int c);

/** @brief d = gcd(x,y)>0 and d = ux+vy and u!= 0 and d>0 and u, v of small absolute value, u not 0
 *
 * More precisely:
 * If x and y are both non 0, -|xy|/d<vy<=0<ux<=|xy|/d.
 * If x=0 and y not 0, then u=1 and v=y/d
 * If x not 0 and y=0, then u=x/d and v=0
 * If y=0 and x=0, d=1, u=1 and v=0
 *
 * @param d Output: gcd of x and y
 * @param u Output: coefficient of x
 * @param v Output: coefficient of y
 * @param x
 * @param y
 */
void ibz_xgcd_with_u_not_0(ibz_t *d, ibz_t *u, ibz_t *v, const ibz_t *x, const ibz_t *y);

/** @}
 */
/** @}
 */
#endif