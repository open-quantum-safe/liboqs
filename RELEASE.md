# liboqs version 0.16.0-rc1

The **Open Quantum Safe (OQS) project** has the goal of developing and
prototyping quantum-resistant cryptography. More information on OQS can be
found on our website: <https://openquantumsafe.org/> and on Github at
<https://github.com/open-quantum-safe/>.

**liboqs** is an open source C library for quantum-resistant cryptographic
algorithms. Details about liboqs can be found in
[README.md](https://github.com/open-quantum-safe/liboqs/blob/main/README.md).
See in particular limitations on intended use.

liboqs can be used with the following Open Quantum Safe application integrations:

- **oqs-provider**: A standalone prototype [OpenSSL 3
provider](https://www.openssl.org/docs/manmaster/man7/provider.html) enabling
liboqs-based quantum-safe and hybrid key authentication and exchange for TLS
1.3, X.509 certificate generation and CMS operations.
- **OQS-BoringSSL**: A prototype integration of liboqs-based authentication and
key exchange into TLS 1.3 in our fork of BoringSSL; see
<https://github.com/open-quantum-safe/boringssl>.
- **OQS-OpenSSH**: A prototype integration of liboqs-based authentication and
key exchange into Secure Shell (SSH) version 2 in our fork of OpenSSH; see
<https://github.com/open-quantum-safe/openssh>.

Several [demos](https://github.com/open-quantum-safe/oqs-demos) are available
for using the above libraries in applications, including Apache, Chromium,
curl, haproxy, nginx, and Wireshark.

liboqs can also be used in the following programming languages via
language-specific wrappers:

- C++, via <https://github.com/open-quantum-safe/liboqs-cpp>
- Go, via <https://github.com/open-quantum-safe/liboqs-go>
- Java, via <https://github.com/open-quantum-safe/liboqs-java>
- Python 3, via <https://github.com/open-quantum-safe/liboqs-python>
- Rust, via <https://github.com/open-quantum-safe/liboqs-rust>

## Release notes

liboqs 0.16.0-rc1 was released on Tuesday, June 9, 2026.

### Deprecation notice

SPHINCS+ was removed in 0.16.0.

### Security issues

- Fixed uninitialized `encaps_derand` pointer dereference
[#2460](https://github.com/open-quantum-safe/liboqs/pull/2460)
- Fixed out-of-bounds read in XMSS/XMSS^MT signature verification
[077e32a](https://github.com/open-quantum-safe/liboqs/commit/077e32a)
[#2384](https://github.com/open-quantum-safe/liboqs/pull/2384)
- Fixed Integer underflow in CROSS `crypto_sign_open()`
[25d0a9e](https://github.com/open-quantum-safe/liboqs/commit/25d0a9e)
- Fixed incorrect array size when calling `secure_clean`
[ab0e07c](https://github.com/open-quantum-safe/liboqs/commit/ab0e07c)
- Implemented optimization barrier `OQS_MEM_BLACK_BOX` and applied to
`ct_select` in FrodoKEM
[#2431](https://github.com/open-quantum-safe/liboqs/pull/2431)

### Significant change

#### FrodoKEM algorithm change

[#2192](https://github.com/open-quantum-safe/liboqs/pull/2192),
[#2342](https://github.com/open-quantum-safe/liboqs/pull/2342)

Existing FrodoKEM in 0.15.0 was renamed to ephemeral FrodoKEM
(`KEM_efrodokem_<640|976|1344>_<aes|shake>`), and the salted variant of FrodoKEM
was added under the prior names (`KEM_frodokem_<640|976|1344>_<aes|shake>`).

Ephemeral FrodoKEM is recommended for applications where each keypair will
encapsulate only a small number of shared secrets and ciphertexts. Standard
(salted) FrodoKEM is recommended for applications where each keypair is
expected to encapsulate large number of ciphertexts. Please consult
[upstream](https://github.com/microsoft/PQCrypto-LWEKE/#frodokem-learning-with-errors-key-encapsulation)
for more details.

#### mldsa-native integration

[#2284](https://github.com/open-quantum-safe/liboqs/pull/2284),
[#2391](https://github.com/open-quantum-safe/liboqs/pull/2391),
[#3445](https://github.com/open-quantum-safe/liboqs/pull/3445)

mldsa-native is a secure, fast, and portable C90 implementation of the ML-DSA
post-quantum signature standard. It also includes optimized builds for x86_64
and aarch64. It is now the default implementation behind
`SIG_ml_dsa_<44|65|87>`.

#### Updated HQC implementation

[#2407](https://github.com/open-quantum-safe/liboqs/pull/2407)

The HQC implementations in liboqs were updated to 20250822 spec. Its upstream
switched from PQClean to the [official repo](https://gitlab.com/pqc-hqc/hqc).
`KEM_hqc_<1|3|5>` is now enabled by default.

#### MQOM integration and memory-optimized build flag

[#2385](https://github.com/open-quantum-safe/liboqs/pull/2385),
[#2367](https://github.com/open-quantum-safe/liboqs/pull/2367)

MQOM is a third-round candidate in NIST's Additional Digital Signatures for the
PQC Standardization Process. Portable, x86_64-optimized, and memory-optimized
implementations were integrate into liboqs under `OQS_ENABLE_SIG_MQOM`.

A new build flag `OQS_MEMOPT_BUILD` can now be used to switch to
memory-optimized builds for algorithms that have memory-optimized
implementations.

#### OpenSSH implementation of NTRU Prime

[#2356](https://github.com/open-quantum-safe/liboqs/pull/2356)

A public-domain OpenSSH implementation of NTRUPrime761 replaced the PQClean
implementation as the default backend for `KEM_ntruprime_sntrup761`.

### Bug fixes

- Fixed incremental absorption bug in AVX512VL SHA3-512
[#2442](https://github.com/open-quantum-safe/liboqs/pull/2442)
- Implemented fallback for when `EVP_DigestSqueeze` is unavailable
[#2433](https://github.com/open-quantum-safe/liboqs/pull/2433)
- Added API for detecting stateful signature support at runtime
[#2434](https://github.com/open-quantum-safe/liboqs/pull/2434)
- Fixed missing initialization and indexing bug in LMS
[#2416](https://github.com/open-quantum-safe/liboqs/pull/2416)
- Fixed erroneous MAYO_OK despite failed sample_solution() attempts in MAYO
[#2403](https://github.com/open-quantum-safe/liboqs/pull/2403)
- Limited pytest parallelism to prevent memory exhaustion in constrained
environment [#2397](https://github.com/open-quantum-safe/liboqs/pull/2397)
- Fixed cuPQC ML-KEM derand symbol names and `#if/#elif` chains
[#2396](https://github.com/open-quantum-safe/liboqs/pull/2396)
- Tightened Windows compiler detection
[#2394](https://github.com/open-quantum-safe/liboqs/pull/2394)
- Fixed mismatched macros in LMS
[#2379](https://github.com/open-quantum-safe/liboqs/pull/2379) <!-- 8f08fd8 has
no external effect and is ignored -->
- Made fuzzers tolerant to disabled algorithms
[#2359](https://github.com/open-quantum-safe/liboqs/pull/2359)
- Removed inlined exponentiation in CROSS-RSDPG-1
[#2357](https://github.com/open-quantum-safe/liboqs/pull/2357)
- Fixed incorrect arg register update in AVX512 Keccak
[#2330](https://github.com/open-quantum-safe/liboqs/pull/2330)

### Miscellaneous

- Update mlkem-native to v1.1.0
[#2376](https://github.com/open-quantum-safe/liboqs/pull/2376)
- Removed legacy HQC constant-time test
[#2467](https://github.com/open-quantum-safe/liboqs/pull/2467)
- Download Intel SDE from alternative source
[#2463](https://github.com/open-quantum-safe/liboqs/pull/2463)
- Updated algorithm support information
[#2441](https://github.com/open-quantum-safe/liboqs/pull/2441)
- Added NIST LMS Signature verify KATs
[#2435](https://github.com/open-quantum-safe/liboqs/pull/2435)
- Fixed comments on SHA3-AVX512VL assembly file. Thank you Anthony Plank for
reporting the issue
[#2438](https://github.com/open-quantum-safe/liboqs/pull/2438)
- Fixed latent warnings exposed by refreshed CI container images
[#2430](https://github.com/open-quantum-safe/liboqs/pull/2430)
- Added common dependencies with `include_only` in `copy_from_upstream`
[#2382](https://github.com/open-quantum-safe/liboqs/pull/2382)
- Documented algorithm support levels
[#2413](https://github.com/open-quantum-safe/liboqs/pull/2413)
- Updated Dependabot configuration
[#2420](https://github.com/open-quantum-safe/liboqs/pull/2420)
- Added @loganaden as codeowner for Kyber
[#2421](https://github.com/open-quantum-safe/liboqs/pull/2421)
- Bumped `gitpython` version for `copy_from_upstream`
[#2414](https://github.com/open-quantum-safe/liboqs/pull/2414)
- Updated GOVERNANCE.md and CODEOWNERS
[#2410](https://github.com/open-quantum-safe/liboqs/pull/2410)
- Added fuzz harness for LMS/XMSS signature verification
[#2398](https://github.com/open-quantum-safe/liboqs/pull/2398)
[#2412](https://github.com/open-quantum-safe/liboqs/pull/2412)
- Added fuzz harness for XMSS signature verification
[#2399](https://github.com/open-quantum-safe/liboqs/pull/2399)
[#2400](https://github.com/open-quantum-safe/liboqs/pull/2400)
- Moved compiler optimization level to CMAKE_BUILD_TYPE
[#2383](https://github.com/open-quantum-safe/liboqs/pull/2383)
- Updated pytest package version
[#2408](https://github.com/open-quantum-safe/liboqs/pull/2408)
- Demoted armhf to tier 3 supported platform
[#2406](https://github.com/open-quantum-safe/liboqs/pull/2406)
- Added Wycheproof tests for ML-DSA
[#2378](https://github.com/open-quantum-safe/liboqs/pull/2378)
- Added overflow check for OQS_MEM_calloc arguments
[#2377](https://github.com/open-quantum-safe/liboqs/pull/2377)
- Updated Wycheproof tests ML-KEM
[#2373](https://github.com/open-quantum-safe/liboqs/pull/2373)
- Updated Zephyr CI tests to recent versions
[#2369](https://github.com/open-quantum-safe/liboqs/pull/2369)
- Documented KAT sources and hash verification process
[#2363](https://github.com/open-quantum-safe/liboqs/pull/2363)
- Added support for ML-DSA pre-hash sign ACVP test vectors
[#2362](https://github.com/open-quantum-safe/liboqs/pull/2362)
- Added external-mu sign gen ACVP tests for ML-DSA
[#2352](https://github.com/open-quantum-safe/liboqs/pull/2352)
- Cached ACVP test vectors
[#2354](https://github.com/open-quantum-safe/liboqs/pull/2354)
- Added OQS_SIG_sign methods tests in vectors_sig
[#2350](https://github.com/open-quantum-safe/liboqs/pull/2350)
- Added the path to the binary include dir
[#2312](https://github.com/open-quantum-safe/liboqs/pull/2312)
- Removed local copies of ACVP test vectors
[#2303](https://github.com/open-quantum-safe/liboqs/pull/2303)
- New stylesheet for Doxygen-generated documentation
[#2337](https://github.com/open-quantum-safe/liboqs/pull/2337)
- Docs: List all optimized implementations
[#2333](https://github.com/open-quantum-safe/liboqs/pull/2333)
- Fixed README href formatting
[#2323](https://github.com/open-quantum-safe/liboqs/pull/2323)
- Unpined OQS_MINIMAL_BUILD alg list link from specific commit
[#2326](https://github.com/open-quantum-safe/liboqs/pull/2326)

### New contributors

- Vishnu Ajith (@Vishnu2707)
- William Bates (@will-bates11)
- Nelonn (@Nelonn)
- Abhi S (@abhi-dev-engg)
- Ryad Benadjila (@rben-dev)
- Billy Brumley (@bbbrumley)
- saanvi ratra (@Anusaa35)
- Harshith Vasireddy (@ode)
- Laugenbraue (@mmarock)
- Sam Van de Velde (@samvandevelde)
- Emil Lundberg (@emlun)
