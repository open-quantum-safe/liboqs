#include <assert.h>

#include <oqs/oqs.h>

#include "aes.h"

#include <openssl/evp.h>

void OQS_AES128_load_schedule(const uint8_t *key, void **schedule, int for_encryption) {
	EVP_CIPHER_CTX *aes_ctx = EVP_CIPHER_CTX_new();
	assert(aes_ctx != NULL);
	if (for_encryption) {
		assert(1 == EVP_EncryptInit_ex(aes_ctx, EVP_aes_128_ecb(), NULL, key, NULL));
	} else {
		assert(1 == EVP_DecryptInit_ex(aes_ctx, EVP_aes_128_ecb(), NULL, key, NULL));
	}
	EVP_CIPHER_CTX_set_padding(aes_ctx, 0);
	*schedule = aes_ctx;
}

void OQS_AES128_free_schedule(void *schedule) {
	if (schedule != NULL) {
		EVP_CIPHER_CTX_free((EVP_CIPHER_CTX *) schedule);
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
	assert(1 == EVP_EncryptUpdate((EVP_CIPHER_CTX *) schedule, ciphertext, &outlen, plaintext, plaintext_len));
	assert((size_t) outlen == plaintext_len);
	assert(1 == EVP_EncryptFinal_ex((EVP_CIPHER_CTX *) schedule, ciphertext, &outlen));
}

void OQS_AES128_ECB_dec_sch(const uint8_t *ciphertext, const size_t ciphertext_len, const void *schedule, uint8_t *plaintext) {
	assert(ciphertext_len % 16 == 0);
	int outlen;
	assert(1 == EVP_DecryptUpdate((EVP_CIPHER_CTX *) schedule, plaintext, &outlen, ciphertext, ciphertext_len));
	assert((size_t) outlen == ciphertext_len);
	assert(1 == EVP_DecryptFinal_ex((EVP_CIPHER_CTX *) schedule, plaintext, &outlen));
}

void OQS_AES256_load_schedule(const uint8_t *key, void **schedule, int for_encryption) {
	EVP_CIPHER_CTX *aes_ctx = EVP_CIPHER_CTX_new();
	assert(aes_ctx != NULL);
	if (for_encryption) {
		assert(1 == EVP_EncryptInit_ex(aes_ctx, EVP_aes_256_ecb(), NULL, key, NULL));
	} else {
		assert(1 == EVP_DecryptInit_ex(aes_ctx, EVP_aes_256_ecb(), NULL, key, NULL));
	}
	EVP_CIPHER_CTX_set_padding(aes_ctx, 0);
	*schedule = aes_ctx;
}

void OQS_AES256_free_schedule(void *schedule) {
	if (schedule != NULL) {
		EVP_CIPHER_CTX_free((EVP_CIPHER_CTX *) schedule);
	}
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
	assert(plaintext_len % 16 == 0);
	int outlen;
	assert(1 == EVP_EncryptUpdate((EVP_CIPHER_CTX *) schedule, ciphertext, &outlen, plaintext, plaintext_len));
	assert((size_t) outlen == plaintext_len);
	assert(1 == EVP_EncryptFinal_ex((EVP_CIPHER_CTX *) schedule, ciphertext, &outlen));
}

void OQS_AES256_ECB_dec_sch(const uint8_t *ciphertext, const size_t ciphertext_len, const void *schedule, uint8_t *plaintext) {
	assert(ciphertext_len % 16 == 0);
	int outlen;
	assert(1 == EVP_DecryptUpdate((EVP_CIPHER_CTX *) schedule, plaintext, &outlen, ciphertext, ciphertext_len));
	assert((size_t) outlen == ciphertext_len);
	assert(1 == EVP_DecryptFinal_ex((EVP_CIPHER_CTX *) schedule, plaintext, &outlen));
}
