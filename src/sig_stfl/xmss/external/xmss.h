#ifndef XMSS_H
#define XMSS_H

#include <stdint.h>
#include "namespace.h"

/**
 * Generates a XMSS key pair for a given parameter set.
 * Format sk: [OID || (32bit) idx || SK_SEED || SK_PRF || PUB_SEED || root]
 * Format pk: [OID || root || PUB_SEED]
 */
#define xmss_keypair XMSS_NAMESPACE(xmss_keypair)
int xmss_keypair(unsigned char *pk, unsigned char *sk, const uint32_t oid);

/**
 * Signs a message using an XMSS secret key.
 * Returns
 * 1. an array containing the signature followed by the message AND
 * 2. an updated secret key!
 */
#define xmss_sign XMSS_NAMESPACE(xmss_sign)
int xmss_sign(unsigned char *sk,
              unsigned char *sm, unsigned long long *smlen,
              const unsigned char *m, unsigned long long mlen);

/**
 * Verifies a given message signature pair using a given public key.
 *
 * Note: m and mlen are pure outputs which carry the message in case
 * verification succeeds. The (input) message is assumed to be contained in sm
 * which has the form [signature || message].
 */
#define xmss_sign_open XMSS_NAMESPACE(xmss_sign_open)
int xmss_sign_open(const unsigned char *m, unsigned long long mlen,
                   const unsigned char *sm, unsigned long long smlen,
                   const unsigned char *pk);

/* 
 * Write number of remaining signature to `remain` variable given `sk`
 */
#define xmss_remaining_signatures XMSS_NAMESPACE(xmss_remaining_signatures)
int xmss_remaining_signatures(unsigned long long *remain, const unsigned  char *sk);

/*
 * Write number of maximum signature to `max` variable given `sk`
 */
#define xmss_total_signatures XMSS_NAMESPACE(xmss_total_signatures)
int xmss_total_signatures(unsigned long long *max, const unsigned  char *sk);

/*
 * Generates a XMSSMT key pair for a given parameter set.
 * Format sk: [OID || (ceil(h/8) bit) idx || SK_SEED || SK_PRF || PUB_SEED || root]
 * Format pk: [OID || root || PUB_SEED]
 */
#define xmssmt_keypair XMSS_NAMESPACE(xmssmt_keypair)
int xmssmt_keypair(unsigned char *pk, unsigned char *sk, const uint32_t oid);

/**
 * Signs a message using an XMSSMT secret key.
 * Returns
 * 1. an array containing the signature followed by the message AND
 * 2. an updated secret key!
 */
#define xmssmt_sign XMSS_NAMESPACE(xmssmt_sign)
int xmssmt_sign(unsigned char *sk,
                unsigned char *sm, unsigned long long *smlen,
                const unsigned char *m, unsigned long long mlen);

/**
 * Verifies a given message signature pair using a given public key.
 *
 * Note: m and mlen are pure outputs which carry the message in case
 * verification succeeds. The (input) message is assumed to be contained in sm
 * which has the form [signature || message].
 */
#define xmssmt_sign_open XMSS_NAMESPACE(xmssmt_sign_open)
int xmssmt_sign_open(const unsigned char *m, unsigned long long mlen,
                     const unsigned char *sm, unsigned long long smlen,
                     const unsigned char *pk);

/* 
 * Write number of remaining signature to `remain` variable given `sk`
 */
#define xmssmt_remaining_signatures XMSS_NAMESPACE(xmssmt_remaining_signatures)
int xmssmt_remaining_signatures(unsigned long long *remain, const unsigned  char *sk);

/*
 * Write number of maximum signature to `max` variable given `sk`
 */
#define xmssmt_total_signatures XMSS_NAMESPACE(xmssmt_total_signatures)
int xmssmt_total_signatures(unsigned long long *max, const unsigned  char *sk);

#endif
