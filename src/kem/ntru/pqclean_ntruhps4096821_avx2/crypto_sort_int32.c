// Based on supercop-20200820/crypto_sort/int32/avx2

#include "crypto_sort_int32.h"

#include <immintrin.h>
#define int32 int32_t

typedef __m256i int32x8;
#define int32x8_load(z) _mm256_loadu_si256((__m256i *) (z))
#define int32x8_store(z,i) _mm256_storeu_si256((__m256i *) (z),(i))
#define int32x8_min _mm256_min_epi32
#define int32x8_max _mm256_max_epi32

#define int32x8_MINMAX(a,b) \
    do { \
        int32x8 c = int32x8_min((a),(b)); \
        (b) = int32x8_max((a),(b)); \
        (a) = c; \
    } while(0)

static inline void int32_MINMAX(int32 *a, int32 *b) {
    int32 ab = *b ^ *a;
    int32 c = (int32)((int64_t) * b - (int64_t) * a);
    c ^= ab & (c ^ *b);
    c >>= 31;
    c &= ab;
    *a ^= c;
    *b ^= c;
}


static void minmax_vector(int32 *x, int32 *y, size_t n) {
    if ((long long) n < 8) {
        while ((long long) n > 0) {
            int32_MINMAX(x, y);
            ++x;
            ++y;
            --n;
        }
        return;
    }
    if (n & 7) {
        int32x8 x0 = int32x8_load(x + n - 8);
        int32x8 y0 = int32x8_load(y + n - 8);
        int32x8_MINMAX(x0, y0);
        int32x8_store(x + n - 8, x0);
        int32x8_store(y + n - 8, y0);
        n &= ~7;
    }
    do {
        int32x8 x0 = int32x8_load(x);
        int32x8 y0 = int32x8_load(y);
        int32x8_MINMAX(x0, y0);
        int32x8_store(x, x0);
        int32x8_store(y, y0);
        x += 8;
        y += 8;
        n -= 8;
    } while (n);
}

/* stages 8,4,2,1 of size-16 bitonic merging */

static void merge16_finish(int32 *x, int32x8 x0, int32x8 x1, int flagdown) {
    int32x8 b0, b1, c0, c1, mask;

    int32x8_MINMAX(x0, x1);

    b0 = _mm256_permute2x128_si256(x0, x1, 0x20); /* A0123B0123 */
    b1 = _mm256_permute2x128_si256(x0, x1, 0x31); /* A4567B4567 */

    int32x8_MINMAX(b0, b1);

    c0 = _mm256_unpacklo_epi64(b0, b1); /* A0145B0145 */
    c1 = _mm256_unpackhi_epi64(b0, b1); /* A2367B2367 */

    int32x8_MINMAX(c0, c1);

    b0 = _mm256_unpacklo_epi32(c0, c1); /* A0213B0213 */
    b1 = _mm256_unpackhi_epi32(c0, c1); /* A4657B4657 */

    c0 = _mm256_unpacklo_epi64(b0, b1); /* A0246B0246 */
    c1 = _mm256_unpackhi_epi64(b0, b1); /* A1357B1357 */

    int32x8_MINMAX(c0, c1);

    b0 = _mm256_unpacklo_epi32(c0, c1); /* A0123B0123 */
    b1 = _mm256_unpackhi_epi32(c0, c1); /* A4567B4567 */

    x0 = _mm256_permute2x128_si256(b0, b1, 0x20); /* A01234567 */
    x1 = _mm256_permute2x128_si256(b0, b1, 0x31); /* A01234567 */

    if (flagdown) {
        mask = _mm256_set1_epi32(-1);
        x0 ^= mask;
        x1 ^= mask;
    }

    int32x8_store(&x[0], x0);
    int32x8_store(&x[8], x1);
}

/* stages 64,32 of bitonic merging; n is multiple of 128 */

static void int32_twostages_32(int32 *x, size_t n) {
    size_t i;

    while (n > 0) {
        for (i = 0; i < 32; i += 8) {
            int32x8 x0 = int32x8_load(&x[i]);
            int32x8 x1 = int32x8_load(&x[i + 32]);
            int32x8 x2 = int32x8_load(&x[i + 64]);
            int32x8 x3 = int32x8_load(&x[i + 96]);

            int32x8_MINMAX(x0, x2);
            int32x8_MINMAX(x1, x3);
            int32x8_MINMAX(x0, x1);
            int32x8_MINMAX(x2, x3);

            int32x8_store(&x[i], x0);
            int32x8_store(&x[i + 32], x1);
            int32x8_store(&x[i + 64], x2);
            int32x8_store(&x[i + 96], x3);
        }
        x += 128;
        n -= 128;
    }
}

/* stages 4q,2q,q of bitonic merging */

static size_t int32_threestages(int32 *x, size_t n, size_t q) {
    size_t k, i;

    for (k = 0; k + 8 * q <= n; k += 8 * q) {
        for (i = k; i < k + q; i += 8) {
            int32x8 x0 = int32x8_load(&x[i]);
            int32x8 x1 = int32x8_load(&x[i + q]);
            int32x8 x2 = int32x8_load(&x[i + 2 * q]);
            int32x8 x3 = int32x8_load(&x[i + 3 * q]);
            int32x8 x4 = int32x8_load(&x[i + 4 * q]);
            int32x8 x5 = int32x8_load(&x[i + 5 * q]);
            int32x8 x6 = int32x8_load(&x[i + 6 * q]);
            int32x8 x7 = int32x8_load(&x[i + 7 * q]);

            int32x8_MINMAX(x0, x4);
            int32x8_MINMAX(x1, x5);
            int32x8_MINMAX(x2, x6);
            int32x8_MINMAX(x3, x7);
            int32x8_MINMAX(x0, x2);
            int32x8_MINMAX(x1, x3);
            int32x8_MINMAX(x4, x6);
            int32x8_MINMAX(x5, x7);
            int32x8_MINMAX(x0, x1);
            int32x8_MINMAX(x2, x3);
            int32x8_MINMAX(x4, x5);
            int32x8_MINMAX(x6, x7);

            int32x8_store(&x[i], x0);
            int32x8_store(&x[i + q], x1);
            int32x8_store(&x[i + 2 * q], x2);
            int32x8_store(&x[i + 3 * q], x3);
            int32x8_store(&x[i + 4 * q], x4);
            int32x8_store(&x[i + 5 * q], x5);
            int32x8_store(&x[i + 6 * q], x6);
            int32x8_store(&x[i + 7 * q], x7);
        }
    }

    return k;
}

/* n is a power of 2; n >= 8; if n == 8 then flagdown */

// NOLINTNEXTLINE(google-readability-function-size)
static void int32_sort_2power(int32 *x, size_t n, int flagdown) {
    size_t p, q, i, j, k;
    int32x8 mask;

    if (n == 8) {
        int32 x0 = x[0];
        int32 x1 = x[1];
        int32 x2 = x[2];
        int32 x3 = x[3];
        int32 x4 = x[4];
        int32 x5 = x[5];
        int32 x6 = x[6];
        int32 x7 = x[7];

        /* odd-even sort instead of bitonic sort */

        int32_MINMAX(&x1, &x0);
        int32_MINMAX(&x3, &x2);
        int32_MINMAX(&x2, &x0);
        int32_MINMAX(&x3, &x1);
        int32_MINMAX(&x2, &x1);

        int32_MINMAX(&x5, &x4);
        int32_MINMAX(&x7, &x6);
        int32_MINMAX(&x6, &x4);
        int32_MINMAX(&x7, &x5);
        int32_MINMAX(&x6, &x5);

        int32_MINMAX(&x4, &x0);
        int32_MINMAX(&x6, &x2);
        int32_MINMAX(&x4, &x2);

        int32_MINMAX(&x5, &x1);
        int32_MINMAX(&x7, &x3);
        int32_MINMAX(&x5, &x3);

        int32_MINMAX(&x2, &x1);
        int32_MINMAX(&x4, &x3);
        int32_MINMAX(&x6, &x5);

        x[0] = x0;
        x[1] = x1;
        x[2] = x2;
        x[3] = x3;
        x[4] = x4;
        x[5] = x5;
        x[6] = x6;
        x[7] = x7;
        return;
    }

    if (n == 16) {
        int32x8 x0, x1, b0, b1, c0, c1;

        x0 = int32x8_load(&x[0]);
        x1 = int32x8_load(&x[8]);

        mask = _mm256_set_epi32(0, 0, -1, -1, 0, 0, -1, -1);

        x0 ^= mask; /* A01234567 */
        x1 ^= mask; /* B01234567 */

        b0 = _mm256_unpacklo_epi32(x0, x1); /* AB0AB1AB4AB5 */
        b1 = _mm256_unpackhi_epi32(x0, x1); /* AB2AB3AB6AB7 */

        c0 = _mm256_unpacklo_epi64(b0, b1); /* AB0AB2AB4AB6 */
        c1 = _mm256_unpackhi_epi64(b0, b1); /* AB1AB3AB5AB7 */

        int32x8_MINMAX(c0, c1);

        mask = _mm256_set_epi32(0, 0, -1, -1, -1, -1, 0, 0);
        c0 ^= mask;
        c1 ^= mask;

        b0 = _mm256_unpacklo_epi32(c0, c1); /* A01B01A45B45 */
        b1 = _mm256_unpackhi_epi32(c0, c1); /* A23B23A67B67 */

        int32x8_MINMAX(b0, b1);

        x0 = _mm256_unpacklo_epi64(b0, b1); /* A01234567 */
        x1 = _mm256_unpackhi_epi64(b0, b1); /* B01234567 */

        b0 = _mm256_unpacklo_epi32(x0, x1); /* AB0AB1AB4AB5 */
        b1 = _mm256_unpackhi_epi32(x0, x1); /* AB2AB3AB6AB7 */

        c0 = _mm256_unpacklo_epi64(b0, b1); /* AB0AB2AB4AB6 */
        c1 = _mm256_unpackhi_epi64(b0, b1); /* AB1AB3AB5AB7 */

        int32x8_MINMAX(c0, c1);

        b0 = _mm256_unpacklo_epi32(c0, c1); /* A01B01A45B45 */
        b1 = _mm256_unpackhi_epi32(c0, c1); /* A23B23A67B67 */

        b0 ^= mask;
        b1 ^= mask;

        c0 = _mm256_permute2x128_si256(b0, b1, 0x20); /* A01B01A23B23 */
        c1 = _mm256_permute2x128_si256(b0, b1, 0x31); /* A45B45A67B67 */

        int32x8_MINMAX(c0, c1);

        b0 = _mm256_permute2x128_si256(c0, c1, 0x20); /* A01B01A45B45 */
        b1 = _mm256_permute2x128_si256(c0, c1, 0x31); /* A23B23A67B67 */

        int32x8_MINMAX(b0, b1);

        x0 = _mm256_unpacklo_epi64(b0, b1); /* A01234567 */
        x1 = _mm256_unpackhi_epi64(b0, b1); /* B01234567 */

        b0 = _mm256_unpacklo_epi32(x0, x1); /* AB0AB1AB4AB5 */
        b1 = _mm256_unpackhi_epi32(x0, x1); /* AB2AB3AB6AB7 */

        c0 = _mm256_unpacklo_epi64(b0, b1); /* AB0AB2AB4AB6 */
        c1 = _mm256_unpackhi_epi64(b0, b1); /* AB1AB3AB5AB7 */

        int32x8_MINMAX(c0, c1);

        b0 = _mm256_unpacklo_epi32(c0, c1); /* A01B01A45B45 */
        b1 = _mm256_unpackhi_epi32(c0, c1); /* A23B23A67B67 */

        x0 = _mm256_unpacklo_epi64(b0, b1); /* A01234567 */
        x1 = _mm256_unpackhi_epi64(b0, b1); /* B01234567 */

        mask = _mm256_set1_epi32(-1);
        if (flagdown) {
            x1 ^= mask;
        } else {
            x0 ^= mask;
        }

        merge16_finish(x, x0, x1, flagdown);
        return;
    }

    if (n == 32) {
        int32x8 x0, x1, x2, x3;

        int32_sort_2power(x, 16, 1);
        int32_sort_2power(x + 16, 16, 0);

        x0 = int32x8_load(&x[0]);
        x1 = int32x8_load(&x[8]);
        x2 = int32x8_load(&x[16]);
        x3 = int32x8_load(&x[24]);

        if (flagdown) {
            mask = _mm256_set1_epi32(-1);
            x0 ^= mask;
            x1 ^= mask;
            x2 ^= mask;
            x3 ^= mask;
        }

        int32x8_MINMAX(x0, x2);
        int32x8_MINMAX(x1, x3);

        merge16_finish(x, x0, x1, flagdown);
        merge16_finish(x + 16, x2, x3, flagdown);
        return;
    }

    p = n >> 3;
    for (i = 0; i < p; i += 8) {
        int32x8 x0 = int32x8_load(&x[i]);
        int32x8 x2 = int32x8_load(&x[i + 2 * p]);
        int32x8 x4 = int32x8_load(&x[i + 4 * p]);
        int32x8 x6 = int32x8_load(&x[i + 6 * p]);

        /* odd-even stage instead of bitonic stage */

        int32x8_MINMAX(x4, x0);
        int32x8_MINMAX(x6, x2);
        int32x8_MINMAX(x2, x0);
        int32x8_MINMAX(x6, x4);
        int32x8_MINMAX(x2, x4);

        int32x8_store(&x[i], x0);
        int32x8_store(&x[i + 2 * p], x2);
        int32x8_store(&x[i + 4 * p], x4);
        int32x8_store(&x[i + 6 * p], x6);

        int32x8 x1 = int32x8_load(&x[i + p]);
        int32x8 x3 = int32x8_load(&x[i + 3 * p]);
        int32x8 x5 = int32x8_load(&x[i + 5 * p]);
        int32x8 x7 = int32x8_load(&x[i + 7 * p]);

        int32x8_MINMAX(x1, x5);
        int32x8_MINMAX(x3, x7);
        int32x8_MINMAX(x1, x3);
        int32x8_MINMAX(x5, x7);
        int32x8_MINMAX(x5, x3);

        int32x8_store(&x[i + p], x1);
        int32x8_store(&x[i + 3 * p], x3);
        int32x8_store(&x[i + 5 * p], x5);
        int32x8_store(&x[i + 7 * p], x7);
    }

    if (n >= 128) {
        int flip, flipflip;

        mask = _mm256_set1_epi32(-1);

        for (j = 0; j < n; j += 32) {
            int32x8 x0 = int32x8_load(&x[j]);
            int32x8 x1 = int32x8_load(&x[j + 16]);
            x0 ^= mask;
            x1 ^= mask;
            int32x8_store(&x[j], x0);
            int32x8_store(&x[j + 16], x1);
        }

        p = 8;
        for (;;) { /* for p in [8, 16, ..., n/16] */
            q = p >> 1;
            while (q >= 128) {
                int32_threestages(x, n, q >> 2);
                q >>= 3;
            }
            if (q == 64) {
                int32_twostages_32(x, n);
                q = 16;
            }
            if (q == 32) {
                q = 8;
                for (k = 0; k < n; k += 8 * q) {
                    for (i = k; i < k + q; i += 8) {
                        int32x8 x0 = int32x8_load(&x[i]);
                        int32x8 x1 = int32x8_load(&x[i + q]);
                        int32x8 x2 = int32x8_load(&x[i + 2 * q]);
                        int32x8 x3 = int32x8_load(&x[i + 3 * q]);
                        int32x8 x4 = int32x8_load(&x[i + 4 * q]);
                        int32x8 x5 = int32x8_load(&x[i + 5 * q]);
                        int32x8 x6 = int32x8_load(&x[i + 6 * q]);
                        int32x8 x7 = int32x8_load(&x[i + 7 * q]);

                        int32x8_MINMAX(x0, x4);
                        int32x8_MINMAX(x1, x5);
                        int32x8_MINMAX(x2, x6);
                        int32x8_MINMAX(x3, x7);
                        int32x8_MINMAX(x0, x2);
                        int32x8_MINMAX(x1, x3);
                        int32x8_MINMAX(x4, x6);
                        int32x8_MINMAX(x5, x7);
                        int32x8_MINMAX(x0, x1);
                        int32x8_MINMAX(x2, x3);
                        int32x8_MINMAX(x4, x5);
                        int32x8_MINMAX(x6, x7);

                        int32x8_store(&x[i], x0);
                        int32x8_store(&x[i + q], x1);
                        int32x8_store(&x[i + 2 * q], x2);
                        int32x8_store(&x[i + 3 * q], x3);
                        int32x8_store(&x[i + 4 * q], x4);
                        int32x8_store(&x[i + 5 * q], x5);
                        int32x8_store(&x[i + 6 * q], x6);
                        int32x8_store(&x[i + 7 * q], x7);
                    }
                }
                q = 4;
            }
            if (q == 16) {
                q = 8;
                for (k = 0; k < n; k += 4 * q) {
                    for (i = k; i < k + q; i += 8) {
                        int32x8 x0 = int32x8_load(&x[i]);
                        int32x8 x1 = int32x8_load(&x[i + q]);
                        int32x8 x2 = int32x8_load(&x[i + 2 * q]);
                        int32x8 x3 = int32x8_load(&x[i + 3 * q]);

                        int32x8_MINMAX(x0, x2);
                        int32x8_MINMAX(x1, x3);
                        int32x8_MINMAX(x0, x1);
                        int32x8_MINMAX(x2, x3);

                        int32x8_store(&x[i], x0);
                        int32x8_store(&x[i + q], x1);
                        int32x8_store(&x[i + 2 * q], x2);
                        int32x8_store(&x[i + 3 * q], x3);
                    }
                }
                q = 4;
            }
            if (q == 8) {
                for (k = 0; k < n; k += q + q) {
                    int32x8 x0 = int32x8_load(&x[k]);
                    int32x8 x1 = int32x8_load(&x[k + q]);

                    int32x8_MINMAX(x0, x1);

                    int32x8_store(&x[k], x0);
                    int32x8_store(&x[k + q], x1);
                }
            }

            q = n >> 3;
            flip = 0;
            if (p << 1 == q) {
                flip = 1;
            }
            flipflip = 1 - flip;
            for (j = 0; j < q; j += p + p) {
                for (k = j; k < j + p + p; k += p) {
                    for (i = k; i < k + p; i += 8) {
                        int32x8 x0 = int32x8_load(&x[i]);
                        int32x8 x1 = int32x8_load(&x[i + q]);
                        int32x8 x2 = int32x8_load(&x[i + 2 * q]);
                        int32x8 x3 = int32x8_load(&x[i + 3 * q]);
                        int32x8 x4 = int32x8_load(&x[i + 4 * q]);
                        int32x8 x5 = int32x8_load(&x[i + 5 * q]);
                        int32x8 x6 = int32x8_load(&x[i + 6 * q]);
                        int32x8 x7 = int32x8_load(&x[i + 7 * q]);

                        int32x8_MINMAX(x0, x1);
                        int32x8_MINMAX(x2, x3);
                        int32x8_MINMAX(x4, x5);
                        int32x8_MINMAX(x6, x7);
                        int32x8_MINMAX(x0, x2);
                        int32x8_MINMAX(x1, x3);
                        int32x8_MINMAX(x4, x6);
                        int32x8_MINMAX(x5, x7);
                        int32x8_MINMAX(x0, x4);
                        int32x8_MINMAX(x1, x5);
                        int32x8_MINMAX(x2, x6);
                        int32x8_MINMAX(x3, x7);

                        if (flip) {
                            x0 ^= mask;
                            x1 ^= mask;
                            x2 ^= mask;
                            x3 ^= mask;
                            x4 ^= mask;
                            x5 ^= mask;
                            x6 ^= mask;
                            x7 ^= mask;
                        }

                        int32x8_store(&x[i], x0);
                        int32x8_store(&x[i + q], x1);
                        int32x8_store(&x[i + 2 * q], x2);
                        int32x8_store(&x[i + 3 * q], x3);
                        int32x8_store(&x[i + 4 * q], x4);
                        int32x8_store(&x[i + 5 * q], x5);
                        int32x8_store(&x[i + 6 * q], x6);
                        int32x8_store(&x[i + 7 * q], x7);
                    }
                    flip ^= 1;
                }
                flip ^= flipflip;
            }

            if (p << 4 == n) {
                break;
            }
            p <<= 1;
        }
    }

    for (p = 4; p >= 1; p >>= 1) {
        int32 *z = x;
        int32 *target = x + n;
        if (p == 4) {
            mask = _mm256_set_epi32(0, 0, 0, 0, -1, -1, -1, -1);
            while (z != target) {
                int32x8 x0 = int32x8_load(&z[0]);
                int32x8 x1 = int32x8_load(&z[8]);
                x0 ^= mask;
                x1 ^= mask;
                int32x8_store(&z[0], x0);
                int32x8_store(&z[8], x1);
                z += 16;
            }
        } else if (p == 2) {
            mask = _mm256_set_epi32(0, 0, -1, -1, -1, -1, 0, 0);
            while (z != target) {
                int32x8 x0 = int32x8_load(&z[0]);
                int32x8 x1 = int32x8_load(&z[8]);
                x0 ^= mask;
                x1 ^= mask;
                int32x8 b0 = _mm256_permute2x128_si256(x0, x1, 0x20);
                int32x8 b1 = _mm256_permute2x128_si256(x0, x1, 0x31);
                int32x8_MINMAX(b0, b1);
                int32x8 c0 = _mm256_permute2x128_si256(b0, b1, 0x20);
                int32x8 c1 = _mm256_permute2x128_si256(b0, b1, 0x31);
                int32x8_store(&z[0], c0);
                int32x8_store(&z[8], c1);
                z += 16;
            }
        } else { /* p == 1 */
            mask = _mm256_set_epi32(0, -1, -1, 0, 0, -1, -1, 0);
            while (z != target) {
                int32x8 x0 = int32x8_load(&z[0]);
                int32x8 x1 = int32x8_load(&z[8]);
                x0 ^= mask;
                x1 ^= mask;
                int32x8 b0 = _mm256_permute2x128_si256(x0, x1, 0x20); /* A0123B0123 */
                int32x8 b1 = _mm256_permute2x128_si256(x0, x1, 0x31); /* A4567B4567 */
                int32x8 c0 = _mm256_unpacklo_epi64(b0, b1); /* A0145B0145 */
                int32x8 c1 = _mm256_unpackhi_epi64(b0, b1); /* A2367B2367 */
                int32x8_MINMAX(c0, c1);
                int32x8 d0 = _mm256_unpacklo_epi64(c0, c1); /* A0123B0123 */
                int32x8 d1 = _mm256_unpackhi_epi64(c0, c1); /* A4567B4567 */
                int32x8_MINMAX(d0, d1);
                int32x8 e0 = _mm256_permute2x128_si256(d0, d1, 0x20);
                int32x8 e1 = _mm256_permute2x128_si256(d0, d1, 0x31);
                int32x8_store(&z[0], e0);
                int32x8_store(&z[8], e1);
                z += 16;
            }
        }

        q = n >> 4;
        while (q >= 128 || q == 32) {
            int32_threestages(x, n, q >> 2);
            q >>= 3;
        }
        while (q >= 16) {
            q >>= 1;
            for (j = 0; j < n; j += 4 * q) {
                for (k = j; k < j + q; k += 8) {
                    int32x8 x0 = int32x8_load(&x[k]);
                    int32x8 x1 = int32x8_load(&x[k + q]);
                    int32x8 x2 = int32x8_load(&x[k + 2 * q]);
                    int32x8 x3 = int32x8_load(&x[k + 3 * q]);

                    int32x8_MINMAX(x0, x2);
                    int32x8_MINMAX(x1, x3);
                    int32x8_MINMAX(x0, x1);
                    int32x8_MINMAX(x2, x3);

                    int32x8_store(&x[k], x0);
                    int32x8_store(&x[k + q], x1);
                    int32x8_store(&x[k + 2 * q], x2);
                    int32x8_store(&x[k + 3 * q], x3);
                }
            }
            q >>= 1;
        }
        if (q == 8) {
            for (j = 0; j < n; j += 2 * q) {
                int32x8 x0 = int32x8_load(&x[j]);
                int32x8 x1 = int32x8_load(&x[j + q]);

                int32x8_MINMAX(x0, x1);

                int32x8_store(&x[j], x0);
                int32x8_store(&x[j + q], x1);
            }
        }

        q = n >> 3;
        for (k = 0; k < q; k += 8) {
            int32x8 x0 = int32x8_load(&x[k]);
            int32x8 x1 = int32x8_load(&x[k + q]);
            int32x8 x2 = int32x8_load(&x[k + 2 * q]);
            int32x8 x3 = int32x8_load(&x[k + 3 * q]);
            int32x8 x4 = int32x8_load(&x[k + 4 * q]);
            int32x8 x5 = int32x8_load(&x[k + 5 * q]);
            int32x8 x6 = int32x8_load(&x[k + 6 * q]);
            int32x8 x7 = int32x8_load(&x[k + 7 * q]);

            int32x8_MINMAX(x0, x1);
            int32x8_MINMAX(x2, x3);
            int32x8_MINMAX(x4, x5);
            int32x8_MINMAX(x6, x7);
            int32x8_MINMAX(x0, x2);
            int32x8_MINMAX(x1, x3);
            int32x8_MINMAX(x4, x6);
            int32x8_MINMAX(x5, x7);
            int32x8_MINMAX(x0, x4);
            int32x8_MINMAX(x1, x5);
            int32x8_MINMAX(x2, x6);
            int32x8_MINMAX(x3, x7);

            int32x8_store(&x[k], x0);
            int32x8_store(&x[k + q], x1);
            int32x8_store(&x[k + 2 * q], x2);
            int32x8_store(&x[k + 3 * q], x3);
            int32x8_store(&x[k + 4 * q], x4);
            int32x8_store(&x[k + 5 * q], x5);
            int32x8_store(&x[k + 6 * q], x6);
            int32x8_store(&x[k + 7 * q], x7);
        }
    }

    /* everything is still masked with _mm256_set_epi32(0,-1,0,-1,0,-1,0,-1); */
    mask = _mm256_set1_epi32(-1);

    for (i = 0; i < n; i += 64) {
        int32x8 a0 = int32x8_load(&x[i]);
        int32x8 a1 = int32x8_load(&x[i + 8]);
        int32x8 a2 = int32x8_load(&x[i + 16]);
        int32x8 a3 = int32x8_load(&x[i + 24]);
        int32x8 a4 = int32x8_load(&x[i + 32]);
        int32x8 a5 = int32x8_load(&x[i + 40]);
        int32x8 a6 = int32x8_load(&x[i + 48]);
        int32x8 a7 = int32x8_load(&x[i + 56]);

        int32x8 b0 = _mm256_unpacklo_epi32(a0, a1); /* AB0AB1AB4AB5 */
        int32x8 b1 = _mm256_unpackhi_epi32(a0, a1); /* AB2AB3AB6AB7 */
        int32x8 b2 = _mm256_unpacklo_epi32(a2, a3); /* CD0CD1CD4CD5 */
        int32x8 b3 = _mm256_unpackhi_epi32(a2, a3); /* CD2CD3CD6CD7 */
        int32x8 b4 = _mm256_unpacklo_epi32(a4, a5); /* EF0EF1EF4EF5 */
        int32x8 b5 = _mm256_unpackhi_epi32(a4, a5); /* EF2EF3EF6EF7 */
        int32x8 b6 = _mm256_unpacklo_epi32(a6, a7); /* GH0GH1GH4GH5 */
        int32x8 b7 = _mm256_unpackhi_epi32(a6, a7); /* GH2GH3GH6GH7 */

        int32x8 c0 = _mm256_unpacklo_epi64(b0, b2); /* ABCD0ABCD4 */
        int32x8 c1 = _mm256_unpacklo_epi64(b1, b3); /* ABCD2ABCD6 */
        int32x8 c2 = _mm256_unpackhi_epi64(b0, b2); /* ABCD1ABCD5 */
        int32x8 c3 = _mm256_unpackhi_epi64(b1, b3); /* ABCD3ABCD7 */
        int32x8 c4 = _mm256_unpacklo_epi64(b4, b6); /* EFGH0EFGH4 */
        int32x8 c5 = _mm256_unpacklo_epi64(b5, b7); /* EFGH2EFGH6 */
        int32x8 c6 = _mm256_unpackhi_epi64(b4, b6); /* EFGH1EFGH5 */
        int32x8 c7 = _mm256_unpackhi_epi64(b5, b7); /* EFGH3EFGH7 */

        if (flagdown) {
            c2 ^= mask;
            c3 ^= mask;
            c6 ^= mask;
            c7 ^= mask;
        } else {
            c0 ^= mask;
            c1 ^= mask;
            c4 ^= mask;
            c5 ^= mask;
        }

        int32x8 d0 = _mm256_permute2x128_si256(c0, c4, 0x20); /* ABCDEFGH0 */
        int32x8 d1 = _mm256_permute2x128_si256(c2, c6, 0x20); /* ABCDEFGH1 */
        int32x8 d2 = _mm256_permute2x128_si256(c1, c5, 0x20); /* ABCDEFGH2 */
        int32x8 d3 = _mm256_permute2x128_si256(c3, c7, 0x20); /* ABCDEFGH5 */
        int32x8 d4 = _mm256_permute2x128_si256(c0, c4, 0x31); /* ABCDEFGH4 */
        int32x8 d5 = _mm256_permute2x128_si256(c2, c6, 0x31); /* ABCDEFGH3 */
        int32x8 d6 = _mm256_permute2x128_si256(c1, c5, 0x31); /* ABCDEFGH6 */
        int32x8 d7 = _mm256_permute2x128_si256(c3, c7, 0x31); /* ABCDEFGH7 */

        int32x8_MINMAX(d0, d1);
        int32x8_MINMAX(d2, d3);
        int32x8_MINMAX(d4, d5);
        int32x8_MINMAX(d6, d7);
        int32x8_MINMAX(d0, d2);
        int32x8_MINMAX(d1, d3);
        int32x8_MINMAX(d4, d6);
        int32x8_MINMAX(d5, d7);
        int32x8_MINMAX(d0, d4);
        int32x8_MINMAX(d1, d5);
        int32x8_MINMAX(d2, d6);
        int32x8_MINMAX(d3, d7);

        int32x8 e0 = _mm256_unpacklo_epi32(d0, d1);
        int32x8 e1 = _mm256_unpackhi_epi32(d0, d1);
        int32x8 e2 = _mm256_unpacklo_epi32(d2, d3);
        int32x8 e3 = _mm256_unpackhi_epi32(d2, d3);
        int32x8 e4 = _mm256_unpacklo_epi32(d4, d5);
        int32x8 e5 = _mm256_unpackhi_epi32(d4, d5);
        int32x8 e6 = _mm256_unpacklo_epi32(d6, d7);
        int32x8 e7 = _mm256_unpackhi_epi32(d6, d7);

        int32x8 f0 = _mm256_unpacklo_epi64(e0, e2);
        int32x8 f1 = _mm256_unpacklo_epi64(e1, e3);
        int32x8 f2 = _mm256_unpackhi_epi64(e0, e2);
        int32x8 f3 = _mm256_unpackhi_epi64(e1, e3);
        int32x8 f4 = _mm256_unpacklo_epi64(e4, e6);
        int32x8 f5 = _mm256_unpacklo_epi64(e5, e7);
        int32x8 f6 = _mm256_unpackhi_epi64(e4, e6);
        int32x8 f7 = _mm256_unpackhi_epi64(e5, e7);

        int32x8 g0 = _mm256_permute2x128_si256(f0, f4, 0x20);
        int32x8 g1 = _mm256_permute2x128_si256(f2, f6, 0x20);
        int32x8 g2 = _mm256_permute2x128_si256(f1, f5, 0x20);
        int32x8 g3 = _mm256_permute2x128_si256(f3, f7, 0x20);
        int32x8 g4 = _mm256_permute2x128_si256(f0, f4, 0x31);
        int32x8 g5 = _mm256_permute2x128_si256(f2, f6, 0x31);
        int32x8 g6 = _mm256_permute2x128_si256(f1, f5, 0x31);
        int32x8 g7 = _mm256_permute2x128_si256(f3, f7, 0x31);

        int32x8_store(&x[i], g0);
        int32x8_store(&x[i + 8], g1);
        int32x8_store(&x[i + 16], g2);
        int32x8_store(&x[i + 24], g3);
        int32x8_store(&x[i + 32], g4);
        int32x8_store(&x[i + 40], g5);
        int32x8_store(&x[i + 48], g6);
        int32x8_store(&x[i + 56], g7);
    }

    q = n >> 4;
    while (q >= 128 || q == 32) {
        q >>= 2;
        for (j = 0; j < n; j += 8 * q) {
            for (i = j; i < j + q; i += 8) {
                int32x8 x0 = int32x8_load(&x[i]);
                int32x8 x1 = int32x8_load(&x[i + q]);
                int32x8 x2 = int32x8_load(&x[i + 2 * q]);
                int32x8 x3 = int32x8_load(&x[i + 3 * q]);
                int32x8 x4 = int32x8_load(&x[i + 4 * q]);
                int32x8 x5 = int32x8_load(&x[i + 5 * q]);
                int32x8 x6 = int32x8_load(&x[i + 6 * q]);
                int32x8 x7 = int32x8_load(&x[i + 7 * q]);
                int32x8_MINMAX(x0, x4);
                int32x8_MINMAX(x1, x5);
                int32x8_MINMAX(x2, x6);
                int32x8_MINMAX(x3, x7);
                int32x8_MINMAX(x0, x2);
                int32x8_MINMAX(x1, x3);
                int32x8_MINMAX(x4, x6);
                int32x8_MINMAX(x5, x7);
                int32x8_MINMAX(x0, x1);
                int32x8_MINMAX(x2, x3);
                int32x8_MINMAX(x4, x5);
                int32x8_MINMAX(x6, x7);
                int32x8_store(&x[i], x0);
                int32x8_store(&x[i + q], x1);
                int32x8_store(&x[i + 2 * q], x2);
                int32x8_store(&x[i + 3 * q], x3);
                int32x8_store(&x[i + 4 * q], x4);
                int32x8_store(&x[i + 5 * q], x5);
                int32x8_store(&x[i + 6 * q], x6);
                int32x8_store(&x[i + 7 * q], x7);
            }
        }
        q >>= 1;
    }
    while (q >= 16) {
        q >>= 1;
        for (j = 0; j < n; j += 4 * q) {
            for (i = j; i < j + q; i += 8) {
                int32x8 x0 = int32x8_load(&x[i]);
                int32x8 x1 = int32x8_load(&x[i + q]);
                int32x8 x2 = int32x8_load(&x[i + 2 * q]);
                int32x8 x3 = int32x8_load(&x[i + 3 * q]);
                int32x8_MINMAX(x0, x2);
                int32x8_MINMAX(x1, x3);
                int32x8_MINMAX(x0, x1);
                int32x8_MINMAX(x2, x3);
                int32x8_store(&x[i], x0);
                int32x8_store(&x[i + q], x1);
                int32x8_store(&x[i + 2 * q], x2);
                int32x8_store(&x[i + 3 * q], x3);
            }
        }
        q >>= 1;
    }
    if (q == 8) {
        for (j = 0; j < n; j += q + q) {
            int32x8 x0 = int32x8_load(&x[j]);
            int32x8 x1 = int32x8_load(&x[j + q]);
            int32x8_MINMAX(x0, x1);
            int32x8_store(&x[j], x0);
            int32x8_store(&x[j + q], x1);
        }
    }

    q = n >> 3;
    for (i = 0; i < q; i += 8) {
        int32x8 x0 = int32x8_load(&x[i]);
        int32x8 x1 = int32x8_load(&x[i + q]);
        int32x8 x2 = int32x8_load(&x[i + 2 * q]);
        int32x8 x3 = int32x8_load(&x[i + 3 * q]);
        int32x8 x4 = int32x8_load(&x[i + 4 * q]);
        int32x8 x5 = int32x8_load(&x[i + 5 * q]);
        int32x8 x6 = int32x8_load(&x[i + 6 * q]);
        int32x8 x7 = int32x8_load(&x[i + 7 * q]);

        int32x8_MINMAX(x0, x1);
        int32x8_MINMAX(x2, x3);
        int32x8_MINMAX(x4, x5);
        int32x8_MINMAX(x6, x7);
        int32x8_MINMAX(x0, x2);
        int32x8_MINMAX(x1, x3);
        int32x8_MINMAX(x4, x6);
        int32x8_MINMAX(x5, x7);
        int32x8_MINMAX(x0, x4);
        int32x8_MINMAX(x1, x5);
        int32x8_MINMAX(x2, x6);
        int32x8_MINMAX(x3, x7);

        int32x8 b0 = _mm256_unpacklo_epi32(x0, x4); /* AE0AE1AE4AE5 */
        int32x8 b1 = _mm256_unpackhi_epi32(x0, x4); /* AE2AE3AE6AE7 */
        int32x8 b2 = _mm256_unpacklo_epi32(x1, x5); /* BF0BF1BF4BF5 */
        int32x8 b3 = _mm256_unpackhi_epi32(x1, x5); /* BF2BF3BF6BF7 */
        int32x8 b4 = _mm256_unpacklo_epi32(x2, x6); /* CG0CG1CG4CG5 */
        int32x8 b5 = _mm256_unpackhi_epi32(x2, x6); /* CG2CG3CG6CG7 */
        int32x8 b6 = _mm256_unpacklo_epi32(x3, x7); /* DH0DH1DH4DH5 */
        int32x8 b7 = _mm256_unpackhi_epi32(x3, x7); /* DH2DH3DH6DH7 */

        int32x8 c0 = _mm256_unpacklo_epi64(b0, b4); /* AECG0AECG4 */
        int32x8 c1 = _mm256_unpacklo_epi64(b1, b5); /* AECG2AECG6 */
        int32x8 c2 = _mm256_unpackhi_epi64(b0, b4); /* AECG1AECG5 */
        int32x8 c3 = _mm256_unpackhi_epi64(b1, b5); /* AECG3AECG7 */
        int32x8 c4 = _mm256_unpacklo_epi64(b2, b6); /* BFDH0BFDH4 */
        int32x8 c5 = _mm256_unpacklo_epi64(b3, b7); /* BFDH2BFDH6 */
        int32x8 c6 = _mm256_unpackhi_epi64(b2, b6); /* BFDH1BFDH5 */
        int32x8 c7 = _mm256_unpackhi_epi64(b3, b7); /* BFDH3BFDH7 */

        int32x8 d0 = _mm256_permute2x128_si256(c0, c4, 0x20); /* AECGBFDH0 */
        int32x8 d1 = _mm256_permute2x128_si256(c1, c5, 0x20); /* AECGBFDH2 */
        int32x8 d2 = _mm256_permute2x128_si256(c2, c6, 0x20); /* AECGBFDH1 */
        int32x8 d3 = _mm256_permute2x128_si256(c3, c7, 0x20); /* AECGBFDH3 */
        int32x8 d4 = _mm256_permute2x128_si256(c0, c4, 0x31); /* AECGBFDH4 */
        int32x8 d5 = _mm256_permute2x128_si256(c1, c5, 0x31); /* AECGBFDH6 */
        int32x8 d6 = _mm256_permute2x128_si256(c2, c6, 0x31); /* AECGBFDH5 */
        int32x8 d7 = _mm256_permute2x128_si256(c3, c7, 0x31); /* AECGBFDH7 */

        if (flagdown) {
            d0 ^= mask;
            d1 ^= mask;
            d2 ^= mask;
            d3 ^= mask;
            d4 ^= mask;
            d5 ^= mask;
            d6 ^= mask;
            d7 ^= mask;
        }

        int32x8_store(&x[i], d0);
        int32x8_store(&x[i + q], d4);
        int32x8_store(&x[i + 2 * q], d1);
        int32x8_store(&x[i + 3 * q], d5);
        int32x8_store(&x[i + 4 * q], d2);
        int32x8_store(&x[i + 5 * q], d6);
        int32x8_store(&x[i + 6 * q], d3);
        int32x8_store(&x[i + 7 * q], d7);
    }
}

void PQCLEAN_NTRUHPS4096821_AVX2_crypto_sort_int32(int32 *x, size_t n) {
    size_t q, i, j;

    if (n <= 8) {
        if (n == 8) {
            int32_MINMAX(&x[0], &x[1]);
            int32_MINMAX(&x[1], &x[2]);
            int32_MINMAX(&x[2], &x[3]);
            int32_MINMAX(&x[3], &x[4]);
            int32_MINMAX(&x[4], &x[5]);
            int32_MINMAX(&x[5], &x[6]);
            int32_MINMAX(&x[6], &x[7]);
        }
        if (n >= 7) {
            int32_MINMAX(&x[0], &x[1]);
            int32_MINMAX(&x[1], &x[2]);
            int32_MINMAX(&x[2], &x[3]);
            int32_MINMAX(&x[3], &x[4]);
            int32_MINMAX(&x[4], &x[5]);
            int32_MINMAX(&x[5], &x[6]);
        }
        if (n >= 6) {
            int32_MINMAX(&x[0], &x[1]);
            int32_MINMAX(&x[1], &x[2]);
            int32_MINMAX(&x[2], &x[3]);
            int32_MINMAX(&x[3], &x[4]);
            int32_MINMAX(&x[4], &x[5]);
        }
        if (n >= 5) {
            int32_MINMAX(&x[0], &x[1]);
            int32_MINMAX(&x[1], &x[2]);
            int32_MINMAX(&x[2], &x[3]);
            int32_MINMAX(&x[3], &x[4]);
        }
        if (n >= 4) {
            int32_MINMAX(&x[0], &x[1]);
            int32_MINMAX(&x[1], &x[2]);
            int32_MINMAX(&x[2], &x[3]);
        }
        if (n >= 3) {
            int32_MINMAX(&x[0], &x[1]);
            int32_MINMAX(&x[1], &x[2]);
        }
        if (n >= 2) {
            int32_MINMAX(&x[0], &x[1]);
        }
        return;
    }

    if (!(n & (n - 1))) {
        int32_sort_2power(x, n, 0);
        return;
    }

    q = 8;
    while (q < n - q) {
        q += q;
    }
    /* n > q >= 8 */

    if (q <= 128) { /* n <= 256 */
        int32x8 y[32];
        for (i = q >> 3; i < q >> 2; ++i) {
            y[i] = _mm256_set1_epi32(0x7fffffff);
        }
        for (i = 0; i < n; ++i) {
            ((int32 *) y)[i] = x[i];
        }
        int32_sort_2power((int32 *) y, 2 * q, 0);
        for (i = 0; i < n; ++i) {
            x[i] = ((int32 *) y)[i];
        }
        return;
    }

    int32_sort_2power(x, q, 1);
    PQCLEAN_NTRUHPS4096821_AVX2_crypto_sort_int32(x + q, n - q);

    while (q >= 64) {
        q >>= 2;
        j = int32_threestages(x, n, q);
        minmax_vector(x + j, x + j + 4 * q, n - 4 * q - j);
        if (j + 4 * q <= n) {
            for (i = j; i < j + q; i += 8) {
                int32x8 x0 = int32x8_load(&x[i]);
                int32x8 x1 = int32x8_load(&x[i + q]);
                int32x8 x2 = int32x8_load(&x[i + 2 * q]);
                int32x8 x3 = int32x8_load(&x[i + 3 * q]);
                int32x8_MINMAX(x0, x2);
                int32x8_MINMAX(x1, x3);
                int32x8_MINMAX(x0, x1);
                int32x8_MINMAX(x2, x3);
                int32x8_store(&x[i], x0);
                int32x8_store(&x[i + q], x1);
                int32x8_store(&x[i + 2 * q], x2);
                int32x8_store(&x[i + 3 * q], x3);
            }
            j += 4 * q;
        }
        minmax_vector(x + j, x + j + 2 * q, n - 2 * q - j);
        if (j + 2 * q <= n) {
            for (i = j; i < j + q; i += 8) {
                int32x8 x0 = int32x8_load(&x[i]);
                int32x8 x1 = int32x8_load(&x[i + q]);
                int32x8_MINMAX(x0, x1);
                int32x8_store(&x[i], x0);
                int32x8_store(&x[i + q], x1);
            }
            j += 2 * q;
        }
        minmax_vector(x + j, x + j + q, n - q - j);
        q >>= 1;
    }
    if (q == 32) {
        j = 0;
        for (; j + 64 <= n; j += 64) {
            int32x8 x0 = int32x8_load(&x[j]);
            int32x8 x1 = int32x8_load(&x[j + 8]);
            int32x8 x2 = int32x8_load(&x[j + 16]);
            int32x8 x3 = int32x8_load(&x[j + 24]);
            int32x8 x4 = int32x8_load(&x[j + 32]);
            int32x8 x5 = int32x8_load(&x[j + 40]);
            int32x8 x6 = int32x8_load(&x[j + 48]);
            int32x8 x7 = int32x8_load(&x[j + 56]);
            int32x8_MINMAX(x0, x4);
            int32x8_MINMAX(x1, x5);
            int32x8_MINMAX(x2, x6);
            int32x8_MINMAX(x3, x7);
            int32x8_MINMAX(x0, x2);
            int32x8_MINMAX(x1, x3);
            int32x8_MINMAX(x4, x6);
            int32x8_MINMAX(x5, x7);
            int32x8_MINMAX(x0, x1);
            int32x8_MINMAX(x2, x3);
            int32x8_MINMAX(x4, x5);
            int32x8_MINMAX(x6, x7);
            int32x8 a0 = _mm256_permute2x128_si256(x0, x1, 0x20);
            int32x8 a1 = _mm256_permute2x128_si256(x0, x1, 0x31);
            int32x8 a2 = _mm256_permute2x128_si256(x2, x3, 0x20);
            int32x8 a3 = _mm256_permute2x128_si256(x2, x3, 0x31);
            int32x8 a4 = _mm256_permute2x128_si256(x4, x5, 0x20);
            int32x8 a5 = _mm256_permute2x128_si256(x4, x5, 0x31);
            int32x8 a6 = _mm256_permute2x128_si256(x6, x7, 0x20);
            int32x8 a7 = _mm256_permute2x128_si256(x6, x7, 0x31);
            int32x8_MINMAX(a0, a1);
            int32x8_MINMAX(a2, a3);
            int32x8_MINMAX(a4, a5);
            int32x8_MINMAX(a6, a7);
            int32x8 b0 = _mm256_permute2x128_si256(a0, a1, 0x20);
            int32x8 b1 = _mm256_permute2x128_si256(a0, a1, 0x31);
            int32x8 b2 = _mm256_permute2x128_si256(a2, a3, 0x20);
            int32x8 b3 = _mm256_permute2x128_si256(a2, a3, 0x31);
            int32x8 b4 = _mm256_permute2x128_si256(a4, a5, 0x20);
            int32x8 b5 = _mm256_permute2x128_si256(a4, a5, 0x31);
            int32x8 b6 = _mm256_permute2x128_si256(a6, a7, 0x20);
            int32x8 b7 = _mm256_permute2x128_si256(a6, a7, 0x31);
            int32x8 c0 = _mm256_unpacklo_epi64(b0, b1);
            int32x8 c1 = _mm256_unpackhi_epi64(b0, b1);
            int32x8 c2 = _mm256_unpacklo_epi64(b2, b3);
            int32x8 c3 = _mm256_unpackhi_epi64(b2, b3);
            int32x8 c4 = _mm256_unpacklo_epi64(b4, b5);
            int32x8 c5 = _mm256_unpackhi_epi64(b4, b5);
            int32x8 c6 = _mm256_unpacklo_epi64(b6, b7);
            int32x8 c7 = _mm256_unpackhi_epi64(b6, b7);
            int32x8_MINMAX(c0, c1);
            int32x8_MINMAX(c2, c3);
            int32x8_MINMAX(c4, c5);
            int32x8_MINMAX(c6, c7);
            int32x8 d0 = _mm256_unpacklo_epi32(c0, c1);
            int32x8 d1 = _mm256_unpackhi_epi32(c0, c1);
            int32x8 d2 = _mm256_unpacklo_epi32(c2, c3);
            int32x8 d3 = _mm256_unpackhi_epi32(c2, c3);
            int32x8 d4 = _mm256_unpacklo_epi32(c4, c5);
            int32x8 d5 = _mm256_unpackhi_epi32(c4, c5);
            int32x8 d6 = _mm256_unpacklo_epi32(c6, c7);
            int32x8 d7 = _mm256_unpackhi_epi32(c6, c7);
            int32x8 e0 = _mm256_unpacklo_epi64(d0, d1);
            int32x8 e1 = _mm256_unpackhi_epi64(d0, d1);
            int32x8 e2 = _mm256_unpacklo_epi64(d2, d3);
            int32x8 e3 = _mm256_unpackhi_epi64(d2, d3);
            int32x8 e4 = _mm256_unpacklo_epi64(d4, d5);
            int32x8 e5 = _mm256_unpackhi_epi64(d4, d5);
            int32x8 e6 = _mm256_unpacklo_epi64(d6, d7);
            int32x8 e7 = _mm256_unpackhi_epi64(d6, d7);
            int32x8_MINMAX(e0, e1);
            int32x8_MINMAX(e2, e3);
            int32x8_MINMAX(e4, e5);
            int32x8_MINMAX(e6, e7);
            int32x8 f0 = _mm256_unpacklo_epi32(e0, e1);
            int32x8 f1 = _mm256_unpackhi_epi32(e0, e1);
            int32x8 f2 = _mm256_unpacklo_epi32(e2, e3);
            int32x8 f3 = _mm256_unpackhi_epi32(e2, e3);
            int32x8 f4 = _mm256_unpacklo_epi32(e4, e5);
            int32x8 f5 = _mm256_unpackhi_epi32(e4, e5);
            int32x8 f6 = _mm256_unpacklo_epi32(e6, e7);
            int32x8 f7 = _mm256_unpackhi_epi32(e6, e7);
            int32x8_store(&x[j], f0);
            int32x8_store(&x[j + 8], f1);
            int32x8_store(&x[j + 16], f2);
            int32x8_store(&x[j + 24], f3);
            int32x8_store(&x[j + 32], f4);
            int32x8_store(&x[j + 40], f5);
            int32x8_store(&x[j + 48], f6);
            int32x8_store(&x[j + 56], f7);
        }
        minmax_vector(x + j, x + j + 32, n - 32 - j);
        goto continue16;
    }
    if (q == 16) {
        j = 0;
continue16:
        for (; j + 32 <= n; j += 32) {
            int32x8 x0 = int32x8_load(&x[j]);
            int32x8 x1 = int32x8_load(&x[j + 8]);
            int32x8 x2 = int32x8_load(&x[j + 16]);
            int32x8 x3 = int32x8_load(&x[j + 24]);
            int32x8_MINMAX(x0, x2);
            int32x8_MINMAX(x1, x3);
            int32x8_MINMAX(x0, x1);
            int32x8_MINMAX(x2, x3);
            int32x8 a0 = _mm256_permute2x128_si256(x0, x1, 0x20);
            int32x8 a1 = _mm256_permute2x128_si256(x0, x1, 0x31);
            int32x8 a2 = _mm256_permute2x128_si256(x2, x3, 0x20);
            int32x8 a3 = _mm256_permute2x128_si256(x2, x3, 0x31);
            int32x8_MINMAX(a0, a1);
            int32x8_MINMAX(a2, a3);
            int32x8 b0 = _mm256_permute2x128_si256(a0, a1, 0x20);
            int32x8 b1 = _mm256_permute2x128_si256(a0, a1, 0x31);
            int32x8 b2 = _mm256_permute2x128_si256(a2, a3, 0x20);
            int32x8 b3 = _mm256_permute2x128_si256(a2, a3, 0x31);
            int32x8 c0 = _mm256_unpacklo_epi64(b0, b1);
            int32x8 c1 = _mm256_unpackhi_epi64(b0, b1);
            int32x8 c2 = _mm256_unpacklo_epi64(b2, b3);
            int32x8 c3 = _mm256_unpackhi_epi64(b2, b3);
            int32x8_MINMAX(c0, c1);
            int32x8_MINMAX(c2, c3);
            int32x8 d0 = _mm256_unpacklo_epi32(c0, c1);
            int32x8 d1 = _mm256_unpackhi_epi32(c0, c1);
            int32x8 d2 = _mm256_unpacklo_epi32(c2, c3);
            int32x8 d3 = _mm256_unpackhi_epi32(c2, c3);
            int32x8 e0 = _mm256_unpacklo_epi64(d0, d1);
            int32x8 e1 = _mm256_unpackhi_epi64(d0, d1);
            int32x8 e2 = _mm256_unpacklo_epi64(d2, d3);
            int32x8 e3 = _mm256_unpackhi_epi64(d2, d3);
            int32x8_MINMAX(e0, e1);
            int32x8_MINMAX(e2, e3);
            int32x8 f0 = _mm256_unpacklo_epi32(e0, e1);
            int32x8 f1 = _mm256_unpackhi_epi32(e0, e1);
            int32x8 f2 = _mm256_unpacklo_epi32(e2, e3);
            int32x8 f3 = _mm256_unpackhi_epi32(e2, e3);
            int32x8_store(&x[j], f0);
            int32x8_store(&x[j + 8], f1);
            int32x8_store(&x[j + 16], f2);
            int32x8_store(&x[j + 24], f3);
        }
        minmax_vector(x + j, x + j + 16, n - 16 - j);
        goto continue8;
    }
    /* q == 8 */
    j = 0;
continue8:
    for (; j + 16 <= n; j += 16) {
        int32x8 x0 = int32x8_load(&x[j]);
        int32x8 x1 = int32x8_load(&x[j + 8]);
        int32x8_MINMAX(x0, x1);
        int32x8_store(&x[j], x0);
        int32x8_store(&x[j + 8], x1);
        int32x8 a0 = _mm256_permute2x128_si256(x0, x1, 0x20); /* x0123y0123 */
        int32x8 a1 = _mm256_permute2x128_si256(x0, x1, 0x31); /* x4567y4567 */
        int32x8_MINMAX(a0, a1);
        int32x8 b0 = _mm256_permute2x128_si256(a0, a1, 0x20); /* x01234567 */
        int32x8 b1 = _mm256_permute2x128_si256(a0, a1, 0x31); /* y01234567 */
        int32x8 c0 = _mm256_unpacklo_epi64(b0, b1); /* x01y01x45y45 */
        int32x8 c1 = _mm256_unpackhi_epi64(b0, b1); /* x23y23x67y67 */
        int32x8_MINMAX(c0, c1);
        int32x8 d0 = _mm256_unpacklo_epi32(c0, c1); /* x02x13x46x57 */
        int32x8 d1 = _mm256_unpackhi_epi32(c0, c1); /* y02y13y46y57 */
        int32x8 e0 = _mm256_unpacklo_epi64(d0, d1); /* x02y02x46y46 */
        int32x8 e1 = _mm256_unpackhi_epi64(d0, d1); /* x13y13x57y57 */
        int32x8_MINMAX(e0, e1);
        int32x8 f0 = _mm256_unpacklo_epi32(e0, e1); /* x01234567 */
        int32x8 f1 = _mm256_unpackhi_epi32(e0, e1); /* y01234567 */
        int32x8_store(&x[j], f0);
        int32x8_store(&x[j + 8], f1);
    }
    minmax_vector(x + j, x + j + 8, n - 8 - j);
    if (j + 8 <= n) {
        int32_MINMAX(&x[j], &x[j + 4]);
        int32_MINMAX(&x[j + 1], &x[j + 5]);
        int32_MINMAX(&x[j + 2], &x[j + 6]);
        int32_MINMAX(&x[j + 3], &x[j + 7]);
        int32_MINMAX(&x[j], &x[j + 2]);
        int32_MINMAX(&x[j + 1], &x[j + 3]);
        int32_MINMAX(&x[j], &x[j + 1]);
        int32_MINMAX(&x[j + 2], &x[j + 3]);
        int32_MINMAX(&x[j + 4], &x[j + 6]);
        int32_MINMAX(&x[j + 5], &x[j + 7]);
        int32_MINMAX(&x[j + 4], &x[j + 5]);
        int32_MINMAX(&x[j + 6], &x[j + 7]);
        j += 8;
    }
    minmax_vector(x + j, x + j + 4, n - 4 - j);
    if (j + 4 <= n) {
        int32_MINMAX(&x[j], &x[j + 2]);
        int32_MINMAX(&x[j + 1], &x[j + 3]);
        int32_MINMAX(&x[j], &x[j + 1]);
        int32_MINMAX(&x[j + 2], &x[j + 3]);
        j += 4;
    }
    if (j + 3 <= n) {
        int32_MINMAX(&x[j], &x[j + 2]);
    }
    if (j + 2 <= n) {
        int32_MINMAX(&x[j], &x[j + 1]);
    }
}
