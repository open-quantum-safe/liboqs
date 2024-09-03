
#define RSDP 1
#define CATEGORY_3 1
#define SPEED 1

#define NO_TREES 1

/* liboqs-edit: when compiling avx2 just assume that Intel Instrinsics are available */
#define IMPLEMENTATION_avx2
#define HIGH_COMPATIBILITY_X86_64
#define HIGH_PERFORMANCE_X86_64

/* PQClean-edit: avoid VLA (don't call sizeof() when creating arrays) */
#define SIZEOF_UINT16 2

/* Undefine unused macros to facilitate dead code removal using unifdef */
#undef SHA_3_LIBKECCAK
/* Variant */
#undef RSDPG
/* Category */
#undef CATEGORY_1
#undef CATEGORY_5
/* Target */
#undef BALANCED
#undef SIG_SIZE
