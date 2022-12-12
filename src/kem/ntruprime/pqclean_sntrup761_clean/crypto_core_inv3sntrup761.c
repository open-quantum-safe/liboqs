#include "crypto_core_inv3sntrup761.h"
#include "params.h"



#define int8 int8_t
#define int16 int16_t
#define int32 int32_t
#define uint16 uint16_t
#define uint32 uint32_t

/* ----- masks */

/* return -1 if x!=0; else return 0 */
static int int16_nonzero_mask(int16 x) {
    uint16 u = (uint16) x; /* 0, else 1...65535 */
    uint32 v = u; /* 0, else 1...65535 */
    v = ~v + 1; /* 0, else 2^32-65535...2^32-1 */
    v >>= 31; /* 0, else 1 */
    return -(int) v; /* 0, else -1 */
}

/* return -1 if x<0; otherwise return 0 */
static int int16_negative_mask(int16 x) {
    uint16 u = (uint16) x;
    u >>= 15;
    return -(int) u;
    /* alternative with gcc -fwrapv: */
    /* x>>15 compiles to CPU's arithmetic right shift */
}

/* ----- arithmetic mod 3 */

typedef int8 small;
/* F3 is always represented as -1,0,1 */

/* works for -16384 <= x < 16384 */
static small F3_freeze(int16 x) {
    return (small) (x - 3 * ((10923 * x + 16384) >> 15));
}

/* byte p of output is 0 if recip succeeded; else -1 */
int PQCLEAN_SNTRUP761_CLEAN_crypto_core_inv3sntrup761(unsigned char *outbytes, const unsigned char *inbytes) {
    small *out = (void *) outbytes;
    small *in = (void *) inbytes;
    small f[p + 1], g[p + 1], v[p + 1], r[p + 1];
    int i, loop, delta;
    int sign, swap, t;

    for (i = 0; i < p + 1; ++i) {
        v[i] = 0;
    }
    for (i = 0; i < p + 1; ++i) {
        r[i] = 0;
    }
    r[0] = 1;
    for (i = 0; i < p; ++i) {
        f[i] = 0;
    }
    f[0] = 1;
    f[p - 1] = f[p] = -1;
    for (i = 0; i < p; ++i) {
        small i1 = in[i] & 1;
        g[p - 1 - i] = (small) (i1 - (in[i] & (i1 << 1)));
    }
    g[p] = 0;

    delta = 1;

    for (loop = 0; loop < 2 * p - 1; ++loop) {
        for (i = p; i > 0; --i) {
            v[i] = v[i - 1];
        }
        v[0] = 0;

        sign = -g[0] * f[0];
        swap = int16_negative_mask((int16) - delta) & int16_nonzero_mask(g[0]);
        delta ^= swap & (delta ^ -delta);
        delta += 1;

        for (i = 0; i < p + 1; ++i) {
            t = swap & (f[i] ^ g[i]);
            f[i] ^= (small) t;
            g[i] ^= (small) t;
            t = swap & (v[i] ^ r[i]);
            v[i] ^= (small) t;
            r[i] ^= (small) t;
        }

        for (i = 0; i < p + 1; ++i) {
            g[i] = F3_freeze((int16) (g[i] + sign * f[i]));
        }
        for (i = 0; i < p + 1; ++i) {
            r[i] = F3_freeze((int16) (r[i] + sign * v[i]));
        }

        for (i = 0; i < p; ++i) {
            g[i] = g[i + 1];
        }
        g[p] = (int16) 0;
    }

    sign = (int) f[0];
    for (i = 0; i < p; ++i) {
        out[i] = (small) (sign * v[p - 1 - i]);
    }

    out[p] = (small) int16_nonzero_mask((int16) delta);
    return 0;
}
