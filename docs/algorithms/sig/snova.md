# SNOVA

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: multivariable quadratic equations, oil and vinegar.
- **Principal submitters**: SNOVA team.
- **Authors' website**: https://snova.pqclab.org/
- **Specification version**: Round 2.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/vacuas/SNOVA/commit/ce243c2570727f837fbd5aeb091e96ba3880893b
  - **Implementation license (SPDX-Identifier)**: MIT


## Parameter set summary

|        Parameter set        | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:---------------------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|       SNOVA\_24\_5\_4       | NA                    | EUF-CMA          |                    1 |                      1016 |                        48 |                      248 |
|   SNOVA\_24\_5\_4\_SHAKE    | NA                    | EUF-CMA          |                    1 |                      1016 |                        48 |                      248 |
|    SNOVA\_24\_5\_4\_esk     | NA                    | EUF-CMA          |                    1 |                      1016 |                     36848 |                      248 |
| SNOVA\_24\_5\_4\_SHAKE\_esk | NA                    | EUF-CMA          |                    1 |                      1016 |                     36848 |                      248 |
|      SNOVA\_37\_17\_2       | NA                    | EUF-CMA          |                    1 |                      9842 |                        48 |                      124 |
|       SNOVA\_25\_8\_3       | NA                    | EUF-CMA          |                    1 |                      2320 |                        48 |                      165 |
|      SNOVA\_56\_25\_2       | NA                    | EUF-CMA          |                    3 |                     31266 |                        48 |                      178 |
|      SNOVA\_49\_11\_3       | NA                    | EUF-CMA          |                    3 |                      6006 |                        48 |                      286 |
|       SNOVA\_37\_8\_4       | NA                    | EUF-CMA          |                    3 |                      4112 |                        48 |                      376 |
|       SNOVA\_24\_5\_5       | NA                    | EUF-CMA          |                    3 |                      1579 |                        48 |                      379 |
|      SNOVA\_60\_10\_4       | NA                    | EUF-CMA          |                    5 |                      8016 |                        48 |                      576 |
|       SNOVA\_29\_6\_5       | NA                    | EUF-CMA          |                    5 |                      2716 |                        48 |                      454 |

## SNOVA\_24\_5\_4 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | False                                          | False                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | False                                          | False                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | False                                          | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## SNOVA\_24\_5\_4\_SHAKE implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_24\_5\_4\_esk implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_24\_5\_4\_SHAKE\_esk implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_37\_17\_2 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | False                                          | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | False                                          | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_25\_8\_3 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | False                                          | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_56\_25\_2 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | False                                          | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | False                                          | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_49\_11\_3 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | False                                          | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | False                                          | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_37\_8\_4 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | False                                          | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | False                                          | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_24\_5\_5 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | False                                          | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | False                                          | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_60\_10\_4 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | False                                          | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | False                                          | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SNOVA\_29\_6\_5 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | False                                          | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux                           | AVX2                    | True                               | False                                          | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.