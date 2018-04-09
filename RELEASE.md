*DRAFT RELEASE NOTES*

liboqs nist-branch snapshot 2018-04
===================================

About
-----

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.  More information on OQS can be found on our website: https://openquantumsafe.org/ and on Github at https://github.com/open-quantum-safe/.  

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  

This branch of liboqs (**nist-branch**) focuses on incorporating submissions to the NIST Post-Quantum Cryptography standardization project.  Details about nist-branch can be found in [README.md](https://github.com/open-quantum-safe/liboqs/blob/nist-branch/README.md).  See in particular limitations on intended use.

Release notes
=============

This snapshot of nist-branch was released on *TODO*.  Its release page on Github is https://github.com/open-quantum-safe/liboqs/releases/tag/nist-branch-snapshot-2018-04.

What's New
----------

This is the first snapshot release of liboqs nist-branch.

This branch of liboqs aims to non-selectively incorporate submissions to the NIST Post-Quantum Cryptography project for the purposes of benchmarking and integration into a common API for liboqs-reliant applications.

This branch takes a "light touch" approach to incorporation:

- Source code from a NIST submission will be included ideally with no changes, in an "upstream" subdirectory.
- A thin wrapper will be written to provide the implementation using the liboqs API.
- The implementation will be added to the build process.
- To avoid namespace collisions between different algorithms, symbol renaming will be used on the compiled files.

### New key encapsulation mechanisms

- FrodoKEM: 4 parameterizations: FrodoKEM-640-AES, FrodoKEM-640-cSHAKE, FrodoKEM-976-AES, FrodoKEM-976-cSHAKE
- CRYSTALS-KYBER: 3 parameterizations: Kyber-512, Kyber-768, Kyber-1024
- NewHopeNIST: 2 parameterizations: NewHope512-CCA-KEM, NewHope1024-CCA-KEM

### Generated executables and libraries

- `test_kem`: Simple test harness for all enabled key encapsulation mechanisms
- `kat_kem`: Known answer test generator for all enabled key encapsulation mechanisms, to compare against KAT values in NIST submissions
- `speed_kem`: Benchmarking program for key encapsulation mechanisms; see `./speed_kem --help` for usage instructions
- `example_kem`: Minimal runnable example showing the usage of the KEM API
- `liboqs.a`: Static library
- `liboqs.so`: Shared library

### Documentation

- Full Doxygen documentation of the public API (`oqs/common.h`, `oqs/config.h`, `oqs/kem.h`, and `oqs/rand.h`)
- Algorithm datasheets for all supported algorithms in [docs/algorithms](https://github.com/open-quantum-safe/liboqs/tree/nist-branch/docs/algorithms)
- Instructions for contributing new algorithms in [CONTRIBUTING.md](https://github.com/open-quantum-safe/liboqs/blob/nist-branch/CONTRIBUTING.md)

Comparison to liboqs master
---------------------------

This snapshot release of nist-branch contains the following differences compared to the current version of liboqs master:

- Algorithms are formulated as key encapsulation mechanisms, rather than key exchange mechanisms
- Integrations are "light touch" -- see README.md for more about integration philosophy.
- A different build process is used
- A global `randombytes` function is available for random number generation, rather than the `OQS_RAND` object in master

Future work
-----------

Snapshot releases of nist-branch will be made monthly.

By mid-May 2018, we intend to have nist-branch and master branch with the same API, and for our OpenSSL and OpenSSH integrations building against both nist-branch and master branch.
