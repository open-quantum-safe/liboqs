# Picnic

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: hash function security (ROM/QROM), key recovery attacks on the lowMC block cipher.
- **Principal submitters**: Greg Zaverucha, Melissa Chase, David Derler, Steven Goldfeder, Claudio Orlandi, Sebastian Ramacher, Christian Rechberger, Daniel Slamanig, Jonathan Katz, Xiao Wang, Vladmir Kolesnikov.
- **Authors' website**: https://microsoft.github.io/Picnic/
- **Specification version**: 3.0.5.
- **Implementation source**: https://github.com/IAIK/Picnic/tree/v3.0.5
- **Implementation license (SPDX-Identifier)**: MIT.

## Parameter set summary

|  Parameter set   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:----------------:|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|  picnic\_L1\_FS  | EUF-CMA          |                    1 |                        33 |                        49 |                    34036 |
|  picnic\_L1\_UR  | EUF-CMA          |                    1 |                        33 |                        49 |                    53965 |
| picnic\_L1\_full | EUF-CMA          |                    1 |                        35 |                        52 |                    32065 |
|  picnic\_L3\_FS  | EUF-CMA          |                    3 |                        49 |                        73 |                    76776 |
|  picnic\_L3\_UR  | EUF-CMA          |                    3 |                        49 |                        73 |                   121849 |
| picnic\_L3\_full | EUF-CMA          |                    3 |                        49 |                        73 |                    71183 |
|  picnic\_L5\_FS  | EUF-CMA          |                    5 |                        65 |                        97 |                   132860 |
|  picnic\_L5\_UR  | EUF-CMA          |                    5 |                        65 |                        97 |                   209510 |
| picnic\_L5\_full | EUF-CMA          |                    5 |                        65 |                        97 |                   126290 |
|   picnic3\_L1    | EUF-CMA          |                    1 |                        35 |                        52 |                    14612 |
|   picnic3\_L3    | EUF-CMA          |                    3 |                        49 |                        73 |                    35028 |
|   picnic3\_L5    | EUF-CMA          |                    5 |                        65 |                        97 |                    61028 |

## picnic\_L1\_FS implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                 |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                 |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## picnic\_L1\_UR implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## picnic\_L1\_full implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## picnic\_L3\_FS implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## picnic\_L3\_UR implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## picnic\_L3\_full implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## picnic\_L5\_FS implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## picnic\_L5\_UR implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## picnic\_L5\_full implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## picnic3\_L1 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## picnic3\_L3 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## picnic3\_L5 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          master          | All                         | All                             | None                    | True                               | True                                           | False                |
|          master          | x86\_64                     | Linux                           | AVX2,SSE2               | True                               | True                                           | False                |
|          master          | x86\_64                     | Darwin,Windows                  | SSE2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.