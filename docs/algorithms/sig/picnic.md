# Picnic

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: hash function security (ROM/QROM), key recovery attacks on the lowMC block cipher.
- **Principal submitters**: Greg Zaverucha, Melissa Chase, David Derler, Steven Goldfeder, Claudio Orlandi, Sebastian Ramacher, Christian Rechberger, Daniel Slamanig, Jonathan Katz, Xiao Wang, Vladmir Kolesnikov.
- **Authors' website**: https://microsoft.github.io/Picnic/
- **Specification version**: 3.0.3.
- **Implementation source**: https://github.com/IAIK/Picnic/tree/v3.0.4
- **Implementation license (SPDX-Identifier)**: MIT.

## Parameter set summary

|  Parameter set  | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:---------------:|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|  Picnic-L1-FS   | EUF-CMA          |                    1 |                        33 |                        49 |                    34036 |
|  Picnic-L1-UR   | EUF-CMA          |                    1 |                        33 |                        49 |                    53965 |
| Picnic-L1-full  | EUF-CMA          |                    1 |                        35 |                        52 |                    32065 |
|  Picnic-L3-FS   | EUF-CMA          |                    3 |                        49 |                        73 |                    76776 |
|  Picnic-L3-UR   | EUF-CMA          |                    3 |                        49 |                        73 |                   121849 |
| Picnic-L3-full  | EUF-CMA          |                    3 |                        49 |                        73 |                    71183 |
|  Picnic-L5-FS   | EUF-CMA          |                    5 |                        65 |                        97 |                   132860 |
|  Picnic-L5-UR   | EUF-CMA          |                    5 |                        65 |                        97 |                   209510 |
| Picnic-L5-full  | EUF-CMA          |                    5 |                        65 |                        97 |                   126290 |
|   Picnic3-L1    | EUF-CMA          |                    1 |                        35 |                        52 |                    14612 |
|   Picnic3-L3    | EUF-CMA          |                    3 |                        49 |                        73 |                    35028 |
|   Picnic3-L5    | EUF-CMA          |                    5 |                        65 |                        97 |                    61028 |

## Picnic-L1-FS implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                 |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                 |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## Picnic-L1-UR implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Picnic-L1-full implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Picnic-L3-FS implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Picnic-L3-UR implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Picnic-L3-full implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Picnic-L5-FS implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Picnic-L5-UR implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Picnic-L5-full implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Picnic3-L1 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Picnic3-L3 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Picnic3-L5 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.