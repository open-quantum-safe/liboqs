liboqs nist-branch algorithm datasheet: `kem_kyber`
===================================================

Summary
-------

- **Name**: CRYSTALS-KYBER
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: module learning with errors (MLWE)
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/CRYSTALS_Kyber.zip
- **Submitters (to NIST competition)**: Peter Schwabe, Roberto Avanzi, Joppe Bos, Léo Ducas, Eike Kiltz, Tancrède Lepoint, Vadim Lyubashevsky, John M. Schanck, Gregor Seiler, Damien Stehle
- **Submitters' website**: https://pq-crystals.org/
- **Added to liboqs by**: Tancrède Lepoint

Parameter sets
--------------

| Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| Kyber512      |     IND-CCA    |              1              |           736           |           1632          |           800           |             32             |
| Kyber768      |     IND-CCA    |              3              |           1088          |           2400          |           1152          |             32             |
| Kyber1024     |     IND-CCA    |              5              |           1440          |           3168          |           1504          |             32             |

Implementation
--------------

- **Source of implementation:** https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/CRYSTALS_Kyber.zip (optimized variant)
- **License:** Public domain
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs nist-branch**: x86, x64

Additional comments
-------------------

https://github.com/pq-crystals/kyber includes an AVX2 optimized implementation.
