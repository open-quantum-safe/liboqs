// SPDX-License-Identifier: Apache-2.0
#ifndef BENCH_H__
#define BENCH_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#if defined(__APPLE__)
#include "bench_macos.h"
#endif

#if defined(TARGET_ARM) || defined(TARGET_S390X) || defined(NO_CYCLE_COUNTER)
#define BENCH_UNIT0 "nanoseconds"
#define BENCH_UNIT3 "microseconds"
#define BENCH_UNIT6 "milliseconds"
#define BENCH_UNIT9 "seconds"
#else
#define BENCH_UNIT0 "cycles"
#define BENCH_UNIT3 "kilocycles"
#define BENCH_UNIT6 "megacycles"
#define BENCH_UNIT9 "gigacycles"
#endif

static inline void
cpucycles_init(void) {
#if defined(__APPLE__) && defined(TARGET_ARM64)
    macos_init_rdtsc();
#endif
}

static inline uint64_t
cpucycles(void)
{
#if defined(TARGET_AMD64) || defined(TARGET_X86)
    uint32_t hi, lo;

    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)lo) | ((uint64_t)hi << 32);
#elif defined(TARGET_S390X)
    uint64_t tod;
    asm volatile("stckf %0\n" : "=Q"(tod) : : "cc");
    return (tod * 1000 / 4096);
#elif defined(TARGET_ARM64) && !defined(NO_CYCLE_COUNTER)
#if defined(__APPLE__)
    return macos_rdtsc();
#else
    uint64_t cycles;
    asm volatile("mrs %0, PMCCNTR_EL0" : "=r"(cycles));
    return cycles;
#endif // __APPLE__
#else
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return (uint64_t)time.tv_sec * 1000000000 + time.tv_nsec;
#endif
}

static inline int
CMPFUNC(const void *a, const void *b)
{
    uint64_t aa = *(uint64_t *)a, bb = *(uint64_t *)b;

    if (aa > bb)
        return +1;
    if (aa < bb)
        return -1;
    return 0;
}

static inline uint32_t
ISQRT(uint64_t x)
{
    uint32_t r = 0;
    for (ssize_t i = 31; i >= 0; --i) {
        uint32_t s = r + (1 << i);
        if ((uint64_t)s * s <= x)
            r = s;
    }
    return r;
}

static inline double
_TRUNC(uint64_t x)
{
    return x / 1000 / 1000.;
}
#define _FMT ".3lf"
#define _UNIT BENCH_UNIT6

#define BENCH_CODE_1(RUNS)                                                                         \
    {                                                                                              \
        const size_t count = (RUNS);                                                               \
        if (!count)                                                                                \
            abort();                                                                               \
        uint64_t cycles, cycles1, cycles2;                                                         \
        uint64_t cycles_list[count];                                                               \
        cycles = 0;                                                                                \
        for (size_t i = 0; i < count; ++i) {                                                       \
            cycles1 = cpucycles();

#define BENCH_CODE_2(name)                                                                         \
    cycles2 = cpucycles();                                                                         \
    cycles_list[i] = cycles2 - cycles1;                                                            \
    cycles += cycles2 - cycles1;                                                                   \
    }                                                                                              \
    qsort(cycles_list, count, sizeof(uint64_t), CMPFUNC);                                          \
    uint64_t variance = 0;                                                                         \
    for (size_t i = 0; i < count; ++i) {                                                           \
        int64_t off = cycles_list[i] - cycles / count;                                             \
        variance += off * off;                                                                     \
    }                                                                                              \
    variance /= count;                                                                             \
    printf("  %-10s", name);                                                                       \
    printf(" | average %9" _FMT " | stddev %9" _FMT,                                               \
           _TRUNC(cycles / count),                                                                 \
           _TRUNC(ISQRT(variance)));                                                               \
    printf(" | median %9" _FMT " | min %9" _FMT " | max %9" _FMT,                                  \
           _TRUNC(cycles_list[count / 2]),                                                         \
           _TRUNC(cycles_list[0]),                                                                 \
           _TRUNC(cycles_list[count - 1]));                                                        \
    printf("  (%s)\n", _UNIT);                                                                     \
    }

#endif
