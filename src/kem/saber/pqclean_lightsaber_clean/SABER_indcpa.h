#ifndef INDCPA_H
#define INDCPA_H
#include "SABER_params.h"
#include <stdint.h>

void PQCLEAN_LIGHTSABER_CLEAN_indcpa_kem_keypair(uint8_t pk[SABER_INDCPA_PUBLICKEYBYTES], uint8_t sk[SABER_INDCPA_SECRETKEYBYTES]);

void PQCLEAN_LIGHTSABER_CLEAN_indcpa_kem_enc(uint8_t ciphertext[SABER_BYTES_CCA_DEC], const uint8_t m[SABER_KEYBYTES], const uint8_t noiseseed[SABER_NOISESEEDBYTES], const uint8_t pk[SABER_INDCPA_PUBLICKEYBYTES]);

void PQCLEAN_LIGHTSABER_CLEAN_indcpa_kem_dec(uint8_t m[SABER_KEYBYTES], const uint8_t sk[SABER_INDCPA_SECRETKEYBYTES], const uint8_t ciphertext[SABER_BYTES_CCA_DEC]);


#endif
