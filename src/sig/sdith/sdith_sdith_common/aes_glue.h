#ifndef AES_GLUE_H
#define AES_GLUE_H

#include <string.h>

#include <oqs/aes.h>

// liboqs keeps AES key schedules in heap contexts, so a round-key buffer holds only the raw 16-byte key.
// This is expanded by aes128_nocarry_ref_impl once per call.
static inline void aes128_set_key_ref(void* rk, const void* key128) { memcpy(rk, key128, 16); }

#endif
