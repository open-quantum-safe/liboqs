liboqs nist-branch algorithm datasheet: `kem_lima`
==================================================

Summary
-------

- **Name**: LIMA
- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: ring learning with errors (RLWE)
- **NIST submission URL**: https://csrc.nist.gov/CSRC/media/Projects/Post-Quantum-Cryptography/documents/round-1/submissions/Lima.zip
- **Submitters (to NIST competition)**: Nigel P. Smart, Martin R. Albrecht, Yehuda Lindell, Emmanuela Orsini, Valery Osheter, Kenny Paterson, Guy Peer
- **Submitters' website**: https://lima-pq.github.io/
- **Added to liboqs by**: Douglas Stebila

Parameter sets
--------------

|                      | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|----------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| Lima-2p-1024-CCA-KEM |     IND-CCA    |              3              |           6145          |           9217          |           4227          |             32             |
| Lima-2p-2048-CCA-KEM |     IND-CCA    |              4              |          12289          |          18433          |           7299          |             32             |
| Lima-sp-1018-CCA-KEM |     IND-CCA    |              1              |           6109          |           9163          |           4209          |             32             |
| Lima-sp-1306-CCA-KEM |     IND-CCA    |              2              |          10449          |          15673          |           6763          |             32             |
| Lima-sp-1822-CCA-KEM |     IND-CCA    |              3              |          14577          |          21865          |           8827          |             32             |
| Lima-sp-2062-CCA-KEM |     IND-CCA    |              4              |          16497          |          24745          |           9787          |             32             |

Implementation
--------------

- **Source of implementation:** https://lima-pq.github.io/ optimized implementation   (commit: c660c24db8ddbdce097a2bb19059c6896ef1c27c)
- **License:** Public domain
- **Language:** C
- **Constant-time:** Unknown
- **Architectures supported in liboqs nist-branch**: x86, x64
