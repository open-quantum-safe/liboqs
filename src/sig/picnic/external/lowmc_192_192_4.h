#ifndef LOWMC_192_192_4_H
#define LOWMC_192_192_4_H

#include "lowmc_pars.h"

#define LOWMC_192_192_4_N 192
#define LOWMC_192_192_4_M 64
#define LOWMC_192_192_4_R 4

extern const lowmc_t lowmc_192_192_4;
#define lowmc_parameters_192_192_4                                                                 \
  { LOWMC_192_192_4_N, LOWMC_192_192_4_M, LOWMC_192_192_4_R }

static const mzd_local_t mask_192_192_64_a[1] = {
    {{UINT64_C(0x9249249249249249), UINT64_C(0x4924924924924924), UINT64_C(0x2492492492492492),
      UINT64_C(0x0)}}};
static const mzd_local_t mask_192_192_64_b[1] = {
    {{UINT64_C(0x2492492492492492), UINT64_C(0x9249249249249249), UINT64_C(0x4924924924924924),
      UINT64_C(0x0)}}};
static const mzd_local_t mask_192_192_64_c[1] = {
    {{UINT64_C(0x4924924924924924), UINT64_C(0x2492492492492492), UINT64_C(0x9249249249249249),
      UINT64_C(0x0)}}};

#endif
