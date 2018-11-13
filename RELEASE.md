liboqs nist-branch snapshot 2018-10
===================================

About
-----

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.  More information on OQS can be found on our website: https://openquantumsafe.org/ and on Github at https://github.com/open-quantum-safe/.  

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  

This branch of liboqs (**nist-branch**) focuses on incorporating submissions to the NIST Post-Quantum Cryptography standardization project.  Details about nist-branch can be found in [README.md](https://github.com/open-quantum-safe/liboqs/blob/nist-branch/README.md).  See in particular limitations on intended use.

This branch of liboqs can be used with the following Open Quantum Safe application integrations:

- **OpenSSL 1.0.2**: A prototype integration of liboqs-based key exchange  into TLS 1.2 in our fork of OpenSSL 1.0.2; see the [OQS-OpenSSL-1\_0\_2-stable](https://github.com/open-quantum-safe/openssl/tree/OQS-OpenSSL_1_0_2-stable) branch of our OpenSSL fork's repository.
- **OpenSSL 1.1.1**: A prototype integration of liboqs-based key exchange and authentication into TLS 1.3 in our fork of OpenSSL 1.1.1; see the [OQS-OpenSSL-1\_1\_1-stable](https://github.com/open-quantum-safe/openssl/tree/OQS-OpenSSL_1_1_1-stable) branch of our OpenSSL fork's repository.
- **OpenSSH 7.7 portable 1**: A prototype integration of liboqs-based key exchange into SSH in our fork of OpenSSH 7.7; see the [OQS-master](https://github.com/open-quantum-safe/openssh-portable/tree/OQS-master) branch of our OpenSSH fork's repository.

Release notes
=============

This snapshot of nist-branch was released on November 13, 2018.  Its release page on Github is https://github.com/open-quantum-safe/liboqs/releases/tag/nist-branch-snapshot-2018-11.

What's New
----------

This is the fourth snapshot release of liboqs nist-branch.

### New key encapsulation mechanisms

The following KEMs have been added in the 2018-11 snapshot release:

- **Titanium**: 4 parameterizations: `Titanium-CCA-std`, `Titanium-CCA-med`, `Titanium-CCA-hi`, `Titanium-CCA-super` (contributed by Ben Davies (University of Waterloo))

The BIKE implementation was updated.

### New signature schemes

The following signature schemes have been added in the 2018-11 snapshot release:

- **CRYSTALS-Dilithium**: 3 parameterizations: `Dilithium_II_medium`, `Dilithium_III_recommended`, `Dilithium_IV_very_high` (contributed by Tancrède Lepoint)

The qTESLA implementation was updated with a new upstream version (2018-07-28).

### API changes

- The public and private API have been more clearly delineated.  The public API is now deemed to consist of `oqs/common.h`, `oqs/oqsconfig.h`, `oqs/kem.h`, `oqs/rand.h`, and `oqs/sig.h` and is marked with `OQS_API`.
- An `alg_version` field is present on the `OQS_KEM` and `OQS_SIG` objects to indicate the version of the algorithm present.

### Fixes

- URL for downloading Keccak Code Package updated.
- See https://github.com/open-quantum-safe/liboqs/issues?q=is%3Aissue+label%3Anist-branch+is%3Aclosed+project%3Aopen-quantum-safe%2Fliboqs%2F9 for a detailed list of changes.

Future work
-----------

Snapshot releases of nist-branch will be made monthly.  Plans for the next snapshot release of nist-branch can be found online at https://github.com/open-quantum-safe/liboqs/projects/10.
