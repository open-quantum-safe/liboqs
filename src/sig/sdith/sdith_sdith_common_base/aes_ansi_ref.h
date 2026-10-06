#ifndef AES_ANSI_REF_H
#define AES_ANSI_REF_H

/* liboqs links one shared copy of this code into every scheme variant, so the
 * internal symbols need a namespace of their own. See gen_namespace.sh. */
#ifdef SDITH3_FOR_LIBOQS
#include "sdith_namespace.h"
#endif

#ifdef __cplusplus
#define EXPORT extern "C"
#include <cstdint>
#else
#define EXPORT
#include "stdint.h"
#endif

/* References implimentations */

/* aes128 reference implementation of the key scheduling (warning:big endian round keys) */
EXPORT void aes128_set_key_ref(void* rk, const void* key128);
/* aes128 reference implementation of the encryption (1 single block) */
EXPORT void aes128_encrypt_1block_ref(void* out_ct128, const void* in_pt128, const void* rk);

/* LIBOQS integration glue */
// The below functions are either no-ops or just call the above functions when not building for liboqs.

// LIBOQS uses a heap-allocated schedule for each key. To prevent a large amount
// of churn, a schedule single pointer is written into each round-key buffer,
// and this schedule is re-keyed (without a free/alloc) when the buffer is
// reused. The following functions help to prepare, reuse, and release the
// round-key buffers.

/* Prepare an nslots x slot_bytes array of round-key buffers.
 *
 * Allocates one liboqs schedule per slot and writes its pointer into
 * the first sizeof(void *) bytes of that slot. This allows each of the
 * thousands of aes128_set_key_reuse_ref calls to re-key without re-allocating.
 */
EXPORT void aes128_prepare_rk_buffer(void* rk, uint64_t nslots, uint64_t slot_bytes);

/* Re-key the schedule held by a prepared buffer.
 *
 * @warning The buffer MUST have been prepared with aes128_prepare_rk_buffer first.
 */
EXPORT void aes128_set_key_reuse_ref(void* rk, const void* key128);

/* Release a key schedule created by aes128_set_key_ref or aes128_set_key_reuse_ref. */
EXPORT void aes128_release_key(void *rk);

#endif  // AES_ANSI_REF_H
