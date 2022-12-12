#include "crypto_core_invsntrup761.h"
#include "params.h"


#define int8 int8_t
#define int16 int16_t
#define int32 int32_t
#define uint16 uint16_t
#define uint32 uint32_t
#define uint64 uint64_t


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

/* ----- arithmetic mod q */

typedef int8 small;

typedef int16 Fq;
/* always represented as -(q-1)/2...(q-1)/2 */

static Fq Fq_bigfreeze(int32 x) {
    x -= q * ((q14 * x) >> 14);
    x -= q * ((q18 * x) >> 18);
    x -= q * ((q27 * x + 67108864) >> 27);
    x -= q * ((q27 * x + 67108864) >> 27);
    return (Fq) x;
}

static Fq Fq_recip(Fq a1) {
    int i = 1;
    Fq ai = a1;

    while (i < q - 2) {
        ai = Fq_bigfreeze(a1 * (int32)ai);
        i += 1;
    }
    return ai;
}

/* ----- polynomials mod q */

/* out = 1/(3*in) in Rq */
/* outbytes[2*p] is 0 if recip succeeded; else -1 */
int PQCLEAN_SNTRUP761_CLEAN_crypto_core_invsntrup761(unsigned char *outbytes, const unsigned char *inbytes) {
    small *in = (void *) inbytes;
    Fq out[p], f[p + 1], g[p + 1], v[p + 1], r[p + 1];
    int i, loop, delta;
    int swap, t;
    int32 f0, g0;
    Fq scale;

    for (i = 0; i < p + 1; ++i) {
        v[i] = 0;
    }
    for (i = 0; i < p + 1; ++i) {
        r[i] = 0;
    }
    r[0] = Fq_recip(3);
    for (i = 0; i < p; ++i) {
        f[i] = 0;
    }
    f[0] = 1;
    f[p - 1] = f[p] = -1;
    for (i = 0; i < p; ++i) {
        g[p - 1 - i] = (Fq) in[i];
    }
    g[p] = 0;

    delta = 1;

    for (loop = 0; loop < 2 * p - 1; ++loop) {
        for (i = p; i > 0; --i) {
            v[i] = v[i - 1];
        }
        v[0] = 0;

        swap = int16_negative_mask((int16) - delta) & int16_nonzero_mask(g[0]);
        delta ^= swap & (delta ^ -delta);
        delta += 1;

        for (i = 0; i < p + 1; ++i) {
            t = swap & (f[i] ^ g[i]);
            f[i] ^= (Fq) t;
            g[i] ^= (Fq) t;
            t = swap & (v[i] ^ r[i]);
            v[i] ^= (Fq) t;
            r[i] ^= (Fq) t;
        }

        f0 = f[0];
        g0 = g[0];
        for (i = 0; i < p + 1; ++i) {
            g[i] = Fq_bigfreeze(f0 * g[i] - g0 * f[i]);
        }
        for (i = 0; i < p + 1; ++i) {
            r[i] = Fq_bigfreeze(f0 * r[i] - g0 * v[i]);
        }

        for (i = 0; i < p; ++i) {
            g[i] = g[i + 1];
        }
        g[p] = 0;
    }

    scale = Fq_recip(f[0]);
    for (i = 0; i < p; ++i) {
        out[i] = Fq_bigfreeze(scale * (int32)v[p - 1 - i]);
    }

    crypto_encode_pxint16(outbytes, out);
    outbytes[2 * p] = (unsigned char) int16_nonzero_mask((int16) delta);
    return 0;
}
