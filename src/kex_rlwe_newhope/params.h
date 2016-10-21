#ifndef PARAMS_H
#define PARAMS_H

#define PARAM_N 1024

#define PARAM_K 16 /* used in sampler */
#define PARAM_Q 12289

#define POLY_BYTES 1792
#define NEWHOPE_SEEDBYTES 32
#define NEWHOPE_RECBYTES 256

#define NEWHOPE_SENDABYTES (POLY_BYTES + NEWHOPE_SEEDBYTES)
#define NEWHOPE_SENDBBYTES (POLY_BYTES + NEWHOPE_RECBYTES)


#if defined(WINDOWS)
typedef unsigned __int16 uint16_t;
#endif

#endif
