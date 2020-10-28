NTRU
====

- **Algorithm type**: key encapsulation mechanism
- **Main cryptographic assumption**: NTRU
- **Scheme authors**: John M. Schanck, Cong Chen, Oussama Danba, Jeffrey Hoffstein, Andreas Hülsing, Joost Rijneveld, Peter Schwabe, William Whyte, Zhenfei Zhang
- **Authors' website**: https://ntru.org
- **Version**: NIST Round 2 submission

Implementation
--------------

- **Source of implementation**: NIST Round 2 submission
- **Implementation version**: https://github.com/jschanck/ntru/tree/6d1f44f5
- **License**: Public domain
- **Constant-time**: Yes
- **Optimizations**: Portable C, AVX2

Parameter sets
--------------

| Parameter set     | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Ciphertext size (bytes) | Shared secret size (bytes) |
|-------------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:-----------------------:|:--------------------------:|
| NTRU-HPS-2048-509 |     IND-CCA    |              1              |           699           |           935           |           699           |             32             |
| NTRU-HPS-2048-677 |     IND-CCA    |              3              |           930           |           1234          |           930           |             32             |
| NTRU-HPS-4096-821 |     IND-CCA    |              5              |           1230          |           1590          |           1230          |             32             |
| NTRU-HRSS-701     |     IND-CCA    |              3              |           1138          |           1450          |           1138          |             32             |
