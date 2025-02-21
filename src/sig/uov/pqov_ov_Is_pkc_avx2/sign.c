// SPDX-License-Identifier: CC0 OR Apache-2.0
///  @file  sign.c
///  @brief the implementations for functions in api.h
///
///
#include <stdlib.h>
#include <string.h>

#include "params.h"
#include "ov_keypair.h"
#include "ov.h"

#include "api.h"

#include "utils_prng.h"

#if defined(_UTILS_SUPERCOP_)
#include "crypto_sign.h"
#endif


#if defined(_VALGRIND_)
#include "valgrind/memcheck.h"
#endif

int
crypto_sign_keypair(unsigned char *pk, unsigned char *sk) {
    unsigned char sk_seed[LEN_SKSEED];
    randombytes( sk_seed, LEN_SKSEED );

    #if defined(_VALGRIND_)
    VALGRIND_MAKE_MEM_UNDEFINED(sk_seed, LEN_SKSEED );  // mark secret data as undefined data
    #endif

    #if defined _OV_CLASSIC
    int r = generate_keypair( (pk_t *) pk, (sk_t *) sk, sk_seed );
    #elif defined _OV_PKC
    int r = generate_keypair_pkc( (cpk_t *) pk, (sk_t *) sk, sk_seed );
    #elif defined _OV_PKC_SKC
    int r = generate_keypair_pkc_skc( (cpk_t *) pk, (csk_t *) sk, sk_seed );
    #else
    error here
    #endif

    #if defined(_VALGRIND_)
    VALGRIND_MAKE_MEM_DEFINED(pk, OV_PUBLICKEYBYTES );  // mark return value as public data
    VALGRIND_MAKE_MEM_DEFINED(&r, sizeof(int) );  // mark return value as public data
    #endif
    return r;
}

int
#if defined(PQM4) || defined(_UTILS_OQS_)
crypto_sign_signature(unsigned char *sig, size_t *siglen, const unsigned char *m, size_t mlen, const unsigned char *sk)
#else
crypto_sign_signature(unsigned char *sig, unsigned long long *siglen, const unsigned char *m, unsigned long long mlen, const unsigned char *sk)
#endif
{
    int r = -1;
    #if defined _OV_CLASSIC

    r = ov_sign( sig, (const sk_t *)sk, m, mlen );

    #elif defined _OV_PKC

    r = ov_sign( sig, (const sk_t *)sk, m, mlen );

    #elif defined _OV_PKC_SKC

    r = ov_expand_and_sign( sig, (const csk_t *)sk, m, mlen );
    #else
    error here
    #endif
    siglen[0] = OV_SIGNATUREBYTES;
    return r;
}




int
#if defined(PQM4) || defined(_UTILS_OQS_)
crypto_sign(unsigned char *sm, size_t *smlen, const unsigned char *m, size_t mlen, const unsigned char *sk)
#else
crypto_sign(unsigned char *sm, unsigned long long *smlen, const unsigned char *m, unsigned long long mlen, const unsigned char *sk)
#endif
{
    int r = crypto_sign_signature(sm + mlen, smlen, m, mlen, sk);

    memmove( sm, m, mlen );
    smlen[0] = mlen + OV_SIGNATUREBYTES;

    return r;
}




int
#if defined(PQM4) || defined(_UTILS_OQS_)
crypto_sign_verify(const unsigned char *sig, size_t siglen, const unsigned char *m, size_t mlen, const unsigned char *pk)
#else
crypto_sign_verify(const unsigned char *sig, unsigned long long siglen, const unsigned char *m, unsigned long long mlen, const unsigned char *pk)
#endif
{
    int r;
    if ( OV_SIGNATUREBYTES != siglen ) {
        return -1;
    }

    #if defined _OV_CLASSIC

    r = ov_verify( m, mlen, sig, (const pk_t *)pk );

    #elif defined _OV_PKC

    r = ov_expand_and_verify( m, mlen, sig, (const cpk_t *)pk );

    #elif defined _OV_PKC_SKC

    r = ov_expand_and_verify( m, mlen, sig, (const cpk_t *)pk );

    #else
    error here
    #endif

    return r;
}

int
#if defined(PQM4) || defined(_UTILS_OQS_)
crypto_sign_open(unsigned char *m, size_t *mlen, const unsigned char *sm, size_t smlen, const unsigned char *pk)
#else
crypto_sign_open(unsigned char *m, unsigned long long *mlen, const unsigned char *sm, unsigned long long smlen, const unsigned char *pk)
#endif
{
    unsigned i;
    if ( OV_SIGNATUREBYTES > smlen ) {
        goto badsig;
    }

    *mlen = smlen - OV_SIGNATUREBYTES;
    if(crypto_sign_verify(sm + *mlen, OV_SIGNATUREBYTES, sm, *mlen, pk)) {
      goto badsig;
    } else {
      /* All good, copy msg, return 0 */
      for(i = 0; i < *mlen; ++i)
        m[i] = sm[i];
      return 0;
    }

badsig:
    /* Signature verification failed */
    *mlen = 0;
    // NOTE: This is not a mistake
    // The NIST API requires the message buffer to be at least smlen bytes
    for(i = 0; i < smlen; ++i) {
      m[i] = 0;
    }

    return -1;
}

