# Kyber

- **Algorithm type**: Key encapsulation mechanism.
- **Main cryptographic assumption**: Module LWE+R with base ring Z[x]/(3329, x^256+1).
- **Principal submitters**: Peter Schwabe.
- **Auxiliary submitters**: Roberto Avanzi, Joppe Bos, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Vadim Lyubashevsky, John M. Schanck, Gregor Seiler, Damien Stehlé.
- **Authors' website**: https://pq-crystals.org/
- **Specification version**: NIST Round 3 submission.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/pq-crystals/kyber/commit/faf5c3fe33e0b61c7c8a7888dd862bf5def17ad2 with copy_from_upstream patches
  - **Implementation license (SPDX-Identifier)**: CC0-1.0
- **Optimized Implementation sources**: https://github.com/pq-crystals/kyber/commit/faf5c3fe33e0b61c7c8a7888dd862bf5def17ad2 with copy_from_upstream patches
  - **pqclean-aarch64**:<a name="pqclean-aarch64"></a>
      - **Source**: https://github.com/PQClean/PQClean/commit/6a32796212b79a5f9126d0a933e1216313f50c16 with copy_from_upstream patches
      - **Implementation license (SPDX-Identifier)**: CC0-1.0


## Parameter set summary

|  Parameter set  | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Ciphertext size (bytes) |   Shared secret size (bytes) |
|:---------------:|:-----------------|---------------------:|--------------------------:|--------------------------:|--------------------------:|-----------------------------:|
|    Kyber512     | IND-CCA2         |                    1 |                       800 |                      1632 |                       768 |                           32 |
|  Kyber512-90s   | IND-CCA2         |                    1 |                       800 |                      1632 |                       768 |                           32 |
|    Kyber768     | IND-CCA2         |                    3 |                      1184 |                      2400 |                      1088 |                           32 |
|  Kyber768-90s   | IND-CCA2         |                    3 |                      1184 |                      2400 |                      1088 |                           32 |
|    Kyber1024    | IND-CCA2         |                    5 |                      1568 |                      3168 |                      1568 |                           32 |
|  Kyber1024-90s  | IND-CCA2         |                    5 |                      1568 |                      3168 |                      1568 |                           32 |

## Kyber512 implementation characteristics

|        Implementation source        | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:-----------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
|  [Primary Source](#primary-source)  | ref                      | All                         | All                             | None                    | True                               | True                                           | False                 |
|  [Primary Source](#primary-source)  | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                 |
| [pqclean-aarch64](#pqclean-aarch64) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## Kyber512-90s implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used           | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:--------------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                            | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AES,AVX2,BMI2,POPCNT,SSE2,SSSE3 | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Kyber768 implementation characteristics

|        Implementation source        | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:-----------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|  [Primary Source](#primary-source)  | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
|  [Primary Source](#primary-source)  | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                |
| [pqclean-aarch64](#pqclean-aarch64) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Kyber768-90s implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used           | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:--------------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                            | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AES,AVX2,BMI2,POPCNT,SSE2,SSSE3 | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Kyber1024 implementation characteristics

|        Implementation source        | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:-----------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|  [Primary Source](#primary-source)  | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
|  [Primary Source](#primary-source)  | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                |
| [pqclean-aarch64](#pqclean-aarch64) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Kyber1024-90s implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used           | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:--------------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                            | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AES,AVX2,BMI2,POPCNT,SSE2,SSSE3 | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.