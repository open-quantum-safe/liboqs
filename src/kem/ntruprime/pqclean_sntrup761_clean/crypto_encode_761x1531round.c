#include "crypto_encode_761x1531.h"
#include "crypto_encode_761x1531round.h"

#define int16 int16_t

#define p 761

void PQCLEAN_SNTRUP761_CLEAN_crypto_encode_761x1531round(unsigned char *out, const void *v) {
    const int16 *a = v;
    int16 x[p];
    int i;

    for (i = 0; i < p; ++i) {
        x[i] = (int16) (3 * ((10923 * a[i] + 16384) >> 15));
    }
    PQCLEAN_SNTRUP761_CLEAN_crypto_encode_761x1531(out, x);
}
