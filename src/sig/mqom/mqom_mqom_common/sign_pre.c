#ifdef SUPERCOP
#include "crypto_sign.h"
#include "crypto_declassify.h"
#else
#include "api.h"
#endif

#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#include "common.h"
#include "fields.h"
#include "xof.h"
#include "expand_mq.h"
#include "piop.h"
#include "benchmark.h"
#include "sign.h"
#include "sign_pre.h"
#include "sample_challenge_sign.h"
#include "domain_separation.h"

/* For crypto_sign_prepare()'s internal randomness - same declaration pattern
 * as crypto_sign.c's crypto_sign_signature(). */
#if !defined(MQOM3_FOR_MUPQ) && !defined(MQOM3_FOR_LIBOQS)
#ifdef SUPERCOP
extern void randombytes(unsigned char* x, unsigned long long xlen);
#else
extern int randombytes(unsigned char* x, unsigned long long xlen);
#endif
#else
#include "randombytes.h"
#endif

/* Compressed opening key ("ckey") - the on-the-wire form of the BLC opening
 * key inside a pre-signature: mseed | Delta_x^(1) in CT, mseed | Delta_x in
 * OT. See the MQOM3_CKEY_SIZE comment in sign_pre.h for why the remaining
 * fields of blc_key_compact_t are redundant there and how each is recovered.
 *
 * parse_ckey() puts those fields back, so the blc_key_compact_t handed to
 * BLC_Open_compact() is bit-for-bit the one BLC_Commit_compact() produced -
 * which is what keeps a Sign_Prepare+Sign_Finalize signature byte-identical
 * to the one a direct Sign() call would produce. */

/* MQOM3_CKEY_SIZE is spelled out in sign_pre.h, while the fields it covers are
 * declared over in blc/blc_{ct,ot}_memopt.h - two separate expressions in two
 * separate files that have to keep agreeing. Hence the sanity checks below.
 * */ 
#if MQOM3_PARAM_OT_VARIANT != 1
MQOM3_STATIC_ASSERT(MQOM3_CKEY_SIZE == (sizeof(((blc_key_compact_t *)0)->mseed)
                                        + sizeof(((blc_key_compact_t *)0)->partial_delta_x)));
MQOM3_STATIC_ASSERT(sizeof(blc_key_compact_t) == (MQOM3_CKEY_SIZE
                    + sizeof(((blc_key_compact_t *)0)->salt)
                    + sizeof(((blc_key_compact_t *)0)->delta)));
#else
MQOM3_STATIC_ASSERT(MQOM3_CKEY_SIZE == (sizeof(((blc_key_compact_t *)0)->mseed)
                                        + sizeof(((blc_key_compact_t *)0)->delta_x)));
MQOM3_STATIC_ASSERT(sizeof(blc_key_compact_t) == (MQOM3_CKEY_SIZE
                    + sizeof(((blc_key_compact_t *)0)->salt)));
#endif

static void serialize_ckey(const blc_key_compact_t *key, uint8_t out[MQOM3_CKEY_SIZE]) {
	memcpy(&out[0], key->mseed, MQOM3_PARAM_SEED_SIZE);
#if MQOM3_PARAM_OT_VARIANT != 1
	memcpy(&out[MQOM3_PARAM_SEED_SIZE], key->partial_delta_x,
	       (size_t)MQOM3_PARAM_TAU * CT_CHUNK_PDELTA_SIZE);
#else
	memcpy(&out[MQOM3_PARAM_SEED_SIZE], key->delta_x,
	       (size_t)MQOM3_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N));
#endif
}

static void parse_ckey(const uint8_t in[MQOM3_CKEY_SIZE], const uint8_t sk[MQOM3_SK_SIZE], const uint8_t salt[MQOM3_PARAM_SALT_SIZE], blc_key_compact_t *key) {
	memcpy(key->salt, salt, MQOM3_PARAM_SALT_SIZE);
	memcpy(key->mseed, &in[0], MQOM3_PARAM_SEED_SIZE);
#if MQOM3_PARAM_OT_VARIANT != 1
	{
		/* delta is recomputed through DeriveDelta() */
		field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
		field_base_parse(&sk[(2 * MQOM3_PARAM_SEED_SIZE) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], MQOM3_PARAM_MQ_N, x);
		DeriveDelta(x, key->delta);
		mqom_cleanse(x, sizeof(x));
	}
	memcpy(key->partial_delta_x, &in[MQOM3_PARAM_SEED_SIZE],
	       (size_t)MQOM3_PARAM_TAU * CT_CHUNK_PDELTA_SIZE);
#else
	/* No delta in the one-tree opening key, hence nothing to recover from sk. */
	(void)sk;
	memcpy(key->delta_x, &in[MQOM3_PARAM_SEED_SIZE],
	       (size_t)MQOM3_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N));
#endif
}

int Sign_Prepare(const uint8_t sk[MQOM3_SK_SIZE], const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], const uint8_t mask_rnd[MQOM3_PARAM_PRESIGN_RND_SIZE], uint8_t presig[MQOM3_PRESIG_SIZE]) {
	int ret = -1;
	uint32_t i;
	int e;
	/* blc_key_compact_t is a small, fixed-size seed representation (never the
	 * fully-expanded GGM tree) - see the comment in sign_pre.h - so unlike
	 * Sign()'s blc_key_t, it never needs a heap fallback here. */
	blc_key_compact_t key_buf = { 0 };
	blc_key_compact_t *key = &key_buf;
	uint8_t mask[MQOM3_PRESIG_DATA_SIZE];
	field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt u0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	field_ext_elt u1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	/* alpha0/alpha1 alias u0/u1 in place: ComputePAlpha_default/_memopt only
	 * ever compute alpha0[e] = tmp + u0[e] (tmp derived from Gamma/z0/z1,
	 * independent of u0[e]) then alpha1[e] similarly from u1[e] - a safe
	 * in-place a = a + b update, verified identical in both PIOP backends
	 * (Sign_Prepare uses the ambient ComputePAlpha, so this must hold for
	 * whichever one gets linked in). Same trick as Sign_default's u0/alpha0
	 * and Sign_memopt's OT branch (sign.c / sign_memopt.c). */
	field_ext_elt (*alpha0)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)] = u0;
	field_ext_elt (*alpha1)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)] = u1;
	uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE];
	uint8_t com2[MQOM3_PARAM_DIGEST_SIZE];
	/* x0 and xof_ctx have disjoint lifetimes: x0 is only live during
	 * BLC_Commit_compact+ComputePAlpha below, entirely before xof_ctx's
	 * first use (Hash_1/Hash_2) - so they share one stack slot, exactly
	 * like Sign_default's x0_xof_workspace union (sign.c). */
	union {
		field_ext_elt x0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
		xof_context xof_ctx;
	} x0_xof_workspace = { 0 };
	field_ext_elt (*x0)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)] = x0_xof_workspace.x0;
	xof_context *xof_ctx = &x0_xof_workspace.xof_ctx;
	int xof_ctx_active = 0;

	const uint8_t *pk = &sk[0];
	field_base_parse(&sk[(2 * MQOM3_PARAM_SEED_SIZE) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], MQOM3_PARAM_MQ_N, x);

	/* Layout the plaintext pre-signature buffer:
	 *   rnd | presig_id | salt | alpha1[TAU] | ckey
	 * Everything past rnd gets masked in place at the end. */
	unsigned int pos = 0;
	memcpy(&presig[pos], mask_rnd, MQOM3_PARAM_PRESIGN_RND_SIZE);
	pos += MQOM3_PARAM_PRESIGN_RND_SIZE;
	uint8_t *presig_id = &presig[pos];
	pos += MQOM3_PARAM_DIGEST_SIZE;
	uint8_t *out_salt = &presig[pos];
	pos += MQOM3_PARAM_SALT_SIZE;
	uint8_t *serialized_alpha1 = &presig[pos];
	pos += MQOM3_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU);
	uint8_t *serialized_ckey = &presig[pos];
	pos += (uint32_t)(MQOM3_CKEY_SIZE);
	/* The last step is never read back: it completes the layout above so a
	 * field added later does not start from a stale offset. */
	(void)pos;

	memcpy(out_salt, salt, MQOM3_PARAM_SALT_SIZE);

	/* Commit Lines (message-independent) - always via the compact/_memopt BLC
	 * functions, regardless of the ambient build's BLC_KEEP_ALL_TREES_IN_MEMORY
	 * setting; see the comment in sign_pre.h. */
	__BENCHMARK_START__(BS_BLC_COMMIT);
	ret = BLC_Commit_compact(mseed, salt, x, com1, key, x0, u0, u1);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_COMMIT);

	/* Compute P_alpha (message-independent) */
	__BENCHMARK_START__(BS_PIOP_COMPUTE);
	ret = ComputePAlpha((const uint8_t (*)[MQOM3_PARAM_DIGEST_SIZE])com1, (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)])x0,
				  (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])u0,
				  (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])u1,
				  x, &sk[0], alpha0, alpha1);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_PIOP_COMPUTE);

	/* x0 is done; re-activate a clean xof_ctx union member (it may still
	 * hold x0 field-element data otherwise, which xof_clean_ctx must never
	 * misinterpret as valid xof_context state). */
	memset(xof_ctx, 0, sizeof(*xof_ctx));

	/* Hash P_alpha (Hash_1: com2) - same transcript as Sign() */
	uint8_t alpha[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU)];
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	xof_ctx_active = 1;
	ret = xof_update(xof_ctx, (const uint8_t*) DOMAIN_SEP_COM2, 1);
	ERR(ret, err);
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		field_ext_serialize(alpha0[e], MQOM3_PARAM_ETA, alpha);
		ret = xof_update(xof_ctx, alpha, BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU));
		ERR(ret, err);
		uint8_t *slot = &serialized_alpha1[e * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU)];
		field_ext_serialize(alpha1[e], MQOM3_PARAM_ETA, slot);
		ret = xof_update(xof_ctx, slot, BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU));
		ERR(ret, err);
	}
	ret = xof_squeeze(xof_ctx, com2, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* presig_id = Hash_2(pk, com1[0..TAU-1], com2) - same transcript as Sign() */
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	ret = xof_update(xof_ctx, (const uint8_t*) DOMAIN_SEP_PRESIGID, 1);
	ERR(ret, err);
	ret = xof_update(xof_ctx, pk, MQOM3_PK_SIZE);
	ERR(ret, err);
	ret = xof_update(xof_ctx, &com1[0][0], MQOM3_PARAM_TAU * MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_update(xof_ctx, com2, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(xof_ctx, presig_id, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* Save the opening key for FinalizeSign, in its compressed (ckey) form */
	serialize_ckey(key, serialized_ckey);

	/* Mask everything past rnd: mask = XOF(sk || rnd, len) - untagged, no
	 * domain-separation byte (spec explicitly uses XOF, not Hash_i/XOF_i). */
	{
		ret = xof_init(xof_ctx);
		ERR(ret, err);
		ret = xof_update(xof_ctx, sk, MQOM3_SK_SIZE);
		ERR(ret, err);
		ret = xof_update(xof_ctx, mask_rnd, MQOM3_PARAM_PRESIGN_RND_SIZE);
		ERR(ret, err);
		ret = xof_squeeze(xof_ctx, mask, MQOM3_PRESIG_DATA_SIZE);
		ERR(ret, err);
		for (i = 0; i < MQOM3_PRESIG_DATA_SIZE; i++) {
			presig[MQOM3_PARAM_PRESIGN_RND_SIZE + i] ^= mask[i];
		}
		mqom_cleanse(mask, MQOM3_PRESIG_DATA_SIZE);
	}

	ret = 0;
err:
	mqom_cleanse((void*)key, sizeof(*key));
	mqom_cleanse((void*)mask, MQOM3_PRESIG_DATA_SIZE);
	mqom_cleanse((void*)x, sizeof(x));
	/* alpha0/alpha1 alias u0/u1 (see declaration above), so cleansing u0/u1
	 * already covers them; a separate cleanse would just re-wipe the same
	 * bytes, and sizeof(alpha0) on a pointer would be wrong anyway. */
	mqom_cleanse((void*)u0, sizeof(u0));
	mqom_cleanse((void*)u1, sizeof(u1));
	mqom_cleanse(alpha, sizeof(alpha));
	/* xof_clean_ctx only covers sizeof(xof_context); cleanse the full union
	 * to erase x0 secret evaluations stored in the larger x0 member. */
	if (xof_ctx_active) {
		xof_clean_ctx(xof_ctx);
	}
	mqom_cleanse(&x0_xof_workspace, sizeof(x0_xof_workspace));
	if (ret) {
		memset(presig, 0, MQOM3_PRESIG_SIZE);
	}
	return ret;
}

int Sign_Finalize(const uint8_t sk[MQOM3_SK_SIZE], const uint8_t *msg, unsigned long long mlen, const uint8_t presig[MQOM3_PRESIG_SIZE], uint8_t sig[MQOM3_SIG_SIZE]) {
	int ret = -1;
	uint32_t i;
	uint8_t msg_hash[MQOM3_PARAM_DIGEST_SIZE];
	xof_context xof_ctx = { 0 };
	/* blc_key_compact_t is small and fixed-size - see the comment in
	 * sign_pre.h - so no heap fallback is needed here. */
	blc_key_compact_t key_buf = { 0 };
	blc_key_compact_t *key = &key_buf;
	uint8_t data[MQOM3_PRESIG_DATA_SIZE];
	uint8_t mask[MQOM3_PRESIG_DATA_SIZE];

	/* De-mask the pre-signature into a plaintext scratch buffer */
	{
		const uint8_t *mask_rnd = &presig[0];
		ret = xof_init(&xof_ctx);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, sk, MQOM3_SK_SIZE);
		ERR(ret, err);
		ret = xof_update(&xof_ctx, mask_rnd, MQOM3_PARAM_PRESIGN_RND_SIZE);
		ERR(ret, err);
		ret = xof_squeeze(&xof_ctx, mask, MQOM3_PRESIG_DATA_SIZE);
		ERR(ret, err);
		for (i = 0; i < MQOM3_PRESIG_DATA_SIZE; i++) {
			data[i] = presig[MQOM3_PARAM_PRESIGN_RND_SIZE + i] ^ mask[i];
		}
		mqom_cleanse(mask, MQOM3_PRESIG_DATA_SIZE);
	}

	/* Parse the plaintext pre-signature data */
	unsigned int pos = 0;
	const uint8_t *presig_id = &data[pos];
	pos += MQOM3_PARAM_DIGEST_SIZE;
	const uint8_t *salt = &data[pos];
	pos += MQOM3_PARAM_SALT_SIZE;
	const uint8_t *serialized_alpha1 = &data[pos];
	pos += MQOM3_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU);
	parse_ckey(&data[pos], sk, salt, key);
	pos += (uint32_t)(MQOM3_CKEY_SIZE);
	/* Same as in Sign_Prepare(): the last step completes the layout and is
	 * deliberately not read back. */
	(void)pos;

	/* Prepare the signature: sig_id[D] | salt | nonce[4] | opening (v3) */
	pos = 0;
	uint8_t *sig_id = &sig[pos];
	pos += MQOM3_PARAM_DIGEST_SIZE;
	memcpy(&sig[pos], salt, MQOM3_PARAM_SALT_SIZE);
	pos += MQOM3_PARAM_SALT_SIZE;
	uint8_t *nonce = &sig[pos];
	pos += 4;
	uint8_t *opening = &sig[pos];

	field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	for (i = 0; i < MQOM3_PARAM_TAU; i++) {
		field_ext_parse(&serialized_alpha1[i * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU)], MQOM3_PARAM_ETA, alpha1[i]);
	}

	/* Hash message (Hash_3: msgdigest) */
	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) DOMAIN_SEP_MSGDIGEST, 1);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, msg, (size_t)mlen);
	ERR(ret, err);
	ret = xof_squeeze(&xof_ctx, msg_hash, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* sig_id = Hash_4(presig_id, msg_hash) - identical transcript to Sign() */
	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) DOMAIN_SEP_SIGID, 1);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, presig_id, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, msg_hash, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(&xof_ctx, sig_id, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* Sample Challenge */
	uint16_t i_star[MQOM3_PARAM_TAU];
	__BENCHMARK_START__(BS_SAMPLE_CHALLENGE);
	ret = SampleChallenge(sig_id, i_star, nonce);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_SAMPLE_CHALLENGE);

	/* Open Line Evaluation - always via the compact/_memopt BLC functions,
	 * regardless of the ambient build's BLC_KEEP_ALL_TREES_IN_MEMORY setting;
	 * see the comment in sign_pre.h. */
	__BENCHMARK_START__(BS_BLC_OPEN);
	ret = BLC_Open_compact(key, i_star, (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])alpha1, opening);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_OPEN);

	ret = 0;
err:
	mqom_cleanse((void*)key, sizeof(*key));
	mqom_cleanse((void*)data, MQOM3_PRESIG_DATA_SIZE);
	mqom_cleanse((void*)mask, MQOM3_PRESIG_DATA_SIZE);
	mqom_cleanse((void*)alpha1, sizeof(alpha1));
	mqom_cleanse((void*)msg_hash, sizeof(msg_hash));
	xof_clean_ctx(&xof_ctx);
	if (ret) {
		memset(sig, 0, MQOM3_SIG_SIZE);
	}
	return ret;
}

int crypto_sign_prepare(uint8_t presig[MQOM3_PRESIG_SIZE], const uint8_t sk[MQOM3_SK_SIZE])
{
	int ret = -1;

	// Sample mseed
	uint8_t mseed[MQOM3_PARAM_SEED_SIZE];
#if defined(SUPERCOP) || defined(MQOM3_FOR_LIBOQS)
	randombytes(mseed, MQOM3_PARAM_SEED_SIZE);
#else
	ret = randombytes(mseed, MQOM3_PARAM_SEED_SIZE);
	ERR(ret, err);
#endif
	// Sample salt
	uint8_t salt[MQOM3_PARAM_SALT_SIZE];
#if defined(SUPERCOP) || defined(MQOM3_FOR_LIBOQS)
	randombytes(salt, MQOM3_PARAM_SALT_SIZE);
#else
	ret = randombytes(salt, MQOM3_PARAM_SALT_SIZE);
	ERR(ret, err);
#endif

#ifdef SUPERCOP
	/* Salt declassification (as it is public) for SUPERCOP */
	crypto_declassify(salt, MQOM3_PARAM_SALT_SIZE);
#endif

	// Sample mask_rnd
	uint8_t mask_rnd[MQOM3_PARAM_PRESIGN_RND_SIZE];
#if defined(SUPERCOP) || defined(MQOM3_FOR_LIBOQS)
	randombytes(mask_rnd, MQOM3_PARAM_PRESIGN_RND_SIZE);
#else
	ret = randombytes(mask_rnd, MQOM3_PARAM_PRESIGN_RND_SIZE);
	ERR(ret, err);
#endif

	ret = Sign_Prepare(sk, salt, mseed, mask_rnd, presig);
	ERR(ret, err);

	ret = 0;
err:
	mqom_cleanse(mseed, sizeof(mseed));
	mqom_cleanse(salt, sizeof(salt));
	mqom_cleanse(mask_rnd, sizeof(mask_rnd));
	return ret;
}

int crypto_sign_finalize(uint8_t sig[MQOM3_SIG_SIZE], unsigned long long *siglen,
                          const uint8_t *msg, unsigned long long mlen,
                          const uint8_t sk[MQOM3_SK_SIZE], uint8_t presig[MQOM3_PRESIG_SIZE])
{
	int ret = -1;

#if !defined(MQOM3_FOR_MUPQ) && !defined(MQOM3_FOR_LIBOQS) && (SIZE_MAX < ULLONG_MAX)
	if (mlen > (unsigned long long)SIZE_MAX) {
		/* Consume presig regardless of outcome - see the comment above the
		 * declaration in sign_pre.h. */
		mqom_cleanse(presig, MQOM3_PRESIG_SIZE);
		if (siglen != NULL) {
			*siglen = 0;
		}
		return -1;
	}
#endif

	ret = Sign_Finalize(sk, msg, (size_t)mlen, presig, sig);
	if (siglen != NULL) {
		*siglen = (ret == 0) ? (unsigned long long) MQOM3_SIG_SIZE : 0;
	}
	/* NOTE: cleanse the presig to enforce one-use only */
	mqom_cleanse(presig, MQOM3_PRESIG_SIZE);
	return ret;
}
