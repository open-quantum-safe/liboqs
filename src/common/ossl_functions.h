// SPDX-License-Identifier: MIT

// This file lists all OpenSSL functions used throughout the liboqs source code.
//
// Note that this file is included multiple times to generate custom
// code by defining the FUNC macro, so no header guard should be
// added here.

VOID_FUNC(void, ERR_print_errors_fp, (FILE *fp), (fp))
VOID_FUNC(void, EVP_CIPHER_CTX_free, (EVP_CIPHER_CTX *c), (c))
FUNC(EVP_CIPHER_CTX *, EVP_CIPHER_CTX_new, (void), ())
FUNC(int, EVP_CIPHER_CTX_set_padding, (EVP_CIPHER_CTX *c, int pad), (c, pad))
FUNC(int, EVP_DigestFinalXOF, (EVP_MD_CTX *ctx, unsigned char *md, size_t len),
     (ctx, md, len))
FUNC(int, EVP_DigestFinal_ex,
     (EVP_MD_CTX *ctx, unsigned char *md, unsigned int *s), (ctx, md, s))
FUNC(int, EVP_DigestInit_ex,
     (EVP_MD_CTX *ctx, const EVP_MD *type, ENGINE *impl), (ctx, type, impl))
FUNC(int, EVP_DigestUpdate, (EVP_MD_CTX *ctx, const void *d, size_t cnt),
     (ctx, d, cnt))
FUNC(int, EVP_EncryptFinal_ex,
     (EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl), (ctx, out, outl))
FUNC(int, EVP_EncryptInit_ex,
     (EVP_CIPHER_CTX *ctx, const EVP_CIPHER *cipher, ENGINE *impl,
      const unsigned char *key, const unsigned char *iv),
     (ctx, cipher, impl, key, iv))
FUNC(int, EVP_EncryptUpdate,
     (EVP_CIPHER_CTX *ctx, unsigned char *out, int *outl,
      const unsigned char *in, int inl),
     (ctx, out, outl, in, inl))
FUNC(int, EVP_MD_CTX_copy_ex, (EVP_MD_CTX *out, const EVP_MD_CTX *in),
     (out, in))
VOID_FUNC(void, EVP_MD_CTX_free, (EVP_MD_CTX *ctx), (ctx))
FUNC(EVP_MD_CTX *, EVP_MD_CTX_new, (void), ())
FUNC(int, EVP_MD_CTX_reset, (EVP_MD_CTX *ctx), (ctx))
FUNC(const EVP_CIPHER *, EVP_aes_128_ecb, (void), ())
FUNC(const EVP_CIPHER *, EVP_aes_128_ctr, (void), ())
FUNC(const EVP_CIPHER *, EVP_aes_256_ecb, (void), ())
FUNC(const EVP_CIPHER *, EVP_aes_256_ctr, (void), ())
#if OPENSSL_VERSION_NUMBER >= 0x30000000L
FUNC(EVP_CIPHER *, EVP_CIPHER_fetch,
     (OSSL_LIB_CTX *ctx, const char *algorithm, const char *properties),
     (ctx, algorithm, properties))
VOID_FUNC(void, EVP_CIPHER_free, (EVP_CIPHER *cipher), (cipher))
FUNC(EVP_MD *, EVP_MD_fetch,
     (OSSL_LIB_CTX *ctx, const char *algorithm, const char *properties),
     (ctx, algorithm, properties))
VOID_FUNC(void, EVP_MD_free, (EVP_MD *md), (md))
#else
FUNC(const EVP_MD *, EVP_sha256, (void), ())
FUNC(const EVP_MD *, EVP_sha384, (void), ())
FUNC(const EVP_MD *, EVP_sha3_256, (void), ())
FUNC(const EVP_MD *, EVP_sha3_384, (void), ())
FUNC(const EVP_MD *, EVP_sha3_512, (void), ())
FUNC(const EVP_MD *, EVP_sha512, (void), ())
FUNC(const EVP_MD *, EVP_shake128, (void), ())
FUNC(const EVP_MD *, EVP_shake256, (void), ())
#endif
VOID_FUNC(void, OPENSSL_cleanse, (void *ptr, size_t len), (ptr, len))
FUNC(int, RAND_bytes, (unsigned char *buf, int num), (buf, num))
FUNC(int, RAND_poll, (void), ())
FUNC(int, RAND_status, (void), ())
VOID_FUNC(void, OPENSSL_thread_stop, (void), ())
FUNC(void *, CRYPTO_malloc, (size_t num, const char *file, int line), (num, file, line))
FUNC(void *, CRYPTO_zalloc, (size_t num, const char *file, int line), (num, file, line))
FUNC(char *, CRYPTO_strdup, (const char *str, const char *file, int line), (str, file, line))
VOID_FUNC(void, CRYPTO_free, (void *ptr, const char *file, int line), (ptr, file, line))
