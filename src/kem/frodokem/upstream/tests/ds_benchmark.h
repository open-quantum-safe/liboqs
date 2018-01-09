/********************************************************************************************
 * ds_benchmark.h: Macros for simple benchmarking of C code.
 *
 * See instructions for usage below.
 * Software originally developed by Douglas Stebila.
 * Most recent version at https://gist.github.com/dstebila/6980008ec98209ef6075
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org>
 ********************************************************************************************/

/** \file ds_benchmark.h
 * Macros for simple benchmarking of C code.
 */

#ifndef _DS_BENCHMARK_H
#define _DS_BENCHMARK_H

#include "../src/config.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#if (OS_TARGET == OS_NIX)
#include <sys/time.h>
#endif
#include <time.h>
#include <math.h>


#if (OS_TARGET == OS_WIN)
#include <Windows.h>

int gettimeofday(struct timeval *tp, struct timezone *tzp) {
	// Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
	static const uint64_t EPOCH = ((uint64_t)116444736000000000ULL);

	SYSTEMTIME  system_time;
	FILETIME    file_time;
	uint64_t    time;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	time = ((uint64_t)file_time.dwLowDateTime);
	time += ((uint64_t)file_time.dwHighDateTime) << 32;

	tp->tv_sec = (long)((time - EPOCH) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);
	return 0;
}
#endif


static uint64_t rdtsc(void) {
#if (OS_TARGET == OS_WIN)
	return __rdtsc();
#elif (OS_TARGET == OS_NIX) && (TARGET == TARGET_AMD64 || TARGET == TARGET_x86)
	uint64_t x;
	__asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
	return x;
#elif (OS_TARGET == OS_NIX && TARGET == TARGET_ARM)
    struct timespec time;
    clock_gettime(CLOCK_REALTIME, &time);
    return (int64_t)(time.tv_sec*1e9 + time.tv_nsec);
#else
    return 0;            
#endif
}


#define DEFINE_TIMER_VARIABLES \
	volatile uint64_t _bench_cycles_start, _bench_cycles_end; \
	uint64_t _bench_cycles_cumulative = 0; \
	int64_t _bench_cycles_diff, _bench_iterations; \
	struct timeval _bench_timeval_start, _bench_timeval_end; \
	double _bench_cycles_x, _bench_cycles_mean, _bench_cycles_delta, _bench_cycles_M2, _bench_cycles_stdev; \
	double _bench_time_x, _bench_time_mean, _bench_time_delta, _bench_time_M2, _bench_time_stdev, _bench_time_cumulative;

#define INITIALIZE_TIMER \
	_bench_iterations = 0; \
	_bench_cycles_mean = 0.0; \
	_bench_cycles_M2 = 0.0; \
	_bench_time_cumulative = 0.0; \
	_bench_time_mean = 0.0; \
	_bench_time_M2 = 0.0;

#define START_TIMER \
	gettimeofday(&_bench_timeval_start, NULL); \
	_bench_cycles_start = rdtsc();


// Mean and population standard deviation are calculated in an online way using the algorithm in
//     http://en.wikipedia.org/wiki/Algorithms_for_calculating_variance#Online_algorithm

#define STOP_TIMER \
	_bench_cycles_end = rdtsc(); \
	gettimeofday(&_bench_timeval_end, NULL); \
	_bench_iterations += 1; \
	if (_bench_cycles_end < _bench_cycles_start) { _bench_cycles_end += (uint64_t)1 << 32; } \
    _bench_cycles_diff = _bench_cycles_end; \
	_bench_cycles_diff -= _bench_cycles_start; \
	_bench_cycles_cumulative += _bench_cycles_diff; \
	_bench_cycles_x = (double) (_bench_cycles_diff); \
	_bench_cycles_delta = _bench_cycles_x - _bench_cycles_mean; \
	_bench_cycles_mean += _bench_cycles_delta / (double) _bench_iterations; \
	_bench_cycles_M2 += _bench_cycles_delta * (_bench_cycles_x - _bench_cycles_mean); \
	_bench_time_x = (double) ((_bench_timeval_end.tv_sec * 1000000 + _bench_timeval_end.tv_usec) - (_bench_timeval_start.tv_sec * 1000000 + _bench_timeval_start.tv_usec)); \
	_bench_time_delta = _bench_time_x - _bench_time_mean; \
	_bench_time_mean += _bench_time_delta / (double) _bench_iterations; \
	_bench_time_M2 += _bench_time_delta * (_bench_time_x - _bench_time_mean); \
	_bench_time_cumulative += _bench_time_x;

#define FINALIZE_TIMER \
	if (_bench_iterations == 2) { _bench_cycles_stdev = 0.0; } \
	else { _bench_cycles_stdev = sqrt(_bench_cycles_M2 / (double) _bench_iterations); } \
	if (_bench_iterations == 2) { _bench_time_stdev = 0.0; } \
	else { _bench_time_stdev = sqrt(_bench_time_M2 / (double) _bench_iterations); }

#if (TARGET == TARGET_ARM)
#define PRINT_TIMER_HEADER \
	printf("%-30s %15s %15s %15s %15s\n", "Operation", "Iterations", "Total time (s)", "Time(us): mean", "pop. stdev");
#else
#define PRINT_TIMER_HEADER \
	printf("%-30s %15s %15s %15s %15s %15s %15s\n", "Operation", "Iterations", "Total time (s)", "Time(us): mean", "pop. stdev", "Cycles: mean", "pop. stdev");
#endif

#if (TARGET == TARGET_ARM)
#define PRINT_TIMER_AVG(op_name) \
	printf("%-30s %15" PRIu64 " %15.3f %15.3f %15.3f\n", (op_name), _bench_iterations, _bench_time_cumulative / 1000000.0, _bench_time_mean, _bench_time_stdev); \
	(void)_bench_cycles_stdev;
#else
#define PRINT_TIMER_AVG(op_name) \
	printf("%-30s %15" PRIu64 " %15.3f %15.3f %15.3f %15.0f %15.0f\n", (op_name), _bench_iterations, _bench_time_cumulative / 1000000.0, _bench_time_mean, _bench_time_stdev, ((double) _bench_cycles_cumulative) / (double) _bench_iterations, _bench_cycles_stdev);
#endif

#define TIME_OPERATION_ITERATIONS(op, op_name, it) \
	{ \
		DEFINE_TIMER_VARIABLES \
		INITIALIZE_TIMER \
		for (int i = 0; i < (it); i++) { \
			START_TIMER \
			{ op ; } \
			STOP_TIMER \
		} \
		FINALIZE_TIMER \
		PRINT_TIMER_AVG(op_name) \
	}

#define TIME_OPERATION_SECONDS(op, op_name, secs) \
	{ \
		DEFINE_TIMER_VARIABLES \
		INITIALIZE_TIMER \
		uint64_t _bench_time_goal_usecs = 1000000 * secs; \
		while ((uint64_t)_bench_time_cumulative < _bench_time_goal_usecs) { \
			START_TIMER \
			{ op ; } \
			STOP_TIMER \
		} \
		FINALIZE_TIMER \
		PRINT_TIMER_AVG(op_name) \
	}
#endif
