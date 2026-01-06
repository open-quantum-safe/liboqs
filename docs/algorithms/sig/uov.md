# UOV

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: multivariable quadratic equations, oil and vinegar.
- **Principal submitters**: Ward Beullens, Ming-Shing Chen, Jintai Ding, Boru Gong, Matthias J. Kannwischer, Jacques Patarin, Bo-Yuan Peng, Dieter Schmidt, Cheng-Jhih Shih, Chengdong Tao, Bo-Yin Yang.
- **Authors' website**: https://www.uovsig.org/
- **Specification version**: NIST Round 2 (February 2025).
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/pqov/pqov/commit/33fa5278754a32064c55901c3a17d48b06cc2351
  - **Implementation license (SPDX-Identifier)**: CC0 OR Apache-2.0
- **Optimized Implementation sources**:
  - **neon**:<a name="neon"></a>
      - **Source**: https://github.com/pqov/pqov/commit/33fa5278754a32064c55901c3a17d48b06cc2351
      - **Implementation license (SPDX-Identifier)**: CC0 OR Apache-2.0
  - **avx2**:<a name="avx2"></a>
      - **Source**: https://github.com/pqov/pqov/commit/33fa5278754a32064c55901c3a17d48b06cc2351
      - **Implementation license (SPDX-Identifier)**: CC0 OR Apache-2.0


## Parameter set summary

|  Parameter set  | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:---------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|      OV-Is      | NA                    | EUF-CMA          |                    1 |                    412160 |                    348704 |                       96 |
|      OV-Ip      | NA                    | EUF-CMA          |                    1 |                    278432 |                    237896 |                      128 |
|     OV-III      | NA                    | EUF-CMA          |                    3 |                   1225440 |                   1044320 |                      200 |
|      OV-V       | NA                    | EUF-CMA          |                    5 |                   2869440 |                   2436704 |                      260 |
|    OV-Is-pkc    | NA                    | EUF-CMA          |                    1 |                     66576 |                    348704 |                       96 |
|    OV-Ip-pkc    | NA                    | EUF-CMA          |                    1 |                     43576 |                    237896 |                      128 |
|   OV-III-pkc    | NA                    | EUF-CMA          |                    3 |                    189232 |                   1044320 |                      200 |
|    OV-V-pkc     | NA                    | EUF-CMA          |                    5 |                    446992 |                   2436704 |                      260 |
|  OV-Is-pkc-skc  | NA                    | EUF-CMA          |                    1 |                     66576 |                        32 |                       96 |
|  OV-Ip-pkc-skc  | NA                    | EUF-CMA          |                    1 |                     43576 |                        32 |                      128 |
| OV-III-pkc-skc  | NA                    | EUF-CMA          |                    3 |                    189232 |                        32 |                      200 |
|  OV-V-pkc-skc   | NA                    | EUF-CMA          |                    5 |                    446992 |                        32 |                      260 |

## OV-Is implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## OV-Ip implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## OV-III implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## OV-V implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## OV-Is-pkc implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## OV-Ip-pkc implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## OV-III-pkc implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## OV-V-pkc implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## OV-Is-pkc-skc implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## OV-Ip-pkc-skc implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## OV-III-pkc-skc implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## OV-V-pkc-skc implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.