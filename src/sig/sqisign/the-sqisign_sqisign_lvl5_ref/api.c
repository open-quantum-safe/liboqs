// SPDX-License-Identifier: Apache-2.0

#include <api.h>
#include <sig.h>

#if defined(ENABLE_SIGN)
SQISIGN_API
int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk)
{

    return sqisign_keypair(pk, sk);
}

SQISIGN_API
int
crypto_sign(unsigned char *sm, unsigned long long *smlen,
            const unsigned char *m, unsigned long long mlen,
            const unsigned char *sk)
{
    return sqisign_sign(sm, smlen, m, mlen, sk);
}
#endif

SQISIGN_API
int
crypto_sign_open(unsigned char *m, unsigned long long *mlen,
                 const unsigned char *sm, unsigned long long smlen,
                 const unsigned char *pk)
{
    return sqisign_open(m, mlen, sm, smlen, pk);
}
