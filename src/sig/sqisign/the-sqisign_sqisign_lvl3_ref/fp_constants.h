#if RADIX == 32
#if defined(SQISIGN_GF_IMPL_BROADWELL)
#define NWORDS_FIELD 12
#else
#define NWORDS_FIELD 14
#endif
#define NWORDS_ORDER 12
#elif RADIX == 64
#if defined(SQISIGN_GF_IMPL_BROADWELL)
#define NWORDS_FIELD 6
#else
#define NWORDS_FIELD 7
#endif
#define NWORDS_ORDER 6
#endif
#define BITS 384
#define LOG2P 9
