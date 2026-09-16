#ifndef __GGM_TREE_COMMON_H__
#define __GGM_TREE_COMMON_H__

#include "enc.h"

#if !defined(GGMTREE_NB_ENC_CTX_IN_MEMORY)
/* Default to 1 */
#define GGMTREE_NB_ENC_CTX_IN_MEMORY 1
#endif
/* Checked unconditionally: keeping the range test inside the #else above
 * trusted the built-in default rather than verifying it. The lower bound
 * rejects a negative -D; 0 is a legal value - and the one test_kat_options.sh
 * exercises - hence < 0 rather than <= 0. */
#if (GGMTREE_NB_ENC_CTX_IN_MEMORY < 0) || (GGMTREE_NB_ENC_CTX_IN_MEMORY > (MQOM3_PARAM_NB_EVALS_LOG-2))
#error "GGMTREE_NB_ENC_CTX_IN_MEMORY must be >= 0 and <= MQOM3_PARAM_NB_EVALS_LOG-2"
#endif
#define GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY ((MQOM3_PARAM_NB_EVALS_LOG-1)-GGMTREE_NB_ENC_CTX_IN_MEMORY)


/* SeedDerive variants
 * NOTE: we factorize the key schedule, the tweaked salt is inside the encryption context */
static inline int SeedDerive(enc_ctx *ctx, const uint8_t seed[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed[MQOM3_PARAM_SEED_SIZE]) {
	int ret = 0;
	uint8_t linortho_seed[MQOM3_PARAM_SEED_SIZE];
	if (seed) {
		LinOrtho(seed, linortho_seed);
		/* Encrypt the LinOrtho seed with the tweaked salt */
		ret = enc_encrypt(ctx, seed, new_seed);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(new_seed, linortho_seed, new_seed);
	}


err:
	mqom_cleanse(linortho_seed, sizeof(linortho_seed));
	return ret;
}
static inline int SeedDerive_pub(enc_ctx_pub *ctx, const uint8_t seed[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed[MQOM3_PARAM_SEED_SIZE]) {
	int ret = 0;
	if (seed) {
		uint8_t linortho_seed[MQOM3_PARAM_SEED_SIZE];
		LinOrtho(seed, linortho_seed);
		/* Encrypt the LinOrtho seed with the tweaked salt */
		ret = enc_encrypt_pub(ctx, seed, new_seed);
		ERR(ret, err);
		/* Xor with LinOrtho seed */
		xor_blocks(new_seed, linortho_seed, new_seed);
	}


err:
	return ret;
}

/* The batched seed derivations come in two flavours that differ only by the
 * block cipher they drive: the constant-time one, which Sign needs for its
 * secret seeds, and the public one, which Verify uses since it only ever
 * handles public data (same split as SeedDerive / SeedDerive_pub above, and
 * as the CT small tree). Both are generated from a single body so the two
 * cannot drift apart. The public flavour skips the cleanse: it has nothing
 * sensitive to wipe, and the wipe is not free. */
#define GGM_NO_CLEANSE(ptr, len) do { (void) (ptr); (void) (len); } while (0)

#define GGM_GEN_SEED_DERIVE_X2(NAME, CTX, ENC, CLEANSE)                                                   \
static inline int NAME(CTX *ctx,                                                                          \
                                 const uint8_t seed1[MQOM3_PARAM_SEED_SIZE], const uint8_t seed2[MQOM3_PARAM_SEED_SIZE], \
                                 uint8_t new_seed1[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed2[MQOM3_PARAM_SEED_SIZE]) {   \
	int ret = -1;                                                                                         \
	/* Contiguous so the whole batch can be wiped with a single mqom_cleanse                               \
	 * call below, instead of one (non-inlinable, volatile-fnptr) call per seed. */                        \
	uint8_t linortho_seed[2][MQOM3_PARAM_SEED_SIZE];                                                      \
	LinOrtho(seed1, linortho_seed[0]);                                                                    \
	LinOrtho(seed2, linortho_seed[1]);                                                                    \
	/* Encrypt the seed with the tweaked salt */                                                           \
	ret = ENC(ctx, seed1, seed2, new_seed1, new_seed2);                                                   \
	ERR(ret, err);                                                                                        \
	/* Xor with LinOrtho seed */                                                                           \
	xor_blocks(new_seed1, linortho_seed[0], new_seed1);                                                   \
	xor_blocks(new_seed2, linortho_seed[1], new_seed2);                                                   \
	ret = 0;                                                                                              \
err:                                                                                                      \
	CLEANSE(linortho_seed, sizeof(linortho_seed));                                                        \
	return ret;                                                                                           \
}

GGM_GEN_SEED_DERIVE_X2(SeedDerive_x2_x2,     enc_ctx_x2,     enc_encrypt_x2_x2,     mqom_cleanse)
GGM_GEN_SEED_DERIVE_X2(SeedDerive_x2_x2_pub, enc_ctx_pub_x2, enc_encrypt_x2_pub_x2, GGM_NO_CLEANSE)

#define GGM_GEN_SEED_DERIVE_X4(NAME, CTX, ENC, CLEANSE)                                                   \
static inline int NAME(CTX *ctx,                                                                          \
                                 const uint8_t seed1[MQOM3_PARAM_SEED_SIZE], const uint8_t seed2[MQOM3_PARAM_SEED_SIZE], \
                                 const uint8_t seed3[MQOM3_PARAM_SEED_SIZE], const uint8_t seed4[MQOM3_PARAM_SEED_SIZE], \
                                 uint8_t new_seed1[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed2[MQOM3_PARAM_SEED_SIZE],     \
                                 uint8_t new_seed3[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed4[MQOM3_PARAM_SEED_SIZE]) {   \
	int ret = -1;                                                                                         \
	/* Contiguous so the whole batch can be wiped with a single mqom_cleanse                               \
	 * call below, instead of one (non-inlinable, volatile-fnptr) call per seed. */                        \
	uint8_t linortho_seed[4][MQOM3_PARAM_SEED_SIZE];                                                      \
	LinOrtho(seed1, linortho_seed[0]);                                                                    \
	LinOrtho(seed2, linortho_seed[1]);                                                                    \
	LinOrtho(seed3, linortho_seed[2]);                                                                    \
	LinOrtho(seed4, linortho_seed[3]);                                                                    \
	/* Encrypt the seed with the tweaked salt */                                                           \
	ret = ENC(ctx, seed1, seed2, seed3, seed4, new_seed1, new_seed2, new_seed3, new_seed4);               \
	ERR(ret, err);                                                                                        \
	/* Xor with LinOrtho seed */                                                                           \
	xor_blocks(new_seed1, linortho_seed[0], new_seed1);                                                   \
	xor_blocks(new_seed2, linortho_seed[1], new_seed2);                                                   \
	xor_blocks(new_seed3, linortho_seed[2], new_seed3);                                                   \
	xor_blocks(new_seed4, linortho_seed[3], new_seed4);                                                   \
	ret = 0;                                                                                              \
err:                                                                                                      \
	CLEANSE(linortho_seed, sizeof(linortho_seed));                                                        \
	return ret;                                                                                           \
}

GGM_GEN_SEED_DERIVE_X4(SeedDerive_x4_x4,     enc_ctx_x4,     enc_encrypt_x4_x4,     mqom_cleanse)
GGM_GEN_SEED_DERIVE_X4(SeedDerive_x4_x4_pub, enc_ctx_pub_x4, enc_encrypt_x4_pub_x4, GGM_NO_CLEANSE)

#define GGM_GEN_SEED_DERIVE_X8(NAME, CTX, ENC, CLEANSE)                                                   \
static inline int NAME(CTX *ctx,                                                                          \
                                 const uint8_t seed1[MQOM3_PARAM_SEED_SIZE], const uint8_t seed2[MQOM3_PARAM_SEED_SIZE], \
                                 const uint8_t seed3[MQOM3_PARAM_SEED_SIZE], const uint8_t seed4[MQOM3_PARAM_SEED_SIZE], \
                                 const uint8_t seed5[MQOM3_PARAM_SEED_SIZE], const uint8_t seed6[MQOM3_PARAM_SEED_SIZE], \
                                 const uint8_t seed7[MQOM3_PARAM_SEED_SIZE], const uint8_t seed8[MQOM3_PARAM_SEED_SIZE], \
                                 uint8_t new_seed1[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed2[MQOM3_PARAM_SEED_SIZE],     \
                                 uint8_t new_seed3[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed4[MQOM3_PARAM_SEED_SIZE],     \
                                 uint8_t new_seed5[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed6[MQOM3_PARAM_SEED_SIZE],     \
                                 uint8_t new_seed7[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed8[MQOM3_PARAM_SEED_SIZE]) {   \
	int ret = -1;                                                                                         \
	/* Contiguous so the whole batch can be wiped with a single mqom_cleanse                               \
	 * call below, instead of one (non-inlinable, volatile-fnptr) call per seed. */                        \
	uint8_t linortho_seed[8][MQOM3_PARAM_SEED_SIZE];                                                      \
                                                                                                          \
	LinOrtho(seed1, linortho_seed[0]);                                                                    \
	LinOrtho(seed2, linortho_seed[1]);                                                                    \
	LinOrtho(seed3, linortho_seed[2]);                                                                    \
	LinOrtho(seed4, linortho_seed[3]);                                                                    \
	LinOrtho(seed5, linortho_seed[4]);                                                                    \
	LinOrtho(seed6, linortho_seed[5]);                                                                    \
	LinOrtho(seed7, linortho_seed[6]);                                                                    \
	LinOrtho(seed8, linortho_seed[7]);                                                                    \
                                                                                                          \
	ret = ENC(ctx,                                                                                        \
	                       seed1, seed2, seed3, seed4,                                                    \
	                       seed5, seed6, seed7, seed8,                                                    \
	                       new_seed1, new_seed2, new_seed3, new_seed4,                                    \
	                       new_seed5, new_seed6, new_seed7, new_seed8);                                   \
	ERR(ret, err);                                                                                        \
                                                                                                          \
	xor_blocks(new_seed1, linortho_seed[0], new_seed1);                                                   \
	xor_blocks(new_seed2, linortho_seed[1], new_seed2);                                                   \
	xor_blocks(new_seed3, linortho_seed[2], new_seed3);                                                   \
	xor_blocks(new_seed4, linortho_seed[3], new_seed4);                                                   \
	xor_blocks(new_seed5, linortho_seed[4], new_seed5);                                                   \
	xor_blocks(new_seed6, linortho_seed[5], new_seed6);                                                   \
	xor_blocks(new_seed7, linortho_seed[6], new_seed7);                                                   \
	xor_blocks(new_seed8, linortho_seed[7], new_seed8);                                                   \
	ret = 0;                                                                                              \
err:                                                                                                      \
	CLEANSE(linortho_seed, sizeof(linortho_seed));                                                        \
	return ret;                                                                                           \
}

GGM_GEN_SEED_DERIVE_X8(SeedDerive_x8_x8,     enc_ctx_x8,     enc_encrypt_x8_x8,     mqom_cleanse)
GGM_GEN_SEED_DERIVE_X8(SeedDerive_x8_x8_pub, enc_ctx_pub_x8, enc_encrypt_x8_pub_x8, GGM_NO_CLEANSE)

#endif /* __GGM_TREE_COMMON_H__ */
