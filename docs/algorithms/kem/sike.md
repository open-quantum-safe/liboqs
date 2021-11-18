# SIKE

- **Algorithm type**: Key encapsulation mechanism.
- **Main cryptographic assumption**: (supersingular) isogeny walk problem.
- **Principal submitters**: David Jao, Reza Azarderakhsh, Matthew Campagna, Craig Costello, Luca De Feo, Basil Hess, Amir Jalali, Brian Koziel, Brian LaMacchia, Patrick Longa, Michael Naehrig, Joost Renes, Vladimir Soukharev, David Urbanik.
- **Authors' website**: https://sike.org
- **Specification version**: NIST Round 3 submission.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/microsoft/PQCrypto-SIDH/commit/effa607f244768cdd38f930887076373604eaa78
  - **Implementation license (SPDX-Identifier)**: MIT


## Parameter set summary

|    Parameter set     | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Ciphertext size (bytes) |   Shared secret size (bytes) |
|:--------------------:|:-----------------|---------------------:|--------------------------:|--------------------------:|--------------------------:|-----------------------------:|
|      SIDH-p434       | IND-CPA          |                    1 |                       330 |                        28 |                       330 |                          110 |
| SIDH-p434-compressed | IND-CPA          |                    1 |                       197 |                        28 |                       197 |                          110 |
|      SIDH-p503       | IND-CPA          |                    2 |                       378 |                        32 |                       378 |                          126 |
| SIDH-p503-compressed | IND-CPA          |                    2 |                       225 |                        32 |                       225 |                          126 |
|      SIDH-p610       | IND-CPA          |                    3 |                       462 |                        39 |                       462 |                          154 |
| SIDH-p610-compressed | IND-CPA          |                    3 |                       274 |                        39 |                       274 |                          154 |
|      SIDH-p751       | IND-CPA          |                    5 |                       564 |                        48 |                       564 |                          188 |
| SIDH-p751-compressed | IND-CPA          |                    5 |                       335 |                        48 |                       335 |                          188 |
|      SIKE-p434       | IND-CCA2         |                    1 |                       330 |                       374 |                       346 |                           16 |
| SIKE-p434-compressed | IND-CCA2         |                    1 |                       197 |                       350 |                       236 |                           16 |
|      SIKE-p503       | IND-CCA2         |                    2 |                       378 |                       434 |                       402 |                           24 |
| SIKE-p503-compressed | IND-CCA2         |                    2 |                       225 |                       407 |                       280 |                           24 |
|      SIKE-p610       | IND-CCA2         |                    3 |                       462 |                       524 |                       486 |                           24 |
| SIKE-p610-compressed | IND-CCA2         |                    3 |                       274 |                       491 |                       336 |                           24 |
|      SIKE-p751       | IND-CCA2         |                    5 |                       564 |                       644 |                       596 |                           32 |
| SIKE-p751-compressed | IND-CCA2         |                    5 |                       335 |                       602 |                       410 |                           32 |

## SIDH-p434 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | optimized                | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | additional\_amd64\_bmi2  | x86\_64                     | Linux,Darwin                    | BMI2                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | additional\_amd64\_adx   | x86\_64                     | Linux,Darwin                    | BMI2,ADX                | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | additional\_arm64        | arm64v8                     | Linux,Darwin                    | None                    | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## SIDH-p434-compressed implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | optimized                | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_bmi2  | x86\_64                     | Linux,Darwin                    | BMI2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_adx   | x86\_64                     | Linux,Darwin                    | BMI2,ADX                | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_arm64        | arm64v8                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## SIDH-p503 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | optimized                | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64        | x86\_64                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_bmi2  | x86\_64                     | Linux,Darwin                    | BMI2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_adx   | x86\_64                     | Linux,Darwin                    | BMI2,ADX                | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_arm64        | arm64v8                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## SIDH-p503-compressed implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | optimized                | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64        | x86\_64                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_bmi2  | x86\_64                     | Linux,Darwin                    | BMI2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_adx   | x86\_64                     | Linux,Darwin                    | BMI2,ADX                | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_arm64        | arm64v8                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## SIDH-p610 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | optimized                | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_bmi2  | x86\_64                     | Linux,Darwin                    | BMI2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_adx   | x86\_64                     | Linux,Darwin                    | BMI2,ADX                | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_arm64        | arm64v8                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## SIDH-p610-compressed implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | optimized                | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_bmi2  | x86\_64                     | Linux,Darwin                    | BMI2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_adx   | x86\_64                     | Linux,Darwin                    | BMI2,ADX                | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_arm64        | arm64v8                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## SIDH-p751 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | optimized                | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64        | x86\_64                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_bmi2  | x86\_64                     | Linux,Darwin                    | BMI2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_adx   | x86\_64                     | Linux,Darwin                    | BMI2,ADX                | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_arm64        | arm64v8                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## SIDH-p751-compressed implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | optimized                | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64        | x86\_64                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_bmi2  | x86\_64                     | Linux,Darwin                    | BMI2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_adx   | x86\_64                     | Linux,Darwin                    | BMI2,ADX                | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_arm64        | arm64v8                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## SIKE-p434 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | optimized                | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_bmi2  | x86\_64                     | Linux,Darwin                    | BMI2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_adx   | x86\_64                     | Linux,Darwin                    | BMI2,ADX                | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_arm64        | arm64v8                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## SIKE-p434-compressed implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | optimized                | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_bmi2  | x86\_64                     | Linux,Darwin                    | BMI2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_adx   | x86\_64                     | Linux,Darwin                    | BMI2,ADX                | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_arm64        | arm64v8                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## SIKE-p503 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | optimized                | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64        | x86\_64                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_bmi2  | x86\_64                     | Linux,Darwin                    | BMI2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_adx   | x86\_64                     | Linux,Darwin                    | BMI2,ADX                | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_arm64        | arm64v8                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## SIKE-p503-compressed implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | optimized                | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64        | x86\_64                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_bmi2  | x86\_64                     | Linux,Darwin                    | BMI2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_adx   | x86\_64                     | Linux,Darwin                    | BMI2,ADX                | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_arm64        | arm64v8                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## SIKE-p610 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | optimized                | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_bmi2  | x86\_64                     | Linux,Darwin                    | BMI2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_adx   | x86\_64                     | Linux,Darwin                    | BMI2,ADX                | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_arm64        | arm64v8                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## SIKE-p610-compressed implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | optimized                | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_bmi2  | x86\_64                     | Linux,Darwin                    | BMI2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_adx   | x86\_64                     | Linux,Darwin                    | BMI2,ADX                | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_arm64        | arm64v8                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## SIKE-p751 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | optimized                | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64        | x86\_64                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_bmi2  | x86\_64                     | Linux,Darwin                    | BMI2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_adx   | x86\_64                     | Linux,Darwin                    | BMI2,ADX                | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_arm64        | arm64v8                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## SIKE-p751-compressed implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | optimized                | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64        | x86\_64                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_bmi2  | x86\_64                     | Linux,Darwin                    | BMI2                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_amd64\_adx   | x86\_64                     | Linux,Darwin                    | BMI2,ADX                | True                               | True                                           | False                |
| [Primary Source](#primary-source) | additional\_arm64        | arm64v8                     | Linux,Darwin                    | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.