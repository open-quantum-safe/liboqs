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

#include <string.h>
#include "KeccakHashtimes4.h"

/* ---------------------------------------------------------------- */

HashReturn Keccak_HashInitializetimes4(Keccak_HashInstancetimes4 *instance, unsigned int rate, unsigned int capacity, unsigned int hashbitlen, unsigned char delimitedSuffix)
{
    HashReturn result;

    if (delimitedSuffix == 0)
        return FAIL;
    result = (HashReturn)KeccakWidth1600times4_SpongeInitialize(&instance->sponge, rate, capacity);
    if (result != SUCCESS)
        return result;
    instance->fixedOutputLength = hashbitlen;
    instance->delimitedSuffix = delimitedSuffix;
    return SUCCESS;
}

/* ---------------------------------------------------------------- */

HashReturn Keccak_HashUpdatetimes4(Keccak_HashInstancetimes4 *instance, const BitSequence **data, BitLength databitlen)
{
    if ((databitlen % 8) != 0)
        return FAIL;
    return (HashReturn)KeccakWidth1600times4_SpongeAbsorb(&instance->sponge, data, databitlen/8);
}

/* ---------------------------------------------------------------- */

HashReturn Keccak_HashFinaltimes4(Keccak_HashInstancetimes4 *instance, BitSequence **hashval)
{
    HashReturn ret = (HashReturn)KeccakWidth1600times4_SpongeAbsorbLastFewBits(&instance->sponge, instance->delimitedSuffix);
    if (ret == SUCCESS)
        return (HashReturn)KeccakWidth1600times4_SpongeSqueeze(&instance->sponge, hashval, instance->fixedOutputLength/8);
    else
        return ret;
}

/* ---------------------------------------------------------------- */

HashReturn Keccak_HashSqueezetimes4(Keccak_HashInstancetimes4 *instance, BitSequence **data, BitLength databitlen)
{
    if ((databitlen % 8) != 0)
        return FAIL;
    return (HashReturn)KeccakWidth1600times4_SpongeSqueeze(&instance->sponge, data, databitlen/8);
}
