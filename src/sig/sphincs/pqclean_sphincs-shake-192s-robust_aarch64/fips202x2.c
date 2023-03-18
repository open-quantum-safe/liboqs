#include <assert.h>
#include <stdint.h>

#include "fips202x2.h"
#include "f1600x2.h"
#include "fips202.h"

uint64_t load64(const unsigned char *x) {
    unsigned long long r = 0, i;

    for (i = 0; i < 8; ++i) {
        r |= (unsigned long long)x[i] << 8 * i;
    }
    return r;
}

void store64(uint8_t *x, uint64_t u) {
    unsigned int i;

    for (i = 0; i < 8; ++i) {
        x[i] = (uint8_t)u;
        u >>= 8;
    }
}

static void keccak_absorb2x(uint64_t *s,
                            unsigned int r,
                            const unsigned char *m0,
                            const unsigned char *m1,
                            unsigned long long int mlen,
                            unsigned char p) {
    unsigned long long i;
    unsigned char t0[200];
    unsigned char t1[200];

    while (mlen >= r) {
        for (i = 0; i < r / 8; ++i) {
            s[2 * i + 0] ^= load64(m0 + 8 * i);
            s[2 * i + 1] ^= load64(m1 + 8 * i);
        }

        f1600x2(s);
        mlen -= r;
        m0 += r;
        m1 += r;
    }

    for (i = 0; i < r; ++i) {
        t0[i] = 0;
        t1[i] = 0;
    }
    for (i = 0; i < mlen; ++i) {
        t0[i] = m0[i];
        t1[i] = m1[i];
    }

    t0[i] = p;
    t1[i] = p;

    t0[r - 1] |= 128;
    t1[r - 1] |= 128;

    for (i = 0; i < r / 8; ++i) {
        s[2 * i + 0] ^= load64(t0 + 8 * i);
        s[2 * i + 1] ^= load64(t1 + 8 * i);
    }
}


static void keccak_squeezeblocks2x(unsigned char *h0,
                                   unsigned char *h1,
                                   unsigned long long int nblocks,
                                   uint64_t *s,
                                   unsigned int r) {
    unsigned int i;

    while (nblocks > 0) {
        f1600x2(s);
        for (i = 0; i < (r >> 3); i++) {
            store64(h0 + 8 * i, s[2 * i + 0]);
            store64(h1 + 8 * i, s[2 * i + 1]);
        }
        h0 += r;
        h1 += r;
        nblocks--;
    }
}



void shake128x2(unsigned char *out0,
                unsigned char *out1,
                unsigned long long outlen,
                unsigned char *in0,
                unsigned char *in1,
                unsigned long long inlen) {
    uint64_t s[50] = {0};
    unsigned char t0[SHAKE128_RATE];
    unsigned char t1[SHAKE128_RATE];
    unsigned int i;

    /* absorb 4 message of identical length in parallel */
    keccak_absorb2x(s, SHAKE128_RATE, in0, in1, inlen, 0x1F);

    /* Squeeze output */
    keccak_squeezeblocks2x(out0, out1, outlen / SHAKE128_RATE, s, SHAKE128_RATE);

    out0 += (outlen / SHAKE128_RATE) * SHAKE128_RATE;
    out1 += (outlen / SHAKE128_RATE) * SHAKE128_RATE;

    if (outlen % SHAKE128_RATE) {
        keccak_squeezeblocks2x(t0, t1, 1, s, SHAKE128_RATE);
        for (i = 0; i < outlen % SHAKE128_RATE; i++) {
            out0[i] = t0[i];
            out1[i] = t1[i];
        }
    }
}


void shake256x2(unsigned char *out0,
                unsigned char *out1,
                unsigned long long outlen,
                unsigned char *in0,
                unsigned char *in1,
                unsigned long long inlen) {
    uint64_t s[50] = {0};
    unsigned char t0[SHAKE256_RATE];
    unsigned char t1[SHAKE256_RATE];
    unsigned int i;

    /* absorb 2 message of identical length in parallel */
    keccak_absorb2x(s, SHAKE256_RATE, in0, in1, inlen, 0x1F);

    /* Squeeze output */
    keccak_squeezeblocks2x(out0, out1, outlen / SHAKE256_RATE, s, SHAKE256_RATE);

    out0 += (outlen / SHAKE256_RATE) * SHAKE256_RATE;
    out1 += (outlen / SHAKE256_RATE) * SHAKE256_RATE;

    if (outlen % SHAKE256_RATE) {
        keccak_squeezeblocks2x(t0, t1, 1, s, SHAKE256_RATE);
        for (i = 0; i < outlen % SHAKE256_RATE; i++) {
            out0[i] = t0[i];
            out1[i] = t1[i];
        }
    }
}
