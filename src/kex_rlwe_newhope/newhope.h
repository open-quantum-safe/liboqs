#ifndef NEWHOPE_H
#define NEWHOPE_H

#include "poly.h"
#include "error_correction.h"
#include <math.h>
#include <stdio.h>
#include <oqs/rand.h>
void OQS_KEX_rlwe_newhope_keygen(unsigned char *send, poly *sk, OQS_RAND *rand);
void OQS_KEX_rlwe_newhope_sharedb(unsigned char *sharedkey, unsigned char *send, const unsigned char *received, OQS_RAND *rand);
void OQS_KEX_rlwe_newhope_shareda(unsigned char *sharedkey, const poly *ska, const unsigned char *received);
#endif
