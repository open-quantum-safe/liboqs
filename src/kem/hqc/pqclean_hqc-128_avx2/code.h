#ifndef CODE_H
#define CODE_H


/**
 * @file code.h
 * Header file of code.c
 */

#include "parameters.h"
#include "parameters.h"
#include <stddef.h>
#include <stdint.h>

void PQCLEAN_HQC128_AVX2_code_encode(uint64_t *em, const uint64_t *message);

void PQCLEAN_HQC128_AVX2_code_decode(uint64_t *m, const uint64_t *em);


#endif
