#include "cbd.h"
#include "ntt.h"
#include "params.h"
#include "poly.h"
#include "reduce.h"
#include "symmetric.h"
#include <arm_neon.h>

/*************************************************
* Name:        poly_compress
*
* Description: Compression and subsequent serialization of a polynomial
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (of length KYBER_POLYCOMPRESSEDBYTES)
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_compress(uint8_t r[KYBER_POLYCOMPRESSEDBYTES], const int16_t a[KYBER_N]) {
    unsigned int i, j;
    int16_t u;
    uint8_t t[8];

    for (i = 0; i < KYBER_N / 8; i++) {
        for (j = 0; j < 8; j++) {
            // map to positive standard representatives
            u  = a[8 * i + j];
            u += (u >> 15) & KYBER_Q;
            t[j] = ((((uint16_t)u << 4) + KYBER_Q / 2) / KYBER_Q) & 15;
        }

        r[0] = t[0] | (t[1] << 4);
        r[1] = t[2] | (t[3] << 4);
        r[2] = t[4] | (t[5] << 4);
        r[3] = t[6] | (t[7] << 4);
        r += 4;
    }
}

/*************************************************
* Name:        poly_decompress
*
* Description: De-serialization and subsequent decompression of a polynomial;
*              approximate inverse of poly_compress
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: pointer to input byte array
*                                  (of length KYBER_POLYCOMPRESSEDBYTES bytes)
**************************************************/
void poly_decompress(int16_t r[KYBER_N], const uint8_t a[KYBER_POLYCOMPRESSEDBYTES]) {
    unsigned int i;

    for (i = 0; i < KYBER_N / 2; i++) {
        r[2 * i + 0] = (((uint16_t)(a[0] & 15) * KYBER_Q) + 8) >> 4;
        r[2 * i + 1] = (((uint16_t)(a[0] >> 4) * KYBER_Q) + 8) >> 4;
        a += 1;
    }
}

/*************************************************
* Name:        poly_tobytes
*
* Description: Serialization of a polynomial
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYBYTES bytes)
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_tobytes(uint8_t r[KYBER_POLYBYTES], const int16_t a[KYBER_N]) {
    unsigned int i;
    uint16_t t0, t1;

    for (i = 0; i < KYBER_N / 2; i++) {
        // map to positive standard representatives
        t0  = a[2 * i];
        t0 += ((int16_t)t0 >> 15) & KYBER_Q;
        t1 = a[2 * i + 1];
        t1 += ((int16_t)t1 >> 15) & KYBER_Q;
        r[3 * i + 0] = (t0 >> 0);
        r[3 * i + 1] = (t0 >> 8) | (t1 << 4);
        r[3 * i + 2] = (t1 >> 4);
    }
}

/*************************************************
* Name:        poly_frombytes
*
* Description: De-serialization of a polynomial;
*              inverse of poly_tobytes
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *a: pointer to input byte array
*                                  (of KYBER_POLYBYTES bytes)
**************************************************/
void poly_frombytes(int16_t r[KYBER_N], const uint8_t a[KYBER_POLYBYTES]) {
    uint8x16x3_t neon_buf;
    uint16x8x4_t tmp;
    int16x8x4_t value;
    uint16x8_t const_0xfff;
    const_0xfff = vdupq_n_u16(0xfff);

    unsigned int i, j = 0;
    for (i = 0; i < KYBER_POLYBYTES; i += 48) {
        neon_buf = vld3q_u8(&a[i]);

        // Val0: 0-1 | 3-4 | 6-7| 9-10
        tmp.val[0] = (uint16x8_t)vzip1q_u8(neon_buf.val[0], neon_buf.val[1]);
        tmp.val[1] = (uint16x8_t)vzip2q_u8(neon_buf.val[0], neon_buf.val[1]);

        tmp.val[0] = vandq_u16(tmp.val[0], const_0xfff);
        tmp.val[1] = vandq_u16(tmp.val[1], const_0xfff);

        // Val1: 1-2 | 4-5 | 7-8 | 10-11
        tmp.val[2] = (uint16x8_t)vzip1q_u8(neon_buf.val[1], neon_buf.val[2]);
        tmp.val[3] = (uint16x8_t)vzip2q_u8(neon_buf.val[1], neon_buf.val[2]);

        tmp.val[2] = vshrq_n_u16(tmp.val[2], 4);
        tmp.val[3] = vshrq_n_u16(tmp.val[3], 4);

        // Final value
        value.val[0] = (int16x8_t)vzip1q_u16(tmp.val[0], tmp.val[2]);
        value.val[1] = (int16x8_t)vzip2q_u16(tmp.val[0], tmp.val[2]);
        value.val[2] = (int16x8_t)vzip1q_u16(tmp.val[1], tmp.val[3]);
        value.val[3] = (int16x8_t)vzip2q_u16(tmp.val[1], tmp.val[3]);

        vst1q_s16_x4(r + j, value);
        j += 32;
    }
}

/*************************************************
* Name:        poly_frommsg
*
* Description: Convert 32-byte message to polynomial
*
* Arguments:   - poly *r: pointer to output polynomial
*              - const uint8_t *msg: pointer to input message
**************************************************/
void poly_frommsg(int16_t r[KYBER_N], const uint8_t msg[KYBER_INDCPA_MSGBYTES]) {
    unsigned int i, j;
    int16_t mask;


    for (i = 0; i < KYBER_N / 8; i++) {
        for (j = 0; j < 8; j++) {
            mask = -(int16_t)((msg[i] >> j) & 1);
            r[8 * i + j] = mask & ((KYBER_Q + 1) / 2);
        }
    }
}

/*************************************************
* Name:        poly_tomsg
*
* Description: Convert polynomial to 32-byte message
*
* Arguments:   - uint8_t *msg: pointer to output message
*              - const poly *a: pointer to input polynomial
**************************************************/
void poly_tomsg(uint8_t msg[KYBER_INDCPA_MSGBYTES], const int16_t a[KYBER_N]) {
    unsigned int i, j;
    uint16_t t;

    for (i = 0; i < KYBER_N / 8; i++) {
        msg[i] = 0;
        for (j = 0; j < 8; j++) {
            t  = a[8 * i + j];
            t += ((int16_t)t >> 15) & KYBER_Q;
            t  = (((t << 1) + KYBER_Q / 2) / KYBER_Q) & 1;
            msg[i] |= t << j;
        }
    }
}

