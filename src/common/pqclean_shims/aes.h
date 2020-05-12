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

#define aes256_ecb_keyexp(r, key) OQS_AES256_ECB_load_schedule((key), (r), 1);
#define aes256_ecb(out, in, nblocks, ctx) OQS_AES256_ECB_enc_sch((in), (nblocks) * AES_BLOCKBYTES, *(ctx), (out));
#define aes256_ctr_keyexp(r, key) OQS_AES256_CTR_load_schedule((key), (r));
#define aes256_ctr(out, outlen, iv, ctx) OQS_AES256_CTR_sch((iv), AESCTR_NONCEBYTES, *(ctx), (out), (outlen))
#define aes256_ctx_release(ctx) OQS_AES256_free_schedule(*(ctx));

#endif
