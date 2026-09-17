/*
This file defines some parameters of the implementation in the parent directory.
*/

#define KeccakP1600times4_implementation_config "AVX2, all rounds unrolled"
#define KeccakP1600times4_fullUnrolling
#define KeccakP1600times4_useAVX2

/* target attribute */
#ifndef __has_attribute
#define __has_attribute(a) 0
#endif
#if defined(__GNUC__) || __has_attribute(target)
#define ATTRIBUTE_TARGET_AVX2 __attribute__((target(("avx2"))))
#else
#define ATTRIBUTE_TARGET_AVX2
#endif

