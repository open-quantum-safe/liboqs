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

#include "KeccakSpongetimes4.h"

#ifdef XKCP_has_KeccakP1600
#include "KeccakP-1600-times4-SnP.h"

#define prefix oqs_sig_picnic_KeccakWidth1600times4
#define PlSnP oqs_sig_picnic_KeccakP1600times4
#define PlSnP_width 1600
#define PlSnP_Permute oqs_sig_picnic_KeccakP1600times4_PermuteAll_24rounds
#if defined(KeccakF1600times4_FastLoop_supported)
//can we enable fastloop absorb?
//#define PlSnP_FastLoop_Absorb KeccakF1600times4_FastLoop_Absorb
#endif
#include "KeccakSpongetimes4.inc"
#undef prefix
#undef PlSnP
#undef PlSnP_width
#undef PlSnP_Permute
#undef PlSnP_FastLoop_Absorb
#endif
