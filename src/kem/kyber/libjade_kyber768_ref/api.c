#include <oqs/rand.h>
#include "api.h"

int libjade_kyber768_ref_keypair(uint8_t *public_key, uint8_t *secret_key) {
    uint8_t keypair_coins[JADE_KEM_kyber_kyber768_amd64_ref_KEYPAIRCOINBYTES];
    OQS_randombytes(keypair_coins, JADE_KEM_kyber_kyber768_amd64_ref_KEYPAIRCOINBYTES/2);
    OQS_randombytes((uint8_t *)(keypair_coins + (JADE_KEM_kyber_kyber768_amd64_ref_KEYPAIRCOINBYTES/2)), JADE_KEM_kyber_kyber768_amd64_ref_KEYPAIRCOINBYTES/2);
    return jade_kem_kyber_kyber768_amd64_ref_keypair_derand(public_key, secret_key, keypair_coins);
}

int libjade_kyber768_ref_enc(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
    uint8_t enc_coins[JADE_KEM_kyber_kyber768_amd64_ref_ENCCOINBYTES];
    OQS_randombytes(enc_coins, JADE_KEM_kyber_kyber768_amd64_ref_ENCCOINBYTES);
    return jade_kem_kyber_kyber768_amd64_ref_enc_derand(ciphertext, shared_secret, public_key, enc_coins);
}

int libjade_kyber768_ref_dec(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key) {
    return jade_kem_kyber_kyber768_amd64_ref_dec(shared_secret, ciphertext, secret_key);
}
