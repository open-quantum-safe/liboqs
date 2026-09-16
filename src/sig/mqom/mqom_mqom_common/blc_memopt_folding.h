#ifndef __BLC_MEMOPT_FOLDING_H__
#define __BLC_MEMOPT_FOLDING_H__

#include "fields.h"
#include "enc.h"
#include "blc_memopt.h"
#include "blc_common.h"

#ifdef BLC_SEEDEXPAND_CACHE
/* Both CT and OT need one extra slot: CT for block[0] (XOR mask), OT for block[0] (first output block) */
#define BLC_NB_SEEDEXPAND_ENC_CTX ((PRG_BLC_SIZE + MQOM3_PARAM_SEED_SIZE - 1) / MQOM3_PARAM_SEED_SIZE + 1)
#endif

typedef struct {
	uint8_t acc[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA)];
#ifndef BLC_NO_FAST_FOLDING
	uint8_t data[MQOM3_PARAM_NB_EVALS_LOG][BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA)];
#else
	/* Slow folding: no NB_EVALS_LOG scratch buckets - x0/u0 point at the
	 * caller's output buffers and are accumulated directly, leaf by leaf,
	 * in SeedExpandThenAccumulate_sign/_verify (see there). */
#if BLC_NB_LEAF_SEEDS_IN_PARALLEL > 1
	/* N > 1: with several leaves batched per SeedExpand call, acc keeps
	 * moving forward as each leaf in the batch is processed in turn, so by
	 * the time the batch finishes only the LAST leaf's snapshot of acc is
	 * still available - the earlier ones are gone. leaf_snapshot preserves
	 * each batch slot's own acc snapshot (taken right after that leaf's
	 * contribution was folded in, exactly like fast folding's
	 * "data[pos[i_]] ^= acc") so every leaf in the batch can still be
	 * scaled and folded into x0/u0 individually once SeedExpand completes.
	 * At N == 1 (the default) this field does not exist at all - acc
	 * itself already holds exactly the one needed snapshot, exactly as
	 * before this was introduced. */
	uint8_t leaf_snapshot[BLC_NB_LEAF_SEEDS_IN_PARALLEL][BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA)];
#endif
	field_ext_elt *x0;
	field_ext_elt *u0;
#endif
	uint32_t e;
	const uint8_t* salt;
#ifdef BLC_SEEDEXPAND_CACHE
	enc_ctx_ecb enc_ctx[BLC_NB_SEEDEXPAND_ENC_CTX];
#endif
} folding_sign_t;

typedef struct {
	uint8_t acc[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA)];
#ifndef BLC_NO_FAST_FOLDING
	uint8_t data[MQOM3_PARAM_NB_EVALS_LOG][BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA)];
#else
	/* Slow folding: see folding_sign_t above. */
#if BLC_NB_LEAF_SEEDS_IN_PARALLEL > 1
	uint8_t leaf_snapshot[BLC_NB_LEAF_SEEDS_IN_PARALLEL][BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA)];
#endif
	field_ext_elt *x0;
	field_ext_elt *u0;
#endif
	uint32_t e;
	const uint8_t* salt;
#ifdef BLC_SEEDEXPAND_CACHE
	enc_ctx_pub_ecb enc_ctx[BLC_NB_SEEDEXPAND_ENC_CTX];
#endif
} folding_verify_t;

#if PRG_BLC_SIZE % MQOM3_PARAM_SEED_SIZE != 0
static inline void xor_blocks_partial(const uint8_t seed_in[PRG_BLC_SIZE % MQOM3_PARAM_SEED_SIZE], const uint8_t delta[PRG_BLC_SIZE % MQOM3_PARAM_SEED_SIZE], uint8_t seed_out[PRG_BLC_SIZE % MQOM3_PARAM_SEED_SIZE]) {
	unsigned int i;

	for (i = 0; i < PRG_BLC_SIZE % MQOM3_PARAM_SEED_SIZE; i++) {
		seed_out[i] = seed_in[i] ^ delta[i];
	}

	return;
}
#endif

/* Both CT and OT: PRG output block j maps to enc block j+1.
 * CT: block[0] consumed as XOR mask before the loop; main loop uses blocks 1..n.
 * OT: block[0] accumulated at acc[0] before the loop; main loop uses blocks 1..n. */
static inline uint32_t seedexpand_blk_idx(uint32_t j) {
	return j + 1;
}

static inline int InitializeFolding_sign(folding_sign_t* folding, const uint8_t salt[MQOM3_PARAM_SALT_SIZE], uint32_t e, field_ext_elt x0[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u0[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;

	memset(folding->acc, 0, sizeof(folding->acc));
#ifndef BLC_NO_FAST_FOLDING
	memset(folding->data, 0, sizeof(folding->data));
	(void) x0;
	(void) u0;
#else
	memset(x0, 0, BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_N));
	memset(u0, 0, BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA));
	folding->x0 = x0;
	folding->u0 = u0;
#endif
	folding->e = e;
	folding->salt = salt;

#ifdef BLC_SEEDEXPAND_CACHE
	uint8_t tweaked_salt[MQOM3_PARAM_SALT_SIZE];
	for (uint32_t j = 0; j < BLC_NB_SEEDEXPAND_ENC_CTX; j++) {
		TweakSalt(salt, tweaked_salt, SALT_SEL_SEED_EXPAND, IndexIdentifier(e, j));
		ret = enc_key_sched_ecb(&folding->enc_ctx[j], tweaked_salt);
		ERR(ret, err);
	}
#endif

	ret = 0;
#ifdef BLC_SEEDEXPAND_CACHE
err:
#endif
	return ret;
}

static inline int InitializeFolding_verify(folding_verify_t* folding, const uint8_t salt[MQOM3_PARAM_SALT_SIZE], uint32_t e, field_ext_elt x0[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u0[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	int ret = -1;

	memset(folding->acc, 0, sizeof(folding->acc));
#ifndef BLC_NO_FAST_FOLDING
	memset(folding->data, 0, sizeof(folding->data));
	(void) x0;
	(void) u0;
#else
	memset(x0, 0, BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_N));
	memset(u0, 0, BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA));
	folding->x0 = x0;
	folding->u0 = u0;
#endif
	folding->e = e;
	folding->salt = salt;

#ifdef BLC_SEEDEXPAND_CACHE
	uint8_t tweaked_salt[MQOM3_PARAM_SALT_SIZE];
	for (uint32_t j = 0; j < BLC_NB_SEEDEXPAND_ENC_CTX; j++) {
		TweakSalt(salt, tweaked_salt, SALT_SEL_SEED_EXPAND, IndexIdentifier(e, j));
		ret = enc_key_sched_pub_ecb(&folding->enc_ctx[j], tweaked_salt);
		ERR(ret, err);
	}
#endif

	ret = 0;
#ifdef BLC_SEEDEXPAND_CACHE
err:
#endif
	return ret;
}

static inline void folding_sign_clean_ctx(folding_sign_t* ctx) {
	(void) ctx;
#ifdef BLC_SEEDEXPAND_CACHE
	for (uint32_t j = 0; j < BLC_NB_SEEDEXPAND_ENC_CTX; j++) {
		enc_clean_ctx_ecb(&ctx->enc_ctx[j]);
	}
#endif
	mqom_cleanse(ctx->acc, sizeof(ctx->acc));
#ifndef BLC_NO_FAST_FOLDING
	mqom_cleanse(ctx->data, sizeof(ctx->data));
#else
#if BLC_NB_LEAF_SEEDS_IN_PARALLEL > 1
	mqom_cleanse(ctx->leaf_snapshot, sizeof(ctx->leaf_snapshot));
#endif
#endif
}

/* Releases the cipher contexts and stops there, unlike folding_sign_clean_ctx
 * which also wipes acc / data / leaf_snapshot. That asymmetry is deliberate,
 * not an omission: on the Verify side those accumulators are built from the
 * opening, so they are public. Only the context release is mandatory, because
 * under other backends (e.g. libOQS) it frees heap state. */
static inline void folding_verify_clean_ctx(folding_verify_t* ctx) {
	(void) ctx;
#ifdef BLC_SEEDEXPAND_CACHE
	for (uint32_t j = 0; j < BLC_NB_SEEDEXPAND_ENC_CTX; j++) {
		enc_clean_ctx_pub_ecb(&ctx->enc_ctx[j]);
	}
#endif
}

static inline int SeedExpandThenAccumulate_sign(folding_sign_t* folding, uint32_t i, const uint8_t lseed[][MQOM3_PARAM_SEED_SIZE]) {
	int ret = -1;
	uint32_t j, i_;
	uint8_t xor_mask[BLC_NB_LEAF_SEEDS_IN_PARALLEL][MQOM3_PARAM_SEED_SIZE];
	uint8_t out_data[BLC_NB_LEAF_SEEDS_IN_PARALLEL][MQOM3_PARAM_SEED_SIZE];
	uint8_t pos[BLC_NB_LEAF_SEEDS_IN_PARALLEL];

#ifndef BLC_SEEDEXPAND_CACHE
	uint8_t tweaked_salt[MQOM3_PARAM_SALT_SIZE];
	enc_ctx_ecb DECL_VAR(enc_ctx_local);
#endif

	for (i_ = 0; i_ < BLC_NB_LEAF_SEEDS_IN_PARALLEL; i_++) {
		pos[i_] = get_gray_code_bit_position(i+i_);
	}

	/* Compute XOR mask and block[0] for first SeedExpand output slot.
	 * CT: xor_mask = block[0] = Enc(ctx[0], seed); accumulate lseed at acc[0].
	 * OT: xor_mask = LinOrtho(seed); block[0] = Enc(ctx[0], seed) ^ LinOrtho(seed); accumulate block[0] at acc[0]. */
	__BENCHMARK_START__(BS_BLC_PRG);
#if MQOM3_PARAM_OT_VARIANT != 1
	/* CT: xor_mask = Enc(ctx[0], seed) */
#ifndef BLC_SEEDEXPAND_CACHE
	TweakSalt(folding->salt, tweaked_salt, SALT_SEL_SEED_EXPAND, IndexIdentifier(folding->e, 0));
	ret = enc_key_sched_ecb(&enc_ctx_local, tweaked_salt);
	ERR(ret, err);
	ret = enc_encrypt_ecb(&enc_ctx_local, BLC_NB_LEAF_SEEDS_IN_PARALLEL, (uint8_t*) lseed, (uint8_t*) xor_mask);
#else
	ret = enc_encrypt_ecb(&folding->enc_ctx[0], BLC_NB_LEAF_SEEDS_IN_PARALLEL, (uint8_t*) lseed, (uint8_t*) xor_mask);
#endif
	ERR(ret, err);
#else
	/* OT: xor_mask = LinOrtho(seed); also compute Enc(ctx[0], seed) for block[0] */
	uint8_t block0_enc[BLC_NB_LEAF_SEEDS_IN_PARALLEL][MQOM3_PARAM_SEED_SIZE];
	for (i_ = 0; i_ < BLC_NB_LEAF_SEEDS_IN_PARALLEL; i_++) {
		LinOrtho(lseed[i_], xor_mask[i_]);
	}
#ifndef BLC_SEEDEXPAND_CACHE
	TweakSalt(folding->salt, tweaked_salt, SALT_SEL_SEED_EXPAND, IndexIdentifier(folding->e, 0));
	ret = enc_key_sched_ecb(&enc_ctx_local, tweaked_salt);
	ERR(ret, err);
	ret = enc_encrypt_ecb(&enc_ctx_local, BLC_NB_LEAF_SEEDS_IN_PARALLEL, (uint8_t*) lseed, (uint8_t*) block0_enc);
#else
	ret = enc_encrypt_ecb(&folding->enc_ctx[0], BLC_NB_LEAF_SEEDS_IN_PARALLEL, (uint8_t*) lseed, (uint8_t*) block0_enc);
#endif
	ERR(ret, err);
#endif
	__BENCHMARK_STOP__(BS_BLC_PRG);

#if MQOM3_PARAM_OT_VARIANT == 1
	/* Hoisted out of the loop below so the wipe at err: covers it. */
	uint8_t block0[MQOM3_PARAM_SEED_SIZE];
#endif
	__BENCHMARK_START__(BS_BLC_ARITH);
	for (i_ = 0; i_ < BLC_NB_LEAF_SEEDS_IN_PARALLEL; i_++) {
#if MQOM3_PARAM_OT_VARIANT == 1
		/* OT: accumulate block[0][i_] = Enc(ctx[0], lseed[i_]) ^ LinOrtho(lseed[i_]) */
		xor_blocks(block0_enc[i_], xor_mask[i_], block0);
		xor_blocks(&folding->acc[0], block0, &folding->acc[0]);
#else
		/* CT: accumulate raw lseed (= first SEED_SIZE bytes of SeedExpand.Correlated output) */
		xor_blocks(&folding->acc[0], lseed[i_], &folding->acc[0]);
#endif
#if defined(BLC_NO_FAST_FOLDING) && (BLC_NB_LEAF_SEEDS_IN_PARALLEL > 1)
		/* Snapshot acc's CUMULATIVE value right after this leaf's own
		 * contribution was just XORed in above - mirrors fast-folding's
		 * "data[pos[i_]] ^= acc" exactly (same value, same timing), just a
		 * plain copy instead of XOR-accumulate since leaf_snapshot[i_] is a
		 * fresh per-batch-slot scratch spot, not a persistent per-Gray-code-
		 * position bucket. NOT the leaf's own isolated SeedExpand output
		 * (lseed[i_]/block0) - that would be a different, incorrect value. */
		memcpy(&folding->leaf_snapshot[i_][0], &folding->acc[0], MQOM3_PARAM_SEED_SIZE);
#endif
#ifndef BLC_NO_FAST_FOLDING
		xor_blocks(&folding->data[pos[i_]][0], &folding->acc[0], &folding->data[pos[i_]][0]);
#endif
	}
	__BENCHMARK_STOP__(BS_BLC_ARITH);

	for (j = 0; j < (PRG_BLC_SIZE / MQOM3_PARAM_SEED_SIZE); j++) {
		/* Key schedule */
		__BENCHMARK_START__(BS_BLC_PRG);
#ifndef BLC_SEEDEXPAND_CACHE
		TweakSalt(folding->salt, tweaked_salt, SALT_SEL_SEED_EXPAND, IndexIdentifier(folding->e, seedexpand_blk_idx(j)));
		ret = enc_key_sched_ecb(&enc_ctx_local, tweaked_salt);
		ERR(ret, err);
		ret = enc_encrypt_ecb(&enc_ctx_local, BLC_NB_LEAF_SEEDS_IN_PARALLEL, (uint8_t*) lseed, (uint8_t*) out_data);
#else
		ret = enc_encrypt_ecb(&folding->enc_ctx[seedexpand_blk_idx(j)], BLC_NB_LEAF_SEEDS_IN_PARALLEL, (uint8_t*) lseed, (uint8_t*) out_data);
#endif
		ERR(ret, err);
		__BENCHMARK_STOP__(BS_BLC_PRG);

		for (i_ = 0; i_ < BLC_NB_LEAF_SEEDS_IN_PARALLEL; i_++) {
			/* Xor with XOR mask */
			xor_blocks(out_data[i_], xor_mask[i_], out_data[i_]);

			__BENCHMARK_START__(BS_BLC_ARITH);
			xor_blocks(&folding->acc[(j+1)*MQOM3_PARAM_SEED_SIZE], out_data[i_], &folding->acc[(j+1)*MQOM3_PARAM_SEED_SIZE]);
#if defined(BLC_NO_FAST_FOLDING) && (BLC_NB_LEAF_SEEDS_IN_PARALLEL > 1)
			/* Snapshot acc's cumulative value, not out_data[i_] - see the
			 * block0 comment above. */
			memcpy(&folding->leaf_snapshot[i_][(j+1)*MQOM3_PARAM_SEED_SIZE], &folding->acc[(j+1)*MQOM3_PARAM_SEED_SIZE], MQOM3_PARAM_SEED_SIZE);
#endif
#ifndef BLC_NO_FAST_FOLDING
			xor_blocks(&folding->data[pos[i_]][(j+1)*MQOM3_PARAM_SEED_SIZE], &folding->acc[(j+1)*MQOM3_PARAM_SEED_SIZE], &folding->data[pos[i_]][(j+1)*MQOM3_PARAM_SEED_SIZE]);
#endif
			__BENCHMARK_STOP__(BS_BLC_ARITH);
		}
	}
	/* Deal with the possible leftover incomplete block */
#if PRG_BLC_SIZE % MQOM3_PARAM_SEED_SIZE != 0
	/* Key schedule */
	__BENCHMARK_START__(BS_BLC_PRG);
#ifndef BLC_SEEDEXPAND_CACHE
	TweakSalt(folding->salt, tweaked_salt, SALT_SEL_SEED_EXPAND, IndexIdentifier(folding->e, seedexpand_blk_idx(j)));
	ret = enc_key_sched_ecb(&enc_ctx_local, tweaked_salt);
	ERR(ret, err);
	ret = enc_encrypt_ecb(&enc_ctx_local, BLC_NB_LEAF_SEEDS_IN_PARALLEL, (uint8_t*) lseed, (uint8_t*) out_data);
#else
	ret = enc_encrypt_ecb(&folding->enc_ctx[seedexpand_blk_idx(j)], BLC_NB_LEAF_SEEDS_IN_PARALLEL, (uint8_t*) lseed, (uint8_t*) out_data);
#endif
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_PRG);

	for (i_ = 0; i_ < BLC_NB_LEAF_SEEDS_IN_PARALLEL; i_++) {
		/* Xor with XOR mask */
		xor_blocks(out_data[i_], xor_mask[i_], out_data[i_]);

		__BENCHMARK_START__(BS_BLC_ARITH);
		xor_blocks_partial(&folding->acc[(j+1)*MQOM3_PARAM_SEED_SIZE], out_data[i_], &folding->acc[(j+1)*MQOM3_PARAM_SEED_SIZE]);
#if defined(BLC_NO_FAST_FOLDING) && (BLC_NB_LEAF_SEEDS_IN_PARALLEL > 1)
		memcpy(&folding->leaf_snapshot[i_][(j+1)*MQOM3_PARAM_SEED_SIZE], &folding->acc[(j+1)*MQOM3_PARAM_SEED_SIZE], PRG_BLC_SIZE % MQOM3_PARAM_SEED_SIZE);
#endif
#ifndef BLC_NO_FAST_FOLDING
		xor_blocks_partial(&folding->data[pos[i_]][(j+1)*MQOM3_PARAM_SEED_SIZE], &folding->acc[(j+1)*MQOM3_PARAM_SEED_SIZE], &folding->data[pos[i_]][(j+1)*MQOM3_PARAM_SEED_SIZE]);
#endif
		__BENCHMARK_STOP__(BS_BLC_ARITH);
	}
#endif

#ifdef BLC_NO_FAST_FOLDING
	/* Slow folding: fold this leaf's now-complete acc snapshot directly into
	 * x0/u0, scaled by (1 << pos) - mirrors the data[pos] += acc updates
	 * removed above, but performs the (expensive) scale-and-add immediately
	 * instead of deferring it via data[]. At N == 1, folding->acc itself
	 * already holds exactly the right (single, most recent) snapshot. At
	 * N > 1, acc keeps moving forward as later leaves in the batch are
	 * processed, so leaf_snapshot[i_] (this leaf's acc value AT THE TIME it
	 * was its turn, captured by the memcpy calls above - same value fast
	 * folding's "data[pos[i_]] ^= acc" would have captured) is used instead. */
	__BENCHMARK_START__(BS_BLC_ARITH);
	field_base_elt bar_x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt bar_u[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	field_ext_elt tmp_n[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt *tmp_eta = bar_u;
	for (i_ = 0; i_ < BLC_NB_LEAF_SEEDS_IN_PARALLEL; i_++) {
#if BLC_NB_LEAF_SEEDS_IN_PARALLEL > 1
		field_base_parse(folding->leaf_snapshot[i_], MQOM3_PARAM_MQ_N, bar_x);
#else
		field_base_parse(folding->acc, MQOM3_PARAM_MQ_N, bar_x);
#endif
		field_ext_base_constant_vect_mult((1 << pos[i_]), bar_x, tmp_n, MQOM3_PARAM_MQ_N);
		field_ext_vect_add(folding->x0, tmp_n, folding->x0, MQOM3_PARAM_MQ_N);

#if BLC_NB_LEAF_SEEDS_IN_PARALLEL > 1
		field_ext_parse(folding->leaf_snapshot[i_] + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N), MQOM3_PARAM_ETA, bar_u);
#else
		field_ext_parse(folding->acc + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N), MQOM3_PARAM_ETA, bar_u);
#endif
		field_ext_constant_vect_mult((1 << pos[i_]), bar_u, tmp_eta, MQOM3_PARAM_ETA);
		field_ext_vect_add(folding->u0, tmp_eta, folding->u0, MQOM3_PARAM_ETA);
	}
	__BENCHMARK_STOP__(BS_BLC_ARITH);
#endif

	ret = 0;
err:
#ifndef BLC_SEEDEXPAND_CACHE
	enc_clean_ctx_ecb(&enc_ctx_local);
#endif
	mqom_cleanse(xor_mask, sizeof(xor_mask));
	mqom_cleanse(out_data, sizeof(out_data));
#if MQOM3_PARAM_OT_VARIANT == 1
	mqom_cleanse(block0_enc, sizeof(block0_enc));
	mqom_cleanse(block0, sizeof(block0));
#endif
#ifdef BLC_NO_FAST_FOLDING
	/* Parsed copies of the accumulator, secret by the same argument. */
	mqom_cleanse(bar_x, sizeof(bar_x));
	mqom_cleanse(bar_u, sizeof(bar_u));
	mqom_cleanse(tmp_n, sizeof(tmp_n));
#endif
	return ret;
}

static inline int SeedExpandThenAccumulate_verify(folding_verify_t* folding, uint32_t i, const uint8_t lseed[][MQOM3_PARAM_SEED_SIZE], uint32_t hidden_index) {
	int ret = -1;
	uint32_t j, i_;
	uint8_t xor_mask[BLC_NB_LEAF_SEEDS_IN_PARALLEL][MQOM3_PARAM_SEED_SIZE];
	uint8_t out_data[BLC_NB_LEAF_SEEDS_IN_PARALLEL][MQOM3_PARAM_SEED_SIZE];
	uint8_t pos[BLC_NB_LEAF_SEEDS_IN_PARALLEL];

#ifndef BLC_SEEDEXPAND_CACHE
	uint8_t tweaked_salt[MQOM3_PARAM_SALT_SIZE];
	enc_ctx_pub_ecb DECL_VAR(enc_ctx_local);
#endif

	for (i_ = 0; i_ < BLC_NB_LEAF_SEEDS_IN_PARALLEL; i_++) {
		pos[i_] = get_gray_code_bit_position(i+i_);
	}

	/* Compute XOR mask and block[0] for first SeedExpand output slot.
	 * CT: xor_mask = block[0] = Enc(ctx[0], seed); accumulate lseed at acc[0] (skip hidden).
	 * OT: xor_mask = LinOrtho(seed); block[0] = Enc(ctx[0], seed) ^ LinOrtho(seed); accumulate block[0] at acc[0] (skip hidden). */
	__BENCHMARK_START__(BS_BLC_PRG);
#if MQOM3_PARAM_OT_VARIANT != 1
	/* CT: xor_mask = Enc(ctx[0], seed) */
#ifndef BLC_SEEDEXPAND_CACHE
	TweakSalt(folding->salt, tweaked_salt, SALT_SEL_SEED_EXPAND, IndexIdentifier(folding->e, 0));
	ret = enc_key_sched_pub_ecb(&enc_ctx_local, tweaked_salt);
	ERR(ret, err);
	ret = enc_encrypt_pub_ecb(&enc_ctx_local, BLC_NB_LEAF_SEEDS_IN_PARALLEL, (uint8_t*) lseed, (uint8_t*) xor_mask);
#else
	ret = enc_encrypt_pub_ecb(&folding->enc_ctx[0], BLC_NB_LEAF_SEEDS_IN_PARALLEL, (uint8_t*) lseed, (uint8_t*) xor_mask);
#endif
	ERR(ret, err);
#else
	/* OT: xor_mask = LinOrtho(seed); also compute Enc(ctx[0], seed) for block[0] */
	uint8_t block0_enc[BLC_NB_LEAF_SEEDS_IN_PARALLEL][MQOM3_PARAM_SEED_SIZE];
	for (i_ = 0; i_ < BLC_NB_LEAF_SEEDS_IN_PARALLEL; i_++) {
		LinOrtho(lseed[i_], xor_mask[i_]);
	}
#ifndef BLC_SEEDEXPAND_CACHE
	TweakSalt(folding->salt, tweaked_salt, SALT_SEL_SEED_EXPAND, IndexIdentifier(folding->e, 0));
	ret = enc_key_sched_pub_ecb(&enc_ctx_local, tweaked_salt);
	ERR(ret, err);
	ret = enc_encrypt_pub_ecb(&enc_ctx_local, BLC_NB_LEAF_SEEDS_IN_PARALLEL, (uint8_t*) lseed, (uint8_t*) block0_enc);
#else
	ret = enc_encrypt_pub_ecb(&folding->enc_ctx[0], BLC_NB_LEAF_SEEDS_IN_PARALLEL, (uint8_t*) lseed, (uint8_t*) block0_enc);
#endif
	ERR(ret, err);
#endif
	__BENCHMARK_STOP__(BS_BLC_PRG);

	__BENCHMARK_START__(BS_BLC_ARITH);
	for (i_ = 0; i_ < BLC_NB_LEAF_SEEDS_IN_PARALLEL; i_++) {
#if MQOM3_PARAM_OT_VARIANT == 1
		/* OT: accumulate block[0][i_] = Enc(ctx[0], lseed[i_]) ^ LinOrtho(lseed[i_]) (skip hidden) */
		uint8_t block0[MQOM3_PARAM_SEED_SIZE];
		xor_blocks(block0_enc[i_], xor_mask[i_], block0);
		if (hidden_index != i + i_) {
			xor_blocks(&folding->acc[0], block0, &folding->acc[0]);
		}
#else
		/* CT: accumulate raw lseed (= first SEED_SIZE bytes of SeedExpand.Correlated output) (skip hidden) */
		if (hidden_index != i + i_) {
			xor_blocks(&folding->acc[0], lseed[i_], &folding->acc[0]);
		}
#endif
#if defined(BLC_NO_FAST_FOLDING) && (BLC_NB_LEAF_SEEDS_IN_PARALLEL > 1)
		/* Snapshot acc's cumulative value unconditionally, exactly like
		 * fast-folding's "data[pos[i_]] ^= acc" does - for the hidden leaf,
		 * acc simply was not updated above, so this just re-captures the
		 * same value as the previous leaf, which is correct (see the
		 * finalize loop's comment below). NOT block0/lseed[i_] - see the
		 * comment in SeedExpandThenAccumulate_sign. */
		memcpy(&folding->leaf_snapshot[i_][0], &folding->acc[0], MQOM3_PARAM_SEED_SIZE);
#endif
#ifndef BLC_NO_FAST_FOLDING
		xor_blocks(&folding->data[pos[i_]][0], &folding->acc[0], &folding->data[pos[i_]][0]);
#endif
	}
	__BENCHMARK_STOP__(BS_BLC_ARITH);

	for (j = 0; j < (PRG_BLC_SIZE / MQOM3_PARAM_SEED_SIZE); j++) {
		/* Key schedule */
		__BENCHMARK_START__(BS_BLC_PRG);
#ifndef BLC_SEEDEXPAND_CACHE
		TweakSalt(folding->salt, tweaked_salt, SALT_SEL_SEED_EXPAND, IndexIdentifier(folding->e, seedexpand_blk_idx(j)));
		ret = enc_key_sched_pub_ecb(&enc_ctx_local, tweaked_salt);
		ERR(ret, err);
		ret = enc_encrypt_pub_ecb(&enc_ctx_local, BLC_NB_LEAF_SEEDS_IN_PARALLEL, (uint8_t*) lseed, (uint8_t*) out_data);
#else
		ret = enc_encrypt_pub_ecb(&folding->enc_ctx[seedexpand_blk_idx(j)], BLC_NB_LEAF_SEEDS_IN_PARALLEL, (uint8_t*) lseed, (uint8_t*) out_data);
#endif
		ERR(ret, err);
		__BENCHMARK_STOP__(BS_BLC_PRG);

		for (i_ = 0; i_ < BLC_NB_LEAF_SEEDS_IN_PARALLEL; i_++) {
			/* Xor with XOR mask */
			xor_blocks(out_data[i_], xor_mask[i_], out_data[i_]);

			if (hidden_index == i + i_) {
				memset(out_data[i_], 0, MQOM3_PARAM_SEED_SIZE);
			}

			__BENCHMARK_START__(BS_BLC_ARITH);
			xor_blocks(&folding->acc[(j+1)*MQOM3_PARAM_SEED_SIZE], out_data[i_], &folding->acc[(j+1)*MQOM3_PARAM_SEED_SIZE]);
#if defined(BLC_NO_FAST_FOLDING) && (BLC_NB_LEAF_SEEDS_IN_PARALLEL > 1)
			/* Snapshot acc's cumulative value, not out_data[i_]. */
			memcpy(&folding->leaf_snapshot[i_][(j+1)*MQOM3_PARAM_SEED_SIZE], &folding->acc[(j+1)*MQOM3_PARAM_SEED_SIZE], MQOM3_PARAM_SEED_SIZE);
#endif
#ifndef BLC_NO_FAST_FOLDING
			xor_blocks(&folding->data[pos[i_]][(j+1)*MQOM3_PARAM_SEED_SIZE], &folding->acc[(j+1)*MQOM3_PARAM_SEED_SIZE], &folding->data[pos[i_]][(j+1)*MQOM3_PARAM_SEED_SIZE]);
#endif
			__BENCHMARK_STOP__(BS_BLC_ARITH);
		}
	}
	/* Deal with the possible leftover incomplete block */
#if PRG_BLC_SIZE % MQOM3_PARAM_SEED_SIZE != 0
	/* Key schedule */
	__BENCHMARK_START__(BS_BLC_PRG);
#ifndef BLC_SEEDEXPAND_CACHE
	TweakSalt(folding->salt, tweaked_salt, SALT_SEL_SEED_EXPAND, IndexIdentifier(folding->e, seedexpand_blk_idx(j)));
	ret = enc_key_sched_pub_ecb(&enc_ctx_local, tweaked_salt);
	ERR(ret, err);
	ret = enc_encrypt_pub_ecb(&enc_ctx_local, BLC_NB_LEAF_SEEDS_IN_PARALLEL, (uint8_t*) lseed, (uint8_t*) out_data);
#else
	ret = enc_encrypt_pub_ecb(&folding->enc_ctx[seedexpand_blk_idx(j)], BLC_NB_LEAF_SEEDS_IN_PARALLEL, (uint8_t*) lseed, (uint8_t*) out_data);
#endif
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_PRG);

	for (i_ = 0; i_ < BLC_NB_LEAF_SEEDS_IN_PARALLEL; i_++) {
		/* Xor with XOR mask */
		xor_blocks(out_data[i_], xor_mask[i_], out_data[i_]);

		if (hidden_index == i + i_) {
			memset(out_data[i_], 0, MQOM3_PARAM_SEED_SIZE);
		}

		__BENCHMARK_START__(BS_BLC_ARITH);
		xor_blocks_partial(&folding->acc[(j+1)*MQOM3_PARAM_SEED_SIZE], out_data[i_], &folding->acc[(j+1)*MQOM3_PARAM_SEED_SIZE]);
#if defined(BLC_NO_FAST_FOLDING) && (BLC_NB_LEAF_SEEDS_IN_PARALLEL > 1)
		memcpy(&folding->leaf_snapshot[i_][(j+1)*MQOM3_PARAM_SEED_SIZE], &folding->acc[(j+1)*MQOM3_PARAM_SEED_SIZE], PRG_BLC_SIZE % MQOM3_PARAM_SEED_SIZE);
#endif
#ifndef BLC_NO_FAST_FOLDING
		xor_blocks_partial(&folding->data[pos[i_]][(j+1)*MQOM3_PARAM_SEED_SIZE], &folding->acc[(j+1)*MQOM3_PARAM_SEED_SIZE], &folding->data[pos[i_]][(j+1)*MQOM3_PARAM_SEED_SIZE]);
#endif
		__BENCHMARK_STOP__(BS_BLC_ARITH);
	}
#endif

#ifdef BLC_NO_FAST_FOLDING
	/* Slow folding: see SeedExpandThenAccumulate_sign. At N == 1, the hidden
	 * leaf is already handled transparently above (acc simply was not
	 * updated for it), so folding it in here unconditionally is correct -
	 * it just re-adds the same acc snapshot as the previous leaf, exactly
	 * like the data[pos] += acc update it replaces would have. At N > 1,
	 * leaf_snapshot[i_] likewise already holds that same "unchanged acc"
	 * snapshot for the hidden leaf (see above), so including it
	 * unconditionally here is correct for the same reason. */
	__BENCHMARK_START__(BS_BLC_ARITH);
	field_base_elt bar_x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt bar_u[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	field_ext_elt tmp_n[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt *tmp_eta = bar_u;
	for (i_ = 0; i_ < BLC_NB_LEAF_SEEDS_IN_PARALLEL; i_++) {
#if BLC_NB_LEAF_SEEDS_IN_PARALLEL > 1
		field_base_parse(folding->leaf_snapshot[i_], MQOM3_PARAM_MQ_N, bar_x);
#else
		field_base_parse(folding->acc, MQOM3_PARAM_MQ_N, bar_x);
#endif
		field_ext_base_constant_vect_mult((1 << pos[i_]), bar_x, tmp_n, MQOM3_PARAM_MQ_N);
		field_ext_vect_add(folding->x0, tmp_n, folding->x0, MQOM3_PARAM_MQ_N);

#if BLC_NB_LEAF_SEEDS_IN_PARALLEL > 1
		field_ext_parse(folding->leaf_snapshot[i_] + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N), MQOM3_PARAM_ETA, bar_u);
#else
		field_ext_parse(folding->acc + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N), MQOM3_PARAM_ETA, bar_u);
#endif
		field_ext_constant_vect_mult((1 << pos[i_]), bar_u, tmp_eta, MQOM3_PARAM_ETA);
		field_ext_vect_add(folding->u0, tmp_eta, folding->u0, MQOM3_PARAM_ETA);
	}
	__BENCHMARK_STOP__(BS_BLC_ARITH);
#endif

	ret = 0;
err:
#ifndef BLC_SEEDEXPAND_CACHE
	enc_clean_ctx_pub_ecb(&enc_ctx_local);
#endif
	return ret;
}

static inline void CT_FinalizeFolding_sign(const folding_sign_t* folding, const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], uint8_t partial_delta_x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)-MQOM3_PARAM_SEED_SIZE], field_ext_elt x0[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u0[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt u1[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	field_base_elt bar_x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	/* Alias acc_x to save stack space */
	field_base_elt *acc_x = bar_x;

#ifndef BLC_NO_FAST_FOLDING
	field_ext_elt bar_u[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	field_ext_elt tmp_n[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	/* Alias tmp_eta to save stack space */
	field_ext_elt *tmp_eta = bar_u;

	memset(x0, 0, BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_N));
	for (uint32_t j = 0; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		field_base_parse(folding->data[j], MQOM3_PARAM_MQ_N, bar_x);
		field_ext_base_constant_vect_mult((1 << j), bar_x, tmp_n, MQOM3_PARAM_MQ_N);
		field_ext_vect_add(x0, tmp_n, x0, MQOM3_PARAM_MQ_N);
	}

	memset(u0, 0, BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA));
	for (uint32_t j = 0; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		field_ext_parse(folding->data[j] + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N), MQOM3_PARAM_ETA, bar_u);
		field_ext_constant_vect_mult((1 << j), bar_u, tmp_eta, MQOM3_PARAM_ETA);
		field_ext_vect_add(u0, tmp_eta, u0, MQOM3_PARAM_ETA);
	}
#else
	/* Slow folding: x0/u0 already hold the final Gray-code-weighted sum,
	 * accumulated incrementally in SeedExpandThenAccumulate_sign via
	 * folding->x0/folding->u0 (which alias these very buffers). */
	(void) x0;
	(void) u0;
#endif

	field_base_parse(folding->acc, MQOM3_PARAM_MQ_N, acc_x);
	field_ext_parse(folding->acc + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N), MQOM3_PARAM_ETA, u1);

	field_base_elt delta_x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	uint8_t serialized_delta_x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)];
	field_base_vect_add(x, acc_x, delta_x, MQOM3_PARAM_MQ_N);
	field_base_serialize(delta_x, MQOM3_PARAM_MQ_N, serialized_delta_x);
	memcpy(partial_delta_x, serialized_delta_x + MQOM3_PARAM_SEED_SIZE, BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) - MQOM3_PARAM_SEED_SIZE);

	mqom_cleanse(bar_x, sizeof(bar_x));
#ifndef BLC_NO_FAST_FOLDING
	mqom_cleanse(bar_u, sizeof(bar_u));
	mqom_cleanse(tmp_n, sizeof(tmp_n));
#endif
}

static inline void CT_FinalizeFolding_verify(const folding_verify_t* folding, uint16_t i_star, const uint8_t partial_delta_x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)-MQOM3_PARAM_SEED_SIZE], field_ext_elt x_eval[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u_eval[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	field_base_elt bar_x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt bar_u[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	field_ext_elt tmp_n[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	/* Alias tmp_eta to save stack space */
	field_ext_elt *tmp_eta = bar_u;
	/* Alias acc_x to save stack space */
	field_base_elt *acc_x = bar_x;

	field_ext_elt r = get_evaluation_point(i_star);

	field_base_elt delta_x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	uint8_t serialized_delta_x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)];
	memset(serialized_delta_x, 0, MQOM3_PARAM_SEED_SIZE);
	memcpy(serialized_delta_x + MQOM3_PARAM_SEED_SIZE, partial_delta_x, BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) - MQOM3_PARAM_SEED_SIZE);
	field_base_parse(serialized_delta_x, MQOM3_PARAM_MQ_N, delta_x);

#ifndef BLC_NO_FAST_FOLDING
	memset(x_eval, 0, BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_N));
	for (uint32_t j = 0; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		field_base_parse(folding->data[j], MQOM3_PARAM_MQ_N, bar_x);
		field_ext_base_constant_vect_mult((1 << j), bar_x, tmp_n, MQOM3_PARAM_MQ_N);
		field_ext_vect_add(x_eval, tmp_n, x_eval, MQOM3_PARAM_MQ_N);
	}
#endif
	/* Slow folding: x_eval already holds the Gray-code-weighted sum,
	 * accumulated incrementally in SeedExpandThenAccumulate_verify; only the
	 * r-scaled term below still needs to be added, same as fast folding. */
	field_base_parse(folding->acc, MQOM3_PARAM_MQ_N, acc_x);
	field_base_vect_add(acc_x, delta_x, acc_x, MQOM3_PARAM_MQ_N);
	field_ext_base_constant_vect_mult(r, acc_x, tmp_n, MQOM3_PARAM_MQ_N);
	field_ext_vect_add(x_eval, tmp_n, x_eval, MQOM3_PARAM_MQ_N);

#ifndef BLC_NO_FAST_FOLDING
	memset(u_eval, 0, BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA));
	for (uint32_t j = 0; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		field_ext_parse(folding->data[j] + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N), MQOM3_PARAM_ETA, bar_u);
		field_ext_constant_vect_mult((1 << j), bar_u, tmp_eta, MQOM3_PARAM_ETA);
		field_ext_vect_add(u_eval, tmp_eta, u_eval, MQOM3_PARAM_ETA);
	}
#endif
	field_ext_parse(folding->acc + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N), MQOM3_PARAM_ETA, tmp_eta);
	field_ext_constant_vect_mult(r, tmp_eta, tmp_eta, MQOM3_PARAM_ETA);
	field_ext_vect_add(u_eval, tmp_eta, u_eval, MQOM3_PARAM_ETA);
}


/* OT variant: delta_x is full BYTE_SIZE_FIELD_BASE(MQ_N) bytes (no FirstBits/partial trick). */
static inline void OT_FinalizeFolding_sign(const folding_sign_t* folding, const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)], uint8_t delta_x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)], field_ext_elt x0[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u0[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)], field_ext_elt u1[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	field_base_elt bar_x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	/* Alias acc_x to save stack space */
	field_base_elt *acc_x = bar_x;

#ifndef BLC_NO_FAST_FOLDING
	field_ext_elt bar_u[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	field_ext_elt tmp_n[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	/* Alias tmp_eta to save stack space */
	field_ext_elt *tmp_eta = bar_u;

	memset(x0, 0, BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_N));
	for (uint32_t j = 0; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		field_base_parse(folding->data[j], MQOM3_PARAM_MQ_N, bar_x);
		field_ext_base_constant_vect_mult((1 << j), bar_x, tmp_n, MQOM3_PARAM_MQ_N);
		field_ext_vect_add(x0, tmp_n, x0, MQOM3_PARAM_MQ_N);
	}

	memset(u0, 0, BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA));
	for (uint32_t j = 0; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		field_ext_parse(folding->data[j] + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N), MQOM3_PARAM_ETA, bar_u);
		field_ext_constant_vect_mult((1 << j), bar_u, tmp_eta, MQOM3_PARAM_ETA);
		field_ext_vect_add(u0, tmp_eta, u0, MQOM3_PARAM_ETA);
	}
#else
	/* Slow folding: see CT_FinalizeFolding_sign. */
	(void) x0;
	(void) u0;
#endif

	field_base_parse(folding->acc, MQOM3_PARAM_MQ_N, acc_x);
	field_ext_parse(folding->acc + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N), MQOM3_PARAM_ETA, u1);

	field_base_elt delta_x_field[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	field_base_vect_add(x, acc_x, delta_x_field, MQOM3_PARAM_MQ_N);
	/* OT: serialize full delta_x (no SEED_SIZE-byte skip as in CT partial_delta_x) */
	field_base_serialize(delta_x_field, MQOM3_PARAM_MQ_N, delta_x);

	mqom_cleanse(bar_x, sizeof(bar_x));
#ifndef BLC_NO_FAST_FOLDING
	mqom_cleanse(bar_u, sizeof(bar_u));
	mqom_cleanse(tmp_n, sizeof(tmp_n));
#endif
}

/* OT variant: reads full delta_x[BYTE_SIZE_FIELD_BASE(MQ_N)] directly (no zero-prefix padding). */
static inline void OT_FinalizeFolding_verify(const folding_verify_t* folding, uint16_t i_star, const uint8_t delta_x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)], field_ext_elt x_eval[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)], field_ext_elt u_eval[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]) {
	field_base_elt bar_x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	field_ext_elt bar_u[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	field_ext_elt tmp_n[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	/* Alias tmp_eta to save stack space */
	field_ext_elt *tmp_eta = bar_u;
	/* Alias acc_x to save stack space */
	field_base_elt *acc_x = bar_x;

	field_ext_elt r = get_evaluation_point(i_star);

	field_base_elt delta_x_field[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	/* OT: parse full delta_x directly (CT zero-pads first SEED_SIZE bytes; OT does not) */
	field_base_parse(delta_x, MQOM3_PARAM_MQ_N, delta_x_field);

#ifndef BLC_NO_FAST_FOLDING
	memset(x_eval, 0, BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_N));
	for (uint32_t j = 0; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		field_base_parse(folding->data[j], MQOM3_PARAM_MQ_N, bar_x);
		field_ext_base_constant_vect_mult((1 << j), bar_x, tmp_n, MQOM3_PARAM_MQ_N);
		field_ext_vect_add(x_eval, tmp_n, x_eval, MQOM3_PARAM_MQ_N);
	}
#endif
	field_base_parse(folding->acc, MQOM3_PARAM_MQ_N, acc_x);
	field_base_vect_add(acc_x, delta_x_field, acc_x, MQOM3_PARAM_MQ_N);
	field_ext_base_constant_vect_mult(r, acc_x, tmp_n, MQOM3_PARAM_MQ_N);
	field_ext_vect_add(x_eval, tmp_n, x_eval, MQOM3_PARAM_MQ_N);

#ifndef BLC_NO_FAST_FOLDING
	memset(u_eval, 0, BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA));
	for (uint32_t j = 0; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		field_ext_parse(folding->data[j] + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N), MQOM3_PARAM_ETA, bar_u);
		field_ext_constant_vect_mult((1 << j), bar_u, tmp_eta, MQOM3_PARAM_ETA);
		field_ext_vect_add(u_eval, tmp_eta, u_eval, MQOM3_PARAM_ETA);
	}
#endif
	field_ext_parse(folding->acc + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N), MQOM3_PARAM_ETA, tmp_eta);
	field_ext_constant_vect_mult(r, tmp_eta, tmp_eta, MQOM3_PARAM_ETA);
	field_ext_vect_add(u_eval, tmp_eta, u_eval, MQOM3_PARAM_ETA);
}

#endif /* __BLC_MEMOPT_FOLDING_H__ */
