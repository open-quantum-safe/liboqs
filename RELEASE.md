liboqs nist-branch snapshot 2018-07 -- DRAFT
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

This snapshot of nist-branch was released on July 27, 2018.  Its release page on Github is https://github.com/open-quantum-safe/liboqs/releases/tag/nist-branch-snapshot-2018-07.

What's New
----------

This is the third snapshot release of liboqs nist-branch.

### New key encapsulation mechanisms

The following KEMs have been added in the 2018-05 snapshot release:

- **LEDAkem**: 9 parameterizations: `LEDAKEM_C1_N02`, `LEDAKEM_C1_N03`, `LEDAKEM_C1_N04`, `LEDAKEM_C3_N02`, `LEDAKEM_C3_N03`, `LEDAKEM_C3_N04`, `LEDAKEM_C5_N02`, `LEDAKEM_C5_N03`, `LEDAKEM_C5_N04` (contributed by Shravan Mashra (University of Waterloo))

### New signature API and schemes

liboqs nist-branch now includes support for signature schemes via the API described in `src/sig/sig.h`; the API is based on the NIST and SUPERCOP APIs.  Signature schemes can be tested using `./test_sig`, benchmarked using `./speed_sig`; `example_sig` contains a minimal example of using the signature API.

The following signature schemes have been added in the 2018-07 snapshot release:

- **Picnic**: 6 parameterizations: `picnic_L1_FS`, `picnic_L1_UR`, `picnic_L3_FS`, `picnic_L3_UR`, `picnic_L5_FS`, `picnic_L5_UR` (contributed by Christian Paquin (Microsoft Research))
- **qTESLA**: 5 parameterizations: `qTESLA_I`, `qTESLA_III_size`, `qTESLA_III_speed`, `qTESLA_p_I`, `qTESLA_p_I` (contributed by Christian Paquin (Microsoft Research))

### Fixes

- Automatic detection of binutils version for BIKE build (contributed by Maxime Anvari)

Future work
-----------

Snapshot releases of nist-branch will be made monthly.  Plans for the next snapshot release of nist-branch can be found online at https://github.com/open-quantum-safe/liboqs/projects/9.
