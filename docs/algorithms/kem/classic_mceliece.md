# Classic McEliece

- **Algorithm type**: Key encapsulation mechanism.
- **Main cryptographic assumption**: Niederreiter's dual version of McEliece's public key encryption using binary Goppa codes.
- **Principal submitters**: Daniel J. Bernstein, Tung Chou, Tanja Lange, Ingo von Maurich, Rafael Misoczki, Ruben Niederhagen, Edoardo Persichetti, Christiane Peters, Peter Schwabe, Nicolas Sendrier, Jakub Szefer, Wen Wang.
- **Authors' website**: https://classic.mceliece.org
- **Specification version**: SUPERCOP-20221025.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/PQClean/PQClean/commit/1eacfdafc15ddc5d5759d0b85b4cef26627df181 with copy_from_upstream patches
  - **Implementation license (SPDX-Identifier)**: Public domain
- **Optimized Implementation sources**:
  - **avx2**:<a name="avx2"></a>
      - **Source**: https://github.com/PQClean/PQClean/commit/1eacfdafc15ddc5d5759d0b85b4cef26627df181 with copy_from_upstream patches
      - **Implementation license (SPDX-Identifier)**: Public domain
- **Ancestors of primary source**:
  - SUPERCOP-20221025 "clean" and "avx2" implementations

## Advisories

- Classic-McEliece-460896, Classic-McEliece-460896f, Classic-McEliece-6960119, and Classic-McEliece-6960119f parameter sets fail memory leak testing on x86-64 when building with ``clang`` using optimization level ``-O2`` and ``-O3``. Care is advised when using the algorithm at higher optimization levels, and any other compiler and architecture.
- Current implementation of the algorithm may not be constant-time. Additionally, environment specific constant-time leaks may not be documented; please report potential constant-time leaks when found.

## Parameter set summary

|       Parameter set       | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Ciphertext size (bytes) |   Shared secret size (bytes) | Keypair seed size (bytes)   | Encapsulation seed size (bytes)   |
|:-------------------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|--------------------------:|-----------------------------:|:----------------------------|:----------------------------------|
|  Classic-McEliece-348864  | NA                    | IND-CCA2         |                    1 |                    261120 |                      6492 |                        96 |                           32 | NA                          | NA                                |
| Classic-McEliece-348864f  | NA                    | IND-CCA2         |                    1 |                    261120 |                      6492 |                        96 |                           32 | NA                          | NA                                |
|  Classic-McEliece-460896  | NA                    | IND-CCA2         |                    3 |                    524160 |                     13608 |                       156 |                           32 | NA                          | NA                                |
| Classic-McEliece-460896f  | NA                    | IND-CCA2         |                    3 |                    524160 |                     13608 |                       156 |                           32 | NA                          | NA                                |
| Classic-McEliece-6688128  | NA                    | IND-CCA2         |                    5 |                   1044992 |                     13932 |                       208 |                           32 | NA                          | NA                                |
| Classic-McEliece-6688128f | NA                    | IND-CCA2         |                    5 |                   1044992 |                     13932 |                       208 |                           32 | NA                          | NA                                |
| Classic-McEliece-6960119  | NA                    | IND-CCA2         |                    5 |                   1047319 |                     13948 |                       194 |                           32 | NA                          | NA                                |
| Classic-McEliece-6960119f | NA                    | IND-CCA2         |                    5 |                   1047319 |                     13948 |                       194 |                           32 | NA                          | NA                                |
| Classic-McEliece-8192128  | NA                    | IND-CCA2         |                    5 |                   1357824 |                     14120 |                       208 |                           32 | NA                          | NA                                |
| Classic-McEliece-8192128f | NA                    | IND-CCA2         |                    5 |                   1357824 |                     14120 |                       208 |                           32 | NA                          | NA                                |

## Classic-McEliece-348864 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | True                  |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT             | False                              | False                                          | True                  |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## Classic-McEliece-348864f implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT,BMI1        | False                              | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Classic-McEliece-460896 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT             | False                              | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Classic-McEliece-460896f implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT,BMI1        | False                              | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Classic-McEliece-6688128 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT             | False                              | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Classic-McEliece-6688128f implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT,BMI1        | False                              | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Classic-McEliece-6960119 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT             | False                              | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Classic-McEliece-6960119f implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT,BMI1        | False                              | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Classic-McEliece-8192128 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT             | False                              | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Classic-McEliece-8192128f implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT,BMI1        | False                              | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.