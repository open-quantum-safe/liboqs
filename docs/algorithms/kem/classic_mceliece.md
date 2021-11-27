# Classic McEliece

- **Algorithm type**: Key encapsulation mechanism.
- **Main cryptographic assumption**: Niederreiter's dual version of McEliece's public key encryption using binary Goppa codes.
- **Principal submitters**: Daniel J. Bernstein, Tung Chou, Tanja Lange, Ingo von Maurich, Rafael Misoczki, Ruben Niederhagen, Edoardo Persichetti, Christiane Peters, Peter Schwabe, Nicolas Sendrier, Jakub Szefer, Wen Wang.
- **Authors' website**: https://classic.mceliece.org
- **Specification version**: SUPERCOP-20191221.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/PQClean/PQClean/commit/6a32796212b79a5f9126d0a933e1216313f50c16
  - **Implementation license (SPDX-Identifier)**: Public domain
, which takes it from:
  - SUPERCOP-20191221 "vec" and "avx" implementations

## Parameter set summary

|       Parameter set       | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Ciphertext size (bytes) |   Shared secret size (bytes) |
|:-------------------------:|:-----------------|---------------------:|--------------------------:|--------------------------:|--------------------------:|-----------------------------:|
|  Classic-McEliece-348864  | IND-CCA2         |                    1 |                    261120 |                      6452 |                       128 |                           32 |
| Classic-McEliece-348864f  | IND-CCA2         |                    1 |                    261120 |                      6452 |                       128 |                           32 |
|  Classic-McEliece-460896  | IND-CCA2         |                    3 |                    524160 |                     13568 |                       188 |                           32 |
| Classic-McEliece-460896f  | IND-CCA2         |                    3 |                    524160 |                     13568 |                       188 |                           32 |
| Classic-McEliece-6688128  | IND-CCA2         |                    5 |                   1044992 |                     13892 |                       240 |                           32 |
| Classic-McEliece-6688128f | IND-CCA2         |                    5 |                   1044992 |                     13892 |                       240 |                           32 |
| Classic-McEliece-6960119  | IND-CCA2         |                    5 |                   1047319 |                     13908 |                       226 |                           32 |
| Classic-McEliece-6960119f | IND-CCA2         |                    5 |                   1047319 |                     13908 |                       226 |                           32 |
| Classic-McEliece-8192128  | IND-CCA2         |                    5 |                   1357824 |                     14080 |                       240 |                           32 |
| Classic-McEliece-8192128f | IND-CCA2         |                    5 |                   1357824 |                     14080 |                       240 |                           32 |

## Classic-McEliece-348864 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | vec                      | All                         | All                             | None                    | True                               | True                                           | True                  |
| [Primary Source](#primary-source) | avx                      | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT             | False                              | True                                           | True                  |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## Classic-McEliece-348864f implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | vec                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx                      | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT,BMI1        | False                              | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Classic-McEliece-460896 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | vec                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx                      | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT             | False                              | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Classic-McEliece-460896f implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | vec                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx                      | x86\_64                     | Linux,Darwin                    | AVX2,BMI1,POPCNT        | False                              | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Classic-McEliece-6688128 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | vec                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx                      | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT             | False                              | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Classic-McEliece-6688128f implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | vec                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx                      | x86\_64                     | Linux,Darwin                    | AVX2,BMI1,POPCNT        | False                              | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Classic-McEliece-6960119 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | vec                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx                      | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT             | False                              | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Classic-McEliece-6960119f implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | vec                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx                      | x86\_64                     | Linux,Darwin                    | AVX2,BMI1,POPCNT        | False                              | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Classic-McEliece-8192128 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | vec                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx                      | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT             | False                              | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Classic-McEliece-8192128f implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | vec                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx                      | x86\_64                     | Linux,Darwin                    | AVX2,POPCNT,BMI1        | False                              | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.