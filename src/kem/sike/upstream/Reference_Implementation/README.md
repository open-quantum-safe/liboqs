# SIKE C reference implementation

C reference implementation of Supersingular Isogeny Key Encapsulation (SIKE).

Implements the following procedures as part of SIKE:
- SIDH key agreement
- PKE public key encryption
- KEM Key encapsulation mechanism
- SIKE instantiations SIKEp503, SIKEp751 and SIKEp964.

## Requirements

- Cmake (version 3.5 or later)
- GMP (tested with version 6.1.2)
- C99-compatible compiler

## Build

- `mkdir -p build`
- `cd build`
- `cmake ..`
- `make`

The following files are available after `make` (in folder `build`):
- `libsike_ref_for_test.a`: SIKE library for testing, using the NIST RNG
- `libsike_ref.a`: SIKE library, using /dev/urandom
- `SIKEp<###>/libsikep<###>_ref.a`: SIKEp<###> library with NIST API. To be linked together with `libsike_ref.a` or `libsike_ref_for_test.a`.
- `SIKEp<###>/kat_SIKEp<###>`: KAT executable for SIKEp<###>
- `test/sike_test`: Executable for self-tests and performance tests 

To clean a build do a `make clean` inside the build folder.

Before doing a fresh build with different cmake options, delete the folder `build`.

## Tests

The following tests are available: NIST KAT, random self-tests and performance tests.

### NIST KAT
NIST KAT executables are built as part of `make`.
The executables are located in `build/SIKEp<###>/`, the executables are named `kat_SIKEp<###>`, where `<###>` is 503, 751, or 964.

The executables should be run from folder `SIKEp<###>` to allow the KAT files to be found (relative to `build`):

- `cd SIKEp<###>`
- `./kat_SIKEp<###>`

Alternatively, building and running the KAT set can be done automatically with a script (path relative to `Reference_Implementation`):

- `./build_run_kat.bash p<###>` using the default cc compiler.
 
### Self-tests
Randomized self-tests for arithmetic, KA, PKE and KEM are available with the following command (relative to `build`):

- `make test`

### Performance tests
KEM performance tests with `<rep>` repetitions are available with the following command (relative to `build`):

- `./test/sike_test SIKEp<###> sike_speed <rep>`

Alternatively, the following script builds and runs all KEM performance tests (using clang as compiler, path relative to `Reference_Implementation`):

- `./build_run_bench_default.bash` using the default cc compiler.

or

- `./build_run_bench_clang.bash` using clang.

## Directory structure
* `src`: C source of SIKE
* `include`: Header files of SIKE
* `SIKEp<xxx>`: SIKE instantiations implementing the NIST KEM API
* `symmetric`: Symmetric primitives and RNG
* `tests`: Self-tests and performance tests
