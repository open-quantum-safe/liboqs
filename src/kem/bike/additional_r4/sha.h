/* Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0"
 *
 * Written by Nir Drucker and Shay Gueron
 * AWS Cryptographic Algorithms Group.
 * (ndrucker@amazon.com, gueron@amazon.com)
 */

#pragma once

#include "cleanup.h"
#include "types.h"
#include "utilities.h"
#include <oqs/sha3.h>

#define SHA384_HASH_SIZE   48ULL
#define SHA384_HASH_QWORDS (SHA384_HASH_SIZE / 8)

typedef struct {
	union {
		uint8_t  raw[SHA384_HASH_SIZE];
		uint64_t qw[SHA384_HASH_QWORDS];
	} u;
} sha_dgst_t;

_INLINE_ void
sha_dgst_cleanup(IN OUT sha_dgst_t *o) {
	secure_clean(o->u.raw, sizeof(*o));
}

_INLINE_ int
sha(OUT sha_dgst_t *hash_out, IN const uint32_t byte_len, IN const uint8_t *msg) {
	OQS_SHA3_sha3_384(hash_out->u.raw, msg, byte_len);
	return SUCCESS;
}
