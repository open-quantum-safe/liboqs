/* ****************************** *
 * Titanium_CCA_std               *
 * Implemented by Raymond K. ZHAO *
 *                                *
 * NTT functions                  *
 * ****************************** */
 
#ifndef NTT_H
#define NTT_H

#include <stdint.h>

void ntt_1024_1792(uint32_t *a);
void intt_1792_768(uint32_t *a);
void ntt_512_1536(uint32_t *a);
void ntt_1024_1536(uint32_t *a);
void intt_1536_1536_inv(uint32_t *a);
void ntt_768_768_inv(uint32_t *a);
void intt_768_256(uint32_t *a);
void ntt_1536_1792(uint32_t *a);
void intt_1792_256(uint32_t *a);

#endif
