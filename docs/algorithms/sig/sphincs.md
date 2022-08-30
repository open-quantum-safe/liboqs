# SPHINCS+

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: hash-based signatures.
- **Principal submitters**: Andreas Hülsing.
- **Auxiliary submitters**: Jean-Philippe Aumasson, Daniel J. Bernstein,, Christoph Dobraunig, Maria Eichlseder, Scott Fluhrer, Stefan-Lukas Gazdag, Panos Kampanakis, Stefan Kölbl, Tanja Lange, Martin M. Lauridsen, Florian Mendel, Ruben Niederhagen, Christian Rechberger, Joost Rijneveld, Peter Schwabe.
- **Authors' website**: https://sphincs.org/
- **Specification version**: NIST Round 3 submission.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/PQClean/PQClean/commit/4c9e5a3aa715cc8d1d0e377e4e6e682ebd7602d6 with copy_from_upstream patches
  - **Implementation license (SPDX-Identifier)**: CC0-1.0

## Test limitation

This algorithm is not tested under Windows.

## Parameter set summary

|         Parameter set         | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:-----------------------------:|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|  SPHINCS+-Haraka-128f-robust  | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|  SPHINCS+-Haraka-128f-simple  | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|  SPHINCS+-Haraka-128s-robust  | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|  SPHINCS+-Haraka-128s-simple  | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|  SPHINCS+-Haraka-192f-robust  | EUF-CMA          |                    3 |                        48 |                        96 |                    35664 |
|  SPHINCS+-Haraka-192f-simple  | EUF-CMA          |                    3 |                        48 |                        96 |                    35664 |
|  SPHINCS+-Haraka-192s-robust  | EUF-CMA          |                    3 |                        48 |                        96 |                    16224 |
|  SPHINCS+-Haraka-192s-simple  | EUF-CMA          |                    3 |                        48 |                        96 |                    16224 |
|  SPHINCS+-Haraka-256f-robust  | EUF-CMA          |                    5 |                        64 |                       128 |                    49856 |
|  SPHINCS+-Haraka-256f-simple  | EUF-CMA          |                    5 |                        64 |                       128 |                    49856 |
|  SPHINCS+-Haraka-256s-robust  | EUF-CMA          |                    5 |                        64 |                       128 |                    29792 |
|  SPHINCS+-Haraka-256s-simple  | EUF-CMA          |                    5 |                        64 |                       128 |                    29792 |
|  SPHINCS+-SHA256-128f-robust  | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|  SPHINCS+-SHA256-128f-simple  | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|  SPHINCS+-SHA256-128s-robust  | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|  SPHINCS+-SHA256-128s-simple  | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|  SPHINCS+-SHA256-192f-robust  | EUF-CMA          |                    3 |                        48 |                        96 |                    35664 |
|  SPHINCS+-SHA256-192f-simple  | EUF-CMA          |                    3 |                        48 |                        96 |                    35664 |
|  SPHINCS+-SHA256-192s-robust  | EUF-CMA          |                    3 |                        48 |                        96 |                    16224 |
|  SPHINCS+-SHA256-192s-simple  | EUF-CMA          |                    3 |                        48 |                        96 |                    16224 |
|  SPHINCS+-SHA256-256f-robust  | EUF-CMA          |                    5 |                        64 |                       128 |                    49856 |
|  SPHINCS+-SHA256-256f-simple  | EUF-CMA          |                    5 |                        64 |                       128 |                    49856 |
|  SPHINCS+-SHA256-256s-robust  | EUF-CMA          |                    5 |                        64 |                       128 |                    29792 |
|  SPHINCS+-SHA256-256s-simple  | EUF-CMA          |                    5 |                        64 |                       128 |                    29792 |
| SPHINCS+-SHAKE256-128f-robust | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
| SPHINCS+-SHAKE256-128f-simple | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
| SPHINCS+-SHAKE256-128s-robust | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
| SPHINCS+-SHAKE256-128s-simple | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
| SPHINCS+-SHAKE256-192f-robust | EUF-CMA          |                    3 |                        48 |                        96 |                    35664 |
| SPHINCS+-SHAKE256-192f-simple | EUF-CMA          |                    3 |                        48 |                        96 |                    35664 |
| SPHINCS+-SHAKE256-192s-robust | EUF-CMA          |                    3 |                        48 |                        96 |                    16224 |
| SPHINCS+-SHAKE256-192s-simple | EUF-CMA          |                    3 |                        48 |                        96 |                    16224 |
| SPHINCS+-SHAKE256-256f-robust | EUF-CMA          |                    5 |                        64 |                       128 |                    49856 |
| SPHINCS+-SHAKE256-256f-simple | EUF-CMA          |                    5 |                        64 |                       128 |                    49856 |
| SPHINCS+-SHAKE256-256s-robust | EUF-CMA          |                    5 |                        64 |                       128 |                    29792 |
| SPHINCS+-SHAKE256-256s-simple | EUF-CMA          |                    5 |                        64 |                       128 |                    29792 |

## SPHINCS+-Haraka-128f-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | aesni                    | x86\_64                     | All                             | AES                     | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## SPHINCS+-Haraka-128f-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | aesni                    | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-128s-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | aesni                    | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-128s-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | aesni                    | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-192f-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | aesni                    | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-192f-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | aesni                    | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-192s-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | aesni                    | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-192s-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | aesni                    | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-256f-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | aesni                    | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-256f-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | aesni                    | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-256s-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | aesni                    | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-256s-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | aesni                    | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-128f-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-128f-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-128s-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-128s-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-192f-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-192f-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-192s-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-192s-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-256f-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-256f-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-256s-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-256s-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-128f-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-128f-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-128s-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-128s-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-192f-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-192f-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-192s-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-192s-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-256f-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-256f-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-256s-robust implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-256s-simple implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.