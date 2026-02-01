#include "expand_mq.h"
#ifdef SUPERCOP
#include "crypto_declassify.h"
#endif

int ExpandEquations_memopt_init(const uint8_t mseed_eq[2 * MQOM2_PARAM_SEED_SIZE], ExpandEquations_ctx *ctx) {
	int ret = -1;

	if ((mseed_eq == NULL) || (ctx == NULL)) {
		goto err;
	}

#ifdef SUPERCOP
        /* Public seed equation declassification for SUPERCOP */
        crypto_declassify(mseed_eq, 2 * MQOM2_PARAM_SEED_SIZE);
#endif

	/* Initialize */
	memcpy(ctx->mseed_eq, mseed_eq, 2 * MQOM2_PARAM_SEED_SIZE);
	ctx->i = ctx->j = ctx->current_nbytes = 0;
	memset(ctx->leftover, 0, MQOM2_PARAM_SEED_SIZE);
	ctx->magic = EXPAND_EQUATIONS_CTX_MAGIC;

	/* Initialize the PRG cache when used */
	uint32_t nf_eq = MQOM2_PARAM_MQ_N + (MQOM2_PARAM_MQ_N * (MQOM2_PARAM_MQ_N + 1) / 2);
	uint32_t nb_eq = nf_eq * FIELD_EXT_LOG2_CARD  / 8;
#ifndef NO_EXPANDMQ_PRG_CACHE
	/* Initialize the PRG cache if asked to */
	ctx->prg_cache = init_prg_cache_pub(nb_eq);
#else
	(void)nb_eq;
#endif

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
		/* Specific case for b, when j == MQOM2_PARAM_MQ_N for b_hat generation */
		if (j == MQOM2_PARAM_MQ_N) {
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
	uint8_t salt[MQOM2_PARAM_SALT_SIZE];
	prg_key_sched_cache_pub *prg_cache = ctx->prg_cache;
	xof_context DECL_VAR(xof_ctx);

	/* Check init */
	if ((ctx == NULL) || (ctx->magic != EXPAND_EQUATIONS_CTX_MAGIC)) {
		goto err;
	}

	/* No more updates allowed, or bad j index ... */
	if ((ctx->i >= MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU) || (ctx->j > MQOM2_PARAM_MQ_N)) {
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
		ret = xof_update(&xof_ctx, (const uint8_t*) "\x01", 1);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, ctx->mseed_eq, 2 * MQOM2_PARAM_SEED_SIZE);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, i_16, sizeof(i_16));
		ERR(ret, err);
		ret = xof_squeeze(&xof_ctx, ctx->seed_eq, MQOM2_PARAM_SEED_SIZE);
		ERR(ret, err);
	}
	/* Call the memopt PRG with our bytes to generate and the leftover to update */
	memset(salt, 0, MQOM2_PARAM_SALT_SIZE);
	ExpandEquations_memopt_get_PRG_index(ctx->i, ctx->j, &start_nbytes, &nbytes);
	ret = PRG_memopt_pub(salt, 0, ctx->seed_eq,  start_nbytes, nbytes, (uint8_t*)row, ctx->leftover, prg_cache);
	ERR(ret, err);
	/* Update i and j */
	ctx->j = (ctx->j + 1) % (MQOM2_PARAM_MQ_N + 1);
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
	destroy_prg_cache_pub(ctx->prg_cache);

	return;
}

int ExpandEquations(const uint8_t mseed_eq[2 * MQOM2_PARAM_SEED_SIZE], field_ext_elt A_hat[MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU][MQOM2_PARAM_MQ_N][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)], field_ext_elt b_hat[MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU][FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N)]) {
	int ret = -1;
	uint32_t i, j, nb_eq, nf_eq;
	uint8_t prg_salt[MQOM2_PARAM_SALT_SIZE] = { 0 };
	uint8_t *stream = NULL;

	prg_key_sched_cache_pub *prg_cache = NULL;

#ifdef SUPERCOP
        /* Public seed equation declassification for SUPERCOP */
        crypto_declassify(mseed_eq, 2 * MQOM2_PARAM_SEED_SIZE);
#endif

	/* Compute the number of PRG bytes */
	nf_eq = MQOM2_PARAM_MQ_N + (MQOM2_PARAM_MQ_N * (MQOM2_PARAM_MQ_N + 1) / 2);
	nb_eq = nf_eq * FIELD_EXT_LOG2_CARD  / 8;

	/* Allocate stream */
	stream = (uint8_t*)mqom_malloc(nb_eq * sizeof(uint8_t));
	if (stream == NULL) {
		ret = -1;
		goto err;
	}

#ifndef NO_EXPANDMQ_PRG_CACHE
	/* Initialize the PRG cache when used */
	prg_cache = init_prg_cache_pub(nb_eq);
#endif

	/* Generate the equations */
	i = 0;
	(void)i;
#if defined(USE_XOF_X4)
	/* When using X4 XOF, we check for the proper size */
	if ((MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU) < 4) {
		/* In all our instances, m should be a multiple of 4:
		 * no leftover should remain for XOF X4. If this is
		 * not the case, return an error. */
		ret = -1;
		goto err;
	}
	/* Treat as many 4 batches as we can, and then deal with the leftover using simple XOFs */
	for (i = 3; i < MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU; i += 4) {
		xof_context_x4 DECL_VAR(xof_ctx_x4);
		uint32_t k, z;
		uint8_t i_16[4][2];
		uint8_t seed_eq[4][MQOM2_PARAM_SEED_SIZE];
		const uint8_t *constant_1[4] = { (const uint8_t*) "\x01", (const uint8_t*) "\x01", (const uint8_t*) "\x01", (const uint8_t*) "\x01" };
		const uint8_t *mseed_eq_ptr[4] = { mseed_eq, mseed_eq, mseed_eq, mseed_eq };
		const uint8_t *i_16_ptr[4] = { i_16[0], i_16[1], i_16[2], i_16[3] };
		uint8_t *seed_eq_ptr[4] = { seed_eq[0], seed_eq[1], seed_eq[2], seed_eq[3] };
		for (z = 0; z < 4; z++) {
			i_16[z][0] = ((i - z) & 0xff);
			i_16[z][1] = (((i - z) >> 8) & 0xff);
		}
		ret = xof_init_x4(&xof_ctx_x4);
		ERR(ret, err);
		ret = xof_update_x4(&xof_ctx_x4, constant_1, 1);
		ERR(ret, err);
		ret = xof_update_x4(&xof_ctx_x4, mseed_eq_ptr, 2 * MQOM2_PARAM_SEED_SIZE);
		ERR(ret, err);
		ret = xof_update_x4(&xof_ctx_x4, i_16_ptr, 2);
		ERR(ret, err);
		ret = xof_squeeze_x4(&xof_ctx_x4, seed_eq_ptr, MQOM2_PARAM_SEED_SIZE);
		ERR(ret, err);
		for (z = 0; z < 4; z++) {
			ret = PRG_pub(prg_salt, 0, seed_eq[z], nb_eq, stream, prg_cache);
			ERR(ret, err);
			k = 0;
			for (j = 0; j < MQOM2_PARAM_MQ_N; j++) {
				/* Fill the jth row of Ai */
				memset(A_hat[i - z][j], 0, FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N) * sizeof(field_ext_elt));
				field_ext_parse(&stream[k], (j + 1), A_hat[i - z][j]);
				k += (j + 1) * sizeof(field_ext_elt);
			}
			/* Fill bi */
			field_ext_parse(&stream[k], MQOM2_PARAM_MQ_N, b_hat[i - z]);
		}
		if ((i + 4) >= MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU) {
			xof_clean_ctx_x4(&xof_ctx_x4);
			goto left_over;
		}
		xof_clean_ctx_x4(&xof_ctx_x4);
	}
left_over:
#endif
	for (; i < MQOM2_PARAM_MQ_M / MQOM2_PARAM_MU; i++) {
		xof_context DECL_VAR(xof_ctx);
		uint32_t k;
		uint8_t i_16[2];
		uint8_t seed_eq[MQOM2_PARAM_SEED_SIZE];
		i_16[0] = (i & 0xff);
		i_16[1] = ((i >> 8) & 0xff);
		ret = xof_init(&xof_ctx);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, (const uint8_t*) "\x01", 1);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, mseed_eq, 2 * MQOM2_PARAM_SEED_SIZE);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, i_16, sizeof(i_16));
		ERR(ret, err);
		ret = xof_squeeze(&xof_ctx, seed_eq, MQOM2_PARAM_SEED_SIZE);
		ERR(ret, err);
		ret = PRG_pub(prg_salt, 0, seed_eq, nb_eq, stream, prg_cache);
		ERR(ret, err);
		k = 0;
		for (j = 0; j < MQOM2_PARAM_MQ_N; j++) {
			/* Fill the jth row of Ai */
			memset(A_hat[i][j], 0, FIELD_EXT_PACKING(MQOM2_PARAM_MQ_N) * sizeof(field_ext_elt));
			field_ext_parse(&stream[k], (j + 1), A_hat[i][j]);
			k += (j + 1) * sizeof(field_ext_elt);
		}
		/* Fill bi */
		field_ext_parse(&stream[k], MQOM2_PARAM_MQ_N, b_hat[i]);
		xof_clean_ctx(&xof_ctx);
	}

	ret = 0;
err:
	if (stream != NULL) {
		mqom_free(stream, nb_eq * sizeof(uint8_t));
	}
	destroy_prg_cache_pub(prg_cache);
	return ret;
}
