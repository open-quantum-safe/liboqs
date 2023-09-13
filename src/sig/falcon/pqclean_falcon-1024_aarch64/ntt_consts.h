#ifndef NTT_CONSTS
#define NTT_CONSTS

#include <stdint.h>

extern const int16_t PQCLEAN_FALCON1024_AARCH64_qmvq[8];

/*
 * Table for NTT, binary case:
 * where g = 7 (it is a 2048-th primitive root of 1 modulo q)
 */
extern const int16_t PQCLEAN_FALCON1024_AARCH64_ntt_br[];
extern const int16_t PQCLEAN_FALCON1024_AARCH64_ntt_qinv_br[];

/*
 * Table for inverse NTT
 * Since g = 7, 1/g = 8778 mod 12289.
 */

extern const int16_t PQCLEAN_FALCON1024_AARCH64_invntt_br[];
extern const int16_t PQCLEAN_FALCON1024_AARCH64_invntt_qinv_br[];

#endif
