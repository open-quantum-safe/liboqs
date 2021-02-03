CRYSTALS-Dilithium
==================

- **Algorithm type**: signature
- **Main cryptographic assumption**: hardness of lattice problems over module lattices.
- **Scheme authors**: Vadim Lyubashevsky, Leo Ducas, Eike Kiltz, Tancrede Lepoint, Peter Schwabe, Gregor Seiler, Damien Stehle
- **Authors' website**: https://pq-crystals.org/dilithium/
- **Version**: NIST Round 3 submission

Implementation
--------------

- **Source of implementation**: https://github.com/pq-crystals/dilithium
- **Implementation version**: https://github.com/pq-crystals/dilithium/tree/71c1e9e912a6eb9bc7a40c9892aa4a382c968ead
- **License**: public domain
- **Constant-time**: Yes
- **Optimizations**: Portable C with AES, AVX2, POPCNT, SSE2 and SSSE3 instructions (if available at run-time)

Parameter sets
--------------

| Parameter set       | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|---------------------|----------------|-----------------------------|-------------------------|-------------------------|------------------------|
| Dilithium2          | EUF-CMA        | 2                           | 1312                    | 2544                    | 2420                   |
| Dilithium2-AES      | EUF-CMA        | 2                           | 1312                    | 2544                    | 2420                   |
| Dilithium3          | EUF-CMA        | 3                           | 1952                    | 4016                    | 3293                   |
| Dilithium3-AES      | EUF-CMA        | 3                           | 1952                    | 4016                    | 3293                   |
| Dilithium5          | EUF-CMA        | 5                           | 2592                    | 4880                    | 4595                   |
| Dilithium5-AES      | EUF-CMA        | 5                           | 2592                    | 4880                    | 4595                   |
