#ifndef REED_MULLER_H
#define REED_MULLER_H


/**
 * @file reed_muller.h
 * Header file of reed_muller.c
 */
#include "parameters.h"
#include <stddef.h>
#include <stdint.h>

void PQCLEAN_HQCRMRS128_CLEAN_reed_muller_encode(uint8_t *cdw, const uint8_t *msg);

void PQCLEAN_HQCRMRS128_CLEAN_reed_muller_decode(uint8_t *msg, const uint8_t *cdw);


#endif
