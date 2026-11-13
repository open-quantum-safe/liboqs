/**
 * @file vector.h
 * @brief Header file for vector.c
 */

#ifndef HQC_VECTOR_H
#define HQC_VECTOR_H

#include <immintrin.h>
#include <stdint.h>
#include "symmetric.h"

#include "namespace.h"
#define vect_generate_random_support1 HQC_NS(vect_generate_random_support1)
#define vect_generate_random_support2 HQC_NS(vect_generate_random_support2)
#define vect_write_support_to_vector HQC_NS(vect_write_support_to_vector)
#define vect_sample_fixed_weight1 HQC_NS(vect_sample_fixed_weight1)
#define vect_sample_fixed_weight2 HQC_NS(vect_sample_fixed_weight2)
#define vect_set_random HQC_NS(vect_set_random)
#define vect_add HQC_NS(vect_add)
#define vect_compare HQC_NS(vect_compare)
#define vect_truncate HQC_NS(vect_truncate)
#define vect_print HQC_NS(vect_print)

void vect_generate_random_support1(shake256_xof_ctx *ctx, uint32_t *support, uint16_t weight);
void vect_generate_random_support2(shake256_xof_ctx *ctx, uint32_t *support, uint16_t weight);
void vect_write_support_to_vector(__m256i *v, uint32_t *support, uint16_t weight);
void vect_sample_fixed_weight1(shake256_xof_ctx *ctx, __m256i *v, uint16_t weight);
void vect_sample_fixed_weight2(shake256_xof_ctx *ctx, __m256i *v, uint16_t weight);
void vect_set_random(shake256_xof_ctx *ctx, uint64_t *v);

void vect_add(uint64_t *o, const uint64_t *v1, const uint64_t *v2, uint32_t size);
uint8_t vect_compare(const uint8_t *v1, const uint8_t *v2, uint32_t size);
void vect_truncate(uint64_t *v);

void vect_print(const uint64_t *v, const uint32_t size);

#endif  // HQC_VECTOR_H
