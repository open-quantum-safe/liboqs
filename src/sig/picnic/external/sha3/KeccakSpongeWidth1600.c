/*
Implementation by the Keccak Team, namely, Guido Bertoni, Joan Daemen,
Michaël Peeters, Gilles Van Assche and Ronny Van Keer,
hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#include "KeccakSpongeWidth1600.h"

#ifdef KeccakReference
    #include "displayIntermediateValues.h"
#endif

#ifndef KeccakP1600_excluded
    #include "KeccakP-1600-SnP.h"

    #define prefix oqs_sig_picnic_KeccakWidth1600
    #define SnP KeccakP1600
    #define SnP_width 1600
    #define SnP_Permute oqs_sig_picnic_KeccakP1600_Permute_24rounds
    #if defined(KeccakF1600_FastLoop_supported)
        #define SnP_FastLoop_Absorb KeccakF1600_FastLoop_Absorb
    #endif
        #include "KeccakSponge.inc"
    #undef prefix
    #undef SnP
    #undef SnP_width
    #undef SnP_Permute
    #undef SnP_FastLoop_Absorb
#endif
