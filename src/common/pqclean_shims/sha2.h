#ifndef SHA2_H
#define SHA2_H

#include <oqs/sha2.h>

/* The incremental API allows hashing of individual input blocks; these blocks
   must be exactly 64 bytes each.
   Use the 'finalize' functions for any remaining bytes (possibly over 64). */

#define sha224_inc_init OQS_SHA2_sha224_inc_init
#define sha224_inc_blocks OQS_SHA2_sha224_inc_blocks
#define sha224_inc_finalize OQS_SHA2_sha224_inc_finalize
#define sha224 OQS_SHA2_sha224

#define sha256_inc_init OQS_SHA2_sha256_inc_init
#define sha256_inc_blocks OQS_SHA2_sha256_inc_blocks
#define sha256_inc_finalize OQS_SHA2_sha256_inc_finalize
#define sha256 OQS_SHA2_sha256

#define sha384_inc_init OQS_SHA2_sha384_inc_init
#define sha384_inc_blocks OQS_SHA2_sha384_inc_blocks
#define sha384_inc_finalize OQS_SHA2_sha384_inc_finalize
#define sha384 OQS_SHA2_sha384

#define sha512_inc_init OQS_SHA2_sha512_inc_init
#define sha512_inc_blocks OQS_SHA2_sha512_inc_blocks
#define sha512_inc_finalize OQS_SHA2_sha512_inc_finalize
#define sha512 OQS_SHA2_sha512

#endif
