#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "fips202.h"
#include "gf31.h"
#include "params.h"

/* This performs a full unique reduction mod 13 on x; x can be any unsigned
   16-bit integer (i.e. in the range [0, 65535]) */
gf31 PQCLEAN_MQDSS48_CLEAN_mod31(gf31 x) {
    gf31 t;

    t = (gf31)(x & 31);
    x >>= 5;
    t = (gf31)(t + (x & 31));
    x >>= 5;
    t = (gf31)(t + (x & 31));
    x >>= 5;
    t = (gf31)(t + (x & 31));

    t = (gf31)((t >> 5) + (t & 31));
    t = (gf31)((t >> 5) + (t & 31));
    return (gf31)((t != 31) * t);
}

/* Given a vector of N elements in the range [0, 31], this reduces the elements
   to the range [0, 30] by mapping 31 to 0 (i.e reduction mod 31) */
void PQCLEAN_MQDSS48_CLEAN_vgf31_unique(gf31 *out, const gf31 *in) {
    int i;

    for (i = 0; i < N; i++) {
        out[i] = (gf31)((1 - (in[i] == 31)) * in[i]);
    }
}

/* Given a vector of 16-bit integers (i.e. in [0, 65535], this reduces the
   elements to the range [0, 30] by mapping 31 to 0 (i.e reduction mod 31) */
void PQCLEAN_MQDSS48_CLEAN_vgf31_shorten_unique(gf31 *out, const gf31 *in) {
    int i;

    for (i = 0; i < N; i++) {
        out[i] = PQCLEAN_MQDSS48_CLEAN_mod31(in[i]);
    }
}

/* Given a seed, samples len gf31 elements (in the range [0, 30]), and places
   them in a vector of 16-bit elements */
void PQCLEAN_MQDSS48_CLEAN_gf31_nrand(gf31 *out, int len, const unsigned char *seed, size_t seedlen) {
    int i = 0, j;
    shake256ctx shakestate;
    unsigned char shakeblock[SHAKE256_RATE];

    shake256_absorb(&shakestate, seed, seedlen);

    while (i < len) {
        shake256_squeezeblocks(shakeblock, 1, &shakestate);
        for (j = 0; j < SHAKE256_RATE && i < len; j++) {
            if ((shakeblock[j] & 31) != 31) {
                out[i] = (shakeblock[j] & 31);
                i++;
            }
        }
    }
}

/* Given a seed, samples len gf31 elements, transposed into unsigned range,
   i.e. in the range [-15, 15], and places them in an array of 8-bit integers.
   This is used for the expansion of F, which wants packed elements. */
void PQCLEAN_MQDSS48_CLEAN_gf31_nrand_schar(signed char *out, int len, const unsigned char *seed, size_t seedlen) {
    int i = 0, j;
    shake256ctx shakestate;
    unsigned char shakeblock[SHAKE256_RATE];

    shake256_absorb(&shakestate, seed, seedlen);

    while (i < len) {
        shake256_squeezeblocks(shakeblock, 1, &shakestate);
        for (j = 0; j < SHAKE256_RATE && i < len; j++) {
            if ((shakeblock[j] & 31) != 31) {
                out[i] = (signed char)(((signed char)shakeblock[j] & 31) - 15);
                i++;
            }
        }
    }
}

/* Unpacks an array of packed GF31 elements to one element per gf31.
   Assumes that there is sufficient empty space available at the end of the
   array to unpack. Can perform in-place. */
void PQCLEAN_MQDSS48_CLEAN_gf31_nunpack(gf31 *out, const unsigned char *in, unsigned int n) {
    size_t i;
    unsigned int j = ((n * 5) >> 3) - 1;
    unsigned int d = 0;

    for (i = n; i > 0; i--) {
        out[i - 1] = (gf31)((in[j] >> d) & 31);
        d += 5;
        if (d > 8) {
            d -= 8;
            j--;
            out[i - 1] = (gf31)(out[i - 1] ^ ((in[j] << (5 - d)) & 31));
        }
    }
}

/* Packs an array of GF31 elements from gf31's to concatenated 5-bit values.
   Assumes that there is sufficient space available to unpack.
   Can perform in-place. */
void PQCLEAN_MQDSS48_CLEAN_gf31_npack(unsigned char *out, const gf31 *in, unsigned int n) {
    unsigned int i = 0;
    unsigned int j;
    int d = 3;

    for (j = 0; j < n; j++) {
        assert(in[j] < 31);
    }

    /* There will be ceil(5n / 8) output blocks */
    memset(out, 0, ((5 * n + 7) & (unsigned int)~7) >> 3);

    for (j = 0; j < n; j++) {
        if (d < 0) {
            d += 8;
            out[i] = (unsigned char)((out[i] & (255 << (d - 3))) |
                                     ((in[j] >> (8 - d)) & ~(255 << (d - 3))));
            i++;
        }
        out[i] = (unsigned char)((out[i] & ~(31 << d)) | ((in[j] << d) & (31 << d)));
        d -= 5;
    }
}
