#include "params.h"
#include "poly.h"
#include "polyvec.h"
#include <stdint.h>

/*************************************************
* Name:        polyvec_compress
*
* Description: Compress and serialize vector of polynomials
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYVECCOMPRESSEDBYTES)
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
void polyvec_compress(uint8_t r[KYBER_POLYVECCOMPRESSEDBYTES], int16_t a[KYBER_K][KYBER_N]) {
    unsigned int i, j, k;

    uint16_t t[4];
    for (i = 0; i < KYBER_K; i++) {
        for (j = 0; j < KYBER_N / 4; j++) {
            for (k = 0; k < 4; k++) {
                t[k]  = a[i][4 * j + k];
                t[k] += ((int16_t)t[k] >> 15) & KYBER_Q;
                t[k]  = ((((uint32_t)t[k] << 10) + KYBER_Q / 2) / KYBER_Q) & 0x3ff;
            }

            r[0] = (t[0] >> 0);
            r[1] = (t[0] >> 8) | (t[1] << 2);
            r[2] = (t[1] >> 6) | (t[2] << 4);
            r[3] = (t[2] >> 4) | (t[3] << 6);
            r[4] = (t[3] >> 2);
            r += 5;
        }
    }
}

/*************************************************
* Name:        polyvec_decompress
*
* Description: De-serialize and decompress vector of polynomials;
*              approximate inverse of polyvec_compress
*
* Arguments:   - polyvec *r:       pointer to output vector of polynomials
*              - const uint8_t *a: pointer to input byte array
*                                  (of length KYBER_POLYVECCOMPRESSEDBYTES)
**************************************************/
void polyvec_decompress(int16_t r[KYBER_K][KYBER_N], const uint8_t a[KYBER_POLYVECCOMPRESSEDBYTES]) {
    unsigned int i, j, k;

    uint16_t t[4];
    for (i = 0; i < KYBER_K; i++) {
        for (j = 0; j < KYBER_N / 4; j++) {
            t[0] = (a[0] >> 0) | ((uint16_t)a[1] << 8);
            t[1] = (a[1] >> 2) | ((uint16_t)a[2] << 6);
            t[2] = (a[2] >> 4) | ((uint16_t)a[3] << 4);
            t[3] = (a[3] >> 6) | ((uint16_t)a[4] << 2);
            a += 5;

            for (k = 0; k < 4; k++) {
                r[i][4 * j + k] = ((uint32_t)(t[k] & 0x3FF) * KYBER_Q + 512) >> 10;
            }
        }
    }
}

/*************************************************
* Name:        polyvec_tobytes
*
* Description: Serialize vector of polynomials
*
* Arguments:   - uint8_t *r: pointer to output byte array
*                            (needs space for KYBER_POLYVECBYTES)
*              - const polyvec *a: pointer to input vector of polynomials
**************************************************/
void polyvec_tobytes(uint8_t r[KYBER_POLYVECBYTES], int16_t a[KYBER_K][KYBER_N]) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        poly_tobytes(r + i * KYBER_POLYBYTES, a[i]);
    }
}

/*************************************************
* Name:        polyvec_frombytes
*
* Description: De-serialize vector of polynomials;
*              inverse of polyvec_tobytes
*
* Arguments:   - uint8_t *r:       pointer to output byte array
*              - const polyvec *a: pointer to input vector of polynomials
*                                  (of length KYBER_POLYVECBYTES)
**************************************************/
void polyvec_frombytes(int16_t r[KYBER_K][KYBER_N], const uint8_t a[KYBER_POLYVECBYTES]) {
    unsigned int i;
    for (i = 0; i < KYBER_K; i++) {
        poly_frombytes(r[i], a + i * KYBER_POLYBYTES);
    }
}

