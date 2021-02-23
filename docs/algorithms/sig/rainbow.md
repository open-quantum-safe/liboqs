# Rainbow

- **Algorithm type**: signature
- **Main cryptographic assumption**: multivariable polynomials, unbalanced oil and vinegar
- **Scheme authors**: Jintai Ding, Ming-Shing Chen, Albrecht Petzoldt, Dieter Schmidt, Bo-Yin Yang
- **Version**: NIST Round 3 submission

## Implementation

- **Source of implementation**: https://github.com/fast-crypto-lab/rainbow-submission-round2/commit/173ada0e077e1b9dbd8e4a78994f87acc0c92263
- **Implementation version**: https://github.com/PQClean/PQClean.git, master, ebcc71c51a30b6e5db4f1fade22999b346fdafce
- **License**: CC0 1.0
- **Constant-time**: Yes
- **Optimizations**: Portable C

## Parameter sets

| Parameter set              | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
| -------------------------- | -------------- | --------------------------- | ----------------------- | ----------------------- | ---------------------- |
| Rainbow-I-Classic          | EUF-CMA        | 1                           | 161600                  | 103648                  | 66                     |
| Rainbow-I-Circumzenithal   | EUF-CMA        | 1                           | 60192                   | 103648                  | 66                     |
| Rainbow-I-Compressed       | EUF-CMA        | 1                           | 60192                   | 64                      | 66                     |
| Rainbow-III-Classic        | EUF-CMA        | 3                           | 882080                  | 626048                  | 164                    |
| Rainbow-III-Circumzenithal | EUF-CMA        | 3                           | 264608                  | 626048                  | 164                    |
| Rainbow-III-Compressed     | EUF-CMA        | 3                           | 264608                  | 64                      | 164                    |
| Rainbow-V-Classic          | EUF-CMA        | 5                           | 1930600                 | 1408736                 | 212                    |
| Rainbow-V-Circumzenithal   | EUF-CMA        | 5                           | 536136                  | 1408736                 | 212                    |
| Rainbow-V-Compressed       | EUF-CMA        | 5                           | 536136                  | 64                      | 212                    |

## Security considerations

In October 2020, Beullens announced [improved cryptanalysis of Rainbow](https://eprint.iacr.org/2020/1343.pdf) that somewhat reduces the security of the Round 2 and Round 3 parameters.  [As of October 28, 2020](https://groups.google.com/a/list.nist.gov/g/pqc-forum/c/70We3SNi7Ss), the scheme authors have acknowledged the attack and are preparing a response.
