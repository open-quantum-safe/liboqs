// SPDX-License-Identifier: Apache-2.0

#include <stddef.h>
#include <sig.h>

#define oqs_sign_signature SQISIGN_NAMESPACE(oqs_sign_signature)
#define oqs_verify SQISIGN_NAMESPACE(oqs_verify)

SQISIGN_API
int
oqs_sign_signature(unsigned char *sig, size_t *siglen, const unsigned char *m, size_t mlen, const unsigned char *sk)
{
    unsigned long long siglen_ull = 0;
    int ret = sqisign_sign_signature(sig, &siglen_ull, m, mlen, sk);
    *siglen = (size_t)siglen_ull;
    return ret;
}

SQISIGN_API
int
oqs_verify(const unsigned char *sig, size_t siglen, const unsigned char *m, size_t mlen, const unsigned char *pk)
{
    return sqisign_verify(sig, siglen, m, mlen, pk);
}
