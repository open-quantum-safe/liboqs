/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef LOWMC_H
#define LOWMC_H

#include "lowmc_pars.h"

typedef struct {
  mzd_local_t state[(MAX_LOWMC_BLOCK_SIZE + 255) / 256];
} recorded_state_t;

// forward decleration to picnic3_types.h since we get some cyclic dependencies otherwise
typedef struct randomTape_t randomTape_t;

void lowmc_compute(const lowmc_parameters_t* params, const lowmc_key_t* key, const mzd_local_t* x,
                   mzd_local_t* y);
void lowmc_record_state(const lowmc_parameters_t* params, const lowmc_key_t* key,
                        const mzd_local_t* x, recorded_state_t* state);
void lowmc_compute_aux(const lowmc_parameters_t* params, lowmc_key_t* key, randomTape_t* tapes);

typedef void (*lowmc_implementation_f)(lowmc_key_t const*, mzd_local_t const*, mzd_local_t*);
lowmc_implementation_f lowmc_get_implementation(const lowmc_parameters_t* lowmc);

#endif
