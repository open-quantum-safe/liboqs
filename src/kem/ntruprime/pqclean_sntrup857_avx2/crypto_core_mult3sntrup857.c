#include "crypto_core_mult3sntrup857.h"
#include "crypto_core_multsntrup857_ntt.h"
#include "crypto_decode_857xint16.h"
#include "crypto_encode_857xint16.h"
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
    int16 v[512];
    int16x16 _dummy;
} vec512;

typedef union {
    int16 v[4][512];
    int16x16 _dummy;
} vec4x512;

typedef union {
    int16 v[1024];
    int16x16 _dummy;
} vec1024;

typedef union {
    int16 v[4 * 512];
    int16x16 _dummy;
} vec2048;

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

static void stride(int16 fpad[4][512], const int16 f[1024]) {
    int16x16 f0, f1, f2, f3, g0, g1, g2, g3;
    int i, j;

    for (j = 0; j < 256; j += 16) {
        f0 = load_x16(&f[0]);
        f1 = load_x16(&f[16]);
        f2 = load_x16(&f[32]);
        f3 = load_x16(&f[48]);
        f += 64;

        g0 = _mm256_permute2x128_si256(f0, f2, 0x20);
        g1 = _mm256_permute2x128_si256(f0, f2, 0x31);
        g2 = _mm256_permute2x128_si256(f1, f3, 0x20);
        g3 = _mm256_permute2x128_si256(f1, f3, 0x31);
        f0 = _mm256_unpacklo_epi16(g0, g1);
        f1 = _mm256_unpackhi_epi16(g0, g1);
        f2 = _mm256_unpacklo_epi16(g2, g3);
        f3 = _mm256_unpackhi_epi16(g2, g3);
        g0 = _mm256_unpacklo_epi16(f0, f1);
        g1 = _mm256_unpackhi_epi16(f0, f1);
        g2 = _mm256_unpacklo_epi16(f2, f3);
        g3 = _mm256_unpackhi_epi16(f2, f3);
        f0 = _mm256_unpacklo_epi64(g0, g2);
        f1 = _mm256_unpackhi_epi64(g0, g2);
        f2 = _mm256_unpacklo_epi64(g1, g3);
        f3 = _mm256_unpackhi_epi64(g1, g3);

        store_x16(&fpad[0][j], f0);
        store_x16(&fpad[1][j], f1);
        store_x16(&fpad[2][j], f2);
        store_x16(&fpad[3][j], f3);
    }

    for (i = 0; i < 4; ++i) {
        for (j = 256; j < 512; ++j) {
            fpad[i][j] = 0;
        }
    }
}

static void unstride(int16 f[2048], const int16 fpad[4][512]) {
    int16x16 f0, f1, f2, f3, g0, g1, g2, g3, h0, h1, h2, h3;
    int j;

    for (j = 0; j < 512; j += 16) {
        f0 = load_x16(&fpad[0][j]);
        f1 = load_x16(&fpad[1][j]);
        f2 = load_x16(&fpad[2][j]);
        f3 = load_x16(&fpad[3][j]);

        g2 = _mm256_unpacklo_epi16(f2, f3);
        g3 = _mm256_unpackhi_epi16(f2, f3);
        g0 = _mm256_unpacklo_epi16(f0, f1);
        h0 = _mm256_unpacklo_epi32(g0, g2);
        h1 = _mm256_unpackhi_epi32(g0, g2);
        g1 = _mm256_unpackhi_epi16(f0, f1);
        h2 = _mm256_unpacklo_epi32(g1, g3);
        h3 = _mm256_unpackhi_epi32(g1, g3);
        f1 = _mm256_permute2x128_si256(h2, h3, 0x20);
        f3 = _mm256_permute2x128_si256(h2, h3, 0x31);
        f0 = _mm256_permute2x128_si256(h0, h1, 0x20);
        f2 = _mm256_permute2x128_si256(h0, h1, 0x31);

        store_x16(&f[0], f0);
        store_x16(&f[16], f1);
        store_x16(&f[32], f2);
        store_x16(&f[48], f3);
        f += 64;
    }
}

static const vec512 y_7681 = { .v = {
        -3593, -617, -2804, 3266, -2194, -1296, -1321, 810, 1414, 3706, -549, -396, -121, -2088, -2555, 1305,
            -3777, 1921, 103, 3600, -2456, 1483, 1399, -1887, -1701, 2006, 1535, -3174, -2250, 2816, -2440, -1760,
            -3625, 2830, 2043, -3689, 1100, 1525, -514, 7, 2876, -1599, 3153, -1881, -2495, -2237, -2535, 438,
            3182, 3364, -1431, 1738, 3696, -2557, -2956, 638, -2319, -1993, -2310, -3555, 834, -1986, 3772, -679,
            3593, 617, 2804, -3266, 2194, 1296, 1321, -810, -1414, -3706, 549, 396, 121, 2088, 2555, -1305,
            3777, -1921, -103, -3600, 2456, -1483, -1399, 1887, 1701, -2006, -1535, 3174, 2250, -2816, 2440, 1760,
            3625, -2830, -2043, 3689, -1100, -1525, 514, -7, -2876, 1599, -3153, 1881, 2495, 2237, 2535, -438,
            -3182, -3364, 1431, -1738, -3696, 2557, 2956, -638, 2319, 1993, 2310, 3555, -834, 1986, -3772, 679,
            2665, 727, -2572, 2426, -2133, -1386, 1681, -1054, 2579, 3750, 373, 3417, 404, -2233, 3135, -3405,
            -1799, 1521, 1497, -3831, -3480, -3428, 2883, -1698, -859, -2762, 2175, -194, -486, -3816, -1756, 2385,
            -783, 1533, 3145, 2, 3310, -2743, 2224, -1166, 2649, -1390, 3692, 2789, 1919, 2835, -2391, -2732,
            1056, 1464, 1350, -915, -1168, -921, -3588, 3456, -2160, -1598, 730, 2919, 1532, -2764, -660, -2113,
            -2665, -727, 2572, -2426, 2133, 1386, -1681, 1054, -2579, -3750, -373, -3417, -404, 2233, -3135, 3405,
            1799, -1521, -1497, 3831, 3480, 3428, -2883, 1698, 859, 2762, -2175, 194, 486, 3816, 1756, -2385,
            783, -1533, -3145, -2, -3310, 2743, -2224, 1166, -2649, 1390, -3692, -2789, -1919, -2835, 2391, 2732,
            -1056, -1464, -1350, 915, 1168, 921, 3588, -3456, 2160, 1598, -730, -2919, -1532, 2764, 660, 2113,
            2005, -188, 2345, -3723, -1403, 2070, 83, -3214, -3752, -1012, 1837, -3208, 3287, 3335, -293, 796,
            592, 1519, -1338, 1931, 509, -2262, -3408, 3334, 3677, 2130, 642, 589, -2167, -1084, -370, -3163,
            3763, -893, -2303, -402, 2937, -1689, -1526, -3745, -2460, 2874, 2965, 124, -1669, -1441, -3312, 3781,
            2812, -2386, -2515, -429, -3343, 777, -826, -3366, -3657, -1404, -791, -2963, -692, 2532, 2083, 2258,
            -2005, 188, -2345, 3723, 1403, -2070, -83, 3214, 3752, 1012, -1837, 3208, -3287, -3335, 293, -796,
            -592, -1519, 1338, -1931, -509, 2262, 3408, -3334, -3677, -2130, -642, -589, 2167, 1084, 370, 3163,
            -3763, 893, 2303, 402, -2937, 1689, 1526, 3745, 2460, -2874, -2965, -124, 1669, 1441, 3312, -3781,
            -2812, 2386, 2515, 429, 3343, -777, 826, 3366, 3657, 1404, 791, 2963, 692, -2532, -2083, -2258,
            179, 1121, 2891, -3581, 3177, -658, -3314, -1509, -17, 151, 2815, 2786, 1278, -2767, -1072, -1151,
            -1242, -2071, 2340, -1586, 2072, 1476, 2998, 2918, -3744, -3794, -1295, 451, -929, 2378, -1144, 434,
            -1070, -436, -3550, -3568, 1649, 715, 3461, -1407, -2001, -1203, 3770, 1712, 2230, -3542, 2589, -3547,
            -2059, -236, 3434, -3693, 2161, -670, 2719, 2339, -2422, 1181, 3450, 222, 1348, -226, 2247, -1779,
            -179, -1121, -2891, 3581, -3177, 658, 3314, 1509, 17, -151, -2815, -2786, -1278, 2767, 1072, 1151,
            1242, 2071, -2340, 1586, -2072, -1476, -2998, -2918, 3744, 3794, 1295, -451, 929, -2378, 1144, -434,
            1070, 436, 3550, 3568, -1649, -715, -3461, 1407, 2001, 1203, -3770, -1712, -2230, 3542, -2589, 3547,
            2059, 236, -3434, 3693, -2161, 670, -2719, -2339, 2422, -1181, -3450, -222, -1348, 226, -2247, 1779,
        }
} ;

static void mult1024(int16 h[2048], const int16 f[1024], const int16 g[1024]) {
    vec4x512 x1, x2;
    vec2048 x3;
#define fpad (x1.v)
#define gpad (x2.v)
#define hpad fpad
#define h_7681 (x3.v)
    int i;

    stride(fpad, f);
    PQCLEAN_SNTRUP857_AVX2_ntt512_7681(fpad[0], 4);

    stride(gpad, g);
    PQCLEAN_SNTRUP857_AVX2_ntt512_7681(gpad[0], 4);

    /* XXX: try arbitrary-degree Karatsuba */

    for (i = 0; i < 512; i += 16) {
        int16x16 f0 = squeeze_7681_x16(load_x16(&fpad[0][i]));
        int16x16 f1 = squeeze_7681_x16(load_x16(&fpad[1][i]));
        int16x16 f2 = squeeze_7681_x16(load_x16(&fpad[2][i]));
        int16x16 f3 = squeeze_7681_x16(load_x16(&fpad[3][i]));
        int16x16 g0 = squeeze_7681_x16(load_x16(&gpad[0][i]));
        int16x16 g1 = squeeze_7681_x16(load_x16(&gpad[1][i]));
        int16x16 g2 = squeeze_7681_x16(load_x16(&gpad[2][i]));
        int16x16 g3 = squeeze_7681_x16(load_x16(&gpad[3][i]));
        int16x16 d0 = mulmod_7681_x16(f0, g0);
        int16x16 d1 = mulmod_7681_x16(f1, g1);
        int16x16 d2 = mulmod_7681_x16(f2, g2);
        int16x16 d3 = mulmod_7681_x16(f3, g3);
        int16x16 d0d1 = add_x16(d0, d1);
        int16x16 d0d1d2 = add_x16(d0d1, d2);
        int16x16 d0d1d2d3 = squeeze_7681_x16(add_x16(d0d1d2, d3));
        int16x16 d2d3 = add_x16(d2, d3);
        int16x16 d1d2d3 = add_x16(d1, d2d3);
        int16x16 e01 = mulmod_7681_x16(sub_x16(f0, f1), sub_x16(g0, g1));
        int16x16 e02 = mulmod_7681_x16(sub_x16(f0, f2), sub_x16(g0, g2));
        int16x16 e03 = mulmod_7681_x16(sub_x16(f0, f3), sub_x16(g0, g3));
        int16x16 e12 = mulmod_7681_x16(sub_x16(f1, f2), sub_x16(g1, g2));
        int16x16 e13 = mulmod_7681_x16(sub_x16(f1, f3), sub_x16(g1, g3));
        int16x16 e23 = mulmod_7681_x16(sub_x16(f2, f3), sub_x16(g2, g3));
        int16x16 h0 = d0;
        int16x16 h1 = sub_x16(d0d1, e01);
        int16x16 h2 = sub_x16(d0d1d2, e02);
        int16x16 h3 = sub_x16(d0d1d2d3, add_x16(e12, e03));
        int16x16 h4 = sub_x16(d1d2d3, e13);
        int16x16 h5 = sub_x16(d2d3, e23);
        int16x16 h6 = d3;
        int16x16 twist = load_x16(&y_7681.v[i]);
        h4 = mulmod_7681_x16(h4, twist);
        h5 = mulmod_7681_x16(h5, twist);
        h6 = mulmod_7681_x16(h6, twist);
        h0 = add_x16(h0, h4);
        h1 = add_x16(h1, h5);
        h2 = add_x16(h2, h6);
        store_x16(&hpad[0][i], squeeze_7681_x16(h0));
        store_x16(&hpad[1][i], squeeze_7681_x16(h1));
        store_x16(&hpad[2][i], squeeze_7681_x16(h2));
        store_x16(&hpad[3][i], squeeze_7681_x16(h3));
    }

    PQCLEAN_SNTRUP857_AVX2_invntt512_7681(hpad[0], 4);
    unstride(h_7681, (const int16(*)[512]) hpad);

    for (i = 0; i < 2048; i += 16) {
        int16x16 u = load_x16(&h_7681[i]);
        u = mulmod_7681_x16(u, const_x16(956));
        store_x16(&h[i], u);
    }
}

#define crypto_decode_pxint16 PQCLEAN_SNTRUP857_AVX2_crypto_decode_857xint16
#define crypto_encode_pxint16 PQCLEAN_SNTRUP857_AVX2_crypto_encode_857xint16

#define p 857

static inline int16x16 freeze_3_x16(int16x16 x) {
    int16x16 mask, x3;
    x = add_x16(x, const_x16(3)&signmask_x16(x));
    mask = signmask_x16(sub_x16(x, const_x16(2)));
    x3 = sub_x16(x, const_x16(3));
    x = _mm256_blendv_epi8(x3, x, mask);
    return x;
}

int PQCLEAN_SNTRUP857_AVX2_crypto_core_mult3sntrup857(unsigned char *outbytes, const unsigned char *inbytes, const unsigned char *kbytes) {
    vec1024 x1, x2;
    vec2048 x3;
#define f (x1.v)
#define g (x2.v)
#define fg (x3.v)
#define h f
    int i;
    int16x16 x;

    x = const_x16(0);
    for (i = p & ~15; i < 1024; i += 16) {
        store_x16(&f[i], x);
    }
    for (i = p & ~15; i < 1024; i += 16) {
        store_x16(&g[i], x);
    }

    for (i = 0; i < p; ++i) {
        int8 fi = (int8) inbytes[i];
        int8 fi0 = fi & 1;
        f[i] = (int16) (fi0 - (fi & (fi0 << 1)));
    }
    for (i = 0; i < p; ++i) {
        int8 gi = (int8) kbytes[i];
        int8 gi0 = gi & 1;
        g[i] = (int16) (gi0 - (gi & (gi0 << 1)));
    }

    mult1024(fg, f, g);

    fg[0] = (int16) (fg[0] - fg[p - 1]);
    for (i = 0; i < 1024; i += 16) {
        int16x16 fgi = load_x16(&fg[i]);
        int16x16 fgip = load_x16(&fg[i + p]);
        int16x16 fgip1 = load_x16(&fg[i + p - 1]);
        x = add_x16(fgi, add_x16(fgip, fgip1));
        x = freeze_3_x16(squeeze_3_x16(x));
        store_x16(&h[i], x);
    }

    for (i = 0; i < p; ++i) {
        outbytes[i] = (unsigned char) h[i];
    }

    return 0;
}
