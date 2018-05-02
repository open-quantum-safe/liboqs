# SIKE-Weierstrass

SIKE-Weierstrass implementation.

This implementation uses Weierstrass curves instead of Montgomery curves as used in SIKE.

Implements the following procedures as part of SIKE-Weierstrass:
- SIDH key agreement
- PKE public key encryption
- KEM Key encapsulation mechanism
- SIKE-Weierstrass instantiations SIKEp503, SIKEp751 and SIKEp964

## SIKE and SIKE-Weierstrass

The SIDH key agreement step is interchangeable between SIKE and SIKE-Weierstrass. 

The public key format is different:
For an elliptic curve in short Weierstrass form `y^2=x^3+ax+b` and generator points `P` and `Q`, the public key consists of:
- `Px`, `Py`, `Qx`, `Qy`, `a`, `b`

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
- `libsike_weierstrass_for_test.a`: SIKE-Weierstrass library for testing, using the NIST RNG
- `libsike_weierstrass.a`: SIKE-Weierstrass library, using /dev/urandom
- `libsikep<###>_weierstrass.a`: SIKEp<###>-Weierstrass library with NIST API. To be linked together with `libsike_weierstrass.a` or `libsike_weierstrass_for_test.a`.

To clean a build do a `make clean` inside the build folder.

Before doing a fresh build with different cmake options, delete the folder `build`.

## Tests

The following tests are available: Random self-tests and performance tests.

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
* `symmetric`: Symmetric primitives
* `tests`: Self-tests and performance tests
