/** @file
 *
 * @brief The lattice-reduction (lll) module's outward-facing surface.
 *
 */

#ifndef QUAT_LLL_H
#define QUAT_LLL_H

#include <quaternion.h>

/** @internal
 * @ingroup quat_helpers
 * @defgroup lll_interface The lattice-reduction module's public interface
 * @{
 */

/**
 * @brief Constant-time Lagrange-Gauss reduction of a rank-2 lattice over Z.
 *
 * @param reduced Output: the reduced basis
 * @param basis Basis of a rank 2 lattice in dimension 2
 * @param det_bits PUBLIC bound on det(basis), and hence on every entry: det <= 2^det_bits
 */
void quat_lll_dim2_short_basis(ibz_mat_2x2_t *reduced, const ibz_mat_2x2_t *basis, int det_bits);

/**
 * @brief Write a prime as a sum of two squares, in constant time.
 *
 * Given a prime p and 0 < r < p with r^2 == -1 mod p, output 0 <= x, y < p with x^2 + y^2 == p.
 *
 * @param x Output: 0 <= x < p
 * @param y Output: 0 <= y < p
 * @param p Prime to decompose
 * @param r Square root of -1 modulo p
 */
void quat_lll_dim2_sumofsquares(ibz_t *x, ibz_t *y, const ibz_t *p, const ibz_t *r);

/** @brief Compute a reduced basis of an O0-ideal given in inert HNF form.
 *
 * @param reduced Output: 2^(-32)-Minkowski-reduced basis over Z (or Z[i] as rank-2).
 *                If (1+2^-32) * lambda1 <= lambda3 and lambda3/lambda1 <= 2^26, then the output
 *                is exactly Minkowski-reduced.
 *                Normalized such that as a rank-2 Z[i] lattice v1,v2 we have Re, Im >= 0 for
 *                v1[0] and <v1,v2> (hermitian inner product).
 * @param hnf     Lattice of an O0-ideal in inert HNF form with denominator 2
 * @param alg     Quaternion algebra defining the norm, must equal O0.
 * @param bound   PUBLIC bound in bits on the input coefficients.
 */
void quat_lll_reduce_O0_ideal(quat_lattice_t *reduced, const quat_lattice_t *hnf, const quat_alg_t *alg, int bound);

/** @brief Compute a dual-Minkowski-reduced basis of a general ideal given in inert HNF shape.
 *
 * @param reduced   Output: basis such that the corresponding reverse dual basis is Minkowski-reduced
 *                  with exception of 2^-SECURITY_BITS probability. Output is canonical w.r.t. the
 *                  ideal class except for a global +-1 sign.
 * @param gram_diag Output: approximate DUAL Gram diagonal of that basis, in the same reversed order,
 *                  scaled by twoN^2 * p with twoN = hnf->basis[0][0]. Relative error is below
 *                  2^-SECURITY_BITS.
 * @param Ainv      Output: exact integer transform with reduced->basis == hnf->basis * Ainv and
 *                  det(Ainv) = +-1.
 * @param hnf       Ideal lattice in the inert triangular quat_lattice_t shape. Off-diagonal entries
 *                  do not have to be reduced mod 2N. Not to be used on O0-ideals.
 * @param alg       The quaternion algebra defining the norm (supplies p).
 */
void quat_lll_dual_reduce_ideal(quat_lattice_t *reduced,
                                ibz_t gram_diag[4],
                                ibz_mat_4x4_t *Ainv,
                                const quat_lattice_t *hnf,
                                const quat_alg_t *alg);

/** @}
 */

// QUAT_LLL_H
#endif
