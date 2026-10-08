#include <string.h>

#include "aes_ansi_ref.h"

#include <oqs/aes.h>
#include <oqs/common.h>


EXPORT void aes128_set_key_ref(void* rk, const void* key128) {
  // Allocates a schedule via liboqs and writes the pointer to the schedule into the rk buffer
  void* schedule = NULL;
  OQS_AES128_ECB_load_schedule(key128, &schedule);
  memcpy(rk, &schedule, sizeof(void*));
}

EXPORT void aes128_encrypt_1block_ref(void* out_ct128, const void* in_pt128, const void* rk) {
  // Interprets first sizeof(void *) bytes of rk as a schedule pointer for liboqs ctx
  void* schedule = NULL;
  memcpy(&schedule, rk, sizeof(void*));
  OQS_AES128_ECB_enc_sch(in_pt128, 16, schedule, out_ct128);
}

EXPORT void aes128_prepare_rk_buffer(void* rk, uint64_t nslots, uint64_t slot_bytes) {
  // The key is a placeholder: each slot is re-keyed by aes128_set_key_reuse_ref before any use.
  static const uint8_t zero_key[16] = {0};
  uint8_t* p = (uint8_t*) rk;
  for (uint64_t i = 0; i < nslots; ++i) {
    void* schedule = NULL;
    OQS_AES128_ECB_load_schedule(zero_key, &schedule);
    memcpy(p + i * slot_bytes, &schedule, sizeof(void*));
  }
}

EXPORT void aes128_set_key_reuse_ref(void* rk, const void* key128) {
  // Interprets first sizeof(void *) bytes of rk as a schedule pointer for liboqs ctx.
  // aes128_prepare_rk_buffer has put a schedule there, so this is always a re-key.
  void* schedule = NULL;
  memcpy(&schedule, rk, sizeof(void*));
  OQS_AES128_ECB_rekey(key128, schedule);
}


EXPORT void aes128_release_key(void* rk) {
  // Interprets first sizeof(void *) bytes of rk as a schedule pointer for liboqs ctx
  void* schedule = NULL;
  memcpy(&schedule, rk, sizeof(void*));
  OQS_AES128_free_schedule(schedule);
  OQS_MEM_cleanse(rk, sizeof(void*));
}
