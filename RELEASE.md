liboqs master branch version 0.1.0
==================================

About
-----

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.  More information on OQS can be found on our website: https://openquantumsafe.org/ and on Github at https://github.com/open-quantum-safe/.  

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  

This branch of liboqs (**master branch**) aims to selectively incorporate allegedly quantum-resistant key encapsulation mechanisms and signature schemes, for the purposes of integration into a common API for liboqs-reliant applications.  Details about liboqs master branch can be found in [README.md](https://github.com/open-quantum-safe/liboqs/blob/master/README.md).  See in particular limitations on intended use and acceptance criteria.

This branch of liboqs can be used with the following Open Quantum Safe application integrations:

- **OpenSSL 1.0.2**: A prototype integration of liboqs-based key exchange  into TLS 1.2 in our fork of OpenSSL 1.0.2; see the [OQS-OpenSSL-1\_0\_2-stable](https://github.com/open-quantum-safe/openssl/tree/OQS-OpenSSL_1_0_2-stable) branch of our OpenSSL fork's repository.
- **OpenSSL 1.1.1**: A prototype integration of liboqs-based key exchange and authentication into TLS 1.3 in our fork of OpenSSL 1.1.1; see the [OQS-OpenSSL-1\_1\_1-stable](https://github.com/open-quantum-safe/openssl/tree/OQS-OpenSSL_1_1_1-stable) branch of our OpenSSL fork's repository.
- **OpenSSH 7.7 portable 1**: A prototype integration of liboqs-based key exchange into SSH in our fork of OpenSSH 7.7; see the [OQS-master](https://github.com/open-quantum-safe/openssh-portable/tree/OQS-master) branch of our OpenSSH fork's repository.

Release notes
=============

This release of liboqs master branch was released on November 13, 2018.  Its release page on GitHub is https://github.com/open-quantum-safe/liboqs/releases/tag/master-0.1.0.

What's New
----------

This is the first release of liboqs master branch.

This branch of liboqs aims to selectively incorporate allegedly quantum-resistant key encapsulation mechanisms and signature schemes, for the purposes of integration into a common API for liboqs-reliant applications.  Implementations on this branch must meet certain acceptance criteria as indicated in README.md.

### Key encapsulation mechanisms

The following KEMs are present in this liboqs master release:

- **BIKE**, based on quasi-cyclic syndrome decoding; 9 parameterizations: `BIKE1-L1`, `BIKE1-L3`, `BIKE1-L5`, `BIKE2-L1`, `BIKE2-L3`, `BIKE2-L5`, `BIKE3-L1`, `BIKE3-L3`, `BIKE3-L5`
- **FrodoKEM**, based on learning with errors; 4 parameterizations: `FrodoKEM-640-AES`, `FrodoKEM-640-cSHAKE`, `FrodoKEM-976-AES`, `FrodoKEM-976-cSHAKE`
- **NewHopeNIST**, based on ring learning with errors; 2 parameterizations: `NewHope-512-CCA-KEM`, `NewHope-1024-CCA-KEM`
- **SIKE** and **SIDH**, based on the supersingular isogeny walk problem; 4 parameterizations: `Sike-p503`, `Sike-p751`, `Sidh-p503`, `Sidh-p751`

### Digital signature schemes

The following signature schemes are present in this liboqs master release:

- **Picnic**: based on hash function and key recovery security of lowMC block cipher; 6 parameterizations: `picnic_L1_FS`, `picnic_L1_UR`, `picnic_L3_FS`, `picnic_L3_UR`, `picnic_L5_FS`, `picnic_L5_UR`
- **qTESLA**, based on ring learning with errors; 3 parameterizations: `qTESLA_I`, `qTESLA_III_size`, `qTESLA_III_speed`

Future work
-----------

Releases of liboqs master branch will be made every 2 to 3 months.  Details about the algorithm lifecycle of master branch can be found in README.md.  Plans for the next release can be found online at https://github.com/open-quantum-safe/liboqs/projects/12.
