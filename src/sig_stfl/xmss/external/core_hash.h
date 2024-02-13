// SPDX-License-Identifier: (Apache-2.0 OR MIT) AND CC0-1.0
#ifndef CORE_HASH
#define CORE_HASH

#include "namespace.h"
#include "params.h"

// N = 24
#define XMSS_CORE_HASH_SHA256_N24   1
#define XMSS_CORE_HASH_SHAKE256_N24 2
// N = 32
#define XMSS_CORE_HASH_SHA256_N32   3
#define XMSS_CORE_HASH_SHAKE128_N32 4
#define XMSS_CORE_HASH_SHAKE256_N32 5
// N = 64
#define XMSS_CORE_HASH_SHA512_N64   6
#define XMSS_CORE_HASH_SHAKE256_N64 7

#define core_hash XMSS_PARAMS_INNER_CORE_HASH(core_hash)
int core_hash(const xmss_params *params,
              unsigned char *out,
              const unsigned char *in, unsigned long long inlen);

#endif
