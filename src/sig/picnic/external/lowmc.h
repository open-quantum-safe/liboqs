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

typedef mzd_local_t* (*lowmc_implementation_f)(lowmc_key_t const*, mzd_local_t const*);
typedef void (*lowmc_store_implementation_f)(lowmc_key_t const*, mzd_local_t const*,
                                             recorded_state_t* state);

lowmc_implementation_f oqs_sig_picnic_lowmc_get_implementation(const lowmc_t* lowmc);
lowmc_store_implementation_f oqs_sig_picnic_lowmc_store_get_implementation(const lowmc_t* lowmc);

#endif
