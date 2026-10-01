# QR-UOV

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: multivariate quadratic equations, oil and vinegar.
- **Principal submitters**: Rika Akiyama, Hiroki Furue, Yasuhiko Ikematsu, Fumitaka Hoshino, Koha Kinjo, Haruhisa Kosuge, Satoshi Nakamura, Shingo Orihara, Tsuyoshi Takagi, Kimihiro Yamakoshi.
- **Authors' website**: https://info.isl.ntt.co.jp/crypt/eng/qr-uov/
- **Specification version**: NIST Round 3.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/0rlych1kk4/round3/commit/36e9a7efadd6bfbba5e8d619507eb158ac60ad16
  - **Implementation license (SPDX-Identifier)**: 0BSD
- **Optimized Implementation sources**:
  - **opt**:<a name="opt"></a>
      - **Source**: https://github.com/0rlych1kk4/round3/commit/36e9a7efadd6bfbba5e8d619507eb158ac60ad16
      - **Implementation license (SPDX-Identifier)**: 0BSD
  - **avx2**:<a name="avx2"></a>
      - **Source**: https://github.com/0rlych1kk4/round3/commit/36e9a7efadd6bfbba5e8d619507eb158ac60ad16
      - **Implementation license (SPDX-Identifier)**: 0BSD


## Support

- **Upstream maintenance**: Best effort
- **OQS support tier**: Tier 3

## Parameter set summary

|    Parameter set     | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:--------------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|  QRUOV-1q127L10-aes  | NA                    | EUF-CMA          |                    1 |                     11041 |                        32 |                      541 |
| QRUOV-1q127L10-shake | NA                    | EUF-CMA          |                    1 |                     11041 |                        32 |                      541 |
|  QRUOV-1q127L3-aes   | NA                    | EUF-CMA          |                    1 |                     24256 |                        32 |                      200 |
| QRUOV-1q127L3-shake  | NA                    | EUF-CMA          |                    1 |                     24256 |                        32 |                      200 |
|  QRUOV-1q31L10-aes   | NA                    | EUF-CMA          |                    1 |                     12266 |                        32 |                      435 |
| QRUOV-1q31L10-shake  | NA                    | EUF-CMA          |                    1 |                     12266 |                        32 |                      435 |
|   QRUOV-1q31L3-aes   | NA                    | EUF-CMA          |                    1 |                     23641 |                        32 |                      157 |
|  QRUOV-1q31L3-shake  | NA                    | EUF-CMA          |                    1 |                     23641 |                        32 |                      157 |
|   QRUOV-1q7L10-aes   | NA                    | EUF-CMA          |                    1 |                     20641 |                        32 |                      331 |
|  QRUOV-1q7L10-shake  | NA                    | EUF-CMA          |                    1 |                     20641 |                        32 |                      331 |
|  QRUOV-3q127L10-aes  | NA                    | EUF-CMA          |                    3 |                     35454 |                        32 |                      821 |
| QRUOV-3q127L10-shake | NA                    | EUF-CMA          |                    3 |                     35454 |                        32 |                      821 |
|  QRUOV-3q127L3-aes   | NA                    | EUF-CMA          |                    3 |                     71884 |                        32 |                      292 |
| QRUOV-3q127L3-shake  | NA                    | EUF-CMA          |                    3 |                     71884 |                        32 |                      292 |
|  QRUOV-3q31L10-aes   | NA                    | EUF-CMA          |                    3 |                     34391 |                        32 |                      643 |
| QRUOV-3q31L10-shake  | NA                    | EUF-CMA          |                    3 |                     34391 |                        32 |                      643 |
|   QRUOV-3q31L3-aes   | NA                    | EUF-CMA          |                    3 |                     70976 |                        32 |                      233 |
|  QRUOV-3q31L3-shake  | NA                    | EUF-CMA          |                    3 |                     70976 |                        32 |                      233 |
|   QRUOV-3q7L10-aes   | NA                    | EUF-CMA          |                    3 |                     55141 |                        32 |                      489 |
|  QRUOV-3q7L10-shake  | NA                    | EUF-CMA          |                    3 |                     55141 |                        32 |                      489 |
|  QRUOV-5q127L10-aes  | NA                    | EUF-CMA          |                    5 |                     63541 |                        32 |                     1039 |
| QRUOV-5q127L10-shake | NA                    | EUF-CMA          |                    5 |                     63541 |                        32 |                     1039 |
|  QRUOV-5q127L3-aes   | NA                    | EUF-CMA          |                    5 |                    173660 |                        32 |                      392 |
| QRUOV-5q127L3-shake  | NA                    | EUF-CMA          |                    5 |                    173660 |                        32 |                      392 |
|  QRUOV-5q31L10-aes   | NA                    | EUF-CMA          |                    5 |                     58516 |                        32 |                      807 |
| QRUOV-5q31L10-shake  | NA                    | EUF-CMA          |                    5 |                     58516 |                        32 |                      807 |
|   QRUOV-5q31L3-aes   | NA                    | EUF-CMA          |                    5 |                    158405 |                        32 |                      306 |
|  QRUOV-5q31L3-shake  | NA                    | EUF-CMA          |                    5 |                    158405 |                        32 |                      306 |
|   QRUOV-5q7L10-aes   | NA                    | EUF-CMA          |                    5 |                    135391 |                        32 |                      662 |
|  QRUOV-5q7L10-shake  | NA                    | EUF-CMA          |                    5 |                    135391 |                        32 |                      662 |

## QRUOV-1q127L10-aes implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## QRUOV-1q127L10-shake implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-1q127L3-aes implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-1q127L3-shake implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-1q31L10-aes implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-1q31L10-shake implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-1q31L3-aes implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-1q31L3-shake implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-1q7L10-aes implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-1q7L10-shake implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-3q127L10-aes implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-3q127L10-shake implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-3q127L3-aes implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-3q127L3-shake implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-3q31L10-aes implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-3q31L10-shake implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-3q31L3-aes implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-3q31L3-shake implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-3q7L10-aes implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-3q7L10-shake implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-5q127L10-aes implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-5q127L10-shake implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-5q127L3-aes implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-5q127L3-shake implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-5q31L10-aes implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-5q31L10-shake implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-5q31L3-aes implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-5q31L3-shake implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-5q7L10-aes implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## QRUOV-5q7L10-shake implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,AES           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.