#ifndef rq_h
#define rq_h

#include "modq.h"
#include "small.h"

#define rq_encode crypto_kem_ntrulpr4591761_ref_rq_encode
extern void rq_encode(unsigned char *, const modq *);

#define rq_decode crypto_kem_ntrulpr4591761_ref_rq_decode
extern void rq_decode(modq *, const unsigned char *);

#define rq_encoderounded crypto_kem_ntrulpr4591761_ref_rq_encoderounded
extern void rq_encoderounded(unsigned char *, const modq *);

#define rq_decoderounded crypto_kem_ntrulpr4591761_ref_rq_decoderounded
extern void rq_decoderounded(modq *, const unsigned char *);

#define rq_round3 crypto_kem_ntrulpr4591761_ref_rq_round
extern void rq_round3(modq *, const modq *);

#define rq_mult crypto_kem_ntrulpr4591761_ref_rq_mult
extern void rq_mult(modq *, const modq *, const small *);

#define rq_recip3 crypto_kem_ntrulpr4591761_ref_rq_recip3
int rq_recip3(modq *, const small *);

#define rq_fromseed crypto_kem_ntrulpr4591761_ref_rq_fromseed
extern void rq_fromseed(modq *, const unsigned char *);

#endif
