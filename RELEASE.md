liboqs version 0.7.2
====================

About
-----

The **Open Quantum Safe (OQS) project** has the goal of developing and prototyping quantum-resistant cryptography.  More information on OQS can be found on our website: https://openquantumsafe.org/ and on Github at https://github.com/open-quantum-safe/.  

**liboqs** is an open source C library for quantum-resistant cryptographic algorithms.  Details about liboqs can be found in [README.md](https://github.com/open-quantum-safe/liboqs/blob/main/README.md).  See in particular limitations on intended use.

liboqs can be used with the following Open Quantum Safe application integrations:

- **OQS-OpenSSL 1.1.1**: A prototype integration of liboqs-based authentication and key exchange into TLS 1.3 in our fork of OpenSSL 1.1.1; see the [OQS-OpenSSL-1\_1\_1-stable](https://github.com/open-quantum-safe/openssl/tree/OQS-OpenSSL_1_1_1-stable) branch of our OpenSSL fork's repository.
- **oqs-provider**: A standalone prototype [OpenSSL 3 provider](https://www.openssl.org/docs/manmaster/man7/provider.html) enabling liboqs-based quantum-safe and hybrid key exchange for TLS 1.3, and post-quantum and hybrid X.509 certificate generation and CMS operations.
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

This is version 0.7.2 of liboqs.  It was released on August 21, 2022.

Security considerations
-----------------------

This release removes Rainbow level 1 and all variants of SIDH and SIKE due to cryptanalytic breaks of those algorithms.  Users are advised to move away from use of those algorithms immediately.

What's New
----------

This release continues from the 0.7.1 release of liboqs. 

The goal of this release is to provide a final release for algorithms that were included in Round 3 of the NIST Post-Quantum Standardization project. As the security status of some algorithms changed during Round 3, it drops algorithms known to be broken as of release (Rainbow level 1, SIKE). Some algorithms also updated their specification during Round 3; version 0.7.2 does not incorporate any algorithm changes that result in different input/output behaviour compared to version 0.7.1; such changes will included in version 0.8.0.

The next release of liboqs, version 0.8.0, will remove some algorithms that did not advance beyond NIST Round 3, update implementations based on algorithm/specification revisions that happened during Round 3, and begin to incorporate Round 4 changes. Discussion of algorithms to be removed in version 0.8.0 can be found in https://github.com/open-quantum-safe/liboqs/issues/1245.

### Key encapsulation mechanisms

- Kyber: Update implementation and switch use of symmetric crypto to OQS common code
- HQC: Fix build on gcc-12
- SIKE: Remove SIKE due to break (https://eprint.iacr.org/2022/975)

### Digital signature schemes

- Dilithium: Add ARMv8 optimized implementation and switch use of symmetric crypto to OQS common code
- Picnic: Update implementation
- Rainbow: Remove Rainbow level 1 due to break (https://eprint.iacr.org/2022/214)

### Other changes

- Add support for building on powerpc64
- Update XKCP implementation
- Improve SHA2 implementation on ARMv8
- Improve AES implementation on ARMv8
- Add aarch64 CPU feature detection on FreeBSD
- Improve cross-compiling on Windows
- Enable integration of liboqs into other CMake-based projects
- Increment shared object library version

---

Detailed changelog
------------------

* Update Picnic to 3.0.8 by @sebastinas in https://github.com/open-quantum-safe/liboqs/pull/1158
* XCode update by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1162
* Add support for powerpc64 by @pkubaj in https://github.com/open-quantum-safe/liboqs/pull/1160
* remove picnic from cygwin build by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1171
* adding constant time test as weekly github action by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1170
* extend weekly run timeout [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1172
* Update XKCP and avoid assembler .ifdef directives by @jschanck in https://github.com/open-quantum-safe/liboqs/pull/1173
* Updated constant_time suppression files after picnic update by @christianpaquin in https://github.com/open-quantum-safe/liboqs/pull/1174
* Update to Picnic 3.0.9 by @sebastinas in https://github.com/open-quantum-safe/liboqs/pull/1167
* Mark stack non-executable when compiling with clang or gcc by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1161
* extend timeout to 10h by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1175
* skip sphincs,rainbow in shortened weekly testing by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1176
* Fixed build issue on arm based macs when using gcc11 by @Martyrshot in https://github.com/open-quantum-safe/liboqs/pull/1177
* disable BIKE on all 32bitters except x86 by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1180
* Update to picnic 3.0.11 (fixes #1178) by @sebastinas in https://github.com/open-quantum-safe/liboqs/pull/1181
* Weekly run update by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1182
* weekly job moved to Sunday [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1183
* Resolves an issue when building sha2 using arm crypto extensions with gcc11 by @Martyrshot in https://github.com/open-quantum-safe/liboqs/pull/1184
* Suppress constant time check for public matrix generation on Kyber AVX2 by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1189
* create and install cmake import files by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1191
* adding warning re Rainbow to documentation [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1193
* ARMv8 optimized Dilithium by @Martyrshot in https://github.com/open-quantum-safe/liboqs/pull/1194
* skip yamllint test for good by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1196
* Edits to key scheduling for AES on ARM to be constant time by @tedeaton in https://github.com/open-quantum-safe/liboqs/pull/1200
* Correct OQS_DIST_BUILD for ARM by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1202
* Deal with some issues identified by clang scan-build by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1211
* Issues and passes for aarch64 const time checks by @tedeaton in https://github.com/open-quantum-safe/liboqs/pull/1214
* Use `SecRandomCopyBytes` for system randomness on iOS (to allow building on iOS) by @zanebeckwith in https://github.com/open-quantum-safe/liboqs/pull/1219
* workaround for picnic under msys2 by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1220
* Adding aarch64 CPU feature detection for FreeBSD by @Martyrshot in https://github.com/open-quantum-safe/liboqs/pull/1210
* Added typecasts to tests/ds_benchmark.h to silence clang warnings by @Martyrshot in https://github.com/open-quantum-safe/liboqs/pull/1225
* doxygen update by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1228
* Link documentation and code by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1229
* Integrates pqcrystals with common-aes / extends common code AES CTR-API by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1221
* Adds AES context release in Dilithium-AES / fix memory leak by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1238
* Update Picnic to 3.0.14 (fixes #1212) by @sebastinas in https://github.com/open-quantum-safe/liboqs/pull/1237
* improve Windows crosscompile handling by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1236
* Bump Picnic to 3.0.15 by @sebastinas in https://github.com/open-quantum-safe/liboqs/pull/1241
* Updated Dilithium sign.c patch with AES context release (2) by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1242
* adding scan-build test by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1240
* adding memory leak testing by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1234
* disable msys2 testing by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1243
* docs: Add valgrind as test dependencies in README.md by @yin19941005 in https://github.com/open-quantum-safe/liboqs/pull/1251
* Sync Kyber with upstream, enable Scan-Build with Kyber by @bhess in https://github.com/open-quantum-safe/liboqs/pull/1252
* Deal with the issue identified by valgrind by @splasky in https://github.com/open-quantum-safe/liboqs/pull/1250
* pqclean_hqc: Fix build on GCC-12 by @vt-alt in https://github.com/open-quantum-safe/liboqs/pull/1254
* Remove Rainbow level 1 by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1263
* Updated to SIKE v3.5.1 by @christianpaquin in https://github.com/open-quantum-safe/liboqs/pull/1231
* update PR template to include oqs-provider (OSSL dependency) [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1269
* Update to Picnic 3.0.16 (fixes #1253) by @sebastinas in https://github.com/open-quantum-safe/liboqs/pull/1271
* Remove SIDH and SIKE by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1272
* fixing FreeBSD runtime ARM CPU feature detection by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1267
* Update CODEOWNERS by @vsoftco in https://github.com/open-quantum-safe/liboqs/pull/1274
* adding library version retrieval function by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1273
* fixup dilithium-avx2 valgrind test file by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1283
* Add option to all pytests to skip particular algorithms by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1282
* Update to Picnic 3.0.17 by @sebastinas in https://github.com/open-quantum-safe/liboqs/pull/1280
* Update release notes by @dstebila in https://github.com/open-quantum-safe/liboqs/pull/1281
* add warning about HQC [skip ci] by @baentsch in https://github.com/open-quantum-safe/liboqs/pull/1284
* Cover SHA3/SHAKE-implementation specific code paths in Picnic suppres… by @sebastinas in https://github.com/open-quantum-safe/liboqs/pull/1286

**Full Changelog**: https://github.com/open-quantum-safe/liboqs/compare/0.7.1...0.7.2
