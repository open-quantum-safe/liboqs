#ifndef PQCLEAN_DILITHIUM3_AVX2_CDECL
#define PQCLEAN_DILITHIUM3_AVX2_CDECL

/* The C ABI on MacOS exports all symbols with a leading
 * underscore. This means that any symbols we refer to from
 * C files (functions) can't be found, and all symbols we
 * refer to from ASM also can't be found (nttconsts.c).
 *
 * This define helps us get around this
 */

#if defined(__WIN32__) || defined(__APPLE__)
#define cdecl(s) _##s
#else
#define cdecl(s) s
#endif

#endif
