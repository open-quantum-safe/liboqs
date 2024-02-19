# ML-DSA

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: hardness of lattice problems over module lattices.
- **Principal submitters**: Vadim Lyubashevsky.
- **Auxiliary submitters**: Shi Bai, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Peter Schwabe, Gregor Seiler, Damien Stehlé.
- **Authors' website**: https://pq-crystals.org/dilithium/ and https://csrc.nist.gov/pubs/fips/204/ipd
- **Specification version**: ML-DSA-ipd.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/pq-crystals/dilithium/commit/e7bed6258b9a3703ce78d4ec38021c86382ce31c with copy_from_upstream patches
  - **Implementation license (SPDX-Identifier)**: CC0-1.0 or Apache-2.0


## Parameter set summary

|  Parameter set  | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:---------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|  ML-DSA-44-ipd  | ML-DSA-44             | EUF-CMA          |                    2 |                      1312 |                      2560 |                     2420 |
|  ML-DSA-65-ipd  | ML-DSA-65             | EUF-CMA          |                    3 |                      1952 |                      4032 |                     3309 |
|  ML-DSA-87-ipd  | ML-DSA-87             | EUF-CMA          |                    5 |                      2592 |                      4896 |                     4627 |

## ML-DSA-44-ipd implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Darwin,Linux                    | AVX2,POPCNT             | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## ML-DSA-65-ipd implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Darwin,Linux                    | AVX2,POPCNT             | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## ML-DSA-87-ipd implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Darwin,Linux                    | AVX2,POPCNT             | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.