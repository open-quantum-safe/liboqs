/*
Implementation by the Keccak Team, namely, Guido Bertoni, Joan Daemen,
Michaël Peeters, Gilles Van Assche and Ronny Van Keer,
hereby denoted as "the implementer".

For more information, feedback or questions, please refer to our website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

SPDX-License-Identifier: CC0-1.0
*/

#include <string.h>
#include "KeccakHashtimes4.h"

/* ---------------------------------------------------------------- */

int oqs_Keccak_HashInitializetimes4(oqs_Keccak_HashInstancetimes4 *instance, unsigned int rate, unsigned int capacity, unsigned int hashbitlen, unsigned char delimitedSuffix)
{
    int result;

    if (delimitedSuffix == 0)
        return 1;
    result = oqs_KeccakWidth1600times4_SpongeInitialize(&instance->sponge, rate, capacity);
    if (result)
        return result;
    instance->fixedOutputLength = hashbitlen;
    instance->delimitedSuffix = delimitedSuffix;
    return 0;
}

/* ---------------------------------------------------------------- */

int oqs_Keccak_HashUpdatetimes4(oqs_Keccak_HashInstancetimes4 *instance, const oqs_BitSequence **data, oqs_BitLength databitlen)
{
    if ((databitlen % 8) != 0)
        return 1;
    return oqs_KeccakWidth1600times4_SpongeAbsorb(&instance->sponge, data, databitlen/8);
}

/* ---------------------------------------------------------------- */

int oqs_Keccak_HashFinaltimes4(oqs_Keccak_HashInstancetimes4 *instance, oqs_BitSequence **hashval)
{
    int ret = oqs_KeccakWidth1600times4_SpongeAbsorbLastFewBits(&instance->sponge, instance->delimitedSuffix);
    if (ret == 0)
        return oqs_KeccakWidth1600times4_SpongeSqueeze(&instance->sponge, hashval, instance->fixedOutputLength/8);
    else
        return ret;
}

/* ---------------------------------------------------------------- */

int oqs_Keccak_HashSqueezetimes4(oqs_Keccak_HashInstancetimes4 *instance, oqs_BitSequence **data, oqs_BitLength databitlen)
{
    if ((databitlen % 8) != 0)
        return 1;
    return oqs_KeccakWidth1600times4_SpongeSqueeze(&instance->sponge, data, databitlen/8);
}
