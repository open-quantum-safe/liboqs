liboqs version 0.6.0-dev
========================

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

This is the development track for version 0.6.0 of liboqs.  It was released on XXX.

What's New
----------

### Key encapsulation mechanisms

### Digital signature schemes
This release includes the NIST PQC round 3 parameters for SPHINCS.

### Other changes

#### Build options changed

By default, liboqs is now no longer built by default with CPU runtime feature detection and thus resulting executables may crash if not compiled suitably for the  CPU on which the code shall be executed. 
For docker files, we recommend setting [OQS_DIST_BUILD](https://github.com/open-quantum-safe/liboqs/wiki/Customizing-liboqs#OQS_DIST_BUILD) to obtain code able to handle different CPU feature sets. Also, [OQS_OPT_TARGET](https://github.com/open-quantum-safe/liboqs/wiki/Customizing-liboqs#oqs_opt_target) can be used to target a specific CPU at compile time. As usual, these flags are documented in the [build options wiki](https://github.com/open-quantum-safe/liboqs/wiki/Customizing-liboqs).
