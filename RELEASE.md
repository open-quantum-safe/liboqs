liboqs version 0.4.0
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
- Go, via https://github.com/open-quantum-safe/liboqs-go
- Java, via https://github.com/open-quantum-safe/liboqs-java
- .NET, via https://github.com/open-quantum-safe/liboqs-dotnet
- Python 3, via https://github.com/open-quantum-safe/liboqs-python

Release notes
=============

This is version 0.4.0 of liboqs.  It was released on August 11, 2020.

What's New
----------

This release continues from the 0.3.0 release of liboqs.

### Key encapsulation mechanisms

- HQC: Added version 2019/08/24
- NewHope: Update to version 1.1
- SIKE: Update to version 3.3

### Digital signature schemes

- Dilithium: Use version directly from PQCrystals GitHub
- Picnic: Update to v3.0

### Other changes

- AES-NI support when liboqs does not use OpenSSL for AES

Deprecations
------------

As a result of NIST's announcement of Round 3 of the Post-Quantum Cryptography Standardization Project, the 0.4.x series will be the last release(s) of liboqs that contain algorithms from Round 2 that are not Round 3 finalists or alternate candidates.  Those algorithms will be removed in the 0.5.0 release.  The algorithms in question are: NewHope, ThreeBears, MQDSS, and qTesla.  These algorithms are considered deprecated within liboqs and will receive no updates after this release.
