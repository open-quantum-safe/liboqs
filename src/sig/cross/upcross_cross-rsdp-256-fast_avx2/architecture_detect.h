
#pragma once

/* liboqs-edit: avoid checking for ISA extensions
 * when compiling avx2 just assume that Intel Instrinsics are available */
#include <immintrin.h>
#include <pmmintrin.h>
#include <stdalign.h>
#include <wmmintrin.h>
//#include <x86intrin.h> // liboqs-edit: x86intrin.h is not available on Windows

#define EPI8_PER_REG 32
#define EPI16_PER_REG 16
