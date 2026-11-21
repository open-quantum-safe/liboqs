// SPDX-License-Identifier: MIT

/*
 * Peak stack usage of liboqs KEM and signature operations, by stack painting.
 *
 * For every enabled algorithm matching an optional name filter, a child process
 * is forked (so that a stack overflow only takes down that algorithm). Each
 * operation (keypair, encaps/sign, decaps/verify) then runs in its own thread
 * on a caller-supplied stack that was pre-filled with a known pattern. After
 * the thread is joined, the stack is scanned from its far (lowest) end for the
 * first word that no longer holds the pattern; everything above it was used.
 * The cost of an empty thread (TCB/static TLS placed on the stack by libc,
 * thread start-up frames) is measured once per child and subtracted.
 *
 * Inputs and outputs (keys, ciphertexts, signatures, messages) are heap
 * allocated, so the figures are the operation's own stack usage.
 *
 * Usage: stack_usage [-s <stack MiB>] [<algorithm name substring>]
 *
 * Output: tab-separated lines "kind  alg  op  peak_bytes  status", where
 * status is "ok", "error" (API call failed / result mismatch) or
 * "crash:<signal>". Lines starting with '#' carry metadata.
 *
 * See docs/STACK_USAGE.md and scripts/measure_stack_usage.py.
 */

#define _GNU_SOURCE
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

#include <oqs/oqs.h>

#ifndef MAP_NORESERVE
#define MAP_NORESERVE 0
#endif
#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif

/* Painting and scanning touch memory that was (or will be) a thread stack;
 * keep AddressSanitizer from instrumenting those accesses. */
#if defined(__has_feature)
#if __has_feature(address_sanitizer)
#define NO_ASAN __attribute__((no_sanitize("address")))
#endif
#endif
#if !defined(NO_ASAN) && defined(__SANITIZE_ADDRESS__)
#define NO_ASAN __attribute__((no_sanitize("address")))
#endif
#ifndef NO_ASAN
#define NO_ASAN
#endif

#define PATTERN UINT64_C(0xA5C3E1F00F1E3C5A)
#define GUARD_SIZE ((size_t)1 << 16)
#define DEFAULT_STACK_MIB 64
#define MSG_LEN 100
#define EXTMU_LEN 64

static uint8_t *stack_base; /* lowest usable address, directly above the guard */
static size_t stack_size;

typedef int (*op_fn)(void *ctx);

struct thread_arg {
	op_fn fn;
	void *ctx;
	int rc;
};

static void *trampoline(void *p) {
	struct thread_arg *t = p;
	t->rc = t->fn(t->ctx);
	return NULL;
}

NO_ASAN static void paint(void) {
	uint64_t *w = (uint64_t *)(void *)stack_base;
	for (size_t i = 0; i < stack_size / sizeof(uint64_t); i++) {
		w[i] = PATTERN;
	}
}

NO_ASAN static size_t scan(void) {
	const uint64_t *w = (const uint64_t *)(void *)stack_base;
	size_t n = stack_size / sizeof(uint64_t);
	size_t i = 0;
	while (i < n && w[i] == PATTERN) {
		i++;
	}
	return stack_size - i * sizeof(uint64_t);
}

/* Runs fn(ctx) in a thread on the painted stack; returns bytes touched, or 0
 * with *rc = -1 if the thread could not be created. */
static size_t measure(op_fn fn, void *ctx, int *rc) {
	pthread_attr_t attr;
	pthread_t th;
	struct thread_arg t = { fn, ctx, -1 };

	paint();
	if (pthread_attr_init(&attr) != 0 ||
	        pthread_attr_setstack(&attr, stack_base, stack_size) != 0 ||
	        pthread_create(&th, &attr, trampoline, &t) != 0) {
		*rc = -1;
		return 0;
	}
	pthread_join(th, NULL);
	pthread_attr_destroy(&attr);
	*rc = t.rc;
	return scan();
}

static int noop(void *ctx) {
	(void)ctx;
	return 0;
}

static void report(const char *kind, const char *alg, const char *op, size_t used, size_t baseline, int ok) {
	size_t net = used > baseline ? used - baseline : 0;
	printf("%s\t%s\t%s\t%zu\t%s\n", kind, alg, op, net, ok ? "ok" : "error");
}

/* ---------------------------------------------------------------- KEM */

struct kem_ctx {
	OQS_KEM *kem;
	uint8_t *pk, *sk, *ct, *ss_e, *ss_d;
};

static int kem_keypair(void *c) {
	struct kem_ctx *x = c;
	return OQS_KEM_keypair(x->kem, x->pk, x->sk) == OQS_SUCCESS ? 0 : 1;
}

static int kem_encaps(void *c) {
	struct kem_ctx *x = c;
	return OQS_KEM_encaps(x->kem, x->ct, x->ss_e, x->pk) == OQS_SUCCESS ? 0 : 1;
}

static int kem_decaps(void *c) {
	struct kem_ctx *x = c;
	if (OQS_KEM_decaps(x->kem, x->ss_d, x->ct, x->sk) != OQS_SUCCESS) {
		return 1;
	}
	return memcmp(x->ss_e, x->ss_d, x->kem->length_shared_secret) == 0 ? 0 : 1;
}

static int run_kem(const char *name, size_t baseline) {
	const op_fn ops[] = { kem_keypair, kem_encaps, kem_decaps };
	const char *op_names[] = { "keypair", "encaps", "decaps" };
	struct kem_ctx x;
	int failed = 0;

	x.kem = OQS_KEM_new(name);
	if (x.kem == NULL) {
		return 1;
	}
	x.pk = malloc(x.kem->length_public_key);
	x.sk = malloc(x.kem->length_secret_key);
	x.ct = malloc(x.kem->length_ciphertext);
	x.ss_e = malloc(x.kem->length_shared_secret);
	x.ss_d = malloc(x.kem->length_shared_secret);
	if (!x.pk || !x.sk || !x.ct || !x.ss_e || !x.ss_d) {
		fprintf(stderr, "%s: out of memory\n", name);
		return 1;
	}
	for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); i++) {
		int rc;
		size_t used = measure(ops[i], &x, &rc);
		report("kem", name, op_names[i], used, baseline, rc == 0);
		failed |= rc != 0;
	}
	free(x.pk);
	free(x.sk);
	free(x.ct);
	free(x.ss_e);
	free(x.ss_d);
	OQS_KEM_free(x.kem);
	return failed;
}

/* ---------------------------------------------------------------- SIG */

struct sig_ctx {
	OQS_SIG *sig;
	uint8_t *pk, *sk, *sm, *msg;
	size_t sm_len, msg_len;
};

static int sig_keypair(void *c) {
	struct sig_ctx *x = c;
	return OQS_SIG_keypair(x->sig, x->pk, x->sk) == OQS_SUCCESS ? 0 : 1;
}

static int sig_sign(void *c) {
	struct sig_ctx *x = c;
	return OQS_SIG_sign(x->sig, x->sm, &x->sm_len, x->msg, x->msg_len, x->sk) == OQS_SUCCESS ? 0 : 1;
}

static int sig_verify(void *c) {
	struct sig_ctx *x = c;
	return OQS_SIG_verify(x->sig, x->msg, x->msg_len, x->sm, x->sm_len, x->pk) == OQS_SUCCESS ? 0 : 1;
}

static int run_sig(const char *name, size_t baseline) {
	const op_fn ops[] = { sig_keypair, sig_sign, sig_verify };
	const char *op_names[] = { "keypair", "sign", "verify" };
	struct sig_ctx x;
	int failed = 0;

	x.sig = OQS_SIG_new(name);
	if (x.sig == NULL) {
		return 1;
	}
	/* external-mu variants sign a 64-byte mu instead of a message */
	x.msg_len = strstr(name, "-extmu") != NULL ? EXTMU_LEN : MSG_LEN;
	x.sm_len = 0;
	x.pk = malloc(x.sig->length_public_key);
	x.sk = malloc(x.sig->length_secret_key);
	x.sm = malloc(x.sig->length_signature);
	x.msg = malloc(x.msg_len);
	if (!x.pk || !x.sk || !x.sm || !x.msg) {
		fprintf(stderr, "%s: out of memory\n", name);
		return 1;
	}
	OQS_randombytes(x.msg, x.msg_len);
	for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); i++) {
		int rc;
		size_t used = measure(ops[i], &x, &rc);
		report("sig", name, op_names[i], used, baseline, rc == 0);
		failed |= rc != 0;
	}
	free(x.pk);
	free(x.sk);
	free(x.sm);
	free(x.msg);
	OQS_SIG_free(x.sig);
	return failed;
}

/* ---------------------------------------------------------------- driver */

static void child(const char *kind, const char *name) {
	size_t baseline;
	int rc;
	uint8_t *m;

	OQS_init();
	m = mmap(NULL, stack_size + GUARD_SIZE, PROT_READ | PROT_WRITE,
	         MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
	if (m == MAP_FAILED) {
		perror("mmap");
		_exit(2);
	}
	/* stacks grow downwards: an overflow runs into this guard and faults */
	if (mprotect(m, GUARD_SIZE, PROT_NONE) != 0) {
		perror("mprotect");
		_exit(2);
	}
	stack_base = m + GUARD_SIZE;
	baseline = measure(noop, NULL, &rc);
	if (rc != 0) {
		fprintf(stderr, "%s: cannot create thread\n", name);
		_exit(2);
	}
	rc = strcmp(kind, "kem") == 0 ? run_kem(name, baseline) : run_sig(name, baseline);
	fflush(stdout);
	OQS_destroy();
	_exit(rc);
}

/* returns 1 if the child did not complete cleanly */
static int spawn(const char *kind, const char *name) {
	int status;
	pid_t pid;

	fflush(stdout);
	pid = fork();
	if (pid < 0) {
		perror("fork");
		return 1;
	}
	if (pid == 0) {
		child(kind, name);
	}
	while (waitpid(pid, &status, 0) < 0) {
		if (errno != EINTR) {
			perror("waitpid");
			return 1;
		}
	}
	if (WIFSIGNALED(status)) {
		printf("%s\t%s\t-\t0\tcrash:%d\n", kind, name, WTERMSIG(status));
		return 1;
	}
	return WEXITSTATUS(status) != 0;
}

static void print_metadata(void) {
	static const struct {
		OQS_CPU_EXT ext;
		const char *name;
	} exts[] = {
		{ OQS_CPU_EXT_ADX, "ADX" }, { OQS_CPU_EXT_AES, "AES" }, { OQS_CPU_EXT_AVX, "AVX" },
		{ OQS_CPU_EXT_AVX2, "AVX2" }, { OQS_CPU_EXT_AVX512, "AVX512" }, { OQS_CPU_EXT_BMI1, "BMI1" },
		{ OQS_CPU_EXT_BMI2, "BMI2" }, { OQS_CPU_EXT_PCLMULQDQ, "PCLMULQDQ" },
		{ OQS_CPU_EXT_VPCLMULQDQ, "VPCLMULQDQ" }, { OQS_CPU_EXT_POPCNT, "POPCNT" },
		{ OQS_CPU_EXT_SSE, "SSE" }, { OQS_CPU_EXT_SSE2, "SSE2" }, { OQS_CPU_EXT_SSE3, "SSE3" },
		{ OQS_CPU_EXT_ARM_AES, "ARM_AES" }, { OQS_CPU_EXT_ARM_SHA2, "ARM_SHA2" },
		{ OQS_CPU_EXT_ARM_SHA3, "ARM_SHA3" }, { OQS_CPU_EXT_ARM_NEON, "ARM_NEON" },
	};
	pthread_attr_t attr;
	size_t def = 0;

	printf("# liboqs\t%s\n", OQS_version());
	printf("# cpu_extensions\t");
	for (size_t i = 0; i < sizeof(exts) / sizeof(exts[0]); i++) {
		if (OQS_CPU_has_extension(exts[i].ext)) {
			printf("%s ", exts[i].name);
		}
	}
	printf("\n");
	if (pthread_attr_init(&attr) == 0) {
		pthread_attr_getstacksize(&attr, &def);
		pthread_attr_destroy(&attr);
	}
	printf("# default_thread_stack_bytes\t%zu\n", def);
	printf("# measurement_stack_bytes\t%zu\n", stack_size);
	printf("# kind\talg\top\tpeak_bytes\tstatus\n");
}

static void usage(const char *prog) {
	fprintf(stderr, "Usage: %s [-s <stack MiB>] [<algorithm name substring>]\n", prog);
}

int main(int argc, char **argv) {
	const char *filter = NULL;
	unsigned long mib = DEFAULT_STACK_MIB;
	int failed = 0;
	int opt;

	while ((opt = getopt(argc, argv, "s:h")) != -1) {
		char *end;
		switch (opt) {
		case 's':
			mib = strtoul(optarg, &end, 10);
			if (*end != '\0' || mib == 0 || mib > 4096) {
				usage(argv[0]);
				return EXIT_FAILURE;
			}
			break;
		default:
			usage(argv[0]);
			return opt == 'h' ? EXIT_SUCCESS : EXIT_FAILURE;
		}
	}
	if (optind < argc) {
		filter = argv[optind];
	}
	stack_size = (size_t)mib << 20;

	OQS_init();
	print_metadata();
	OQS_destroy();

	for (int i = 0; i < OQS_KEM_alg_count(); i++) {
		const char *name = OQS_KEM_alg_identifier((size_t)i);
		if (OQS_KEM_alg_is_enabled(name) && (filter == NULL || strstr(name, filter) != NULL)) {
			failed |= spawn("kem", name);
		}
	}
	for (int i = 0; i < OQS_SIG_alg_count(); i++) {
		const char *name = OQS_SIG_alg_identifier((size_t)i);
		if (OQS_SIG_alg_is_enabled(name) && (filter == NULL || strstr(name, filter) != NULL)) {
			failed |= spawn("sig", name);
		}
	}
	return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
