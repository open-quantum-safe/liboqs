liboqs version 0.9.1
====================

About
-----

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.  More information on OQS can be found on our website: https://openquantumsafe.org/ and on Github at https://github.com/open-quantum-safe/.  

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  Details about liboqs can be found in [README.md](https://github.com/open-quantum-safe/liboqs/blob/main/README.md).  See in particular limitations on intended use.

liboqs can be used with the following Open Quantum Safe application integrations:

- **oqs-provider**: A standalone prototype [OpenSSL 3 provider](https://www.openssl.org/docs/manmaster/man7/provider.html) enabling liboqs-based quantum-safe and hybrid key authentication and exchange for TLS 1.3, X.509 certificate generation and CMS operations.
- **OQS-BoringSSL**: A prototype integration of liboqs-based authentication and key exchange into TLS 1.3 in our fork of BoringSSL; see https://github.com/open-quantum-safe/boringssl.
- **OQS-OpenSSH**: A prototype integration of liboqs-based authentication and key exchange into Secure Shell (SSH) version 2 in our fork of OpenSSH; see https://github.com/open-quantum-safe/openssh.

Several [demos](https://github.com/open-quantum-safe/oqs-demos) are available for using the above libraries in applications, including Apache, Chromium, curl, haproxy, nginx, and Wireshark.  Performance of liboqs in several settings is measured at https://openquantumsafe.org/benchmarking/.

liboqs can also be used in the following programming languages via language-specific wrappers:

- C++, via https://github.com/open-quantum-safe/liboqs-cpp
- Go, via https://github.com/open-quantum-safe/liboqs-go
- Java, via https://github.com/open-quantum-safe/liboqs-java
- .NET, via https://github.com/open-quantum-safe/liboqs-dotnet
- Python 3, via https://github.com/open-quantum-safe/liboqs-python
- Rust, via https://github.com/open-quantum-safe/liboqs-rust

Release notes
=============

This is version 0.9.1 of liboqs. It was released on December 22, 2023.

This release is a security release which fixes potential non-constant-time behaviour in Kyber based on https://github.com/pq-crystals/kyber/commit/dda29cc63af721981ee2c831cf00822e69be3220

What's New
----------

This release continues from the 0.9.0 release of liboqs.

### Key encapsulation mechanisms

- Kyber: C, AVX2, and aarch64 implementation updated

Detailed changelog
------------------

* pull kyber from upstream: dda29cc63af721981ee2c831cf00822e69be3220 (#1631) by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1633
* Apply patch to Kyber aarch64 code from PQClean for variable-time division issue -> targeting 0.9.1 by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1637

**Full Changelog**: https://github.com/open-quantum-safe/liboqs/compare/0.9.0...0.9.1