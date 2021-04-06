/*
The Keccak-p permutations, designed by Guido Bertoni, Joan Daemen, Michaël Peeters and Gilles Van Assche.

Implementation by Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

---

This file implements Keccak-p[1600]×4 in a PlSnP-compatible way.
Please refer to PlSnP-documentation.h for more details.

This implementation comes with KeccakP-1600-times4-SnP.h in the same folder.
Please refer to LowLevel.build for the exact list of other files it must be combined with.
*/

#include "KeccakP-1600-SnP.h"

#define prefix                          KeccakP1600times4
#define suffix                          serial
#define PlSnP_baseParallelism           1
#define PlSnP_targetParallelism         4
#define SnP_laneLengthInBytes           8
#define SnP_prefix                      KeccakP1600
#define SnP_suffix                      KECCAK_SYMBOL_SUFFIX

#if defined(ADD_SYMBOL_SUFFIX)
#define JOIN0(a,b,c) a##_##b##_##c
#define JOIN(a,b)   JOIN0(a,b,c)

#define SnP_Permute                     JOIN(SnP_prefix, Permute_24rounds, SnP_suffix)
#define SnP_Permute_12rounds            JOIN(SnP_prefix, Permute_12rounds, SnP_suffix)
#define SnP_Permute_Nrounds             JOIN(SnP_prefix, Permute_Nrounds, SnP_suffix)
#define PlSnP_PermuteAll                JOIN(prefix, PermuteAll_24rounds, suffix)
#define PlSnP_PermuteAll_12rounds       JOIN(prefix, PermuteAll_12rounds, suffix)
#define PlSnP_PermuteAll_6rounds        JOIN(prefix, PermuteAll_6rounds, suffix)
#define PlSnP_PermuteAll_4rounds        JOIN(prefix, PermuteAll_4rounds, suffix)

#undef JOIN0
#undef JOIN
#else
#define SnP_Permute                     KeccakP1600_Permute_24rounds
#define SnP_Permute_12rounds            KeccakP1600_Permute_12rounds
#define SnP_Permute_Nrounds             KeccakP1600_Permute_Nrounds
#define PlSnP_PermuteAll                KeccakP1600times4_PermuteAll_24rounds
#define PlSnP_PermuteAll_12rounds       KeccakP1600times4_PermuteAll_12rounds
#define PlSnP_PermuteAll_6rounds        KeccakP1600times4_PermuteAll_6rounds
#define PlSnP_PermuteAll_4rounds        KeccakP1600times4_PermuteAll_4rounds
#endif

#include "PlSnP-Fallback.inc"
