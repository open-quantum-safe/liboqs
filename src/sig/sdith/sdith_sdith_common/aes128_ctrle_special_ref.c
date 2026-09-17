#include <string.h>

#include "aes128_ctrle.h"
#include "aes_ansi_ref.h"


/* -------- batched key schedule (ref: just loop the scalar x1) -------- */
EXPORT void aes128_key_schedule_x1_ref(void* rk0, const void* k0) { aes128_set_key_ref(rk0, k0); }
EXPORT void aes128_key_schedule_x2_ref(void* rk0, void* rk1, const void* k0, const void* k1) {
  aes128_set_key_ref(rk0, k0);
  aes128_set_key_ref(rk1, k1);
}
EXPORT void aes128_key_schedule_x3_ref(void* rk0, void* rk1, void* rk2, const void* k0, const void* k1,
                                       const void* k2) {
  aes128_set_key_ref(rk0, k0);
  aes128_set_key_ref(rk1, k1);
  aes128_set_key_ref(rk2, k2);
}
EXPORT void aes128_key_schedule_x4_ref(void* rk0, void* rk1, void* rk2, void* rk3,  //
                                       const void* k0, const void* k1, const void* k2, const void* k3) {
  aes128_set_key_ref(rk0, k0);
  aes128_set_key_ref(rk1, k1);
  aes128_set_key_ref(rk2, k2);
  aes128_set_key_ref(rk3, k3);
}

/* -------- no-carry CTR encryption (ref) --------
 * block i uses the counter with only its low 64 bits incremented by i; there is
 * no carry into the upper 64 bits (that is the caller's guarantee). */
static inline void aes128_nocarry_ref_impl(void* out, const void* round_keys, const void* ctr_block, uint64_t nblocks) {
  ctr128_t ctr;
  uint8_t* oo = (uint8_t*)out;
  memcpy(ctr.v8, ctr_block, 16);
  for (uint64_t i = 0; i < nblocks; ++i) {
    aes128_encrypt_1block_ref(oo + 16 * i, ctr.v8, round_keys);
    ctr.v64[0] += 1;  // low-64 increment only, no middle carry
  }
}
EXPORT void aes128_ctrle_nocarry_1block_ref(void* out, const void* round_keys, const void* ctr_block) {
  aes128_nocarry_ref_impl(out, round_keys, ctr_block, 1);
}
EXPORT void aes128_ctrle_nocarry_2block_ref(void* out, const void* round_keys, const void* ctr_block) {
  aes128_nocarry_ref_impl(out, round_keys, ctr_block, 2);
}
EXPORT void aes128_ctrle_nocarry_3blocks_ref(void* out, const void* round_keys, const void* ctr_block) {
  aes128_nocarry_ref_impl(out, round_keys, ctr_block, 3);
}
EXPORT void aes128_ctrle_nocarry_4blocks_ref(void* out, const void* round_keys, const void* ctr_block) {
  aes128_nocarry_ref_impl(out, round_keys, ctr_block, 4);
}
EXPORT void aes128_ctrle_nocarry_nblocks_ref(void* out, const void* round_keys, const void* ctr_block,
                                             uint64_t nblocks) {
  aes128_nocarry_ref_impl(out, round_keys, ctr_block, nblocks);
}
