# Instructions For Security Researchers And Automated Agents

This repository contains a C cryptography library. Low-quality reports waste
scarce maintainer time, so do not submit a security finding until you have read
the threat model, built the affected configuration, produced a working PoC or
test, and checked whether the result is in scope.

Start here:

- `.github/THREAT_MODEL.md`
- `SECURITY.md`
- `PLATFORMS.md`
- `ALGORITHMS.md`
- `CONFIGURE.md`
- `CONTRIBUTING.md`
- `CI.md`
- `docs/FUZZING.md`
- Relevant files under `docs/algorithms/`

## Security Handling

Do not publicly disclose plausible vulnerabilities before following the process
in `SECURITY.md`. If a finding could affect confidentiality, integrity,
availability, secret-key material, stateful-signature safety, or constant-time
behavior, treat it as security-sensitive until maintainers say otherwise.

Reports and patches that use generative AI must say so, as required by
`CONTRIBUTING.md` and the issue/PR templates. You are responsible for verifying
AI-generated code, tests, and prose before submitting them.

## Research Priorities

Prefer work that helps maintainers triage real risk:

- Public API memory safety and input validation within documented pointer and
  memory-region preconditions.
- Shared cryptographic primitives and support code under `src/common`.
- Stateful signature safety, especially LMS/HSS and XMSS/XMSSMT persistence,
  locking, exhaustion, deserialization, and one-time-signature reuse.
- Secret-material lifetime and zeroization when residual secret-derived bytes
  remain observable after an intended cleanup boundary.
- Constant-time behavior involving secret data on supported platforms.
- Unit tests, fuzz tests, sanitizer coverage, Valgrind constant-time tests,
  benchmarking methodology, and other support infrastructure.
- Wrapper and integration logic that liboqs maintains directly.

When reviewing imported third-party implementations, identify the upstream source
and explain whether the fix belongs in liboqs, upstream, or both.

## C API Preconditions

Most liboqs APIs cannot verify whether a caller-provided pointer actually refers
to a region as large as the API contract requires. Treat a report as in scope
only if the reproducer satisfies the documented pointer and memory-region
preconditions, unless the API explicitly accepts a shorter or `NULL` buffer.

Once those preconditions hold, bytes inside the region and any explicit length,
count, identifier, or serialized-state field are still untrusted inputs. liboqs
must not read past the supplied length, accept malformed serialized data, or
trust attacker-controlled metadata without validation.

For zeroization claims, trace secret-derived bytes across the intended cleanup
boundary and show an observation or reuse path. A missing cleanse call by itself
is a hardening lead, not a confirmed vulnerability.

## Required Finding Workflow

Use this workflow for every proposed security finding:

1. Read `.github/THREAT_MODEL.md` and write down the boundary crossed.
2. Check existing issues, pull requests, `SECURITY.md`, `tests/constant_time`,
   and relevant `docs/algorithms` files for known behavior.
3. Identify the affected public API, algorithm, parameter set, source location,
   platform tier, and build options.
4. For pointer-plus-length APIs, state whether the PoC satisfies documented
   memory-region preconditions and which bytes or explicit lengths remain
   attacker-controlled.
5. Build the affected configuration from a clean worktree.
6. Create a minimal PoC or regression test that fails on the vulnerable build.
7. Run the PoC with a useful detector when possible: AddressSanitizer,
   UndefinedBehaviorSanitizer, Valgrind, a constant-time checker, or a
   differential oracle.
8. Confirm the impact against the threat model. Do not report out-of-scope
   behavior as a vulnerability.
9. Minimize the reproducer and document exact commands, expected vulnerable
   output, and expected fixed output.
10. If proposing a patch, add a regression test that fails before the fix and
   passes after it.

A report without a build, runnable reproducer, or clear threat-model mapping
should be treated as a draft note, not a confirmed vulnerability.

## Build Prerequisites

Before running builds or tests, verify the local toolchain instead of assuming it
is suitable:

```sh
cmake --version
ninja --version
python3 --version
cc --version
```

Useful optional tools:

```sh
clang --version
valgrind --version
astyle --version
actionlint --version
```

Install dependencies according to `README.md`. On macOS, Valgrind may be
unavailable or unreliable; do not claim a Valgrind constant-time result unless
you actually ran it successfully.

For long-running commands, write full output to a log file and keep only the
tail, failing command, and final summary in the report:

```sh
log="$(mktemp "${TMPDIR:-/tmp}/liboqs-build.XXXXXX.log")"
cmake -S . -B build/agent -GNinja -DCMAKE_BUILD_TYPE=Debug >"$log" 2>&1
rc=$?
tail -200 "$log"
echo "full log: $log"
exit "$rc"
```

## Standard Builds

Default debug build:

```sh
cmake -S . -B build/agent -GNinja -DCMAKE_BUILD_TYPE=Debug
ninja -C build/agent
```

Strict warning build:

```sh
cmake -S . -B build/strict -GNinja -DCMAKE_BUILD_TYPE=Debug -DOQS_STRICT_WARNINGS=ON
ninja -C build/strict
```

Minimal builds are preferred for focused PoCs because they reduce noise and
runtime:

```sh
cmake -S . -B build/minimal -GNinja -DCMAKE_BUILD_TYPE=Debug \
  '-DOQS_MINIMAL_BUILD=KEM_ml_kem_768;SIG_ml_dsa_44'
ninja -C build/minimal
```

When an issue requires non-default options, include the exact flags in the
report. This is especially important for stateful signature key/sign generation
and other hazardous or experimental options.

## Running Tests

Run the main test target after broad changes:

```sh
ninja -C build/agent run_tests
```

Run focused test binaries for algorithm-specific work:

```sh
build/agent/tests/test_kem ML-KEM-768
build/agent/tests/test_sig ML-DSA-44
build/agent/tests/test_sig_stfl LMS_SHA256_H5_W1
```

If a test binary does not accept the example algorithm name in your build, dump
the enabled algorithm metadata and choose the closest affected variant:

```sh
build/agent/tests/dump_alg_info
python3 -m pytest --collect-only tests/test_cmdline.py
```

Do not submit a patch that changes algorithm outputs without addressing KATs and
the PR-template question about cryptographic input/output behavior.

## Sanitizers

Use sanitizers for memory-safety claims when practical.

AddressSanitizer with Clang:

```sh
cmake -S . -B build/asan -GNinja -DCMAKE_C_COMPILER=clang \
  -DCMAKE_BUILD_TYPE=Debug -DUSE_SANITIZER=Address
ninja -C build/asan
```

UndefinedBehaviorSanitizer with Clang:

```sh
cmake -S . -B build/ubsan -GNinja -DCMAKE_C_COMPILER=clang \
  -DCMAKE_BUILD_TYPE=Debug -DUSE_SANITIZER=Undefined
ninja -C build/ubsan
```

For a standalone PoC, link against the same build you tested. Keep the PoC
small, deterministic, and focused on one bug.

## Fuzzing

Build fuzz tests with sanitizer instrumentation:

```sh
export CFLAGS=-fsanitize=fuzzer-no-link,address
export LDFLAGS=-fsanitize=address
cmake -S . -B build/fuzz -GNinja -DCMAKE_BUILD_TYPE=Debug -DOQS_BUILD_FUZZ_TESTS=ON
ninja -C build/fuzz
```

Run fuzzers with a timeout and save any crashing input:

```sh
build/fuzz/tests/fuzz_test_sig -max_total_time=300
build/fuzz/tests/fuzz_test_kem -max_total_time=300
```

When filing a fuzz-derived report, include the minimized input, the exact fuzzer
command, sanitizer output, and a standalone regression test. A fuzzer crash
without a minimized reproducer is usually not ready for maintainer triage.

## Constant-Time Review

The existing constant-time tests use Valgrind to look for suspected
secret-dependent branches and memory accesses. They are not proofs, and they do
not cover every form of non-constant-time behavior.

Build constant-time test support:

```sh
cmake -S . -B build/ct -GNinja -DCMAKE_BUILD_TYPE=Debug \
  -DOQS_DIST_BUILD=OFF -DOQS_OPT_TARGET=generic -DOQS_ENABLE_TEST_CONSTANT_TIME=ON
ninja -C build/ct
```

Then run the relevant pytest target from that build context. If you add or
update suppressions, explain whether they are documented false positives
(`passes`) or accepted issues (`issues`) and update the JSON mapping files under
`tests/constant_time`.

Example focused run:

```sh
OQS_BUILD_DIR=build/ct python3 -m pytest tests/test_constant_time.py -k test_constant_time_kem
```

For binary-level timing claims, provide the target triple, compiler, optimization
level, disassembly evidence, and a path from secret data to the instruction or
memory access. Do not report public-data branches as constant-time
vulnerabilities.

Do not report these known rejected CT patterns without new evidence:

- NTRUPrime `Decode` division over public radix values and attacker-controlled
  ciphertext-derived numerators.
- MAYO `sample_solution` division over public dimensions and loop indices.
- MQOM PRG division over fixed protocol sizes or public output lengths.
- MQOM `RecomputePAlpha_default` apparent `idiv` decoded from Mach-O jump-table
  bytes or any other `data-in-code` range.
- XMSS/XMSSMT parameter initialization division over public OID-selected
  parameters.
- LMS/HSS allocation, aux sizing, and work-scheduling division over public
  parameters, memory limits, aux lengths, or thread counts.
- LM-OTS coefficient/checksum division over public widths and transcript-visible
  chain schedules.
- SNOVA public-key expansion table lookups and explicitly declassified signing
  retry timing already covered by suppressions.
- XMSS WOTS and LMS LM-OTS chain loops where the chain length is visible from the
  signature transcript.

For every CT candidate, state which operands are secret, which are public, and
which are transcript-visible. If the only signal is an instruction mnemonic such
as `DIV`, `IDIV`, `UDIV`, `SDIV`, `FDIV`, or a ctgrind branch without a
secret-data path, keep it as a rejected hypothesis.

## Benchmarking And Performance Claims

Use the existing benchmark programs and CI benchmarking workflow as the baseline.
Performance reports should include:

- CPU model and architecture.
- Compiler and flags.
- OpenSSL and liboqs build options.
- Algorithm and parameter set.
- Multiple runs or a reason a single run is sufficient.
- Whether the change affects generic code, optimized code, or dispatch logic.

Do not report small one-off timing differences as regressions without controlling
for CPU frequency, thermal state, background load, and build configuration.

## Patch Expectations

Keep changes narrow and reviewable.

- Prefer wrapper-level validation when it fixes every implementation of an API
  contract consistently.
- Avoid algorithm-specific special cases in shared logic unless the algorithm
  genuinely requires them.
- Add tests for new features and bug fixes.
- Run `LIBOQS_DIR=<liboqs directory> ./scripts/format_code.sh` before submitting
  C formatting changes.
- Run `actionlint .github/workflows/*.yml` for workflow changes.
- Update documentation when behavior, build options, algorithm lists, or
  supported platforms change.

If a patch changes available algorithms, public APIs, or KAT outputs, call that
out explicitly in the PR description and update the downstream/benchmarking
metadata described in the PR template.

## Report Template For Agents

Use this structure for security findings:

```md
# Title

## Summary
One or two paragraphs describing the bug and why it matters.

## Threat Model Fit
- Boundary crossed:
- Attacker capability:
- Caller memory preconditions:
- In-scope rationale:
- Out-of-scope considerations:

## Affected Target
- Repository and commit:
- Platform and compiler:
- Build options:
- Public API:
- Algorithm and parameter set:
- Source location:

## Impact
State the concrete impact without exaggeration.

For secret-lifetime findings, state the secret source, intended cleanup
boundary, residual bytes, and observation or reuse path.

## Reproduction
Commands from a clean checkout.

## Expected Output
Vulnerable output and fixed output.

## PoC Or Regression Test
Minimal source or path to test file.

## Recommended Fix
Patch direction and regression coverage.
```

If you cannot fill in these fields, keep investigating before submitting.
