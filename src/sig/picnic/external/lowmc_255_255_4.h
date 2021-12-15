#ifndef LOWMC_255_255_4_H
#define LOWMC_255_255_4_H

#include "lowmc_pars.h"

#define LOWMC_255_255_4_N 255
#define LOWMC_255_255_4_M 85
#define LOWMC_255_255_4_R 4

extern const lowmc_t lowmc_255_255_4;
#define lowmc_parameters_255_255_4                                                                 \
  { LOWMC_255_255_4_N, LOWMC_255_255_4_M, LOWMC_255_255_4_R }

static const mzd_local_t mask_255_255_85_a[1] = {
    {{UINT64_C(0x2492492492492492), UINT64_C(0x9249249249249249), UINT64_C(0x4924924924924924),
      UINT64_C(0x2492492492492492)}}};
static const mzd_local_t mask_255_255_85_b[1] = {
    {{UINT64_C(0x4924924924924924), UINT64_C(0x2492492492492492), UINT64_C(0x9249249249249249),
      UINT64_C(0x4924924924924924)}}};
static const mzd_local_t mask_255_255_85_c[1] = {
    {{UINT64_C(0x9249249249249248), UINT64_C(0x4924924924924924), UINT64_C(0x2492492492492492),
      UINT64_C(0x9249249249249249)}}};

#endif
