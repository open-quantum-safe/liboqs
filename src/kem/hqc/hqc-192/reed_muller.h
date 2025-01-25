#ifndef HQC192_REED_MULLER_H
#define HQC192_REED_MULLER_H

/**
 * @file reed_muller.h
 * @brief Header file of reed_muller.c
 */

#include "parameters.h"
#include <stddef.h>
#include <stdint.h>

void HQC192_reed_muller_encode(uint64_t* cdw, const uint64_t* msg);
void HQC192_reed_muller_decode(uint64_t* msg, const uint64_t* cdw);

#endif
