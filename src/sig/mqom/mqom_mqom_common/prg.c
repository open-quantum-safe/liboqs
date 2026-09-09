#include "prg.h"

/* Fill a lambda-byte counter block: Bits_lambda(idx) little-endian, zero-padded. */
#define PRG_SET_CTR(ctr_arr, idx) do { \
	memset((ctr_arr), 0, MQOM3_PARAM_SEED_EQ_SIZE); \
	(ctr_arr)[0] = (uint8_t)((idx) & 0xff); \
	(ctr_arr)[1] = (uint8_t)(((idx) >> 8) & 0xff); \
	(ctr_arr)[2] = (uint8_t)(((idx) >> 16) & 0xff); \
	(ctr_arr)[3] = (uint8_t)(((idx) >> 24) & 0xff); \
} while(0)


/* In-place ECB helper: every PRG call site below encrypts a buffer onto itself.
 * Nothing in the enc_* contract promises that in == out is supported - it happens
 * to hold for all in-tree backends, but RIJNDAEL_EXTERNAL, liboqs and mupq
 * delegate to third-party code that is not bound by it. The default assumption
 * is that the backend DOES support aliasing; an integrator whose backend does
 * not must say so explicitly by building with RIJNDAEL_NO_INPUTS_ALIASING=1.
 * The Makefile only relays that flag - it does not, and cannot, deduce it from
 * MQOM3_FOR_LIBOQS / MQOM3_FOR_MUPQ / RIJNDAEL_EXTERNAL. When set, we stage
 * through a bounded scratch buffer, which keeps the batching instead of
 * degrading to one block at a time. The default path is unchanged and costs
 * nothing. */
#ifndef PRG_ECB_INPLACE_CHUNK
#define PRG_ECB_INPLACE_CHUNK 8
#endif
/* Overridable, and 0 would size scratch[] to zero and make the staging loop below
 * spin forever on off += PRG_ECB_INPLACE_CHUNK. */
#if PRG_ECB_INPLACE_CHUNK < 1
#error "PRG_ECB_INPLACE_CHUNK must be at least 1"
#endif

static inline int PRG_enc_ecb_inplace(const enc_ctx_pub_ecb *ctx, uint32_t nblocks, uint8_t *buf) {
#ifdef RIJNDAEL_NO_INPUTS_ALIASING
	uint8_t scratch[PRG_ECB_INPLACE_CHUNK * MQOM3_PARAM_SEED_EQ_SIZE];
	uint32_t off;
	int ret = 0;

	for (off = 0; off < nblocks; off += PRG_ECB_INPLACE_CHUNK) {
		uint32_t m = (nblocks - off);
		if (m > PRG_ECB_INPLACE_CHUNK) {
			m = PRG_ECB_INPLACE_CHUNK;
		}
		memcpy(scratch, buf + ((size_t) off * MQOM3_PARAM_SEED_EQ_SIZE),
		       (size_t) m * MQOM3_PARAM_SEED_EQ_SIZE);
		ret = enc_encrypt_pub_ecb(ctx, m, scratch,
		                          buf + ((size_t) off * MQOM3_PARAM_SEED_EQ_SIZE));
		if (ret) {
			break;
		}
	}
	mqom_cleanse(scratch, sizeof(scratch));
	return ret;
#else
	return enc_encrypt_pub_ecb(ctx, nblocks, buf, buf);
#endif
}

/* v3 spec PRG(seed, n_bytes): key=seed, ptx=Bits_lambda(counter), Enc = native cipher.
 * Full blocks: counters written directly into out_data, encrypted in-place (no malloc). */
int PRG(const uint8_t seed[MQOM3_PARAM_SEED_EQ_SIZE], uint32_t nbytes, uint8_t *out_data) {
	int ret = -1;
	uint32_t nblocks, leftover, k;
	enc_ctx_pub_ecb DECL_VAR(ctx);

	nblocks = nbytes / MQOM3_PARAM_SEED_EQ_SIZE;
	leftover = nbytes % MQOM3_PARAM_SEED_EQ_SIZE;

	ret = enc_key_sched_pub_ecb(&ctx, seed);
	ERR(ret, err);

	/* Full blocks: write counters into out_data, encrypt in-place. */
	if (nblocks > 0) {
		for (k = 0; k < nblocks; k++) {
			PRG_SET_CTR(&out_data[k * MQOM3_PARAM_SEED_EQ_SIZE], k);
		}
		ret = PRG_enc_ecb_inplace(&ctx, nblocks, out_data);
		ERR(ret, err);
	}

	/* Partial last block: needs a separate buffer (out_data has only leftover bytes there). */
	if (leftover) {
		uint8_t tail[MQOM3_PARAM_SEED_EQ_SIZE];
		PRG_SET_CTR(tail, nblocks);
		ret = PRG_enc_ecb_inplace(&ctx, 1, tail);
		ERR(ret, err);
		memcpy(&out_data[nblocks * MQOM3_PARAM_SEED_EQ_SIZE], tail, leftover);
	}

	ret = 0;
err:
	enc_clean_ctx_pub_ecb(&ctx);
	return ret;
}

/* v3 spec PRG(seed, n_bytes): key=seed, ptx=Bits_lambda(counter), Enc = native cipher.
 * Memory-efficient variant: generate bytes [start_nbytes, start_nbytes+nbytes).
 * in_out_leftover holds the last computed block for reuse on the next call.
 * Full blocks written directly into out_data and encrypted in-place via ECB. */
int PRG_memopt(const uint8_t seed[MQOM3_PARAM_SEED_EQ_SIZE], uint32_t start_nbytes, uint32_t nbytes, uint8_t *out_data, uint8_t in_out_leftover[MQOM3_PARAM_SEED_EQ_SIZE]) {
	int ret = -1;
	uint32_t start_block, end_block, treated_bytes, n_full, k;
	uint32_t first_enc_block;
	enc_ctx_pub_ecb DECL_VAR(ctx);

	ret = enc_key_sched_pub_ecb(&ctx, seed);
	ERR(ret, err);

	start_block = (start_nbytes / MQOM3_PARAM_SEED_EQ_SIZE);
	end_block   = ((start_nbytes + nbytes) / MQOM3_PARAM_SEED_EQ_SIZE);
	treated_bytes = 0;
	first_enc_block = start_block;

	/* Handle partial start block: use cached leftover from previous call */
	if ((start_nbytes % MQOM3_PARAM_SEED_EQ_SIZE) && (start_block < end_block)) {
		uint32_t to_copy = MQOM3_PARAM_SEED_EQ_SIZE - (start_nbytes % MQOM3_PARAM_SEED_EQ_SIZE);
		memcpy(&out_data[0], &in_out_leftover[start_nbytes % MQOM3_PARAM_SEED_EQ_SIZE], to_copy);
		treated_bytes += to_copy;
		first_enc_block = start_block + 1;
	}

	/* Full blocks: write counters into out_data, encrypt in-place. */
	n_full = end_block - first_enc_block;
	if (n_full > 0) {
		for (k = 0; k < n_full; k++) {
			PRG_SET_CTR(&out_data[treated_bytes + k * MQOM3_PARAM_SEED_EQ_SIZE], first_enc_block + k);
		}
		ret = PRG_enc_ecb_inplace(&ctx, n_full, out_data + treated_bytes);
		ERR(ret, err);
	}
	treated_bytes += n_full * MQOM3_PARAM_SEED_EQ_SIZE;

	/* Partial end block: encrypt into leftover cache in-place, copy tail */
	if ((nbytes - treated_bytes) % MQOM3_PARAM_SEED_EQ_SIZE) {
		PRG_SET_CTR(in_out_leftover, end_block);
		ret = PRG_enc_ecb_inplace(&ctx, 1, in_out_leftover);
		ERR(ret, err);
		if (treated_bytes == 0) {
			memcpy(&out_data[0], &in_out_leftover[start_nbytes % MQOM3_PARAM_SEED_EQ_SIZE], nbytes % MQOM3_PARAM_SEED_EQ_SIZE);
		} else {
			memcpy(&out_data[treated_bytes], in_out_leftover, (nbytes - treated_bytes) % MQOM3_PARAM_SEED_EQ_SIZE);
		}
	}

	ret = 0;
err:
	enc_clean_ctx_pub_ecb(&ctx);
	return ret;
}

/* PRG_memopt variant that reuses a pre-computed key schedule.
 * Identical to PRG_memopt except the enc_key_sched is skipped. */
int PRG_memopt_with_ctx(const enc_ctx_pub_ecb *prg_ctx, uint32_t start_nbytes, uint32_t nbytes, uint8_t *out_data, uint8_t in_out_leftover[MQOM3_PARAM_SEED_EQ_SIZE]) {
	int ret = -1;
	uint32_t start_block, end_block, treated_bytes, n_full, k;
	uint32_t first_enc_block;

	start_block = (start_nbytes / MQOM3_PARAM_SEED_EQ_SIZE);
	end_block   = ((start_nbytes + nbytes) / MQOM3_PARAM_SEED_EQ_SIZE);
	treated_bytes = 0;
	first_enc_block = start_block;

	/* Handle partial start block: use cached leftover from previous call */
	if ((start_nbytes % MQOM3_PARAM_SEED_EQ_SIZE) && (start_block < end_block)) {
		uint32_t to_copy = MQOM3_PARAM_SEED_EQ_SIZE - (start_nbytes % MQOM3_PARAM_SEED_EQ_SIZE);
		memcpy(&out_data[0], &in_out_leftover[start_nbytes % MQOM3_PARAM_SEED_EQ_SIZE], to_copy);
		treated_bytes += to_copy;
		first_enc_block = start_block + 1;
	}

	/* Full blocks: write counters into out_data, encrypt in-place. */
	n_full = end_block - first_enc_block;
	if (n_full > 0) {
		for (k = 0; k < n_full; k++) {
			PRG_SET_CTR(&out_data[treated_bytes + k * MQOM3_PARAM_SEED_EQ_SIZE], first_enc_block + k);
		}
		ret = PRG_enc_ecb_inplace(prg_ctx, n_full, out_data + treated_bytes);
		ERR(ret, err);
	}
	treated_bytes += n_full * MQOM3_PARAM_SEED_EQ_SIZE;

	/* Partial end block: encrypt into leftover cache in-place, copy tail */
	if ((nbytes - treated_bytes) % MQOM3_PARAM_SEED_EQ_SIZE) {
		PRG_SET_CTR(in_out_leftover, end_block);
		ret = PRG_enc_ecb_inplace(prg_ctx, 1, in_out_leftover);
		ERR(ret, err);
		if (treated_bytes == 0) {
			memcpy(&out_data[0], &in_out_leftover[start_nbytes % MQOM3_PARAM_SEED_EQ_SIZE], nbytes % MQOM3_PARAM_SEED_EQ_SIZE);
		} else {
			memcpy(&out_data[treated_bytes], in_out_leftover, (nbytes - treated_bytes) % MQOM3_PARAM_SEED_EQ_SIZE);
		}
	}

	ret = 0;
err:
	return ret;
}
