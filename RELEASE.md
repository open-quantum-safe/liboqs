liboqs nist-branch snapshot 2018-05
===================================

About
-----

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.  More information on OQS can be found on our website: https://openquantumsafe.org/ and on Github at https://github.com/open-quantum-safe/.  

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  

This branch of liboqs (**nist-branch**) focuses on incorporating submissions to the NIST Post-Quantum Cryptography standardization project.  Details about nist-branch can be found in [README.md](https://github.com/open-quantum-safe/liboqs/blob/nist-branch/README.md).  See in particular limitations on intended use.

This branch of liboqs can be used with the following Open Quantum Safe application integrations:

- OpenSSL 1.0.2: A prototype integration of liboqs-based key exchange into TLS 1.2 in our fork of OpenSSL 1.0.2; see the [OQS-OpenSSL-1_0_2-stable](https://github.com/open-quantum-safe/openssl/tree/OQS-OpenSSL_1_0_2-stable) branch of our OpenSSL fork's repository.


Release notes
=============

This snapshot of nist-branch was released on May 30, 2018.  Its release page on Github is https://github.com/open-quantum-safe/liboqs/releases/tag/nist-branch-snapshot-2018-05.

What's New
----------

This is the second snapshot release of liboqs nist-branch.

### New key encapsulation mechanisms

The following KEMs have been added in the 2018-05 snapshot release:

- **BIG QUAKE**: 3 parameterization: BIG_QUAKE_1, BIG_QUAKE_3, BIG_QUAKE_5 (contributed by Shravan Mashra (University of Waterloo))
- **BIKE**: 9 parameterizations: BIKE1-L1, BIKE1-L3, BIKE1-L5, BIKE2-L1, BIKE2-L3, BIKE2-L5, BIKE3-L1, BIKE3-L3, BIKE3-L5; optimized builds on Linux platforms with AVX/AVX2/AVX512 support (contributed by Nir Drucker and Shay Gueron (Amazon Web Services))
- **LIMA**: 6 parameterizations: Lima-2p-1024-CCA-KEM, Lima-2p-2048-CCA-KEM, Lima-sp-1018-CCA-KEM, Lima-sp-1306-CCA-KEM, Lima-sp-1822-CCA-KEM, Lima-sp-2062-CCA-KEM (contributed by Douglas Stebila (McMaster University))
- **Saber**: 3 parameterizations: LightSaber-KEM, Saber-KEM, FireSaber-KEM (contributed by Douglas Stebila (McMaster University))
- **SIKE**: 2 parameterizations: Sike-p503, Sike-p751 (contributed by Christian Paquin (Microsoft Research))

### New signature schemes

The following signature schemes have been added in the 2018-07 snapshot release:
- **Picnic**: 6 parameterizations: picnic_L1_FS, picnic_L1_UR, picnic_L3_FS, picnic_L3_UR, picnic_L5_FS, picnic_L5_UR (contributed by Christian Paquin (Microsoft Research))

### General improvements

- Can now be built with multi-threaded make (e.g., `make -j8`)
- The default pseudorandom number generator is now OpenSSL's `RAND_bytes` function for better performance; applications can choose a different PRNG at runtime
- `example_kem` matches documented example in https://github.com/open-quantum-safe/liboqs/wiki/Minimal-example-of-a-post-quantum-key-encapsulation-mechanism-(using-the-new-NIST-like)-API (contributed by Vlad Gheorghiu)

### Fixes

- Fixed improperly built shared library
- Cleansed secret variables in example programs

Comparison to liboqs master
---------------------------

This snapshot release of nist-branch contains the following differences compared to the current version of liboqs master:

- Algorithms are formulated as key encapsulation mechanisms, rather than key exchange mechanisms.
- Integrations are "light touch" -- see README.md for more about integration philosophy.
- A different build process is used.
- A global `randombytes` function is available for random number generation, rather than the `OQS_RAND` object in master.
- Signature schemes are not yet supported.

Future work
-----------

Snapshot releases of nist-branch will be made monthly.  Plans for the next snapshot release of nist-branch can be found online at https://github.com/open-quantum-safe/liboqs/projects/8.

By the end of June 2018, we aim to release a new version of our master branch that uses the same API as nist-branch.
