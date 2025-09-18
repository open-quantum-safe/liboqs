#ifndef ENDOMORPHISM_ACTION_H
#define ENDOMORPHISM_ACTION_H
#include <sqisign_namespace.h>
#include <ec.h>
#include <quaternion.h>
/** Type for precomputed endomorphism rings applied to precomputed torsion bases.
 *
 * Precomputed by the precompute scripts.
 *
 * @typedef curve_with_endomorphism_ring_t
 *
 * @struct curve_with_endomorphism_ring
 **/
typedef struct curve_with_endomorphism_ring {
    ec_curve_t curve;
    ec_basis_t basis_even;
    ibz_mat_2x2_t action_i, action_j, action_k;
    ibz_mat_2x2_t action_gen2, action_gen3, action_gen4;
} curve_with_endomorphism_ring_t;
#define CURVE_E0 (CURVES_WITH_ENDOMORPHISMS->curve)
#define BASIS_EVEN (CURVES_WITH_ENDOMORPHISMS->basis_even)
#define ACTION_I (CURVES_WITH_ENDOMORPHISMS->action_i)
#define ACTION_J (CURVES_WITH_ENDOMORPHISMS->action_j)
#define ACTION_K (CURVES_WITH_ENDOMORPHISMS->action_k)
#define ACTION_GEN2 (CURVES_WITH_ENDOMORPHISMS->action_gen2)
#define ACTION_GEN3 (CURVES_WITH_ENDOMORPHISMS->action_gen3)
#define ACTION_GEN4 (CURVES_WITH_ENDOMORPHISMS->action_gen4)
#define NUM_ALTERNATE_STARTING_CURVES 7
#define ALTERNATE_STARTING_CURVES (CURVES_WITH_ENDOMORPHISMS+1)
extern const curve_with_endomorphism_ring_t CURVES_WITH_ENDOMORPHISMS[8];
#endif
