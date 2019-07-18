#ifndef SHA2_H
#define SHA2_H

#include <oqs/sha2.h>

#define sha256 OQS_SHA2_sha256
#define sha384 OQS_SHA2_sha384
#define sha512 OQS_SHA2_sha512

typedef void * sha256ctx;

#define sha256_inc_init OQS_SHA2_sha256_inc_init
#define sha256_inc_blocks OQS_SHA2_sha256_inc_blocks
#define sha256_inc_finalize OQS_SHA2_sha256_inc_finalize

#endif
