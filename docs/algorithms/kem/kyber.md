# CRYSTALS-Kyber

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: module learning with errors (MLWE)
- **Scheme authors**: Roberto Avanzi, Joppe Bos, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Vadim Lyubashevsky, John M. Schanck, Peter Schwabe, Gregor Seiler, Damien Stehle
- **Authors' website**: https://pq-crystals.org/kyber
- **Version**: NIST Round 3 submission

## Implementation

- **Source of implementation**: https://github.com/pq-crystals/kyber
- **Implementation version**: https://github.com/pq-crystals/kyber.git, master, 8e9308bd0f25fa698e4f37aba216249261f8b352
- **License**: Public domain
- **Constant-time**: Yes
- **Optimizations**: Portable C with AES, AVX2, BMI2, POPCNT, SSE2, SSSE3 instructions (if available at run-time)

## Parameter sets

| Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
| ------------- | -------------- | --------------------------- | ----------------------- | ----------------------- | ----------------------- | -------------------------- |
| Kyber512      | IND-CCA2       | 1                           | 800                     | 1632                    | 768                     | 32                         |
| Kyber768      | IND-CCA2       | 3                           | 1184                    | 2400                    | 1088                    | 32                         |
| Kyber1024     | IND-CCA2       | 5                           | 1568                    | 3168                    | 1568                    | 32                         |
| Kyber512-90s  | IND-CCA2       | 1                           | 800                     | 1632                    | 768                     | 32                         |
| Kyber768-90s  | IND-CCA2       | 1                           | 1184                    | 2400                    | 1088                    | 32                         |
| Kyber1024-90s | IND-CCA2       | 5                           | 1568                    | 3168                    | 1568                    | 32                         |
