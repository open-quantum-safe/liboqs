# XMSS

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: hash-based signatures.
- **Principal submitters**: Joost Rijneveld, A. Huelsing, David Cooper, Bas Westerbaan.
- **Authors' website**: https://www.rfc-editor.org/info/rfc8391
- **Specification version**: None.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/XMSS/xmss-reference
  - **Implementation license (SPDX-Identifier)**: (Apache-2.0 OR MIT) AND CC0-1.0


## Parameter set summary

|     Parameter set      | Security model   | Claimed NIST Level   |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:----------------------:|:-----------------|:---------------------|--------------------------:|--------------------------:|-------------------------:|
|    XMSS-SHA2_10_256    |                  |                      |                        64 |                      1373 |                     2500 |
|    XMSS-SHA2_16_256    |                  |                      |                        64 |                      2093 |                     2692 |
|    XMSS-SHA2_20_256    |                  |                      |                        64 |                      2573 |                     2820 |
|   XMSS-SHAKE_10_256    |                  |                      |                        64 |                      1373 |                     2500 |
|   XMSS-SHAKE_16_256    |                  |                      |                        64 |                      2093 |                     2692 |
|   XMSS-SHAKE_20_256    |                  |                      |                        64 |                      2573 |                     2820 |
|    XMSS-SHA2_10_512    |                  |                      |                       128 |                      2653 |                     9092 |
|    XMSS-SHA2_16_512    |                  |                      |                       128 |                      4045 |                     9476 |
|    XMSS-SHA2_20_512    |                  |                      |                       128 |                      2653 |                     9732 |
|   XMSS-SHAKE_10_512    |                  |                      |                       128 |                      2653 |                     9092 |
|   XMSS-SHAKE_16_512    |                  |                      |                       128 |                      4045 |                     9476 |
|   XMSS-SHAKE_20_512    |                  |                      |                       128 |                      4973 |                     9732 |
|    XMSS-SHA2_10_192    |                  |                      |                        48 |                      1053 |                     1492 |
|    XMSS-SHA2_16_192    |                  |                      |                        48 |                      1605 |                     1636 |
|    XMSS-SHA2_20_192    |                  |                      |                        48 |                      1973 |                     1732 |
|  XMSS-SHAKE256_10_192  |                  |                      |                        48 |                      1053 |                     1492 |
|  XMSS-SHAKE256_16_192  |                  |                      |                        48 |                      1605 |                     1636 |
|  XMSS-SHAKE256_20_192  |                  |                      |                        48 |                      1973 |                     1732 |
|  XMSS-SHAKE256_10_256  |                  |                      |                        64 |                      1373 |                     2500 |
|  XMSS-SHAKE256_16_256  |                  |                      |                        64 |                      2093 |                     2692 |
|  XMSS-SHAKE256_20_256  |                  |                      |                        64 |                      2573 |                     2820 |
|  XMSSMT-SHA2_20/2_256  |                  |                      |                        64 |                      5998 |                     4963 |
|  XMSSMT-SHA2_20/4_256  |                  |                      |                        64 |                     10938 |                     9251 |
|  XMSSMT-SHA2_40/2_256  |                  |                      |                        64 |                      9600 |                     5605 |
|  XMSSMT-SHA2_40/4_256  |                  |                      |                        64 |                     15252 |                     9893 |
|  XMSSMT-SHA2_40/8_256  |                  |                      |                        64 |                     24516 |                    18469 |
|  XMSSMT-SHA2_60/3_256  |                  |                      |                        64 |                     16629 |                     8392 |
|  XMSSMT-SHA2_60/6_256  |                  |                      |                        64 |                     24507 |                    14824 |
| XMSSMT-SHA2_60/12_256  |                  |                      |                        64 |                     38095 |                    27688 |
| XMSSMT-SHAKE_20/2_256  |                  |                      |                        64 |                      5998 |                     4963 |
| XMSSMT-SHAKE_20/4_256  |                  |                      |                        64 |                     10938 |                     9251 |
| XMSSMT-SHAKE_40/2_256  |                  |                      |                        64 |                      9600 |                     5605 |
| XMSSMT-SHAKE_40/4_256  |                  |                      |                        64 |                     15252 |                     9893 |
| XMSSMT-SHAKE_40/8_256  |                  |                      |                        64 |                     24516 |                    18469 |
| XMSSMT-SHAKE_60/3_256  |                  |                      |                        64 |                     24516 |                     8392 |
| XMSSMT-SHAKE_60/6_256  |                  |                      |                        64 |                     24507 |                    14824 |
| XMSSMT-SHAKE_60/12_256 |                  |                      |                        64 |                     38095 |                    27688 |
