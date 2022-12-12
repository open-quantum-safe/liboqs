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
    int16 v[8][512];
    int16x16 _dummy;
} vec8x512;

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
        -3593, 3364, 1701, 2237, 2194, 2557, 834, -1599, 514, 438, 2555, -1887, 103, 1881, -549, -1738,
            -3777, -2830, 1414, 1986, 2456, 1525, 2495, -1993, 2956, -679, 2440, -810, 2804, 3555, 1535, -3689,
            -3625, 617, 2319, -2816, 3696, -1483, -2250, 3706, -1399, -1760, 2535, 638, 2043, 396, 2310, -3600,
            3182, -1921, 2876, -2088, -1100, -1296, 121, 2006, -1321, -1305, -3772, -7, -1431, 3174, 3153, 3266,
            3593, -3364, -1701, -2237, -2194, -2557, -834, 1599, -514, -438, -2555, 1887, -103, -1881, 549, 1738,
            3777, 2830, -1414, -1986, -2456, -1525, -2495, 1993, -2956, 679, -2440, 810, -2804, -3555, -1535, 3689,
            3625, -617, -2319, 2816, -3696, 1483, 2250, -3706, 1399, 1760, -2535, -638, -2043, -396, -2310, 3600,
            -3182, 1921, -2876, 2088, 1100, 1296, -121, -2006, 1321, 1305, 3772, 7, 1431, -3174, -3153, -3266,
            -1532, -3816, 783, -921, -2160, 2762, 3310, 727, 2789, -373, -3456, -1497, -2385, -2391, -2426, 2883,
            1919, 2233, -1056, 2743, -2649, 3750, -1168, 1521, 2919, -2175, -1166, -2572, -3405, -660, 3831, -1681,
            404, -2764, 1799, 1386, -859, 1390, -2133, -1464, -194, -3692, -1054, -1350, 2732, 3135, -915, 2224,
            -486, -2835, 2665, 3428, -2579, 1598, -3480, 1533, -3417, -730, -1698, 3145, 2113, -1756, -2, -3588,
            1532, 3816, -783, 921, 2160, -2762, -3310, -727, -2789, 373, 3456, 1497, 2385, 2391, 2426, -2883,
            -1919, -2233, 1056, -2743, 2649, -3750, 1168, -1521, -2919, 2175, 1166, 2572, 3405, 660, -3831, 1681,
            -404, 2764, -1799, -1386, 859, -1390, 2133, 1464, 194, 3692, 1054, 1350, -2732, -3135, 915, -2224,
            486, 2835, -2665, -3428, 2579, -1598, 3480, -1533, 3417, 730, 1698, -3145, -2113, 1756, 2, 3588,
            -1404, -509, -1689, -3752, 3335, 2812, -1519, 1669, -402, -2345, 2963, 370, 3745, 83, -796, 642,
            -2874, -1403, 777, 3677, -1084, -3763, -188, 692, -429, 1338, 124, -293, 3366, -3408, 3163, -1837,
            1012, 3343, -2262, -2460, -2532, 592, 893, -3287, 1931, 2303, -3208, -2083, 3214, 826, 2258, 2965,
            -2130, 2937, -2070, -3657, -1441, -2005, 2386, 2167, 3723, 2515, 589, -3312, -3334, -1526, -3781, -791,
            1404, 509, 1689, 3752, -3335, -2812, 1519, -1669, 402, 2345, -2963, -370, -3745, -83, 796, -642,
            2874, 1403, -777, -3677, 1084, 3763, 188, -692, 429, -1338, -124, 293, -3366, 3408, -3163, 1837,
            -1012, -3343, 2262, 2460, 2532, -592, -893, 3287, -1931, -2303, 3208, 2083, -3214, -826, -2258, -2965,
            2130, -2937, 2070, 3657, 1441, 2005, -2386, -2167, -3723, -2515, -589, 3312, 3334, 1526, 3781, 791,
            658, 1278, -226, 1649, -436, 17, -1181, 1242, -3434, 451, -3770, 3581, 2719, 1779, -1144, -1509,
            -1476, -929, 3542, 2161, -236, 3744, -1203, 179, -3550, -2786, -3450, 1586, -3461, -3547, 1072, 2918,
            -715, 2230, 2767, 2072, 1121, -2422, 3794, 1070, 2891, 222, 1295, 3568, 2998, -434, 2589, -2339,
            670, 1348, -2378, -3177, -2071, 2001, 151, 2059, 2340, -1712, 2815, 3693, 3314, -1151, 2247, -1407,
            -658, -1278, 226, -1649, 436, -17, 1181, -1242, 3434, -451, 3770, -3581, -2719, -1779, 1144, 1509,
            1476, 929, -3542, -2161, 236, -3744, 1203, -179, 3550, 2786, 3450, -1586, 3461, 3547, -1072, -2918,
            715, -2230, -2767, -2072, -1121, 2422, -3794, -1070, -2891, -222, -1295, -3568, -2998, 434, -2589, 2339,
            -670, -1348, 2378, 3177, 2071, -2001, -151, -2059, -2340, 1712, -2815, -3693, -3314, 1151, -2247, 1407,
        }
} ;

static void mult1024(int16 h[2048], const int16 f[1024], const int16 g[1024]) {
    vec8x512 fgpad;
#define fpad (fgpad.v)
#define gpad (fgpad.v+4)
#define hpad fpad
    vec2048 aligned_h_7681;
#define h_7681 (aligned_h_7681.v)
    int i;

    stride(fpad, f);
    stride(gpad, g);

    PQCLEAN_SNTRUP857_AVX2_ntt512_7681(fgpad.v[0], 8);

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
