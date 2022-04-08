// SPDX-License-Identifier: Public domain

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <oqs/common.h>

#include <arm_neon.h>

#define PQC_AES128_STATESIZE 88
typedef struct {
	uint64_t sk_exp[PQC_AES128_STATESIZE];
} aes128ctx;

// From crypto_core/aes128encrypt/dolbeau/armv8crypto
static inline void aes128_armv8_encrypt(const unsigned char *rkeys, const unsigned char *n, unsigned char *out) {
	uint8x16_t temp = vld1q_u8(n);
	//int i;

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

	temp = vaeseq_u8(temp, vld1q_u8((rkeys + 144)));
	temp = veorq_u8(temp, vld1q_u8((rkeys + 160)));

	vst1q_u8(out, temp);
}

void oqs_aes128_enc_sch_block_armv8(const uint8_t *plaintext, const void *_schedule, uint8_t *ciphertext) {
	const unsigned char *schedule = (const unsigned char *) _schedule;
	aes128_armv8_encrypt(schedule, plaintext, ciphertext);
}

void oqs_aes128_ecb_enc_sch_armv8(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	assert(plaintext_len % 16 == 0);
	const aes128ctx *ctx = (const aes128ctx *) schedule;

	for (size_t block = 0; block < plaintext_len / 16; block++) {
		oqs_aes128_enc_sch_block_armv8(plaintext + (16 * block), (const void *) ctx->sk_exp, ciphertext + (16 * block));
	}
}
