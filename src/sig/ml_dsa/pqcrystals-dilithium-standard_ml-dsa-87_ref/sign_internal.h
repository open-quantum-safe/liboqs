#ifndef SIGN_INTERNAL_H
#define SIGN_INTERNAL_H

#include <stddef.h>
#include <stdint.h>
#include "params.h"
#include "polyvec.h"
#include "poly.h"

#define crypto_sign_signature_internal DILITHIUM_NAMESPACE(signature_internal)
int crypto_sign_signature_internal(uint8_t *sig,
                                   size_t *siglen,
                                   const uint8_t *m,
                                   size_t mlen,
                                   const uint8_t *mpfx,
                                   size_t mpfxlen,
                                   const uint8_t *ctx,
                                   size_t ctxlen,
                                   const uint8_t rnd[RNDBYTES],
                                   const uint8_t *sk);

#define crypto_sign_verify_internal DILITHIUM_NAMESPACE(verify_internal)
int crypto_sign_verify_internal(const uint8_t *sig,
                                size_t siglen,
                                const uint8_t *m,
                                size_t mlen,
                                const uint8_t *mpfx,
                                size_t mpfxlen,
                                const uint8_t *ctx,
                                size_t ctxlen,
                                const uint8_t *pk);

#endif
