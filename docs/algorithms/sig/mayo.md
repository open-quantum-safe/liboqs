# MAYO

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: multivariable quadratic equations, oil and vinegar.
- **Principal submitters**: Ward Beullens, Fabio Campos, Sofía Celi, Basil Hess, Matthias J. Kannwischer.
- **Authors' website**: https://pqmayo.org
- **Specification version**: NIST Round 3 (September 2026).
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/PQCMayo/MAYO-C/commit/74de637a7c4a7f9fafa888183bc05f5cf920cd2d with copy_from_upstream patches
  - **Implementation license (SPDX-Identifier)**: Apache-2.0
- **Optimized Implementation sources**:
  - **avx2**:<a name="avx2"></a>
      - **Source**: https://github.com/PQCMayo/MAYO-C/commit/74de637a7c4a7f9fafa888183bc05f5cf920cd2d with copy_from_upstream patches
      - **Implementation license (SPDX-Identifier)**: Apache-2.0
  - **neon**:<a name="neon"></a>
      - **Source**: https://github.com/PQCMayo/MAYO-C/commit/74de637a7c4a7f9fafa888183bc05f5cf920cd2d with copy_from_upstream patches
      - **Implementation license (SPDX-Identifier)**: Apache-2.0


## Support

- **Upstream maintenance**: Actively maintained ([statement](https://github.com/PQCMayo/MAYO-C))
- **OQS support tier**: Tier 3

## Parameter set summary

|  Parameter set  | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:---------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|     MAYO-1      | NA                    | EUF-CMA          |                    1 |                      1456 |                        24 |                      464 |
|     MAYO-2      | NA                    | EUF-CMA          |                    1 |                      2928 |                        24 |                      239 |
|     MAYO-3      | NA                    | EUF-CMA          |                    3 |                      2986 |                        32 |                      681 |
|     MAYO-5      | NA                    | EUF-CMA          |                    5 |                      5554 |                        40 |                      964 |

## MAYO-1 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Darwin,Linux                    | AVX2                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | False                                          | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## MAYO-2 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Darwin,Linux                    | AVX2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## MAYO-3 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Darwin,Linux                    | AVX2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## MAYO-5 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | opt                      | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Darwin,Linux                    | AVX2                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | neon                     | ARM64\_V8                   | Darwin,Linux                    | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.