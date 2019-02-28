#ifndef VERIFY_H
#define VERIFY_H

#include <stdio.h>

int PQCLEAN_KYBER768_CLEAN_verify(const unsigned char *a, const unsigned char *b, size_t len);

void PQCLEAN_KYBER768_CLEAN_cmov(unsigned char *r, const unsigned char *x, size_t len, unsigned char b);

#endif
