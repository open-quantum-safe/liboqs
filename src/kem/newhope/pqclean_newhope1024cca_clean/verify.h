#ifndef VERIFY_H
#define VERIFY_H

#include <stdio.h>

/* returns 0 for equal strings, 1 for non-equal strings */
int PQCLEAN_NEWHOPE1024CCA_CLEAN_verify(const unsigned char *a, const unsigned char *b, size_t len);

/* b = 1 means mov, b = 0 means don't mov*/
void PQCLEAN_NEWHOPE1024CCA_CLEAN_cmov(unsigned char *r, const unsigned char *x, size_t len, unsigned char b);

#endif
