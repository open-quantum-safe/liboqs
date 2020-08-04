#ifndef PQCLEAN_HQC2563CCA2_LEAKTIME_TENSOR_H
#define PQCLEAN_HQC2563CCA2_LEAKTIME_TENSOR_H

/**
 * @file tensor.h
 * @brief Header file for tensor.c
 */

#include <stdint.h>

void PQCLEAN_HQC2563CCA2_LEAKTIME_tensor_code_encode(uint8_t *em, const uint8_t *m);
void PQCLEAN_HQC2563CCA2_LEAKTIME_tensor_code_decode(uint8_t *m, const uint8_t *em);

#endif
