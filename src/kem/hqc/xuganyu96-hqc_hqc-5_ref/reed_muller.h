/**
 * @file reed_muller.h
 * @brief Header file of reed_muller.c
 */

#ifndef HQC_REED_MULLER_H
#define HQC_REED_MULLER_H

#include <stddef.h>
#include <stdint.h>
#include "parameters.h"

void reed_muller_encode(uint64_t* cdw, const uint64_t* msg);
void reed_muller_decode(uint64_t* msg, const uint64_t* cdw);

#endif  // HQC_REED_MULLER_H
