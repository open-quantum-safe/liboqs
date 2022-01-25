# Changelog for the optimized Picnic implementation

## Version 3.0.11 -- 2022-01-25

* Fix NEON code on M1.
* Ensure SSE2/AVX2/NEON shift intrinsics with immediate operands are used correctly.
* Use Boost.Test as unit test framework.

## Version 3.0.10 -- 2022-01-08

* Fix build with llvm on ARM with NEON enabled

## Version 3.0.9 -- 2021-12-22

* Unbreak x86-32 build.
* Fix build on M1 with NEON enabled.

## Version 3.0.8 -- 2021-12-18

* Prefix compat function implementations with `picnic_`.
* Use OQS instruction set checking functions.
* Use OQS implementations of `aligned_alloc`, `aligned_free`, `explicit_bzero`, and `timingsafe_bcmp`.
* Install cmake configuration files.

## Version 3.0.7 -- 2021-12-15

* Various changes to improve OQS integration.
* Require cmake version 3.10.

## Version 3.0.6 -- 2021-12-14

* Reduce size of global parameters for instance specification to 12 bytes per instance.
* Provide compat implementation of `clz` on MSVC using `_BitScanReverse`.
* Do not assume that `aligned_alloc` is available on MSVC.

## Version 3.0.5 -- 2021-10-19

* Update SHAKE3 implementation.
* Fix build with GCC 11.
* Reduce memory consumption slightly.
* Expose `picnic_get_{private,public}key_size` as part of the public API.
* Add `picnic_get_{private,public}_key_param` to retrieve a key's parameter set.

## Version 3.0.4 -- 2020-12-17

* Slightly improve memory consumption.
* Initial work to support PQClean integration in the future.
* Add cmake options to control availability of specific LowMC instances.

## Version 3.0.3 -- 2020-10-12

* Fix `explicit_bzero` fallback implementation.
* Remove some unused code.

## Version 3.0.2 -- 2020-10-06

* Update SHAKE3 implementation.
* Add support to check constant time implementation with TIMECOP.
* Slightly reduce memory consumption.
* Add support for BSD variants.

## Version 3.0.1 -- 2020-08-11

* Expose `picnic_sk_to_pk` as part of the public API.
* Add `picnic_clear_private_key` to clear the private key.

## Version 3.0 -- 2020-04-15

* Implement new Picnic 3 parameter set. This implementation replaces the Picnic 2 parameter set.
* Implement new Picnic instances with full Sbox layer.
* Various small improvements and bug fixes.
* Remove all optimizations for partial LowMC instances except for OLLE.

## Version 2.2 -- 2020-04-08

* Fix Picnic2 implementation on big endian systems.
* Add support for SHA3/SHAKE3 instructions on IBM z.
* Various small improvements and bug fixes.
* Remove LowMC instances with m=1.

## Version 2.1.2 -- 2019-10-03

* Add options to build with ZKB++- or KKW-based instances only.
* Fix ARM NEON optimizations.
* Slightly reduce heap usage.
* Remove more unused code.

## Version 2.1.1 -- 2019-08-07

* Various small improvements and bug fixes.

## Version 2.1 -- 2019-07-29

* Remove M4RM-based implementation.
* Fix input size in Picnic2's commitment implementation.
* Additional improvements and optimizations of the Picnic2 code.

## Version 2.0 -- 2019-04-08

* Implement Picnic 2.
* Use 4-times parallel SHAKE3 for faster PRF evaluation, commitment generation, etc.
* Fix size of salts to 32 bytes.

## Version 1.3.1 -- 2018-12-21

* Reduce heap usage.

## Version 1.3 -- 2018-12-21

* Implement linear layer optimizations to speed up LowMC evaluations. Besides the runtime improvements, this optimization also greatly reduces the memory size of the LowMC instances.
* Provide LowMC instances with m=1 to demonstrate feasibility of those instances.
* Slightly improve internal storage of matrices to require less memory.
* Remove unused code and support for dynamic LowMC instances.

## Version 1.2 -- 2018-12-05

* Implement RRKC optimizations for round constants.
* Compatibility fixes for Mac OS X.
* Reduce memory usage when using Fiat-Shamir and Unruh transform in the same process.
* Fix deviations from specification. The KDF was missing the output length as input and the public key was incorrectly serialized. Note that this change requires an update of the test vectors.
* Update SHA3 implementation and fix endiannes bug on big-endian.
* Record state before Sbox evaluation and drop one branch of XOR computations. This optimization is based based on an idea by Markus Schofnegger.
* Add per-signature salt to random tapes generation. Prevents a seed-guessing attack reported by Itai Dinur.

## Version 1.1 -- 2018-06-29

* Compatibility fixes for Visual Studio, clang and MinGW.
* Various improvements to the SIMD versions of the matrix operations.
* Default to constant-time matrix multiplication algorithms without lookup tables.
* Add option to feed extra randomness to initial seed expansion to counter fault attacks.
* Version submitted for inclusion in SUPERCOP.

## Version 1.0 -- 2017-11-28

* Initial release.
* Version submitted to the NIST PQC project.
