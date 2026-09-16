#ifndef __BLC_CONVERT_H__
#define __BLC_CONVERT_H__

#include <string.h>
#include "enc.h"
#include "xof.h"
#include "fields.h"
#include "benchmark.h"
#include "blc_common.h"
#include "seed_commit.h"
#include "seed_expand.h"
#include "domain_separation.h"

#define BLC_CONVERT_EXP_ROW_BYTES (BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA))
#define BLC_CONVERT_EXP_GROUP_BYTES (8 * BLC_CONVERT_EXP_ROW_BYTES)

/* BLC_ConvertToLine / BLC_ConvertToLineEval loop with stride 8; guard against
 * future parameter sets where NB_EVALS could be < 8 or not divisible by 8. */
#if MQOM3_PARAM_NB_EVALS < 8
#error "BLC_ConvertToLine requires MQOM3_PARAM_NB_EVALS >= 8"
#endif
#if (MQOM3_PARAM_NB_EVALS % 8) != 0
#error "BLC_ConvertToLine requires MQOM3_PARAM_NB_EVALS divisible by 8"
#endif
/* The commitments of a group of 8 leaves are handed to SeedCommit_x8_* as a
 * single contiguous array of stride 2 * SEED_SIZE, obtained by casting a slice
 * of ls_com[], whose stride is DIGEST_SIZE. The two strides must agree. */
#if MQOM3_PARAM_DIGEST_SIZE != (2 * MQOM3_PARAM_SEED_SIZE)
#error "BLC_ConvertToLine requires MQOM3_PARAM_DIGEST_SIZE == 2 * MQOM3_PARAM_SEED_SIZE"
#endif

/* Maximum batch size for Hash_7 based on available SIMD XOF width */
#if defined(USE_XOF_X8)
#define BLC_HASH7_BATCH_MAX 8
#elif defined(USE_XOF_X4)
#define BLC_HASH7_BATCH_MAX 4
#else
#define BLC_HASH7_BATCH_MAX 1
#endif

/* Byte size of delta_x input to Hash_7 (OT: full N bytes; CT: N minus SEED_SIZE) */
#if MQOM3_PARAM_OT_VARIANT == 1
#define BLC_HASH7_DELTAX_SIZE BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)
#else
#define BLC_HASH7_DELTAX_SIZE (BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) - MQOM3_PARAM_SEED_SIZE)
#endif

/* Invalidate all active entries in a SeedExpand cache without freeing it,
 * so the same allocation can be reused across successive executions (e).
 * Shared by all default BLC variants (CT and OT). */
static inline void reset_SeedExpand_cache_ecb_all(SeedExpand_cache_ecb *cache) {
	if (cache != NULL) {
		uint32_t i, n;
		n = cache->size / sizeof(SeedExpand_cache_ecb);
		for (i = 0; i < n; i++) {
			invalidate_entry_SeedExpand_cache_ecb(cache, i);
		}
	}
}

static inline void reset_SeedExpand_cache_pub_ecb_all(SeedExpand_cache_pub_ecb *cache) {
	if (cache != NULL) {
		uint32_t i, n;
		n = cache->size / sizeof(SeedExpand_cache_pub_ecb);
		for (i = 0; i < n; i++) {
			invalidate_entry_SeedExpand_cache_pub_ecb(cache, i);
		}
	}
}

/* BLC.ConvertToLine (see spec): for execution e, given all leaf seeds lseed[],
 * commit each seed, expand each seed via Gray-code folding.
 * com1 is computed separately by the caller (see BLC_Hash_com1*). */
static inline int BLC_ConvertToLine(
    const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
    uint32_t e,
    const uint8_t lseed[MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE],
    const field_base_elt x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)],
    uint8_t ls_com[MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_DIGEST_SIZE],
#if MQOM3_PARAM_OT_VARIANT == 1
    uint8_t delta_x_out[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)],
#else
    uint8_t delta_x_out[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) - MQOM3_PARAM_SEED_SIZE],
#endif
    field_ext_elt x0[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)],
    field_ext_elt u0[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)],
    field_ext_elt u1[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)],
    SeedExpand_cache_ecb *cache_ecb) {
	int ret = -1;
	uint32_t i;
	enc_ctx_ecb DECL_VAR(ctx_sc1), DECL_VAR(ctx_sc2);
	uint8_t tweaked_salt_0[MQOM3_PARAM_SALT_SIZE], tweaked_salt_1[MQOM3_PARAM_SALT_SIZE];
	/* One group of 8 expanded leaves only (see the header comment above) */
	uint8_t exp[8][BLC_CONVERT_EXP_ROW_BYTES];
	/* aligned(64): GCC infers 64-byte alignment for these from the preceding
	 * exp[] array, but ASan's fake stack breaks it on AVX-512 loads, causing a #GP fault.
	 * Explicit aligned(64) forces both the real stack and ASan to honour it. */
	ALIGN(64) field_base_elt bar_x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	ALIGN(64) field_ext_elt bar_u[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	ALIGN(64) field_ext_elt tmp_n[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	ALIGN(64) field_ext_elt tmp_eta[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	field_base_elt acc_x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	uint8_t data_folding[MQOM3_PARAM_NB_EVALS_LOG][BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA)];
	uint8_t acc[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA)];

	__BENCHMARK_START__(BS_BLC_SEED_COMMIT);
	TweakSalt(salt, tweaked_salt_0, SALT_SEL_BLC_LEFT, e);
	ret = enc_key_sched_ecb(&ctx_sc1, tweaked_salt_0);
	ERR(ret, err);
	TweakSalt(salt, tweaked_salt_1, SALT_SEL_BLC_RIGHT, e);
	ret = enc_key_sched_ecb(&ctx_sc2, tweaked_salt_1);
	ERR(ret, err);
	__BENCHMARK_STOP__(BS_BLC_SEED_COMMIT);

	/* Gray-code folding bucket of each leaf of a group of 8 (see the
	 * assignment of gpos[7] in the loop below for the eighth one). */
	uint8_t gpos[8] = { 0, 1, 0, 2, 0, 1, 0, 0 };
	/* exp_ptr[] is loop-invariant now that exp[] is a fixed 8-row group */
#if MQOM3_PARAM_OT_VARIANT == 1
	/* OT: SeedExpand.Uncorrelated output = block[0]||block[1]||... (all AES^LinOrtho, no raw seed) */
	uint8_t *exp_ptr[8] = {
		(uint8_t*) exp[0], (uint8_t*) exp[1], (uint8_t*) exp[2], (uint8_t*) exp[3],
		(uint8_t*) exp[4], (uint8_t*) exp[5], (uint8_t*) exp[6], (uint8_t*) exp[7]
	};
#else
	/* CT: SeedExpand.Correlated output = seed||block[1]^block[0]||... (raw seed first) */
	uint8_t *exp_ptr[8] = {
		(uint8_t*) (exp[0] + MQOM3_PARAM_SEED_SIZE), (uint8_t*) (exp[1] + MQOM3_PARAM_SEED_SIZE),
		(uint8_t*) (exp[2] + MQOM3_PARAM_SEED_SIZE), (uint8_t*) (exp[3] + MQOM3_PARAM_SEED_SIZE),
		(uint8_t*) (exp[4] + MQOM3_PARAM_SEED_SIZE), (uint8_t*) (exp[5] + MQOM3_PARAM_SEED_SIZE),
		(uint8_t*) (exp[6] + MQOM3_PARAM_SEED_SIZE), (uint8_t*) (exp[7] + MQOM3_PARAM_SEED_SIZE)
	};
#endif

	memset((uint8_t*) data_folding, 0, sizeof(data_folding));
	memset((uint8_t*) acc, 0, sizeof(acc));
	for (i = 0; i < MQOM3_PARAM_NB_EVALS; i += 8) {
		__BENCHMARK_START__(BS_BLC_SEED_COMMIT);
		/* lseed[i..i+7] and ls_com[i..i+7] are contiguous slices, so the whole
		 * group of 8 goes through a single pair of 8-block ECB encryptions
		 * instead of two pairs of 4-block ones. */
		ret = SeedCommit_x8_ecb(&ctx_sc1, &ctx_sc2,
		              (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) &lseed[i],
		              (uint8_t (*)[2 * MQOM3_PARAM_SEED_SIZE]) &ls_com[i]);
		ERR(ret, err);
		__BENCHMARK_STOP__(BS_BLC_SEED_COMMIT);

		__BENCHMARK_START__(BS_BLC_PRG);
#if MQOM3_PARAM_OT_VARIANT == 1
		ret = SeedExpand_x8_ecb(salt, e, (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE])&lseed[i], PRG_BLC_SIZE + MQOM3_PARAM_SEED_SIZE, exp_ptr, cache_ecb);
		ERR(ret, err);
#else
		/* CT: the raw seed is the first SEED_SIZE bytes of each row */
		memcpy(exp[0], lseed[i + 0], MQOM3_PARAM_SEED_SIZE);
		memcpy(exp[1], lseed[i + 1], MQOM3_PARAM_SEED_SIZE);
		memcpy(exp[2], lseed[i + 2], MQOM3_PARAM_SEED_SIZE);
		memcpy(exp[3], lseed[i + 3], MQOM3_PARAM_SEED_SIZE);
		memcpy(exp[4], lseed[i + 4], MQOM3_PARAM_SEED_SIZE);
		memcpy(exp[5], lseed[i + 5], MQOM3_PARAM_SEED_SIZE);
		memcpy(exp[6], lseed[i + 6], MQOM3_PARAM_SEED_SIZE);
		memcpy(exp[7], lseed[i + 7], MQOM3_PARAM_SEED_SIZE);
		ret = SeedExpand_x8_ecb(salt, e, (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE])&lseed[i], PRG_BLC_SIZE, exp_ptr, cache_ecb);
		ERR(ret, err);
#endif
		__BENCHMARK_STOP__(BS_BLC_PRG);

		__BENCHMARK_START__(BS_BLC_ARITH);
		/* i is a multiple of 8, so the Gray-code bucket of the first seven
		 * leaves of the group is the fixed ruler pattern 0,1,0,2,0,1,0 (see
		 * gpos[] above) and only the eighth one has to be computed. */
		gpos[7] = get_gray_code_bit_position(i + 7);
		for (uint32_t i_ = 0; i_ < 8; i_++) {
			uint8_t j = gpos[i_];
			field_base_vect_add(acc, exp[i_], acc, MQOM3_PARAM_MQ_N + MQOM3_PARAM_ETA * MQOM3_PARAM_MU);
			field_base_vect_add(data_folding[j], acc, data_folding[j], MQOM3_PARAM_MQ_N + MQOM3_PARAM_ETA * MQOM3_PARAM_MU);
		}
		__BENCHMARK_STOP__(BS_BLC_ARITH);
	}

	/* Compute P_u and P_z */
	__BENCHMARK_START__(BS_BLC_ARITH);
	field_base_parse(acc, MQOM3_PARAM_MQ_N, acc_x);
	field_ext_parse(acc + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N), MQOM3_PARAM_ETA, u1);

	memset(x0, 0, sizeof(field_ext_elt[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)]));
	for (uint32_t j = 0; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		field_base_parse(data_folding[j], MQOM3_PARAM_MQ_N, bar_x);
		field_ext_base_constant_vect_mult((1 << j), bar_x, tmp_n, MQOM3_PARAM_MQ_N);
		field_ext_vect_add(x0, tmp_n, x0, MQOM3_PARAM_MQ_N);
	}

	memset(u0, 0, sizeof(field_ext_elt[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]));
	for (uint32_t j = 0; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		field_ext_parse(data_folding[j] + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N), MQOM3_PARAM_ETA, bar_u);
		field_ext_constant_vect_mult((1 << j), bar_u, tmp_eta, MQOM3_PARAM_ETA);
		field_ext_vect_add(u0, tmp_eta, u0, MQOM3_PARAM_ETA);
	}

	field_base_elt delta_x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	uint8_t serialized_delta_x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)];
	field_base_vect_add(x, acc_x, delta_x, MQOM3_PARAM_MQ_N);
	field_base_serialize(delta_x, MQOM3_PARAM_MQ_N, serialized_delta_x);
#if MQOM3_PARAM_OT_VARIANT == 1
	memcpy(delta_x_out, serialized_delta_x, BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N));
#else
	memcpy(delta_x_out, serialized_delta_x + MQOM3_PARAM_SEED_SIZE, BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) - MQOM3_PARAM_SEED_SIZE);
#endif
	__BENCHMARK_STOP__(BS_BLC_ARITH);

	ret = 0;
err:
	mqom_cleanse(exp, BLC_CONVERT_EXP_GROUP_BYTES);
	mqom_cleanse(data_folding, sizeof(data_folding));
	mqom_cleanse(acc, sizeof(acc));
	mqom_cleanse(acc_x, sizeof(acc_x));
	mqom_cleanse(bar_x, sizeof(bar_x));
	mqom_cleanse(bar_u, sizeof(bar_u));
	mqom_cleanse(tmp_n, sizeof(tmp_n));
	mqom_cleanse(tmp_eta, sizeof(tmp_eta));
	enc_clean_ctx_ecb(&ctx_sc1);
	enc_clean_ctx_ecb(&ctx_sc2);
	return ret;
}

/* BLC.ConvertToLineEval (see spec): verifier path for execution e, given partially-expanded
 * leaf seeds lseed[] (lseed[i_star] is zero), computes x_eval, u_eval, and ls_com_out[].
 * com1 is computed separately by the caller (see BLC_Hash_com1*).
 * Shared between BLC.Eval.CT and BLC.Eval.OT. */
static inline int BLC_ConvertToLineEval(
    const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
    uint32_t e,
    const uint8_t lseed[MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_SEED_SIZE],
    uint16_t i_star,
    const uint8_t out_ls_com[MQOM3_PARAM_DIGEST_SIZE],
#if MQOM3_PARAM_OT_VARIANT == 1
    const uint8_t delta_x_in[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)],
#else
    const uint8_t delta_x_in[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) - MQOM3_PARAM_SEED_SIZE],
#endif
    field_ext_elt x_eval[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)],
    field_ext_elt u_eval[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)],
    uint8_t ls_com_out[MQOM3_PARAM_NB_EVALS][MQOM3_PARAM_DIGEST_SIZE],
    SeedExpand_cache_pub_ecb *cache_ecb) {
	int ret = -1;
	uint32_t i;
	enc_ctx_pub_ecb DECL_VAR(ctx_sc1), DECL_VAR(ctx_sc2);
	uint8_t tweaked_salt_0[MQOM3_PARAM_SALT_SIZE], tweaked_salt_1[MQOM3_PARAM_SALT_SIZE];
	/* ls_com_out is the caller-provided buffer (NB_EVALS * DIGEST_SIZE) */
	uint8_t (*ls_com)[MQOM3_PARAM_DIGEST_SIZE] = ls_com_out;
	/* One group of 8 expanded leaves only (see the header comment above) */
	uint8_t exp[8][BLC_CONVERT_EXP_ROW_BYTES];
	/* See aligned(64) rationale in BLC_ConvertToLine above. */
	ALIGN(64) field_base_elt bar_x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	ALIGN(64) field_ext_elt bar_u[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	//field_base_elt bar_x_i[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)] = {0};
	//field_ext_elt bar_u_i[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)] = {0};
	ALIGN(64) field_ext_elt tmp_n[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)];
	ALIGN(64) field_ext_elt tmp_eta[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)];
	field_base_elt acc_x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
	uint8_t data_folding[MQOM3_PARAM_NB_EVALS_LOG][BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA)];
	uint8_t acc[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) + BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_ETA)];

	TweakSalt(salt, tweaked_salt_0, SALT_SEL_BLC_LEFT, e);
	ret = enc_key_sched_pub_ecb(&ctx_sc1, tweaked_salt_0);
	ERR(ret, err);
	TweakSalt(salt, tweaked_salt_1, SALT_SEL_BLC_RIGHT, e);
	ret = enc_key_sched_pub_ecb(&ctx_sc2, tweaked_salt_1);
	ERR(ret, err);

	/* Gray-code folding bucket of each leaf of a group of 8 (see the
	 * assignment of gpos[7] in the loop below for the eighth one). */
	uint8_t gpos[8] = { 0, 1, 0, 2, 0, 1, 0, 0 };
	/* exp_ptr[] is loop-invariant now that exp[] is a fixed 8-row group */
#if MQOM3_PARAM_OT_VARIANT == 1
	/* OT: SeedExpand.Uncorrelated output = block[0]||block[1]||... (all AES^LinOrtho, no raw seed) */
	uint8_t *exp_ptr[8] = {
		(uint8_t*) exp[0], (uint8_t*) exp[1], (uint8_t*) exp[2], (uint8_t*) exp[3],
		(uint8_t*) exp[4], (uint8_t*) exp[5], (uint8_t*) exp[6], (uint8_t*) exp[7]
	};
#else
	/* CT: SeedExpand.Correlated output = seed||block[1]^block[0]||... (raw seed first) */
	uint8_t *exp_ptr[8] = {
		(uint8_t*) (exp[0] + MQOM3_PARAM_SEED_SIZE), (uint8_t*) (exp[1] + MQOM3_PARAM_SEED_SIZE),
		(uint8_t*) (exp[2] + MQOM3_PARAM_SEED_SIZE), (uint8_t*) (exp[3] + MQOM3_PARAM_SEED_SIZE),
		(uint8_t*) (exp[4] + MQOM3_PARAM_SEED_SIZE), (uint8_t*) (exp[5] + MQOM3_PARAM_SEED_SIZE),
		(uint8_t*) (exp[6] + MQOM3_PARAM_SEED_SIZE), (uint8_t*) (exp[7] + MQOM3_PARAM_SEED_SIZE)
	};
#endif

	memset((uint8_t*) data_folding, 0, sizeof(data_folding));
	memset((uint8_t*) acc, 0, sizeof(acc));
	for (i = 0; i < MQOM3_PARAM_NB_EVALS; i += 8) {
		// lseed[i_star] is a bytestring with only zeroes
		// it does not prevent to compute seed commitment of it,
		// but we will not use the result
		/* See BLC_ConvertToLine: one pair of 8-block ECB encryptions for the
		 * whole contiguous group of 8 leaves. */
		ret = SeedCommit_x8_pub_ecb(&ctx_sc1, &ctx_sc2,
		                  (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE]) &lseed[i],
		                  (uint8_t (*)[2 * MQOM3_PARAM_SEED_SIZE]) &ls_com[i]);
		ERR(ret, err);

#if MQOM3_PARAM_OT_VARIANT == 1
		ret = SeedExpand_x8_pub_ecb(salt, e, (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE])&lseed[i], PRG_BLC_SIZE + MQOM3_PARAM_SEED_SIZE, exp_ptr, cache_ecb);
		ERR(ret, err);
#else
		/* CT: the raw seed is the first SEED_SIZE bytes of each row */
		memcpy(exp[0], lseed[i + 0], MQOM3_PARAM_SEED_SIZE);
		memcpy(exp[1], lseed[i + 1], MQOM3_PARAM_SEED_SIZE);
		memcpy(exp[2], lseed[i + 2], MQOM3_PARAM_SEED_SIZE);
		memcpy(exp[3], lseed[i + 3], MQOM3_PARAM_SEED_SIZE);
		memcpy(exp[4], lseed[i + 4], MQOM3_PARAM_SEED_SIZE);
		memcpy(exp[5], lseed[i + 5], MQOM3_PARAM_SEED_SIZE);
		memcpy(exp[6], lseed[i + 6], MQOM3_PARAM_SEED_SIZE);
		memcpy(exp[7], lseed[i + 7], MQOM3_PARAM_SEED_SIZE);
		ret = SeedExpand_x8_pub_ecb(salt, e, (const uint8_t (*)[MQOM3_PARAM_SEED_SIZE])&lseed[i], PRG_BLC_SIZE, exp_ptr, cache_ecb);
		ERR(ret, err);
#endif

		/* See BLC_ConvertToLine: only the eighth Gray-code bucket of a group
		 * has to be computed, the first seven are the fixed ruler pattern. */
		gpos[7] = get_gray_code_bit_position(i + 7);
		for (uint32_t i_ = 0; i_ < 8; i_++) {
			if (i + i_ == (uint32_t) i_star) {
				memcpy(ls_com[i_star], out_ls_com, MQOM3_PARAM_DIGEST_SIZE);
				memset(exp[i_], 0, MQOM3_PARAM_SEED_SIZE + PRG_BLC_SIZE);
			}
			field_base_vect_add(acc, exp[i_], acc, MQOM3_PARAM_MQ_N + MQOM3_PARAM_ETA * MQOM3_PARAM_MU);
			uint8_t j = gpos[i_];
			field_base_vect_add(data_folding[j], acc, data_folding[j], MQOM3_PARAM_MQ_N + MQOM3_PARAM_ETA * MQOM3_PARAM_MU);
		}
	}

	field_ext_elt r = get_evaluation_point(i_star);
	field_base_elt delta_x[FIELD_BASE_PACKING(MQOM3_PARAM_MQ_N)];
#if MQOM3_PARAM_OT_VARIANT == 1
	field_base_parse(delta_x_in, MQOM3_PARAM_MQ_N, delta_x);
#else
	uint8_t serialized_delta_x[BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N)];
	memset(serialized_delta_x, 0, MQOM3_PARAM_SEED_SIZE);
	memcpy(serialized_delta_x + MQOM3_PARAM_SEED_SIZE, delta_x_in, BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N) - MQOM3_PARAM_SEED_SIZE);
	field_base_parse(serialized_delta_x, MQOM3_PARAM_MQ_N, delta_x);
#endif

	// Compute v_u and v_x
	memset(x_eval, 0, sizeof(field_ext_elt[FIELD_EXT_PACKING(MQOM3_PARAM_MQ_N)]));
	for (uint32_t j = 0; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		field_base_parse(data_folding[j], MQOM3_PARAM_MQ_N, bar_x);
		field_ext_base_constant_vect_mult((1 << j), bar_x, tmp_n, MQOM3_PARAM_MQ_N);
		field_ext_vect_add(x_eval, tmp_n, x_eval, MQOM3_PARAM_MQ_N);
	}
	field_base_parse(acc, MQOM3_PARAM_MQ_N, acc_x);
	field_base_vect_add(acc_x, delta_x, acc_x, MQOM3_PARAM_MQ_N);
	field_ext_base_constant_vect_mult(r, acc_x, tmp_n, MQOM3_PARAM_MQ_N);
	field_ext_vect_add(x_eval, tmp_n, x_eval, MQOM3_PARAM_MQ_N);

	memset((uint8_t*) u_eval, 0, sizeof(field_ext_elt[FIELD_EXT_PACKING(MQOM3_PARAM_ETA)]));
	for (uint32_t j = 0; j < MQOM3_PARAM_NB_EVALS_LOG; j++) {
		field_ext_parse(data_folding[j] + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N), MQOM3_PARAM_ETA, bar_u);
		field_ext_constant_vect_mult((1 << j), bar_u, tmp_eta, MQOM3_PARAM_ETA);
		field_ext_vect_add(u_eval, tmp_eta, u_eval, MQOM3_PARAM_ETA);
	}
	field_ext_parse(acc + BYTE_SIZE_FIELD_BASE(MQOM3_PARAM_MQ_N), MQOM3_PARAM_ETA, tmp_eta);
	field_ext_constant_vect_mult(r, tmp_eta, tmp_eta, MQOM3_PARAM_ETA);
	field_ext_vect_add(u_eval, tmp_eta, u_eval, MQOM3_PARAM_ETA);

	ret = 0;
err:
	/* Verifier side: exp[] only holds public material, but wipe the small
	 * group buffer anyway to keep both paths symmetric. */
	mqom_cleanse(exp, BLC_CONVERT_EXP_GROUP_BYTES);
	enc_clean_ctx_pub_ecb(&ctx_sc1);
	enc_clean_ctx_pub_ecb(&ctx_sc2);
	return ret;
}

/* Scalar Hash_com1: com1[e] = Hash_7(Bits_8(e), salt, ls_com_flat, delta_x) for one
 * execution (see spec). ls_com_flat points to NB_EVALS * DIGEST_SIZE contiguous bytes. */
static inline int BLC_Hash_com1(
    uint8_t com1[MQOM3_PARAM_DIGEST_SIZE],
    const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
    uint32_t e,
    const uint8_t *ls_com_flat,
    const uint8_t *delta_x) {
	int ret;
	uint8_t e_byte = (uint8_t) e;
	xof_context xof_ctx = { 0 };
	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) DOMAIN_SEP_COM1, 1);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, &e_byte, 1);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, salt, MQOM3_PARAM_SALT_SIZE);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, ls_com_flat, MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, delta_x, BLC_HASH7_DELTAX_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(&xof_ctx, com1, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = 0;
err:
	xof_clean_ctx(&xof_ctx);
	return ret;
}

/* Batched Hash_com1: compute com1[0..n-1] using xof_x8 / xof_x4 / scalar cascade.
 * com1[base_e+i] = Hash_7(Bits_8(base_e+i), salt, ls_com_ptrs[i], delta_x_ptrs[i]) (see spec).
 * base_e is the execution index of the first entry (batch callers may start mid-range).
 * ls_com_ptrs[i] -> flat NB_EVALS * DIGEST_SIZE bytes for execution (base_e+i).
 * delta_x_ptrs[i] -> BLC_HASH7_DELTAX_SIZE bytes for execution (base_e+i).
 * com1[][DIGEST_SIZE] must be a contiguous array of n outputs. */
static inline int BLC_Hash_com1_batch(
    uint32_t base_e,
    uint32_t n,
    uint8_t com1[][MQOM3_PARAM_DIGEST_SIZE],
    const uint8_t salt[MQOM3_PARAM_SALT_SIZE],
    const uint8_t * const ls_com_ptrs[],
    const uint8_t * const delta_x_ptrs[]) {
	int ret = 0;
	uint32_t e = 0;

#if defined(USE_XOF_X8)
	for (; (e + 8) <= n; e += 8) {
		xof_context_x8 ctx_x8 = { 0 };
		const uint8_t *c6_[8] = { (const uint8_t*) DOMAIN_SEP_COM1, (const uint8_t*) DOMAIN_SEP_COM1, (const uint8_t*) DOMAIN_SEP_COM1, (const uint8_t*) DOMAIN_SEP_COM1,
		                           (const uint8_t*) DOMAIN_SEP_COM1, (const uint8_t*) DOMAIN_SEP_COM1, (const uint8_t*) DOMAIN_SEP_COM1, (const uint8_t*) DOMAIN_SEP_COM1 };
		uint8_t e_bytes[8] = {
			(uint8_t)(base_e + e + 0), (uint8_t)(base_e + e + 1), (uint8_t)(base_e + e + 2), (uint8_t)(base_e + e + 3),
			(uint8_t)(base_e + e + 4), (uint8_t)(base_e + e + 5), (uint8_t)(base_e + e + 6), (uint8_t)(base_e + e + 7)
		};
		const uint8_t *e_ptrs[8] = {
			&e_bytes[0], &e_bytes[1], &e_bytes[2], &e_bytes[3],
			&e_bytes[4], &e_bytes[5], &e_bytes[6], &e_bytes[7]
		};
		const uint8_t *salt_ptrs[8] = { salt, salt, salt, salt, salt, salt, salt, salt };
		const uint8_t *lsc[8] = {
			ls_com_ptrs[e + 0], ls_com_ptrs[e + 1], ls_com_ptrs[e + 2], ls_com_ptrs[e + 3],
			ls_com_ptrs[e + 4], ls_com_ptrs[e + 5], ls_com_ptrs[e + 6], ls_com_ptrs[e + 7]
		};
		const uint8_t *dxp[8] = {
			delta_x_ptrs[e + 0], delta_x_ptrs[e + 1], delta_x_ptrs[e + 2], delta_x_ptrs[e + 3],
			delta_x_ptrs[e + 4], delta_x_ptrs[e + 5], delta_x_ptrs[e + 6], delta_x_ptrs[e + 7]
		};
		uint8_t *out[8] = {
			com1[e + 0], com1[e + 1], com1[e + 2], com1[e + 3],
			com1[e + 4], com1[e + 5], com1[e + 6], com1[e + 7]
		};
		int r = xof_init_x8(&ctx_x8);
		if (!r) { r = xof_update_x8(&ctx_x8, c6_, 1); }
		if (!r) { r = xof_update_x8(&ctx_x8, e_ptrs, 1); }
		if (!r) { r = xof_update_x8(&ctx_x8, salt_ptrs, MQOM3_PARAM_SALT_SIZE); }
		if (!r) { r = xof_update_x8(&ctx_x8, lsc, MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_DIGEST_SIZE); }
		if (!r) { r = xof_update_x8(&ctx_x8, dxp, BLC_HASH7_DELTAX_SIZE); }
		if (!r) { r = xof_squeeze_x8(&ctx_x8, out, MQOM3_PARAM_DIGEST_SIZE); }
		xof_clean_ctx_x8(&ctx_x8);
		if (r) { ret = r; goto err; }
	}
#endif
#if defined(USE_XOF_X4)
	for (; (e + 4) <= n; e += 4) {
		xof_context_x4 ctx_x4 = { 0 };
		const uint8_t *c6_[4] = { (const uint8_t*) DOMAIN_SEP_COM1, (const uint8_t*) DOMAIN_SEP_COM1, (const uint8_t*) DOMAIN_SEP_COM1, (const uint8_t*) DOMAIN_SEP_COM1 };
		uint8_t e_bytes[4] = {
			(uint8_t)(base_e + e + 0), (uint8_t)(base_e + e + 1), (uint8_t)(base_e + e + 2), (uint8_t)(base_e + e + 3)
		};
		const uint8_t *e_ptrs[4] = { &e_bytes[0], &e_bytes[1], &e_bytes[2], &e_bytes[3] };
		const uint8_t *salt_ptrs[4] = { salt, salt, salt, salt };
		const uint8_t *lsc[4] = {
			ls_com_ptrs[e + 0], ls_com_ptrs[e + 1], ls_com_ptrs[e + 2], ls_com_ptrs[e + 3]
		};
		const uint8_t *dxp[4] = {
			delta_x_ptrs[e + 0], delta_x_ptrs[e + 1], delta_x_ptrs[e + 2], delta_x_ptrs[e + 3]
		};
		uint8_t *out[4] = {
			com1[e + 0], com1[e + 1], com1[e + 2], com1[e + 3]
		};
		int r = xof_init_x4(&ctx_x4);
		if (!r) { r = xof_update_x4(&ctx_x4, c6_, 1); }
		if (!r) { r = xof_update_x4(&ctx_x4, e_ptrs, 1); }
		if (!r) { r = xof_update_x4(&ctx_x4, salt_ptrs, MQOM3_PARAM_SALT_SIZE); }
		if (!r) { r = xof_update_x4(&ctx_x4, lsc, MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_DIGEST_SIZE); }
		if (!r) { r = xof_update_x4(&ctx_x4, dxp, BLC_HASH7_DELTAX_SIZE); }
		if (!r) { r = xof_squeeze_x4(&ctx_x4, out, MQOM3_PARAM_DIGEST_SIZE); }
		xof_clean_ctx_x4(&ctx_x4);
		if (r) { ret = r; goto err; }
	}
#endif
	for (; e < n; e++) {
		xof_context ctx1 = { 0 };
		uint8_t e_byte = (uint8_t)(base_e + e);
		int r = xof_init(&ctx1);
		if (!r) { r = xof_update(&ctx1, (const uint8_t*) DOMAIN_SEP_COM1, 1); }
		if (!r) { r = xof_update(&ctx1, &e_byte, 1); }
		if (!r) { r = xof_update(&ctx1, salt, MQOM3_PARAM_SALT_SIZE); }
		if (!r) { r = xof_update(&ctx1, ls_com_ptrs[e], MQOM3_PARAM_NB_EVALS * MQOM3_PARAM_DIGEST_SIZE); }
		if (!r) { r = xof_update(&ctx1, delta_x_ptrs[e], BLC_HASH7_DELTAX_SIZE); }
		if (!r) { r = xof_squeeze(&ctx1, com1[e], MQOM3_PARAM_DIGEST_SIZE); }
		xof_clean_ctx(&ctx1);
		if (r) { ret = r; goto err; }
	}
	return 0;
err:
	return ret;
}

#endif /* __BLC_CONVERT_H__ */
