# Constant-Time Tooling

Framework constant-time testing of liboqs across compilers, optimization flags, and `OQS_OPT_TARGET` build modes.

## Repository Structure

```
tests/ct_tooling/
├── README.md
├── ct_test.sh                  # Unified shell script handling CT test execution for all tools
├── local_testing_example.sh    # Example script for running CT tests locally
├── analyze_results.py          # Parse log output and generate summary CSVs and graphs
└── tools/
    ├── memsan/
    │   ├── CMakeLists.txt          # MemSan-specific CMake configuration for liboqs/tests
    │   ├── rng_poison_memsan.c     # RNG poisoning for MemSan testing
    │   ├── test_kem.c              # MemSan-specific KEM test
    │   ├── test_sig.c              # MemSan-specific SIG test
    │   └── README.md
    └── valgrind_varlat/
        ├── false_positives/        # Directory containing false-positives suppression files
        │   └── *.supp
        ├── valgrind-try-patch-20250805.txt     # Valgrind patch file
        ├── valgrind-varlat-patch-20250805.txt  # Valgrind variable-latency patch
        ├── valgrind-varlat-sup-block.txt       # Valgrind suppression block
        └── README.md
```

## Tools

### 1. Valgrind-Varlat (`valgrind_varlat/`)
- **Purpose**: Uninitialized memory error detection for constant-time analysis using Kyberslash patch for Valgrind
- **Output**: Directory containing all unique suppression blocks for each warning output

### 2. MemSan (`memsan/`)
- **Purpose**: LLVM-based uninitialized memory error detection for constant-time analysis using MemorySanitizer
- **Output**: Unique `SUMMARY: MemorySanitizer` lines for each warning output

Both tools are driven by the single unified `ct_test.sh` script located at the root of `tests/ct_tooling/`. The tool to execute is selected via the first argument:

```bash
./ct_test.sh <tool> <compiler_version> <liboqs_build> <opt_flags...> <input>
```

- `tool`: `valgrind-varlat` or `memsan`
- `compiler_version`: clang, clang-20, gcc, gcc-14, ...
- `liboqs_build`: `generic` or `auto`
- `input`: `all`, `kems`, `sigs`, or a specific enabled algorithm variant
- `opt_flags`: All arguments from position 4 up to position N-1 are treated as compiler optimization flags (including multi-flag combinations such as `-O3 -fno-tree-vectorize`).

Examples:

```bash
./ct_test.sh valgrind-varlat clang generic -O2 all
./ct_test.sh valgrind-varlat clang-20 auto -O3 -fno-tree-vectorize kems
./ct_test.sh valgrind-varlat gcc-14 generic -O2 -fno-tree-vectorize Kyber768
./ct_test.sh memsan clang generic -O1 ML-DSA-44
./ct_test.sh memsan clang-20 auto -O3 all
```

The `local_testing_example.sh` script demonstrates how to use `ct_test.sh` to run CT tests locally across a variety of compilers, compiler versions, liboqs target builds, and optimization flags.

The `ct-tooling-valgrind-varlat.yml` and `ct-tooling-memsan.yml` workflows also use `ct_test.sh` to execute CT tests in CI on user-selected algorithms (using [interactive-inputs](https://github.com/marketplace/actions/interactive-inputs)) when the workflows are manually triggered on GitHub Actions.

### Configuration
For Valgrind-Varlat configuration, see: [Valgrind-Varlat's README](tools/valgrind_varlat/README.md)
For MemSan configuration, see: [MemSan's README](tools/memsan/README.md)

## Workflow

### Running Tests

Each tool follows the same testing workflow, implemented through two functions in the unified `ct_test.sh` script:

1. **Building liboqs**
First, the `build()` function builds liboqs with the compiler options desired. In the case of `local_testing_example.sh`, these options are set to:

- Compiler versions: gcc, gcc-14, clang, and clang-20
- liboqs build: auto (with optimizations), and generic
- Optimization flags: -O0, -O1, -O2, -O3, -Os, -Ofast, -O2 -fno-tree-vectorize, and -O3 -fno-tree-vectorize

When `ct_test.sh` is executed on a single algorithm variant, `build()` builds liboqs with the `OQS_MINIMAL_BUILD` option, minimizing the time and resources required for building. For the other input options, liboqs is built entirely. Each build is placed into a dedicated directory at the liboqs root, named using the pattern.

Note that Valgrind-Varlat tests can be compiled using both gcc and clang, while MemSan is only native to the clang compiler. This leaves the following possible configurations for each of the optimization flags in the case of `local_testing_example.sh`:

<table>
  <thead>
    <tr>
      <th></th>
      <th colspan="4">Valgrind-Varlat</th>
      <th colspan="2">MemSan</th>
    </tr>
    <tr>
      <th></th>
      <th>gcc</th>
      <th>gcc-14</th>
      <th>clang</th>
      <th>clang-20</th>
      <th>clang</th>
      <th>clang-20</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>generic</td>
      <td>All opt flags</td>
      <td>All opt flags</td>
      <td>All opt flags</td>
      <td>All opt flags</td>
      <td>All opt flags</td>
      <td>All opt flags</td>
    </tr>
    <tr>
      <td>auto</td>
      <td>All opt flags</td>
      <td>All opt flags</td>
      <td>All opt flags</td>
      <td>All opt flags</td>
      <td>All opt flags</td>
      <td>All opt flags</td>
    </tr>
  </tbody>
</table>

Once the script builds each configuration into a build folder, it calls the test execution function (`test()`) on the build folder generated.

2. **Test execution**
Then, `test()` is tasked with executing the tool's test on selected liboqs algorithms. Each tool has a different process through which it parses the tool's output to keep unique instances of the warnings, which are further detailed in their respective README files: [Valgrind-Varlat's README](tools/valgrind_varlat/README.md) and [MemSan's README](tools/memsan/README.md).

Both tools enforce a warning cap of 100,000 unique warnings per algorithm run. Once the cap is reached, further warnings are suppressed. All SPHINCS and SLH-DSA signature variants are currently skipped during SIG tests due to the excessive time they require to execute.

### Output Structure
The workflow organizes test outputs into log files that capture unique warnings for each algorithm. These logs are written inside `tests/ct_tooling/tools/<tool>/logs/`, categorized into concrete subdirectories based on the compiler and build configuration (`gcc_14_auto`, `clang_generic`, ...), which then contain further subdivisions by optimization levels (`O0`, `O1`, ...) and algorithm types (`kem` or `sig`). The structure is as follows:

```
tests/ct_tooling/tools/<tool>/logs/
├── clang_generic/
│   ├── O0/
│   │   ├── kem/
│   │   │   ├── kem_summary.txt              # Pass/fail summary with compiler info
│   │   │   ├── <algorithm>_<timestamp>.log  # Unique warnings for the algorithm
│   │   └── sig/
│   │       └── ...
│   ├── O1/
│   └── ...
├── <compiler>_<build_config>/
│   ├── O0/
│   └── ...
```

The summary file for each run includes the compiler path, compiler version, target architecture, and compilation flags used, followed by a pass/fail line for each algorithm tested.

### Simultaneous testing
Since MemSan requires to replace several files within liboqs/tests, it is not recommended to run both tests at the same time. This would cause Valgrind_Varlat tests to fail because of using MemSan-oriented files.

### Analyzing Results

Use `analyze_results.py` to parse the warnings data from the log files and generate graphs describing the results.

```bash
python3 analyze_results.py \
    --tool <Valgrind-Varlat|MemSan> \
    --input tests/ct_tooling/tools/<tool>/logs \
    --output results_<tool>
```

**Generates**:
- `KEM_warnings_per_opt_level.csv` - Warning counts per algorithm per optimization
- `SIG_warnings_per_opt_level.csv` - Same for signature schemes
- `*.png` - 4 visualization graphs regarding the total and average number of warnings per KEM and signature