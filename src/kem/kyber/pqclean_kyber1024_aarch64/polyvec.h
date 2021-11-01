#ifndef POLYVEC_H
#define POLYVEC_H

#include "params.h"
#include "poly.h"
#include <stdint.h>

typedef struct {
    poly vec[KYBER_K];
} polyvec;

#define polyvec_compress KYBER_NAMESPACE(polyvec_compress)
void polyvec_compress(uint8_t r[KYBER_POLYVECCOMPRESSEDBYTES], int16_t a[KYBER_K][KYBER_N]);
#define polyvec_decompress KYBER_NAMESPACE(polyvec_decompress)
void polyvec_decompress(int16_t r[KYBER_K][KYBER_N], const uint8_t a[KYBER_POLYVECCOMPRESSEDBYTES]);

#define polyvec_tobytes KYBER_NAMESPACE(polyvec_tobytes)
void polyvec_tobytes(uint8_t r[KYBER_POLYVECBYTES], int16_t a[KYBER_K][KYBER_N]);
#define polyvec_frombytes KYBER_NAMESPACE(polyvec_frombytes)
void polyvec_frombytes(int16_t r[KYBER_K][KYBER_N], const uint8_t a[KYBER_POLYVECBYTES]);

// Add NEON

#define neon_polyvec_ntt KYBER_NAMESPACE(polyvec_ntt)
void neon_polyvec_ntt(int16_t r[KYBER_K][KYBER_N]);

#define neon_polyvec_invntt_to_mont KYBER_NAMESPACE(polyvec_invntt_to_mont)
void neon_polyvec_invntt_to_mont(int16_t r[KYBER_K][KYBER_N]);

#define neon_polyvec_add_reduce KYBER_NAMESPACE(polyvec_add_reduce)
void neon_polyvec_add_reduce(int16_t c[KYBER_K][KYBER_N], int16_t a[KYBER_K][KYBER_N]);


#endif
