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
  mzd_local_t** state;
} recorded_state_t;

// forward decleration to picnic2_types.h since we get some cyclic dependencies otherwise
typedef struct randomTape_t randomTape_t;

typedef void (*lowmc_implementation_f)(lowmc_key_t const*, mzd_local_t const*, mzd_local_t*);
typedef void (*lowmc_store_implementation_f)(lowmc_key_t const*, mzd_local_t const*,
                                             recorded_state_t* state);
typedef void (*lowmc_compute_aux_implementation_f)(lowmc_key_t const*, randomTape_t* tapes);

lowmc_implementation_f lowmc_get_implementation(const lowmc_t* lowmc);
lowmc_store_implementation_f lowmc_store_get_implementation(const lowmc_t* lowmc);
lowmc_compute_aux_implementation_f lowmc_compute_aux_get_implementation(const lowmc_t* lowmc);

#endif
