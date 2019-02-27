#include <oqs/rand.h>
#include "ref/randombytes.h"

int randombytes(unsigned char *x, unsigned long long xlen) {
	OQS_randombytes(x, xlen);
	return 0;
}
