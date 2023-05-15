/*=============================================================================
 * Copyright (c) 2022 by SandboxAQ Inc
 * Author: Duc Tri Nguyen (ductri.nguyen@sandboxaq.com)
 * SPDX-License-Identifier: MIT
=============================================================================*/
#include <oqs/common.h>
#include <stdio.h>

#include "sign.h"
#include "sign_params.h"

/*************************************************
 * Name:        XMSS_crypto_sign_keypair
 *
 * Description: Generates public and private key.
 *
 * Arguments:   - uint8_t *pk: pointer to output public key (allocated
 *                             array of CRYPTO_PUBLIC_KEY bytes)
 *              - uint8_t *sk: pointer to output private key (allocated
 *                             array of CRYPTO_SECRET_KEY bytes)
 *
 * Returns 0 (success), -1 otherwise
 **************************************************/
int crypto_sign_keypair(unsigned char *pk, unsigned char *sk)
{
    xmss_params params;
    uint32_t oid;
    int ret = 0;

    ret |= XMSS_STR_TO_OID(&oid, XMSS_OID);
    if (ret)
    {
        return OQS_ERROR;
    }

    ret |= XMSS_PARSE_OID(&params, oid);
    if (ret)
    {
        return OQS_ERROR;
    }

    // TODO: set OID directly here
    ret |= XMSS_KEYPAIR(pk, sk, oid);
    if (ret)
    {
        return OQS_ERROR;
    }

    return OQS_SUCCESS;
}

/*************************************************
 * Name:        XMSS_crypto_sign
 *
 * Description: Computes signature.
 *
 * Arguments:   - uint8_t *sm:   pointer to output signature (of length CRYPTO_BYTES)
 *              - uint64_t *smlen: pointer to output length of signature
 *              - uint8_t *m:     pointer to message to be signed
 *              - uint64_t mlen:    length of message
 *              - uint8_t *sk:    pointer to bit-packed secret key
 *
 * Returns 0 (success), -1 otherwise
 **************************************************/
int crypto_sign(unsigned char *sm, unsigned long long *smlen,
                const unsigned char *m, unsigned long long mlen, unsigned char *sk)
{
    int ret = XMSS_SIGN(sk, sm, smlen, m, mlen);
    if (ret)
    {
        return OQS_ERROR;
    }

    return OQS_SUCCESS;
}

/*************************************************
 * Name:        XMSS_crypto_sign_open
 *
 * Description: Verify signed message.
 *
 * Arguments:
 *              - uint8_t *m: pointer to output message (allocated
 *                            array with smlen bytes), can be equal to sm
 *              - uint64_t *mlen: pointer to output length of message
 *              - uint8_t *sm: pointer to signed message
 *              - uint64_t smlen: length of signed message
 *              - uint8_t *pk: pointer to bit-packed public key
 *
 * Returns 0 if signed message could be verified correctly and -1 otherwise
 **************************************************/
int crypto_sign_open(const unsigned char *m, unsigned long long mlen,
                     const unsigned char *sm, unsigned long long smlen, const unsigned char *pk)
{
    if (XMSS_SIGN_OPEN(m, mlen, sm, smlen, pk))
    {
        return OQS_ERROR;
    }

    return OQS_SUCCESS;
}

/*************************************************
 * Name:        XMSS_crypto_remaining_signatures
 *
 * Description: Return number of signatures left
 *
 * Arguments:   - uint64_t *remain: remaining signatures
 *              - uint8_t *sk: pointer to bit-packed private key
 *
 * Returns 0 (sucess), -1 otherwise
 **************************************************/
int crypto_remaining_signatures(unsigned long long *remain, const unsigned char *sk)
{
    if (XMSS_REMAINING_SIG(remain, sk))
    {
        return OQS_ERROR;
    }
    return OQS_SUCCESS;
}
