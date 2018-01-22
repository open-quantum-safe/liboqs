/*
 *  This file is part of the optimized implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#include "timing.h"

#ifdef WITH_DETAILED_TIMING
static timing_and_size_t storage;
timing_and_size_t* timing_and_size = &storage;
#endif
