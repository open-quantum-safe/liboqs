#ifndef r3_h
#define r3_h

#include "small.h"

#define r3_mult crypto_kem_ntrulpr4591761_ref_r3_mult
extern void r3_mult(small *, const small *, const small *);

#define r3_recip crypto_kem_ntrulpr4591761_ref_r3_recip
extern int r3_recip(small *, const small *);

#endif
