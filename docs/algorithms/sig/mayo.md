# MAYO

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: multivariable quadratic equations, oil and vinegar.
- **Principal submitters**: Ward Beullens, Fabio Campos, Sofía Celi, Basil Hess, Matthias J. Kannwischer.
- **Authors' website**: https://pqmayo.org
- **Specification version**: NIST Round 2 (February 2025).
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/PQCMayo/MAYO-C/commit/4b7cd94c96b9522864efe40c6ad1fa269584a807 with copy_from_upstream patches
  - **Implementation license (SPDX-Identifier)**: Apache-2.0
- **Optimized Implementation sources**:
  - **avx2**:<a name="avx2"></a>
      - **Source**: https://github.com/PQCMayo/MAYO-C/commit/4b7cd94c96b9522864efe40c6ad1fa269584a807 with copy_from_upstream patches
      - **Implementation license (SPDX-Identifier)**: Apache-2.0
  - **neon**:<a name="neon"></a>
      - **Source**: https://github.com/PQCMayo/MAYO-C/commit/4b7cd94c96b9522864efe40c6ad1fa269584a807 with copy_from_upstream patches
      - **Implementation license (SPDX-Identifier)**: Apache-2.0


## Parameter set summary

|  Parameter set  | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:---------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|     MAYO-1      | NA                    | EUF-CMA          |                    1 |                      1420 |                        24 |                      454 |
|     MAYO-2      | NA                    | EUF-CMA          |                    1 |                      4912 |                        24 |                      186 |
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