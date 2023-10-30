# CRYSTALS-Dilithium

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: hardness of lattice problems over module lattices.
- **Principal submitters**: Vadim Lyubashevsky.
- **Auxiliary submitters**: Shi Bai, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Peter Schwabe, Gregor Seiler, Damien Stehlé.
- **Authors' website**: https://pq-crystals.org/dilithium/
- **Specification version**: 3.1.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/pq-crystals/dilithium/commit/3e9b9f1412f6c7435dbeb4e10692ea58f181ee51 with copy_from_upstream patches
  - **Implementation license (SPDX-Identifier)**: CC0-1.0 or Apache-2.0
- **Optimized Implementation sources**: https://github.com/pq-crystals/dilithium/commit/3e9b9f1412f6c7435dbeb4e10692ea58f181ee51 with copy_from_upstream patches
  - **oldpqclean-aarch64**:<a name="oldpqclean-aarch64"></a>
      - **Source**: https://github.com/PQClean/PQClean/commit/8e220a87308154d48fdfac40abbb191ac7fce06a with copy_from_upstream patches
      - **Implementation license (SPDX-Identifier)**: CC0-1.0 and (CC0-1.0 or Apache-2.0) and (CC0-1.0 or MIT) and MIT


## Parameter set summary

|  Parameter set  | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:---------------:|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|   Dilithium2    | EUF-CMA          |                    2 |                      1312 |                      2528 |                     2420 |
|   Dilithium3    | EUF-CMA          |                    3 |                      1952 |                      4000 |                     3293 |
|   Dilithium5    | EUF-CMA          |                    5 |                      2592 |                      4864 |                     4595 |

## Dilithium2 implementation characteristics

|           Implementation source           | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:-----------------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
|     [Primary Source](#primary-source)     | ref                      | All                         | All                             | None                    | True                               | True                                           | False                 |
|     [Primary Source](#primary-source)     | avx2                     | x86\_64                     | Darwin,Linux                    | AVX2,POPCNT             | True                               | True                                           | False                 |
| [oldpqclean-aarch64](#oldpqclean-aarch64) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## Dilithium3 implementation characteristics

|           Implementation source           | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:-----------------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|     [Primary Source](#primary-source)     | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
|     [Primary Source](#primary-source)     | avx2                     | x86\_64                     | Darwin,Linux                    | AVX2,POPCNT             | True                               | True                                           | False                |
| [oldpqclean-aarch64](#oldpqclean-aarch64) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Dilithium5 implementation characteristics

|           Implementation source           | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:-----------------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|     [Primary Source](#primary-source)     | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
|     [Primary Source](#primary-source)     | avx2                     | x86\_64                     | Darwin,Linux                    | AVX2,POPCNT             | True                               | True                                           | False                |
| [oldpqclean-aarch64](#oldpqclean-aarch64) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.