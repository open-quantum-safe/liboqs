# Fuzzing

liboqs fuzz targets focus on public inputs that can reach scheme-specific
decoders or deserializers before authentication succeeds. The targets start
from a valid cached baseline for each enabled algorithm and then mutate the
attacker-controlled fields, which keeps the fuzzer in deep parsing and
verification paths instead of spending most iterations on early rejects.

## Targets

| Target | Public API surface | Mutated attacker input |
| --- | --- | --- |
| `fuzz_test_kem_encaps` | `OQS_KEM_encaps`, `OQS_KEM_encaps_derand` | KEM public keys |
| `fuzz_test_kem_decaps` | `OQS_KEM_decaps` | KEM ciphertexts |
| `fuzz_test_sig_verify` | `OQS_SIG_verify`, `OQS_SIG_verify_with_ctx_str` | Public keys, signatures, messages, contexts |
| `fuzz_test_sig_stfl_verify` | `OQS_SIG_STFL_verify` | Stateful public keys, signatures, messages |
| `fuzz_test_sig_stfl_deserialize` | `OQS_SIG_STFL_SECRET_KEY_deserialize` and round-trip serialization | Serialized stateful secret keys |

`fuzz_test_sig_stfl_deserialize` is only built when
`OQS_HAZARDOUS_EXPERIMENTAL_ENABLE_SIG_STFL_KEY_SIG_GEN=ON`, because the
target relies on the full stateful object layout and persistence callback path
available in key/sign builds.

## Building

Use Clang and sanitizer instrumentation for local runs:

```bash
export CC=clang
export CXX=clang++
export CFLAGS=-fsanitize=fuzzer-no-link,address
export LDFLAGS=-fsanitize=address

cmake -S . -B build/fuzz -GNinja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DOQS_BUILD_FUZZ_TESTS=ON \
  -DOQS_HAZARDOUS_EXPERIMENTAL_ENABLE_SIG_STFL_KEY_SIG_GEN=ON \
  -DOQS_ENABLE_SIG_STFL_XMSS=ON \
  -DOQS_ENABLE_SIG_STFL_LMS=ON
ninja -C build/fuzz \
  fuzz_test_kem_encaps \
  fuzz_test_kem_decaps \
  fuzz_test_sig_verify \
  fuzz_test_sig_stfl_verify \
  fuzz_test_sig_stfl_deserialize
```

For focused local work, prefer a minimal build:

```bash
cmake -S . -B build/fuzz-minimal -GNinja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DOQS_BUILD_FUZZ_TESTS=ON \
  -DOQS_HAZARDOUS_EXPERIMENTAL_ENABLE_SIG_STFL_KEY_SIG_GEN=ON \
  -DOQS_ENABLE_SIG_STFL_XMSS=ON \
  -DOQS_ENABLE_SIG_STFL_LMS=ON \
  '-DOQS_MINIMAL_BUILD=KEM_ml_kem_768;KEM_bike_l1;SIG_ml_dsa_44;SIG_falcon_512;SIG_STFL_xmss_shake128_h10;SIG_STFL_lms_sha256_h5_w2'
```

## Corpus

The committed seed corpus is generated from the algorithm registries in
`src/kem/kem.c`, `src/sig/sig.c`, and `src/sig_stfl/sig_stfl.c`. Each seed is a
small binary header selecting one algorithm and API mode; the target then
reconstructs a valid baseline in memory.

Regenerate or validate the corpus after algorithm registry changes:

```bash
python3 tests/fuzz/generate_corpus.py --write
python3 tests/fuzz/generate_corpus.py --check
```

## Running

Pass the matching corpus directory to libFuzzer and use an artifact prefix so a
crashing input is preserved:

```bash
mkdir -p fuzz-artifacts
build/fuzz/tests/fuzz_test_sig_verify \
  tests/fuzz/corpus/fuzz_test_sig_verify \
  -max_total_time=300 \
  -timeout=10 \
  -max_len=65536 \
  -keep_seed=1 \
  -verbosity=0 \
  -print_final_stats=1 \
  -artifact_prefix=fuzz-artifacts/fuzz_test_sig_verify-
```

The PR workflow runs a short sanitizer smoke profile over representative KEM,
signature, XMSS, and LMS algorithms. The weekly workflow enables all default
algorithms plus both stateful families and runs every target for longer.
