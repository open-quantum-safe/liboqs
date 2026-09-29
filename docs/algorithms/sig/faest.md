# FAEST

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: Syndrome decoding in the random code model and AES..
- **Principal submitters**: Carsten Baum, Waard Beullens, Lennart Braun, Cyprien Delpech de Saint Guilhem, Michael Klooß, Christian Majenz, Shibam Mukherjee, Emmanuela Orsini, Sebastian Ramacher, Christian Rechberger, Lawrence Roy, Peter Scholl.
- **Authors' website**: https://faest.info/
- **Specification version**: 3.0.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/faest-sign/faest-ref/commit/fe428d05a8cbea9c38fbb9266e43fba2c80b621d
  - **Implementation license (SPDX-Identifier)**: MIT


## Support

- **Upstream maintenance**: Actively maintained ([statement](https://github.com/open-quantum-safe/liboqs/blob/main/scripts/copy_from_upstream/faest/SUPPORT.md))
- **OQS support tier**: Tier 3

## Parameter set summary

|  Parameter set  | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:---------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|   FAEST-128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        32 |                     4066 |
|   FAEST-128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        32 |                     5170 |
|  FAEST-EM-128S  | NA                    | EUF-CMA          |                    1 |                        32 |                        32 |                     3466 |
|  FAEST-EM-128F  | NA                    | EUF-CMA          |                    1 |                        32 |                        32 |                     4170 |
|   FAEST-192S    | NA                    | EUF-CMA          |                    3 |                        48 |                        40 |                     9410 |
|   FAEST-192F    | NA                    | EUF-CMA          |                    3 |                        48 |                        40 |                    11738 |
|  FAEST-EM-192S  | NA                    | EUF-CMA          |                    3 |                        48 |                        48 |                     7874 |
|  FAEST-EM-192F  | NA                    | EUF-CMA          |                    3 |                        48 |                        48 |                     9818 |
|   FAEST-256S    | NA                    | EUF-CMA          |                    5 |                        48 |                        48 |                    16626 |
|   FAEST-256F    | NA                    | EUF-CMA          |                    5 |                        48 |                        48 |                    20856 |
|  FAEST-EM-256S  | NA                    | EUF-CMA          |                    5 |                        64 |                        64 |                    14554 |
|  FAEST-EM-256F  | NA                    | EUF-CMA          |                    5 |                        64 |                        64 |                    18084 |

## FAEST-128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | True                  |

Are implementations chosen based on runtime CPU feature detection? **No**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## FAEST-128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## FAEST-EM-128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## FAEST-EM-128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## FAEST-192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## FAEST-192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## FAEST-EM-192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## FAEST-EM-192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## FAEST-256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## FAEST-256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## FAEST-EM-256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## FAEST-EM-256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | False                                          | True                 |

Are implementations chosen based on runtime CPU feature detection? **No**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.