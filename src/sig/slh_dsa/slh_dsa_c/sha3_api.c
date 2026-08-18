/*
 * Copyright (c) The slhdsa-c project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* === FIPS 202: SHA-3 hash and SHAKE eXtensible Output Functions (XOF) */

#include "sha3_api.h"

/* These functions have not been optimized for performance. */
/* initialize the context for SHA3 */

void sha3_init(sha3_var_t *c, size_t md_sz)
{
  size_t i;

  for (i = 0; i < 25; i++)
  {
    c->st.d[i] = 0;
  }
  c->md_sz = md_sz; /* in SHAKE; if 0, padding done */
  c->r_sz = 200 - 2 * md_sz;
  c->pt = 0;
}

/* update state with more data */

void sha3_update(sha3_var_t *c, const void *data, size_t len)
{
  size_t i, j;

  j = c->pt;
  for (i = 0; i < len; i++)
  {
    c->st.b[j++] ^= ((const uint8_t *)data)[i];
    if (j >= c->r_sz)
    {
      keccak_f1600(c->st.d);
      j = 0;
    }
  }
  c->pt = j;
}

/* finalize and output a hash */

void sha3_final(sha3_var_t *c, uint8_t *md)
{
  size_t i;

  c->st.b[c->pt] ^= 0x06;
  c->st.b[c->r_sz - 1] ^= 0x80;
  keccak_f1600(c->st.d);

  for (i = 0; i < c->md_sz; i++)
  {
    md[i] = c->st.b[i];
  }
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
  size_t i, j;

  /* add padding on the first call */
  if (c->md_sz != 0)
  {
    c->st.b[c->pt] ^= 0x1F;
    c->st.b[c->r_sz - 1] ^= 0x80;
    keccak_f1600(c->st.d);
    c->pt = 0;
    c->md_sz = 0;
  }

  j = c->pt;
  for (i = 0; i < out_sz; i++)
  {
    if (j >= c->r_sz)
    {
      keccak_f1600(c->st.d);
      j = 0;
    }
    out[i] = c->st.b[j++];
  }
  c->pt = j;
}

/* compute a SHAKE hash "md" of "md_sz" bytes from data in "in" */

void shake(uint8_t *md, size_t md_sz, const void *in, size_t in_sz, size_t r_sz)
{
  sha3_var_t sha3;

  sha3_init(&sha3, r_sz);
  sha3_update(&sha3, in, in_sz);
  shake_out(&sha3, md, md_sz);
}
