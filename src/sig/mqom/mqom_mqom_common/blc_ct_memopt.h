#ifndef __BLC_CT_MEMOPT_H__
#define __BLC_CT_MEMOPT_H__

/* MQOM3 parameters */
#include "mqom3_parameters.h"

/* CT-only header: see the symmetric note in blc_ot_default.h. */
#if MQOM3_PARAM_OT_VARIANT == 1
#error "CT-only header reached from an OT build"
#endif
#include "enc.h"
#include "prg.h"
#include "xof.h"
#include "fields.h"


#include "blc_common.h"

/* Deal with namespacing */
#define CT_BLC_Commit_memopt MQOM_NAMESPACE(CT_BLC_Commit_memopt)
#define CT_BLC_Commit_memopt_partial MQOM_NAMESPACE(CT_BLC_Commit_memopt_partial)
#define CT_BLC_Open_memopt MQOM_NAMESPACE(CT_BLC_Open_memopt)
#define CT_BLC_Eval_memopt MQOM_NAMESPACE(CT_BLC_Eval_memopt)
#define CT_BLC_Eval_memopt_partial MQOM_NAMESPACE(CT_BLC_Eval_memopt_partial)
#define CT_BLC_PrintConfig_memopt MQOM_NAMESPACE(CT_BLC_PrintConfig_memopt)

typedef struct CT_blc_key_memopt_t {
	uint8_t salt[MQOM3_PARAM_SALT_SIZE];
	uint8_t delta[MQOM3_PARAM_SEED_SIZE];
	uint8_t mseed[MQOM3_PARAM_SEED_SIZE];
	uint8_t partial_delta_x[MQOM3_PARAM_TAU][BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) - MQOM3_PARAM_SEED_SIZE];
} CT_blc_key_memopt_t;

int CT_BLC_Commit_memopt(const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], CT_blc_key_memopt_t* key, field_ext_elt x0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt u1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]);

int CT_BLC_Commit_memopt_partial(uint32_t e, const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], const uint8_t delta[MQOM3_PARAM_SEED_SIZE], uint8_t com[MQOM3_PARAM_DIGEST_SIZE], uint8_t partial_delta_x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)-MQOM3_PARAM_SEED_SIZE], field_ext_elt x0[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u0[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt u1[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]);

int CT_BLC_Open_memopt(const CT_blc_key_memopt_t* key, const uint16_t i_star[MQOM3_PARAM_TAU], const field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], uint8_t opening[MQOM3_PARAM_OPENING_SIZE]);

int CT_BLC_Eval_memopt(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t opening[MQOM3_PARAM_OPENING_SIZE], const uint16_t i_star[MQOM3_PARAM_TAU], uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], field_ext_elt x_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]);

int CT_BLC_Eval_memopt_partial(uint32_t e, const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t path[MQOM3_PARAM_SEED_SIZE*MQOM3_PARAM_NB_EVALS_LOG], const uint8_t out_ls_com[MQOM3_PARAM_DIGEST_SIZE], const uint8_t partial_delta_x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)-MQOM3_PARAM_SEED_SIZE], uint16_t i_star, uint8_t com[MQOM3_PARAM_DIGEST_SIZE], field_ext_elt x_eval[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u_eval[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]);

void CT_BLC_PrintConfig_memopt(void);

#endif /* __BLC_CT_MEMOPT_H__ */
