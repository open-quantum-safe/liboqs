#if RADIX == 32
#if defined(SQISIGN_GF_IMPL_SAT32)
#define NWORDS_FIELD 21
#else
#define NWORDS_FIELD 24
#endif
#define NWORDS_ORDER 21
#elif RADIX == 64
#if defined(SQISIGN_GF_IMPL_SAT64)
#define NWORDS_FIELD 11
#else
#define NWORDS_FIELD 11
#endif
#define NWORDS_ORDER 11
#endif
#define BITS 704
#define LOG2P 10
