/*
The Keccak-p permutations, designed by Guido Bertoni, Joan Daemen, Michaël Peeters and Gilles Van Assche.

Implementation by Gilles Van Assche and Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

---

Please refer to PlSnP-documentation.h for more details.
*/

#ifndef _KeccakP_1600_times4_SnP_h_
#define _KeccakP_1600_times4_SnP_h_

#include <stdint.h>
#include "SIMD256-config.h"

#include <stddef.h>

#define KeccakP1600times4_implementation_avx2        "256-bit SIMD implementation (" KeccakP1600times4_implementation_config ")"
#define KeccakP1600times4_statesSizeInBytes_avx2     800
#define KeccakP1600times4_statesAlignment_avx2       32
#define KeccakF1600times4_FastLoop_supported_avx2
#define KeccakP1600times4_12rounds_FastLoop_supported_avx2
#define KeccakF1600times4_FastKravatte_supported_avx2

#if defined(ADD_SYMBOL_SUFFIX)
#define KECCAKTIMES4_IMPL_NAMESPACE(x) x##_avx2
#else
#define KECCAKTIMES4_IMPL_NAMESPACE(x) x
#define KeccakP1600times4_implementation KeccakP1600times4_implementation_avx2
#define KeccakP1600times4_statesSizeInBytes KeccakP1600times4_statesSizeInBytes_avx2
#define KeccakP1600times4_statesAlignment KeccakP1600times4_statesAlignment_avx2
#define KeccakF1600times4_FastLoop_supported KeccakF1600times4_FastLoop_supported_avx2
#define KeccakP1600times4_12rounds_FastLoop_supported KeccakP1600times4_12rounds_FastLoop_supported_avx2
#define KeccakF1600times4_FastKravatte_supported KeccakF1600times4_FastKravatte_supported_avx2
#endif

#define KeccakP1600times4_StaticInitialize()

#define KeccakP1600times4_InitializeAll KECCAKTIMES4_IMPL_NAMESPACE(KeccakP1600times4_InitializeAll)
void KeccakP1600times4_InitializeAll(void *states);

#define KeccakP1600times4_AddByte KECCAKTIMES4_IMPL_NAMESPACE(KeccakP1600times4_AddByte)
void KeccakP1600times4_AddByte(void *states, unsigned int instanceIndex, unsigned char byte, unsigned int offset);

#define KeccakP1600times4_AddBytes KECCAKTIMES4_IMPL_NAMESPACE(KeccakP1600times4_AddBytes)
void KeccakP1600times4_AddBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);

#define KeccakP1600times4_AddLanesAll KECCAKTIMES4_IMPL_NAMESPACE(KeccakP1600times4_AddLanesAll)
void KeccakP1600times4_AddLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);

#define KeccakP1600times4_OverwriteBytes KECCAKTIMES4_IMPL_NAMESPACE(KeccakP1600times4_OverwriteBytes)
void KeccakP1600times4_OverwriteBytes(void *states, unsigned int instanceIndex, const unsigned char *data, unsigned int offset, unsigned int length);

#define KeccakP1600times4_OverwriteLanesAll KECCAKTIMES4_IMPL_NAMESPACE(KeccakP1600times4_OverwriteLanesAll)
void KeccakP1600times4_OverwriteLanesAll(void *states, const unsigned char *data, unsigned int laneCount, unsigned int laneOffset);

#define KeccakP1600times4_OverwriteWithZeroes KECCAKTIMES4_IMPL_NAMESPACE(KeccakP1600times4_OverwriteWithZeroes)
void KeccakP1600times4_OverwriteWithZeroes(void *states, unsigned int instanceIndex, unsigned int byteCount);

#define KeccakP1600times4_PermuteAll_4rounds KECCAKTIMES4_IMPL_NAMESPACE(KeccakP1600times4_PermuteAll_4rounds)
void KeccakP1600times4_PermuteAll_4rounds(void *states);

#define KeccakP1600times4_PermuteAll_6rounds KECCAKTIMES4_IMPL_NAMESPACE(KeccakP1600times4_PermuteAll_6rounds)
void KeccakP1600times4_PermuteAll_6rounds(void *states);

#define KeccakP1600times4_PermuteAll_12rounds KECCAKTIMES4_IMPL_NAMESPACE(KeccakP1600times4_PermuteAll_12rounds)
void KeccakP1600times4_PermuteAll_12rounds(void *states);

#define KeccakP1600times4_PermuteAll_24rounds KECCAKTIMES4_IMPL_NAMESPACE(KeccakP1600times4_PermuteAll_24rounds)
void KeccakP1600times4_PermuteAll_24rounds(void *states);

#define KeccakP1600times4_ExtractBytes KECCAKTIMES4_IMPL_NAMESPACE(KeccakP1600times4_ExtractBytes)
void KeccakP1600times4_ExtractBytes(const void *states, unsigned int instanceIndex, unsigned char *data, unsigned int offset, unsigned int length);

#define KeccakP1600times4_ExtractLanesAll KECCAKTIMES4_IMPL_NAMESPACE(KeccakP1600times4_ExtractLanesAll)
void KeccakP1600times4_ExtractLanesAll(const void *states, unsigned char *data, unsigned int laneCount, unsigned int laneOffset);

#define KeccakP1600times4_ExtractAndAddBytes KECCAKTIMES4_IMPL_NAMESPACE(KeccakP1600times4_ExtractAndAddBytes)
void KeccakP1600times4_ExtractAndAddBytes(const void *states, unsigned int instanceIndex,  const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);

#define KeccakP1600times4_ExtractAndAddLanesAll KECCAKTIMES4_IMPL_NAMESPACE(KeccakP1600times4_ExtractAndAddLanesAll)
void KeccakP1600times4_ExtractAndAddLanesAll(const void *states, const unsigned char *input, unsigned char *output, unsigned int laneCount, unsigned int laneOffset);

#define KeccakF1600times4_FastLoop_Absorb KECCAKTIMES4_IMPL_NAMESPACE(KeccakF1600times4_FastLoop_Absorb)
size_t KeccakF1600times4_FastLoop_Absorb(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *data, size_t dataByteLen);

#define KeccakP1600times4_12rounds_FastLoop_Absorb KECCAKTIMES4_IMPL_NAMESPACE(KeccakP1600times4_12rounds_FastLoop_Absorb)
size_t KeccakP1600times4_12rounds_FastLoop_Absorb(void *states, unsigned int laneCount, unsigned int laneOffsetParallel, unsigned int laneOffsetSerial, const unsigned char *data, size_t dataByteLen);

#endif
