#ifndef CODE_H
#define CODE_H


/**
 * @file code.h
 * Header file of code.c
 */
#include "parameters.h"
#include <stddef.h>
#include <stdint.h>

void PQCLEAN_HQCRMRS256_CLEAN_code_encode(uint8_t *em, const uint8_t *message);

void PQCLEAN_HQCRMRS256_CLEAN_code_decode(uint8_t *m, const uint8_t *em);


#endif
