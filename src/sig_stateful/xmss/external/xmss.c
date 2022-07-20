#include <stdint.h>
#include <stdio.h>
#include <math.h>

#include "params.h"
#include "utils.h"
#include "../../sig_stfl.h"
#include "xmss_core.h"

/* This file provides wrapper functions that take keys that include OIDs to
identify the parameter set to be used. After setting the parameters accordingly
it falls back to the regular XMSS core functions. */

int xmss_keypair(uint8_t *pk, OQS_SECRET_KEY *sk, const uint32_t oid)
{
    xmss_params params;
    unsigned int i;

    if (xmss_parse_oid(&params, oid)) {
        return -1;
    }
    for (i = 0; i < XMSS_OID_LEN; i++) {
        pk[XMSS_OID_LEN - i - 1] = (oid >> (8 * i)) & 0xFF;
        /* For an implementation that uses runtime parameters, it is crucial
        that the OID is part of the secret key as well;
        i.e. not just for interoperability, but also for internal use. */
        sk->secret_key[XMSS_OID_LEN - i - 1] = (oid >> (8 * i)) & 0xFF;
    }
    int ret = xmss_core_keypair(&params, pk + XMSS_OID_LEN, sk->secret_key + XMSS_OID_LEN);
    if (ret != 0) return ret;

    unsigned long long max = pow(2, params.tree_height) - (unsigned long) 1;
    ull_to_bytes(sk->secret_key + sk->length_secret_key - params.bytes_for_max, params.bytes_for_max, max);
    
    return ret;
}

#ifdef MAX_MOD

int xmss_modify_maximum(OQS_SECRET_KEY *sk, unsigned long long new_max) {
    
    xmss_params params;
    unsigned int i;

    if (xmss_parse_oid(&params, sk->oid)) {
        return -1;
    }

    for (i = params.sk_bytes - params.bytes_for_max; i < params.sk_bytes; i++) {
        sk->secret_key[i] = (new_max >> (8 * (params.sk_bytes - i - 1))) & 0xFF;

        #ifdef DEBUGGING
        printf("%02x\n", (new_max >> (8 * (params.sk_bytes - i - 1))) & 0xFF);
        #endif
    }
    printf("\n");
    return 0;
}

#endif

int xmss_sign(OQS_SECRET_KEY *sk,
              uint8_t *sm, unsigned long long *smlen,
              const uint8_t *m, unsigned long long mlen)
{
    xmss_params params;
    uint32_t oid = sk->oid;
    if (xmss_parse_oid(&params, oid)) {
        return -1;
    }
    return xmss_core_sign(&params, sk, sm, smlen, m, mlen);
}

int xmss_sign_open(uint8_t *m, unsigned long long *mlen,
                   const uint8_t *sm, unsigned long long smlen,
                   const uint8_t *pk)
{
    xmss_params params;
    uint32_t oid = 0;
    unsigned int i;

    for (i = 0; i < XMSS_OID_LEN; i++) {
        oid |= pk[XMSS_OID_LEN - i - 1] << (i * 8);
    }
    if (xmss_parse_oid(&params, oid)) {
        return -1;
    }
    return xmss_core_sign_open(&params, m, mlen, sm, smlen, pk + XMSS_OID_LEN);
}

int xmssmt_keypair(uint8_t *pk, OQS_SECRET_KEY *sk, const uint32_t oid)
{
    xmss_params params;
    unsigned int i;

    if (xmssmt_parse_oid(&params, oid)) {
        return -1;
    }
    for (i = 0; i < XMSS_OID_LEN; i++) {
        pk[XMSS_OID_LEN - i - 1] = (oid >> (8 * i)) & 0xFF;
        sk->secret_key[XMSS_OID_LEN - i - 1] = (oid >> (8 * i)) & 0xFF;
    }
    return xmssmt_core_keypair(&params, pk + XMSS_OID_LEN, sk->secret_key + XMSS_OID_LEN);
}

int xmssmt_sign(OQS_SECRET_KEY *sk,
                uint8_t *sm, unsigned long long *smlen,
                const uint8_t *m, unsigned long long mlen)
{
    xmss_params params;
    uint32_t oid = 0;
    unsigned int i;

    for (i = 0; i < XMSS_OID_LEN; i++) {
        oid |= sk->secret_key[XMSS_OID_LEN - i - 1] << (i * 8);
    }
    if (xmssmt_parse_oid(&params, oid)) {
        return -1;
    }
    return xmssmt_core_sign(&params, sk, sm, smlen, m, mlen);
}

int xmssmt_sign_open(uint8_t *m, unsigned long long *mlen,
                     const uint8_t *sm, unsigned long long smlen,
                     const uint8_t *pk)
{
    xmss_params params;
    uint32_t oid = 0;
    unsigned int i;

    for (i = 0; i < XMSS_OID_LEN; i++) {
        oid |= pk[XMSS_OID_LEN - i - 1] << (i * 8);
    }
    if (xmssmt_parse_oid(&params, oid)) {
        return -1;
    }
    return xmssmt_core_sign_open(&params, m, mlen, sm, smlen, pk + XMSS_OID_LEN);
}
