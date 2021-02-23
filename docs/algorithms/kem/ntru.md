# NTRU

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: NTRU
- **Scheme authors**: John M. Schanck, Cong Chen, Oussama Danba, Jeffrey Hoffstein, Andreas Hülsing, Joost Rijneveld, Peter Schwabe, William Whyte, Zhenfei Zhang
- **Authors' website**: https://ntru.org
- **Version**: NIST Round 3 submission

## Implementation

- **Source of implementation**: https://github.com/jschanck/ntru/tree/a43a4457
- **Implementation version**: https://github.com/PQClean/PQClean.git, master, ebcc71c51a30b6e5db4f1fade22999b346fdafce
- **License**: Public domain
- **Constant-time**: Yes
- **Optimizations**: Portable C with AVX2, BMI2 instructions (if available at run-time)

## Parameter sets

| Parameter set     | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
| ----------------- | -------------- | --------------------------- | ----------------------- | ----------------------- | ----------------------- | -------------------------- |
| NTRU-HPS-2048-509 | IND-CCA2       | 1                           | 699                     | 935                     | 699                     | 32                         |
| NTRU-HPS-2048-677 | IND-CCA2       | 3                           | 930                     | 1234                    | 930                     | 32                         |
| NTRU-HPS-4096-821 | IND-CCA2       | 5                           | 1230                    | 1590                    | 1230                    | 32                         |
| NTRU-HRSS-701     | IND-CCA2       | 3                           | 1138                    | 1450                    | 1138                    | 32                         |
