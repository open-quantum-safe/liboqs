#ifndef sdith_inline_H
#define sdith_inline_H

// Force inlining of a file-local helper.
// Designed to be behaviourally consistent across different compilers.
#if defined(_MSC_VER)
#define SDITH_ALWAYS_INLINE static __forceinline
#elif defined(__GNUC__) || defined(__clang__)
#define SDITH_ALWAYS_INLINE static inline __attribute__((__always_inline__))
#else
#define SDITH_ALWAYS_INLINE static inline
#endif

// Mark a variable as possibly unused, to suppress compiler warnings.
#if defined(__GNUC__) || defined(__clang__)
#define SDITH_MAYBE_UNUSED __attribute__((__unused__))
#else
#define SDITH_MAYBE_UNUSED
#endif

#endif  // sdith_inline_H
