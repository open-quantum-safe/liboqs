#ifndef __PIOP_COMMON_H__
#define __PIOP_COMMON_H__

#include "mqom3_parameters.h"
#include "fields.h"
#include "xof.h"
#include "common.h"
#include "domain_separation.h"

/* Gamma^{[e]} = XOF_8(com[e])  - per-execution batching challenge (v3)
 * Shared across all PIOP variants (default, memopt, bitslice). */
static inline int ExpandBatchingChallenge(const uint8_t com[MQOM3_PARAM_DIGEST_SIZE], field_ext_elt Gamma[MQOM3_PARAM_ETA][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)]) {
	int ret = -1;
	uint32_t i;
	uint8_t stream[MQOM3_PARAM_ETA * BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
	xof_context xof_ctx = { 0 };

	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) DOMAIN_SEP_GAMMA, 1);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, com, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	ret = xof_squeeze(&xof_ctx, stream, MQOM3_PARAM_ETA * BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU));
	ERR(ret, err);
	for (i = 0; i < MQOM3_PARAM_ETA; i++) {
		field_ext_parse(&stream[i * BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU, Gamma[i]);
	}

	ret = 0;
err:
	xof_clean_ctx(&xof_ctx);
	return ret;
}

/* Same as ExpandBatchingChallenge, for memopt callers only: squeezes row by
 * row instead of materializing the full ETA-row stream at once. A Keccak
 * sponge squeeze phase is sequential, so ETA calls of BYTE_SIZE_FIELD_EXT(M/MU)
 * bytes produce the exact same byte stream as one call of
 * ETA*BYTE_SIZE_FIELD_EXT(M/MU) bytes - this is a stack/cycle tradeoff
 * (measured: -424B stack in RecomputePAlpha_partial_memopt, but ~+17% Sign
 * cycles under PIOP_BITSLICE where grinding calls this many times per trial),
 * so it is kept as a separate function rather than folded into
 * ExpandBatchingChallenge above, to leave the default/bitslice paths
 * untouched. */
static inline int ExpandBatchingChallenge_memopt(const uint8_t com[MQOM3_PARAM_DIGEST_SIZE], field_ext_elt Gamma[MQOM3_PARAM_ETA][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)]) {
	int ret = -1;
	uint32_t i;
	uint8_t row[BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
	xof_context xof_ctx = { 0 };

	ret = xof_init(&xof_ctx);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, (const uint8_t*) DOMAIN_SEP_GAMMA, 1);
	ERR(ret, err);
	ret = xof_update(&xof_ctx, com, MQOM3_PARAM_DIGEST_SIZE);
	ERR(ret, err);
	for (i = 0; i < MQOM3_PARAM_ETA; i++) {
		ret = xof_squeeze(&xof_ctx, row, BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU));
		ERR(ret, err);
		field_ext_parse(row, MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU, Gamma[i]);
	}

	ret = 0;
err:
	xof_clean_ctx(&xof_ctx);
	return ret;
}

/* Batched ExpandBatchingChallenge for n executions using xof_x8/xof_x4/scalar cascade.
 * com_ptrs[i] -> DIGEST_SIZE bytes for execution i.
 * Gamma_out[i] -> ETA * FIELD_EXT_PACKING(M/MU) output matrix for execution i. */
static inline int ExpandBatchingChallenge_batch(
    uint32_t n,
    const uint8_t * const com_ptrs[],
    field_ext_elt Gamma_out[][MQOM3_PARAM_ETA][FIELD_EXT_PACKING(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)]) {
	int ret = 0;
	uint32_t e = 0;
#if defined(USE_XOF_X8) || defined(USE_XOF_X4)
	uint32_t i, z;
#endif

#if defined(USE_XOF_X8)
	for (; (e + 8) <= n; e += 8) {
		xof_context_x8 ctx_x8 = { 0 };
		uint8_t streams[8][MQOM3_PARAM_ETA * BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
		const uint8_t *dsep_[8] = { (const uint8_t*) DOMAIN_SEP_GAMMA, (const uint8_t*) DOMAIN_SEP_GAMMA, (const uint8_t*) DOMAIN_SEP_GAMMA, (const uint8_t*) DOMAIN_SEP_GAMMA,
		                             (const uint8_t*) DOMAIN_SEP_GAMMA, (const uint8_t*) DOMAIN_SEP_GAMMA, (const uint8_t*) DOMAIN_SEP_GAMMA, (const uint8_t*) DOMAIN_SEP_GAMMA };
		const uint8_t *com_[8] = {
			com_ptrs[e + 0], com_ptrs[e + 1], com_ptrs[e + 2], com_ptrs[e + 3],
			com_ptrs[e + 4], com_ptrs[e + 5], com_ptrs[e + 6], com_ptrs[e + 7]
		};
		uint8_t *out_[8] = {
			streams[0], streams[1], streams[2], streams[3],
			streams[4], streams[5], streams[6], streams[7]
		};
		int r = xof_init_x8(&ctx_x8);
		if (!r) { r = xof_update_x8(&ctx_x8, dsep_, 1); }
		if (!r) { r = xof_update_x8(&ctx_x8, com_, MQOM3_PARAM_DIGEST_SIZE); }
		if (!r) { r = xof_squeeze_x8(&ctx_x8, out_, MQOM3_PARAM_ETA * BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)); }
		xof_clean_ctx_x8(&ctx_x8);
		if (r) { ret = r; goto err; }
		for (z = 0; z < 8; z++) {
			for (i = 0; i < MQOM3_PARAM_ETA; i++) {
				field_ext_parse(&streams[z][i * BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU, Gamma_out[e + z][i]);
			}
		}
	}
#endif
#if defined(USE_XOF_X4)
	for (; (e + 4) <= n; e += 4) {
		xof_context_x4 ctx_x4 = { 0 };
		uint8_t streams[4][MQOM3_PARAM_ETA * BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)];
		const uint8_t *dsep_[4] = { (const uint8_t*) DOMAIN_SEP_GAMMA, (const uint8_t*) DOMAIN_SEP_GAMMA, (const uint8_t*) DOMAIN_SEP_GAMMA, (const uint8_t*) DOMAIN_SEP_GAMMA };
		const uint8_t *com_[4] = {
			com_ptrs[e + 0], com_ptrs[e + 1], com_ptrs[e + 2], com_ptrs[e + 3]
		};
		uint8_t *out_[4] = { streams[0], streams[1], streams[2], streams[3] };
		int r = xof_init_x4(&ctx_x4);
		if (!r) { r = xof_update_x4(&ctx_x4, dsep_, 1); }
		if (!r) { r = xof_update_x4(&ctx_x4, com_, MQOM3_PARAM_DIGEST_SIZE); }
		if (!r) { r = xof_squeeze_x4(&ctx_x4, out_, MQOM3_PARAM_ETA * BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)); }
		xof_clean_ctx_x4(&ctx_x4);
		if (r) { ret = r; goto err; }
		for (z = 0; z < 4; z++) {
			for (i = 0; i < MQOM3_PARAM_ETA; i++) {
				field_ext_parse(&streams[z][i * BYTE_SIZE_FIELD_EXT(MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU)], MQOM3_PARAM_MQ_M / MQOM3_PARAM_MU, Gamma_out[e + z][i]);
			}
		}
	}
#endif
	for (; e < n; e++) {
		ret = ExpandBatchingChallenge(com_ptrs[e], Gamma_out[e]);
		ERR(ret, err);
	}
	return 0;
err:
	return ret;
}

#endif /* __PIOP_COMMON_H__ */
