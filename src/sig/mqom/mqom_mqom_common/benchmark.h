#ifndef __BENCHMARK_H__
#define __BENCHMARK_H__

#define NUMBER_OF_BENCHES 21

// Signing
#define BS_BLC_COMMIT 0
#define BS_PIOP_COMPUTE 1
#define BS_SAMPLE_CHALLENGE 2
#define BS_BLC_OPEN 3

#define BS_BLC_EXPAND_TREE 4
#define BS_BLC_SEED_COMMIT 5
#define BS_BLC_PRG 6
#define BS_BLC_XOF 7
#define BS_BLC_ARITH 8

#define BS_PIOP_EXPAND_MQ 9
#define BS_PIOP_MAT_MUL_EXT 10
#define BS_PIOP_COMPUTE_T1 11
#define BS_PIOP_COMPUTE_PZI 12
#define BS_PIOP_EXPAND_BATCHING_MAT 13
#define BS_PIOP_BATCH_AND_MASK 14

// Verification
// Currently no detailed benchmark is performed on the
//   verification algorithm

// Sign overhead (Fiat-Shamir hash chain, not covered by BLC/PIOP probes)
#define BS_SIGN_HASH_MSG   15   // Hash_3: message hash before BLC.Commit
#define BS_SIGN_HASH_FS    16   // Hash_1+2+4: alpha serialization + Fiat-Shamir after PIOP

// User wildcard probes (no fixed semantics, available for ad-hoc instrumentation)
#define B_PIN_A 17
#define B_PIN_B 18
#define B_PIN_C 19
#define B_PIN_D 20

#ifndef BENCHMARK
#define __BENCHMARK_START__(label) {}
#define __BENCHMARK_STOP__(label) {}

#else
#include "benchmark/timing.h"
__attribute__((weak)) btimer_t timers[NUMBER_OF_BENCHES];
void btimer_start(btimer_t *timer);
void btimer_end(btimer_t *timer);

/* NOTE: dummy definitions to be replaced when benchmarks needed */
__attribute__((weak, noinline)) void btimer_start(btimer_t *timer) {
	(void)timer;
}
__attribute__((weak, noinline)) void btimer_end(btimer_t *timer) {
	(void)timer;
}

#define __BENCHMARK_START__(label) btimer_start(&timers[label])
#define __BENCHMARK_STOP__(label) btimer_end(&timers[label])
#endif

#endif /* __BENCHMARK_H__ */
