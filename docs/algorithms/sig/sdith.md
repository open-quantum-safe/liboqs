# SDITH

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: Hardness of syndrom decoding problem for random linear codes on a finite field..
- **Principal submitters**: Carlos Aguilar Melchor, Slim Bettaieb, Loïc Bidoux, Thibauld Feneuil, Philippe Gaborit, Nicolas Gama, Shay Gueron, James Howe, Andreas Hülsing, David Joseph, Antoine Joux, Mukul Kulkarni, Edoardo Persichetti, Tovohery H. Randrianarisoa, Matthieu Rivain, Dongze Yue.
- **Authors' website**: https://sdith.org/
- **Specification version**: NIST Round 3.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/owaldron/sdith/commit/8a4bf13e932358f95c5117a492992d7552352dc6
  - **Implementation license (SPDX-Identifier)**: Apache-2.0
- **Optimized Implementation sources**:
  - **avx2**:<a name="avx2"></a>
      - **Source**: https://github.com/owaldron/sdith/commit/8a4bf13e932358f95c5117a492992d7552352dc6
      - **Implementation license (SPDX-Identifier)**: Apache-2.0


## Support

- **Upstream maintenance**: Actively maintained ([statement](NA))
- **OQS support tier**: Tier 3

## Parameter set summary

|         Parameter set         | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:-----------------------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|      SDitH3-L1-gf2-short      | NA                    | EUF-CMA          |                    1 |                        70 |                       147 |                     3721 |
| SDitH3-L1-gf2-short-cipherpow | NA                    | EUF-CMA          |                    1 |                        70 |                       147 |                     3721 |
|      SDitH3-L1-gf2-fast       | NA                    | EUF-CMA          |                    1 |                        70 |                       147 |                     4914 |
| SDitH3-L1-gf2-fast-cipherpow  | NA                    | EUF-CMA          |                    1 |                        70 |                       147 |                     4914 |
|      SDitH3-L3-gf2-short      | NA                    | EUF-CMA          |                    3 |                        98 |                       208 |                     8484 |
| SDitH3-L3-gf2-short-cipherpow | NA                    | EUF-CMA          |                    3 |                        98 |                       208 |                     8484 |
|      SDitH3-L3-gf2-fast       | NA                    | EUF-CMA          |                    3 |                        98 |                       208 |                    10852 |
| SDitH3-L3-gf2-fast-cipherpow  | NA                    | EUF-CMA          |                    3 |                        98 |                       208 |                    10452 |
|      SDitH3-L5-gf2-short      | NA                    | EUF-CMA          |                    5 |                       132 |                       275 |                    15147 |
| SDitH3-L5-gf2-short-cipherpow | NA                    | EUF-CMA          |                    5 |                       132 |                       275 |                    15147 |
|      SDitH3-L5-gf2-fast       | NA                    | EUF-CMA          |                    5 |                       132 |                       275 |                    19144 |
| SDitH3-L5-gf2-fast-cipherpow  | NA                    | EUF-CMA          |                    5 |                       132 |                       275 |                    19144 |

## SDitH3-L1-gf2-short implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | False                              | False                                          | False                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AES,AVX,AVX2,PCLMULQDQ  | True                               | False                                          | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## SDitH3-L1-gf2-short-cipherpow implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AES,AVX,AVX2,PCLMULQDQ  | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SDitH3-L1-gf2-fast implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AES,AVX,AVX2,PCLMULQDQ  | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SDitH3-L1-gf2-fast-cipherpow implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AES,AVX,AVX2,PCLMULQDQ  | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SDitH3-L3-gf2-short implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AES,AVX,AVX2,PCLMULQDQ  | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SDitH3-L3-gf2-short-cipherpow implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AES,AVX,AVX2,PCLMULQDQ  | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SDitH3-L3-gf2-fast implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AES,AVX,AVX2,PCLMULQDQ  | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SDitH3-L3-gf2-fast-cipherpow implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AES,AVX,AVX2,PCLMULQDQ  | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SDitH3-L5-gf2-short implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AES,AVX,AVX2,PCLMULQDQ  | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SDitH3-L5-gf2-short-cipherpow implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AES,AVX,AVX2,PCLMULQDQ  | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SDitH3-L5-gf2-fast implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AES,AVX,AVX2,PCLMULQDQ  | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SDitH3-L5-gf2-fast-cipherpow implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AES,AVX,AVX2,PCLMULQDQ  | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.