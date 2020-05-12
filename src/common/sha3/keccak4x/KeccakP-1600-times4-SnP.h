/*
Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
denoted as "the implementer".

For more information, feedback or questions, please refer to our websites:
http://keccak.noekeon.org/
http://keyak.noekeon.org/
http://ketje.noekeon.org/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/

SPDX-License-Identifier: CC0-1.0
*/

/* Simplified for liboqs: remove code that wasn't used in shake128_4x */

#ifndef _KeccakP_1600_times4_SnP_h_
#define _KeccakP_1600_times4_SnP_h_

/** For the documentation, see PlSnP-documentation.h.
 */

#include "SIMD256-config.h"

#define KeccakP1600times4_implementation "256-bit SIMD implementation (" KeccakP1600times4_implementation_config ")"
#define KeccakP1600times4_statesSizeInBytes 800
#define KeccakP1600times4_statesAlignment 32
#define KeccakF1600times4_FastLoop_supported
#define KeccakP1600times4_12rounds_FastLoop_supported

#include <stddef.h>

#define KeccakP1600times4_StaticInitialize()
#define KeccakP1600times4_AddByte(states, instanceIndex, byte, offset) \
    ((unsigned char *) (states))[(instanceIndex) *8 + ((offset) / 8) * 4 * 8 + (offset) % 8] ^= (byte)
void KeccakP1600times4_PermuteAll_24rounds(void *states);

#endif
