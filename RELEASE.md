liboqs version 0.11.0
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

This is version 0.11.0 of liboqs. It was released on September 27, 2024.

This release updates ML-KEM implementations to their [final FIPS 203](https://csrc.nist.gov/pubs/fips/203/final) versions. This release still includes the NIST Round 3 version of Kyber for interoperability purposes, but we plan to remove Kyber Round 3 in a future release. Additionally, this release adds support for MAYO and CROSS digital signature schemes from [NIST Additional Signatures Round 1](https://csrc.nist.gov/Projects/pqc-dig-sig/round-1-additional-signatures) along with stateful hash-based signature schemes [XMSS](https://datatracker.ietf.org/doc/html/rfc8391) and [LMS](https://datatracker.ietf.org/doc/html/rfc8554). Finally, this release provides formally verified implementations of Kyber-512 and Kyber-768 from [libjade](https://github.com/formosa-crypto/libjade/releases/tag/release%2F2023.05-2).

LMS and XMSS are disabled by default due to the security risks associated with their use in software. See the note on stateful hash-based signatures in [CONFIGURE.md](https://github.com/open-quantum-safe/liboqs/blob/0.11.0/CONFIGURE.md#stateful-hash-based-signatures).

What's New
----------

This release continues from the 0.10.1 release of liboqs.

### Key encapsulation mechanisms

- Kyber: Added formally-verified portable C and AVX2 implementations of Kyber-512 and Kyber-768 from [libjade](https://github.com/formosa-crypto/libjade/releases/tag/release%2F2023.05-2).
- ML-KEM: Updated portable C and AVX2 implementations of ML-KEM-512, ML-KEM-768, and ML-KEM-1024 to FIP 203 version.
- Kyber: Patched ARM64 implementations of Kyber-512, Kyber-768, and Kyber-1024 to work with AddressSanitizer.

### Digital signature schemes

- LMS/XMSS: Added implementations of stateful hash-based signature schemes: [XMSS](https://datatracker.ietf.org/doc/html/rfc8391) and [LMS](https://datatracker.ietf.org/doc/html/rfc8554).
- MAYO: Added portable C and AVX2 implementations of MAYO signature scheme from NIST Additional Signatures Round 1.
- CROSS: Added portable C and AVX2 implementations of CROSS signature scheme from NIST Additional Signatures Round 1.

### Other changes

- Added callback API to use custom implementations of AES, SHA2, and SHA3.
- Refactor SHA3 implementation to use OpenSSL's EVP_DigestSqueeze() API.

---

Detailed changelog
------------------

* [NFCI] Move Keccak rhotates tables to rodata by @aaupov in https://github.com/open-quantum-safe/liboqs/pull/1739
* Document Fix by @pi-314159 in https://github.com/open-quantum-safe/liboqs/pull/1735
* Add option to dynamically load libcrypto.so.* by @ueno in https://github.com/open-quantum-safe/liboqs/pull/1603
* Allow windows linking of test programs by @matlimatli in https://github.com/open-quantum-safe/liboqs/pull/1751
* Refactor OpenSSL Implementation of SHA3 SHAKE to use new Squeeze API by @Eddy-M-K in https://github.com/open-quantum-safe/liboqs/pull/1694
* remove "maximum" words for most length fields by @wangweij in https://github.com/open-quantum-safe/liboqs/pull/1747
* add compile_commands.json to .gitignore by @carsonRadtke in https://github.com/open-quantum-safe/liboqs/pull/1754
* Fix linking of test programs on msys by @d0p1s4m4 in https://github.com/open-quantum-safe/liboqs/pull/1758
* restrict Windows platform support documentation [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1762
* Add workflow dispatch to action by @ryjones in https://github.com/open-quantum-safe/liboqs/pull/1778
* Bump jinja2 from 3.1.3 to 3.1.4 in /scripts/copy_from_upstream by @dependabot in https://github.com/open-quantum-safe/liboqs/pull/1782
* Algorithm selection clarification by @beldmit in https://github.com/open-quantum-safe/liboqs/pull/1784
* Use OPENSSL_cleanse if OpenSSL is used by @bencemali in https://github.com/open-quantum-safe/liboqs/pull/1773
* Errors not printed out when OPENSSL_NO_STDIO is set by @bencemali in https://github.com/open-quantum-safe/liboqs/pull/1774
* Add Stateful Signature (XMSS and LMS) by @ashman-p in https://github.com/open-quantum-safe/liboqs/pull/1650
* Forward-declare OQS_SIG in sig_stfl.h by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1820
* Move Linux ARM64 "build" test from CircleCI to GitHub Actions by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1814
* Fix test_alg_info.py on Windows platform by @qnfm in https://github.com/open-quantum-safe/liboqs/pull/1821
* Increment version string to 0.10.2-dev by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1813
* Add XMSS-SHA256_{10, 16, 20}_192 parameters by @cothan in https://github.com/open-quantum-safe/liboqs/pull/1817
* Add XMSS-SHAKE256_{10, 16, 20}_192 parameters by @cothan in https://github.com/open-quantum-safe/liboqs/pull/1818
* Add XMSS-SHAKE256_{10, 16, 20}_256 parameters by @cothan in https://github.com/open-quantum-safe/liboqs/pull/1819
* Create scorecard.yml (OpenSSF) by @planetf1 in https://github.com/open-quantum-safe/liboqs/pull/1708
* Expose callback API for replacing low-level cryptographic primitives by @ueno in https://github.com/open-quantum-safe/liboqs/pull/1832
* Add MAYO signature scheme from NIST onramp by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1707
* Bump zipp from 3.4.0 to 3.19.1 in /scripts/copy_from_upstream in the pip group by @dependabot in https://github.com/open-quantum-safe/liboqs/pull/1836
* Update and fix CI status badges by @anvega in https://github.com/open-quantum-safe/liboqs/pull/1844
* Use `cmake -LA -N` instead of `cmake -LA` in CI by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1848
* Fix passes.json entries for MAYO by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1852
* ML-KEM NIST tests, fix order of d and z by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1854
* Move from CircleCI to GitHub Actions by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1849
* Add a convenience script for consistent astyle formatting by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1861
* Quick fixes from Trail of Bits audit Week 1 by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1869
* Check return value of fscanf in LMS/XMSS KAT tests by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1874
* Fix downstream CI trigger by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1857
* Don't hardcode OPENSSL_ROOT_DIR to /usr on Linux by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1873
* Fix overflow in stateful sigs tests by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1887
* Integrate Kyber from libjade  by @praveksharma in https://github.com/open-quantum-safe/liboqs/pull/1745
* Use explicit_memset if available. NetBSD has support for it: by @loganaden in https://github.com/open-quantum-safe/liboqs/pull/1872
* Disable erroring TravisCI build by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1901
* Update OpenSSH downstream branch to OQS-v9 by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1898
* Fix incorrect formatting in unix.yml by @praveksharma in https://github.com/open-quantum-safe/liboqs/pull/1902
* CMakeLists: add ppc case to known archs by @barracuda156 in https://github.com/open-quantum-safe/liboqs/pull/1816
* Remove old ad hoc CI for Apple M1 by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1907
* Add ML-KEM / FIPS203 final by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1899
* Update checkout action in weekly.yml by @praveksharma in https://github.com/open-quantum-safe/liboqs/pull/1908
* Add CROSS by @rtjk in https://github.com/open-quantum-safe/liboqs/pull/1881
* Refactor liboqs CI and update Ubuntu images by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1909
* Check workflows for issues during CI by @jplomas in https://github.com/open-quantum-safe/liboqs/pull/1916
* Patch Kyber to fix ASAN error on ARM64 by @praveksharma in https://github.com/open-quantum-safe/liboqs/pull/1922
* Change README links to be doxygen-friendly by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1927

## New Contributors
* @aaupov made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1739
* @pi-314159 made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1735
* @ueno made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1603
* @matlimatli made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1751
* @Eddy-M-K made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1694
* @wangweij made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1747
* @carsonRadtke made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1754
* @d0p1s4m4 made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1758
* @ryjones made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1778
* @bencemali made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1773
* @qnfm made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1821
* @anvega made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1844
* @loganaden made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1872
* @barracuda156 made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1816
* @rtjk made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1881
* @jplomas made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1916

**Full Changelog**: https://github.com/open-quantum-safe/liboqs/compare/0.10.1...0.11.0