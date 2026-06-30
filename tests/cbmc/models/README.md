# Model Boundary

The proofs in this directory target source files owned by liboqs rather than
imported algorithm implementations.

`oqs/oqsconfig.h` exposes the full stateful-signature object layout by defining
`OQS_ALLOW_STFL_KEY_AND_SIG_GEN`, but it does not enable any KEM, signature, or
stateful-signature algorithm implementation. `oqs/oqs.h` is a narrow umbrella
header that includes only the public common, signature, and stateful-signature
headers needed by the selected translation units.

`oqs_memory.c` provides no-op models for free helpers referenced by wrapper
translation units. Those functions are outside the reachable claims in the
dispatch harnesses. The `mem_secure_bcmp` proof compiles the real
`src/common/common.c` directly and does not use the free-helper model.

The model does not cover:

- imported algorithm constructors or cryptographic callbacks
- allocator behavior, cleansing guarantees, or platform-specific free logic
- thread scheduling or application callback implementations
- constant-time behavior
