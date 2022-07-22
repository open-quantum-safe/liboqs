#ifndef XMSS_CORE_H
#define XMSS_CORE_H

#include "params.h"
#include "../../sig_stfl.h"

/**
 * Given a set of parameters, this function returns the size of the secret key.
 * This is implementation specific, as varying choices in tree traversal will
 * result in varying requirements for state storage.
 *
 * This function handles both XMSS and XMSSMT parameter sets.
 */
unsigned long long xmss_xmssmt_core_sk_bytes(const xmss_params *params);

/**
 * @brief Increments the secret key by a certain number by a certain 
 * number of signatures. 
 * @param params parameters for the XMSS variant 
 * @param sk secret key byte array (without the OID)
 * @param amount number of times to increment the secret key auth path
 * @return int - Returns 0 on success, -1 on failure.
 */
int 
xmss_core_increment_authpath(const xmss_params *params,
                          uint8_t *sk,
                          unsigned long long amount);

/*
 * Generates a XMSS key pair for a given parameter set.
 * Format sk: [(32bit) index || SK_SEED || SK_PRF || PUB_SEED || root]
 * Format pk: [root || PUB_SEED], omitting algorithm OID.
 */
int xmss_core_keypair(const xmss_params *params,
                      uint8_t *pk, uint8_t *sk);

/**
 * Signs a message. Returns an array containing the signature followed by the
 * message and an updated secret key.
 */
int xmss_core_sign(const xmss_params *params,
                   OQS_SECRET_KEY *secret_key,
                   uint8_t *sm, unsigned long long *smlen,
                   const uint8_t *m, unsigned long long mlen);

/**
 * Verifies a given message signature pair under a given public key.
 * Note that this assumes a pk without an OID, i.e. [root || PUB_SEED]
 */
int xmss_core_sign_open(const xmss_params *params,
                        uint8_t *m, unsigned long long *mlen,
                        const uint8_t *sm, unsigned long long smlen,
                        const uint8_t *pk);

/**
 * @brief Increments the secret key by a certain number by a certain 
 * number of signatures. 
 * @param params parameters for the XMSS^MT variant 
 * @param sk secret key byte array (without the OID)
 * @param amount number of times to increment the secret key auth path
 * @return int - Returns 0 on success, -1 on failure.
 */
int 
xmssmt_core_increment_authpath(const xmss_params *params,
                               uint8_t *sk,
                               unsigned long long amount);

/*
 * Generates a XMSSMT key pair for a given parameter set.
 * Format sk: [(ceil(h/8) bit) index || SK_SEED || SK_PRF || PUB_SEED || root]
 * Format pk: [root || PUB_SEED] omitting algorithm OID.
 */
int xmssmt_core_keypair(const xmss_params *params,
                        uint8_t *pk, uint8_t *sk);

/**
 * Signs a message. Returns an array containing the signature followed by the
 * message and an updated secret key.
 */
int xmssmt_core_sign(const xmss_params *params,
                     OQS_SECRET_KEY *secret_key,
                     uint8_t *sm, unsigned long long *smlen,
                     const uint8_t *m, unsigned long long mlen);

/**
 * Verifies a given message signature pair under a given public key.
 * Note that this assumes a pk without an OID, i.e. [root || PUB_SEED]
 */
int xmssmt_core_sign_open(const xmss_params *params,
                          uint8_t *m, unsigned long long *mlen,
                          const uint8_t *sm, unsigned long long smlen,
                          const uint8_t *pk);

#endif
