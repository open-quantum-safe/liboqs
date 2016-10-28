/**
 * \file aes.h
 * \brief Header defining the API for OQS AES
 */


#ifndef __OQS_AES_H
#define __OQS_AES_H

#include <stdint.h>
#include <stdlib.h>

/**
 * Function to fill a key schedule given an inital key
 *
 * @param key            Initial Key
 * @param schedule       Abstract data structure for a key schedule
 */
void OQS_AES128_load_schedule(const uint8_t *key, void **schedule);

/**
 * Function to free a key schedule
 *
 * @param schedule       Schedule generated with OQS_AES128_load_schedule
 */
void OQS_AES128_free_schedule(void *schedule);

/**
 * AES 128 encryption function
 *
 * @param plaintext      Plaintext message to encrypt (16-byte array)
 * @param schedule       Schedule generated with OQS_AES128_load_schedule
 * @param ciphertext     16-byte array to store ciphertext
 */
void OQS_AES128_enc(const uint8_t *plaintext, const void *schedule, uint8_t *ciphertext);

/**
 * AES 128 decryption function
 *
 * @param ciphertext     Ciphertext message to decrypt (16-byte array)
 * @param schedule       Schedule generated with OQS_AES128_load_schedule
 * @param plaintext      16-byte array to store ciphertext
 */
void OQS_AES128_dec(const uint8_t *ciphertext, const void *schedule, uint8_t *plaintext);

void OQS_AES128_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext);
void OQS_AES128_ECB_dec(const uint8_t *ciphertext, const size_t ciphertext_len, const uint8_t *key, uint8_t *plaintext);
void OQS_AES128_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext);

void OQS_AES128_load_schedule_ni(const uint8_t *key, void **schedule);
void OQS_AES128_free_schedule_ni(void *schedule);
void OQS_AES128_enc_ni(const uint8_t *plaintext, const void *schedule, uint8_t *ciphertext);
void OQS_AES128_dec_ni(const uint8_t *ciphertext, const void *schedule, uint8_t *plaintext);

void OQS_AES128_load_schedule_c(const uint8_t *key, void **schedule);
void OQS_AES128_free_schedule_c(void *schedule);
void OQS_AES128_enc_c(const uint8_t *plaintext, const void *schedule, uint8_t *ciphertext);
void OQS_AES128_dec_c(const uint8_t *ciphertext, const void *schedule, uint8_t *plaintext);

#endif
