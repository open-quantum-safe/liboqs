#include <string.h>

#include "rijndael256.h"
#include "rijndael256_ctrle.h"

#if defined(_WIN32) || defined(__APPLE__)
#define __always_inline inline __attribute((always_inline))
#endif

/* -------- batched key schedule (ref: just loop the scalar x1) -------- */
EXPORT void rijndael256_key_schedule_x1_ref(void* rk0, const void* k0) { rijndael256_key_schedule_ref(rk0, k0); }
EXPORT void rijndael256_key_schedule_x2_ref(void* rk0, void* rk1, const void* k0, const void* k1) {
  rijndael256_key_schedule_ref(rk0, k0);
  rijndael256_key_schedule_ref(rk1, k1);
}
EXPORT void rijndael256_key_schedule_x3_ref(void* rk0, void* rk1, void* rk2,  //
                                            const void* k0, const void* k1, const void* k2) {
  rijndael256_key_schedule_ref(rk0, k0);
  rijndael256_key_schedule_ref(rk1, k1);
  rijndael256_key_schedule_ref(rk2, k2);
}
EXPORT void rijndael256_key_schedule_x4_ref(void* rk0, void* rk1, void* rk2, void* rk3,  //
                                            const void* k0, const void* k1, const void* k2, const void* k3) {
  rijndael256_key_schedule_ref(rk0, k0);
  rijndael256_key_schedule_ref(rk1, k1);
  rijndael256_key_schedule_ref(rk2, k2);
  rijndael256_key_schedule_ref(rk3, k3);
}

/* -------- no-carry CTR encryption (ref) --------
 * block i uses the counter with only its low 64 bits incremented by i; no carry
 * into the upper bits (caller's guarantee). */
static inline void rijndael256_nocarry_ref_impl(void* out, const void* round_keys, const void* ctr_block,
                                                uint64_t nblocks) {
  ctr256_t ctr;
  uint8_t* oo = (uint8_t*)out;
  memcpy(ctr.v8, ctr_block, 32);
  for (uint64_t i = 0; i < nblocks; ++i) {
    rijndael256_encrypt_1block_ref(oo + 32 * i, ctr.v8, (const rijndael256_rk_t*)round_keys);
    ctr.v64[0] += 1;  // low-64 increment only, no middle carry
  }
}
EXPORT void rijndael256_ctrle_nocarry_1block_ref(void* out, const void* round_keys, const void* ctr_block) {
  rijndael256_nocarry_ref_impl(out, round_keys, ctr_block, 1);
}
EXPORT void rijndael256_ctrle_nocarry_2block_ref(void* out, const void* round_keys, const void* ctr_block) {
  rijndael256_nocarry_ref_impl(out, round_keys, ctr_block, 2);
}
EXPORT void rijndael256_ctrle_nocarry_nblocks_ref(void* out, const void* round_keys, const void* ctr_block,
                                                  uint64_t nblocks) {
  rijndael256_nocarry_ref_impl(out, round_keys, ctr_block, nblocks);
}
