#ifndef __VERIFY_STREAM_CT_H__
#define __VERIFY_STREAM_CT_H__

#include "common.h"

/* CT-only header: see the symmetric note in blc_ot_default.h. */
#if MQOM3_PARAM_OT_VARIANT == 1
#error "CT-only header reached from an OT build"
#endif
#include "fields.h"
#include "xof.h"
#include "blc_ct_common.h"
#include "domain_separation.h"

/* Deal with namespacing */
#define CT_StreamedVerify_Init MQOM_NAMESPACE(CT_StreamedVerify_Init)
#define CT_StreamedVerify_Update MQOM_NAMESPACE(CT_StreamedVerify_Update)
#define CT_StreamedVerify_Finalize MQOM_NAMESPACE(CT_StreamedVerify_Finalize)
#define CT_StreamedVerify_Clean MQOM_NAMESPACE(CT_StreamedVerify_Clean)

/* Signature is consumed as: header chunk (sig_id|salt|nonce), then one
 * CT_CHUNK_SIZE chunk per execution (path|ls_com|partial_delta_x|alpha1). */
#define STREAM_VERIFY_CT_HEADER_SIZE ((MQOM3_PARAM_DIGEST_SIZE) + (MQOM3_PARAM_SALT_SIZE) + 4)
#if STREAM_VERIFY_CT_HEADER_SIZE > CT_CHUNK_SIZE
#define STREAM_VERIFY_CT_LARGEST_CHUNK STREAM_VERIFY_CT_HEADER_SIZE
#else
#define STREAM_VERIFY_CT_LARGEST_CHUNK CT_CHUNK_SIZE
#endif

typedef struct {
	uint8_t pk[MQOM3_PK_SIZE];
	uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE];
	field_ext_elt y[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];

	uint8_t sig_id[MQOM3_PARAM_DIGEST_SIZE];
	uint8_t salt[MQOM3_PARAM_SALT_SIZE];
	uint16_t i_star[MQOM3_PARAM_TAU];

	/* com2 accumulator (Hash_1) and presig_id accumulator (Hash_2), kept
	 * open across Update() calls: sponge absorption is streaming-safe, so
	 * batches are absorbed as they are computed instead of being buffered. */
	xof_context com2_ctx;
	xof_context presigid_ctx;

	/* Batch-local scratch: up to STREAM_VERIFY_BATCH executions at a time. */
	uint8_t com1_batch[STREAM_VERIFY_BATCH][MQOM3_PARAM_DIGEST_SIZE];
	field_ext_elt x_eval_batch[STREAM_VERIFY_BATCH][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt u_eval_batch[STREAM_VERIFY_BATCH][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	field_ext_elt alpha1_batch[STREAM_VERIFY_BATCH][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	uint32_t batch_count;

	/* Byte-level chunk reassembly */
	uint8_t unprocessed[STREAM_VERIFY_CT_LARGEST_CHUNK];
	uint32_t pos;
	uint32_t num_current_chunk; /* 0 = header not yet consumed; e+1 once execution e is done */

	int header_ok;
	int state; /* STREAM_VERIFY_STATE_{ACTIVE,FAILED,DONE} */
} CT_stream_verify_ctx_t;

CT_stream_verify_ctx_t *CT_StreamedVerify_Init(const uint8_t pk[MQOM3_PK_SIZE]);

int CT_StreamedVerify_Update(CT_stream_verify_ctx_t *ctx, const uint8_t *sigpart, size_t sigpartlen);

int CT_StreamedVerify_Finalize(CT_stream_verify_ctx_t *ctx, const uint8_t *msg, unsigned long long mlen);

void CT_StreamedVerify_Clean(CT_stream_verify_ctx_t *ctx);

#endif /* __VERIFY_STREAM_CT_H__ */
