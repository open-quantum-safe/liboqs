liboqs version 0.3.0
====================

About
-----

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.  More information on OQS can be found on our website: https://openquantumsafe.org/ and on Github at https://github.com/open-quantum-safe/.  

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  Details about liboqs can be found in [README.md](https://github.com/open-quantum-safe/liboqs/blob/master/README.md).  See in particular limitations on intended use.

liboqs can be used with the following Open Quantum Safe application integrations:

- **OQS-OpenSSL 1.1.1**: A prototype integration of liboqs-based authentication and key exchange into TLS 1.3 in our fork of OpenSSL 1.1.1; see the [OQS-OpenSSL-1\_1\_1-stable](https://github.com/open-quantum-safe/openssl/tree/OQS-OpenSSL_1_1_1-stable) branch of our OpenSSL fork's repository.
- **OQS-OpenSSH 7.9 portable 1**: A prototype integration of liboqs-based authentication and key exchange into SSH in our fork of OpenSSH 7.9; see the [OQS-master](https://github.com/open-quantum-safe/openssh-portable/tree/OQS-master) branch of our OpenSSH fork's repository.
- **OQS-BoringSSL**: A prototype integration of liboqs-based authentication and key exchange into TLS 1.3 in our fork of BoringSSL; see https://github.com/open-quantum-safe/boringssl.

liboqs can also be used in the following programming languages via language-specific wrappers:

- C++, via https://github.com/open-quantum-safe/liboqs-cpp
- Go, via https://github.com/thales-e-security/goliboqs
- .NET, via https://github.com/open-quantum-safe/liboqs-dotnet
- Python 3, via https://github.com/open-quantum-safe/liboqs-python

Release notes
=============

This is version 0.3.0 of liboqs, which was released on June 10, 2020.  Its release page on GitHub is https://github.com/open-quantum-safe/liboqs/releases/tag/0.3.0.

What's New
----------

This release continues from the 0.2.0 release of liboqs.

### Key encapsulation mechanisms

- BIKE: Update to Round 2 submission; removes `BIKE2-*`, `BIKE3-*`, `BIKE1-L5`, renames `BIKE1-L1` and `BIKE1-L3` to `BIKE1-L1-CPA` and `BIKE1-L3-CPA`, and adds `BIKE1-L1-FO` and `BIKE-L3-FO`
- Classic McEliece: Newly added
- Kyber: Add "90s variants": `Kyber512-90s`, `Kyber768-90s`, `Kyber1024-90s`
- LEDAcrypt: Newly added
- ThreeBears: Newly added

### Digital signature schemes

- Falcon: Newly added
- MQDSS: Update to v2.1
- Picnic: Update to v2.2
- qTesla: Update to v1.0
- Rainbow: Newly added
- SPHINCS+: Add AVX2 and AESNI implementations

### Other changes

- Switch build system to CMake
- Add support for building via Mingw on Windows
- Support cross compilation via CMake to Raspberry Pi, Android, Windows, and more
- Most optimizations dependent on specific CPU features are automatically detected at runtime, rather than being selected at compile-time
