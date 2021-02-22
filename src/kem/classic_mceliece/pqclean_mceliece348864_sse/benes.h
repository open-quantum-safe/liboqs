#ifndef PQCLEAN_MCELIECE348864_SSE_BENES_H
#define PQCLEAN_MCELIECE348864_SSE_BENES_H
/*
  This file is for Benes network related functions
*/


#include "gf.h"
#include "vec128.h"

void PQCLEAN_MCELIECE348864_SSE_load_bits(uint64_t  /*out*/[][32], const unsigned char * /*bits*/);
void PQCLEAN_MCELIECE348864_SSE_benes(uint64_t * /*r*/, uint64_t  /*cond*/[][32], int /*rev*/);

#endif

