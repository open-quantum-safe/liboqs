// SPDX-License-Identifier: Apache-2.0

#include <api.h>
#include <encoded_sizes.h>
#include <sig.h>

// The CRYPTO_* macros in api.h and the sizes in encoded_sizes.h are generated separately. Callers allocate by the
// former, while the library checks lengths against the latter, so a mismatch would mean undersized buffers at runtime.
_Static_assert(CRYPTO_BYTES == SIGNATURE_BYTES, "api.h and encoded_sizes.h disagree on the signature size");
_Static_assert(CRYPTO_PUBLICKEYBYTES == PUBLICKEY_BYTES, "api.h and encoded_sizes.h disagree on the public key size");
_Static_assert(CRYPTO_SECRETKEYBYTES == SECRETKEY_BYTES, "api.h and encoded_sizes.h disagree on the secret key size");

#if defined(ENABLE_SIGN)

SQISIGN_API
int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk)
{
    return sqisign_keypair(pk, sk);
}

SQISIGN_API
int
crypto_sign_signature(unsigned char *sig, size_t *siglen, const unsigned char *m, size_t mlen, const unsigned char *sk)
{
    return sqisign_sign_signature(sig, siglen, m, mlen, sk);
}

SQISIGN_API
int
crypto_sign(unsigned char *sm, size_t *smlen, const unsigned char *m, size_t mlen, const unsigned char *sk)
{
    return sqisign_sign(sm, smlen, m, mlen, sk);
}
#endif

SQISIGN_API
int
crypto_sign_verify(const unsigned char *sig,
                   size_t siglen,
                   const unsigned char *m,
                   size_t mlen,
                   const unsigned char *pk)
{
    return sqisign_verify(sig, siglen, m, mlen, pk);
}

SQISIGN_API
int
crypto_sign_open(unsigned char *m, size_t *mlen, const unsigned char *sm, size_t smlen, const unsigned char *pk)
{
    return sqisign_open(m, mlen, sm, smlen, pk);
}
