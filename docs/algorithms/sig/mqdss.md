MQDSS
=====

- **Algorithm type**: signature
- **Main cryptographic assumption**: intractability of the search version of the MQ problem in the average case
- **Scheme authors**: Ming-Shing Chen, Andreas Hülsing, Joost Rijneveld, Simona Samardjiska, Peter Schwabe
- **Authors' website**: http://mqdss.org
- **Version**: April 2, 2020

Implementation
--------------

- **Source of implementation**: https://github.com/joostrijneveld/MQDSS
- **Implementation version**: https://github.com/PQClean/PQClean/commit/9023fef55861faccd82146cf599b9e46fb9606aa
- **License**: CC0 1.0 Universal
- **Constant-time**: Yes
- **Optimizations**: Portable C

Parameter sets
--------------

| Parameter set | Security model | Claimed NIST security level | Public key size (bytes) | Secret key size (bytes) | Signature size (bytes) |
|---------------|:--------------:|:---------------------------:|:-----------------------:|:-----------------------:|:----------------------:|
| MQDSS-31-48   |     EUF-CMA    |             1-2             |            46           |            16           |          28400         |
| MQDSS-31-64   |     EUF-CMA    |             3-4             |            64           |            24           |          59928         |

