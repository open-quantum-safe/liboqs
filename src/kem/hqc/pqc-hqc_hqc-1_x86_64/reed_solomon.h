/**
 * @file reed_solomon.h
 * @brief Header file of reed_solomon.c
 */

#ifndef HQC_REED_SOLOMON_H
#define HQC_REED_SOLOMON_H

#include <stddef.h>
#include <stdint.h>
#include "parameters.h"

void reed_solomon_encode(uint64_t *cdw, const uint64_t *msg);
void reed_solomon_decode(uint64_t *msg, uint64_t *cdw);

void compute_generator_poly(uint16_t *poly);

#endif  // HQC_REED_SOLOMON_H
