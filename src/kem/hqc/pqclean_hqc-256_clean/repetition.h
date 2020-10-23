#ifndef REPETITION_H
#define REPETITION_H


/**
 * @file repetition.h
 * @brief Header file for repetition.c
 */

#include <stdint.h>

void PQCLEAN_HQC256_CLEAN_repetition_code_encode(uint64_t *em, const uint64_t *m);

void PQCLEAN_HQC256_CLEAN_repetition_code_decode(uint64_t *m, const uint64_t *em);


#endif
