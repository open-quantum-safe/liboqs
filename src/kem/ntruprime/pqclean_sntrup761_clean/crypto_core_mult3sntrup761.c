#include "crypto_core_mult3sntrup761.h"
#include "params.h"


#define int8 int8_t
#define int16 int16_t
typedef int8 small;

/* works for -16384 <= x < 16384 */
static small F3_freeze(int16 x) {
    return (small) (x - 3 * ((10923 * x + 16384) >> 15));
}

int PQCLEAN_SNTRUP761_CLEAN_crypto_core_mult3sntrup761(unsigned char *outbytes, const unsigned char *inbytes, const unsigned char *kbytes) {
    small *h = (void *) outbytes;
    small f[p];
    small g[p];
    small fg[p + p - 1];
    int16 result;
    int i, j;

    for (i = 0; i < p; ++i) {
        small fi = (small) inbytes[i];
        small fi0 = fi & 1;
        f[i] = (small) (fi0 - (fi & (fi0 << 1)));
    }
    for (i = 0; i < p; ++i) {
        small gi = (small) kbytes[i];
        small gi0 = gi & 1;
        g[i] = (small) (gi0 - (gi & (gi0 << 1)));
    }

    for (i = 0; i < p; ++i) {
        result = 0;
        for (j = 0; j <= i; ++j) {
            result += (small) (f[j] * g[i - j]);
        }
        fg[i] = F3_freeze(result);
    }
    for (i = p; i < p + p - 1; ++i) {
        result = 0;
        for (j = i - p + 1; j < p; ++j) {
            result += (small) (f[j] * g[i - j]);
        }
        fg[i] = F3_freeze(result);
    }

    for (i = p + p - 2; i >= p; --i) {
        fg[i - p] = F3_freeze(fg[i - p] + fg[i]);
        fg[i - p + 1] = F3_freeze(fg[i - p + 1] + fg[i]);
    }

    for (i = 0; i < p; ++i) {
        h[i] = fg[i];
    }
    return 0;
}
