#ifndef KEM_H
#define KEM_H

#include <string.h>
#include "sizes.h"
#include "api.h"


void xor(unsigned char * a, unsigned char * b, int length);
#define addto(a, b) (xor(a, b, BITS_TO_BYTES(CODIMENSION)))

void shift_cols(unsigned char * in, unsigned char * out, int d);


int encrypt_nied(OUT unsigned char *syndrome, IN int * e, unsigned char * pk);
int decrypt_nied(IN unsigned char *syndrome, OUT int * error, IN unsigned char * sk);

#endif
