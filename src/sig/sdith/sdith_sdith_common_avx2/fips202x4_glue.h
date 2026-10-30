#ifndef FIPS202X4_GLUE_H
#define FIPS202X4_GLUE_H

#include <assert.h>

#include <oqs/sha3.h>
#include <oqs/sha3x4.h>

typedef struct {
  uint32_t shake;  // 128 or 256, set by SpongeInitialize
  union {
    OQS_SHA3_shake128_inc_ctx shake128_state;
    OQS_SHA3_shake256_inc_ctx shake256_state;
  };
} KeccakWidth1600_SpongeInstance;

typedef struct {
  uint32_t shake;  // 128 or 256, set by SpongeInitialize
  union {
    OQS_SHA3_shake128_x4_inc_ctx shake128_state;
    OQS_SHA3_shake256_x4_inc_ctx shake256_state;
  };
} KeccakWidth1600times4_SpongeInstance;


static inline void KeccakWidth1600_SpongeInitialize(KeccakWidth1600_SpongeInstance* inst, unsigned int rate,
                                                    unsigned int capacity) {
  (void)rate;  // only read by the asserts, which NDEBUG removes
  inst->shake = capacity / 2; // SHAKE128 has capacity 256 and SHAKE256 has capacity 512
  if (inst->shake == 128) {
    assert(rate == (OQS_SHA3_SHAKE128_RATE << 3));  // XKCP rates are in bits, liboqs rates in bytes
    OQS_SHA3_shake128_inc_init(&inst->shake128_state);
  } else {
    assert(rate == (OQS_SHA3_SHAKE256_RATE << 3));
    OQS_SHA3_shake256_inc_init(&inst->shake256_state);
  }
}

static inline void KeccakWidth1600_SpongeAbsorb(KeccakWidth1600_SpongeInstance* inst, const unsigned char* data,
                                                size_t dataByteLen) {
  if (inst->shake == 128) {
    OQS_SHA3_shake128_inc_absorb(&inst->shake128_state, data, dataByteLen);
  } else {
    OQS_SHA3_shake256_inc_absorb(&inst->shake256_state, data, dataByteLen);
  }
}

// sdith always passes 0x1F (the SHAKE padding), which liboqs's finalize applies
static inline void KeccakWidth1600_SpongeAbsorbLastFewBits(KeccakWidth1600_SpongeInstance* inst,
                                                           unsigned char delimitedData) {
  (void)delimitedData; // liboqs finalize applies the delimiter already
  if (inst->shake == 128) {
    OQS_SHA3_shake128_inc_finalize(&inst->shake128_state);
  } else {
    OQS_SHA3_shake256_inc_finalize(&inst->shake256_state);
  }
}

static inline void KeccakWidth1600_SpongeSqueeze(KeccakWidth1600_SpongeInstance* inst, unsigned char* data,
                                                 size_t dataByteLen) {
  if (inst->shake == 128) {
    OQS_SHA3_shake128_inc_squeeze(data, dataByteLen, &inst->shake128_state);
  } else {
    OQS_SHA3_shake256_inc_squeeze(data, dataByteLen, &inst->shake256_state);
  }
}

static inline void KeccakWidth1600times4_SpongeInitialize(KeccakWidth1600times4_SpongeInstance* inst,
                                                          unsigned int rate, unsigned int capacity) {
  (void)rate;  // implied by capacity
  inst->shake = capacity / 2; // SHAKE128 has capacity 256 and SHAKE256 has capacity 512
  if (inst->shake == 128) {
    assert(rate == (OQS_SHA3_SHAKE128_RATE << 3));  // XKCP in bits, liboqs in bytes
    OQS_SHA3_shake128_x4_inc_init(&inst->shake128_state);
  } else {
    assert(rate == (OQS_SHA3_SHAKE256_RATE << 3));
    OQS_SHA3_shake256_x4_inc_init(&inst->shake256_state);
  }
}

static inline void KeccakWidth1600times4_SpongeAbsorb(KeccakWidth1600times4_SpongeInstance* inst,
                                                      const unsigned char** data, size_t dataByteLen) {
  if (inst->shake == 128) {
    OQS_SHA3_shake128_x4_inc_absorb(&inst->shake128_state, data[0], data[1], data[2], data[3], dataByteLen);
  } else {
    OQS_SHA3_shake256_x4_inc_absorb(&inst->shake256_state, data[0], data[1], data[2], data[3], dataByteLen);
  }
}

static inline void KeccakWidth1600times4_SpongeAbsorbLastFewBits(KeccakWidth1600times4_SpongeInstance* inst,
                                                                 unsigned char delimitedData) {
  (void)delimitedData; // liboqs finalize applies the delimiter already
  if (inst->shake == 128) {
    OQS_SHA3_shake128_x4_inc_finalize(&inst->shake128_state);
  } else {
    OQS_SHA3_shake256_x4_inc_finalize(&inst->shake256_state);
  }
}

static inline void KeccakWidth1600times4_SpongeSqueeze(KeccakWidth1600times4_SpongeInstance* inst,
                                                       unsigned char** data, size_t dataByteLen) {
  if (inst->shake == 128) {
    OQS_SHA3_shake128_x4_inc_squeeze(data[0], data[1], data[2], data[3], dataByteLen, &inst->shake128_state);
  } else {
    OQS_SHA3_shake256_x4_inc_squeeze(data[0], data[1], data[2], data[3], dataByteLen, &inst->shake256_state);
  }
}

#endif
