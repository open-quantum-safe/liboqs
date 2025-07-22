/*
 * Copyright (c) The slhdsa-c project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* === Internal ADRS manipulation functions (FIPS 205 Section 4.3) */

#ifndef _SLH_ADRS_H_
#define _SLH_ADRS_H_

#include <string.h>
#include "slh_param.h"
#include "slh_var.h"
#include "slh_sys.h"

/* ADRS type constants */

#define ADRS_WOTS_HASH 0
#define ADRS_WOTS_PK 1
#define ADRS_TREE 2
#define ADRS_FORS_TREE 3
#define ADRS_FORS_ROOTS 4
#define ADRS_WOTS_PRF 5
#define ADRS_FORS_PRF 6

/* Algorithm 1: toInt(x, n) */

static SLH_INLINE uint64_t slh_toint(const uint8_t *x, unsigned n)
{
  unsigned i;
  uint64_t t;

  if (n == 0)
  {
    return 0;
  }
  t = (uint64_t)x[0];
  for (i = 1; i < n; i++)
  {
    t <<= 8;
    t += (uint64_t)x[i];
  }
  return t;
}

/* Algorithm 2: toByte(x, n) */

static SLH_INLINE void slh_tobyte(uint8_t *x, uint64_t t, unsigned n)
{
  unsigned i;

  if (n == 0)
  {
    return;
  }
  for (i = n - 1; i > 0; i--)
  {
    x[i] = (uint8_t)(t & 0xFF);
    t >>= 8;
  }
  x[0] = (uint8_t)t;
}

/* === Clear / initialize */
static SLH_INLINE void adrs_zero(slh_var_t *var)
{
  var->adrs->u32[0] = 0;
  var->adrs->u32[1] = 0;
  var->adrs->u32[2] = 0;
  var->adrs->u32[3] = 0;
  var->adrs->u32[4] = 0;
  var->adrs->u32[5] = 0;
  var->adrs->u32[6] = 0;
  var->adrs->u32[7] = 0;
}

/* === Set layer address. */
static SLH_INLINE void adrs_set_layer_address(slh_var_t *var, uint32_t x)
{
  var->adrs->u32[0] = rev8_be32(x);
}

/* === Set tree addresss. */
static SLH_INLINE void adrs_set_tree_address(slh_var_t *var, uint64_t x)
{
  /* bytes a[4:8] of tree address are always zero */
  var->adrs->u32[2] = rev8_be32(x >> 32);
  var->adrs->u32[3] = rev8_be32(x & 0xFFFFFFFF);
}

/* === Set key pair Address. */
static SLH_INLINE void adrs_set_key_pair_address(slh_var_t *var, uint32_t x)
{
  var->adrs->u32[5] = rev8_be32(x);
}

/* === Get key pair Address. */
static SLH_INLINE uint32_t adrs_get_key_pair_address(const slh_var_t *var)
{
  return rev8_be32(var->adrs->u32[5]);
}

/* === Set FORS tree height. */
static SLH_INLINE void adrs_set_tree_height(slh_var_t *var, uint32_t x)
{
  var->adrs->u32[6] = rev8_be32(x);
}

/* === Set WOTS+ chain address. */
static SLH_INLINE void adrs_set_chain_address(slh_var_t *var, uint32_t x)
{
  var->adrs->u32[6] = rev8_be32(x);
}

/* === Set FORS tree index. */
static SLH_INLINE void adrs_set_tree_index(slh_var_t *var, uint32_t x)
{
  var->adrs->u32[7] = rev8_be32(x);
}

/* === Get FORS tree index. */
static SLH_INLINE uint32_t adrs_get_tree_index(const slh_var_t *var)
{
  return rev8_be32(var->adrs->u32[7]);
}

/* === Set WOTS+ hash address. */
static SLH_INLINE void adrs_set_hash_address(slh_var_t *var, uint32_t x)
{
  var->adrs->u32[7] = rev8_be32(x);
}

static SLH_INLINE void adrs_set_type(slh_var_t *var, uint32_t y)
{
  var->adrs->u32[4] = rev8_be32(y);
}

static SLH_INLINE uint32_t adrs_get_type(slh_var_t *var)
{
  return rev8_be32(var->adrs->u32[4]);
}

/* === "Function ADRS.setTypeAndClear(Y) for addresses sets the type */
/* of the ADRS to Y and sets the final 12 bytes of the ADRS to zero." */
static SLH_INLINE void adrs_set_type_and_clear(slh_var_t *var, uint32_t y)
{
  var->adrs->u32[4] = rev8_be32(y);
  var->adrs->u32[5] = 0;
  var->adrs->u32[6] = 0;
  var->adrs->u32[7] = 0;
}

static SLH_INLINE void adrs_set_type_and_clear_not_kp(slh_var_t *var, uint32_t y)
{
  var->adrs->u32[4] = rev8_be32(y);
  var->adrs->u32[6] = 0;
  var->adrs->u32[7] = 0;
}

/* === Compressed 22-byte address ADRSc used with SHA-2. */
static SLH_INLINE void adrsc_22(const slh_var_t *var, uint8_t *ac)
{
  int i;
  ac[0] = var->adrs->u8[3];
  for (i = 0; i < 8; i++)
  {
    ac[i + 1] = var->adrs->u8[i + 8];
  }
  for (i = 0; i < 13; i++)
  {
    ac[i + 9] = var->adrs->u8[i + 19];
  }
}

/* _SLH_ADRS_H_ */
#endif
