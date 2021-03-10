liboqs version 0.5.0
====================

About
-----

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.  More information on OQS can be found on our website: https://openquantumsafe.org/ and on Github at https://github.com/open-quantum-safe/.  

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  Details about liboqs can be found in [README.md](https://github.com/open-quantum-safe/liboqs/blob/main/README.md).  See in particular limitations on intended use.

liboqs can be used with the following Open Quantum Safe application integrations:

- **OQS-OpenSSL 1.1.1**: A prototype integration of liboqs-based authentication and key exchange into TLS 1.3 in our fork of OpenSSL 1.1.1; see the [OQS-OpenSSL-1\_1\_1-stable](https://github.com/open-quantum-safe/openssl/tree/OQS-OpenSSL_1_1_1-stable) branch of our OpenSSL fork's repository.
- **oqs-provider**: A standalone prototype of liboqs-based key exchange for TLS 1.3 using the OpenSSL 3 (alpha) provider integration mechanism.
- **OQS-OpenSSH**: A prototype integration of liboqs-based authentication and key exchange into SSH in our fork of OpenSSH 7.9; see the [OQS-master](https://github.com/open-quantum-safe/openssh-portable/tree/OQS-master) branch of our OpenSSH fork's repository.
- **OQS-BoringSSL**: A prototype integration of liboqs-based authentication and key exchange into TLS 1.3 in our fork of BoringSSL; see https://github.com/open-quantum-safe/boringssl.

Several [demos](https://github.com/open-quantum-safe/oqs-demos) are available for using the above libraries in applications, including Apache, Chromium, curl, haproxy, and nginx.  Performance of liboqs in several settings is measured at https://openquantumsafe.org/benchmarking/.

liboqs can also be used in the following programming languages via language-specific wrappers:

- C++, via https://github.com/open-quantum-safe/liboqs-cpp
- Go, via https://github.com/open-quantum-safe/liboqs-go
- Java, via https://github.com/open-quantum-safe/liboqs-java
- .NET, via https://github.com/open-quantum-safe/liboqs-dotnet
- Python 3, via https://github.com/open-quantum-safe/liboqs-python
- Rust, via https://github.com/open-quantum-safe/liboqs-rust

Release notes
=============

This is version 0.5.0 of liboqs.  It was released on March 10, 2021.

What's New
----------

This release continues from the 0.4.0 release of liboqs.  Its main goal is to incorporate updates submitted to Round 3 of the NIST Post-Quantum Cryptography Standardization Project.

### Deprecations

As a result of NIST's announcement of Round 3 of the Post-Quantum Cryptography Standardization Project, the 0.4.x series was the last release of liboqs to contain algorithms from Round 2 that are not Round 3 finalists or alternate candidates.  Those algorithms have been removed in the 0.5.0 series.  The algorithms in question are: NewHope, ThreeBears, MQDSS, and qTesla.

### Key encapsulation mechanisms

- BIKE: No changes.
- Classic McEliece: Added AVX2 implementation.
- FrodoKEM: Incorporate upstream bug fixes and add runtime CPU feature detection for AVX2/AES-NI implementation.
- HQC: Update to Round 3 version.
- Kyber: Update to Round 3 version.
- NTRU: Incorporate upstream bug fixes.
- NTRUPrime: Add NTRUPrime.
- Saber: Update to Round 3 version.
- SIKE: Update to version 3.4.

### Digital signature schemes

- Dilithium: Update to version 3.1.
- Falcon: Update to Round 3 version.
- Picnic: Update to Picnic v3.0.4.
- Rainbow: Update to Round 3 version.
- SPHINCS+: Incorporate upstream bug fixes.

### Other changes

- Support for building liboqs with CPU extensions has been enhanced, and most implementations now have runtime CPU feature detection.
- A tool for benchmarking memory usage has been added (`test_kem_mem` and `test_sig_mem`).
- Valgrind is used to check for secret-dependent branch throughout the library. Note that not all implementations currently avoid secret-dependent branching; see `tests/constant_time` for details.
