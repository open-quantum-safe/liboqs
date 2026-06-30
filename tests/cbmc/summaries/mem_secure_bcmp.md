# mem_secure_bcmp

Status: `PROVED_BOUNDED`

Source: `src/common/common.c:259`

Harness: `tests/cbmc/harnesses/mem_secure_bcmp_harness.c`

Property: For byte arrays of length at least `len`, with `0 <= len <= 4`,
`OQS_MEM_secure_bcmp` returns `0` iff all compared bytes match, returns only
`0` or `1`, and does not modify either input array.

Bound and assumptions:

- The harness allocates two four-byte arrays and assumes `len <= 4`.
- CBMC unwinds every loop to 5 with unwinding assertions enabled.
- The proof checks the real implementation in `src/common/common.c`; it does
  not model constant-time execution.

Result: The safety run reports `0 of 46 failed` and ends in
`VERIFICATION SUCCESSFUL`. The companion coverage run reports `26 of 26
covered`, including all four harness assertions.

Evidence:

- `tests/cbmc/raw-runs/mem_secure_bcmp.txt`
- `tests/cbmc/raw-runs/mem_secure_bcmp.coverage.txt`
