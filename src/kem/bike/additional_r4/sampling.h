/* Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0"
 *
 * Written by Nir Drucker, Shay Gueron and Dusan Kostic,
 * AWS Cryptographic Algorithms Group.
 */

#pragma once

#if defined(USE_NIST_RAND)
#  include "../../tests/FromNIST/rng.h"
#else
#  include <stdlib.h>
#endif

#include "prf_internal.h"
#include "utilities.h"

typedef enum
{
  NO_RESTRICTION = 0,
  MUST_BE_ODD    = 1
} must_be_odd_t;

void get_seeds(OUT seeds_t *seeds);

ret_t generate_secret_key(OUT pad_r_t *h0, OUT pad_r_t *h1,
                          OUT idx_t *h0_wlist, OUT idx_t *h1_wlist,
                          IN const seed_t *seed);

ret_t generate_error_vector(OUT pad_e_t *e, IN const seed_t *seed);
