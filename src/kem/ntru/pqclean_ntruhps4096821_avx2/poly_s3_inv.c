#include "poly.h"

#include <immintrin.h>

typedef signed char small;

#define p 820
#define ppad 1024
#define numvec 4

typedef __m256i vec256;

/*
This code stores 1024-coeff poly as vec256[4].
Order of 256 coefficients in each vec256
is optimized in light of costs of vector instructions:
  0,4,...,252 in 64-bit word;
  1,5,...,253 in 64-bit word;
  2,6,...,254 in 64-bit word;
  3,7,...,255 in 64-bit word.
*/

static inline void vec256_frombits(vec256 *v, const small *b) {
    int i;

    for (i = 0; i < numvec; ++i) {
        vec256 b0 = _mm256_loadu_si256((vec256 *) b);
        b += 32; /* 0,1,...,31 */
        vec256 b1 = _mm256_loadu_si256((vec256 *) b);
        b += 32; /* 32,33,... */
        vec256 b2 = _mm256_loadu_si256((vec256 *) b);
        b += 32;
        vec256 b3 = _mm256_loadu_si256((vec256 *) b);
        b += 32;
        vec256 b4 = _mm256_loadu_si256((vec256 *) b);
        b += 32;
        vec256 b5 = _mm256_loadu_si256((vec256 *) b);
        b += 32;
        vec256 b6 = _mm256_loadu_si256((vec256 *) b);
        b += 32;
        vec256 b7 = _mm256_loadu_si256((vec256 *) b);
        b += 32;

        vec256 c0 = _mm256_unpacklo_epi32(b0, b1); /* 0 1 2 3 32 33 34 35 4 5 6 7 36 37 38 39 ... 55 */
        vec256 c1 = _mm256_unpackhi_epi32(b0, b1); /* 8 9 10 11 40 41 42 43 ... 63 */
        vec256 c2 = _mm256_unpacklo_epi32(b2, b3);
        vec256 c3 = _mm256_unpackhi_epi32(b2, b3);
        vec256 c4 = _mm256_unpacklo_epi32(b4, b5);
        vec256 c5 = _mm256_unpackhi_epi32(b4, b5);
        vec256 c6 = _mm256_unpacklo_epi32(b6, b7);
        vec256 c7 = _mm256_unpackhi_epi32(b6, b7);

        vec256 d0 = c0 | _mm256_slli_epi32(c1, 2); /* 0 8, 1 9, 2 10, 3 11, 32 40, 33 41, ..., 55 63 */
        vec256 d2 = c2 | _mm256_slli_epi32(c3, 2);
        vec256 d4 = c4 | _mm256_slli_epi32(c5, 2);
        vec256 d6 = c6 | _mm256_slli_epi32(c7, 2);

        vec256 e0 = _mm256_unpacklo_epi64(d0, d2);
        vec256 e2 = _mm256_unpackhi_epi64(d0, d2);
        vec256 e4 = _mm256_unpacklo_epi64(d4, d6);
        vec256 e6 = _mm256_unpackhi_epi64(d4, d6);

        vec256 f0 = e0 | _mm256_slli_epi32(e2, 1);
        vec256 f4 = e4 | _mm256_slli_epi32(e6, 1);

        vec256 g0 = _mm256_permute2x128_si256(f0, f4, 0x20);
        vec256 g4 = _mm256_permute2x128_si256(f0, f4, 0x31);

        vec256 h = g0 | _mm256_slli_epi32(g4, 4);

#define TRANSPOSE _mm256_set_epi8( 31,27,23,19, 30,26,22,18, 29,25,21,17, 28,24,20,16, 15,11,7,3, 14,10,6,2, 13,9,5,1, 12,8,4,0 )
        h = _mm256_shuffle_epi8(h, TRANSPOSE);
        h = _mm256_permute4x64_epi64(h, 0xd8);
        h = _mm256_shuffle_epi32(h, 0xd8);

        *v++ = h;
    }
}

static inline void vec256_tobits(const vec256 *v, small *b) {
    int i;

    for (i = 0; i < numvec; ++i) {
        vec256 h = *v++;

        h = _mm256_shuffle_epi32(h, 0xd8);
        h = _mm256_permute4x64_epi64(h, 0xd8);
        h = _mm256_shuffle_epi8(h, TRANSPOSE);

        vec256 g0 = h & _mm256_set1_epi8(15);
        vec256 g4 = _mm256_srli_epi32(h, 4) & _mm256_set1_epi8(15);

        vec256 f0 = _mm256_permute2x128_si256(g0, g4, 0x20);
        vec256 f4 = _mm256_permute2x128_si256(g0, g4, 0x31);

        vec256 e0 = f0 & _mm256_set1_epi8(5);
        vec256 e2 = _mm256_srli_epi32(f0, 1) & _mm256_set1_epi8(5);
        vec256 e4 = f4 & _mm256_set1_epi8(5);
        vec256 e6 = _mm256_srli_epi32(f4, 1) & _mm256_set1_epi8(5);

        vec256 d0 = _mm256_unpacklo_epi32(e0, e2);
        vec256 d2 = _mm256_unpackhi_epi32(e0, e2);
        vec256 d4 = _mm256_unpacklo_epi32(e4, e6);
        vec256 d6 = _mm256_unpackhi_epi32(e4, e6);

        vec256 c0 = d0 & _mm256_set1_epi8(1);
        vec256 c1 = _mm256_srli_epi32(d0, 2) & _mm256_set1_epi8(1);
        vec256 c2 = d2 & _mm256_set1_epi8(1);
        vec256 c3 = _mm256_srli_epi32(d2, 2) & _mm256_set1_epi8(1);
        vec256 c4 = d4 & _mm256_set1_epi8(1);
        vec256 c5 = _mm256_srli_epi32(d4, 2) & _mm256_set1_epi8(1);
        vec256 c6 = d6 & _mm256_set1_epi8(1);
        vec256 c7 = _mm256_srli_epi32(d6, 2) & _mm256_set1_epi8(1);

        vec256 b0 = _mm256_unpacklo_epi64(c0, c1);
        vec256 b1 = _mm256_unpackhi_epi64(c0, c1);
        vec256 b2 = _mm256_unpacklo_epi64(c2, c3);
        vec256 b3 = _mm256_unpackhi_epi64(c2, c3);
        vec256 b4 = _mm256_unpacklo_epi64(c4, c5);
        vec256 b5 = _mm256_unpackhi_epi64(c4, c5);
        vec256 b6 = _mm256_unpacklo_epi64(c6, c7);
        vec256 b7 = _mm256_unpackhi_epi64(c6, c7);

        _mm256_storeu_si256((vec256 *) b, b0);
        b += 32;
        _mm256_storeu_si256((vec256 *) b, b1);
        b += 32;
        _mm256_storeu_si256((vec256 *) b, b2);
        b += 32;
        _mm256_storeu_si256((vec256 *) b, b3);
        b += 32;
        _mm256_storeu_si256((vec256 *) b, b4);
        b += 32;
        _mm256_storeu_si256((vec256 *) b, b5);
        b += 32;
        _mm256_storeu_si256((vec256 *) b, b6);
        b += 32;
        _mm256_storeu_si256((vec256 *) b, b7);
        b += 32;
    }
}

static void vec256_init(vec256 *G0, vec256 *G1, const small *s) {
    int i;
    small srev[ppad + (ppad - p)];
    small si;
    small g0[ppad];
    small g1[ppad];

    for (i = 0; i < p; ++i) {
        srev[ppad - 1 - i] = s[i];
    }
    for (i = 0; i < ppad - p; ++i) {
        srev[i] = 0;
    }
    for (i = p; i < ppad; ++i) {
        srev[i + ppad - p] = 0;
    }

    for (i = 0; i < ppad; ++i) {
        si = srev[i + ppad - p];
        g0[i] = si & 1;
        g1[i] = (si >> 1) & g0[i];
    }

    vec256_frombits(G0, g0);
    vec256_frombits(G1, g1);
}

static void vec256_final(small *out, const vec256 *V0, const vec256 *V1) {
    int i;
    small v0[ppad];
    small v1[ppad];
    small v[ppad];
    small vrev[ppad + (ppad - p)];

    vec256_tobits(V0, v0);
    vec256_tobits(V1, v1);

    for (i = 0; i < ppad; ++i) {
        v[i] = v0[i] + 2 * v1[i] - 4 * (v0[i] & v1[i]);
    }

    for (i = 0; i < ppad; ++i) {
        vrev[i] = v[ppad - 1 - i];
    }
    for (i = ppad; i < ppad + (ppad - p); ++i) {
        vrev[i] = 0;
    }

    for (i = 0; i < p; ++i) {
        out[i] = vrev[i + ppad - p];
    }
}

static inline int negative_mask(int x) {
    return x >> 31;
}

static inline void vec256_swap(vec256 *f, vec256 *g, int len, vec256 mask) {
    vec256 flip;
    int i;

    for (i = 0; i < len; ++i) {
        flip = mask & (f[i] ^ g[i]);
        f[i] ^= flip;
        g[i] ^= flip;
    }
}

static inline void vec256_scale(vec256 *f0, vec256 *f1, const vec256 c0, const vec256 c1) {
    int i;

    for (i = 0; i < numvec; ++i) {
        vec256 f0i = f0[i];
        vec256 f1i = f1[i];

        f0i &= c0;
        f1i ^= c1;
        f1i &= f0i;

        f0[i] = f0i;
        f1[i] = f1i;
    }
}

static inline void vec256_eliminate(vec256 *f0, vec256 *f1, vec256 *g0, vec256 *g1, int len, const vec256 c0, const vec256 c1) {
    int i;

    for (i = 0; i < len; ++i) {
        vec256 f0i = f0[i];
        vec256 f1i = f1[i];
        vec256 g0i = g0[i];
        vec256 g1i = g1[i];
        vec256 t;

        f0i &= c0;
        f1i ^= c1;
        f1i &= f0i;

        t = g0i ^ f0i;
        g0[i] = t | (g1i ^ f1i);
        g1[i] = (g1i ^ f0i) & (f1i ^ t);
    }
}

static inline int vec256_bit0mask(vec256 *f) {
    return -(_mm_cvtsi128_si32(_mm256_castsi256_si128(f[0])) & 1);
}

static inline void vec256_divx_1(vec256 *f) {
    vec256 f0 = f[0];

    unsigned long long low0 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f0));

    low0 = low0 >> 1;

    f0 = _mm256_blend_epi32(f0, _mm256_set_epi64x(0, 0, 0, low0), 0x3);

    f[0] = _mm256_permute4x64_epi64(f0, 0x39);
}

static inline void vec256_divx_2(vec256 *f) {
    vec256 f0 = f[0];
    vec256 f1 = f[1];

    unsigned long long low0 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f0));
    unsigned long long low1 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f1));

    low0 = (low0 >> 1) | (low1 << 63);
    low1 = low1 >> 1;

    f0 = _mm256_blend_epi32(f0, _mm256_set_epi64x(0, 0, 0, low0), 0x3);
    f1 = _mm256_blend_epi32(f1, _mm256_set_epi64x(0, 0, 0, low1), 0x3);

    f[0] = _mm256_permute4x64_epi64(f0, 0x39);
    f[1] = _mm256_permute4x64_epi64(f1, 0x39);
}

static inline void vec256_divx_3(vec256 *f) {
    vec256 f0 = f[0];
    vec256 f1 = f[1];
    vec256 f2 = f[2];

    unsigned long long low0 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f0));
    unsigned long long low1 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f1));
    unsigned long long low2 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f2));

    low0 = (low0 >> 1) | (low1 << 63);
    low1 = (low1 >> 1) | (low2 << 63);
    low2 = low2 >> 1;

    f0 = _mm256_blend_epi32(f0, _mm256_set_epi64x(0, 0, 0, low0), 0x3);
    f1 = _mm256_blend_epi32(f1, _mm256_set_epi64x(0, 0, 0, low1), 0x3);
    f2 = _mm256_blend_epi32(f2, _mm256_set_epi64x(0, 0, 0, low2), 0x3);

    f[0] = _mm256_permute4x64_epi64(f0, 0x39);
    f[1] = _mm256_permute4x64_epi64(f1, 0x39);
    f[2] = _mm256_permute4x64_epi64(f2, 0x39);
}

static inline void vec256_divx_4(vec256 *f) {
    vec256 f0 = f[0];
    vec256 f1 = f[1];
    vec256 f2 = f[2];
    vec256 f3 = f[3];

    unsigned long long low0 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f0));
    unsigned long long low1 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f1));
    unsigned long long low2 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f2));
    unsigned long long low3 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f3));

    low0 = (low0 >> 1) | (low1 << 63);
    low1 = (low1 >> 1) | (low2 << 63);
    low2 = (low2 >> 1) | (low3 << 63);
    low3 = low3 >> 1;

    f0 = _mm256_blend_epi32(f0, _mm256_set_epi64x(0, 0, 0, low0), 0x3);
    f1 = _mm256_blend_epi32(f1, _mm256_set_epi64x(0, 0, 0, low1), 0x3);
    f2 = _mm256_blend_epi32(f2, _mm256_set_epi64x(0, 0, 0, low2), 0x3);
    f3 = _mm256_blend_epi32(f3, _mm256_set_epi64x(0, 0, 0, low3), 0x3);

    f[0] = _mm256_permute4x64_epi64(f0, 0x39);
    f[1] = _mm256_permute4x64_epi64(f1, 0x39);
    f[2] = _mm256_permute4x64_epi64(f2, 0x39);
    f[3] = _mm256_permute4x64_epi64(f3, 0x39);
}

static inline void vec256_timesx_1(vec256 *f) {
    vec256 f0 = _mm256_permute4x64_epi64(f[0], 0x93);

    unsigned long long low0 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f0));

    low0 = low0 << 1;

    f0 = _mm256_blend_epi32(f0, _mm256_set_epi64x(0, 0, 0, low0), 0x3);

    f[0] = f0;
}

static inline void vec256_timesx_2(vec256 *f) {
    vec256 f0 = _mm256_permute4x64_epi64(f[0], 0x93);
    vec256 f1 = _mm256_permute4x64_epi64(f[1], 0x93);

    unsigned long long low0 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f0));
    unsigned long long low1 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f1));

    low1 = (low1 << 1) | (low0 >> 63);
    low0 = low0 << 1;

    f0 = _mm256_blend_epi32(f0, _mm256_set_epi64x(0, 0, 0, low0), 0x3);
    f1 = _mm256_blend_epi32(f1, _mm256_set_epi64x(0, 0, 0, low1), 0x3);

    f[0] = f0;
    f[1] = f1;
}

static inline void vec256_timesx_3(vec256 *f) {
    vec256 f0 = _mm256_permute4x64_epi64(f[0], 0x93);
    vec256 f1 = _mm256_permute4x64_epi64(f[1], 0x93);
    vec256 f2 = _mm256_permute4x64_epi64(f[2], 0x93);

    unsigned long long low0 = *(unsigned long long *) &f0;
    unsigned long long low1 = *(unsigned long long *) &f1;
    unsigned long long low2 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f2));

    low2 = (low2 << 1) | (low1 >> 63);
    low1 = (low1 << 1) | (low0 >> 63);
    low0 = low0 << 1;

    *(unsigned long long *) &f0 = low0;
    *(unsigned long long *) &f1 = low1;
    f2 = _mm256_blend_epi32(f2, _mm256_set_epi64x(0, 0, 0, low2), 0x3);

    f[0] = f0;
    f[1] = f1;
    f[2] = f2;
}

static inline void vec256_timesx_4(vec256 *f) {
    vec256 f0 = _mm256_permute4x64_epi64(f[0], 0x93);
    vec256 f1 = _mm256_permute4x64_epi64(f[1], 0x93);
    vec256 f2 = _mm256_permute4x64_epi64(f[2], 0x93);
    vec256 f3 = _mm256_permute4x64_epi64(f[3], 0x93);

    unsigned long long low0 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f0));
    unsigned long long low1 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f1));
    unsigned long long low2 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f2));
    unsigned long long low3 = _mm_cvtsi128_si64(_mm256_castsi256_si128(f3));

    low3 = (low3 << 1) | (low2 >> 63);
    low2 = (low2 << 1) | (low1 >> 63);
    low1 = (low1 << 1) | (low0 >> 63);
    low0 = low0 << 1;

    f0 = _mm256_blend_epi32(f0, _mm256_set_epi64x(0, 0, 0, low0), 0x3);
    f1 = _mm256_blend_epi32(f1, _mm256_set_epi64x(0, 0, 0, low1), 0x3);
    f2 = _mm256_blend_epi32(f2, _mm256_set_epi64x(0, 0, 0, low2), 0x3);
    f3 = _mm256_blend_epi32(f3, _mm256_set_epi64x(0, 0, 0, low3), 0x3);

    f[0] = f0;
    f[1] = f1;
    f[2] = f2;
    f[3] = f3;
}


static int __poly_S3_inv(unsigned char *outbytes, const unsigned char *inbytes) {
    small *out = (void *) outbytes;
    small *in = (void *) inbytes;
    vec256 F0[numvec];
    vec256 F1[numvec];
    vec256 G0[numvec];
    vec256 G1[numvec];
    vec256 V0[numvec];
    vec256 V1[numvec];
    vec256 R0[numvec];
    vec256 R1[numvec];
    vec256 c0vec, c1vec;
    int loop;
    int c0, c1;
    int minusdelta = -1;
    int swapmask;
    vec256 swapvec;

    vec256_init(G0, G1, in);
    F0[0] = _mm256_set_epi32(-1, -1, -1, -1, -1, -1, -1, -1);
    F0[1] = _mm256_set_epi32(-1, -1, -1, -1, -1, -1, -1, -1);
    F0[2] = _mm256_set_epi32(-1, -1, -1, -1, -1, -1, -1, -1);
    F0[3] = _mm256_set_epi32(0, 8191, 0, 8191, 0, 8191, 0, 16383);
    F1[0] = _mm256_set1_epi32(0);
    F1[1] = _mm256_set1_epi32(0);
    F1[2] = _mm256_set1_epi32(0);
    F1[3] = _mm256_set1_epi32(0);

    V0[0] = _mm256_set1_epi32(0);
    V1[0] = _mm256_set1_epi32(0);
    V0[1] = _mm256_set1_epi32(0);
    V1[1] = _mm256_set1_epi32(0);
    V0[2] = _mm256_set1_epi32(0);
    V1[2] = _mm256_set1_epi32(0);
    V0[3] = _mm256_set1_epi32(0);
    V1[3] = _mm256_set1_epi32(0);

    R0[0] = _mm256_set_epi32(0, 0, 0, 0, 0, 0, 0, 1);
    R1[0] = _mm256_set1_epi32(0);
    R0[1] = _mm256_set1_epi32(0);
    R1[1] = _mm256_set1_epi32(0);
    R0[2] = _mm256_set1_epi32(0);
    R1[2] = _mm256_set1_epi32(0);
    R0[3] = _mm256_set1_epi32(0);
    R1[3] = _mm256_set1_epi32(0);

    for (loop = 256; loop > 0; --loop) {
        vec256_timesx_1(V0);
        vec256_timesx_1(V1);
        swapmask = negative_mask(minusdelta) & vec256_bit0mask(G0);

        c0 = vec256_bit0mask(F0) & vec256_bit0mask(G0);
        c1 = vec256_bit0mask(F1) ^ vec256_bit0mask(G1);
        c1 &= c0;

        minusdelta ^= swapmask & (minusdelta ^ -minusdelta);
        minusdelta -= 1;

        swapvec = _mm256_set1_epi32(swapmask);
        vec256_swap(F0, G0, 4, swapvec);
        vec256_swap(F1, G1, 4, swapvec);

        c0vec = _mm256_set1_epi32(c0);
        c1vec = _mm256_set1_epi32(c1);

        vec256_eliminate(F0, F1, G0, G1, 4, c0vec, c1vec);
        vec256_divx_4(G0);
        vec256_divx_4(G1);

        vec256_swap(V0, R0, 1, swapvec);
        vec256_swap(V1, R1, 1, swapvec);
        vec256_eliminate(V0, V1, R0, R1, 1, c0vec, c1vec);
    }

    for (loop = 256; loop > 0; --loop) {
        vec256_timesx_2(V0);
        vec256_timesx_2(V1);
        swapmask = negative_mask(minusdelta) & vec256_bit0mask(G0);

        c0 = vec256_bit0mask(F0) & vec256_bit0mask(G0);
        c1 = vec256_bit0mask(F1) ^ vec256_bit0mask(G1);
        c1 &= c0;

        minusdelta ^= swapmask & (minusdelta ^ -minusdelta);
        minusdelta -= 1;

        swapvec = _mm256_set1_epi32(swapmask);
        vec256_swap(F0, G0, 4, swapvec);
        vec256_swap(F1, G1, 4, swapvec);

        c0vec = _mm256_set1_epi32(c0);
        c1vec = _mm256_set1_epi32(c1);

        vec256_eliminate(F0, F1, G0, G1, 4, c0vec, c1vec);
        vec256_divx_4(G0);
        vec256_divx_4(G1);

        vec256_swap(V0, R0, 2, swapvec);
        vec256_swap(V1, R1, 2, swapvec);
        vec256_eliminate(V0, V1, R0, R1, 2, c0vec, c1vec);
    }

    for (loop = 256; loop > 0; --loop) {
        vec256_timesx_3(V0);
        vec256_timesx_3(V1);
        swapmask = negative_mask(minusdelta) & vec256_bit0mask(G0);

        c0 = vec256_bit0mask(F0) & vec256_bit0mask(G0);
        c1 = vec256_bit0mask(F1) ^ vec256_bit0mask(G1);
        c1 &= c0;

        minusdelta ^= swapmask & (minusdelta ^ -minusdelta);
        minusdelta -= 1;

        swapvec = _mm256_set1_epi32(swapmask);
        vec256_swap(F0, G0, 4, swapvec);
        vec256_swap(F1, G1, 4, swapvec);

        c0vec = _mm256_set1_epi32(c0);
        c1vec = _mm256_set1_epi32(c1);

        vec256_eliminate(F0, F1, G0, G1, 4, c0vec, c1vec);
        vec256_divx_4(G0);
        vec256_divx_4(G1);

        vec256_swap(V0, R0, 3, swapvec);
        vec256_swap(V1, R1, 3, swapvec);
        vec256_eliminate(V0, V1, R0, R1, 3, c0vec, c1vec);
    }

    for (loop = 103; loop > 0; --loop) {
        vec256_timesx_4(V0);
        vec256_timesx_4(V1);
        swapmask = negative_mask(minusdelta) & vec256_bit0mask(G0);

        c0 = vec256_bit0mask(F0) & vec256_bit0mask(G0);
        c1 = vec256_bit0mask(F1) ^ vec256_bit0mask(G1);
        c1 &= c0;

        minusdelta ^= swapmask & (minusdelta ^ -minusdelta);
        minusdelta -= 1;

        swapvec = _mm256_set1_epi32(swapmask);
        vec256_swap(F0, G0, 4, swapvec);
        vec256_swap(F1, G1, 4, swapvec);

        c0vec = _mm256_set1_epi32(c0);
        c1vec = _mm256_set1_epi32(c1);

        vec256_eliminate(F0, F1, G0, G1, 4, c0vec, c1vec);
        vec256_divx_4(G0);
        vec256_divx_4(G1);

        vec256_swap(V0, R0, 4, swapvec);
        vec256_swap(V1, R1, 4, swapvec);
        vec256_eliminate(V0, V1, R0, R1, 4, c0vec, c1vec);
    }

    for (loop = 256; loop > 0; --loop) {
        vec256_timesx_4(V0);
        vec256_timesx_4(V1);
        swapmask = negative_mask(minusdelta) & vec256_bit0mask(G0);

        c0 = vec256_bit0mask(F0) & vec256_bit0mask(G0);
        c1 = vec256_bit0mask(F1) ^ vec256_bit0mask(G1);
        c1 &= c0;

        minusdelta ^= swapmask & (minusdelta ^ -minusdelta);
        minusdelta -= 1;

        swapvec = _mm256_set1_epi32(swapmask);
        vec256_swap(F0, G0, 3, swapvec);
        vec256_swap(F1, G1, 3, swapvec);

        c0vec = _mm256_set1_epi32(c0);
        c1vec = _mm256_set1_epi32(c1);

        vec256_eliminate(F0, F1, G0, G1, 3, c0vec, c1vec);
        vec256_divx_3(G0);
        vec256_divx_3(G1);

        vec256_swap(V0, R0, 4, swapvec);
        vec256_swap(V1, R1, 4, swapvec);
        vec256_eliminate(V0, V1, R0, R1, 4, c0vec, c1vec);
    }

    for (loop = 256; loop > 0; --loop) {
        vec256_timesx_4(V0);
        vec256_timesx_4(V1);
        swapmask = negative_mask(minusdelta) & vec256_bit0mask(G0);

        c0 = vec256_bit0mask(F0) & vec256_bit0mask(G0);
        c1 = vec256_bit0mask(F1) ^ vec256_bit0mask(G1);
        c1 &= c0;

        minusdelta ^= swapmask & (minusdelta ^ -minusdelta);
        minusdelta -= 1;

        swapvec = _mm256_set1_epi32(swapmask);
        vec256_swap(F0, G0, 2, swapvec);
        vec256_swap(F1, G1, 2, swapvec);

        c0vec = _mm256_set1_epi32(c0);
        c1vec = _mm256_set1_epi32(c1);

        vec256_eliminate(F0, F1, G0, G1, 2, c0vec, c1vec);
        vec256_divx_2(G0);
        vec256_divx_2(G1);

        vec256_swap(V0, R0, 4, swapvec);
        vec256_swap(V1, R1, 4, swapvec);
        vec256_eliminate(V0, V1, R0, R1, 4, c0vec, c1vec);
    }

    for (loop = 256; loop > 0; --loop) {
        vec256_timesx_4(V0);
        vec256_timesx_4(V1);
        swapmask = negative_mask(minusdelta) & vec256_bit0mask(G0);

        c0 = vec256_bit0mask(F0) & vec256_bit0mask(G0);
        c1 = vec256_bit0mask(F1) ^ vec256_bit0mask(G1);
        c1 &= c0;

        minusdelta ^= swapmask & (minusdelta ^ -minusdelta);
        minusdelta -= 1;

        swapvec = _mm256_set1_epi32(swapmask);
        vec256_swap(F0, G0, 1, swapvec);
        vec256_swap(F1, G1, 1, swapvec);

        c0vec = _mm256_set1_epi32(c0);
        c1vec = _mm256_set1_epi32(c1);

        vec256_eliminate(F0, F1, G0, G1, 1, c0vec, c1vec);
        vec256_divx_1(G0);
        vec256_divx_1(G1);

        vec256_swap(V0, R0, 4, swapvec);
        vec256_swap(V1, R1, 4, swapvec);
        vec256_eliminate(V0, V1, R0, R1, 4, c0vec, c1vec);
    }

    c0vec = _mm256_set1_epi32(vec256_bit0mask(F0));
    c1vec = _mm256_set1_epi32(vec256_bit0mask(F1));
    vec256_scale(V0, V1, c0vec, c1vec);

    vec256_final(out, V0, V1);
    out[p] = negative_mask(minusdelta);
    return 0;
}

// This code is based on crypto_core/invhrss701/faster from SUPERCOP. The code was written as a case study
// for the paper "Fast constant-time gcd computation and modular inversion" by Daniel J. Bernstein and Bo-Yin Yang.
void PQCLEAN_NTRUHPS4096821_AVX2_poly_S3_inv(poly *r_out, const poly *a) {
    const unsigned char *in = (void *) a;
    unsigned char *out = (void *) r_out;

    small input[ppad];
    small output[ppad];
    int i;

    /* XXX: obviously input/output format should be packed into bytes */

    for (i = 0; i < p; ++i) {
        small x = in[2 * i] & 3; /* 0 1 2 3 */
        x += 1; /* 0 1 2 3 4 5 6, offset by 1 */
        x &= (x - 3) >> 5; /* 0 1 2, offset by 1 */
        input[i] = x - 1;
    }
    /* XXX: merge with vec256_init */

    __poly_S3_inv((unsigned char *)output, (unsigned char *)input);

    for (i = 0; i < p; ++i) {
        out[2 * i] = (3 & output[i]) ^ ((3 & output[i]) >> 1);
        out[2 * i + 1] = 0;
    }
}
