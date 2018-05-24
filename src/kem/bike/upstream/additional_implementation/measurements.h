/***************************************************************************
* Additional implementation of "BIKE: Bit Flipping Key Encapsulation". 
* Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* Written by Nir Drucker and Shay Gueron
* AWS Cryptographic Algorithms Group
* (ndrucker@amazon.com, gueron@amazon.com)
*
* The license is detailed in the file LICENSE.txt, and applies to this file.
* ***************************************************************************/

#ifndef MEASURE_H
#define MEASURE_H

#ifndef RDTSC
    //Less accurate measurement than with RDTSC
    #include <time.h>
    clock_t start;
    clock_t end;

    #define MEASURE(msg, x) start = clock(); {x}; end = clock(); \
    printf(msg); \
    printf("\ttook %lfs\n", ((double) (end-start)/CLOCKS_PER_SEC));
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
      #define WARMUP REPEAT/4
   #endif

    unsigned long long RDTSC_start_clk, RDTSC_end_clk;
    double RDTSC_total_clk;
    double RDTSC_TEMP_CLK;
    int RDTSC_MEASURE_ITERATOR;
    int RDTSC_OUTER_ITERATOR;

inline static uint64_t get_Clks(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtscp\n\t" : "=a"(lo), "=d"(hi)::"rcx");
    return ( (uint64_t)lo)^( ((uint64_t)hi)<<32 );
}

   /* 
   This MACRO measures the number of cycles "x" runs. This is the flow:
      1) it sets the priority to FIFO, to avoid time slicing if possible.
      2) it repeats "x" WARMUP times, in order to warm the cache.
      3) it reads the Time Stamp Counter at the beginning of the test.
      4) it repeats "x" REPEAT number of times.
      5) it reads the Time Stamp Counter again at the end of the test
      6) it calculates the average number of cycles per one iteration of "x", by calculating the total number of cycles, and dividing it by REPEAT
    */      
   #define RDTSC_MEASURE(msg, x)                                                                    \
   for(RDTSC_MEASURE_ITERATOR=0; RDTSC_MEASURE_ITERATOR< WARMUP; RDTSC_MEASURE_ITERATOR++)          \
      {                                                                                             \
         {x};                                                                                       \
      }                                                                                                \
    RDTSC_total_clk = 1.7976931348623157e+308;                                                      \
    for(RDTSC_OUTER_ITERATOR=0;RDTSC_OUTER_ITERATOR<OUTER_REPEAT; RDTSC_OUTER_ITERATOR++){          \
      RDTSC_start_clk = get_Clks();                                                                 \
      for (RDTSC_MEASURE_ITERATOR = 0; RDTSC_MEASURE_ITERATOR < REPEAT; RDTSC_MEASURE_ITERATOR++)   \
      {                                                                                             \
         {x};                                                                                       \
      }                                                                                             \
      RDTSC_end_clk = get_Clks();                                                                   \
      RDTSC_TEMP_CLK = (double)(RDTSC_end_clk-RDTSC_start_clk)/REPEAT;                              \
        if(RDTSC_total_clk>RDTSC_TEMP_CLK) RDTSC_total_clk = RDTSC_TEMP_CLK;                        \
    } \
  printf(msg); \
  printf(" took %0.2f cycles\n", RDTSC_total_clk );

   #define MEASURE(msg, x) RDTSC_MEASURE(msg, x)

#endif

#endif
