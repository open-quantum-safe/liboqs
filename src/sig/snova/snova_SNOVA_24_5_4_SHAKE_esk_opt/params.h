#ifndef PARAMS_H
#define PARAMS_H

#ifndef sk_is_seed
#define sk_is_seed 1
#endif

#ifndef PK_EXPAND_SHAKE
#define PK_EXPAND_SHAKE 1
#endif

#ifndef v_SNOVA
#define v_SNOVA 24
#endif

#ifndef o_SNOVA
#define o_SNOVA 5
#endif

#ifndef l_SNOVA
#define l_SNOVA 4
#endif

// Optimisation level.  0: Reference, 1:Optimised, 2:Vectorized
#ifndef OPTIMISATION
#if __AVX2__ || __ARM_NEON
#define OPTIMISATION 2
#else
#define OPTIMISATION 1
#endif
#endif

#endif
