#if RADIX == 32
#if defined(SQISIGN_GF_IMPL_BROADWELL)
#define NWORDS_FIELD 16
#else
#define NWORDS_FIELD 18
#endif
#define NWORDS_ORDER 16
#elif RADIX == 64
#if defined(SQISIGN_GF_IMPL_BROADWELL)
#define NWORDS_FIELD 8
#else
#define NWORDS_FIELD 9
#endif
#define NWORDS_ORDER 8
#endif
#define BITS 512
#define LOG2P 9
