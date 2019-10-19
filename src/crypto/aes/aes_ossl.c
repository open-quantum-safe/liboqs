#include <assert.h>
#include <string.h>

#include <oqs/oqs.h>

#include "aes.h"

#include <openssl/evp.h>

struct key_schedule {
	uint8_t key[32];
	EVP_CIPHER_CTX *ecb_ctx;
	EVP_CIPHER_CTX *ctr_ctx;
};

void OQS_AES128_load_schedule(const uint8_t *key, void **schedule, int for_encryption) {
	*schedule = malloc(sizeof(struct key_schedule));
	assert(*schedule != NULL);
	struct key_schedule *ks = (struct key_schedule *) *schedule;
	memcpy(ks->key, key, 16);
	ks->ctr_ctx = NULL;
	ks->ecb_ctx = EVP_CIPHER_CTX_new();
	assert(ks->ecb_ctx != NULL);
	if (for_encryption) {
		assert(1 == EVP_EncryptInit_ex(ks->ecb_ctx, EVP_aes_128_ecb(), NULL, key, NULL));
	} else {
		assert(1 == EVP_DecryptInit_ex(ks->ecb_ctx, EVP_aes_128_ecb(), NULL, key, NULL));
	}
	EVP_CIPHER_CTX_set_padding(ks->ecb_ctx, 0);
}

void OQS_AES128_free_schedule(void *schedule) {
	if (schedule != NULL) {
		struct key_schedule *ks = (struct key_schedule *) schedule;
		if (ks->ecb_ctx != NULL) {
			EVP_CIPHER_CTX_free(ks->ecb_ctx);
		}
		if (ks->ctr_ctx != NULL) {
			EVP_CIPHER_CTX_free(ks->ctr_ctx);
		}
		OQS_MEM_secure_free(ks, sizeof(struct key_schedule));
	}
}

void OQS_AES128_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	void *schedule = NULL;
	OQS_AES128_load_schedule(key, &schedule, 1);
	OQS_AES128_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
	OQS_AES128_free_schedule(schedule);
}

void OQS_AES128_ECB_dec(const uint8_t *ciphertext, const size_t ciphertext_len, const uint8_t *key, uint8_t *plaintext) {
	void *schedule = NULL;
	OQS_AES128_load_schedule(key, &schedule, 0);
	OQS_AES128_ECB_dec_sch(ciphertext, ciphertext_len, schedule, plaintext);
	OQS_AES128_free_schedule(schedule);
}

void OQS_AES128_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	assert(plaintext_len % 16 == 0);
	int outlen;
	struct key_schedule *ks = (struct key_schedule *) schedule;
	assert(1 == EVP_EncryptUpdate(ks->ecb_ctx, ciphertext, &outlen, plaintext, plaintext_len));
	assert((size_t) outlen == plaintext_len);
	assert(1 == EVP_EncryptFinal_ex(ks->ecb_ctx, ciphertext, &outlen));
}

void OQS_AES128_ECB_dec_sch(const uint8_t *ciphertext, const size_t ciphertext_len, const void *schedule, uint8_t *plaintext) {
	assert(ciphertext_len % 16 == 0);
	int outlen;
	struct key_schedule *ks = (struct key_schedule *) schedule;
	assert(1 == EVP_DecryptUpdate(ks->ecb_ctx, plaintext, &outlen, ciphertext, ciphertext_len));
	assert((size_t) outlen == ciphertext_len);
	assert(1 == EVP_DecryptFinal_ex(ks->ecb_ctx, plaintext, &outlen));
}

void OQS_AES256_load_schedule(const uint8_t *key, void **schedule, int for_encryption) {
	*schedule = malloc(sizeof(struct key_schedule));
	assert(*schedule != NULL);
	struct key_schedule *ks = (struct key_schedule *) *schedule;
	memcpy(ks->key, key, 32);
	ks->ctr_ctx = NULL;
	ks->ecb_ctx = EVP_CIPHER_CTX_new();
	assert(ks->ecb_ctx != NULL);
	if (for_encryption) {
		assert(1 == EVP_EncryptInit_ex(ks->ecb_ctx, EVP_aes_256_ecb(), NULL, key, NULL));
	} else {
		assert(1 == EVP_DecryptInit_ex(ks->ecb_ctx, EVP_aes_256_ecb(), NULL, key, NULL));
	}
	EVP_CIPHER_CTX_set_padding(ks->ecb_ctx, 0);
}

void OQS_AES256_free_schedule(void *schedule) {
	// actually same code as AES 128
	return OQS_AES128_free_schedule(schedule);
}

void OQS_AES256_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext) {
	void *schedule = NULL;
	OQS_AES256_load_schedule(key, &schedule, 1);
	OQS_AES256_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
	OQS_AES256_free_schedule(schedule);
}

void OQS_AES256_ECB_dec(const uint8_t *ciphertext, const size_t ciphertext_len, const uint8_t *key, uint8_t *plaintext) {
	void *schedule = NULL;
	OQS_AES256_load_schedule(key, &schedule, 0);
	OQS_AES256_ECB_dec_sch(ciphertext, ciphertext_len, schedule, plaintext);
	OQS_AES256_free_schedule(schedule);
}

void OQS_AES256_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext) {
	// actually same code as AES 128
	return OQS_AES128_ECB_enc_sch(plaintext, plaintext_len, schedule, ciphertext);
}

void OQS_AES256_ECB_dec_sch(const uint8_t *ciphertext, const size_t ciphertext_len, const void *schedule, uint8_t *plaintext) {
	// actually same code as AES 128
	return OQS_AES128_ECB_dec_sch(ciphertext, ciphertext_len, schedule, plaintext);
}

void OQS_AES256_CTR_sch(const uint8_t *iv, size_t iv_len, const void *schedule, uint8_t *out, size_t out_len) {
	struct key_schedule *ks = (struct key_schedule *) schedule;
	if (ks->ctr_ctx == NULL) {
		ks->ctr_ctx = EVP_CIPHER_CTX_new();
		assert(ks->ctr_ctx != NULL);
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
			assert(0);
		}
		assert(1 == EVP_EncryptInit_ex(ks->ctr_ctx, EVP_aes_256_ctr(), NULL, ks->key, iv_ctr));
	}
	memset(out, 0, out_len);
	int outlen;
	assert(1 == EVP_EncryptUpdate(ks->ctr_ctx, out, &outlen, out, out_len));
	assert(1 == EVP_EncryptFinal_ex(ks->ctr_ctx, out + outlen, &outlen));
}