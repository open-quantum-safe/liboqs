# SQIsign

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: Computing the endomorphism ring of a supersingular elliptic curve..
- **Principal submitters**: Marius A. Aardal, Gora Adj, Diego F.Aranha, Andrea Basso, Isaac Andrés Canales Martínez, Jorge Chávez-Saab, Maria Corte-Real Santos, Pierrick Dartois, Luca De Feo, Max Duparc, Jonathan Komada Eriksen, Tako Boris Fouotsa, Décio Luiz Gazzoni Filho, Basil Hess, David Kohel, Antonin Leroux, Patrick Longa, Luciano Maino, Michael Meyer, Kohei Nakagawa, Hiroshi Onuki, Lorenz Panny, Sikhar Patranabis, Christophe Petit, Giacomo Pope, Krijn Reijnders, Damien Robert, Francisco Rodríguez Henríquez, Sina Schaeffler, Benjamin Wesolowski.
- **Authors' website**: https://sqisign.org/
- **Specification version**: Round 2.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/shane-digi/the-sqisign/commit/a8c8a3d5acbeaecec179d9088c8eab7216db4db5 with copy_from_upstream patches
  - **Implementation license (SPDX-Identifier)**: Apache-2.0


## Parameter set summary

|  Parameter set  | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:---------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|  SQIsign-lvl1   | NA                    | EUF-CMA          |                    1 |                        65 |                       353 |                      148 |
|  SQIsign-lvl3   | NA                    | EUF-CMA          |                    3 |                        97 |                       529 |                      224 |
|  SQIsign-lvl5   | NA                    | EUF-CMA          |                    5 |                       129 |                       701 |                      292 |

## SQIsign-lvl1 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                 |
| [Primary Source](#primary-source) | broadwell                | x86\_64                     | Darwin,Linux                    | AVX2                    | True                               | True                                           | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## SQIsign-lvl3 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | broadwell                | x86\_64                     | Darwin,Linux                    | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SQIsign-lvl5 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | True                               | True                                           | False                |
| [Primary Source](#primary-source) | broadwell                | x86\_64                     | Darwin,Linux                    | AVX2                    | True                               | True                                           | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.