/*
 * Copyright (c) 2016 Microsoft Corporation.
 */

#define OS_WIN 1
#define OS_LINUX 2

#define TARGET_AMD64 1

#if defined(__WINDOWS__)
#define OS_TARGET OS_WIN
#elif defined(__LINUX__)
#define OS_TARGET OS_LINUX
#else
#error-- "Unknown OS"
#endif

#ifdef __X64__
#define TARGET TARGET_AMD64
#else
// Default to x64
#define TARGET TARGET_AMD64
#endif

#include <stdint.h>

// Print benchmarking result with the corresponding unit
void bench_print(char *label, unsigned long long count,
                 unsigned long long num_runs);

// Access system counter for benchmarking
int64_t cpucycles(void);
