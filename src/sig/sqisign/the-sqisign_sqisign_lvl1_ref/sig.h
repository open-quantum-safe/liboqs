// SPDX-License-Identifier: Apache-2.0

#ifndef SQISIGN_H
#define SQISIGN_H

#include <stdint.h>
#include <sqisign_namespace.h>

#if defined(ENABLE_SIGN)
/**
 * SQIsign keypair generation.
 *
 * The implementation corresponds to SQIsign.CompactKeyGen() in the SQIsign spec.
 * The caller is responsible to allocate sufficient memory to hold pk and sk.
 *
 * @param[out] pk SQIsign public key
 * @param[out] sk SQIsign secret key
 * @return int status code
 */
SQISIGN_API 
int sqisign_keypair(unsigned char *pk, unsigned char *sk);

/**
 * SQIsign signature generation.
 *
 * The implementation performs SQIsign.expandSK() + SQIsign.sign() in the SQIsign spec.
 * Keys provided is a compacted secret keys.
 * The caller is responsible to allocate sufficient memory to hold sm.
 *
 * @param[out] sm Signature concatenated with message
 * @param[out] smlen Pointer to the length of sm
 * @param[in] m Message to be signed
 * @param[in] mlen Message length
 * @param[in] sk Compacted secret key
 * @return int status code
 */
SQISIGN_API 
int sqisign_sign(unsigned char *sm,
                 unsigned long long *smlen,
                 const unsigned char *m,
                 unsigned long long mlen,
                 const unsigned char *sk);
#endif

/**
 * SQIsign open signature.
 *
 * The implementation performs SQIsign.verify(). If the signature verification succeeded, the
 * original message is stored in m. Keys provided is a compact public key. The caller is responsible
 * to allocate sufficient memory to hold m.
 *
 * @param[out] m Message stored if verification succeeds
 * @param[out] mlen Pointer to the length of m
 * @param[in] sm Signature concatenated with message
 * @param[in] smlen Length of sm
 * @param[in] pk Compacted public key
 * @return int status code
 */
SQISIGN_API
int sqisign_open(unsigned char *m,
                 unsigned long long *mlen,
                 const unsigned char *sm,
                 unsigned long long smlen,
                 const unsigned char *pk);

/**
 * SQIsign verify signature.
 *
 * If the signature verification succeeded, returns 0, otherwise 1.
 *
 * @param[out] m Message stored if verification succeeds
 * @param[out] mlen Pointer to the length of m
 * @param[in] sig Signature
 * @param[in] siglen Length of sig
 * @param[in] pk Compacted public key
 * @return int 0 if verification succeeded, 1 otherwise.
 */
SQISIGN_API 
int sqisign_verify(const unsigned char *m,
                   unsigned long long mlen,
                   const unsigned char *sig,
                   unsigned long long siglen,
                   const unsigned char *pk);

#endif
