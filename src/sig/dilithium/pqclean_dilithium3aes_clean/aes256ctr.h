#ifndef PQCLEAN_DILITHIUM3AES_CLEAN_AES256CTR_H
#define PQCLEAN_DILITHIUM3AES_CLEAN_AES256CTR_H

#include <stddef.h>
#include <stdint.h>

#define AES256CTR_BLOCKBYTES 64


typedef struct {
    uint64_t sk_exp[120];
    uint32_t ivw[16];
} aes256ctr_ctx;

void PQCLEAN_DILITHIUM3AES_CLEAN_aes256ctr_prf(uint8_t *out,
        size_t outlen,
        const uint8_t key[32],
        const uint8_t nonce[12]);

void PQCLEAN_DILITHIUM3AES_CLEAN_aes256ctr_init(aes256ctr_ctx *state,
        const uint8_t key[32],
        const uint8_t nonce[12]);

void PQCLEAN_DILITHIUM3AES_CLEAN_aes256ctr_squeezeblocks(uint8_t *out,
        size_t nblocks,
        aes256ctr_ctx *state);

#endif
