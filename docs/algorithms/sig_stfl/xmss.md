# XMSS/XMSS^MT

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: hash function security (ROM/QROM).
- **Authors**: Johannes Buchmann, Erik Dahmen, Andreas Hülsing.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/mkannwischer/xmssfs


## Parameter set summary

|      Parameter set      | Security model   |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:-----------------------:|:-----------------|--------------------------:|--------------------------:|-------------------------:|
|   XMSS-SHA2\_10\_256    | EUF-CMA          |                        68 |                      2051 |                     2500 |
|   XMSS-SHA2\_16\_256    | EUF-CMA          |                        68 |                      3155 |                     2692 |
|   XMSS-SHA2\_20\_256    | EUF-CMA          |                        68 |                      3893 |                     2820 |
|   XMSS-SHA2\_10\_512    | EUF-CMA          |                       132 |                      4003 |                     9092 |
|   XMSS-SHA2\_16\_512    | EUF-CMA          |                       132 |                      6163 |                     9476 |
|   XMSS-SHA2\_20\_512    | EUF-CMA          |                       132 |                      7605 |                     9732 |
|   XMSS-SHAKE\_10\_256   | EUF-CMA          |                        68 |                      2051 |                     2500 |
|   XMSS-SHAKE\_16\_256   | EUF-CMA          |                        68 |                      3155 |                     2692 |
|   XMSS-SHAKE\_20\_256   | EUF-CMA          |                        68 |                      3893 |                     2820 |
|   XMSS-SHAKE\_10\_512   | EUF-CMA          |                       132 |                      4003 |                     9092 |
|   XMSS-SHAKE\_16\_512   | EUF-CMA          |                       132 |                      6163 |                     9476 |
|   XMSS-SHAKE\_20\_512   | EUF-CMA          |                       132 |                      7605 |                     9732 |
| XMSSMT-SHA2\_20/2\_256  | EUF-CMA          |                        68 |                      8086 |                     4963 |
| XMSSMT-SHA2\_20/4\_256  | EUF-CMA          |                        68 |                     13602 |                     9251 |
| XMSSMT-SHA2\_40/2\_256  | EUF-CMA          |                        68 |                     13612 |                     5605 |
| XMSSMT-SHA2\_40/4\_256  | EUF-CMA          |                        68 |                     20160 |                     9893 |
| XMSSMT-SHA2\_40/8\_256  | EUF-CMA          |                        68 |                     30256 |                    18469 |
| XMSSMT-SHA2\_60/3\_256  | EUF-CMA          |                        68 |                     23329 |                     8392 |
| XMSSMT-SHA2\_60/6\_256  | EUF-CMA          |                        68 |                     32231 |                    14824 |
| XMSSMT-SHA2\_60/12\_256 | EUF-CMA          |                        68 |                     46907 |                    27688 |

## XMSS-SHA2\_10\_256 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## XMSS-SHA2\_16\_256 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSS-SHA2\_20\_256 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSS-SHA2\_10\_512 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSS-SHA2\_16\_512 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSS-SHA2\_20\_512 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSS-SHAKE\_10\_256 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSS-SHAKE\_16\_256 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSS-SHAKE\_20\_256 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSS-SHAKE\_10\_512 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSS-SHAKE\_16\_512 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSS-SHAKE\_20\_512 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSSMT-SHA2\_20/2\_256 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSSMT-SHA2\_20/4\_256 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSSMT-SHA2\_40/2\_256 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSSMT-SHA2\_40/4\_256 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSSMT-SHA2\_40/8\_256 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSSMT-SHA2\_60/3\_256 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSSMT-SHA2\_60/6\_256 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## XMSSMT-SHA2\_60/12\_256 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | master                   | All                         | All                             | None                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.