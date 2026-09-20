#ifndef QRUOV_SIMD_H_
#define QRUOV_SIMD_H_

#include "qruov.h"
#include "simd.h"

#if QRUOV_q == 7
#define ymm_subq_if_greater_epi8 ymm_sub7_if_greater_epi8
#define ymm_modq_epi15           ymm_mod7_epi15
#define ymm_modq_epi32_epi8      ymm_mod7_epi32_epi8
#define ymm2_modq_epi32_epi8     ymm2_mod7_epi32_epi8
#define ymm_reduceq_epi16        ymm_reduce7_epi16
#elif QRUOV_q == 31
#define ymm_subq_if_greater_epi8 ymm_sub31_if_greater_epi8
#define ymm_modq_epi15           ymm_mod31_epi15
#define ymm_modq_epi32_epi8      ymm_mod31_epi32_epi8
#define ymm2_modq_epi32_epi8     ymm2_mod31_epi32_epi8
#define ymm_reduceq_epi16        ymm_reduce31_epi16
#elif QRUOV_q == 127
#define ymm_subq_if_greater_epi8 ymm_sub127_if_greater_epi8
#define ymm_modq_epi15           ymm_mod127_epi15
#define ymm_modq_epi32_epi8      ymm_mod127_epi32_epi8
#define ymm2_modq_epi32_epi8     ymm2_mod127_epi32_epi8
#define ymm_reduceq_epi16        ymm_reduce127_epi16
#else
#error "Unsupported QRUOV_q"
#endif

#endif // QRUOV_SIMD_H_
