#include "crypto_encode_1277x2627.h"
#include "crypto_encode_1277x2627round.h"

#define int16 int16_t

#define p 1277

void PQCLEAN_SNTRUP1277_CLEAN_crypto_encode_1277x2627round(unsigned char *out, const void *v) {
    const int16 *a = v;
    int16 x[p];
    int i;

    for (i = 0; i < p; ++i) {
        x[i] = 3 * ((10923 * a[i] + 16384) >> 15);
    }
    PQCLEAN_SNTRUP1277_CLEAN_crypto_encode_1277x2627(out, x);
}
