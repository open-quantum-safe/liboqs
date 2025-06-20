#if RADIX == 32
#if defined(SQISIGN_GF_IMPL_BROADWELL)
#define NWORDS_FIELD 8
#else
#define NWORDS_FIELD 9
#endif
#define NWORDS_ORDER 8
#elif RADIX == 64
#if defined(SQISIGN_GF_IMPL_BROADWELL)
#define NWORDS_FIELD 4
#else
#define NWORDS_FIELD 5
#endif
#define NWORDS_ORDER 4
#endif
#define BITS 256
#define LOG2P 8
