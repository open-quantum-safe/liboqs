# Public API Mutation Testing

This campaign measures whether existing correctness tests distinguish mutations
in liboqs-owned public API dispatch code:

- `src/kem/kem.c`
- `src/sig/sig.c`
- `src/sig_stfl/sig_stfl.c`

The scope is intentionally narrower than a full imported-implementation
campaign. Trailmark preanalysis places these files on public API paths, and
Genotoxic/Mull gives a repeatable signal for contract coverage without making
routine CI depend on every upstream implementation family.

## Local Run

Mull 0.34.0 with LLVM 18 is required. Build with the Mull frontend loaded, then
run the representative campaign:

```bash
export MULL_FRONTEND=/path/to/mull-ir-frontend-18
export MULL_RUNNER=/path/to/mull-runner-18

tests/mutation/run_public_input.sh prepare build/mutation
export MULL_CONFIG="$PWD/build/mutation/mull-build.yml"

cmake -S . -B build/mutation -GNinja \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_C_COMPILER=clang-18 \
  -DCMAKE_ASM_COMPILER=clang-18 \
  "-DCMAKE_C_FLAGS=-fpass-plugin=${MULL_FRONTEND} -g -O0 -grecord-command-line" \
  -DOQS_HAZARDOUS_EXPERIMENTAL_ENABLE_SIG_STFL_KEY_SIG_GEN=ON \
  -DOQS_ENABLE_SIG_STFL_XMSS=ON \
  -DOQS_ENABLE_SIG_STFL_LMS=ON \
  '-DOQS_MINIMAL_BUILD=KEM_bike_l1;KEM_hqc_1;KEM_ntruprime_sntrup761;KEM_ntru_hps2048509;SIG_mayo_1;SIG_snova_SNOVA_25_8_3;SIG_cross_rsdp_128_small;SIG_falcon_512;SIG_falcon_padded_512;SIG_STFL_xmss_sha256_h10;SIG_STFL_lms_sha256_h5_w1;SIG_STFL_lms_sha256_h5_w8'
cmake --build build/mutation --target test_kem test_sig test_sig_stfl

tests/mutation/run_public_input.sh run build/mutation build/mutation/mull-reports
python3 tests/mutation/summarize.py \
  --reports build/mutation/mull-reports \
  --baseline tests/mutation/baseline.json \
  --json-out build/mutation/mull-reports/summary.json \
  --markdown-out build/mutation/mull-reports/summary.md
```

The GitHub workflow is dispatchable, runs weekly via `weekly.yml`, and runs on
published releases. It uploads the raw Elements reports and the aggregate
summary. Baseline enforcement is opt-in so scheduled measurement does not block
unrelated maintenance while the survivor backlog is being reduced.
