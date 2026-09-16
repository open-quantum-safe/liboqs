# liboqs Security Threat Model

This document describes the security boundaries that liboqs expects researchers,
contributors, and automated agents to use when evaluating possible
vulnerabilities. It complements `SECURITY.md`, `PLATFORMS.md`, `ALGORITHMS.md`,
and the per-algorithm documentation under `docs/algorithms/`.

The purpose is practical triage: a good report should explain which boundary is
crossed, how the issue is reachable, what evidence reproduces it, and why it is
not merely a known limitation, unsupported platform behavior, or misuse of the
library.

## Project Security Role

liboqs is a C library that exposes post-quantum KEM, signature, stateful
signature, random, hash, AES, SHA-2, SHA-3, memory, test, and benchmark support
to downstream applications. liboqs is often embedded into other systems, so the
remote attacker is usually remote to the application using liboqs rather than
remote to liboqs as a standalone network service.

For threat-model purposes, liboqs assumes callers satisfy the C memory contract
documented by each API: pointer arguments refer to readable or writable regions
of at least the documented size, unless the API explicitly permits `NULL` or a
shorter buffer. liboqs cannot determine whether a caller allocated less memory
than it claims.

Within those preconditions, treat liboqs as responsible for safely handling
attacker-controlled byte contents and any explicit lengths, counts, identifiers,
or serialized-state fields accepted by its public APIs, preserving documented
API contracts, and avoiding secret-dependent behavior within the scope described
below.

## Protected Assets And Security Goals

In this document, assets are the values or security properties whose compromise
matters. The list includes both secret material and required behavior:

- Secret keys, stateful signature private state, seed material, shared secrets,
  and intermediate secret values.
- Correct, specification-compliant behavior for both honest inputs and
  maliciously crafted inputs that satisfy documented API preconditions.
- Public API integrity for KEM, signature, stateful signature, and common crypto
  operations.
- Correct rejection of malformed ciphertexts, signatures, public keys, secret-key
  encodings, serialized state, context strings, and parameter identifiers.
- Correct state transitions for stateful signatures, including key exhaustion,
  one-time-signature index use, persistence callbacks, locking, and deserialize
  behavior.
- Memory safety of public entry points and testable internal entry points used by
  liboqs wrappers.
- Cleansing of secret-derived material when an API or implementation cleanup path
  is intended to end its lifetime.
- Constant-time behavior where liboqs claims or expects it, especially for
  secret-dependent branches, memory accesses, and variable-time operations.

## Trust Boundaries

### Public API Boundary

The public API is the main boundary for security reports. Inputs crossing this
boundary include:

- KEM public keys, ciphertexts, and secret keys.
- Signature public keys, signatures, messages, context strings, and secret keys.
- Stateful signature public keys, signatures, serialized secret keys, lock/store
  callback results, and exhausted-key states.
- Algorithm names and build-time feature selections that control which
  implementations are reachable.

For APIs that take pointer-plus-length pairs, the pointer must satisfy the
documented memory-region precondition. Once that precondition holds, the bytes
inside the region and the explicit length value are untrusted inputs: liboqs
should not read past the supplied length, accept malformed serialized data, or
rely on unchecked attacker-controlled metadata.

A report is stronger when it reaches the issue through an `OQS_API` function or
through a generated test binary that exercises the same wrapper path.

### Common Crypto Boundary

The `src/common` subsystem is high value because it is shared by multiple
algorithms and wrappers. Bugs in random generation, memory cleansing, hashing,
AES, SHA-2, SHA-3, CPU feature dispatch, OpenSSL integration, or internal common
APIs may affect many algorithms at once.

### Stateful Signature Boundary

Stateful signatures have an additional persistence and concurrency boundary. A
valid finding may involve reuse of one-time signing material, stale signatures,
incorrect success statuses, key exhaustion mishandling, failed state persistence,
bad lock semantics, or unsafe deserialization that can affect future signatures.

### Upstream Implementation Boundary

liboqs integrates code from multiple upstream cryptographic implementations.
Findings in imported third-party code can still affect liboqs users, but the
correct fix may belong upstream. Reports should identify whether the affected
code is liboqs-maintained wrapper/common code, generated integration code, or an
imported implementation. Do not assume OQS maintainers own the long-term fix for
every imported implementation.

## In-Scope Vulnerability Classes

The following classes are generally in scope when they are reachable in a
supported build and backed by a working reproducer.

### Memory Safety

- Out-of-bounds read or write.
- Use-after-free, double free, invalid free, or uninitialized memory disclosure.
- Integer overflow or truncation that leads to memory corruption, invalid
  acceptance, incorrect output lengths, or denial of service.
- NULL pointer dereference reachable through a public API with attacker-controlled
  input.

### Cryptographic API Correctness

- Verifiers accepting invalid signatures, signatures from the wrong parameter
  set, or non-canonical encodings when exact encoding matters to the algorithm or
  API contract.
- KEM operations accepting malformed public keys or ciphertexts in a way that
  changes the expected security or canonicalization contract.
- Wrong success or failure status when callers rely on the return value before
  using generated output.
- Mismatched public/secret key metadata, serialized state, OIDs, parameter sets,
  context strings, or algorithm identifiers that lead to unsafe behavior.

### Stateful Signature Safety

- Reuse of LM-OTS, WOTS+, or other one-time signing material.
- Signing after key exhaustion returning success or leaving stale output.
- Failed lock or store callbacks that still allow publication of signatures whose
  state was not safely committed.
- Deserialization of corrupt, cross-parameter, or mismatched state that can
  corrupt signing state or bypass wrapper validation.

### Secret Material Lifetime

- Secret keys, seeds, shared secrets, or secret-derived intermediates remaining
  in memory after a documented free, clear, or cleanup path when a realistic
  caller-visible or post-free disclosure path can recover them.
- Error paths that skip required cleansing and leave secret-derived bytes in
  output buffers, reusable allocator memory, or other storage beyond their
  intended lifetime.

A missing `OQS_MEM_cleanse` call or similar source pattern is not enough by
itself. A report should identify the secret source, the intended lifetime
boundary, the residual bytes, and how they can be observed or reused.

### Side Channels

Some timing-based side-channel attacks are in scope. Reports are most actionable
when they show secret-dependent control flow, secret-dependent memory access, or
variable-time instructions involving secret material in a concrete binary for a
supported platform.

Use the existing Valgrind-based constant-time tests as evidence, but do not treat
them as proofs. They are auditor aids and have known coverage limits.

Timing is security-relevant only when it reveals data that is not already public
at the API boundary or visible in the request/response transcript. Do not report
variable-time arithmetic, branches, table lookups, or loops as vulnerabilities
until the operands have been traced to hidden secret material.

Rejected constant-time patterns include:

- Division in NTRUPrime `Decode` where the divisor is the public radix vector and
  the numerator is derived from attacker-controlled ciphertext bytes.
- Division in MAYO `sample_solution` where operands are public dimensions and
  loop indices, not secret pivot locations.
- Division in MQOM PRG helpers where lengths are fixed protocol sizes or public
  call parameters.
- Apparent x86 `idiv` instructions decoded from Mach-O jump-table data or other
  `data-in-code` ranges rather than executable code.
- XMSS/XMSSMT parameter arithmetic over public OID-selected parameters.
- LMS/HSS sizing arithmetic over public parameter sets, memory budgets,
  auxiliary-data lengths, or thread counts.
- LM-OTS coefficient and checksum divisions over public widths, lengths, and
  transcript-visible chain schedules.
- SNOVA public-key expansion table lookups and explicitly declassified signing
  retry timing already documented in constant-time suppressions.
- XMSS WOTS and LMS LM-OTS chain-length timing derived from signature-visible
  digits rather than hidden authentication state or secret key material.

### Denial Of Service

Denial-of-service reports are in scope when an attacker can trigger a crash,
unbounded allocation, infinite loop, or severe resource exhaustion through an
ordinary public API use by a downstream application. A report should describe the
attacker-controlled input, expected deployment path, and whether the issue
requires an application to expose signing, verification, decapsulation, or
deserialization to remote clients.

## Out-Of-Scope Or Usually Non-Vulnerabilities

The following are usually not liboqs vulnerabilities by themselves:

- Physical access attacks.
- Physical observation side channels such as power analysis or electromagnetic
  emissions.
- Physical fault injection attacks, including Rowhammer-style attacks.
- CPU or hardware flaws.
- Same-physical-system side channels outside the documented timing model.
- Reports that only state a post-quantum algorithm may someday be cryptanalyzed,
  without an implementation flaw in liboqs.
- Claims based only on source pattern matching with no reachable PoC, test, or
  binary evidence.
- Timing claims where the branch, memory access, or variable-time operation is
  demonstrably controlled only by public data.
- Timing claims matching a rejected pattern listed in this threat model without
  new evidence that a hidden secret reaches the flagged operation.
- Behavior on unsupported or lower-tier platforms without evidence that the same
  issue affects a prioritized supported platform.
- Reports requiring an attacker to modify the installed library, test binaries,
  build scripts, compiler, linker, source tree, or runtime environment before the
  application uses liboqs.
- Crashes or out-of-bounds accesses that require a caller to pass a pointer that
  does not reference the documented readable or writable region while claiming
  that it does, unless the API explicitly permits that shorter region.
- Claims of missing zeroization with no secret-derived dataflow, no intended
  cleanup boundary, or no realistic observation path.
- Vulnerabilities solely in downstream applications that misuse liboqs despite a
  clear API contract.
- Vulnerabilities solely in imported upstream implementations where the report
  does not explain how the code is reachable through liboqs or how OQS should
  coordinate the fix.

Mitigations for out-of-scope issues may still be accepted as hardening, but they
should not be submitted as confirmed security vulnerabilities without maintainer
agreement.

## Platform And Algorithm Priority

Prioritize Tier 1 platforms from `PLATFORMS.md`, especially x86_64 Linux and
aarch64 platforms. Tier 1 platforms receive the strongest CI coverage and
security support. Reports on Tier 2 or Tier 3 platforms should explain whether
the issue also affects Tier 1, whether it is platform-specific, and whether the
platform has enough maintainer support for a fix.

Prioritize algorithms and code according to `ALGORITHMS.md`:

- Tier 1 and Tier 2 algorithms generally deserve more attention than Tier 3
  community algorithms.
- Shared code under `src/common` deserves extra attention because one defect can
  affect many algorithms.
- Experimental, disabled, or hazardous build options should be clearly identified
  in reports. A bug that requires such an option may still matter, but the report
  must say so.

## Report Quality Requirements

A security report should include:

- Repository, commit, branch, and affected build options.
- Exact platform, compiler, sanitizer, OpenSSL, CMake, Ninja, Python, and
  Valgrind versions when relevant.
- A threat-model statement: attacker capability, trust boundary crossed, and why
  the issue is in scope.
- Whether the reproducer satisfies documented pointer and memory-region
  preconditions for the affected API.
- A minimal working PoC or failing regression test.
- Build and run commands that a maintainer can paste into a clean checkout.
- Expected vulnerable output and expected fixed output.
- The affected public API, algorithm family, parameter set, and representative
  source location.
- Impact without exaggeration. Distinguish memory safety, denial of service,
  signature acceptance, key reuse, canonicalization, and side-channel impact.
- Whether the issue appears to be in liboqs-owned code, generated wrapper code,
  or imported upstream code.
- Suggested regression tests and, if possible, a small patch.

Do not submit reports that have not been built and run unless the report is
explicitly labeled as an unverified hypothesis.

## Triage Checklist For Researchers

Before filing a security issue, answer these questions:

1. Which public or internal liboqs boundary is crossed?
2. What attacker controls the input bytes, explicit length, key state,
   serialized state, callback result, or build configuration?
3. Does the reproducer satisfy the documented pointer and memory-region
   preconditions for the affected API?
4. Is the affected platform in Tier 1, Tier 2, or Tier 3?
5. Is the affected algorithm enabled by default, standard-only, experimental, or
   hazardous?
6. Does a clean build reproduce the issue with the provided commands?
7. Does a sanitizer, Valgrind, constant-time tool, differential test, KAT failure,
   or verification result provide concrete evidence?
8. Is the issue already documented in `tests/constant_time`, `docs/algorithms`,
   open issues, or a known upstream advisory?
9. Is this a vulnerability, a hardening opportunity, a test gap, or a
   documentation issue?

If the answer to any of these questions is unknown, include that uncertainty in
the report rather than filling the gap with speculation.
