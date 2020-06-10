/* Written by Dr Stephen N Henson (steve@openssl.org) for the OpenSSL
 * project 1999.
 */
/* ====================================================================
 * Copyright (c) 1999 The OpenSSL Project.  All rights reserved.
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
 *    for use in the OpenSSL Toolkit. (http://www.OpenSSL.org/)"
 *
 * 4. The names "OpenSSL Toolkit" and "OpenSSL Project" must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission. For written permission, please contact
 *    licensing@OpenSSL.org.
 *
 * 5. Products derived from this software may not be called "OpenSSL"
 *    nor may "OpenSSL" appear in their names without prior written
 *    permission of the OpenSSL Project.
 *
 * 6. Redistributions of any form whatsoever must retain the following
 *    acknowledgment:
 *    "This product includes software developed by the OpenSSL Project
 *    for use in the OpenSSL Toolkit (http://www.OpenSSL.org/)"
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
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com). */

#ifndef OPENSSL_HEADER_PKCS8_INTERNAL_H
#define OPENSSL_HEADER_PKCS8_INTERNAL_H

#include <openssl/base.h>

#if defined(__cplusplus)
extern "C" {
#endif


#define PBE_UCS2_CONVERT_PASSWORD 0x1

struct pbe_suite {
  int pbe_nid;
  const EVP_CIPHER *(*cipher_func)(void);
  const EVP_MD *(*md_func)(void);
  /* decrypt_init initialize |ctx| for decrypting. The password is specified by
   * |pass_raw| and |pass_raw_len|. |param| contains the serialized parameters
   * field of the AlgorithmIdentifier.
   *
   * It returns one on success and zero on error. */
  int (*decrypt_init)(const struct pbe_suite *suite, EVP_CIPHER_CTX *ctx,
                      const uint8_t *pass_raw, size_t pass_raw_len, CBS *param);
  int flags;
};

#define PKCS5_DEFAULT_ITERATIONS 2048
#define PKCS5_SALT_LEN 8

int PKCS5_pbe2_decrypt_init(const struct pbe_suite *suite, EVP_CIPHER_CTX *ctx,
                            const uint8_t *pass_raw, size_t pass_raw_len,
                            CBS *param);

/* PKCS5_pbe2_encrypt_init configures |ctx| for encrypting with PKCS #5 PBES2,
 * as defined in RFC 2998, with the specified parameters. It writes the
 * corresponding AlgorithmIdentifier to |out|. */
int PKCS5_pbe2_encrypt_init(CBB *out, EVP_CIPHER_CTX *ctx,
                            const EVP_CIPHER *cipher, unsigned iterations,
                            const uint8_t *pass_raw, size_t pass_raw_len,
                            const uint8_t *salt, size_t salt_len);


#if defined(__cplusplus)
}  /* extern C */
#endif

#endif  /* OPENSSL_HEADER_PKCS8_INTERNAL_H */
