CRYSTALS-Kyber
==============

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: module learning with errors (MLWE)
- **Scheme authors**: Roberto Avanzi, Joppe Bos, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Vadim Lyubashevsky, John M. Schanck, Peter Schwabe, Gregor Seiler, Damien Stehle
- **Authors' website**: https://pq-crystals.org/kyber
- **Version**: NIST Round 3 submission

Implementation
--------------

- **Source of implementation**: https://github.com/pq-crystals/kyber/commit/e7faae9f662f5b92fee4e966f09b2f23e1e91c65 via https://github.com/jschanck/package-pqclean/tree/231c9bec/kyber
- **Implementation version**: https://github.com/PQClean/PQClean/commit/168e3625988defacdf10812642e4d412fb295dc0
- **License**: Public domain
- **Constant-time**: Yes
- **Optimizations**: Portable C with AVX2 instructions (if available at run-time)

Parameter sets
--------------

| Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------|----------------|-----------------------------|-------------------------|-------------------------|-------------------------|----------------------------|
| Kyber512      | IND-CCA        | 1                           | 800                     | 1632                    | 768                     | 32                         |
| Kyber512-90s  | IND-CCA        | 1                           | 800                     | 1632                    | 768                     | 32                         |
| Kyber768      | IND-CCA        | 3                           | 1184                    | 2400                    | 1088                    | 32                         |
| Kyber768-90s  | IND-CCA        | 3                           | 1184                    | 2400                    | 1088                    | 32                         |
| Kyber1024     | IND-CCA        | 5                           | 1568                    | 3168                    | 1568                    | 32                         |
| Kyber1024-90s | IND-CCA        | 5                           | 1568                    | 3168                    | 1568                    | 32                         |
