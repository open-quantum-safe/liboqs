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
#include "align.h"

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
    #include "KeccakP-1600-times4-SnP.h"
    KCP_DeclareSpongeStructuretimes4(oqs_sig_picnic_KeccakWidth1600times4, oqs_sig_picnic_KeccakP1600times4_statesSizeInBytes, oqs_sig_picnic_KeccakP1600times4_statesAlignment)
    KCP_DeclareSpongeFunctionstimes4(oqs_sig_picnic_KeccakWidth1600times4)
#endif

#endif
