#ifndef KEM_H
#define KEM_H
/*=============================================================================
This file has been adapted from the implementation
(available at, Public Domain https://github.com/KULeuven-COSIC/SABER)
of "Saber: Module-LWR based key exchange, CPA-secure encryption and CCA-secure KEM"
by : Jan-Pieter D'Anvers, Angshuman Karmakar, Sujoy Sinha Roy, and Frederik Vercauteren
Jose Maria Bermudo Mera, Michiel Van Beirendonck, Andrea Basso.
=============================================================================*/

#include <stdint.h>

int PQCLEAN_SABER_AARCH64_crypto_kem_keypair(uint8_t *pk, uint8_t *sk);
int PQCLEAN_SABER_AARCH64_crypto_kem_enc(uint8_t *c, uint8_t *k, const uint8_t *pk);
int PQCLEAN_SABER_AARCH64_crypto_kem_dec(uint8_t *k, const uint8_t *c, const uint8_t *sk);

#endif
