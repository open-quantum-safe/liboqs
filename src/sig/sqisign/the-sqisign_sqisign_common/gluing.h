/** @file
 *
 * @authors Max Duparc
 *
 * @brief the gluing isogeny header
 */

#ifndef GLUING_ISOGENY_H
#define GLUING_ISOGENY_H

#include "theta_structure.h"

#include <sqisign_namespace.h>
#include <fp2.h>
#include <ec.h>
#include <hd.h>

/**
 * @ingroup hd_module
 * @defgroup hd_glue Gluing isogenies
 * @{
 */

/** @brief Type for a symetric element induced by a 4 torsion point
 * @typedef gluing_symmetric_element_t
 *
 * @struct gluing_symmetric_element
 *
 * Structure for a projective symetric element induced by a 4 torsion points. It consists in the element of the 2x2
 * matrix with index zero
 */
typedef struct gluing_symmetric_element
{
    fp2_t s00;
    fp2_t s01;
    fp2_t s10;

} gluing_symmetric_element_t;

/** @brief Type for a basis of symetric element induced by a 4 torsion basis
 * @typedef gluing_symmetric_element_basis_t
 *
 * @struct gluing_symmetric_element_basis
 *
 * Structure for a projective symmetric element induced by a 4 torsion basis. It consists in delta the determinant of
 * all symmetric elements and of 3 symmetric elements
 */
typedef struct gluing_symmetric_element_basis
{
    fp2_t delta;
    gluing_symmetric_element_t g1;
    gluing_symmetric_element_t g2;
    gluing_symmetric_element_t g3;
} gluing_symmetric_element_basis_t;

/** @brief A 4x4 matrix used for basis changes
 * @typedef gluing_change_coord_matrix_t
 *
 * @struct gluing_change_coord_matrix
 *
 * Structure to hold 16 elements representing a 4x4 matrix used for changing the basis of a theta point.
 */
typedef struct gluing_change_coord_matrix
{
    fp2_t m[4][4];
} gluing_change_coord_matrix_t;

/** @brief Type for gluing (2,2) theta isogeny *
 * @typedef theta_gluing_t
 *
 * @struct theta_gluing
 *
 * the theta_gluing structure
 */
typedef struct theta_gluing
{
    theta_point_t inv_imageK1_8;
    theta_point_t inv_imageK2_8;
    gluing_change_coord_matrix_t M;
    theta_point_t inv_dual_theta_null;
    theta_point_t codomain;

} theta_gluing_t;

/**
 * @brief Compute the gluing first isogeny of the HD chain, returns evalutation of the gluing points
 *        together with
 *
 * @param first_step    Output: the gluing isogeny
 * @param thetaQ1       Output : an array of the first points in the chain evaluated through the gluing isogeny
 * @param thetaQ2       Output : an array of the second points in the chain evaluated through the gluing isogeny
 * @param pts           Output : an array of the additional points evaluated throught the isogeny chain
 * @param todo          Output : an array of the todo vectors computed during the strategy
 * @param space         Input : the limit space for the array in the strategy
 * @param numP          Input : the number of additional points to be evaluated
 * @param E12           Input : the couple curve domain of the 2D isogeny chain
 * @param P12           Input : the additional points to be avaualted
 * @param ker           Input : the kernel of the 2D chain
 * @param mode          Input : the mode the chain is computed in
 * @return              a index indicating where we are in the strategy
 **/
int gluing_start_chain(theta_gluing_t *first_step,
                       theta_point_t *thetaQ1,
                       theta_point_t *thetaQ2,
                       theta_point_t *pts,
                       uint16_t *todo,
                       const uint8_t space,
                       const uint8_t numP,
                       const theta_couple_curve_t *E12,
                       const theta_couple_point_t *P12,
                       const theta_kernel_couple_points_t *ker,
                       const int8_t mode);

/** @}
 */

#endif
