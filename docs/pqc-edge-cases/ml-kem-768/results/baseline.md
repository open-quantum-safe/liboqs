# EC-01 : Valid ML-KEM-768 Operation

## Objective

Verify that a valid ML-KEM-768 keypair, ciphertext,
and decapsulation operation produce matching shared secrets.

## Experiment File

experiments/pqc-edge-cases/ml-kem-768/test_valid_operation.c

## Procedure

1. Initialize the liboqs library.
2. Create an ML-KEM-768 KEM object.
3. Allocate buffers using the lengths provided by the KEM object.
4. Generate a public key and secret key.
5. Encapsulate using the public key.
6. Decapsulate using the secret key and ciphertext.
7. Compare both shared secrets using memcmp().
8. Clean up allocated resources.

## Execution Command

gcc test_valid_operation.c \
-I/home/ubuntu/liboqs-work/build/include \
-L/home/ubuntu/liboqs-work/build/lib \
-loqs -lcrypto -lssl \
-o test_valid_operation

./test_valid_operation

## Observed Output

EC-01 keypair return : 0
EC-01 encapsulation return : 0
EC-01 decapsulation return : 0
EC-01 shared secrets : MATCH

## Results

| Operation | Return value | Result |
|---|---:|---|
| Keypair generation | 0 | Success |
| Encapsulation | 0 | Success |
| Decapsulation | 0 | Success |
| Shared-secret comparison | N/A | MATCH |

## Conclusion

The valid ML-KEM-768 operation completed successfully.
The encapsulated and decapsulated shared secrets matched.

This result serves as the baseline for subsequent
edge-case experiments.

## Integration Relevance

This baseline provides a known-good reference for comparing
the behavior of modified ciphertexts, incorrect keys,
and other non-standard inputs.
