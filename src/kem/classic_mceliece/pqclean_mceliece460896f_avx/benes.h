#ifndef PQCLEAN_MCELIECE460896F_AVX_BENES_H
#define PQCLEAN_MCELIECE460896F_AVX_BENES_H
/*
  This file is for Benes network related functions
*/


#include "vec128.h"

void PQCLEAN_MCELIECE460896F_AVX_load_bits(vec128  /*bits_int*/[][32], const unsigned char * /*bits*/);
void PQCLEAN_MCELIECE460896F_AVX_benes(vec128 * /*r*/, vec128  /*b*/[][32], int /*rev*/);

#endif

