#if RADIX == 32
#if defined(SQISIGN_GF_IMPL_SAT32)
#define NWORDS_FIELD 11
#else
#define NWORDS_FIELD 12
#endif
#define NWORDS_ORDER 11
#elif RADIX == 64
#if defined(SQISIGN_GF_IMPL_SAT64)
#define NWORDS_FIELD 6
#else
#define NWORDS_FIELD 6
#endif
#define NWORDS_ORDER 6
#endif
#define BITS 384
#define LOG2P 9
