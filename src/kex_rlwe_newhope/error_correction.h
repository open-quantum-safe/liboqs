#ifndef ERROR_CORRECTION_H
#define ERROR_CORRECTION_H

#include "inttypes.h"
#include "params.h"
#include "math.h"
#include "poly.h"
#include <stdio.h>

void helprec(poly *c, const poly *v, OQS_RAND *rand);
void rec(unsigned char *key, const poly *v, const poly *c);

#endif
