#ifndef __SEED_COMMIT_MEMOPT_H__
#define __SEED_COMMIT_MEMOPT_H__

#include "enc.h"
#include "common.h"
#include "prg.h"


/* XXX: NOTE: in this SeedCommit variant, we *DO NOT* factorize the key schedules to save memory
 * in the context at the expanse of additional cycles  */

typedef struct {
	uint8_t tweaked_salt[2][MQOM2_PARAM_SALT_SIZE];
} seedcommit_ctx_t;

#define seedcommit_ctx_pub_t seedcommit_ctx_t

static inline int init_seedcommit(seedcommit_ctx_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], uint32_t e) {
	TweakSalt(salt, ctx->tweaked_salt[0], 0, e, 0);
	memcpy(ctx->tweaked_salt[1], ctx->tweaked_salt[0], MQOM2_PARAM_SALT_SIZE);
	ctx->tweaked_salt[1][0] ^= 0x01;

	return 0;
}
static inline void seedcommit_clean_ctx(seedcommit_ctx_t* ctx) {
	(void) ctx;
}
static inline void SeedCommit(seedcommit_ctx_t* ctx, const uint8_t seed[MQOM2_PARAM_SEED_SIZE], uint8_t seed_com[2 * MQOM2_PARAM_SEED_SIZE]) {
	enc_ctx DECL_VAR(enc_context);

        enc_key_sched(&enc_context, ctx->tweaked_salt[0]);
	enc_encrypt(&enc_context, seed, &seed_com[0]);
        enc_key_sched(&enc_context, ctx->tweaked_salt[1]);
	enc_encrypt(&enc_context, seed, &seed_com[MQOM2_PARAM_SEED_SIZE]);

	uint8_t linortho_seed[MQOM2_PARAM_SEED_SIZE];
	LinOrtho(seed, linortho_seed);
	/* Xor with LinOrtho seed */
	xor_blocks(&seed_com[0], linortho_seed, &seed_com[0]);
	xor_blocks(&seed_com[MQOM2_PARAM_SEED_SIZE], linortho_seed, &seed_com[MQOM2_PARAM_SEED_SIZE]);

	enc_clean_ctx(&enc_context);

	return;
}

#define init_seedcommit_pub init_seedcommit
#define seedcommit_clean_ctx_pub seedcommit_clean_ctx
static inline void SeedCommit_pub(seedcommit_ctx_pub_t* ctx, const uint8_t seed[MQOM2_PARAM_SEED_SIZE], uint8_t seed_com[2 * MQOM2_PARAM_SEED_SIZE]) {
	enc_ctx_pub DECL_VAR(enc_context);

        enc_key_sched_pub(&enc_context, ctx->tweaked_salt[0]);
	enc_encrypt_pub(&enc_context, seed, &seed_com[0]);
        enc_key_sched_pub(&enc_context, ctx->tweaked_salt[1]);
	enc_encrypt_pub(&enc_context, seed, &seed_com[MQOM2_PARAM_SEED_SIZE]);

	uint8_t linortho_seed[MQOM2_PARAM_SEED_SIZE];
	LinOrtho(seed, linortho_seed);
	/* Xor with LinOrtho seed */
	xor_blocks(&seed_com[0], linortho_seed, &seed_com[0]);
	xor_blocks(&seed_com[MQOM2_PARAM_SEED_SIZE], linortho_seed, &seed_com[MQOM2_PARAM_SEED_SIZE]);

	enc_clean_ctx_pub(&enc_context);

	return;
}

/***/
typedef struct {
	seedcommit_ctx_t ctx[2];
} seedcommit_ctx_x2_t;
typedef struct {
	seedcommit_ctx_pub_t ctx[2];
} seedcommit_ctx_pub_x2_t;

static inline int init_seedcommit_x2(seedcommit_ctx_x2_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e) {
	int ret = 0;
	uint32_t i;
	for(i = 0; i < 2; i++){
		ret |= init_seedcommit(&ctx->ctx[i], salt, e[i]);
	}
	return ret;
}
static inline void seedcommit_clean_ctx_x2(seedcommit_ctx_x2_t* ctx) {
	uint32_t i;
	for(i = 0; i < 2; i++){
		seedcommit_clean_ctx(&ctx->ctx[i]);
	}
	return;
}
static inline void SeedCommit_x2(seedcommit_ctx_x2_t* ctx, const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE], uint8_t seed_com1[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com2[2 * MQOM2_PARAM_SEED_SIZE]) {
	SeedCommit(&ctx->ctx[0], seed1, seed_com1);
	SeedCommit(&ctx->ctx[1], seed2, seed_com2);
	return;
}

static inline int init_seedcommit_pub_x2(seedcommit_ctx_pub_x2_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e) {
	int ret = 0;
	uint32_t i;
	for(i = 0; i < 2; i++){
		ret |= init_seedcommit_pub(&ctx->ctx[i], salt, e[i]);
	}
	return ret;
}
static inline void seedcommit_clean_ctx_pub_x2(seedcommit_ctx_pub_x2_t* ctx) {
	uint32_t i;
	for(i = 0; i < 2; i++){
		seedcommit_clean_ctx_pub(&ctx->ctx[i]);
	}
	return;
}
static inline void SeedCommit_pub_x2(seedcommit_ctx_pub_x2_t* ctx, const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE], uint8_t seed_com1[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com2[2 * MQOM2_PARAM_SEED_SIZE]) {
        SeedCommit_pub(&ctx->ctx[0], seed1, seed_com1);
        SeedCommit_pub(&ctx->ctx[1], seed2, seed_com2);
        return;
}


/****/
typedef struct {
	seedcommit_ctx_t ctx[4];
} seedcommit_ctx_x4_t;
typedef struct {
	seedcommit_ctx_pub_t ctx[4];
} seedcommit_ctx_pub_x4_t;

static inline int init_seedcommit_x4(seedcommit_ctx_x4_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e) {
	int ret = 0;
	uint32_t i;
	for(i = 0; i < 4; i++){
		ret |= init_seedcommit(&ctx->ctx[i], salt, e[i]);
	}
	return ret;
}
static inline void seedcommit_clean_ctx_x4(seedcommit_ctx_x4_t* ctx) {
	uint32_t i;
	for(i = 0; i < 4; i++){
		seedcommit_clean_ctx(&ctx->ctx[i]);
	}
	return;
}
static inline void SeedCommit_x4_x4(seedcommit_ctx_x4_t *ctx, const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE], const uint8_t seed3[MQOM2_PARAM_SEED_SIZE], const uint8_t seed4[MQOM2_PARAM_SEED_SIZE], uint8_t seed_com1[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com2[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com3[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com4[2 * MQOM2_PARAM_SEED_SIZE]) {
	SeedCommit(&ctx->ctx[0], seed1, seed_com1);
	SeedCommit(&ctx->ctx[1], seed2, seed_com2);
	SeedCommit(&ctx->ctx[2], seed3, seed_com3);
	SeedCommit(&ctx->ctx[3], seed4, seed_com4);
	return;
}


static inline int init_seedcommit_pub_x4(seedcommit_ctx_pub_x4_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e) {
	int ret = 0;
	uint32_t i;
	for(i = 0; i < 4; i++){
		ret |= init_seedcommit_pub(&ctx->ctx[i], salt, e[i]);
	}
	return ret;
}
static inline void seedcommit_clean_ctx_pub_x4(seedcommit_ctx_pub_x4_t* ctx) {
	uint32_t i;
	for(i = 0; i < 4; i++){
		seedcommit_clean_ctx_pub(&ctx->ctx[i]);
	}
	return;
}
static inline void SeedCommit_x4_x4_pub(seedcommit_ctx_pub_x4_t *ctx, const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE], const uint8_t seed3[MQOM2_PARAM_SEED_SIZE], const uint8_t seed4[MQOM2_PARAM_SEED_SIZE], uint8_t seed_com1[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com2[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com3[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com4[2 * MQOM2_PARAM_SEED_SIZE]) {
	SeedCommit_pub(&ctx->ctx[0], seed1, seed_com1);
	SeedCommit_pub(&ctx->ctx[1], seed2, seed_com2);
	SeedCommit_pub(&ctx->ctx[2], seed3, seed_com3);
	SeedCommit_pub(&ctx->ctx[3], seed4, seed_com4);
	return;
}

static inline void SeedCommit_x4(enc_ctx *ctx1, enc_ctx *ctx2, const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE], const uint8_t seed3[MQOM2_PARAM_SEED_SIZE], const uint8_t seed4[MQOM2_PARAM_SEED_SIZE], uint8_t seed_com1[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com2[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com3[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com4[2 * MQOM2_PARAM_SEED_SIZE]) {
        uint8_t linortho_seed1[MQOM2_PARAM_SEED_SIZE];
        uint8_t linortho_seed2[MQOM2_PARAM_SEED_SIZE];
        uint8_t linortho_seed3[MQOM2_PARAM_SEED_SIZE];
        uint8_t linortho_seed4[MQOM2_PARAM_SEED_SIZE];
        LinOrtho(seed1, linortho_seed1);
        LinOrtho(seed2, linortho_seed2);
        LinOrtho(seed3, linortho_seed3);
        LinOrtho(seed4, linortho_seed4);
#ifdef USE_ENC_X8
        enc_encrypt_x8(ctx1, ctx2, ctx1, ctx2, ctx1, ctx2, ctx1, ctx2,
                       seed1, seed1, seed2, seed2, seed3, seed3, seed4, seed4,
                       &seed_com1[0], &seed_com1[MQOM2_PARAM_SEED_SIZE], &seed_com2[0], &seed_com2[MQOM2_PARAM_SEED_SIZE],
                       &seed_com3[0], &seed_com3[MQOM2_PARAM_SEED_SIZE], &seed_com4[0], &seed_com4[MQOM2_PARAM_SEED_SIZE]);
#else
        enc_encrypt_x4(ctx1, ctx2, ctx1, ctx2,
                       seed1, seed1, seed2, seed2,
                       &seed_com1[0], &seed_com1[MQOM2_PARAM_SEED_SIZE],
                       &seed_com2[0], &seed_com2[MQOM2_PARAM_SEED_SIZE]);
        enc_encrypt_x4(ctx1, ctx2, ctx1, ctx2,
                       seed3, seed3, seed4, seed4,
                       &seed_com3[0], &seed_com3[MQOM2_PARAM_SEED_SIZE],
                       &seed_com4[0], &seed_com4[MQOM2_PARAM_SEED_SIZE]);
#endif
        /* Xor with LinOrtho seed */
        xor_blocks(&seed_com1[0], linortho_seed1, &seed_com1[0]);
        xor_blocks(&seed_com1[MQOM2_PARAM_SEED_SIZE], linortho_seed1, &seed_com1[MQOM2_PARAM_SEED_SIZE]);
        xor_blocks(&seed_com2[0], linortho_seed2, &seed_com2[0]);
        xor_blocks(&seed_com2[MQOM2_PARAM_SEED_SIZE], linortho_seed2, &seed_com2[MQOM2_PARAM_SEED_SIZE]);
        xor_blocks(&seed_com3[0], linortho_seed3, &seed_com3[0]);
        xor_blocks(&seed_com3[MQOM2_PARAM_SEED_SIZE], linortho_seed3, &seed_com3[MQOM2_PARAM_SEED_SIZE]);
        xor_blocks(&seed_com4[0], linortho_seed4, &seed_com4[0]);
        xor_blocks(&seed_com4[MQOM2_PARAM_SEED_SIZE], linortho_seed4, &seed_com4[MQOM2_PARAM_SEED_SIZE]);
	
        return;
}
static inline void SeedCommit_x4_pub(enc_ctx_pub *ctx1, enc_ctx_pub *ctx2, const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE], const uint8_t seed3[MQOM2_PARAM_SEED_SIZE], const uint8_t seed4[MQOM2_PARAM_SEED_SIZE], uint8_t seed_com1[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com2[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com3[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com4[2 * MQOM2_PARAM_SEED_SIZE]) {
        uint8_t linortho_seed1[MQOM2_PARAM_SEED_SIZE];
        uint8_t linortho_seed2[MQOM2_PARAM_SEED_SIZE];
        uint8_t linortho_seed3[MQOM2_PARAM_SEED_SIZE];
        uint8_t linortho_seed4[MQOM2_PARAM_SEED_SIZE];
        LinOrtho(seed1, linortho_seed1);
        LinOrtho(seed2, linortho_seed2);
        LinOrtho(seed3, linortho_seed3);
        LinOrtho(seed4, linortho_seed4);
#ifdef USE_ENC_X8
        enc_encrypt_x8_pub(ctx1, ctx2, ctx1, ctx2, ctx1, ctx2, ctx1, ctx2,
                           seed1, seed1, seed2, seed2, seed3, seed3, seed4, seed4,
                           &seed_com1[0], &seed_com1[MQOM2_PARAM_SEED_SIZE], &seed_com2[0], &seed_com2[MQOM2_PARAM_SEED_SIZE],
                           &seed_com3[0], &seed_com3[MQOM2_PARAM_SEED_SIZE], &seed_com4[0], &seed_com4[MQOM2_PARAM_SEED_SIZE]);
#else
        enc_encrypt_x4_pub(ctx1, ctx2, ctx1, ctx2,
                           seed1, seed1, seed2, seed2,
                           &seed_com1[0], &seed_com1[MQOM2_PARAM_SEED_SIZE],
                           &seed_com2[0], &seed_com2[MQOM2_PARAM_SEED_SIZE]);
        enc_encrypt_x4_pub(ctx1, ctx2, ctx1, ctx2,
                           seed3, seed3, seed4, seed4,
                           &seed_com3[0], &seed_com3[MQOM2_PARAM_SEED_SIZE],
                           &seed_com4[0], &seed_com4[MQOM2_PARAM_SEED_SIZE]);
#endif
        /* Xor with LinOrtho seed */
        xor_blocks(&seed_com1[0], linortho_seed1, &seed_com1[0]);
        xor_blocks(&seed_com1[MQOM2_PARAM_SEED_SIZE], linortho_seed1, &seed_com1[MQOM2_PARAM_SEED_SIZE]);
        xor_blocks(&seed_com2[0], linortho_seed2, &seed_com2[0]);
        xor_blocks(&seed_com2[MQOM2_PARAM_SEED_SIZE], linortho_seed2, &seed_com2[MQOM2_PARAM_SEED_SIZE]);
        xor_blocks(&seed_com3[0], linortho_seed3, &seed_com3[0]);
        xor_blocks(&seed_com3[MQOM2_PARAM_SEED_SIZE], linortho_seed3, &seed_com3[MQOM2_PARAM_SEED_SIZE]);
        xor_blocks(&seed_com4[0], linortho_seed4, &seed_com4[0]);
        xor_blocks(&seed_com4[MQOM2_PARAM_SEED_SIZE], linortho_seed4, &seed_com4[MQOM2_PARAM_SEED_SIZE]);

        return;
}

#endif /* __SEED_COMMIT_MEMOPT_H__ */
