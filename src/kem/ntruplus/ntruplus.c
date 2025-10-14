#include "ntruplus.h"
#include <string.h>

int crypto_kem_keypair(uint8_t *pk, uint8_t *sk) {
    // TODO: 실제 NTRU+ keypair 구현
    memset(pk, 0, NTRUPLUS_PUBLICKEYBYTES);
    memset(sk, 0, NTRUPLUS_SECRETKEYBYTES);
    return 0;
}

int crypto_kem_enc(uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    // TODO: 실제 NTRU+ encapsulation 구현
    (void)pk;
    memset(ct, 0, NTRUPLUS_CIPHERTEXTBYTES);
    memset(ss, 0, NTRUPLUS_BYTES);
    return 0;
}

int crypto_kem_dec(uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
    // TODO: 실제 NTRU+ decapsulation 구현
    (void)ct;
    (void)sk;
    memset(ss, 0, NTRUPLUS_BYTES);
    return 0;
}