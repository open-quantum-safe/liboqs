/*
 * Copyright (c) 2016 Microsoft Corporation.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "test_util.h"

/* Platform-specific includes for timing code */
#if OS_TARGET == OS_WIN
#include <windows.h>
#include <intrin.h>
#endif
#if (OS_TARGET == OS_LINUX) && (TARGET == TARGET_ARM)
#include <time.h>
#endif

int64_t cpucycles(void) // Access system counter for benchmarking
{
#if (OS_TARGET == OS_WIN) && (TARGET == TARGET_AMD64 || TARGET == TARGET_x86)
	return __rdtsc();
#elif (OS_TARGET == OS_WIN) && (TARGET == TARGET_ARM)
	return __rdpmccntr64();
#elif (OS_TARGET == OS_LINUX) && (TARGET == TARGET_AMD64 || TARGET == TARGET_x86)
	unsigned int hi, lo;

	asm volatile("rdtsc\n\t"
	             : "=a"(lo), "=d"(hi));
	return ((int64_t) lo) | (((int64_t) hi) << 32);
#elif (OS_TARGET == OS_LINUX) && (TARGET == TARGET_ARM)
	struct timespec time;

	clock_gettime(CLOCK_REALTIME, &time);
	return (int64_t)(time.tv_sec * 1e9 + time.tv_nsec);
#else
	return 0;
#endif
}

void bench_print(char *label, unsigned long long count,
                 unsigned long long num_runs) // Print benchmarking result with the corresponding unit
{
	char *unit;
	unsigned long long result = count / num_runs;

#if (OS_TARGET == OS_LINUX) && (TARGET == TARGET_ARM)
	if (result >= 1e7) {
		unit = "msec";
		result = result / 1e6;
	} else if (result >= 1e4) {
		unit = "usec";
		result = result / 1e3;
	} else {
		unit = "nsec";
	}
#else
	unit = "cycles";
#endif

	printf("  %s runs in %*s %10lld %s\n", label, (int) (50 - strlen(label)),
	       " ", result, unit);

	return;
}
