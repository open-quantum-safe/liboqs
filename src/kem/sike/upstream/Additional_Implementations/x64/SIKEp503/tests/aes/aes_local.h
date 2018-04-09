/********************************************************************************************
* Header defining additional internal functions for AES
*********************************************************************************************/

#ifndef AES_LOCAL_H
#define AES_LOCAL_H

#include <stdint.h>
#include <stdlib.h>

void aes256_load_schedule_c(const uint8_t *key, uint8_t *schedule);
void aes256_enc_c(const uint8_t *plaintext, const void *schedule, uint8_t *ciphertext);

#endif
