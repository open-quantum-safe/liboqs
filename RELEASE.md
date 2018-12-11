liboqs nist-branch snapshot 2019-01-dev
=======================================

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

This snapshot of nist-branch was released on XXX.  Its release page on Github is XXX.

What's New
----------

This is the fifth snapshot release of liboqs nist-branch.

### New key encapsulation mechanisms

The following KEMs have been added in the 2019-01 snapshot release:

### New signature schemes

The following signature schemes have been added in the 2019-01 snapshot release:

### API changes

### Fixes

- See https://github.com/open-quantum-safe/liboqs/issues?q=is%3Aissue+label%3Anist-branch+is%3Aclosed+project%3Aopen-quantum-safe%2Fliboqs%2F9 for a detailed list of changes.

Future work
-----------

Snapshot releases of nist-branch will be made monthly.  Plans for the next snapshot release of nist-branch can be found online at https://github.com/open-quantum-safe/liboqs/projects/10.
