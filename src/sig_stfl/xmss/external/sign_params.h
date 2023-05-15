#ifndef NIST_PARAM_H
#define NIST_PARAM_H

#include "params.h"
#include "xmss.h"

#ifndef TREE_LEVEL
#define TREE_LEVEL 0
#endif

#ifndef XMSSMT
#define XMSSMT 0
#endif

#if XMSSMT == 0
    /* 
    * Maximum signatures: 2^h = 2^10
    */
    #if TREE_LEVEL == 0

    #define XMSS_OID "XMSS-SHA2_10_256"

    #define XMSS_PUBLICKEYBYTES 64
    #define XMSS_SECRETKEYBYTES_SMALL 132
    #define XMSS_SECRETKEYBYTES 1373

    #define XMSS_SIGNBYTES 2500

    /* 
    * Maximum signatures: 2^h = 2^16
    */
    #elif TREE_LEVEL == 1

    #define XMSS_OID "XMSS-SHA2_16_256"

    #define XMSS_PUBLICKEYBYTES 64
    #define XMSS_SECRETKEYBYTES_SMALL 132
    #define XMSS_SECRETKEYBYTES 2093

    #define XMSS_SIGNBYTES 2692

    /* 
    * Maximum signatures: 2^h = 2^20
    */
    #elif TREE_LEVEL == 2

    #define XMSS_OID "XMSS-SHA2_20_256"

    #define XMSS_PUBLICKEYBYTES 64
    #define XMSS_SECRETKEYBYTES_SMALL 132
    #define XMSS_SECRETKEYBYTES 2573

    #define XMSS_SIGNBYTES 2820


    #else

    #error "Unspecified TREE_LEVEL {0,1,2}"

    #endif
#else 
    /* 
    * Maximum signatures: 2^h = 2^20
    * XMSS^MT has bigger signature and secret key (secret is not transfer), but better speed
    */
    #if TREE_LEVEL == 0

    #define XMSS_OID "XMSSMT-SHA2_20/2_256"

    #define XMSS_PUBLICKEYBYTES 64
    #define XMSS_SECRETKEYBYTES_SMALL 131
    #define XMSS_SECRETKEYBYTES 5998

    #define XMSS_SIGNBYTES 4963

    /* 
    * Maximum signatures: 2^h = 2^40
    * XMSS^MT has bigger signature and secret key (secret is not transfer), but better speed
    */
    #elif TREE_LEVEL == 1

    #define XMSS_OID "XMSSMT-SHA2_40/2_256"

    #define XMSS_PUBLICKEYBYTES 64
    #define XMSS_SECRETKEYBYTES_SMALL 133
    #define XMSS_SECRETKEYBYTES 9600

    #define XMSS_SIGNBYTES 5605

    /* 
    * Maximum signatures: 2^h = 2^60
    * XMSS^MT has bigger signature and secret key (secret is not transfer), but better speed
    */
    #elif TREE_LEVEL == 2

    #define XMSS_OID "XMSSMT-SHA2_60/3_256"

    #define XMSS_PUBLICKEYBYTES 64
    #define XMSS_SECRETKEYBYTES_SMALL 136
    #define XMSS_SECRETKEYBYTES 16629

    #define XMSS_SIGNBYTES 8392


    #else

    #error "Unspecified TREE_LEVEL {0,1,2}"

    #endif

#endif

#if XMSSMT == 1
    #define XMSS_PARSE_OID xmssmt_parse_oid
    #define XMSS_STR_TO_OID xmssmt_str_to_oid
    #define XMSS_KEYPAIR xmssmt_keypair
    #define XMSS_SIGN xmssmt_sign
    #define XMSS_SIGN_OPEN xmssmt_sign_open
    #define XMSS_REMAINING_SIG xmssmt_remaining_signatures
#else
    #define XMSS_PARSE_OID xmss_parse_oid
    #define XMSS_STR_TO_OID xmss_str_to_oid
    #define XMSS_KEYPAIR xmss_keypair
    #define XMSS_SIGN xmss_sign
    #define XMSS_SIGN_OPEN xmss_sign_open
    #define XMSS_REMAINING_SIG xmss_remaining_signatures
#endif

#if XMSS_SECRETKEYBYTES_SMALL_ENABLE
#define CRYPTO_SECRET_KEY (XMSS_SECRETKEYBYTES_SMALL + XMSS_OID_LEN)
#else
#define CRYPTO_SECRET_KEY (XMSS_SECRETKEYBYTES + XMSS_OID_LEN)
#endif

#define CRYPTO_PUBLIC_KEY (XMSS_PUBLICKEYBYTES + XMSS_OID_LEN)
#define CRYPTO_BYTES XMSS_SIGNBYTES

#endif
