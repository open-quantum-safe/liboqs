#include "crypto_core_mult3sntrup1277.h"
#include "crypto_core_multsntrup1277_ntt.h"
#include "crypto_decode_1277xint16.h"
#include "crypto_encode_1277xint16.h"
#include <immintrin.h>

typedef int8_t int8;
typedef int16_t int16;

#define int16x16 __m256i
#define load_x16(p) _mm256_loadu_si256((int16x16 *) (p))
#define store_x16(p,v) _mm256_storeu_si256((int16x16 *) (p),(v))
#define const_x16 _mm256_set1_epi16
#define add_x16 _mm256_add_epi16
#define sub_x16 _mm256_sub_epi16
#define mullo_x16 _mm256_mullo_epi16
#define mulhi_x16 _mm256_mulhi_epi16
#define mulhrs_x16 _mm256_mulhrs_epi16
#define signmask_x16(x) _mm256_srai_epi16((x),15)

typedef union {
    int16 v[10][512];
    int16x16 _dummy;
} vec10x512;

typedef union {
    int16 v[1280];
    int16x16 _dummy;
} vec1280;

typedef union {
    int16 v[2560];
    int16x16 _dummy;
} vec2560;

static int16x16 squeeze_3_x16(int16x16 x) {
    return sub_x16(x, mullo_x16(mulhrs_x16(x, const_x16(10923)), const_x16(3)));
}

static int16x16 squeeze_7681_x16(int16x16 x) {
    return sub_x16(x, mullo_x16(mulhrs_x16(x, const_x16(4)), const_x16(7681)));
}

static int16x16 mulmod_7681_x16(int16x16 x, int16x16 y) {
    int16x16 yqinv = mullo_x16(y, const_x16(-7679)); /* XXX: precompute */
    int16x16 b = mulhi_x16(x, y);
    int16x16 d = mullo_x16(x, yqinv);
    int16x16 e = mulhi_x16(d, const_x16(7681));
    return sub_x16(b, e);
}

#define mask0 _mm256_set_epi16(-1,0,0,0,0,-1,0,0,0,0,-1,0,0,0,0,-1)
#define mask1 _mm256_set_epi16(0,0,-1,0,0,0,0,-1,0,0,0,0,-1,0,0,0)
#define mask2 _mm256_set_epi16(0,0,0,0,-1,0,0,0,0,-1,0,0,0,0,-1,0)
#define mask3 _mm256_set_epi16(0,-1,0,0,0,0,-1,0,0,0,0,-1,0,0,0,0)
#define mask4 _mm256_set_epi16(0,0,0,-1,0,0,0,0,-1,0,0,0,0,-1,0,0)

static void good(int16 fpad[5][512], const int16 f[1280]) {
    int j;
    int16x16 f0, f1, f2;

    j = 0;
    for (;;) {
        f0 = load_x16(f + j);
        f1 = load_x16(f + 512 + j);
        f2 = load_x16(f + 1024 + j);
        store_x16(&fpad[0][j], (f0 & mask0) | (f1 & mask1) | (f2 & mask2));
        store_x16(&fpad[1][j], (f0 & mask1) | (f1 & mask2) | (f2 & mask3));
        store_x16(&fpad[2][j], (f0 & mask2) | (f1 & mask3) | (f2 & mask4));
        store_x16(&fpad[3][j], (f0 & mask3) | (f1 & mask4) | (f2 & mask0));
        store_x16(&fpad[4][j], (f0 & mask4) | (f1 & mask0) | (f2 & mask1));
        j += 16;
        if (j == 256) {
            break;
        }

        f0 = load_x16(f + j);
        f1 = load_x16(f + 512 + j);
        f2 = load_x16(f + 1024 + j);
        store_x16(&fpad[0][j], (f0 & mask3) | (f1 & mask4) | (f2 & mask0));
        store_x16(&fpad[1][j], (f0 & mask4) | (f1 & mask0) | (f2 & mask1));
        store_x16(&fpad[2][j], (f0 & mask0) | (f1 & mask1) | (f2 & mask2));
        store_x16(&fpad[3][j], (f0 & mask1) | (f1 & mask2) | (f2 & mask3));
        store_x16(&fpad[4][j], (f0 & mask2) | (f1 & mask3) | (f2 & mask4));
        j += 16;

        f0 = load_x16(f + j);
        f1 = load_x16(f + 512 + j);
        f2 = load_x16(f + 1024 + j);
        store_x16(&fpad[0][j], (f0 & mask1) | (f1 & mask2) | (f2 & mask3));
        store_x16(&fpad[1][j], (f0 & mask2) | (f1 & mask3) | (f2 & mask4));
        store_x16(&fpad[2][j], (f0 & mask3) | (f1 & mask4) | (f2 & mask0));
        store_x16(&fpad[3][j], (f0 & mask4) | (f1 & mask0) | (f2 & mask1));
        store_x16(&fpad[4][j], (f0 & mask0) | (f1 & mask1) | (f2 & mask2));
        j += 16;

        f0 = load_x16(f + j);
        f1 = load_x16(f + 512 + j);
        f2 = load_x16(f + 1024 + j);
        store_x16(&fpad[0][j], (f0 & mask4) | (f1 & mask0) | (f2 & mask1));
        store_x16(&fpad[1][j], (f0 & mask0) | (f1 & mask1) | (f2 & mask2));
        store_x16(&fpad[2][j], (f0 & mask1) | (f1 & mask2) | (f2 & mask3));
        store_x16(&fpad[3][j], (f0 & mask2) | (f1 & mask3) | (f2 & mask4));
        store_x16(&fpad[4][j], (f0 & mask3) | (f1 & mask4) | (f2 & mask0));
        j += 16;

        f0 = load_x16(f + j);
        f1 = load_x16(f + 512 + j);
        f2 = load_x16(f + 1024 + j);
        store_x16(&fpad[0][j], (f0 & mask2) | (f1 & mask3) | (f2 & mask4));
        store_x16(&fpad[1][j], (f0 & mask3) | (f1 & mask4) | (f2 & mask0));
        store_x16(&fpad[2][j], (f0 & mask4) | (f1 & mask0) | (f2 & mask1));
        store_x16(&fpad[3][j], (f0 & mask0) | (f1 & mask1) | (f2 & mask2));
        store_x16(&fpad[4][j], (f0 & mask1) | (f1 & mask2) | (f2 & mask3));
        j += 16;
    }
    for (;;) {
        f0 = load_x16(f + j);
        f1 = load_x16(f + 512 + j);
        store_x16(&fpad[0][j], (f0 & mask3) | (f1 & mask4));
        store_x16(&fpad[1][j], (f0 & mask4) | (f1 & mask0));
        store_x16(&fpad[2][j], (f0 & mask0) | (f1 & mask1));
        store_x16(&fpad[3][j], (f0 & mask1) | (f1 & mask2));
        store_x16(&fpad[4][j], (f0 & mask2) | (f1 & mask3));
        j += 16;
        if (j == 512) {
            break;
        }

        f0 = load_x16(f + j);
        f1 = load_x16(f + 512 + j);
        store_x16(&fpad[0][j], (f0 & mask1) | (f1 & mask2));
        store_x16(&fpad[1][j], (f0 & mask2) | (f1 & mask3));
        store_x16(&fpad[2][j], (f0 & mask3) | (f1 & mask4));
        store_x16(&fpad[3][j], (f0 & mask4) | (f1 & mask0));
        store_x16(&fpad[4][j], (f0 & mask0) | (f1 & mask1));
        j += 16;

        f0 = load_x16(f + j);
        f1 = load_x16(f + 512 + j);
        store_x16(&fpad[0][j], (f0 & mask4) | (f1 & mask0));
        store_x16(&fpad[1][j], (f0 & mask0) | (f1 & mask1));
        store_x16(&fpad[2][j], (f0 & mask1) | (f1 & mask2));
        store_x16(&fpad[3][j], (f0 & mask2) | (f1 & mask3));
        store_x16(&fpad[4][j], (f0 & mask3) | (f1 & mask4));
        j += 16;

        f0 = load_x16(f + j);
        f1 = load_x16(f + 512 + j);
        store_x16(&fpad[0][j], (f0 & mask2) | (f1 & mask3));
        store_x16(&fpad[1][j], (f0 & mask3) | (f1 & mask4));
        store_x16(&fpad[2][j], (f0 & mask4) | (f1 & mask0));
        store_x16(&fpad[3][j], (f0 & mask0) | (f1 & mask1));
        store_x16(&fpad[4][j], (f0 & mask1) | (f1 & mask2));
        j += 16;

        f0 = load_x16(f + j);
        f1 = load_x16(f + 512 + j);
        store_x16(&fpad[0][j], (f0 & mask0) | (f1 & mask1));
        store_x16(&fpad[1][j], (f0 & mask1) | (f1 & mask2));
        store_x16(&fpad[2][j], (f0 & mask2) | (f1 & mask3));
        store_x16(&fpad[3][j], (f0 & mask3) | (f1 & mask4));
        store_x16(&fpad[4][j], (f0 & mask4) | (f1 & mask0));
        j += 16;
    }
}

static void ungood(int16 f[2560], const int16 fpad[5][512]) {
    int j;
    int16x16 f0, f1, f2, f3, f4, g0, g1, g2, g3, g4;

    j = 0;

    for (;;) {
        f0 = load_x16(&fpad[0][j]);
        f1 = load_x16(&fpad[1][j]);
        f2 = load_x16(&fpad[2][j]);
        f3 = load_x16(&fpad[3][j]);
        f4 = load_x16(&fpad[4][j]);
        g0 = (f0 & mask0) | (f1 & mask1) | (f2 & mask2) | (f3 & mask3) | (f4 & mask4);
        g1 = (f0 & mask1) | (f1 & mask2) | (f2 & mask3) | (f3 & mask4) | (f4 & mask0);
        g2 = (f0 & mask2) | (f1 & mask3) | (f2 & mask4) | (f3 & mask0) | (f4 & mask1);
        g3 = (f0 & mask3) | (f1 & mask4) | (f2 & mask0) | (f3 & mask1) | (f4 & mask2);
        g4 = f0 ^ f1 ^ f2 ^ f3 ^ f4 ^ g0 ^ g1 ^ g2 ^ g3; /* same as (f0&mask4)|(f1&mask0)|(f2&mask1)|(f3&mask2)|(f4&mask3); */
        store_x16(f + 0 + j, g0);
        store_x16(f + 512 + j, g1);
        store_x16(f + 1024 + j, g2);
        store_x16(f + 1536 + j, g3);
        store_x16(f + 2048 + j, g4);
        j += 16;

        f0 = load_x16(&fpad[0][j]);
        f1 = load_x16(&fpad[1][j]);
        f2 = load_x16(&fpad[2][j]);
        f3 = load_x16(&fpad[3][j]);
        f4 = load_x16(&fpad[4][j]);
        g0 = (f0 & mask3) | (f1 & mask4) | (f2 & mask0) | (f3 & mask1) | (f4 & mask2);
        g1 = (f0 & mask4) | (f1 & mask0) | (f2 & mask1) | (f3 & mask2) | (f4 & mask3);
        g2 = (f0 & mask0) | (f1 & mask1) | (f2 & mask2) | (f3 & mask3) | (f4 & mask4);
        g3 = (f0 & mask1) | (f1 & mask2) | (f2 & mask3) | (f3 & mask4) | (f4 & mask0);
        g4 = f0 ^ f1 ^ f2 ^ f3 ^ f4 ^ g0 ^ g1 ^ g2 ^ g3;
        store_x16(f + 0 + j, g0);
        store_x16(f + 512 + j, g1);
        store_x16(f + 1024 + j, g2);
        store_x16(f + 1536 + j, g3);
        store_x16(f + 2048 + j, g4);
        j += 16;
        if (j == 512) {
            break;
        }

        f0 = load_x16(&fpad[0][j]);
        f1 = load_x16(&fpad[1][j]);
        f2 = load_x16(&fpad[2][j]);
        f3 = load_x16(&fpad[3][j]);
        f4 = load_x16(&fpad[4][j]);
        g0 = (f0 & mask1) | (f1 & mask2) | (f2 & mask3) | (f3 & mask4) | (f4 & mask0);
        g1 = (f0 & mask2) | (f1 & mask3) | (f2 & mask4) | (f3 & mask0) | (f4 & mask1);
        g2 = (f0 & mask3) | (f1 & mask4) | (f2 & mask0) | (f3 & mask1) | (f4 & mask2);
        g3 = (f0 & mask4) | (f1 & mask0) | (f2 & mask1) | (f3 & mask2) | (f4 & mask3);
        g4 = f0 ^ f1 ^ f2 ^ f3 ^ f4 ^ g0 ^ g1 ^ g2 ^ g3;
        store_x16(f + 0 + j, g0);
        store_x16(f + 512 + j, g1);
        store_x16(f + 1024 + j, g2);
        store_x16(f + 1536 + j, g3);
        store_x16(f + 2048 + j, g4);
        j += 16;

        f0 = load_x16(&fpad[0][j]);
        f1 = load_x16(&fpad[1][j]);
        f2 = load_x16(&fpad[2][j]);
        f3 = load_x16(&fpad[3][j]);
        f4 = load_x16(&fpad[4][j]);
        g0 = (f0 & mask4) | (f1 & mask0) | (f2 & mask1) | (f3 & mask2) | (f4 & mask3);
        g1 = (f0 & mask0) | (f1 & mask1) | (f2 & mask2) | (f3 & mask3) | (f4 & mask4);
        g2 = (f0 & mask1) | (f1 & mask2) | (f2 & mask3) | (f3 & mask4) | (f4 & mask0);
        g3 = (f0 & mask2) | (f1 & mask3) | (f2 & mask4) | (f3 & mask0) | (f4 & mask1);
        g4 = f0 ^ f1 ^ f2 ^ f3 ^ f4 ^ g0 ^ g1 ^ g2 ^ g3;
        store_x16(f + 0 + j, g0);
        store_x16(f + 512 + j, g1);
        store_x16(f + 1024 + j, g2);
        store_x16(f + 1536 + j, g3);
        store_x16(f + 2048 + j, g4);
        j += 16;

        f0 = load_x16(&fpad[0][j]);
        f1 = load_x16(&fpad[1][j]);
        f2 = load_x16(&fpad[2][j]);
        f3 = load_x16(&fpad[3][j]);
        f4 = load_x16(&fpad[4][j]);
        g0 = (f0 & mask2) | (f1 & mask3) | (f2 & mask4) | (f3 & mask0) | (f4 & mask1);
        g1 = (f0 & mask3) | (f1 & mask4) | (f2 & mask0) | (f3 & mask1) | (f4 & mask2);
        g2 = (f0 & mask4) | (f1 & mask0) | (f2 & mask1) | (f3 & mask2) | (f4 & mask3);
        g3 = (f0 & mask0) | (f1 & mask1) | (f2 & mask2) | (f3 & mask3) | (f4 & mask4);
        g4 = f0 ^ f1 ^ f2 ^ f3 ^ f4 ^ g0 ^ g1 ^ g2 ^ g3;
        store_x16(f + 0 + j, g0);
        store_x16(f + 512 + j, g1);
        store_x16(f + 1024 + j, g2);
        store_x16(f + 1536 + j, g3);
        store_x16(f + 2048 + j, g4);
        j += 16;
    }
}

static void mult1280(int16 h[2560], const int16 f[1280], const int16 g[1280]) {
    vec10x512 fgpad;
#define fpad (fgpad.v)
#define gpad (fgpad.v+5)
#define hpad fpad
    vec2560 aligned_h_7681;
#define h_7681 (aligned_h_7681.v)
    int i;

    good(fpad, f);
    good(gpad, g);

    PQCLEAN_SNTRUP1277_AVX2_ntt512_7681(fgpad.v[0], 10);

    for (i = 0; i < 512; i += 16) {
        int16x16 f0 = squeeze_7681_x16(load_x16(&fpad[0][i]));
        int16x16 f1 = squeeze_7681_x16(load_x16(&fpad[1][i]));
        int16x16 f2 = squeeze_7681_x16(load_x16(&fpad[2][i]));
        int16x16 f3 = squeeze_7681_x16(load_x16(&fpad[3][i]));
        int16x16 f4 = squeeze_7681_x16(load_x16(&fpad[4][i]));
        int16x16 g0 = squeeze_7681_x16(load_x16(&gpad[0][i]));
        int16x16 g1 = squeeze_7681_x16(load_x16(&gpad[1][i]));
        int16x16 g2 = squeeze_7681_x16(load_x16(&gpad[2][i]));
        int16x16 g3 = squeeze_7681_x16(load_x16(&gpad[3][i]));
        int16x16 g4 = squeeze_7681_x16(load_x16(&gpad[4][i]));
        int16x16 d0 = mulmod_7681_x16(f0, g0);
        int16x16 d1 = mulmod_7681_x16(f1, g1);
        int16x16 d2 = mulmod_7681_x16(f2, g2);
        int16x16 d3 = mulmod_7681_x16(f3, g3);
        int16x16 d4 = mulmod_7681_x16(f4, g4);
        int16x16 dsum;
        int16x16 h0, h1, h2, h3, h4;

        dsum = add_x16(add_x16(d0, d1), d2);
        dsum = squeeze_7681_x16(dsum);
        dsum = add_x16(add_x16(d3, d4), dsum);
        dsum = squeeze_7681_x16(dsum);

        /* could save one mult here using sum hi = (sum fi)(sum gi) */

        h0 = add_x16(dsum, mulmod_7681_x16(sub_x16(f4, f1), sub_x16(g1, g4)));
        h1 = add_x16(dsum, mulmod_7681_x16(sub_x16(f1, f0), sub_x16(g0, g1)));
        h2 = add_x16(dsum, mulmod_7681_x16(sub_x16(f2, f0), sub_x16(g0, g2)));
        h3 = add_x16(dsum, mulmod_7681_x16(sub_x16(f3, f0), sub_x16(g0, g3)));
        h4 = add_x16(dsum, mulmod_7681_x16(sub_x16(f4, f0), sub_x16(g0, g4)));
        h0 = add_x16(h0, mulmod_7681_x16(sub_x16(f3, f2), sub_x16(g2, g3)));
        h1 = add_x16(h1, mulmod_7681_x16(sub_x16(f4, f2), sub_x16(g2, g4)));
        h2 = add_x16(h2, mulmod_7681_x16(sub_x16(f4, f3), sub_x16(g3, g4)));
        h3 = add_x16(h3, mulmod_7681_x16(sub_x16(f2, f1), sub_x16(g1, g2)));
        h4 = add_x16(h4, mulmod_7681_x16(sub_x16(f3, f1), sub_x16(g1, g3)));

        store_x16(&hpad[0][i], squeeze_7681_x16(h0));
        store_x16(&hpad[1][i], squeeze_7681_x16(h1));
        store_x16(&hpad[2][i], squeeze_7681_x16(h2));
        store_x16(&hpad[3][i], squeeze_7681_x16(h3));
        store_x16(&hpad[4][i], squeeze_7681_x16(h4));
    }

    PQCLEAN_SNTRUP1277_AVX2_invntt512_7681(hpad[0], 5);
    ungood(h_7681, (const int16(*)[512])hpad);

    for (i = 0; i < 2560; i += 16) {
        int16x16 u = load_x16(&h_7681[i]);
        u = mulmod_7681_x16(u, const_x16(956));
        store_x16(&h[i], u);
    }
}

#define crypto_decode_pxint16 PQCLEAN_SNTRUP1277_AVX2_crypto_decode_1277xint16
#define crypto_encode_pxint16 PQCLEAN_SNTRUP1277_AVX2_crypto_encode_1277xint16

#define p 1277

static inline int16x16 freeze_3_x16(int16x16 x) {
    int16x16 mask, x3;
    x = add_x16(x, const_x16(3)&signmask_x16(x));
    mask = signmask_x16(sub_x16(x, const_x16(2)));
    x3 = sub_x16(x, const_x16(3));
    x = _mm256_blendv_epi8(x3, x, mask);
    return x;
}

int PQCLEAN_SNTRUP1277_AVX2_crypto_core_mult3sntrup1277(unsigned char *outbytes, const unsigned char *inbytes, const unsigned char *kbytes) {
    vec1280 x1, x2;
    vec2560 c3;
#define f (x1.v)
#define g (x2.v)
#define fg (c3.v)
#define h f
    int i;
    int16x16 x;

    x = const_x16(0);
    for (i = p & ~15; i < 1280; i += 16) {
        store_x16(&f[i], x);
    }
    for (i = p & ~15; i < 1280; i += 16) {
        store_x16(&g[i], x);
    }

    for (i = 0; i < p; ++i) {
        int8 fi = inbytes[i];
        int8 fi0 = fi & 1;
        f[i] = fi0 - (fi & (fi0 << 1));
    }
    for (i = 0; i < p; ++i) {
        int8 gi = kbytes[i];
        int8 gi0 = gi & 1;
        g[i] = gi0 - (gi & (gi0 << 1));
    }

    mult1280(fg, f, g);

    fg[0] -= fg[p - 1];
    for (i = 0; i < 1280; i += 16) {
        int16x16 fgi = load_x16(&fg[i]);
        int16x16 fgip = load_x16(&fg[i + p]);
        int16x16 fgip1 = load_x16(&fg[i + p - 1]);
        x = add_x16(fgi, add_x16(fgip, fgip1));
        x = freeze_3_x16(squeeze_3_x16(x));
        store_x16(&h[i], x);
    }

    for (i = 0; i < p; ++i) {
        outbytes[i] = h[i];
    }

    return 0;
}
