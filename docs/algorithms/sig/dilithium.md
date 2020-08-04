CRYSTALS-Dilithium
==================

- **Algorithm type**: signature
- **Main cryptographic assumption**: hardness of lattice problems over module lattices.
- **Scheme authors**: Vadim Lyubashevsky, Leo Ducas, Eike Kiltz, Tancrede Lepoint, Peter Schwabe, Gregor Seiler, Damien Stehle
- **Authors' website**: https://pq-crystals.org/dilithium/
- **Version**: NIST Round 2 submission

Implementation
--------------

- **Source of implementation**: https://github.com/pq-crystals/dilithium
- **Implementation version**: https://github.com/pq-crystals/dilithium/tree/497a98bc7efe48cb0d18fd20d6d9a4b5629406ef
- **License**: public domain
- **Constant-time**: Yes
- **Optimizations**: Portable C with AVX2, BMI1, and POPCNT instructions (if available at run-time)

Parameter sets
--------------

| Parameter set       | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|---------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| DILITHIUM_2         |    EUF-CMA     |              1              |          1184           |          2800           |          2044          |
| DILITHIUM_3         |    EUF-CMA     |              2              |          1472           |          3504           |          2701          |
| DILITHIUM_4         |    EUF-CMA     |              3              |          1760           |          3856           |          3366          |
