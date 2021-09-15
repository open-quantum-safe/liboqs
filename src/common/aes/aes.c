// SPDX-License-Identifier: MIT

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <oqs/common.h>

#include "aes.h"
#include "aes_local.h"

#if defined(OQS_DIST_X86_64_BUILD)
#define C_OR_NI_OR_ARM(stmt_c, stmt_ni, stmt_arm) \
    if (OQS_CPU_has_extension(OQS_CPU_EXT_AES)) { \
        stmt_ni; \
    } else { \
        stmt_c; \
    }
#elif defined(OQS_USE_AES_INSTRUCTIONS)
#define  C_OR_NI_OR_ARM(stmt_c, stmt_ni, stmt_arm) \
    stmt_ni;
#elif defined(OQS_USE_ARM_AES_INSTRUCTIONS)
#define C_OR_NI_OR_ARM(stmt_c, stmt_ni, stmt_arm) \
    stmt_arm;
#else
#define  C_OR_NI_OR_ARM(stmt_c, stmt_ni, stmt_arm) \
    stmt_c;
#endif

void OQS_AES128_ECB_load_schedule(const uint8_t *key, void **_schedule) {
	C_OR_NI_OR_ARM(
	    oqs_aes128_load_schedule_c(key, _schedule),
	    oqs_aes128_load_schedule_ni(key, _schedule),
	    oqs_aes128_load_schedule_armv8(key, _schedule)
	)
}

void OQS_AES128_free_schedule(void *schedule) {
	C_OR_NI_OR_ARM(
	    oqs_aes128_free_schedule_c(schedule),
	    oqs_aes128_free_schedule_ni(schedule),
	    oqs_aes128_free_schedule_armv8(schedule)
	)
}

void OQS_AES256_ECB_load_schedule(const uint8_t *key, void **_schedule) {
	C_OR_NI_OR_ARM(
	    oqs_aes256_load_schedule_c(key, _schedule),
	    oqs_aes256_load_schedule_ni(key, _schedule),
	    oqs_aes256_load_schedule_armv8(key, _schedule)
	)
}

void OQS_AES256_CTR_load_schedule(const uint8_t *key, void **_schedule) {
	OQS_AES256_ECB_load_schedule(key, _schedule);
}

void OQS_AES256_free_schedule(void *schedule) {
	C_OR_NI_OR_ARM(
	    oqs_aes256_free_schedule_c(schedule),
	    oqs_aes256_free_schedule_ni(schedule),
	    oqs_aes256_free_schedule_armv8(schedule)
	)
}

void OQS_AES128_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	void *schedule = NULL;
	OQS_AES128_ECB_load_schedule(key, &schedule);
	OQS_AES128_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
	OQS_AES128_free_schedule(schedule);
}

void OQS_AES128_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	C_OR_NI_OR_ARM(
	    oqs_aes128_ecb_enc_sch_c(plaintext, plaintext_len, schedule, ciphertext),
	    oqs_aes128_ecb_enc_sch_ni(plaintext, plaintext_len, schedule, ciphertext),
	    oqs_aes128_ecb_enc_sch_armv8(plaintext, plaintext_len, schedule, ciphertext)
	)
}

void OQS_AES256_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	void *schedule = NULL;
	OQS_AES256_ECB_load_schedule(key, &schedule);
	OQS_AES256_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
	OQS_AES256_free_schedule(schedule);
}

void OQS_AES256_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	C_OR_NI_OR_ARM(
	    oqs_aes256_ecb_enc_sch_c(plaintext, plaintext_len, schedule, ciphertext),
	    oqs_aes256_ecb_enc_sch_ni(plaintext, plaintext_len, schedule, ciphertext),
	    oqs_aes256_ecb_enc_sch_armv8(plaintext, plaintext_len, schedule, ciphertext)
	)
}

#if defined(OQS_DIST_X86_64_BUILD) || defined(OQS_USE_AES_INSTRUCTIONS) || defined(OQS_USE_ARM_AES_INSTRUCTIONS)

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
#endif

#if defined(OQS_DIST_X86_64_BUILD) || defined(OQS_USE_AES_INSTRUCTIONS)
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
#endif

#if defined(OQS_USE_ARM_AES_INSTRUCTIONS)
void oqs_aes256_ctr_enc_sch_armv8(const uint8_t *iv, const size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
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
		oqs_aes256_enc_sch_block_armv8(block, schedule, out);
		out += 16;
		out_len -= 16;
		ctr++;
	}
	if (out_len > 0) {
		uint8_t tmp[16];
		ctr_be = UINT32_TO_BE(ctr);
		memcpy(&block[12], (uint8_t *) &ctr_be, 4);
		oqs_aes256_enc_sch_block_armv8(block, schedule, tmp);
		memcpy(out, tmp, out_len);
	}
}
#endif


void OQS_AES256_CTR_sch(const uint8_t *iv, const size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
	C_OR_NI_OR_ARM(
	    oqs_aes256_ctr_enc_sch_c(iv, iv_len, schedule, out, out_len),
	    oqs_aes256_ctr_enc_sch_ni(iv, iv_len, schedule, out, out_len),
	    oqs_aes256_ctr_enc_sch_armv8(iv, iv_len, schedule, out, out_len)
	)
}
