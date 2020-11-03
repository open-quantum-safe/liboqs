#include "crypto_encode_857x1723.h"
#include "crypto_encode_857x1723round.h"

#define int16 int16_t

#define p 857

void PQCLEAN_SNTRUP857_CLEAN_crypto_encode_857x1723round(unsigned char *out, const void *v) {
    const int16 *a = v;
    int16 x[p];
    int i;

    for (i = 0; i < p; ++i) {
        x[i] = (int16) (3 * ((10923 * a[i] + 16384) >> 15));
    }
    PQCLEAN_SNTRUP857_CLEAN_crypto_encode_857x1723(out, x);
}
