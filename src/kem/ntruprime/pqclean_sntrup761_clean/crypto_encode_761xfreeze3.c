#include "crypto_encode_761xfreeze3.h"

#define int16 int16_t

#define p 761

/* valid inputs: -16384 <= x < 16384 */
/* then 3 divides x-F3_freeze(x) */
/* and F3_freeze(x) is in {-1,0,1} */

/* all inputs: 3 divides x-F3_freeze(x) */
/* and F3_freeze(x) is in {-2,-1,0,1,2} */

static inline unsigned char F3_freeze(int16 x) {
    return (unsigned char) (x - 3 * ((10923 * x + 16384) >> 15));
}

void PQCLEAN_SNTRUP761_CLEAN_crypto_encode_761xfreeze3(unsigned char *s, const void *v) {
    const int16 *r = v;

    int i;
    for (i = 0; i < p; ++i) {
        s[i] = F3_freeze(r[i]);
    }
}
