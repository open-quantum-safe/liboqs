#ifndef HQC256_REED_MULLER_H
#define HQC256_REED_MULLER_H

/**
 * @file reed_muller.h
 * @brief Header file of reed_muller.c
 */

#include "parameters.h"
#include <stddef.h>
#include <stdint.h>

void HQC256_reed_muller_encode(uint64_t* cdw, const uint64_t* msg);
void HQC256_reed_muller_decode(uint64_t* msg, const uint64_t* cdw);

#endif
