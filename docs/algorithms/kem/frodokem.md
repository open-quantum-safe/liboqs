# FrodoKEM

- **Algorithm type**: Key encapsulation mechanism.
- **Main cryptographic assumption**: learning with errors (LWE).
- **Principal submitters**: Michael Naehrig, Erdem Alkim, Joppe Bos, Léo Ducas, Karen Easterbrook, Brian LaMacchia, Patrick Longa, Ilya Mironov, Valeria Nikolaenko, Christopher Peikert, Ananth Raghunathan, Douglas Stebila.
- **Authors' website**: https://frodokem.org/
- **Specification version**: ISO Preliminary Standardization Proposal - 2023/03/14.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/microsoft/PQCrypto-LWEKE/commit/a2f9dec8917ccc3464b3378d46b140fa7353320d
  - **Implementation license (SPDX-Identifier)**: MIT


## Parameter set summary

|    Parameter set     | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Ciphertext size (bytes) |   Shared secret size (bytes) | Keypair seed size (bytes)   | Encapsulation seed size (bytes)   |
|:--------------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|--------------------------:|-----------------------------:|:----------------------------|:----------------------------------|
|   FrodoKEM-640-AES   | NA                    | IND-CCA2         |                    1 |                      9616 |                     19888 |                      9752 |                           16 | NA                          | NA                                |
|  FrodoKEM-640-SHAKE  | NA                    | IND-CCA2         |                    1 |                      9616 |                     19888 |                      9752 |                           16 | NA                          | NA                                |
|   FrodoKEM-976-AES   | NA                    | IND-CCA2         |                    3 |                     15632 |                     31296 |                     15792 |                           24 | NA                          | NA                                |
|  FrodoKEM-976-SHAKE  | NA                    | IND-CCA2         |                    3 |                     15632 |                     31296 |                     15792 |                           24 | NA                          | NA                                |
|  FrodoKEM-1344-AES   | NA                    | IND-CCA2         |                    5 |                     21520 |                     43088 |                     21696 |                           32 | NA                          | NA                                |
| FrodoKEM-1344-SHAKE  | NA                    | IND-CCA2         |                    5 |                     21520 |                     43088 |                     21696 |                           32 | NA                          | NA                                |
|  eFrodoKEM-640-AES   | NA                    | IND-CCA2         |                    1 |                      9616 |                     19888 |                      9720 |                           16 | NA                          | NA                                |
| eFrodoKEM-640-SHAKE  | NA                    | IND-CCA2         |                    1 |                      9616 |                     19888 |                      9720 |                           16 | NA                          | NA                                |
|  eFrodoKEM-976-AES   | NA                    | IND-CCA2         |                    3 |                     15632 |                     31296 |                     15744 |                           24 | NA                          | NA                                |
| eFrodoKEM-976-SHAKE  | NA                    | IND-CCA2         |                    3 |                     15632 |                     31296 |                     15744 |                           24 | NA                          | NA                                |
|  eFrodoKEM-1344-AES  | NA                    | IND-CCA2         |                    5 |                     21520 |                     43088 |                     21632 |                           32 | NA                          | NA                                |
| eFrodoKEM-1344-SHAKE | NA                    | IND-CCA2         |                    5 |                     21520 |                     43088 |                     21632 |                           32 | NA                          | NA                                |

## FrodoKEM-640-AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | master                   | x86\_64                     | Linux,Darwin,Windows            | AVX2                    | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## FrodoKEM-640-SHAKE implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | master                   | x86\_64                     | Linux,Darwin,Windows            | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## FrodoKEM-976-AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | master                   | x86\_64                     | Linux,Darwin,Windows            | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## FrodoKEM-976-SHAKE implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | master                   | x86\_64                     | Linux,Darwin,Windows            | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## FrodoKEM-1344-AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | master                   | x86\_64                     | Linux,Darwin,Windows            | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## FrodoKEM-1344-SHAKE implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | master                   | x86\_64                     | Linux,Darwin,Windows            | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## eFrodoKEM-640-AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | master                   | x86\_64                     | Linux,Darwin,Windows            | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## eFrodoKEM-640-SHAKE implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | master                   | x86\_64                     | Linux,Darwin,Windows            | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## eFrodoKEM-976-AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | master                   | x86\_64                     | Linux,Darwin,Windows            | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## eFrodoKEM-976-SHAKE implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | master                   | x86\_64                     | Linux,Darwin,Windows            | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## eFrodoKEM-1344-AES implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | master                   | x86\_64                     | Linux,Darwin,Windows            | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## eFrodoKEM-1344-SHAKE implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | master                   | x86\_64                     | Linux,Darwin,Windows            | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.