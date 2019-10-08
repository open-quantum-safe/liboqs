liboqs version 0.2.0
====================

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

This release of liboqs was released on October 9, 2019.  Its release page on GitHub is https://github.com/open-quantum-safe/liboqs/releases/tag/0.2.0.

What's New
----------

This release continues from the 0.1.0 release of liboqs "master branch".  

### Key encapsulation mechanisms

- Update FrodoKEM, NewHope, and SIDH/SIKE to NIST Round 2 submissions
- Add Kyber, NTRU, and Saber NIST Round 2 submissions

### Digital signature schemes

- Update Picnic to NIST Round 2 submissions
- Add Dilithium, MQDSS, Rainbow, and SPHINCS+ NIST Round 2 submissions

### Other changes

- Add support for cross-compilation on ARM
- Add more extensive continuous integration testing
- Use OpenSSL for AES, SHA-2, and SHA-3 where available
- Add functions for runtime detection of whether KEM and signature scheme algorithms are enabled

Previous releases of liboqs differentiated between "master branch" and "nist-branch", with nist-branch supporting more algorithms.  As of this release, nist-branch will no longer be developed or released, and all future releases are planned to be based on master branch.

Future work
-----------

Releases of liboqs master branch will be made every 2 to 3 months.  Details about the algorithm lifecycle of master branch can be found at https://github.com/open-quantum-safe/liboqs/wiki/Contributing-Guide.  Plans for the next release can be found online at https://github.com/open-quantum-safe/liboqs/projects/11
