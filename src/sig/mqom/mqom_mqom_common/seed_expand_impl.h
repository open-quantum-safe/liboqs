/* seed_expand_impl.h - X-macro template for the SeedExpand / SeedExpand_pub
 * single-seed function pairs.  Included TWICE from seed_expand.c without an
 * include guard; do not add one.
 *
 *   pass 1 (SEED_EXPAND_PUBLIC undefined): generates SeedExpand_memopt, SeedExpand
 *   pass 2 (#define SEED_EXPAND_PUBLIC):   generates SeedExpand_memopt_pub, SeedExpand_pub
 *
 * Functions with an xN context tuple (SeedExpand_x2, _x4, _x8, _x8_ecb and
 * their _pub counterparts) are structurally different and remain in seed_expand.c.
 */

#ifdef SEED_EXPAND_PUBLIC
#define SE_CTX              enc_ctx_pub
#define SE_CACHE_T          SeedExpand_cache_pub
#define se_key_sched(salt, ep, idx, ctx, cache)              SeedExpand_key_sched_pub(salt, ep, idx, ctx, cache)
#define se_encrypt(ctx, in, out)                             enc_encrypt_pub(ctx, in, out)
#define se_encrypt_x2(c1,c2,i1,i2,o1,o2)                   enc_encrypt_x2_pub(c1,c2,i1,i2,o1,o2)
#define se_encrypt_x4(c1,c2,c3,c4,i1,i2,i3,i4,o1,o2,o3,o4) enc_encrypt_x4_pub(c1,c2,c3,c4,i1,i2,i3,i4,o1,o2,o3,o4)
#define se_encrypt_x8(c1,c2,c3,c4,c5,c6,c7,c8,i1,i2,i3,i4,i5,i6,i7,i8,o1,o2,o3,o4,o5,o6,o7,o8) \
	enc_encrypt_x8_pub(c1,c2,c3,c4,c5,c6,c7,c8,i1,i2,i3,i4,i5,i6,i7,i8,o1,o2,o3,o4,o5,o6,o7,o8)
#define se_clean_ctx(ctx)   enc_clean_ctx_pub(ctx)
#define SeedExpand_memopt_F SeedExpand_memopt_pub
#define SeedExpand_F        SeedExpand_pub
#else
#define SE_CTX              enc_ctx
#define SE_CACHE_T          SeedExpand_cache
#define se_key_sched(salt, ep, idx, ctx, cache)              SeedExpand_key_sched(salt, ep, idx, ctx, cache)
#define se_encrypt(ctx, in, out)                             enc_encrypt(ctx, in, out)
#define se_encrypt_x2(c1,c2,i1,i2,o1,o2)                   enc_encrypt_x2(c1,c2,i1,i2,o1,o2)
#define se_encrypt_x4(c1,c2,c3,c4,i1,i2,i3,i4,o1,o2,o3,o4) enc_encrypt_x4(c1,c2,c3,c4,i1,i2,i3,i4,o1,o2,o3,o4)
#define se_encrypt_x8(c1,c2,c3,c4,c5,c6,c7,c8,i1,i2,i3,i4,i5,i6,i7,i8,o1,o2,o3,o4,o5,o6,o7,o8) \
	enc_encrypt_x8(c1,c2,c3,c4,c5,c6,c7,c8,i1,i2,i3,i4,i5,i6,i7,i8,o1,o2,o3,o4,o5,o6,o7,o8)
#define se_clean_ctx(ctx)   enc_clean_ctx(ctx)
#define SeedExpand_memopt_F SeedExpand_memopt
#define SeedExpand_F        SeedExpand
#endif

/* ================================================================= */
/* SeedExpand_memopt_F                                                */
/* ================================================================= */
int SeedExpand_memopt_F(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM3_PARAM_SEED_SIZE], uint32_t start_nbytes, uint32_t nbytes, uint8_t *out_data, uint8_t in_out_leftover[MQOM3_PARAM_SEED_SIZE], SE_CACHE_T *cache) {
	int ret = -1;
	uint32_t i, start_block, end_block, treated_bytes;
	SE_CTX DECL_VAR(ctx);
	uint8_t xor_mask[MQOM3_PARAM_SEED_SIZE];

#if MQOM3_PARAM_OT_VARIANT != 1
	/* CT: block[0] = Enc(TweakSalt(salt,3,IndexIdentifier(e,0)), seed) as XOR mask */
	ret = se_key_sched(salt, &e, 0, &ctx, cache);
	ERR(ret, err);
	ret = se_encrypt(&ctx, seed, xor_mask);
	ERR(ret, err);
#else
	/* OT: Psi(seed) as XOR mask */
	LinOrtho(seed, xor_mask);
#endif

	start_block   = (start_nbytes / MQOM3_PARAM_SEED_SIZE);
	end_block     = ((start_nbytes + nbytes) / MQOM3_PARAM_SEED_SIZE);
	treated_bytes = 0;
	for (i = start_block; i < end_block; i++) {
		/* Deal with the beginning leftover */
		if ((i == start_block) && (start_nbytes % MQOM3_PARAM_SEED_SIZE)) {
			uint32_t to_copy = MQOM3_PARAM_SEED_SIZE - (start_nbytes % MQOM3_PARAM_SEED_SIZE);
			/* Directly use the leftover from a previous computation */
			memcpy(&out_data[treated_bytes], &in_out_leftover[start_nbytes % MQOM3_PARAM_SEED_SIZE], to_copy);
			treated_bytes += to_copy;
		} else {
#if MQOM3_PARAM_OT_VARIANT != 1
			/* CT: output block i maps to spec block i+1 */
			ret = se_key_sched(salt, &e, i + 1, &ctx, cache);
#else
			ret = se_key_sched(salt, &e, i, &ctx, cache);
#endif
			ERR(ret, err);
			ret = se_encrypt(&ctx, seed, &out_data[treated_bytes]);
			ERR(ret, err);
			xor_blocks(&out_data[treated_bytes], xor_mask, &out_data[treated_bytes]);
			treated_bytes += MQOM3_PARAM_SEED_SIZE;
		}
	}
	/* Deal with the possible leftover incomplete block at the end */
	if ((nbytes - treated_bytes) % MQOM3_PARAM_SEED_SIZE) {
		/* Do we have to recompute the leftover? */
#if MQOM3_PARAM_OT_VARIANT != 1
		ret = se_key_sched(salt, &e, i + 1, &ctx, cache);
#else
		ret = se_key_sched(salt, &e, i, &ctx, cache);
#endif
		ERR(ret, err);
		ret = se_encrypt(&ctx, seed, in_out_leftover);
		ERR(ret, err);
		xor_blocks(in_out_leftover, xor_mask, in_out_leftover);
		if (treated_bytes == 0) {
			memcpy(&out_data[0], &in_out_leftover[start_nbytes % MQOM3_PARAM_SEED_SIZE], nbytes % MQOM3_PARAM_SEED_SIZE);
		} else {
			memcpy(&out_data[treated_bytes], in_out_leftover, (nbytes - treated_bytes) % MQOM3_PARAM_SEED_SIZE);
		}
	}

	ret = 0;
err:
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		se_clean_ctx(&ctx);
	}
#ifndef SEED_EXPAND_PUBLIC
	mqom_cleanse(xor_mask, sizeof(xor_mask));
#endif
	return ret;
}

/* ================================================================= */
/* SeedExpand_F                                                       */
/* ================================================================= */
#if defined(PRG_ONE_RIJNDAEL_CTX)
int SeedExpand_F(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM3_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t *out_data, SE_CACHE_T *cache) {
	int ret = -1;
	uint32_t i;
	SE_CTX DECL_VAR(ctx);
	uint8_t xor_mask[MQOM3_PARAM_SEED_SIZE];
	uint8_t leftover[MQOM3_PARAM_SEED_SIZE];

#if MQOM3_PARAM_OT_VARIANT != 1
	/* CT: block[0] as XOR mask */
	ret = se_key_sched(salt, &e, 0, &ctx, cache);
	ERR(ret, err);
	ret = se_encrypt(&ctx, seed, xor_mask);
	ERR(ret, err);
#else
	LinOrtho(seed, xor_mask);
#endif

	for (i = 0; i < (nbytes / MQOM3_PARAM_SEED_SIZE); i++) {
		/* Key schedule */
#if MQOM3_PARAM_OT_VARIANT != 1
		ret = se_key_sched(salt, &e, i + 1, &ctx, cache);
#else
		ret = se_key_sched(salt, &e, i, &ctx, cache);
#endif
		ERR(ret, err);
		ret = se_encrypt(&ctx, seed, &out_data[MQOM3_PARAM_SEED_SIZE * i]);
		ERR(ret, err);
		/* Xor with mask */
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * i], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * i]);
	}
	/* Deal with the possible leftover incomplete block */
	if (nbytes % MQOM3_PARAM_SEED_SIZE) {
#if MQOM3_PARAM_OT_VARIANT != 1
		ret = se_key_sched(salt, &e, i + 1, &ctx, cache);
#else
		ret = se_key_sched(salt, &e, i, &ctx, cache);
#endif
		ERR(ret, err);
		ret = se_encrypt(&ctx, seed, leftover);
		ERR(ret, err);
		/* Xor with mask */
		xor_blocks(leftover, xor_mask, leftover);
		memcpy(&out_data[MQOM3_PARAM_SEED_SIZE * i], leftover, nbytes % MQOM3_PARAM_SEED_SIZE);
	}

	ret = 0;
err:
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		se_clean_ctx(&ctx);
	}
#ifndef SEED_EXPAND_PUBLIC
	mqom_cleanse(xor_mask, sizeof(xor_mask));
	mqom_cleanse(leftover, sizeof(leftover));
#endif
	return ret;
}

#else /* !PRG_ONE_RIJNDAEL_CTX */
int SeedExpand_F(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[MQOM3_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t *out_data, SE_CACHE_T *cache) {
	int ret = -1;
	uint32_t i, filled_blocks;
	SE_CTX DECL_VAR(ctx1), DECL_VAR(ctx2), DECL_VAR(ctx3), DECL_VAR(ctx4);
#ifdef USE_ENC_X8
	SE_CTX DECL_VAR(ctx5), DECL_VAR(ctx6), DECL_VAR(ctx7), DECL_VAR(ctx8);
	uint32_t num_blocks = 8;
#else
	uint32_t num_blocks = 4;
#endif
	uint8_t xor_mask[MQOM3_PARAM_SEED_SIZE];
	uint8_t leftover[MQOM3_PARAM_SEED_SIZE];
	/* CT: block indices are offset by 1 (block[0] is the XOR mask, not an output block) */
#if MQOM3_PARAM_OT_VARIANT != 1
	uint32_t idx_base = 1;
#else
	uint32_t idx_base = 0;
#endif

#if MQOM3_PARAM_OT_VARIANT != 1
	/* CT: compute block[0] as XOR mask */
	ret = se_key_sched(salt, &e, 0, &ctx1, cache);
	ERR(ret, err);
	ret = se_encrypt(&ctx1, seed, xor_mask);
	ERR(ret, err);
#else
	LinOrtho(seed, xor_mask);
#endif

	/* Depending on the number of blocks, exploit the 2x, 4x or 8x variants */
	filled_blocks = 0;
	for (i = 0; i < (nbytes / (num_blocks * MQOM3_PARAM_SEED_SIZE)); i++) {
		/* Key schedule */
		ret = se_key_sched(salt, &e, idx_base + filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 3, &ctx4, cache);
		ERR(ret, err);
#ifdef USE_ENC_X8
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 4, &ctx5, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 5, &ctx6, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 6, &ctx7, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 7, &ctx8, cache);
		ERR(ret, err);
#endif
		/* Encryption */
#ifdef USE_ENC_X8
		ret = se_encrypt_x8(&ctx1, &ctx2, &ctx3, &ctx4, &ctx5, &ctx6, &ctx7, &ctx8,
		                    seed, seed, seed, seed, seed, seed, seed, seed,
		                    &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 3)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 4)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 5)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 6)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 7)]);
		ERR(ret, err);
#else
		ret = se_encrypt_x4(&ctx1, &ctx2, &ctx3, &ctx4,
		                    seed, seed, seed, seed,
		                    &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		ERR(ret, err);
#endif
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 3)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 3)]);
#ifdef USE_ENC_X8
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 4)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 4)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 5)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 5)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 6)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 6)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 7)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 7)]);
#endif
		filled_blocks += num_blocks;
	}
	switch ((nbytes - (num_blocks * MQOM3_PARAM_SEED_SIZE * i)) / MQOM3_PARAM_SEED_SIZE) {
	case 0: {
		/* No remaining block */
		break;
	}
	case 1: {
		/* One remaining block: 1x */
		ret = se_key_sched(salt, &e, idx_base + filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = se_encrypt(&ctx1, seed, &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks]);
		filled_blocks += 1;
		break;
	}
	case 2: {
		/* Two remaining blocks: 2x */
		ret = se_key_sched(salt, &e, idx_base + filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = se_encrypt_x2(&ctx1, &ctx2, seed, seed,
		                    &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		filled_blocks += 2;
		break;
	}
	case 3: {
		/* Three remaining blocks: 2x and then 1x */
		ret = se_key_sched(salt, &e, idx_base + filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = se_encrypt_x2(&ctx1, &ctx2, seed, seed,
		                    &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		ERR(ret, err);
		ret = se_encrypt(&ctx3, seed, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		filled_blocks += 3;
		break;
	}
#ifdef USE_ENC_X8
	case 4: {
		/* Four remaining blocks: 4x */
		ret = se_key_sched(salt, &e, idx_base + filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 3, &ctx4, cache);
		ERR(ret, err);
		ret = se_encrypt_x4(&ctx1, &ctx2, &ctx3, &ctx4,
		                    seed, seed, seed, seed,
		                    &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		ERR(ret, err);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 3)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		filled_blocks += 4;
		break;
	}
	case 5: {
		/* Five remaining blocks: 4x and 1x */
		ret = se_key_sched(salt, &e, idx_base + filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 3, &ctx4, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 4, &ctx5, cache);
		ERR(ret, err);
		ret = se_encrypt_x4(&ctx1, &ctx2, &ctx3, &ctx4,
		                    seed, seed, seed, seed,
		                    &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		ERR(ret, err);
		ret = se_encrypt(&ctx5, seed, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 4)]);
		ERR(ret, err);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 3)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 4)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 4)]);
		filled_blocks += 5;
		break;
	}
	case 6: {
		/* Six remaining blocks: 4x and 2x */
		ret = se_key_sched(salt, &e, idx_base + filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 3, &ctx4, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 4, &ctx5, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 5, &ctx6, cache);
		ERR(ret, err);
		ret = se_encrypt_x4(&ctx1, &ctx2, &ctx3, &ctx4,
		                    seed, seed, seed, seed,
		                    &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		ERR(ret, err);
		ret = se_encrypt_x2(&ctx5, &ctx6, seed, seed,
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 4)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 5)]);
		ERR(ret, err);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 3)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 4)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 4)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 5)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 5)]);
		filled_blocks += 6;
		break;
	}
	case 7: {
		/* Seven remaining blocks: 4x, 2x and 1x */
		ret = se_key_sched(salt, &e, idx_base + filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 1, &ctx2, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 2, &ctx3, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 3, &ctx4, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 4, &ctx5, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 5, &ctx6, cache);
		ERR(ret, err);
		ret = se_key_sched(salt, &e, idx_base + filled_blocks + 6, &ctx7, cache);
		ERR(ret, err);
		ret = se_encrypt_x4(&ctx1, &ctx2, &ctx3, &ctx4,
		                    seed, seed, seed, seed,
		                    &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		ERR(ret, err);
		ret = se_encrypt_x2(&ctx5, &ctx6, seed, seed,
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 4)],
		                    &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 5)]);
		ERR(ret, err);
		ret = se_encrypt(&ctx7, seed, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 6)]);
		ERR(ret, err);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 1)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 2)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 3)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 3)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 4)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 4)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 5)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 5)]);
		xor_blocks(&out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 6)], xor_mask, &out_data[MQOM3_PARAM_SEED_SIZE * (filled_blocks + 6)]);
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
	if (nbytes % MQOM3_PARAM_SEED_SIZE) {
		ret = se_key_sched(salt, &e, idx_base + filled_blocks, &ctx1, cache);
		ERR(ret, err);
		ret = se_encrypt(&ctx1, seed, leftover);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(leftover, xor_mask, leftover);
		memcpy(&out_data[MQOM3_PARAM_SEED_SIZE * filled_blocks], leftover, nbytes % MQOM3_PARAM_SEED_SIZE);
	}

	ret = 0;
err:
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		se_clean_ctx(&ctx1);
		se_clean_ctx(&ctx2);
		se_clean_ctx(&ctx3);
		se_clean_ctx(&ctx4);
#ifdef USE_ENC_X8
		se_clean_ctx(&ctx5);
		se_clean_ctx(&ctx6);
		se_clean_ctx(&ctx7);
		se_clean_ctx(&ctx8);
#endif
	}
#ifndef SEED_EXPAND_PUBLIC
	mqom_cleanse(xor_mask, sizeof(xor_mask));
	mqom_cleanse(leftover, sizeof(leftover));
#endif
	return ret;
}
#endif /* PRG_ONE_RIJNDAEL_CTX */

/* Cleanup all macros so this file can be safely included a second time. */
#undef SE_CTX
#undef SE_CACHE_T
#undef se_key_sched
#undef se_encrypt
#undef se_encrypt_x2
#undef se_encrypt_x4
#undef se_encrypt_x8
#undef se_clean_ctx
#undef SeedExpand_memopt_F
#undef SeedExpand_F
#undef SEED_EXPAND_PUBLIC
