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
 * @param forEncryption  1 if key schedule is for encryption, 0 if for decryption
 */
void OQS_AES128_load_schedule(const uint8_t *key, void **schedule, int for_encryption);

/**
 * Function to free a key schedule
 *
 * @param schedule       Schedule generated with OQS_AES128_load_schedule
 */
void OQS_AES128_free_schedule(void *schedule);

void OQS_AES128_ECB_enc(const uint8_t *plaintext, const size_t plaintext_len, const uint8_t *key, uint8_t *ciphertext);
void OQS_AES128_ECB_dec(const uint8_t *ciphertext, const size_t ciphertext_len, const uint8_t *key, uint8_t *plaintext);
void OQS_AES128_ECB_enc_sch(const uint8_t *plaintext, const size_t plaintext_len, const void *schedule, uint8_t *ciphertext);
void OQS_AES128_ECB_dec_sch(const uint8_t *ciphertext, const size_t ciphertext_len, const void *schedule, uint8_t *plaintext);

#endif
