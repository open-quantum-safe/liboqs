/*
 *  SPDX-License-Identifier: MIT
 */

#ifndef OWF_H
#define OWF_H

#include "macros.h"

#include <stdint.h>

FAEST_BEGIN_C_DECL

void owf_128(const uint8_t* key, const uint8_t* input, uint8_t* output);
void owf_192(const uint8_t* key, const uint8_t* input, uint8_t* output);
void owf_256(const uint8_t* key, const uint8_t* input, uint8_t* output);

void owf_em_128(const uint8_t* key, const uint8_t* input, uint8_t* output);
void owf_em_192(const uint8_t* key, const uint8_t* input, uint8_t* output);
void owf_em_256(const uint8_t* key, const uint8_t* input, uint8_t* output);

#define faest_128s_owf owf_128
#define faest_128f_owf owf_128
#define faest_192s_owf owf_192
#define faest_192f_owf owf_192
#define faest_256s_owf owf_256
#define faest_256f_owf owf_256

#define faest_em_128s_owf owf_em_128
#define faest_em_128f_owf owf_em_128
#define faest_em_192s_owf owf_em_192
#define faest_em_192f_owf owf_em_192
#define faest_em_256s_owf owf_em_256
#define faest_em_256f_owf owf_em_256

FAEST_END_C_DECL

#endif
