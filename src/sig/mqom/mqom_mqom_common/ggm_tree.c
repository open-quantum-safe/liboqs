#include "ggm_tree.h"

/* SeedDerive variants
 * NOTE: we factorize the key schedule, the tweaked salt is inside the encryption context */
static inline void SeedDerive(enc_ctx *ctx, const uint8_t seed[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed[MQOM2_PARAM_SEED_SIZE]) {
	if (seed) {
		uint8_t linortho_seed[MQOM2_PARAM_SEED_SIZE];
		LinOrtho(seed, linortho_seed);
		/* Encrypt the LinOrtho seed with the tweaked salt */
		enc_encrypt(ctx, seed, new_seed);
		/* Xor with LinOrtho seed */
		xor_blocks(new_seed, linortho_seed, new_seed);
	}

	return;
}
static inline void SeedDerive_pub(enc_ctx_pub *ctx, const uint8_t seed[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed[MQOM2_PARAM_SEED_SIZE]) {
	if (seed) {
		uint8_t linortho_seed[MQOM2_PARAM_SEED_SIZE];
		LinOrtho(seed, linortho_seed);
		/* Encrypt the LinOrtho seed with the tweaked salt */
		enc_encrypt_pub(ctx, seed, new_seed);
		/* Xor with LinOrtho seed */
		xor_blocks(new_seed, linortho_seed, new_seed);
	}

	return;
}

static inline void SeedDerive_x2(enc_ctx *ctx1, enc_ctx *ctx2, const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE],
                                 uint8_t new_seed1[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed2[MQOM2_PARAM_SEED_SIZE]) {
	if (seed1 && seed2) {
		uint8_t linortho_seed1[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed2[MQOM2_PARAM_SEED_SIZE];
		LinOrtho(seed1, linortho_seed1);
		LinOrtho(seed2, linortho_seed2);
		/* Encrypt the LinOrtho seed with the tweaked salt */
		enc_encrypt_x2(ctx1, ctx2, seed1, seed2, new_seed1, new_seed2);
		/* Xor with LinOrtho seed */
		xor_blocks(new_seed1, linortho_seed1, new_seed1);
		xor_blocks(new_seed2, linortho_seed2, new_seed2);
	} else {
		SeedDerive(ctx1, seed1, new_seed1);
		SeedDerive(ctx2, seed2, new_seed2);
	}

	return;
}
static inline void SeedDerive_x2_pub(enc_ctx_pub *ctx1, enc_ctx_pub *ctx2, const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE],
                                     uint8_t new_seed1[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed2[MQOM2_PARAM_SEED_SIZE]) {
	if (seed1 && seed2) {
		uint8_t linortho_seed1[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed2[MQOM2_PARAM_SEED_SIZE];
		LinOrtho(seed1, linortho_seed1);
		LinOrtho(seed2, linortho_seed2);
		/* Encrypt the LinOrtho seed with the tweaked salt */
		enc_encrypt_x2_pub(ctx1, ctx2, seed1, seed2, new_seed1, new_seed2);
		/* Xor with LinOrtho seed */
		xor_blocks(new_seed1, linortho_seed1, new_seed1);
		xor_blocks(new_seed2, linortho_seed2, new_seed2);
	} else {
		SeedDerive_pub(ctx1, seed1, new_seed1);
		SeedDerive_pub(ctx2, seed2, new_seed2);
	}

	return;
}

static inline void SeedDerive_x2_x2(enc_ctx_x2 *ctx, const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE],
                                 uint8_t new_seed1[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed2[MQOM2_PARAM_SEED_SIZE]) {
	uint8_t linortho_seed1[MQOM2_PARAM_SEED_SIZE];
	uint8_t linortho_seed2[MQOM2_PARAM_SEED_SIZE];
	LinOrtho(seed1, linortho_seed1);
	LinOrtho(seed2, linortho_seed2);
	/* Encrypt the LinOrtho seed with the tweaked salt */
	enc_encrypt_x2_x2(ctx, seed1, seed2, new_seed1, new_seed2);
	/* Xor with LinOrtho seed */
	xor_blocks(new_seed1, linortho_seed1, new_seed1);
	xor_blocks(new_seed2, linortho_seed2, new_seed2);
	return;
}

static inline void SeedDerive_x4(enc_ctx *ctx1, enc_ctx *ctx2, enc_ctx *ctx3, enc_ctx *ctx4,
                                 const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE],
                                 const uint8_t seed3[MQOM2_PARAM_SEED_SIZE], const uint8_t seed4[MQOM2_PARAM_SEED_SIZE],
                                 uint8_t new_seed1[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed2[MQOM2_PARAM_SEED_SIZE],
                                 uint8_t new_seed3[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed4[MQOM2_PARAM_SEED_SIZE]) {
	if (seed1 && seed2 && seed3 && seed4) {
		uint8_t linortho_seed1[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed2[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed3[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed4[MQOM2_PARAM_SEED_SIZE];
		LinOrtho(seed1, linortho_seed1);
		LinOrtho(seed2, linortho_seed2);
		LinOrtho(seed3, linortho_seed3);
		LinOrtho(seed4, linortho_seed4);
		/* Encrypt the seed with the tweaked salt */
		enc_encrypt_x4(ctx1, ctx2, ctx3, ctx4, seed1, seed2, seed3, seed4, new_seed1, new_seed2, new_seed3, new_seed4);
		/* Xor with LinOrtho seed */
		xor_blocks(new_seed1, linortho_seed1, new_seed1);
		xor_blocks(new_seed2, linortho_seed2, new_seed2);
		xor_blocks(new_seed3, linortho_seed3, new_seed3);
		xor_blocks(new_seed4, linortho_seed4, new_seed4);
	} else {
		SeedDerive_x2(ctx1, ctx2, seed1, seed2, new_seed1, new_seed2);
		SeedDerive_x2(ctx3, ctx4, seed3, seed4, new_seed3, new_seed4);
	}
	return;
}
static inline void SeedDerive_x4_pub(enc_ctx_pub *ctx1, enc_ctx_pub *ctx2, enc_ctx_pub *ctx3, enc_ctx_pub *ctx4,
                                     const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE],
                                     const uint8_t seed3[MQOM2_PARAM_SEED_SIZE], const uint8_t seed4[MQOM2_PARAM_SEED_SIZE],
                                     uint8_t new_seed1[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed2[MQOM2_PARAM_SEED_SIZE],
                                     uint8_t new_seed3[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed4[MQOM2_PARAM_SEED_SIZE]) {
	if (seed1 && seed2 && seed3 && seed4) {
		uint8_t linortho_seed1[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed2[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed3[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed4[MQOM2_PARAM_SEED_SIZE];
		LinOrtho(seed1, linortho_seed1);
		LinOrtho(seed2, linortho_seed2);
		LinOrtho(seed3, linortho_seed3);
		LinOrtho(seed4, linortho_seed4);
		/* Encrypt the seed with the tweaked salt */
		enc_encrypt_x4_pub(ctx1, ctx2, ctx3, ctx4, seed1, seed2, seed3, seed4, new_seed1, new_seed2, new_seed3, new_seed4);
		/* Xor with LinOrtho seed */
		xor_blocks(new_seed1, linortho_seed1, new_seed1);
		xor_blocks(new_seed2, linortho_seed2, new_seed2);
		xor_blocks(new_seed3, linortho_seed3, new_seed3);
		xor_blocks(new_seed4, linortho_seed4, new_seed4);
	} else {
		SeedDerive_x2_pub(ctx1, ctx2, seed1, seed2, new_seed1, new_seed2);
		SeedDerive_x2_pub(ctx3, ctx4, seed3, seed4, new_seed3, new_seed4);
	}
	return;
}
static inline void SeedDerive_x4_x4(enc_ctx_x4 *ctx,
                                 const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE],
                                 const uint8_t seed3[MQOM2_PARAM_SEED_SIZE], const uint8_t seed4[MQOM2_PARAM_SEED_SIZE],
                                 uint8_t new_seed1[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed2[MQOM2_PARAM_SEED_SIZE],
                                 uint8_t new_seed3[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed4[MQOM2_PARAM_SEED_SIZE]) {
	uint8_t linortho_seed1[MQOM2_PARAM_SEED_SIZE];
	uint8_t linortho_seed2[MQOM2_PARAM_SEED_SIZE];
	uint8_t linortho_seed3[MQOM2_PARAM_SEED_SIZE];
	uint8_t linortho_seed4[MQOM2_PARAM_SEED_SIZE];
	LinOrtho(seed1, linortho_seed1);
	LinOrtho(seed2, linortho_seed2);
	LinOrtho(seed3, linortho_seed3);
	LinOrtho(seed4, linortho_seed4);
	/* Encrypt the seed with the tweaked salt */
	enc_encrypt_x4_x4(ctx, seed1, seed2, seed3, seed4, new_seed1, new_seed2, new_seed3, new_seed4);
	/* Xor with LinOrtho seed */
	xor_blocks(new_seed1, linortho_seed1, new_seed1);
	xor_blocks(new_seed2, linortho_seed2, new_seed2);
	xor_blocks(new_seed3, linortho_seed3, new_seed3);
	xor_blocks(new_seed4, linortho_seed4, new_seed4);
	return;
}

static inline void SeedDerive_x8(enc_ctx *ctx1, enc_ctx *ctx2, enc_ctx *ctx3, enc_ctx *ctx4, enc_ctx *ctx5, enc_ctx *ctx6, enc_ctx *ctx7, enc_ctx *ctx8,
                                 const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE],
                                 const uint8_t seed3[MQOM2_PARAM_SEED_SIZE], const uint8_t seed4[MQOM2_PARAM_SEED_SIZE],
                                 const uint8_t seed5[MQOM2_PARAM_SEED_SIZE], const uint8_t seed6[MQOM2_PARAM_SEED_SIZE],
                                 const uint8_t seed7[MQOM2_PARAM_SEED_SIZE], const uint8_t seed8[MQOM2_PARAM_SEED_SIZE],
                                 uint8_t new_seed1[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed2[MQOM2_PARAM_SEED_SIZE],
                                 uint8_t new_seed3[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed4[MQOM2_PARAM_SEED_SIZE],
                                 uint8_t new_seed5[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed6[MQOM2_PARAM_SEED_SIZE],
                                 uint8_t new_seed7[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed8[MQOM2_PARAM_SEED_SIZE])

{
	if (seed1 && seed2 && seed3 && seed4 && seed5 && seed6 && seed7 && seed8) {
		uint8_t linortho_seed1[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed2[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed3[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed4[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed5[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed6[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed7[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed8[MQOM2_PARAM_SEED_SIZE];
		LinOrtho(seed1, linortho_seed1);
		LinOrtho(seed2, linortho_seed2);
		LinOrtho(seed3, linortho_seed3);
		LinOrtho(seed4, linortho_seed4);
		LinOrtho(seed5, linortho_seed5);
		LinOrtho(seed6, linortho_seed6);
		LinOrtho(seed7, linortho_seed7);
		LinOrtho(seed8, linortho_seed8);
		/* Encrypt the seed with the tweaked salt */
		enc_encrypt_x8(ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, ctx7, ctx8,
		               seed1, seed2, seed3, seed4, seed5, seed6, seed7, seed8,
		               new_seed1, new_seed2, new_seed3, new_seed4, new_seed5, new_seed6, new_seed7, new_seed8);
		/* Xor with LinOrtho seed */
		xor_blocks(new_seed1, linortho_seed1, new_seed1);
		xor_blocks(new_seed2, linortho_seed2, new_seed2);
		xor_blocks(new_seed3, linortho_seed3, new_seed3);
		xor_blocks(new_seed4, linortho_seed4, new_seed4);
		xor_blocks(new_seed5, linortho_seed5, new_seed5);
		xor_blocks(new_seed6, linortho_seed6, new_seed6);
		xor_blocks(new_seed7, linortho_seed7, new_seed7);
		xor_blocks(new_seed8, linortho_seed8, new_seed8);
	} else {
		SeedDerive_x4(ctx1, ctx2, ctx3, ctx4, seed1, seed2, seed3, seed4, new_seed1, new_seed2, new_seed3, new_seed4);
		SeedDerive_x4(ctx5, ctx6, ctx7, ctx8, seed5, seed6, seed7, seed8, new_seed5, new_seed6, new_seed7, new_seed8);
	}
	return;
}
static inline void SeedDerive_x8_pub(enc_ctx_pub *ctx1, enc_ctx_pub *ctx2, enc_ctx_pub *ctx3, enc_ctx_pub *ctx4, enc_ctx_pub *ctx5, enc_ctx_pub *ctx6, enc_ctx_pub *ctx7, enc_ctx_pub *ctx8,
                                     const uint8_t seed1[MQOM2_PARAM_SEED_SIZE], const uint8_t seed2[MQOM2_PARAM_SEED_SIZE],
                                     const uint8_t seed3[MQOM2_PARAM_SEED_SIZE], const uint8_t seed4[MQOM2_PARAM_SEED_SIZE],
                                     const uint8_t seed5[MQOM2_PARAM_SEED_SIZE], const uint8_t seed6[MQOM2_PARAM_SEED_SIZE],
                                     const uint8_t seed7[MQOM2_PARAM_SEED_SIZE], const uint8_t seed8[MQOM2_PARAM_SEED_SIZE],
                                     uint8_t new_seed1[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed2[MQOM2_PARAM_SEED_SIZE],
                                     uint8_t new_seed3[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed4[MQOM2_PARAM_SEED_SIZE],
                                     uint8_t new_seed5[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed6[MQOM2_PARAM_SEED_SIZE],
                                     uint8_t new_seed7[MQOM2_PARAM_SEED_SIZE], uint8_t new_seed8[MQOM2_PARAM_SEED_SIZE])

{
	if (seed1 && seed2 && seed3 && seed4 && seed5 && seed6 && seed7 && seed8) {
		uint8_t linortho_seed1[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed2[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed3[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed4[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed5[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed6[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed7[MQOM2_PARAM_SEED_SIZE];
		uint8_t linortho_seed8[MQOM2_PARAM_SEED_SIZE];
		LinOrtho(seed1, linortho_seed1);
		LinOrtho(seed2, linortho_seed2);
		LinOrtho(seed3, linortho_seed3);
		LinOrtho(seed4, linortho_seed4);
		LinOrtho(seed5, linortho_seed5);
		LinOrtho(seed6, linortho_seed6);
		LinOrtho(seed7, linortho_seed7);
		LinOrtho(seed8, linortho_seed8);
		/* Encrypt the seed with the tweaked salt */
		enc_encrypt_x8_pub(ctx1, ctx2, ctx3, ctx4, ctx5, ctx6, ctx7, ctx8,
		                   seed1, seed2, seed3, seed4, seed5, seed6, seed7, seed8,
		                   new_seed1, new_seed2, new_seed3, new_seed4, new_seed5, new_seed6, new_seed7, new_seed8);
		/* Xor with LinOrtho seed */
		xor_blocks(new_seed1, linortho_seed1, new_seed1);
		xor_blocks(new_seed2, linortho_seed2, new_seed2);
		xor_blocks(new_seed3, linortho_seed3, new_seed3);
		xor_blocks(new_seed4, linortho_seed4, new_seed4);
		xor_blocks(new_seed5, linortho_seed5, new_seed5);
		xor_blocks(new_seed6, linortho_seed6, new_seed6);
		xor_blocks(new_seed7, linortho_seed7, new_seed7);
		xor_blocks(new_seed8, linortho_seed8, new_seed8);
	} else {
		SeedDerive_x4_pub(ctx1, ctx2, ctx3, ctx4, seed1, seed2, seed3, seed4, new_seed1, new_seed2, new_seed3, new_seed4);
		SeedDerive_x4_pub(ctx5, ctx6, ctx7, ctx8, seed5, seed6, seed7, seed8, new_seed5, new_seed6, new_seed7, new_seed8);
	}
	return;
}

/* NOTE: in the "node" tree representation, we accept to remove the two first cells to simplify the indices computations.
 * The first cell is used to avoid the "0" index, and the second cell is the root of the correlated tree that is not used.
 * */
int GGMTree_Expand(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t rseed[MQOM2_PARAM_SEED_SIZE], const uint8_t delta[MQOM2_PARAM_SEED_SIZE], uint32_t e, uint8_t node[MQOM2_PARAM_FULL_TREE_SIZE + 1][MQOM2_PARAM_SEED_SIZE], uint8_t lseed[MQOM2_PARAM_NB_EVALS][MQOM2_PARAM_SEED_SIZE]) {
	int ret = -1;
	/* j is the level in the tree, k is the index in the nodes array */
	uint32_t j, k;
	enc_ctx DECL_VAR(ctx);
	uint8_t tweaked_salt[MQOM2_PARAM_SALT_SIZE];

	/* Some sanity check */
	if ((1 << MQOM2_PARAM_NB_EVALS_LOG) != MQOM2_PARAM_NB_EVALS) {
		ret = -1;
		goto err;
	}

	/* The root node is not defined, we begin with the first level */
	memcpy(node[2], rseed, MQOM2_PARAM_SEED_SIZE);
	xor_blocks(node[2], delta, node[3]);

	/* Now deal with the other levels */
	for (j = 1; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		/* Level 1 has 2 derivations, levels > 1 allow for more derivations */
		uint32_t num_derivations;
		if (j == 1) {
			num_derivations = 2;
		}
#ifdef USE_ENC_X8
		else if (j == 2) {
			num_derivations = 4;
		} else {
			num_derivations = 8;
		}
#else
		else {
			num_derivations = 4;
		}
#endif
		/* For a whole level in the tree, we use the same key, which is the tweaked salt, hence
		 * the common key schedule */
		TweakSalt(salt, tweaked_salt, 2, e, j - 1);
		ret = enc_key_sched(&ctx, tweaked_salt);
		ERR(ret, err);
		/* NOTE: hereafter, when using x2 or x4 we are sure that there
		         * are no leftover nodes. The encryptions performed correspong to the
		         * SeedDerive procedure.
		 */
		for (k = ((uint32_t)1 << j); k < ((uint32_t)1 << (j + 1)); k += num_derivations) {
			switch (num_derivations) {
			case 2: {
				uint8_t *pt1 = node[k];
				uint8_t *pt2 = node[k + 1];
				uint8_t *ct1 = node[2 * k];
				uint8_t *ct2 = node[2 * (k + 1)];
				/* We perform an x2 on 2 nodes */
				SeedDerive_x2(&ctx, &ctx,
				              /* Input nodes to encrypt */
				              pt1, pt2,
				              /* Output nodes to encrypt */
				              ct1, ct2);
				/* Compute the corresponding xors */
				uint8_t *rnode_ct1 = node[(2 * k) + 1];
				uint8_t *rnode_ct2 = node[(2 * (k + 1)) + 1];
				xor_blocks(ct1, pt1, rnode_ct1);
				xor_blocks(ct2, pt2, rnode_ct2);
				break;
			}
			case 4: {
				/* We perform an x4 on 4 nodes */
				uint8_t *pt1 = node[k];
				uint8_t *pt2 = node[k + 1];
				uint8_t *pt3 = node[k + 2];
				uint8_t *pt4 = node[k + 3];
				uint8_t *ct1 = node[2 * k];
				uint8_t *ct2 = node[2 * (k + 1)];
				uint8_t *ct3 = node[2 * (k + 2)];
				uint8_t *ct4 = node[2 * (k + 3)];
				/* We perform an x4 on 4 nodes */
				SeedDerive_x4(&ctx, &ctx, &ctx, &ctx,
				              /* Input nodes to encrypt */
				              pt1, pt2, pt3, pt4,
				              /* Output nodes to encrypt */
				              ct1, ct2, ct3, ct4);
				/* Compute the corresponding xors */
				uint8_t *rnode_ct1 = node[(2 * k) + 1];
				uint8_t *rnode_ct2 = node[(2 * (k + 1)) + 1];
				uint8_t *rnode_ct3 = node[(2 * (k + 2)) + 1];
				uint8_t *rnode_ct4 = node[(2 * (k + 3)) + 1];
				xor_blocks(ct1, pt1, rnode_ct1);
				xor_blocks(ct2, pt2, rnode_ct2);
				xor_blocks(ct3, pt3, rnode_ct3);
				xor_blocks(ct4, pt4, rnode_ct4);
				break;
			}
#ifdef USE_ENC_X8
			case 8: {
				/* We perform an x8 on 8 nodes */
				uint8_t *pt1 = node[k];
				uint8_t *pt2 = node[k + 1];
				uint8_t *pt3 = node[k + 2];
				uint8_t *pt4 = node[k + 3];
				uint8_t *pt5 = node[k + 4];
				uint8_t *pt6 = node[k + 5];
				uint8_t *pt7 = node[k + 6];
				uint8_t *pt8 = node[k + 7];
				uint8_t *ct1 = node[2 * k];
				uint8_t *ct2 = node[2 * (k + 1)];
				uint8_t *ct3 = node[2 * (k + 2)];
				uint8_t *ct4 = node[2 * (k + 3)];
				uint8_t *ct5 = node[2 * (k + 4)];
				uint8_t *ct6 = node[2 * (k + 5)];
				uint8_t *ct7 = node[2 * (k + 6)];
				uint8_t *ct8 = node[2 * (k + 7)];
				/* We perform an x8 on 8 nodes */
				SeedDerive_x8(&ctx, &ctx, &ctx, &ctx, &ctx, &ctx, &ctx, &ctx,
				              /* Input nodes to encrypt */
				              pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8,
				              /* Output nodes to encrypt */
				              ct1, ct2, ct3, ct4, ct5, ct6, ct7, ct8);
				/* Compute the corresponding xors */
				uint8_t *rnode_ct1 = node[(2 * k) + 1];
				uint8_t *rnode_ct2 = node[(2 * (k + 1)) + 1];
				uint8_t *rnode_ct3 = node[(2 * (k + 2)) + 1];
				uint8_t *rnode_ct4 = node[(2 * (k + 3)) + 1];
				uint8_t *rnode_ct5 = node[(2 * (k + 4)) + 1];
				uint8_t *rnode_ct6 = node[(2 * (k + 5)) + 1];
				uint8_t *rnode_ct7 = node[(2 * (k + 6)) + 1];
				uint8_t *rnode_ct8 = node[(2 * (k + 7)) + 1];
				xor_blocks(ct1, pt1, rnode_ct1);
				xor_blocks(ct2, pt2, rnode_ct2);
				xor_blocks(ct3, pt3, rnode_ct3);
				xor_blocks(ct4, pt4, rnode_ct4);
				xor_blocks(ct5, pt5, rnode_ct5);
				xor_blocks(ct6, pt6, rnode_ct6);
				xor_blocks(ct7, pt7, rnode_ct7);
				xor_blocks(ct8, pt8, rnode_ct8);
				break;
			}
#endif
			default: {
				ret = -1;
				goto err;
			}
			}
		}
	}

	/* The lseed are the leaves of the tree, copied from it */
	memcpy(lseed, node[MQOM2_PARAM_FULL_TREE_SIZE + 1 - MQOM2_PARAM_NB_EVALS], MQOM2_PARAM_NB_EVALS * MQOM2_PARAM_SEED_SIZE);

	ret = 0;
err:
	enc_clean_ctx(&ctx);
	return ret;
}

int GGMTree_Open(const uint8_t node[MQOM2_PARAM_FULL_TREE_SIZE + 1][MQOM2_PARAM_SEED_SIZE], uint32_t i_star, uint8_t path[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE]) {
	int ret = -1;
	uint32_t i, j;

	/* Sanity check */
	if (i_star >= MQOM2_PARAM_NB_EVALS) {
		ret = -1;
		goto err;
	}

	i = MQOM2_PARAM_NB_EVALS + i_star;
	for (j = 0; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		memcpy(path[j], node[i ^ 1], MQOM2_PARAM_SEED_SIZE);
		i = (i / 2);
	}

	ret = 0;
err:
	return ret;
}

/* XXX: TODO: for now, we allocate the full tree to compute the leaves seeds. It is possible to perform this *in place* with
 * a dedicated index handling, allowing to save memory space as well as a memcpy */
/* XXX: NOTE: we can use public encryption API here as this function is used for verification */
int GGMTree_PartiallyExpand(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t path[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE], uint32_t e, uint32_t i_star, uint8_t lseed[MQOM2_PARAM_NB_EVALS][MQOM2_PARAM_SEED_SIZE]) {
	int ret = -1;
	/* j is the level in the tree, k is the index in the nodes array */
	uint32_t i, j, k;
	enc_ctx_pub DECL_VAR(ctx);
	uint8_t tweaked_salt[MQOM2_PARAM_SALT_SIZE];
	/* Locally allocate the full tree */
	uint8_t node[MQOM2_PARAM_FULL_TREE_SIZE + 1][MQOM2_PARAM_SEED_SIZE];
	/* We use a shadow map to indicate bot values */
	uint8_t node_map[MQOM2_PARAM_FULL_TREE_SIZE + 1];
	memset(node_map, 0, MQOM2_PARAM_FULL_TREE_SIZE + 1);

	/* Some sanity checks */
	if ((1 << MQOM2_PARAM_NB_EVALS_LOG) != MQOM2_PARAM_NB_EVALS) {
		ret = -1;
		goto err;
	}
	if (i_star >= MQOM2_PARAM_NB_EVALS) {
		ret = -1;
		goto err;
	}

	/* Copy the path */
	i = MQOM2_PARAM_NB_EVALS + i_star;
	for (j = 0; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		memcpy(node[i ^ 1], path[j], MQOM2_PARAM_SEED_SIZE);
		node_map[i ^ 1] = 1;
		i = (i / 2);
	}

	/* Compute the other nodes when possible.
	 * */
	for (j = 1; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		/* Level 1 has 2 derivations, levels > 1 allow for more derivations */
		uint32_t num_derivations;
		if (j == 1) {
			num_derivations = 2;
		}
#ifdef USE_ENC_X8
		else if (j == 2) {
			num_derivations = 4;
		} else {
			num_derivations = 8;
		}
#else
		else {
			num_derivations = 4;
		}
#endif
		/* For a whole level in the tree, we use the same key, which is the tweaked salt, hence
		 * the common key schedule */
		TweakSalt(salt, tweaked_salt, 2, e, j - 1);
		ret = enc_key_sched_pub(&ctx, tweaked_salt);
		ERR(ret, err);
		/* NOTE: hereafter, when using x2 or x4 we are sure that there
		         * are no leftover nodes. The encryptions performed correspong to the
		         * SeedDerive procedure.
		 */
		for (k = ((uint32_t)1 << j); k < ((uint32_t)1 << (j + 1)); k += num_derivations) {
			switch (num_derivations) {
			case 2: {
				uint8_t *pt1 = node_map[k]     ? node[k]     : NULL;
				uint8_t *pt2 = node_map[k + 1] ? node[k + 1] : NULL;
				uint8_t *ct1 = node[2 * k];
				uint8_t *ct2 = node[2 * (k + 1)];
				/* We perform an x2 on 2 nodes */
				SeedDerive_x2_pub(&ctx, &ctx,
				                  /* Input nodes to encrypt */
				                  pt1, pt2,
				                  /* Output nodes to encrypt */
				                  ct1, ct2);
				/* Compute the corresponding xors */
				uint8_t *rnode_ct1 = node[(2 * k) + 1];
				uint8_t *rnode_ct2 = node[(2 * (k + 1)) + 1];
				if (pt1) {
					xor_blocks(ct1, pt1, rnode_ct1);
					node_map[(2 * k)] = 1;
					node_map[(2 * k) + 1] = 1;
				}
				if (pt2) {
					xor_blocks(ct2, pt2, rnode_ct2);
					node_map[(2 * (k + 1))] = 1;
					node_map[(2 * (k + 1)) + 1] = 1;
				}
				break;
			}
			case 4: {
				/* We perform an x4 on 4 nodes */
				uint8_t *pt1 = node_map[k]     ? node[k]     : NULL;
				uint8_t *pt2 = node_map[k + 1] ? node[k + 1] : NULL;
				uint8_t *pt3 = node_map[k + 2] ? node[k + 2] : NULL;
				uint8_t *pt4 = node_map[k + 3] ? node[k + 3] : NULL;
				uint8_t *ct1 = node[2 * k];
				uint8_t *ct2 = node[2 * (k + 1)];
				uint8_t *ct3 = node[2 * (k + 2)];
				uint8_t *ct4 = node[2 * (k + 3)];
				/* We perform an x4 on 4 nodes */
				SeedDerive_x4_pub(&ctx, &ctx, &ctx, &ctx,
				                  /* Input nodes to encrypt */
				                  pt1, pt2, pt3, pt4,
				                  /* Output nodes to encrypt */
				                  ct1, ct2, ct3, ct4);
				/* Compute the corresponding xors */
				uint8_t *rnode_ct1 = node[(2 * k) + 1];
				uint8_t *rnode_ct2 = node[(2 * (k + 1)) + 1];
				uint8_t *rnode_ct3 = node[(2 * (k + 2)) + 1];
				uint8_t *rnode_ct4 = node[(2 * (k + 3)) + 1];
				if (pt1) {
					xor_blocks(ct1, pt1, rnode_ct1);
					node_map[(2 * k)] = 1;
					node_map[(2 * k) + 1] = 1;
				}
				if (pt2) {
					xor_blocks(ct2, pt2, rnode_ct2);
					node_map[(2 * (k + 1))] = 1;
					node_map[(2 * (k + 1)) + 1] = 1;
				}
				if (pt3) {
					xor_blocks(ct3, pt3, rnode_ct3);
					node_map[(2 * (k + 2))] = 1;
					node_map[(2 * (k + 2)) + 1] = 1;
				}
				if (pt4) {
					xor_blocks(ct4, pt4, rnode_ct4);
					node_map[(2 * (k + 3))] = 1;
					node_map[(2 * (k + 3)) + 1] = 1;
				}
				break;
			}
#ifdef USE_ENC_X8
			case 8: {
				/* We perform an x8 on 8 nodes */
				uint8_t *pt1 = node_map[k]     ? node[k]     : NULL;
				uint8_t *pt2 = node_map[k + 1] ? node[k + 1] : NULL;
				uint8_t *pt3 = node_map[k + 2] ? node[k + 2] : NULL;
				uint8_t *pt4 = node_map[k + 3] ? node[k + 3] : NULL;
				uint8_t *pt5 = node_map[k + 4] ? node[k + 4] : NULL;
				uint8_t *pt6 = node_map[k + 5] ? node[k + 5] : NULL;
				uint8_t *pt7 = node_map[k + 6] ? node[k + 6] : NULL;
				uint8_t *pt8 = node_map[k + 7] ? node[k + 7] : NULL;
				uint8_t *ct1 = node[2 * k];
				uint8_t *ct2 = node[2 * (k + 1)];
				uint8_t *ct3 = node[2 * (k + 2)];
				uint8_t *ct4 = node[2 * (k + 3)];
				uint8_t *ct5 = node[2 * (k + 4)];
				uint8_t *ct6 = node[2 * (k + 5)];
				uint8_t *ct7 = node[2 * (k + 6)];
				uint8_t *ct8 = node[2 * (k + 7)];
				/* We perform an x4 on 4 nodes */
				SeedDerive_x8_pub(&ctx, &ctx, &ctx, &ctx, &ctx, &ctx, &ctx, &ctx,
				                  /* Input nodes to encrypt */
				                  pt1, pt2, pt3, pt4, pt5, pt6, pt7, pt8,
				                  /* Output nodes to encrypt */
				                  ct1, ct2, ct3, ct4, ct5, ct6, ct7, ct8);
				/* Compute the corresponding xors */
				uint8_t *rnode_ct1 = node[(2 * k) + 1];
				uint8_t *rnode_ct2 = node[(2 * (k + 1)) + 1];
				uint8_t *rnode_ct3 = node[(2 * (k + 2)) + 1];
				uint8_t *rnode_ct4 = node[(2 * (k + 3)) + 1];
				uint8_t *rnode_ct5 = node[(2 * (k + 4)) + 1];
				uint8_t *rnode_ct6 = node[(2 * (k + 5)) + 1];
				uint8_t *rnode_ct7 = node[(2 * (k + 6)) + 1];
				uint8_t *rnode_ct8 = node[(2 * (k + 7)) + 1];
				if (pt1) {
					xor_blocks(ct1, pt1, rnode_ct1);
					node_map[(2 * k)] = 1;
					node_map[(2 * k) + 1] = 1;
				}
				if (pt2) {
					xor_blocks(ct2, pt2, rnode_ct2);
					node_map[(2 * (k + 1))] = 1;
					node_map[(2 * (k + 1)) + 1] = 1;
				}
				if (pt3) {
					xor_blocks(ct3, pt3, rnode_ct3);
					node_map[(2 * (k + 2))] = 1;
					node_map[(2 * (k + 2)) + 1] = 1;
				}
				if (pt4) {
					xor_blocks(ct4, pt4, rnode_ct4);
					node_map[(2 * (k + 3))] = 1;
					node_map[(2 * (k + 3)) + 1] = 1;
				}
				if (pt5) {
					xor_blocks(ct5, pt5, rnode_ct5);
					node_map[(2 * (k + 4))] = 1;
					node_map[(2 * (k + 4)) + 1] = 1;
				}
				if (pt6) {
					xor_blocks(ct6, pt6, rnode_ct6);
					node_map[(2 * (k + 5))] = 1;
					node_map[(2 * (k + 5)) + 1] = 1;
				}
				if (pt7) {
					xor_blocks(ct7, pt7, rnode_ct7);
					node_map[(2 * (k + 6))] = 1;
					node_map[(2 * (k + 6)) + 1] = 1;
				}
				if (pt8) {
					xor_blocks(ct8, pt8, rnode_ct8);
					node_map[(2 * (k + 7))] = 1;
					node_map[(2 * (k + 7)) + 1] = 1;
				}
				break;
			}
#endif
			default: {
				ret = -1;
				goto err;
			}
			}
		}

	}

	/* The lseed are the leaves of the tree, copied from it */
	memcpy(lseed, node[MQOM2_PARAM_FULL_TREE_SIZE + 1 - MQOM2_PARAM_NB_EVALS], MQOM2_PARAM_NB_EVALS * MQOM2_PARAM_SEED_SIZE);
	/* Set the hidden leave to zero */
	memset(lseed[i_star], 0, MQOM2_PARAM_SEED_SIZE);

	ret = 0;
err:
	enc_clean_ctx_pub(&ctx);
	return ret;
}

int GGMTree_ExpandPath(const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t rseed[MQOM2_PARAM_SEED_SIZE], const uint8_t delta[MQOM2_PARAM_SEED_SIZE], uint32_t e, uint32_t i_star, uint8_t path[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE], uint8_t lseed[MQOM2_PARAM_SEED_SIZE]) {
	int ret = -1;
	uint32_t j;
	enc_ctx DECL_VAR(ctx);
	uint8_t tweaked_salt[MQOM2_PARAM_SALT_SIZE];

	/* Sanity check */
	if (i_star >= MQOM2_PARAM_NB_EVALS) {
		ret = -1;
		goto err;
	}

	uint32_t num_leaf = MQOM2_PARAM_NB_EVALS + i_star;

	uint8_t node[2][MQOM2_PARAM_SEED_SIZE];
	uint8_t parent[MQOM2_PARAM_SEED_SIZE];
	memcpy(parent, delta, MQOM2_PARAM_SEED_SIZE);

	for (j = 0; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		if (j == 0) {
			memcpy(node[0], rseed, MQOM2_PARAM_SEED_SIZE);
		} else {
			TweakSalt(salt, tweaked_salt, 2, e, j - 1);
			ret = enc_key_sched(&ctx, tweaked_salt);
			ERR(ret, err);
			SeedDerive(&ctx, parent, node[0]);
		}
		xor_blocks(node[0], parent, node[1]);

		uint32_t bit = (num_leaf >> (MQOM2_PARAM_NB_EVALS_LOG - 1 - j)) & 0x01;
		memcpy(path[MQOM2_PARAM_NB_EVALS_LOG - 1 - j], node[bit ^ 1], MQOM2_PARAM_SEED_SIZE);
		memcpy(parent, node[bit], MQOM2_PARAM_SEED_SIZE);
	}

	memcpy(lseed, parent, MQOM2_PARAM_SEED_SIZE);

	ret = 0;
err:
	enc_clean_ctx(&ctx);
	return ret;
}

int GGMTree_InitIncrementalExpansion(ggmtree_ctx_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t rseed[MQOM2_PARAM_SEED_SIZE], const uint8_t delta[MQOM2_PARAM_SEED_SIZE], uint32_t e) {
	uint32_t j;
	int ret = -1;
	uint8_t tweaked_salt[MQOM2_PARAM_SEED_SIZE];
	for (j = 1; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		if (j < GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY + 1) {
			TweakSalt(salt, ctx->tweaked_salts[j - 1], 2, e, j - 1);
		} else {
			TweakSalt(salt, tweaked_salt, 2, e, j - 1);
			ret = enc_key_sched(&ctx->ctx_enc[j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY], tweaked_salt);
			ERR(ret, err);
		}
	}
	memcpy(ctx->path[0], delta, MQOM2_PARAM_SEED_SIZE);
	memcpy(ctx->path[1], rseed, MQOM2_PARAM_SEED_SIZE);
	ctx->active = 0;

	ret = 0;
err:
	return ret;
}

int GGMTree_GetNextLeaf(ggmtree_ctx_t* ctx, uint8_t lseed[MQOM2_PARAM_SEED_SIZE]) {
	uint32_t j;
	int ret = -1;

	if (ctx->active) {
		uint32_t new_num_leaf = ctx->num_leaf + 1;
		uint32_t diff = ctx->num_leaf ^ new_num_leaf;
		ctx->num_leaf = new_num_leaf;
		j = 1;
		while (((diff >> (MQOM2_PARAM_NB_EVALS_LOG - j)) & 0x1) == 0) {
			j++;
		}
		xor_blocks(ctx->path[j - 1], ctx->path[j], ctx->path[j]);
	} else {
		ctx->num_leaf = 0;
		ctx->active = 1;
		j = 1;
	}
	for (; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		if (j < GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY + 1) {
			enc_ctx DECL_VAR(ctx_enc);
			ret = enc_key_sched(&ctx_enc, ctx->tweaked_salts[j - 1]);
			ERR(ret, err1);
			SeedDerive(&ctx_enc, ctx->path[j], ctx->path[j + 1]);
err1:
			enc_clean_ctx(&ctx_enc);
			ERR(ret, err);
		} else {
			enc_ctx* ctx_enc_precomputed = &ctx->ctx_enc[j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY];
			SeedDerive(ctx_enc_precomputed, ctx->path[j], ctx->path[j + 1]);
		}
	}
	memcpy(lseed, ctx->path[MQOM2_PARAM_NB_EVALS_LOG], MQOM2_PARAM_SEED_SIZE);

	ret = 0;
err:
	return ret;
}

int GGMTree_InitIncrementalExpansion_x2(ggmtree_ctx_x2_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t rseed[2][MQOM2_PARAM_SEED_SIZE], const uint8_t delta[MQOM2_PARAM_SEED_SIZE], const uint32_t e[2]) {
	uint32_t j;
	int ret = -1;
	uint8_t tweaked_salt[2][MQOM2_PARAM_SEED_SIZE];
	for (j = 1; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		TweakSalt(salt, tweaked_salt[0], 2, e[0], j - 1);
		TweakSalt(salt, tweaked_salt[1], 2, e[1], j - 1);
		if (j < GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY + 1) {
			memcpy(ctx->tweaked_salts[0][j - 1], tweaked_salt[0], MQOM2_PARAM_SALT_SIZE);
			memcpy(ctx->tweaked_salts[1][j - 1], tweaked_salt[1], MQOM2_PARAM_SALT_SIZE);
		} else {
			ret = enc_key_sched_x2(&ctx->ctx_enc_x2[j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY], tweaked_salt[0], tweaked_salt[1]);
			ERR(ret, err);
		}
	}
	memcpy(ctx->path[0][0], delta, MQOM2_PARAM_SEED_SIZE);
	memcpy(ctx->path[0][1], rseed[0], MQOM2_PARAM_SEED_SIZE);
	memcpy(ctx->path[1][0], delta, MQOM2_PARAM_SEED_SIZE);
	memcpy(ctx->path[1][1], rseed[1], MQOM2_PARAM_SEED_SIZE);
	ctx->active = 0;

	ret = 0;
err:
	return ret;
}

int GGMTree_GetNextLeaf_x2(ggmtree_ctx_x2_t* ctx, uint8_t lseed[2][MQOM2_PARAM_SEED_SIZE]) {
	enc_ctx_x2 DECL_VAR(ctx_enc_x2);
	uint32_t j;
	int ret = -1;

	if (ctx->active) {
		uint32_t new_num_leaf = ctx->num_leaf + 1;
		uint32_t diff = ctx->num_leaf ^ new_num_leaf;
		ctx->num_leaf = new_num_leaf;
		j = 1;
		while (((diff >> (MQOM2_PARAM_NB_EVALS_LOG - j)) & 0x1) == 0) {
			j++;
		}
		xor_blocks(ctx->path[0][j - 1], ctx->path[0][j], ctx->path[0][j]);
		xor_blocks(ctx->path[1][j - 1], ctx->path[1][j], ctx->path[1][j]);
	} else {
		ctx->num_leaf = 0;
		ctx->active = 1;
		j = 1;
	}
	enc_ctx_x2* ctx_enc_ptr = &ctx_enc_x2;
	for (; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		if (j < GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY + 1) {
			ret = enc_key_sched_x2(ctx_enc_ptr, ctx->tweaked_salts[0][j-1], ctx->tweaked_salts[1][j-1]);
			ERR(ret, err);
		} else {
			ctx_enc_ptr = &ctx->ctx_enc_x2[j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY];
		}
		SeedDerive_x2_x2(ctx_enc_ptr,
		              ctx->path[0][j], ctx->path[1][j],
		              ctx->path[0][j + 1], ctx->path[1][j + 1]);
	}
	memcpy(lseed[0], ctx->path[0][MQOM2_PARAM_NB_EVALS_LOG], MQOM2_PARAM_SEED_SIZE);
	memcpy(lseed[1], ctx->path[1][MQOM2_PARAM_NB_EVALS_LOG], MQOM2_PARAM_SEED_SIZE);

	ret = 0;
err:
	enc_clean_ctx_x2(&ctx_enc_x2);
	return ret;
}

int GGMTree_InitIncrementalExpansion_x4(ggmtree_ctx_x4_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t rseed[4][MQOM2_PARAM_SEED_SIZE], const uint8_t delta[MQOM2_PARAM_SEED_SIZE], const uint32_t e[4]) {
	uint32_t j;
	int ret = -1;
	uint8_t tweaked_salt[4][MQOM2_PARAM_SEED_SIZE];
	for (j = 1; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		TweakSalt(salt, tweaked_salt[0], 2, e[0], j - 1);
		TweakSalt(salt, tweaked_salt[1], 2, e[1], j - 1);
		TweakSalt(salt, tweaked_salt[2], 2, e[2], j - 1);
		TweakSalt(salt, tweaked_salt[3], 2, e[3], j - 1);
		if (j < GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY + 1) {
			memcpy(ctx->tweaked_salts[0][j - 1], tweaked_salt[0], MQOM2_PARAM_SALT_SIZE);
			memcpy(ctx->tweaked_salts[1][j - 1], tweaked_salt[1], MQOM2_PARAM_SALT_SIZE);
			memcpy(ctx->tweaked_salts[2][j - 1], tweaked_salt[2], MQOM2_PARAM_SALT_SIZE);
			memcpy(ctx->tweaked_salts[3][j - 1], tweaked_salt[3], MQOM2_PARAM_SALT_SIZE);
		} else {
			ret = enc_key_sched_x4(&ctx->ctx_enc_x4[j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY], tweaked_salt[0], tweaked_salt[1], tweaked_salt[2], tweaked_salt[3]);
			ERR(ret, err);
		}
	}
	memcpy(ctx->path[0][0], delta, MQOM2_PARAM_SEED_SIZE);
	memcpy(ctx->path[0][1], rseed[0], MQOM2_PARAM_SEED_SIZE);
	memcpy(ctx->path[1][0], delta, MQOM2_PARAM_SEED_SIZE);
	memcpy(ctx->path[1][1], rseed[1], MQOM2_PARAM_SEED_SIZE);
	memcpy(ctx->path[2][0], delta, MQOM2_PARAM_SEED_SIZE);
	memcpy(ctx->path[2][1], rseed[2], MQOM2_PARAM_SEED_SIZE);
	memcpy(ctx->path[3][0], delta, MQOM2_PARAM_SEED_SIZE);
	memcpy(ctx->path[3][1], rseed[3], MQOM2_PARAM_SEED_SIZE);
	ctx->active = 0;

	ret = 0;
err:
	return ret;
}

int GGMTree_GetNextLeaf_x4(ggmtree_ctx_x4_t* ctx, uint8_t lseed[4][MQOM2_PARAM_SEED_SIZE]) {
	enc_ctx_x4 DECL_VAR(ctx_enc_x4);
	uint32_t j;
	int ret = -1;

	if (ctx->active) {
		uint32_t new_num_leaf = ctx->num_leaf + 1;
		uint32_t diff = ctx->num_leaf ^ new_num_leaf;
		ctx->num_leaf = new_num_leaf;
		j = 1;
		while (((diff >> (MQOM2_PARAM_NB_EVALS_LOG - j)) & 0x1) == 0) {
			j++;
		}
		xor_blocks(ctx->path[0][j - 1], ctx->path[0][j], ctx->path[0][j]);
		xor_blocks(ctx->path[1][j - 1], ctx->path[1][j], ctx->path[1][j]);
		xor_blocks(ctx->path[2][j - 1], ctx->path[2][j], ctx->path[2][j]);
		xor_blocks(ctx->path[3][j - 1], ctx->path[3][j], ctx->path[3][j]);
	} else {
		ctx->num_leaf = 0;
		ctx->active = 1;
		j = 1;
	}
	enc_ctx_x4* ctx_enc_ptr = &ctx_enc_x4;
	for (; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		if (j < GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY + 1) {
			ret = enc_key_sched_x4(ctx_enc_ptr, ctx->tweaked_salts[0][j-1], ctx->tweaked_salts[1][j-1], ctx->tweaked_salts[2][j-1], ctx->tweaked_salts[3][j-1]);
			ERR(ret, err);
		} else {
			ctx_enc_ptr = &ctx->ctx_enc_x4[j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY];
		}
		SeedDerive_x4_x4(ctx_enc_ptr,
		              ctx->path[0][j], ctx->path[1][j], ctx->path[2][j], ctx->path[3][j],
		              ctx->path[0][j + 1], ctx->path[1][j + 1], ctx->path[2][j + 1], ctx->path[3][j + 1]);
	}
	memcpy(lseed[0], ctx->path[0][MQOM2_PARAM_NB_EVALS_LOG], MQOM2_PARAM_SEED_SIZE);
	memcpy(lseed[1], ctx->path[1][MQOM2_PARAM_NB_EVALS_LOG], MQOM2_PARAM_SEED_SIZE);
	memcpy(lseed[2], ctx->path[2][MQOM2_PARAM_NB_EVALS_LOG], MQOM2_PARAM_SEED_SIZE);
	memcpy(lseed[3], ctx->path[3][MQOM2_PARAM_NB_EVALS_LOG], MQOM2_PARAM_SEED_SIZE);

	ret = 0;
err:
	enc_clean_ctx_x4(&ctx_enc_x4);
	return ret;
}

int GGMTree_InitIncrementalPartialExpansion(ggmtree_ctx_partial_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t path[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE], uint32_t e, uint32_t i_star) {
	uint32_t j;
	int ret = -1;
	uint8_t tweaked_salt[MQOM2_PARAM_SEED_SIZE];
	for (j = 1; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		if (j < GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY + 1) {
			TweakSalt(salt, ctx->tweaked_salts[j - 1], 2, e, j - 1);
		} else {
			TweakSalt(salt, tweaked_salt, 2, e, j - 1);
			ret = enc_key_sched_pub(&ctx->ctx_enc[j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY], tweaked_salt);
			ERR(ret, err);
		}
	}
	memcpy((uint8_t*) ctx->opening, (uint8_t*) path, sizeof(ctx->opening));
	ctx->i_star = i_star;
	ctx->active = 0;

	ret = 0;
err:
	return ret;
}

int GGMTree_GetNextLeafPartial(ggmtree_ctx_partial_t* ctx, uint8_t lseed[MQOM2_PARAM_SEED_SIZE]) {
	uint32_t j;
	int ret = -1;

	if (ctx->active) {
		uint32_t new_num_leaf = ctx->num_leaf + 1;
		uint32_t diff = ctx->num_leaf ^ new_num_leaf;
		ctx->num_leaf = new_num_leaf;
		j = 1;
		while (((diff >> (MQOM2_PARAM_NB_EVALS_LOG - j)) & 0x1) == 0) {
			j++;
		}
	} else {
		ctx->num_leaf = 0;
		ctx->active = 1;
		j = 1;
	}
	uint32_t diff2 = ctx->num_leaf ^ ctx->i_star;
	if (diff2 == 0) {
		memset(lseed, 0, MQOM2_PARAM_SEED_SIZE);
	} else {
		uint32_t higher = 1;
		while (((diff2 >> (MQOM2_PARAM_NB_EVALS_LOG - higher)) & 0x1) == 0) {
			higher++;
		}
		if (j <= higher) {
			memcpy(ctx->path[higher], ctx->opening[MQOM2_PARAM_NB_EVALS_LOG - higher], MQOM2_PARAM_SEED_SIZE);
			j = higher;
		} else { // j > higher
			xor_blocks(ctx->path[j - 1], ctx->path[j], ctx->path[j]);
		}
		for (; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
			if (j < GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY + 1) {
				enc_ctx_pub DECL_VAR(ctx_enc);
				ret = enc_key_sched_pub(&ctx_enc, ctx->tweaked_salts[j - 1]);
				ERR(ret, err1);
				SeedDerive_pub(&ctx_enc, ctx->path[j], ctx->path[j + 1]);
err1:
				enc_clean_ctx_pub(&ctx_enc);
				ERR(ret, err);
			} else {
				enc_ctx_pub* ctx_enc_precomputed = &ctx->ctx_enc[j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY];
				SeedDerive_pub(ctx_enc_precomputed, ctx->path[j], ctx->path[j + 1]);
			}
		}
		memcpy(lseed, ctx->path[MQOM2_PARAM_NB_EVALS_LOG], MQOM2_PARAM_SEED_SIZE);
	}

	ret = 0;
err:
	return ret;
}

int GGMTree_InitIncrementalPartialExpansion_x2(ggmtree_ctx_partial_x2_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t (*path[2])[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE], const uint32_t e[2], const uint32_t i_star[2]) {
	uint32_t i, j;
	int ret = -1;
	uint8_t tweaked_salt[MQOM2_PARAM_SEED_SIZE];
	for (i = 0; i < 2; i++) {
		for (j = 1; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
			if (j < GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY + 1) {
				TweakSalt(salt, ctx->tweaked_salts[i][j - 1], 2, e[i], j - 1);
			} else {
				TweakSalt(salt, tweaked_salt, 2, e[i], j - 1);
				ret = enc_key_sched_pub(&ctx->ctx_enc[i][j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY], tweaked_salt);
				ERR(ret, err);
			}
		}
	}
	memcpy((uint8_t*) ctx->opening[0], (uint8_t*) path[0], sizeof(ctx->opening[0]));
	memcpy((uint8_t*) ctx->opening[1], (uint8_t*) path[1], sizeof(ctx->opening[1]));
	ctx->i_star[0] = i_star[0];
	ctx->i_star[1] = i_star[1];
	ctx->active = 0;

	ret = 0;
err:
	return ret;
}

int GGMTree_GetNextLeafPartial_x2(ggmtree_ctx_partial_x2_t* ctx, uint8_t lseed[2][MQOM2_PARAM_SEED_SIZE]) {
	enc_ctx_pub DECL_VAR(ctx_enc[2]);
	uint32_t i, j;
	int ret = -1;

	if (ctx->active) {
		uint32_t new_num_leaf = ctx->num_leaf + 1;
		uint32_t diff = ctx->num_leaf ^ new_num_leaf;
		ctx->num_leaf = new_num_leaf;
		j = 1;
		while (((diff >> (MQOM2_PARAM_NB_EVALS_LOG - j)) & 0x1) == 0) {
			j++;
		}
	} else {
		ctx->num_leaf = 0;
		ctx->active = 1;
		j = 1;
	}
	uint32_t diffs[2] = {ctx->num_leaf ^ctx->i_star[0], ctx->num_leaf ^ctx->i_star[1]};
	uint8_t node_map[2][MQOM2_PARAM_NB_EVALS_LOG];
	for (i = 0; i < 2; i++) {
		uint32_t higher = 1;
		if (diffs[i]) {
			while (((diffs[i] >> (MQOM2_PARAM_NB_EVALS_LOG - higher)) & 0x1) == 0) {
				higher++;
			}
		} else {
			higher = MQOM2_PARAM_NB_EVALS_LOG;
		}
		for (uint32_t k = j; k < higher; k++) {
			node_map[i][k] = 0;
		}
		for (uint32_t k = higher; k < MQOM2_PARAM_NB_EVALS_LOG; k++) {
			node_map[i][k] = 1;
		}
		if (j > higher) {
			xor_blocks(ctx->path[i][j - 1], ctx->path[i][j], ctx->path[i][j]);
		} else if (j <= higher && diffs[i]) {
			memcpy(ctx->path[i][higher], ctx->opening[i][MQOM2_PARAM_NB_EVALS_LOG - higher], MQOM2_PARAM_SEED_SIZE);
		}
	}
	for (; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		enc_ctx_pub* ctx_enc0 = NULL;
		enc_ctx_pub* ctx_enc1 = NULL;
		uint8_t *seed0 = node_map[0][j] ? ctx->path[0][j] : NULL;
		uint8_t *seed1 = node_map[1][j] ? ctx->path[1][j] : NULL;
		uint8_t *new_seed0 = ctx->path[0][j + 1];
		uint8_t *new_seed1 = ctx->path[1][j + 1];
		if (j < GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY + 1) {
			ctx_enc0 = &ctx_enc[0];
			ctx_enc1 = &ctx_enc[1];
			ret = enc_key_sched_pub(ctx_enc0, ctx->tweaked_salts[0][j - 1]);
			ERR(ret, err);
			ret = enc_key_sched_pub(ctx_enc1, ctx->tweaked_salts[1][j - 1]);
			ERR(ret, err);
		} else {
			ctx_enc0 = &ctx->ctx_enc[0][j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY];
			ctx_enc1 = &ctx->ctx_enc[1][j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY];
		}
		SeedDerive_x2_pub(ctx_enc0, ctx_enc1,
		                  seed0, seed1,
		                  new_seed0, new_seed1);
	}
	for (i = 0; i < 2; i++) {
		if (diffs[i]) {
			memcpy(lseed[i], ctx->path[i][MQOM2_PARAM_NB_EVALS_LOG], MQOM2_PARAM_SEED_SIZE);
		} else {
			memset(lseed[i], 0, MQOM2_PARAM_SEED_SIZE);
		}
	}

	ret = 0;
err:
	for (j = 0; j < 2; j++) {
		enc_clean_ctx_pub(&ctx_enc[j]);
	}

	return ret;
}

int GGMTree_InitIncrementalPartialExpansion_x4(ggmtree_ctx_partial_x4_t* ctx, const uint8_t salt[MQOM2_PARAM_SALT_SIZE], const uint8_t (*path[4])[MQOM2_PARAM_NB_EVALS_LOG][MQOM2_PARAM_SEED_SIZE], const uint32_t e[4], const uint32_t i_star[4]) {
	uint32_t i, j;
	int ret = -1;
	uint8_t tweaked_salt[MQOM2_PARAM_SEED_SIZE];
	for (i = 0; i < 4; i++) {
		for (j = 1; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
			if (j < GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY + 1) {
				TweakSalt(salt, ctx->tweaked_salts[i][j - 1], 2, e[i], j - 1);
			} else {
				TweakSalt(salt, tweaked_salt, 2, e[i], j - 1);
				ret = enc_key_sched_pub(&ctx->ctx_enc[i][j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY], tweaked_salt);
				ERR(ret, err);
			}
		}
	}
	memcpy((uint8_t*) ctx->opening[0], (uint8_t*) path[0], sizeof(ctx->opening[0]));
	memcpy((uint8_t*) ctx->opening[1], (uint8_t*) path[1], sizeof(ctx->opening[1]));
	memcpy((uint8_t*) ctx->opening[2], (uint8_t*) path[2], sizeof(ctx->opening[2]));
	memcpy((uint8_t*) ctx->opening[3], (uint8_t*) path[3], sizeof(ctx->opening[3]));
	ctx->i_star[0] = i_star[0];
	ctx->i_star[1] = i_star[1];
	ctx->i_star[2] = i_star[2];
	ctx->i_star[3] = i_star[3];
	ctx->active = 0;

	ret = 0;
err:
	return ret;
}

int GGMTree_GetNextLeafPartial_x4(ggmtree_ctx_partial_x4_t* ctx, uint8_t lseed[4][MQOM2_PARAM_SEED_SIZE]) {
	enc_ctx_pub DECL_VAR(ctx_enc[4]);
	uint32_t i, j;
	int ret = -1;

	if (ctx->active) {
		uint32_t new_num_leaf = ctx->num_leaf + 1;
		uint32_t diff = ctx->num_leaf ^ new_num_leaf;
		ctx->num_leaf = new_num_leaf;
		j = 1;
		while (((diff >> (MQOM2_PARAM_NB_EVALS_LOG - j)) & 0x1) == 0) {
			j++;
		}
	} else {
		ctx->num_leaf = 0;
		ctx->active = 1;
		j = 1;
	}
	uint32_t diffs[4] = {ctx->num_leaf ^ctx->i_star[0], ctx->num_leaf ^ctx->i_star[1], ctx->num_leaf ^ctx->i_star[2], ctx->num_leaf ^ctx->i_star[3]};
	uint8_t node_map[4][MQOM2_PARAM_NB_EVALS_LOG];
	for (i = 0; i < 4; i++) {
		uint32_t higher = 1;
		if (diffs[i]) {
			while (((diffs[i] >> (MQOM2_PARAM_NB_EVALS_LOG - higher)) & 0x1) == 0) {
				higher++;
			}
		} else {
			higher = MQOM2_PARAM_NB_EVALS_LOG;
		}
		for (uint32_t k = j; k < higher; k++) {
			node_map[i][k] = 0;
		}
		for (uint32_t k = higher; k < MQOM2_PARAM_NB_EVALS_LOG; k++) {
			node_map[i][k] = 1;
		}
		if (j > higher) {
			xor_blocks(ctx->path[i][j - 1], ctx->path[i][j], ctx->path[i][j]);
		} else if (j <= higher && diffs[i]) {
			memcpy(ctx->path[i][higher], ctx->opening[i][MQOM2_PARAM_NB_EVALS_LOG - higher], MQOM2_PARAM_SEED_SIZE);
		}
	}
	for (; j < MQOM2_PARAM_NB_EVALS_LOG; j++) {
		enc_ctx_pub* ctx_enc0 = NULL;
		enc_ctx_pub* ctx_enc1 = NULL;
		enc_ctx_pub* ctx_enc2 = NULL;
		enc_ctx_pub* ctx_enc3 = NULL;
		uint8_t *seed0 = node_map[0][j] ? ctx->path[0][j] : NULL;
		uint8_t *seed1 = node_map[1][j] ? ctx->path[1][j] : NULL;
		uint8_t *seed2 = node_map[2][j] ? ctx->path[2][j] : NULL;
		uint8_t *seed3 = node_map[3][j] ? ctx->path[3][j] : NULL;
		uint8_t *new_seed0 = ctx->path[0][j + 1];
		uint8_t *new_seed1 = ctx->path[1][j + 1];
		uint8_t *new_seed2 = ctx->path[2][j + 1];
		uint8_t *new_seed3 = ctx->path[3][j + 1];
		if (j < GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY + 1) {
			ctx_enc0 = &ctx_enc[0];
			ctx_enc1 = &ctx_enc[1];
			ctx_enc2 = &ctx_enc[2];
			ctx_enc3 = &ctx_enc[3];
			ret = enc_key_sched_pub(ctx_enc0, ctx->tweaked_salts[0][j - 1]);
			ERR(ret, err);
			ret = enc_key_sched_pub(ctx_enc1, ctx->tweaked_salts[1][j - 1]);
			ERR(ret, err);
			ret = enc_key_sched_pub(ctx_enc2, ctx->tweaked_salts[2][j - 1]);
			ERR(ret, err);
			ret = enc_key_sched_pub(ctx_enc3, ctx->tweaked_salts[3][j - 1]);
			ERR(ret, err);
		} else {
			ctx_enc0 = &ctx->ctx_enc[0][j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY];
			ctx_enc1 = &ctx->ctx_enc[1][j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY];
			ctx_enc2 = &ctx->ctx_enc[2][j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY];
			ctx_enc3 = &ctx->ctx_enc[3][j - 1 - GGMTREE_NB_TWEAKED_SALTS_IN_MEMORY];
		}
		SeedDerive_x4_pub(ctx_enc0, ctx_enc1, ctx_enc2, ctx_enc3,
		                  seed0, seed1, seed2, seed3,
		                  new_seed0, new_seed1, new_seed2, new_seed3);
	}
	for (i = 0; i < 4; i++) {
		if (diffs[i]) {
			memcpy(lseed[i], ctx->path[i][MQOM2_PARAM_NB_EVALS_LOG], MQOM2_PARAM_SEED_SIZE);
		} else {
			memset(lseed[i], 0, MQOM2_PARAM_SEED_SIZE);
		}
	}

	ret = 0;
err:
	for (j = 0; j < 4; j++) {
		enc_clean_ctx_pub(&ctx_enc[j]);
	}

	return ret;
}
