# MQOM

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: Computing the solution of a Multivariate Quadratic problem..
- **Principal submitters**: Ryad Benadjila, Charles Bouillaguet, Thibauld Feneuil, Matthieu Rivain.
- **Authors' website**: https://mqom.org/
- **Specification version**: NIST Round 2.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/rben-dev/mqom-liboqs/commit/e7c08113e75466f1cf16776d259528ae38864ecb
  - **Implementation license (SPDX-Identifier)**: MIT
- **Optimized Implementation sources**:
  - **memopt**:<a name="memopt"></a>
      - **Source**: https://github.com/rben-dev/mqom-liboqs/commit/e7c08113e75466f1cf16776d259528ae38864ecb
      - **Implementation license (SPDX-Identifier)**: MIT
  - **avx2**:<a name="avx2"></a>
      - **Source**: https://github.com/rben-dev/mqom-liboqs/commit/e7c08113e75466f1cf16776d259528ae38864ecb
      - **Implementation license (SPDX-Identifier)**: MIT


## Parameter set summary

|        Parameter set         | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:----------------------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
| mqom2\_cat1\_gf16\_fast\_r5  | NA                    | EUF-CMA          |                    1 |                        60 |                        88 |                     3280 |
| mqom2\_cat1\_gf16\_fast\_r3  | NA                    | EUF-CMA          |                    1 |                        60 |                        88 |                     3484 |
| mqom2\_cat1\_gf16\_short\_r5 | NA                    | EUF-CMA          |                    1 |                        60 |                        88 |                     2916 |
| mqom2\_cat1\_gf16\_short\_r3 | NA                    | EUF-CMA          |                    1 |                        60 |                        88 |                     3060 |
| mqom2\_cat3\_gf16\_fast\_r5  | NA                    | EUF-CMA          |                    3 |                        90 |                       132 |                     7738 |
| mqom2\_cat3\_gf16\_fast\_r3  | NA                    | EUF-CMA          |                    3 |                        90 |                       132 |                     8224 |
| mqom2\_cat3\_gf16\_short\_r5 | NA                    | EUF-CMA          |                    3 |                        90 |                       132 |                     6496 |
| mqom2\_cat3\_gf16\_short\_r3 | NA                    | EUF-CMA          |                    3 |                        90 |                       132 |                     6820 |
| mqom2\_cat5\_gf16\_fast\_r5  | NA                    | EUF-CMA          |                    5 |                       122 |                       180 |                    13772 |
| mqom2\_cat5\_gf16\_fast\_r3  | NA                    | EUF-CMA          |                    5 |                       122 |                       180 |                    14708 |
| mqom2\_cat5\_gf16\_short\_r5 | NA                    | EUF-CMA          |                    5 |                       122 |                       180 |                    12014 |
| mqom2\_cat5\_gf16\_short\_r3 | NA                    | EUF-CMA          |                    5 |                       122 |                       180 |                    12664 |

## mqom2\_cat1\_gf16\_fast\_r5 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                  |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                  |

Are implementations chosen based on runtime CPU feature detection? **No**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## mqom2\_cat1\_gf16\_fast\_r3 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom2\_cat1\_gf16\_short\_r5 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom2\_cat1\_gf16\_short\_r3 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom2\_cat3\_gf16\_fast\_r5 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom2\_cat3\_gf16\_fast\_r3 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom2\_cat3\_gf16\_short\_r5 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom2\_cat3\_gf16\_short\_r3 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom2\_cat5\_gf16\_fast\_r5 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom2\_cat5\_gf16\_fast\_r3 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom2\_cat5\_gf16\_short\_r5 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom2\_cat5\_gf16\_short\_r3 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.