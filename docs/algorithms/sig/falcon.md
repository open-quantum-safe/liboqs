# Falcon

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: hardness of NTRU lattice problems.
- **Principal submitters**: Thomas Prest.
- **Auxiliary submitters**: Pierre-Alain Fouque, Jeffrey Hoffstein, Paul Kirchner, Vadim Lyubashevsky, Thomas Pornin, Thomas Prest, Thomas Ricosset, Gregor Seiler, William Whyte, Zhenfei Zhang.
- **Authors' website**: https://falcon-sign.info
- **Specification version**: 20211101.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/PQClean/PQClean/commit/0657749a785db30e7f49e9435452cb042edb1852
  - **Implementation license (SPDX-Identifier)**: MIT


## Parameter set summary

|  Parameter set  | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:---------------:|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|   Falcon-512    | EUF-CMA          |                    1 |                       897 |                      1281 |                      666 |
|   Falcon-1024   | EUF-CMA          |                    5 |                      1793 |                      2305 |                     1280 |

## Falcon-512 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | False                              | False                                          | False                 |
| [Primary Source](#primary-source) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | False                              | False                                          | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## Falcon-1024 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | aarch64                  | ARM64\_V8                   | Linux,Darwin                    | None                    | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.