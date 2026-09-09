# SNOVA

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: multivariable quadratic equations, oil and vinegar.
- **Principal submitters**: Lih-Chung Wang, Chun-Yen Chou, Jintai Ding, Hao Guo, Yen-Liang Kuan, Jan Adriaan Leegwater, Ming-Siou Li, Peigen Li, Bo-Shu Tseng, Po-En Tseng, Chia-Chun Wang.
- **Authors' website**: https://snova.pqclab.org/
- **Specification version**: 3.0.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/PQCLAB-SNOVA/S/commit/9b29c1e332cbd85757c8d6362a57b9a8c8298a38
  - **Implementation license (SPDX-Identifier)**: MIT
- **Optimized Implementation sources**:
  - **avx2**:<a name="avx2"></a>
      - **Source**: https://github.com/PQCLAB-SNOVA/S/commit/9b29c1e332cbd85757c8d6362a57b9a8c8298a38
      - **Implementation license (SPDX-Identifier)**: MIT
  - **neon**:<a name="neon"></a>
      - **Source**: https://github.com/PQCLAB-SNOVA/S/commit/9b29c1e332cbd85757c8d6362a57b9a8c8298a38
      - **Implementation license (SPDX-Identifier)**: MIT


## Support

- **Upstream maintenance**: Best effort
- **OQS support tier**: Tier 3

## Parameter set summary

|   Parameter set    | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:------------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|    SNOVA\_I\_K     | snova1k               | EUF-CMA          |                    1 |                       376 |                        96 |                      528 |
|  SNOVA\_I\_K\_AES  | snova1ka              | EUF-CMA          |                    1 |                       376 |                        96 |                      528 |
|    SNOVA\_I\_B     | snova1b               | EUF-CMA          |                    1 |                       656 |                        96 |                      388 |
|  SNOVA\_I\_B\_AES  | snova1ba              | EUF-CMA          |                    1 |                       656 |                        96 |                      388 |
|    SNOVA\_I\_S     | snova1s               | EUF-CMA          |                    1 |                      1016 |                        96 |                      272 |
|  SNOVA\_I\_S\_AES  | snova1sa              | EUF-CMA          |                    1 |                      1016 |                        96 |                      272 |
|  SNOVA\_I\_X\_AES  | snova1xa              | EUF-CMA          |                    1 |                      9842 |                        48 |                      150 |
|   SNOVA\_III\_K    | snova3k               | EUF-CMA          |                    3 |                       912 |                        96 |                      688 |
| SNOVA\_III\_K\_AES | snova3ka              | EUF-CMA          |                    3 |                       912 |                        96 |                      688 |
|   SNOVA\_III\_B    | snova3b               | EUF-CMA          |                    3 |                      1416 |                        96 |                      532 |
| SNOVA\_III\_B\_AES | snova3ba              | EUF-CMA          |                    3 |                      1416 |                        96 |                      532 |
|   SNOVA\_III\_S    | snova3s               | EUF-CMA          |                    3 |                      2032 |                        96 |                      456 |
| SNOVA\_III\_S\_AES | snova3sa              | EUF-CMA          |                    3 |                      2032 |                        96 |                      456 |
| SNOVA\_III\_X\_AES | snova3xa              | EUF-CMA          |                    3 |                     31266 |                        48 |                      218 |
|    SNOVA\_V\_K     | snova5k               | EUF-CMA          |                    5 |                      1216 |                        96 |                      896 |
|  SNOVA\_V\_K\_AES  | snova5ka              | EUF-CMA          |                    5 |                      1216 |                        96 |                      896 |
|    SNOVA\_V\_B     | snova5b               | EUF-CMA          |                    5 |                      1891 |                        96 |                      691 |
|  SNOVA\_V\_B\_AES  | snova5ba              | EUF-CMA          |                    5 |                      1891 |                        96 |                      691 |
|    SNOVA\_V\_S     | snova5s               | EUF-CMA          |                    5 |                      2716 |                        96 |                      591 |
|  SNOVA\_V\_S\_AES  | snova5sa              | EUF-CMA          |                    5 |                      2716 |                        96 |                      591 |

## SNOVA\_I\_K implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## SNOVA\_I\_K\_AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_I\_B implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_I\_B\_AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_I\_S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_I\_S\_AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_I\_X\_AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_III\_K implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_III\_K\_AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_III\_B implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_III\_B\_AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_III\_S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_III\_S\_AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_III\_X\_AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_V\_K implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_V\_K\_AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_V\_B implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_V\_B\_AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_V\_S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_V\_S\_AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.