#ifndef PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_HARAKA_H
#define PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_HARAKA_H

#include <immintrin.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    __m128i rc[40];
    __m128i rc_sseed[40];
} harakactx;

/* Tweak constants with seed */
void PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_tweak_constants(
    harakactx *state,
    const unsigned char *pk_seed, const unsigned char *sk_seed,
    unsigned long long seed_length);

/* Haraka Sponge */
void PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_haraka_S_inc_init(uint8_t *s_inc);
void PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_haraka_S_inc_absorb(uint8_t *s_inc, const uint8_t *m, size_t mlen, const harakactx *state);
void PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_haraka_S_inc_finalize(uint8_t *s_inc);
void PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_haraka_S_inc_squeeze(uint8_t *out, size_t outlen, uint8_t *s_inc, const harakactx *state);
void PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_haraka_S(
    unsigned char *out, unsigned long long outlen,
    const unsigned char *in, unsigned long long inlen, const harakactx *state);
void PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_haraka_Sx4(
    unsigned char *out0,
    unsigned char *out1,
    unsigned char *out2,
    unsigned char *out3,
    unsigned long long outlen,
    const unsigned char *in0,
    const unsigned char *in1,
    const unsigned char *in2,
    const unsigned char *in3,
    unsigned long long inlen,
    const harakactx *state);


/* Applies the 512-bit Haraka permutation to in. */
void PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_haraka512_perm(unsigned char *out, const unsigned char *in, const harakactx *state);
void PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_haraka512_perm_x4(unsigned char *out, const unsigned char *in, const harakactx *state);

/* Implementation of Haraka-512 */
void PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_haraka512(unsigned char *out, const unsigned char *in, const harakactx *state);
void PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_haraka512x4(unsigned char *out, const unsigned char *in, const harakactx *state);

/* Implementation of Haraka-256 */
void PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_haraka256(unsigned char *out, const unsigned char *in, const harakactx *state);
void PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_haraka256x4(unsigned char *out, const unsigned char *in, const harakactx *state);

/* Implementation of Haraka-256 using sk.seed constants */
void PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_haraka256_sk(unsigned char *out, const unsigned char *in, const harakactx *state);
void PQCLEAN_SPHINCSHARAKA192FSIMPLE_AESNI_haraka256_skx4(unsigned char *out, const unsigned char *in, const harakactx *state);

#endif
