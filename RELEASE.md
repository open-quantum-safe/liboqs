liboqs version 0.12.0
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

This is version 0.12.0 of liboqs. It was released on December 9, 2024.

This release updates the ML-DSA implementation to the [final FIPS 204](https://csrc.nist.gov/pubs/fips/204/final) version. This release still includes the NIST Round 3 version of Dilithium for interoperability purposes, but we plan to remove Dilithium Round 3 in a future release.

Deprecation notice
==================

This will be the last release of liboqs to include Kyber (that is, the NIST Round 3 version of Kyber, prior to its standardization by NIST as ML-KEM in FIPS 203). Applications should switch to ML-KEM (FIPS 203).

The addition of ML-DSA FIPS 204 final version to liboqs has introduced a new signature API which includes a context string parameter. We are planning to remove the old version of the API without a context string in the next release to streamline the API and bring it in line with NIST specifications. Users who have an opinion on this removal are invited to provide input at https://github.com/open-quantum-safe/liboqs/issues/2001.

Security issues
===============

- CVE-2024-54137: Fixed bug in HQC decapsulation that leads to incorrect shared secret value during decapsulation when called with an invalid ciphertext. Thank you to Célian Glénaz and Dahmun Goudarzi from Quarkslab for identifying the issue.

What's New
----------

This release continues from the 0.11.0 release of liboqs.

### Key encapsulation mechanisms

- HQC: Fixed bug in decapsulation that leads to incorrect shared secret value during decapsulation when called with an invalid ciphertext. Thank you to Célian Glénaz and Dahmun Goudarzi from Quarkslab for identifying the issue.
- Kyber: This is the last release of liboqs to include Kyber.
- ML-KEM: Improved testing of ML-KEM.

### Digital signature schemes

- LMS: Fixed crashing bug.
- ML-DSA: Removed FIPS 204-ipd (initial public draft) and replaced it with FIPS 204 final version.
- Added new API for digital signatures with context strings; see https://github.com/open-quantum-safe/liboqs/issues/2001 for plan to remove old API without context string.
- Added fuzzing tests for signature schemes.
- Added benchmarking for stateful hash-based signature schemes.

### Other changes

- Updated CBOM format to version 1.6.
- Added a function `OQS_thread_stop` to be called by multi-threaded applications to properly deallocate resources in a threaded execution.
- Added preprocessor macros conveying liboqs version information.

---

Detailed changelog
------------------

## What's Changed

* 0.11.0 release by @praveksharma in https://github.com/open-quantum-safe/liboqs/pull/1939
* Bump version to 0.11.1-dev by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1940
* Remove hardcoded build patch from test script by @iyanmv in https://github.com/open-quantum-safe/liboqs/pull/1938
* Don't include dlfcn.h for Windows by @steenrasmussen in https://github.com/open-quantum-safe/liboqs/pull/1936
* Update CBOM format to upstream v1.6 by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1834
* Downgrade zephyr container to v0.26.14 to avoid build failures by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1949
* Fix for Zephyr CI by @Frauschi in https://github.com/open-quantum-safe/liboqs/pull/1953
* Add a basic fuzz testing harness for Dilithium2 by @nathaniel-brough in https://github.com/open-quantum-safe/liboqs/pull/1905
* [#1823] replace malloc/calloc/strdup/free with openssl allocator by @songlingatpan in https://github.com/open-quantum-safe/liboqs/pull/1926
* Add benchmarking for stateful hash based schemes: speed_sig_stfl by @cr-marcstevens in https://github.com/open-quantum-safe/liboqs/pull/1952
* Update CODEOWNERS by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1943
* Add new API to cleanup OpenSSL threads. by @ashman-p in https://github.com/open-quantum-safe/liboqs/pull/1959
* Adapt existing sig fuzz harness including more algorithms by @nathaniel-brough in https://github.com/open-quantum-safe/liboqs/pull/1955
* add C++ linking test by @aidenfoxivey in https://github.com/open-quantum-safe/liboqs/pull/1971
* Make random/functions deterministic during fuzzing by @nathaniel-brough in https://github.com/open-quantum-safe/liboqs/pull/1974
* Remove SPHINCS+ aarch64 code by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1972
* Remove macos-12 runner due to GitHub deprecation. by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1977
* Revert "Disable erroring TravisCI build" by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1960
* imported fix from CROSS upstream: endianness-aware csprng by @rtjk in https://github.com/open-quantum-safe/liboqs/pull/1983
* chore: Add CI badges to README.md by @ChinoUkaegbu in https://github.com/open-quantum-safe/liboqs/pull/1987
* Update PLATFORMS.md / re-enable CROSS on s390x by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1988
* Avoid OpenSSL functions are unconditionally called at OQS_destroy by @ueno in https://github.com/open-quantum-safe/liboqs/pull/1982
* Test Improvements for ML-KEM by @abhinav-thales in https://github.com/open-quantum-safe/liboqs/pull/1947
* Add patch to fix HQC decapsulation in https://github.com/open-quantum-safe/liboqs/commit/95f904bcaafe4cb12cfdf029347ac3b7c795308a
* Run copy_from_upstream.py in https://github.com/open-quantum-safe/liboqs/commit/cce1bfde4e52c524b087b9687020d283fbde0f24
* Fix LMS crash by @ashman-p in https://github.com/open-quantum-safe/liboqs/pull/1998
* Set ML-KEM alg_version to "FIPS203" by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1997
* Add ML-DSA / FIPS 204 final by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1919
* Add defines for OQS version components by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/2000
* Create liboqs 0.12.0 release candidate 1 by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/2006

## New Contributors

* @steenrasmussen made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1936
* @nathaniel-brough made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1905
* @songlingatpan made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1926
* @cr-marcstevens made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1952
* @aidenfoxivey made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1971
* @ChinoUkaegbu made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1987
* @abhinav-thales made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1947

**Full Changelog**: https://github.com/open-quantum-safe/liboqs/compare/0.11.0...0.12.0
