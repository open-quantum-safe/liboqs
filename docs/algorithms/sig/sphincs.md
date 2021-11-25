# SPHINCS+

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: hash-based signatures.
- **Principal submitters**: Andreas Hülsing.
- **Auxiliary submitters**: Jean-Philippe Aumasson, Daniel J. Bernstein,, Christoph Dobraunig, Maria Eichlseder, Scott Fluhrer, Stefan-Lukas Gazdag, Panos Kampanakis, Stefan Kölbl, Tanja Lange, Martin M. Lauridsen, Florian Mendel, Ruben Niederhagen, Christian Rechberger, Joost Rijneveld, Peter Schwabe.
- **Authors' website**: https://sphincs.org/
- **Specification version**: NIST Round 3 submission.
- **Implementation source**: https://github.com/PQClean/PQClean/commit/6a32796212b79a5f9126d0a933e1216313f50c16 with copy_from_upstream patches, which takes it from:
  - https://github.com/sphincs/sphincsplus
- **Implementation license (SPDX-Identifier)**: CC0-1.0.

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

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                 |
|          aesni           | x86\_64                     | All                             | AES                     | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## SPHINCS+-Haraka-128f-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|          aesni           | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-128s-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|          aesni           | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-128s-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|          aesni           | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-192f-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|          aesni           | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-192f-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|          aesni           | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-192s-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|          aesni           | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-192s-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|          aesni           | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-256f-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|          aesni           | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-256f-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|          aesni           | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-256s-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|          aesni           | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-Haraka-256s-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|          aesni           | x86\_64                     | All                             | AES                     | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-128f-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-128f-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-128s-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-128s-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-192f-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-192f-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-192s-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-192s-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-256f-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-256f-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-256s-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHA256-256s-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-128f-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-128f-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-128s-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-128s-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-192f-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-192f-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-192s-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-192s-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-256f-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-256f-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-256s-robust implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SPHINCS+-SHAKE256-256s-simple implementation characteristics

|  Identifier in upstream  | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:------------------------:|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
|          clean           | All                         | All                             | None                    | True                               | True                                           | False                |
|           avx2           | x86\_64                     | All                             | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.