#ifndef AES_ANSI_REF_H
#define AES_ANSI_REF_H

#ifdef __cplusplus
#define EXPORT extern "C"
#include <cstdint>
#else
#define EXPORT
#include "stdint.h"
#endif

/* aes128 reference implementation of the key scheduling (warning:big endian round keys) */
EXPORT void aes128_set_key_ref(void* rk, const void* key128);
/* aes128 reference implementation of the encryption (1 single block) */
EXPORT void aes128_encrypt_1block_ref(void* out_ct128, const void* in_pt128, const void* rk);

#endif  // AES_ANSI_REF_H
