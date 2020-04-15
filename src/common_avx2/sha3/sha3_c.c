/**
* \file sha3_c.c
* \brief Implementation of the OQS SHA3 API via the files fips202.c
* from PQClean (https://github.com/PQClean/PQClean/tree/master/common)
*/

#include <oqs/oqs.h>

#include "sha3.h"

#define SHA3_256_RATE OQS_SHA3_SHA3_256_RATE
#ifndef OQS_USE_SHA3_OPENSSL
#define sha3_256 OQS_AVX2_SHA3_sha3_256
#else
#define sha3_256 oqs_avx2_unused_SHA3_sha3_256
#endif

typedef struct {
	uint64_t *ctx;
} sha3_256incctx;
#define sha3_256_inc_init oqs_avx2_sha3_sha3_256_inc_init
#define sha3_256_inc_absorb oqs_avx2_sha3_sha3_256_inc_absorb
#define sha3_256_inc_finalize oqs_avx2_sha3_sha3_256_inc_finalize
#define sha3_256_inc_ctx_release oqs_avx2_sha3_sha3_256_inc_ctx_release
#define sha3_256_inc_ctx_clone oqs_avx2_sha3_sha3_256_inc_ctx_clone

#define SHA3_384_RATE OQS_SHA3_SHA3_384_RATE
#ifndef OQS_USE_SHA3_OPENSSL
#define sha3_384 OQS_AVX2_SHA3_sha3_384
#else
#define sha3_384 oqs_avx2_unused_SHA3_sha3_384
#endif

typedef struct {
	uint64_t *ctx;
} sha3_384incctx;
#define sha3_384_inc_init oqs_avx2_sha3_sha3_384_inc_init
#define sha3_384_inc_absorb oqs_avx2_sha3_sha3_384_inc_absorb
#define sha3_384_inc_finalize oqs_avx2_sha3_sha3_384_inc_finalize
#define sha3_384_inc_ctx_release oqs_avx2_sha3_sha3_384_inc_ctx_release
#define sha3_384_inc_ctx_clone oqs_avx2_sha3_sha3_384_inc_ctx_clone

#define SHA3_512_RATE OQS_SHA3_SHA3_512_RATE
#ifndef OQS_USE_SHA3_OPENSSL
#define sha3_512 OQS_AVX2_SHA3_sha3_512
#else
#define sha3_512 oqs_avx2_unused_SHA3_sha3_512
#endif

typedef struct {
	uint64_t *ctx;
} sha3_512incctx;
#define sha3_512_inc_init oqs_avx2_sha3_sha3_512_inc_init
#define sha3_512_inc_absorb oqs_avx2_sha3_sha3_512_inc_absorb
#define sha3_512_inc_finalize oqs_avx2_sha3_sha3_512_inc_finalize
#define sha3_512_inc_ctx_release oqs_avx2_sha3_sha3_512_inc_ctx_release
#define sha3_512_inc_ctx_clone oqs_avx2_sha3_sha3_512_inc_ctx_clone

#define SHAKE128_RATE OQS_SHA3_SHAKE128_RATE
#ifndef OQS_USE_SHA3_OPENSSL
#define shake128 OQS_AVX2_SHA3_shake128
#else
#define shake128 oqs_avx2_unused_SHA3_shake128
#endif

typedef struct {
	uint64_t *ctx;
} shake128ctx;
#define shake128_absorb oqs_avx2_sha3_shake128_absorb
#define shake128_squeezeblocks oqs_avx2_sha3_shake128_squeezeblocks
#define shake128_ctx_release oqs_avx2_sha3_shake128_ctx_release
#define shake128_ctx_clone oqs_avx2_sha3_shake128_ctx_clone

typedef struct {
	uint64_t *ctx;
} shake128incctx;
#define shake128_inc_init oqs_avx2_sha3_shake128_inc_init
#define shake128_inc_absorb oqs_avx2_sha3_shake128_inc_absorb
#define shake128_inc_finalize oqs_avx2_sha3_shake128_inc_finalize
#define shake128_inc_squeeze oqs_avx2_sha3_shake128_inc_squeeze
#define shake128_inc_ctx_release oqs_avx2_sha3_shake128_inc_ctx_release
#define shake128_inc_ctx_clone oqs_avx2_sha3_shake128_inc_ctx_clone

#define SHAKE256_RATE OQS_SHA3_SHAKE256_RATE
#ifndef OQS_USE_SHA3_OPENSSL
#define shake256 OQS_AVX2_SHA3_shake256
#else
#define shake256 oqs_avx2_unused_SHA3_shake256
#endif

typedef struct {
	uint64_t *ctx;
} shake256ctx;
#define shake256_absorb oqs_avx2_sha3_shake256_absorb
#define shake256_squeezeblocks oqs_avx2_sha3_shake256_squeezeblocks
#define shake256_ctx_release oqs_avx2_sha3_shake256_ctx_release
#define shake256_ctx_clone oqs_avx2_sha3_shake256_ctx_clone

typedef struct {
	uint64_t *ctx;
} shake256incctx;
#define shake256_inc_init oqs_avx2_sha3_shake256_inc_init
#define shake256_inc_absorb oqs_avx2_sha3_shake256_inc_absorb
#define shake256_inc_finalize oqs_avx2_sha3_shake256_inc_finalize
#define shake256_inc_squeeze oqs_avx2_sha3_shake256_inc_squeeze
#define shake256_inc_ctx_release oqs_avx2_sha3_shake256_inc_ctx_release
#define shake256_inc_ctx_clone oqs_avx2_sha3_shake256_inc_ctx_clone

#include "fips202.c"

#define cshake128 OQS_SHA3_avx2_cshake128

#define cshake128_inc_init oqs_avx2_sha3_cshake128_inc_init
#define cshake128_inc_absorb oqs_avx2_sha3_cshake128_inc_absorb
#define cshake128_inc_finalize oqs_avx2_sha3_cshake128_inc_finalize
#define cshake128_inc_squeeze oqs_avx2_sha3_cshake128_inc_squeeze
#define cshake128_inc_ctx_release oqs_avx2_sha3_cshake128_inc_ctx_release
#define cshake128_inc_ctx_clone oqs_avx2_sha3_cshake128_inc_ctx_clone

#define cshake256 OQS_SHA3_avx2_cshake256

#define cshake256_inc_init oqs_avx2_sha3_cshake256_inc_init
#define cshake256_inc_absorb oqs_avx2_sha3_cshake256_inc_absorb
#define cshake256_inc_finalize oqs_avx2_sha3_cshake256_inc_finalize
#define cshake256_inc_squeeze oqs_avx2_sha3_cshake256_inc_squeeze
#define cshake256_inc_ctx_release oqs_avx2_sha3_cshake256_inc_ctx_release
#define cshake256_inc_ctx_clone oqs_avx2_sha3_cshake256_inc_ctx_clone

#include "sp800-185.c"

void OQS_SHA3_avx2_cshake128_simple(uint8_t *output, size_t outlen, uint16_t cstm, const uint8_t *input, size_t inplen) {

	shake128incctx state;

	oqs_avx2_sha3_shake128_inc_init(&state);

	/* Note: This function doesn't align exactly to cSHAKE (SP800-185 3.2), which should produce
	SHAKE output if S and N = zero (sort of a customized custom-SHAKE function).
	Padding is hard-coded as the first 32 bits, plus 16 bits of fixed S,
	and 16 bits of counter, equivalant to S=cstm, N=0.
	The short integer optimizes this function for a digest counter configuration */

	uint8_t sep[8];
	sep[0] = 0x01; /* bytepad */
	sep[1] = 0xA8;
	sep[2] = 0x01;
	sep[3] = 0x00;
	sep[4] = 0x01;
	sep[5] = 0x10; /* bitlen of cstm */
	sep[6] = cstm & 0xFF;
	sep[7] = cstm >> 8;

	state.ctx[0] = load64(sep);

	/* transform the domain string */
	KeccakF1600_StatePermute(state.ctx);

	/* absorb the state */
	oqs_avx2_sha3_cshake128_inc_absorb(&state, input, inplen);

	/* generate output */
	oqs_avx2_sha3_cshake128_inc_finalize(&state);
	oqs_avx2_sha3_cshake128_inc_squeeze(output, outlen, &state);
	oqs_avx2_sha3_shake128_inc_ctx_release(&state);
}

void OQS_SHA3_avx2_cshake256_simple(uint8_t *output, size_t outlen, uint16_t cstm, const uint8_t *input, size_t inplen) {

	shake256incctx state;

	oqs_avx2_sha3_shake256_inc_init(&state);

	/* Note: This function doesn't align exactly to cSHAKE (SP800-185 3.2), which should produce
	SHAKE output if S and N = zero (sort of a customized custom-SHAKE function).
	Padding is hard-coded as the first 32 bits, plus 16 bits of fixed S,
	and 16 bits of counter, equivalant to S=cstm, N=0.
	The short integer optimizes this function for a digest counter configuration */

	uint8_t sep[8];
	sep[0] = 0x01; /* bytepad */
	sep[1] = 0x88;
	sep[2] = 0x01;
	sep[3] = 0x00;
	sep[4] = 0x01;
	sep[5] = 0x10; /* bitlen of cstm */
	sep[6] = cstm & 0xFF;
	sep[7] = cstm >> 8;

	state.ctx[0] = load64(sep);

	/* transform the domain string */
	KeccakF1600_StatePermute(state.ctx);

	/* absorb the state */
	oqs_avx2_sha3_cshake256_inc_absorb(&state, input, inplen);

	/* generate output */
	oqs_avx2_sha3_cshake256_inc_finalize(&state);
	oqs_avx2_sha3_cshake256_inc_squeeze(output, outlen, &state);
	oqs_avx2_sha3_shake256_inc_ctx_release(&state);
}
