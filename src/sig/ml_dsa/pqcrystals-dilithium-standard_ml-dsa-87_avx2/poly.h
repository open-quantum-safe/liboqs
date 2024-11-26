#ifndef POLY_H
#define POLY_H

#include <stdint.h>
#include "align.h"
#include "params.h"
#include "symmetric.h"

typedef ALIGNED_INT32(N) poly;

#define poly_reduce DILITHIUM_NAMESPACE(poly_reduce)
void poly_reduce(poly *a);
#define poly_caddq DILITHIUM_NAMESPACE(poly_caddq)
void poly_caddq(poly *a);

#define poly_add DILITHIUM_NAMESPACE(poly_add)
void poly_add(poly *c, const poly *a, const poly *b);
#define poly_sub DILITHIUM_NAMESPACE(poly_sub)
void poly_sub(poly *c, const poly *a, const poly *b);
#define poly_shiftl DILITHIUM_NAMESPACE(poly_shiftl)
void poly_shiftl(poly *a);

#define poly_ntt DILITHIUM_NAMESPACE(poly_ntt)
void poly_ntt(poly *a);
#define poly_invntt_tomont DILITHIUM_NAMESPACE(poly_invntt_tomont)
void poly_invntt_tomont(poly *a);
#define poly_nttunpack DILITHIUM_NAMESPACE(poly_nttunpack)
void poly_nttunpack(poly *a);
#define poly_pointwise_montgomery DILITHIUM_NAMESPACE(poly_pointwise_montgomery)
void poly_pointwise_montgomery(poly *c, const poly *a, const poly *b);

#define poly_power2round DILITHIUM_NAMESPACE(poly_power2round)
void poly_power2round(poly *a1, poly *a0, const poly *a);
#define poly_decompose DILITHIUM_NAMESPACE(poly_decompose)
void poly_decompose(poly *a1, poly *a0, const poly *a);
#define poly_make_hint DILITHIUM_NAMESPACE(poly_make_hint)
unsigned int poly_make_hint(uint8_t hint[N], const poly *a0, const poly *a1);
#define poly_use_hint DILITHIUM_NAMESPACE(poly_use_hint)
void poly_use_hint(poly *b, const poly *a, const poly *h);

#define poly_chknorm DILITHIUM_NAMESPACE(poly_chknorm)
int poly_chknorm(const poly *a, int32_t B);
#define poly_uniform_preinit DILITHIUM_NAMESPACE(poly_uniform_preinit)
void poly_uniform_preinit(poly *a, stream128_state *state);
#define poly_uniform DILITHIUM_NAMESPACE(poly_uniform)
void poly_uniform(poly *a, const uint8_t seed[SEEDBYTES], uint16_t nonce);
#define poly_uniform_eta_preinit DILITHIUM_NAMESPACE(poly_uniform_eta_preinit)
void poly_uniform_eta_preinit(poly *a, stream256_state *state);
#define poly_uniform_eta DILITHIUM_NAMESPACE(poly_uniform_eta)
void poly_uniform_eta(poly *a, const uint8_t seed[CRHBYTES], uint16_t nonce);
#define poly_uniform_gamma1_preinit DILITHIUM_NAMESPACE(poly_uniform_gamma1_preinit)
void poly_uniform_gamma1_preinit(poly *a, stream256_state *state);
#define poly_uniform_gamma1 DILITHIUM_NAMESPACE(poly_uniform_gamma1)
void poly_uniform_gamma1(poly *a, const uint8_t seed[CRHBYTES], uint16_t nonce);
#define poly_challenge DILITHIUM_NAMESPACE(poly_challenge)
void poly_challenge(poly *c, const uint8_t seed[CTILDEBYTES]);

#define poly_uniform_4x DILITHIUM_NAMESPACE(poly_uniform_4x)
void poly_uniform_4x(poly *a0,
                     poly *a1,
                     poly *a2,
                     poly *a3,
                     const uint8_t seed[SEEDBYTES],
                     uint16_t nonce0,
                     uint16_t nonce1,
                     uint16_t nonce2,
                     uint16_t nonce3);
#define poly_uniform_eta_4x DILITHIUM_NAMESPACE(poly_uniform_eta_4x)
void poly_uniform_eta_4x(poly *a0,
                         poly *a1,
                         poly *a2,
                         poly *a3,
                         const uint8_t seed[CRHBYTES],
                         uint16_t nonce0,
                         uint16_t nonce1,
                         uint16_t nonce2,
                         uint16_t nonce3);
#define poly_uniform_gamma1_4x DILITHIUM_NAMESPACE(poly_uniform_gamma1_4x)
void poly_uniform_gamma1_4x(poly *a0,
                            poly *a1,
                            poly *a2,
                            poly *a3,
                            const uint8_t seed[CRHBYTES],
                            uint16_t nonce0,
                            uint16_t nonce1,
                            uint16_t nonce2,
                            uint16_t nonce3);

#define polyeta_pack DILITHIUM_NAMESPACE(polyeta_pack)
void polyeta_pack(uint8_t r[POLYETA_PACKEDBYTES], const poly *a);
#define polyeta_unpack DILITHIUM_NAMESPACE(polyeta_unpack)
void polyeta_unpack(poly *r, const uint8_t a[POLYETA_PACKEDBYTES]);

#define polyt1_pack DILITHIUM_NAMESPACE(polyt1_pack)
void polyt1_pack(uint8_t r[POLYT1_PACKEDBYTES], const poly *a);
#define polyt1_unpack DILITHIUM_NAMESPACE(polyt1_unpack)
void polyt1_unpack(poly *r, const uint8_t a[POLYT1_PACKEDBYTES]);

#define polyt0_pack DILITHIUM_NAMESPACE(polyt0_pack)
void polyt0_pack(uint8_t r[POLYT0_PACKEDBYTES], const poly *a);
#define polyt0_unpack DILITHIUM_NAMESPACE(polyt0_unpack)
void polyt0_unpack(poly *r, const uint8_t a[POLYT0_PACKEDBYTES]);

#define polyz_pack DILITHIUM_NAMESPACE(polyz_pack)
void polyz_pack(uint8_t r[POLYZ_PACKEDBYTES], const poly *a);
#define polyz_unpack DILITHIUM_NAMESPACE(polyz_unpack)
void polyz_unpack(poly *r, const uint8_t *a);

#define polyw1_pack DILITHIUM_NAMESPACE(polyw1_pack)
void polyw1_pack(uint8_t *r, const poly *a);

#endif
