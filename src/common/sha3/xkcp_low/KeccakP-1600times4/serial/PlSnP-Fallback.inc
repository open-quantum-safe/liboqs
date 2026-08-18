/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

Implementation by Gilles Van Assche, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

---

This file contains macros that help make a PlSnP-compatible implementation by
serially falling back on a SnP-compatible implementation or on a PlSnP-compatible
implementation of lower parallism degree.

Please refer to PlSnP-documentation.h for more details.
*/

/* expect PlSnP_baseParallelism, PlSnP_targetParallelism */
/* expect SnP_stateSizeInBytes, SnP_stateAlignment */
/* expect prefix */
/* expect SnP_* */

#if defined(ADD_SYMBOL_SUFFIX)
#define JOIN0(a, b, c)                  a ## _ ## b ## _ ## c
#define JOIN(a, b, c)                   JOIN0(a, b, c)
#else
#define JOIN0(a, b)                     a ## _ ## b
#define JOIN(a, b, c)                   JOIN0(a, b)
#endif

#define PlSnP_StaticInitialize          JOIN(prefix, StaticInitialize, suffix)
#define PlSnP_InitializeAll             JOIN(prefix, InitializeAll, suffix)
#define PlSnP_AddByte                   JOIN(prefix, AddByte, suffix)
#define PlSnP_AddBytes                  JOIN(prefix, AddBytes, suffix)
#define PlSnP_AddLanesAll               JOIN(prefix, AddLanesAll, suffix)
#define PlSnP_OverwriteBytes            JOIN(prefix, OverwriteBytes, suffix)
#define PlSnP_OverwriteLanesAll         JOIN(prefix, OverwriteLanesAll, suffix)
#define PlSnP_OverwriteWithZeroes       JOIN(prefix, OverwriteWithZeroes, suffix)
#define PlSnP_ExtractBytes              JOIN(prefix, ExtractBytes, suffix)
#define PlSnP_ExtractLanesAll           JOIN(prefix, ExtractLanesAll, suffix)
#define PlSnP_ExtractAndAddBytes        JOIN(prefix, ExtractAndAddBytes, suffix)
#define PlSnP_ExtractAndAddLanesAll     JOIN(prefix, ExtractAndAddLanesAll, suffix)

#if (PlSnP_baseParallelism == 1)
    #define SnP_stateSizeInBytes            JOIN(SnP_prefix, stateSizeInBytes, SnP_suffix)
    #define SnP_stateAlignment              JOIN(SnP_prefix, stateAlignment, SnP_suffix)
#else
    #define SnP_stateSizeInBytes            JOIN(SnP_prefix, statesSizeInBytes, SnP_suffix)
    #define SnP_stateAlignment              JOIN(SnP_prefix, statesAlignment, SnP_suffix)
#endif
#define PlSnP_factor ((PlSnP_targetParallelism)/(PlSnP_baseParallelism))
#define SnP_stateOffset (((SnP_stateSizeInBytes+(SnP_stateAlignment-1))/SnP_stateAlignment)*SnP_stateAlignment)
#define stateWithIndex(i) ((unsigned char *)states+((i)*SnP_stateOffset))

#define SnP_StaticInitialize            JOIN(SnP_prefix, StaticInitialize, SnP_suffix)
#define SnP_Initialize                  JOIN(SnP_prefix, Initialize, SnP_suffix)
#define SnP_InitializeAll               JOIN(SnP_prefix, InitializeAll, SnP_suffix)
#define SnP_AddByte                     JOIN(SnP_prefix, AddByte, SnP_suffix)
#define SnP_AddBytes                    JOIN(SnP_prefix, AddBytes, SnP_suffix)
#define SnP_AddLanesAll                 JOIN(SnP_prefix, AddLanesAll, SnP_suffix)
#define SnP_OverwriteBytes              JOIN(SnP_prefix, OverwriteBytes, SnP_suffix)
#define SnP_OverwriteLanesAll           JOIN(SnP_prefix, OverwriteLanesAll, SnP_suffix)
#define SnP_OverwriteWithZeroes         JOIN(SnP_prefix, OverwriteWithZeroes, SnP_suffix)
#define SnP_ExtractBytes                JOIN(SnP_prefix, ExtractBytes, SnP_suffix)
#define SnP_ExtractLanesAll             JOIN(SnP_prefix, ExtractLanesAll, SnP_suffix)
#define SnP_ExtractAndAddBytes          JOIN(SnP_prefix, ExtractAndAddBytes, SnP_suffix)
#define SnP_ExtractAndAddLanesAll       JOIN(SnP_prefix, ExtractAndAddLanesAll, SnP_suffix)

void PlSnP_StaticInitialize( void )
{
    SnP_StaticInitialize();
}

void PlSnP_InitializeAll(void *states)
{
    unsigned int i;

    for(i=0; i<PlSnP_factor; i++)
    #if (PlSnP_baseParallelism == 1)
        SnP_Initialize(stateWithIndex(i));
    #else
        SnP_InitializeAll(stateWithIndex(i));
    #endif
}

void PlSnP_AddByte(void *states, unsigned int instanceIndex, unsigned char byte, unsigned int offset)
{
    #if (PlSnP_baseParallelism == 1)
        SnP_AddByte(stateWithIndex(instanceIndex), byte, offset);
    #else
        SnP_AddByte(stateWithIndex(instanceIndex/PlSnP_baseParallelism), instanceIndex%PlSnP_baseParallelism, byte, offset);
    #endif
}

void PlSnP_AddBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length)
{
    #if (PlSnP_baseParallelism == 1)
        SnP_AddBytes(stateWithIndex(instanceIndex), data, offset, length);
    #else
        SnP_AddBytes(stateWithIndex(instanceIndex/PlSnP_baseParallelism), instanceIndex%PlSnP_baseParallelism, data, offset, length);
    #endif
}

void PlSnP_AddLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
{
    unsigned int i;

    for(i=0; i<PlSnP_factor; i++) {
        #if (PlSnP_baseParallelism == 1)
            SnP_AddBytes(stateWithIndex(i), data, 0, laneCount*SnP_laneLengthInBytes);
        #else
            SnP_AddLanesAll(stateWithIndex(i), data, laneCount, laneOffset);
        #endif
        data += PlSnP_baseParallelism*laneOffset*SnP_laneLengthInBytes;
    }
}

void PlSnP_OverwriteBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length)
{
    #if (PlSnP_baseParallelism == 1)
        SnP_OverwriteBytes(stateWithIndex(instanceIndex), data, offset, length);
    #else
        SnP_OverwriteBytes(stateWithIndex(instanceIndex/PlSnP_baseParallelism), instanceIndex%PlSnP_baseParallelism, data, offset, length);
    #endif
}

void PlSnP_OverwriteLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
{
    unsigned int i;

    for(i=0; i<PlSnP_factor; i++) {
        #if (PlSnP_baseParallelism == 1)
            SnP_OverwriteBytes(stateWithIndex(i), data, 0, laneCount*SnP_laneLengthInBytes);
        #else
            SnP_OverwriteLanesAll(stateWithIndex(i), data, laneCount, laneOffset);
        #endif
        data += PlSnP_baseParallelism*laneOffset*SnP_laneLengthInBytes;
    }
}

void PlSnP_OverwriteWithZeroes(void *states, unsigned int instanceIndex, unsigned int byteCount)
{
    #if (PlSnP_baseParallelism == 1)
        SnP_OverwriteWithZeroes(stateWithIndex(instanceIndex), byteCount);
    #else
        SnP_OverwriteWithZeroes(stateWithIndex(instanceIndex/PlSnP_baseParallelism), instanceIndex%PlSnP_baseParallelism, byteCount);
    #endif
}

void PlSnP_PermuteAll(void *states)
{
    unsigned int i;

    for(i=0; i<PlSnP_factor; i++) {
        #if (PlSnP_baseParallelism == 1)
            SnP_Permute(stateWithIndex(i));
        #else
            SnP_PermuteAll(stateWithIndex(i));
        #endif
    }
}

#if (defined(SnP_Permute_12rounds) || defined(SnP_PermuteAll_12rounds))
void PlSnP_PermuteAll_12rounds(void *states)
{
    unsigned int i;

    for(i=0; i<PlSnP_factor; i++) {
        #if (PlSnP_baseParallelism == 1)
            SnP_Permute_12rounds(stateWithIndex(i));
        #else
            SnP_PermuteAll_12rounds(stateWithIndex(i));
        #endif
    }
}
#endif

#if (defined(SnP_Permute_Nrounds) || defined(SnP_PermuteAll_6rounds))
void PlSnP_PermuteAll_6rounds(void *states)
{
    unsigned int i;

    for(i=0; i<PlSnP_factor; i++) {
        #if (PlSnP_baseParallelism == 1)
            SnP_Permute_Nrounds(stateWithIndex(i), 6);
        #else
            SnP_PermuteAll_6rounds(stateWithIndex(i));
        #endif
    }
}
#endif

#if (defined(SnP_Permute_Nrounds) || defined(SnP_PermuteAll_4rounds))
void PlSnP_PermuteAll_4rounds(void *states)
{
    unsigned int i;

    for(i=0; i<PlSnP_factor; i++) {
        #if (PlSnP_baseParallelism == 1)
            SnP_Permute_Nrounds(stateWithIndex(i), 4);
        #else
            SnP_PermuteAll_4rounds(stateWithIndex(i));
        #endif
    }
}
#endif

void PlSnP_ExtractBytes(void *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length)
{
    #if (PlSnP_baseParallelism == 1)
        SnP_ExtractBytes(stateWithIndex(instanceIndex), data, offset, length);
    #else
        SnP_ExtractBytes(stateWithIndex(instanceIndex/PlSnP_baseParallelism), instanceIndex%PlSnP_baseParallelism, data, offset, length);
    #endif
}

void PlSnP_ExtractLanesAll(void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset)
{
    unsigned int i;

    for(i=0; i<PlSnP_factor; i++) {
        #if (PlSnP_baseParallelism == 1)
            SnP_ExtractBytes(stateWithIndex(i), data, 0, laneCount*SnP_laneLengthInBytes);
        #else
            SnP_ExtractLanesAll(stateWithIndex(i), data, laneCount, laneOffset);
        #endif
        data += laneOffset*SnP_laneLengthInBytes*PlSnP_baseParallelism;
    }
}

void PlSnP_ExtractAndAddBytes(void *states, unsigned int instanceIndex, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length)
{
    #if (PlSnP_baseParallelism == 1)
        SnP_ExtractAndAddBytes(stateWithIndex(instanceIndex), input, output, offset, length);
    #else
        SnP_ExtractAndAddBytes(stateWithIndex(instanceIndex/PlSnP_baseParallelism), instanceIndex%PlSnP_baseParallelism, input, output, offset, length);
    #endif
}

void PlSnP_ExtractAndAddLanesAll(void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset)
{
    unsigned int i;

    for(i=0; i<PlSnP_factor; i++) {
        #if (PlSnP_baseParallelism == 1)
            SnP_ExtractAndAddBytes(stateWithIndex(i), input, output, 0, laneCount*SnP_laneLengthInBytes);
        #else
            SnP_ExtractAndAddLanesAll(stateWithIndex(i), input, output, laneCount, laneOffset);
        #endif
        input += laneOffset*SnP_laneLengthInBytes*PlSnP_baseParallelism;
        output += laneOffset*SnP_laneLengthInBytes*PlSnP_baseParallelism;
    }
}

#undef PlSnP_factor
#undef SnP_stateOffset
#undef stateWithIndex
#undef JOIN0
#undef JOIN
#undef PlSnP_StaticInitialize
#undef PlSnP_InitializeAll
#undef PlSnP_AddByte
#undef PlSnP_AddBytes
#undef PlSnP_AddLanesAll
#undef PlSnP_OverwriteBytes
#undef PlSnP_OverwriteLanesAll
#undef PlSnP_OverwriteWithZeroes
#undef PlSnP_PermuteAll
#undef PlSnP_ExtractBytes
#undef PlSnP_ExtractLanesAll
#undef PlSnP_ExtractAndAddBytes
#undef PlSnP_ExtractAndAddLanesAll
#undef SnP_stateAlignment
#undef SnP_stateSizeInBytes
#undef PlSnP_factor
#undef SnP_stateOffset
#undef stateWithIndex
#undef SnP_StaticInitialize
#undef SnP_Initialize
#undef SnP_InitializeAll
#undef SnP_AddByte
#undef SnP_AddBytes
#undef SnP_AddLanesAll
#undef SnP_OverwriteBytes
#undef SnP_OverwriteWithZeroes
#undef SnP_OverwriteLanesAll
#undef SnP_ExtractBytes
#undef SnP_ExtractLanesAll
#undef SnP_ExtractAndAddBytes
#undef SnP_ExtractAndAddLanesAll
