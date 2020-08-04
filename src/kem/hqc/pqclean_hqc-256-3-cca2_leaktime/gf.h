#ifndef PQCLEAN_HQC2563CCA2_LEAKTIME_GF_H
#define PQCLEAN_HQC2563CCA2_LEAKTIME_GF_H

/**
 * @file gf.h
 * Header file of gf.c
 */

#include <stddef.h>
#include <stdint.h>

uint16_t PQCLEAN_HQC2563CCA2_LEAKTIME_gf_log(uint16_t elt);
uint16_t PQCLEAN_HQC2563CCA2_LEAKTIME_gf_mul(uint16_t a, uint16_t b);
uint16_t PQCLEAN_HQC2563CCA2_LEAKTIME_gf_square(uint16_t a);
uint16_t PQCLEAN_HQC2563CCA2_LEAKTIME_gf_inverse(uint16_t a);
uint16_t PQCLEAN_HQC2563CCA2_LEAKTIME_gf_mod(uint16_t i);

#endif
