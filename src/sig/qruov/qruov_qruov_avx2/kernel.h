#ifndef AVX2_KERNEL_H
#define AVX2_KERNEL_H

#include <stdint.h>
#include "qruov.h"

void matmulfql_vnni_1x8(uint8_t *out, const uint8_t *A, const uint8_t *Bvnni, int K,
        int incout, int incA, int incB, int incLout, int incLA, int incLB);
void matmulfql_vnni_1x7(uint8_t *out, const uint8_t *A, const uint8_t *Bvnni, int K,
        int incout, int incA, int incB, int incLout, int incLA, int incLB);
void matmulfql_vnni_1x6(uint8_t *out, const uint8_t *A, const uint8_t *Bvnni, int K,
        int incout, int incA, int incB, int incLout, int incLA, int incLB);
void matmulfql_vnni_1x4(uint8_t *out, const uint8_t *A, const uint8_t *Bvnni, int K,
        int incout, int incA, int incB, int incLout, int incLA, int incLB);

void matmulfql_symmetric_upper(uint8_t *output_soa, const uint8_t *vector_soa, int size);

void matmulfq_vnni_4x16(uint8_t *out, const uint8_t *A, const uint8_t *B, int K,
        int incout, int incA, int incB);
void matmulfq_vnni_4x8(uint8_t *out, const uint8_t *A, const uint8_t *B, int K,
        int incout, int incA, int incB);
void matmulfq_vnni_4x7(uint8_t *out, const uint8_t *A, const uint8_t *B, int K,
        int incout, int incA, int incB);
void matmulfq_vnni_4x6(uint8_t *out, const uint8_t *A, const uint8_t *B, int K,
        int incout, int incA, int incB);
void matmulfq_vnni_4x4(uint8_t *out, const uint8_t *A, const uint8_t *B, int K,
        int incout, int incA, int incB);

#endif
