/* Copyright (C) 1995-1998 Eric Young (eay@cryptsoft.com)
 * All rights reserved.
 *
 * This package is an SSL implementation written
 * by Eric Young (eay@cryptsoft.com).
 * The implementation was written so as to conform with Netscapes SSL.
 *
 * This library is free for commercial and non-commercial use as long as
 * the following conditions are aheared to.  The following conditions
 * apply to all code found in this distribution, be it the RC4, RSA,
 * lhash, DES, etc., code; not just the SSL code.  The SSL documentation
 * included with this distribution is covered by the same copyright terms
 * except that the holder is Tim Hudson (tjh@cryptsoft.com).
 *
 * Copyright remains Eric Young's, and as such any Copyright notices in
 * the code are not to be removed.
 * If this package is used in a product, Eric Young should be given attribution
 * as the author of the parts of the library used.
 * This can be in the form of a textual message at program startup or
 * in documentation (online or textual) provided with the package.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    "This product includes cryptographic software written by
 *     Eric Young (eay@cryptsoft.com)"
 *    The word 'cryptographic' can be left out if the rouines from the library
 *    being used are not cryptographic related :-).
 * 4. If you include any Windows specific code (or a derivative thereof) from
 *    the apps directory (application code) you must include an acknowledgement:
 *    "This product includes software written by Tim Hudson (tjh@cryptsoft.com)"
 *
 * THIS SOFTWARE IS PROVIDED BY ERIC YOUNG ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * The licence and distribution terms for any publically available version or
 * derivative of this code cannot be changed.  i.e. this code cannot simply be
 * copied and put under another distribution licence
 * [including the GNU Public Licence.]
 */
/* ====================================================================
 * Copyright (c) 1998-2007 The OpenSSL Project.  All rights reserved.
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
 * ====================================================================
 *
 * This product includes cryptographic software written by Eric Young
 * (eay@cryptsoft.com).  This product includes software written by Tim
 * Hudson (tjh@cryptsoft.com).
 *
 */
/* ====================================================================
 * Copyright 2005 Nokia. All rights reserved.
 *
 * The portions of the attached software ("Contribution") is developed by
 * Nokia Corporation and is licensed pursuant to the OpenSSL open source
 * license.
 *
 * The Contribution, originally written by Mika Kousa and Pasi Eronen of
 * Nokia Corporation, consists of the "PSK" (Pre-Shared Key) ciphersuites
 * support (see RFC 4279) to OpenSSL.
 *
 * No patent licenses or other rights except those expressly stated in
 * the OpenSSL open source license shall be deemed granted or received
 * expressly, by implication, estoppel, or otherwise.
 *
 * No assurances are provided by Nokia that the Contribution does not
 * infringe the patent or other intellectual property rights of any third
 * party or that the license provides you with all the necessary rights
 * to make use of the Contribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND. IN
 * ADDITION TO THE DISCLAIMERS INCLUDED IN THE LICENSE, NOKIA
 * SPECIFICALLY DISCLAIMS ANY LIABILITY FOR CLAIMS BROUGHT BY YOU OR ANY
 * OTHER ENTITY BASED ON INFRINGEMENT OF INTELLECTUAL PROPERTY RIGHTS OR
 * OTHERWISE. */

#include <openssl/ssl.h>

#include <assert.h>
#include <string.h>

#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/md5.h>
#include <openssl/mem.h>
#include <openssl/nid.h>
#include <openssl/rand.h>

#include "../crypto/internal.h"
#include "internal.h"


/* tls1_P_hash computes the TLS P_<hash> function as described in RFC 5246,
 * section 5. It XORs |out_len| bytes to |out|, using |md| as the hash and
 * |secret| as the secret. |seed1| through |seed3| are concatenated to form the
 * seed parameter. It returns one on success and zero on failure. */
static int tls1_P_hash(uint8_t *out, size_t out_len, const EVP_MD *md,
                       const uint8_t *secret, size_t secret_len,
                       const uint8_t *seed1, size_t seed1_len,
                       const uint8_t *seed2, size_t seed2_len,
                       const uint8_t *seed3, size_t seed3_len) {
  HMAC_CTX ctx, ctx_tmp, ctx_init;
  uint8_t A1[EVP_MAX_MD_SIZE];
  unsigned A1_len;
  int ret = 0;

  size_t chunk = EVP_MD_size(md);

  HMAC_CTX_init(&ctx);
  HMAC_CTX_init(&ctx_tmp);
  HMAC_CTX_init(&ctx_init);
  if (!HMAC_Init_ex(&ctx_init, secret, secret_len, md, NULL) ||
      !HMAC_CTX_copy_ex(&ctx, &ctx_init) ||
      !HMAC_Update(&ctx, seed1, seed1_len) ||
      !HMAC_Update(&ctx, seed2, seed2_len) ||
      !HMAC_Update(&ctx, seed3, seed3_len) ||
      !HMAC_Final(&ctx, A1, &A1_len)) {
    goto err;
  }

  for (;;) {
    unsigned len;
    uint8_t hmac[EVP_MAX_MD_SIZE];
    if (!HMAC_CTX_copy_ex(&ctx, &ctx_init) ||
        !HMAC_Update(&ctx, A1, A1_len) ||
        /* Save a copy of |ctx| to compute the next A1 value below. */
        (out_len > chunk && !HMAC_CTX_copy_ex(&ctx_tmp, &ctx)) ||
        !HMAC_Update(&ctx, seed1, seed1_len) ||
        !HMAC_Update(&ctx, seed2, seed2_len) ||
        !HMAC_Update(&ctx, seed3, seed3_len) ||
        !HMAC_Final(&ctx, hmac, &len)) {
      goto err;
    }
    assert(len == chunk);

    /* XOR the result into |out|. */
    if (len > out_len) {
      len = out_len;
    }
    unsigned i;
    for (i = 0; i < len; i++) {
      out[i] ^= hmac[i];
    }
    out += len;
    out_len -= len;

    if (out_len == 0) {
      break;
    }

    /* Calculate the next A1 value. */
    if (!HMAC_Final(&ctx_tmp, A1, &A1_len)) {
      goto err;
    }
  }

  ret = 1;

err:
  HMAC_CTX_cleanup(&ctx);
  HMAC_CTX_cleanup(&ctx_tmp);
  HMAC_CTX_cleanup(&ctx_init);
  OPENSSL_cleanse(A1, sizeof(A1));
  return ret;
}

int tls1_prf(const EVP_MD *digest, uint8_t *out, size_t out_len,
             const uint8_t *secret, size_t secret_len, const char *label,
             size_t label_len, const uint8_t *seed1, size_t seed1_len,
             const uint8_t *seed2, size_t seed2_len) {
  if (out_len == 0) {
    return 1;
  }

  OPENSSL_memset(out, 0, out_len);

  if (digest == EVP_md5_sha1()) {
    /* If using the MD5/SHA1 PRF, |secret| is partitioned between SHA-1 and
     * MD5, MD5 first. */
    size_t secret_half = secret_len - (secret_len / 2);
    if (!tls1_P_hash(out, out_len, EVP_md5(), secret, secret_half,
                     (const uint8_t *)label, label_len, seed1, seed1_len, seed2,
                     seed2_len)) {
      return 0;
    }

    /* Note that, if |secret_len| is odd, the two halves share a byte. */
    secret = secret + (secret_len - secret_half);
    secret_len = secret_half;

    digest = EVP_sha1();
  }

  if (!tls1_P_hash(out, out_len, digest, secret, secret_len,
                   (const uint8_t *)label, label_len, seed1, seed1_len, seed2,
                   seed2_len)) {
    return 0;
  }

  return 1;
}

static int ssl3_prf(uint8_t *out, size_t out_len, const uint8_t *secret,
                    size_t secret_len, const char *label, size_t label_len,
                    const uint8_t *seed1, size_t seed1_len,
                    const uint8_t *seed2, size_t seed2_len) {
  EVP_MD_CTX md5;
  EVP_MD_CTX sha1;
  uint8_t buf[16], smd[SHA_DIGEST_LENGTH];
  uint8_t c = 'A';
  size_t i, j, k;

  k = 0;
  EVP_MD_CTX_init(&md5);
  EVP_MD_CTX_init(&sha1);
  for (i = 0; i < out_len; i += MD5_DIGEST_LENGTH) {
    k++;
    if (k > sizeof(buf)) {
      /* bug: 'buf' is too small for this ciphersuite */
      OPENSSL_PUT_ERROR(SSL, ERR_R_INTERNAL_ERROR);
      return 0;
    }

    for (j = 0; j < k; j++) {
      buf[j] = c;
    }
    c++;
    if (!EVP_DigestInit_ex(&sha1, EVP_sha1(), NULL)) {
      OPENSSL_PUT_ERROR(SSL, ERR_LIB_EVP);
      return 0;
    }
    EVP_DigestUpdate(&sha1, buf, k);
    EVP_DigestUpdate(&sha1, secret, secret_len);
    /* |label| is ignored for SSLv3. */
    if (seed1_len) {
      EVP_DigestUpdate(&sha1, seed1, seed1_len);
    }
    if (seed2_len) {
      EVP_DigestUpdate(&sha1, seed2, seed2_len);
    }
    EVP_DigestFinal_ex(&sha1, smd, NULL);

    if (!EVP_DigestInit_ex(&md5, EVP_md5(), NULL)) {
      OPENSSL_PUT_ERROR(SSL, ERR_LIB_EVP);
      return 0;
    }
    EVP_DigestUpdate(&md5, secret, secret_len);
    EVP_DigestUpdate(&md5, smd, SHA_DIGEST_LENGTH);
    if (i + MD5_DIGEST_LENGTH > out_len) {
      EVP_DigestFinal_ex(&md5, smd, NULL);
      OPENSSL_memcpy(out, smd, out_len - i);
    } else {
      EVP_DigestFinal_ex(&md5, out, NULL);
    }

    out += MD5_DIGEST_LENGTH;
  }

  OPENSSL_cleanse(smd, SHA_DIGEST_LENGTH);
  EVP_MD_CTX_cleanup(&md5);
  EVP_MD_CTX_cleanup(&sha1);

  return 1;
}

static int tls1_setup_key_block(SSL_HANDSHAKE *hs) {
  SSL *const ssl = hs->ssl;
  if (hs->key_block_len != 0) {
    return 1;
  }

  SSL_SESSION *session = ssl->session;
  if (hs->new_session != NULL) {
    session = hs->new_session;
  }

  const EVP_AEAD *aead = NULL;
  size_t mac_secret_len, fixed_iv_len;
  if (session->cipher == NULL ||
      !ssl_cipher_get_evp_aead(&aead, &mac_secret_len, &fixed_iv_len,
                               session->cipher, ssl3_protocol_version(ssl))) {
    OPENSSL_PUT_ERROR(SSL, SSL_R_CIPHER_OR_HASH_UNAVAILABLE);
    return 0;
  }
  size_t key_len = EVP_AEAD_key_length(aead);
  if (mac_secret_len > 0) {
    /* For "stateful" AEADs (i.e. compatibility with pre-AEAD cipher suites) the
     * key length reported by |EVP_AEAD_key_length| will include the MAC key
     * bytes and initial implicit IV. */
    if (key_len < mac_secret_len + fixed_iv_len) {
      OPENSSL_PUT_ERROR(SSL, ERR_R_INTERNAL_ERROR);
      return 0;
    }
    key_len -= mac_secret_len + fixed_iv_len;
  }

  assert(mac_secret_len < 256);
  assert(key_len < 256);
  assert(fixed_iv_len < 256);

  ssl->s3->tmp.new_mac_secret_len = (uint8_t)mac_secret_len;
  ssl->s3->tmp.new_key_len = (uint8_t)key_len;
  ssl->s3->tmp.new_fixed_iv_len = (uint8_t)fixed_iv_len;

  size_t key_block_len = SSL_get_key_block_len(ssl);

  uint8_t *keyblock = OPENSSL_malloc(key_block_len);
  if (keyblock == NULL) {
    OPENSSL_PUT_ERROR(SSL, ERR_R_MALLOC_FAILURE);
    return 0;
  }

  if (!SSL_generate_key_block(ssl, keyblock, key_block_len)) {
    OPENSSL_free(keyblock);
    return 0;
  }

  assert(key_block_len < 256);
  hs->key_block_len = (uint8_t)key_block_len;
  hs->key_block = keyblock;
  return 1;
}

int tls1_change_cipher_state(SSL_HANDSHAKE *hs, int which) {
  SSL *const ssl = hs->ssl;
  /* Ensure the key block is set up. */
  if (!tls1_setup_key_block(hs)) {
    return 0;
  }

  /* is_read is true if we have just read a ChangeCipherSpec message - i.e. we
   * need to update the read cipherspec. Otherwise we have just written one. */
  const char is_read = (which & SSL3_CC_READ) != 0;
  /* use_client_keys is true if we wish to use the keys for the "client write"
   * direction. This is the case if we're a client sending a ChangeCipherSpec,
   * or a server reading a client's ChangeCipherSpec. */
  const char use_client_keys = which == SSL3_CHANGE_CIPHER_CLIENT_WRITE ||
                               which == SSL3_CHANGE_CIPHER_SERVER_READ;

  size_t mac_secret_len = ssl->s3->tmp.new_mac_secret_len;
  size_t key_len = ssl->s3->tmp.new_key_len;
  size_t iv_len = ssl->s3->tmp.new_fixed_iv_len;
  assert((mac_secret_len + key_len + iv_len) * 2 == hs->key_block_len);

  const uint8_t *key_data = hs->key_block;
  const uint8_t *client_write_mac_secret = key_data;
  key_data += mac_secret_len;
  const uint8_t *server_write_mac_secret = key_data;
  key_data += mac_secret_len;
  const uint8_t *client_write_key = key_data;
  key_data += key_len;
  const uint8_t *server_write_key = key_data;
  key_data += key_len;
  const uint8_t *client_write_iv = key_data;
  key_data += iv_len;
  const uint8_t *server_write_iv = key_data;
  key_data += iv_len;

  const uint8_t *mac_secret, *key, *iv;
  if (use_client_keys) {
    mac_secret = client_write_mac_secret;
    key = client_write_key;
    iv = client_write_iv;
  } else {
    mac_secret = server_write_mac_secret;
    key = server_write_key;
    iv = server_write_iv;
  }

  SSL_AEAD_CTX *aead_ctx = SSL_AEAD_CTX_new(
      is_read ? evp_aead_open : evp_aead_seal, ssl3_protocol_version(ssl),
      hs->new_cipher, key, key_len, mac_secret, mac_secret_len, iv, iv_len);
  if (aead_ctx == NULL) {
    return 0;
  }

  if (is_read) {
    return ssl->method->set_read_state(ssl, aead_ctx);
  }

  return ssl->method->set_write_state(ssl, aead_ctx);
}

size_t SSL_get_key_block_len(const SSL *ssl) {
  return 2 * ((size_t)ssl->s3->tmp.new_mac_secret_len +
              (size_t)ssl->s3->tmp.new_key_len +
              (size_t)ssl->s3->tmp.new_fixed_iv_len);
}

int SSL_generate_key_block(const SSL *ssl, uint8_t *out, size_t out_len) {
  if (ssl3_protocol_version(ssl) == SSL3_VERSION) {
    return ssl3_prf(out, out_len, SSL_get_session(ssl)->master_key,
                    SSL_get_session(ssl)->master_key_length,
                    TLS_MD_KEY_EXPANSION_CONST, TLS_MD_KEY_EXPANSION_CONST_SIZE,
                    ssl->s3->server_random, SSL3_RANDOM_SIZE,
                    ssl->s3->client_random, SSL3_RANDOM_SIZE);
  }

  const EVP_MD *digest = ssl_get_handshake_digest(
      SSL_get_session(ssl)->cipher->algorithm_prf, ssl3_protocol_version(ssl));
  if (digest == NULL) {
    OPENSSL_PUT_ERROR(SSL, ERR_R_INTERNAL_ERROR);
    return 0;
  }
  return tls1_prf(digest, out, out_len, SSL_get_session(ssl)->master_key,
                  SSL_get_session(ssl)->master_key_length,
                  TLS_MD_KEY_EXPANSION_CONST, TLS_MD_KEY_EXPANSION_CONST_SIZE,
                  ssl->s3->server_random, SSL3_RANDOM_SIZE,
                  ssl->s3->client_random, SSL3_RANDOM_SIZE);
}

int tls1_generate_master_secret(SSL_HANDSHAKE *hs, uint8_t *out,
                                const uint8_t *premaster,
                                size_t premaster_len) {
  const SSL *ssl = hs->ssl;
  if (hs->extended_master_secret) {
    uint8_t digests[EVP_MAX_MD_SIZE];
    size_t digests_len;
    if (!SSL_TRANSCRIPT_get_hash(&hs->transcript, digests, &digests_len) ||
        !tls1_prf(SSL_TRANSCRIPT_md(&hs->transcript), out,
                  SSL3_MASTER_SECRET_SIZE, premaster, premaster_len,
                  TLS_MD_EXTENDED_MASTER_SECRET_CONST,
                  TLS_MD_EXTENDED_MASTER_SECRET_CONST_SIZE, digests,
                  digests_len, NULL, 0)) {
      return 0;
    }
  } else {
    if (ssl3_protocol_version(ssl) == SSL3_VERSION) {
      if (!ssl3_prf(out, SSL3_MASTER_SECRET_SIZE, premaster, premaster_len,
                    TLS_MD_MASTER_SECRET_CONST, TLS_MD_MASTER_SECRET_CONST_SIZE,
                    ssl->s3->client_random, SSL3_RANDOM_SIZE,
                    ssl->s3->server_random, SSL3_RANDOM_SIZE)) {
        return 0;
      }
    } else {
      if (!tls1_prf(SSL_TRANSCRIPT_md(&hs->transcript), out,
                    SSL3_MASTER_SECRET_SIZE, premaster, premaster_len,
                    TLS_MD_MASTER_SECRET_CONST, TLS_MD_MASTER_SECRET_CONST_SIZE,
                    ssl->s3->client_random, SSL3_RANDOM_SIZE,
                    ssl->s3->server_random, SSL3_RANDOM_SIZE)) {
        return 0;
      }
    }
  }

  return SSL3_MASTER_SECRET_SIZE;
}

int SSL_export_keying_material(SSL *ssl, uint8_t *out, size_t out_len,
                               const char *label, size_t label_len,
                               const uint8_t *context, size_t context_len,
                               int use_context) {
  if (!ssl->s3->have_version || ssl->version == SSL3_VERSION) {
    return 0;
  }

  /* Exporters may not be used in the middle of a renegotiation. */
  if (SSL_in_init(ssl) && !SSL_in_false_start(ssl)) {
    return 0;
  }

  if (ssl3_protocol_version(ssl) >= TLS1_3_VERSION) {
    return tls13_export_keying_material(ssl, out, out_len, label, label_len,
                                        context, context_len, use_context);
  }

  size_t seed_len = 2 * SSL3_RANDOM_SIZE;
  if (use_context) {
    if (context_len >= 1u << 16) {
      OPENSSL_PUT_ERROR(SSL, ERR_R_OVERFLOW);
      return 0;
    }
    seed_len += 2 + context_len;
  }
  uint8_t *seed = OPENSSL_malloc(seed_len);
  if (seed == NULL) {
    OPENSSL_PUT_ERROR(SSL, ERR_R_MALLOC_FAILURE);
    return 0;
  }

  OPENSSL_memcpy(seed, ssl->s3->client_random, SSL3_RANDOM_SIZE);
  OPENSSL_memcpy(seed + SSL3_RANDOM_SIZE, ssl->s3->server_random,
                 SSL3_RANDOM_SIZE);
  if (use_context) {
    seed[2 * SSL3_RANDOM_SIZE] = (uint8_t)(context_len >> 8);
    seed[2 * SSL3_RANDOM_SIZE + 1] = (uint8_t)context_len;
    OPENSSL_memcpy(seed + 2 * SSL3_RANDOM_SIZE + 2, context, context_len);
  }

  const EVP_MD *digest = ssl_get_handshake_digest(
      SSL_get_session(ssl)->cipher->algorithm_prf, ssl3_protocol_version(ssl));
  if (digest == NULL) {
    OPENSSL_PUT_ERROR(SSL, ERR_R_INTERNAL_ERROR);
    return 0;
  }
  int ret = tls1_prf(digest, out, out_len, SSL_get_session(ssl)->master_key,
                     SSL_get_session(ssl)->master_key_length, label, label_len,
                     seed, seed_len, NULL, 0);
  OPENSSL_free(seed);
  return ret;
}
