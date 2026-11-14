/*
 *  SPDX-License-Identifier: MIT
 */

#ifndef RANDOMNESS_H
#define RANDOMNESS_H

#include "macros.h"

#include <stddef.h>
#include <stdint.h>

FAEST_BEGIN_C_DECL

int rand_bytes(uint8_t* dst, size_t num_bytes);

FAEST_END_C_DECL

#endif
