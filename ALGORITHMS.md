# Supported algorithms

## Key Encapsulation Mechanisms (KEMs)

| Algorithm | Level of standardization | Implementation source |
| - | - | - |
| BIKE | Not selected | https://github.com/awslabs/bike-kem |
| Classic McEliece | Standards-track | https://github.com/PQClean/PQClean |
| CRYSTALS-Kyber | Standards-track | https://github.com/pq-crystals/kyber |
| FrodoKEM | Standards-track | https://github.com/Microsoft/PQCrypto-LWEKE |
| ML-KEM | Standardized | https://github.com/pq-code-package/mlkem-native |
| HQC | Standards-track | https://github.com/PQClean/PQClean |
| NTRU-Prime | Not selected | https://github.com/PQClean/PQClean |

## Digital Signature Schemes

| Algorithm | Level of standardization | Implementation source |
| - | - | - |
| CROSS | Under consideration | https://github.com/CROSS-signature/CROSS-lib-oqs |
| CRYSTALS-Dilithium | Standards-track | https://github.com/pq-crystals/dilithium (master branch) |
| Falcon | Standards-track |
| LMS | Standardized | https://github.com/cisco/hash-sigs |
| MAYO | Under consideration | https://github.com/PQCMayo/MAYO-C |
| ML-DSA | Standardized | https://github.com/pq-crystals/dilithium (round3 branch) |
| SPHINCS+ | Standards-track | https://github.com/PQClean/PQClean |
| UOV | Under consideration | https://github.com/PQCMayo/MAYO-C |
| XMSS | Standardized | Contributed directly to liboqs |

Standardization levels
- standardized: the algorithm has standardized by a recognized standards body (e.g., NIST, ISO)
- standards-track: the algorithm has been selected for standardization by a recognized standards body
- under consideration: the algorithm is under consideration for standardization by a recognized standards body
- not selected: the algorithm is no longer or never was under consideration for standardization by a recognized standards body

Maintenance levels
- does OQS have a close working relationship with the maintainers of the imported code?
- is the upstream committed to security patches?

Implementation quality
- formally verified
- passes constant-time tests

## Support tiers

### Tier 1 ("Production")

- standardized by a recognized body
- "production-grade" implementation
- close working relationship with the upstream source
- commitment to security patches
- commitment to long-term support
- constant-time tested

currently only ML-KEM

### Tier 2 ("Production-track")

- standards-track or higher

ML-DSA, Kyber, Dilithium, Falcon, SPHINCS+

### Tier 3 ("Experimental")

HQC, LMS, XMSS, UOV, MAYO, CROSS

### Tier 4 ("Legacy")

BIKE, NTRU-Prime
