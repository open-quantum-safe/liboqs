# sig_status_normalization

Status: `PROVED_BOUNDED`

Source: `src/sig/sig.c:3062`

Harness: `tests/cbmc/harnesses/sig_status_normalization_harness.c`

Property: `OQS_SIG_keypair`, `OQS_SIG_sign`, `OQS_SIG_sign_with_ctx_str`,
`OQS_SIG_verify`, and `OQS_SIG_verify_with_ctx_str` reject a missing object or
missing selected callback without calling through it. For a present callback,
they forward every argument unchanged, invoke exactly once, and return
`OQS_SUCCESS` only when the callback returned `OQS_SUCCESS`; all other callback
statuses become `OQS_ERROR`.

Bound and assumptions:

- Imported algorithms are disabled; callbacks are harness models with
  nondeterministic `OQS_STATUS` results.
- The harness nondeterministically selects each of the five generic
  dispatchers, a NULL object case, and a NULL callback case.
- No callback dereferences caller-provided byte buffers; the claim is about the
  liboqs-owned dispatch contract only.

Result: The safety run reports `0 of 137 failed` and ends in
`VERIFICATION SUCCESSFUL`. The companion assertion-coverage run marks every
harness callback assertion and every top-level dispatch assertion as
`SATISFIED`.

Evidence:

- `tests/cbmc/raw-runs/sig_status_normalization.txt`
- `tests/cbmc/raw-runs/sig_status_normalization.coverage.txt`
