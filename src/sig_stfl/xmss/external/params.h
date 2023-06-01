#ifndef XMSS_PARAMS_H
#define XMSS_PARAMS_H

#include <stdint.h>
#include "namespace.h"

/* These are merely internal identifiers for the supported hash functions. */
#define XMSS_SHA2 0
#define XMSS_SHAKE128 1
#define XMSS_SHAKE256 2

/* This is a result of the OID definitions in the draft; needed for parsing. */
#define XMSS_OID_LEN 4

/* This structure will be populated when calling xmss[mt]_parse_oid. */
typedef struct {
    unsigned int func;
    unsigned int n;
    unsigned int padding_len;
    unsigned int wots_w;
    unsigned int wots_log_w;
    unsigned int wots_len1;
    unsigned int wots_len2;
    unsigned int wots_len;
    unsigned int wots_sig_bytes;
    unsigned int full_height;
    unsigned int tree_height;
    unsigned int d;
    unsigned int index_bytes;
    unsigned int sig_bytes;
    unsigned int pk_bytes;
    unsigned long long sk_bytes;
    unsigned int bds_k;
} xmss_params;

/**
 * Accepts strings such as "XMSS-SHA2_10_256"
 *  and outputs OIDs such as 0x01000001.
 * Returns -1 when the parameter set is not found, 0 otherwise
 */
#define xmss_str_to_oid XMSS_NAMESPACE(xmss_str_to_oid)
int xmss_str_to_oid(uint32_t *oid, const char *s);

/**
 * Accepts takes strings such as "XMSSMT-SHA2_20/2_256"
 *  and outputs OIDs such as 0x01000001.
 * Returns -1 when the parameter set is not found, 0 otherwise
 */
#define xmssmt_str_to_oid XMSS_NAMESPACE(xmssmt_str_to_oid)
int xmssmt_str_to_oid(uint32_t *oid, const char *s);

/**
 * Accepts OIDs such as 0x01000001, and configures params accordingly.
 * Returns -1 when the OID is not found, 0 otherwise.
 */
#define xmss_parse_oid XMSS_NAMESPACE(xmss_parse_oid)
int xmss_parse_oid(xmss_params *params, const uint32_t oid);

/**
 * Accepts OIDs such as 0x01000001, and configures params accordingly.
 * Returns -1 when the OID is not found, 0 otherwise.
 */
#define xmssmt_parse_oid XMSS_NAMESPACE(xmssmt_parse_oid)
int xmssmt_parse_oid(xmss_params *params, const uint32_t oid);


/* Given a params struct where the following properties have been initialized;
    - full_height; the height of the complete (hyper)tree
    - n; the number of bytes of hash function output
    - d; the number of layers (d > 1 implies XMSSMT)
    - func; one of {XMSS_SHA2, XMSS_SHAKE128, XMSS_SHAKE256}
    - wots_w; the Winternitz parameter
    - optionally, bds_k; the BDS traversal trade-off parameter,
    this function initializes the remainder of the params structure. */
#define xmss_xmssmt_initialize_params XMSS_NAMESPACE(xmss_xmssmt_initialize_params)
int xmss_xmssmt_initialize_params(xmss_params *params);

#endif
