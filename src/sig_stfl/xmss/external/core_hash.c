// SPDX-License-Identifier: (Apache-2.0 OR MIT) AND CC0-1.0
#include <oqs/sha2.h>
#include <oqs/sha3.h>
#include "core_hash.h"
#include <string.h>

int core_hash(const xmss_params *params,
              unsigned char *out,
              const unsigned char *in, unsigned long long inlen) {

	(void)params;
#if HASH == XMSS_CORE_HASH_SHA256_N24
	unsigned char buf[32];
	OQS_SHA2_sha256(buf, in, inlen);
	memcpy(out, buf, 24);

#elif HASH == XMSS_CORE_HASH_SHAKE256_N24
	OQS_SHA3_shake256(out, 24, in, inlen);

#elif HASH == XMSS_CORE_HASH_SHA256_N32
	OQS_SHA2_sha256(out, in, inlen);

#elif HASH == XMSS_CORE_HASH_SHAKE128_N32
	OQS_SHA3_shake128(out, 32, in, inlen);

#elif HASH == XMSS_CORE_HASH_SHAKE256_N32
	OQS_SHA3_shake256(out, 32, in, inlen);

#elif HASH == XMSS_CORE_HASH_SHA512_N64
	OQS_SHA2_sha512(out, in, inlen);

#elif HASH == XMSS_CORE_HASH_SHAKE256_N64
	OQS_SHA3_shake256(out, 64, in, inlen);
#else
	return -1;
#endif

	return 0;
}
