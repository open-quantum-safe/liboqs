liboqs version 0.10.0
=====================

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

This is version 0.10.0 of liboqs. It was released on March 20, 2024.

This release adds support for ML-KEM (previously known as CRYSTALS-Kyber) and ML-DSA (previously known as CRYSTALS-Dilithium), based on the initial public drafts of [FIPS 203](https://csrc.nist.gov/pubs/fips/203/ipd) and [FIPS 204](https://csrc.nist.gov/pubs/fips/204/ipd), respectively. OQS continues to support the NIST Round 3 versions of Kyber and Dilithium for interoperability purposes. This release additionally updates HQC to the NIST Round 4 version and adds support for fixed-length Falcon signatures.

What's New
----------

This release continues from the 0.9.2 release of liboqs.

### Key encapsulation mechanisms

- BIKE: Updated portable C implementation to include constant-time fixes from upstream.
- HQC: Updated to NIST Round 4 version.
- ML-KEM: Added portable C and AVX2 implementations of Initial Public Draft (IPD) versions of ML-KEM-512, ML-KEM-768, and ML-KEM-1024.

### Digital signature schemes

- Falcon: Updated portable C, AVX2, and AArch64 implementations to support fixed-length (PADDED-format) signatures. Fixed the maximum length of variable-length signatures to comply with the NIST Round 3 specification.
- ML-DSA: Added portable C and AVX2 implementations of Initial Public Draft (IPD) versions of ML-DSA-44, ML-DSA-65, and ML-DSA-87.

### Other changes

- Improved thread safety.
- Added uninstall support via `ninja uninstall`
- Documented platforms by support tier in PLATFORMS.md.
- Added support for Zephyr RTOS.
- Improved support for macOS on Apple Silicon.
- Removed support for the "NIST-KAT" DRBG.
- Added extended KAT test programs.

---

Detailed changelog
------------------

* PR template update & OpenSSL clarification by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1582
* Use CMAKE_USE_PTHREADS_INIT by @zxjtan in https://github.com/open-quantum-safe/liboqs/pull/1576
* Add section to CONFIGURE.md link by @iyanmv in https://github.com/open-quantum-safe/liboqs/pull/1578
* Run copy_from_upstream and test by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1589
* Support several pqclean upstream versions by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1595
* Call Keccak_(X4_)Dispatch with pthread_once by @zxjtan in https://github.com/open-quantum-safe/liboqs/pull/1549
* minor updates by @vsoftco in https://github.com/open-quantum-safe/liboqs/pull/1600
* Pull new HQC implementation from upstream by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1585
* add uninstall support by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1604
* Ensure generic OQS_OPT_TARGET in weekly CT tests by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1618
* update .travis.yml by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1629
* Pull latest Kyber version from upstream by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1631
* platform support documentation [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1605
* Add support for Zephyr RTOS by @Frauschi in https://github.com/open-quantum-safe/liboqs/pull/1621
* Apply patch to Kyber aarch64 code from PQClean for variable-time division issue. by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1636
* Fix BIKE constant-time errors by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1632
* Fix falcon constant time check in Valgrind by @cothan in https://github.com/open-quantum-safe/liboqs/pull/1646
* Correct cmake version requirement by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1643
* Pull Kyber division fixes from PQ-Crystals into main by @praveksharma in https://github.com/open-quantum-safe/liboqs/pull/1649
* Bump gitpython from 3.1.37 to 3.1.41 in /scripts/copy_from_upstream by @dependabot in https://github.com/open-quantum-safe/liboqs/pull/1659
* Zephyr: fixes for platform support by @Frauschi in https://github.com/open-quantum-safe/liboqs/pull/1658
* Bump jinja2 from 2.11.3 to 3.1.3 in /scripts/copy_from_upstream by @dependabot in https://github.com/open-quantum-safe/liboqs/pull/1661
* Riscv zephyr support by @trigpolynom in https://github.com/open-quantum-safe/liboqs/pull/1641
* Zephyr: CMake fixes by @Frauschi in https://github.com/open-quantum-safe/liboqs/pull/1664
* Clarify that copyright is held by authors and not the project itself [skip ci] by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1668
* Make internal API available to (only) test programs by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1667
* Remove reference to old BIKE variants from CONFIGURE.md [skip ci] by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1669
* Add a document describing our subproject governance by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1675
* Set the correct compile flag for the memory sanitizer build by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1680
* Test against all 100 KAT values by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1560
* Update BIKE documentation to exclude x86 by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1679
* find_package(Threads) regardless of BUILD_ONLY_LIB by @zxjtan in https://github.com/open-quantum-safe/liboqs/pull/1653
* Call set_available_cpu_extensions using pthread_once by @zxjtan in https://github.com/open-quantum-safe/liboqs/pull/1671
* Discontinue AppVeyor CI testing by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1682
* Run oqs-provider release tests in CI on release candidate branches by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1654
* Fix link in GOVERNANCE.md by @Martyrshot in https://github.com/open-quantum-safe/liboqs/pull/1686
* Rename weekly runs and skip Falcon-1024 [skip ci] by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1684
* Update McEliece suppression files for generic config by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1677
* Update SPHINCS+ "clean" suppression files by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1683
* Update Sphincs+ Markdown documentation from YAML by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1690
* properly document release support level [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1688
* set(OQS_USE_PTHREADS OFF) on MinGW/Cygwin by @zxjtan in https://github.com/open-quantum-safe/liboqs/pull/1695
* Fix cross compilation and test in CI by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1696
* update brew install instructions to use openssl@3 instead of openssl@1.1.1 [skip ci] by @Martyrshot in https://github.com/open-quantum-safe/liboqs/pull/1701
* Add ML-DSA-ipd and ML-KEM-ipd & NIST supplied test vectors by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1626
* Small fixes after adding ML-\* by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1702
* Move MacOS CI tests to GitHub Actions; add M1 CI tests by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1709
* Update liboqs readme to point to oqs-provider instead of deprecated openssl1.1.1 fork [skip ci] by @Martyrshot in https://github.com/open-quantum-safe/liboqs/pull/1699
* Fix for the Zephyr CI tests by @Frauschi in https://github.com/open-quantum-safe/liboqs/pull/1714
* remove references to unsupported openssh [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1713
* fix documentation generation by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1715
* Support Falcon PADDED format by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1710
* Fix for alg_support.cmake by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1716
* Fix SPHINCS+ naming in CT tests [skip ci] by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1720
* improve algorithm documentation [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1721
* Always build "internal" library as static by @SWilson4 in https://github.com/open-quantum-safe/liboqs/pull/1725

## New Contributors
* @zxjtan made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1576
* @iyanmv made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1578
* @Frauschi made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1621
* @cothan made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1646
* @trigpolynom made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1641

**Full Changelog**: https://github.com/open-quantum-safe/liboqs/compare/0.9.2...0.10.0
