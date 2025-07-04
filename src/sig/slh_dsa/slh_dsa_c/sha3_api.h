/*
 * Copyright (c) The slhdsa-c project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* === FIPS 202: SHA-3 hash and SHAKE eXtensible Output Functions (XOF) */

#ifndef _SHA3_API_H_
#define _SHA3_API_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>

  typedef struct
  { /* state context */
    union
    {
      uint8_t b[200]; /* 8-bit bytes */
      uint64_t d[25]; /* 64-bit words */
    } st;
    size_t pt, r_sz, md_sz;
  } sha3_var_t;

  /* compute a hash "md" of "md_sz" bytes from data in "in" */
  void sha3(uint8_t *md, size_t md_sz, const void *in, size_t in_sz);

  /* incremental interfece */
  void sha3_init(sha3_var_t *c,
                 size_t md_sz); /* md_sz = hash output in bytes */
  void sha3_update(sha3_var_t *c, const void *data, size_t data_sz);
  void sha3_final(sha3_var_t *c, uint8_t *md); /* digest goes to md */

/* SHAKE128 and SHAKE256 extensible-output functions */
#define shake128_init(c) sha3_init(c, 16)
#define shake256_init(c) sha3_init(c, 32)
#define shake_update sha3_update

  /* compute a hash "md" of "md_sz" bytes from data in "in" */
  void shake(uint8_t *md, size_t md_sz, const void *in, size_t in_sz,
             size_t r_sz);
#define shake128(md, md_sz, in, in_sz) shake(md, md_sz, in, in_sz, 16)
#define shake256(md, md_sz, in, in_sz) shake(md, md_sz, in, in_sz, 32)

  /* squeeze output (can call repeat) */
  void shake_out(sha3_var_t *c, uint8_t *out, size_t out_sz);

  /* core permutation */
  void keccak_f1600(uint64_t x[25]);

#ifdef __cplusplus
}
#endif

#endif /* _SHA3_API_H_ */
