/**
 * \file aes.h
 * \brief Header defining the API for OQS AES
 */


#ifndef __OQS_AES_H
#define __OQS_AES_H

#include <stdint.h>

/**
* Function to fill a key schedule given an inital key
*
* @param key            Initial Key
* @param schedule       a 16*20 byte array to store the key schedual
*/
void OQS_AES128_load_schedule(uint8_t *key, uint8_t *schedule);

/**
* AES 128 ECB encryption function
*
* @param plain_text     Plain-text message to encrypt (16-byte array)
* @param schedule       Schedule generated with OQS_AES128_load_schedule
* @param cipher_text    16-byte array to store cipher-text
*/
void OQS_AES128_enc(uint8_t *plain_text, uint8_t *schedule, uint8_t *cipher_text);

/**
* AES 128 ECB decryption function
*
* @param cipher_text    Cipher-text message to decrypt (16-byte array)
* @param schedule       Schedule generated with OQS_AES128_load_schedule
* @param plain_text     16-byte array to store cipher-text
*/
void OQS_AES128_dec(uint8_t *cipher_text, uint8_t *schedule, uint8_t *plain_text);

#endif
