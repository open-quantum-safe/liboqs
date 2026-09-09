/* seed_expand_xn_impl.h - X-macro template for the SeedExpand xN / xN_pub
 * function pairs.  Included TWICE from seed_expand.c without an include
 * guard; do not add one.
 *
 *   pass 1 (SEED_EXPAND_PUBLIC undefined): generates SeedExpand_x2, _x4, _x8, _x8_ecb
 *   pass 2 (#define SEED_EXPAND_PUBLIC):   generates SeedExpand_x2_pub, _x4_pub, _x8_pub, _x8_pub_ecb
 */

#ifdef SEED_EXPAND_PUBLIC
/* x2 public */
#define SE_XN2_CTX                                           enc_ctx_pub_x2
#define SE_XN2_CACHE                                         SeedExpand_cache_pub_x2
#define se_xn2_key_sched(s, es, i, ctx, cache)              SeedExpand_key_sched_pub_x2(s, es, i, ctx, cache)
#define se_xn2_encrypt(ctx, i0, i1, o0, o1)                 enc_encrypt_x2_pub_x2(ctx, i0, i1, o0, o1)
#define se_xn2_clean(ctx)                                    enc_clean_ctx_pub_x2(ctx)
#define SeedExpand_x2_F                                      SeedExpand_x2_pub
/* x4 public */
#define SE_XN4_CTX                                           enc_ctx_pub_x4
#define SE_XN4_CACHE                                         SeedExpand_cache_pub_x4
#define se_xn4_key_sched(s, es, i, ctx, cache)              SeedExpand_key_sched_pub_x4(s, es, i, ctx, cache)
#define se_xn4_encrypt(ctx, i0, i1, i2, i3, o0, o1, o2, o3) enc_encrypt_x4_pub_x4(ctx, i0, i1, i2, i3, o0, o1, o2, o3)
#define se_xn4_clean(ctx)                                    enc_clean_ctx_pub_x4(ctx)
#define SeedExpand_x4_F                                      SeedExpand_x4_pub
/* x8 public */
#define SE_XN8_CTX                                           enc_ctx_pub_x8
#define SE_XN8_CACHE                                         SeedExpand_cache_pub_x8
#define se_xn8_key_sched(s, es, i, ctx, cache)              SeedExpand_key_sched_pub_x8(s, es, i, ctx, cache)
#define se_xn8_encrypt(ctx, \
	i0, i1, i2, i3, i4, i5, i6, i7, \
	o0, o1, o2, o3, o4, o5, o6, o7) \
	enc_encrypt_x8_pub_x8(ctx, \
	                       i0, i1, i2, i3, i4, i5, i6, i7, \
	                       o0, o1, o2, o3, o4, o5, o6, o7)
#define se_xn8_clean(ctx)                                    enc_clean_ctx_pub_x8(ctx)
#define SeedExpand_x8_F                                      SeedExpand_x8_pub
/* x8 ECB public */
#define SE_XNECB_CTX                                         enc_ctx_pub_ecb
#define SE_XNECB_CACHE                                       SeedExpand_cache_pub_ecb
#define se_xnecb_key_sched(s, e, i, ctx, cache)             SeedExpand_key_sched_pub_ecb(s, e, i, ctx, cache)
#define se_xnecb_encrypt(ctx, n, pt, ct)                     enc_encrypt_pub_ecb(ctx, n, pt, ct)
#define se_xnecb_clean(ctx)                                  enc_clean_ctx_pub_ecb(ctx)
#define SeedExpand_x8_ecb_F                                  SeedExpand_x8_pub_ecb
#else
/* x2 secret */
#define SE_XN2_CTX                                           enc_ctx_x2
#define SE_XN2_CACHE                                         SeedExpand_cache_x2
#define se_xn2_key_sched(s, es, i, ctx, cache)              SeedExpand_key_sched_x2(s, es, i, ctx, cache)
#define se_xn2_encrypt(ctx, i0, i1, o0, o1)                 enc_encrypt_x2_x2(ctx, i0, i1, o0, o1)
#define se_xn2_clean(ctx)                                    enc_clean_ctx_x2(ctx)
#define SeedExpand_x2_F                                      SeedExpand_x2
/* x4 secret */
#define SE_XN4_CTX                                           enc_ctx_x4
#define SE_XN4_CACHE                                         SeedExpand_cache_x4
#define se_xn4_key_sched(s, es, i, ctx, cache)              SeedExpand_key_sched_x4(s, es, i, ctx, cache)
#define se_xn4_encrypt(ctx, i0, i1, i2, i3, o0, o1, o2, o3) enc_encrypt_x4_x4(ctx, i0, i1, i2, i3, o0, o1, o2, o3)
#define se_xn4_clean(ctx)                                    enc_clean_ctx_x4(ctx)
#define SeedExpand_x4_F                                      SeedExpand_x4
/* x8 secret */
#define SE_XN8_CTX                                           enc_ctx_x8
#define SE_XN8_CACHE                                         SeedExpand_cache_x8
#define se_xn8_key_sched(s, es, i, ctx, cache)              SeedExpand_key_sched_x8(s, es, i, ctx, cache)
#define se_xn8_encrypt(ctx, \
	i0, i1, i2, i3, i4, i5, i6, i7, \
	o0, o1, o2, o3, o4, o5, o6, o7) \
	enc_encrypt_x8_x8(ctx, \
	                   i0, i1, i2, i3, i4, i5, i6, i7, \
	                   o0, o1, o2, o3, o4, o5, o6, o7)
#define se_xn8_clean(ctx)                                    enc_clean_ctx_x8(ctx)
#define SeedExpand_x8_F                                      SeedExpand_x8
/* x8 ECB secret */
#define SE_XNECB_CTX                                         enc_ctx_ecb
#define SE_XNECB_CACHE                                       SeedExpand_cache_ecb
#define se_xnecb_key_sched(s, e, i, ctx, cache)             SeedExpand_key_sched_ecb(s, e, i, ctx, cache)
#define se_xnecb_encrypt(ctx, n, pt, ct)                     enc_encrypt_ecb(ctx, n, pt, ct)
#define se_xnecb_clean(ctx)                                  enc_clean_ctx_ecb(ctx)
#define SeedExpand_x8_ecb_F                                  SeedExpand_x8_ecb
#endif

/* ================================================================= */
/* SeedExpand_x2_F                                                    */
/* ================================================================= */
int SeedExpand_x2_F(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint32_t *e, const uint8_t seed[2][MQOM3_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t *out_data[2], SE_XN2_CACHE *cache, uint8_t nb_contexts) {
	int ret = -1;
	uint32_t i;
	SE_XN2_CTX DECL_VAR(ctx_x2);
	uint8_t xor_mask[2][MQOM3_PARAM_SEED_SIZE];
	uint8_t leftover[2][MQOM3_PARAM_SEED_SIZE];
	uint32_t es[2];

	if (nb_contexts == 1) {
		es[0] = e[0];
		es[1] = e[0];
	} else if (nb_contexts == 2) {
		es[0] = e[0];
		es[1] = e[1];
	} else {
		goto err;
	}

#if MQOM3_PARAM_OT_VARIANT != 1
	/* CT: block[0] as XOR mask */
	ret = se_xn2_key_sched(salt, es, 0, &ctx_x2, cache);
	ERR(ret, err);
	ret = se_xn2_encrypt(&ctx_x2, seed[0], seed[1], xor_mask[0], xor_mask[1]);
	ERR(ret, err);
#else
	LinOrtho(seed[0], xor_mask[0]);
	LinOrtho(seed[1], xor_mask[1]);
#endif

	for (i = 0; i < (nbytes / MQOM3_PARAM_SEED_SIZE); i++) {
		/* Key schedule */
#if MQOM3_PARAM_OT_VARIANT != 1
		ret = se_xn2_key_sched(salt, es, i + 1, &ctx_x2, cache);
#else
		ret = se_xn2_key_sched(salt, es, i, &ctx_x2, cache);
#endif
		ERR(ret, err);
		/* Encryption */
		ret = se_xn2_encrypt(&ctx_x2, seed[0], seed[1],
		                     &out_data[0][MQOM3_PARAM_SEED_SIZE * i], &out_data[1][MQOM3_PARAM_SEED_SIZE * i]);
		ERR(ret, err);
		/* Xor with mask */
		xor_blocks(&out_data[0][MQOM3_PARAM_SEED_SIZE * i], xor_mask[0], &out_data[0][MQOM3_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[1][MQOM3_PARAM_SEED_SIZE * i], xor_mask[1], &out_data[1][MQOM3_PARAM_SEED_SIZE * i]);
	}
	/* Deal with the possible leftover incomplete block */
	uint32_t remaining_bytes = nbytes % MQOM3_PARAM_SEED_SIZE;
	if (remaining_bytes) {
#if MQOM3_PARAM_OT_VARIANT != 1
		ret = se_xn2_key_sched(salt, es, i + 1, &ctx_x2, cache);
#else
		ret = se_xn2_key_sched(salt, es, i, &ctx_x2, cache);
#endif
		ERR(ret, err);
		ret = se_xn2_encrypt(&ctx_x2, seed[0], seed[1], leftover[0], leftover[1]);
		ERR(ret, err);
		xor_blocks(leftover[0], xor_mask[0], leftover[0]);
		xor_blocks(leftover[1], xor_mask[1], leftover[1]);
		memcpy(&out_data[0][MQOM3_PARAM_SEED_SIZE * i], leftover[0], remaining_bytes);
		memcpy(&out_data[1][MQOM3_PARAM_SEED_SIZE * i], leftover[1], remaining_bytes);
	}

	ret = 0;
err:
#ifndef SEED_EXPAND_PUBLIC
	mqom_cleanse(xor_mask, sizeof(xor_mask));
	mqom_cleanse(leftover, sizeof(leftover));
#endif
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		se_xn2_clean(&ctx_x2);
	}
	return ret;
}

/* ================================================================= */
/* SeedExpand_x4_F                                                    */
/* ================================================================= */
int SeedExpand_x4_F(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint32_t *e, const uint8_t seed[4][MQOM3_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t *out_data[4], SE_XN4_CACHE *cache, uint8_t nb_contexts) {
	int ret = -1;
	uint32_t i;
	SE_XN4_CTX DECL_VAR(ctx_x4);
	uint8_t xor_mask[4][MQOM3_PARAM_SEED_SIZE];
	uint8_t leftover[4][MQOM3_PARAM_SEED_SIZE];
	uint32_t es[4];

	if (nb_contexts == 1) {
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

#if MQOM3_PARAM_OT_VARIANT != 1
	/* CT: block[0] as XOR mask */
	ret = se_xn4_key_sched(salt, es, 0, &ctx_x4, cache);
	ERR(ret, err);
	ret = se_xn4_encrypt(&ctx_x4, seed[0], seed[1], seed[2], seed[3],
	                     xor_mask[0], xor_mask[1], xor_mask[2], xor_mask[3]);
	ERR(ret, err);
#else
	LinOrtho(seed[0], xor_mask[0]);
	LinOrtho(seed[1], xor_mask[1]);
	LinOrtho(seed[2], xor_mask[2]);
	LinOrtho(seed[3], xor_mask[3]);
#endif

	for (i = 0; i < (nbytes / MQOM3_PARAM_SEED_SIZE); i++) {
		/* Key schedule */
#if MQOM3_PARAM_OT_VARIANT != 1
		ret = se_xn4_key_sched(salt, es, i + 1, &ctx_x4, cache);
#else
		ret = se_xn4_key_sched(salt, es, i, &ctx_x4, cache);
#endif
		ERR(ret, err);
		/* Encryption */
		ret = se_xn4_encrypt(&ctx_x4, seed[0], seed[1], seed[2], seed[3],
		                     &out_data[0][MQOM3_PARAM_SEED_SIZE * i], &out_data[1][MQOM3_PARAM_SEED_SIZE * i],
		                     &out_data[2][MQOM3_PARAM_SEED_SIZE * i], &out_data[3][MQOM3_PARAM_SEED_SIZE * i]);
		ERR(ret, err);
		/* Xor with mask */
		xor_blocks(&out_data[0][MQOM3_PARAM_SEED_SIZE * i], xor_mask[0], &out_data[0][MQOM3_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[1][MQOM3_PARAM_SEED_SIZE * i], xor_mask[1], &out_data[1][MQOM3_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[2][MQOM3_PARAM_SEED_SIZE * i], xor_mask[2], &out_data[2][MQOM3_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[3][MQOM3_PARAM_SEED_SIZE * i], xor_mask[3], &out_data[3][MQOM3_PARAM_SEED_SIZE * i]);
	}
	/* Deal with the possible leftover incomplete block */
	uint32_t remaining_bytes = nbytes % MQOM3_PARAM_SEED_SIZE;
	if (remaining_bytes) {
#if MQOM3_PARAM_OT_VARIANT != 1
		ret = se_xn4_key_sched(salt, es, i + 1, &ctx_x4, cache);
#else
		ret = se_xn4_key_sched(salt, es, i, &ctx_x4, cache);
#endif
		ERR(ret, err);
		ret = se_xn4_encrypt(&ctx_x4, seed[0], seed[1], seed[2], seed[3],
		                     leftover[0], leftover[1], leftover[2], leftover[3]);
		ERR(ret, err);
		xor_blocks(leftover[0], xor_mask[0], leftover[0]);
		xor_blocks(leftover[1], xor_mask[1], leftover[1]);
		xor_blocks(leftover[2], xor_mask[2], leftover[2]);
		xor_blocks(leftover[3], xor_mask[3], leftover[3]);
		memcpy(&out_data[0][MQOM3_PARAM_SEED_SIZE * i], leftover[0], remaining_bytes);
		memcpy(&out_data[1][MQOM3_PARAM_SEED_SIZE * i], leftover[1], remaining_bytes);
		memcpy(&out_data[2][MQOM3_PARAM_SEED_SIZE * i], leftover[2], remaining_bytes);
		memcpy(&out_data[3][MQOM3_PARAM_SEED_SIZE * i], leftover[3], remaining_bytes);
	}

	ret = 0;
err:
#ifndef SEED_EXPAND_PUBLIC
	mqom_cleanse(xor_mask, sizeof(xor_mask));
	mqom_cleanse(leftover, sizeof(leftover));
#endif
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		se_xn4_clean(&ctx_x4);
	}
	return ret;
}

/* ================================================================= */
/* SeedExpand_x8_F                                                    */
/* ================================================================= */
int SeedExpand_x8_F(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint32_t *e, const uint8_t seed[8][MQOM3_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t *out_data[8], SE_XN8_CACHE *cache, uint8_t nb_contexts) {
	int ret = -1;
	uint32_t i;
	SE_XN8_CTX DECL_VAR(ctx_x8);
	uint8_t xor_mask[8][MQOM3_PARAM_SEED_SIZE];
	uint8_t leftover[8][MQOM3_PARAM_SEED_SIZE];
	uint32_t es[8];

	if (nb_contexts == 1) {
		es[0] = e[0]; es[1] = e[0]; es[2] = e[0]; es[3] = e[0];
		es[4] = e[0]; es[5] = e[0]; es[6] = e[0]; es[7] = e[0];
	} else if (nb_contexts == 8) {
		es[0] = e[0]; es[1] = e[1]; es[2] = e[2]; es[3] = e[3];
		es[4] = e[4]; es[5] = e[5]; es[6] = e[6]; es[7] = e[7];
	} else {
		goto err;
	}

#if MQOM3_PARAM_OT_VARIANT != 1
	/* CT: block[0] as XOR mask */
	ret = se_xn8_key_sched(salt, es, 0, &ctx_x8, cache);
	ERR(ret, err);
	ret = se_xn8_encrypt(&ctx_x8,
	                     seed[0], seed[1], seed[2], seed[3], seed[4], seed[5], seed[6], seed[7],
	                     xor_mask[0], xor_mask[1], xor_mask[2], xor_mask[3],
	                     xor_mask[4], xor_mask[5], xor_mask[6], xor_mask[7]);
	ERR(ret, err);
#else
	LinOrtho(seed[0], xor_mask[0]); LinOrtho(seed[1], xor_mask[1]);
	LinOrtho(seed[2], xor_mask[2]); LinOrtho(seed[3], xor_mask[3]);
	LinOrtho(seed[4], xor_mask[4]); LinOrtho(seed[5], xor_mask[5]);
	LinOrtho(seed[6], xor_mask[6]); LinOrtho(seed[7], xor_mask[7]);
#endif

	for (i = 0; i < (nbytes / MQOM3_PARAM_SEED_SIZE); i++) {
		/* Key schedule */
#if MQOM3_PARAM_OT_VARIANT != 1
		ret = se_xn8_key_sched(salt, es, i + 1, &ctx_x8, cache);
#else
		ret = se_xn8_key_sched(salt, es, i, &ctx_x8, cache);
#endif
		ERR(ret, err);
		/* Encryption */
		ret = se_xn8_encrypt(&ctx_x8,
		                     seed[0], seed[1], seed[2], seed[3], seed[4], seed[5], seed[6], seed[7],
		                     &out_data[0][MQOM3_PARAM_SEED_SIZE * i], &out_data[1][MQOM3_PARAM_SEED_SIZE * i],
		                     &out_data[2][MQOM3_PARAM_SEED_SIZE * i], &out_data[3][MQOM3_PARAM_SEED_SIZE * i],
		                     &out_data[4][MQOM3_PARAM_SEED_SIZE * i], &out_data[5][MQOM3_PARAM_SEED_SIZE * i],
		                     &out_data[6][MQOM3_PARAM_SEED_SIZE * i], &out_data[7][MQOM3_PARAM_SEED_SIZE * i]);
		ERR(ret, err);
		/* Xor with mask */
		xor_blocks(&out_data[0][MQOM3_PARAM_SEED_SIZE * i], xor_mask[0], &out_data[0][MQOM3_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[1][MQOM3_PARAM_SEED_SIZE * i], xor_mask[1], &out_data[1][MQOM3_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[2][MQOM3_PARAM_SEED_SIZE * i], xor_mask[2], &out_data[2][MQOM3_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[3][MQOM3_PARAM_SEED_SIZE * i], xor_mask[3], &out_data[3][MQOM3_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[4][MQOM3_PARAM_SEED_SIZE * i], xor_mask[4], &out_data[4][MQOM3_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[5][MQOM3_PARAM_SEED_SIZE * i], xor_mask[5], &out_data[5][MQOM3_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[6][MQOM3_PARAM_SEED_SIZE * i], xor_mask[6], &out_data[6][MQOM3_PARAM_SEED_SIZE * i]);
		xor_blocks(&out_data[7][MQOM3_PARAM_SEED_SIZE * i], xor_mask[7], &out_data[7][MQOM3_PARAM_SEED_SIZE * i]);
	}
	/* Deal with the possible leftover incomplete block */
	uint32_t remaining_bytes = nbytes % MQOM3_PARAM_SEED_SIZE;
	if (remaining_bytes) {
#if MQOM3_PARAM_OT_VARIANT != 1
		ret = se_xn8_key_sched(salt, es, i + 1, &ctx_x8, cache);
#else
		ret = se_xn8_key_sched(salt, es, i, &ctx_x8, cache);
#endif
		ERR(ret, err);
		ret = se_xn8_encrypt(&ctx_x8,
		                     seed[0], seed[1], seed[2], seed[3], seed[4], seed[5], seed[6], seed[7],
		                     leftover[0], leftover[1], leftover[2], leftover[3],
		                     leftover[4], leftover[5], leftover[6], leftover[7]);
		ERR(ret, err);
		xor_blocks(leftover[0], xor_mask[0], leftover[0]); xor_blocks(leftover[1], xor_mask[1], leftover[1]);
		xor_blocks(leftover[2], xor_mask[2], leftover[2]); xor_blocks(leftover[3], xor_mask[3], leftover[3]);
		xor_blocks(leftover[4], xor_mask[4], leftover[4]); xor_blocks(leftover[5], xor_mask[5], leftover[5]);
		xor_blocks(leftover[6], xor_mask[6], leftover[6]); xor_blocks(leftover[7], xor_mask[7], leftover[7]);
		memcpy(&out_data[0][MQOM3_PARAM_SEED_SIZE * i], leftover[0], remaining_bytes);
		memcpy(&out_data[1][MQOM3_PARAM_SEED_SIZE * i], leftover[1], remaining_bytes);
		memcpy(&out_data[2][MQOM3_PARAM_SEED_SIZE * i], leftover[2], remaining_bytes);
		memcpy(&out_data[3][MQOM3_PARAM_SEED_SIZE * i], leftover[3], remaining_bytes);
		memcpy(&out_data[4][MQOM3_PARAM_SEED_SIZE * i], leftover[4], remaining_bytes);
		memcpy(&out_data[5][MQOM3_PARAM_SEED_SIZE * i], leftover[5], remaining_bytes);
		memcpy(&out_data[6][MQOM3_PARAM_SEED_SIZE * i], leftover[6], remaining_bytes);
		memcpy(&out_data[7][MQOM3_PARAM_SEED_SIZE * i], leftover[7], remaining_bytes);
	}

	ret = 0;
err:
#ifndef SEED_EXPAND_PUBLIC
	mqom_cleanse(xor_mask, sizeof(xor_mask));
	mqom_cleanse(leftover, sizeof(leftover));
#endif
	/* XXX: NOTE: we do NOT clean the contexts when using a cache as these must be preserved
	 * and will be cleaned when the cache is destroyed.
	 */
	if (cache == NULL) {
		se_xn8_clean(&ctx_x8);
	}
	return ret;
}

/* ================================================================= */
/* SeedExpand_x8_ecb_F                                                */
/* ECB variants: all 8 seeds share the same key derived from          */
/* (salt, e, block_index). Uses a compact single-key cache            */
/* (enc_ctx_ecb = enc_ctx) instead of enc_ctx_x8, cutting cache entry */
/* size from 8x to 1x the single-context size and eliminating 7       */
/* redundant key schedule computations on cache misses.               */
/* ================================================================= */
int SeedExpand_x8_ecb_F(const uint8_t salt[MQOM3_PARAM_SALT_SIZE], uint32_t e, const uint8_t seed[8][MQOM3_PARAM_SEED_SIZE], uint32_t nbytes, uint8_t *out_data[8], SE_XNECB_CACHE *cache) {
	int ret = -1;
	uint32_t i;
	SE_XNECB_CTX DECL_VAR(ctx_ecb);
	uint8_t xor_mask[8][MQOM3_PARAM_SEED_SIZE];
	uint8_t leftover[8][MQOM3_PARAM_SEED_SIZE];
	uint8_t ct_packed[8 * MQOM3_PARAM_SEED_SIZE];

	/* seed[] is a genuine 8 x SEED_SIZE C array (every caller passes a
	 * contiguous slice of an lseed[] array), so seed[0] already is the packed
	 * plaintext the ECB primitive wants: no per-block repacking is needed.
	 * The previous code rebuilt an identical pt_packed[] buffer at every block
	 * index even though the plaintext is loop-invariant. */
	const uint8_t *pt_packed = (const uint8_t*) seed;

#if MQOM3_PARAM_OT_VARIANT != 1
	/* CT: block[0] as XOR mask (one key schedule, encrypt all 8 seeds) */
	{
		ret = se_xnecb_key_sched(salt, &e, 0, &ctx_ecb, cache);
		ERR(ret, err);
		ret = se_xnecb_encrypt(&ctx_ecb, 8, pt_packed, (uint8_t*)xor_mask);
		ERR(ret, err);
	}
#else
	LinOrtho(seed[0], xor_mask[0]); LinOrtho(seed[1], xor_mask[1]);
	LinOrtho(seed[2], xor_mask[2]); LinOrtho(seed[3], xor_mask[3]);
	LinOrtho(seed[4], xor_mask[4]); LinOrtho(seed[5], xor_mask[5]);
	LinOrtho(seed[6], xor_mask[6]); LinOrtho(seed[7], xor_mask[7]);
#endif

	for (i = 0; i < (nbytes / MQOM3_PARAM_SEED_SIZE); i++) {
		/* Single key schedule: all 8 seeds use the same (salt, e, block_index) */
#if MQOM3_PARAM_OT_VARIANT != 1
		ret = se_xnecb_key_sched(salt, &e, i + 1, &ctx_ecb, cache);
#else
		ret = se_xnecb_key_sched(salt, &e, i, &ctx_ecb, cache);
#endif
		ERR(ret, err);
		/* Encrypt 8 seeds with the same key using ECB parallelism.
		 * se_xnecb_encrypt takes contiguous buffers; the plaintext side already
		 * is contiguous (pt_packed above), and the scatter to out_data[] is
		 * fused with the LinOrtho xor below, so no unpack memcpy is needed. */
		{
			ret = se_xnecb_encrypt(&ctx_ecb, 8, pt_packed, ct_packed);
			ERR(ret, err);
			/* Xor with LinOrtho seed, scattering to the 8 output buffers */
			xor_blocks(ct_packed + 0 * MQOM3_PARAM_SEED_SIZE, xor_mask[0], &out_data[0][MQOM3_PARAM_SEED_SIZE * i]);
			xor_blocks(ct_packed + 1 * MQOM3_PARAM_SEED_SIZE, xor_mask[1], &out_data[1][MQOM3_PARAM_SEED_SIZE * i]);
			xor_blocks(ct_packed + 2 * MQOM3_PARAM_SEED_SIZE, xor_mask[2], &out_data[2][MQOM3_PARAM_SEED_SIZE * i]);
			xor_blocks(ct_packed + 3 * MQOM3_PARAM_SEED_SIZE, xor_mask[3], &out_data[3][MQOM3_PARAM_SEED_SIZE * i]);
			xor_blocks(ct_packed + 4 * MQOM3_PARAM_SEED_SIZE, xor_mask[4], &out_data[4][MQOM3_PARAM_SEED_SIZE * i]);
			xor_blocks(ct_packed + 5 * MQOM3_PARAM_SEED_SIZE, xor_mask[5], &out_data[5][MQOM3_PARAM_SEED_SIZE * i]);
			xor_blocks(ct_packed + 6 * MQOM3_PARAM_SEED_SIZE, xor_mask[6], &out_data[6][MQOM3_PARAM_SEED_SIZE * i]);
			xor_blocks(ct_packed + 7 * MQOM3_PARAM_SEED_SIZE, xor_mask[7], &out_data[7][MQOM3_PARAM_SEED_SIZE * i]);
		}
	}
	/* Deal with the possible leftover incomplete block */
	uint32_t remaining_bytes = nbytes % MQOM3_PARAM_SEED_SIZE;
	if (remaining_bytes) {
#if MQOM3_PARAM_OT_VARIANT != 1
		ret = se_xnecb_key_sched(salt, &e, i + 1, &ctx_ecb, cache);
#else
		ret = se_xnecb_key_sched(salt, &e, i, &ctx_ecb, cache);
#endif
		ERR(ret, err);
		ret = se_xnecb_encrypt(&ctx_ecb, 8, pt_packed, (uint8_t*)leftover);
		ERR(ret, err);
		xor_blocks(leftover[0], xor_mask[0], leftover[0]); xor_blocks(leftover[1], xor_mask[1], leftover[1]);
		xor_blocks(leftover[2], xor_mask[2], leftover[2]); xor_blocks(leftover[3], xor_mask[3], leftover[3]);
		xor_blocks(leftover[4], xor_mask[4], leftover[4]); xor_blocks(leftover[5], xor_mask[5], leftover[5]);
		xor_blocks(leftover[6], xor_mask[6], leftover[6]); xor_blocks(leftover[7], xor_mask[7], leftover[7]);
		memcpy(&out_data[0][MQOM3_PARAM_SEED_SIZE * i], leftover[0], remaining_bytes);
		memcpy(&out_data[1][MQOM3_PARAM_SEED_SIZE * i], leftover[1], remaining_bytes);
		memcpy(&out_data[2][MQOM3_PARAM_SEED_SIZE * i], leftover[2], remaining_bytes);
		memcpy(&out_data[3][MQOM3_PARAM_SEED_SIZE * i], leftover[3], remaining_bytes);
		memcpy(&out_data[4][MQOM3_PARAM_SEED_SIZE * i], leftover[4], remaining_bytes);
		memcpy(&out_data[5][MQOM3_PARAM_SEED_SIZE * i], leftover[5], remaining_bytes);
		memcpy(&out_data[6][MQOM3_PARAM_SEED_SIZE * i], leftover[6], remaining_bytes);
		memcpy(&out_data[7][MQOM3_PARAM_SEED_SIZE * i], leftover[7], remaining_bytes);
	}

	ret = 0;
err:
#ifndef SEED_EXPAND_PUBLIC
	mqom_cleanse(xor_mask, sizeof(xor_mask));
	mqom_cleanse(leftover, sizeof(leftover));
	mqom_cleanse(ct_packed, sizeof(ct_packed));
#endif
	if (cache == NULL) {
		se_xnecb_clean(&ctx_ecb);
	}
	return ret;
}

/* Cleanup all macros so this file can be safely included a second time. */
#undef SE_XN2_CTX
#undef SE_XN2_CACHE
#undef se_xn2_key_sched
#undef se_xn2_encrypt
#undef se_xn2_clean
#undef SeedExpand_x2_F
#undef SE_XN4_CTX
#undef SE_XN4_CACHE
#undef se_xn4_key_sched
#undef se_xn4_encrypt
#undef se_xn4_clean
#undef SeedExpand_x4_F
#undef SE_XN8_CTX
#undef SE_XN8_CACHE
#undef se_xn8_key_sched
#undef se_xn8_encrypt
#undef se_xn8_clean
#undef SeedExpand_x8_F
#undef SE_XNECB_CTX
#undef SE_XNECB_CACHE
#undef se_xnecb_key_sched
#undef se_xnecb_encrypt
#undef se_xnecb_clean
#undef SeedExpand_x8_ecb_F
#undef SEED_EXPAND_PUBLIC
