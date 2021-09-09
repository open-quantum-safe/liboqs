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


#define FSbData                                         \
  {                                                     \
    0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5,     \
    0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,     \
    0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0,     \
    0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,     \
    0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC,     \
    0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,     \
    0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A,     \
    0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,     \
    0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0,     \
    0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,     \
    0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B,     \
    0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,     \
    0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85,     \
    0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,     \
    0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5,     \
    0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,     \
    0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17,     \
    0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,     \
    0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88,     \
    0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,     \
    0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C,     \
    0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,     \
    0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9,     \
    0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,     \
    0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6,     \
    0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,     \
    0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E,     \
    0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,     \
    0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94,     \
    0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,     \
    0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68,     \
    0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16      \
  }

static unsigned int FSb[256] = FSbData;
#undef FSbData

#define f_FSb_32__1(x)  ((FSb[((x) >> 24) &0xFF] << 24) ^ \
                         (FSb[((x) >> 16) &0xFF] << 16))

#define f_FSb_32__2(x)  ((FSb[((x) >>  8) &0xFF] <<  8 ) ^ \
                         (FSb[((x)      ) &0xFF] & 0xFF))



static inline unsigned int rotr(const unsigned int x, const unsigned int n) {
	unsigned int r;
	r = ((x >> n) | (x << (32 - n)));
	return r;
}
static inline unsigned int rotl(const unsigned int x, const unsigned int n) {
	unsigned int r;
	r = ((x << n) | (x >> (32 - n)));
	return r;
}

// From crypto_core/aes128encrypt/dolbeau/armv8crypto
static inline void aes128_armv8_keysched(const unsigned int key[], unsigned int *aes_edrk) {
	unsigned int i = 0;
	unsigned int rotl_aes_edrk;
	unsigned int tmp8, tmp9, tmp10, tmp11;
	unsigned int temp_lds;
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
	unsigned int round = 0x01000000;
#else
	unsigned int round = 0x00000001;
#endif

	tmp8 = (key[0]);
	aes_edrk[0] = tmp8;
	tmp9 = (key[1]);
	aes_edrk[1] = tmp9;
	tmp10 = (key[2]);
	aes_edrk[2] = tmp10;
	tmp11 = (key[3]);
	aes_edrk[3] = tmp11;

	for ( i = 4; i < 36; /* i += 4 */ ) {
#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
		rotl_aes_edrk = rotl(tmp11, 8);
#else
		rotl_aes_edrk = rotr(tmp11, 8);
#endif
		temp_lds = f_FSb_32__1(rotl_aes_edrk) ^ f_FSb_32__2(rotl_aes_edrk);

		tmp8 = tmp8 ^ round ^ temp_lds;
		round = round << 1;

		aes_edrk[i++] = tmp8;
		tmp9 = tmp9 ^ tmp8;
		aes_edrk[i++] = tmp9;
		tmp10 = tmp10 ^ tmp9;
		aes_edrk[i++] = tmp10;
		tmp11 = tmp11 ^ tmp10;
		aes_edrk[i++] = tmp11;
	}

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
	round = 0x1B000000;
	rotl_aes_edrk = rotl(tmp11, 8);
#else
	round = 0x0000001B;
	rotl_aes_edrk = rotr(tmp11, 8);
#endif
	temp_lds = f_FSb_32__1(rotl_aes_edrk) ^ f_FSb_32__2(rotl_aes_edrk);

	tmp8 = tmp8 ^ round ^ temp_lds;

	aes_edrk[i++] = tmp8;
	tmp9 = tmp9 ^ tmp8;
	aes_edrk[i++] = tmp9;
	tmp10 = tmp10 ^ tmp9;
	aes_edrk[i++] = tmp10;
	tmp11 = tmp11 ^ tmp10;
	aes_edrk[i++] = tmp11;

#if defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
	round = 0x36000000;
	rotl_aes_edrk = rotl(tmp11, 8);
#else
	round = 0x00000036;
	rotl_aes_edrk = rotr(tmp11, 8);
#endif
	temp_lds = f_FSb_32__1(rotl_aes_edrk) ^ f_FSb_32__2(rotl_aes_edrk);

	tmp8 = tmp8 ^ round ^ temp_lds;

	aes_edrk[i++] = tmp8;
	tmp9 = tmp9 ^ tmp8;
	aes_edrk[i++] = tmp9;
	tmp10 = tmp10 ^ tmp9;
	aes_edrk[i++] = tmp10;
	tmp11 = tmp11 ^ tmp10;
	aes_edrk[i++] = tmp11;
}


void oqs_aes128_load_schedule_armv8(const uint8_t *key, void **_schedule) {
	*_schedule = malloc(44 * sizeof(int));
	assert(*_schedule != NULL);
	unsigned int *schedule = (unsigned int *) *_schedule;
	aes128_armv8_keysched((const unsigned int *) key, schedule);
}

void oqs_aes128_free_schedule_armv8(void *schedule) {
	if (schedule != NULL) {
		OQS_MEM_secure_free(schedule, 44 * sizeof(int));
	}
}


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
