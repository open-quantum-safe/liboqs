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

#ifndef _KeccakSpongeWidth1600times4_h_
#define _KeccakSpongeWidth1600times4_h_

#include <string.h>
#include "config.h"
#if !defined(SUPERCOP)
#include "align.h"
#else
#include <libkeccak.a.headers/align.h>
#endif

#define KCP_DeclareSpongeStructuretimes4(prefix, size, alignment) \
    ALIGN(alignment) typedef struct prefix##_SpongeInstanceStruct { \
        unsigned char state[size]; \
        unsigned int rate; \
        unsigned int byteIOIndex; \
        int squeezing; \
    } prefix##_SpongeInstance;

#define KCP_DeclareSpongeFunctionstimes4(prefix) \
    int prefix##_SpongeInitialize(prefix##_SpongeInstance *spongeInstance, unsigned int rate, unsigned int capacity); \
    int prefix##_SpongeAbsorb(prefix##_SpongeInstance *spongeInstance, const unsigned char **data, size_t dataByteLen); \
    int prefix##_SpongeAbsorbLastFewBits(prefix##_SpongeInstance *spongeInstance, unsigned char delimitedData); \
    int prefix##_SpongeSqueeze(prefix##_SpongeInstance *spongeInstance, unsigned char **data, size_t dataByteLen);

#ifdef XKCP_has_KeccakP1600times4
#if !defined(SUPERCOP)
    #include "KeccakP-1600-times4-SnP.h"
#else
    #include <libkeccak.a.headers/KeccakP-1600-times4-SnP.h>
#endif
    KCP_DeclareSpongeStructuretimes4(KeccakWidth1600times4, KeccakP1600times4_statesSizeInBytes, KeccakP1600times4_statesAlignment)
    KCP_DeclareSpongeFunctionstimes4(KeccakWidth1600times4)
#endif

#endif
