// SPDX-License-Identifier: Public domain
// Based on public domain code by Romain Dolbeau
// http://dolbeau.name/dolbeau/crypto/crypto.html


#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <oqs/common.h>

#include <wmmintrin.h>

#define AES_BLOCKBYTES 16

typedef struct {
	__m128i sk_exp[15];
	uint8_t iv[AES_BLOCKBYTES];
} aes256ctx;

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
#define BE_TO_UINT32(n) (uint32_t)((((uint8_t *) &(n))[0] << 24) | (((uint8_t *) &(n))[1] << 16) | (((uint8_t *) &(n))[2] << 8) | (((uint8_t *) &(n))[3] << 0))

// From crypto_core/aes256encrypt/dolbeau/aesenc-int
static inline void aes256ni_setkey_encrypt(const unsigned char *key, __m128i rkeys[15]) {
	__m128i key0 = _mm_loadu_si128((const __m128i *)(key + 0));
	__m128i key1 = _mm_loadu_si128((const __m128i *)(key + 16));
	__m128i temp0, temp1, temp2, temp4;
	int idx = 0;

	rkeys[idx++] = key0;
	temp0 = key0;
	temp2 = key1;

	/* blockshift-based block by Cedric Bourrasset & Romain Dolbeau */
#define BLOCK1(IMM)                                \
    temp1 = _mm_aeskeygenassist_si128(temp2, IMM); \
    rkeys[idx++] = temp2;                          \
    temp4 = _mm_slli_si128(temp0,4);               \
    temp0 = _mm_xor_si128(temp0,temp4);            \
    temp4 = _mm_slli_si128(temp0,8);               \
    temp0 = _mm_xor_si128(temp0,temp4);            \
    temp1 = _mm_shuffle_epi32(temp1,0xff);         \
    temp0 = _mm_xor_si128(temp0,temp1)

#define BLOCK2(IMM)                                \
    temp1 = _mm_aeskeygenassist_si128(temp0, IMM); \
    rkeys[idx++] = temp0;                          \
    temp4 = _mm_slli_si128(temp2,4);               \
    temp2 = _mm_xor_si128(temp2,temp4);            \
    temp4 = _mm_slli_si128(temp2,8);               \
    temp2 = _mm_xor_si128(temp2,temp4);            \
    temp1 = _mm_shuffle_epi32(temp1,0xaa);         \
    temp2 = _mm_xor_si128(temp2,temp1)

	BLOCK1(0x01);
	BLOCK2(0x01);

	BLOCK1(0x02);
	BLOCK2(0x02);

	BLOCK1(0x04);
	BLOCK2(0x04);

	BLOCK1(0x08);
	BLOCK2(0x08);

	BLOCK1(0x10);
	BLOCK2(0x10);

	BLOCK1(0x20);
	BLOCK2(0x20);

	BLOCK1(0x40);
	rkeys[idx++] = temp0;
}

void oqs_aes256_load_schedule_ni(const uint8_t *key, void **_schedule) {
	//*_schedule = malloc(15 * sizeof(__m128i));
	*_schedule = malloc(sizeof(aes256ctx));
	OQS_EXIT_IF_NULLPTR(*_schedule);
	assert(*_schedule != NULL);
	__m128i *schedule = ((aes256ctx *) *_schedule)->sk_exp;
	aes256ni_setkey_encrypt(key, schedule);
}

void oqs_aes256_load_nonce_ni(const uint8_t *iv, size_t iv_len, void **_schedule) {
	aes256ctx *ctx = *_schedule;
	if (iv_len == 12) {
		memcpy(ctx->iv, iv, 12);
		memset(&ctx->iv[12], 0, 4);
	} else if (iv_len == 16) {
		memcpy(ctx->iv, iv, 16);
	} else {
		exit(EXIT_FAILURE);
	}
}

void oqs_aes256_free_schedule_ni(void *schedule) {
	if (schedule != NULL) {
		//OQS_MEM_secure_free(schedule, 15 * sizeof(__m128i));
		OQS_MEM_secure_free(schedule, sizeof(aes256ctx));
	}
}

// From crypto_core/aes256encrypt/dolbeau/aesenc-int
static inline void aes256ni_encrypt(const __m128i rkeys[15], const unsigned char *n, unsigned char *out) {
	__m128i nv = _mm_loadu_si128((const __m128i *)n);
	__m128i temp = _mm_xor_si128(nv, rkeys[0]);
	temp = _mm_aesenc_si128(temp, rkeys[1]);
	temp = _mm_aesenc_si128(temp, rkeys[2]);
	temp = _mm_aesenc_si128(temp, rkeys[3]);
	temp = _mm_aesenc_si128(temp, rkeys[4]);
	temp = _mm_aesenc_si128(temp, rkeys[5]);
	temp = _mm_aesenc_si128(temp, rkeys[6]);
	temp = _mm_aesenc_si128(temp, rkeys[7]);
	temp = _mm_aesenc_si128(temp, rkeys[8]);
	temp = _mm_aesenc_si128(temp, rkeys[9]);
	temp = _mm_aesenc_si128(temp, rkeys[10]);
	temp = _mm_aesenc_si128(temp, rkeys[11]);
	temp = _mm_aesenc_si128(temp, rkeys[12]);
	temp = _mm_aesenc_si128(temp, rkeys[13]);
	temp = _mm_aesenclast_si128(temp, rkeys[14]);
	_mm_storeu_si128((__m128i *)(out), temp);
}

void oqs_aes256_enc_sch_block_ni(const uint8_t *plaintext, const void *_schedule, uint8_t *ciphertext) {
	const __m128i *schedule = ((const aes256ctx *) _schedule)->sk_exp;
	aes256ni_encrypt(schedule, plaintext, ciphertext);
}

void oqs_aes256_ecb_enc_sch_ni(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	assert(plaintext_len % 16 == 0);
	for (size_t block = 0; block < plaintext_len / 16; block++) {
		oqs_aes256_enc_sch_block_ni(plaintext + (16 * block), schedule, ciphertext + (16 * block));
	}
}

void oqs_aes256_ctr_enc_sch_ivinit_ni(void *schedule, uint8_t *out, size_t out_len) {
	uint32_t ctr;
	uint32_t ctr_be;
	uint8_t *block = ((aes256ctx *) schedule)->iv;

	memcpy(&ctr_be, &block[12], 4);
	ctr = BE_TO_UINT32(ctr_be);

	while (out_len >= 16) {
		ctr_be = UINT32_TO_BE(ctr);
		memcpy(&block[12], (uint8_t *) &ctr_be, 4);
		oqs_aes256_enc_sch_block_ni(block, schedule, out);
		out += 16;
		out_len -= 16;
		ctr++;
	}
	if (out_len > 0) {
		uint8_t tmp[16];
		ctr_be = UINT32_TO_BE(ctr);
		memcpy(&block[12], (uint8_t *) &ctr_be, 4);
		oqs_aes256_enc_sch_block_ni(block, schedule, tmp);
		memcpy(out, tmp, out_len);
	}
}

void oqs_aes256_ctr_enc_sch_ni(const uint8_t *iv, const size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
	uint8_t block[16];
	uint32_t ctr;
	uint32_t ctr_be;

	memcpy(block, iv, 12);
	if (iv_len == 12) {
		ctr = 0;
	} else if (iv_len == 16) {
		memcpy(&ctr_be, &iv[12], 4);
		ctr = BE_TO_UINT32(ctr_be);
	} else {
		exit(EXIT_FAILURE);
	}
	while (out_len >= 16) {
		ctr_be = UINT32_TO_BE(ctr);
		memcpy(&block[12], (uint8_t *) &ctr_be, 4);
		oqs_aes256_enc_sch_block_ni(block, schedule, out);
		out += 16;
		out_len -= 16;
		ctr++;
	}
	if (out_len > 0) {
		uint8_t tmp[16];
		ctr_be = UINT32_TO_BE(ctr);
		memcpy(&block[12], (uint8_t *) &ctr_be, 4);
		oqs_aes256_enc_sch_block_ni(block, schedule, tmp);
		memcpy(out, tmp, out_len);
	}
}
