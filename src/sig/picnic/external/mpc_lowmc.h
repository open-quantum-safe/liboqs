/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef MPC_LOWMC_H
#define MPC_LOWMC_H

#include <stdbool.h>

#include "lowmc.h"

/* Share count for proofs */
#define SC_PROOF 3
/* Share count for verification */
#define SC_VERIFY 2

typedef union {
  uint64_t t[SC_PROOF];
  mzd_local_t s[SC_PROOF];
} view_t;

typedef view_t rvec_t;

typedef struct {
  mzd_local_t s[SC_PROOF][(MAX_LOWMC_BLOCK_SIZE + 255) / 256];
} in_out_shares_t ATTR_ALIGNED(32);

typedef void (*zkbpp_lowmc_implementation_f)(mzd_local_t const*, view_t*, in_out_shares_t*, rvec_t*,
                                             recorded_state_t*);
typedef void (*zkbpp_lowmc_verify_implementation_f)(mzd_local_t const*, view_t*, in_out_shares_t*,
                                                    rvec_t*, unsigned int);
typedef void (*zkbpp_share_implementation_f)(mzd_local_t*, const mzd_local_t*, const mzd_local_t*,
                                             const mzd_local_t*);

zkbpp_lowmc_implementation_f get_zkbpp_lowmc_implementation(const lowmc_parameters_t* lowmc);
zkbpp_lowmc_verify_implementation_f get_zkbpp_lowmc_verify_implementation(const lowmc_parameters_t* lowmc);
zkbpp_share_implementation_f get_zkbpp_share_implentation(const lowmc_parameters_t* lowmc);

#endif
