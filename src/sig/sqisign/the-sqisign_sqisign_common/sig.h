// SPDX-License-Identifier: Apache-2.0

#ifndef SQISIGN_H
#define SQISIGN_H

#include <stdint.h>
#include <sqisign_namespace.h>

#if defined(ENABLE_SIGN)
/**
 * SQIsign keypair generation.
 *
 * The implementation corresponds to SQIsign.CompactKeyGen() in the SQIsign spec. The caller is responsible to allocate
 * sufficient memory to hold pk and sk.
 *
 * @param[out] pk SQIsign public key
 * @param[out] sk SQIsign secret key
 * @return 0 if success, -1 otherwise
 */
SQISIGN_API
int sqisign_keypair(unsigned char *pk, unsigned char *sk);

/**
 * SQIsign signature generation.
 *
 * The implementation performs SQIsign.expandSK() + SQIsign.sign() in the SQIsign spec. Keys provided is a compacted
 * secret keys. The caller is responsible to allocate sufficient memory to hold sm, that is, at least
 * SIGNATURE_BYTES + mlen bytes.
 *
 * smlen is an output parameter only: the value it points to on entry is ignored. On success it is set to
 * SIGNATURE_BYTES + mlen, and on failure to 0.
 *
 * @param[out] sm Signature concatenated with message
 * @param[out] smlen Pointer to the length of sm; on entry its value is ignored
 * @param[in] m Message to be signed
 * @param[in] mlen Message length
 * @param[in] sk Compacted secret key
 * @return 0 if success, -1 otherwise
 */
SQISIGN_API
int sqisign_sign(unsigned char *sm,
                 unsigned long long *smlen,
                 const unsigned char *m,
                 unsigned long long mlen,
                 const unsigned char *sk);

/**
 * SQIsign signature generation (signature only).
 *
 * Identical to sqisign_sign() but writes only the signature to sig and sets siglen to the signature length; the message
 * is not appended. The caller is responsible to allocate at least SIGNATURE_BYTES bytes to hold sig.
 *
 * As in sqisign_sign(), siglen is an output parameter only. On success it is set to SIGNATURE_BYTES, and on failure
 * to 0.
 *
 * @param[out] sig SQIsign signature
 * @param[out] siglen Pointer to the length of sig; on entry its value is ignored
 * @param[in] m Message to be signed
 * @param[in] mlen Message length
 * @param[in] sk Compacted secret key
 * @return 0 if success, -1 otherwise
 */
SQISIGN_API
int sqisign_sign_signature(unsigned char *sig,
                           unsigned long long *siglen,
                           const unsigned char *m,
                           unsigned long long mlen,
                           const unsigned char *sk);
#endif

/**
 * SQIsign open signature.
 *
 * The implementation performs SQIsign.verify(). If the signature verification succeeded, the original message is stored
 * in m. Keys provided is a compact public key.
 *
 * smlen is the combined length of the signature and the appended message, so it must be at least SIGNATURE_BYTES;
 * anything shorter is rejected with -1. The caller must provide smlen - SIGNATURE_BYTES bytes of space for m, which is
 * the length of the recovered message.
 *
 * @param[out] m Message stored if verification succeeds, zeroed otherwise
 * @param[out] mlen Pointer to the length of m; set to smlen - SIGNATURE_BYTES on success and to 0 on failure
 * @param[in] sm Signature concatenated with message
 * @param[in] smlen Length of sm; must be at least SIGNATURE_BYTES
 * @param[in] pk Compacted public key
 * @return 0 if verification succeeded, -1 otherwise
 */
SQISIGN_API
int sqisign_open(unsigned char *m,
                 unsigned long long *mlen,
                 const unsigned char *sm,
                 unsigned long long smlen,
                 const unsigned char *pk);

/**
 * SQIsign verify detached signature.
 *
 * Signatures have a fixed size, so siglen must equal SIGNATURE_BYTES exactly; anything else is rejected with -1.
 *
 * @param[in] sig Signature
 * @param[in] siglen Length of sig; must equal SIGNATURE_BYTES
 * @param[in] m Message
 * @param[in] mlen Length of m
 * @param[in] pk Compacted public key
 * @return 0 if verification succeeded, -1 otherwise
 */
SQISIGN_API
int sqisign_verify(const unsigned char *sig,
                   unsigned long long siglen,
                   const unsigned char *m,
                   unsigned long long mlen,
                   const unsigned char *pk);

#endif
