#ifndef small_h
#define small_h

#include "crypto_int8.h"
#include "crypto_int32.h"

typedef crypto_int8 small;

#define small_encode crypto_kem_ntrulpr4591761_ref_small_encode
extern void small_encode(unsigned char *, const small *);

#define small_decode crypto_kem_ntrulpr4591761_ref_small_decode
extern void small_decode(small *, const unsigned char *);

#define small_random32 crypto_kem_ntrulpr4591761_ref_small_random32
extern crypto_int32 small_random32(void);

#define small_random crypto_kem_ntrulpr4591761_ref_small_random
extern void small_random(small *);

#define small_seeded_weightw crypto_kem_ntrulpr4591761_ref_small_seeded_weightw
extern void small_seeded_weightw(small *, const unsigned char *);

#define small_random_weightw crypto_kem_ntrulpr4591761_ref_small_random_weightw
extern void small_random_weightw(small *);

#endif
