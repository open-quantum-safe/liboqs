#ifndef CODE_H
#define CODE_H

/**
 * @file code.h
 * @brief Header file of code.c
 */

#include <stdint.h>

void PQCLEAN_HQC128_CLEAN_code_encode(uint64_t *em, const uint8_t *message);

void PQCLEAN_HQC128_CLEAN_code_decode(uint8_t *m, const uint64_t *em);

#endif
