/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
#ifndef MLD_FIPS202_FIPS202_H
#define MLD_FIPS202_FIPS202_H

#include <stddef.h>
#include <stdint.h>
#include "../cbmc.h"

#define SHAKE128_RATE 168
#define SHAKE256_RATE 136
#define SHA3_256_RATE 136
#define SHA3_512_RATE 72
#define MLD_KECCAK_LANES 25
#define SHA3_256_HASHBYTES 32
#define SHA3_512_HASHBYTES 64

#define FIPS202_NAMESPACE(s) mldsa_fips202_ref_##s

typedef struct
{
  uint64_t s[MLD_KECCAK_LANES];
  unsigned int pos;
} keccak_state;

#define KeccakF_RoundConstants FIPS202_NAMESPACE(KeccakF_RoundConstants)
extern const uint64_t KeccakF_RoundConstants[];

#define shake128_init FIPS202_NAMESPACE(shake128_init)
void shake128_init(keccak_state *state)
__contract__(
  requires(memory_no_alias(state, sizeof(keccak_state)))
  assigns(memory_slice(state, sizeof(keccak_state)))
  ensures(state->pos == 0)
);

#define shake128_absorb FIPS202_NAMESPACE(shake128_absorb)
void shake128_absorb(keccak_state *state, const uint8_t *in, size_t inlen)
__contract__(
  requires(memory_no_alias(state, sizeof(keccak_state)))
  requires(memory_no_alias(in, inlen))
  requires(state->pos <= SHAKE128_RATE)
  assigns(memory_slice(state, sizeof(keccak_state)))
  ensures(state->pos <= SHAKE128_RATE)
);

#define shake128_finalize FIPS202_NAMESPACE(shake128_finalize)
void shake128_finalize(keccak_state *state)
__contract__(
  requires(memory_no_alias(state, sizeof(keccak_state)))
  requires(state->pos <= SHAKE128_RATE)
  assigns(memory_slice(state, sizeof(keccak_state)))
  ensures(state->pos <= SHAKE128_RATE)
);

#define shake128_squeeze FIPS202_NAMESPACE(shake128_squeeze)
void shake128_squeeze(uint8_t *out, size_t outlen, keccak_state *state)
__contract__(
  requires(outlen <= 8 * SHAKE128_RATE /* somewhat arbitrary bound */)
  requires(memory_no_alias(state, sizeof(keccak_state)))
  requires(memory_no_alias(out, outlen))
  requires(state->pos <= SHAKE128_RATE)
  assigns(memory_slice(state, sizeof(keccak_state)))
  assigns(memory_slice(out, outlen))
  ensures(state->pos <= SHAKE128_RATE)
);

#define shake128_absorb_once FIPS202_NAMESPACE(shake128_absorb_once)
void shake128_absorb_once(keccak_state *state, const uint8_t *in, size_t inlen)
__contract__(
  requires(memory_no_alias(state, sizeof(keccak_state)))
  requires(memory_no_alias(in, inlen))
  assigns(memory_slice(state, sizeof(keccak_state)))
  ensures(state->pos == SHAKE128_RATE)
);

#define shake128_squeezeblocks FIPS202_NAMESPACE(shake128_squeezeblocks)
void shake128_squeezeblocks(uint8_t *out, size_t nblocks, keccak_state *state)
__contract__(
  requires(nblocks <= 8 /* somewhat arbitrary bound */)
  requires(memory_no_alias(state, sizeof(keccak_state)))
  requires(memory_no_alias(out, nblocks * SHAKE128_RATE))
  requires(state->pos <= SHAKE128_RATE)
  assigns(memory_slice(state, sizeof(keccak_state)))
  assigns(memory_slice(out, nblocks * SHAKE128_RATE))
  ensures(state->pos <= SHAKE128_RATE)
);

#define shake128_release FIPS202_NAMESPACE(shake128_release)
void shake128_release(keccak_state *state);

#define shake256_init FIPS202_NAMESPACE(shake256_init)
void shake256_init(keccak_state *state)
__contract__(
  requires(memory_no_alias(state, sizeof(keccak_state)))
  assigns(memory_slice(state, sizeof(keccak_state)))
  ensures(state->pos == 0)
);

#define shake256_absorb FIPS202_NAMESPACE(shake256_absorb)
void shake256_absorb(keccak_state *state, const uint8_t *in, size_t inlen)
__contract__(
  requires(memory_no_alias(state, sizeof(keccak_state)))
  requires(memory_no_alias(in, inlen))
  requires(state->pos <= SHAKE256_RATE)
  assigns(memory_slice(state, sizeof(keccak_state)))
  ensures(state->pos <= SHAKE256_RATE)
);

#define shake256_finalize FIPS202_NAMESPACE(shake256_finalize)
void shake256_finalize(keccak_state *state)
__contract__(
  requires(memory_no_alias(state, sizeof(keccak_state)))
  requires(state->pos <= SHAKE256_RATE)
  assigns(memory_slice(state, sizeof(keccak_state)))
  ensures(state->pos <= SHAKE256_RATE)
);

#define shake256_squeeze FIPS202_NAMESPACE(shake256_squeeze)
void shake256_squeeze(uint8_t *out, size_t outlen, keccak_state *state)
__contract__(
  requires(outlen <= 8 * SHAKE256_RATE /* somewhat arbitrary bound */)
  requires(memory_no_alias(state, sizeof(keccak_state)))
  requires(memory_no_alias(out, outlen))
  requires(state->pos <= SHAKE256_RATE)
  assigns(memory_slice(state, sizeof(keccak_state)))
  assigns(memory_slice(out, outlen))
  ensures(state->pos <= SHAKE256_RATE)
);

#define shake256_absorb_once FIPS202_NAMESPACE(shake256_absorb_once)
void shake256_absorb_once(keccak_state *state, const uint8_t *in, size_t inlen)
__contract__(
  requires(memory_no_alias(state, sizeof(keccak_state)))
  requires(memory_no_alias(in, inlen))
  assigns(memory_slice(state, sizeof(keccak_state)))
  ensures(state->pos == SHAKE256_RATE)
);

#define shake256_squeezeblocks FIPS202_NAMESPACE(shake256_squeezeblocks)
void shake256_squeezeblocks(uint8_t *out, size_t nblocks, keccak_state *state)
__contract__(
  requires(nblocks <= 8 /* somewhat arbitrary bound */)
  requires(memory_no_alias(state, sizeof(keccak_state)))
  requires(memory_no_alias(out, nblocks * SHAKE256_RATE))
  requires(state->pos <= SHAKE256_RATE)
  assigns(memory_slice(state, sizeof(keccak_state)))
  assigns(memory_slice(out, nblocks * SHAKE256_RATE))
  ensures(state->pos <= SHAKE256_RATE)
);

#define shake256_release FIPS202_NAMESPACE(shake256_release)
void shake256_release(keccak_state *state);

#define shake128 FIPS202_NAMESPACE(shake128)
void shake128(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen)
__contract__(
  requires(outlen <= 8 * SHAKE128_RATE /* somewhat arbitrary bound */)
  requires(memory_no_alias(in, inlen))
  requires(memory_no_alias(out, outlen))
  assigns(memory_slice(out, outlen))
);

#define shake256 FIPS202_NAMESPACE(shake256)
void shake256(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen)
__contract__(
  requires(outlen <= 8 * SHAKE256_RATE /* somewhat arbitrary bound */)
  requires(memory_no_alias(in, inlen))
  requires(memory_no_alias(out, outlen))
  assigns(memory_slice(out, outlen))
);

#define sha3_256 FIPS202_NAMESPACE(sha3_256)
void sha3_256(uint8_t h[SHA3_256_HASHBYTES], const uint8_t *in, size_t inlen)
__contract__(
  requires(memory_no_alias(in, inlen))
  requires(memory_no_alias(h, SHA3_256_HASHBYTES))
  assigns(memory_slice(h, SHA3_256_HASHBYTES))
);

#define sha3_512 FIPS202_NAMESPACE(sha3_512)
void sha3_512(uint8_t h[SHA3_512_HASHBYTES], const uint8_t *in, size_t inlen)
__contract__(
  requires(memory_no_alias(in, inlen))
  requires(memory_no_alias(h, SHA3_512_HASHBYTES))
  assigns(memory_slice(h, SHA3_512_HASHBYTES))
);

#endif /* !MLD_FIPS202_FIPS202_H */
