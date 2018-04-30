liboqs nist-branch algorithm datasheet: `kem_ledakem`
======================================================

Summary
-------

- **Name**: LedaKEM
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: Niederreiter cryptosystem based on linear error-correcting codes
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/LEDAkem.zip
- **Submitters (to NIST competition)**: Marco Baldi Alessandro Barenghi Franco Chiaraluce Gerardo Pelosi Paolo Santini
- **Submitters' website**: https://www.ledacrypt.org/LEDAkem/
- **Added to liboqs by**: Shravan Mishra 

Parameter sets
--------------

| Parameter set       | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|---------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| LEDAkem_C1_N02      |     IND-CCA    |              1              |           3480      |          24                 |         3480            |             32             |
| LEDAkem_C1_N03      |     IND-CCA    |              1              |           4688      |          24                 |         2344            |             32             |
| LEDAkem_C1_N04      |     IND-CCA    |              1              |           6408      |          24                 |         2136            |             32             |
| LEDAkem_C3_N02      |     IND-CCA    |              3              |           7200      |          32                 |         7200            |             48             |
| LEDAkem_C3_N03      |     IND-CCA    |              3              |           10384     |          32                 |         5192            |             48             |
| LEDAkem_C3_N04      |     IND-CCA    |              3              |           13152     |          32                 |         4384            |             48             |
| LEDAkem_C5_N02      |     IND-CCA    |              5              |           12384     |          40                 |         12384           |             64             |
| LEDAkem_C5_N03      |     IND-CCA    |              5              |           18016     |          40                 |         9008            |             64             |
| LEDAkem_C5_N04      |     IND-CCA    |              5              |           22704     |          40                 |         7568            |             64             |

Implementation
--------------

- **Source of implementation:** https://github.com/LEDAcrypt/LEDAkem
- **License:** MIT License
- **Language:** C
- **Constant-time:** Yes
- **Architectures supported in liboqs nist-branch**: x86, x64

