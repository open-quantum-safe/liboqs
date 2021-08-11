liboqs version 0.7.0
====================

About
-----

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.  More information on OQS can be found on our website: https://openquantumsafe.org/ and on Github at https://github.com/open-quantum-safe/.  

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  Details about liboqs can be found in [README.md](https://github.com/open-quantum-safe/liboqs/blob/main/README.md).  See in particular limitations on intended use.

liboqs can be used with the following Open Quantum Safe application integrations:

- **OQS-OpenSSL 1.1.1**: A prototype integration of liboqs-based authentication and key exchange into TLS 1.3 in our fork of OpenSSL 1.1.1; see the [OQS-OpenSSL-1\_1\_1-stable](https://github.com/open-quantum-safe/openssl/tree/OQS-OpenSSL_1_1_1-stable) branch of our OpenSSL fork's repository.
- **oqs-provider**: A standalone prototype of liboqs-based key exchange for TLS 1.3 using the OpenSSL 3 (alpha) provider integration mechanism.
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

This is version 0.7.0 of liboqs.  It was released on August 11, 2021.

What's New
----------

This release continues from the 0.6.0 release of liboqs. 

### Key encapsulation mechanisms

- Update BIKE to Round 3 (version 3.2) -- add BIKE1, BIKE3, remove BIKE1-L1-CPA, BIKE1-L1-FO, BIKE1-L3-CPA, BIKE1-L3-FO
- Update NTRU Prime implementation
- Update SIKE implementation
- Remove OQS\_KEM\_DEFAULT alias

### Digital signature schemes

- Remove OQS\_SIG\_DEFAULT alias

### Other changes

- Allow liboqs to build on other architectures, with preliminary builds on ppc64le and x86.
- Support for building liboqs on Apple Silicon
- New algorithm datasheets with more details on implementations.
- Optimized SHA-256 implementation on ARMv8 with crypto extensions.
