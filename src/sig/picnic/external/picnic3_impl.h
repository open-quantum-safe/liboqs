/*! @file picnic3_impl.h
 *  @brief This is the main implementation file of the signature scheme for
 *  the Picnic3 parameter sets.
 *
 *  This file is part of the reference implementation of the Picnic signature scheme.
 *  See the accompanying documentation for complete details.
 *
 *  The code is provided under the MIT license, see LICENSE for
 *  more details.
 *  SPDX-License-Identifier: MIT
 */

#ifndef PICNIC3_IMPL_H
#define PICNIC3_IMPL_H

#include <stdint.h>
#include <stddef.h>
#include "picnic_instances.h"

int impl_sign_picnic3(const picnic_instance_t* pp, const uint8_t* plaintext,
                      const uint8_t* private_key, const uint8_t* public_key, const uint8_t* msg,
                      size_t msglen, uint8_t* sig, size_t* siglen);
int impl_verify_picnic3(const picnic_instance_t* instance, const uint8_t* plaintext,
                        const uint8_t* public_key, const uint8_t* msg, size_t msglen,
                        const uint8_t* signature, size_t signature_len);

#endif /* PICNIC3_IMPL_H */
