#ifndef PQCLEAN_DILITHIUM2_AVX2_POLY_H
#define PQCLEAN_DILITHIUM2_AVX2_POLY_H
#include "align.h"
#include "params.h"
#include "symmetric.h"
#include <stdint.h>

typedef ALIGNED_INT32(N) poly;

void PQCLEAN_DILITHIUM2_AVX2_poly_reduce(poly *a);
void PQCLEAN_DILITHIUM2_AVX2_poly_caddq(poly *a);
void PQCLEAN_DILITHIUM2_AVX2_poly_freeze(poly *a);

void PQCLEAN_DILITHIUM2_AVX2_poly_add(poly *c, const poly *a, const poly *b);
void PQCLEAN_DILITHIUM2_AVX2_poly_sub(poly *c, const poly *a, const poly *b);
void PQCLEAN_DILITHIUM2_AVX2_poly_shiftl(poly *a);

void PQCLEAN_DILITHIUM2_AVX2_poly_ntt(poly *a);
void PQCLEAN_DILITHIUM2_AVX2_poly_invntt_tomont(poly *a);
void PQCLEAN_DILITHIUM2_AVX2_poly_nttunpack(poly *a);
void PQCLEAN_DILITHIUM2_AVX2_poly_pointwise_montgomery(poly *c, const poly *a, const poly *b);

void PQCLEAN_DILITHIUM2_AVX2_poly_power2round(poly *a1, poly *a0, const poly *a);
void PQCLEAN_DILITHIUM2_AVX2_poly_decompose(poly *a1, poly *a0, const poly *a);
unsigned int PQCLEAN_DILITHIUM2_AVX2_poly_make_hint(uint8_t hint[N], const poly *a0, const poly *a1);
void PQCLEAN_DILITHIUM2_AVX2_poly_use_hint(poly *b, const poly *a, const poly *h);

int PQCLEAN_DILITHIUM2_AVX2_poly_chknorm(const poly *a, int32_t B);
void PQCLEAN_DILITHIUM2_AVX2_poly_uniform_preinit(poly *a, stream128_state *state);
void PQCLEAN_DILITHIUM2_AVX2_poly_uniform(poly *a, const uint8_t seed[SEEDBYTES], uint16_t nonce);
void PQCLEAN_DILITHIUM2_AVX2_poly_uniform_eta_preinit(poly *a, stream128_state *state);
void PQCLEAN_DILITHIUM2_AVX2_poly_uniform_eta(poly *a, const uint8_t seed[SEEDBYTES], uint16_t nonce);
void PQCLEAN_DILITHIUM2_AVX2_poly_uniform_gamma1_preinit(poly *a, stream256_state *state);
void PQCLEAN_DILITHIUM2_AVX2_poly_uniform_gamma1(poly *a, const uint8_t seed[CRHBYTES], uint16_t nonce);
void PQCLEAN_DILITHIUM2_AVX2_poly_challenge(poly *c, const uint8_t seed[SEEDBYTES]);

void PQCLEAN_DILITHIUM2_AVX2_poly_uniform_4x(poly *a0,
        poly *a1,
        poly *a2,
        poly *a3,
        const uint8_t seed[SEEDBYTES],
        uint16_t nonce0,
        uint16_t nonce1,
        uint16_t nonce2,
        uint16_t nonce3);
void PQCLEAN_DILITHIUM2_AVX2_poly_uniform_eta_4x(poly *a0,
        poly *a1,
        poly *a2,
        poly *a3,
        const uint8_t seed[SEEDBYTES],
        uint16_t nonce0,
        uint16_t nonce1,
        uint16_t nonce2,
        uint16_t nonce3);
void PQCLEAN_DILITHIUM2_AVX2_poly_uniform_gamma1_4x(poly *a0,
        poly *a1,
        poly *a2,
        poly *a3,
        const uint8_t seed[CRHBYTES],
        uint16_t nonce0,
        uint16_t nonce1,
        uint16_t nonce2,
        uint16_t nonce3);

void PQCLEAN_DILITHIUM2_AVX2_polyeta_pack(uint8_t r[POLYETA_PACKEDBYTES], const poly *a);
void PQCLEAN_DILITHIUM2_AVX2_polyeta_unpack(poly *r, const uint8_t a[POLYETA_PACKEDBYTES]);

void PQCLEAN_DILITHIUM2_AVX2_polyt1_pack(uint8_t r[POLYT1_PACKEDBYTES], const poly *a);
void PQCLEAN_DILITHIUM2_AVX2_polyt1_unpack(poly *r, const uint8_t a[POLYT1_PACKEDBYTES]);

void PQCLEAN_DILITHIUM2_AVX2_polyt0_pack(uint8_t r[POLYT0_PACKEDBYTES], const poly *a);
void PQCLEAN_DILITHIUM2_AVX2_polyt0_unpack(poly *r, const uint8_t a[POLYT0_PACKEDBYTES]);

void PQCLEAN_DILITHIUM2_AVX2_polyz_pack(uint8_t r[POLYZ_PACKEDBYTES], const poly *a);
void PQCLEAN_DILITHIUM2_AVX2_polyz_unpack(poly *r, const uint8_t a[POLYZ_PACKEDBYTES + 14]);

void PQCLEAN_DILITHIUM2_AVX2_polyw1_pack(uint8_t r[POLYW1_PACKEDBYTES + 8], const poly *a);

#endif
