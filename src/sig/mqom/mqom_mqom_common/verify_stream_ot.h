#ifndef __VERIFY_STREAM_OT_H__
#define __VERIFY_STREAM_OT_H__

#include "common.h"

/* OT-only header. Until MQOM3_PARAM_OPENING_SIZE became the family-agnostic
 * alias (common.h), reaching this file from a CT build failed loudly on the
 * undefined MQOM3_PARAM_OT_OPENING_SIZE. That accident was the only thing
 * catching a stray include; state the requirement instead. */
#if MQOM3_PARAM_OT_VARIANT != 1
#error "OT-only header reached from a non-OT build"
#endif
#include "fields.h"
#include "xof.h"
#include "blc.h"
#include "blc_ot_memopt.h"
#include "domain_separation.h"

/* Deal with namespacing */
#define OT_StreamedVerify_Init MQOM_NAMESPACE(OT_StreamedVerify_Init)
#define OT_StreamedVerify_Update MQOM_NAMESPACE(OT_StreamedVerify_Update)
#define OT_StreamedVerify_Finalize MQOM_NAMESPACE(OT_StreamedVerify_Finalize)
#define OT_StreamedVerify_Clean MQOM_NAMESPACE(OT_StreamedVerify_Clean)

/* Signature is consumed in 3 phases:
 *  - header chunk: sig_id | salt | nonce
 *  - one monolithic "prefix" chunk: path | ls_com_hidden | delta_x. Must be
 *    buffered whole: every pass over the shared large tree needs the full
 *    T_open path and the full set of hidden leaf indices (TAU * i_star[e] + e
 *    for every e), so no slice of these prefix bytes is usable on its own.
 *    Note this bounds the *bytes*, not the executions: since the batch-relative
 *    refactor, OT_BLC_Eval_memopt_partial does slice the executions, and
 *    process_prefix() walks the tree in ceil(TAU / BLC_OT_BATCH_SIZE) passes -
 *    but each of those passes still needs the whole prefix. (The hidden leaf
 *    indices themselves come from process_header(), not from these bytes; what
 *    the prefix carries is the T_open path, consumed left-first across the
 *    whole traversal, and ls_com_hidden, needed for every e.)
 *  - TAU per-execution alpha1 chunks, batched by STREAM_VERIFY_BATCH. */
#define STREAM_VERIFY_OT_HEADER_SIZE ((MQOM3_PARAM_DIGEST_SIZE) + (MQOM3_PARAM_SALT_SIZE) + 4)
#define STREAM_VERIFY_OT_PREFIX_SIZE (                                            \
    MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE                          \
      + MQOM3_PARAM_TAU * MQOM3_PARAM_DIGEST_SIZE                                 \
      + MQOM3_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)                  \
)
/* OT_StreamVerify allocates a single buffer of PREFIX_SIZE and reuses it as the
 * scratch for the header during phase 0, so the prefix must be at least as
 * large as the header. It is, by a factor of 30 to 69 on the 9 OT parameter
 * sets, but the reuse writes attacker-controlled bytes: if the invariant ever
 * broke, that would be a heap overflow rather than a wrong result. */
#if STREAM_VERIFY_OT_HEADER_SIZE > STREAM_VERIFY_OT_PREFIX_SIZE
#error "OT_StreamVerify reuses prefix_buf as header scratch: STREAM_VERIFY_OT_PREFIX_SIZE must be >= STREAM_VERIFY_OT_HEADER_SIZE"
#endif
#define STREAM_VERIFY_OT_ALPHA1_CHUNK_SIZE (BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU))

typedef struct {
	uint8_t pk[MQOM3_PK_SIZE];
	uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE];
	field_ext_elt y[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];

	uint8_t sig_id[MQOM3_PARAM_DIGEST_SIZE];
	uint8_t salt[MQOM3_PARAM_SALT_SIZE];
	uint16_t i_star[MQOM3_PARAM_TAU];
	uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU];

	xof_context com2_ctx; /* com2 accumulator (Hash_1) */
	xof_context presigid_ctx; /* presig_id accumulator (Hash_2) */

	/* Phase B persistent state: the whole tree pass must complete before any
	 * alpha1 arrives, so x_eval/u_eval/com1 for all TAU executions must be
	 * kept around for phase C (memory floor, independent of STREAM_VERIFY_BATCH). */
	uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE];
	field_ext_elt x_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt u_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];

	/* Phase C batch-local scratch */
	field_ext_elt alpha1_batch[STREAM_VERIFY_BATCH][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	uint32_t batch_count;

	/* Byte-level chunk reassembly. The prefix chunk is the largest. */
	uint8_t *prefix_buf; /* heap-allocated: STREAM_VERIFY_OT_PREFIX_SIZE can be large */
	uint8_t alpha1_chunk_buf[STREAM_VERIFY_OT_ALPHA1_CHUNK_SIZE];
	uint32_t pos;
	int phase; /* 0 = header, 1 = prefix, 2 = alpha1 stream */
	uint32_t e; /* next execution index expected in phase 2 (0..TAU) */

	int header_ok;
	int state; /* STREAM_VERIFY_STATE_{ACTIVE,FAILED,DONE} */
} OT_stream_verify_ctx_t;

OT_stream_verify_ctx_t *OT_StreamedVerify_Init(const uint8_t pk[MQOM3_PK_SIZE]);

int OT_StreamedVerify_Update(OT_stream_verify_ctx_t *ctx, const uint8_t *sigpart, size_t sigpartlen);

int OT_StreamedVerify_Finalize(OT_stream_verify_ctx_t *ctx, const uint8_t *msg, unsigned long long mlen);

void OT_StreamedVerify_Clean(OT_stream_verify_ctx_t *ctx);

#endif /* __VERIFY_STREAM_OT_H__ */
