/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef PICNIC2_SIMULATE_H
#define PICNIC2_SIMULATE_H

#include "lowmc_pars.h"

typedef struct randomTape_t randomTape_t;
typedef struct shares_t shares_t;
typedef struct msgs_t msgs_t;
typedef struct picnic_instance_t picnic_instance_t;

typedef int (*lowmc_simulate_online_f)(mzd_local_t* maskedKey, shares_t* mask_shares,
                                       randomTape_t* tapes, msgs_t* msgs, const mzd_local_t* plaintext,
                                       const uint32_t* pubKey, const picnic_instance_t* params);

lowmc_simulate_online_f lowmc_simulate_online_get_implementation(const lowmc_t* lowmc);

#endif
