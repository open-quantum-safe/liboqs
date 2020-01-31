#include "api.h"
#include "params.h"
#include "randombytes.h"
#include "threebears.h"

int PQCLEAN_MAMABEAR_CLEAN_crypto_kem_keypair(uint8_t *pk, uint8_t *sk) {
    randombytes(sk, PRIVATE_KEY_BYTES);
    PQCLEAN_MAMABEAR_CLEAN_get_pubkey(pk, sk);
    return 0;
}

int PQCLEAN_MAMABEAR_CLEAN_crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    uint8_t seed[ENC_SEED_BYTES + IV_BYTES];
    randombytes(seed, sizeof(seed));
    encapsulate(ss, ct, pk, seed);
    return 0;
}

int PQCLEAN_MAMABEAR_CLEAN_crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
    PQCLEAN_MAMABEAR_CLEAN_decapsulate(ss, ct, sk);
    return 0;
}
