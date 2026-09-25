/*
 *  SPDX-License-Identifier: MIT
 */

#ifndef FAEST_IMPL_H
#define FAEST_IMPL_H

#include <stdint.h>
#include <stddef.h>

#include "instances.h"

void faest_sign(uint8_t* sig, const uint8_t* msg, size_t msglen, const uint8_t* owf_key,
                const uint8_t* owf_input, const uint8_t* owf_output, const uint8_t* witness,
                const uint8_t* rho, size_t rholen, const faest_paramset_t* params);

int faest_verify(const uint8_t* msg, size_t msglen, const uint8_t* sig, const uint8_t* owf_input,
                 const uint8_t* owf_output, const faest_paramset_t* params);

#endif
