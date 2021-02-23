#ifndef PQCLEAN_MCELIECE348864_CLEAN_UTIL_H
#define PQCLEAN_MCELIECE348864_CLEAN_UTIL_H
/*
  This file is for loading/storing data in a little-endian fashion
*/


#include "gf.h"
#include <stdint.h>

void PQCLEAN_MCELIECE348864_CLEAN_store2(unsigned char * /*dest*/, gf /*a*/);
uint16_t PQCLEAN_MCELIECE348864_CLEAN_load2(const unsigned char * /*src*/);

uint32_t PQCLEAN_MCELIECE348864_CLEAN_load4(const unsigned char * /*in*/);

void PQCLEAN_MCELIECE348864_CLEAN_store8(unsigned char * /*out*/, uint64_t  /*in*/);
uint64_t PQCLEAN_MCELIECE348864_CLEAN_load8(const unsigned char * /*in*/);

gf PQCLEAN_MCELIECE348864_CLEAN_bitrev(gf /*a*/);

#endif

