# MQOM

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: Computing the solution of a Multivariate Quadratic problem..
- **Principal submitters**: Ryad Benadjila, Charles Bouillaguet, Thibauld Feneuil, Matthieu Rivain.
- **Authors' website**: https://mqom.org/
- **Specification version**: NIST Round 3.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/mqom/mqom-v3/commit/df26239dd22f194371003f403ec1a9920772b769
  - **Implementation license (SPDX-Identifier)**: MIT
- **Optimized Implementation sources**:
  - **memopt**:<a name="memopt"></a>
      - **Source**: https://github.com/mqom/mqom-v3/commit/df26239dd22f194371003f403ec1a9920772b769
      - **Implementation license (SPDX-Identifier)**: MIT
  - **avx2**:<a name="avx2"></a>
      - **Source**: https://github.com/mqom/mqom-v3/commit/df26239dd22f194371003f403ec1a9920772b769
      - **Implementation license (SPDX-Identifier)**: MIT
  - **neon**:<a name="neon"></a>
      - **Source**: https://github.com/mqom/mqom-v3/commit/df26239dd22f194371003f403ec1a9920772b769
      - **Implementation license (SPDX-Identifier)**: MIT


## Support

- **Upstream maintenance**: Actively maintained ([statement](https://github.com/mqom/mqom-v3/blob/main/integration/liboqs/SUPPORT.md))
- **OQS support tier**: Tier 3

## Parameter set summary

|         Parameter set         | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:-----------------------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|  mqom3\_cat1\_gf16\_fast\_ct  | NA                    | EUF-CMA          |                    1 |                        64 |                        96 |                     3316 |
|  mqom3\_cat1\_gf16\_fast\_ot  | NA                    | EUF-CMA          |                    1 |                        64 |                        96 |                     3316 |
| mqom3\_cat1\_gf16\_short\_ct  | NA                    | EUF-CMA          |                    1 |                        64 |                        96 |                     2932 |
| mqom3\_cat1\_gf16\_short\_ot  | NA                    | EUF-CMA          |                    1 |                        64 |                        96 |                     2932 |
| mqom3\_cat1\_gf2\_shorter\_ct | NA                    | EUF-CMA          |                    1 |                        52 |                        72 |                     2492 |
| mqom3\_cat1\_gf2\_shorter\_ot | NA                    | EUF-CMA          |                    1 |                        52 |                        72 |                     2492 |
|  mqom3\_cat3\_gf16\_fast\_ct  | NA                    | EUF-CMA          |                    3 |                        96 |                       144 |                     7564 |
|  mqom3\_cat3\_gf16\_fast\_ot  | NA                    | EUF-CMA          |                    3 |                        96 |                       144 |                     7660 |
| mqom3\_cat3\_gf16\_short\_ct  | NA                    | EUF-CMA          |                    3 |                        96 |                       144 |                     6556 |
| mqom3\_cat3\_gf16\_short\_ot  | NA                    | EUF-CMA          |                    3 |                        96 |                       144 |                     6556 |
| mqom3\_cat3\_gf2\_shorter\_ct | NA                    | EUF-CMA          |                    3 |                        78 |                       108 |                     5932 |
| mqom3\_cat3\_gf2\_shorter\_ot | NA                    | EUF-CMA          |                    3 |                        78 |                       108 |                     5890 |
|  mqom3\_cat5\_gf16\_fast\_ct  | NA                    | EUF-CMA          |                    5 |                       128 |                       192 |                    13540 |
|  mqom3\_cat5\_gf16\_fast\_ot  | NA                    | EUF-CMA          |                    5 |                       128 |                       192 |                    13380 |
| mqom3\_cat5\_gf16\_short\_ct  | NA                    | EUF-CMA          |                    5 |                       128 |                       192 |                    12100 |
| mqom3\_cat5\_gf16\_short\_ot  | NA                    | EUF-CMA          |                    5 |                       128 |                       192 |                    11716 |
| mqom3\_cat5\_gf2\_shorter\_ct | NA                    | EUF-CMA          |                    5 |                       104 |                       144 |                    10836 |
| mqom3\_cat5\_gf2\_shorter\_ot | NA                    | EUF-CMA          |                    5 |                       104 |                       144 |                    10804 |

## mqom3\_cat1\_gf16\_fast\_ct implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                  |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                  |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                  |

Are implementations chosen based on runtime CPU feature detection? **No**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## mqom3\_cat1\_gf16\_fast\_ot implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom3\_cat1\_gf16\_short\_ct implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom3\_cat1\_gf16\_short\_ot implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom3\_cat1\_gf2\_shorter\_ct implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom3\_cat1\_gf2\_shorter\_ot implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom3\_cat3\_gf16\_fast\_ct implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom3\_cat3\_gf16\_fast\_ot implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom3\_cat3\_gf16\_short\_ct implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom3\_cat3\_gf16\_short\_ot implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom3\_cat3\_gf2\_shorter\_ct implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom3\_cat3\_gf2\_shorter\_ot implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom3\_cat5\_gf16\_fast\_ct implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom3\_cat5\_gf16\_fast\_ot implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom3\_cat5\_gf16\_short\_ct implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom3\_cat5\_gf16\_short\_ot implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom3\_cat5\_gf2\_shorter\_ct implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## mqom3\_cat5\_gf2\_shorter\_ot implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | default                  | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | memopt                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2,AES                | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Linux,Darwin                    | SHA3                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.