#ifndef LOWMC_129_129_4_H
#define LOWMC_129_129_4_H

#include "lowmc_pars.h"

#define LOWMC_129_129_4_N 129
#define LOWMC_129_129_4_M 43
#define LOWMC_129_129_4_R 4

extern const lowmc_t lowmc_129_129_4;
#define lowmc_parameters_129_129_4                                                                 \
  { LOWMC_129_129_4_N, LOWMC_129_129_4_M, LOWMC_129_129_4_R }

static const mzd_local_t mask_129_129_43_a[1] = {
    {{UINT64_C(0x8000000000000000), UINT64_C(0x4924924924924924), UINT64_C(0x2492492492492492),
      UINT64_C(0x0)}}};
static const mzd_local_t mask_129_129_43_b[1] = {
    {{UINT64_C(0x0), UINT64_C(0x9249249249249249), UINT64_C(0x4924924924924924), UINT64_C(0x0)}}};
static const mzd_local_t mask_129_129_43_c[1] = {
    {{UINT64_C(0x0), UINT64_C(0x2492492492492492), UINT64_C(0x9249249249249249), UINT64_C(0x0)}}};

#endif
