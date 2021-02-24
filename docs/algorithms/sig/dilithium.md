# CRYSTALS-Dilithium

- **Algorithm type**: signature
- **Main cryptographic assumption**: hardness of lattice problems over module lattices.
- **Scheme authors**: Vadim Lyubashevsky, Leo Ducas, Eike Kiltz, Tancrede Lepoint, Peter Schwabe, Gregor Seiler, Damien Stehle
- **Authors' website**: https://pq-crystals.org/dilithium/
- **Version**: 3.1

## Implementation

- **Source of implementation**: https://github.com/pq-crystals/dilithium
- **Implementation version**: https://github.com/pq-crystals/dilithium.git, master, 9dddb2a0537734e749ec2c8d4f952cb90cd9e67b
- **License**: public domain
- **Constant-time**: Yes
- **Optimizations**: Portable C with AES, AVX2, POPCNT, SSE2, SSSE3 instructions (if available at run-time)

## Parameter sets

| Parameter set  | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
| -------------- | -------------- | --------------------------- | ----------------------- | ----------------------- | ---------------------- |
| Dilithium2     | EUF-CMA        | 2                           | 1312                    | 2528                    | 2420                   |
| Dilithium3     | EUF-CMA        | 3                           | 1952                    | 4000                    | 3293                   |
| Dilithium5     | EUF-CMA        | 5                           | 2592                    | 4864                    | 4595                   |
| Dilithium2-AES | EUF-CMA        | 2                           | 1312                    | 2528                    | 2420                   |
| Dilithium3-AES | EUF-CMA        | 3                           | 1952                    | 4000                    | 3293                   |
| Dilithium5-AES | EUF-CMA        | 5                           | 2592                    | 4864                    | 4595                   |
