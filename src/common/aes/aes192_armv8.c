// SPDX-License-Identifier: Public domain

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <oqs/common.h>

#include <arm_neon.h>

typedef struct {
	uint32_t sk_exp[52];
	uint8_t iv[16];
} aes192ctx_nobitslice;

static inline void aes192_armv8_encrypt(const unsigned char *rkeys, const unsigned char *n, unsigned char *out) {
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
	temp = veorq_u8(temp, vld1q_u8(rkeys + 192));

	vst1q_u8(out, temp);
}

// Four blocks at a time, with the AESE/AESMC chains interleaved so that the
// latency of one chain is hidden behind the other three.
static inline void aes192_armv8_encrypt_x4(const unsigned char *rkeys, const unsigned char *in, unsigned char *out) {
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

	rk = vld1q_u8(rkeys + 176);
	t0 = vaeseq_u8(t0, rk);
	t1 = vaeseq_u8(t1, rk);
	t2 = vaeseq_u8(t2, rk);
	t3 = vaeseq_u8(t3, rk);
	rk = vld1q_u8(rkeys + 192);
	t0 = veorq_u8(t0, rk);
	t1 = veorq_u8(t1, rk);
	t2 = veorq_u8(t2, rk);
	t3 = veorq_u8(t3, rk);

	vst1q_u8(out, t0);
	vst1q_u8(out + 16, t1);
	vst1q_u8(out + 32, t2);
	vst1q_u8(out + 48, t3);
}

void oqs_aes192_enc_sch_block_armv8(const uint8_t *plaintext, const void *_schedule, uint8_t *ciphertext) {
	const unsigned char *schedule = (const unsigned char *) ((const aes192ctx_nobitslice *) _schedule)->sk_exp;
	aes192_armv8_encrypt(schedule, plaintext, ciphertext);
}

void oqs_aes192_ecb_enc_sch_armv8(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	assert(plaintext_len % 16 == 0);
	const unsigned char *rkeys = (const unsigned char *) ((const aes192ctx_nobitslice *) schedule)->sk_exp;
	const size_t nblocks = plaintext_len / 16;
	size_t block = 0;

	for (; block + 4 <= nblocks; block += 4) {
		aes192_armv8_encrypt_x4(rkeys, plaintext + (16 * block), ciphertext + (16 * block));
	}
	for (; block < nblocks; block++) {
		oqs_aes192_enc_sch_block_armv8(plaintext + (16 * block), schedule, ciphertext + (16 * block));
	}
}
