# liboqs version 0.16.0-rc1

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

## Release notes

liboqs 0.16.0-rc1 was released on Tuesday, June 9, 2026.

### Deprecation notice

SPHINCS+ was removed in 0.16.0.

### Security issues

<!--- TODO: requires attention --->
- 077e32a: Fixed heap out-of-bounds read in XMSS/XMSS^MT verify
- ef70dea: Fixed XMSS/XMSS^MT signature verification OOB read (#2384)
- 25d0a9e: <!--TODO: this is a security issue, no commit message-->
- ab0e07c: Fixed incorrect array size when calling `secure_clean`

### Significant change

<!-- TODO: ca732f3 overwrote c343814, so c343814 is ignored -->
- ca732f3: Update mldsa-native implementations to v1.0.0-beta2 (#2445)
- ba66c3b: Pull MAYO update from upstream (#2403)
- 4369178: Update mlkem-native to v1.1.0 (#2376)
- 67b35f2: Added MQOM (#2385)
- c01e18f: Added public domain OpenSSH implementation of sntrup761 (#2356)
- a2fb264: Introduce `OQS_MEMOPT_BUILD` to enable memory‑optimized builds (#2367)
<!-- TODO: renamed old FrodoKEM to ephemeralFrodoKEM and added the salted version -->
- 6b390dd: Add both variants of frodokem (#2342)
- 8078e89: Remove SPHINCS+ (#2339)
<!-- TODO: this duplicates ca732f3 -->
- 2959f1a: mldsa-native integration (#2284)

### Bug fixes

- f986aea: Fixed incremental absorption bug in AVX512VL SHA3-512 (#2442)
<!--TODO: this reads a bit weird-->
- f28cef1: Probe once for EVP_DigestSqueeze; fall back when unsupported (#2433)
<!--TODO: this reads unclear-->
- 4cc83e5: Add `OQS_SIG_STFL_keygen_and_sign_supported()` for runtime detection (#2434)
- 2c8352c: Fixed missing initialization and indexing bug in LMS (#2416)
- 0e325cc: Limit pytest processes to prevent OOM on low-RAM systems (#2397)
- 1ae9cf6: Fixed cuPQC ML-KEM derand symbol names and `#if/#elif` chains (#2396)
- 03681f8: Tightened Windows compiler detection (#2394)
- 65e23ac: Fixed mismatched macros in LMS (#2379)
<!-- 8f08fd8 has no external effect and is ignored -->
- c58e93e: Make fuzzers tolerant to disabled algorithms (#2359)
- cb2d43e: Avoid inlined exponentiation in CROSS-RSDPG-1 (#2357)
- 1477303: Fix incorrect arg register update in AVX512 Keccak (#2330)

### Miscellaneous

- 5a8d047: Add NIST LMS Signature verify KATs (#2435)
- 813ba34: Fix comments on SHA3-AVX512VL assembly file (#2438)
- da7cea3: Fix latent warnings exposed by refreshed CI container images (#2430)
- 4956ce7: add common dependencies with include_only in `copy_from_upstream` (#2382)
<!--- TODO: this seems like a bigger deal than just "Miscellaneous" --->
- 2c6c917: Create a table of algorithm support levels (#2413)
- 2297d7a: Dependabot configuration and dependency updates (#2420)
- e2a79a3: Add loganaden as codeowner for Kyber (#2421)
- 42a52fa: Bump `gitpython` version for `copy_from_upstream` (#2414)
<!--- TODO: Should I put this here? --->
- 9fb382e: Update GOVERNANCE.md and CODEOWNERS (#2410)
- cd8a5d9: Add fuzz harness for LMS/XMSS signature verification (#2400)
- d850938: Add fuzz harness for XMSS signature verification (#2400)
- 69d2333: Move compiler optimization level to CMAKE_BUILD_TYPE (#2383)
- 3cb781f: Update pytest package version (#2408)
- 8bffe2e: Demote armhf to tier 3 supported platform (#2406)
- e0de251: Add Wycheproof tests for ML-DSA (#2378)
- e8b2e77: Check for overflow in arguments passed to OQS_MEM_calloc (#2377)
- 94662d9: Add latest Wycheproof tests ML-KEM (#2373)
- 04ab195: Update Zephyr CI tests to recent versions (#2369)
- d1d8447: document KAT sources and hash verification process (#2363)
- f1e80d1: Add support for ML-DSA pre-hash sign ACVP test vectors (#2362)
- d205492: Add external-mu sign gen ACVP tests for ML-DSA (#2352)
- 4fa3045: Cache ACVP test vectors (#2354)
- 30edf7b: test OQS_SIG_sign methods in vectors_sig (#2350)
- ab41146: Add the path to the binary include dir (#2312)
- 137860b: Remove local copies of ACVP test vectors (#2303)
- 754e1ed: New stylesheet for Doxygen-generated documentation (#2337)
- 04d9b6d: Docs: List all optimized implementations (#2333)
- 589731b: Fixed README href formatting (#2323)
- 39ea281: Unpin OQS_MINIMAL_BUILD alg list link from specific commit (#2326)

### New contributors

- Anthony Plank (report SHA3 AVX512VL comment in #2438)
- Vishnu Ajith (@Vishnu2707)
- William Bates
- Marco Gianvecchio for authoring 25d0a9e
- Nelonn
- Abhi S
- Ryad Benadjila <ryadbenadjila@gmail.com>
- Billy Brumley <bbb@iki.fi>
- Tobias Frauenschläger <t.frauenschlaeger@me.com>
- saanvi ratra <saanvisaanvi358@gmail.com>
- Harshith Vasireddy <34184577+ode@users.noreply.github.com>
- Nathaniel Brough <nathaniel.brough@gmail.com>
- Laugenbraue <36441810+mmarock@users.noreply.github.com>
- Sam Van de Velde <58397500+samvandevelde@users.noreply.github.com>
- Emil Lundberg <emil@emlun.se>
