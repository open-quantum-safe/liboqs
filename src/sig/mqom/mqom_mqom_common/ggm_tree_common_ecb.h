#ifndef __GGM_TREE_COMMON_ECB_H__
#define __GGM_TREE_COMMON_ECB_H__

#include "common.h"
#include "enc.h"
/* For mqom_cleanse */
#include "ggm_tree_common.h"

static inline int SeedDerive_ecb(enc_ctx_ecb *ctx, const uint8_t seed[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed[MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
    uint8_t linortho_seed[MQOM3_PARAM_SEED_SIZE];
    LinOrtho(seed, linortho_seed);
    /* Encrypt the LinOrtho seed with the tweaked salt */
	ret = enc_encrypt_ecb(ctx, 1, seed, new_seed);
	ERR(ret, err);
    /* Xor with LinOrtho seed */
    xor_blocks(new_seed, linortho_seed, new_seed);
	ret = 0;
err:
	mqom_cleanse(linortho_seed, sizeof(linortho_seed));
	return ret;
}

static inline int SeedDerive_pub_ecb(enc_ctx_pub_ecb *ctx, const uint8_t seed[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed[MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
    uint8_t linortho_seed[MQOM3_PARAM_SEED_SIZE];
    LinOrtho(seed, linortho_seed);
    /* Encrypt the LinOrtho seed with the tweaked salt */
	ret = enc_encrypt_pub_ecb(ctx, 1, seed, new_seed);
	ERR(ret, err);
    /* Xor with LinOrtho seed */
    xor_blocks(new_seed, linortho_seed, new_seed);
	ret = 0;
err:
	return ret;
}

/* PRECONDITION when children[] and in_seeds[] overlap (in-place derivation):
 * children must start at or after the end of in_seeds, i.e.
 *     children >= in_seeds + nb_in_seeds. */
static inline int SeedDerive_nb_ecb(enc_ctx_ecb *ctx, const uint8_t in_seeds[][MQOM3_PARAM_SEED_SIZE], uint8_t children[][MQOM3_PARAM_SEED_SIZE], uint32_t nb_in_seeds) {
	int ret = 0;
	/* Contiguous so the whole scratch can be wiped with a single mqom_cleanse
	 * call below, instead of one (non-inlinable, volatile-fnptr) call per buffer. */
	uint8_t buf[2][MQOM3_PARAM_SEED_SIZE];
	uint8_t *linortho_seed = buf[0];
	uint8_t *enc_seed = buf[1];
	uint32_t i;

	/* Encrypt the LinOrtho seed with the tweaked salt */
	if (nb_in_seeds == 0) {
		goto err;
	}
	ret = enc_encrypt_ecb(ctx, nb_in_seeds, (const uint8_t*) in_seeds, (uint8_t*) children);
	ERR(ret, err);

	/* Interleave all the derived seeds, xor with the LinOrtho seed and derive
	 * the sibling nodes -- all in a single backward pass. Walking i downwards
	 * is what makes the in-place de-interleaving safe: when step i runs, the
	 * only slots already rewritten are 2i' and 2i'+1 for i' > i, i.e. indices
	 * >= 2i+2 > i, so children[i] still holds its raw ciphertext.
	 * (This used to be three separate loops -- an interleaving memcpy pass,
	 * then a LinOrtho-xor pass, then a sibling pass -- over the same data.) */
	for (i = nb_in_seeds; i > 0; i--) {
		uint32_t k = i - 1;
		memcpy(enc_seed, children[k], MQOM3_PARAM_SEED_SIZE);
		/* Xor with LinOrtho seed */
		LinOrtho(in_seeds[k], linortho_seed);
		xor_blocks(enc_seed, linortho_seed, children[2 * k]);

		/* Derive the sibling node */
		xor_blocks(children[2 * k], in_seeds[k], children[(2 * k) + 1]);
	}
err:
	mqom_cleanse(buf, sizeof(buf));
	return ret;
}

/* Same in-place precondition as SeedDerive_nb_ecb above:
 * children >= in_seeds + nb_in_seeds whenever the two overlap. */
static inline int SeedDerive_nb_pub_ecb(enc_ctx_pub_ecb *ctx, const uint8_t in_seeds[][MQOM3_PARAM_SEED_SIZE], uint8_t children[][MQOM3_PARAM_SEED_SIZE], uint32_t nb_in_seeds) {
	int ret = 0;
	/* Kept contiguous to mirror SeedDerive_nb_ecb above, though this flavour has
	 * nothing to wipe (see the err: label). */
	uint8_t buf[2][MQOM3_PARAM_SEED_SIZE];
	uint8_t *linortho_seed = buf[0];
	uint8_t *enc_seed = buf[1];
	uint32_t i;

	/* Encrypt the LinOrtho seed with the tweaked salt */
	if (nb_in_seeds == 0) {
		goto err;
	}
	ret = enc_encrypt_pub_ecb(ctx, nb_in_seeds, (const uint8_t*) in_seeds, (uint8_t*) children);
	ERR(ret, err);

	/* Interleave all the derived seeds, xor with the LinOrtho seed and derive
	 * the sibling nodes -- single backward pass, see SeedDerive_nb_ecb above
	 * for why walking i downwards keeps the in-place de-interleaving safe. */
	for (i = nb_in_seeds; i > 0; i--) {
		uint32_t k = i - 1;
		memcpy(enc_seed, children[k], MQOM3_PARAM_SEED_SIZE);
		/* Xor with LinOrtho seed */
		LinOrtho(in_seeds[k], linortho_seed);
		xor_blocks(enc_seed, linortho_seed, children[2 * k]);

		/* Derive the sibling node */
		xor_blocks(children[2 * k], in_seeds[k], children[(2 * k) + 1]);
	}
err:
	return ret;
}

/* SeedDerive_x8_ecb: same key for all 8 derivations (the common GGM-tree-level
 * case, where ctx1..ctx8 in SeedDerive_x8 below are always the same *ctx).
 * Uses the existing ECB batch-encrypt primitive. */
static inline int SeedDerive_x8_ecb(enc_ctx_ecb *ctx,
                                 const uint8_t seed1[MQOM3_PARAM_SEED_SIZE], const uint8_t seed2[MQOM3_PARAM_SEED_SIZE],
                                 const uint8_t seed3[MQOM3_PARAM_SEED_SIZE], const uint8_t seed4[MQOM3_PARAM_SEED_SIZE],
                                 const uint8_t seed5[MQOM3_PARAM_SEED_SIZE], const uint8_t seed6[MQOM3_PARAM_SEED_SIZE],
                                 const uint8_t seed7[MQOM3_PARAM_SEED_SIZE], const uint8_t seed8[MQOM3_PARAM_SEED_SIZE],
                                 uint8_t new_seed1[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed2[MQOM3_PARAM_SEED_SIZE],
                                 uint8_t new_seed3[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed4[MQOM3_PARAM_SEED_SIZE],
                                 uint8_t new_seed5[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed6[MQOM3_PARAM_SEED_SIZE],
                                 uint8_t new_seed7[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed8[MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	const uint8_t *seeds[8]    = { seed1, seed2, seed3, seed4, seed5, seed6, seed7, seed8 };
	uint8_t *new_seeds[8]      = { new_seed1, new_seed2, new_seed3, new_seed4, new_seed5, new_seed6, new_seed7, new_seed8 };
	/* gather/enc_out/linortho are aliases into one contiguous buffer, so the
	 * whole batch can be wiped with a single mqom_cleanse call below, instead
	 * of one (non-inlinable, volatile-fnptr) call per sub-buffer. */
	uint8_t buf[3][8][MQOM3_PARAM_SEED_SIZE];
	uint8_t (*gather)[MQOM3_PARAM_SEED_SIZE]   = buf[0];
	uint8_t (*enc_out)[MQOM3_PARAM_SEED_SIZE]  = buf[1];
	uint8_t (*linortho)[MQOM3_PARAM_SEED_SIZE] = buf[2];
	uint32_t dst_of[8];
	uint32_t n = 0, i;

	for (i = 0; i < 8; i++) {
		if (seeds[i] != NULL) {
			memcpy(gather[n], seeds[i], MQOM3_PARAM_SEED_SIZE);
			LinOrtho(seeds[i], linortho[n]);
			dst_of[n] = i;
			n++;
		}
	}
	if (n == 0) {
		ret = 0;
		goto err;
	}

	ret = enc_encrypt_ecb(ctx, n, gather[0], enc_out[0]);
	ERR(ret, err);

	for (i = 0; i < n; i++) {
		xor_blocks(enc_out[i], linortho[i], new_seeds[dst_of[i]]);
	}
	ret = 0;
err:
	mqom_cleanse(buf, sizeof(buf));
	return ret;
}
/* See SeedDerive_x8_ecb above: same NULL-tolerant compaction, public variant. */
static inline int SeedDerive_x8_pub_ecb(enc_ctx_pub_ecb *ctx,
                                 const uint8_t seed1[MQOM3_PARAM_SEED_SIZE], const uint8_t seed2[MQOM3_PARAM_SEED_SIZE],
                                 const uint8_t seed3[MQOM3_PARAM_SEED_SIZE], const uint8_t seed4[MQOM3_PARAM_SEED_SIZE],
                                 const uint8_t seed5[MQOM3_PARAM_SEED_SIZE], const uint8_t seed6[MQOM3_PARAM_SEED_SIZE],
                                 const uint8_t seed7[MQOM3_PARAM_SEED_SIZE], const uint8_t seed8[MQOM3_PARAM_SEED_SIZE],
                                 uint8_t new_seed1[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed2[MQOM3_PARAM_SEED_SIZE],
                                 uint8_t new_seed3[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed4[MQOM3_PARAM_SEED_SIZE],
                                 uint8_t new_seed5[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed6[MQOM3_PARAM_SEED_SIZE],
                                 uint8_t new_seed7[MQOM3_PARAM_SEED_SIZE], uint8_t new_seed8[MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	const uint8_t *seeds[8]    = { seed1, seed2, seed3, seed4, seed5, seed6, seed7, seed8 };
	uint8_t *new_seeds[8]      = { new_seed1, new_seed2, new_seed3, new_seed4, new_seed5, new_seed6, new_seed7, new_seed8 };
	uint8_t buf[3][8][MQOM3_PARAM_SEED_SIZE];
	uint8_t (*gather)[MQOM3_PARAM_SEED_SIZE]   = buf[0];
	uint8_t (*enc_out)[MQOM3_PARAM_SEED_SIZE]  = buf[1];
	uint8_t (*linortho)[MQOM3_PARAM_SEED_SIZE] = buf[2];
	uint32_t dst_of[8];
	uint32_t n = 0, i;

	for (i = 0; i < 8; i++) {
		if (seeds[i] != NULL) {
			memcpy(gather[n], seeds[i], MQOM3_PARAM_SEED_SIZE);
			LinOrtho(seeds[i], linortho[n]);
			dst_of[n] = i;
			n++;
		}
	}
	if (n == 0) {
		ret = 0;
		goto err;
	}

	ret = enc_encrypt_pub_ecb(ctx, n, gather[0], enc_out[0]);
	ERR(ret, err);

	for (i = 0; i < n; i++) {
		xor_blocks(enc_out[i], linortho[i], new_seeds[dst_of[i]]);
	}
	ret = 0;
err:
	return ret;
}

#endif /* __GGM_TREE_COMMON_ECB_H__ */
