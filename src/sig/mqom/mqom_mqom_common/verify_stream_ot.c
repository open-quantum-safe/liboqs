/* MQOM3_VARIANT_GUARD: this unit implements the OT variant only. Build systems
 * that compile every .c of a directory - SUPERCOP does, and so does any wildcard
 * Makefile - hand it to the compiler for CT instances too, where its parameters
 * simply do not exist. Rather than requiring every such build to filter its source
 * list, the unit reduces to a single typedef there. Not an empty file: ISO C forbids
 * an empty translation unit and -Wpedantic says so.
 * The in-tree Makefile still selects the right family, so nothing changes for it. */
#include "mqom3_parameters.h"
#if MQOM3_PARAM_OT_VARIANT == 1

#ifdef SUPERCOP
#include "crypto_sign.h"
#else
#include "api.h"
#endif

#include <stdlib.h>
#include "common.h"
#include "fields.h"
#include "xof.h"
#include "blc.h"
#include "blc_ot_memopt.h"
#include "piop.h"
#include "piop_memopt.h"
#include "sample_challenge_verify.h"
#include "verify_stream.h"
#include "domain_separation.h"

static int process_header(OT_stream_verify_ctx_t *ctx, const uint8_t *header) {
	int ret = -1;
	uint32_t e;
	const uint8_t *sig_id = &header[0];
	const uint8_t *salt   = &header[MQOM3_PARAM_DIGEST_SIZE];
	const uint8_t *nonce  = &header[MQOM3_PARAM_DIGEST_SIZE + MQOM3_PARAM_SALT_SIZE];

	memcpy(ctx->sig_id, sig_id, MQOM3_PARAM_DIGEST_SIZE);
	memcpy(ctx->salt, salt, MQOM3_PARAM_SALT_SIZE);

	{
		grinding_material_t mat = { 0 };
		int mat_precomputed = 0;
		ret = DeriveChallenge(ctx->sig_id, nonce, ctx->i_star, &mat, &mat_precomputed);
		grinding_material_clean(&mat);
	}
	ERR(ret, err);

	/* hidden_leaf_idxs[e] = TAU * i_star[e] + e (column-major BLC convention) */
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		ctx->hidden_leaf_idxs[e] = (uint32_t)(MQOM3_PARAM_TAU * (uint32_t)ctx->i_star[e] + e);
	}

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

static int process_prefix(OT_stream_verify_ctx_t *ctx) {
	int ret = -1;
	uint32_t e_start, batch;
	const uint8_t *path          = &ctx->prefix_buf[0];
	const uint8_t *ls_com_hidden = path + (size_t)MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE;
	const uint8_t *delta_x_in    = ls_com_hidden + (size_t)MQOM3_PARAM_TAU * MQOM3_PARAM_DIGEST_SIZE;

	for (e_start = 0; e_start < MQOM3_PARAM_TAU; e_start += BLC_OT_BATCH_SIZE) {
		batch = (MQOM3_PARAM_TAU - e_start < BLC_OT_BATCH_SIZE) ? (MQOM3_PARAM_TAU - e_start) : BLC_OT_BATCH_SIZE;
		ret = OT_BLC_Eval_memopt_partial(e_start, batch, ctx->salt, path, ctx->i_star, ctx->hidden_leaf_idxs,
		                                 ls_com_hidden, delta_x_in, &ctx->com1[e_start],
		                                 &ctx->x_eval[e_start], &ctx->u_eval[e_start]);
		ERR(ret, err);
	}

	/* Absorb com1[0..TAU-1] into the still-open presigid (Hash_2) context, in
	 * the exact same raw-array form Sign()/Verify_default() feed it. */
	ret = xof_update(&ctx->presigid_ctx, &ctx->com1[0][0], (size_t)MQOM3_PARAM_TAU * MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}

static int flush_alpha1_batch(OT_stream_verify_ctx_t *ctx, uint32_t e_batch_start) {
	int ret = -1;
	uint32_t i;
	field_ext_elt alpha0_batch[STREAM_VERIFY_BATCH][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	uint8_t alpha_ser[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU)];

	ret = RecomputePAlpha_partial_memopt(ctx->batch_count,
	        (const uint8_t (*)[MQOM3_PARAM_DIGEST_SIZE])&ctx->com1[e_batch_start],
	        (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])ctx->alpha1_batch,
	        &ctx->i_star[e_batch_start],
	        (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)])&ctx->x_eval[e_batch_start],
	        (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])&ctx->u_eval[e_batch_start],
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

	ctx->batch_count = 0;
	ret = 0;
err:
	return ret;
}

static int process_alpha1_chunk(OT_stream_verify_ctx_t *ctx) {
	int ret = -1;
	uint32_t b = ctx->batch_count;

	field_ext_parse(ctx->alpha1_chunk_buf, MQOM3_PARAM_ETA, ctx->alpha1_batch[b]);
	ctx->batch_count++;

	if ((ctx->batch_count == STREAM_VERIFY_BATCH) || (ctx->e == MQOM3_PARAM_TAU - 1)) {
		ret = flush_alpha1_batch(ctx, ctx->e - ctx->batch_count + 1);
		ERR(ret, err);
	}

	ret = 0;
err:
	return ret;
}

OT_stream_verify_ctx_t *OT_StreamedVerify_Init(const uint8_t pk[MQOM3_PK_SIZE]) {
	OT_stream_verify_ctx_t *ctx;

	if (pk == NULL) {
		return NULL;
	}
	ctx = mqom_malloc(sizeof(OT_stream_verify_ctx_t));
	if (ctx == NULL) {
		return NULL;
	}
	memset(ctx, 0, sizeof(*ctx));

	ctx->prefix_buf = mqom_malloc((size_t)STREAM_VERIFY_OT_PREFIX_SIZE);
	if (ctx->prefix_buf == NULL) {
		mqom_free(ctx, sizeof(*ctx));
		return NULL;
	}

	memcpy(ctx->pk, pk, MQOM3_PK_SIZE);
	memcpy(ctx->mseed_eq, &pk[0], 2 * MQOM3_PARAM_SEED_SIZE);
	field_ext_parse(&pk[2 * MQOM3_PARAM_SEED_SIZE], MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU, ctx->y);

	return ctx;
}

int OT_StreamedVerify_Update(OT_stream_verify_ctx_t *ctx, const uint8_t *sigpart, size_t sigpartlen) {
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

		/* Exactly TAU alpha1 chunks are expected in phase 2; anything past
		 * that must be rejected here, before it can drive
		 * process_alpha1_chunk()/flush_alpha1_batch() into indexing
		 * ctx->com1[]/ctx->x_eval[]/ctx->u_eval[] at or past MQOM3_PARAM_TAU. */
		if ((ctx->phase == 2) && (ctx->e >= MQOM3_PARAM_TAU)) {
			ret = -1;
			goto err;
		}

		if (ctx->phase == 0) {
			/* Header is small; reuse prefix_buf as scratch (always large enough). */
			next_chunk_size = STREAM_VERIFY_OT_HEADER_SIZE;
		} else if (ctx->phase == 1) {
			next_chunk_size = (uint32_t)STREAM_VERIFY_OT_PREFIX_SIZE;
		} else {
			next_chunk_size = STREAM_VERIFY_OT_ALPHA1_CHUNK_SIZE;
		}

		remaining_needed = (size_t)next_chunk_size - ctx->pos;
		remaining_avail = sigpartlen - offset;
		to_copy = (remaining_needed < remaining_avail) ? remaining_needed : remaining_avail;

		if (ctx->phase == 2) {
			memcpy(&ctx->alpha1_chunk_buf[ctx->pos], sigpart + offset, to_copy);
		} else {
			memcpy(&ctx->prefix_buf[ctx->pos], sigpart + offset, to_copy);
		}
		offset += to_copy;
		ctx->pos += (uint32_t)to_copy;

		if (ctx->pos == next_chunk_size) {
			if (ctx->phase == 0) {
				ret = process_header(ctx, ctx->prefix_buf);
				ERR(ret, err);
				ctx->phase = 1;
			} else if (ctx->phase == 1) {
				ret = process_prefix(ctx);
				ERR(ret, err);
				/* prefix_buf is only read by process_prefix(); phase 2 writes
				 * into alpha1_chunk_buf instead, so free it now rather than
				 * holding it until Clean(). */
				mqom_cleanse(ctx->prefix_buf, STREAM_VERIFY_OT_PREFIX_SIZE);
				mqom_free(ctx->prefix_buf, STREAM_VERIFY_OT_PREFIX_SIZE);
				ctx->prefix_buf = NULL;
				ctx->phase = 2;
				ctx->e = 0;
			} else {
				ret = process_alpha1_chunk(ctx);
				ERR(ret, err);
				ctx->e++;
			}
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

int OT_StreamedVerify_Finalize(OT_stream_verify_ctx_t *ctx, const uint8_t *msg, unsigned long long mlen) {
	int ret = -1;
	uint8_t msg_hash[MQOM3_PARAM_DIGEST_SIZE];
	uint8_t com2[MQOM3_PARAM_DIGEST_SIZE];
	uint8_t presig_id[MQOM3_PARAM_DIGEST_SIZE];
	uint8_t sig_id_[MQOM3_PARAM_DIGEST_SIZE];
	xof_context xof_ctx = { 0 };

	if ((ctx == NULL) || (ctx->state != STREAM_VERIFY_STATE_ACTIVE) || !ctx->header_ok ||
	    (ctx->phase != 2) || (ctx->e != MQOM3_PARAM_TAU) || (ctx->pos != 0)) {
		return -1;
	}

	ret = xof_squeeze(&ctx->com2_ctx, com2, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	ret = xof_update(&ctx->presigid_ctx, com2, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(&ctx->presigid_ctx, presig_id, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) DOMAIN_SEP_MSGDIGEST, 1);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, msg, (size_t)mlen);
	ERR(ret, err);
	ret = xof_squeeze(&xof_ctx, msg_hash, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

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

void OT_StreamedVerify_Clean(OT_stream_verify_ctx_t *ctx) {
	if (ctx == NULL) {
		return;
	}
	xof_clean_ctx(&ctx->com2_ctx);
	xof_clean_ctx(&ctx->presigid_ctx);
	if (ctx->prefix_buf != NULL) {
		mqom_cleanse(ctx->prefix_buf, STREAM_VERIFY_OT_PREFIX_SIZE);
		mqom_free(ctx->prefix_buf, STREAM_VERIFY_OT_PREFIX_SIZE);
	}
	mqom_cleanse(ctx, sizeof(*ctx));
	mqom_free(ctx, sizeof(*ctx));
}

#else /* MQOM3_VARIANT_GUARD: CT build, nothing to compile here */
typedef int verify_stream_ot_unused_in_ct_build;
#endif
