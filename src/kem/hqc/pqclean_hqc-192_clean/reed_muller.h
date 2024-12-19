#ifndef REED_MULLER_H
#define REED_MULLER_H


/**
 * @file reed_muller.h
 * @brief Header file of reed_muller.c
 */

#include <stdint.h>

void PQCLEAN_HQC192_CLEAN_reed_muller_encode(uint64_t *cdw, const uint8_t *msg);

void PQCLEAN_HQC192_CLEAN_reed_muller_decode(uint8_t *msg, const uint64_t *cdw);


#endif
