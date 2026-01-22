# ML-DSA

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: hardness of lattice problems over module lattices.
- **Principal submitters**: Vadim Lyubashevsky.
- **Auxiliary submitters**: Shi Bai, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Peter Schwabe, Gregor Seiler, Damien Stehlé.
- **Authors' website**: https://pq-crystals.org/dilithium/ and https://csrc.nist.gov/pubs/fips/204/final
- **Specification version**: ML-DSA.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/pq-code-package/mldsa-native/commit/f48f164cefb07f4ffa519ddda7cee670b8ee3517
  - **Implementation license (SPDX-Identifier)**: MIT or Apache-2.0 or ISC
- **Optimized Implementation sources**:
  - **x86_64**:<a name="x86_64"></a>
      - **Source**: https://github.com/pq-code-package/mldsa-native/commit/f48f164cefb07f4ffa519ddda7cee670b8ee3517
      - **Implementation license (SPDX-Identifier)**: MIT or Apache-2.0 or ISC
  - **aarch64**:<a name="aarch64"></a>
      - **Source**: https://github.com/pq-code-package/mldsa-native/commit/f48f164cefb07f4ffa519ddda7cee670b8ee3517
      - **Implementation license (SPDX-Identifier)**: MIT or Apache-2.0 or ISC


## Parameter set summary

|  Parameter set  | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:---------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|    ML-DSA-44    | NA                    | SUF-CMA          |                    2 |                      1312 |                      2560 |                     2420 |
|    ML-DSA-65    | NA                    | SUF-CMA          |                    3 |                      1952 |                      4032 |                     3309 |
|    ML-DSA-87    | NA                    | SUF-CMA          |                    5 |                      2592 |                      4896 |                     4627 |

## ML-DSA-44 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | x86\_64                  | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## ML-DSA-65 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | x86\_64                  | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                |
| [Primary Source](#primary-source) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## ML-DSA-87 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | x86\_64                  | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                |
| [Primary Source](#primary-source) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.