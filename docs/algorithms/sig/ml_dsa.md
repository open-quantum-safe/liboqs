# ML-DSA

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: hardness of lattice problems over module lattices.
- **Principal submitters**: Vadim Lyubashevsky.
- **Auxiliary submitters**: Shi Bai, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Peter Schwabe, Gregor Seiler, Damien Stehlé.
- **Authors' website**: https://pq-crystals.org/dilithium/
- **Specification version**: pq-crystals 20230825.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/pq-crystals/dilithium/commit/918af1a6eaedcedf9fdd8aaaca6c1fccd5a7a51f with copy_from_upstream patches
  - **Implementation license (SPDX-Identifier)**: CC0-1.0 or Apache-2.0
- **Optimized Implementation sources**: https://github.com/pq-crystals/dilithium/commit/918af1a6eaedcedf9fdd8aaaca6c1fccd5a7a51f with copy_from_upstream patches
  - **oldpqclean-aarch64**:<a name="oldpqclean-aarch64"></a>
      - **Source**: https://github.com/PQClean/PQClean/commit/8e220a87308154d48fdfac40abbb191ac7fce06a with copy_from_upstream patches
      - **Implementation license (SPDX-Identifier)**: CC0-1.0 and (CC0-1.0 or Apache-2.0) and (CC0-1.0 or MIT) and MIT


## Parameter set summary

|  Parameter set  | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:---------------:|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|  ML-DSA-44-ipd  | EUF-CMA          |                    2 |                      1312 |                      2560 |                     2420 |
|  ML-DSA-65-ipd  | EUF-CMA          |                    3 |                      1952 |                      4032 |                     3309 |
|  ML-DSA-87-ipd  | EUF-CMA          |                    5 |                      2592 |                      4896 |                     4627 |

## ML-DSA-44-ipd implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Darwin,Linux                    | AVX2,POPCNT             | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## ML-DSA-65-ipd implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Darwin,Linux                    | AVX2,POPCNT             | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## ML-DSA-87-ipd implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Darwin,Linux                    | AVX2,POPCNT             | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.