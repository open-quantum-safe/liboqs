/********************************************************************************************
* Supersingular Isogeny Key Encapsulation Library
*
*
* Abstract: utility functions for testing and benchmarking
*
*********************************************************************************************/

#include "test_extras.h"

#include <time.h>
#include <stdlib.h>

#include "test_extras.h"
#if (OS_TARGET == OS_WIN)
#include <intrin.h>
#endif
#if (OS_TARGET == OS_UNIX) && (TARGET == TARGET_ARM || TARGET == TARGET_ARM64)
#include <time.h>
#endif
#include <stdlib.h>


int64_t cpucycles(void) { // Access system counter for benchmarking
#if (OS_TARGET == OS_WIN) && (TARGET == TARGET_AMD64 || TARGET == TARGET_x86)
  return __rdtsc();
#elif (OS_TARGET == OS_WIN) && (TARGET == TARGET_ARM)
  return __rdpmccntr64();
#elif (OS_TARGET == OS_UNIX) && (TARGET == TARGET_AMD64 || TARGET == TARGET_x86)
  unsigned int hi, lo;

  asm volatile ("rdtsc\n\t" : "=a" (lo), "=d"(hi));
  return ((int64_t) lo) | (((int64_t) hi) << 32);
#elif (OS_TARGET == OS_UNIX) && (TARGET == TARGET_ARM || TARGET == TARGET_ARM64)
  struct timespec time;

    clock_gettime(CLOCK_REALTIME, &time);
    return (int64_t)(time.tv_sec*1e9 + time.tv_nsec);
#else
    return 0;
#endif
}