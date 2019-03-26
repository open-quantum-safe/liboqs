#ifndef FIPS202_H
#define FIPS202_H

#include <oqs/sha3.h>

#define SHAKE128_RATE OQS_SHA3_SHAKE128_RATE
#define SHAKE256_RATE OQS_SHA3_SHAKE256_RATE
#define SHA3_256_RATE OQS_SHA3_SHA3_256_RATE
#define SHA3_512_RATE OQS_SHA3_SHA3_512_RATE

#define shake128_absorb OQS_SHA3_shake128_absorb
#define shake128_squeezeblocks OQS_SHA3_shake128_squeezeblocks
#define shake256_absorb OQS_SHA3_shake256_absorb
#define shake256_squeezeblocks OQS_SHA3_shake256_squeezeblocks
#define shake128 OQS_SHA3_shake128
#define shake256 OQS_SHA3_shake256

#define sha3_256 OQS_SHA3_sha3256
#define sha3_512 OQS_SHA3_sha3512

#endif
