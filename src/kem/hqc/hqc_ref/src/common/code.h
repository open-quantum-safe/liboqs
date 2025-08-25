/**
 * @file code.h
 * @brief Header file of code.c
 */

#ifndef HQC_CODE_H
#define HQC_CODE_H

#include <stddef.h>
#include <stdint.h>
#include "parameters.h"

void code_encode(uint64_t *em, const uint64_t *m);
void code_decode(uint64_t *m, const uint64_t *em);

#endif  // HQC_CODE_H
