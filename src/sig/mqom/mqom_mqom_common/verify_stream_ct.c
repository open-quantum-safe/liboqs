/* MQOM3_VARIANT_GUARD: this unit implements the CT variant only. Build systems
 * that compile every .c of a directory - SUPERCOP does, and so does any wildcard
 * Makefile - hand it to the compiler for OT instances too, where its parameters
 * simply do not exist. Rather than requiring every such build to filter its source
 * list, the unit reduces to a single typedef there. Not an empty file: ISO C forbids
 * an empty translation unit and -Wpedantic says so.
 * The in-tree Makefile still selects the right family, so nothing changes for it. */
#include "mqom3_parameters.h"
#if MQOM3_PARAM_OT_VARIANT != 1

#ifdef SUPERCOP
#include "crypto_sign.h"
#else
#include "api.h"
#endif

#include <stdlib.h>
#include "common.h"
#include "fields.h"
#include "xof.h"
#include "blc_ct_memopt.h"
#include "piop.h"
#include "piop_memopt.h"
#include "sample_challenge_verify.h"
#include "verify_stream.h"
#include "domain_separation.h"

static int process_header(CT_stream_verify_ctx_t *ctx) {
	int ret = -1;
	const uint8_t *sig_id  = &ctx->unprocessed[0];
	const uint8_t *salt    = &ctx->unprocessed[MQOM3_PARAM_DIGEST_SIZE];
	const uint8_t *nonce   = &ctx->unprocessed[MQOM3_PARAM_DIGEST_SIZE + MQOM3_PARAM_SALT_SIZE];

	memcpy(ctx->sig_id, sig_id, MQOM3_PARAM_DIGEST_SIZE);
	memcpy(ctx->salt, salt, MQOM3_PARAM_SALT_SIZE);

	{
		grinding_material_t mat = { 0 };
		int mat_precomputed = 0;
		ret = DeriveChallenge(ctx->sig_id, nonce, ctx->i_star, &mat, &mat_precomputed);
		grinding_material_clean(&mat);
	}
	ERR(ret, err);

	ret = xof_init(&ctx->presigid_ctx);
	ERR(ret, err);
	ret = xof_update(&ctx->presigid_ctx, (const uint8_t*) DOMAIN_SEP_PRESIGID, 1);
	ERR(ret, err);
	ret = xof_update(&ctx->presigid_ctx, ctx->pk, MQOM3_PK_SIZE);
	ERR(ret, err);

	ret = xof_init(&ctx->com2_ctx);
	ERR(ret, err);
	ret = xof_update(&ctx->com2_ctx, (const uint8_t*) DOMAIN_SEP_COM2, 1);
	ERR(ret, err);

	ctx->header_ok = 1;
	ret = 0;
err:
	return ret;
}

static int flush_batch(CT_stream_verify_ctx_t *ctx, uint32_t e_batch_start) {
	int ret = -1;
	uint32_t i;
	field_ext_elt alpha0_batch[STREAM_VERIFY_BATCH][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	uint8_t alpha_ser[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU)];

	ret = RecomputePAlpha_partial_memopt(ctx->batch_count,
	        (const uint8_t (*)[MQOM3_PARAM_DIGEST_SIZE])ctx->com1_batch,
	        (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])ctx->alpha1_batch,
	        &ctx->i_star[e_batch_start],
	        (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)])ctx->x_eval_batch,
	        (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])ctx->u_eval_batch,
	        ctx->mseed_eq, ctx->y, alpha0_batch);
	ERR(ret, err);

	for (i = 0; i < ctx->batch_count; i++) {
		field_ext_serialize(alpha0_batch[i], MQOM3_PARAM_ETA, alpha_ser);
		ret = xof_update(&ctx->com2_ctx, alpha_ser, BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU));
		ERR(ret, err);
		field_ext_serialize(ctx->alpha1_batch[i], MQOM3_PARAM_ETA, alpha_ser);
		ret = xof_update(&ctx->com2_ctx, alpha_ser, BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU));
		ERR(ret, err);
	}

	ret = xof_update(&ctx->presigid_ctx, &ctx->com1_batch[0][0], (size_t)ctx->batch_count * MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	ctx->batch_count = 0;
	ret = 0;
err:
	return ret;
}

static int process_execution_chunk(CT_stream_verify_ctx_t *ctx, uint32_t e) {
	int ret = -1;
	uint32_t b = ctx->batch_count;
	const uint8_t *chunk       = &ctx->unprocessed[0];
	const uint8_t *path_e      = chunk;
	const uint8_t *ls_com_e    = chunk + CT_CHUNK_LSCOM_OFF;
	const uint8_t *pdelta_e    = chunk + CT_CHUNK_PDELTA_OFF;
	const uint8_t *alpha1_e    = chunk + CT_CHUNK_ALPHA1_OFF;

	field_ext_parse(alpha1_e, MQOM3_PARAM_ETA, ctx->alpha1_batch[b]);

	ret = CT_BLC_Eval_memopt_partial(e, ctx->salt, path_e, ls_com_e, pdelta_e, ctx->i_star[e],
	                            ctx->com1_batch[b], ctx->x_eval_batch[b], ctx->u_eval_batch[b]);
	ERR(ret, err);

	ctx->batch_count++;

	if ((ctx->batch_count == STREAM_VERIFY_BATCH) || (e == MQOM3_PARAM_TAU - 1)) {
		ret = flush_batch(ctx, e - ctx->batch_count + 1);
		ERR(ret, err);
	}

	ret = 0;
err:
	return ret;
}

CT_stream_verify_ctx_t *CT_StreamedVerify_Init(const uint8_t pk[MQOM3_PK_SIZE]) {
	CT_stream_verify_ctx_t *ctx;

	if (pk == NULL) {
		return NULL;
	}
	ctx = mqom_malloc(sizeof(CT_stream_verify_ctx_t));
	if (ctx == NULL) {
		return NULL;
	}
	memset(ctx, 0, sizeof(*ctx));

	memcpy(ctx->pk, pk, MQOM3_PK_SIZE);
	memcpy(ctx->mseed_eq, &pk[0], 2 * MQOM3_PARAM_SEED_SIZE);
	field_ext_parse(&pk[2 * MQOM3_PARAM_SEED_SIZE], MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU, ctx->y);

	return ctx;
}

int CT_StreamedVerify_Update(CT_stream_verify_ctx_t *ctx, const uint8_t *sigpart, size_t sigpartlen) {
	int ret = -1;
	size_t offset = 0;

	if ((ctx == NULL) || ((sigpart == NULL) && (sigpartlen != 0))) {
		return -1;
	}
	if (ctx->state != STREAM_VERIFY_STATE_ACTIVE) {
		return -1;
	}

	while (offset < sigpartlen) {
		uint32_t next_chunk_size;
		size_t remaining_needed, remaining_avail, to_copy;

		/* Header (chunk 0) plus exactly TAU execution chunks are expected;
		 * anything past that must be rejected here, before it can drive
		 * process_execution_chunk() into indexing ctx->i_star[]/batch arrays
		 * at or past MQOM3_PARAM_TAU. */
		if (ctx->num_current_chunk > MQOM3_PARAM_TAU) {
			ret = -1;
			goto err;
		}

		next_chunk_size = (ctx->num_current_chunk == 0) ? STREAM_VERIFY_CT_HEADER_SIZE : CT_CHUNK_SIZE;
		remaining_needed = (size_t)next_chunk_size - ctx->pos;
		remaining_avail = sigpartlen - offset;
		to_copy = (remaining_needed < remaining_avail) ? remaining_needed : remaining_avail;

		memcpy(&ctx->unprocessed[ctx->pos], sigpart + offset, to_copy);
		offset += to_copy;
		ctx->pos += (uint32_t)to_copy;

		if (ctx->pos == next_chunk_size) {
			if (ctx->num_current_chunk == 0) {
				ret = process_header(ctx);
			} else {
				ret = process_execution_chunk(ctx, ctx->num_current_chunk - 1);
			}
			ERR(ret, err);

			ctx->num_current_chunk++;
			ctx->pos = 0;
		}
	}

	ret = 0;
err:
	if (ret != 0) {
		ctx->state = STREAM_VERIFY_STATE_FAILED;
	}
	return ret;
}

int CT_StreamedVerify_Finalize(CT_stream_verify_ctx_t *ctx, const uint8_t *msg, unsigned long long mlen) {
	int ret = -1;
	uint8_t msg_hash[MQOM3_PARAM_DIGEST_SIZE];
	uint8_t com2[MQOM3_PARAM_DIGEST_SIZE];
	uint8_t presig_id[MQOM3_PARAM_DIGEST_SIZE];
	uint8_t sig_id_[MQOM3_PARAM_DIGEST_SIZE];
	xof_context xof_ctx = { 0 };

	if ((ctx == NULL) || (ctx->state != STREAM_VERIFY_STATE_ACTIVE) || !ctx->header_ok ||
	    (ctx->num_current_chunk != MQOM3_PARAM_TAU + 1) || (ctx->pos != 0)) {
		return -1;
	}

	/* com2 = squeeze(Hash_1 ctx) */
	ret = xof_squeeze(&ctx->com2_ctx, com2, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* presig_id = squeeze(Hash_2 ctx || com2) */
	ret = xof_update(&ctx->presigid_ctx, com2, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(&ctx->presigid_ctx, presig_id, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* msg_hash = Hash_3(msg) */
	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) DOMAIN_SEP_MSGDIGEST, 1);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, msg, (size_t)mlen);
	ERR(ret, err);
	ret = xof_squeeze(&xof_ctx, msg_hash, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* sig_id' = Hash_4(presig_id, msg_hash) */
	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) DOMAIN_SEP_SIGID, 1);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, presig_id, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, msg_hash, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(&xof_ctx, sig_id_, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	if (memcmp(ctx->sig_id, sig_id_, MQOM3_PARAM_DIGEST_SIZE) != 0) {
		ret = -1;
		goto err;
	}

	ret = 0;
err:
	xof_clean_ctx(&xof_ctx);
	ctx->state = (ret == 0) ? STREAM_VERIFY_STATE_DONE : STREAM_VERIFY_STATE_FAILED;
	return ret;
}

void CT_StreamedVerify_Clean(CT_stream_verify_ctx_t *ctx) {
	if (ctx == NULL) {
		return;
	}
	xof_clean_ctx(&ctx->com2_ctx);
	xof_clean_ctx(&ctx->presigid_ctx);
	mqom_cleanse(ctx, sizeof(*ctx));
	mqom_free(ctx, sizeof(*ctx));
}

#else /* MQOM3_VARIANT_GUARD: OT build, nothing to compile here */
typedef int verify_stream_ct_unused_in_ot_build;
#endif
