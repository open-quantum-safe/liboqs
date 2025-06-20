/* Copyright (c) 2017, Google Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */

/***************************************************************************
* Small modification by Nir Drucker and Shay Gueron
* AWS Cryptographic Algorithms Group
* (ndrucker@amazon.com, gueron@amazon.com)
* include:
* 1) Use memcpy/memset instead of OPENSSL_memcpy/memset
* 2) Include aes.h as the underlying aes code
* 3) Modifying the drbg structure
* ***************************************************************************/

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif

#include "aes_ni.h"

// CTR_DRBG_STATE contains the state of a CTR_DRBG based on AES-256. See SP
// 800-90Ar1.
typedef struct {
  aes256_ks_t ks;
  union {
    uint8_t bytes[16];
    uint32_t words[4];
  } counter;
  uint64_t reseed_counter;
} CTR_DRBG_STATE;

// See SP 800-90Ar1, table 3.
#define CTR_DRBG_ENTROPY_LEN 48

// CTR_DRBG_init initialises |*drbg| given |CTR_DRBG_ENTROPY_LEN| bytes of
// entropy in |entropy| and, optionally, a personalization string up to
// |CTR_DRBG_ENTROPY_LEN| bytes in length. It returns one on success and zero
// on error.
int CTR_DRBG_init(CTR_DRBG_STATE *drbg,
                  const uint8_t entropy[CTR_DRBG_ENTROPY_LEN],
                  const uint8_t *personalization,
                  size_t personalization_len);

// CTR_DRBG_reseed reseeds |drbg| given |CTR_DRBG_ENTROPY_LEN| bytes of entropy
// in |entropy| and, optionally, up to |CTR_DRBG_ENTROPY_LEN| bytes of
// additional data. It returns one on success or zero on error.
int CTR_DRBG_reseed(CTR_DRBG_STATE *drbg,
                    const uint8_t entropy[CTR_DRBG_ENTROPY_LEN],
                    const uint8_t *additional_data,
                    size_t additional_data_len);

// CTR_DRBG_generate processes to up |CTR_DRBG_ENTROPY_LEN| bytes of additional
// data (if any) and then writes |out_len| random bytes to |out|. It returns one on success or
// zero on error.
int CTR_DRBG_generate(CTR_DRBG_STATE *drbg, uint8_t *out,
                      size_t out_len,
                      const uint8_t *additional_data,
                      size_t additional_data_len);

// CTR_DRBG_clear zeroises the state of |drbg|.
void CTR_DRBG_clear(CTR_DRBG_STATE *drbg);


#if defined(__cplusplus)
}  // extern C
#endif
