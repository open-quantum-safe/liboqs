Options for configuring liboqs builds
=====================================

The following options can be passed to CMake before the build file generation process to customize the way liboqs is built. The syntax for doing so is: `cmake .. [ARGS] [-D<OPTION_NAME>=<OPTION_VALUE>]...`, where `<OPTON_NAME>` is:

- [BUILD_SHARED_LIBS](#BUILD_SHARED_LIBS)
- [CMAKE_BUILD_TYPE](#CMAKE_BUILD_TYPE)
- [CMAKE_INSTALL_PREFIX](#CMAKE_INSTALL_PREFIX)
- [OQS_ALGS_ENABLED](#OQS_ALGS_ENABLED)
- [OQS_BUILD_ONLY_LIB](#OQS_BUILD_ONLY_LIB)
- [OQS_ENABLE_KEM_ALG/OQS_ENABLE_SIG_ALG/OQS_ENABLE_SIG_STFL_ALG](#OQS_ENABLE_KEM_ALG/OQS_ENABLE_SIG_ALG/OQS_ENABLE_SIG_STFL_ALG)
- [OQS_MINIMAL_BUILD](#OQS_MINIMAL_BUILD)
- [OQS_DIST_BUILD](#OQS_DIST_BUILD)
- [OQS_USE_CPUFEATURE_INSTRUCTIONS](#OQS_USE_CPUFEATURE_INSTRUCTIONS)
- [OQS_USE_OPENSSL](#OQS_USE_OPENSSL)
- [OQS_USE_CUPQC](#OQS_USE_CUPQC)
- [OQS_USE_ICICLE](#OQS_USE_ICICLE)
- [OQS_OPT_TARGET](#OQS_OPT_TARGET)
- [OQS_SPEED_USE_ARM_PMU](#OQS_SPEED_USE_ARM_PMU)
- [USE_COVERAGE](#USE_COVERAGE)
- [USE_SANITIZER](#USE_SANITIZER)
- [OQS_ENABLE_TEST_CONSTANT_TIME](#OQS_ENABLE_TEST_CONSTANT_TIME)
- [OQS_STRICT_WARNINGS](#OQS_STRICT_WARNINGS)
- [OQS_EMBEDDED_BUILD](#OQS_EMBEDDED_BUILD)
- [OQS_LIBJADE_BUILD](#OQS_LIBJADE_BUILD)
- [OQS_ENABLE_LIBJADE_KEM_ALG/OQS_ENABLE_LIBJADE_SIG_ALG](#OQS_ENABLE_LIBJADE_KEM_ALG/OQS_ENABLE_LIBJADE_SIG_ALG)
- [OQS_BUILD_FUZZ_TESTS](#OQS_BUILD_FUZZ_TESTS)

## BUILD_SHARED_LIBS

Can be set to `ON` or `OFF`. When `ON`, liboqs is built as a shared library.

**Default**: `OFF`.

This means liboqs is built as a static library by default.

## CMAKE_BUILD_TYPE

Can be set to the following values:

- `Debug`: This turns off all compiler optimizations and produces debugging information. **This value only has effect when the compiler is GCC or Clang**
  - The [USE_COVERAGE](#USE_COVERAGE) option can also be specified to enable code coverage testing.
  - When the compiler is Clang, the [USE_SANITIZER](#USE_SANITIZER) option can also be specified to enable a Clang sanitizer.

- `Release`: This compiles code at the `O3` optimization level, and sets other compiler flags that reduce the size of the binary.

**Default**: `Release`.

## CMAKE_INSTALL_PREFIX

See the [CMake documentation](https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html).

## OQS_ENABLE_KEM_ALG/OQS_ENABLE_SIG_ALG/OQS_ENABLE_SIG_STFL_ALG

Note: `ALG` in `OQS_ENABLE_KEM_ALG/OQS_ENABLE_SIG_ALG/OQS_ENABLE_SIG_STFL_ALG` should be replaced with the specific algorithm name as demonstrated below.

This can be set to `ON` or `OFF`, and is `ON` by default. When `OFF`, `ALG` and its code are excluded from the build process. When `ON`, made available are additional options whereby individual variants of `ALG` can be excluded from the build process. 

For example: if `OQS_ENABLE_KEM_BIKE` is set to `ON`, the options `OQS_ENABLE_KEM_bike_l1`, `OQS_ENABLE_KEM_bike_l3`, and `OQS_ENABLE_KEM_bike_l5` are made available (and are set to be `ON` by default).

To enable `XMSS` stateful signature, set `OQS_ENABLE_SIG_STFL_XMSS` to `ON`, the options `OQS_ENABLE_SIG_STFL_xmss_sha256_h10` and its variants are also set to be `ON` by default. Similarly, `LMS` stateful signature family can also be enabled by setting `OQS_ENABLE_SIG_STFL_LMS` to `ON`.

For a full list of such options and their default values, consult [.CMake/alg_support.cmake](https://github.com/open-quantum-safe/liboqs/blob/master/.CMake/alg_support.cmake).

**Default**: Unset.

## OQS_ALGS_ENABLED

A selected algorithm set is enabled. Possible values are "STD" selecting all algorithms standardized by NIST; "NIST_R4" selecting all algorithms evaluated in round 4 of the NIST PQC competition; "NIST_SIG_ONRAMP" selecting algorithms evaluated in the NIST PQC "onramp" standardization for additional signature schemes; "All" (or any other value) selecting all algorithms integrated into liboqs. Parameter setting "STD" minimizes library size but may require re-running code generator scripts in projects integrating `liboqs`; e.g., [oqs-provider](https://github.com/open-quantum-safe/oqs-provider) and [oqs-boringssl](https://github.com/open-quantum-safe/boringssl).

**Attention**: If you use any predefined value (`STD` or `NIST_R4` or `NIST_SIG_ONRAMP` as of now) for this variable, the values added via [OQS_ENABLE_KEM_ALG/OQS_ENABLE_SIG_ALG/OQS_ENABLE_SIG_STFL_ALG](#OQS_ENABLE_KEM_ALG/OQS_ENABLE_SIG_ALG/OQS_ENABLE_SIG_STFL_ALG) variables will be ignored.

**Default**: `All`.

## OQS_BUILD_ONLY_LIB

Can be `ON` or `OFF`. When `ON`, only liboqs is built, and all the targets: `run_tests`, `gen_docs`, and `prettyprint` are excluded from the build system.

**Default**: `OFF`.

## OQS_MINIMAL_BUILD

If set, this defines a semicolon-delimited list of algorithms to be contained in a minimal build of `liboqs`: Only algorithms explicitly set here are included in a build: For example running `cmake -DOQS_MINIMAL_BUILD="KEM_ml_kem_768;SIG_ml_dsa_44" ..` will build a minimum-size `liboqs` library only containing support for ML-KEM-768 and ML-DSA-44.

The full list of identifiers that can be set is listed [here for KEM algorithms](https://github.com/open-quantum-safe/liboqs/blob/main/src/kem/kem.h#L34) and [here for Signature algorithms](https://github.com/open-quantum-safe/liboqs/blob/main/src/sig/sig.h#L34). The default setting is empty, thus including all [supported algorithms](https://github.com/open-quantum-safe/liboqs#supported-algorithms) in the build.

**Default**: Unset.

## OQS_DIST_BUILD

Can be `ON` or `OFF`. When `ON`, build liboqs for distribution. When `OFF`, build liboqs for use on a single machine.

The library is always built for a particular architecture, either x86-64, ARM32v7, or ARM64v8, depending on the setting of CMAKE_SYSTEM_PROCESSOR. But liboqs contains code that is optimized for micro-architectures as well, e.g. x86-64 with the AVX2 extension.

When built for distribution, the library will run on any CPU of the target architecture. Function calls will be dispatched to micro-architecture optimized routines at run-time using CPU feature detection.

When built for use on a single machine, the library will only include the best available code for the target micro-architecture (see [OQS_OPT_TARGET](#OQS_OPT_TARGET)).

**Default**: `ON`.

## OQS_USE_CPUFEATURE_INSTRUCTIONS

Note: `CPUFEATURE` in `OQS_USE_CPUFEATURE_INSTRUCTIONS` should be replaced with the specific CPU feature as noted below.

These can be set to `ON` or `OFF` and take effect if liboqs is built for use on a single machine. By default, the CPU features are automatically determined and set to `ON` or `OFF` based on the CPU features available on the build system. The default values can be overridden by providing CMake build options. The available options on x86-64 are: `OQS_USE_ADX_INSTRUCTIONS`, `OQS_USE_AES_INSTRUCTIONS`, `OQS_USE_AVX_INSTRUCTIONS`, `OQS_USE_AVX2_INSTRUCTIONS`, `OQS_USE_AVX512_INSTRUCTIONS`, `OQS_USE_BMI1_INSTRUCTIONS`, `OQS_USE_BMI2_INSTRUCTIONS`, `OQS_USE_PCLMULQDQ_INSTRUCTIONS`, `OQS_USE_VPCLMULQDQ_INSTRUCTIONS`, `OQS_USE_POPCNT_INSTRUCTIONS`, `OQS_USE_SSE_INSTRUCTIONS`, `OQS_USE_SSE2_INSTRUCTIONS` and `OQS_USE_SSE3_INSTRUCTIONS`. The available options on ARM64v8 are `OQS_USE_ARM_AES_INSTRUCTIONS`, `OQS_USE_ARM_SHA2_INSTRUCTIONS`, `OQS_USE_ARM_SHA3_INSTRUCTIONS` and `OQS_USE_ARM_NEON_INSTRUCTIONS`.

**Default**: Options valid on the build machine.

## OQS_USE_OPENSSL

To save size and limit the amount of different cryptographic code bases, it is possible to use OpenSSL as a crypto code provider by setting this configuration option.

This can be set to `ON` or `OFF`. When `ON`, the additional options `OQS_USE_AES_OPENSSL`, `OQS_USE_SHA2_OPENSSL`, and `OQS_USE_SHA3_OPENSSL` are made available to control whether liboqs uses OpenSSL's AES, SHA-2, and SHA-3 implementations.

By default,
- `OQS_USE_AES_OPENSSL` is `ON` (on x86-64 only if `OQS_DIST_BUILD` and `OQS_USE_AES_INSTRUCTIONS` are not set)
- `OQS_USE_SHA2_OPENSSL` is `ON`
- `OQS_USE_SHA3_OPENSSL` is `OFF`.

These default choices have been made to optimize the default performance of all algorithms. Changing them implies performance penalties.

When `OQS_USE_OPENSSL` is `ON`, CMake also scans the filesystem to find the minimum version of OpenSSL required by liboqs (which happens to be 1.1.1). The [OPENSSL_ROOT_DIR](https://cmake.org/cmake/help/latest/module/FindOpenSSL.html) option can be set to aid CMake in its search.

**Default**: `ON`.

### OQS_DLOPEN_OPENSSL

Dynamically load OpenSSL through `dlopen`. When using liboqs from other cryptographic libraries, hard dependency on OpenSSL is sometimes undesirable. If this option is `ON`, loading of OpenSSL will be deferred until any of the OpenSSL functions is used.

Only has an effect if the system supports `dlopen` and ELF binary format, such as Linux or BSD family.

### OQS_USE_CUPQC

Can be `ON` or `OFF`.  When `ON`, use NVIDIA's cuPQC library where able (currently just ML-KEM).  When this option is enabled, liboqs may not run correctly on machines that lack supported GPUs. To download cuPQC follow the instructions at (https://developer.nvidia.com/cupqc-download/). Detailed descriptions of the API, requirements, and installation guide are in the cuPQC documentation (https://docs.nvidia.com/cuda/cupqc/index.html). While the code shipped by liboqs required to use cuPQC is licensed under Apache 2.0 the cuPQC SDK comes with its own license agreement (https://docs.nvidia.com/cuda/cupqc/license.html). 

**Default**: `OFF`

### OQS_USE_ICICLE

This CMake option can be set to `ON` or `OFF`. When enabled (`ON`), it configures liboqs to use ICICLE as the backend for supported post-quantum cryptographic (PQC) algorithms — currently ML-KEM.
ICICLE is a GPU-accelerated cryptographic library developed by Ingonyama. It provides CUDA-based implementations of PQC algorithms to boost the performance on systems with compatible NVIDIA GPUs.
To use ICICLE, the user needs to build and install the `icicle_pqc_package`, which contains the necessary CUDA kernels and runtime support. This package must be compiled separately before configuring liboqs with `OQS_USE_ICICLE` enabled, and its installation path should be made available to CMake.

Enabling this option also automatically enables C++ support in CMake, as required by ICICLE’s implementations.

To build ICICLE with the required PQC package:

```bash
cmake -S icicle -B "$BUILD_DIR" \
  -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" \
  -DCPU_BACKEND=OFF \
  -DDISABLE_ALL_FEATURES=ON \
  -DPQC=ON \
  -DCUDA_PQC_BACKEND=ON \
  -DICICLE_STATIC_LINK=ON \
  -DPQC_PACKAGE=ON
cmake --build "$BUILD_DIR"
cmake --install "$BUILD_DIR"
```

For full documentation, setup instructions, and backend support details, see the [Ingonyama documentation](https://dev.ingonyama.com/)

**Default**: `OFF`.

## Stateful Hash Based Signatures 

XMSS and LMS are the two supported Hash-Based Signatures schemes.
`OQS_ENABLE_SIG_STFL_XMSS` and `OQS_ENABLE_SIG_STFL_LMS` control these algorithms, which are disabled by default.
A third variable, `OQS_HAZARDOUS_EXPERIMENTAL_ENABLE_SIG_STFL_KEY_SIG_GEN`, also controls the ability to generate keys and signatures. This is also disabled by default.
Each of these variables can be set to `ON` or `OFF`.
When all three are `ON`, stateful signatures are fully functional and can generate key pairs, sign data, and verify signatures.
If `OQS_HAZARDOUS_EXPERIMENTAL_ENABLE_SIG_STFL_KEY_SIG_GEN` is `OFF` signature verification is the only functional operation.

Standards bodies, such as NIST, recommend that key and signature generation only by done in hardware in order to best enforce the one-time use of secret keys.
Keys stored in a file system are extremely susceptible to simultaneous use.
When enabled in this library a warning message will be generated by the config process.
The name of the configuration variable has been chosen to make every user of this feature aware of its security risks.
The OQS team explicitly discourages enabling this variable and reserves the right to remove this feature in future releases if its use causes actual harm.
It remains present as long as it is responsibly used as per the stated warnings.

By default,
- `OQS_ENABLE_SIG_STFL_XMSS` is `OFF` 
- `OQS_ENABLE_SIG_STFL_LMS` is `OFF`
- `OQS_HAZARDOUS_EXPERIMENTAL_ENABLE_SIG_STFL_KEY_SIG_GEN` is `OFF`.

**Default**: `OFF`.

## OQS_OPT_TARGET

An optimization target. Only has an effect if the compiler is GCC or Clang and `OQS_DIST_BUILD=OFF`. Can take any valid input to the `-march` (on x86-64) or `-mcpu` (on ARM32v7 or ARM64v8) option for `CMAKE_C_COMPILER`. Can also be set to one of the following special values.
  - `auto`: Use `-march=native` or `-mcpu=native` (if the compiler supports it).
  - `generic`: Use `-march=x86-64` on x86-64, or `-mcpu=cortex-a5` on ARM32v7, or `-mcpu=cortex-a53` on ARM64v8.

**Default**: `auto`.

## OQS_SPEED_USE_ARM_PMU

Can be `ON` or `OFF`. When `ON`, the benchmarking script will try to use the ARMv8 Performance Monitoring Unit (PMU). This will make cycle counts on ARMv8 platforms significantly more accurate.

In order to use this option, user mode access to the PMU must be enabled via a kernel module. If user mode access is not enabled via the kernel module, benchmarking will throw an `Illegal Instruction` error. A kernel module that has been found to work on several platforms can be found [here for Linux](https://github.com/mupq/pqax#enable-access-to-performance-counters). Follow the instructions there (i.e., clone the repository, `cd enable_ccr` and `make install`) to load the kernel module, after which benchmarking should work. Superuser permissions are required. Linux header files must also be installed on your platform, which may not be present by default.

Note that this option is not known to work on Apple M1 chips.

**Default**: `OFF`.

## USE_COVERAGE

This has an effect when the compiler is GCC or Clang and when [CMAKE_BUILD_TYPE](#CMAKE_BUILD_TYPE) is `Debug`. Can be `ON` or `OFF`. When `ON`, code coverage testing will be enabled.

**Default**: Unset.

## USE_SANITIZER

This has an effect when the compiler is Clang and when [CMAKE_BUILD_TYPE](#CMAKE_BUILD_TYPE) is `Debug`. Then, it can be set to:

- `Address`: This enables Clang's `AddressSanitizer`
- `Memory`: This enables Clang's `MemorySanitizer`
- `MemoryWithOrigins`: This enables Clang's `MemorySanitizer` with the added functionality of being able to track the origins of uninitialized values
- `Undefined`: This enables Clang's `UndefinedBehaviorSanitizer`. The `BLACKLIST_FILE` option can be additionally set to a path to a file listing the entities Clang should ignore.
- `Thread`: This enables Clang's `ThreadSanitizer`
- `Leak`: This enables Clang's `LeakSanitizer`

**Default**: Unset.

## OQS_ENABLE_TEST_CONSTANT_TIME

This is used in conjunction with `tests/test_constant_time.py` to use Valgrind to look for instances of secret-dependent control flow.  liboqs must also be compiled with [CMAKE_BUILD_TYPE](#CMAKE_BUILD_TYPE) set to `Debug`.  

See the documentation in [`tests/test_constant_time.py`](https://github.com/open-quantum-safe/liboqs/blob/main/tests/test_constant_time.py) for more usage information.

When this option is set to `ON`, the additional option `OQS_ENABLE_TEST_CONSTANT_TIME_OPTIMIZED` is made available to control whether liboqs is built using `-O3` optimization, as in a release build, or using the default "Debug" profile. By default, `OQS_ENABLE_TEST_CONSTANT_TIME_OPTIMIZED` is `OFF`.

**Default**: `OFF`.

## OQS_STRICT_WARNINGS

Can be `ON` or `OFF`. When `ON`, all compiler warnings are enabled and treated as errors. This setting is recommended to be enabled prior to submission of a Pull Request as CI runs with this setting active. When `OFF`, significantly fewer compiler warnings are enabled such as to avoid undue build errors triggered by (future) compiler warning features/unknown at the development time of this library.

**Default**: `OFF`.

## OQS_EMBEDDED_BUILD

Can be `ON` or `OFF`. When `ON`, calls to standard library functions typically not present in a bare-metal embedded environment are excluded from compilation. 

At the moment, this is **only** considered for random number generation, as both `getentropy()` and a file based `/dev/urandom` are not available on embedded targets (e.g. the Zephyr port).

**Attention**: When this option is enabled, you have to supply a custom callback for obtaining random numbers using the `OQS_randombytes_custom_algorithm()` API before accessing the cryptographic API. Otherwise, all key generation and signing operations will fail. 

**Default**: `OFF`.

## OQS_LIBJADE_BUILD
Can be `ON` or `OFF`. When `ON` liboqs is built to use high assurance implementations of cryptographic algorithms from [Libjade](https://github.com/formosa-crypto/libjade). The cryptographic primitives in Libjade are written using [Jasmin](https://github.com/jasmin-lang/jasmin) and built using the Jasmin compiler. The Jasmin compiler is proven (in Coq) to preserve semantic correctness of a program, maintain secret-independence of control flow, and maintain secret independence of locations of memory access through compilation. Additionally, the Jasmin compiler guarantees thread safety because Jasmin doesn't support global variables. 

At the moment, Libjade only provides Kyber512 and Kyber768 KEMs. 

At the moment, libjade only supports Linux and Darwin based operating systems on x86_64 platforms.

**Default** `OFF`.

## OQS_ENABLE_LIBJADE_KEM_ALG/OQS_ENABLE_LIBJADE_SIG_ALG

Note: `ALG` in `OQS_ENABLE_LIBJADE_KEM_ALG/OQS_ENABLE_LIBJADE_SIG_ALG` should be replaced with the specific algorithm name as demonstrated in OQS_ENABLE_KEM_ALG/OQS_ENABLE_SIG_ALG.

**Default**: `OFF` if OQS_LIBJADE_BUILD is `OFF` else unset.

## OQS_BUILD_FUZZ_TESTS
Can be `ON` or `OFF`. When `ON` liboqs the fuzz test-suite will be enabled. This option is only available if the c compiler is set to clang i.e. `-DCMAKE_C_COMPILER=clang`.

Note: It is strongly recommended that this configuration be enabled with `CFLAGS=-fsanitize=address,fuzzer-no-link LDFLAGS=-fsanitize=address`. While fuzzing will run without these flags, enabling this instrumentation will make fuzzing performance much faster and catch [potential memory related bugs](https://clang.llvm.org/docs/AddressSanitizer.html). 

**Default** `OFF`.
