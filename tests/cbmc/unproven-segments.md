# Unproven Segments

This proof set is intentionally limited to small liboqs-owned utility and
dispatch functions. The following segments were not claimed by any bounded
proof in this directory.

| Segment | Status | Reason |
| --- | --- | --- |
| Imported KEM and signature algorithm implementations | `SKIPPED_OUT_OF_SCOPE` | The deliverable is limited to code directly owned by liboqs. |
| XMSS and LMS algorithm-specific signing, verification, deserialization, and lock usage | `SKIPPED_OUT_OF_SCOPE` | These paths are copied/imported implementation code rather than the common wrapper boundary. |
| Registry constructor chains and algorithm identifier string matching | `SKIPPED_OUT_OF_SCOPE` | The selected properties concern callback dispatch after object construction. |
| Platform-specific allocation, cleansing, and free behavior | `SKIPPED_OUT_OF_SCOPE` | The dispatch proofs use no-op free models, and the common proof targets only `OQS_MEM_secure_bcmp`. |
| Constant-time behavior of `OQS_MEM_secure_bcmp` or any cryptographic primitive | `SKIPPED_OUT_OF_SCOPE` | CBMC safety proofs here do not model timing or generated assembly. |
| Application callback implementations and thread scheduling | `SKIPPED_OUT_OF_SCOPE` | The lock proof establishes only the common helper's callback boundary contract. |
