
/*
 * This file is licensed
 * under Apache 2.0 (https://www.apache.org/licenses/LICENSE-2.0.html)
 * at https://github.com/GMUCERG/PQC_NEON/blob/main/neon/kyber or
 * public domain at https://github.com/cothan/kyber/blob/master/neon
 */

#include <arm_neon.h>
#include <stdint.h>
#include "params.h"
#include "cbd.h"

#define vload2(c, ptr) c = vld2q_u8(ptr);

#define vstore4(ptr, c) vst4q_s16(ptr, c);

// c = a >> n
#define vsr8(c, a, n) c = vshrq_n_u8(a, n);

// c = a & b
#define vand8(c, a, b) c = vandq_u8(a, b);

// c = a + b
#define vadd8(c, a, b) c = vaddq_u8(a, b);

// long c = a - b
#define vsubll8(c, a, b) c = (int16x8_t)vsubl_u8(a, b);

// long c = a - b
#define vsublh8(c, a, b) c = (int16x8_t)vsubl_high_u8(a, b);

static
void neon_cbd2(int16_t *r, const uint8_t buf[2 * KYBER_N / 4])
{
    uint8x16x2_t t, d;      // 4
    uint8x16x2_t a, b;      // 4
    int16x8x4_t res1, res2; // 4

    uint8x16_t const_0x55, const_0x3; // 2
    const_0x55 = vdupq_n_u8(0x55);
    const_0x3 = vdupq_n_u8(0x3);

    // Total SIMD register: 18
    unsigned int j = 0;
    for (unsigned int i = 0; i < KYBER_N / 2; i += 16 * 2) {
        // 0, 2, 4 , 6,...
        // 1, 3, 5 , 7,...
        vload2(t, &buf[i]);
        // d = t & 0x55555555
        vand8(d.val[0], t.val[0], const_0x55);
        vand8(d.val[1], t.val[1], const_0x55);
        // t = (t >> 1) & 0x55555555
        vsr8(t.val[0], t.val[0], 1);
        vsr8(t.val[1], t.val[1], 1);
        vand8(t.val[0], t.val[0], const_0x55);
        vand8(t.val[1], t.val[1], const_0x55);

        // d += t
        vadd8(d.val[0], d.val[0], t.val[0]);
        vadd8(d.val[1], d.val[1], t.val[1]);

        // Get a0, a2
        vand8(a.val[0], d.val[0], const_0x3);
        vand8(a.val[1], d.val[1], const_0x3);

        // Get b0, b2
        vsr8(b.val[0], d.val[0], 2);
        vsr8(b.val[1], d.val[1], 2);

        vand8(b.val[0], b.val[0], const_0x3);
        vand8(b.val[1], b.val[1], const_0x3);

        // 0 2  4 6  -- 8 10 12 14 | 0 4  8 12 -- 16 20 24 28 | 0
        // 1 3  5 7  -- 9 11 13 15 | 2 6 10 14 -- 18 22 26 30 | 2
        vsubll8(res1.val[0], vget_low_u8(a.val[0]), vget_low_u8(b.val[0]));
        vsubll8(res1.val[2], vget_low_u8(a.val[1]), vget_low_u8(b.val[1]));

        // 16 18 20 22  -- 24 26 28 30 | 32 36 40 44 -- 48 52 56 60 | 0
        // 17 19 21 23  -- 25 27 29 31 | 34 38 42 46 -- 50 54 58 62 | 2
        vsublh8(res2.val[0], a.val[0], b.val[0]);
        vsublh8(res2.val[2], a.val[1], b.val[1]);

        // Get a1, a3
        vsr8(a.val[0], d.val[0], 4);
        vsr8(a.val[1], d.val[1], 4);

        vand8(a.val[0], a.val[0], const_0x3);
        vand8(a.val[1], a.val[1], const_0x3);

        // Get b1, b3
        vsr8(b.val[0], d.val[0], 6);
        vsr8(b.val[1], d.val[1], 6);

        // 0 2  4 6  -- 8 10 12 14 | 1 5  9 13 -- 17 21 25 29 | 1
        // 1 3  5 7  -- 9 11 13 15 | 3 7 11 15 -- 19 23 27 31 | 3
        vsubll8(res1.val[1], vget_low_u8(a.val[0]), vget_low_u8(b.val[0]));
        vsubll8(res1.val[3], vget_low_u8(a.val[1]), vget_low_u8(b.val[1]));

        // 16 18 20 22  -- 24 26 28 30 | 33 37 41 45 -- 49 53 57 61 | 1
        // 17 19 21 23  -- 25 27 29 31 | 35 39 43 47 -- 51 55 59 63 | 3
        vsublh8(res2.val[1], a.val[0], b.val[0]);
        vsublh8(res2.val[3], a.val[1], b.val[1]);

        // 0 2  4 6  -- 8 10 12 14 | 0 4  8 12 -- 16 20 24 28 | 1-0
        // 0 2  4 6  -- 8 10 12 14 | 1 5  9 13 -- 17 21 25 29 | 1-1
        // 1 3  5 7  -- 9 11 13 15 | 2 6 10 14 -- 18 22 26 30 | 1-2
        // 1 3  5 7  -- 9 11 13 15 | 3 7 11 15 -- 19 23 27 31 | 1-3
        // 16 18 20 22  -- 24 26 28 30 | 32 36 40 44 -- 48 52 56 60 | 2-0
        // 16 18 20 22  -- 24 26 28 30 | 33 37 41 45 -- 49 53 57 61 | 2-1
        // 17 19 21 23  -- 25 27 29 31 | 34 38 42 46 -- 50 54 58 62 | 2-2
        // 17 19 21 23  -- 25 27 29 31 | 35 39 43 47 -- 51 55 59 63 | 2-3
        vstore4(&r[j], res1);
        j += 32;
        vstore4(&r[j], res2);
        j += 32;
    }
}

/*************************************************
* Name:        load24_littleendian
*
* Description: load 3 bytes into a 32-bit integer
*              in little-endian order.
*              This function is only needed for Kyber-512
*
* Arguments:   - const uint8_t *x: pointer to input byte array
*
* Returns 32-bit unsigned integer loaded from x (most significant byte is zero)
**************************************************/
static uint32_t load24_littleendian(const uint8_t x[3]) {
    uint32_t r;
    r  = (uint32_t)x[0];
    r |= (uint32_t)x[1] << 8;
    r |= (uint32_t)x[2] << 16;
    return r;
}

/*************************************************
* Name:        cbd3
*
* Description: Given an array of uniformly random bytes, compute
*              polynomial with coefficients distributed according to
*              a centered binomial distribution with parameter eta=3.
*              This function is only needed for Kyber-512
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *buf: pointer to input byte array
**************************************************/
static void cbd3(int16_t *r, const uint8_t buf[3 * KYBER_N / 4]) {
    unsigned int i, j;
    uint32_t t, d;
    int16_t a, b;

    for (i = 0; i < KYBER_N / 4; i++) {
        t  = load24_littleendian(buf + 3 * i);
        d  = t & 0x00249249;
        d += (t >> 1) & 0x00249249;
        d += (t >> 2) & 0x00249249;

        for (j = 0; j < 4; j++) {
            a = (d >> (6 * j + 0)) & 0x7;
            b = (d >> (6 * j + 3)) & 0x7;
            r[4 * i + j] = a - b;
        }
    }
}

void poly_cbd_eta1(int16_t *r, const uint8_t buf[KYBER_ETA1 * KYBER_N / 4]) {
    cbd3(r, buf);
}

void poly_cbd_eta2(int16_t *r, const uint8_t buf[KYBER_ETA2 * KYBER_N / 4]) {
    neon_cbd2(r, buf);
}
