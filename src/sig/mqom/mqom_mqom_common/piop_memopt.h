#ifndef __PIOP_MEMOPT_H__
#define __PIOP_MEMOPT_H__

#include "mqom2_parameters.h"
#include "fields.h"

/* Deal with namespacing */
#define ComputePAlpha_partial_memopt MQOM_NAMESPACE(ComputePAlpha_partial_memopt)
#define RecomputePAlpha_partial_memopt MQOM_NAMESPACE(RecomputePAlpha_partial_memopt)

#ifndef PIOP_NB_PARALLEL_REPETITIONS_SIGN
#define PIOP_NB_PARALLEL_REPETITIONS_SIGN MQOM2_PARAM_TAU
#endif
#ifndef PIOP_NB_PARALLEL_REPETITIONS_VERIFY
#define PIOP_NB_PARALLEL_REPETITIONS_VERIFY MQOM2_PARAM_TAU
#endif

#if (PIOP_NB_PARALLEL_REPETITIONS_SIGN == 0) || (PIOP_NB_PARALLEL_REPETITIONS_SIGN > MQOM2_PARAM_TAU)
#error "PIOP_NB_PARALLEL_REPETITIONS_SIGN must be > 0 and <= MQOM2_PARAM_TAU."
#endif
#if (PIOP_NB_PARALLEL_REPETITIONS_VERIFY == 0) || (PIOP_NB_PARALLEL_REPETITIONS_VERIFY > MQOM2_PARAM_TAU)
#error "PIOP_NB_PARALLEL_REPETITIONS_VERIFY must be > 0 and <= MQOM2_PARAM_TAU."
#endif

int ComputePAlpha_memopt(const uint8_t com[MQOM2_PARAM_DIGEST_SIZE], const field_ext_elt x0[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], const field_ext_elt u0[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], const field_ext_elt u1[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], const field_base_elt x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)], const uint8_t mseed_eq[2 * MQOM2_PARAM_SEED_SIZE], field_ext_elt alpha0[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], field_ext_elt alpha1[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]);

int RecomputePAlpha_memopt(const uint8_t com[MQOM2_PARAM_DIGEST_SIZE], const field_ext_elt alpha1[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], const uint16_t i_star[MQOM2_PARAM_TAU], const field_ext_elt x_eval[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], const field_ext_elt u_eval[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], const uint8_t mseed_eq[2 * MQOM2_PARAM_SEED_SIZE], const field_ext_elt y[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU)], field_ext_elt alpha0[MQOM2_PARAM_TAU][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]);

//-------

int ComputePAlpha_partial_memopt(uint32_t nb_reps, const uint8_t com[MQOM2_PARAM_DIGEST_SIZE], const field_ext_elt x0[][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], const field_ext_elt u0[][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], const field_ext_elt u1[][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], const field_base_elt x[FIELD_BASE_PACKING(MQOM2_PARAM_MQ_N)], const uint8_t mseed_eq[2 * MQOM2_PARAM_SEED_SIZE], field_ext_elt alpha0[][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], field_ext_elt alpha1[][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]);

int RecomputePAlpha_partial_memopt(uint32_t nb_reps, const uint8_t com[MQOM2_PARAM_DIGEST_SIZE], const field_ext_elt alpha1[][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], const uint16_t i_star[], const field_ext_elt x_eval[][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], const field_ext_elt u_eval[][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)], const uint8_t mseed_eq[2 * MQOM2_PARAM_SEED_SIZE], const field_ext_elt y[FIELD_EXT_PACKING(MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU)], field_ext_elt alpha0[][FIELD_EXT_PACKING(MQOM2_PARAM_ETA)]);

#endif /* __PIOP_MEMOPT_H__ */
