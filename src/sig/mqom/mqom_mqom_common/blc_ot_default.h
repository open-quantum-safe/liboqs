#ifndef __BLC_OT_DEFAULT_H__
#define __BLC_OT_DEFAULT_H__

/* MQOM3 parameters */
#include "mqom3_parameters.h"

/* OT-only header. Until MQOM3_PARAM_OPENING_SIZE became the family-agnostic
 * alias (common.h), reaching this file from a CT build failed loudly on the
 * undefined MQOM3_PARAM_OT_OPENING_SIZE. That accident was the only thing
 * catching a stray include; state the requirement instead. */
#if MQOM3_PARAM_OT_VARIANT != 1
#error "OT-only header reached from a non-OT build"
#endif
#include "enc.h"
#include "prg.h"
#include "xof.h"
#include "fields.h"
#include "ggm_tree_large.h"

/* Deal with namespacing */
#define OT_BLC_Commit_default MQOM_NAMESPACE(OT_BLC_Commit_default)
#define OT_BLC_Open_default MQOM_NAMESPACE(OT_BLC_Open_default)
#define OT_BLC_Eval_default MQOM_NAMESPACE(OT_BLC_Eval_default)
#define OT_BLC_PrintConfig_default MQOM_NAMESPACE(OT_BLC_PrintConfig_default)

#if !defined(MEMORY_EFFICIENT_BLC) && !defined(BLC_DEFAULT_NO_KEEP_TREES)
#define BLC_KEEP_ALL_TREES_IN_MEMORY
#endif

typedef struct OT_blc_key_default_t {
#ifndef BLC_KEEP_ALL_TREES_IN_MEMORY
	/* Memory-efficient: re-derive tree in BLC_Open via LargeGGMTree_ExpandPath */
	uint8_t mseed[MQOM3_PARAM_SEED_SIZE];
	uint8_t salt[MQOM3_PARAM_SALT_SIZE];
#else
	/* Fast path: store full tree + precomputed leaf commitments */
	uint8_t node[LARGE_GGM_TREE_SIZE][MQOM3_PARAM_SEED_SIZE];
	uint8_t ls_com[MQOM3_PARAM_TAU][MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_DIGEST_SIZE];
#endif
	/* Full delta_x (no FirstBits trick in OT) */
	uint8_t delta_x[MQOM3_PARAM_TAU][BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)];
} OT_blc_key_default_t;

int OT_BLC_Commit_default(const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], OT_blc_key_default_t* key, field_ext_elt x0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt u1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]);

int OT_BLC_Open_default(const OT_blc_key_default_t* key, const uint16_t i_star[MQOM3_PARAM_TAU], const field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], uint8_t opening[MQOM3_PARAM_OPENING_SIZE]);

int OT_BLC_Eval_default(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t opening[MQOM3_PARAM_OPENING_SIZE], const uint16_t i_star[MQOM3_PARAM_TAU], uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], field_ext_elt x_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]);

void OT_BLC_PrintConfig_default(void);

#endif /* __BLC_OT_DEFAULT_H__ */
