/**
 * @file crypto.h
 * @author Takuya HAYASHI (t-hayashi@eedept.kobe-u.ac.jp)
 */

#ifndef _CRYPTO_H
#define _CRYPTO_H

#include <openssl/evp.h>
/* hash function */
#define _LOTUS_HASH_TYPE EVP_sha512()
#define _LOTUS_HASH_DIGEST_BYTES 64
#define _LOTUS_HASH_FLAG_G 0x01 /* flag for hash function G */
#define _LOTUS_HASH_FLAG_H 0x02 /* flag for hash function H */

/* symmetric key encryption */
typedef EVP_CIPHER_CTX LOTUS_SYMENC_ctx;
#define _LOTUS_SYMENC_TYPE EVP_aes_256_ctr() 
#define _LOTUS_SYMENC_BLOCK_BYTES 16
#define _LOTUS_SYMENC_KEY_BYTES 32

/* size for internal randomness pool */
#define _LOTUS_RANDOMNESS_BYTES 512
#define _LOTUS_RANDOMNESS_BITS (_LOTUS_RANDOMNESS_BYTES * 8)

/* bytes of seed */
#define _LOTUS_SEED_BYTES _LOTUS_SYMENC_KEY_BYTES

void crypto_hash(unsigned char *digest, const unsigned char *msg, const unsigned int len);
LOTUS_SYMENC_ctx *crypto_symenc_keysetup(LOTUS_SYMENC_ctx *ctx, const unsigned char *key);
void crypto_symenc_keystream(LOTUS_SYMENC_ctx *ctx, unsigned char *stream, const unsigned int len);
void crypto_symenc_keystream_13block(LOTUS_SYMENC_ctx *ctx, unsigned char *stream);
void crypto_symenc_keystream_32block(LOTUS_SYMENC_ctx *ctx, unsigned char *stream);
#endif
