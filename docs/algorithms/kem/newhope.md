NewHope
=======

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: ring learning with errors (RLWE)
- **Scheme authors**: Thomas Pöppelmann, Erdem Alkim, Roberto Avanzi, Joppe Bos, Léo Ducas, Antonio de la Piedra, Peter Schwabe, Douglas Stebila, Martin R. Albrecht, Emmanuela Orsini, Valery Osheter, Kenneth G. Paterson, Guy Peer, Nigel P. Smart
- **Authors' website**: https://newhopecrypto.org/
- **Version**: NIST Round 2 submission

Implementation
--------------

- **Source of implementation**: https://github.com/newhopecrypto
- **Implementation version**: https://github.com/PQClean/PQClean/commit/90630db2ebada4bacceb5331b0a1a9a356ba65b9
- **License**: Public domain
- **Constant-time**: Yes
- **Optimizations**: Portable C

Parameter sets
--------------

| Parameter set        | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|----------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| NewHope-512-CCA      |     IND-CCA    |              1              |           928           |          1888           |           1120          |             32             |
| NewHope-1024-CCA     |     IND-CCA    |              5              |          1824           |          3680           |           2208          |             32             |
