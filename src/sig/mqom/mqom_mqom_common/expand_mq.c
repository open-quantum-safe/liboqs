#include "expand_mq.h"
#ifdef SUPERCOP
#include "crypto_declassify.h"
#endif

int ExpandEquations_memopt_init(const uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE], ExpandEquations_ctx *ctx) {
	int ret = -1;

	if ((mseed_eq == NULL) || (ctx == NULL)) {
		goto err;
	}

#ifdef SUPERCOP
        /* Public seed equation declassification for SUPERCOP */
        crypto_declassify(mseed_eq, 2 * MQOM3_PARAM_SEED_SIZE);
#endif

	/* Initialize */
	memcpy(ctx->mseed_eq, mseed_eq, 2 * MQOM3_PARAM_SEED_SIZE);
	ctx->i = ctx->j = ctx->current_nbytes = 0;
	memset(ctx->leftover, 0, MQOM3_PARAM_SEED_EQ_SIZE);
	memset(&ctx->prg_enc_ctx, 0, sizeof(ctx->prg_enc_ctx));
	ctx->magic = EXPAND_EQUATIONS_CTX_MAGIC;

	ret = 0;
err:
	return ret;
}

/* Helper to compute the proper indices for the PRG
 * XXX: since we are sure that the ExpandEquations primitive works on
 * extension fields of MQOM, we are sure that the rows are on bytes boundaries and use
 * this fact in the helper below.
 */
static void ExpandEquations_memopt_get_PRG_index(uint32_t i, uint32_t j, uint32_t *start_nbytes, uint32_t *nbytes) {
	(void)i;

	if (start_nbytes != NULL) {
		(*start_nbytes) = (j * (j + 1) / 2) * (FIELD_EXT_LOG2_CARD / 8);
	}
	if (nbytes != NULL) {
		/* Specific case for b, when j == MQOM3_PARAM_MQ_N for b_hat generation */
		if (j == MQOM3_PARAM_MQ_N) {
			(*nbytes) = j * (FIELD_EXT_LOG2_CARD / 8);
		} else {
			(*nbytes) = (j + 1) * (FIELD_EXT_LOG2_CARD / 8);
		}
	}

	return;
}

/* Extract elements one row at a time */
int ExpandEquations_memopt_update(ExpandEquations_ctx *ctx, field_ext_elt *row) {
	int ret = -1;
	uint32_t start_nbytes, nbytes;
	xof_context xof_ctx = { 0 };

	/* Check init */
	if ((ctx == NULL) || (ctx->magic != EXPAND_EQUATIONS_CTX_MAGIC)) {
		goto err;
	}

	/* No more updates allowed, or bad j index ... */
	if ((ctx->i >= MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU) || (ctx->j > MQOM3_PARAM_MQ_N)) {
		goto err;
	}

	/* Are we on a boundary of an i?
	 * In this case, we compute the local seed_eq
	 */
	if (ctx->j == 0) {
		uint8_t i_16[2];
		i_16[0] = (ctx->i & 0xff);
		i_16[1] = ((ctx->i >> 8) & 0xff);
		ret = xof_init(&xof_ctx);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, (const uint8_t*) DOMAIN_SEP_SEEDEQ, 1);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, ctx->mseed_eq, 2 * MQOM3_PARAM_SEED_SIZE);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, i_16, sizeof(i_16));
		ERR(ret, err);
		ret = xof_squeeze(&xof_ctx, ctx->seed_eq, MQOM3_PARAM_SEED_EQ_SIZE);
		ERR(ret, err);
		/* Pre-compute the key schedule for seed_eq[i]: reused for all N+1 rows of this equation */
		ret = enc_key_sched_pub_ecb(&ctx->prg_enc_ctx, ctx->seed_eq);
		ERR(ret, err);
	}
	/* Call the memopt PRG with the cached key schedule */
	ExpandEquations_memopt_get_PRG_index(ctx->i, ctx->j, &start_nbytes, &nbytes);
	ret = PRG_memopt_with_ctx(&ctx->prg_enc_ctx, start_nbytes, nbytes, (uint8_t*)row, ctx->leftover);
	ERR(ret, err);
	/* On big-endian platforms, PRG writes bytes in canonical (LE) order but reading
	 * them as multi-byte field_ext_elt values requires byte-swapping. field_ext_parse
	 * handles this correctly (no-op on LE, byte-swap per element on BE). */
	if (arch_is_big_endian() && (sizeof(field_ext_elt) > 1)) {
		field_ext_parse((const uint8_t*)row, (nbytes / sizeof(field_ext_elt)), row);
	}
	/* Update i and j */
	ctx->j = (ctx->j + 1) % (MQOM3_PARAM_MQ_N + 1);
	if (ctx->j == 0) {
		ctx->i++;
	}

	ret = 0;
err:
	xof_clean_ctx(&xof_ctx);
	return ret;
}

void ExpandEquations_memopt_final(ExpandEquations_ctx *ctx) {
	/* Check init */
	if ((ctx == NULL) || (ctx->magic != EXPAND_EQUATIONS_CTX_MAGIC)) {
		return;
	}

	mqom_cleanse(ctx->seed_eq,  sizeof(ctx->seed_eq));
	mqom_cleanse(ctx->mseed_eq, sizeof(ctx->mseed_eq));
	enc_clean_ctx_pub_ecb(&ctx->prg_enc_ctx);
	ctx->magic = 0;
}

int ExpandEquations(const uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE], field_ext_elt A_hat[MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU][MQOM3_PARAM_MQ_N][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt b_hat[MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)]) {
	int ret = -1;
	uint32_t i, j, nb_eq, nf_eq;
	uint8_t *stream = NULL;

#ifdef SUPERCOP
        /* Public seed equation declassification for SUPERCOP */
        crypto_declassify(mseed_eq, 2 * MQOM3_PARAM_SEED_SIZE);
#endif

	/* Compute the number of PRG bytes */
	nf_eq = MQOM3_PARAM_MQ_N + (MQOM3_PARAM_MQ_N * (MQOM3_PARAM_MQ_N + 1) / 2);
	nb_eq = nf_eq * FIELD_EXT_LOG2_CARD  / 8;

	/* Allocate stream */
	stream = (uint8_t*)mqom_malloc(nb_eq * sizeof(uint8_t));
	if (stream == NULL) {
		ret = -1;
		goto err;
	}

	/* Generate the equations */
	i = 0;
	(void)i;
#if defined(USE_XOF_X8)
	/* x8 batches; the x4 remainder and single-element fallback are below,
	 * shared with the USE_XOF_X4-only case. */
	for (; i + 8 <= MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU; i += 8) {
		xof_context_x8 xof_ctx_x8 = { 0 };
		uint32_t k, z;
		uint8_t i_16[8][2];
		uint8_t seed_eq[8][MQOM3_PARAM_SEED_EQ_SIZE];
		const uint8_t *constant_1[8] = { (const uint8_t*) DOMAIN_SEP_SEEDEQ, (const uint8_t*) DOMAIN_SEP_SEEDEQ, (const uint8_t*) DOMAIN_SEP_SEEDEQ, (const uint8_t*) DOMAIN_SEP_SEEDEQ,
		                                  (const uint8_t*) DOMAIN_SEP_SEEDEQ, (const uint8_t*) DOMAIN_SEP_SEEDEQ, (const uint8_t*) DOMAIN_SEP_SEEDEQ, (const uint8_t*) DOMAIN_SEP_SEEDEQ };
		const uint8_t *mseed_eq_ptr[8] = { mseed_eq, mseed_eq, mseed_eq, mseed_eq, mseed_eq, mseed_eq, mseed_eq, mseed_eq };
		const uint8_t *i_16_ptr[8] = { i_16[0], i_16[1], i_16[2], i_16[3], i_16[4], i_16[5], i_16[6], i_16[7] };
		uint8_t *seed_eq_ptr[8] = { seed_eq[0], seed_eq[1], seed_eq[2], seed_eq[3], seed_eq[4], seed_eq[5], seed_eq[6], seed_eq[7] };
		for (z = 0; z < 8; z++) {
			uint32_t idx = i + z;
			i_16[z][0] = (idx & 0xff);
			i_16[z][1] = ((idx >> 8) & 0xff);
		}
		int xof_ret = xof_init_x8(&xof_ctx_x8);
		if (!xof_ret) { xof_ret = xof_update_x8(&xof_ctx_x8, constant_1, 1); }
		if (!xof_ret) { xof_ret = xof_update_x8(&xof_ctx_x8, mseed_eq_ptr, 2 * MQOM3_PARAM_SEED_SIZE); }
		if (!xof_ret) { xof_ret = xof_update_x8(&xof_ctx_x8, i_16_ptr, 2); }
		if (!xof_ret) { xof_ret = xof_squeeze_x8(&xof_ctx_x8, seed_eq_ptr, MQOM3_PARAM_SEED_EQ_SIZE); }
		xof_clean_ctx_x8(&xof_ctx_x8);
		if (xof_ret) { ret = xof_ret; goto err; }
		for (z = 0; z < 8; z++) {
			uint32_t idx = i + z;
			ret = PRG(seed_eq[z], nb_eq, stream);
			ERR(ret, err);
			k = 0;
			for (j = 0; j < MQOM3_PARAM_MQ_N; j++) {
				/* Fill the jth row of Ai */
				memset(A_hat[idx][j], 0, FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N) * sizeof(field_ext_elt));
				field_ext_parse(&stream[k], (j + 1), A_hat[idx][j]);
				k += (j + 1) * sizeof(field_ext_elt);
			}
			/* Fill bi */
			field_ext_parse(&stream[k], MQOM3_PARAM_MQ_N, b_hat[idx]);
		}
	}
#endif /* USE_XOF_X8 */
#if defined(USE_XOF_X8) || defined(USE_XOF_X4)
	/* x4 batches: written once and shared between two cases - it consumes
	 * the x8 loop's remainder above (when USE_XOF_X8 is also set), or the
	 * whole range from i=0 (when only USE_XOF_X4 is set) - so there is a
	 * single x4-batching pattern in this file to audit instead of two.
	 * The "i + 4 <= M" loop condition naturally handles M < 4 (the loop
	 * just never executes and everything falls to the scalar loop below),
	 * so no separate precondition check / early error is needed. */
	for (; i + 4 <= MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU; i += 4) {
		xof_context_x4 xof_ctx_x4 = { 0 };
		uint32_t k, z;
		uint8_t i_16[4][2];
		uint8_t seed_eq[4][MQOM3_PARAM_SEED_EQ_SIZE];
		const uint8_t *constant_1[4] = { (const uint8_t*) DOMAIN_SEP_SEEDEQ, (const uint8_t*) DOMAIN_SEP_SEEDEQ, (const uint8_t*) DOMAIN_SEP_SEEDEQ, (const uint8_t*) DOMAIN_SEP_SEEDEQ };
		const uint8_t *mseed_eq_ptr[4] = { mseed_eq, mseed_eq, mseed_eq, mseed_eq };
		const uint8_t *i_16_ptr[4] = { i_16[0], i_16[1], i_16[2], i_16[3] };
		uint8_t *seed_eq_ptr[4] = { seed_eq[0], seed_eq[1], seed_eq[2], seed_eq[3] };
		for (z = 0; z < 4; z++) {
			uint32_t idx = i + z;
			i_16[z][0] = (idx & 0xff);
			i_16[z][1] = ((idx >> 8) & 0xff);
		}
		int xof_ret = xof_init_x4(&xof_ctx_x4);
		if (!xof_ret) { xof_ret = xof_update_x4(&xof_ctx_x4, constant_1, 1); }
		if (!xof_ret) { xof_ret = xof_update_x4(&xof_ctx_x4, mseed_eq_ptr, 2 * MQOM3_PARAM_SEED_SIZE); }
		if (!xof_ret) { xof_ret = xof_update_x4(&xof_ctx_x4, i_16_ptr, 2); }
		if (!xof_ret) { xof_ret = xof_squeeze_x4(&xof_ctx_x4, seed_eq_ptr, MQOM3_PARAM_SEED_EQ_SIZE); }
		xof_clean_ctx_x4(&xof_ctx_x4);
		if (xof_ret) { ret = xof_ret; goto err; }
		for (z = 0; z < 4; z++) {
			uint32_t idx = i + z;
			ret = PRG(seed_eq[z], nb_eq, stream);
			ERR(ret, err);
			k = 0;
			for (j = 0; j < MQOM3_PARAM_MQ_N; j++) {
				/* Fill the jth row of Ai */
				memset(A_hat[idx][j], 0, FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N) * sizeof(field_ext_elt));
				field_ext_parse(&stream[k], (j + 1), A_hat[idx][j]);
				k += (j + 1) * sizeof(field_ext_elt);
			}
			/* Fill bi */
			field_ext_parse(&stream[k], MQOM3_PARAM_MQ_N, b_hat[idx]);
		}
	}
#endif /* USE_XOF_X8 || USE_XOF_X4 */
	for (; i < MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU; i++) {
		xof_context xof_ctx = { 0 };
		uint32_t k;
		uint8_t i_16[2];
		uint8_t seed_eq[MQOM3_PARAM_SEED_EQ_SIZE];
		i_16[0] = (i & 0xff);
		i_16[1] = ((i >> 8) & 0xff);
		int xof_ret = xof_init(&xof_ctx);
		if (!xof_ret) { xof_ret = xof_update(&xof_ctx, (const uint8_t*) DOMAIN_SEP_SEEDEQ, 1); }
		if (!xof_ret) { xof_ret = xof_update(&xof_ctx, mseed_eq, 2 * MQOM3_PARAM_SEED_SIZE); }
		if (!xof_ret) { xof_ret = xof_update(&xof_ctx, i_16, sizeof(i_16)); }
		if (!xof_ret) { xof_ret = xof_squeeze(&xof_ctx, seed_eq, MQOM3_PARAM_SEED_EQ_SIZE); }
		xof_clean_ctx(&xof_ctx);
		if (xof_ret) { ret = xof_ret; goto err; }
		ret = PRG(seed_eq, nb_eq, stream);
		ERR(ret, err);
		k = 0;
		for (j = 0; j < MQOM3_PARAM_MQ_N; j++) {
			/* Fill the jth row of Ai */
			memset(A_hat[i][j], 0, FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N) * sizeof(field_ext_elt));
			field_ext_parse(&stream[k], (j + 1), A_hat[i][j]);
			k += (j + 1) * sizeof(field_ext_elt);
		}
		/* Fill bi */
		field_ext_parse(&stream[k], MQOM3_PARAM_MQ_N, b_hat[i]);
	}

	ret = 0;
err:
	if (stream != NULL) {
		mqom_free(stream, nb_eq * sizeof(uint8_t));
	}
	return ret;
}
