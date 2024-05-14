# LMS

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: hash-based signatures.
- **Principal submitters**: Scott Fluhrer.
- **Auxiliary submitters**: C Martin, Maurice Hieronymus.
- **Authors' website**: https://www.rfc-editor.org/info/rfc8554
- **Specification version**: None.
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/cisco/hash-sigs
  - **Implementation license (SPDX-Identifier)**: MIT


## Parameter set summary

|      Parameter set       | Security model   | Claimed NIST Level   |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:------------------------:|:-----------------|:---------------------|--------------------------:|--------------------------:|-------------------------:|
|     LMS_SHA256_H5_W1     |                  |                      |                        60 |                        64 |                     8688 |
|     LMS_SHA256_H5_W2     |                  |                      |                        60 |                        64 |                     4464 |
|     LMS_SHA256_H5_W4     |                  |                      |                        60 |                        64 |                     2352 |
|     LMS_SHA256_H5_W8     |                  |                      |                        60 |                        64 |                     1296 |
|    LMS_SHA256_H10_W1     |                  |                      |                        60 |                        64 |                     8848 |
|    LMS_SHA256_H10_W2     |                  |                      |                        60 |                        64 |                     4624 |
|    LMS_SHA256_H10_W4     |                  |                      |                        60 |                        64 |                     2512 |
|    LMS_SHA256_H10_W8     |                  |                      |                        60 |                        64 |                     1456 |
|    LMS_SHA256_H15_W1     |                  |                      |                        60 |                        64 |                     9008 |
|    LMS_SHA256_H15_W2     |                  |                      |                        60 |                        64 |                     4784 |
|    LMS_SHA256_H15_W4     |                  |                      |                        60 |                        64 |                     2672 |
|    LMS_SHA256_H15_W8     |                  |                      |                        60 |                        64 |                     1616 |
|    LMS_SHA256_H20_W1     |                  |                      |                        60 |                        64 |                     9168 |
|    LMS_SHA256_H20_W2     |                  |                      |                        60 |                        64 |                     4944 |
|    LMS_SHA256_H20_W4     |                  |                      |                        60 |                        64 |                     2832 |
|    LMS_SHA256_H20_W8     |                  |                      |                        60 |                        64 |                     1776 |
|    LMS_SHA256_H25_W1     |                  |                      |                        60 |                        64 |                     9328 |
|    LMS_SHA256_H25_W2     |                  |                      |                        60 |                        64 |                     5104 |
|    LMS_SHA256_H25_W4     |                  |                      |                        60 |                        64 |                     2992 |
|    LMS_SHA256_H25_W8     |                  |                      |                        60 |                        64 |                     1936 |
|  LMS_SHA256_H5_W8_H5_W8  |                  |                      |                        60 |                        64 |                     2644 |
| LMS_SHA256_H10_W4_H5_W8  |                  |                      |                        60 |                        64 |                     2804 |
| LMS_SHA256_H10_W8_H5_W8  |                  |                      |                        60 |                        64 |                     3860 |
| LMS_SHA256_H10_W2_H10_W2 |                  |                      |                        60 |                        64 |                     9300 |
| LMS_SHA256_H10_W4_H10_W4 |                  |                      |                        60 |                        64 |                     5076 |
| LMS_SHA256_H10_W8_H10_W8 |                  |                      |                        60 |                        64 |                     2964 |
| LMS_SHA256_H15_W8_H5_W8  |                  |                      |                        60 |                        64 |                     2964 |
| LMS_SHA256_H15_W8_H10_W8 |                  |                      |                        60 |                        64 |                     3124 |
| LMS_SHA256_H15_W8_H15_W8 |                  |                      |                        60 |                        64 |                     3284 |
| LMS_SHA256_H20_W8_H5_W8  |                  |                      |                        60 |                        64 |                     3124 |
| LMS_SHA256_H20_W8_H10_W8 |                  |                      |                        60 |                        64 |                     3284 |
| LMS_SHA256_H20_W8_H15_W8 |                  |                      |                        60 |                        64 |                     3444 |
| LMS_SHA256_H20_W8_H20_W8 |                  |                      |                        60 |                        64 |                     3604 |
