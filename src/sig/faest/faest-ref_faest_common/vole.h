/*
 *  SPDX-License-Identifier: MIT
 */

#ifndef FAEST_VOLE_H
#define FAEST_VOLE_H

#include <stdbool.h>

#include "bavc.h"

FAEST_BEGIN_C_DECL

void vole_commit(const uint8_t* rootKey, const uint8_t* iv, unsigned int ellhat,
                 const faest_paramset_t* params, bavc_t* bavc, uint8_t* c, uint8_t* c_mult,
                 uint8_t* u, uint8_t** v, uint8_t* u_bar, uint8_t* v_bar);

bool vole_reconstruct(uint8_t* com, uint8_t** q, const uint8_t* iv, const uint8_t* chall_3,
                      const uint8_t* decom_i, const uint8_t* c, const uint8_t* c_mult,
                      uint8_t* q_bar, uint8_t* Delta, unsigned int ellhat,
                      const faest_paramset_t* params);

#if defined(FAEST_TESTS)
unsigned int convert_to_vole(const uint8_t* iv, const uint8_t* sd, bool sd0_bot, unsigned int i,
                             unsigned int outLenBytes, uint8_t* u, uint8_t* v,
                             const faest_paramset_t* params);
#endif

FAEST_END_C_DECL

#endif
