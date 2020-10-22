Version 3.0.3 -- 2020-10-12
---------------------------

* Fix explicit_bzero fallback implementation
* Remove some unused code

Version 3.0.2 -- 2020-10-06
---------------------------

* Update SHAKE3 implementation
* Add support to check constant time implementation with TIMECOP
* Slightly reduce memory consumption
* Add support for BSD variants

Version 3.0.1 -- 2020-08-11
---------------------------

* Expose picnic_sk_to_pk as part of the public API
* Add picnic_clear_private_key to clear the private key

Version 3.0 -- 2020-04-15
-------------------------

* Implement new Picnic 3 parameter set. This implementation replaces the Picnic 2 parameter set.
* Implement new Picnic instances with full Sbox layer.
* Various small improvements and bug fixes.
* Remove all optimizations for partial LowMC instances except for OLLE.

Version 2.2 -- 2020-04-08
---------------------------

* Fix Picnic2 implementation on big endian systems
* Add support for SHA3/SHAKE3 instructions on IBM z.
* Various small improvements and bug fixes.
* Remove LowMC instances with m=1.

Version 2.1.2 -- 2019-10-03
---------------------------

* Add options to build with ZKB++- or KKW-based instances only.
* Fix ARM NEON optimizations.
* Slightly reduce heap usage.
* Remove more unused code.

Version 2.1.1 -- 2019-08-07
---------------------------

* Various small improvements and bug fixes.

Version 2.1 -- 2019-07-29
-------------------------

* Remove M4RM-based implementation.
* Fix input size in Picnic2's commitment implementation.
* Additional improvements and optimizations of the Picnic2 code.

Version 2.0 -- 2019-04-08
-------------------------

* Implement Picnic 2.
* Use 4-times parallel SHAKE3 for faster PRF evaluation, commitment generation, etc.
* Fix size of salts to 32 bytes.

Version 1.3.1 -- 2018-12-21
---------------------------

* Reduce heap usage.

Version 1.3 -- 2018-12-21
-------------------------

* Implement linear layer optimizations to speed up LowMC evaluations. Besides the runtime improvements, this optimization also greatly reduces the memory size of the LowMC instances.
* Provide LowMC instances with m=1 to demonstrate feasibility of those instances.
* Slightly improve internal storage of matrices to require less memory.
* Remove unused code and support for dynamic LowMC instances.

Version 1.2 -- 2018-12-05
-------------------------

* Implement RRKC optimizations for round constants.
* Compatibility fixes for Mac OS X.
* Reduce memory usage when using Fiat-Shamir and Unruh transform in the same process.
* Fix deviations from specification. The KDF was missing the output length as input and the public key was incorrectly serialized. Note that this change requires an update of the test vectors.
* Update SHA3 implementation and fix endiannes bug on big-endian.
* Record state before Sbox evaluation and drop one branch of XOR computations. This optimization is based based on an idea by Markus Schofnegger.
* Add per-signature salt to random tapes generation. Prevents a seed-guessing attack reported by Itai Dinur.

Version 1.1 -- 2018-06-29
-------------------------

* Compatibility fixes for Visual Studio, clang and MinGW.
* Various improvements to the SIMD versions of the matrix operations.
* Default to constant-time matrix multiplication algorithms without lookup tables.
* Add option to feed extra randomness to initial seed expansion to counter fault attacks.
* Version submitted for inclusion in SUPERCOP.

Version 1.0 -- 2017-11-28
-------------------------

* Initial release.
* Version submitted to the NIST PQC project.
