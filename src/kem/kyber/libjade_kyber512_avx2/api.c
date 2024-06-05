#include <oqs/rand.h>
#include "api.h"

int libjade_kyber512_avx2_keypair(uint8_t *public_key, uint8_t *secret_key) {
    uint8_t keypair_coins[JADE_KEM_kyber_kyber512_amd64_avx2_KEYPAIRCOINBYTES];
    OQS_randombytes(keypair_coins, JADE_KEM_kyber_kyber512_amd64_avx2_KEYPAIRCOINBYTES/2);
    OQS_randombytes((uint8_t *)(keypair_coins + (JADE_KEM_kyber_kyber512_amd64_avx2_KEYPAIRCOINBYTES/2)), JADE_KEM_kyber_kyber512_amd64_avx2_KEYPAIRCOINBYTES/2);
    return jade_kem_kyber_kyber512_amd64_avx2_keypair_derand(public_key, secret_key, keypair_coins);
}

int libjade_kyber512_avx2_enc(uint8_t *ciphertext, uint8_t *shared_secret, const uint8_t *public_key) {
    uint8_t enc_coins[JADE_KEM_kyber_kyber512_amd64_avx2_ENCCOINBYTES];
    OQS_randombytes(enc_coins, JADE_KEM_kyber_kyber512_amd64_avx2_ENCCOINBYTES);
    return jade_kem_kyber_kyber512_amd64_avx2_enc_derand(ciphertext, shared_secret, public_key, enc_coins);
}

int libjade_kyber512_avx2_dec(uint8_t *shared_secret, const uint8_t *ciphertext, const uint8_t *secret_key) {
    return jade_kem_kyber_kyber512_amd64_avx2_dec( shared_secret, ciphertext, secret_key);
}
