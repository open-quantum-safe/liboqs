/*
 * Copyright (c) The slhdsa-c project authors
 * SPDX-License-Identifier: Apache-2.0 OR ISC OR MIT
 */

/* === FIPS 180-4 (SHA-2) -- traditional "MD" type hash API interface. */

#ifndef _SHA2_API_H_
#define _SHA2_API_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>
#include "cbmc.h"

  /* === Single-call hash wrappers === */

  /* SHA2-224: Compute 28-byte hash to "md" from "m" which has "m_sz" bytes. */
  void sha2_224(uint8_t *md, const void *m, size_t m_sz);

  /* SHA2-256: Compute 32-byte hash to "md" from "m" which has "m_sz" bytes. */
  void sha2_256(uint8_t *md, const void *m, size_t m_sz);

  /* SHA2-384: Compute 48-byte hash to "md" from "m" which has "m_sz" bytes. */
  void sha2_384(uint8_t *md, const void *m, size_t m_sz);

  /* SHA2-512: Compute 64-byte hash to "md" from "m" which has "m_sz" bytes. */
  void sha2_512(uint8_t *md, const void *m, size_t m_sz);

  /* SHA2-512/224: Compute 64-byte hash to "md" from "m" which has "m_sz" */
  /* bytes. */
  void sha2_512_224(uint8_t *md, const void *m, size_t m_sz);

  /* SHA2-512/256: Compute 64-byte hash to "md" from "m" which has "m_sz" */
  /* bytes. */
  void sha2_512_256(uint8_t *md, const void *m, size_t m_sz);

  /* === incremental interface === */

  typedef struct
  {
    uint32_t s[8 + 24];
    size_t i, len;
  } sha2_256_t;

  typedef sha2_256_t sha2_224_t;

  typedef struct
  {
    uint64_t s[8 + 24];
    size_t i, len;
  } sha2_512_t;

  typedef sha2_512_t sha2_384_t;

  /* shaNNN_init(var): Initialize context for hashing. */
  void sha2_256_init(sha2_256_t *sha)
  __contract__(
    requires(memory_no_alias(sha, sizeof(sha2_256_t)))
    assigns(object_whole(sha))
  );
  void sha2_224_init(sha2_256_t *sha);
  void sha2_512_init(sha2_512_t *sha);
  void sha2_384_init(sha2_512_t *sha);
  void sha2_512_224_init(sha2_512_t *sha);
  void sha2_512_256_init(sha2_512_t *sha);

  /* shaNNN_update(var, m, m_sz): Include "m" of "m_sz" bytes in hash. */
  void sha2_256_update(sha2_256_t *sha, const uint8_t *m, size_t m_sz);
#define sha2_224_update(sha, m, m_sz) sha2_256_update(sha, m, m_sz)
  void sha2_512_update(sha2_512_t *sha, const uint8_t *m, size_t m_sz);
#define sha2_384_update(sha, m, m_sz) sha2_512_update(sha, m, m_sz)

  /* shaNNN_final(var, h): Finalize hash to "h", and clear the state. */
  void sha2_256_final_len(sha2_256_t *sha, uint8_t *h, size_t hlen);
#define sha2_256_final(sha, h) sha2_256_final_len(sha, h, 32)
#define sha2_224_final(sha, h) sha2_256_final_len(sha, h, 28)
  void sha2_512_final_len(sha2_512_t *sha, uint8_t *h, size_t hlen);
#define sha2_512_final(sha, h) sha2_512_final_len(sha, h, 64)
#define sha2_384_final(sha, h) sha2_512_final_len(sha, h, 48)

  /* final padding */
  void sha2_256_final_pad(sha2_256_t *sha);
  void sha2_512_final_pad(sha2_512_t *sha);

  /* copy state */
  void sha2_256_copy(sha2_256_t *dst, const sha2_256_t *src);
  void sha2_512_copy(sha2_512_t *dst, const sha2_512_t *src);

  /* compression functions */
  void sha2_256_compress(void *v);
  void sha2_512_compress(void *v);

#ifdef __cplusplus
}
#endif

#endif /* _SHA2_API_H_ */
