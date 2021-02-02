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
- **Implementation version**: https://github.com/PQClean/PQClean/commit/4f86c3951562af84ea4036a7e0483a9e96cebba9
- **License**: public domain
- **Constant-time**: Yes
- **Optimizations**: Portable C with AVX2 and POPCNT instructions (if available at run-time)

Parameter sets
--------------

| Parameter set       | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|---------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| Dilithium_2         |    EUF-CMA     |              2              |          1312           |          2544           |          2420          |
| Dilithium_3         |    EUF-CMA     |              3              |          1952           |          4016           |          3293          |
| Dilithium_5         |    EUF-CMA     |              5              |          2592           |          4880           |          4595          |
| Dilithium_2_AES     |    EUF-CMA     |              2              |          1312           |          2544           |          2420          |
| Dilithium_3_AES     |    EUF-CMA     |              3              |          1952           |          4016           |          3293          |
| Dilithium_5_AES     |    EUF-CMA     |              5              |          2592           |          4880           |          4595          |
