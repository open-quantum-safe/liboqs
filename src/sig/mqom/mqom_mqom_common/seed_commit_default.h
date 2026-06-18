#ifndef __SEED_COMMIT_DEFAULT_H__
#define __SEED_COMMIT_DEFAULT_H__

#include "enc.h"
#include "common.h"
#include "prg.h"

/* XXX: NOTE: in this SeedCommit variant, we factorize the key schedules and save
 * them in the context to save cycles */
typedef struct {
	enc_ctx_x2 enc_ctx_x2;
} seedcommit_ctx_t;

typedef struct {
	enc_ctx_pub_x2 enc_ctx_x2;
} seedcommit_ctx_pub_t;

static inline int init_seedcommit(seedcommit_ctx_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], uint32_t e) {
	int ret = -1;
	uint8_t tweaked_salt[2][MQOM2_PARAM_SALT_SIZE];

	TweakSalt(salt, tweaked_salt[0], 0, e, 0);
	memcpy(tweaked_salt[1], tweaked_salt[0], MQOM2_PARAM_SALT_SIZE);
	tweaked_salt[1][0] ^= 0x01;

	ret = enc_key_sched_x2(&ctx->enc_ctx_x2, tweaked_salt[0], tweaked_salt[1]);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}
static inline void seedcommit_clean_ctx(seedcommit_ctx_t* ctx) {
	(void)ctx;
	enc_clean_ctx_x2(&ctx->enc_ctx_x2);
}
static inline void SeedCommit(seedcommit_ctx_t* ctx, const uint8_t seed[MQOM2_PARAM_SEED_SIZE], uint8_t seed_com[2 * MQOM2_PARAM_SEED_SIZE]) {
	uint8_t linortho_seed[MQOM2_PARAM_SEED_SIZE];
	LinOrtho(seed, linortho_seed);
	enc_encrypt_x2_x2(&ctx->enc_ctx_x2, seed, seed, &seed_com[0], &seed_com[MQOM2_PARAM_SEED_SIZE]);
	/* Xor with LinOrtho seed */
	xor_blocks(&seed_com[0], linortho_seed, &seed_com[0]);
	xor_blocks(&seed_com[MQOM2_PARAM_SEED_SIZE], linortho_seed, &seed_com[MQOM2_PARAM_SEED_SIZE]);

	return;
}

static inline int init_seedcommit_pub(seedcommit_ctx_pub_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], uint32_t e) {
	int ret = -1;
	uint8_t tweaked_salt[2][MQOM2_PARAM_SALT_SIZE];

	TweakSalt(salt, tweaked_salt[0], 0, e, 0);
	memcpy(tweaked_salt[1], tweaked_salt[0], MQOM2_PARAM_SALT_SIZE);
	tweaked_salt[1][0] ^= 0x01;

	ret = enc_key_sched_pub_x2(&ctx->enc_ctx_x2, tweaked_salt[0], tweaked_salt[1]);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}
static inline void seedcommit_clean_ctx_pub(seedcommit_ctx_pub_t* ctx) {
	(void)ctx;
	enc_clean_ctx_pub_x2(&ctx->enc_ctx_x2);
}
static inline void SeedCommit_pub(seedcommit_ctx_pub_t* ctx, const uint8_t seed[MQOM2_PARAM_SEED_SIZE], uint8_t seed_com[2 * MQOM2_PARAM_SEED_SIZE]) {
	uint8_t linortho_seed[MQOM2_PARAM_SEED_SIZE];
	LinOrtho(seed, linortho_seed);
	enc_encrypt_x2_pub_x2(&ctx->enc_ctx_x2, seed, seed, &seed_com[0], &seed_com[MQOM2_PARAM_SEED_SIZE]);
	/* Xor with LinOrtho seed */
	xor_blocks(&seed_com[0], linortho_seed, &seed_com[0]);
	xor_blocks(&seed_com[MQOM2_PARAM_SEED_SIZE], linortho_seed, &seed_com[MQOM2_PARAM_SEED_SIZE]);

	return;
}

typedef struct {
	enc_ctx_x4 enc_ctx_x4;
} seedcommit_ctx_x2_t;

typedef struct {
	enc_ctx_pub_x4 enc_ctx_x4;
} seedcommit_ctx_pub_x2_t;

static inline int init_seedcommit_x2(seedcommit_ctx_x2_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e) {
	int ret = -1;
	uint8_t tweaked_salt[4][MQOM2_PARAM_SALT_SIZE];

	TweakSalt(salt, tweaked_salt[0], 0, e[0], 0);
	memcpy(tweaked_salt[1], tweaked_salt[0], MQOM2_PARAM_SALT_SIZE);
	tweaked_salt[1][0] ^= 0x01;

	TweakSalt(salt, tweaked_salt[2], 0, e[1], 0);
	memcpy(tweaked_salt[3], tweaked_salt[2], MQOM2_PARAM_SALT_SIZE);
	tweaked_salt[3][0] ^= 0x01;
	
	ret = enc_key_sched_x4(&ctx->enc_ctx_x4, tweaked_salt[0], tweaked_salt[1], tweaked_salt[2], tweaked_salt[3]);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}
static inline void seedcommit_clean_ctx_x2(seedcommit_ctx_x2_t* ctx) {
	(void)ctx;
	enc_clean_ctx_x4(&ctx->enc_ctx_x4);
}
static inline void SeedCommit_x2(seedcommit_ctx_x2_t* ctx, const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE], uint8_t seed_com1[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com2[2 * MQOM2_PARAM_SEED_SIZE]) {
	uint8_t linortho_seed1[MQOM2_PARAM_SEED_SIZE];
	uint8_t linortho_seed2[MQOM2_PARAM_SEED_SIZE];
	LinOrtho(seed1, linortho_seed1);
	LinOrtho(seed2, linortho_seed2);
	enc_encrypt_x4_x4(&ctx->enc_ctx_x4, seed1, seed1, seed2, seed2, &seed_com1[0], &seed_com1[MQOM2_PARAM_SEED_SIZE], &seed_com2[0], &seed_com2[MQOM2_PARAM_SEED_SIZE]);
	
	/* Xor with LinOrtho seed */
	xor_blocks(&seed_com1[0], linortho_seed1, &seed_com1[0]);
	xor_blocks(&seed_com1[MQOM2_PARAM_SEED_SIZE], linortho_seed1, &seed_com1[MQOM2_PARAM_SEED_SIZE]);
	xor_blocks(&seed_com2[0], linortho_seed2, &seed_com2[0]);
	xor_blocks(&seed_com2[MQOM2_PARAM_SEED_SIZE], linortho_seed2, &seed_com2[MQOM2_PARAM_SEED_SIZE]);

	return;
}

static inline int init_seedcommit_pub_x2(seedcommit_ctx_pub_x2_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e) {
	int ret = -1;
	uint8_t tweaked_salt[4][MQOM2_PARAM_SALT_SIZE];

	TweakSalt(salt, tweaked_salt[0], 0, e[0], 0);
	memcpy(tweaked_salt[1], tweaked_salt[0], MQOM2_PARAM_SALT_SIZE);
	tweaked_salt[1][0] ^= 0x01;

	TweakSalt(salt, tweaked_salt[2], 0, e[1], 0);
	memcpy(tweaked_salt[3], tweaked_salt[2], MQOM2_PARAM_SALT_SIZE);
	tweaked_salt[3][0] ^= 0x01;

	ret = enc_key_sched_pub_x4(&ctx->enc_ctx_x4, tweaked_salt[0], tweaked_salt[1], tweaked_salt[2], tweaked_salt[3]);
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}
static inline void seedcommit_clean_ctx_pub_x2(seedcommit_ctx_pub_x2_t* ctx) {
	(void)ctx;
	enc_clean_ctx_pub_x4(&ctx->enc_ctx_x4);
}
static inline void SeedCommit_pub_x2(seedcommit_ctx_pub_x2_t* ctx, const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE], uint8_t seed_com1[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com2[2 * MQOM2_PARAM_SEED_SIZE]) {
	uint8_t linortho_seed1[MQOM2_PARAM_SEED_SIZE];
	uint8_t linortho_seed2[MQOM2_PARAM_SEED_SIZE];
	LinOrtho(seed1, linortho_seed1);
	LinOrtho(seed2, linortho_seed2);
	enc_encrypt_x4_pub_x4(&ctx->enc_ctx_x4, seed1, seed1, seed2, seed2, &seed_com1[0], &seed_com1[MQOM2_PARAM_SEED_SIZE], &seed_com2[0], &seed_com2[MQOM2_PARAM_SEED_SIZE]);

	/* Xor with LinOrtho seed */
	xor_blocks(&seed_com1[0], linortho_seed1, &seed_com1[0]);
	xor_blocks(&seed_com1[MQOM2_PARAM_SEED_SIZE], linortho_seed1, &seed_com1[MQOM2_PARAM_SEED_SIZE]);
	xor_blocks(&seed_com2[0], linortho_seed2, &seed_com2[0]);
	xor_blocks(&seed_com2[MQOM2_PARAM_SEED_SIZE], linortho_seed2, &seed_com2[MQOM2_PARAM_SEED_SIZE]);

	return;
}

typedef struct {
#ifdef USE_ENC_X8
	enc_ctx_x8 enc_ctx_x8;
#else
	enc_ctx_x4 enc_ctx_x4[2];
#endif
} seedcommit_ctx_x4_t;

typedef struct {
#ifdef USE_ENC_X8
	enc_ctx_pub_x8 enc_ctx_x8;
#else
	enc_ctx_pub_x4 enc_ctx_x4[2];
#endif
} seedcommit_ctx_pub_x4_t;

static inline int init_seedcommit_x4(seedcommit_ctx_x4_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e) {
	int ret = -1;
	uint8_t tweaked_salt[8][MQOM2_PARAM_SALT_SIZE];

	TweakSalt(salt, tweaked_salt[0], 0, e[0], 0);
	memcpy(tweaked_salt[1], tweaked_salt[0], MQOM2_PARAM_SALT_SIZE);
	tweaked_salt[1][0] ^= 0x01;

	TweakSalt(salt, tweaked_salt[2], 0, e[1], 0);
	memcpy(tweaked_salt[3], tweaked_salt[2], MQOM2_PARAM_SALT_SIZE);
	tweaked_salt[3][0] ^= 0x01;

	TweakSalt(salt, tweaked_salt[4], 0, e[2], 0);
	memcpy(tweaked_salt[5], tweaked_salt[4], MQOM2_PARAM_SALT_SIZE);
	tweaked_salt[5][0] ^= 0x01;

	TweakSalt(salt, tweaked_salt[6], 0, e[3], 0);
	memcpy(tweaked_salt[7], tweaked_salt[6], MQOM2_PARAM_SALT_SIZE);
	tweaked_salt[7][0] ^= 0x01;

#ifdef USE_ENC_X8
	ret = enc_key_sched_x8(&ctx->enc_ctx_x8, tweaked_salt[0], tweaked_salt[1], tweaked_salt[2], tweaked_salt[3], tweaked_salt[4], tweaked_salt[5], tweaked_salt[6], tweaked_salt[7]);
#else
	ret = enc_key_sched_x4(&ctx->enc_ctx_x4[0], tweaked_salt[0], tweaked_salt[1], tweaked_salt[2], tweaked_salt[3]);
	ret = enc_key_sched_x4(&ctx->enc_ctx_x4[1], tweaked_salt[4], tweaked_salt[5], tweaked_salt[6], tweaked_salt[7]);
#endif
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}
static inline void seedcommit_clean_ctx_x4(seedcommit_ctx_x4_t* ctx) {
	(void) ctx;
#ifdef USE_ENC_X8
	enc_clean_ctx_x8(&ctx->enc_ctx_x8);
#else
	enc_clean_ctx_x4(&ctx->enc_ctx_x4[0]);
	enc_clean_ctx_x4(&ctx->enc_ctx_x4[1]);
#endif
}
static inline void SeedCommit_x4_x4(seedcommit_ctx_x4_t *ctx, const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE], const uint8_t seed3[MQOM2_PARAM_SEED_SIZE], const uint8_t seed4[MQOM2_PARAM_SEED_SIZE], uint8_t seed_com1[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com2[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com3[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com4[2 * MQOM2_PARAM_SEED_SIZE]) {
	uint8_t linortho_seed1[MQOM2_PARAM_SEED_SIZE];
	uint8_t linortho_seed2[MQOM2_PARAM_SEED_SIZE];
	uint8_t linortho_seed3[MQOM2_PARAM_SEED_SIZE];
	uint8_t linortho_seed4[MQOM2_PARAM_SEED_SIZE];
	LinOrtho(seed1, linortho_seed1);
	LinOrtho(seed2, linortho_seed2);
	LinOrtho(seed3, linortho_seed3);
	LinOrtho(seed4, linortho_seed4);
#ifdef USE_ENC_X8
	enc_encrypt_x8_x8(&ctx->enc_ctx_x8,
	               seed1, seed1, seed2, seed2, seed3, seed3, seed4, seed4,
	               &seed_com1[0], &seed_com1[MQOM2_PARAM_SEED_SIZE], &seed_com2[0], &seed_com2[MQOM2_PARAM_SEED_SIZE],
	               &seed_com3[0], &seed_com3[MQOM2_PARAM_SEED_SIZE], &seed_com4[0], &seed_com4[MQOM2_PARAM_SEED_SIZE]);
#else
	enc_encrypt_x4_x4(&ctx->enc_ctx_x4[0],
	               seed1, seed1, seed2, seed2,
	               &seed_com1[0], &seed_com1[MQOM2_PARAM_SEED_SIZE],
	               &seed_com2[0], &seed_com2[MQOM2_PARAM_SEED_SIZE]);
	enc_encrypt_x4_x4(&ctx->enc_ctx_x4[1],
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

static inline int init_seedcommit_pub_x4(seedcommit_ctx_pub_x4_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint32_t* e) {
	int ret = -1;
	uint8_t tweaked_salt[8][MQOM2_PARAM_SALT_SIZE];

	TweakSalt(salt, tweaked_salt[0], 0, e[0], 0);
	memcpy(tweaked_salt[1], tweaked_salt[0], MQOM2_PARAM_SALT_SIZE);
	tweaked_salt[1][0] ^= 0x01;

	TweakSalt(salt, tweaked_salt[2], 0, e[1], 0);
	memcpy(tweaked_salt[3], tweaked_salt[2], MQOM2_PARAM_SALT_SIZE);
	tweaked_salt[3][0] ^= 0x01;

	TweakSalt(salt, tweaked_salt[4], 0, e[2], 0);
	memcpy(tweaked_salt[5], tweaked_salt[4], MQOM2_PARAM_SALT_SIZE);
	tweaked_salt[5][0] ^= 0x01;

	TweakSalt(salt, tweaked_salt[6], 0, e[3], 0);
	memcpy(tweaked_salt[7], tweaked_salt[6], MQOM2_PARAM_SALT_SIZE);
	tweaked_salt[7][0] ^= 0x01;

#ifdef USE_ENC_X8
	ret = enc_key_sched_pub_x8(&ctx->enc_ctx_x8, tweaked_salt[0], tweaked_salt[1], tweaked_salt[2], tweaked_salt[3], tweaked_salt[4], tweaked_salt[5], tweaked_salt[6], tweaked_salt[7]);
#else
	ret = enc_key_sched_pub_x4(&ctx->enc_ctx_x4[0], tweaked_salt[0], tweaked_salt[1], tweaked_salt[2], tweaked_salt[3]);
	ret = enc_key_sched_pub_x4(&ctx->enc_ctx_x4[1], tweaked_salt[4], tweaked_salt[5], tweaked_salt[6], tweaked_salt[7]);
#endif
	ERR(ret, err);

	ret = 0;
err:
	return ret;
}
static inline void seedcommit_clean_ctx_pub_x4(seedcommit_ctx_pub_x4_t* ctx) {
	(void) ctx;
#ifdef USE_ENC_X8
	enc_clean_ctx_pub_x8(&ctx->enc_ctx_x8);
#else
	enc_clean_ctx_pub_x4(&ctx->enc_ctx_x4[0]);
	enc_clean_ctx_pub_x4(&ctx->enc_ctx_x4[1]);
#endif
}
static inline void SeedCommit_x4_x4_pub(seedcommit_ctx_pub_x4_t *ctx, const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE], const uint8_t seed3[MQOM2_PARAM_SEED_SIZE], const uint8_t seed4[MQOM2_PARAM_SEED_SIZE], uint8_t seed_com1[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com2[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com3[2 * MQOM2_PARAM_SEED_SIZE], uint8_t seed_com4[2 * MQOM2_PARAM_SEED_SIZE]) {
	uint8_t linortho_seed1[MQOM2_PARAM_SEED_SIZE];
	uint8_t linortho_seed2[MQOM2_PARAM_SEED_SIZE];
	uint8_t linortho_seed3[MQOM2_PARAM_SEED_SIZE];
	uint8_t linortho_seed4[MQOM2_PARAM_SEED_SIZE];
	LinOrtho(seed1, linortho_seed1);
	LinOrtho(seed2, linortho_seed2);
	LinOrtho(seed3, linortho_seed3);
	LinOrtho(seed4, linortho_seed4);
#ifdef USE_ENC_X8
	enc_encrypt_x8_pub_x8(&ctx->enc_ctx_x8,
	               seed1, seed1, seed2, seed2, seed3, seed3, seed4, seed4,
	               &seed_com1[0], &seed_com1[MQOM2_PARAM_SEED_SIZE], &seed_com2[0], &seed_com2[MQOM2_PARAM_SEED_SIZE],
	               &seed_com3[0], &seed_com3[MQOM2_PARAM_SEED_SIZE], &seed_com4[0], &seed_com4[MQOM2_PARAM_SEED_SIZE]);
#else
	enc_encrypt_x4_pub_x4(&ctx->enc_ctx_x4[0],
	               seed1, seed1, seed2, seed2,
	               &seed_com1[0], &seed_com1[MQOM2_PARAM_SEED_SIZE],
	               &seed_com2[0], &seed_com2[MQOM2_PARAM_SEED_SIZE]);
	enc_encrypt_x4_pub_x4(&ctx->enc_ctx_x4[1],
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

#endif /* __SEED_COMMIT_DEFAULT_H__ */
