# SQIsign

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: Hardness of the supersingular endomorphism ring problem..
- **Principal submitters**: Marius A. Aardal, Gora Adj, Diego F. Aranha, Andrea Basso, Giacomo Borin, Isaac Andrés Canales Martínez, Jorge Chávez-Saab, Maria Corte-Real Santos, Pierrick Dartois, Luca De Feo, Max Duparc, Thomas Espitau, Jonathan Komada Eriksen, Tako Boris Fouotsa, Décio Luiz Gazzoni Filho, Basil Hess, Riccardo Invernizzi, David Kohel, Antonin Leroux, Patrick Longa, Luciano Maino, Michael Meyer, Marzio Mula, Kohei Nakagawa, Hiroshi Onuki, Lorenz Panny, Sikhar Patranabis, Christophe Petit, Giacomo Pope, Krijn Reijnders, Damien Robert, Francisco Rodríguez-Henríquez, Sina Schaeffler, Frederik Vercauteren, Alexandre Wallet, Benjamin Wesolowski, Wessel van Woerden.
- **Authors' website**: https://sqisign.org/
- **Specification version**: 3.0.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/SQISign/the-sqisign/commit/7358e8786e2990fbf68ec2171606966d16872ddc with copy_from_upstream patches
  - **Implementation license (SPDX-Identifier)**: Apache-2.0
- **Optimized Implementation sources**:
  - **broadwell**:<a name="broadwell"></a>
      - **Source**: https://github.com/SQISign/the-sqisign/commit/7358e8786e2990fbf68ec2171606966d16872ddc with copy_from_upstream patches
      - **Implementation license (SPDX-Identifier)**: Apache-2.0


## Support

- **Upstream maintenance**: Best effort
- **OQS support tier**: Tier 3

## Parameter set summary

|  Parameter set  | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:---------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
| SQIsign-p324-3  | NA                    | EUF-CMA          |                    1 |                        83 |                       270 |                      200 |
| SQIsign-p500-27 | NA                    | EUF-CMA          |                    3 |                       129 |                       417 |                      306 |
| SQIsign-p664-17 | NA                    | EUF-CMA          |                    5 |                       169 |                       549 |                      406 |

## SQIsign-p324-3 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | False                              | False                                          | False                 |
| [Primary Source](#primary-source) | broadwell                | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,ADX           | False                              | False                                          | False                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## SQIsign-p500-27 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | broadwell                | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,ADX           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SQIsign-p664-17 implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | ref                      | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | broadwell                | x86\_64                     | Linux,Darwin                    | AVX2,BMI2,ADX           | False                              | False                                          | False                |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.