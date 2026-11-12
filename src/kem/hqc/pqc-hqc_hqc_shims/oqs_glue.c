#include <stdint.h>
#include <stdlib.h>

#include "hqc-kem.h"
#include "randombytes_fallible.h"
#include "oqs/rand.h"


/* TODO: HQC's randombytes API is fallible: it might return non-zero code
 * indicating secure entropy failure. OQS_randombytes is not fallible and will
 * either mask the underlying failure or panic and exit. This makes it impossible
 * to do a plug-and-play shim for randombytes. See mlkem-native for another
 * example: https://github.com/pq-code-package/mlkem-native/blob/60fb495/integration/liboqs/config_c.h#L177
 */
int randombytes_fallible(uint8_t *out, size_t outlen) {
    OQS_randombytes(out, outlen);
    return 0;
}

int8_t hqc_kem_encaps_oqs(uint8_t *ct, uint8_t *ss, const uint8_t *pk) {
    return hqc_kem_encaps(ss, ct, pk);
}

int8_t hqc_kem_decaps_oqs(uint8_t *ss, const uint8_t *ct, const uint8_t *sk) {
    return hqc_kem_decaps(ss, sk, ct);
}
