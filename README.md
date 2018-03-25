liboqs - nist-branch
====================

**This is a development branch that is currently not for use.**

liboqs is a C library for quantum-resistant cryptographic algorithms.  This branch of liboqs focuses on incorporating submissions to the NIST Post-Quantum Cryptography standardization project.

Overview
--------

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  liboqs provides:

- a common API for post-quantum key encapsulation mechanisms and digital signature schemes
- a collection of open source implementations of post-quantum cryptography algorithms
- a test harness and benchmarking routines

The OQS project also provides prototype integrations into application-level protocols to enable testing of quantum-resistant cryptography.  

More information on OQS can be found on our website: [https://openquantumsafe.org/](https://openquantumsafe.org/).

nist-branch
-----------

This branch of liboqs aims to non-selectively incorporate submissions to the NIST Post-Quantum Cryptography project for the purposes of benchmarking and integration into a common API for liboqs-reliant applications.  

This branch takes a "light touch" approach to incorporation:

- Source code from a NIST submission will be included ideally with no changes, in an "upstream" subdirectory.
- A thin wrapper will be written to provide the implementation using the liboqs API.
- The implementation will be added to the build process.
- To avoid namespace collisions between different algorithms, symbol renaming will be used on the compiled files.

Limitations and security
------------------------

liboqs is designed for prototyping and evaluating quantum-resistant cryptography.  Security of proposed quantum-resistant algorithms may rapidly change as research advances, and may ultimately be completely insecure against either classical or quantum computers.  

We believe that the NIST Post-Quantum Cryptography standardization project is currently the best avenue to identifying potentially quantum-resistant algorithms.  liboqs does not intend to "pick winners", and we strongly recommend that applications and protocols rely on the outcomes of the NIST standardization project when deploying post-quantum cryptography.  

We acknowledge that some parties may want to begin deploying post-quantum cryptography prior to the conclusion of the NIST standardization project.  We strongly recommend that any attempts to do make use of so-called **hybrid cryptography**, in which post-quantum public-key algorithms are used alongside traditional public key algorithms (like RSA or elliptic curves) so that the solution is at least no less secure than existing traditional cryptography.

liboqs is provided "as is", without warranty of any kind.  See [LICENSE.txt](https://github.com/open-quantum-safe/liboqs/blob/ds-nist-branch/LICENSE.txt) for the full disclaimer.

In addition, implementations that we have included on this nist-branch of liboqs have received no quality control or vetting by OQS.  **THE NIST-BRANCH OF LIBOQS SHOULD BE USED EXCLUSIVELY FOR EXPERIMENTATION AND PROTOTYPING, AND SHOULD NEVER BE USED IN ANY PRODUCTION ENVIRONMENT OR TO PROTECT ANY SENSITIVE DATA.**

Acceptance criteria for nist-branch
-----------------------------------

- **Algorithmic requirements:**
	- Any algorithm accepted as a full and complete submission to round 1 of the NIST Post-Quantum Cryptography project is eligible for inclusion.
	- Algorithms whose security is considered effectively broken are not eligible for addition; see the Lifecycle section below for conditions on their removal.
	- KEMs can be IND-CPA or IND-CCA-secure, at any NIST security level.
	- Signature schemes can be EUF-CMA-secure, at any NIST security level.
- **Targets**:
	- **Operating systems:** The code must build on Linux and macOS.
	- **Architecture:** The code must build at least on x64.  Targets are currently provided for x86.  We plan to add an AVX2 target, and possibly others.
- **Source code requirements:**
	- The source code can be from the original submission, or can be an updated version.
	- **License:** Source code licensed under the MIT License, the BSD license, or in the public domain can be directly incorporated into the repository.  GPL code will not be included in the repository, but a wrapper to the OQS API may be included, as well as a script that downloads and compiles in GPL code if the algorithm is requested at compile-time.
	- **Code quality:** Given the "light touch" philosophy of nist-branch, we have no requirements on source code quality, other than that it compile on the targets.

Contributing
------------

*TODO*

Lifecycle for nist-branch
-------------------------

**Release cycle:** We aim to tag monthly snapshot of nist-branch, released around the end of each month.  Plans for each individual snapshot release can be found on our [Github projects board](https://github.com/open-quantum-safe/liboqs/projects/).

**Algorithm deprecation:** If an algorithm in nist-branch is found to be insecure in month $X$, a compile-time warning will be added in the tagged snapshot for month $X+1$, and it may be removed in month $X+2$.

**Algorithm compatibility:** Algorithm implementations on nist-branch may be updated at any time.  This may cause changes in runtime or even a change in the message generation and processing.  Thus, no guarantees are made for compatibility of messages exchange between different snapshots of nist-branch.

**API stability:** The public API of liboqs nist-branch is considered to be the functions in `oqs/common.h`, `oqs/config.h`, `oqs/kem.h`, and `oqs/rand.h`.  We intend to maintain this API for the foreseeable future.

**Binary compatibility:** *TODO*

Building and running liboqs nist-branch
---------------------------------------

[Build status using Travis continuous integration system:](https://travis-ci.org/open-quantum-safe/liboqs/branches) ![Build status image](https://travis-ci.org/open-quantum-safe/liboqs.svg?branch=ds-nist-branch)

*TODO*

License
-------

liboqs is licensed under the MIT License; see [LICENSE.txt](https://github.com/open-quantum-safe/liboqs/blob/ds-nist-branch/LICENSE.txt) for details.  

liboqs includes some third party libraries or modules that may be licensed differently.  All third-party code is contained in directories labelled `upstream`, and each such directory contains a license file indicating the license that applies to code in that directory.

Contributors
------------

*TODO*
