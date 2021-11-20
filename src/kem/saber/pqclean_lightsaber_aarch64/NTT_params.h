#ifndef NTT_PARAMS_H
#define NTT_PARAMS_H

#define ARRAY_N 256

#define NTT_N 64
#define LOGNTT_N 6

// Q1
#define Q1 25570817
// omegaQ1 = 3^( (Q1 - 1) / (NTT_N << 1) ) mod Q1
#define omegaQ1 21614269
// invomegaQ1 = omegaQ^{-1} mod Q1
#define invomegaQ1 8215090
// R = 2^32 below
// RmodQ1 = 2^32 mod^{+-} Q1
#define RmodQ1 (-929960)
// Q1prime = Q1^{-1} mod^{+-} 2^32
#define Q1prime (-155332095)
// invNQ1 = NTT_N^{-1} mod Q1
#define invNQ1 25171273
// R2modQ1 = 2^32 mod^{+-} Q1
#define R2modQ1 (-929960)
// Q1prime2 = -Q1^{-1} mod^{+-} 2^32
#define Q1prime2 155332095

#endif





