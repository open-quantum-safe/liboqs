# CRYSTALS-Dilithium

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: hardness of lattice problems over module lattices.
- **Principal submitters**: Vadim Lyubashevsky.
- **Auxiliary submitters**: Shi Bai, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Peter Schwabe, Gregor Seiler, Damien Stehlé.
- **Authors' website**: https://pq-crystals.org/dilithium/
- **Specification version**: 3.1.
- **Implementation source**: https://github.com/pq-crystals/dilithium/commit/61b51a71701b8ae9f546a1e5d220e1950ed20d06 with copy_from_upstream patches
- **Implementation license (SPDX-Identifier)**: CC0-1.0.

## Parameter set summary

|  Parameter set  | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:---------------:|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|   Dilithium2    | EUF-CMA          |                    2 |                      1312 |                      2528 |                     2420 |
|   Dilithium3    | EUF-CMA          |                    3 |                      1952 |                      4000 |                     3293 |
|   Dilithium5    | EUF-CMA          |                    5 |                      2592 |                      4864 |                     4595 |
| Dilithium2-AES  | EUF-CMA          |                    2 |                      1312 |                      2528 |                     2420 |
| Dilithium3-AES  | EUF-CMA          |                    3 |                      1952 |                      4000 |                     3293 |
| Dilithium5-AES  | EUF-CMA          |                    5 |                      2592 |                      4864 |                     4595 |

## Dilithium2 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
|           ref            | All                         | All                             | None                    | True                               | True                                           | False                 |
|           avx2           | x86\_64                     | Darwin,Linux                    | AVX2,POPCNT             | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## Dilithium3 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|           ref            | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | Darwin,Linux                    | AVX2,POPCNT             | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Dilithium5 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|           ref            | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | Darwin,Linux                    | AVX2,POPCNT             | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Dilithium2-AES implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used      | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:---------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|           ref            | All                         | All                             | None                       | True                               | True                                           | False                |
|           avx2           | x86\_64                     | Darwin,Linux                    | AES,AVX2,POPCNT,SSE2,SSSE3 | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Dilithium3-AES implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used      | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:---------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|           ref            | All                         | All                             | None                       | True                               | True                                           | False                |
|           avx2           | x86\_64                     | Darwin,Linux                    | AES,AVX2,POPCNT,SSE2,SSSE3 | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Dilithium5-AES implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used      | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:---------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|           ref            | All                         | All                             | None                       | True                               | True                                           | False                |
|           avx2           | x86\_64                     | Darwin,Linux                    | AES,AVX2,POPCNT,SSE2,SSSE3 | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.