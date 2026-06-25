# CBMC Proofs For Liboqs-Owned Code

This directory contains bounded proofs for small API and utility functions that
are implemented directly by liboqs. The proof set intentionally excludes copied
or imported cryptographic implementations.

Run the suite from the repository root:

```sh
tests/cbmc/run.sh
```

The runner writes safety results and companion assertion-coverage runs to
`tests/cbmc/raw-runs/`. The checked-in results were produced with CBMC 6.9.0 at
commit `d95fc734f5de9bdfc80c8c65d8166b30e9cff5cd`.

| Proof | Owned source | Status |
| --- | --- | --- |
| `mem_secure_bcmp` | `src/common/common.c` | `PROVED_BOUNDED` |
| `sig_status_normalization` | `src/sig/sig.c` | `PROVED_BOUNDED` |
| `sig_stfl_dispatch_contract` | `src/sig_stfl/sig_stfl.c` | `PROVED_BOUNDED` |
| `sig_stfl_lock_contract` | `src/sig_stfl/sig_stfl.c` | `PROVED_BOUNDED` |

The model header disables every imported algorithm implementation while keeping
the public `OQS_SIG` and full `OQS_SIG_STFL` layouts visible. This lets CBMC
compile the real liboqs-owned translation units and reason about their wrapper
control flow without claiming anything about algorithm internals.

Every safety run uses bounds, pointer, pointer-overflow, division-by-zero,
signed-overflow, and conversion checks with loop unwinding set to 5 and
unwinding assertions enabled. The companion coverage runs are not proof
statuses; they are kept to show that the harness assertions are reachable.
