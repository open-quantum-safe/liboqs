# CROSS

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: hardness of the restricted syndrome decoding problem for random linear codes on a finite field.
- **Principal submitters**: Marco Baldi, Alessandro Barenghi, Michele Battagliola, Sebastian Bitzer, Patrick Karl, Felice Manganiello, Alessio Pavoni, Gerardo Pelosi, Federico Pintore, Paolo Santini, Jonas Schupp, Edoardo Signorini, Freeman Slaughter, Antonia Wachter-Zeh, Violetta Weger.
- **Auxiliary submitters**: Marco Gianvecchio.
- **Authors' website**: https://www.cross-crypto.com/
- **Specification version**: 2.2 + PQClean and OQS patches.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/CROSS-signature/CROSS-lib-oqs/commit/a21ebc314e06b0972a9bbcf2813a185ecb2917f1
  - **Implementation license (SPDX-Identifier)**: CC0-1.0
- **Optimized Implementation sources**:
  - **avx2**:<a name="avx2"></a>
      - **Source**: https://github.com/CROSS-signature/CROSS-lib-oqs/commit/a21ebc314e06b0972a9bbcf2813a185ecb2917f1
      - **Implementation license (SPDX-Identifier)**: CC0-1.0


## Parameter set summary

|      Parameter set       | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:------------------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
| cross-rsdp-128-balanced  | NA                    | EUF-CMA          |                    1 |                        77 |                        32 |                    13152 |
|   cross-rsdp-128-fast    | NA                    | EUF-CMA          |                    1 |                        77 |                        32 |                    18432 |
|   cross-rsdp-128-small   | NA                    | EUF-CMA          |                    1 |                        77 |                        32 |                    12432 |
| cross-rsdp-192-balanced  | NA                    | EUF-CMA          |                    3 |                       115 |                        48 |                    29853 |
|   cross-rsdp-192-fast    | NA                    | EUF-CMA          |                    3 |                       115 |                        48 |                    41406 |
|   cross-rsdp-192-small   | NA                    | EUF-CMA          |                    3 |                       115 |                        48 |                    28391 |
| cross-rsdp-256-balanced  | NA                    | EUF-CMA          |                    5 |                       153 |                        64 |                    53527 |
|   cross-rsdp-256-fast    | NA                    | EUF-CMA          |                    5 |                       153 |                        64 |                    74590 |
|   cross-rsdp-256-small   | NA                    | EUF-CMA          |                    5 |                       153 |                        64 |                    50818 |
| cross-rsdpg-128-balanced | NA                    | EUF-CMA          |                    1 |                        54 |                        32 |                     9120 |
|   cross-rsdpg-128-fast   | NA                    | EUF-CMA          |                    1 |                        54 |                        32 |                    11980 |
|  cross-rsdpg-128-small   | NA                    | EUF-CMA          |                    1 |                        54 |                        32 |                     8960 |
| cross-rsdpg-192-balanced | NA                    | EUF-CMA          |                    3 |                        83 |                        48 |                    22464 |
|   cross-rsdpg-192-fast   | NA                    | EUF-CMA          |                    3 |                        83 |                        48 |                    26772 |
|  cross-rsdpg-192-small   | NA                    | EUF-CMA          |                    3 |                        83 |                        48 |                    20452 |
| cross-rsdpg-256-balanced | NA                    | EUF-CMA          |                    5 |                       106 |                        64 |                    40100 |
|   cross-rsdpg-256-fast   | NA                    | EUF-CMA          |                    5 |                       106 |                        64 |                    48102 |
|  cross-rsdpg-256-small   | NA                    | EUF-CMA          |                    5 |                       106 |                        64 |                    36454 |

## cross-rsdp-128-balanced implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## cross-rsdp-128-fast implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## cross-rsdp-128-small implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## cross-rsdp-192-balanced implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## cross-rsdp-192-fast implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## cross-rsdp-192-small implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## cross-rsdp-256-balanced implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## cross-rsdp-256-fast implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## cross-rsdp-256-small implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## cross-rsdpg-128-balanced implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## cross-rsdpg-128-fast implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## cross-rsdpg-128-small implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## cross-rsdpg-192-balanced implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## cross-rsdpg-192-fast implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## cross-rsdpg-192-small implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## cross-rsdpg-256-balanced implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## cross-rsdpg-256-fast implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## cross-rsdpg-256-small implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | True                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.