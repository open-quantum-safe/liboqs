/**
 * \file aes_local.h
 * \brief Header defining additional internal functions for OQS AES
 */


#ifndef __OQS_AES_LOCAL_H
#define __OQS_AES_LOCAL_H

#include <stdint.h>
#include <stdlib.h>

void oqs_aes128_enc(const uint8_t *plaintext, const void *schedule, uint8_t *ciphertext);
void oqs_aes128_dec(const uint8_t *ciphertext, const void *schedule, uint8_t *plaintext);

void oqs_aes128_load_schedule_ni(const uint8_t *key, void **schedule);
void oqs_aes128_free_schedule_ni(void *schedule);
void oqs_aes128_enc_ni(const uint8_t *plaintext, const void *schedule, uint8_t *ciphertext);
void oqs_aes128_dec_ni(const uint8_t *ciphertext, const void *schedule, uint8_t *plaintext);

void oqs_aes128_load_schedule_c(const uint8_t *key, void **schedule);
void oqs_aes128_free_schedule_c(void *schedule);
void oqs_aes128_enc_c(const uint8_t *plaintext, const void *schedule, uint8_t *ciphertext);
void oqs_aes128_dec_c(const uint8_t *ciphertext, const void *schedule, uint8_t *plaintext);

#endif
