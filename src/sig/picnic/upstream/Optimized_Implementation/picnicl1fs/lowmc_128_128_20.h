#ifndef LOWMC_128_128_20_H
#define LOWMC_128_128_20_H

#include <stdint.h>

#include "mzd_additional.h"

const mzd_local_t* lowmc_128_128_20_get_linear_layer(uint32_t r);
const mzd_local_t* lowmc_128_128_20_get_round_const(uint32_t r);
const mzd_local_t* lowmc_128_128_20_get_precomputed_round_key_matrix_non_linear_part(void);
const mzd_local_t* lowmc_128_128_20_get_precomputed_round_key_matrix_linear_part(void);

#endif
