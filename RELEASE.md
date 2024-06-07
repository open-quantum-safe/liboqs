liboqs version 0.10.1
=====================

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

This is version 0.10.1 of liboqs. It was released on June 7, 2024.

This release is a security release which fixes potential non-constant-time behaviour in ML-KEM and Kyber based on https://github.com/pq-crystals/kyber/commit/0264efacf18dd665d2066f21df3a3290b52ba240 and https://github.com/pq-crystals/kyber/commit/9b8d30698a3e7449aeb34e62339d4176f11e3c6c. It also includes a fix for incorrectly named macros in the ML-DSA implementation.

What's New
----------

This release continues from the 0.10.0 release of liboqs.

### Key encapsulation mechanisms

- Kyber: portable C implementation updated
- ML-KEM: portable C implementation updated

### Digital signature schemes

- ML-DSA: incorrectly named macros renamed

---

Detailed changelog
------------------

* switching to dev mode again by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1743
* Update README.md by @vsoftco in https://github.com/open-quantum-safe/liboqs/pull/1769
* Fix README.md to work with Doxygen release 1.10.0 by @praveksharma in https://github.com/open-quantum-safe/liboqs/pull/1775
* Fix for incorrect macros in signatures. by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1799
* Pull Kyber/ML-KEM CT-Fix from upstream by @bhess
* Force gcc 13.2.0 over 13.3.0 by @planetf1 in https://github.com/open-quantum-safe/liboqs/pull/1805

**Full Changelog**: https://github.com/open-quantum-safe/liboqs/compare/0.10.0...0.10.1
