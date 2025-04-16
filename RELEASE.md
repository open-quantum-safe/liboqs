liboqs version 0.13.0
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

This is version 0.13.0 of liboqs. It was released on April 16, 2025.

This release improves support for NIST Additional Signatures Round 2 candidates: CROSS and MAYO implementations are updated and support is added for UOV. This release also adds a new KEM API for deterministic key generation (only supported by ML-KEM at the moment). Finally, this release adds support for ML-KEM implementations from 2 new sources: formally verified portable C, AVX2, and AArch64 implementations from [PQCP's mlkem-native](https://github.com/pq-code-package/mlkem-native) and a GPU accelerated CUDA implementation from [Nvidia cuPQC](https://developer.nvidia.com/cupqc). 

OQS is running a survey to better understand our community. We would like to hear from organizations and individuals about their interest in and use of the Open Quantum Safe project. Please take a few minutes to fill out the survey: https://linuxfoundation.surveymonkey.com/r/oqssurvey

What's New
----------

This release continues from the 0.12.0 release of liboqs.

### Key encapsulation mechanisms

- New API: Added a deterministic key generation and API for KEMs (only ML-KEM supported at the moment).
- ML-KEM: Changed the default ML-KEM implementation to [PQCP's mlkem-native](https://github.com/pq-code-package/mlkem-native). There are three variants: Portable C, AVX2, and AArch64. Large parts of these implementations are formally verified: all of the C code is verified for memory and type safety using [CBMC](https://github.com/diffblue/cbmc) and the functional correctness of the core AArch64 assembly routines is verified using [HOL-Light](https://github.com/jrh13/hol-light). 
- ML-KEM: Added support for the ML-KEM implementation from [Nvidia cuPQC](https://developer.nvidia.com/cupqc), a GPU accelerated cryptography library.
- ML-KEM: Implementation from mlkem-native upstream updated to add Pair-wise Consistency Test (PCT) and Intel CET support.
- ML-KEM: Improved testing of ML-KEM keys.
- HQC: Disabled HQC by default until [a new security flaw](https://groups.google.com/a/list.nist.gov/g/pqc-forum/c/Wiu4ZQo3fP8) is fixed.

### Digital signature schemes

- ML-DSA: Improved testing for ML-DSA.
- CROSS: Updated to NIST Additional Signatures Round 2 version.
- MAYO: Updated to NIST Additional Signatures Round 2 version.
- UOV: Added support for UOV algorithm from NIST Additional Signatures Round 2.

### Other changes

- Added support for loongarch64 architecture.

---

Detailed changelog
------------------

## What's Changed
* Bump version to 0.12.1-dev by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/2015
* Add loongarch64 support by @zhaixiaojuan in https://github.com/open-quantum-safe/liboqs/pull/2010
* Minor changes to ML_DSA ACVP tests by @abhinav-thales in https://github.com/open-quantum-safe/liboqs/pull/2007
* Update upload-artifact action to v4 by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/2017
* Remove hardcoded build paths & modify basic workflow to build in random path by @iyanmv in https://github.com/open-quantum-safe/liboqs/pull/2019
* Trigger liboqs-java and liboqs-rust downstream CI by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/2021
* #1830 update scorecard to v5 (gh action 2.4.0) by @planetf1 in https://github.com/open-quantum-safe/liboqs/pull/1890
* Update PQClean commit and delete patch for HQC by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/2026
* Bump jinja2 from 3.1.4 to 3.1.5 in /scripts/copy_from_upstream in the pip group by @dependabot in https://github.com/open-quantum-safe/liboqs/pull/2036
* Avoid unresolved symbols from libcrypto when compiled with OQS_DLOPEN_OPENSSL by @ueno in https://github.com/open-quantum-safe/liboqs/pull/2043
* Update to public Ubuntu 24.04 ARM runner by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/2050
* NVIDIA: Adding cuPQC as a backend for ML-KEM. by @stevenireeves in https://github.com/open-quantum-safe/liboqs/pull/2044
* Update ACVP vectors for KEM and DSA by @abhinav-thales in https://github.com/open-quantum-safe/liboqs/pull/2051
* CI: Check unresolved symbols when compiled with OQS_DLOPEN_OPENSSL by @ueno in https://github.com/open-quantum-safe/liboqs/pull/2058
* Fix failing zephyr CI workflows, pinning v0.27.4 by @bhess in https://github.com/open-quantum-safe/liboqs/pull/2063
* Update sig_stfl Doxygen documentation by @pablo-gf in https://github.com/open-quantum-safe/liboqs/pull/2059
* Import ML-KEM from mlkem-native/PQ code package by @bhess in https://github.com/open-quantum-safe/liboqs/pull/2041
* Update example files by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/2071
* GitHub runner updates by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/2069
* Disable cupqc-buildcheck by @praveksharma in https://github.com/open-quantum-safe/liboqs/pull/2075
* Add threat model by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/2033
* Update CROSS to version 2.0 by @rtjk in https://github.com/open-quantum-safe/liboqs/pull/2078
* improving CONTRIBUTING.md for maintainability [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/2081
* Ensure that building against liboqs build directory works by @levitte in https://github.com/open-quantum-safe/liboqs/pull/2086
* Added alg_version details to test output by @pablo-gf in https://github.com/open-quantum-safe/liboqs/pull/2080
* Add checks for ML-KEM keys by @abhinav-thales in https://github.com/open-quantum-safe/liboqs/pull/2009
* Update actions/cache to v4.2.2 by @mkannwischer in https://github.com/open-quantum-safe/liboqs/pull/2093
* Add Nix flake by @aidenfoxivey in https://github.com/open-quantum-safe/liboqs/pull/1970
* Update MAYO to NIST round 2 by @bhess in https://github.com/open-quantum-safe/liboqs/pull/2095
* Update mlkem-native to v1.0.0-beta by @mkannwischer in https://github.com/open-quantum-safe/liboqs/pull/2092
* Add references to security response process by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/2077
* Bump version to 0.13.0-dev [skip ci] by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/2099
* Add UOV  by @mkannwischer in https://github.com/open-quantum-safe/liboqs/pull/2094
* Add bitflip test for trivial SUF-CMA forgeries by @rtjk in https://github.com/open-quantum-safe/liboqs/pull/2090
* Update MAYO version in algorithm datasheet by @bhess in https://github.com/open-quantum-safe/liboqs/pull/2103
* Add DeriveKeyPair API by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/2070
* Update nist-round in UOV and MAYO data sheet by @bhess in https://github.com/open-quantum-safe/liboqs/pull/2105
* build: search unistd.h separately from sys/random.h for getentropy by @mkroening in https://github.com/open-quantum-safe/liboqs/pull/2104
* Add support caveat by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/2114
* Temporarily disable HQC by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/2122
* Fix PR workflow runs by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/2123

## New Contributors
* @zhaixiaojuan made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/2010
* @stevenireeves made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/2044
* @pablo-gf made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/2059
* @levitte made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/2086
* @mkannwischer made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/2093
* @mkroening made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/2104

**Full Changelog**: https://github.com/open-quantum-safe/liboqs/compare/0.12.0...0.13.0