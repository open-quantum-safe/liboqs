/**
 * @file crypto.c
 * @author Takuya HAYASHI (t-hayashi@eedept.kobe-u.ac.jp)
 * @brief Wrapper for implementations (currently OpenSSL) of crypto primitives
 */

#include "crypto.h"

/**
 * @brief wrapper for cryptographically hash function
 * @param[out] digest message digest, must have _LOTUS_HASH_DIGEST_BYTES bytes
 * @param[in] msg message to be hashed
 * @param[in] len message length (bytes)
 */
void crypto_hash(unsigned char *digest, const unsigned char *msg, const unsigned int len){
  EVP_MD_CTX *ctx;
  ctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex(ctx, _LOTUS_HASH_TYPE, NULL);
  EVP_DigestUpdate(ctx, msg, len);
  EVP_DigestFinal(ctx, digest, NULL);
  EVP_MD_CTX_free(ctx);
}

/**
 * @brief wrapper for key-set-up function of symmetric key encryption 
 * @param[in] ctx context to be destoryed, can be NULL
 * @param[in] key a key to be used to initialize ctx, assumed to have _LOTUS_SYMENC_KEY_BYTES bytes
 * @return memory allocated ctx
 */
LOTUS_SYMENC_ctx *crypto_symenc_keysetup(LOTUS_SYMENC_ctx *ctx, const unsigned char *key){
  EVP_CIPHER_CTX_free(ctx);
  ctx = EVP_CIPHER_CTX_new();
  EVP_EncryptInit_ex(ctx, _LOTUS_SYMENC_TYPE, NULL, key, NULL);
  return ctx;
}

/**
 * @brief wrapper for function to output key-stream, used to obtain cryptographically random bytes
 * @param[in,out] ctx context of symmetric key encryption
 * @param[out] stream key stream (or cryptographically random bytes) 
 * @param[in] len length of key stream (bytes)
 */
void crypto_symenc_keystream(LOTUS_SYMENC_ctx *ctx, unsigned char *stream, const unsigned int len){
  const unsigned char zero[_LOTUS_SYMENC_BLOCK_BYTES] = {0};
  unsigned char tmp[_LOTUS_SYMENC_BLOCK_BYTES];
  int i, q = len / _LOTUS_SYMENC_BLOCK_BYTES, r = len % _LOTUS_SYMENC_BLOCK_BYTES, olen;
  for(i = 0; i < q; ++i){
    EVP_EncryptUpdate(ctx, stream + i * _LOTUS_SYMENC_BLOCK_BYTES, &olen,
                      zero, _LOTUS_SYMENC_BLOCK_BYTES);
  }

  if(r){
    EVP_EncryptUpdate(ctx, tmp, &olen, zero, _LOTUS_SYMENC_BLOCK_BYTES);
  }
  
  for(i = 0; i < r; ++i){
    stream[q * _LOTUS_SYMENC_BLOCK_BYTES + i] = tmp[i];
  }
}

/**
 * @brief take key-stream 13 blocks (13 * 16 bytes)
 */
void crypto_symenc_keystream_13block(LOTUS_SYMENC_ctx *ctx, unsigned char *stream)
{
  const unsigned char zero[_LOTUS_SYMENC_BLOCK_BYTES] = {0};
  int i, olen;
  for(i = 0; i < 13; ++i){
    EVP_EncryptUpdate(ctx, stream, &olen, zero, _LOTUS_SYMENC_BLOCK_BYTES);
    stream += _LOTUS_SYMENC_BLOCK_BYTES;
  }
}

/**
 * @brief take key-stream 32 blocks (32 * 16 bytes)
 */
void crypto_symenc_keystream_32block(LOTUS_SYMENC_ctx *ctx, unsigned char *stream)
{
  const unsigned char zero[_LOTUS_SYMENC_BLOCK_BYTES] = {0};
  int i, olen;
  for(i = 0; i < 32; ++i){
    EVP_EncryptUpdate(ctx, stream, &olen, zero, _LOTUS_SYMENC_BLOCK_BYTES);
    stream += _LOTUS_SYMENC_BLOCK_BYTES;
  }
}
