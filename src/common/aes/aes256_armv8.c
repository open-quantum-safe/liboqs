// SPDX-License-Identifier: Public domain

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <oqs/common.h>

#include <arm_neon.h>

typedef struct {
	uint32_t sk_exp[60];
	uint8_t iv[16];
} aes256ctx_nobitslice;

void oqs_aes256_load_iv_armv8(const uint8_t *iv, size_t iv_len, void *_schedule) {
	aes256ctx_nobitslice *ctx = _schedule;
	if (iv_len == 12) {
		memcpy(ctx->iv, iv, 12);
		memset(&ctx->iv[12], 0, 4);
	} else if (iv_len == 16) {
		memcpy(ctx->iv, iv, 16);
	} else {
		exit(EXIT_FAILURE);
	}
}

// From crypto_core/aes256encrypt/dolbeau/armv8crypto
static inline void aes256_armv8_encrypt(const unsigned char *rkeys, const unsigned char *n, unsigned char *out) {
	uint8x16_t temp = vld1q_u8(n);

	/*
	    In ARMv8+crypto, the AESE instruction does the 'AddRoundKey' first then SubBytes and ShiftRows.
	    The AESMC instruction does the MixColumns.
	    So instead of a single XOR of the first round key before the rounds,
	    we end up having a single XOR of the last round key after the rounds.
	*/

	temp = vaeseq_u8(temp, vld1q_u8(rkeys));
	temp = vaesmcq_u8(temp);
	temp = vaeseq_u8(temp, vld1q_u8(rkeys + 16));
	temp = vaesmcq_u8(temp);
	temp = vaeseq_u8(temp, vld1q_u8(rkeys + 32));
	temp = vaesmcq_u8(temp);
	temp = vaeseq_u8(temp, vld1q_u8(rkeys + 48));
	temp = vaesmcq_u8(temp);
	temp = vaeseq_u8(temp, vld1q_u8(rkeys + 64));
	temp = vaesmcq_u8(temp);
	temp = vaeseq_u8(temp, vld1q_u8(rkeys + 80));
	temp = vaesmcq_u8(temp);
	temp = vaeseq_u8(temp, vld1q_u8(rkeys + 96));
	temp = vaesmcq_u8(temp);
	temp = vaeseq_u8(temp, vld1q_u8(rkeys + 112));
	temp = vaesmcq_u8(temp);
	temp = vaeseq_u8(temp, vld1q_u8(rkeys + 128));
	temp = vaesmcq_u8(temp);
	temp = vaeseq_u8(temp, vld1q_u8(rkeys + 144));
	temp = vaesmcq_u8(temp);
	temp = vaeseq_u8(temp, vld1q_u8(rkeys + 160));
	temp = vaesmcq_u8(temp);
	temp = vaeseq_u8(temp, vld1q_u8(rkeys + 176));
	temp = vaesmcq_u8(temp);
	temp = vaeseq_u8(temp, vld1q_u8(rkeys + 192));
	temp = vaesmcq_u8(temp);

	temp = vaeseq_u8(temp, vld1q_u8(rkeys + 208));
	temp = veorq_u8(temp, vld1q_u8((rkeys + 224)));

	vst1q_u8(out, temp);
}

// Four blocks at a time, with the AESE/AESMC chains interleaved so that the
// latency of one chain is hidden behind the other three.
static inline void aes256_armv8_encrypt_x4(const unsigned char *rkeys, const unsigned char *in, unsigned char *out) {
	uint8x16_t t0 = vld1q_u8(in);
	uint8x16_t t1 = vld1q_u8(in + 16);
	uint8x16_t t2 = vld1q_u8(in + 32);
	uint8x16_t t3 = vld1q_u8(in + 48);
	uint8x16_t rk;

#define AESEMCX4(OFFSET) \
    rk = vld1q_u8(rkeys + (OFFSET)); \
    t0 = vaesmcq_u8(vaeseq_u8(t0, rk)); \
    t1 = vaesmcq_u8(vaeseq_u8(t1, rk)); \
    t2 = vaesmcq_u8(vaeseq_u8(t2, rk)); \
    t3 = vaesmcq_u8(vaeseq_u8(t3, rk))

	AESEMCX4(0);
	AESEMCX4(16);
	AESEMCX4(32);
	AESEMCX4(48);
	AESEMCX4(64);
	AESEMCX4(80);
	AESEMCX4(96);
	AESEMCX4(112);
	AESEMCX4(128);
	AESEMCX4(144);
	AESEMCX4(160);
	AESEMCX4(176);
	AESEMCX4(192);

	rk = vld1q_u8(rkeys + 208);
	t0 = vaeseq_u8(t0, rk);
	t1 = vaeseq_u8(t1, rk);
	t2 = vaeseq_u8(t2, rk);
	t3 = vaeseq_u8(t3, rk);
	rk = vld1q_u8(rkeys + 224);
	t0 = veorq_u8(t0, rk);
	t1 = veorq_u8(t1, rk);
	t2 = veorq_u8(t2, rk);
	t3 = veorq_u8(t3, rk);

	vst1q_u8(out, t0);
	vst1q_u8(out + 16, t1);
	vst1q_u8(out + 32, t2);
	vst1q_u8(out + 48, t3);
}

void oqs_aes256_enc_sch_block_armv8(const uint8_t *plaintext, const void *_schedule, uint8_t *ciphertext) {
	const unsigned char *schedule = (const unsigned char *) ((const aes256ctx_nobitslice *) _schedule)->sk_exp;
	aes256_armv8_encrypt(schedule, plaintext, ciphertext);
}

void oqs_aes256_ecb_enc_sch_armv8(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	assert(plaintext_len % 16 == 0);
	const unsigned char *rkeys = (const unsigned char *) ((const aes256ctx_nobitslice *) schedule)->sk_exp;
	const size_t nblocks = plaintext_len / 16;
	size_t block = 0;

	for (; block + 4 <= nblocks; block += 4) {
		aes256_armv8_encrypt_x4(rkeys, plaintext + (16 * block), ciphertext + (16 * block));
	}
	for (; block < nblocks; block++) {
		oqs_aes256_enc_sch_block_armv8(plaintext + (16 * block), schedule, ciphertext + (16 * block));
	}
}

static uint32_t UINT32_TO_BE(const uint32_t x) {
	union {
		uint32_t val;
		uint8_t bytes[4];
	} y;
	y.bytes[0] = (x >> 24) & 0xFF;
	y.bytes[1] = (x >> 16) & 0xFF;
	y.bytes[2] = (x >> 8) & 0xFF;
	y.bytes[3] = x & 0xFF;
	return y.val;
}
#define BE_TO_UINT32(n) (((uint32_t)((uint8_t *) &(n))[0] << 24) | ((uint32_t)((uint8_t *) &(n))[1] << 16) | ((uint32_t)((uint8_t *) &(n))[2] << 8) | ((uint32_t)((uint8_t *) &(n))[3] << 0))

/* Fills four consecutive counter blocks. Bytes 0 to 11 are the nonce and
 * bytes 12 to 15 the big-endian 32-bit counter, laid out exactly as the
 * single-block path lays them out; the counter wraps modulo 2^32 as there. */
static inline void aes256_armv8_ctr_blocks_x4(uint8_t blocks[64], const uint8_t *nonce, uint32_t ctr) {
	for (size_t i = 0; i < 4; i++) {
		uint32_t ctr_be = UINT32_TO_BE(ctr + (uint32_t) i);
		memcpy(blocks + 16 * i, nonce, 12);
		memcpy(blocks + 16 * i + 12, (uint8_t *) &ctr_be, 4);
	}
}

void oqs_aes256_ctr_enc_sch_upd_blks_armv8(void *schedule, uint8_t *out, size_t out_blks) {
	aes256ctx_nobitslice *ctx = (aes256ctx_nobitslice *) schedule;
	const unsigned char *rkeys = (const unsigned char *) ctx->sk_exp;
	uint8_t *block = ctx->iv;
	uint8_t blocks[64];
	uint32_t ctr;
	uint32_t ctr_be;
	memcpy(&ctr_be, &block[12], 4);
	ctr = BE_TO_UINT32(ctr_be);
	while (out_blks >= 4) {
		aes256_armv8_ctr_blocks_x4(blocks, block, ctr);
		aes256_armv8_encrypt_x4(rkeys, blocks, out);
		out += 64;
		out_blks -= 4;
		ctr += 4;
	}
	while (out_blks >= 1) {
		ctr_be = UINT32_TO_BE(ctr);
		memcpy(&block[12], (uint8_t *) &ctr_be, 4);
		aes256_armv8_encrypt(rkeys, block, out);
		out += 16;
		out_blks--;
		ctr++;
	}
	/* Leave the counter of the next block in the schedule for the next call. */
	ctr_be = UINT32_TO_BE(ctr);
	memcpy(&block[12], (uint8_t *) &ctr_be, 4);
}

void oqs_aes256_ctr_enc_sch_armv8(const uint8_t *iv, const size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
	const unsigned char *rkeys = (const unsigned char *) ((const aes256ctx_nobitslice *) schedule)->sk_exp;
	uint8_t blocks[64];
	uint32_t ctr;
	uint32_t ctr_be;
	if (iv_len == 12) {
		ctr = 0;
	} else if (iv_len == 16) {
		memcpy(&ctr_be, &iv[12], 4);
		ctr = BE_TO_UINT32(ctr_be);
	} else {
		exit(EXIT_FAILURE);
	}
	while (out_len >= 64) {
		aes256_armv8_ctr_blocks_x4(blocks, iv, ctr);
		aes256_armv8_encrypt_x4(rkeys, blocks, out);
		out += 64;
		out_len -= 64;
		ctr += 4;
	}
	memcpy(blocks, iv, 12);
	while (out_len >= 16) {
		ctr_be = UINT32_TO_BE(ctr);
		memcpy(&blocks[12], (uint8_t *) &ctr_be, 4);
		aes256_armv8_encrypt(rkeys, blocks, out);
		out += 16;
		out_len -= 16;
		ctr++;
	}
	if (out_len > 0) {
		uint8_t tmp[16];
		ctr_be = UINT32_TO_BE(ctr);
		memcpy(&blocks[12], (uint8_t *) &ctr_be, 4);
		aes256_armv8_encrypt(rkeys, blocks, tmp);
		memcpy(out, tmp, out_len);
	}
}
