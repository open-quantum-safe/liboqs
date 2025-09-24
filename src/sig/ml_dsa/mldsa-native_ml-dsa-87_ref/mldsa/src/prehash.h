/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

#ifndef MLD_PREHASH_H
#define MLD_PREHASH_H

#include <stddef.h>
#include <stdint.h>
#include "common.h"
#include "sign.h"

/* Maximum formatted pre-hash message length: 0x01 || ctxlen || ctx (max 255) ||
 * oid (11) || ph (max 64) */
#define MLD_PRE_HASH_MAX_FORMATTED_MESSAGE_BYTES (2 + 255 + 11 + 64)

#define mld_validate_hash_length MLD_NAMESPACE(validate_hash_length)
/*************************************************
 * Name:        mld_validate_hash_length
 *
 * Description: Validates that the given hash length matches the expected
 *              length for the given hash algorithm.
 *
 * Arguments:   - int hashalg: hash algorithm constant (MLD_PREHASH_*)
 *              - size_t len: Hash length to be checked
 *
 * Returns 0 if hash algorithm is known and the hash length matches
 * and -1 otherwise.
 **************************************************/
MLD_MUST_CHECK_RETURN_VALUE
MLD_INTERNAL_API
int mld_validate_hash_length(int hashalg, size_t len);

#define mld_format_pre_hash_message MLD_NAMESPACE(format_pre_hash_message)
/*************************************************
 * Name:        mld_format_pre_hash_message
 *
 * Description: Formats a pre-hash message according to FIPS 204:
 *              0x01 || ctxlen (1 byte) || ctx || oid || ph
 *
 * Arguments:   - uint8_t fmsg[MLD_PRE_HASH_MAX_FORMATTED_MESSAGE_BYTES]:
 *                output formatted message buffer
 *              - const uint8_t *ph: pointer to pre-hashed message
 *              - size_t phlen: length of pre-hashed message
 *              - const uint8_t *ctx: pointer to context string (may be NULL)
 *              - size_t ctxlen: length of context string
 *              - int hashalg: hash algorithm constant (MLD_PREHASH_*)
 *
 * Returns the total length of the formatted message (2 + ctxlen + 11 + phlen).
 **************************************************/
MLD_INTERNAL_API
size_t mld_format_pre_hash_message(
    uint8_t fmsg[MLD_PRE_HASH_MAX_FORMATTED_MESSAGE_BYTES], const uint8_t *ph,
    size_t phlen, const uint8_t *ctx, size_t ctxlen, int hashalg);

#endif /* !MLD_PREHASH_H */
