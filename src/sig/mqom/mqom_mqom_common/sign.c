#ifdef SUPERCOP
#include "crypto_sign.h"
#include "crypto_declassify.h"
#else
#include "api.h"
#endif

#include <stdlib.h>
#include "common.h"
#include "fields.h"
#include "xof.h"
#include "expand_mq.h"
#include "blc.h"
#include "piop.h"
#include "benchmark.h"
#include "sign.h"
#include "sample_challenge_sign.h"
#include "sample_challenge_verify.h"
#include "domain_separation.h"

/* The v3 opening is the only signature area large enough to be reusable while
 * signing.  It can be used as typed x0 storage only for the byte-sized
 * extension field; uint16_t storage would not have a valid declared type or a
 * guaranteed alignment in a caller-provided signature buffer. */
#define SIGN_X0_BYTES \
	(MQOM3_PARAM_TAU * BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_N))
/* MQOM3_PARAM_OPENING_SIZE already resolves to this build's family (common.h),
 * so the local CT/OT switch this used to carry is gone. */
#if defined(USE_SIGNATURE_BUFFER_AS_TEMP) && \
	(MQOM3_PARAM_EXT_FIELD == 8) && (SIGN_X0_BYTES <= MQOM3_PARAM_OPENING_SIZE)
#define SIGN_X0_IN_SIGNATURE 1
#else
#define SIGN_X0_IN_SIGNATURE 0
#endif

int Sign_default(const uint8_t sk[MQOM3_SK_SIZE], const uint8_t *msg, unsigned long long mlen, const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], uint8_t sig[MQOM3_SIG_SIZE]) {
	int ret = -1;
	int e;
	uint8_t msg_hash[MQOM3_PARAM_DIGEST_SIZE];
#ifdef BLC_KEEP_ALL_TREES_IN_MEMORY
	/* blc_key_t can be several MB in this mode - heap-allocate to avoid stack overflow. */
	blc_key_t *key = mqom_malloc(sizeof(*key));
	if (key == NULL) {
		/* Same signature-buffer policy as the err: label below. Not a goto:
		 * that would jump over xof_ctx_active's initialiser, which err: reads. */
		memset(sig, 0, MQOM3_SIG_SIZE);
		return -1;
	}
#else
	blc_key_t key_buf = { 0 };
	blc_key_t *key = &key_buf;
#endif
	field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	/* One (alpha0,alpha1) pair, streamed into the Hash_1 XOF and reused
	 * across the TAU iterations below */
	uint8_t alpha_pair[2 * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU)];
	/* u0/alpha0 and i_star have disjoint lifetimes.  A union preserves the
	 * stack saving without casts between incompatible declared types. */
	union {
		field_ext_elt u0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
		uint16_t i_star[MQOM3_PARAM_TAU];
	} challenge_workspace;
	field_ext_elt (*u0)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)] = challenge_workspace.u0;
	field_ext_elt u1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	/* Aliasing to save stack space */
	field_ext_elt (*alpha0)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)] = u0;
	field_ext_elt (*alpha1)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)] = u1;

	/* Prepare the signature: sig_id[D] | salt | nonce[4] | opening (v3) */
	unsigned int pos = 0;
	uint8_t *sig_id = &sig[pos];
	pos += MQOM3_PARAM_DIGEST_SIZE;
	memcpy(&sig[pos], salt, MQOM3_PARAM_SALT_SIZE);
	pos += MQOM3_PARAM_SALT_SIZE;
	uint8_t *nonce = &sig[pos];
	pos += 4;
	uint8_t *opening = &sig[pos];
	/* com1 and com2 are local (not stored in sig in v3) */
	uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE];
	uint8_t com2[MQOM3_PARAM_DIGEST_SIZE];

#if SIGN_X0_IN_SIGNATURE
	field_ext_elt (*x0)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)] =
		(field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)])opening;
	xof_context xof_ctx_storage = { 0 };
	xof_context *xof_ctx = &xof_ctx_storage;
#else
	union {
		field_ext_elt x0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
		xof_context xof_ctx;
	} x0_xof_workspace = { 0 };
	field_ext_elt (*x0)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)] = x0_xof_workspace.x0;
	xof_context *xof_ctx = &x0_xof_workspace.xof_ctx;
#endif
	int xof_ctx_active = 0;

	/* Parse the secret key */
	const uint8_t *pk = &sk[0];
	field_base_parse(&sk[(2 * MQOM3_PARAM_SEED_SIZE) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], MQOM3_PARAM_MQ_N, x);

	/* Hash message (Hash_3: msgdigest) */
	__BENCHMARK_START__(BS_SIGN_HASH_MSG);
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	xof_ctx_active = 1;
	ret = xof_update(xof_ctx, (const uint8_t*) DOMAIN_SEP_MSGDIGEST, 1);
	ERR(ret, err);
	ret = xof_update(xof_ctx, msg, (size_t)mlen);
	ERR(ret, err);
	ret = xof_squeeze(xof_ctx, msg_hash, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_SIGN_HASH_MSG);

#if !SIGN_X0_IN_SIGNATURE
	/* x0 is the next active union member. */
	xof_clean_ctx(xof_ctx);
	xof_ctx_active = 0;
#endif

	/* Commit Lines */
	__BENCHMARK_START__(BS_BLC_COMMIT);
	ret = BLC_Commit(mseed, salt, x, com1, key, x0, u0, u1);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_COMMIT);

	/* Compute P_alpha */
	__BENCHMARK_START__(BS_PIOP_COMPUTE);
	ret = ComputePAlpha((const uint8_t (*)[MQOM3_PARAM_DIGEST_SIZE])com1, (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)])x0,
				  (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])u0,
				  (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])u1,
				  x, &sk[0], alpha0, alpha1);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_PIOP_COMPUTE);

#if !SIGN_X0_IN_SIGNATURE
	/* Re-activate a clean XOF member after the last use of x0. */
	memset(xof_ctx, 0, sizeof(*xof_ctx));
#else
	mqom_cleanse(opening, SIGN_X0_BYTES);
#endif

	/* Hash P_alpha + Fiat-Shamir (Hash_1: com2, Hash_4: sig_id) */
	__BENCHMARK_START__(BS_SIGN_HASH_FS);
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	xof_ctx_active = 1;
	ret = xof_update(xof_ctx, (const uint8_t*) DOMAIN_SEP_COM2, 1);
	ERR(ret, err);

	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		field_ext_serialize(alpha0[e], MQOM3_PARAM_ETA, alpha_pair);
		field_ext_serialize(alpha1[e], MQOM3_PARAM_ETA, alpha_pair + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU));
		ret = xof_update(xof_ctx, alpha_pair, 2 * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU));
		ERR(ret, err);
	}
	ret = xof_squeeze(xof_ctx, com2, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* presig_id = Hash_2(pk, com1[0..TAU-1], com2) - message-independent;
	 * this is the value carried by a pre-signature. */
	uint8_t presig_id[MQOM3_PARAM_DIGEST_SIZE];
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

	/* sig_id = Hash_4(presig_id, msg_hash) */
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	ret = xof_update(xof_ctx, (const uint8_t*) DOMAIN_SEP_SIGID, 1);
	ERR(ret, err);
	ret = xof_update(xof_ctx, presig_id, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_update(xof_ctx, msg_hash, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(xof_ctx, sig_id, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_SIGN_HASH_FS);

	/* Sample Challenge */
	uint16_t *i_star = challenge_workspace.i_star;
	__BENCHMARK_START__(BS_SAMPLE_CHALLENGE);
	ret = SampleChallenge(sig_id, i_star, nonce);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_SAMPLE_CHALLENGE);

	/* Open Line Evaluation */
	__BENCHMARK_START__(BS_BLC_OPEN);
	ret = BLC_Open(key, i_star, (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])alpha1, opening);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_OPEN);

	ret = 0;
err:
	mqom_cleanse((void*)key, sizeof(*key));
#ifdef BLC_KEEP_ALL_TREES_IN_MEMORY
	mqom_free(key, sizeof(*key));
#endif
	mqom_cleanse((void*)x, sizeof(x));
	mqom_cleanse((void*)msg_hash, sizeof(msg_hash));
	if (xof_ctx_active) {
		xof_clean_ctx(xof_ctx);
	}
#if !SIGN_X0_IN_SIGNATURE
	/* xof_clean_ctx only covers sizeof(xof_context); cleanse the full union
	 * to erase x0 secret evaluations stored in the larger x0 member. */
	mqom_cleanse(&x0_xof_workspace, sizeof(x0_xof_workspace));
#endif
	mqom_cleanse(alpha_pair, sizeof(alpha_pair));
	mqom_cleanse((void*)&challenge_workspace, sizeof(challenge_workspace));
	mqom_cleanse((void*)u1, sizeof(u1));
	/* In case of error, clean the signature buffer */
	if(ret){
		memset(sig, 0, MQOM3_SIG_SIZE);
	}

	return ret;
}

int Verify_default(const uint8_t pk[MQOM3_PK_SIZE], const uint8_t *msg, unsigned long long mlen, const uint8_t sig[MQOM3_SIG_SIZE]) {
	int ret = -1;
	int e;
	uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE];
	uint8_t msg_hash[MQOM3_PARAM_DIGEST_SIZE], com2_[MQOM3_PARAM_DIGEST_SIZE], sig_id_[MQOM3_PARAM_DIGEST_SIZE];
	uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE];
	field_ext_elt y[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
	field_ext_elt u_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	field_ext_elt alpha0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	/* Streamed into Hash_1 below, see the matching comment in Sign(). */
	uint8_t alpha_pair[2 * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU)];
	union {
		field_ext_elt x_eval[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
		xof_context xof_ctx;
	} eval_xof_workspace = { 0 };
	field_ext_elt (*x_eval)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)] =
		eval_xof_workspace.x_eval;
	xof_context *xof_ctx = &eval_xof_workspace.xof_ctx;
	int xof_ctx_active = 0;

	/* Parse the public key */
	memcpy(mseed_eq, &pk[0], 2 * MQOM3_PARAM_SEED_SIZE);
	field_ext_parse(&pk[2 * MQOM3_PARAM_SEED_SIZE], MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU, y);

	/* Parse the signature: sig_id[D] | salt | nonce[4] | opening (v3) */
	unsigned int pos = 0;
	const uint8_t *sig_id = &sig[pos];
	pos += MQOM3_PARAM_DIGEST_SIZE;
	const uint8_t *salt = &sig[pos];
	pos += MQOM3_PARAM_SALT_SIZE;
	const uint8_t *nonce = &sig[pos];
	pos += 4;
	const uint8_t *opening = &sig[pos];

	/* Hash message (Hash_3: msgdigest) */
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	xof_ctx_active = 1;
	ret = xof_update(xof_ctx, (const uint8_t*) DOMAIN_SEP_MSGDIGEST, 1);
	ERR(ret, err);
	ret = xof_update(xof_ctx, msg, (size_t)mlen);
	ERR(ret, err);
	ret = xof_squeeze(xof_ctx, msg_hash, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* x_eval is the next active union member; clean before DeriveChallenge. */
	xof_clean_ctx(xof_ctx);
	xof_ctx_active = 0;

	/* Sample Challenge - verify nonce grinding and derive i* from sig_id */
	uint16_t i_star[MQOM3_PARAM_TAU];
	{
		grinding_material_t _mat = { 0 };
		int _mat_precomputed = 0;
		ret = DeriveChallenge(sig_id, nonce, i_star, &_mat, &_mat_precomputed);
		grinding_material_clean(&_mat);
	}
	ERR(ret, err);

	/* Get Opened Evaluations  - com1 is output (v3: not in sig) */
	ret = BLC_Eval(salt, opening, i_star, com1, x_eval, u_eval, alpha1);
	ERR(ret, err);

	/* Recompute P_alpha */
	ret = RecomputePAlpha((const uint8_t (*)[MQOM3_PARAM_DIGEST_SIZE])com1, (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])alpha1, i_star,
				    (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)])x_eval,
				    (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])u_eval, mseed_eq, y, alpha0);
	ERR(ret, err);

	/* Re-activate a clean XOF member after the last use of x_eval. */
	memset(xof_ctx, 0, sizeof(*xof_ctx));

	/* Hash P_alpha (Hash_1: com2) */
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	xof_ctx_active = 1;
	ret = xof_update(xof_ctx, (const uint8_t*) DOMAIN_SEP_COM2, 1);
	ERR(ret, err);
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		field_ext_serialize(alpha0[e], MQOM3_PARAM_ETA, alpha_pair);
		field_ext_serialize(alpha1[e], MQOM3_PARAM_ETA, alpha_pair + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU));
		ret = xof_update(xof_ctx, alpha_pair, 2 * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU));
		ERR(ret, err);
	}
	ret = xof_squeeze(xof_ctx, com2_, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* Recompute presig_id' = Hash_2(pk, com1[0..TAU-1], com2') */
	uint8_t presig_id_[MQOM3_PARAM_DIGEST_SIZE];
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	ret = xof_update(xof_ctx, (const uint8_t*) DOMAIN_SEP_PRESIGID, 1);
	ERR(ret, err);
	ret = xof_update(xof_ctx, pk, MQOM3_PK_SIZE);
	ERR(ret, err);
	ret = xof_update(xof_ctx, &com1[0][0], MQOM3_PARAM_TAU * MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_update(xof_ctx, com2_, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(xof_ctx, presig_id_, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* Recompute sig_id' = Hash_4(presig_id', msg_hash) and verify */
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	ret = xof_update(xof_ctx, (const uint8_t*) DOMAIN_SEP_SIGID, 1);
	ERR(ret, err);
	ret = xof_update(xof_ctx, presig_id_, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_update(xof_ctx, msg_hash, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(xof_ctx, sig_id_, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	if (memcmp(sig_id, sig_id_, MQOM3_PARAM_DIGEST_SIZE)) {
		ret = -1;
		goto err;
	}

	ret = 0;
err:
	if (xof_ctx_active) {
		/* Resource release, not a wipe: under libOQS this frees the sponge's
		 * heap state. It stays whatever the policy on secrets below. */
		xof_clean_ctx(xof_ctx);
	}
	return ret;
}
