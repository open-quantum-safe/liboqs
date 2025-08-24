/**
 * @file vector.h
 * @brief Header file for vector.c
 */

#ifndef HQC_VECTOR_H
#define HQC_VECTOR_H

#if defined(__i386__) || defined(__x86_64__)
#include <immintrin.h>
#endif
#include <stdint.h>
#include "symmetric.h"

void vect_generate_random_support1(shake256_xof_ctx *ctx, uint32_t *support, uint16_t weight);
void vect_generate_random_support2(shake256_xof_ctx *ctx, uint32_t *support, uint16_t weight);
void vect_write_support_to_vector(uint64_t *v, uint32_t *support, uint16_t weight);
void vect_sample_fixed_weight1(shake256_xof_ctx *ctx, uint64_t *v, uint16_t weight);
void vect_sample_fixed_weight2(shake256_xof_ctx *ctx, uint64_t *v, uint16_t weight);
void vect_set_random(shake256_xof_ctx *ctx, uint64_t *v);

void vect_add(uint64_t *o, const uint64_t *v1, const uint64_t *v2, uint32_t size);
uint8_t vect_compare(const uint8_t *v1, const uint8_t *v2, uint32_t size);
void vect_truncate(uint64_t *v);

void vect_print(const uint64_t *v, const uint32_t size);

#endif  // HQC_VECTOR_H
