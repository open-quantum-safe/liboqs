#include "prg.h"

/* Memory efficient version of the PRG.
 * The idea is to only generate the necessary blocks, with a starting number of bytes
 * and total number of bytes to output.
 */
int PRG_memopt(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM2_PARAM_SEED_SIZE], uint32_t start_nbytes, uint32_t nbytes, uint8_t *out_data, uint8_t in_out_leftover[MQOM2_PARAM_SEED_SIZE], prg_key_sched_cache *cache) {
	int ret = -1;
	uint32_t i, start_block, end_block, treated_bytes;
	enc_ctx DECL_VAR(ctx);
	uint8_t linortho_seed[MQOM2_PARAM_SEED_SIZE];

	/* Compute Psi(seed) once and for all */
	LinOrtho(seed, linortho_seed);

	start_block = (start_nbytes / MQOM2_PARAM_SEED_SIZE);
	end_block   = ((start_nbytes + nbytes) / MQOM2_PARAM_SEED_SIZE);
	treated_bytes = 0;
	for (i = start_block; i < end_block; i++) {
		/* Deal with the beginning leftover */
		if ((i == start_block) && (start_nbytes % MQOM2_PARAM_SEED_SIZE)) {
			uint32_t to_copy = MQOM2_PARAM_SEED_SIZE - (start_nbytes % MQOM2_PARAM_SEED_SIZE);
			/* Directly use the leftover from a previous computation */
			memcpy(&out_data[treated_bytes], &in_out_leftover[start_nbytes % MQOM2_PARAM_SEED_SIZE], to_copy);
			treated_bytes += to_copy;
		} else {
			ret = prg_key_sched(salt, &e, i, &ctx, cache);
			ERR(ret, err);
			ret = enc_encrypt(&ctx, seed, &out_data[treated_bytes]);
			ERR(ret, err);
			xor_blocks(&out_data[treated_bytes], linortho_seed, &out_data[treated_bytes]);
			treated_bytes += MQOM2_PARAM_SEED_SIZE;
		}
	}
	/* Deal with the possible leftover incomplete block at the end */
	if ((nbytes - treated_bytes) % MQOM2_PARAM_SEED_SIZE) {
		/* Do we have to recompute the leftover? */
		ret = prg_key_sched(salt, &e, i, &ctx, cache);
		ERR(ret, err);
		ret = enc_encrypt(&ctx, seed, in_out_leftover);
		ERR(ret, err);
		xor_blocks(in_out_leftover, linortho_seed, in_out_leftover);
		if (treated_bytes == 0) {
			memcpy(&out_data[0], &in_out_leftover[start_nbytes % MQOM2_PARAM_SEED_SIZE], nbytes % MQOM2_PARAM_SEED_SIZE);
		} else {
			memcpy(&out_data[treated_bytes], in_out_leftover, (nbytes - treated_bytes) % MQOM2_PARAM_SEED_SIZE);
		}
	}

	ret = 0;
err:
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		enc_clean_ctx(&ctx);
	}
	return ret;
}
int PRG_memopt_pub(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM2_PARAM_SEED_SIZE], uint32_t start_nbytes, uint32_t nbytes, uint8_t *out_data, uint8_t in_out_leftover[MQOM2_PARAM_SEED_SIZE], prg_key_sched_cache_pub *cache) {
	int ret = -1;
	uint32_t i, start_block, end_block, treated_bytes;
	enc_ctx_pub DECL_VAR(ctx);
	uint8_t linortho_seed[MQOM2_PARAM_SEED_SIZE];

	/* Compute Psi(seed) once and for all */
	LinOrtho(seed, linortho_seed);

	start_block = (start_nbytes / MQOM2_PARAM_SEED_SIZE);
	end_block   = ((start_nbytes + nbytes) / MQOM2_PARAM_SEED_SIZE);
	treated_bytes = 0;
	for (i = start_block; i < end_block; i++) {
		/* Deal with the beginning leftover */
		if ((i == start_block) && (start_nbytes % MQOM2_PARAM_SEED_SIZE)) {
			uint32_t to_copy = MQOM2_PARAM_SEED_SIZE - (start_nbytes % MQOM2_PARAM_SEED_SIZE);
			/* Directly use the leftover from a previous computation */
			memcpy(&out_data[treated_bytes], &in_out_leftover[start_nbytes % MQOM2_PARAM_SEED_SIZE], to_copy);
			treated_bytes += to_copy;
		} else {
			ret = prg_key_sched_pub(salt, &e, i, &ctx, cache);
			ERR(ret, err);
			ret = enc_encrypt_pub(&ctx, seed, &out_data[treated_bytes]);
			ERR(ret, err);
			xor_blocks(&out_data[treated_bytes], linortho_seed, &out_data[treated_bytes]);
			treated_bytes += MQOM2_PARAM_SEED_SIZE;
		}
	}
	/* Deal with the possible leftover incomplete block at the end */
	if ((nbytes - treated_bytes) % MQOM2_PARAM_SEED_SIZE) {
		/* Do we have to recompute the leftover? */
		ret = prg_key_sched_pub(salt, &e, i, &ctx, cache);
		ERR(ret, err);
		ret = enc_encrypt_pub(&ctx, seed, in_out_leftover);
		ERR(ret, err);
		xor_blocks(in_out_leftover, linortho_seed, in_out_leftover);
		if (treated_bytes == 0) {
			memcpy(&out_data[0], &in_out_leftover[start_nbytes % MQOM2_PARAM_SEED_SIZE], nbytes % MQOM2_PARAM_SEED_SIZE);
		} else {
			memcpy(&out_data[treated_bytes], in_out_leftover, (nbytes - treated_bytes) % MQOM2_PARAM_SEED_SIZE);
		}
	}

	ret = 0;
err:
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		enc_clean_ctx_pub(&ctx);
	}
	return ret;
}


/* ================================================================= */
#if defined(PRG_ONE_RIJNDAEL_CTX)
int PRG(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t *out_data, prg_key_sched_cache *cache) {
	int ret = -1;
	uint32_t i;
	enc_ctx DECL_VAR(ctx);
	uint8_t linortho_seed[MQOM2_PARAM_SEED_SIZE];

	/* Compute Psi(seed) once and for all */
	LinOrtho(seed, linortho_seed);

	for (i = 0; i < (nbytes / MQOM2_PARAM_SEED_SIZE); i++) {
		/* Key schedule */
		ret = prg_key_sched(salt, &e, i, &ctx, cache);
		ERR(ret, err);
		ret = enc_encrypt(&ctx, seed, &out_data[MQOM2_PARAM_SEED_SIZE * i]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * i], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * i]);
	}
	/* Deal with the possible leftover incomplete block */
	if (nbytes % MQOM2_PARAM_SEED_SIZE) {
		uint8_t leftover[MQOM2_PARAM_SEED_SIZE];
		ret = prg_key_sched(salt, &e, i, &ctx, cache);
		ERR(ret, err);
		ret = enc_encrypt(&ctx, seed, leftover);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(leftover, linortho_seed, leftover);
		memcpy(&out_data[MQOM2_PARAM_SEED_SIZE * i], leftover, nbytes % MQOM2_PARAM_SEED_SIZE);
	}

	ret = 0;
err:
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		enc_clean_ctx(&ctx);
	}
	return ret;
}
int PRG_pub(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t *out_data, prg_key_sched_cache_pub *cache) {
	int ret = -1;
	uint32_t i;
	enc_ctx_pub DECL_VAR(ctx);
	uint8_t linortho_seed[MQOM2_PARAM_SEED_SIZE];

	/* Compute Psi(seed) once and for all */
	LinOrtho(seed, linortho_seed);

	for (i = 0; i < (nbytes / MQOM2_PARAM_SEED_SIZE); i++) {
		/* Key schedule */
		ret = prg_key_sched_pub(salt, &e, i, &ctx, cache);
		ERR(ret, err);
		ret = enc_encrypt_pub(&ctx, seed, &out_data[MQOM2_PARAM_SEED_SIZE * i]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * i], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * i]);
	}
	/* Deal with the possible leftover incomplete block */
	if (nbytes % MQOM2_PARAM_SEED_SIZE) {
		uint8_t leftover[MQOM2_PARAM_SEED_SIZE];
		ret = prg_key_sched_pub(salt, &e, i, &ctx, cache);
		ERR(ret, err);
		ret = enc_encrypt_pub(&ctx, seed, leftover);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(leftover, linortho_seed, leftover);
		memcpy(&out_data[MQOM2_PARAM_SEED_SIZE * i], leftover, nbytes % MQOM2_PARAM_SEED_SIZE);
	}

	ret = 0;
err:
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		enc_clean_ctx_pub(&ctx);
	}
	return ret;
}

#else
/* ================================================================= */
int PRG(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t *out_data, prg_key_sched_cache *cache) {
	int ret = -1;
	uint32_t i, filled_blocks;
	enc_ctx DECL_VAR(ctx1), DECL_VAR(ctx2), DECL_VAR(ctx3), DECL_VAR(ctx4);
#ifdef USE_ENC_X8
	enc_ctx DECL_VAR(ctx5), DECL_VAR(ctx6), DECL_VAR(ctx7), DECL_VAR(ctx8);
	uint32_t num_blocks = 8;
#else
	uint32_t num_blocks = 4;
#endif
	uint8_t linortho_seed[MQOM2_PARAM_SEED_SIZE];

	/* Compute Psi(seed) once and for all */
	LinOrtho(seed, linortho_seed);

	/* Depending on the number of blocks, exploit the 2x, 4x or 8x variants */
	filled_blocks = 0;
	for (i = 0; i < (nbytes / (num_blocks * MQOM2_PARAM_SEED_SIZE)); i++) {
		/* Key schedule */
		ret = prg_key_sched(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 3, &ctx4, cache);
		ERR(ret, err);
#ifdef USE_ENC_X8
		ret = prg_key_sched(salt, &e, filled_blocks + 4, &ctx5, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 5, &ctx6, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 6, &ctx7, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 7, &ctx8, cache);
		ERR(ret, err);
#endif
		/* Encryption */
#ifdef USE_ENC_X8
		ret = enc_encrypt_x8(&ctx1, &ctx2, &ctx3, &ctx4, &ctx5, &ctx6, &ctx7, &ctx8, seed, seed, seed, seed, seed, seed, seed, seed,
		                     &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                     &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)],
		                     &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)],
		                     &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 6)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 7)]);
		ERR(ret, err);
#else
		ret = enc_encrypt_x4(&ctx1, &ctx2, &ctx3, &ctx4, seed, seed, seed, seed,
		                     &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                     &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		ERR(ret, err);
#endif
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
#ifdef USE_ENC_X8
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 6)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 6)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 7)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 7)]);
#endif
		filled_blocks += num_blocks;
	}
	switch ((nbytes - (num_blocks * MQOM2_PARAM_SEED_SIZE * i)) / MQOM2_PARAM_SEED_SIZE) {
	case 0: {
		/* No remaining block */
		break;
	}
	case 1: {
		/* One remaining block: 1x */
		ret = prg_key_sched(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = enc_encrypt(&ctx1, seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		filled_blocks += 1;
		break;
	}
	case 2: {
		/* Two remaining blocks: 2x */
		ret = prg_key_sched(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = enc_encrypt_x2(&ctx1, &ctx2, seed, seed,
		                     &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		filled_blocks += 2;
		break;
	}
	case 3: {
		/* Three remaining blocks: 2x and then 1x */
		ret = prg_key_sched(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = enc_encrypt_x2(&ctx1, &ctx2, seed, seed,
		                     &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		ERR(ret, err);
		ret = enc_encrypt(&ctx3, seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		filled_blocks += 3;
		break;
	}
#ifdef USE_ENC_X8
	case 4: {
		/* Four remaining blocks: 4x */
		ret = prg_key_sched(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 3, &ctx4, cache);
		ERR(ret, err);
		ret = enc_encrypt_x4(&ctx1, &ctx2, &ctx3, &ctx4, seed, seed, seed, seed,
		                     &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                     &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		ERR(ret, err);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		filled_blocks += 4;
		break;
	}
	case 5: {
		/* Five remaining blocks: 4x and 1x */
		ret = prg_key_sched(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 3, &ctx4, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 4, &ctx5, cache);
		ERR(ret, err);
		ret = enc_encrypt_x4(&ctx1, &ctx2, &ctx3, &ctx4, seed, seed, seed, seed,
		                     &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                     &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		ERR(ret, err);
		ret = enc_encrypt(&ctx5, seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)]);
		ERR(ret, err);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)]);
		filled_blocks += 5;
		break;
	}
	case 6: {
		/* Six remaining blocks: 4x and 2x */
		ret = prg_key_sched(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 3, &ctx4, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 4, &ctx5, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 5, &ctx6, cache);
		ERR(ret, err);
		ret = enc_encrypt_x4(&ctx1, &ctx2, &ctx3, &ctx4, seed, seed, seed, seed,
		                     &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                     &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		ERR(ret, err);
		ret = enc_encrypt_x2(&ctx5, &ctx6, seed, seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)]);
		ERR(ret, err);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)]);
		filled_blocks += 6;
		break;
	}
	case 7: {
		/* Seven remaining blocks: 4x, 2x and 1x */
		ret = prg_key_sched(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 3, &ctx4, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 4, &ctx5, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 5, &ctx6, cache);
		ERR(ret, err);
		ret = prg_key_sched(salt, &e, filled_blocks + 6, &ctx7, cache);
		ERR(ret, err);
		ret = enc_encrypt_x4(&ctx1, &ctx2, &ctx3, &ctx4, seed, seed, seed, seed,
		                     &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                     &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		ERR(ret, err);
		ret = enc_encrypt_x2(&ctx5, &ctx6, seed, seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)]);
		ERR(ret, err);
		ret = enc_encrypt(&ctx7, seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 6)]);
		ERR(ret, err);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 6)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 6)]);
		filled_blocks += 7;
		break;
	}
#endif
	default: {
		ret = -1;
		goto err;
	}
	}
	/* Deal with the possible leftover incomplete block */
	if (nbytes % MQOM2_PARAM_SEED_SIZE) {
		uint8_t leftover[MQOM2_PARAM_SEED_SIZE];
		ret = prg_key_sched(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = enc_encrypt(&ctx1, seed, leftover);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(leftover, linortho_seed, leftover);
		memcpy(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], leftover, nbytes % MQOM2_PARAM_SEED_SIZE);
	}

	ret = 0;
err:
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		enc_clean_ctx(&ctx1);
		enc_clean_ctx(&ctx2);
		enc_clean_ctx(&ctx3);
		enc_clean_ctx(&ctx4);
#ifdef USE_ENC_X8
		enc_clean_ctx(&ctx5);
		enc_clean_ctx(&ctx6);
		enc_clean_ctx(&ctx7);
		enc_clean_ctx(&ctx8);
#endif
	}
	return ret;
}
int PRG_pub(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t *out_data, prg_key_sched_cache_pub *cache) {
	int ret = -1;
	uint32_t i, filled_blocks;
	enc_ctx_pub DECL_VAR(ctx1), DECL_VAR(ctx2), DECL_VAR(ctx3), DECL_VAR(ctx4);
#ifdef USE_ENC_X8
	enc_ctx_pub DECL_VAR(ctx5), DECL_VAR(ctx6), DECL_VAR(ctx7), DECL_VAR(ctx8);
	uint32_t num_blocks = 8;
#else
	uint32_t num_blocks = 4;
#endif
	uint8_t linortho_seed[MQOM2_PARAM_SEED_SIZE];

	/* Compute Psi(seed) once and for all */
	LinOrtho(seed, linortho_seed);

	/* Depending on the number of blocks, exploit the 2x, 4x or 8x variants */
	filled_blocks = 0;
	for (i = 0; i < (nbytes / (num_blocks * MQOM2_PARAM_SEED_SIZE)); i++) {
		/* Key schedule */
		ret = prg_key_sched_pub(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 3, &ctx4, cache);
		ERR(ret, err);
#ifdef USE_ENC_X8
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 4, &ctx5, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 5, &ctx6, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 6, &ctx7, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 7, &ctx8, cache);
		ERR(ret, err);
#endif
		/* Encryption */
#ifdef USE_ENC_X8
		ret = enc_encrypt_x8_pub(&ctx1, &ctx2, &ctx3, &ctx4, &ctx5, &ctx6, &ctx7, &ctx8, seed, seed, seed, seed, seed, seed, seed, seed,
		                         &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                         &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)],
		                         &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)],
		                         &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 6)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 7)]);
		ERR(ret, err);
#else
		ret = enc_encrypt_x4_pub(&ctx1, &ctx2, &ctx3, &ctx4, seed, seed, seed, seed,
		                         &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                         &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		ERR(ret, err);
#endif
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
#ifdef USE_ENC_X8
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 6)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 6)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 7)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 7)]);
#endif
		filled_blocks += num_blocks;
	}
	switch ((nbytes - (num_blocks * MQOM2_PARAM_SEED_SIZE * i)) / MQOM2_PARAM_SEED_SIZE) {
	case 0: {
		/* No remaining block */
		break;
	}
	case 1: {
		/* One remaining block: 1x */
		ret = prg_key_sched_pub(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = enc_encrypt_pub(&ctx1, seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		filled_blocks += 1;
		break;
	}
	case 2: {
		/* Two remaining blocks: 2x */
		ret = prg_key_sched_pub(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = enc_encrypt_x2_pub(&ctx1, &ctx2, seed, seed,
		                         &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		filled_blocks += 2;
		break;
	}
	case 3: {
		/* Three remaining blocks: 2x and then 1x */
		ret = prg_key_sched_pub(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = enc_encrypt_x2_pub(&ctx1, &ctx2, seed, seed,
		                         &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		ERR(ret, err);
		ret = enc_encrypt_pub(&ctx3, seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		filled_blocks += 3;
		break;
	}
#ifdef USE_ENC_X8
	case 4: {
		/* Four remaining blocks: 4x */
		ret = prg_key_sched_pub(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 3, &ctx4, cache);
		ERR(ret, err);
		ret = enc_encrypt_x4_pub(&ctx1, &ctx2, &ctx3, &ctx4, seed, seed, seed, seed,
		                         &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                         &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		ERR(ret, err);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		filled_blocks += 4;
		break;
	}
	case 5: {
		/* Five remaining blocks: 4x and 1x */
		ret = prg_key_sched_pub(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 3, &ctx4, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 4, &ctx5, cache);
		ERR(ret, err);
		ret = enc_encrypt_x4_pub(&ctx1, &ctx2, &ctx3, &ctx4, seed, seed, seed, seed,
		                         &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                         &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		ERR(ret, err);
		ret = enc_encrypt_pub(&ctx5, seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)]);
		ERR(ret, err);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)]);
		filled_blocks += 5;
		break;
	}
	case 6: {
		/* Six remaining blocks: 4x and 2x */
		ret = prg_key_sched_pub(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 3, &ctx4, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 4, &ctx5, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 5, &ctx6, cache);
		ERR(ret, err);
		ret = enc_encrypt_x4_pub(&ctx1, &ctx2, &ctx3, &ctx4, seed, seed, seed, seed,
		                         &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                         &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		ERR(ret, err);
		ret = enc_encrypt_x2_pub(&ctx5, &ctx6, seed, seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)]);
		ERR(ret, err);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)]);
		filled_blocks += 6;
		break;
	}
	case 7: {
		/* Seven remaining blocks: 4x, 2x and 1x */
		ret = prg_key_sched_pub(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 3, &ctx4, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 4, &ctx5, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 5, &ctx6, cache);
		ERR(ret, err);
		ret = prg_key_sched_pub(salt, &e, filled_blocks + 6, &ctx7, cache);
		ERR(ret, err);
		ret = enc_encrypt_x4_pub(&ctx1, &ctx2, &ctx3, &ctx4, seed, seed, seed, seed,
		                         &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                         &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		ERR(ret, err);
		ret = enc_encrypt_x2_pub(&ctx5, &ctx6, seed, seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)], &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)]);
		ERR(ret, err);
		ret = enc_encrypt_pub(&ctx7, seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 6)]);
		ERR(ret, err);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 4)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 5)]);
		xor_blocks(&out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 6)], linortho_seed, &out_data[MQOM2_PARAM_SEED_SIZE * (filled_blocks + 6)]);
		filled_blocks += 7;
		break;
	}
#endif
	default: {
		ret = -1;
		goto err;
	}
	}
	/* Deal with the possible leftover incomplete block */
	if (nbytes % MQOM2_PARAM_SEED_SIZE) {
		uint8_t leftover[MQOM2_PARAM_SEED_SIZE];
		ret = prg_key_sched_pub(salt, &e, filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = enc_encrypt_pub(&ctx1, seed, leftover);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(leftover, linortho_seed, leftover);
		memcpy(&out_data[MQOM2_PARAM_SEED_SIZE * filled_blocks], leftover, nbytes % MQOM2_PARAM_SEED_SIZE);
	}

	ret = 0;
err:
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		enc_clean_ctx_pub(&ctx1);
		enc_clean_ctx_pub(&ctx2);
		enc_clean_ctx_pub(&ctx3);
		enc_clean_ctx_pub(&ctx4);
#ifdef USE_ENC_X8
		enc_clean_ctx_pub(&ctx5);
		enc_clean_ctx_pub(&ctx6);
		enc_clean_ctx_pub(&ctx7);
		enc_clean_ctx_pub(&ctx8);
#endif
	}
	return ret;
}

#endif /* PRG_ONE_RIJNDAEL_CTX */


int PRG_x2(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e, const uint8_t seed[2][MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[2], prg_key_sched_cache_x2* cache, uint8_t nb_contexts) {
	int ret = -1;
	uint32_t i;
	enc_ctx_x2 DECL_VAR(ctx_x2);
	uint8_t linortho_seed[2][MQOM2_PARAM_SEED_SIZE];
	uint32_t es[2];

	if(nb_contexts == 1) {
		es[0] = e[0];
		es[1] = e[0];
	} else if (nb_contexts == 2) {
		es[0] = e[0];
		es[1] = e[1];
	} else {
		goto err;
	}

	/* Compute Psi(seed) once and for all */
	LinOrtho(seed[0], linortho_seed[0]);
	LinOrtho(seed[1], linortho_seed[1]);

	for (i = 0; i < (nbytes / MQOM2_PARAM_SEED_SIZE); i++) {
		/* Key schedule */
		ret = prg_key_sched_x2(salt, es, i, &ctx_x2, cache);
		ERR(ret, err);
		/* Encryption */
		ret = enc_encrypt_x2_x2(&ctx_x2, seed[0], seed[1],
		                     &out_data[0][MQOM2_PARAM_SEED_SIZE * i], &out_data[1][MQOM2_PARAM_SEED_SIZE * i]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[0][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[0], &out_data[0][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[1][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[1], &out_data[1][MQOM2_PARAM_SEED_SIZE * i]);
	}
	/* Deal with the possible leftover incomplete block */
	uint32_t remaining_bytes = nbytes % MQOM2_PARAM_SEED_SIZE;
	if (remaining_bytes) {
		uint8_t leftover[2][MQOM2_PARAM_SEED_SIZE];
		ret = prg_key_sched_x2(salt, es, i, &ctx_x2, cache);
		ERR(ret, err);
		ret = enc_encrypt_x2_x2(&ctx_x2, seed[0], seed[1],
		                     leftover[0], leftover[1]);
		ERR(ret, err);
		xor_blocks(leftover[0], linortho_seed[0], leftover[0]);
		xor_blocks(leftover[1], linortho_seed[1], leftover[1]);
		memcpy(&out_data[0][MQOM2_PARAM_SEED_SIZE * i], leftover[0], remaining_bytes);
		memcpy(&out_data[1][MQOM2_PARAM_SEED_SIZE * i], leftover[1], remaining_bytes);
	}

	ret = 0;
err:
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		enc_clean_ctx_x2(&ctx_x2);
	}
	return ret;
}
int PRG_x2_pub(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e, const uint8_t seed[2][MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[2], prg_key_sched_cache_pub_x2* cache, uint8_t nb_contexts) {
	int ret = -1;
	uint32_t i;
	enc_ctx_pub_x2 DECL_VAR(ctx_x2);
	uint8_t linortho_seed[2][MQOM2_PARAM_SEED_SIZE];
	uint32_t es[2];

	if(nb_contexts == 1) {
		es[0] = e[0];
		es[1] = e[0];
	} else if (nb_contexts == 2) {
		es[0] = e[0];
		es[1] = e[1];
	} else {
		goto err;
	}

	/* Compute Psi(seed) once and for all */
	LinOrtho(seed[0], linortho_seed[0]);
	LinOrtho(seed[1], linortho_seed[1]);

	for (i = 0; i < (nbytes / MQOM2_PARAM_SEED_SIZE); i++) {
		/* Key schedule */
		ret = prg_key_sched_pub_x2(salt, es, i, &ctx_x2, cache);
		ERR(ret, err);
		/* Encryption */
		ret = enc_encrypt_x2_pub_x2(&ctx_x2, seed[0], seed[1],
		                         &out_data[0][MQOM2_PARAM_SEED_SIZE * i], &out_data[1][MQOM2_PARAM_SEED_SIZE * i]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[0][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[0], &out_data[0][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[1][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[1], &out_data[1][MQOM2_PARAM_SEED_SIZE * i]);
	}
	/* Deal with the possible leftover incomplete block */
	uint32_t remaining_bytes = nbytes % MQOM2_PARAM_SEED_SIZE;
	if (remaining_bytes) {
		uint8_t leftover[2][MQOM2_PARAM_SEED_SIZE];
		ret = prg_key_sched_pub_x2(salt, es, i, &ctx_x2, cache);
		ERR(ret, err);
		ret = enc_encrypt_x2_pub_x2(&ctx_x2, seed[0], seed[1],
		                         leftover[0], leftover[1]);
		ERR(ret, err);
		xor_blocks(leftover[0], linortho_seed[0], leftover[0]);
		xor_blocks(leftover[1], linortho_seed[1], leftover[1]);
		memcpy(&out_data[0][MQOM2_PARAM_SEED_SIZE * i], leftover[0], remaining_bytes);
		memcpy(&out_data[1][MQOM2_PARAM_SEED_SIZE * i], leftover[1], remaining_bytes);
	}

	ret = 0;
err:
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		enc_clean_ctx_pub_x2(&ctx_x2);
	}
	return ret;
}

int PRG_x4(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e, const uint8_t seed[4][MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[4], prg_key_sched_cache_x4* cache, uint8_t nb_contexts) {
	int ret = -1;
	uint32_t i;
	enc_ctx_x4 DECL_VAR(ctx_x4);
	uint8_t linortho_seed[4][MQOM2_PARAM_SEED_SIZE];
	uint32_t es[4];

	if(nb_contexts == 1) {
		es[0] = e[0];
		es[1] = e[0];
		es[2] = e[0];
		es[3] = e[0];
	} else if (nb_contexts == 4) {
		es[0] = e[0];
		es[1] = e[1];
		es[2] = e[2];
		es[3] = e[3];
	} else {
		goto err;
	}

	/* Compute Psi(seed) once and for all */
	LinOrtho(seed[0], linortho_seed[0]);
	LinOrtho(seed[1], linortho_seed[1]);
	LinOrtho(seed[2], linortho_seed[2]);
	LinOrtho(seed[3], linortho_seed[3]);

	for (i = 0; i < (nbytes / MQOM2_PARAM_SEED_SIZE); i++) {
		/* Key schedule */
		ret = prg_key_sched_x4(salt, es, i, &ctx_x4, cache);
		ERR(ret, err);
		/* Encryption */
		ret = enc_encrypt_x4_x4(&ctx_x4, seed[0], seed[1], seed[2], seed[3],
		                     &out_data[0][MQOM2_PARAM_SEED_SIZE * i], &out_data[1][MQOM2_PARAM_SEED_SIZE * i],
		                     &out_data[2][MQOM2_PARAM_SEED_SIZE * i], &out_data[3][MQOM2_PARAM_SEED_SIZE * i]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[0][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[0], &out_data[0][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[1][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[1], &out_data[1][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[2][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[2], &out_data[2][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[3][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[3], &out_data[3][MQOM2_PARAM_SEED_SIZE * i]);
	}
	/* Deal with the possible leftover incomplete block */
	uint32_t remaining_bytes = nbytes % MQOM2_PARAM_SEED_SIZE;
	if (remaining_bytes) {
		uint8_t leftover[4][MQOM2_PARAM_SEED_SIZE];
		ret = prg_key_sched_x4(salt, es, i, &ctx_x4, cache);
		ERR(ret, err);
		ret = enc_encrypt_x4_x4(&ctx_x4, seed[0], seed[1], seed[2], seed[3],
		                     leftover[0], leftover[1], leftover[2], leftover[3]);
		ERR(ret, err);
		xor_blocks(leftover[0], linortho_seed[0], leftover[0]);
		xor_blocks(leftover[1], linortho_seed[1], leftover[1]);
		xor_blocks(leftover[2], linortho_seed[2], leftover[2]);
		xor_blocks(leftover[3], linortho_seed[3], leftover[3]);
		memcpy(&out_data[0][MQOM2_PARAM_SEED_SIZE * i], leftover[0], remaining_bytes);
		memcpy(&out_data[1][MQOM2_PARAM_SEED_SIZE * i], leftover[1], remaining_bytes);
		memcpy(&out_data[2][MQOM2_PARAM_SEED_SIZE * i], leftover[2], remaining_bytes);
		memcpy(&out_data[3][MQOM2_PARAM_SEED_SIZE * i], leftover[3], remaining_bytes);
	}

	ret = 0;
err:
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	*/
	if (cache == NULL) {
		enc_clean_ctx_x4(&ctx_x4);
	}
	return ret;
}
int PRG_x4_pub(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e, const uint8_t seed[4][MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[4], prg_key_sched_cache_pub_x4* cache, uint8_t nb_contexts) {
	int ret = -1;
	uint32_t i;
	enc_ctx_pub_x4 DECL_VAR(ctx_x4);
	uint8_t linortho_seed[4][MQOM2_PARAM_SEED_SIZE];
	uint32_t es[4];

	if(nb_contexts == 1) {
		es[0] = e[0];
		es[1] = e[0];
		es[2] = e[0];
		es[3] = e[0];
	} else if (nb_contexts == 4) {
		es[0] = e[0];
		es[1] = e[1];
		es[2] = e[2];
		es[3] = e[3];
	} else {
		goto err;
	}

	/* Compute Psi(seed) once and for all */
	LinOrtho(seed[0], linortho_seed[0]);
	LinOrtho(seed[1], linortho_seed[1]);
	LinOrtho(seed[2], linortho_seed[2]);
	LinOrtho(seed[3], linortho_seed[3]);

	for (i = 0; i < (nbytes / MQOM2_PARAM_SEED_SIZE); i++) {
		/* Key schedule */
		ret = prg_key_sched_pub_x4(salt, es, i, &ctx_x4, cache);
		ERR(ret, err);
		/* Encryption */
		ret = enc_encrypt_x4_pub_x4(&ctx_x4, seed[0], seed[1], seed[2], seed[3],
		                         &out_data[0][MQOM2_PARAM_SEED_SIZE * i], &out_data[1][MQOM2_PARAM_SEED_SIZE * i],
		                         &out_data[2][MQOM2_PARAM_SEED_SIZE * i], &out_data[3][MQOM2_PARAM_SEED_SIZE * i]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[0][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[0], &out_data[0][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[1][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[1], &out_data[1][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[2][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[2], &out_data[2][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[3][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[3], &out_data[3][MQOM2_PARAM_SEED_SIZE * i]);
	}
	/* Deal with the possible leftover incomplete block */
	uint32_t remaining_bytes = nbytes % MQOM2_PARAM_SEED_SIZE;
	if (remaining_bytes) {
		uint8_t leftover[4][MQOM2_PARAM_SEED_SIZE];
		ret = prg_key_sched_pub_x4(salt, es, i, &ctx_x4, cache);
		ERR(ret, err);
		ret = enc_encrypt_x4_pub_x4(&ctx_x4, seed[0], seed[1], seed[2], seed[3],
		                         leftover[0], leftover[1], leftover[2], leftover[3]);
		ERR(ret, err);
		xor_blocks(leftover[0], linortho_seed[0], leftover[0]);
		xor_blocks(leftover[1], linortho_seed[1], leftover[1]);
		xor_blocks(leftover[2], linortho_seed[2], leftover[2]);
		xor_blocks(leftover[3], linortho_seed[3], leftover[3]);
		memcpy(&out_data[0][MQOM2_PARAM_SEED_SIZE * i], leftover[0], remaining_bytes);
		memcpy(&out_data[1][MQOM2_PARAM_SEED_SIZE * i], leftover[1], remaining_bytes);
		memcpy(&out_data[2][MQOM2_PARAM_SEED_SIZE * i], leftover[2], remaining_bytes);
		memcpy(&out_data[3][MQOM2_PARAM_SEED_SIZE * i], leftover[3], remaining_bytes);
	}

	ret = 0;
err:
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		enc_clean_ctx_pub_x4(&ctx_x4);
	}
	return ret;
}

int PRG_x8(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t *e, const uint8_t seed[8][MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[8], prg_key_sched_cache_x8* cache, uint8_t nb_contexts) {
	int ret = -1;
	uint32_t i;
	enc_ctx_x8 DECL_VAR(ctx_x8);
	uint8_t linortho_seed[8][MQOM2_PARAM_SEED_SIZE];
	uint32_t es[8];

	if(nb_contexts == 1) {
		es[0] = e[0];
		es[1] = e[0];
		es[2] = e[0];
		es[3] = e[0];
		es[4] = e[0];
		es[5] = e[0];
		es[6] = e[0];
		es[7] = e[0];
	} else if (nb_contexts == 8) {
		es[0] = e[0];
		es[1] = e[1];
		es[2] = e[2];
		es[3] = e[3];
		es[4] = e[4];
		es[5] = e[5];
		es[6] = e[6];
		es[7] = e[7];
	} else {
		goto err;
	}

	/* Compute Psi(seed) once and for all */
	LinOrtho(seed[0], linortho_seed[0]);
	LinOrtho(seed[1], linortho_seed[1]);
	LinOrtho(seed[2], linortho_seed[2]);
	LinOrtho(seed[3], linortho_seed[3]);
	LinOrtho(seed[4], linortho_seed[4]);
	LinOrtho(seed[5], linortho_seed[5]);
	LinOrtho(seed[6], linortho_seed[6]);
	LinOrtho(seed[7], linortho_seed[7]);

	for (i = 0; i < (nbytes / MQOM2_PARAM_SEED_SIZE); i++) {
		/* Key schedule */
		ret = prg_key_sched_x8(salt, es, i, &ctx_x8, cache);
		ERR(ret, err);
		/* Encryption */
		ret = enc_encrypt_x8_x8(&ctx_x8,
		                     seed[0], seed[1], seed[2], seed[3], seed[4], seed[5], seed[6], seed[7],
		                     &out_data[0][MQOM2_PARAM_SEED_SIZE * i], &out_data[1][MQOM2_PARAM_SEED_SIZE * i],
		                     &out_data[2][MQOM2_PARAM_SEED_SIZE * i], &out_data[3][MQOM2_PARAM_SEED_SIZE * i],
		                     &out_data[4][MQOM2_PARAM_SEED_SIZE * i], &out_data[5][MQOM2_PARAM_SEED_SIZE * i],
		                     &out_data[6][MQOM2_PARAM_SEED_SIZE * i], &out_data[7][MQOM2_PARAM_SEED_SIZE * i]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[0][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[0], &out_data[0][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[1][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[1], &out_data[1][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[2][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[2], &out_data[2][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[3][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[3], &out_data[3][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[4][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[4], &out_data[4][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[5][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[5], &out_data[5][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[6][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[6], &out_data[6][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[7][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[7], &out_data[7][MQOM2_PARAM_SEED_SIZE * i]);
	}
	/* Deal with the possible leftover incomplete block */
	uint32_t remaining_bytes = nbytes % MQOM2_PARAM_SEED_SIZE;
	if (remaining_bytes) {
		uint8_t leftover[8][MQOM2_PARAM_SEED_SIZE];
		ret = prg_key_sched_x8(salt, es, i, &ctx_x8, cache);
		ERR(ret, err);
		ret = enc_encrypt_x8_x8(&ctx_x8,
		                     seed[0], seed[1], seed[2], seed[3], seed[4], seed[5], seed[6], seed[7],
		                     leftover[0], leftover[1], leftover[2], leftover[3],
		                     leftover[4], leftover[5], leftover[6], leftover[7]);
		ERR(ret, err);
		xor_blocks(leftover[0], linortho_seed[0], leftover[0]);
		xor_blocks(leftover[1], linortho_seed[1], leftover[1]);
		xor_blocks(leftover[2], linortho_seed[2], leftover[2]);
		xor_blocks(leftover[3], linortho_seed[3], leftover[3]);
		xor_blocks(leftover[4], linortho_seed[4], leftover[4]);
		xor_blocks(leftover[5], linortho_seed[5], leftover[5]);
		xor_blocks(leftover[6], linortho_seed[6], leftover[6]);
		xor_blocks(leftover[7], linortho_seed[7], leftover[7]);
		memcpy(&out_data[0][MQOM2_PARAM_SEED_SIZE * i], leftover[0], remaining_bytes);
		memcpy(&out_data[1][MQOM2_PARAM_SEED_SIZE * i], leftover[1], remaining_bytes);
		memcpy(&out_data[2][MQOM2_PARAM_SEED_SIZE * i], leftover[2], remaining_bytes);
		memcpy(&out_data[3][MQOM2_PARAM_SEED_SIZE * i], leftover[3], remaining_bytes);
		memcpy(&out_data[4][MQOM2_PARAM_SEED_SIZE * i], leftover[4], remaining_bytes);
		memcpy(&out_data[5][MQOM2_PARAM_SEED_SIZE * i], leftover[5], remaining_bytes);
		memcpy(&out_data[6][MQOM2_PARAM_SEED_SIZE * i], leftover[6], remaining_bytes);
		memcpy(&out_data[7][MQOM2_PARAM_SEED_SIZE * i], leftover[7], remaining_bytes);
	}

	ret = 0;
err:
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		enc_clean_ctx_x8(&ctx_x8);
	}
	return ret;
}
int PRG_x8_pub(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t *e, const uint8_t seed[8][MQOM2_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t* out_data[8], prg_key_sched_cache_pub_x8* cache, uint8_t nb_contexts) {
	int ret = -1;
	uint32_t i;
	enc_ctx_pub_x8 DECL_VAR(ctx_x8);
	uint8_t linortho_seed[8][MQOM2_PARAM_SEED_SIZE];
	uint32_t es[8];

	if(nb_contexts == 1) {
		es[0] = e[0];
		es[1] = e[0];
		es[2] = e[0];
		es[3] = e[0];
		es[4] = e[0];
		es[5] = e[0];
		es[6] = e[0];
		es[7] = e[0];
	} else if (nb_contexts == 8) {
		es[0] = e[0];
		es[1] = e[1];
		es[2] = e[2];
		es[3] = e[3];
		es[4] = e[4];
		es[5] = e[5];
		es[6] = e[6];
		es[7] = e[7];
	} else {
		goto err;
	}

	/* Compute Psi(seed) once and for all */
	LinOrtho(seed[0], linortho_seed[0]);
	LinOrtho(seed[1], linortho_seed[1]);
	LinOrtho(seed[2], linortho_seed[2]);
	LinOrtho(seed[3], linortho_seed[3]);
	LinOrtho(seed[4], linortho_seed[4]);
	LinOrtho(seed[5], linortho_seed[5]);
	LinOrtho(seed[6], linortho_seed[6]);
	LinOrtho(seed[7], linortho_seed[7]);

	for (i = 0; i < (nbytes / MQOM2_PARAM_SEED_SIZE); i++) {
		/* Key schedule */
		ret = prg_key_sched_pub_x8(salt, es, i, &ctx_x8, cache);
		ERR(ret, err);
		/* Encryption */
		ret = enc_encrypt_x8_pub_x8(&ctx_x8,
		                         seed[0], seed[1], seed[2], seed[3], seed[4], seed[5], seed[6], seed[7],
		                         &out_data[0][MQOM2_PARAM_SEED_SIZE * i], &out_data[1][MQOM2_PARAM_SEED_SIZE * i],
		                         &out_data[2][MQOM2_PARAM_SEED_SIZE * i], &out_data[3][MQOM2_PARAM_SEED_SIZE * i],
		                         &out_data[4][MQOM2_PARAM_SEED_SIZE * i], &out_data[5][MQOM2_PARAM_SEED_SIZE * i],
		                         &out_data[6][MQOM2_PARAM_SEED_SIZE * i], &out_data[7][MQOM2_PARAM_SEED_SIZE * i]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[0][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[0], &out_data[0][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[1][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[1], &out_data[1][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[2][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[2], &out_data[2][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[3][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[3], &out_data[3][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[4][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[4], &out_data[4][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[5][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[5], &out_data[5][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[6][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[6], &out_data[6][MQOM2_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[7][MQOM2_PARAM_SEED_SIZE * i], linortho_seed[7], &out_data[7][MQOM2_PARAM_SEED_SIZE * i]);
	}
	/* Deal with the possible leftover incomplete block */
	uint32_t remaining_bytes = nbytes % MQOM2_PARAM_SEED_SIZE;
	if (remaining_bytes) {
		uint8_t leftover[8][MQOM2_PARAM_SEED_SIZE];
		ret = prg_key_sched_pub_x8(salt, es, i, &ctx_x8, cache);
		ERR(ret, err);
		ret = enc_encrypt_x8_pub_x8(&ctx_x8,
		                         seed[0], seed[1], seed[2], seed[3], seed[4], seed[5], seed[6], seed[7],
		                         leftover[0], leftover[1], leftover[2], leftover[3],
		                         leftover[4], leftover[5], leftover[6], leftover[7]);
		ERR(ret, err);
		xor_blocks(leftover[0], linortho_seed[0], leftover[0]);
		xor_blocks(leftover[1], linortho_seed[1], leftover[1]);
		xor_blocks(leftover[2], linortho_seed[2], leftover[2]);
		xor_blocks(leftover[3], linortho_seed[3], leftover[3]);
		xor_blocks(leftover[4], linortho_seed[4], leftover[4]);
		xor_blocks(leftover[5], linortho_seed[5], leftover[5]);
		xor_blocks(leftover[6], linortho_seed[6], leftover[6]);
		xor_blocks(leftover[7], linortho_seed[7], leftover[7]);
		memcpy(&out_data[0][MQOM2_PARAM_SEED_SIZE * i], leftover[0], remaining_bytes);
		memcpy(&out_data[1][MQOM2_PARAM_SEED_SIZE * i], leftover[1], remaining_bytes);
		memcpy(&out_data[2][MQOM2_PARAM_SEED_SIZE * i], leftover[2], remaining_bytes);
		memcpy(&out_data[3][MQOM2_PARAM_SEED_SIZE * i], leftover[3], remaining_bytes);
		memcpy(&out_data[4][MQOM2_PARAM_SEED_SIZE * i], leftover[4], remaining_bytes);
		memcpy(&out_data[5][MQOM2_PARAM_SEED_SIZE * i], leftover[5], remaining_bytes);
		memcpy(&out_data[6][MQOM2_PARAM_SEED_SIZE * i], leftover[6], remaining_bytes);
		memcpy(&out_data[7][MQOM2_PARAM_SEED_SIZE * i], leftover[7], remaining_bytes);
	}

	ret = 0;
err:
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		enc_clean_ctx_pub_x8(&ctx_x8);
	}
	return ret;
}
