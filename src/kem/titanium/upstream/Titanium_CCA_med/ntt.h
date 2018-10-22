/* ****************************** *
 * Titanium_CCA_med               *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * NTT functions                  *
 * ****************************** */
 
#ifndef NTT_H
#define NTT_H

#include <stdint.h>

void ntt_1280_2048(uint32_t *a);
void intt_2048_768(uint32_t *a);
void ntt_512_1792(uint32_t *a);
void ntt_1280_1792(uint32_t *a);
void intt_1792_1792_inv(uint32_t *a);
void ntt_768_768_inv(uint32_t *a);
void ntt_512_768(uint32_t *a);
void intt_768_256(uint32_t *a);
void ntt_1792_2048(uint32_t *a);
void intt_2048_256(uint32_t *a);

#endif
