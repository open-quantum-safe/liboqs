/* seed_commit_impl.h - X-macro template for seed_commit_default.h
 * No include guard: this file is included twice.
 *   pass 1 (SEED_COMMIT_PUBLIC undefined): generates non-pub functions/types
 *   pass 2 (#define SEED_COMMIT_PUBLIC):   generates pub functions/types
 *
 * Functions generated per pass:
 *   seedcommit_ctx_{t,pub_t}  + init/clean/SeedCommit
 *   seedcommit_ctx_{x2,pub_x2}_t  + init/clean/SeedCommit_x2
 *   seedcommit_ctx_{x4,pub_x4}_t  + init/clean/SeedCommit_x4_x4
 *   SeedCommit_x4_ecb / SeedCommit_x4_pub_ecb  (bare ECB ctx, no wrapper struct)
 */

#ifdef SEED_COMMIT_PUBLIC
#  define SC_CTX_X2              enc_ctx_pub_x2
#  define SC_CTX_X4              enc_ctx_pub_x4
#  define SC_CTX_X8              enc_ctx_pub_x8
#  define SC_CTX_ECB             enc_ctx_pub_ecb
#  define sc_key_sched_x2        enc_key_sched_pub_x2
#  define sc_key_sched_x4        enc_key_sched_pub_x4
#  define sc_key_sched_x8        enc_key_sched_pub_x8
#  define sc_key_sched_ecb       enc_key_sched_pub_ecb
#  define sc_clean_ctx_x2        enc_clean_ctx_pub_x2
#  define sc_clean_ctx_x4        enc_clean_ctx_pub_x4
#  define sc_clean_ctx_x8        enc_clean_ctx_pub_x8
#  define sc_encrypt_x2_x2       enc_encrypt_x2_pub_x2
#  define sc_encrypt_x4_x4       enc_encrypt_x4_pub_x4
#  define sc_encrypt_x8_x8       enc_encrypt_x8_pub_x8
#  define sc_encrypt_x4          enc_encrypt_x4_pub
#  define sc_encrypt_x8          enc_encrypt_x8_pub
#  define sc_encrypt_ecb         enc_encrypt_pub_ecb
#  define seedcommit_ctx_F       seedcommit_ctx_pub_t
#  define seedcommit_ctx_x2_F    seedcommit_ctx_pub_x2_t
#  define seedcommit_ctx_x4_F    seedcommit_ctx_pub_x4_t
#  define init_seedcommit_F      init_seedcommit_pub
#  define init_seedcommit_x2_F   init_seedcommit_pub_x2
#  define init_seedcommit_x4_F   init_seedcommit_pub_x4
#  define seedcommit_clean_ctx_F       seedcommit_clean_ctx_pub
#  define seedcommit_clean_ctx_x2_F    seedcommit_clean_ctx_pub_x2
#  define seedcommit_clean_ctx_x4_F    seedcommit_clean_ctx_pub_x4
#  define SeedCommit_F           SeedCommit_pub
#  define SeedCommit_x2_F        SeedCommit_pub_x2
#  define SeedCommit_x4_x4_F     SeedCommit_x4_x4_pub
#  define SeedCommit_x4_ecb_F    SeedCommit_x4_pub_ecb
#  define SeedCommit_x8_ecb_F    SeedCommit_x8_pub_ecb
#else
#  define SC_CTX_X2              enc_ctx_x2
#  define SC_CTX_X4              enc_ctx_x4
#  define SC_CTX_X8              enc_ctx_x8
#  define SC_CTX_ECB             enc_ctx_ecb
#  define sc_key_sched_x2        enc_key_sched_x2
#  define sc_key_sched_x4        enc_key_sched_x4
#  define sc_key_sched_x8        enc_key_sched_x8
#  define sc_key_sched_ecb       enc_key_sched_ecb
#  define sc_clean_ctx_x2        enc_clean_ctx_x2
#  define sc_clean_ctx_x4        enc_clean_ctx_x4
#  define sc_clean_ctx_x8        enc_clean_ctx_x8
#  define sc_encrypt_x2_x2       enc_encrypt_x2_x2
#  define sc_encrypt_x4_x4       enc_encrypt_x4_x4
#  define sc_encrypt_x8_x8       enc_encrypt_x8_x8
#  define sc_encrypt_x4          enc_encrypt_x4
#  define sc_encrypt_x8          enc_encrypt_x8
#  define sc_encrypt_ecb         enc_encrypt_ecb
#  define seedcommit_ctx_F       seedcommit_ctx_t
#  define seedcommit_ctx_x2_F    seedcommit_ctx_x2_t
#  define seedcommit_ctx_x4_F    seedcommit_ctx_x4_t
#  define init_seedcommit_F      init_seedcommit
#  define init_seedcommit_x2_F   init_seedcommit_x2
#  define init_seedcommit_x4_F   init_seedcommit_x4
#  define seedcommit_clean_ctx_F       seedcommit_clean_ctx
#  define seedcommit_clean_ctx_x2_F    seedcommit_clean_ctx_x2
#  define seedcommit_clean_ctx_x4_F    seedcommit_clean_ctx_x4
#  define SeedCommit_F           SeedCommit
#  define SeedCommit_x2_F        SeedCommit_x2
#  define SeedCommit_x4_x4_F     SeedCommit_x4_x4
#  define SeedCommit_x4_ecb_F    SeedCommit_x4_ecb
#  define SeedCommit_x8_ecb_F    SeedCommit_x8_ecb
#endif

/* --- Per-execution (1 seed) context: caches the two tweaked-key schedules --- */

/* XXX: NOTE: in this SeedCommit variant, we factorize the key schedules and save
 * them in the context to save cycles */
typedef struct {
	SC_CTX_X2 enc_ctx_x2;
} seedcommit_ctx_F;

static inline int init_seedcommit_F(seedcommit_ctx_F *ctx, const uint8_t tweaked_salt_0[MQOM3_PARAM_SALT_SIZE], const uint8_t tweaked_salt_1[MQOM3_PARAM_SALT_SIZE]) {
	int ret = -1;

	ret = sc_key_sched_x2(&ctx->enc_ctx_x2, tweaked_salt_0, tweaked_salt_1);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}
static inline void seedcommit_clean_ctx_F(seedcommit_ctx_F *ctx) {
	(void)ctx;
	sc_clean_ctx_x2(&ctx->enc_ctx_x2);
}
static inline int SeedCommit_F(seedcommit_ctx_F *ctx, const uint8_t seed[MQOM3_PARAM_SEED_SIZE], uint8_t seed_com[2 * MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	uint8_t linortho_seed[MQOM3_PARAM_SEED_SIZE];
	LinOrtho(seed, linortho_seed);
	ret = sc_encrypt_x2_x2(&ctx->enc_ctx_x2, seed, seed, &seed_com[0], &seed_com[MQOM3_PARAM_SEED_SIZE]);
	ERR(ret, err);
	/* Xor with LinOrtho seed */
	xor_blocks(&seed_com[0], linortho_seed, &seed_com[0]);
	xor_blocks(&seed_com[MQOM3_PARAM_SEED_SIZE], linortho_seed, &seed_com[MQOM3_PARAM_SEED_SIZE]);

	ret = 0;
err:
#ifndef SEED_COMMIT_PUBLIC
	mqom_cleanse(linortho_seed, sizeof(linortho_seed));
#endif
	return ret;
}

/* --- x2 context: 2 executions, 4 tweaked-key schedules cached --- */

typedef struct {
	SC_CTX_X4 enc_ctx_x4;
} seedcommit_ctx_x2_F;

static inline int init_seedcommit_x2_F(seedcommit_ctx_x2_F *ctx, const uint8_t tweaked_salts[4][MQOM3_PARAM_SALT_SIZE]) {
	int ret = -1;

	ret = sc_key_sched_x4(&ctx->enc_ctx_x4, tweaked_salts[0], tweaked_salts[1], tweaked_salts[2], tweaked_salts[3]);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}
static inline void seedcommit_clean_ctx_x2_F(seedcommit_ctx_x2_F *ctx) {
	(void)ctx;
	sc_clean_ctx_x4(&ctx->enc_ctx_x4);
}
static inline int SeedCommit_x2_F(seedcommit_ctx_x2_F *ctx, const uint8_t seed1[MQOM3_PARAM_SEED_SIZE], const uint8_t seed2[MQOM3_PARAM_SEED_SIZE], uint8_t seed_com1[2 * MQOM3_PARAM_SEED_SIZE], uint8_t seed_com2[2 * MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	uint8_t linortho_seed1[MQOM3_PARAM_SEED_SIZE];
	uint8_t linortho_seed2[MQOM3_PARAM_SEED_SIZE];
	LinOrtho(seed1, linortho_seed1);
	LinOrtho(seed2, linortho_seed2);
	ret = sc_encrypt_x4_x4(&ctx->enc_ctx_x4, seed1, seed1, seed2, seed2, &seed_com1[0], &seed_com1[MQOM3_PARAM_SEED_SIZE], &seed_com2[0], &seed_com2[MQOM3_PARAM_SEED_SIZE]);
	ERR(ret, err);

	/* Xor with LinOrtho seed */
	xor_blocks(&seed_com1[0], linortho_seed1, &seed_com1[0]);
	xor_blocks(&seed_com1[MQOM3_PARAM_SEED_SIZE], linortho_seed1, &seed_com1[MQOM3_PARAM_SEED_SIZE]);
	xor_blocks(&seed_com2[0], linortho_seed2, &seed_com2[0]);
	xor_blocks(&seed_com2[MQOM3_PARAM_SEED_SIZE], linortho_seed2, &seed_com2[MQOM3_PARAM_SEED_SIZE]);

	ret = 0;
err:
	return ret;
}

/* --- x4 context: 4 executions, 8 tweaked-key schedules cached --- */

typedef struct {
#ifdef USE_ENC_X8
	SC_CTX_X8 enc_ctx_x8;
#else
	SC_CTX_X4 enc_ctx_x4[2];
#endif
} seedcommit_ctx_x4_F;

static inline int init_seedcommit_x4_F(seedcommit_ctx_x4_F *ctx, const uint8_t tweaked_salts[8][MQOM3_PARAM_SALT_SIZE]) {
	int ret = -1;

#ifdef USE_ENC_X8
	ret = sc_key_sched_x8(&ctx->enc_ctx_x8, tweaked_salts[0], tweaked_salts[1], tweaked_salts[2], tweaked_salts[3], tweaked_salts[4], tweaked_salts[5], tweaked_salts[6], tweaked_salts[7]);
#else
	ret = sc_key_sched_x4(&ctx->enc_ctx_x4[0], tweaked_salts[0], tweaked_salts[1], tweaked_salts[2], tweaked_salts[3]);
	ERR(ret, err);
	ret = sc_key_sched_x4(&ctx->enc_ctx_x4[1], tweaked_salts[4], tweaked_salts[5], tweaked_salts[6], tweaked_salts[7]);
#endif
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}
static inline void seedcommit_clean_ctx_x4_F(seedcommit_ctx_x4_F *ctx) {
	(void)ctx;
#ifdef USE_ENC_X8
	sc_clean_ctx_x8(&ctx->enc_ctx_x8);
#else
	sc_clean_ctx_x4(&ctx->enc_ctx_x4[0]);
	sc_clean_ctx_x4(&ctx->enc_ctx_x4[1]);
#endif
}
static inline int SeedCommit_x4_x4_F(seedcommit_ctx_x4_F *ctx, const uint8_t seed1[MQOM3_PARAM_SEED_SIZE], const uint8_t seed2[MQOM3_PARAM_SEED_SIZE], const uint8_t seed3[MQOM3_PARAM_SEED_SIZE], const uint8_t seed4[MQOM3_PARAM_SEED_SIZE], uint8_t seed_com1[2 * MQOM3_PARAM_SEED_SIZE], uint8_t seed_com2[2 * MQOM3_PARAM_SEED_SIZE], uint8_t seed_com3[2 * MQOM3_PARAM_SEED_SIZE], uint8_t seed_com4[2 * MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	uint8_t linortho_seed1[MQOM3_PARAM_SEED_SIZE];
	uint8_t linortho_seed2[MQOM3_PARAM_SEED_SIZE];
	uint8_t linortho_seed3[MQOM3_PARAM_SEED_SIZE];
	uint8_t linortho_seed4[MQOM3_PARAM_SEED_SIZE];
	LinOrtho(seed1, linortho_seed1);
	LinOrtho(seed2, linortho_seed2);
	LinOrtho(seed3, linortho_seed3);
	LinOrtho(seed4, linortho_seed4);
#ifdef USE_ENC_X8
	ret = sc_encrypt_x8_x8(&ctx->enc_ctx_x8,
	               seed1, seed1, seed2, seed2, seed3, seed3, seed4, seed4,
	               &seed_com1[0], &seed_com1[MQOM3_PARAM_SEED_SIZE], &seed_com2[0], &seed_com2[MQOM3_PARAM_SEED_SIZE],
	               &seed_com3[0], &seed_com3[MQOM3_PARAM_SEED_SIZE], &seed_com4[0], &seed_com4[MQOM3_PARAM_SEED_SIZE]);
	ERR(ret, err);
#else
	ret = sc_encrypt_x4_x4(&ctx->enc_ctx_x4[0],
	               seed1, seed1, seed2, seed2,
	               &seed_com1[0], &seed_com1[MQOM3_PARAM_SEED_SIZE],
	               &seed_com2[0], &seed_com2[MQOM3_PARAM_SEED_SIZE]);
	ERR(ret, err);
	ret = sc_encrypt_x4_x4(&ctx->enc_ctx_x4[1],
	               seed3, seed3, seed4, seed4,
	               &seed_com3[0], &seed_com3[MQOM3_PARAM_SEED_SIZE],
	               &seed_com4[0], &seed_com4[MQOM3_PARAM_SEED_SIZE]);
	ERR(ret, err);
#endif
	/* Xor with LinOrtho seed */
	xor_blocks(&seed_com1[0], linortho_seed1, &seed_com1[0]);
	xor_blocks(&seed_com1[MQOM3_PARAM_SEED_SIZE], linortho_seed1, &seed_com1[MQOM3_PARAM_SEED_SIZE]);
	xor_blocks(&seed_com2[0], linortho_seed2, &seed_com2[0]);
	xor_blocks(&seed_com2[MQOM3_PARAM_SEED_SIZE], linortho_seed2, &seed_com2[MQOM3_PARAM_SEED_SIZE]);
	xor_blocks(&seed_com3[0], linortho_seed3, &seed_com3[0]);
	xor_blocks(&seed_com3[MQOM3_PARAM_SEED_SIZE], linortho_seed3, &seed_com3[MQOM3_PARAM_SEED_SIZE]);
	xor_blocks(&seed_com4[0], linortho_seed4, &seed_com4[0]);
	xor_blocks(&seed_com4[MQOM3_PARAM_SEED_SIZE], linortho_seed4, &seed_com4[MQOM3_PARAM_SEED_SIZE]);

	ret = 0;
err:
	return ret;
}

/* --- ECB x4: same execution, 4 seeds, same two key contexts for all seeds --- */

/* ECB variant: the same two tweaked-salt keys (ctx1, ctx2) apply to all 4 seeds,
 * allowing bulk parallel encryption via enc_encrypt_ecb. */
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
	/* Flat input buffer: 4 seeds packed contiguously for ECB bulk encrypt */
	uint8_t seeds_flat[4 * MQOM3_PARAM_SEED_SIZE];
	/* Output ciphertext buffers, one per tweaked-salt key */
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

	/* ECB encrypt 4 seeds with each tweaked-salt key in parallel */
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
 *
 * Wider sibling of SeedCommit_x4_ecb_F above.  Both the input seeds and the
 * output commitments are already contiguous at every call site (they are
 * slices of lseed[] and ls_com[]), so the input needs no packing at all and
 * the two bulk encryptions run on 8 blocks instead of 4.
 *
 * That halves the number of enc_encrypt_ecb calls per 8 leaves (4 -> 2) and,
 * on a SIMD AES backend, moves each call from the latency-bound 4-block
 * remainder shape to the full 8-block loop, which costs the same because the
 * AES rounds are dependency-bound rather than throughput-bound at that width.
 */
static inline int SeedCommit_x8_ecb_F(SC_CTX_ECB *ctx1, SC_CTX_ECB *ctx2,
    const uint8_t seeds[8][MQOM3_PARAM_SEED_SIZE],
    uint8_t seed_com[8][2 * MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	uint32_t j;
#ifndef SEED_COMMIT_PUBLIC
	uint8_t sc_scratch[24 * MQOM3_PARAM_SEED_SIZE];
	uint8_t (*linortho_seeds)[MQOM3_PARAM_SEED_SIZE] = (uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) &sc_scratch[0];
	/* Output ciphertext buffers, one per tweaked-salt key */
	uint8_t *ct1_out = &sc_scratch[8 * MQOM3_PARAM_SEED_SIZE];
	uint8_t *ct2_out = &sc_scratch[16 * MQOM3_PARAM_SEED_SIZE];
#else
	uint8_t linortho_seeds[8][MQOM3_PARAM_SEED_SIZE];
	/* Output ciphertext buffers, one per tweaked-salt key */
	uint8_t ct1_out[8 * MQOM3_PARAM_SEED_SIZE];
	uint8_t ct2_out[8 * MQOM3_PARAM_SEED_SIZE];
#endif

	for (j = 0; j < 8; j++) {
		LinOrtho(seeds[j], linortho_seeds[j]);
	}

	/* ECB encrypt the 8 seeds with each tweaked-salt key in parallel.
	 * seeds[0] already points at 8 contiguous blocks: no packing needed. */
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
	/* Signer side only: the verifier instantiation of this file only ever sees
	 * public material, so it pays nothing here. */
	mqom_cleanse(sc_scratch, sizeof(sc_scratch));
#endif
	return ret;
}

/* --- Macro cleanup --- */
#undef SC_CTX_X2
#undef SC_CTX_X4
#undef SC_CTX_X8
#undef SC_CTX_ECB
#undef sc_key_sched_x2
#undef sc_key_sched_x4
#undef sc_key_sched_x8
#undef sc_key_sched_ecb
#undef sc_clean_ctx_x2
#undef sc_clean_ctx_x4
#undef sc_clean_ctx_x8
#undef sc_encrypt_x2_x2
#undef sc_encrypt_x4_x4
#undef sc_encrypt_x8_x8
#undef sc_encrypt_x4
#undef sc_encrypt_x8
#undef sc_encrypt_ecb
#undef seedcommit_ctx_F
#undef seedcommit_ctx_x2_F
#undef seedcommit_ctx_x4_F
#undef init_seedcommit_F
#undef init_seedcommit_x2_F
#undef init_seedcommit_x4_F
#undef seedcommit_clean_ctx_F
#undef seedcommit_clean_ctx_x2_F
#undef seedcommit_clean_ctx_x4_F
#undef SeedCommit_F
#undef SeedCommit_x2_F
#undef SeedCommit_x4_x4_F
#undef SeedCommit_x4_ecb_F
#undef SeedCommit_x8_ecb_F
