# SPHINCS+

- **Algorithm type**: Digital signature scheme.
- **Main cryptographic assumption**: hash-based signatures.
- **Principal submitters**: Andreas Hülsing.
- **Auxiliary submitters**: Jean-Philippe Aumasson.
- **Authors' website**: https://csrc.nist.gov/pubs/fips/205/final
- **Specification version**: NIST Round 3 submission, v3.1 (June 10, 2022).
- **Primary Source**<a name="primary-source"></a>:
  - **Source**: https://github.com/h2parson/slhdsa-c/commit/16cdd85ee74095592d975ff30afa682075a5a00b
  - **Implementation license (SPDX-Identifier)**: CC0-1.0


## Advisories

- This algorithm is not tested under Windows.

## Parameter set summary

|                 Parameter set                  | Parameter set alias   | Security model   |   Claimed NIST Level |   Public key size (bytes) |   Secret key size (bytes) |   Signature size (bytes) |
|:----------------------------------------------:|:----------------------|:-----------------|---------------------:|--------------------------:|--------------------------:|-------------------------:|
|           SLH\_DSA\_PURE\_SHA2\_128S           | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|           SLH\_DSA\_PURE\_SHA2\_128F           | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|           SLH\_DSA\_PURE\_SHA2\_192S           | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|           SLH\_DSA\_PURE\_SHA2\_192F           | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|           SLH\_DSA\_PURE\_SHA2\_256S           | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|           SLH\_DSA\_PURE\_SHA2\_256F           | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|          SLH\_DSA\_PURE\_SHAKE\_128S           | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|          SLH\_DSA\_PURE\_SHAKE\_128F           | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|          SLH\_DSA\_PURE\_SHAKE\_192S           | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|          SLH\_DSA\_PURE\_SHAKE\_192F           | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|          SLH\_DSA\_PURE\_SHAKE\_256S           | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|          SLH\_DSA\_PURE\_SHAKE\_256F           | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|    SLH\_DSA\_SHA2\_224\_PREHASH\_SHA2\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|    SLH\_DSA\_SHA2\_224\_PREHASH\_SHA2\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|    SLH\_DSA\_SHA2\_224\_PREHASH\_SHA2\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|    SLH\_DSA\_SHA2\_224\_PREHASH\_SHA2\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|    SLH\_DSA\_SHA2\_224\_PREHASH\_SHA2\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|    SLH\_DSA\_SHA2\_224\_PREHASH\_SHA2\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|   SLH\_DSA\_SHA2\_224\_PREHASH\_SHAKE\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|   SLH\_DSA\_SHA2\_224\_PREHASH\_SHAKE\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|   SLH\_DSA\_SHA2\_224\_PREHASH\_SHAKE\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|   SLH\_DSA\_SHA2\_224\_PREHASH\_SHAKE\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|   SLH\_DSA\_SHA2\_224\_PREHASH\_SHAKE\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|   SLH\_DSA\_SHA2\_224\_PREHASH\_SHAKE\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|    SLH\_DSA\_SHA2\_256\_PREHASH\_SHA2\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|    SLH\_DSA\_SHA2\_256\_PREHASH\_SHA2\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|    SLH\_DSA\_SHA2\_256\_PREHASH\_SHA2\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|    SLH\_DSA\_SHA2\_256\_PREHASH\_SHA2\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|    SLH\_DSA\_SHA2\_256\_PREHASH\_SHA2\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|    SLH\_DSA\_SHA2\_256\_PREHASH\_SHA2\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|   SLH\_DSA\_SHA2\_256\_PREHASH\_SHAKE\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|   SLH\_DSA\_SHA2\_256\_PREHASH\_SHAKE\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|   SLH\_DSA\_SHA2\_256\_PREHASH\_SHAKE\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|   SLH\_DSA\_SHA2\_256\_PREHASH\_SHAKE\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|   SLH\_DSA\_SHA2\_256\_PREHASH\_SHAKE\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|   SLH\_DSA\_SHA2\_256\_PREHASH\_SHAKE\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|    SLH\_DSA\_SHA2\_384\_PREHASH\_SHA2\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|    SLH\_DSA\_SHA2\_384\_PREHASH\_SHA2\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|    SLH\_DSA\_SHA2\_384\_PREHASH\_SHA2\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|    SLH\_DSA\_SHA2\_384\_PREHASH\_SHA2\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|    SLH\_DSA\_SHA2\_384\_PREHASH\_SHA2\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|    SLH\_DSA\_SHA2\_384\_PREHASH\_SHA2\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|   SLH\_DSA\_SHA2\_384\_PREHASH\_SHAKE\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|   SLH\_DSA\_SHA2\_384\_PREHASH\_SHAKE\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|   SLH\_DSA\_SHA2\_384\_PREHASH\_SHAKE\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|   SLH\_DSA\_SHA2\_384\_PREHASH\_SHAKE\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|   SLH\_DSA\_SHA2\_384\_PREHASH\_SHAKE\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|   SLH\_DSA\_SHA2\_384\_PREHASH\_SHAKE\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|    SLH\_DSA\_SHA2\_512\_PREHASH\_SHA2\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|    SLH\_DSA\_SHA2\_512\_PREHASH\_SHA2\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|    SLH\_DSA\_SHA2\_512\_PREHASH\_SHA2\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|    SLH\_DSA\_SHA2\_512\_PREHASH\_SHA2\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|    SLH\_DSA\_SHA2\_512\_PREHASH\_SHA2\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|    SLH\_DSA\_SHA2\_512\_PREHASH\_SHA2\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|   SLH\_DSA\_SHA2\_512\_PREHASH\_SHAKE\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|   SLH\_DSA\_SHA2\_512\_PREHASH\_SHAKE\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|   SLH\_DSA\_SHA2\_512\_PREHASH\_SHAKE\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|   SLH\_DSA\_SHA2\_512\_PREHASH\_SHAKE\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|   SLH\_DSA\_SHA2\_512\_PREHASH\_SHAKE\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|   SLH\_DSA\_SHA2\_512\_PREHASH\_SHAKE\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
| SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHA2\_128S  | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
| SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHA2\_128F  | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
| SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHA2\_192S  | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
| SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHA2\_192F  | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
| SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHA2\_256S  | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
| SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHA2\_256F  | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
| SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHAKE\_128S | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
| SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHAKE\_128F | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
| SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHAKE\_192S | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
| SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHAKE\_192F | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
| SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHAKE\_256S | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
| SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHAKE\_256F | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
| SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHA2\_128S  | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
| SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHA2\_128F  | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
| SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHA2\_192S  | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
| SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHA2\_192F  | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
| SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHA2\_256S  | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
| SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHA2\_256F  | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
| SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHAKE\_128S | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
| SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHAKE\_128F | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
| SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHAKE\_192S | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
| SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHAKE\_192F | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
| SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHAKE\_256S | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
| SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHAKE\_256F | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|    SLH\_DSA\_SHA3\_224\_PREHASH\_SHA2\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|    SLH\_DSA\_SHA3\_224\_PREHASH\_SHA2\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|    SLH\_DSA\_SHA3\_224\_PREHASH\_SHA2\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|    SLH\_DSA\_SHA3\_224\_PREHASH\_SHA2\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|    SLH\_DSA\_SHA3\_224\_PREHASH\_SHA2\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|    SLH\_DSA\_SHA3\_224\_PREHASH\_SHA2\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|   SLH\_DSA\_SHA3\_224\_PREHASH\_SHAKE\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|   SLH\_DSA\_SHA3\_224\_PREHASH\_SHAKE\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|   SLH\_DSA\_SHA3\_224\_PREHASH\_SHAKE\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|   SLH\_DSA\_SHA3\_224\_PREHASH\_SHAKE\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|   SLH\_DSA\_SHA3\_224\_PREHASH\_SHAKE\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|   SLH\_DSA\_SHA3\_224\_PREHASH\_SHAKE\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|    SLH\_DSA\_SHA3\_256\_PREHASH\_SHA2\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|    SLH\_DSA\_SHA3\_256\_PREHASH\_SHA2\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|    SLH\_DSA\_SHA3\_256\_PREHASH\_SHA2\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|    SLH\_DSA\_SHA3\_256\_PREHASH\_SHA2\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|    SLH\_DSA\_SHA3\_256\_PREHASH\_SHA2\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|    SLH\_DSA\_SHA3\_256\_PREHASH\_SHA2\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|   SLH\_DSA\_SHA3\_256\_PREHASH\_SHAKE\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|   SLH\_DSA\_SHA3\_256\_PREHASH\_SHAKE\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|   SLH\_DSA\_SHA3\_256\_PREHASH\_SHAKE\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|   SLH\_DSA\_SHA3\_256\_PREHASH\_SHAKE\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|   SLH\_DSA\_SHA3\_256\_PREHASH\_SHAKE\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|   SLH\_DSA\_SHA3\_256\_PREHASH\_SHAKE\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|    SLH\_DSA\_SHA3\_384\_PREHASH\_SHA2\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|    SLH\_DSA\_SHA3\_384\_PREHASH\_SHA2\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|    SLH\_DSA\_SHA3\_384\_PREHASH\_SHA2\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|    SLH\_DSA\_SHA3\_384\_PREHASH\_SHA2\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|    SLH\_DSA\_SHA3\_384\_PREHASH\_SHA2\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|    SLH\_DSA\_SHA3\_384\_PREHASH\_SHA2\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|   SLH\_DSA\_SHA3\_384\_PREHASH\_SHAKE\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|   SLH\_DSA\_SHA3\_384\_PREHASH\_SHAKE\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|   SLH\_DSA\_SHA3\_384\_PREHASH\_SHAKE\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|   SLH\_DSA\_SHA3\_384\_PREHASH\_SHAKE\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|   SLH\_DSA\_SHA3\_384\_PREHASH\_SHAKE\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|   SLH\_DSA\_SHA3\_384\_PREHASH\_SHAKE\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|    SLH\_DSA\_SHA3\_512\_PREHASH\_SHA2\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|    SLH\_DSA\_SHA3\_512\_PREHASH\_SHA2\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|    SLH\_DSA\_SHA3\_512\_PREHASH\_SHA2\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|    SLH\_DSA\_SHA3\_512\_PREHASH\_SHA2\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|    SLH\_DSA\_SHA3\_512\_PREHASH\_SHA2\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|    SLH\_DSA\_SHA3\_512\_PREHASH\_SHA2\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|   SLH\_DSA\_SHA3\_512\_PREHASH\_SHAKE\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|   SLH\_DSA\_SHA3\_512\_PREHASH\_SHAKE\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|   SLH\_DSA\_SHA3\_512\_PREHASH\_SHAKE\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|   SLH\_DSA\_SHA3\_512\_PREHASH\_SHAKE\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|   SLH\_DSA\_SHA3\_512\_PREHASH\_SHAKE\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|   SLH\_DSA\_SHA3\_512\_PREHASH\_SHAKE\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|   SLH\_DSA\_SHAKE\_128\_PREHASH\_SHA2\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|   SLH\_DSA\_SHAKE\_128\_PREHASH\_SHA2\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|   SLH\_DSA\_SHAKE\_128\_PREHASH\_SHA2\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|   SLH\_DSA\_SHAKE\_128\_PREHASH\_SHA2\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|   SLH\_DSA\_SHAKE\_128\_PREHASH\_SHA2\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|   SLH\_DSA\_SHAKE\_128\_PREHASH\_SHA2\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|   SLH\_DSA\_SHAKE\_128\_PREHASH\_SHAKE\_128S   | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|   SLH\_DSA\_SHAKE\_128\_PREHASH\_SHAKE\_128F   | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|   SLH\_DSA\_SHAKE\_128\_PREHASH\_SHAKE\_192S   | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|   SLH\_DSA\_SHAKE\_128\_PREHASH\_SHAKE\_192F   | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|   SLH\_DSA\_SHAKE\_128\_PREHASH\_SHAKE\_256S   | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|   SLH\_DSA\_SHAKE\_128\_PREHASH\_SHAKE\_256F   | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|   SLH\_DSA\_SHAKE\_256\_PREHASH\_SHA2\_128S    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|   SLH\_DSA\_SHAKE\_256\_PREHASH\_SHA2\_128F    | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|   SLH\_DSA\_SHAKE\_256\_PREHASH\_SHA2\_192S    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|   SLH\_DSA\_SHAKE\_256\_PREHASH\_SHA2\_192F    | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|   SLH\_DSA\_SHAKE\_256\_PREHASH\_SHA2\_256S    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|   SLH\_DSA\_SHAKE\_256\_PREHASH\_SHA2\_256F    | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |
|   SLH\_DSA\_SHAKE\_256\_PREHASH\_SHAKE\_128S   | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                     7856 |
|   SLH\_DSA\_SHAKE\_256\_PREHASH\_SHAKE\_128F   | NA                    | EUF-CMA          |                    1 |                        32 |                        64 |                    17088 |
|   SLH\_DSA\_SHAKE\_256\_PREHASH\_SHAKE\_192S   | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    16224 |
|   SLH\_DSA\_SHAKE\_256\_PREHASH\_SHAKE\_192F   | NA                    | EUF-CMA          |                    1 |                        48 |                        96 |                    35664 |
|   SLH\_DSA\_SHAKE\_256\_PREHASH\_SHAKE\_256S   | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    29792 |
|   SLH\_DSA\_SHAKE\_256\_PREHASH\_SHAKE\_256F   | NA                    | EUF-CMA          |                    1 |                        64 |                       128 |                    49856 |

## SLH\_DSA\_PURE\_SHA2\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?‡   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:----------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                 |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                  |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

 ‡For an explanation of what this denotes, consult the [Explanation of Terms](#explanation-of-terms) section at the end of this file.

## SLH\_DSA\_PURE\_SHA2\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_PURE\_SHA2\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_PURE\_SHA2\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_PURE\_SHA2\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_PURE\_SHA2\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_PURE\_SHAKE\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_PURE\_SHAKE\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_PURE\_SHAKE\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_PURE\_SHAKE\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_PURE\_SHAKE\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_PURE\_SHAKE\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_224\_PREHASH\_SHA2\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_224\_PREHASH\_SHA2\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_224\_PREHASH\_SHA2\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_224\_PREHASH\_SHA2\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_224\_PREHASH\_SHA2\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_224\_PREHASH\_SHA2\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_224\_PREHASH\_SHAKE\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_224\_PREHASH\_SHAKE\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_224\_PREHASH\_SHAKE\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_224\_PREHASH\_SHAKE\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_224\_PREHASH\_SHAKE\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_224\_PREHASH\_SHAKE\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_256\_PREHASH\_SHA2\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_256\_PREHASH\_SHA2\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_256\_PREHASH\_SHA2\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_256\_PREHASH\_SHA2\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_256\_PREHASH\_SHA2\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_256\_PREHASH\_SHA2\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_256\_PREHASH\_SHAKE\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_256\_PREHASH\_SHAKE\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_256\_PREHASH\_SHAKE\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_256\_PREHASH\_SHAKE\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_256\_PREHASH\_SHAKE\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_256\_PREHASH\_SHAKE\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_384\_PREHASH\_SHA2\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_384\_PREHASH\_SHA2\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_384\_PREHASH\_SHA2\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_384\_PREHASH\_SHA2\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_384\_PREHASH\_SHA2\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_384\_PREHASH\_SHA2\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_384\_PREHASH\_SHAKE\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_384\_PREHASH\_SHAKE\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_384\_PREHASH\_SHAKE\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_384\_PREHASH\_SHAKE\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_384\_PREHASH\_SHAKE\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_384\_PREHASH\_SHAKE\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_PREHASH\_SHA2\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_PREHASH\_SHA2\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_PREHASH\_SHA2\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_PREHASH\_SHA2\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_PREHASH\_SHA2\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_PREHASH\_SHA2\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_PREHASH\_SHAKE\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_PREHASH\_SHAKE\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_PREHASH\_SHAKE\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_PREHASH\_SHAKE\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_PREHASH\_SHAKE\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_PREHASH\_SHAKE\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHA2\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHA2\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHA2\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHA2\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHA2\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHA2\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHAKE\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHAKE\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHAKE\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHAKE\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHAKE\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_224\_PREHASH\_SHAKE\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHA2\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHA2\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHA2\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHA2\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHA2\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHA2\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHAKE\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHAKE\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHAKE\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHAKE\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHAKE\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA2\_512\_256\_PREHASH\_SHAKE\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_224\_PREHASH\_SHA2\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_224\_PREHASH\_SHA2\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_224\_PREHASH\_SHA2\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_224\_PREHASH\_SHA2\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_224\_PREHASH\_SHA2\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_224\_PREHASH\_SHA2\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_224\_PREHASH\_SHAKE\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_224\_PREHASH\_SHAKE\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_224\_PREHASH\_SHAKE\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_224\_PREHASH\_SHAKE\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_224\_PREHASH\_SHAKE\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_224\_PREHASH\_SHAKE\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_256\_PREHASH\_SHA2\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_256\_PREHASH\_SHA2\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_256\_PREHASH\_SHA2\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_256\_PREHASH\_SHA2\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_256\_PREHASH\_SHA2\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_256\_PREHASH\_SHA2\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_256\_PREHASH\_SHAKE\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_256\_PREHASH\_SHAKE\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_256\_PREHASH\_SHAKE\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_256\_PREHASH\_SHAKE\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_256\_PREHASH\_SHAKE\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_256\_PREHASH\_SHAKE\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_384\_PREHASH\_SHA2\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_384\_PREHASH\_SHA2\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_384\_PREHASH\_SHA2\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_384\_PREHASH\_SHA2\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_384\_PREHASH\_SHA2\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_384\_PREHASH\_SHA2\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_384\_PREHASH\_SHAKE\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_384\_PREHASH\_SHAKE\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_384\_PREHASH\_SHAKE\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_384\_PREHASH\_SHAKE\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_384\_PREHASH\_SHAKE\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_384\_PREHASH\_SHAKE\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_512\_PREHASH\_SHA2\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_512\_PREHASH\_SHA2\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_512\_PREHASH\_SHA2\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_512\_PREHASH\_SHA2\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_512\_PREHASH\_SHA2\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_512\_PREHASH\_SHA2\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_512\_PREHASH\_SHAKE\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_512\_PREHASH\_SHAKE\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_512\_PREHASH\_SHAKE\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_512\_PREHASH\_SHAKE\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_512\_PREHASH\_SHAKE\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHA3\_512\_PREHASH\_SHAKE\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_128\_PREHASH\_SHA2\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_128\_PREHASH\_SHA2\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_128\_PREHASH\_SHA2\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_128\_PREHASH\_SHA2\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_128\_PREHASH\_SHA2\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_128\_PREHASH\_SHA2\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_128\_PREHASH\_SHAKE\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_128\_PREHASH\_SHAKE\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_128\_PREHASH\_SHAKE\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_128\_PREHASH\_SHAKE\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_128\_PREHASH\_SHAKE\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_128\_PREHASH\_SHAKE\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_256\_PREHASH\_SHA2\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_256\_PREHASH\_SHA2\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_256\_PREHASH\_SHA2\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_256\_PREHASH\_SHA2\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_256\_PREHASH\_SHA2\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_256\_PREHASH\_SHA2\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_256\_PREHASH\_SHAKE\_128S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_256\_PREHASH\_SHAKE\_128F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_256\_PREHASH\_SHAKE\_192S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_256\_PREHASH\_SHAKE\_192F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_256\_PREHASH\_SHAKE\_256S implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## SLH\_DSA\_SHAKE\_256\_PREHASH\_SHAKE\_256F implementation characteristics

|       Implementation source       | Identifier in upstream   | Supported architecture(s)   | Supported operating system(s)   | CPU extension(s) used   | No branching-on-secrets claimed?   | No branching-on-secrets checked by valgrind?   | Large stack usage?   |
|:---------------------------------:|:-------------------------|:----------------------------|:--------------------------------|:------------------------|:-----------------------------------|:-----------------------------------------------|:---------------------|
| [Primary Source](#primary-source) | clean                    | All                         | All                             | None                    | False                              | False                                          | False                |
| [Primary Source](#primary-source) | avx2                     | x86\_64                     | Linux,Darwin                    | AVX2                    | True                               | True                                           | True                 |

Are implementations chosen based on runtime CPU feature detection? **Yes**.

## Explanation of Terms

- **Large Stack Usage**: Implementations identified as having such may cause failures when running in threads or in constrained environments.