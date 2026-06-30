# sig_stfl_lock_contract

Status: `PROVED_BOUNDED`

Source: `src/sig_stfl/sig_stfl.c:1563`, `src/sig_stfl/sig_stfl.c:1587`

Harness: `tests/cbmc/harnesses/sig_stfl_lock_contract_harness.c`

Property: The common stateful-signature setter functions store the selected
lock, unlock, and mutex fields. `OQS_SIG_STFL_SECRET_KEY_lock` and
`OQS_SIG_STFL_SECRET_KEY_unlock` return `OQS_ERROR` for a NULL key, return
`OQS_SUCCESS` without invoking anything when the selected callback is unset,
return `OQS_ERROR` without invoking anything when a selected callback has no
mutex, and otherwise invoke exactly once with the configured mutex while
preserving the callback status.

Bound and assumptions:

- The harness selects lock or unlock nondeterministically and explores NULL
  key, absent callback, absent mutex, and configured callback cases.
- The application callback is modeled as a side-effect-free function that
  records its argument and returns a nondeterministic `OQS_STATUS`.
- This proof is about the common helper functions, not algorithm-specific use
  of those helpers inside XMSS or LMS signing code.

Result: The safety run reports `0 of 105 failed` and ends in
`VERIFICATION SUCCESSFUL`. The companion assertion-coverage run marks all 13
harness assertions as `SATISFIED`.

Evidence:

- `tests/cbmc/raw-runs/sig_stfl_lock_contract.txt`
- `tests/cbmc/raw-runs/sig_stfl_lock_contract.coverage.txt`
