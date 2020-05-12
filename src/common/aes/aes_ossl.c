// SPDX-License-Identifier: MIT

#include <assert.h>
#include <string.h>

#include <oqs/oqs.h>

#include "aes.h"

#include <openssl/evp.h>

struct key_schedule {
	int for_ECB;
	EVP_CIPHER_CTX *ctx;
	uint8_t key[32];
};

void OQS_AES128_ECB_load_schedule(const uint8_t *key, void **schedule, int for_encryption) {
	*schedule = malloc(sizeof(struct key_schedule));
	OQS_EXIT_IF_NULLPTR(*schedule);
	struct key_schedule *ks = (struct key_schedule *) *schedule;
	ks->for_ECB = 1;
	ks->ctx = EVP_CIPHER_CTX_new();
	OQS_EXIT_IF_NULLPTR(ks->ctx);
	if (for_encryption) {
		OQS_OPENSSL_GUARD(EVP_EncryptInit_ex(ks->ctx, EVP_aes_128_ecb(), NULL, key, NULL));
	} else {
		OQS_OPENSSL_GUARD(EVP_DecryptInit_ex(ks->ctx, EVP_aes_128_ecb(), NULL, key, NULL));
	}
	EVP_CIPHER_CTX_set_padding(ks->ctx, 0);
}

void OQS_AES128_free_schedule(void *schedule) {
	if (schedule != NULL) {
		struct key_schedule *ks = (struct key_schedule *) schedule;
		if (ks->ctx != NULL) {
			EVP_CIPHER_CTX_free(ks->ctx);
		}
		OQS_MEM_cleanse(ks->key, 32);
		OQS_MEM_secure_free(schedule, sizeof(struct key_schedule));
	}
}

void OQS_AES128_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	void *schedule = NULL;
	OQS_AES128_ECB_load_schedule(key, &schedule, 1);
	OQS_AES128_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
	OQS_AES128_free_schedule(schedule);
}

void OQS_AES128_ECB_dec(const uint8_t *ciphertext, const size_t ciphertext_len, const uint8_t *key, uint8_t *plaintext) {
	void *schedule = NULL;
	OQS_AES128_ECB_load_schedule(key, &schedule, 0);
	OQS_AES128_ECB_dec_sch(ciphertext, ciphertext_len, schedule, plaintext);
	OQS_AES128_free_schedule(schedule);
}

void OQS_AES128_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	assert(plaintext_len % 16 == 0);
	int outlen;
	const struct key_schedule *ks = (const struct key_schedule *) schedule;
	SIZE_T_TO_INT_OR_EXIT(plaintext_len, plaintext_len_int)
	OQS_OPENSSL_GUARD(EVP_EncryptUpdate(ks->ctx, ciphertext, &outlen, plaintext, plaintext_len_int));
	assert(outlen == plaintext_len_int);
	OQS_OPENSSL_GUARD(EVP_EncryptFinal_ex(ks->ctx, ciphertext, &outlen));
}

void OQS_AES128_ECB_dec_sch(const uint8_t *ciphertext, const size_t ciphertext_len, const void *schedule, uint8_t *plaintext) {
	assert(ciphertext_len % 16 == 0);
	int outlen;
	const struct key_schedule *ks = (const struct key_schedule *) schedule;
	SIZE_T_TO_INT_OR_EXIT(ciphertext_len, ciphertext_len_int)
	OQS_OPENSSL_GUARD(EVP_DecryptUpdate(ks->ctx, plaintext, &outlen, ciphertext, ciphertext_len_int));
	assert(outlen == ciphertext_len_int);
	OQS_OPENSSL_GUARD(EVP_DecryptFinal_ex(ks->ctx, plaintext, &outlen));
}

void OQS_AES256_ECB_load_schedule(const uint8_t *key, void **schedule, int for_encryption) {
	*schedule = malloc(sizeof(struct key_schedule));
	OQS_EXIT_IF_NULLPTR(*schedule);
	struct key_schedule *ks = (struct key_schedule *) *schedule;
	ks->for_ECB = 1;
	ks->ctx = EVP_CIPHER_CTX_new();
	OQS_EXIT_IF_NULLPTR(ks->ctx);
	if (for_encryption) {
		OQS_OPENSSL_GUARD(EVP_EncryptInit_ex(ks->ctx, EVP_aes_256_ecb(), NULL, key, NULL));
	} else {
		OQS_OPENSSL_GUARD(EVP_DecryptInit_ex(ks->ctx, EVP_aes_256_ecb(), NULL, key, NULL));
	}
	EVP_CIPHER_CTX_set_padding(ks->ctx, 0);
}

void OQS_AES256_CTR_load_schedule(const uint8_t *key, void **schedule) {
	*schedule = malloc(sizeof(struct key_schedule));
	OQS_EXIT_IF_NULLPTR(*schedule);
	struct key_schedule *ks = (struct key_schedule *) *schedule;
	ks->for_ECB = 0;
	ks->ctx = NULL;
	memcpy(ks->key, key, 32);
}

void OQS_AES256_free_schedule(void *schedule) {
	// actually same code as AES 128
	OQS_AES128_free_schedule(schedule);
}

void OQS_AES256_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	void *schedule = NULL;
	OQS_AES256_ECB_load_schedule(key, &schedule, 1);
	OQS_AES256_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
	OQS_AES256_free_schedule(schedule);
}

void OQS_AES256_ECB_dec(const uint8_t *ciphertext, const size_t ciphertext_len, const uint8_t *key, uint8_t *plaintext) {
	void *schedule = NULL;
	OQS_AES256_ECB_load_schedule(key, &schedule, 0);
	OQS_AES256_ECB_dec_sch(ciphertext, ciphertext_len, schedule, plaintext);
	OQS_AES256_free_schedule(schedule);
}

void OQS_AES256_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	// actually same code as AES 128
	OQS_AES128_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
}

void OQS_AES256_ECB_dec_sch(const uint8_t *ciphertext, const size_t ciphertext_len, const void *schedule, uint8_t *plaintext) {
	// actually same code as AES 128
	OQS_AES128_ECB_dec_sch(ciphertext, ciphertext_len, schedule, plaintext);
}

void OQS_AES256_CTR_sch(const uint8_t *iv, size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
	EVP_CIPHER_CTX *ctr_ctx = EVP_CIPHER_CTX_new();
	assert(ctr_ctx != NULL);
	uint8_t iv_ctr[16];
	if (iv_len == 12) {
		memcpy(iv_ctr, iv, 12);
		iv_ctr[12] = 0;
		iv_ctr[13] = 0;
		iv_ctr[14] = 0;
		iv_ctr[15] = 0;
	} else if (iv_len == 16) {
		memcpy(iv_ctr, iv, 16);
	} else {
		exit(EXIT_FAILURE);
	}
	const struct key_schedule *ks = (const struct key_schedule *) schedule;
	OQS_OPENSSL_GUARD(EVP_EncryptInit_ex(ctr_ctx, EVP_aes_256_ctr(), NULL, ks->key, iv_ctr));

	SIZE_T_TO_INT_OR_EXIT(out_len, out_len_input_int)
	memset(out, 0, (size_t)out_len_input_int);
	int out_len_output;
	OQS_OPENSSL_GUARD(EVP_EncryptUpdate(ctr_ctx, out, &out_len_output, out, out_len_input_int));
	OQS_OPENSSL_GUARD(EVP_EncryptFinal_ex(ctr_ctx, out + out_len_output, &out_len_output));
	EVP_CIPHER_CTX_free(ctr_ctx);
}
