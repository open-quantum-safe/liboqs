liboqs version 0.9.0
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

This is version 0.9.0 of liboqs. It was released on October 12, 2023.

This release features an update to the Classic McEliece KEM, bringing it in line with NIST Round 4. It also adds or updates ARM implementations for Kyber, Dilithium, and Falcon.

What's New
----------

This release continues from the 0.8.0 release of liboqs.

### Key encapsulation mechanisms

- Classic McEliece: updated to Round 4 version.
- Kyber: aarch64 implementation updated.

### Digital signature schemes

- Dilithium: aarch64 implementation updated.
- Falcon: aarch64 implementation added.

### Other changes

- Update algorithm documentation
- Support compilation for Windows on ARM64, Apple mobile, and Android platforms
- Improve resilience of randombytes on Apple systems

Release call
============

Users of liboqs are invited to join a webinar on Thursday, November 2, 2023, from 12-1pm US Eastern time for information on this release, plans for the next release cycle, and to provide feedback on OQS usage and features.  

The Zoom link for the webinar is: https://uwaterloo.zoom.us/j/98288698086

---

Detailed changelog
------------------

* Fix libdir value in liboqs.pc by @vt-alt in https://github.com/open-quantum-safe/liboqs/pull/1496
* update version and remove CCI triggers by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1498
* create deb package and retain as artifact by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1501
* README correction to docs path & additional gitignore to macos + vscode by @planetf1 in https://github.com/open-quantum-safe/liboqs/pull/1503
* Trigger liboqs-python CI via GitHub API by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1507
* Update Classic McEliece by @praveksharma in https://github.com/open-quantum-safe/liboqs/pull/1470
* update BIKE documentation by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1509
* kyber/dilithium aarch64 pull from pqclean + patches by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1512
* Pull Falcon updates from PQClean by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1523
* Bump XCode by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1526
* Update Classic McEliece supression files by @praveksharma in https://github.com/open-quantum-safe/liboqs/pull/1527
* Bump gitpython from 3.1.30 to 3.1.32 in /scripts/copy_from_upstream by @dependabot in https://github.com/open-quantum-safe/liboqs/pull/1524
* ci: add CI for android by @res0nance in https://github.com/open-quantum-safe/liboqs/pull/1531
* re-enable armhf speed testing by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1535
* Bump gitpython from 3.1.32 to 3.1.34 in /scripts/copy_from_upstream by @dependabot in https://github.com/open-quantum-safe/liboqs/pull/1538
* Prefer arc4random on Apple platforms by @res0nance in https://github.com/open-quantum-safe/liboqs/pull/1544
* Bump gitpython from 3.1.34 to 3.1.35 in /scripts/copy_from_upstream by @dependabot in https://github.com/open-quantum-safe/liboqs/pull/1551
* Update Classic McEliece suppression files by @praveksharma in https://github.com/open-quantum-safe/liboqs/pull/1541
* Pull Neon implementation of Falcon from PQClean by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1547
* ci: add CI for apple mobile platforms by @res0nance in https://github.com/open-quantum-safe/liboqs/pull/1546
* Add Windows ARM64 support by @res0nance in https://github.com/open-quantum-safe/liboqs/pull/1545
* Document Falcon constant time errors by @praveksharma in https://github.com/open-quantum-safe/liboqs/pull/1552
* ci: github actions CI for Windows x86 and x64 by @res0nance in https://github.com/open-quantum-safe/liboqs/pull/1554
* build: Align VS test folder with all other Generators by @res0nance in https://github.com/open-quantum-safe/liboqs/pull/1557
* Fix weekly.yml to skip McEliece by @praveksharma in https://github.com/open-quantum-safe/liboqs/pull/1562
* Enable extensions in constant-time tests by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1567
* Update Classic McEliece supression files by @praveksharma in https://github.com/open-quantum-safe/liboqs/pull/1568
* liboqs 0.9.0 release candidate 1 by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1570
* add community standard documentation [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1565
* Bump gitpython from 3.1.35 to 3.1.37 in /scripts/copy_from_upstream by @dependabot in https://github.com/open-quantum-safe/liboqs/pull/1575

## New Contributors
* @planetf1 made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1503
* @SWilson4 made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1507
* @praveksharma made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1470
* @res0nance made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1531

**Full Changelog**: https://github.com/open-quantum-safe/liboqs/compare/0.8.0...0.9.0
