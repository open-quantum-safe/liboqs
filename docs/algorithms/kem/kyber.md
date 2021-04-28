# Kyber

- **Algorithm type**: Key encapsulation mechanism.
- **Main cryptographic assumption**: Module LWE+R with base ring Z[x]/(3329, x^256+1).
- **Principal submitters**: Peter Schwabe.
- **Auxiliary submitters**: Roberto Avanzi, Joppe Bos, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Vadim Lyubashevsky, John M. Schanck, Gregor Seiler, Damien Stehlé.
- **Authors website**: https://pq-crystals.org/
- **Specification version**: NIST Round 3 submission.
- **Implementation source**: https://github.com/pq-crystals/kyber/commit/8e9308bd with copy_from_upstream patches
- **Implementation license (SPDX-Identifier)**: CC0-1.0.

## Parameter set summary

|  Parameter set  | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Ciphertext size (bytes) |   Shared secret size (bytes) |
|:---------------:|:-----------------|---------------------:|--------------------------:|--------------------------:|--------------------------:|-----------------------------:|
|    Kyber512     | IND-CCA2         |                    1 |                       800 |                      1632 |                       768 |                           32 |
|  Kyber512-90s   | IND-CCA2         |                    1 |                       800 |                      1632 |                       768 |                           32 |
|    Kyber768     | IND-CCA2         |                    3 |                      1184 |                      2400 |                      1088 |                           32 |
|  Kyber768-90s   | IND-CCA2         |                    3 |                      1184 |                      2400 |                      1088 |                           32 |
|    Kyber1024    | IND-CCA2         |                    5 |                      1568 |                      3168 |                      1568 |                           32 |
|  Kyber1024-90s  | IND-CCA2         |                    5 |                      1568 |                      3168 |                      1568 |                           32 |

## Kyber512 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|           ref            | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86_64                      | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Kyber512-90s implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|           ref            | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86_64                      | Linux,Darwin                    | AES,AVX2,BMI2,POPCNT    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Kyber768 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|           ref            | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86_64                      | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Kyber768-90s implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|           ref            | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86_64                      | Linux,Darwin                    | AES,AVX2,BMI2,POPCNT    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Kyber1024 implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|           ref            | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86_64                      | Linux,Darwin                    | AVX2,BMI2,POPCNT        | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Kyber1024-90s implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|           ref            | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86_64                      | Linux,Darwin                    | AES,AVX2,BMI2,POPCNT    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.
