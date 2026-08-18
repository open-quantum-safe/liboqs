/*
The eXtended Keccak Code Package (XKCP)
https://github.com/XKCP/XKCP

The Keccak-p permutations, designed by Guido Bertoni, Joan Daemen, Michaël Peeters and Gilles Van Assche.

Implementation by Ronny Van Keer, hereby denoted as "the implementer".

For more information, feedback or questions, please refer to the Keccak Team website:
https://keccak.team/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

---

Please refer to SnP-documentation.h for more details.
*/

#ifndef _KeccakP_1600_SnP_h_
#define _KeccakP_1600_SnP_h_

#include <stddef.h>

#ifdef __MINGW32__
#define FORCE_SYSV __attribute__((sysv_abi))
#else
#define FORCE_SYSV
#endif

#define KeccakP1600_implementation_avx2      "AVX2 optimized implementation"
#define KeccakP1600_stateSizeInBytes_avx2    200
#define KeccakP1600_stateAlignment_avx2      32
#define KeccakF1600_FastLoop_supported_avx2

#if defined(ADD_SYMBOL_SUFFIX)
#define KECCAK_SYMBOL_SUFFIX avx2
#define KECCAK_IMPL_NAMESPACE(x) x##_avx2
#else
#define KECCAK_IMPL_NAMESPACE(x) x
#define KeccakP1600_implementation KeccakP1600_implementation_avx2
#define KeccakP1600_stateSizeInBytes KeccakP1600_stateSizeInBytes_avx2
#define KeccakP1600_stateAlignment KeccakP1600_stateAlignment_avx2
#define KeccakF1600_FastLoop_supported KeccakF1600_FastLoop_supported_avx2
#endif

#define KeccakP1600_StaticInitialize KECCAK_IMPL_NAMESPACE(KeccakP1600_StaticInitialize)
void KeccakP1600_StaticInitialize(void);

#define KeccakP1600_Initialize KECCAK_IMPL_NAMESPACE(KeccakP1600_Initialize)
FORCE_SYSV void KeccakP1600_Initialize(void *state);

#define KeccakP1600_AddByte KECCAK_IMPL_NAMESPACE(KeccakP1600_AddByte)
FORCE_SYSV void KeccakP1600_AddByte(void *state, unsigned char data, unsigned int offset);

#define KeccakP1600_AddBytes KECCAK_IMPL_NAMESPACE(KeccakP1600_AddBytes)
FORCE_SYSV void KeccakP1600_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);

#define KeccakP1600_OverwriteBytes KECCAK_IMPL_NAMESPACE(KeccakP1600_OverwriteBytes)
FORCE_SYSV void KeccakP1600_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);

#define KeccakP1600_OverwriteWithZeroes KECCAK_IMPL_NAMESPACE(KeccakP1600_OverwriteWithZeroes)
FORCE_SYSV void KeccakP1600_OverwriteWithZeroes(void *state, unsigned int byteCount);

#define KeccakP1600_Permute_Nrounds KECCAK_IMPL_NAMESPACE(KeccakP1600_Permute_Nrounds)
FORCE_SYSV void KeccakP1600_Permute_Nrounds(void *state, unsigned int nrounds);

#define KeccakP1600_Permute_12rounds KECCAK_IMPL_NAMESPACE(KeccakP1600_Permute_12rounds)
FORCE_SYSV void KeccakP1600_Permute_12rounds(void *state);

#define KeccakP1600_Permute_24rounds KECCAK_IMPL_NAMESPACE(KeccakP1600_Permute_24rounds)
FORCE_SYSV void KeccakP1600_Permute_24rounds(void *state);

#define KeccakP1600_ExtractBytes KECCAK_IMPL_NAMESPACE(KeccakP1600_ExtractBytes)
FORCE_SYSV void KeccakP1600_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);

#define KeccakP1600_ExtractAndAddBytes KECCAK_IMPL_NAMESPACE(KeccakP1600_ExtractAndAddBytes)
FORCE_SYSV void KeccakP1600_ExtractAndAddBytes(const void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);

#define KeccakF1600_FastLoop_Absorb KECCAK_IMPL_NAMESPACE(KeccakF1600_FastLoop_Absorb)
FORCE_SYSV size_t KeccakF1600_FastLoop_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen);

#define KeccakP1600_12rounds_FastLoop_Absorb KECCAK_IMPL_NAMESPACE(KeccakP1600_12rounds_FastLoop_Absorb)
FORCE_SYSV size_t KeccakP1600_12rounds_FastLoop_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen);

#endif
