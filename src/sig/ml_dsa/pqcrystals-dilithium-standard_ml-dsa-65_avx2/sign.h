#ifndef SIGN_H
#define SIGN_H

#include <stddef.h>
#include <stdint.h>
#include "params.h"
#include "polyvec.h"
#include "poly.h"

#define crypto_sign_keypair DILITHIUM_NAMESPACE(keypair)
int crypto_sign_keypair(uint8_t *pk, uint8_t *sk);

#define crypto_sign_signature_ctx DILITHIUM_NAMESPACE(signature_ctx)
int crypto_sign_signature_ctx(uint8_t *sig, size_t *siglen,
                              const uint8_t *m, size_t mlen,
                              const uint8_t *ctx, size_t ctxlen,
                              const uint8_t *sk);

#define crypto_sign_signature DILITHIUM_NAMESPACE(signature)
int crypto_sign_signature(uint8_t *sig, size_t *siglen,
                          const uint8_t *m, size_t mlen,
                          const uint8_t *sk);

#define crypto_sign_ctx DILITHIUM_NAMESPACE(ctx)
int crypto_sign_ctx(uint8_t *sm, size_t *smlen,
                    const uint8_t *m, size_t mlen,
                    const uint8_t *ctx, size_t ctxlen,
                    const uint8_t *sk);

#define crypto_sign DILITHIUM_NAMESPACETOP
int crypto_sign(uint8_t *sm, size_t *smlen,
                const uint8_t *m, size_t mlen,
                const uint8_t *sk);

#define crypto_sign_verify_ctx DILITHIUM_NAMESPACE(verify_ctx)
int crypto_sign_verify_ctx(const uint8_t *sig, size_t siglen,
                           const uint8_t *m, size_t mlen,
                           const uint8_t *ctx, size_t ctxlen,
                           const uint8_t *pk);

#define crypto_sign_verify DILITHIUM_NAMESPACE(verify)
int crypto_sign_verify(const uint8_t *sig, size_t siglen,
                       const uint8_t *m, size_t mlen,
                       const uint8_t *pk);

#define crypto_sign_open_ctx DILITHIUM_NAMESPACE(open_ctx)
int crypto_sign_open_ctx(uint8_t *m, size_t *mlen,
                         const uint8_t *sm, size_t smlen,
                         const uint8_t *ctx, size_t ctxlen,
                         const uint8_t *pk);

#define crypto_sign_open DILITHIUM_NAMESPACE(open)
int crypto_sign_open(uint8_t *m, size_t *mlen,
                     const uint8_t *sm, size_t smlen,
                     const uint8_t *pk);

#endif
