# Stack usage

Some implementations need much more stack than others. Applications that run liboqs operations in threads, or on platforms with small default stacks, can crash if a thread's stack is too small. The `large-stack-usage` field in each implementation's entry in [`docs/algorithms/**/*.yml`](algorithms) warns about this, and it appears in the "Large stack usage?" column of the generated algorithm documentation.

This document describes how to **measure** stack usage, so that the flag is based on data rather than on whether something happened to crash.

## When to measure

Stack usage only changes when implementation code changes. Measure when:

- adding a new algorithm or implementation,
- updating an upstream (for example via [`copy_from_upstream`](../scripts/copy_from_upstream/README.md)),
- changing code in a way that moves large buffers between the stack and the heap.

Regular CI doesn't measure stack usage. It only runs a smoke test of the tooling (`tests/test_stack_usage.py`).

## Threshold

> **Note:** the threshold is under discussion in [#2723](https://github.com/open-quantum-safe/liboqs/issues/2723). Until that is settled, the tooling uses **512 KiB** as its default. That's the default secondary-thread stack size on macOS.

An implementation counts as having large stack usage if its peak stack usage in any single API call (`keypair`, `encaps`/`sign`, `decaps`/`verify`) exceeds the threshold. Common default thread-stack sizes for comparison:

| Platform | Default thread stack |
|---|---:|
| Linux / glibc | 8 MiB (follows `ulimit -s`) |
| Windows | 1 MiB |
| macOS (secondary threads) | 512 KiB |
| Linux / musl (e.g. Alpine) | 128 KiB |

liboqs's own test programs are linked with a 16 MiB stack on musl and MinGW (see `.CMake/compiler_opts.cmake`). Applications that use liboqs don't inherit that setting, so they get the defaults above.

## Quick start

From the repository root (requires CMake, a C compiler and Python 3 with PyYAML):

```sh
# default flavour (dist build, i.e. the code paths selected at runtime on this CPU)
python3 scripts/measure_stack_usage.py

# all flavours, only one family, only rows that disagree with the YAML
python3 scripts/measure_stack_usage.py --flavour dist --flavour portable --flavour memopt \
    --filter SNOVA --mismatches-only
```

The script configures and builds liboqs (only the `stack_usage` target) under `build-stack-usage/<flavour>`, runs the measurement, and prints a Markdown table:

```
| Flavour | Algorithm | Implementation | Peak KiB | Peak op | YAML flag | Implied | Verdict |
|---|---|---|---:|---|---|---|---|
| dist | SNOVA_V_S | avx2 | 1057 | keypair | true | true | ok |
| memopt | SNOVA_V_S | memopt | 77 | sign | false | false | ok |
| dist | mqom3_cat3_gf2_shorter_ot | avx2 | 48 | verify | true | false | MISMATCH |
```

- **Implementation** is the YAML `upstream-id` the figure most likely belongs to. The script infers it from the build flavour, the host architecture and the CPU extensions the library detected at runtime. If several implementations could apply, it lists them separated by `|`.
- **Implied** is `true` if the peak exceeds the threshold.
- **Verdict** is `MISMATCH` if the implied flag differs from the YAML `large-stack-usage` value of that implementation, and `unknown` if the implementation couldn't be identified or its candidates are flagged differently.

A full run over all algorithms takes a few minutes per flavour on a current x86-64 machine.

### Options

| Option | Meaning |
|---|---|
| `--flavour {dist,portable,memopt}` | Build configuration(s) to measure; repeatable. Default `dist`. |
| `--build-dir DIR` | Measure an existing build instead (must contain `tests/stack_usage`); its flavour is read from `CMakeCache.txt`. Repeatable. |
| `--filter STR` | Only algorithms whose name contains `STR`. |
| `--threshold-kib N` | Threshold for the implied flag (default 512). |
| `--cmake-arg ARG` | Extra CMake argument for builds; repeatable. Replaces the default `-DOQS_ENABLE_KEM_CLASSIC_MCELIECE=ON`. |
| `--tsv FILE` | Also write all results as TSV. |
| `--mismatches-only` | Only list rows whose verdict isn't `ok`. |
| `--check` | Exit with a non-zero status if any `MISMATCH` is found. |

### Flavours

Peak stack usage can differ a lot between implementations of the same parameter set (5–15× has been observed, in both directions). Measure every flavour that builds the implementations whose flag you set:

| Flavour | CMake configuration | Measures |
|---|---|---|
| `dist` | `OQS_DIST_BUILD=ON` | the implementation runtime dispatch selects on this CPU (e.g. `avx2` on a recent x86-64, `neon`/`aarch64` on ARM64) |
| `portable` | `OQS_DIST_BUILD=OFF`, `OQS_OPT_TARGET=generic` | the portable C implementation |
| `memopt` | `OQS_MEMOPT_BUILD=ON` | `memopt` implementations where they exist |

To measure ARM implementations, run the script on an ARM64 host.

## Measuring a single build directly

The measurement program is built as the `stack_usage` target of the test suite (Unix-like systems with pthreads only):

```sh
cmake -S . -B build -GNinja && ninja -C build stack_usage
./build/tests/stack_usage ML-DSA            # all algorithms containing "ML-DSA"
./build/tests/stack_usage -s 16 HQC         # 16 MiB measurement stack
```

The output is tab-separated: `kind alg op peak_bytes status`, preceded by `#` metadata lines (liboqs version, detected CPU extensions, the platform's default thread-stack size). `status` is `ok`, `error` (the API call failed or produced a wrong result) or `crash:<signal>` (for example a stack overflow beyond the measurement stack).

## How it works

`tests/stack_usage.c` uses *stack painting*:

1. For each algorithm, a child process is forked, so a crash only affects that algorithm.
2. The child `mmap`s a measurement stack (default 64 MiB, `-s`) with an inaccessible guard region below it. An overflow faults instead of corrupting memory.
3. For each operation, the stack is filled with a known 64-bit pattern, the operation runs in a thread on that stack (`pthread_attr_setstack`), and after `pthread_join` the stack is scanned from its far end for the first overwritten word. The distance from there to the top of the stack is the peak usage.
4. The usage of an empty thread (thread control block and TLS that libc places on the stack, plus thread start-up frames) is subtracted.

Keys, ciphertexts, signatures and messages are allocated on the heap, so they don't count towards the figures.

### Limitations

- Figures depend on compiler, optimisation level and code path. Always state the platform and flavour when quoting numbers.
- Each operation runs once. Operations with data-dependent control flow (e.g. rejection sampling) may occasionally use slightly more stack.
- Only stack that is actually *written* is counted. That's what matters for stack overflows, but it isn't necessarily the same as the frame sizes the compiler reports (`-fstack-usage`).
- Under AddressSanitizer and similar tools, the figures include instrumentation overhead and aren't representative.
- Heap usage isn't measured (see `test_kem_mem`/`test_sig_mem` for that).
