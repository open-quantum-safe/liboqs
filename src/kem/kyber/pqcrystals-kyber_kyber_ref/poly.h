#ifndef POLY_H
#define POLY_H

#include <stdint.h>
#include "params.h"

/*
 * Elements of R_q = Z_q[X]/(X^n + 1). Represents polynomial
 * coeffs[0] + X*coeffs[1] + X^2*xoeffs[2] + ... + X^{n-1}*coeffs[n-1]
 */
typedef struct{
  int16_t coeffs[256];  // KYBER_N = 256
} poly;

// #define poly_compress KYBER_NAMESPACE(poly_compress)
void poly_compress(uint8_t *r, const poly *a,int8_t security_level);
// #define poly_decompress KYBER_NAMESPACE(poly_decompress)
void poly_decompress(poly *r, const uint8_t *a, int8_t security_level);

// #define poly_tobytes KYBER_NAMESPACE(poly_tobytes)
void poly_tobytes(uint8_t *r, const poly *a, int8_t security_level);
// #define poly_frombytes KYBER_NAMESPACE(poly_frombytes)
void poly_frombytes(poly *r, const uint8_t *a, int8_t security_level);

// #define poly_frommsg KYBER_NAMESPACE(poly_frommsg)
void poly_frommsg(poly *r, const uint8_t *msg, int8_t security_level);
// #define poly_tomsg KYBER_NAMESPACE(poly_tomsg)
void poly_tomsg(uint8_t *msg, const poly *r,int8_t security_level);

// #define poly_getnoise_eta1 KYBER_NAMESPACE(poly_getnoise_eta1)
void poly_getnoise_eta1(poly *r, const uint8_t *seed, uint8_t nonce, int8_t security_level);

// #define poly_getnoise_eta2 KYBER_NAMESPACE(poly_getnoise_eta2)
void poly_getnoise_eta2(poly *r, const uint8_t *seed, uint8_t nonce, int8_t security_level);

// #define poly_ntt KYBER_NAMESPACE(poly_ntt)
void poly_ntt(poly *r,int8_t security_level);
// #define poly_invntt_tomont KYBER_NAMESPACE(poly_invntt_tomont)
void poly_invntt_tomont(poly *r);
// #define poly_basemul_montgomery KYBER_NAMESPACE(poly_basemul_montgomery)
void poly_basemul_montgomery(poly *r, const poly *a, const poly *b, int8_t security_level);
// #define poly_tomont KYBER_NAMESPACE(poly_tomont)
void poly_tomont(poly *r,int8_t security_level);

// #define poly_reduce KYBER_NAMESPACE(poly_reduce)
void poly_reduce(poly *r,int8_t security_level);

// #define poly_add KYBER_NAMESPACE(poly_add)
void poly_add(poly *r, const poly *a, const poly *b,int8_t security_level);
// #define poly_sub KYBER_NAMESPACE(poly_sub)
void poly_sub(poly *r, const poly *a, const poly *b, int8_t security_level);

#endif
