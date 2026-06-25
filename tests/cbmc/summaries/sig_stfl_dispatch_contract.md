# sig_stfl_dispatch_contract

Status: `PROVED_BOUNDED`

Source: `src/sig_stfl/sig_stfl.c:1006`, `src/sig_stfl/sig_stfl.c:1545`

Harness: `tests/cbmc/harnesses/sig_stfl_dispatch_contract_harness.c`

Property: `OQS_SIG_STFL_keypair`, `OQS_SIG_STFL_sign`,
`OQS_SIG_STFL_verify`, `OQS_SIG_STFL_sigs_remaining`, and
`OQS_SIG_STFL_sigs_total` reject missing objects or callbacks, forward all
arguments unchanged, invoke exactly once, and normalize nonzero callback
results to `OQS_ERROR`. `OQS_SIG_STFL_SECRET_KEY_serialize` and
`OQS_SIG_STFL_SECRET_KEY_deserialize` reject their wrapper-owned NULL object,
callback, output-pointer, output-length, and input-buffer cases before
dispatching, then preserve the selected callback's `OQS_STATUS` result.

Bound and assumptions:

- The model defines `OQS_ALLOW_STFL_KEY_AND_SIG_GEN` so the actual stateful
  object layout is visible while imported XMSS/LMS implementations remain
  disabled.
- The harness nondeterministically selects all seven wrapper paths plus NULL
  object, NULL callback, and wrapper-owned NULL serialization argument cases.
- Callback internals are intentionally abstract; the proof covers only the
  liboqs-owned dispatch boundary.

Result: The safety run reports `0 of 168 failed` and ends in
`VERIFICATION SUCCESSFUL`. The companion assertion-coverage run marks every
top-level harness assertion and every modeled callback forwarding assertion as
`SATISFIED`.

Evidence:

- `tests/cbmc/raw-runs/sig_stfl_dispatch_contract.txt`
- `tests/cbmc/raw-runs/sig_stfl_dispatch_contract.coverage.txt`
