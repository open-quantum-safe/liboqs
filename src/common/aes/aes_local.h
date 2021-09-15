// SPDX-License-Identifier: MIT

#include <stdint.h>

void oqs_aes128_load_schedule_ni(const uint8_t *key, void **_schedule);
void oqs_aes128_free_schedule_ni(void *schedule);
void oqs_aes128_enc_sch_block_ni(const uint8_t *plaintext, const void *_schedule, uint8_t *ciphertext);
void oqs_aes128_ecb_enc_sch_ni(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext);

void oqs_aes128_load_schedule_c(const uint8_t *key, void **_schedule);
void oqs_aes128_free_schedule_c(void *schedule);
void oqs_aes128_ecb_enc_sch_c(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext);

void oqs_aes128_load_schedule_armv8(const uint8_t *key, void **_schedule);
void oqs_aes128_free_schedule_armv8(void *schedule);
void oqs_aes128_enc_sch_block_armv8(const uint8_t *plaintext, const void *_schedule, uint8_t *ciphertext);
void oqs_aes128_ecb_enc_sch_armv8(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext);

void oqs_aes256_load_schedule_ni(const uint8_t *key, void **_schedule);
void oqs_aes256_free_schedule_ni(void *schedule);
void oqs_aes256_enc_sch_block_ni(const uint8_t *plaintext, const void *_schedule, uint8_t *ciphertext);
void oqs_aes256_ecb_enc_sch_ni(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext);
void oqs_aes256_ctr_enc_sch_ni(const uint8_t *iv, const size_t iv_len, const void *schedule, uint8_t *out, size_t out_len);

void oqs_aes256_load_schedule_c(const uint8_t *key, void **_schedule);
void oqs_aes256_free_schedule_c(void *schedule);
void oqs_aes256_ecb_enc_sch_c(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext);
void oqs_aes256_ctr_enc_sch_c(const uint8_t *iv, const size_t iv_len, const void *schedule, uint8_t *out, size_t out_len);

void oqs_aes256_free_schedule_armv8(void *schedule);
void oqs_aes256_load_schedule_armv8(const uint8_t *key, void **_schedule);
void oqs_aes256_enc_sch_block_armv8(const uint8_t *plaintext, const void *_schedule, uint8_t *ciphertext);
void oqs_aes256_ecb_enc_sch_armv8(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext);

