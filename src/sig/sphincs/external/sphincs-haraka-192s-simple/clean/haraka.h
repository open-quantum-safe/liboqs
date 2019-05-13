#ifndef SPX_HARAKA_H
#define SPX_HARAKA_H

/* Tweak constants with seed */
void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_tweak_constants(
    const unsigned char *pk_seed, const unsigned char *sk_seed,
    unsigned long long seed_length);

/* Haraka Sponge */
void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka_S_inc_init(uint8_t *s_inc);
void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka_S_inc_absorb(uint8_t *s_inc, const uint8_t *m, size_t mlen);
void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka_S_inc_finalize(uint8_t *s_inc);
void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka_S_inc_squeeze(uint8_t *out, size_t outlen, uint8_t *s_inc);
void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka_S(
    unsigned char *out, unsigned long long outlen,
    const unsigned char *in, unsigned long long inlen);

/* Applies the 512-bit Haraka permutation to in. */
void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka512_perm(unsigned char *out, const unsigned char *in);

/* Implementation of Haraka-512 */
void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka512(unsigned char *out, const unsigned char *in);

/* Implementation of Haraka-256 */
void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka256(unsigned char *out, const unsigned char *in);

/* Implementation of Haraka-256 using sk.seed constants */
void PQCLEAN_SPHINCSHARAKA192SSIMPLE_CLEAN_haraka256_sk(unsigned char *out, const unsigned char *in);

#endif
