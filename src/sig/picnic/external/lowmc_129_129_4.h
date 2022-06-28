#ifndef LOWMC_129_129_4_H
#define LOWMC_129_129_4_H

#include "lowmc_pars.h"

#define LOWMC_129_129_4_N 129
#define LOWMC_129_129_4_M 43
#define LOWMC_129_129_4_R 4

extern const lowmc_t lowmc_129_129_4;
#define lowmc_parameters_129_129_4                                                                 \
  { LOWMC_129_129_4_N, LOWMC_129_129_4_M, LOWMC_129_129_4_R }

#define MASK_129_129_43_A_0 UINT64_C(0x8000000000000000)
#define MASK_129_129_43_A_1 UINT64_C(0x4924924924924924)
#define MASK_129_129_43_A_2 UINT64_C(0x2492492492492492)
#define MASK_129_129_43_A_3 UINT64_C(0x0)

#define MASK_129_129_43_B_0 UINT64_C(0x0)
#define MASK_129_129_43_B_1 UINT64_C(0x9249249249249249)
#define MASK_129_129_43_B_2 UINT64_C(0x4924924924924924)
#define MASK_129_129_43_B_3 UINT64_C(0x0)

#define MASK_129_129_43_C_0 UINT64_C(0x0)
#define MASK_129_129_43_C_1 UINT64_C(0x2492492492492492)
#define MASK_129_129_43_C_2 UINT64_C(0x9249249249249249)
#define MASK_129_129_43_C_3 UINT64_C(0x0)

static const mzd_local_t mask_129_129_43_a[1] = {
    {{MASK_129_129_43_A_0, MASK_129_129_43_A_1, MASK_129_129_43_A_2, MASK_129_129_43_A_3}}};
static const mzd_local_t mask_129_129_43_b[1] = {
    {{MASK_129_129_43_B_0, MASK_129_129_43_B_1, MASK_129_129_43_B_2, MASK_129_129_43_B_3}}};
static const mzd_local_t mask_129_129_43_c[1] = {
    {{MASK_129_129_43_C_0, MASK_129_129_43_C_1, MASK_129_129_43_C_2, MASK_129_129_43_C_3}}};

#endif
