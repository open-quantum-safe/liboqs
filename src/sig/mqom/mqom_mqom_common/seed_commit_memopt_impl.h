/* seed_commit_memopt_impl.h - X-macro template for seed_commit_memopt.h
 * No include guard: this file is included twice.
 *   pass 1 (SEED_COMMIT_PUBLIC undefined): generates non-pub functions
 *   pass 2 (#define SEED_COMMIT_PUBLIC):   generates pub functions
 *
 * In the memopt variant, key schedules are NOT cached (computed fresh each call
 * to save memory at the cost of extra cycles).  The context struct is therefore
 * identical for pub and non-pub; only the encrypt primitives differ.
 *
 * Functions generated per pass:
 *   SeedCommit / SeedCommit_pub
 *   SeedCommit_x2 / SeedCommit_pub_x2
 *   SeedCommit_x4_x4 / SeedCommit_x4_x4_pub
 *   SeedCommit_x4_ecb / SeedCommit_x4_pub_ecb  (bare ECB ctx, same body as default)
 */

#ifdef SEED_COMMIT_PUBLIC
#  define SC_CTX          enc_ctx_pub
#  define sc_key_sched    enc_key_sched_pub
#  define sc_encrypt      enc_encrypt_pub
#  define sc_clean_ctx    enc_clean_ctx_pub
#  define sc_encrypt_x8   enc_encrypt_x8_pub
#  define sc_encrypt_x4   enc_encrypt_x4_pub
#  define SC_CTX_ECB         enc_ctx_pub_ecb
#  define sc_encrypt_ecb     enc_encrypt_pub_ecb
#  define seedcommit_ctx_x2_F    seedcommit_ctx_pub_x2_t
#  define seedcommit_ctx_x4_F    seedcommit_ctx_pub_x4_t
#  define init_seedcommit_x2_F   init_seedcommit_pub_x2
#  define init_seedcommit_x4_F   init_seedcommit_pub_x4
#  define seedcommit_clean_ctx_x2_F  seedcommit_clean_ctx_pub_x2
#  define seedcommit_clean_ctx_x4_F  seedcommit_clean_ctx_pub_x4
#  define SeedCommit_F           SeedCommit_pub
#  define SeedCommit_x2_F        SeedCommit_pub_x2
#  define SeedCommit_x4_x4_F     SeedCommit_x4_x4_pub
#  define SeedCommit_x4_ecb_F    SeedCommit_x4_pub_ecb
#  define SeedCommit_x8_ecb_F    SeedCommit_x8_pub_ecb
#else
#  define SC_CTX          enc_ctx
#  define sc_key_sched    enc_key_sched
#  define sc_encrypt      enc_encrypt
#  define sc_clean_ctx    enc_clean_ctx
#  define sc_encrypt_x8   enc_encrypt_x8
#  define sc_encrypt_x4   enc_encrypt_x4
#  define SC_CTX_ECB         enc_ctx_ecb
#  define sc_encrypt_ecb     enc_encrypt_ecb
#  define seedcommit_ctx_x2_F    seedcommit_ctx_x2_t
#  define seedcommit_ctx_x4_F    seedcommit_ctx_x4_t
#  define init_seedcommit_x2_F   init_seedcommit_x2
#  define init_seedcommit_x4_F   init_seedcommit_x4
#  define seedcommit_clean_ctx_x2_F  seedcommit_clean_ctx_x2
#  define seedcommit_clean_ctx_x4_F  seedcommit_clean_ctx_x4
#  define SeedCommit_F           SeedCommit
#  define SeedCommit_x2_F        SeedCommit_x2
#  define SeedCommit_x4_x4_F     SeedCommit_x4_x4
#  define SeedCommit_x4_ecb_F    SeedCommit_x4_ecb
#  define SeedCommit_x8_ecb_F    SeedCommit_x8_ecb
#endif

/* Single-seed commit: key schedule recomputed every call (memopt: no caching).
 * Context type is the same for pub/non-pub since it holds only the salts. */
static inline int SeedCommit_F(seedcommit_ctx_t *ctx, const uint8_t seed[MQOM3_PARAM_SEED_SIZE], uint8_t seed_com[2 * MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	SC_CTX DECL_VAR(enc_context);

	ret = sc_key_sched(&enc_context, ctx->tweaked_salt[0]);
	ERR(ret, err);
	ret = sc_encrypt(&enc_context, seed, &seed_com[0]);
	ERR(ret, err);
	ret = sc_key_sched(&enc_context, ctx->tweaked_salt[1]);
	ERR(ret, err);
	ret = sc_encrypt(&enc_context, seed, &seed_com[MQOM3_PARAM_SEED_SIZE]);
	ERR(ret, err);

	uint8_t linortho_seed[MQOM3_PARAM_SEED_SIZE];
	LinOrtho(seed, linortho_seed);
	/* Xor with LinOrtho seed */
	xor_blocks(&seed_com[0], linortho_seed, &seed_com[0]);
	xor_blocks(&seed_com[MQOM3_PARAM_SEED_SIZE], linortho_seed, &seed_com[MQOM3_PARAM_SEED_SIZE]);

	ret = 0;
err:
	sc_clean_ctx(&enc_context);
#ifndef SEED_COMMIT_PUBLIC
	mqom_cleanse(linortho_seed, sizeof(linortho_seed));
#endif
	return ret;
}

/* x2: 2-seed wrapper, delegates to single-seed SeedCommit_F */
typedef struct {
	seedcommit_ctx_t ctx[2];
} seedcommit_ctx_x2_F;

static inline int init_seedcommit_x2_F(seedcommit_ctx_x2_F *ctx, const uint8_t tweaked_salts[4][MQOM3_PARAM_SALT_SIZE]) {
	int ret = 0;
	uint32_t i;
	for (i = 0; i < 2; i++) {
		ret = init_seedcommit(&ctx->ctx[i], tweaked_salts[2 * i], tweaked_salts[2 * i + 1]);
		ERR(ret, err);
	}
err:
	return ret;
}
static inline void seedcommit_clean_ctx_x2_F(seedcommit_ctx_x2_F *ctx) {
	uint32_t i;
	for (i = 0; i < 2; i++) {
		seedcommit_clean_ctx(&ctx->ctx[i]);
	}
}
static inline int SeedCommit_x2_F(seedcommit_ctx_x2_F *ctx, const uint8_t seed1[MQOM3_PARAM_SEED_SIZE], const uint8_t seed2[MQOM3_PARAM_SEED_SIZE], uint8_t seed_com1[2 * MQOM3_PARAM_SEED_SIZE], uint8_t seed_com2[2 * MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	ret = SeedCommit_F(&ctx->ctx[0], seed1, seed_com1);
	ERR(ret, err);
	ret = SeedCommit_F(&ctx->ctx[1], seed2, seed_com2);
	ERR(ret, err);
	ret = 0;
err:
	return ret;
}

/* x4: 4-seed wrapper, delegates to single-seed SeedCommit_F */
typedef struct {
	seedcommit_ctx_t ctx[4];
} seedcommit_ctx_x4_F;

static inline int init_seedcommit_x4_F(seedcommit_ctx_x4_F *ctx, const uint8_t tweaked_salts[8][MQOM3_PARAM_SALT_SIZE]) {
	int ret = 0;
	uint32_t i;
	for (i = 0; i < 4; i++) {
		ret = init_seedcommit(&ctx->ctx[i], tweaked_salts[2 * i], tweaked_salts[2 * i + 1]);
		ERR(ret, err);
	}
err:
	return ret;
}
static inline void seedcommit_clean_ctx_x4_F(seedcommit_ctx_x4_F *ctx) {
	uint32_t i;
	for (i = 0; i < 4; i++) {
		seedcommit_clean_ctx(&ctx->ctx[i]);
	}
}
static inline int SeedCommit_x4_x4_F(seedcommit_ctx_x4_F *ctx, const uint8_t seed1[MQOM3_PARAM_SEED_SIZE], const uint8_t seed2[MQOM3_PARAM_SEED_SIZE], const uint8_t seed3[MQOM3_PARAM_SEED_SIZE], const uint8_t seed4[MQOM3_PARAM_SEED_SIZE], uint8_t seed_com1[2 * MQOM3_PARAM_SEED_SIZE], uint8_t seed_com2[2 * MQOM3_PARAM_SEED_SIZE], uint8_t seed_com3[2 * MQOM3_PARAM_SEED_SIZE], uint8_t seed_com4[2 * MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	ret = SeedCommit_F(&ctx->ctx[0], seed1, seed_com1);
	ERR(ret, err);
	ret = SeedCommit_F(&ctx->ctx[1], seed2, seed_com2);
	ERR(ret, err);
	ret = SeedCommit_F(&ctx->ctx[2], seed3, seed_com3);
	ERR(ret, err);
	ret = SeedCommit_F(&ctx->ctx[3], seed4, seed_com4);
	ERR(ret, err);
	ret = 0;
err:
	return ret;
}

/* --- ECB x4: same execution, 4 seeds, same two key contexts for all seeds ---
 * Memopt variant: key schedule is computed by the caller (not cached in a ctx
 * struct).  The function body is identical to the default variant since it only
 * receives pre-computed SC_CTX_ECB pointers and calls sc_encrypt_ecb. */
static inline int SeedCommit_x4_ecb_F(SC_CTX_ECB *ctx1, SC_CTX_ECB *ctx2,
    const uint8_t seed1[MQOM3_PARAM_SEED_SIZE], const uint8_t seed2[MQOM3_PARAM_SEED_SIZE],
    const uint8_t seed3[MQOM3_PARAM_SEED_SIZE], const uint8_t seed4[MQOM3_PARAM_SEED_SIZE],
    uint8_t seed_com1[2 * MQOM3_PARAM_SEED_SIZE], uint8_t seed_com2[2 * MQOM3_PARAM_SEED_SIZE],
    uint8_t seed_com3[2 * MQOM3_PARAM_SEED_SIZE], uint8_t seed_com4[2 * MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	uint8_t linortho_seed1[MQOM3_PARAM_SEED_SIZE];
	uint8_t linortho_seed2[MQOM3_PARAM_SEED_SIZE];
	uint8_t linortho_seed3[MQOM3_PARAM_SEED_SIZE];
	uint8_t linortho_seed4[MQOM3_PARAM_SEED_SIZE];
	uint8_t seeds_flat[4 * MQOM3_PARAM_SEED_SIZE];
	uint8_t ct1_out[4 * MQOM3_PARAM_SEED_SIZE];
	uint8_t ct2_out[4 * MQOM3_PARAM_SEED_SIZE];

	LinOrtho(seed1, linortho_seed1);
	LinOrtho(seed2, linortho_seed2);
	LinOrtho(seed3, linortho_seed3);
	LinOrtho(seed4, linortho_seed4);

	memcpy(seeds_flat + (0 * MQOM3_PARAM_SEED_SIZE), seed1, MQOM3_PARAM_SEED_SIZE);
	memcpy(seeds_flat + (1 * MQOM3_PARAM_SEED_SIZE), seed2, MQOM3_PARAM_SEED_SIZE);
	memcpy(seeds_flat + (2 * MQOM3_PARAM_SEED_SIZE), seed3, MQOM3_PARAM_SEED_SIZE);
	memcpy(seeds_flat + (3 * MQOM3_PARAM_SEED_SIZE), seed4, MQOM3_PARAM_SEED_SIZE);

	ret = sc_encrypt_ecb(ctx1, 4, seeds_flat, ct1_out);
	ERR(ret, err);
	ret = sc_encrypt_ecb(ctx2, 4, seeds_flat, ct2_out);
	ERR(ret, err);

	xor_blocks(ct1_out + (0 * MQOM3_PARAM_SEED_SIZE), linortho_seed1, &seed_com1[0]);
	xor_blocks(ct2_out + (0 * MQOM3_PARAM_SEED_SIZE), linortho_seed1, &seed_com1[MQOM3_PARAM_SEED_SIZE]);
	xor_blocks(ct1_out + (1 * MQOM3_PARAM_SEED_SIZE), linortho_seed2, &seed_com2[0]);
	xor_blocks(ct2_out + (1 * MQOM3_PARAM_SEED_SIZE), linortho_seed2, &seed_com2[MQOM3_PARAM_SEED_SIZE]);
	xor_blocks(ct1_out + (2 * MQOM3_PARAM_SEED_SIZE), linortho_seed3, &seed_com3[0]);
	xor_blocks(ct2_out + (2 * MQOM3_PARAM_SEED_SIZE), linortho_seed3, &seed_com3[MQOM3_PARAM_SEED_SIZE]);
	xor_blocks(ct1_out + (3 * MQOM3_PARAM_SEED_SIZE), linortho_seed4, &seed_com4[0]);
	xor_blocks(ct2_out + (3 * MQOM3_PARAM_SEED_SIZE), linortho_seed4, &seed_com4[MQOM3_PARAM_SEED_SIZE]);

	ret = 0;
err:
	return ret;
}

/* --- ECB x8: same execution, 8 seeds, same two key contexts for all seeds ---
 * Memopt variant: body identical to the default variant (it only receives
 * pre-computed SC_CTX_ECB pointers).  See seed_commit_impl.h for the rationale
 * of the wider batch. */
static inline int SeedCommit_x8_ecb_F(SC_CTX_ECB *ctx1, SC_CTX_ECB *ctx2,
    const uint8_t seeds[8][MQOM3_PARAM_SEED_SIZE],
    uint8_t seed_com[8][2 * MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	uint32_t j;
#ifndef SEED_COMMIT_PUBLIC
	uint8_t sc_scratch[24 * MQOM3_PARAM_SEED_SIZE];
	uint8_t (*linortho_seeds)[MQOM3_PARAM_SEED_SIZE] = (uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) &sc_scratch[0];
	uint8_t *ct1_out = &sc_scratch[8 * MQOM3_PARAM_SEED_SIZE];
	uint8_t *ct2_out = &sc_scratch[16 * MQOM3_PARAM_SEED_SIZE];
#else
	uint8_t linortho_seeds[8][MQOM3_PARAM_SEED_SIZE];
	uint8_t ct1_out[8 * MQOM3_PARAM_SEED_SIZE];
	uint8_t ct2_out[8 * MQOM3_PARAM_SEED_SIZE];
#endif

	for (j = 0; j < 8; j++) {
		LinOrtho(seeds[j], linortho_seeds[j]);
	}

	ret = sc_encrypt_ecb(ctx1, 8, (const uint8_t*) seeds, ct1_out);
	ERR(ret, err);
	ret = sc_encrypt_ecb(ctx2, 8, (const uint8_t*) seeds, ct2_out);
	ERR(ret, err);

	for (j = 0; j < 8; j++) {
		xor_blocks(ct1_out + (j * MQOM3_PARAM_SEED_SIZE), linortho_seeds[j], &seed_com[j][0]);
		xor_blocks(ct2_out + (j * MQOM3_PARAM_SEED_SIZE), linortho_seeds[j], &seed_com[j][MQOM3_PARAM_SEED_SIZE]);
	}

	ret = 0;
err:
#ifndef SEED_COMMIT_PUBLIC
	mqom_cleanse(sc_scratch, sizeof(sc_scratch));
#endif
	return ret;
}

/* --- Macro cleanup --- */
#undef SC_CTX
#undef sc_key_sched
#undef sc_encrypt
#undef sc_clean_ctx
#undef sc_encrypt_x8
#undef sc_encrypt_x4
#undef SC_CTX_ECB
#undef sc_encrypt_ecb
#undef seedcommit_ctx_x2_F
#undef seedcommit_ctx_x4_F
#undef init_seedcommit_x2_F
#undef init_seedcommit_x4_F
#undef seedcommit_clean_ctx_x2_F
#undef seedcommit_clean_ctx_x4_F
#undef SeedCommit_F
#undef SeedCommit_x2_F
#undef SeedCommit_x4_x4_F
#undef SeedCommit_x4_ecb_F
#undef SeedCommit_x8_ecb_F
