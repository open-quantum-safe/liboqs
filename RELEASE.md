liboqs version 0.8.0
====================

About
-----

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.  More information on OQS can be found on our website: https://openquantumsafe.org/ and on Github at https://github.com/open-quantum-safe/.  

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  Details about liboqs can be found in [README.md](https://github.com/open-quantum-safe/liboqs/blob/main/README.md).  See in particular limitations on intended use.

liboqs can be used with the following Open Quantum Safe application integrations:

- **OQS-OpenSSL 1.1.1**: A prototype integration of liboqs-based authentication and key exchange into TLS 1.3 in our fork of OpenSSL 1.1.1; see the [OQS-OpenSSL-1\_1\_1-stable](https://github.com/open-quantum-safe/openssl/tree/OQS-OpenSSL_1_1_1-stable) branch of our OpenSSL fork's repository.
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

This is version 0.8.0 of liboqs.  It was released on June 7, 2023.

What's New
----------

This release continues from the 0.7.2 release of liboqs. 

This release features many algorithm updates, including removal of algorithms and variants no longer proceeding through NIST standardization and updates to newer versions.  See the detailed list of algorithm updates below.

### Key encapsulation mechanisms

- BIKE: updated to Round 4 version.
- Kyber: 90s variants were removed.
- NTRU Prime: All variants were removed, except for sntrup761.
- Saber: removed.

### Digital signature schemes

- Dilithium; AES variants were removed.
- Falcon: updated to the 2023-02-07 version.
- Picnic: removed.
- Rainbow: removed.
- SPHINCS+: updated to version 3.1; SPHINCS+-Haraka variants were removed; SPHINCS+-SHA256 and SPHINCS+-SHAKE variants were renamed

### Other changes

- Add Cryptographic Bill of Materials (CBOM)
- Improve building on ARM platforms
- Improve performance when using OpenSSL 3 for symmetric cryptography
- Increment shared object library version
- New configure-time options for algorithm selections
- pkgconfig file now available

---

Detailed changelog
------------------

* Add missing requirements to the requirements.txt by @thb-sb in https://github.com/open-quantum-safe/liboqs/pull/1295
* Solve '-Wstrict-prototypes' for clang >= 15.0 by @thb-sb in https://github.com/open-quantum-safe/liboqs/pull/1293
* Ensure build without an executable stack (fixes #1285) by @sebastinas in https://github.com/open-quantum-safe/liboqs/pull/1294
* Fix typo in Picnic's NEON detection by @sebastinas in https://github.com/open-quantum-safe/liboqs/pull/1298
* ARM32 gcc12 build workaround by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1297
* Fallback code for `aligned_alloc` and use of `explicit_bzero` by @sebastinas in https://github.com/open-quantum-safe/liboqs/pull/1300
* update version string indicating dev status by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1305
* addressing sig length questions by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1306
* Integer overflow leading to incorrect SHA3 computation by @jschanck in https://github.com/open-quantum-safe/liboqs/pull/1312
* Fixing OQS ARM inconsistencies by @Martyrshot in https://github.com/open-quantum-safe/liboqs/pull/1307
* automatically activate USE_RASPBERRY_PI define by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1313
* update Kyber and Dilithium from upstream by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1316
* Remove rainbow by @xvzcf in https://github.com/open-quantum-safe/liboqs/pull/1321
* Removed Picnic signature scheme. by @xvzcf in https://github.com/open-quantum-safe/liboqs/pull/1323
* Removed NTRU-Prime. by @xvzcf in https://github.com/open-quantum-safe/liboqs/pull/1325
* Removed SABER. by @xvzcf in https://github.com/open-quantum-safe/liboqs/pull/1326
* add valgrind option by @malbert1 in https://github.com/open-quantum-safe/liboqs/pull/1327
* Removed NTRU. by @xvzcf in https://github.com/open-quantum-safe/liboqs/pull/1335
* Add ntruprime by @ryndia in https://github.com/open-quantum-safe/liboqs/pull/1328
* fix: initialize context after reset in ossl_sha3x4 by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1339
* Enable algorithm filtering by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1333
* Revert "Enable algorithm filtering (#1333)" by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1351
* llvm15 update by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1350
* Adds CBOM for liboqs by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1337
* Fix Doxygen Markdown failures by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1349
* Build dump_alg_info in tests by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1353
* Build Doxygen docs in whatever the CMake build directory is by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1357
* NIST std algs list selection enablement by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1355
* Config update by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1361
* add cpack (for .deb packages) by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1362
* Updated PQClean commit in `copy_from_upstream.yml` by @xvzcf in https://github.com/open-quantum-safe/liboqs/pull/1359
* Bump gitpython from 3.0.7 to 3.1.30 in /scripts/copy_from_upstream by @dependabot in https://github.com/open-quantum-safe/liboqs/pull/1354
* Fixed mismatch between YAML and markdown docs for some algorithms. by @xvzcf in https://github.com/open-quantum-safe/liboqs/pull/1365
* adding OpenSSL3 test; activating sanitizer test by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1363
* re-enabling msys2 testing after picnic is gone by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1373
* Use OQS_STATUS types in FrodoKEM by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1377
* compiler future-proofing Release builds by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1378
* BIKE Round-4 update by @dkostic in https://github.com/open-quantum-safe/liboqs/pull/1369
* Fix rendering error in Markdown by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1384
* Update Falcon to 20230207  by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1386
* Revert "Update Falcon to 20230207 (PQClean commit 96dfee95cc56207d1ec… by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1392
* Add full-cycle speed test by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1391
* update BIKE documentation by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1387
* correct free in test_kem/sig by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1399
* Copy from upstream (Kyber), add pqcrystals-* licenses to README by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1403
* Update Falcon implementation by @thomwiggers in https://github.com/open-quantum-safe/liboqs/pull/1395
* adding issue template [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1410
* Copy_from_upstream: no subprocess call & update_cbom fix for CI. by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1412
* CI test copy_from_upstream by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1405
* Fix constant time failure for Falcon AVX2 by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1415
* clarify OpenSSL config [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1429
* Make BIKE decode function void to avoid ct issues by @dkostic in https://github.com/open-quantum-safe/liboqs/pull/1400
* rm duped "the" in pull_request_template.md by @Rudxain in https://github.com/open-quantum-safe/liboqs/pull/1439
* fix "ths" typo by @Rudxain in https://github.com/open-quantum-safe/liboqs/pull/1438
* Generate and install pkgconfig file by @tranzystorek-io in https://github.com/open-quantum-safe/liboqs/pull/1435
* Initial fetching of MD and Cipher objects from OpenSSL(3) by @beldmit in https://github.com/open-quantum-safe/liboqs/pull/1431
* Use CMake flag for -Werror by @thomwiggers in https://github.com/open-quantum-safe/liboqs/pull/1444
* Add missing x86 check in CMakeLists by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1445
* src/common/common.c (set_available_cpu_extensions): Cope without `HWCAP_SHA3' by @distorted-mdw in https://github.com/open-quantum-safe/liboqs/pull/1447
* Limit gcc version supported by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1451
* disable OpenSSL if neither AES,SHA2, or SHA3 are OSSL-provided by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1453
* Fixing OpenSSL SHA2 incremental API integration by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1454
* Update SPHINCS+ by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1420
* Remove remaining references to SPHINCS+-Haraka, Kyber 90s, Dilithium-AES by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1465
* relax OpenSSL initialization error handling by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1463
* once OSSL init by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1469
* Renamed sphincs-sha256-X to sphincs-sha2-X and sphincs-shake256-X to … by @xvzcf in https://github.com/open-quantum-safe/liboqs/pull/1467
* Extend test_hash to cover more input lengths by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1468
* protect ossl cleanup from multithreading errors by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1472
* Update SPHINCS+ specification version [skip ci] by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1477
* Patch AVX2 support: No Win for Sphincs+ by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1478
* Don't use cycle counter on ARM32 by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1485
* Fix SPHINCS+ constant-time suppression files by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1480

## New Contributors
* @thb-sb made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1295
* @malbert1 made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1327
* @ryndia made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1328
* @dependabot made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1354
* @Rudxain made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1439
* @tranzystorek-io made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1435
* @beldmit made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1431
* @distorted-mdw made their first contribution in https://github.com/open-quantum-safe/liboqs/pull/1447

**Full Changelog**: https://github.com/open-quantum-safe/liboqs/compare/0.7.2...0.8.0