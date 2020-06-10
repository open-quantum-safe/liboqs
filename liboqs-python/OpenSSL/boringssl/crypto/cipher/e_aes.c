/* ====================================================================
 * Copyright (c) 2001-2011 The OpenSSL Project.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit. (http://www.openssl.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    openssl-core@openssl.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.openssl.org/)"
 *
 * THIS SOFTWARE IS PROVIDED BY THE OpenSSL PROJECT ``AS IS'' AND ANY
 * EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE OpenSSL PROJECT OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * ==================================================================== */

#include <string.h>

#include <openssl/aead.h>
#include <openssl/aes.h>
#include <openssl/cipher.h>
#include <openssl/cpu.h>
#include <openssl/err.h>
#include <openssl/mem.h>
#include <openssl/nid.h>
#include <openssl/rand.h>
#include <openssl/sha.h>

#include "internal.h"
#include "../internal.h"
#include "../modes/internal.h"

#if defined(OPENSSL_ARM) || defined(OPENSSL_AARCH64)
#include <openssl/arm_arch.h>
#endif


OPENSSL_MSVC_PRAGMA(warning(disable: 4702)) /* Unreachable code. */

typedef struct {
  union {
    double align;
    AES_KEY ks;
  } ks;
  block128_f block;
  union {
    cbc128_f cbc;
    ctr128_f ctr;
  } stream;
} EVP_AES_KEY;

typedef struct {
  union {
    double align;
    AES_KEY ks;
  } ks;        /* AES key schedule to use */
  int key_set; /* Set if key initialised */
  int iv_set;  /* Set if an iv is set */
  GCM128_CONTEXT gcm;
  uint8_t *iv; /* Temporary IV store */
  int ivlen;         /* IV length */
  int taglen;
  int iv_gen;      /* It is OK to generate IVs */
  ctr128_f ctr;
} EVP_AES_GCM_CTX;

#if !defined(OPENSSL_NO_ASM) && \
    (defined(OPENSSL_X86_64) || defined(OPENSSL_X86))
#define VPAES
static char vpaes_capable(void) {
  return (OPENSSL_ia32cap_P[1] & (1 << (41 - 32))) != 0;
}

#if defined(OPENSSL_X86_64)
#define BSAES
static char bsaes_capable(void) {
  return vpaes_capable();
}
#endif

#elif !defined(OPENSSL_NO_ASM) && \
    (defined(OPENSSL_ARM) || defined(OPENSSL_AARCH64))

#if defined(OPENSSL_ARM) && __ARM_MAX_ARCH__ >= 7
#define BSAES
static char bsaes_capable(void) {
  return CRYPTO_is_NEON_capable();
}
#endif

#define HWAES
static int hwaes_capable(void) {
  return CRYPTO_is_ARMv8_AES_capable();
}

#elif !defined(OPENSSL_NO_ASM) && defined(OPENSSL_PPC64LE)

#define HWAES
static int hwaes_capable(void) {
  return CRYPTO_is_PPC64LE_vcrypto_capable();
}

#endif  /* OPENSSL_PPC64LE */


#if defined(BSAES)
/* On platforms where BSAES gets defined (just above), then these functions are
 * provided by asm. */
void bsaes_cbc_encrypt(const uint8_t *in, uint8_t *out, size_t length,
                       const AES_KEY *key, uint8_t ivec[16], int enc);
void bsaes_ctr32_encrypt_blocks(const uint8_t *in, uint8_t *out, size_t len,
                                const AES_KEY *key, const uint8_t ivec[16]);
#else
static char bsaes_capable(void) {
  return 0;
}

/* On other platforms, bsaes_capable() will always return false and so the
 * following will never be called. */
static void bsaes_cbc_encrypt(const uint8_t *in, uint8_t *out, size_t length,
                              const AES_KEY *key, uint8_t ivec[16], int enc) {
  abort();
}

static void bsaes_ctr32_encrypt_blocks(const uint8_t *in, uint8_t *out,
                                       size_t len, const AES_KEY *key,
                                       const uint8_t ivec[16]) {
  abort();
}
#endif

#if defined(VPAES)
/* On platforms where VPAES gets defined (just above), then these functions are
 * provided by asm. */
int vpaes_set_encrypt_key(const uint8_t *userKey, int bits, AES_KEY *key);
int vpaes_set_decrypt_key(const uint8_t *userKey, int bits, AES_KEY *key);

void vpaes_encrypt(const uint8_t *in, uint8_t *out, const AES_KEY *key);
void vpaes_decrypt(const uint8_t *in, uint8_t *out, const AES_KEY *key);

void vpaes_cbc_encrypt(const uint8_t *in, uint8_t *out, size_t length,
                       const AES_KEY *key, uint8_t *ivec, int enc);
#else
static char vpaes_capable(void) {
  return 0;
}

/* On other platforms, vpaes_capable() will always return false and so the
 * following will never be called. */
static int vpaes_set_encrypt_key(const uint8_t *userKey, int bits,
                                 AES_KEY *key) {
  abort();
}
static int vpaes_set_decrypt_key(const uint8_t *userKey, int bits,
                                 AES_KEY *key) {
  abort();
}
static void vpaes_encrypt(const uint8_t *in, uint8_t *out, const AES_KEY *key) {
  abort();
}
static void vpaes_decrypt(const uint8_t *in, uint8_t *out, const AES_KEY *key) {
  abort();
}
static void vpaes_cbc_encrypt(const uint8_t *in, uint8_t *out, size_t length,
                              const AES_KEY *key, uint8_t *ivec, int enc) {
  abort();
}
#endif

#if defined(HWAES)
int aes_hw_set_encrypt_key(const uint8_t *user_key, const int bits,
                           AES_KEY *key);
int aes_hw_set_decrypt_key(const uint8_t *user_key, const int bits,
                           AES_KEY *key);
void aes_hw_encrypt(const uint8_t *in, uint8_t *out, const AES_KEY *key);
void aes_hw_decrypt(const uint8_t *in, uint8_t *out, const AES_KEY *key);
void aes_hw_cbc_encrypt(const uint8_t *in, uint8_t *out, size_t length,
                        const AES_KEY *key, uint8_t *ivec, const int enc);
void aes_hw_ctr32_encrypt_blocks(const uint8_t *in, uint8_t *out, size_t len,
                                 const AES_KEY *key, const uint8_t ivec[16]);
#else
/* If HWAES isn't defined then we provide dummy functions for each of the hwaes
 * functions. */
static int hwaes_capable(void) {
  return 0;
}

static int aes_hw_set_encrypt_key(const uint8_t *user_key, int bits,
                                  AES_KEY *key) {
  abort();
}

static int aes_hw_set_decrypt_key(const uint8_t *user_key, int bits,
                                  AES_KEY *key) {
  abort();
}

static void aes_hw_encrypt(const uint8_t *in, uint8_t *out,
                           const AES_KEY *key) {
  abort();
}

static void aes_hw_decrypt(const uint8_t *in, uint8_t *out,
                           const AES_KEY *key) {
  abort();
}

static void aes_hw_cbc_encrypt(const uint8_t *in, uint8_t *out, size_t length,
                               const AES_KEY *key, uint8_t *ivec, int enc) {
  abort();
}

static void aes_hw_ctr32_encrypt_blocks(const uint8_t *in, uint8_t *out,
                                        size_t len, const AES_KEY *key,
                                        const uint8_t ivec[16]) {
  abort();
}
#endif

#if !defined(OPENSSL_NO_ASM) && \
    (defined(OPENSSL_X86_64) || defined(OPENSSL_X86))
int aesni_set_encrypt_key(const uint8_t *userKey, int bits, AES_KEY *key);
int aesni_set_decrypt_key(const uint8_t *userKey, int bits, AES_KEY *key);

void aesni_encrypt(const uint8_t *in, uint8_t *out, const AES_KEY *key);
void aesni_decrypt(const uint8_t *in, uint8_t *out, const AES_KEY *key);

void aesni_ecb_encrypt(const uint8_t *in, uint8_t *out, size_t length,
                       const AES_KEY *key, int enc);
void aesni_cbc_encrypt(const uint8_t *in, uint8_t *out, size_t length,
                       const AES_KEY *key, uint8_t *ivec, int enc);

#else

/* On other platforms, aesni_capable() will always return false and so the
 * following will never be called. */
static void aesni_encrypt(const uint8_t *in, uint8_t *out, const AES_KEY *key) {
  abort();
}
static int aesni_set_encrypt_key(const uint8_t *userKey, int bits,
                                 AES_KEY *key) {
  abort();
}
static void aesni_ctr32_encrypt_blocks(const uint8_t *in, uint8_t *out,
                                       size_t blocks, const void *key,
                                       const uint8_t *ivec) {
  abort();
}

#endif

static int aes_init_key(EVP_CIPHER_CTX *ctx, const uint8_t *key,
                        const uint8_t *iv, int enc) {
  int ret, mode;
  EVP_AES_KEY *dat = (EVP_AES_KEY *)ctx->cipher_data;

  mode = ctx->cipher->flags & EVP_CIPH_MODE_MASK;
  if ((mode == EVP_CIPH_ECB_MODE || mode == EVP_CIPH_CBC_MODE) && !enc) {
    if (hwaes_capable()) {
      ret = aes_hw_set_decrypt_key(key, ctx->key_len * 8, &dat->ks.ks);
      dat->block = (block128_f)aes_hw_decrypt;
      dat->stream.cbc = NULL;
      if (mode == EVP_CIPH_CBC_MODE) {
        dat->stream.cbc = (cbc128_f)aes_hw_cbc_encrypt;
      }
    } else if (bsaes_capable() && mode == EVP_CIPH_CBC_MODE) {
      ret = AES_set_decrypt_key(key, ctx->key_len * 8, &dat->ks.ks);
      dat->block = (block128_f)AES_decrypt;
      dat->stream.cbc = (cbc128_f)bsaes_cbc_encrypt;
    } else if (vpaes_capable()) {
      ret = vpaes_set_decrypt_key(key, ctx->key_len * 8, &dat->ks.ks);
      dat->block = (block128_f)vpaes_decrypt;
      dat->stream.cbc =
          mode == EVP_CIPH_CBC_MODE ? (cbc128_f)vpaes_cbc_encrypt : NULL;
    } else {
      ret = AES_set_decrypt_key(key, ctx->key_len * 8, &dat->ks.ks);
      dat->block = (block128_f)AES_decrypt;
      dat->stream.cbc =
          mode == EVP_CIPH_CBC_MODE ? (cbc128_f)AES_cbc_encrypt : NULL;
    }
  } else if (hwaes_capable()) {
    ret = aes_hw_set_encrypt_key(key, ctx->key_len * 8, &dat->ks.ks);
    dat->block = (block128_f)aes_hw_encrypt;
    dat->stream.cbc = NULL;
    if (mode == EVP_CIPH_CBC_MODE) {
      dat->stream.cbc = (cbc128_f)aes_hw_cbc_encrypt;
    } else if (mode == EVP_CIPH_CTR_MODE) {
      dat->stream.ctr = (ctr128_f)aes_hw_ctr32_encrypt_blocks;
    }
  } else if (bsaes_capable() && mode == EVP_CIPH_CTR_MODE) {
    ret = AES_set_encrypt_key(key, ctx->key_len * 8, &dat->ks.ks);
    dat->block = (block128_f)AES_encrypt;
    dat->stream.ctr = (ctr128_f)bsaes_ctr32_encrypt_blocks;
  } else if (vpaes_capable()) {
    ret = vpaes_set_encrypt_key(key, ctx->key_len * 8, &dat->ks.ks);
    dat->block = (block128_f)vpaes_encrypt;
    dat->stream.cbc =
        mode == EVP_CIPH_CBC_MODE ? (cbc128_f)vpaes_cbc_encrypt : NULL;
  } else {
    ret = AES_set_encrypt_key(key, ctx->key_len * 8, &dat->ks.ks);
    dat->block = (block128_f)AES_encrypt;
    dat->stream.cbc =
        mode == EVP_CIPH_CBC_MODE ? (cbc128_f)AES_cbc_encrypt : NULL;
  }

  if (ret < 0) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_AES_KEY_SETUP_FAILED);
    return 0;
  }

  return 1;
}

static int aes_cbc_cipher(EVP_CIPHER_CTX *ctx, uint8_t *out, const uint8_t *in,
                          size_t len) {
  EVP_AES_KEY *dat = (EVP_AES_KEY *)ctx->cipher_data;

  if (dat->stream.cbc) {
    (*dat->stream.cbc)(in, out, len, &dat->ks, ctx->iv, ctx->encrypt);
  } else if (ctx->encrypt) {
    CRYPTO_cbc128_encrypt(in, out, len, &dat->ks, ctx->iv, dat->block);
  } else {
    CRYPTO_cbc128_decrypt(in, out, len, &dat->ks, ctx->iv, dat->block);
  }

  return 1;
}

static int aes_ecb_cipher(EVP_CIPHER_CTX *ctx, uint8_t *out, const uint8_t *in,
                          size_t len) {
  size_t bl = ctx->cipher->block_size;
  EVP_AES_KEY *dat = (EVP_AES_KEY *)ctx->cipher_data;

  if (len < bl) {
    return 1;
  }

  len -= bl;
  for (size_t i = 0; i <= len; i += bl) {
    (*dat->block)(in + i, out + i, &dat->ks);
  }

  return 1;
}

static int aes_ctr_cipher(EVP_CIPHER_CTX *ctx, uint8_t *out, const uint8_t *in,
                          size_t len) {
  EVP_AES_KEY *dat = (EVP_AES_KEY *)ctx->cipher_data;

  if (dat->stream.ctr) {
    CRYPTO_ctr128_encrypt_ctr32(in, out, len, &dat->ks, ctx->iv, ctx->buf,
                                &ctx->num, dat->stream.ctr);
  } else {
    CRYPTO_ctr128_encrypt(in, out, len, &dat->ks, ctx->iv, ctx->buf, &ctx->num,
                          dat->block);
  }
  return 1;
}

static int aes_ofb_cipher(EVP_CIPHER_CTX *ctx, uint8_t *out, const uint8_t *in,
                          size_t len) {
  EVP_AES_KEY *dat = (EVP_AES_KEY *)ctx->cipher_data;

  CRYPTO_ofb128_encrypt(in, out, len, &dat->ks, ctx->iv, &ctx->num, dat->block);
  return 1;
}

static char aesni_capable(void);

static ctr128_f aes_ctr_set_key(AES_KEY *aes_key, GCM128_CONTEXT *gcm_ctx,
                                block128_f *out_block, const uint8_t *key,
                                size_t key_len) {
  if (aesni_capable()) {
    aesni_set_encrypt_key(key, key_len * 8, aes_key);
    if (gcm_ctx != NULL) {
      CRYPTO_gcm128_init(gcm_ctx, aes_key, (block128_f)aesni_encrypt);
    }
    if (out_block) {
      *out_block = (block128_f) aesni_encrypt;
    }
    return (ctr128_f)aesni_ctr32_encrypt_blocks;
  }

  if (hwaes_capable()) {
    aes_hw_set_encrypt_key(key, key_len * 8, aes_key);
    if (gcm_ctx != NULL) {
      CRYPTO_gcm128_init(gcm_ctx, aes_key, (block128_f)aes_hw_encrypt);
    }
    if (out_block) {
      *out_block = (block128_f) aes_hw_encrypt;
    }
    return (ctr128_f)aes_hw_ctr32_encrypt_blocks;
  }

  if (bsaes_capable()) {
    AES_set_encrypt_key(key, key_len * 8, aes_key);
    if (gcm_ctx != NULL) {
      CRYPTO_gcm128_init(gcm_ctx, aes_key, (block128_f)AES_encrypt);
    }
    if (out_block) {
      *out_block = (block128_f) AES_encrypt;
    }
    return (ctr128_f)bsaes_ctr32_encrypt_blocks;
  }

  if (vpaes_capable()) {
    vpaes_set_encrypt_key(key, key_len * 8, aes_key);
    if (out_block) {
      *out_block = (block128_f) vpaes_encrypt;
    }
    if (gcm_ctx != NULL) {
      CRYPTO_gcm128_init(gcm_ctx, aes_key, (block128_f)vpaes_encrypt);
    }
    return NULL;
  }

  AES_set_encrypt_key(key, key_len * 8, aes_key);
  if (gcm_ctx != NULL) {
    CRYPTO_gcm128_init(gcm_ctx, aes_key, (block128_f)AES_encrypt);
  }
  if (out_block) {
    *out_block = (block128_f) AES_encrypt;
  }
  return NULL;
}

static int aes_gcm_init_key(EVP_CIPHER_CTX *ctx, const uint8_t *key,
                            const uint8_t *iv, int enc) {
  EVP_AES_GCM_CTX *gctx = ctx->cipher_data;
  if (!iv && !key) {
    return 1;
  }
  if (key) {
    gctx->ctr =
        aes_ctr_set_key(&gctx->ks.ks, &gctx->gcm, NULL, key, ctx->key_len);
    /* If we have an iv can set it directly, otherwise use saved IV. */
    if (iv == NULL && gctx->iv_set) {
      iv = gctx->iv;
    }
    if (iv) {
      CRYPTO_gcm128_setiv(&gctx->gcm, &gctx->ks.ks, iv, gctx->ivlen);
      gctx->iv_set = 1;
    }
    gctx->key_set = 1;
  } else {
    /* If key set use IV, otherwise copy */
    if (gctx->key_set) {
      CRYPTO_gcm128_setiv(&gctx->gcm, &gctx->ks.ks, iv, gctx->ivlen);
    } else {
      OPENSSL_memcpy(gctx->iv, iv, gctx->ivlen);
    }
    gctx->iv_set = 1;
    gctx->iv_gen = 0;
  }
  return 1;
}

static void aes_gcm_cleanup(EVP_CIPHER_CTX *c) {
  EVP_AES_GCM_CTX *gctx = c->cipher_data;
  OPENSSL_cleanse(&gctx->gcm, sizeof(gctx->gcm));
  if (gctx->iv != c->iv) {
    OPENSSL_free(gctx->iv);
  }
}

/* increment counter (64-bit int) by 1 */
static void ctr64_inc(uint8_t *counter) {
  int n = 8;
  uint8_t c;

  do {
    --n;
    c = counter[n];
    ++c;
    counter[n] = c;
    if (c) {
      return;
    }
  } while (n);
}

static int aes_gcm_ctrl(EVP_CIPHER_CTX *c, int type, int arg, void *ptr) {
  EVP_AES_GCM_CTX *gctx = c->cipher_data;
  switch (type) {
    case EVP_CTRL_INIT:
      gctx->key_set = 0;
      gctx->iv_set = 0;
      gctx->ivlen = c->cipher->iv_len;
      gctx->iv = c->iv;
      gctx->taglen = -1;
      gctx->iv_gen = 0;
      return 1;

    case EVP_CTRL_GCM_SET_IVLEN:
      if (arg <= 0) {
        return 0;
      }

      /* Allocate memory for IV if needed */
      if (arg > EVP_MAX_IV_LENGTH && arg > gctx->ivlen) {
        if (gctx->iv != c->iv) {
          OPENSSL_free(gctx->iv);
        }
        gctx->iv = OPENSSL_malloc(arg);
        if (!gctx->iv) {
          return 0;
        }
      }
      gctx->ivlen = arg;
      return 1;

    case EVP_CTRL_GCM_SET_TAG:
      if (arg <= 0 || arg > 16 || c->encrypt) {
        return 0;
      }
      OPENSSL_memcpy(c->buf, ptr, arg);
      gctx->taglen = arg;
      return 1;

    case EVP_CTRL_GCM_GET_TAG:
      if (arg <= 0 || arg > 16 || !c->encrypt || gctx->taglen < 0) {
        return 0;
      }
      OPENSSL_memcpy(ptr, c->buf, arg);
      return 1;

    case EVP_CTRL_GCM_SET_IV_FIXED:
      /* Special case: -1 length restores whole IV */
      if (arg == -1) {
        OPENSSL_memcpy(gctx->iv, ptr, gctx->ivlen);
        gctx->iv_gen = 1;
        return 1;
      }
      /* Fixed field must be at least 4 bytes and invocation field
       * at least 8. */
      if (arg < 4 || (gctx->ivlen - arg) < 8) {
        return 0;
      }
      if (arg) {
        OPENSSL_memcpy(gctx->iv, ptr, arg);
      }
      if (c->encrypt && !RAND_bytes(gctx->iv + arg, gctx->ivlen - arg)) {
        return 0;
      }
      gctx->iv_gen = 1;
      return 1;

    case EVP_CTRL_GCM_IV_GEN:
      if (gctx->iv_gen == 0 || gctx->key_set == 0) {
        return 0;
      }
      CRYPTO_gcm128_setiv(&gctx->gcm, &gctx->ks.ks, gctx->iv, gctx->ivlen);
      if (arg <= 0 || arg > gctx->ivlen) {
        arg = gctx->ivlen;
      }
      OPENSSL_memcpy(ptr, gctx->iv + gctx->ivlen - arg, arg);
      /* Invocation field will be at least 8 bytes in size and
       * so no need to check wrap around or increment more than
       * last 8 bytes. */
      ctr64_inc(gctx->iv + gctx->ivlen - 8);
      gctx->iv_set = 1;
      return 1;

    case EVP_CTRL_GCM_SET_IV_INV:
      if (gctx->iv_gen == 0 || gctx->key_set == 0 || c->encrypt) {
        return 0;
      }
      OPENSSL_memcpy(gctx->iv + gctx->ivlen - arg, ptr, arg);
      CRYPTO_gcm128_setiv(&gctx->gcm, &gctx->ks.ks, gctx->iv, gctx->ivlen);
      gctx->iv_set = 1;
      return 1;

    case EVP_CTRL_COPY: {
      EVP_CIPHER_CTX *out = ptr;
      EVP_AES_GCM_CTX *gctx_out = out->cipher_data;
      if (gctx->iv == c->iv) {
        gctx_out->iv = out->iv;
      } else {
        gctx_out->iv = OPENSSL_malloc(gctx->ivlen);
        if (!gctx_out->iv) {
          return 0;
        }
        OPENSSL_memcpy(gctx_out->iv, gctx->iv, gctx->ivlen);
      }
      return 1;
    }

    default:
      return -1;
  }
}

static int aes_gcm_cipher(EVP_CIPHER_CTX *ctx, uint8_t *out, const uint8_t *in,
                          size_t len) {
  EVP_AES_GCM_CTX *gctx = ctx->cipher_data;

  /* If not set up, return error */
  if (!gctx->key_set) {
    return -1;
  }
  if (!gctx->iv_set) {
    return -1;
  }

  if (in) {
    if (out == NULL) {
      if (!CRYPTO_gcm128_aad(&gctx->gcm, in, len)) {
        return -1;
      }
    } else if (ctx->encrypt) {
      if (gctx->ctr) {
        if (!CRYPTO_gcm128_encrypt_ctr32(&gctx->gcm, &gctx->ks.ks, in, out, len,
                                         gctx->ctr)) {
          return -1;
        }
      } else {
        if (!CRYPTO_gcm128_encrypt(&gctx->gcm, &gctx->ks.ks, in, out, len)) {
          return -1;
        }
      }
    } else {
      if (gctx->ctr) {
        if (!CRYPTO_gcm128_decrypt_ctr32(&gctx->gcm, &gctx->ks.ks, in, out, len,
                                         gctx->ctr)) {
          return -1;
        }
      } else {
        if (!CRYPTO_gcm128_decrypt(&gctx->gcm, &gctx->ks.ks, in, out, len)) {
          return -1;
        }
      }
    }
    return len;
  } else {
    if (!ctx->encrypt) {
      if (gctx->taglen < 0 ||
          !CRYPTO_gcm128_finish(&gctx->gcm, ctx->buf, gctx->taglen)) {
        return -1;
      }
      gctx->iv_set = 0;
      return 0;
    }
    CRYPTO_gcm128_tag(&gctx->gcm, ctx->buf, 16);
    gctx->taglen = 16;
    /* Don't reuse the IV */
    gctx->iv_set = 0;
    return 0;
  }
}

static const EVP_CIPHER aes_128_cbc = {
    NID_aes_128_cbc,     16 /* block_size */, 16 /* key_size */,
    16 /* iv_len */,     sizeof(EVP_AES_KEY), EVP_CIPH_CBC_MODE,
    NULL /* app_data */, aes_init_key,        aes_cbc_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aes_128_ctr = {
    NID_aes_128_ctr,     1 /* block_size */,  16 /* key_size */,
    16 /* iv_len */,     sizeof(EVP_AES_KEY), EVP_CIPH_CTR_MODE,
    NULL /* app_data */, aes_init_key,        aes_ctr_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aes_128_ecb = {
    NID_aes_128_ecb,     16 /* block_size */, 16 /* key_size */,
    0 /* iv_len */,      sizeof(EVP_AES_KEY), EVP_CIPH_ECB_MODE,
    NULL /* app_data */, aes_init_key,        aes_ecb_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aes_128_ofb = {
    NID_aes_128_ofb128,  1 /* block_size */,  16 /* key_size */,
    16 /* iv_len */,     sizeof(EVP_AES_KEY), EVP_CIPH_OFB_MODE,
    NULL /* app_data */, aes_init_key,        aes_ofb_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aes_128_gcm = {
    NID_aes_128_gcm, 1 /* block_size */, 16 /* key_size */, 12 /* iv_len */,
    sizeof(EVP_AES_GCM_CTX),
    EVP_CIPH_GCM_MODE | EVP_CIPH_CUSTOM_IV | EVP_CIPH_FLAG_CUSTOM_CIPHER |
        EVP_CIPH_ALWAYS_CALL_INIT | EVP_CIPH_CTRL_INIT |
        EVP_CIPH_FLAG_AEAD_CIPHER,
    NULL /* app_data */, aes_gcm_init_key, aes_gcm_cipher, aes_gcm_cleanup,
    aes_gcm_ctrl};


static const EVP_CIPHER aes_192_cbc = {
    NID_aes_192_cbc,     16 /* block_size */, 24 /* key_size */,
    16 /* iv_len */,     sizeof(EVP_AES_KEY), EVP_CIPH_CBC_MODE,
    NULL /* app_data */, aes_init_key,        aes_cbc_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aes_192_ctr = {
    NID_aes_192_ctr,     1 /* block_size */,  24 /* key_size */,
    16 /* iv_len */,     sizeof(EVP_AES_KEY), EVP_CIPH_CTR_MODE,
    NULL /* app_data */, aes_init_key,        aes_ctr_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aes_192_ecb = {
    NID_aes_192_ecb,     16 /* block_size */, 24 /* key_size */,
    0 /* iv_len */,      sizeof(EVP_AES_KEY), EVP_CIPH_ECB_MODE,
    NULL /* app_data */, aes_init_key,        aes_ecb_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aes_192_gcm = {
    NID_aes_192_gcm, 1 /* block_size */, 24 /* key_size */, 12 /* iv_len */,
    sizeof(EVP_AES_GCM_CTX),
    EVP_CIPH_GCM_MODE | EVP_CIPH_CUSTOM_IV | EVP_CIPH_FLAG_CUSTOM_CIPHER |
        EVP_CIPH_ALWAYS_CALL_INIT | EVP_CIPH_CTRL_INIT |
        EVP_CIPH_FLAG_AEAD_CIPHER,
    NULL /* app_data */, aes_gcm_init_key, aes_gcm_cipher, aes_gcm_cleanup,
    aes_gcm_ctrl};


static const EVP_CIPHER aes_256_cbc = {
    NID_aes_256_cbc,     16 /* block_size */, 32 /* key_size */,
    16 /* iv_len */,     sizeof(EVP_AES_KEY), EVP_CIPH_CBC_MODE,
    NULL /* app_data */, aes_init_key,        aes_cbc_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aes_256_ctr = {
    NID_aes_256_ctr,     1 /* block_size */,  32 /* key_size */,
    16 /* iv_len */,     sizeof(EVP_AES_KEY), EVP_CIPH_CTR_MODE,
    NULL /* app_data */, aes_init_key,        aes_ctr_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aes_256_ecb = {
    NID_aes_256_ecb,     16 /* block_size */, 32 /* key_size */,
    0 /* iv_len */,      sizeof(EVP_AES_KEY), EVP_CIPH_ECB_MODE,
    NULL /* app_data */, aes_init_key,        aes_ecb_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aes_256_ofb = {
    NID_aes_256_ofb128,  1 /* block_size */,  32 /* key_size */,
    16 /* iv_len */,     sizeof(EVP_AES_KEY), EVP_CIPH_OFB_MODE,
    NULL /* app_data */, aes_init_key,        aes_ofb_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aes_256_gcm = {
    NID_aes_256_gcm, 1 /* block_size */, 32 /* key_size */, 12 /* iv_len */,
    sizeof(EVP_AES_GCM_CTX),
    EVP_CIPH_GCM_MODE | EVP_CIPH_CUSTOM_IV | EVP_CIPH_FLAG_CUSTOM_CIPHER |
        EVP_CIPH_ALWAYS_CALL_INIT | EVP_CIPH_CTRL_INIT |
        EVP_CIPH_FLAG_AEAD_CIPHER,
    NULL /* app_data */, aes_gcm_init_key, aes_gcm_cipher, aes_gcm_cleanup,
    aes_gcm_ctrl};

#if !defined(OPENSSL_NO_ASM) && \
    (defined(OPENSSL_X86_64) || defined(OPENSSL_X86))

/* AES-NI section. */

static char aesni_capable(void) {
  return (OPENSSL_ia32cap_P[1] & (1 << (57 - 32))) != 0;
}

static int aesni_init_key(EVP_CIPHER_CTX *ctx, const uint8_t *key,
                          const uint8_t *iv, int enc) {
  int ret, mode;
  EVP_AES_KEY *dat = (EVP_AES_KEY *)ctx->cipher_data;

  mode = ctx->cipher->flags & EVP_CIPH_MODE_MASK;
  if ((mode == EVP_CIPH_ECB_MODE || mode == EVP_CIPH_CBC_MODE) && !enc) {
    ret = aesni_set_decrypt_key(key, ctx->key_len * 8, ctx->cipher_data);
    dat->block = (block128_f)aesni_decrypt;
    dat->stream.cbc =
        mode == EVP_CIPH_CBC_MODE ? (cbc128_f)aesni_cbc_encrypt : NULL;
  } else {
    ret = aesni_set_encrypt_key(key, ctx->key_len * 8, ctx->cipher_data);
    dat->block = (block128_f)aesni_encrypt;
    if (mode == EVP_CIPH_CBC_MODE) {
      dat->stream.cbc = (cbc128_f)aesni_cbc_encrypt;
    } else if (mode == EVP_CIPH_CTR_MODE) {
      dat->stream.ctr = (ctr128_f)aesni_ctr32_encrypt_blocks;
    } else {
      dat->stream.cbc = NULL;
    }
  }

  if (ret < 0) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_AES_KEY_SETUP_FAILED);
    return 0;
  }

  return 1;
}

static int aesni_cbc_cipher(EVP_CIPHER_CTX *ctx, uint8_t *out,
                            const uint8_t *in, size_t len) {
  aesni_cbc_encrypt(in, out, len, ctx->cipher_data, ctx->iv, ctx->encrypt);

  return 1;
}

static int aesni_ecb_cipher(EVP_CIPHER_CTX *ctx, uint8_t *out,
                            const uint8_t *in, size_t len) {
  size_t bl = ctx->cipher->block_size;

  if (len < bl) {
    return 1;
  }

  aesni_ecb_encrypt(in, out, len, ctx->cipher_data, ctx->encrypt);

  return 1;
}

static int aesni_gcm_init_key(EVP_CIPHER_CTX *ctx, const uint8_t *key,
                              const uint8_t *iv, int enc) {
  EVP_AES_GCM_CTX *gctx = ctx->cipher_data;
  if (!iv && !key) {
    return 1;
  }
  if (key) {
    aesni_set_encrypt_key(key, ctx->key_len * 8, &gctx->ks.ks);
    CRYPTO_gcm128_init(&gctx->gcm, &gctx->ks, (block128_f)aesni_encrypt);
    gctx->ctr = (ctr128_f)aesni_ctr32_encrypt_blocks;
    /* If we have an iv can set it directly, otherwise use
     * saved IV. */
    if (iv == NULL && gctx->iv_set) {
      iv = gctx->iv;
    }
    if (iv) {
      CRYPTO_gcm128_setiv(&gctx->gcm, &gctx->ks.ks, iv, gctx->ivlen);
      gctx->iv_set = 1;
    }
    gctx->key_set = 1;
  } else {
    /* If key set use IV, otherwise copy */
    if (gctx->key_set) {
      CRYPTO_gcm128_setiv(&gctx->gcm, &gctx->ks.ks, iv, gctx->ivlen);
    } else {
      OPENSSL_memcpy(gctx->iv, iv, gctx->ivlen);
    }
    gctx->iv_set = 1;
    gctx->iv_gen = 0;
  }
  return 1;
}

static const EVP_CIPHER aesni_128_cbc = {
    NID_aes_128_cbc,     16 /* block_size */, 16 /* key_size */,
    16 /* iv_len */,     sizeof(EVP_AES_KEY), EVP_CIPH_CBC_MODE,
    NULL /* app_data */, aesni_init_key,      aesni_cbc_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aesni_128_ctr = {
    NID_aes_128_ctr,     1 /* block_size */,  16 /* key_size */,
    16 /* iv_len */,     sizeof(EVP_AES_KEY), EVP_CIPH_CTR_MODE,
    NULL /* app_data */, aesni_init_key,      aes_ctr_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aesni_128_ecb = {
    NID_aes_128_ecb,     16 /* block_size */, 16 /* key_size */,
    0 /* iv_len */,      sizeof(EVP_AES_KEY), EVP_CIPH_ECB_MODE,
    NULL /* app_data */, aesni_init_key,      aesni_ecb_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aesni_128_ofb = {
    NID_aes_128_ofb128,  1 /* block_size */,  16 /* key_size */,
    16 /* iv_len */,     sizeof(EVP_AES_KEY), EVP_CIPH_OFB_MODE,
    NULL /* app_data */, aesni_init_key,      aes_ofb_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aesni_128_gcm = {
    NID_aes_128_gcm, 1 /* block_size */, 16 /* key_size */, 12 /* iv_len */,
    sizeof(EVP_AES_GCM_CTX),
    EVP_CIPH_GCM_MODE | EVP_CIPH_CUSTOM_IV | EVP_CIPH_FLAG_CUSTOM_CIPHER |
        EVP_CIPH_ALWAYS_CALL_INIT | EVP_CIPH_CTRL_INIT |
        EVP_CIPH_FLAG_AEAD_CIPHER,
    NULL /* app_data */, aesni_gcm_init_key, aes_gcm_cipher, aes_gcm_cleanup,
    aes_gcm_ctrl};


static const EVP_CIPHER aesni_192_cbc = {
    NID_aes_192_cbc,     16 /* block_size */, 24 /* key_size */,
    16 /* iv_len */,     sizeof(EVP_AES_KEY), EVP_CIPH_CBC_MODE,
    NULL /* app_data */, aesni_init_key,      aesni_cbc_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aesni_192_ctr = {
    NID_aes_192_ctr,     1 /* block_size */,  24 /* key_size */,
    16 /* iv_len */,     sizeof(EVP_AES_KEY), EVP_CIPH_CTR_MODE,
    NULL /* app_data */, aesni_init_key,      aes_ctr_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aesni_192_ecb = {
    NID_aes_192_ecb,     16 /* block_size */, 24 /* key_size */,
    0 /* iv_len */,      sizeof(EVP_AES_KEY), EVP_CIPH_ECB_MODE,
    NULL /* app_data */, aesni_init_key,      aesni_ecb_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aesni_192_gcm = {
    NID_aes_192_gcm, 1 /* block_size */, 24 /* key_size */, 12 /* iv_len */,
    sizeof(EVP_AES_GCM_CTX),
    EVP_CIPH_GCM_MODE | EVP_CIPH_CUSTOM_IV | EVP_CIPH_FLAG_CUSTOM_CIPHER |
        EVP_CIPH_ALWAYS_CALL_INIT | EVP_CIPH_CTRL_INIT |
        EVP_CIPH_FLAG_AEAD_CIPHER,
    NULL /* app_data */, aesni_gcm_init_key, aes_gcm_cipher, aes_gcm_cleanup,
    aes_gcm_ctrl};


static const EVP_CIPHER aesni_256_cbc = {
    NID_aes_256_cbc,     16 /* block_size */, 32 /* key_size */,
    16 /* iv_len */,     sizeof(EVP_AES_KEY), EVP_CIPH_CBC_MODE,
    NULL /* app_data */, aesni_init_key,      aesni_cbc_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aesni_256_ctr = {
    NID_aes_256_ctr,     1 /* block_size */,  32 /* key_size */,
    16 /* iv_len */,     sizeof(EVP_AES_KEY), EVP_CIPH_CTR_MODE,
    NULL /* app_data */, aesni_init_key,      aes_ctr_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aesni_256_ecb = {
    NID_aes_256_ecb,     16 /* block_size */, 32 /* key_size */,
    0 /* iv_len */,      sizeof(EVP_AES_KEY), EVP_CIPH_ECB_MODE,
    NULL /* app_data */, aesni_init_key,      aesni_ecb_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aesni_256_ofb = {
    NID_aes_256_ofb128,  1 /* block_size */,  32 /* key_size */,
    16 /* iv_len */,     sizeof(EVP_AES_KEY), EVP_CIPH_OFB_MODE,
    NULL /* app_data */, aesni_init_key,      aes_ofb_cipher,
    NULL /* cleanup */,  NULL /* ctrl */};

static const EVP_CIPHER aesni_256_gcm = {
    NID_aes_256_gcm, 1 /* block_size */, 32 /* key_size */, 12 /* iv_len */,
    sizeof(EVP_AES_GCM_CTX),
    EVP_CIPH_GCM_MODE | EVP_CIPH_CUSTOM_IV | EVP_CIPH_FLAG_CUSTOM_CIPHER |
        EVP_CIPH_ALWAYS_CALL_INIT | EVP_CIPH_CTRL_INIT | EVP_CIPH_CUSTOM_COPY |
        EVP_CIPH_FLAG_AEAD_CIPHER,
    NULL /* app_data */, aesni_gcm_init_key, aes_gcm_cipher, aes_gcm_cleanup,
    aes_gcm_ctrl};

#define EVP_CIPHER_FUNCTION(keybits, mode)             \
  const EVP_CIPHER *EVP_aes_##keybits##_##mode(void) { \
    if (aesni_capable()) {                             \
      return &aesni_##keybits##_##mode;                \
    } else {                                           \
      return &aes_##keybits##_##mode;                  \
    }                                                  \
  }

#else  /* ^^^  OPENSSL_X86_64 || OPENSSL_X86 */

static char aesni_capable(void) {
  return 0;
}

#define EVP_CIPHER_FUNCTION(keybits, mode)             \
  const EVP_CIPHER *EVP_aes_##keybits##_##mode(void) { \
    return &aes_##keybits##_##mode;                    \
  }

#endif

EVP_CIPHER_FUNCTION(128, cbc)
EVP_CIPHER_FUNCTION(128, ctr)
EVP_CIPHER_FUNCTION(128, ecb)
EVP_CIPHER_FUNCTION(128, ofb)
EVP_CIPHER_FUNCTION(128, gcm)

EVP_CIPHER_FUNCTION(192, cbc)
EVP_CIPHER_FUNCTION(192, ctr)
EVP_CIPHER_FUNCTION(192, ecb)
EVP_CIPHER_FUNCTION(192, gcm)

EVP_CIPHER_FUNCTION(256, cbc)
EVP_CIPHER_FUNCTION(256, ctr)
EVP_CIPHER_FUNCTION(256, ecb)
EVP_CIPHER_FUNCTION(256, ofb)
EVP_CIPHER_FUNCTION(256, gcm)


#define EVP_AEAD_AES_GCM_TAG_LEN 16

struct aead_aes_gcm_ctx {
  union {
    double align;
    AES_KEY ks;
  } ks;
  GCM128_CONTEXT gcm;
  ctr128_f ctr;
  uint8_t tag_len;
};

static int aead_aes_gcm_init(EVP_AEAD_CTX *ctx, const uint8_t *key,
                             size_t key_len, size_t tag_len) {
  struct aead_aes_gcm_ctx *gcm_ctx;
  const size_t key_bits = key_len * 8;

  if (key_bits != 128 && key_bits != 256) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_BAD_KEY_LENGTH);
    return 0; /* EVP_AEAD_CTX_init should catch this. */
  }

  if (tag_len == EVP_AEAD_DEFAULT_TAG_LENGTH) {
    tag_len = EVP_AEAD_AES_GCM_TAG_LEN;
  }

  if (tag_len > EVP_AEAD_AES_GCM_TAG_LEN) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_TAG_TOO_LARGE);
    return 0;
  }

  gcm_ctx = OPENSSL_malloc(sizeof(struct aead_aes_gcm_ctx));
  if (gcm_ctx == NULL) {
    return 0;
  }

  gcm_ctx->ctr =
      aes_ctr_set_key(&gcm_ctx->ks.ks, &gcm_ctx->gcm, NULL, key, key_len);
  gcm_ctx->tag_len = tag_len;
  ctx->aead_state = gcm_ctx;

  return 1;
}

static void aead_aes_gcm_cleanup(EVP_AEAD_CTX *ctx) {
  struct aead_aes_gcm_ctx *gcm_ctx = ctx->aead_state;
  OPENSSL_cleanse(gcm_ctx, sizeof(struct aead_aes_gcm_ctx));
  OPENSSL_free(gcm_ctx);
}

static int aead_aes_gcm_seal(const EVP_AEAD_CTX *ctx, uint8_t *out,
                             size_t *out_len, size_t max_out_len,
                             const uint8_t *nonce, size_t nonce_len,
                             const uint8_t *in, size_t in_len,
                             const uint8_t *ad, size_t ad_len) {
  const struct aead_aes_gcm_ctx *gcm_ctx = ctx->aead_state;
  GCM128_CONTEXT gcm;

  if (in_len + gcm_ctx->tag_len < in_len) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_TOO_LARGE);
    return 0;
  }

  if (max_out_len < in_len + gcm_ctx->tag_len) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_BUFFER_TOO_SMALL);
    return 0;
  }

  const AES_KEY *key = &gcm_ctx->ks.ks;

  OPENSSL_memcpy(&gcm, &gcm_ctx->gcm, sizeof(gcm));
  CRYPTO_gcm128_setiv(&gcm, key, nonce, nonce_len);

  if (ad_len > 0 && !CRYPTO_gcm128_aad(&gcm, ad, ad_len)) {
    return 0;
  }

  if (gcm_ctx->ctr) {
    if (!CRYPTO_gcm128_encrypt_ctr32(&gcm, key, in, out, in_len,
                                     gcm_ctx->ctr)) {
      return 0;
    }
  } else {
    if (!CRYPTO_gcm128_encrypt(&gcm, key, in, out, in_len)) {
      return 0;
    }
  }

  CRYPTO_gcm128_tag(&gcm, out + in_len, gcm_ctx->tag_len);
  *out_len = in_len + gcm_ctx->tag_len;
  return 1;
}

static int aead_aes_gcm_open(const EVP_AEAD_CTX *ctx, uint8_t *out,
                             size_t *out_len, size_t max_out_len,
                             const uint8_t *nonce, size_t nonce_len,
                             const uint8_t *in, size_t in_len,
                             const uint8_t *ad, size_t ad_len) {
  const struct aead_aes_gcm_ctx *gcm_ctx = ctx->aead_state;
  uint8_t tag[EVP_AEAD_AES_GCM_TAG_LEN];
  size_t plaintext_len;
  GCM128_CONTEXT gcm;

  if (in_len < gcm_ctx->tag_len) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_BAD_DECRYPT);
    return 0;
  }

  plaintext_len = in_len - gcm_ctx->tag_len;

  if (max_out_len < plaintext_len) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_BUFFER_TOO_SMALL);
    return 0;
  }

  const AES_KEY *key = &gcm_ctx->ks.ks;

  OPENSSL_memcpy(&gcm, &gcm_ctx->gcm, sizeof(gcm));
  CRYPTO_gcm128_setiv(&gcm, key, nonce, nonce_len);

  if (!CRYPTO_gcm128_aad(&gcm, ad, ad_len)) {
    return 0;
  }

  if (gcm_ctx->ctr) {
    if (!CRYPTO_gcm128_decrypt_ctr32(&gcm, key, in, out,
                                     in_len - gcm_ctx->tag_len, gcm_ctx->ctr)) {
      return 0;
    }
  } else {
    if (!CRYPTO_gcm128_decrypt(&gcm, key, in, out, in_len - gcm_ctx->tag_len)) {
      return 0;
    }
  }

  CRYPTO_gcm128_tag(&gcm, tag, gcm_ctx->tag_len);
  if (CRYPTO_memcmp(tag, in + plaintext_len, gcm_ctx->tag_len) != 0) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_BAD_DECRYPT);
    return 0;
  }

  *out_len = plaintext_len;
  return 1;
}

static const EVP_AEAD aead_aes_128_gcm = {
    16,                       /* key len */
    12,                       /* nonce len */
    EVP_AEAD_AES_GCM_TAG_LEN, /* overhead */
    EVP_AEAD_AES_GCM_TAG_LEN, /* max tag length */
    aead_aes_gcm_init,
    NULL, /* init_with_direction */
    aead_aes_gcm_cleanup,
    aead_aes_gcm_seal,
    aead_aes_gcm_open,
    NULL,                     /* get_iv */
};

static const EVP_AEAD aead_aes_256_gcm = {
    32,                       /* key len */
    12,                       /* nonce len */
    EVP_AEAD_AES_GCM_TAG_LEN, /* overhead */
    EVP_AEAD_AES_GCM_TAG_LEN, /* max tag length */
    aead_aes_gcm_init,
    NULL, /* init_with_direction */
    aead_aes_gcm_cleanup,
    aead_aes_gcm_seal,
    aead_aes_gcm_open,
    NULL,                     /* get_iv */
};

const EVP_AEAD *EVP_aead_aes_128_gcm(void) { return &aead_aes_128_gcm; }

const EVP_AEAD *EVP_aead_aes_256_gcm(void) { return &aead_aes_256_gcm; }


#define EVP_AEAD_AES_CTR_HMAC_SHA256_TAG_LEN SHA256_DIGEST_LENGTH
#define EVP_AEAD_AES_CTR_HMAC_SHA256_NONCE_LEN 12

struct aead_aes_ctr_hmac_sha256_ctx {
  union {
    double align;
    AES_KEY ks;
  } ks;
  ctr128_f ctr;
  block128_f block;
  SHA256_CTX inner_init_state;
  SHA256_CTX outer_init_state;
  uint8_t tag_len;
};

static void hmac_init(SHA256_CTX *out_inner, SHA256_CTX *out_outer,
                      const uint8_t hmac_key[32]) {
  static const size_t hmac_key_len = 32;
  uint8_t block[SHA256_CBLOCK];
  OPENSSL_memcpy(block, hmac_key, hmac_key_len);
  OPENSSL_memset(block + hmac_key_len, 0x36, sizeof(block) - hmac_key_len);

  unsigned i;
  for (i = 0; i < hmac_key_len; i++) {
    block[i] ^= 0x36;
  }

  SHA256_Init(out_inner);
  SHA256_Update(out_inner, block, sizeof(block));

  OPENSSL_memset(block + hmac_key_len, 0x5c, sizeof(block) - hmac_key_len);
  for (i = 0; i < hmac_key_len; i++) {
    block[i] ^= (0x36 ^ 0x5c);
  }

  SHA256_Init(out_outer);
  SHA256_Update(out_outer, block, sizeof(block));
}

static int aead_aes_ctr_hmac_sha256_init(EVP_AEAD_CTX *ctx, const uint8_t *key,
                                         size_t key_len, size_t tag_len) {
  struct aead_aes_ctr_hmac_sha256_ctx *aes_ctx;
  static const size_t hmac_key_len = 32;

  if (key_len < hmac_key_len) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_BAD_KEY_LENGTH);
    return 0; /* EVP_AEAD_CTX_init should catch this. */
  }

  const size_t aes_key_len = key_len - hmac_key_len;
  if (aes_key_len != 16 && aes_key_len != 32) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_BAD_KEY_LENGTH);
    return 0; /* EVP_AEAD_CTX_init should catch this. */
  }

  if (tag_len == EVP_AEAD_DEFAULT_TAG_LENGTH) {
    tag_len = EVP_AEAD_AES_CTR_HMAC_SHA256_TAG_LEN;
  }

  if (tag_len > EVP_AEAD_AES_CTR_HMAC_SHA256_TAG_LEN) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_TAG_TOO_LARGE);
    return 0;
  }

  aes_ctx = OPENSSL_malloc(sizeof(struct aead_aes_ctr_hmac_sha256_ctx));
  if (aes_ctx == NULL) {
    OPENSSL_PUT_ERROR(CIPHER, ERR_R_MALLOC_FAILURE);
    return 0;
  }

  aes_ctx->ctr =
      aes_ctr_set_key(&aes_ctx->ks.ks, NULL, &aes_ctx->block, key, aes_key_len);
  aes_ctx->tag_len = tag_len;
  hmac_init(&aes_ctx->inner_init_state, &aes_ctx->outer_init_state,
            key + aes_key_len);

  ctx->aead_state = aes_ctx;

  return 1;
}

static void aead_aes_ctr_hmac_sha256_cleanup(EVP_AEAD_CTX *ctx) {
  struct aead_aes_ctr_hmac_sha256_ctx *aes_ctx = ctx->aead_state;
  OPENSSL_cleanse(aes_ctx, sizeof(struct aead_aes_ctr_hmac_sha256_ctx));
  OPENSSL_free(aes_ctx);
}

static void hmac_update_uint64(SHA256_CTX *sha256, uint64_t value) {
  unsigned i;
  uint8_t bytes[8];

  for (i = 0; i < sizeof(bytes); i++) {
    bytes[i] = value & 0xff;
    value >>= 8;
  }
  SHA256_Update(sha256, bytes, sizeof(bytes));
}

static void hmac_calculate(uint8_t out[SHA256_DIGEST_LENGTH],
                           const SHA256_CTX *inner_init_state,
                           const SHA256_CTX *outer_init_state,
                           const uint8_t *ad, size_t ad_len,
                           const uint8_t *nonce, const uint8_t *ciphertext,
                           size_t ciphertext_len) {
  SHA256_CTX sha256;
  OPENSSL_memcpy(&sha256, inner_init_state, sizeof(sha256));
  hmac_update_uint64(&sha256, ad_len);
  hmac_update_uint64(&sha256, ciphertext_len);
  SHA256_Update(&sha256, nonce, EVP_AEAD_AES_CTR_HMAC_SHA256_NONCE_LEN);
  SHA256_Update(&sha256, ad, ad_len);

  /* Pad with zeros to the end of the SHA-256 block. */
  const unsigned num_padding =
      (SHA256_CBLOCK - ((sizeof(uint64_t)*2 +
                         EVP_AEAD_AES_CTR_HMAC_SHA256_NONCE_LEN + ad_len) %
                        SHA256_CBLOCK)) %
      SHA256_CBLOCK;
  uint8_t padding[SHA256_CBLOCK];
  OPENSSL_memset(padding, 0, num_padding);
  SHA256_Update(&sha256, padding, num_padding);

  SHA256_Update(&sha256, ciphertext, ciphertext_len);

  uint8_t inner_digest[SHA256_DIGEST_LENGTH];
  SHA256_Final(inner_digest, &sha256);

  OPENSSL_memcpy(&sha256, outer_init_state, sizeof(sha256));
  SHA256_Update(&sha256, inner_digest, sizeof(inner_digest));
  SHA256_Final(out, &sha256);
}

static void aead_aes_ctr_hmac_sha256_crypt(
    const struct aead_aes_ctr_hmac_sha256_ctx *aes_ctx, uint8_t *out,
    const uint8_t *in, size_t len, const uint8_t *nonce) {
  /* Since the AEAD operation is one-shot, keeping a buffer of unused keystream
   * bytes is pointless. However, |CRYPTO_ctr128_encrypt| requires it. */
  uint8_t partial_block_buffer[AES_BLOCK_SIZE];
  unsigned partial_block_offset = 0;
  OPENSSL_memset(partial_block_buffer, 0, sizeof(partial_block_buffer));

  uint8_t counter[AES_BLOCK_SIZE];
  OPENSSL_memcpy(counter, nonce, EVP_AEAD_AES_CTR_HMAC_SHA256_NONCE_LEN);
  OPENSSL_memset(counter + EVP_AEAD_AES_CTR_HMAC_SHA256_NONCE_LEN, 0, 4);

  if (aes_ctx->ctr) {
    CRYPTO_ctr128_encrypt_ctr32(in, out, len, &aes_ctx->ks.ks, counter,
                                partial_block_buffer, &partial_block_offset,
                                aes_ctx->ctr);
  } else {
    CRYPTO_ctr128_encrypt(in, out, len, &aes_ctx->ks.ks, counter,
                          partial_block_buffer, &partial_block_offset,
                          aes_ctx->block);
  }
}

static int aead_aes_ctr_hmac_sha256_seal(const EVP_AEAD_CTX *ctx, uint8_t *out,
                                         size_t *out_len, size_t max_out_len,
                                         const uint8_t *nonce, size_t nonce_len,
                                         const uint8_t *in, size_t in_len,
                                         const uint8_t *ad, size_t ad_len) {
  const struct aead_aes_ctr_hmac_sha256_ctx *aes_ctx = ctx->aead_state;
  const uint64_t in_len_64 = in_len;

  if (in_len + aes_ctx->tag_len < in_len ||
      /* This input is so large it would overflow the 32-bit block counter. */
      in_len_64 >= (UINT64_C(1) << 32) * AES_BLOCK_SIZE) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_TOO_LARGE);
    return 0;
  }

  if (max_out_len < in_len + aes_ctx->tag_len) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_BUFFER_TOO_SMALL);
    return 0;
  }

  if (nonce_len != EVP_AEAD_AES_CTR_HMAC_SHA256_NONCE_LEN) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_UNSUPPORTED_NONCE_SIZE);
    return 0;
  }

  aead_aes_ctr_hmac_sha256_crypt(aes_ctx, out, in, in_len, nonce);

  uint8_t hmac_result[SHA256_DIGEST_LENGTH];
  hmac_calculate(hmac_result, &aes_ctx->inner_init_state,
                 &aes_ctx->outer_init_state, ad, ad_len, nonce, out, in_len);
  OPENSSL_memcpy(out + in_len, hmac_result, aes_ctx->tag_len);
  *out_len = in_len + aes_ctx->tag_len;

  return 1;
}

static int aead_aes_ctr_hmac_sha256_open(const EVP_AEAD_CTX *ctx, uint8_t *out,
                                         size_t *out_len, size_t max_out_len,
                                         const uint8_t *nonce, size_t nonce_len,
                                         const uint8_t *in, size_t in_len,
                                         const uint8_t *ad, size_t ad_len) {
  const struct aead_aes_ctr_hmac_sha256_ctx *aes_ctx = ctx->aead_state;
  size_t plaintext_len;

  if (in_len < aes_ctx->tag_len) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_BAD_DECRYPT);
    return 0;
  }

  plaintext_len = in_len - aes_ctx->tag_len;

  if (max_out_len < plaintext_len) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_BUFFER_TOO_SMALL);
    return 0;
  }

  if (nonce_len != EVP_AEAD_AES_CTR_HMAC_SHA256_NONCE_LEN) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_UNSUPPORTED_NONCE_SIZE);
    return 0;
  }

  uint8_t hmac_result[SHA256_DIGEST_LENGTH];
  hmac_calculate(hmac_result, &aes_ctx->inner_init_state,
                 &aes_ctx->outer_init_state, ad, ad_len, nonce, in,
                 plaintext_len);
  if (CRYPTO_memcmp(hmac_result, in + plaintext_len, aes_ctx->tag_len) != 0) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_BAD_DECRYPT);
    return 0;
  }

  aead_aes_ctr_hmac_sha256_crypt(aes_ctx, out, in, plaintext_len, nonce);

  *out_len = plaintext_len;
  return 1;
}

static const EVP_AEAD aead_aes_128_ctr_hmac_sha256 = {
    16 /* AES key */ + 32 /* HMAC key */,
    12,                                       /* nonce length */
    EVP_AEAD_AES_CTR_HMAC_SHA256_TAG_LEN,     /* overhead */
    EVP_AEAD_AES_CTR_HMAC_SHA256_TAG_LEN,     /* max tag length */

    aead_aes_ctr_hmac_sha256_init,
    NULL /* init_with_direction */,
    aead_aes_ctr_hmac_sha256_cleanup,
    aead_aes_ctr_hmac_sha256_seal,
    aead_aes_ctr_hmac_sha256_open,
    NULL /* get_iv */,
};

static const EVP_AEAD aead_aes_256_ctr_hmac_sha256 = {
    32 /* AES key */ + 32 /* HMAC key */,
    12,                                       /* nonce length */
    EVP_AEAD_AES_CTR_HMAC_SHA256_TAG_LEN,     /* overhead */
    EVP_AEAD_AES_CTR_HMAC_SHA256_TAG_LEN,     /* max tag length */

    aead_aes_ctr_hmac_sha256_init,
    NULL /* init_with_direction */,
    aead_aes_ctr_hmac_sha256_cleanup,
    aead_aes_ctr_hmac_sha256_seal,
    aead_aes_ctr_hmac_sha256_open,
    NULL /* get_iv */,
};

const EVP_AEAD *EVP_aead_aes_128_ctr_hmac_sha256(void) {
  return &aead_aes_128_ctr_hmac_sha256;
}

const EVP_AEAD *EVP_aead_aes_256_ctr_hmac_sha256(void) {
  return &aead_aes_256_ctr_hmac_sha256;
}

#if !defined(OPENSSL_SMALL)

#define EVP_AEAD_AES_GCM_SIV_NONCE_LEN 12
#define EVP_AEAD_AES_GCM_SIV_TAG_LEN 16

struct aead_aes_gcm_siv_ctx {
  union {
    double align;
    AES_KEY ks;
  } ks;
  block128_f kgk_block;
  unsigned is_256:1;
};

static int aead_aes_gcm_siv_init(EVP_AEAD_CTX *ctx, const uint8_t *key,
                                 size_t key_len, size_t tag_len) {
  const size_t key_bits = key_len * 8;

  if (key_bits != 128 && key_bits != 256) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_BAD_KEY_LENGTH);
    return 0; /* EVP_AEAD_CTX_init should catch this. */
  }

  if (tag_len == EVP_AEAD_DEFAULT_TAG_LENGTH) {
    tag_len = EVP_AEAD_AES_GCM_SIV_TAG_LEN;
  }

  if (tag_len != EVP_AEAD_AES_GCM_SIV_TAG_LEN) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_TAG_TOO_LARGE);
    return 0;
  }

  struct aead_aes_gcm_siv_ctx *gcm_siv_ctx =
      OPENSSL_malloc(sizeof(struct aead_aes_gcm_siv_ctx));
  if (gcm_siv_ctx == NULL) {
    return 0;
  }
  OPENSSL_memset(gcm_siv_ctx, 0, sizeof(struct aead_aes_gcm_siv_ctx));

  if (aesni_capable()) {
    aesni_set_encrypt_key(key, key_len * 8, &gcm_siv_ctx->ks.ks);
    gcm_siv_ctx->kgk_block = (block128_f)aesni_encrypt;
  } else if (hwaes_capable()) {
    aes_hw_set_encrypt_key(key, key_len * 8, &gcm_siv_ctx->ks.ks);
    gcm_siv_ctx->kgk_block = (block128_f)aes_hw_encrypt;
  } else if (vpaes_capable()) {
    vpaes_set_encrypt_key(key, key_len * 8, &gcm_siv_ctx->ks.ks);
    gcm_siv_ctx->kgk_block = (block128_f)vpaes_encrypt;
  } else {
    AES_set_encrypt_key(key, key_len * 8, &gcm_siv_ctx->ks.ks);
    gcm_siv_ctx->kgk_block = (block128_f)AES_encrypt;
  }

  gcm_siv_ctx->is_256 = (key_len == 32);
  ctx->aead_state = gcm_siv_ctx;

  return 1;
}

static void aead_aes_gcm_siv_cleanup(EVP_AEAD_CTX *ctx) {
  struct aead_aes_gcm_siv_ctx *gcm_siv_ctx = ctx->aead_state;
  OPENSSL_cleanse(gcm_siv_ctx, sizeof(struct aead_aes_gcm_siv_ctx));
  OPENSSL_free(gcm_siv_ctx);
}

/* gcm_siv_crypt encrypts (or decrypts—it's the same thing) |in_len| bytes from
 * |in| to |out|, using the block function |enc_block| with |key| in counter
 * mode, starting at |initial_counter|. This differs from the traditional
 * counter mode code in that the counter is handled little-endian, only the
 * first four bytes are used and the GCM-SIV tweak to the final byte is
 * applied. The |in| and |out| pointers may be equal but otherwise must not
 * alias. */
static void gcm_siv_crypt(uint8_t *out, const uint8_t *in, size_t in_len,
                          const uint8_t initial_counter[AES_BLOCK_SIZE],
                          block128_f enc_block, const AES_KEY *key) {
  union {
    uint32_t w[4];
    uint8_t c[16];
  } counter;

  OPENSSL_memcpy(counter.c, initial_counter, AES_BLOCK_SIZE);
  counter.c[15] |= 0x80;

  for (size_t done = 0; done < in_len;) {
    uint8_t keystream[AES_BLOCK_SIZE];
    enc_block(counter.c, keystream, key);
    counter.w[0]++;

    size_t todo = AES_BLOCK_SIZE;
    if (in_len - done < todo) {
      todo = in_len - done;
    }

    for (size_t i = 0; i < todo; i++) {
      out[done + i] = keystream[i] ^ in[done + i];
    }

    done += todo;
  }
}

/* gcm_siv_polyval evaluates POLYVAL at |auth_key| on the given plaintext and
 * AD. The result is written to |out_tag|. */
static void gcm_siv_polyval(
    uint8_t out_tag[16], const uint8_t *in, size_t in_len, const uint8_t *ad,
    size_t ad_len, const uint8_t auth_key[16],
    const uint8_t nonce[EVP_AEAD_AES_GCM_SIV_NONCE_LEN]) {
  struct polyval_ctx polyval_ctx;
  CRYPTO_POLYVAL_init(&polyval_ctx, auth_key);

  CRYPTO_POLYVAL_update_blocks(&polyval_ctx, ad, ad_len & ~15);

  uint8_t scratch[16];
  if (ad_len & 15) {
    OPENSSL_memset(scratch, 0, sizeof(scratch));
    OPENSSL_memcpy(scratch, &ad[ad_len & ~15], ad_len & 15);
    CRYPTO_POLYVAL_update_blocks(&polyval_ctx, scratch, sizeof(scratch));
  }

  CRYPTO_POLYVAL_update_blocks(&polyval_ctx, in, in_len & ~15);
  if (in_len & 15) {
    OPENSSL_memset(scratch, 0, sizeof(scratch));
    OPENSSL_memcpy(scratch, &in[in_len & ~15], in_len & 15);
    CRYPTO_POLYVAL_update_blocks(&polyval_ctx, scratch, sizeof(scratch));
  }

  union {
    uint8_t c[16];
    struct {
      uint64_t ad;
      uint64_t in;
    } bitlens;
  } length_block;

  length_block.bitlens.ad = ad_len * 8;
  length_block.bitlens.in = in_len * 8;
  CRYPTO_POLYVAL_update_blocks(&polyval_ctx, length_block.c,
                               sizeof(length_block));

  CRYPTO_POLYVAL_finish(&polyval_ctx, out_tag);
  for (size_t i = 0; i < EVP_AEAD_AES_GCM_SIV_NONCE_LEN; i++) {
    out_tag[i] ^= nonce[i];
  }
  out_tag[15] &= 0x7f;
}

/* gcm_siv_record_keys contains the keys used for a specific GCM-SIV record. */
struct gcm_siv_record_keys {
  uint8_t auth_key[16];
  union {
    double align;
    AES_KEY ks;
  } enc_key;
  block128_f enc_block;
};

/* gcm_siv_keys calculates the keys for a specific GCM-SIV record with the
 * given nonce and writes them to |*out_keys|. */
static void gcm_siv_keys(
    const struct aead_aes_gcm_siv_ctx *gcm_siv_ctx,
    struct gcm_siv_record_keys *out_keys,
    const uint8_t nonce[EVP_AEAD_AES_GCM_SIV_NONCE_LEN]) {
  const AES_KEY *const key = &gcm_siv_ctx->ks.ks;
  uint8_t key_material[(128 /* POLYVAL key */ + 256 /* max AES key */) / 8];
  const size_t blocks_needed = gcm_siv_ctx->is_256 ? 6 : 4;

  uint8_t counter[AES_BLOCK_SIZE];
  OPENSSL_memset(counter, 0, AES_BLOCK_SIZE - EVP_AEAD_AES_GCM_SIV_NONCE_LEN);
  OPENSSL_memcpy(counter + AES_BLOCK_SIZE - EVP_AEAD_AES_GCM_SIV_NONCE_LEN,
                 nonce, EVP_AEAD_AES_GCM_SIV_NONCE_LEN);
  for (size_t i = 0; i < blocks_needed; i++) {
    counter[0] = i;

    uint8_t ciphertext[AES_BLOCK_SIZE];
    gcm_siv_ctx->kgk_block(counter, ciphertext, key);
    OPENSSL_memcpy(&key_material[i * 8], ciphertext, 8);
  }

  OPENSSL_memcpy(out_keys->auth_key, key_material, 16);
  aes_ctr_set_key(&out_keys->enc_key.ks, NULL, &out_keys->enc_block,
                  key_material + 16, gcm_siv_ctx->is_256 ? 32 : 16);
}

static int aead_aes_gcm_siv_seal(const EVP_AEAD_CTX *ctx, uint8_t *out,
                                 size_t *out_len, size_t max_out_len,
                                 const uint8_t *nonce, size_t nonce_len,
                                 const uint8_t *in, size_t in_len,
                                 const uint8_t *ad, size_t ad_len) {
  const struct aead_aes_gcm_siv_ctx *gcm_siv_ctx = ctx->aead_state;
  const uint64_t in_len_64 = in_len;
  const uint64_t ad_len_64 = ad_len;

  if (in_len + EVP_AEAD_AES_GCM_SIV_TAG_LEN < in_len ||
      in_len_64 > (UINT64_C(1) << 36) ||
      ad_len_64 >= (UINT64_C(1) << 61)) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_TOO_LARGE);
    return 0;
  }

  if (max_out_len < in_len + EVP_AEAD_AES_GCM_SIV_TAG_LEN) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_BUFFER_TOO_SMALL);
    return 0;
  }

  if (nonce_len != EVP_AEAD_AES_GCM_SIV_NONCE_LEN) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_UNSUPPORTED_NONCE_SIZE);
    return 0;
  }

  struct gcm_siv_record_keys keys;
  gcm_siv_keys(gcm_siv_ctx, &keys, nonce);

  uint8_t tag[16];
  gcm_siv_polyval(tag, in, in_len, ad, ad_len, keys.auth_key, nonce);
  keys.enc_block(tag, tag, &keys.enc_key.ks);

  gcm_siv_crypt(out, in, in_len, tag, keys.enc_block, &keys.enc_key.ks);

  OPENSSL_memcpy(&out[in_len], tag, EVP_AEAD_AES_GCM_SIV_TAG_LEN);
  *out_len = in_len + EVP_AEAD_AES_GCM_SIV_TAG_LEN;

  return 1;
}

static int aead_aes_gcm_siv_open(const EVP_AEAD_CTX *ctx, uint8_t *out,
                                 size_t *out_len, size_t max_out_len,
                                 const uint8_t *nonce, size_t nonce_len,
                                 const uint8_t *in, size_t in_len,
                                 const uint8_t *ad, size_t ad_len) {
  const uint64_t ad_len_64 = ad_len;
  if (ad_len_64 >= (UINT64_C(1) << 61)) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_TOO_LARGE);
    return 0;
  }

  const uint64_t in_len_64 = in_len;
  if (in_len < EVP_AEAD_AES_GCM_SIV_TAG_LEN ||
      in_len_64 > (UINT64_C(1) << 36) + AES_BLOCK_SIZE) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_BAD_DECRYPT);
    return 0;
  }

  if (nonce_len != EVP_AEAD_AES_GCM_SIV_NONCE_LEN) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_UNSUPPORTED_NONCE_SIZE);
    return 0;
  }

  const struct aead_aes_gcm_siv_ctx *gcm_siv_ctx = ctx->aead_state;
  const size_t plaintext_len = in_len - EVP_AEAD_AES_GCM_SIV_TAG_LEN;

  if (max_out_len < plaintext_len) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_BUFFER_TOO_SMALL);
    return 0;
  }

  struct gcm_siv_record_keys keys;
  gcm_siv_keys(gcm_siv_ctx, &keys, nonce);

  gcm_siv_crypt(out, in, plaintext_len, &in[plaintext_len], keys.enc_block,
                &keys.enc_key.ks);

  uint8_t expected_tag[EVP_AEAD_AES_GCM_SIV_TAG_LEN];
  gcm_siv_polyval(expected_tag, out, plaintext_len, ad, ad_len, keys.auth_key,
                  nonce);
  keys.enc_block(expected_tag, expected_tag, &keys.enc_key.ks);

  if (CRYPTO_memcmp(expected_tag, &in[plaintext_len], sizeof(expected_tag)) !=
      0) {
    OPENSSL_PUT_ERROR(CIPHER, CIPHER_R_BAD_DECRYPT);
    return 0;
  }

  *out_len = plaintext_len;
  return 1;
}

static const EVP_AEAD aead_aes_128_gcm_siv = {
    16,                             /* key length */
    EVP_AEAD_AES_GCM_SIV_NONCE_LEN, /* nonce length */
    EVP_AEAD_AES_GCM_SIV_TAG_LEN,   /* overhead */
    EVP_AEAD_AES_GCM_SIV_TAG_LEN,   /* max tag length */

    aead_aes_gcm_siv_init,
    NULL /* init_with_direction */,
    aead_aes_gcm_siv_cleanup,
    aead_aes_gcm_siv_seal,
    aead_aes_gcm_siv_open,
    NULL /* get_iv */,
};

static const EVP_AEAD aead_aes_256_gcm_siv = {
    32,                             /* key length */
    EVP_AEAD_AES_GCM_SIV_NONCE_LEN, /* nonce length */
    EVP_AEAD_AES_GCM_SIV_TAG_LEN,   /* overhead */
    EVP_AEAD_AES_GCM_SIV_TAG_LEN,   /* max tag length */

    aead_aes_gcm_siv_init,
    NULL /* init_with_direction */,
    aead_aes_gcm_siv_cleanup,
    aead_aes_gcm_siv_seal,
    aead_aes_gcm_siv_open,
    NULL /* get_iv */,
};

const EVP_AEAD *EVP_aead_aes_128_gcm_siv(void) {
  return &aead_aes_128_gcm_siv;
}

const EVP_AEAD *EVP_aead_aes_256_gcm_siv(void) {
  return &aead_aes_256_gcm_siv;
}

#endif  /* !OPENSSL_SMALL */

int EVP_has_aes_hardware(void) {
#if defined(OPENSSL_X86) || defined(OPENSSL_X86_64)
  return aesni_capable() && crypto_gcm_clmul_enabled();
#elif defined(OPENSSL_ARM) || defined(OPENSSL_AARCH64)
  return hwaes_capable() && CRYPTO_is_ARMv8_PMULL_capable();
#else
  return 0;
#endif
}
