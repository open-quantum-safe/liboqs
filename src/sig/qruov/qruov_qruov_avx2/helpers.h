#ifndef AVX2_HELPERS_H
#define AVX2_HELPERS_H

#include <stdint.h>

#define SYMM_COMPACT_INDEX(i, j, N) ((j) + (i) * (N) - ((i) * ((i) + 1) / 2))

// AoS layout stores F_{q^L} coefficients as [items][L].
// SoA layout stores them as [L][plane], where plane may include padding.
void fql_aos_to_soa(uint8_t dst_soa[], const uint8_t src_aos[], int items, int plane);
void fql_soa_to_aos_permute(uint8_t dst_aos[], const uint8_t src_soa[], int items, int plane);
void fql_soa_to_aos_permute_double(uint8_t dst_aos[], const uint8_t src_soa[], int items, int plane);

void fql_matmul_vnni_fast(uint8_t C[], const uint8_t A[], const uint8_t Bvnni[], int M, int N, int K);
void fql_matmul_eval_vnni_fast(uint8_t C[], const uint8_t A[], const uint8_t Bvnni[], int M, int N, int K);
void fq_eval_transpose_planes(uint8_t B[], const uint8_t A[], int planeB, int planeA, int ldB, int ldA, int M, int N);

// Convert AoS coefficient matrices to AVX2 VNNI layouts, optionally after
// applying the EMI evaluation transform. The compact_sym variants take
// upper-triangle compact symmetric input.
void fql_aos_to_vnni(uint8_t Avnni[], int M, int N, const uint8_t bytes[], int origM, int origN);
void fql_compact_sym_aos_to_vnni(uint8_t Avnni[], int M, int N, const uint8_t bytes[], int origM);
void fql_aos_to_eval_vnni(uint8_t Avnni[], int M, int N, const uint8_t bytes[], int origM, int origN);
void fql_compact_sym_aos_to_eval_vnni(uint8_t Avnni[], int M, int N, const uint8_t bytes[], int origM);
void fql_aos_to_eval_transposed_soa(uint8_t A[], int M, int N, const uint8_t bytes[], int origM, int origN);

#endif
