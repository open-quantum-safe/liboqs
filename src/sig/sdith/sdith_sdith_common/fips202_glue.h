#ifndef FIPS202_GLUE_H
#define FIPS202_GLUE_H

#include <oqs/sha3.h>

typedef xof_ctx Keccak_HashInstance; // defined in sdith_prng.h

static inline void Keccak_HashInitialize_SHAKE128(Keccak_HashInstance* inst) {
  inst->shake = 128;
  OQS_SHA3_shake128_inc_init(&inst->shake128_state);
}

static inline void Keccak_HashInitialize_SHAKE256(Keccak_HashInstance* inst) {
  inst->shake = 256;
  OQS_SHA3_shake256_inc_init(&inst->shake256_state);
}

// XKCP lengths are in bits, liboqs lengths are in bytes
static inline void Keccak_HashUpdate(Keccak_HashInstance* inst, const void* data, uint64_t databitlen) {
  if (inst->shake == 128) {
    OQS_SHA3_shake128_inc_absorb(&inst->shake128_state, data, databitlen >> 3);
  } else {
    OQS_SHA3_shake256_inc_absorb(&inst->shake256_state, data, databitlen >> 3);
  }
}

static inline void Keccak_HashFinal(Keccak_HashInstance* inst, void* hashval) {
  (void)hashval;  // XKCP ignores it for SHAKE
  if (inst->shake == 128) {
    OQS_SHA3_shake128_inc_finalize(&inst->shake128_state);
  } else {
    OQS_SHA3_shake256_inc_finalize(&inst->shake256_state);
  }
}

static inline void Keccak_HashSqueeze(Keccak_HashInstance* inst, void* data, uint64_t databitlen) {
  if (inst->shake == 128) {
    OQS_SHA3_shake128_inc_squeeze(data, databitlen >> 3, &inst->shake128_state);
  } else {
    OQS_SHA3_shake256_inc_squeeze(data, databitlen >> 3, &inst->shake256_state);
  }
}

#endif
