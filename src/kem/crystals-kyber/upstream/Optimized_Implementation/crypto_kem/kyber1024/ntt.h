#ifndef NTT_H
#define NTT_H

#include <stdint.h>

void ntt(uint16_t* poly);
void invntt(uint16_t* poly);

#endif
