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
  // Marks every slot empty by zeroing its schedule pointer.
  // No schedule is allocated here: `aes128_set_key_reuse_ref` does this in a later call
  uint8_t* p = (uint8_t*) rk;
  for (uint64_t i = 0; i < nslots; ++i) {
    memset(p + i * slot_bytes, 0, sizeof(void*));
  }
}

EXPORT void aes128_set_key_reuse_ref(void* rk, const void* key128) {
  // The slot holds a schedule rather than round keys, so replacing the key means
  // freeing any schedule already there and loading a new one in its place.
  aes128_release_key(rk);
  aes128_set_key_ref(rk, key128);
}


EXPORT void aes128_release_key(void* rk) {
  // Interprets first sizeof(void *) bytes of rk as a schedule pointer for liboqs ctx
  void* schedule = NULL;
  memcpy(&schedule, rk, sizeof(void*));
  // An empty slot has nothing to free.
  if (schedule == NULL) return;
  OQS_AES128_free_schedule(schedule);
  OQS_MEM_cleanse(rk, sizeof(void*));
}
