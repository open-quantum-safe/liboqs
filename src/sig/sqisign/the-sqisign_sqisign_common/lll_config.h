/** @file
 *
 * @brief Per-level integer and precision bounds for the constant-time lattice reduction.
 */

#ifndef LLL_CONFIG_H
#define LLL_CONFIG_H

#include "mp.h"
#include <encoded_sizes.h>
#include <quaternion_constants.h> // QUAT_P_HEX, QUAT_P_BITS, QUAT_SQRT_P_HEX, QUAT_SQRT_P_BITS

// --- per security level constants
// QUAT_P_HEX, QUAT_SQRT_P_HEX, QUAT_P_BITS and QUAT_SQRT_P_BITS are generated per level into
// <quaternion_constants.h> by scripts/precomp/precompute_quaternion_constants.sage
#if !defined(QUAT_P_BITS) || !defined(QUAT_SQRT_P_BITS) || !defined(QUAT_P_HEX) || !defined(QUAT_SQRT_P_HEX)
#error "quaternion_constants.h did not supply the per-level prime constants -- re-run the precomp target."
#endif

// --- Problem shape

// clang-format off
#define FP_LAMBDA       SECURITY_BITS

#ifndef FP_PRIME
#define FP_PRIME        QUAT_P_BITS
#endif

#ifndef FP_NORM
// Bound on double ideal norm 2N+1. Validate further later.
#define FP_NORM         (FP_PRIME + FP_LAMBDA)
#endif
// DERIVED: public bound, in bits, on the D-profile gap.
// The initial profile is ((2N)^2, (2N)^2, p, p) bits.
#define FP_GAP_PUB      (2 * FP_NORM - FP_PRIME)

// --- Fixed-point scale

// Bound on total U transform. Expectation + FP_LAMBDA/2 tail-bound.
#define FP_U_BITS       LIMB_ALIGN(FP_GAP_PUB/4+FP_LAMBDA/2)

// Update procedure creates relative error of roughly 2*u_bits - FP_P_BITS <= - FP_LAMBDA. Use u_bits <= FP_U_BITS.
#define FP_P_BITS       LIMB_ALIGN(2*FP_U_BITS + FP_LAMBDA)

// DERIVED: headroom for L's growth under wide U
#define FP_HEAD         (FP_U_BITS)

// DERIVED: total bitsize of L, mu, eta
#define FP_L_BITS       LIMB_ALIGN(FP_P_BITS + FP_HEAD)

// Profile is [(2N)^2, (2N)^2, p, p]; can scale down by 2^FP_D_SHIFT as long as
// FP_PRIME - FP_D_SHIFT >= X to keep +- X precision. Take X = 3/2 * FP_LAMBDA for extra margin.
#define FP_D_SHIFT      (FP_PRIME - FP_LAMBDA - FP_LAMBDA/2)

// DERIVED: bound on the D entries after the downscale. (2N)^2 needs ~2*FP_NORM bits raw, LIMB_ALIGN rounds that
// to a limb multiple, and fp_ldl_init_dual shifts every entry right by the public FP_D_SHIFT.
// Checked by the "D must hold (2N)^2" assert below.
#define FP_E_BITS       (LIMB_ALIGN(2 * FP_NORM) - FP_D_SHIFT)

// --- Per-block LG transform entries

#define BLOCK_U_BITS    LIMB_ALIGN(FP_GAP_PUB / 4 + FP_LAMBDA / 2)

#ifndef LG_OUTER_ITS
#define LG_OUTER_ITS     (BLOCK_U_BITS/8)
#endif

// Outer passes for block-reduction round `round`: halve every LG_OUTER_HALVE_ROUNDS rounds from
// round 1, floor at LG_OUTER_MIN. LG_OUTER_ITS still sets the width, so this tracks the shape.
// PUBLIC: a function of the round index alone, never of the data.
#ifndef LG_OUTER_MIN
#define LG_OUTER_MIN           4
#endif
#ifndef LG_OUTER_HALVE_ROUNDS
#define LG_OUTER_HALVE_ROUNDS  3
#endif

static inline int
lg_outer_its_for_round(int round)
{
    int shift = (round > 1 ? round - 1 : 0) / LG_OUTER_HALVE_ROUNDS;
    int its = (LG_OUTER_ITS) >> (shift > 24 ? 24 : shift);
    return its > LG_OUTER_MIN ? its : LG_OUTER_MIN;
}

// --- The truncated 2x2 block Gram the LG kernel operates on

#define GRAM_TOT_BITS   LIMB_ALIGN(MAXB(FP_GAP_PUB + FP_LAMBDA, 0))

#define GRAM_WORK_BITS  (GRAM_TOT_BITS+32) // DERIVED: bound for internal 32-bit Gram updates.

// --- Newton reciprocal

// DERIVED: Newton-Raphson rounds in ibz_ct_fp_recip. Correct bits roughly double per round from
// the 61-bit seed: 61 -> 120 -> 238 -> 474 -> 946 -> ... 4 rounds suffice for LVL1 and probably
// LVL3; LVL5 needs 5, so all three levels use 5. The "FP_NR_ROUNDS too small" assert below is
// what actually guarantees it.
#define FP_NR_ROUNDS    5

// --- LLL

#ifndef CT_LLL_TOURS
#define CT_LLL_TOURS       (LOG2(FP_GAP_PUB)+4) // Heuristic
#endif

// --- Public ideal-level entry point (quat_lll_dual_reduce_ideal)

#define QUAT_HNF_BITS   (FP_NORM + 2)
#define QUAT_AINV_BITS  (FP_U_BITS + 32)
#define QUAT_AINVOUT_BITS (QUAT_AINV_BITS)
#define QUAT_BASIS_BITS LIMB_ALIGN(QUAT_HNF_BITS + QUAT_AINVOUT_BITS + 2)
#define QUAT_GRAM_BITS  (FP_E_BITS + 40)
#define QUAT_GRAM_SHIFT (FP_D_SHIFT)
#define QUAT_GRAM_OUT_BITS (QUAT_GRAM_BITS + QUAT_GRAM_SHIFT)

// clang-format on

// --- Static asserts

// ibz_ct_fp_recip's seed is accurate to ~61 bits and each round roughly doubles that:
// ((61 - 2) << FP_NR_ROUNDS) + 2 >= FP_P_BITS + 4.
_Static_assert((((int64_t)61 - 2) << FP_NR_ROUNDS) + 2 >= FP_P_BITS + 4, "FP_NR_ROUNDS too small");

_Static_assert(GRAM_WORK_BITS <= IBZ_MAX_BITS, "bsum must fit a single ibz_t");
_Static_assert(FP_GAP_PUB > 0, "(2N)^2 must exceed p");
_Static_assert(FP_L_BITS >= FP_P_BITS + FP_HEAD, "FP_L_BITS under-sized");
_Static_assert(FP_E_BITS + FP_D_SHIFT >= 2 * FP_NORM, "D must hold (2N)^2");
_Static_assert(GRAM_TOT_BITS >= FP_GAP_PUB + FP_LAMBDA, "GRAM_TOT_BITS: precision");
_Static_assert(BLOCK_U_BITS > FP_GAP_PUB / 4 + 1, "BLOCK_U_BITS below the proven worst-case |U|");

// The two IBZ_MAX_BITS bounds below are sufficient, not tight.
_Static_assert(IBZ_MAX_BITS >= FP_E_BITS + BLOCK_U_BITS,
               "IBZ_MAX_BITS must cover fp_update_block's widest plain-ibz_mul operand pair");
_Static_assert(IBZ_MAX_BITS >= FP_L_BITS + BLOCK_U_BITS,
               "IBZ_MAX_BITS must cover fp_update_block's L-domain multiplies too");

_Static_assert(IBZ_MAX_BITS >= QUAT_HNF_BITS + QUAT_AINVOUT_BITS, "IBZ_MAX_BITS must cover hnf->basis * Ainv_out");
_Static_assert(IBZ_MAX_BITS >= QUAT_BASIS_BITS, "QUAT_BASIS_BITS must fit one ibz_t");
_Static_assert(IBZ_MAX_BITS >= QUAT_GRAM_BITS, "QUAT_GRAM_BITS must fit one ibz_t");
_Static_assert(IBZ_MAX_BITS >= QUAT_GRAM_OUT_BITS, "QUAT_GRAM_OUT_BITS must fit one ibz_t");

// ==== dimension-2 reductions

// clang-format off

// --- dimension 2 over Z

#define DIM2_THRESHOLD 30

// DERIVED: bits extracted per window, 2*THRESHOLD + 3.
#define DIM2_W (2 * DIM2_THRESHOLD + 3)

// Heuristic but wide bound on number of outer iterations for now
#define DIM2_OUTER_ITS(in) ((in) / (DIM2_THRESHOLD - 1) + 1)

// --- bitlens
#define DIM2_VAL_BITS(in) ((in) + 1)
#define DIM2_WORK_BITS(in) (DIM2_VAL_BITS(in) + 32)
#define DIM2_SHIFT_MAX(in) MAXB(DIM2_VAL_BITS(in) - DIM2_THRESHOLD, 0)

// --- dimension 2 over Z[i]

#define DIM2I_THRESHOLD 27
#define DIM2I_W (2 * DIM2I_THRESHOLD + 5)
#define DIM2I_UP_SHIFT 3

#define DIM2I_REQ_DROP(in) ((MAXB((in) - QUAT_SQRT_P_BITS, 0) + 1) / 2 + 1)
#ifndef DIM2I_OUTER_ITS
#define DIM2I_OUTER_ITS(in) ((DIM2I_REQ_DROP(in) + DIM2I_THRESHOLD - 2) / (DIM2I_THRESHOLD - 1) + 4)
#endif

#define DIM2I_INNER_ITS (DIM2I_THRESHOLD + 1)
#define DIM2I_SHIFT_MAX(in) MAXB(DIM2_VAL_BITS(in) - DIM2I_THRESHOLD, 0)

// DERIVED: width of every intermediate of ct_zi_hermitian. The p-weighted term dominates:
// two DIM2_VAL_BITS operands, times p, plus carries.
#define DIM2I_HERM_BITS(in) (2 * DIM2_VAL_BITS(in) + QUAT_P_BITS + 3)

// clang-format on

_Static_assert(DIM2_W <= 63, "the Z window must fit a signed 64-bit word");
_Static_assert(DIM2I_W <= 63, "the Z[i] window must fit a signed 64-bit word");
_Static_assert(DIM2I_OUTER_ITS(QUAT_SQRT_P_BITS + 1) >= 1, "DIM2I_OUTER_ITS must be positive");

#endif /* LLL_CONFIG_H */
