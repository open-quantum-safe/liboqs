# Liboqs-Owned CBMC Proof Report

## Scope

This report records four bounded CBMC proofs over source files implemented
directly by liboqs:

- `src/common/common.c`
- `src/sig/sig.c`
- `src/sig_stfl/sig_stfl.c`

Imported algorithm implementations are disabled by the model header and are
not part of these claims.

## Results

| Proof | Property | Status | Safety result |
| --- | --- | --- | --- |
| `mem_secure_bcmp` | Four-byte bounded functional and non-mutation contract for `OQS_MEM_secure_bcmp` | `PROVED_BOUNDED` | `0 of 46 failed` |
| `sig_status_normalization` | Generic signature callback forwarding and status normalization | `PROVED_BOUNDED` | `0 of 137 failed` |
| `sig_stfl_dispatch_contract` | Stateful wrapper forwarding, null rejection, and status handling | `PROVED_BOUNDED` | `0 of 168 failed` |
| `sig_stfl_lock_contract` | Common stateful lock helper callback boundary contract | `PROVED_BOUNDED` | `0 of 105 failed` |

Every safety run ended in `VERIFICATION SUCCESSFUL`. Companion
assertion-coverage runs are checked in beside the safety output and show the
harness assertions as reachable; they are not used as proof statuses.

## Method

The suite runs CBMC 6.9.0 with bounds, pointer, pointer-overflow,
division-by-zero, signed-overflow, and conversion checks. Loops are unwound to
5 with unwinding assertions enabled. `mem_secure_bcmp` is bounded to four
compared bytes, which requires at most four loop iterations plus the terminating
condition.

The signature and stateful-signature harnesses compile the real liboqs-owned
translation units with a narrow public-header model. The model keeps the
relevant object layouts intact, disables imported algorithms, and replaces only
unreachable free helpers with no-op stubs. Callback bodies are harness models
because the claims stop at the liboqs-owned dispatch boundary.

## Interpretation

These proofs establish small, reusable contracts at the common API boundary:

- generic signature wrappers cannot leak backend-specific nonzero statuses
  through the generic API
- stateful wrapper null checks and callback routing behave consistently with
  their source implementation
- the common stateful lock helpers enforce their local callback/mutex contract
- the constant-time compare helper's bounded functional result is normalized
  and non-mutating

They do not establish algorithm-level parsing, canonicality, side-channel, or
state-machine properties. Those exclusions are recorded in
`tests/cbmc/unproven-segments.md`.
