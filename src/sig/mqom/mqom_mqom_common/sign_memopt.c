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
#if MQOM3_PARAM_OT_VARIANT != 1
#include "blc_ct_common.h"
#endif
#include "piop.h"
#include "piop_memopt.h"
#include "benchmark.h"
#include "sign.h"
#include "sample_challenge_sign.h"
#include "sample_challenge_verify.h"
#include "domain_separation.h"

/* Does alpha0/alpha1 share u0/u1's storage in Sign_memopt? Only when the BLC
 * commits all TAU executions in one pass, which is OT-only and only at the
 * default batch size (see the declarations in Sign_memopt for why). This is
 * the single source of truth for that question: the declaration and the
 * cleanse both key off it, so they can never disagree on who owns the bytes. */
#if (MQOM3_PARAM_OT_VARIANT == 1) && (BLC_OT_BATCH_SIZE >= MQOM3_PARAM_TAU)
#define SIGN_MEMOPT_ALPHA_ALIASED 1
#else
#define SIGN_MEMOPT_ALPHA_ALIASED 0
#endif

#if SIGN_MEMOPT_ALPHA_ALIASED
/* Sign_memopt's OT workspace overlays a { x0, u0, u1 } struct and an
 * xof_context in one union, and aliases alpha0/alpha1 onto u0/u1.
 * Hence sanity check on the sizes. Only meaningful when the BLC is
 * unbatched: once batched the struct shrinks to the window and may
 * legitimately be smaller than the xof_context it shares the union with. */
MQOM3_STATIC_ASSERT(sizeof(xof_context) <= (MQOM3_PARAM_TAU * BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_N)));
#endif

#include <stdio.h>
#if !defined(MQOM3_FOR_MUPQ) && !defined(MQOM3_FOR_LIBOQS)
#ifdef SUPERCOP
extern void randombytes(unsigned char* x, unsigned long long xlen);
#else
extern int randombytes(unsigned char* x, unsigned long long xlen);
#endif
#else
#include "randombytes.h"
#endif

#if defined(VERIFY_MEMOPT) && !defined(MEMORY_EFFICIENT_BLC)
#error "Verify memopt relies on BLC memopt, which is not defined."
#endif

#if defined(VERIFY_MEMOPT) && !defined(MEMORY_EFFICIENT_PIOP)
#error "Verify memopt relies on PIOP memopt, which is not defined."
#endif

#if defined(SIGN_MEMOPT) && !defined(MEMORY_EFFICIENT_BLC)
#error "Sign memopt relies on BLC memopt, which is not defined."
#endif

#if defined(SIGN_MEMOPT) && !defined(MEMORY_EFFICIENT_PIOP)
#error "Sign memopt relies on PIOP memopt, which is not defined."
#endif

/* --- Streaming com1 into the presig_id XOF ------------------------
 * Effective batch: the size of the live com1 window, i.e. of every buffer that
 * bridges the commitment to the PIOP. In CT the PIOP batch alone drives it,
 * one execution being committed at a time. In OT it is the BLC batch instead:
 * one pass over the large tree produces BLC_OT_BATCH_SIZE executions at once,
 * so the window cannot be narrower than that - the PIOP then consumes it in
 * sub-batches of PIOP_NB_PARALLEL_REPETITIONS_* (clamped to what is there).
 * Both parameters default to TAU, i.e. no batching. */
#if MQOM3_PARAM_OT_VARIANT == 1
#define SIGN_MEMOPT_BATCH   BLC_OT_BATCH_SIZE
#define VERIFY_MEMOPT_BATCH BLC_OT_BATCH_SIZE
#else
#define SIGN_MEMOPT_BATCH   PIOP_NB_PARALLEL_REPETITIONS_SIGN
#define VERIFY_MEMOPT_BATCH PIOP_NB_PARALLEL_REPETITIONS_VERIFY
#endif

/* Unless overloaded, activate the com1 streaming whenever the batch size is less than \tau */
#ifndef SIGN_MEMOPT_STREAM_COM1
#define SIGN_MEMOPT_STREAM_COM1   (SIGN_MEMOPT_BATCH   < MQOM3_PARAM_TAU)
#endif
#ifndef VERIFY_MEMOPT_STREAM_COM1
#define VERIFY_MEMOPT_STREAM_COM1 (VERIFY_MEMOPT_BATCH < MQOM3_PARAM_TAU)
#endif

int Sign_memopt(const uint8_t sk[MQOM3_SK_SIZE], const uint8_t *msg, unsigned long long mlen, const uint8_t salt[MQOM3_PARAM_SALT_SIZE], const uint8_t mseed[MQOM3_PARAM_SEED_SIZE], uint8_t sig[MQOM3_SIG_SIZE]) {
	int ret = -1;
	uint32_t e;
	/* blc_key_memopt_t (blc.h): always the memopt-backend key struct,
	 * regardless of the ambient MEMORY_EFFICIENT_BLC choice for this build -
	 * Sign_memopt is always compiled, whether or not it's selected. */
	blc_key_memopt_t key = { 0 };
	field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	uint8_t msg_hash[MQOM3_PARAM_DIGEST_SIZE];
	uint8_t com2[MQOM3_PARAM_DIGEST_SIZE];
#if SIGN_MEMOPT_STREAM_COM1
	/* Streamed: only the current batch is kept, absorbed into presigid_ctx as
	 * soon as it is complete. See the SIGN_MEMOPT_STREAM_COM1 comment above. */
	uint8_t com1[SIGN_MEMOPT_BATCH][MQOM3_PARAM_DIGEST_SIZE];
	/* Zero-initialized like every other xof_context in the tree: the libOQS
	 * backend's _XOF_Init reads ctx->magic to choose between init and reset,
	 * so an indeterminate stack slot could send it resetting a garbage
	 * pointer. Costs nothing - it is written by xof_init immediately after. */
	xof_context presigid_ctx = { 0 };
	int presigid_ctx_active = 0;
#define SIGN_COM1_SLOT(e, e_mod)  (com1[(e_mod)])
#define SIGN_COM1_BATCH(e, e_mod) (com1)
#else
	/* com1 stays TAU-sized and is absorbed in one xof_update at the end, like
	 * Sign_default. */
	uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE];
#define SIGN_COM1_SLOT(e, e_mod)  (com1[(e)])
#define SIGN_COM1_BATCH(e, e_mod) (&com1[(e) - (e_mod)])
#endif

	/* x0/u0/u1 (the BLC_Commit outputs bridging to ComputePAlpha) and
	 * xof_ctx have disjoint lifetimes: x0/u0/u1 are only live during the
	 * Commit+ComputePAlpha loop below, itself sandwiched between xof_ctx's
	 * two uses (msg_hash before, com2/presig_id/sig_id after) - so they
	 * share one stack slot, exactly like Sign_default's x0/xof_ctx union
	 * (see SIGN_X0_IN_SIGNATURE in sign.c). This is the real win: driving
	 * BLC_Commit_*_memopt_partial directly keeps them SIGN_MEMOPT_BATCH-sized
	 * instead of TAU-sized, and the union still avoids paying for xof_ctx on
	 * top.
	 *
	 * One exception to "disjoint", and it is the aliased case below
	 * (SIGN_MEMOPT_ALPHA_ALIASED, i.e. OT at the default batch - the shipped
	 * OT configuration): there alpha0/alpha1 ARE u0/u1, and they are read
	 * during xof_ctx's second use (the Hash_1/com2 loop, then BLC_Open_memopt).
	 * The two lifetimes genuinely overlap there. What makes it safe is the
	 * static assertion at the top of this file: it pins sizeof(xof_context)
	 * below the offset of u0 inside the struct, so everything the XOF writes
	 * stays within x0's bytes, which really are dead by then. Move that
	 * assertion, or reorder the struct members, and the aliased mode breaks
	 * silently. */
#if MQOM3_PARAM_OT_VARIANT == 1
	union {
		struct {
			field_ext_elt x0[SIGN_MEMOPT_BATCH][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
			field_ext_elt u0[SIGN_MEMOPT_BATCH][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
			field_ext_elt u1[SIGN_MEMOPT_BATCH][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
		} b;
		xof_context xof_ctx_storage;
	} ws = { 0 };
	field_ext_elt (*x0)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)] = ws.b.x0;
	field_ext_elt (*u0)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)] = ws.b.u0;
	field_ext_elt (*u1)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)] = ws.b.u1;
#if !SIGN_MEMOPT_ALPHA_ALIASED
	/* Batched: u0/u1 only hold the current BLC window, whereas alpha0/alpha1
	 * must survive until the Hash_1/com2 pass over all TAU executions, so
	 * they get their own storage - same situation as CT below. */
	field_ext_elt alpha0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
#else
	/* Unbatched: one single BLC pass covers all TAU executions, so u0/u1 have
	 * exactly alpha0/alpha1's shape and lifetime, and the two can share
	 * storage - exactly like Sign_default's u0/alpha0 and u1/alpha1 aliasing
	 * (sign.c). ComputePAlpha_partial_memopt only ever computes
	 * alpha0[e] = tmp + u0[e] (tmp derived from Gamma/z0, independent of
	 * u0[e]) then alpha1[e] similarly from u1[e] - a safe in-place
	 * a = a + b update. Keeping the alias here is what makes the batched
	 * variant a pure trade rather than a regression at the default. */
	field_ext_elt (*alpha0)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)] = u0;
	field_ext_elt (*alpha1)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)] = u1;
#endif
#else /* CT variant below */
	field_ext_elt alpha0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	field_ext_elt alpha1[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	union {
		struct {
			field_ext_elt x0[PIOP_NB_PARALLEL_REPETITIONS_SIGN][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
			field_ext_elt u0[PIOP_NB_PARALLEL_REPETITIONS_SIGN][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
			field_ext_elt u1[PIOP_NB_PARALLEL_REPETITIONS_SIGN][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
		} b;
		xof_context xof_ctx_storage;
	} ws = { 0 };
	field_ext_elt (*x0_batch)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)] = ws.b.x0;
	field_ext_elt (*u0_batch)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)] = ws.b.u0;
	field_ext_elt (*u1_batch)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)] = ws.b.u1;
#endif
	xof_context *xof_ctx = &ws.xof_ctx_storage;
	int xof_ctx_active = 0;

	/* Prepare the signature: sig_id[D] | salt | nonce[4] | opening (v3) */
	unsigned int pos = 0;
	uint8_t *sig_id = &sig[pos];
	pos += MQOM3_PARAM_DIGEST_SIZE;
	memcpy(&sig[pos], salt, MQOM3_PARAM_SALT_SIZE);
	pos += MQOM3_PARAM_SALT_SIZE;
	uint8_t *nonce = &sig[pos];
	pos += 4;
	uint8_t *opening = &sig[pos];

	/* Parse the secret key */
	const uint8_t *pk = &sk[0];
	field_base_parse(&sk[(2 * MQOM3_PARAM_SEED_SIZE) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], MQOM3_PARAM_MQ_N, x);

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

	/* x0/u0/u1 is the next active union member. */
	xof_clean_ctx(xof_ctx);
	xof_ctx_active = 0;

	/* Mirrors the BLC_Commit_memopt wrapper's own setup, since we drive
	 * BLC_Commit_memopt_partial directly below instead of going through the
	 * wrapper: mseed/salt copy for both families, plus DeriveDelta for CT. */
#if MQOM3_PARAM_OT_VARIANT != 1
	DeriveDelta(x, key.delta);
#endif
	memcpy(key.mseed, mseed, MQOM3_PARAM_SEED_SIZE);
	memcpy(key.salt, salt, MQOM3_PARAM_SALT_SIZE);

#if SIGN_MEMOPT_STREAM_COM1
	/* Open presig_id now and absorb its message-independent prefix, so each
	 * batch of com1 can go in as it is produced. Same shape as the streaming
	 * verifier (verify_stream_ct.c). */
	ret = xof_init(&presigid_ctx);
	ERR(ret, err);
	presigid_ctx_active = 1;
	ret = xof_update(&presigid_ctx, (const uint8_t*) DOMAIN_SEP_PRESIGID, 1);
	ERR(ret, err);
	ret = xof_update(&presigid_ctx, pk, MQOM3_PK_SIZE);
	ERR(ret, err);
#endif

#if MQOM3_PARAM_OT_VARIANT == 1
	/* One pass over the large tree per BLC_OT_BATCH_SIZE executions; the PIOP
	 * then eats that window in sub-batches. SIGN_COM1_BATCH(e, 0) is the base
	 * of the window: com1 itself when streaming, &com1[e] otherwise. */
	for (e = 0; e < MQOM3_PARAM_TAU; e += BLC_OT_BATCH_SIZE) {
		uint32_t blc_batch = (uint32_t)(MQOM3_PARAM_TAU - e);
		uint32_t p;

		if (blc_batch > BLC_OT_BATCH_SIZE) {
			blc_batch = BLC_OT_BATCH_SIZE;
		}

		ret = OT_BLC_Commit_memopt_partial(e, blc_batch, mseed, salt, x, SIGN_COM1_BATCH(e, 0), &key, x0, u0, u1);
		ERR(ret, err);

		for (p = 0; p < blc_batch; p += PIOP_NB_PARALLEL_REPETITIONS_SIGN) {
			uint32_t piop_batch = blc_batch - p;

			if (piop_batch > PIOP_NB_PARALLEL_REPETITIONS_SIGN) {
				piop_batch = PIOP_NB_PARALLEL_REPETITIONS_SIGN;
			}
			ret = ComputePAlpha_partial_memopt(piop_batch, (const uint8_t (*)[MQOM3_PARAM_DIGEST_SIZE])(SIGN_COM1_BATCH(e, 0) + p), (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)])&x0[p], (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])&u0[p], (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])&u1[p], x, &sk[0], &alpha0[e + p], &alpha1[e + p]);
			ERR(ret, err);
		}

#if SIGN_MEMOPT_STREAM_COM1
		/* The window is complete and already consumed by the PIOP: fold it
		 * into presig_id now, in execution order, and reuse the buffer. */
		ret = xof_update(&presigid_ctx, &com1[0][0], (size_t)blc_batch * MQOM3_PARAM_DIGEST_SIZE);
		ERR(ret, err);
#endif
	}
#else /* CT variant below */
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		uint32_t e_mod = e % PIOP_NB_PARALLEL_REPETITIONS_SIGN;

		ret = CT_BLC_Commit_memopt_partial(e, key.mseed, salt, x, key.delta, SIGN_COM1_SLOT(e, e_mod), key.partial_delta_x[e], x0_batch[e_mod], u0_batch[e_mod], u1_batch[e_mod]);
		ERR(ret, err);

		if ((e_mod == PIOP_NB_PARALLEL_REPETITIONS_SIGN - 1) || (e == MQOM3_PARAM_TAU - 1)) {
			uint32_t batch_count = e_mod + 1;
			ret = ComputePAlpha_partial_memopt(batch_count, (const uint8_t (*)[MQOM3_PARAM_DIGEST_SIZE])SIGN_COM1_BATCH(e, e_mod), (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)])x0_batch, (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])u0_batch, (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])u1_batch, x, &sk[0], &alpha0[e - e_mod], &alpha1[e - e_mod]);
			ERR(ret, err);
#if SIGN_MEMOPT_STREAM_COM1
			/* The batch is complete and already consumed by the PIOP: fold it
			 * into presig_id now, in execution order, and reuse the buffer. */
			ret = xof_update(&presigid_ctx, &com1[0][0], (size_t)batch_count * MQOM3_PARAM_DIGEST_SIZE);
			ERR(ret, err);
#endif
		}
	}
#endif

	/* Re-activate a clean xof_ctx member after the last use of x0/u0/u1.
	 * Precisely: the last use of x0. In the aliased case u0/u1 live on as
	 * alpha0/alpha1 past this point - safely, since the assertion at the top
	 * of this file keeps the XOF's footprint inside x0. */
	memset(xof_ctx, 0, sizeof(*xof_ctx));

	/* Hash P_alpha  - interleaved (alpha0[e], alpha1[e]) per execution (spec Hash_1: com2) */
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	xof_ctx_active = 1;
	ret = xof_update(xof_ctx, (const uint8_t*) DOMAIN_SEP_COM2, 1);
	ERR(ret, err);
	uint8_t alpha_pair[2 * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU)];
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
#if SIGN_MEMOPT_STREAM_COM1
	/* pk and every com1[e] are already in; only com2 is left. */
	ret = xof_update(&presigid_ctx, com2, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(&presigid_ctx, presig_id, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	xof_clean_ctx(&presigid_ctx);
	presigid_ctx_active = 0;
#else
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
#endif

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

	/* Sample Challenge */
	uint16_t i_star[MQOM3_PARAM_TAU];
	__BENCHMARK_START__(BS_SAMPLE_CHALLENGE);
	ret = SampleChallenge(sig_id, i_star, nonce);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_SAMPLE_CHALLENGE);

	/* Open Line Evaluation */
	ret = BLC_Open_memopt(&key, i_star, (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])alpha1, opening);
	ERR(ret, err);

	ret = 0;
err:
	mqom_cleanse((void*)&key, sizeof(key));
	mqom_cleanse((void*)x, sizeof(x));
	mqom_cleanse((void*)msg_hash, sizeof(msg_hash));
	if (xof_ctx_active) {
		xof_clean_ctx(xof_ctx);
	}
#if SIGN_MEMOPT_STREAM_COM1
	/* presigid_ctx stays open across the whole commit loop, so an early error
	 * can leave it holding absorbed state; wipe it like any other context. */
	if (presigid_ctx_active) {
		xof_clean_ctx(&presigid_ctx);
	}
#endif
	/* xof_clean_ctx only covers sizeof(xof_context); cleanse the full union
	 * to erase x0/u0/u1 secret evaluations stored in the larger batch member.
	 * In unbatched OT, alpha0/alpha1 alias u0/u1 (see declaration above) so
	 * this already covers them too; a separate cleanse would only re-wipe the
	 * same bytes, and sizeof(alpha0) on a pointer would be wrong anyway. */
	mqom_cleanse(&ws, sizeof(ws));
#if !SIGN_MEMOPT_ALPHA_ALIASED
	/* Own storage: the two sizeof below must be the full TAU-sized arrays, not
	 * a pointer - the assertions pin that down, since silently wiping 8 bytes
	 * instead of the array would leave secret alpha material on the stack. */
	MQOM3_STATIC_ASSERT_BLOCK(sizeof(alpha0) == (MQOM3_PARAM_TAU * FIELD_EXT_PACKING(MQOM3_PARAM_ETA) * sizeof(field_ext_elt)));
	MQOM3_STATIC_ASSERT_BLOCK(sizeof(alpha1) == (MQOM3_PARAM_TAU * FIELD_EXT_PACKING(MQOM3_PARAM_ETA) * sizeof(field_ext_elt)));
	mqom_cleanse((void*)alpha0, sizeof(alpha0));
	mqom_cleanse((void*)alpha1, sizeof(alpha1));
#endif
	mqom_cleanse(alpha_pair, sizeof(alpha_pair));
	/* In case of error, clean the signature buffer */
	if (ret) {
		memset(sig, 0, MQOM3_SIG_SIZE);
	}

	return ret;
}

int Verify_memopt(const uint8_t pk[MQOM3_PK_SIZE], const uint8_t *msg, unsigned long long mlen, const uint8_t sig[MQOM3_SIG_SIZE]) {
	int ret = -1;
	int e;
	uint8_t mseed_eq[2 * MQOM3_PARAM_SEED_SIZE];
	uint8_t msg_hash[MQOM3_PARAM_DIGEST_SIZE], com2_[MQOM3_PARAM_DIGEST_SIZE], sig_id_[MQOM3_PARAM_DIGEST_SIZE];
	field_ext_elt y[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
	field_ext_elt alpha0[MQOM3_PARAM_TAU][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
#if VERIFY_MEMOPT_STREAM_COM1
	/* Streamed: only the current batch is kept. See SIGN_MEMOPT_STREAM_COM1 above. */
	uint8_t com1[VERIFY_MEMOPT_BATCH][MQOM3_PARAM_DIGEST_SIZE];
	/* Zero-initialized - see the matching comment in Sign_memopt. */
	xof_context presigid_ctx = { 0 };
	int presigid_ctx_active = 0;
#define VFY_COM1_SLOT(e, e_mod)  (com1[(e_mod)])
#define VFY_COM1_BATCH(e, e_mod) (com1)
#else
	/* com1 stays TAU-sized and is absorbed in one xof_update below for
	 * presig_id'. */
	uint8_t com1[MQOM3_PARAM_TAU][MQOM3_PARAM_DIGEST_SIZE];
#define VFY_COM1_SLOT(e, e_mod)  (com1[(e)])
#define VFY_COM1_BATCH(e, e_mod) (&com1[(e) - (e_mod)])
#endif

	/* x_eval/u_eval/alpha1 and xof_ctx have disjoint lifetimes - see the
	 * matching comment in Sign_memopt. alpha1 is batch-local in both families:
	 * it lives in the opening (CT: per-execution chunk, OT: alpha1 section),
	 * so the later Hash_1/com2 pass simply re-parses it from there rather
	 * than keeping a TAU-sized copy alive across the whole loop. */
	union {
		struct {
			field_ext_elt alpha1[VERIFY_MEMOPT_BATCH][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
			field_ext_elt x_eval[VERIFY_MEMOPT_BATCH][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
			field_ext_elt u_eval[VERIFY_MEMOPT_BATCH][FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
		} b;
		xof_context xof_ctx_storage;
	} ws = { 0 };
	field_ext_elt (*alpha1)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)] = ws.b.alpha1;
	field_ext_elt (*x_eval)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)] = ws.b.x_eval;
	field_ext_elt (*u_eval)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)] = ws.b.u_eval;
	xof_context *xof_ctx = &ws.xof_ctx_storage;
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

	/* x_eval/u_eval(/alpha1) is the next active union member. */
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

#if MQOM3_PARAM_OT_VARIANT == 1
	/* Opening layout: path | ls_com_hidden | delta_x | alpha1_section. Parsed
	 * here rather than inside the BLC because we drive OT_BLC_Eval_memopt_partial
	 * directly, and because alpha1_section is read twice (PIOP below, then the
	 * Hash_1/com2 pass). Mirrors OT_BLC_Eval_memopt's own parsing. */
	const uint8_t *ot_path           = &opening[0];
	const uint8_t *ot_ls_com_hidden  = ot_path + ((size_t)MQOM3_PARAM_LARGE_GGM_T_OPEN * MQOM3_PARAM_SEED_SIZE);
	const uint8_t *ot_delta_x_in     = ot_ls_com_hidden + ((size_t)MQOM3_PARAM_TAU * MQOM3_PARAM_DIGEST_SIZE);
	const uint8_t *ot_alpha1_section = ot_delta_x_in + ((size_t)MQOM3_PARAM_TAU * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N));
	uint32_t hidden_leaf_idxs[MQOM3_PARAM_TAU];
	uint32_t e_start;
#endif

	#if VERIFY_MEMOPT_STREAM_COM1
	/* Open presig_id' and absorb its message-independent prefix before the
	 * loop, so each batch of com1 can be folded in as it is recomputed. */
	ret = xof_init(&presigid_ctx);
	ERR(ret, err);
	presigid_ctx_active = 1;
	ret = xof_update(&presigid_ctx, (const uint8_t*) DOMAIN_SEP_PRESIGID, 1);
	ERR(ret, err);
	ret = xof_update(&presigid_ctx, pk, MQOM3_PK_SIZE);
	ERR(ret, err);
#endif

#if MQOM3_PARAM_OT_VARIANT == 1
	/* hidden_leaf_idxs[e] = TAU * i_star[e] + e - the large tree is indexed
	 * column-major, so execution e's leaves are strided by TAU. */
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		hidden_leaf_idxs[e] = (uint32_t)((MQOM3_PARAM_TAU * (uint32_t)i_star[e]) + (uint32_t)e);
	}

	/* One pass over the large tree per BLC_OT_BATCH_SIZE executions, then the
	 * PIOP eats that window in sub-batches - mirror image of Sign_memopt.
	 * e_start is unsigned, unlike the CT path's int e: every index derived
	 * from it feeds unsigned parameters and array subscripts. */
	for (e_start = 0; e_start < MQOM3_PARAM_TAU; e_start += BLC_OT_BATCH_SIZE) {
		uint32_t blc_batch = (uint32_t)MQOM3_PARAM_TAU - e_start;
		uint32_t p;

		if (blc_batch > BLC_OT_BATCH_SIZE) {
			blc_batch = BLC_OT_BATCH_SIZE;
		}

		/* Get Opened Evaluations - com1[e] = Hash_7(ls_com[e], partial_delta_x[e]) (v3) */
		ret = OT_BLC_Eval_memopt_partial(e_start, blc_batch, salt, ot_path, i_star, hidden_leaf_idxs,
		                                 ot_ls_com_hidden, ot_delta_x_in, VFY_COM1_BATCH(e_start, 0), x_eval, u_eval);
		ERR(ret, err);

		/* Recompute P_alpha */
		for (p = 0; p < blc_batch; p += PIOP_NB_PARALLEL_REPETITIONS_VERIFY) {
			uint32_t e_, piop_batch = blc_batch - p;

			if (piop_batch > PIOP_NB_PARALLEL_REPETITIONS_VERIFY) {
				piop_batch = PIOP_NB_PARALLEL_REPETITIONS_VERIFY;
			}
			/* Parse alpha1 from opening (contiguous alpha1 section) */
			for (e_ = 0; e_ < piop_batch; e_++) {
				field_ext_parse(&ot_alpha1_section[(size_t)(e_start + p + e_) * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU)], MQOM3_PARAM_ETA, alpha1[p + e_]);
			}
			ret = RecomputePAlpha_partial_memopt(piop_batch, (const uint8_t (*)[MQOM3_PARAM_DIGEST_SIZE])(VFY_COM1_BATCH(e_start, 0) + p),
			        (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])&alpha1[p],
			        &i_star[e_start + p],
			        (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)])&x_eval[p],
			        (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])&u_eval[p],
			        mseed_eq, y, &alpha0[e_start + p]);
			ERR(ret, err);
		}

#if VERIFY_MEMOPT_STREAM_COM1
		ret = xof_update(&presigid_ctx, &com1[0][0], (size_t)blc_batch * MQOM3_PARAM_DIGEST_SIZE);
		ERR(ret, err);
#endif
	}
#else
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		const uint8_t *chunk_e       = &opening[e * CT_CHUNK_SIZE];
		const uint8_t *path_e        = chunk_e;
		const uint8_t *out_ls_com_e  = chunk_e + CT_CHUNK_LSCOM_OFF;
		const uint8_t *pdelta_e      = chunk_e + CT_CHUNK_PDELTA_OFF;
		uint32_t e_mod = e % PIOP_NB_PARALLEL_REPETITIONS_VERIFY;

		/* Get Opened Evaluations  - com1[e] = Hash_7(ls_com[e], partial_delta_x[e]) (v3) */
		ret = CT_BLC_Eval_memopt_partial(e, salt, path_e, out_ls_com_e, pdelta_e, i_star[e], VFY_COM1_SLOT(e, e_mod), x_eval[e_mod], u_eval[e_mod]);
		ERR(ret, err);

		/* Recompute P_alpha */
		if((e_mod == PIOP_NB_PARALLEL_REPETITIONS_VERIFY - 1) || (e == MQOM3_PARAM_TAU - 1)) {
			uint32_t e_, batch_count = e_mod + 1;
			/* Parse alpha1 from opening (interleaved chunk format) */
			for (e_ = 0; e_ <= e_mod; e_++) {
				field_ext_parse(&opening[(e - e_mod + e_) * CT_CHUNK_SIZE + CT_CHUNK_ALPHA1_OFF], MQOM3_PARAM_ETA, alpha1[e_]);
			}
			ret = RecomputePAlpha_partial_memopt(batch_count, (const uint8_t (*)[MQOM3_PARAM_DIGEST_SIZE])VFY_COM1_BATCH(e, e_mod), (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])&alpha1[0],
							     &i_star[e - e_mod], (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)])x_eval,
							     (const field_ext_elt (*)[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)])u_eval, mseed_eq, y, &alpha0[e - e_mod]);
			ERR(ret, err);
#if VERIFY_MEMOPT_STREAM_COM1
			ret = xof_update(&presigid_ctx, &com1[0][0], (size_t)batch_count * MQOM3_PARAM_DIGEST_SIZE);
			ERR(ret, err);
#endif
		}
	}
#endif

	/* Re-activate a clean xof_ctx member after the last use of x_eval/u_eval(/alpha1). */
	memset(xof_ctx, 0, sizeof(*xof_ctx));

	/* Hash P_alpha  - interleaved (alpha0[e], alpha1[e]) per execution (spec Hash_1: com2) */
	ret = xof_init(xof_ctx);
	ERR(ret, err);
	xof_ctx_active = 1;
	ret = xof_update(xof_ctx, (const uint8_t*) DOMAIN_SEP_COM2, 1);
	ERR(ret, err);
	uint8_t alpha[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU)];
	for (e = 0; e < MQOM3_PARAM_TAU; e++) {
		field_ext_serialize(alpha0[e], MQOM3_PARAM_ETA, alpha);
		ret = xof_update(xof_ctx, alpha, BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU));
		ERR(ret, err);
		{
			/* Re-parsed from the opening rather than kept alive since the PIOP
			 * loop: alpha1 is signature material, so this costs nothing but a
			 * parse and keeps the batch-local buffer batch-sized. Only the
			 * layout differs - CT interleaves it per-execution chunk, OT has
			 * one contiguous section. */
			field_ext_elt alpha1_e[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
#if MQOM3_PARAM_OT_VARIANT == 1
			field_ext_parse(&ot_alpha1_section[(size_t)e * BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU)], MQOM3_PARAM_ETA, alpha1_e);
#else
			field_ext_parse(&opening[e * CT_CHUNK_SIZE + CT_CHUNK_ALPHA1_OFF], MQOM3_PARAM_ETA, alpha1_e);
#endif
			field_ext_serialize(alpha1_e, MQOM3_PARAM_ETA, alpha);
		}
		ret = xof_update(xof_ctx, alpha, BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_ETA * MQOM3_PARAM_MU));
		ERR(ret, err);
	}
	ret = xof_squeeze(xof_ctx, com2_, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);

	/* Recompute presig_id' = Hash_2(pk, com1[0..TAU-1], com2') */
	uint8_t presig_id_[MQOM3_PARAM_DIGEST_SIZE];
#if VERIFY_MEMOPT_STREAM_COM1
	/* pk and every com1[e] are already in; only com2' is left. */
	ret = xof_update(&presigid_ctx, com2_, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(&presigid_ctx, presig_id_, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	xof_clean_ctx(&presigid_ctx);
	presigid_ctx_active = 0;
#else
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
#endif

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

	if (memcmp(sig_id, sig_id_, MQOM3_PARAM_DIGEST_SIZE) != 0) {
		ret = -1;
		goto err;
	}

	ret = 0;
err:
#if VERIFY_MEMOPT_STREAM_COM1
	if (presigid_ctx_active) {
		xof_clean_ctx(&presigid_ctx);
	}
#endif
	if (xof_ctx_active) {
		xof_clean_ctx(xof_ctx);
	}
	return ret;
}
