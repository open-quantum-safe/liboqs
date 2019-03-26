/*
Implementation by Vladimir Sedach, hereby denoted as "the implementer".

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _KeccakP_1600_SnP_h_
#define _KeccakP_1600_SnP_h_

/** For the documentation, see SnP-documentation.h.
 */

#define KeccakP1600_implementation      "AVX2 optimized implementation"
#define KeccakP1600_stateSizeInBytes    (7 * 4 * 8)
#define KeccakP1600_stateAlignment      32
#define KeccakF1600_FastLoop_supported

#include <stddef.h>
#include "KeccakP-1600-AVX2.h"

#endif
