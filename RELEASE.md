liboqs version 0.2.1-dev
========================

About
-----

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.  More information on OQS can be found on our website: https://openquantumsafe.org/ and on Github at https://github.com/open-quantum-safe/.  

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  Details about liboqs can be found in [README.md](https://github.com/open-quantum-safe/liboqs/blob/master/README.md).  See in particular limitations on intended use.

liboqs can be used with the following Open Quantum Safe application integrations:

- **OpenSSL 1.0.2**: A prototype integration of liboqs-based authentication and key exchange into TLS 1.2 in our fork of OpenSSL 1.0.2; see the [OQS-OpenSSL-1\_0\_2-stable](https://github.com/open-quantum-safe/openssl/tree/OQS-OpenSSL_1_0_2-stable) branch of our OpenSSL fork's repository.
- **OpenSSL 1.1.1**: A prototype integration of liboqs-based authentication and key exchange into TLS 1.3 in our fork of OpenSSL 1.1.1; see the [OQS-OpenSSL-1\_1\_1-stable](https://github.com/open-quantum-safe/openssl/tree/OQS-OpenSSL_1_1_1-stable) branch of our OpenSSL fork's repository.
- **OpenSSH 7.9 portable 1**: A prototype integration of liboqs-based authentication and key exchange into SSH in our fork of OpenSSH 7.9; see the [OQS-master](https://github.com/open-quantum-safe/openssh-portable/tree/OQS-master) branch of our OpenSSH fork's repository.

liboqs can also be used in the following programming languages via language-specific wrappers:

- C++, via https://github.com/open-quantum-safe/liboqs-cpp
- Go, via https://github.com/thales-e-security/goliboqs
- .NET, via https://github.com/open-quantum-safe/liboqs-dotnet
- Python 3, via https://github.com/open-quantum-safe/liboqs-python

Release notes
=============

This release of liboqs was released on TODO.  Its release page on GitHub is https://github.com/open-quantum-safe/liboqs/releases/tag/0.2.1.

What's New
----------

This release continues from the 0.2.0 release of liboqs.

### Key encapsulation mechanisms

- Update BIKE to Round 2 submission; removes `BIKE2-*`, `BIKE3-*`, `BIKE1-L5`, renames `BIKE1-L1` and `BIKE1-L3` to `BIKE1-L1-CPA` and `BIKE1-L3-CPA`, and adds `BIKE1-L1-FO` and `BIKE-L3-FO`
- Add Kyber "90s variants": `Kyber512-90s`, `Kyber768-90s`, `Kyber1024-90s`

### Digital signature schemes

- TBA

### Other changes

- TBA

Future work
-----------

Releases of liboqs master branch will be made every 2 to 3 months.  Details about the algorithm lifecycle of master branch can be found at https://github.com/open-quantum-safe/liboqs/wiki/Contributing-Guide.  Plans for the next release can be found online at https://github.com/open-quantum-safe/liboqs/projects/11
