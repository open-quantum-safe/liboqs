#ifndef REED_SOLOMON_H
#define REED_SOLOMON_H

/**
 * @file reed_solomon.h
 * @brief Header file of reed_solomon.c
 */

#include "parameters.h"
#include <stddef.h>
#include <stdint.h>

void HQC192_reed_solomon_encode(uint64_t *cdw, const uint64_t *msg);
void HQC192_reed_solomon_decode(uint64_t *msg, uint64_t *cdw);

void HQC192_compute_generator_poly(uint16_t *poly);

#endif
