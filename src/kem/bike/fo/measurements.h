/*
 * Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 * http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 * The license is detailed in the file LICENSE.md, and applies to this file.
 *
 * Written by Nir Drucker and Shay Gueron
 * AWS Cryptographic Algorithms Group.
 * (ndrucker@amazon.com, gueron@amazon.com)
 */

#ifndef MEASURE_H
#define MEASURE_H

#ifndef RDTSC
// Less accurate measurement than with RDTSC
#include <time.h>
clock_t start;
clock_t end;

#define MEASURE(msg, x) \
	start = clock();    \
	{x};                \
	end = clock();      \
	printf(msg);        \
	printf("\ttook %lfs\n", ((double) (end - start) / CLOCKS_PER_SEC));
#endif

/* This part defines the functions and MACROS needed to measure using RDTSC */
#ifdef RDTSC

#ifndef REPEAT
#define REPEAT 100
#endif

#ifndef OUTER_REPEAT
#define OUTER_REPEAT 10
#endif

#ifndef WARMUP
#define WARMUP REPEAT / 4
#endif

#define MAX_DOUBLE_VALUE 1.7976931348623157e+308

unsigned long long RDTSC_start_clk, RDTSC_end_clk;
double RDTSC_total_clk;
double RDTSC_TEMP_CLK;
int RDTSC_MEASURE_ITERATOR;
int RDTSC_OUTER_ITERATOR;

inline static uint64_t
get_Clks(void) {
	unsigned hi, lo;
	__asm__ __volatile__("rdtscp\n\t"
	                     : "=a"(lo), "=d"(hi)::"rcx");
	return ((uint64_t) lo) ^ (((uint64_t) hi) << 32);
}

/*
This MACRO measures the number of cycles "x" runs. This is the flow:
   1) it repeats "x" WARMUP times, in order to warm the cache.
   2) it reads the Time Stamp Counter at the beginning of the test.
   3) it repeats "x" REPEAT number of times.
   4) it reads the Time Stamp Counter again at the end of the test
   5) it calculates the average number of cycles per one iteration of "x", by
calculating the total number of cycles, and dividing it by REPEAT
 */
#define RDTSC_MEASURE(msg, x)                                                 \
	for (RDTSC_MEASURE_ITERATOR = 0; RDTSC_MEASURE_ITERATOR < WARMUP;         \
	     RDTSC_MEASURE_ITERATOR++) {                                          \
		{x};                                                                  \
	}                                                                         \
	RDTSC_total_clk = MAX_DOUBLE_VALUE;                                       \
	for (RDTSC_OUTER_ITERATOR = 0; RDTSC_OUTER_ITERATOR < OUTER_REPEAT;       \
	     RDTSC_OUTER_ITERATOR++) {                                            \
		RDTSC_start_clk = get_Clks();                                         \
		for (RDTSC_MEASURE_ITERATOR = 0; RDTSC_MEASURE_ITERATOR < REPEAT;     \
		     RDTSC_MEASURE_ITERATOR++) {                                      \
			{x};                                                              \
		}                                                                     \
		RDTSC_end_clk = get_Clks();                                           \
		RDTSC_TEMP_CLK = (double) (RDTSC_end_clk - RDTSC_start_clk) / REPEAT; \
		if (RDTSC_total_clk > RDTSC_TEMP_CLK)                                 \
			RDTSC_total_clk = RDTSC_TEMP_CLK;                                 \
	}                                                                         \
	printf(msg);                                                              \
	printf(" took %0.2f cycles\n", RDTSC_total_clk);

#define MEASURE(msg, x) RDTSC_MEASURE(msg, x)

#endif

#endif
