#include <stdint.h>
#include <stdlib.h>

#include "hqc-kem.h"
#include "randombytes.h"
#include "oqs/rand.h"


int randombytes(uint8_t *out, size_t outlen) {
    OQS_randombytes(out, outlen);
    return 0;
}

int8_t hqc_kem_encaps_oqs(uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    return hqc_kem_encaps(ss, ct, pk);
}

int8_t hqc_kem_decaps_oqs(uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
    return hqc_kem_decaps(ss, sk, ct);
}
