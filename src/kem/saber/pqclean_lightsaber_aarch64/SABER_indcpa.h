#ifndef INDCPA_H
#define INDCPA_H

#include "SABER_params.h"
#include <stdint.h>

#define indcpa_kem_keypair SABER_NAMESPACE(indcpa_kem_keypair)
void indcpa_kem_keypair(uint8_t pk[SABER_INDCPA_PUBLICKEYBYTES], uint8_t sk[SABER_INDCPA_SECRETKEYBYTES]);
#define indcpa_kem_enc SABER_NAMESPACE(indcpa_kem_enc)
void indcpa_kem_enc(const uint8_t m[SABER_KEYBYTES], const uint8_t seed_sp[SABER_NOISE_SEEDBYTES], const uint8_t pk[SABER_INDCPA_PUBLICKEYBYTES], uint8_t ciphertext[SABER_BYTES_CCA_DEC]);
#define indcpa_kem_dec SABER_NAMESPACE(indcpa_kem_dec)
void indcpa_kem_dec(const uint8_t sk[SABER_INDCPA_SECRETKEYBYTES], const uint8_t ciphertext[SABER_BYTES_CCA_DEC], uint8_t m[SABER_KEYBYTES]);

#endif
