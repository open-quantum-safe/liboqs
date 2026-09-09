#ifndef __BLC_OT_MEMOPT_H__
#define __BLC_OT_MEMOPT_H__

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

/* BLC_OT_BATCH_SIZE: number of executions processed per large-tree traversal pass.
 * Tradeoff: 1 = min memory (O(H*SEED) stack), max time (TAU tree passes).
 *           TAU = max memory (TAU parallel contexts), min time (1 tree pass). */
#ifndef BLC_OT_BATCH_SIZE
#define BLC_OT_BATCH_SIZE MQOM3_PARAM_TAU
#endif
#if BLC_OT_BATCH_SIZE < 1
#error "BLC_OT_BATCH_SIZE must be >= 1"
#endif
#if BLC_OT_BATCH_SIZE > MQOM3_PARAM_TAU
#error "BLC_OT_BATCH_SIZE must be <= MQOM3_PARAM_TAU"
#endif

/* Deal with namespacing */
#define OT_BLC_Commit_memopt MQOM_NAMESPACE(OT_BLC_Commit_memopt)
#define OT_BLC_Commit_memopt_partial MQOM_NAMESPACE(OT_BLC_Commit_memopt_partial)
#define OT_BLC_Open_memopt MQOM_NAMESPACE(OT_BLC_Open_memopt)
#define OT_BLC_Eval_memopt MQOM_NAMESPACE(OT_BLC_Eval_memopt)
#define OT_BLC_Eval_memopt_partial MQOM_NAMESPACE(OT_BLC_Eval_memopt_partial)
#define OT_BLC_PrintConfig_memopt MQOM_NAMESPACE(OT_BLC_PrintConfig_memopt)

typedef struct OT_blc_key_memopt_t {
	/* Re-derive tree in BLC_Open via LargeGGMTree_ExpandPath(salt, mseed, ...) */
	uint8_t mseed[MQOM3_PARAM_SEED_SIZE];
	uint8_t salt[MQOM3_PARAM_SALT_SIZE];
	/* Full delta_x (no FirstBits trick in OT) */
	uint8_t delta_x[MQOM3_PARAM_TAU][BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)];
} OT_blc_key_memopt_t;

int OT_BLC_Commit_memopt(const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], OT_blc_key_memopt_t* key, field_ext_elt x0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt u1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]);

/* Commit a single batch [e_start, e_start+batch) via one large-tree traversal pass.
 * batch must be <= BLC_OT_BATCH_SIZE (bounds the per-batch scratch arrays). Exposed
 * so Sign/Verify memopt (and streaming verify) can drive the tree-pass granularity
 * directly, the same way piop_memopt.h exposes RecomputePAlpha_partial_memopt. */
int OT_BLC_Commit_memopt_partial(uint32_t e_start, uint32_t batch, const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], uint8_t com1[][MQOM3_PARAM_DIGEST_SIZE], OT_blc_key_memopt_t* key, field_ext_elt x0[][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u0[][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt u1[][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]);

int OT_BLC_Open_memopt(const OT_blc_key_memopt_t* key, const uint16_t i_star[MQOM3_PARAM_TAU], const field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], uint8_t opening[MQOM3_PARAM_OPENING_SIZE]);

int OT_BLC_Eval_memopt(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t opening[MQOM3_PARAM_OPENING_SIZE], const uint16_t i_star[MQOM3_PARAM_TAU], uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE], field_ext_elt x_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]);

/* Evaluate a single batch [e_start, e_start+batch) via one large-tree traversal pass.
 * batch must be <= BLC_OT_BATCH_SIZE. hidden_leaf_idxs must be the full-TAU array
 * (TAU * i_star[e] + e for every e), since the tree opening was built against all
 * TAU hidden leaves jointly and cannot be reconstructed from a slice. path and
 * ls_com_hidden/delta_x_in point into the full opening buffer, exactly as parsed
 * by OT_BLC_Eval_memopt. */
int OT_BLC_Eval_memopt_partial(uint32_t e_start, uint32_t batch, const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t path[MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE], const uint16_t i_star[MQOM3_PARAM_TAU], const uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU], const uint8_t *ls_com_hidden, const uint8_t *delta_x_in, uint8_t com1[][MQOM3_PARAM_DIGEST_SIZE], field_ext_elt x_eval[][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u_eval[][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]);

void OT_BLC_PrintConfig_memopt(void);

#endif /* __BLC_OT_MEMOPT_H__ */
