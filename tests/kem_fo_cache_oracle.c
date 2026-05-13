// SPDX-License-Identifier: MIT
/*
 *
 * Flush+Reload Cache Timing PoC
 *
 * Usage:
 * sudo ./tests/kem_fo_cache_oracle <KEM> <secret key location>
 *
 * Demonstrates secret-dependent cache residency in FO-transformed KEM decapsulation
 *
 * Raw statistics are printed to stdout. Human-readable summary is printed to stderr.
 *
 * x86_64 Linux only
 *
 * Exampe build commands:
 * mkdir build && cd build
 * cmake -GNinja -DCMAKE_BUILD_TYPE=<Debug|MinSizeRel|RelWithDebInfo|Release> \
 *     -DCMAKE_C_COMPILER="/path/to/clang" \
 *     ..
 * ninja
 * sudo ./tests/kem_fo_cache_oracle FrodoKEM-640-AES 0 2>/dev/null 1>raw.csv
 *
 * NOTE:
 * - sudo may or may not be necessary
 * TODO:
 * - expand to AArch64
 */

#define _GNU_SOURCE
#include <inttypes.h>
#include <oqs/oqs.h>
#include <sched.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#define CALIBRATE_SAMPLE_SIZE 100
#define SAMPLES_PER_ROUND 10000
#define NUM_ROUNDS 10
#define MEMALIGN 4096
#define ONE_BILLION 1000000000
#define CSV_HEADER "epoch,sample,good,probe,ctrl\n"
#define CSV_DATA "%d,%d,%d,%" PRIu64 ",%" PRIu64 "\n"

#define ARGS_HELP_TEXT                                                         \
    "Usage: %s <kem_name> <probe_loc>\n"                                       \
    "Arguments:\n"                                                             \
    "    kem_name: FrodoKEM-640-AES\n"                                         \
    "    probe_loc: 0\n"

static inline void mem_fence(void);
static inline void load_fence(void);
static inline uint64_t probe(volatile void *addr);
static inline void cache_flush(volatile void *addr);
static void setup_realtime(int cpu);

#if defined(__x86_64__)
static inline uint64_t rdtsc(void) {
	uint32_t lo, hi;
	__asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
	return ((uint64_t)hi << 32) | lo;
}

static inline uint64_t rdtscp(void) {
	uint32_t lo, hi, aux;
	__asm__ volatile("rdtscp" : "=a"(lo), "=d"(hi), "=c"(aux));
	return ((uint64_t)hi << 32) | lo;
}

static inline void cache_flush(volatile void *p) {
	__asm__ volatile("clflush (%0)" ::"r"(p) : "memory");
}

static inline uint64_t probe(volatile void *addr) {
	uint64_t t1, t2;
	load_fence();
	t1 = rdtsc();
	load_fence();
	*(volatile char *)addr;
	load_fence();
	t2 = rdtscp();
	return t2 - t1;
}

static inline void mem_fence(void) {
	__asm__ volatile("mfence" ::: "memory");
}

static inline void load_fence(void) {
	__asm__ volatile("lfence" ::: "memory");
}

static void setup_realtime(int cpu) {
	cpu_set_t set;
	CPU_ZERO(&set);
	CPU_SET(cpu, &set);
	if (sched_setaffinity(0, sizeof(set), &set) == 0) {
		fprintf(stderr, "  CPU pinned to %d\n", cpu);
	} else {
		perror("  sched_setaffinity");
	}

	struct sched_param sp;
	sp.sched_priority = sched_get_priority_max(SCHED_FIFO);
	if (sched_setscheduler(0, SCHED_FIFO, &sp) == 0) {
		fprintf(stderr, "  SCHED_FIFO prio %d\n", sp.sched_priority);
	} else {
		perror("  sched_setscheduler");
	}

	if (mlockall(MCL_CURRENT | MCL_FUTURE) == 0) {
		fprintf(stderr, "  mlockall OK\n");
	} else {
		perror("  mlockall");
	}
}
#elif defined(__APPLE__)
#include <mach/mach_time.h>

static inline uint64_t probe(volatile void *addr) {
	uint64_t t1, t2;
	load_fence();
	t1 = mach_absolute_time();
	load_fence();
	*(volatile char *)addr;
	load_fence();
	t2 = mach_absolute_time();
	return t2 - t1;
}

/* FIX:
 * According to Hetterich et al.
 * (https://dl.acm.org/doi/10.1007/978-3-031-09484-2_7) ARMv8 instructions like
 * DC CIVAC do not work on Apple Silicon: the instruction will be executed
 * without exception but actually fails silently.
 */
static inline void cache_flush(volatile void *p) {
	__asm__ volatile("dc civac, %0" ::"r"(p) : "memory");
	__asm__ volatile("dsb ish" ::: "memory"); /* wait for completion */
}

static void setup_realtime(int cpu) {
	(void)cpu;
}

static inline void mem_fence(void) {
	__asm__ volatile("dmb ish" ::: "memory");
}
static inline void load_fence(void) {
	__asm__ volatile("dmb ishld; isb" ::: "memory");
}
#else
/* A truly portable setup that is basically useless for deteting timing channel,
 * but useful for testing compilation
 */
#include <time.h>

static inline uint64_t probe(volatile void *addr) {
	struct timespec t1, t2;
	load_fence();
	clock_gettime(CLOCK_MONOTONIC, &t1);
	load_fence();
	*(volatile char *)addr;
	load_fence();
	clock_gettime(CLOCK_MONOTONIC, &t2);
	return (uint64_t)((t2.tv_sec - t1.tv_sec) * ONE_BILLION) +
	       (uint64_t)(t2.tv_nsec - t1.tv_nsec);
}

static inline void cache_flush(volatile void *addr) {
	(void)addr;
}

static void setup_realtime(int cpu) {
	(void)cpu;
}

static inline void mem_fence(void) {}

static inline void load_fence(void) {}
#endif /* platform-specific code */

static int cmp_u64(const void *a, const void *b) {
	uint64_t va = *(const uint64_t *)a, vb = *(const uint64_t *)b;
	return (va > vb) - (va < vb);
}

static int cmp_double(const void *a, const void *b) {
	double va = *(const double *)a, vb = *(const double *)b;
	return (va > vb) - (va < vb);
}

/* naive prng */
static uint32_t xorshift32(uint32_t *s) {
	uint32_t x = *s;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return (*s = x);
}

static double trimmed_mean(uint64_t *sorted, size_t n) {
	size_t trim = n / 10;
	if (trim < 1) {
		trim = 1;
	}
	double sum = 0;
	for (size_t i = trim; i < n - trim; i++) {
		sum += (double)sorted[i];
	}
	return sum / (double)(n - 2 * trim);
}

typedef struct {
	double good_median, bad_median;
	double good_tmean, bad_tmean;
	double ctrl_good_median, ctrl_bad_median;
	double ctrl_good_tmean, ctrl_bad_tmean;
} round_result_t;

static void run_round(OQS_KEM *kem, uint8_t *sk, uint8_t *ct, uint8_t *ct_bad,
                      uint8_t *ss2, uint32_t *rng, round_result_t *res,
                      size_t probe_loc, size_t ctrl_loc, int epoch) {
	uint64_t *vg = malloc(SAMPLES_PER_ROUND * sizeof(uint64_t));
	uint64_t *vb = malloc(SAMPLES_PER_ROUND * sizeof(uint64_t));
	uint64_t *cg = malloc(SAMPLES_PER_ROUND * sizeof(uint64_t));
	uint64_t *cb = malloc(SAMPLES_PER_ROUND * sizeof(uint64_t));
	size_t ng = 0, nb = 0;

	for (int i = 0; i < SAMPLES_PER_ROUND * 3 &&
	        (ng < SAMPLES_PER_ROUND || nb < SAMPLES_PER_ROUND);
	        i++) {
		int use_bad = xorshift32(rng) & 1;
		if (use_bad && nb >= SAMPLES_PER_ROUND) {
			continue;
		}
		if (!use_bad && ng >= SAMPLES_PER_ROUND) {
			continue;
		}

		uint8_t *tct = use_bad ? ct_bad : ct;

		cache_flush(&sk[probe_loc]);
		cache_flush(&sk[ctrl_loc]);
		mem_fence();
		load_fence();

		OQS_KEM_decaps(kem, ss2, tct, sk);

		mem_fence();
		load_fence();
		uint64_t t_probe = probe(&sk[probe_loc]);
		uint64_t t_ctrl = probe(&sk[ctrl_loc]);

		if (use_bad) {
			vb[nb] = t_probe;
			cb[nb] = t_ctrl;
			printf(CSV_DATA, epoch, i, 0, t_probe, t_ctrl);
			nb++;
		} else {
			vg[ng] = t_probe;
			cg[ng] = t_ctrl;
			printf(CSV_DATA, epoch, i, 1, t_probe, t_ctrl);
			ng++;
		}
	}

	qsort(vg, ng, sizeof(uint64_t), cmp_u64);
	qsort(vb, nb, sizeof(uint64_t), cmp_u64);
	qsort(cg, ng, sizeof(uint64_t), cmp_u64);
	qsort(cb, nb, sizeof(uint64_t), cmp_u64);

	size_t vg_median_loc = ng / 2;
	size_t vb_median_loc = nb / 2;
	res->good_median = (double)vg[vg_median_loc];
	res->bad_median = (double)vb[vb_median_loc];
	res->good_tmean = trimmed_mean(vg, ng);
	res->bad_tmean = trimmed_mean(vb, nb);

	res->ctrl_good_median = (double)cg[vg_median_loc];
	res->ctrl_bad_median = (double)cb[vb_median_loc];
	res->ctrl_good_tmean = trimmed_mean(cg, ng);
	res->ctrl_bad_tmean = trimmed_mean(cb, nb);

	free(vg);
	free(vb);
	free(cg);
	free(cb);
}

static void print_aggregate(const char *label, double *diffs, unsigned int n) {
	qsort(diffs, n, sizeof(double), cmp_double);
	double sum = 0;
	int pos = 0;
	for (int r = 0; r < n; r++) {
		sum += diffs[r];
		if (diffs[r] > 0) {
			pos++;
		}
	}
	fprintf(stderr, "%-10s | %+7.1f  | %+7.1f  | %+7.1f  | %+7.1f  | %d/%d\n",
	        label, diffs[n / 2], sum / n, diffs[n / 4], diffs[n * 3 / 4], pos,
	        n);
}

struct Args {
	const char *kem_name;
	size_t probe_loc;
	size_t ctrl_loc;
};

static void args_init(struct Args *args) {
	args->kem_name = NULL;
	args->ctrl_loc = 63;
}

/* Expected usage:
 * kem_fo_cache_oracle <kem_name> <probe_loc>
 */
static int args_parse(int argc, char *argv[], struct Args *args) {
	if (argc != 3) {
		fprintf(stderr, ARGS_HELP_TEXT, argv[0]);
		return -1;
	}

	args->kem_name = argv[1];
	args->probe_loc = (size_t)strtoull(argv[2], NULL, 10);
	return 0;
}

static void calibrate(unsigned int nsamples, uint8_t *addr) {
	fprintf(stderr, "\n  Calibration:\n");
	uint64_t *cal = malloc(nsamples * sizeof(uint64_t));
	if (!cal) {
		exit(EXIT_FAILURE);
	}
	for (volatile unsigned int i = 0; i < nsamples; i++) {
		*(volatile char *)addr;
		mem_fence();
		cal[i] = probe(addr);
	}
	qsort(cal, nsamples, sizeof(uint64_t), cmp_u64);
	fprintf(stderr, "    Warm hit (read+probe):   p50=%" PRIu64 "\n", cal[nsamples / 2]);

	for (volatile unsigned int i = 0; i < nsamples; i++) {
		cache_flush(addr);
		mem_fence();
		load_fence();
		for (volatile int j = 0; j < 30; j++)
			;
		cal[i] = probe(addr);
	}
	qsort(cal, nsamples, sizeof(uint64_t), cmp_u64);
	fprintf(stderr, "    Cold miss (flush+probe): p50=%" PRIu64 "\n", cal[nsamples / 2]);
}

int main(int argc, char *argv[]) {
	int exitcode = 0;
	struct Args args;
	args_init(&args);
	if (args_parse(argc, argv, &args) < 0) {
		exit(EXIT_FAILURE);
	}
	char probelabel[16], ctrllabel[16], probelabel_good[24], probelabel_bad[24],
	     ctrllabel_good[24], ctrllabel_bad[24];
	double *d0 = NULL, *dc = NULL;
	snprintf(probelabel, sizeof(probelabel), "sk[%lu]", args.probe_loc);
	snprintf(ctrllabel, sizeof(ctrllabel), "sk[%lu]", args.ctrl_loc);
	snprintf(probelabel_good, sizeof(probelabel_good), "%s G.CT", probelabel);
	snprintf(probelabel_bad, sizeof(probelabel_bad), "%s B.CT", probelabel);
	snprintf(ctrllabel_good, sizeof(ctrllabel_good), "%s G.CT", ctrllabel);
	snprintf(ctrllabel_bad, sizeof(ctrllabel_bad), "%s B.CT", ctrllabel);

	setup_realtime(0);

	OQS_KEM *kem = OQS_KEM_new(args.kem_name);
	if (!kem) {
		fprintf(stderr, "%s init failed\n", args.kem_name);
		exit(EXIT_FAILURE);
	}
	if (args.probe_loc >= kem->length_secret_key) {
		fprintf(stderr, "probe offset %lu is off limit %lu\n", args.probe_loc,
		        kem->length_secret_key);
		OQS_KEM_free(kem);
		exit(EXIT_FAILURE);
	}

	fprintf(stderr, "%s Flush+Reload cache timing detector\n", args.kem_name);
	fprintf(stderr, "  Probes: %s (target) + %s (control)\n", probelabel,
	        ctrllabel);
	fprintf(stderr, "  Rounds=%d  Samples/round=%d\n", NUM_ROUNDS,
	        SAMPLES_PER_ROUND);

	uint8_t *pk = malloc(kem->length_public_key);
	uint8_t *sk;
	if (posix_memalign((void **)&sk, MEMALIGN, kem->length_secret_key)) {
		fprintf(stderr, "posix_memalign failed\n");
		return 1;
	}
	uint8_t *ct = malloc(kem->length_ciphertext);
	uint8_t *ct_bad = malloc(kem->length_ciphertext);
	uint8_t *ss1 = malloc(kem->length_shared_secret);
	uint8_t *ss2 = malloc(kem->length_shared_secret);
	if (!pk || !ct || !ct_bad || !ss1 || !ss2) {
		fprintf(stderr, "malloc failed\n");
		exitcode = 1;
		goto exit;
	}

	calibrate(CALIBRATE_SAMPLE_SIZE, sk);

	if (OQS_KEM_keypair(kem, pk, sk) != OQS_SUCCESS) {
		fprintf(stderr, "OQS_KEM_keypair failed\n");
		exitcode = 1;
		goto exit;
	}
	if (OQS_KEM_encaps(kem, ct, ss1, pk) != OQS_SUCCESS) {
		fprintf(stderr, "OQS_KEM_encaps failed\n");
		exitcode = 1;
		goto exit;
	}
	memcpy(ct_bad, ct, kem->length_ciphertext);
	for (int i = 0; i < 32; i++) {
		ct_bad[i] ^= 0xFF;
	}

	fprintf(stderr, "  sk @ %p\n", (void *)sk);

	for (volatile int i = 0; i < CALIBRATE_SAMPLE_SIZE; i++) {
		OQS_KEM_decaps(kem, ss2, ct, sk);
		OQS_KEM_decaps(kem, ss2, ct_bad, sk);
	}

	round_result_t R[NUM_ROUNDS];
	uint32_t rng = 42;

	fprintf(stderr, "\n=== Per-round statistics (cycles, trimmed mean) ===\n");
	fprintf(stderr, "%4s | %18s | %18s | %18s | %18s\n", "R", probelabel_good,
	        probelabel_bad, ctrllabel_good, ctrllabel_bad);

	/* the main loop */
	printf(CSV_HEADER);
	for (int r = 0; r < NUM_ROUNDS; r++) {
		run_round(kem, sk, ct, ct_bad, ss2, &rng, &R[r], args.probe_loc,
		          args.ctrl_loc, r);
		fprintf(stderr,
		        "  %02d | %14.1f     | %14.1f     | %14.1f     | %14.1f\n",
		        r + 1, R[r].good_tmean, R[r].bad_tmean, R[r].ctrl_good_tmean,
		        R[r].ctrl_bad_tmean);
	}

	double diff_tmean_0[NUM_ROUNDS], diff_tmean_ctrl[NUM_ROUNDS];
	for (int r = 0; r < NUM_ROUNDS; r++) {
		diff_tmean_0[r] = R[r].good_tmean - R[r].bad_tmean;
		diff_tmean_ctrl[r] = R[r].ctrl_good_tmean - R[r].ctrl_bad_tmean;
	}

	fprintf(stderr,
	        "\n=== Aggregate: good - bad (positive = good slower = target "
	        "colder for good) ===\n");
	fprintf(stderr, "%-10s | %8s | %8s | %8s | %8s | positive\n", "Probe",
	        "Median", "Mean", "Q1", "Q3");
	print_aggregate(probelabel, diff_tmean_0, NUM_ROUNDS);
	print_aggregate(ctrllabel, diff_tmean_ctrl, NUM_ROUNDS);

	d0 = malloc(NUM_ROUNDS * sizeof(double));
	dc = malloc(NUM_ROUNDS * sizeof(double));
	memcpy(d0, diff_tmean_0, NUM_ROUNDS * sizeof(double));
	memcpy(dc, diff_tmean_ctrl, NUM_ROUNDS * sizeof(double));
	qsort(d0, NUM_ROUNDS, sizeof(double), cmp_double);
	qsort(dc, NUM_ROUNDS, sizeof(double), cmp_double);

	int pos_0 = 0, pos_c = 0;
	for (int r = 0; r < NUM_ROUNDS; r++) {
		if (diff_tmean_0[r] > 0) {
			pos_0++;
		}
		if (diff_tmean_ctrl[r] > 0) {
			pos_c++;
		}
	}

	fprintf(stderr, "\n=== Verdict ===\n");
	fprintf(stderr,
	        "%s  (cmovs target):  median diff = %+.1f  positive = %d/%d\n",
	        probelabel, d0[NUM_ROUNDS / 2], pos_0, NUM_ROUNDS);
	fprintf(stderr,
	        "%s (control line):  median diff = %+.1f  positive = %d/%d\n",
	        ctrllabel, dc[NUM_ROUNDS / 2], pos_c, NUM_ROUNDS);

	if (d0[NUM_ROUNDS / 2] > 1.0 && pos_0 > NUM_ROUNDS * 2 / 3) {
		fprintf(stderr,
		        "\n%s: SECRET-DEPENDENT cache residency change detected\n",
		        probelabel);
		fprintf(stderr, "  %d/%d rounds show consistent direction\n", pos_0,
		        NUM_ROUNDS);
		if (pos_c <= NUM_ROUNDS * 2 / 3) {
			fprintf(stderr, "%s: No consistent direction (%d/%d)\n", ctrllabel,
			        pos_c, NUM_ROUNDS);
			fprintf(stderr,
			        "  -> Signal is specific to the conditional load target, "
			        "not overall noise\n");
		}
	} else {
		fprintf(stderr,
		        "\nNo significant difference at %s (med=%.1f, pos=%d/%d)\n",
		        probelabel, d0[NUM_ROUNDS / 2], pos_0, NUM_ROUNDS);
	}

exit:
	free(d0);
	free(dc);
	free(pk);
	free(sk);
	free(ct);
	free(ct_bad);
	free(ss1);
	free(ss2);
	OQS_KEM_free(kem);
	return exitcode;
}
