#ifndef __BLC_CT_DEFAULT_H__
#define __BLC_CT_DEFAULT_H__

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
#include "blc_ct_common.h"

/* Deal with namespacing */
#define CT_BLC_Commit_default MQOM_NAMESPACE(CT_BLC_Commit_default)
#define CT_BLC_Open_default MQOM_NAMESPACE(CT_BLC_Open_default)
#define CT_BLC_Eval_default MQOM_NAMESPACE(CT_BLC_Eval_default)
#define CT_BLC_PrintConfig_default MQOM_NAMESPACE(CT_BLC_PrintConfig_default)

#if !defined(MEMORY_EFFICIENT_BLC) && !defined(BLC_DEFAULT_NO_KEEP_TREES)
#define BLC_KEEP_ALL_TREES_IN_MEMORY
#endif

typedef struct CT_blc_key_default_t {
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	uint8_t salt[MQOM3_PARAM_SALT_SIZE];
	uint8_t delta[MQOM3_PARAM_SEED_SIZE];
	uint8_t mseed[MQOM3_PARAM_SEED_SIZE];
#else
	uint8_t node[MQOM3_PARAM_TAU][MQOM3_PARAM_FULL_TREE_SIZE + 1][MQOM3_PARAM_SEED_SIZE];
	uint8_t ls_com[MQOM3_PARAM_TAU][MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_DIGEST_SIZE];
#endif
	uint8_t partial_delta_x[MQOM3_PARAM_TAU][BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) - MQOM3_PARAM_SEED_SIZE];
} CT_blc_key_default_t;

int CT_BLC_Commit_default(const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], CT_blc_key_default_t* key, field_ext_elt x0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt u1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]);

int CT_BLC_Open_default(const CT_blc_key_default_t* key, const uint16_t i_star[MQOM3_PARAM_TAU], const field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], uint8_t opening[MQOM3_PARAM_OPENING_SIZE]);

int CT_BLC_Eval_default(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t opening[MQOM3_PARAM_OPENING_SIZE], const uint16_t i_star[MQOM3_PARAM_TAU], uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], field_ext_elt x_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]);

void CT_BLC_PrintConfig_default(void);

#endif /* __BLC_CT_DEFAULT_H__ */
