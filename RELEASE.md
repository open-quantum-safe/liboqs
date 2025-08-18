liboqs version 0.14.0
=====================

About
-----

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.  More information on OQS can be found on our website: https://openquantumsafe.org/ and on Github at https://github.com/open-quantum-safe/.  

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  Details about liboqs can be found in [README.md](https://github.com/open-quantum-safe/liboqs/blob/main/README.md).  See in particular limitations on intended use.

liboqs can be used with the following Open Quantum Safe application integrations:

- **oqs-provider**: A standalone prototype [OpenSSL 3 provider](https://www.openssl.org/docs/manmaster/man7/provider.html) enabling liboqs-based quantum-safe and hybrid key authentication and exchange for TLS 1.3, X.509 certificate generation and CMS operations.
- **OQS-BoringSSL**: A prototype integration of liboqs-based authentication and key exchange into TLS 1.3 in our fork of BoringSSL; see https://github.com/open-quantum-safe/boringssl.
- **OQS-OpenSSH**: A prototype integration of liboqs-based authentication and key exchange into Secure Shell (SSH) version 2 in our fork of OpenSSH; see https://github.com/open-quantum-safe/openssh.

Several [demos](https://github.com/open-quantum-safe/oqs-demos) are available for using the above libraries in applications, including Apache, Chromium, curl, haproxy, nginx, and Wireshark.

liboqs can also be used in the following programming languages via language-specific wrappers:

- C++, via https://github.com/open-quantum-safe/liboqs-cpp
- Go, via https://github.com/open-quantum-safe/liboqs-go
- Java, via https://github.com/open-quantum-safe/liboqs-java
- Python 3, via https://github.com/open-quantum-safe/liboqs-python
- Rust, via https://github.com/open-quantum-safe/liboqs-rust

Release notes
=============

This is version 0.14.0 of liboqs. It was released on July 10, 2025.

This release contains a security fix for secret-dependent branching in HQC. It introduces support for SNOVA, a NIST Additional Signatures Round 2 candidate, and a new optimized implementation of SHA3 using AVX-512VL instructions. Additionally, this is the first liboqs release to include the [stable 1.0.0 version of PQ Code Package's mlkem-native](https://github.com/pq-code-package/mlkem-native/releases/tag/v1.0.0).

This release also introduces a number of improvements to testing and infrastructure. The OQS project is now publishing benchmarking data on https://openquantumsafe.org/benchmarking and code coverage data on https://coveralls.io/github/open-quantum-safe/liboqs.

Deprecation notice
==================

This will be the last release of liboqs to include Dilithium (that is, the NIST Round 3 version of Dilithium, prior to its standardization by NIST as ML-DSA in FIPS 204). Applications should switch to ML-DSA (FIPS 204). Please contact us if you have any concerns.

Security issues
===============

- CVE-2025-52473: Disabled compiler optimizations for HQC to avoid secret-dependent branches. Thank you to Zhenzhi Lai and Zhiyuan Zhang from from the University of Melbourne and the Max Planck Institute for Security and Privacy for identifying the issue.

What's New
----------

This release continues from the 0.13.0 release of liboqs.

### Key encapsulation mechanisms

- HQC: Disabled compiler optimizations to avoid secret-dependent branching in certain configurations. HQC remains disabled by default.
- ML-KEM: Updated the default ML-KEM implementation to [PQCP's mlkem-native v1.0.0](https://github.com/pq-code-package/mlkem-native/releases/tag/v1.0.0).

### Digital signature schemes

- New API: added an API function to check if a signature scheme supports signing with a context string.
- SNOVA: added [SNOVA](https://snova.pqclab.org/) from NIST Additional Signature Schemes Round 2.

### Other changes

- Added an AVX512VL-optimized backend for SHA3.
- Improved memory management throughout the codebase.

---

Detailed changelog
------------------

## What's Changed
* Switch to dev mode after 0.13.0 release by @praveksharma in https://github.com/open-quantum-safe/liboqs/pull/2125
* Restrict -Wno-maybe-uninitialized to GCC and fix stack size typo by @alraddady in https://github.com/open-quantum-safe/liboqs/pull/2111
* Promote @SWilson4 from Committer to Maintainer [skip ci] by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/2120
* Update Nix flake inputs by @aidenfoxivey in https://github.com/open-quantum-safe/liboqs/pull/2126
* Change cuPQC upstream repo by @praveksharma in https://github.com/open-quantum-safe/liboqs/pull/2115
* Integrate SNOVA into liboqs by @vacuas in https://github.com/open-quantum-safe/liboqs/pull/2109
* Update ACVP vectors to latest release by @abhinav-thales in https://github.com/open-quantum-safe/liboqs/pull/2131
* Add a function to check if context string is supported by @M-AlNoaimi in https://github.com/open-quantum-safe/liboqs/pull/2142
* Skip failing CI test by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/2157
* Use OQS_MEM_cleanse() instead of memset() by @Hussain1811 in https://github.com/open-quantum-safe/liboqs/pull/2158
* Check for NULL dereference before using secure free by @Hussain1811 in https://github.com/open-quantum-safe/liboqs/pull/2151
* Update mlkem-native to v1.0.0 by @mkannwischer in https://github.com/open-quantum-safe/liboqs/pull/2146
* test: Use secure free for freeing secret key objects by @Hussain1811 in https://github.com/open-quantum-safe/liboqs/pull/2149
* tests: Remove unused variables by @Hussain1811 in https://github.com/open-quantum-safe/liboqs/pull/2152
* Wycheproof by @h2parson in https://github.com/open-quantum-safe/liboqs/pull/2145
* tests: Check OQS_STATUS of RNG and fstore functions by @Hussain1811 in https://github.com/open-quantum-safe/liboqs/pull/2153
* Adjust constant-time test exception for mlkem-native by @mkannwischer in https://github.com/open-quantum-safe/liboqs/pull/2162
* Continuous Benchmarking using Github Actions by @pablo-gf in https://github.com/open-quantum-safe/liboqs/pull/2134
* test: Add basic kem fuzz testing by @nathaniel-brough in https://github.com/open-quantum-safe/liboqs/pull/2133
* Increase alert threshold for continuous benchmarking by @pablo-gf in https://github.com/open-quantum-safe/liboqs/pull/2166
* Benchmarking comments only on alerts by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/2168
* Adding code coverage by @aidenfoxivey in https://github.com/open-quantum-safe/liboqs/pull/2148
* Add AVX512VL-Optimized SHA3/SHAKE Implementations by @mdcornu in https://github.com/open-quantum-safe/liboqs/pull/2167
* Zeroize memory in SHA3 implementation by @aidenfoxivey in https://github.com/open-quantum-safe/liboqs/pull/2171
* Disable compiler optimizations for HQC by @SWilson4 in https://github.com/open-quantum-safe/liboqs/commit/4215362acbf69b88fe1777c4c052f154e29f9897
* liboqs 0.14.0 release candidate 1 by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/2180
* Document public / internal API split [skip ci] by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/2182

## New Contributors
* @alraddady made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/2111
* @vacuas made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/2109
* @M-AlNoaimi made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/2142
* @Hussain1811 made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/2158
* @h2parson made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/2145
* @mdcornu made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/2167

**Full Changelog**: https://github.com/open-quantum-safe/liboqs/compare/0.13.0...0.14.0