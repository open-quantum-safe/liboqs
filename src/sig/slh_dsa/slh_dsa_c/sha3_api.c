/*
 * Copyright (c) The slhdsa-c project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* === FIPS 202: SHA-3 hash and SHAKE eXtensible Output Functions (XOF) */

#include <string.h>
#include "sha3_api.h"
#include "cbmc.h"

/* These functions have not been optimized for performance. */
/* initialize the context for SHA3 */

void sha3_init(sha3_var_t *c, size_t md_sz)
{
  memset(c->st.d, 0, 25*sizeof(uint64_t));
  
  c->md_sz = md_sz; /* in SHAKE; if 0, padding done */
  c->r_sz = 200 - 2 * md_sz;
  c->pt = 0;
}

/* update state with more data */

void sha3_update(sha3_var_t *c, const void *data, size_t len)
{
  const uint8_t *in = data;
  size_t l;

  while (len > 0)
  {
    l = c->r_sz - c->pt;
    if (l > len)
    {
      l = len;
    }
    keccak_f1600_xor_bytes(c->st.d, in, c->pt, l);
    c->pt += l;
    in += l;
    len -= l;
    if (c->pt >= c->r_sz)
    {
      keccak_f1600(c->st.d);
      c->pt = 0;
    }
  }
}

/* finalize and output a hash */

void sha3_final(sha3_var_t *c, uint8_t *md)
{
  const uint8_t pad = 0x06, pad_end = 0x80;

  keccak_f1600_xor_bytes(c->st.d, &pad, c->pt, 1);
  keccak_f1600_xor_bytes(c->st.d, &pad_end, c->r_sz - 1, 1);
  keccak_f1600(c->st.d);

  keccak_f1600_extract_bytes(c->st.d, md, 0, c->md_sz);
}

/* compute a SHA-3 hash "md" of "md_sz" bytes from data in "in" */

void sha3(uint8_t *md, size_t md_sz, const void *in, size_t in_sz)
{
  sha3_var_t sha3;

  sha3_init(&sha3, md_sz);
  sha3_update(&sha3, in, in_sz);
  sha3_final(&sha3, md);
}

/* SHAKE128 and SHAKE256 extensible-output functionality */
/* squeeze output */

void shake_out(sha3_var_t *c, uint8_t *out, size_t out_sz)
{
  const uint8_t pad = 0x1F, pad_end = 0x80;
  size_t l;

  /* add padding on the first call */
  if (c->md_sz != 0)
  {
    keccak_f1600_xor_bytes(c->st.d, &pad, c->pt, 1);
    keccak_f1600_xor_bytes(c->st.d, &pad_end, c->r_sz - 1, 1);
    keccak_f1600(c->st.d);
    c->pt = 0;
    c->md_sz = 0;
  }

  while (out_sz > 0)
  {
    if (c->pt >= c->r_sz)
    {
      keccak_f1600(c->st.d);
      c->pt = 0;
    }
    l = c->r_sz - c->pt;
    if (l > out_sz)
    {
      l = out_sz;
    }
    keccak_f1600_extract_bytes(c->st.d, out, c->pt, l);
    c->pt += l;
    out += l;
    out_sz -= l;
  }
}

/* compute a SHAKE hash "md" of "md_sz" bytes from data in "in" */

void shake(uint8_t *md, size_t md_sz, const void *in, size_t in_sz, size_t r_sz)
{
  sha3_var_t sha3;

  sha3_init(&sha3, r_sz);
  sha3_update(&sha3, in, in_sz);
  shake_out(&sha3, md, md_sz);
}
