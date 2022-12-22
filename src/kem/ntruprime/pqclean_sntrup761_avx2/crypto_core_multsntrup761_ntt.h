#ifndef ntt_H
#define ntt_H

#include <stdint.h>



extern void PQCLEAN_SNTRUP761_AVX2_ntt512_7681(int16_t *f, int reps);
extern void PQCLEAN_SNTRUP761_AVX2_ntt512_10753(int16_t *f, int reps);
extern void PQCLEAN_SNTRUP761_AVX2_invntt512_7681(int16_t *f, int reps);
extern void PQCLEAN_SNTRUP761_AVX2_invntt512_10753(int16_t *f, int reps);

#endif
