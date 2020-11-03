#include "crypto_encode_653x1541.h"
#include "crypto_encode_653x1541round.h"

#define int16 int16_t

#define p 653

void PQCLEAN_NTRULPR653_CLEAN_crypto_encode_653x1541round(unsigned char *out, const void *v) {
    const int16 *a = v;
    int16 x[p];
    int i;

    for (i = 0; i < p; ++i) {
        x[i] = (int16) (3 * ((10923 * a[i] + 16384) >> 15));
    }
    PQCLEAN_NTRULPR653_CLEAN_crypto_encode_653x1541(out, x);
}
