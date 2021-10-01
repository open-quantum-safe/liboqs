# NTRU-Prime

- **Algorithm type**: Key encapsulation mechanism.
- **Main cryptographic assumption**: NTRU.
- **Principal submitters**: Daniel J. Bernstein, Billy Bob Brumley, Ming-Shing Chen, Chitchanok Chuengsatiansup, Tanja Lange, Adrian Marotzke, Bo-Yuan Peng, Nicola Tuveri, Christine van Vredendaal, Bo-Yin Yang.
- **Authors' website**: https://ntruprime.cr.yp.to
- **Specification version**: supercop-20200826.
- **Implementation source**: https://github.com/PQClean/PQClean/commit/5b8ef3baea3ffdfbf688a3a1bb8f02de44a67ec0, which takes it from:
  - https://github.com/jschanck/package-pqclean/tree/4d9f08c3/ntruprime, which takes it from:
  - supercop-20210604
- **Implementation license (SPDX-Identifier)**: Public domain.

## Parameter set summary

|  Parameter set  | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Ciphertext size (bytes) |   Shared secret size (bytes) |
|:---------------:|:-----------------|---------------------:|--------------------------:|--------------------------:|--------------------------:|-----------------------------:|
|   ntrulpr653    | IND-CCA2         |                    1 |                       897 |                      1125 |                      1025 |                           32 |
|   ntrulpr761    | IND-CCA2         |                    2 |                      1039 |                      1294 |                      1167 |                           32 |
|   ntrulpr857    | IND-CCA2         |                    3 |                      1184 |                      1463 |                      1312 |                           32 |
|    sntrup653    | IND-CCA2         |                    1 |                       994 |                      1518 |                       897 |                           32 |
|    sntrup761    | IND-CCA2         |                    2 |                      1158 |                      1763 |                      1039 |                           32 |
|    sntrup857    | IND-CCA2         |                    3 |                      1322 |                      1999 |                      1184 |                           32 |

## ntrulpr653 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                 |
|           avx2           | x86\_64                     | Linux,Darwin                    | AVX2                    | False                              | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## ntrulpr761 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | Linux,Darwin                    | AVX2                    | False                              | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## ntrulpr857 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | Linux,Darwin                    | AVX2                    | False                              | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## sntrup653 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | Linux,Darwin                    | AVX2                    | False                              | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## sntrup761 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | Linux,Darwin                    | AVX2                    | False                              | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## sntrup857 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | Linux,Darwin                    | AVX2                    | False                              | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.