/*
 * Copyright (c) The mldsa-native project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */
/* Based on the public domain implementation in crypto_hash/keccakc512/simple/
 * from http://bench.cr.yp.to/supercop.html by Ronny Van Keer and the public
 * domain "TweetFips202" implementation from https://twitter.com/tweetfips202 by
 * Gilles Van Assche, Daniel J. Bernstein, and Peter Schwabe */

#include <stddef.h>
#include <stdint.h>

#include "fips202.h"
#include "keccakf1600.h"

#define NROUNDS 24
#define ROL(a, offset) ((a << offset) ^ (a >> (64 - offset)))

/*************************************************
 * Name:        load64
 *
 * Description: Load 8 bytes into uint64_t in little-endian order
 *
 * Arguments:   - const uint8_t *x: pointer to input byte array
 *
 * Returns the loaded 64-bit unsigned integer
 **************************************************/
static uint64_t load64(const uint8_t x[8])
__contract__(
  requires(memory_no_alias(x, sizeof(uint8_t) * 8))
)
{
  unsigned int i;
  uint64_t r = 0;

  for (i = 0; i < 8; i++)
  __loop__(
    invariant(i <= 8)
  )
  {
    r |= (uint64_t)x[i] << 8 * i;
  }

  return r;
}

/*************************************************
 * Name:        store64
 *
 * Description: Store a 64-bit integer to array of 8 bytes in little-endian
 *order
 *
 * Arguments:   - uint8_t *x: pointer to the output byte array (allocated)
 *              - uint64_t u: input 64-bit unsigned integer
 **************************************************/
static void store64(uint8_t x[8], uint64_t u)
__contract__(
  requires(memory_no_alias(x, sizeof(uint8_t) * 8))
  assigns(memory_slice(x, sizeof(uint8_t) * 8))
)
{
  unsigned int i;

  for (i = 0; i < 8; i++)
  __loop__(
    invariant(i <= 8)
  )
  {
    /* Explicitly truncate to uint8_t */
    x[i] = (uint8_t)((u >> (8 * i)) & 0xFF);
  }
}

/*************************************************
 * Name:        keccak_init
 *
 * Description: Initializes the Keccak state.
 *
 * Arguments:   - uint64_t *s: pointer to Keccak state
 **************************************************/
static void keccak_init(uint64_t s[MLD_KECCAK_LANES])
__contract__(
  requires(memory_no_alias(s, sizeof(uint64_t) * MLD_KECCAK_LANES))
  assigns(memory_slice(s, sizeof(uint64_t) * MLD_KECCAK_LANES))
)
{
  unsigned int i;
  for (i = 0; i < MLD_KECCAK_LANES; i++)
  __loop__(
    invariant(i <= MLD_KECCAK_LANES)
    invariant(forall(k, 0, i, s[k] == 0))
  )
  {
    s[i] = 0;
  }

  cassert(forall(k, 0, MLD_KECCAK_LANES, s[k] == 0));
}

/*************************************************
 * Name:        keccak_absorb
 *
 * Description: Absorb step of Keccak; incremental.
 *
 * Arguments:   - uint64_t *s: pointer to Keccak state
 *              - unsigned int pos: position in current block to be absorbed
 *              - unsigned int r: rate in bytes (e.g., 168 for SHAKE128)
 *              - const uint8_t *in: pointer to input to be absorbed into s
 *              - size_t inlen: length of input in bytes
 *
 * Returns new position pos in current block
 **************************************************/
static unsigned int keccak_absorb(uint64_t s[MLD_KECCAK_LANES],
                                  unsigned int pos, unsigned int r,
                                  const uint8_t *in, size_t inlen)
__contract__(
  requires(r < sizeof(uint64_t) * MLD_KECCAK_LANES)
  requires(pos <= r)
  requires(memory_no_alias(s, sizeof(uint64_t) * MLD_KECCAK_LANES))
  requires(memory_no_alias(in, inlen))
  assigns(memory_slice(s, sizeof(uint64_t) * MLD_KECCAK_LANES))
  ensures(return_value < r))
{
  unsigned int i;

  while (pos + inlen >= r)
  __loop__(
    assigns(pos, i, in, inlen,
      memory_slice(s, sizeof(uint64_t) *  MLD_KECCAK_LANES))
    invariant(inlen <= loop_entry(inlen))
    invariant(pos <= r)
    invariant(in == loop_entry(in) + (loop_entry(inlen) - inlen)))
  {
    for (i = pos; i < r; i++)
    __loop__(
      assigns(i, in, memory_slice(s, sizeof(uint64_t) * MLD_KECCAK_LANES))
      invariant(i >= pos && i <= r)
      invariant(in == loop_entry(in) + (i - pos)))
    {
      s[i / 8] ^= (uint64_t)*in++ << 8 * (i % 8);
    }
    inlen -= r - pos;
    mld_keccakf1600_permute(s);
    pos = 0;
  }

  for (i = pos; i < pos + inlen; i++)
  __loop__(
    invariant(i >= pos && i - pos <= inlen)
    invariant(in == loop_entry(in) + (i - pos)))
  {
    s[i / 8] ^= (uint64_t)*in++ << 8 * (i % 8);
  }

  return i;
}

/*************************************************
 * Name:        keccak_finalize
 *
 * Description: Finalize absorb step.
 *
 * Arguments:   - uint64_t *s: pointer to Keccak state
 *              - unsigned int pos: position in current block to be absorbed
 *              - unsigned int r: rate in bytes (e.g., 168 for SHAKE128)
 *              - uint8_t p: domain separation byte
 **************************************************/
static void keccak_finalize(uint64_t s[MLD_KECCAK_LANES], unsigned int pos,
                            unsigned int r, uint8_t p)
__contract__(
  requires(pos <= r && r < sizeof(uint64_t) * MLD_KECCAK_LANES)
  requires((r / 8) >= 1)
  requires(memory_no_alias(s, sizeof(uint64_t) * MLD_KECCAK_LANES))
  assigns(memory_slice(s, sizeof(uint64_t) * MLD_KECCAK_LANES))
)
{
  s[pos / 8] ^= (uint64_t)p << 8 * (pos % 8);
  s[r / 8 - 1] ^= 1ULL << 63;
}

/*************************************************
 * Name:        keccak_squeeze
 *
 * Description: Squeeze step of Keccak. Squeezes arbitratrily many bytes.
 *              Modifies the state. Can be called multiple times to keep
 *              squeezing, i.e., is incremental.
 *
 * Arguments:   - uint8_t *out: pointer to output data
 *              - size_t outlen: number of bytes to be squeezed (written to out)
 *              - uint64_t *s: pointer to input/output Keccak state
 *              - unsigned int pos: number of bytes in current block already
 *squeezed
 *              - unsigned int r: rate in bytes (e.g., 168 for SHAKE128)
 *
 * Returns new position pos in current block
 **************************************************/
static unsigned int keccak_squeeze(uint8_t *out, size_t outlen,
                                   uint64_t s[MLD_KECCAK_LANES],
                                   unsigned int pos, unsigned int r)
__contract__(
  requires((r == SHAKE128_RATE && pos <= SHAKE128_RATE) ||
    (r == SHAKE256_RATE && pos <= SHAKE256_RATE))
  requires(outlen <= 8 * r /* somewhat arbitrary bound */)
  requires(memory_no_alias(s, sizeof(uint64_t) * MLD_KECCAK_LANES))
  requires(memory_no_alias(out, outlen))
  assigns(memory_slice(s, sizeof(uint64_t) * MLD_KECCAK_LANES))
  assigns(memory_slice(out, outlen))
  ensures(return_value <= r))
{
  unsigned int i;
  size_t out_offset = 0;

  /* Reference: This code is re-factored from the reference implementation
   * to facilitate proof with CBMC and to improve readability.
   *
   * Take a mutable copy of outlen to count down the number of bytes
   * still to squeeze. The initial value of outlen is needed for the CBMC
   * assigns() clauses. */
  size_t bytes_to_go = outlen;

  while (bytes_to_go > 0)
  __loop__(
    assigns(i, bytes_to_go, pos, out_offset, memory_slice(s, sizeof(uint64_t) * MLD_KECCAK_LANES), memory_slice(out, outlen))
    invariant(bytes_to_go <= outlen)
    invariant(out_offset == outlen - bytes_to_go)
    invariant(pos <= r)
  )
  {
    if (pos == r)
    {
      mld_keccakf1600_permute(s);
      pos = 0;
    }
    for (i = pos; i < r && i < pos + bytes_to_go; i++)
    __loop__(
      assigns(i, out_offset, memory_slice(out, outlen))
      invariant(i >= pos && i <= r && i <= pos + bytes_to_go)
      invariant(out_offset == loop_entry(out_offset) + (i - pos))
    )
    {
      const uint64_t lane = s[i / 8];
      out[out_offset] = (uint8_t)((lane >> (8 * (i % 8))) & 0xFF);
      out_offset++;
    }
    bytes_to_go -= i - pos;
    pos = i;
  }

  return pos;
}

/*************************************************
 * Name:        keccak_absorb_once
 *
 * Description: Absorb step of Keccak;
 *              non-incremental, starts by zeroeing the state.
 *
 * Arguments:   - uint64_t *s: pointer to (uninitialized) output Keccak state
 *              - const unsigned int r: rate in bytes (e.g., 168 for SHAKE128)
 *              - const uint8_t *in: pointer to input to be absorbed into s
 *              - size_t inlen: length of input in bytes
 *              - uint8_t p: domain-separation byte for different Keccak-derived
 *functions
 **************************************************/
static void keccak_absorb_once(uint64_t s[MLD_KECCAK_LANES],
                               const unsigned int r, const uint8_t *in,
                               size_t inlen, uint8_t p)
__contract__(
  requires(r <= sizeof(uint64_t) * MLD_KECCAK_LANES)
  requires(memory_no_alias(s, sizeof(uint64_t) * MLD_KECCAK_LANES))
  requires(memory_no_alias(in, inlen))
  assigns(memory_slice(s, sizeof(uint64_t) * MLD_KECCAK_LANES)))
{
  unsigned int i;

  for (i = 0; i < MLD_KECCAK_LANES; i++)
  __loop__(invariant(i <= MLD_KECCAK_LANES))
  {
    s[i] = 0;
  }

  while (inlen >= r)
  __loop__(
    invariant(inlen <= loop_entry(inlen))
    invariant(in == loop_entry(in) + (loop_entry(inlen) - inlen)))
  {
    for (i = 0; i < r / 8; i++)
    __loop__(invariant(i <= r / 8))
    {
      s[i] ^= load64(in + 8 * i);
    }
    in += r;
    inlen -= r;
    mld_keccakf1600_permute(s);
  }

  for (i = 0; i < inlen; i++)
  __loop__(invariant(i <= inlen))
  {
    s[i / 8] ^= (uint64_t)in[i] << 8 * (i % 8);
  }

  s[i / 8] ^= (uint64_t)p << 8 * (i % 8);
  s[(r - 1) / 8] ^= 1ULL << 63;
}

/*************************************************
 * Name:        keccak_squeezeblocks
 *
 * Description: Squeeze step of Keccak. Squeezes full blocks of r bytes each.
 *              Modifies the state. Can be called multiple times to keep
 *              squeezing, i.e., is incremental. Assumes zero bytes of current
 *              block have already been squeezed.
 *
 * Arguments:   - uint8_t *out: pointer to output blocks
 *              - size_t nblocks: number of blocks to be squeezed (written to
 *out)
 *              - uint64_t *s: pointer to input/output Keccak state
 *              - unsigned int r: rate in bytes (e.g., 168 for SHAKE128)
 **************************************************/
static void keccak_squeezeblocks(uint8_t *out, size_t nblocks,
                                 uint64_t s[MLD_KECCAK_LANES], unsigned int r)
__contract__(
  requires(r == SHAKE128_RATE || r == SHAKE256_RATE)
  requires(nblocks <= 8 /* somewhat arbitrary bound */)
  requires(memory_no_alias(s, sizeof(uint64_t) * MLD_KECCAK_LANES))
  requires(memory_no_alias(out, nblocks * r))
  assigns(memory_slice(s, sizeof(uint64_t) * MLD_KECCAK_LANES))
  assigns(memory_slice(out, nblocks * r)))
{
  while (nblocks > 0)
  __loop__(
    assigns(out, nblocks, memory_slice(s, sizeof(uint64_t) * MLD_KECCAK_LANES),
      memory_slice(out, nblocks * r))
    invariant(nblocks <= loop_entry(nblocks))
    invariant(out == loop_entry(out) + r * (loop_entry(nblocks) - nblocks))
  )
  {
    mld_keccakf1600_permute(s);
    mld_keccakf1600_extract_bytes(s, out, 0, r);
    out += r;
    nblocks -= 1;
  }
}

/*************************************************
 * Name:        shake128_init
 *
 * Description: Initilizes Keccak state for use as SHAKE128 XOF
 *
 * Arguments:   - keccak_state *state: pointer to (uninitialized) Keccak state
 **************************************************/
void shake128_init(keccak_state *state)
{
  keccak_init(state->s);
  state->pos = 0;
}

/*************************************************
 * Name:        shake128_absorb
 *
 * Description: Absorb step of the SHAKE128 XOF; incremental.
 *
 * Arguments:   - keccak_state *state: pointer to (initialized) output Keccak
 *state
 *              - const uint8_t *in: pointer to input to be absorbed into s
 *              - size_t inlen: length of input in bytes
 **************************************************/
void shake128_absorb(keccak_state *state, const uint8_t *in, size_t inlen)
{
  state->pos = keccak_absorb(state->s, state->pos, SHAKE128_RATE, in, inlen);
}

/*************************************************
 * Name:        shake128_finalize
 *
 * Description: Finalize absorb step of the SHAKE128 XOF.
 *
 * Arguments:   - keccak_state *state: pointer to Keccak state
 **************************************************/
void shake128_finalize(keccak_state *state)
{
  keccak_finalize(state->s, state->pos, SHAKE128_RATE, 0x1F);
  state->pos = SHAKE128_RATE;
}

/*************************************************
 * Name:        shake128_squeeze
 *
 * Description: Squeeze step of SHAKE128 XOF. Squeezes arbitraily many
 *              bytes. Can be called multiple times to keep squeezing.
 *
 * Arguments:   - uint8_t *out: pointer to output blocks
 *              - size_t outlen : number of bytes to be squeezed (written to
 *output)
 *              - keccak_state *s: pointer to input/output Keccak state
 **************************************************/
void shake128_squeeze(uint8_t *out, size_t outlen, keccak_state *state)
{
  state->pos = keccak_squeeze(out, outlen, state->s, state->pos, SHAKE128_RATE);
}

/*************************************************
 * Name:        shake128_absorb_once
 *
 * Description: Initialize, absorb into and finalize SHAKE128 XOF;
 *non-incremental.
 *
 * Arguments:   - keccak_state *state: pointer to (uninitialized) output Keccak
 *state
 *              - const uint8_t *in: pointer to input to be absorbed into s
 *              - size_t inlen: length of input in bytes
 **************************************************/
void shake128_absorb_once(keccak_state *state, const uint8_t *in, size_t inlen)
{
  keccak_absorb_once(state->s, SHAKE128_RATE, in, inlen, 0x1F);
  state->pos = SHAKE128_RATE;
}

/*************************************************
 * Name:        shake128_squeezeblocks
 *
 * Description: Squeeze step of SHAKE128 XOF. Squeezes full blocks of
 *              SHAKE128_RATE bytes each. Can be called multiple times
 *              to keep squeezing. Assumes new block has not yet been
 *              started (state->pos = SHAKE128_RATE).
 *
 * Arguments:   - uint8_t *out: pointer to output blocks
 *              - size_t nblocks: number of blocks to be squeezed (written to
 *output)
 *              - keccak_state *s: pointer to input/output Keccak state
 **************************************************/
void shake128_squeezeblocks(uint8_t *out, size_t nblocks, keccak_state *state)
{
  keccak_squeezeblocks(out, nblocks, state->s, SHAKE128_RATE);
}

void shake128_release(keccak_state *state)
{
  /* FIPS 204. Section 3.6.3 Destruction of intermediate values. */
  mld_zeroize(state, sizeof(keccak_state));
}

/*************************************************
 * Name:        shake256_init
 *
 * Description: Initilizes Keccak state for use as SHAKE256 XOF
 *
 * Arguments:   - keccak_state *state: pointer to (uninitialized) Keccak state
 **************************************************/
void shake256_init(keccak_state *state)
{
  keccak_init(state->s);
  state->pos = 0;
}

/*************************************************
 * Name:        shake256_absorb
 *
 * Description: Absorb step of the SHAKE256 XOF; incremental.
 *
 * Arguments:   - keccak_state *state: pointer to (initialized) output Keccak
 *state
 *              - const uint8_t *in: pointer to input to be absorbed into s
 *              - size_t inlen: length of input in bytes
 **************************************************/
void shake256_absorb(keccak_state *state, const uint8_t *in, size_t inlen)
{
  state->pos = keccak_absorb(state->s, state->pos, SHAKE256_RATE, in, inlen);
}

/*************************************************
 * Name:        shake256_finalize
 *
 * Description: Finalize absorb step of the SHAKE256 XOF.
 *
 * Arguments:   - keccak_state *state: pointer to Keccak state
 **************************************************/
void shake256_finalize(keccak_state *state)
{
  keccak_finalize(state->s, state->pos, SHAKE256_RATE, 0x1F);
  state->pos = SHAKE256_RATE;
}

/*************************************************
 * Name:        shake256_squeeze
 *
 * Description: Squeeze step of SHAKE256 XOF. Squeezes arbitraily many
 *              bytes. Can be called multiple times to keep squeezing.
 *
 * Arguments:   - uint8_t *out: pointer to output blocks
 *              - size_t outlen : number of bytes to be squeezed (written to
 *output)
 *              - keccak_state *s: pointer to input/output Keccak state
 **************************************************/
void shake256_squeeze(uint8_t *out, size_t outlen, keccak_state *state)
{
  state->pos = keccak_squeeze(out, outlen, state->s, state->pos, SHAKE256_RATE);
}

/*************************************************
 * Name:        shake256_absorb_once
 *
 * Description: Initialize, absorb into and finalize SHAKE256 XOF;
 *non-incremental.
 *
 * Arguments:   - keccak_state *state: pointer to (uninitialized) output Keccak
 *state
 *              - const uint8_t *in: pointer to input to be absorbed into s
 *              - size_t inlen: length of input in bytes
 **************************************************/
void shake256_absorb_once(keccak_state *state, const uint8_t *in, size_t inlen)
{
  keccak_absorb_once(state->s, SHAKE256_RATE, in, inlen, 0x1F);
  state->pos = SHAKE256_RATE;
}

/*************************************************
 * Name:        shake256_squeezeblocks
 *
 * Description: Squeeze step of SHAKE256 XOF. Squeezes full blocks of
 *              SHAKE256_RATE bytes each. Can be called multiple times
 *              to keep squeezing. Assumes next block has not yet been
 *              started (state->pos = SHAKE256_RATE).
 *
 * Arguments:   - uint8_t *out: pointer to output blocks
 *              - size_t nblocks: number of blocks to be squeezed (written to
 *output)
 *              - keccak_state *s: pointer to input/output Keccak state
 **************************************************/
void shake256_squeezeblocks(uint8_t *out, size_t nblocks, keccak_state *state)
{
  keccak_squeezeblocks(out, nblocks, state->s, SHAKE256_RATE);
}

void shake256_release(keccak_state *state)
{
  /* FIPS 204. Section 3.6.3 Destruction of intermediate values. */
  mld_zeroize(state, sizeof(keccak_state));
}

/*************************************************
 * Name:        shake128
 *
 * Description: SHAKE128 XOF with non-incremental API
 *
 * Arguments:   - uint8_t *out: pointer to output
 *              - size_t outlen: requested output length in bytes
 *              - const uint8_t *in: pointer to input
 *              - size_t inlen: length of input in bytes
 **************************************************/
void shake128(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen)
{
  size_t nblocks;
  keccak_state state;

  shake128_absorb_once(&state, in, inlen);
  nblocks = outlen / SHAKE128_RATE;
  shake128_squeezeblocks(out, nblocks, &state);
  outlen -= nblocks * SHAKE128_RATE;
  out += nblocks * SHAKE128_RATE;
  shake128_squeeze(out, outlen, &state);

  /* FIPS 204. Section 3.6.3 Destruction of intermediate values. */
  mld_zeroize(&state, sizeof(state));
}

/*************************************************
 * Name:        shake256
 *
 * Description: SHAKE256 XOF with non-incremental API
 *
 * Arguments:   - uint8_t *out: pointer to output
 *              - size_t outlen: requested output length in bytes
 *              - const uint8_t *in: pointer to input
 *              - size_t inlen: length of input in bytes
 **************************************************/
void shake256(uint8_t *out, size_t outlen, const uint8_t *in, size_t inlen)
{
  size_t nblocks;
  keccak_state state;

  shake256_absorb_once(&state, in, inlen);
  nblocks = outlen / SHAKE256_RATE;
  shake256_squeezeblocks(out, nblocks, &state);
  outlen -= nblocks * SHAKE256_RATE;
  out += nblocks * SHAKE256_RATE;
  shake256_squeeze(out, outlen, &state);

  /* FIPS 204. Section 3.6.3 Destruction of intermediate values. */
  mld_zeroize(&state, sizeof(state));
}

/*************************************************
 * Name:        sha3_256
 *
 * Description: SHA3-256 with non-incremental API
 *
 * Arguments:   - uint8_t *h: pointer to output (32 bytes)
 *              - const uint8_t *in: pointer to input
 *              - size_t inlen: length of input in bytes
 **************************************************/
void sha3_256(uint8_t h[SHA3_256_HASHBYTES], const uint8_t *in, size_t inlen)
{
  unsigned int i;
  uint64_t s[MLD_KECCAK_LANES];

  keccak_absorb_once(s, SHA3_256_RATE, in, inlen, 0x06);
  mld_keccakf1600_permute(s);
  for (i = 0; i < 4; i++)
  __loop__(invariant(i <= 4))
  {
    store64(h + 8 * i, s[i]);
  }

  /* FIPS 204. Section 3.6.3 Destruction of intermediate values. */
  mld_zeroize(s, sizeof(s));
}

/*************************************************
 * Name:        sha3_512
 *
 * Description: SHA3-512 with non-incremental API
 *
 * Arguments:   - uint8_t *h: pointer to output (64 bytes)
 *              - const uint8_t *in: pointer to input
 *              - size_t inlen: length of input in bytes
 **************************************************/
void sha3_512(uint8_t h[SHA3_512_HASHBYTES], const uint8_t *in, size_t inlen)
{
  unsigned int i;
  uint64_t s[MLD_KECCAK_LANES];

  keccak_absorb_once(s, SHA3_512_RATE, in, inlen, 0x06);
  mld_keccakf1600_permute(s);
  for (i = 0; i < 8; i++)
  __loop__(invariant(i <= 8))
  {
    store64(h + 8 * i, s[i]);
  }

  /* FIPS 204. Section 3.6.3 Destruction of intermediate values. */
  mld_zeroize(s, sizeof(s));
}
