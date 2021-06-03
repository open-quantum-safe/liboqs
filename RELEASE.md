liboqs version 0.6.0-rc2
========================

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

This is release candidate 2 for version 0.6.0 of liboqs.  It was released on June 2, 2021.

What's New
----------

This release continues from the 0.5.0 release of liboqs. 

### Key encapsulation mechanisms

- Update Classic McEliece implementation
- Bug fixes in SIKE
- Bug fixes in HQC
- Change unsigned char to uint8_t in KEM API

### Digital signature schemes

- Update SPHINCS+ to Round 3 version

### Other changes

- Improve random number generator when not relying on OpenSSL
- Improve run-time and compile-time guarding of optimized code
- Remove (unused) AES decryption code from common symmetric encryption code
- Replace AES plain C implementation with a constant-time version
- Update Windows cross-compiling toolchain
- **Build options changed**:
	- By default, liboqs is now no longer built by default with CPU runtime feature detection and thus resulting executables may crash if not compiled suitably for the  CPU on which the code shall be executed.  For Docker files, we recommend setting [OQS\_DIST\_BUILD](https://github.com/open-quantum-safe/liboqs/wiki/Customizing-liboqs#OQS_DIST_BUILD) to obtain code able to handle different CPU feature sets. Also, [OQS\_OPT\_TARGET](https://github.com/open-quantum-safe/liboqs/wiki/Customizing-liboqs#oqs_opt_target) can be used to target a specific CPU at compile time. These flags are documented on the [build options wiki page](https://github.com/open-quantum-safe/liboqs/wiki/Customizing-liboqs).
