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

#define KeccakP1600_implementation      "AVX2 optimized implementation"
#define KeccakP1600_stateSizeInBytes    200
#define KeccakP1600_stateAlignment      8
#define KeccakF1600_FastLoop_supported
#define KeccakP1600_12rounds_FastLoop_supported

#define KeccakP1600_StaticInitialize()
FORCE_SYSV void KeccakP1600_Initialize(void *state);
FORCE_SYSV void KeccakP1600_AddByte(void *state, unsigned char data, unsigned int offset);
FORCE_SYSV void KeccakP1600_AddBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
FORCE_SYSV void KeccakP1600_OverwriteBytes(void *state, const unsigned char *data, unsigned int offset, unsigned int length);
FORCE_SYSV void KeccakP1600_OverwriteWithZeroes(void *state, unsigned int byteCount);
FORCE_SYSV void KeccakP1600_Permute_Nrounds(void *state, unsigned int nrounds);
FORCE_SYSV void KeccakP1600_Permute_12rounds(void *state);
FORCE_SYSV void KeccakP1600_Permute_24rounds(void *state);
FORCE_SYSV void KeccakP1600_ExtractBytes(const void *state, unsigned char *data, unsigned int offset, unsigned int length);
FORCE_SYSV void KeccakP1600_ExtractAndAddBytes(const void *state, const unsigned char *input, unsigned char *output, unsigned int offset, unsigned int length);
FORCE_SYSV size_t KeccakF1600_FastLoop_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen);
FORCE_SYSV size_t KeccakP1600_12rounds_FastLoop_Absorb(void *state, unsigned int laneCount, const unsigned char *data, size_t dataByteLen);

#endif
