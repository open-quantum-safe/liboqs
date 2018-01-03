/**
 * \file kex_lwe_frodokem.h
 * \brief Header for LWE key exchange protocol FrodoKEM.
 */

#ifndef __OQS_KEX_LWE_FRODOKEM_H
#define __OQS_KEX_LWE_FRODOKEM_H

#include <stddef.h>
#include <stdint.h>

#include <oqs/kex.h>
#include <oqs/rand.h>

OQS_KEX *OQS_KEX_lwe_frodokem_640_aes_new();
int OQS_KEX_lwe_frodokem_640_aes_alice_0(OQS_KEX *k, void **alice_priv, uint8_t **alice_msg, size_t *alice_msg_len);
int OQS_KEX_lwe_frodokem_640_aes_bob(OQS_KEX *k, const uint8_t *alice_msg, const size_t alice_msg_len, uint8_t **bob_msg, size_t *bob_msg_len, uint8_t **key, size_t *key_len);
int OQS_KEX_lwe_frodokem_640_aes_alice_1(OQS_KEX *k, const void *alice_priv, const uint8_t *bob_msg, const size_t bob_msg_len, uint8_t **key, size_t *key_len);

OQS_KEX *OQS_KEX_lwe_frodokem_640_cshake_new();
int OQS_KEX_lwe_frodokem_640_cshake_alice_0(OQS_KEX *k, void **alice_priv, uint8_t **alice_msg, size_t *alice_msg_len);
int OQS_KEX_lwe_frodokem_640_cshake_bob(OQS_KEX *k, const uint8_t *alice_msg, const size_t alice_msg_len, uint8_t **bob_msg, size_t *bob_msg_len, uint8_t **key, size_t *key_len);
int OQS_KEX_lwe_frodokem_640_cshake_alice_1(OQS_KEX *k, const void *alice_priv, const uint8_t *bob_msg, const size_t bob_msg_len, uint8_t **key, size_t *key_len);

void OQS_KEX_lwe_frodokem_alice_priv_free(OQS_KEX *k, void *alice_priv);
void OQS_KEX_lwe_frodokem_free(OQS_KEX *k);

#endif
