// SPDX-License-Identifier: MIT

#ifndef AES_H
#define AES_H

#include <stdint.h>
#include <stdlib.h>

#include <oqs/aes.h>

#define AES256_KEYBYTES 32
#define AESCTR_NONCEBYTES 12
#define AES_BLOCKBYTES 16

typedef void *aes256ctx;

#define aes256_ecb_keyexp(r, key) OQS_AES256_ECB_load_schedule((key), (r));
#define aes256_ecb(out, in, nblocks, ctx) OQS_AES256_ECB_enc_sch((in), (nblocks) * AES_BLOCKBYTES, *(ctx), (out));
#define aes256_ctr_keyexp(r, key) OQS_AES256_CTR_load_schedule((key), (r));
#define aes256_ctr(out, outlen, iv, ctx) OQS_AES256_CTR_sch((iv), AESCTR_NONCEBYTES, *(ctx), (out), (outlen));
#define aes256_ctx_release(ctx) OQS_AES256_free_schedule(*(ctx));

#define aes256ctr_squeezeblocks(out, outlen, state) OQS_AES256_CTR_sch_ivinit(*state, out, 64*outlen);
#define aes256ctr_init_key(state, key) OQS_AES256_CTR_load_schedule(key, state);
#define aes256ctr_init_nonce(state, nonce) OQS_AES256_CTR_load_nonce(nonce, 12, state);

static inline void aes256ctr_init(void **_schedule, const uint8_t *key, const uint8_t *nonce) {
	aes256ctr_init_key(_schedule, key);
	aes256ctr_init_nonce(_schedule, nonce);
}

static inline void aes256ctr_prf(uint8_t *out, size_t outlen, const uint8_t key[32], uint8_t nonce[12]) {
	aes256ctx state;
	OQS_AES256_CTR_load_schedule(key, &state);
	OQS_AES256_CTR_sch(nonce, 12, state, out, outlen);
	OQS_AES256_free_schedule(state);
}

#endif
