# copy_from_upstream

This directory contains the tooling that imports upstream algorithm implementations into liboqs, generates build files, and keeps documentation in sync.

## Quick Start

```bash
cd scripts/copy_from_upstream
export LIBOQS_DIR=/path/to/liboqs
python3 copy_from_upstream.py copy        # fetch upstreams, copy code, generate build/docs
python3 copy_from_upstream.py verify      # diff integrated code against upstream
python3 copy_from_upstream.py libjade     # import libjade (Jasmin) implementations
```

**Flags:**
- `-k` / `--keep_data` — keep cloned upstream repos in `repos/` after completion
- `-d` / `--delete` — delete untracked files from implementation directories
- `-v N` / `--verbosity N` — set debug output level

## Configuration

All upstreams, KEM families, and signature families are declared in [`copy_from_upstream.yml`](copy_from_upstream.yml). The file has three main sections:

- **`upstreams`** — external Git repositories (URL, pinned commit, path templates for META.yml and source files, optional patches)
- **`kems`** — KEM algorithm families, each listing schemes (parameter sets), implementations, and the upstream they come from
- **`sigs`** — same structure for digital signatures

Each upstream's `META.yml` provides per-scheme metadata (e.g., security level, lengths, KAT values, supported platforms, source file lists).

### Upstream Entry

An upstream declares the Git repo and how to locate META.yml and source files:

```yaml
upstreams:
  - name: myupstream
    git_url: https://github.com/example/myalg.git
    git_branch: main
    git_commit: abc123def456...
    sig_meta_path: 'META/{pretty_name_full}_META.yml'
    sig_scheme_path: '.'
    common_meta_path: 'META/COMMONS.yml'       # optional: shared code metadata
    patches: [myupstream-fix.patch]            # optional: local patches
```

### Algorithm Family Entry

A family groups related schemes and maps them to an upstream:

```yaml
sigs:
  - name: myalg
    default_implementation: ref
    upstream_location: myupstream
    schemes:
      - scheme: "128"
        pqclean_scheme: myalg-128  # upstream's name for this scheme; (historic artifact: named after PQClean, now generic)
        pretty_name_full: MyAlg-128
      - scheme: "256"
        pqclean_scheme: myalg-256
        pretty_name_full: MyAlg-256
```

### Upstream META.yml

Each scheme in the upstream repo provides a `META.yml` describing its properties and implementations:

```yaml
name: MyAlg-128
type: signature              # or "kem"
claimed-nist-level: 1
length-public-key: 1024
length-secret-key: 32
length-signature: 512
nistkat-sha256: abcdef0123456789...
principal-submitters:
  - Alice Submitter
implementations:
  - name: ref
    version: v1.0
    folder_name: .
    compile_opts: -DVARIANT=128 -DBUILD_TYPE_REF
    sources: src/core.c src/sign.c include/myalg.h
    common_dep: myalg_common          # optional: references a common dependency
  - name: avx2
    version: v1.0
    compile_opts: -DVARIANT=128 -DBUILD_TYPE_AVX2
    sources: src/core.c src/sign.c include/myalg.h
    common_dep: myalg_common myalg_common_avx2  # multiple deps, space-separated
    supported_platforms:
      - architecture: x86_64
        required_flags: [avx2]
```

### Common Dependencies

Shared code used by multiple implementations is declared in a commons YAML file (referenced by `common_meta_path` in the upstream entry):

```yaml
commons:
  - name: myalg_common
    folder_name: src/common
    sources: field.c field.h hash.c hash.h
    include_only: true               # sources compiled per-variant with variant flags
  - name: myalg_common_avx2
    folder_name: src/common_avx2
    sources: avx2_utils.c avx2_utils.h
    include_only: false              # compiled as shared OBJECT library (default)
    supported_platforms:
      - architecture: x86_64
        required_flags: [avx2]
```

When `include_only: false` (default), common sources are compiled once as a CMake OBJECT library shared by all implementations. When `include_only: true`, sources are added to each implementation's target individually, allowing variant-specific compile flags to apply.

## What `copy` Does

1. **Fetch** — clone/checkout each upstream at its pinned commit and apply patches from `patches/`
2. **Copy** — copy implementation source files into `src/kem/<family>/` and `src/sig/<family>/`
3. **Common deps** — copy shared source files used across implementations
4. **Generate CMake** — render Jinja2 templates in `src/` to produce per-family `CMakeLists.txt`
5. **Generate headers** — produce algorithm dispatch headers and wrapper `.c` files
6. **Update docs** — generate algorithm YAML datasheets, README support tables, and CBOM
7. **Update KATs** — refresh `tests/KATs/` JSON files
8. **Cleanup** — remove `repos/` (unless `-k`)

## Key Concepts

| Term | Meaning |
|------|---------|
| **Upstream** | An external Git repo providing algorithm code (e.g., mlkem-native) |
| **Family** | A group of related algorithms (e.g., ML-KEM) |
| **Scheme** | A specific parameter set (e.g., ML-KEM-512) |
| **Implementation** | A code variant for a scheme (e.g., `clean`, `avx2`) |
| **Common dep** | Shared source files used by multiple implementations within a family |

## Generated vs. Hand-Written Files

`copy` produces two kinds of output. Both are overwritten on every run and checked by `verify`, so **never hand-edit them**:

- **Whole-file** — the per-family OQS wrapper and build files, rendered from the templates in [`src/kem/family/`](src/kem/family) / [`src/sig/family/`](src/sig/family):
  - `src/<kem|sig>/<family>/<kem|sig>_<family>_<scheme>.c` — the `OQS_KEM`/`OQS_SIG` wrapper (constructor + keypair/encaps/decaps or sign/verify) with runtime CPU dispatch across implementations
  - `src/<kem|sig>/<family>/<kem|sig>_<family>.h` — public length macros and prototypes
  - `src/<kem|sig>/<family>/CMakeLists.txt`
- **Fragments** — snippets injected between marker comments in otherwise hand-maintained files (`src/kem/kem.c`, `src/kem/kem.h`, `src/sig/sig.c`, `src/sig/sig.h`, `.CMake/alg_support.cmake`, `src/oqsconfig.h.cmake`, …):
  ```c
  ///// OQS_COPY_FROM_UPSTREAM_FRAGMENT_ALG_IDENTIFIER_START
  ...generated...
  ///// OQS_COPY_FROM_UPSTREAM_FRAGMENT_ALG_IDENTIFIER_END
  ```
  Edit only outside the markers.

The hand-written parts live **in the upstream repo** (its `META.yml` and integration glue — see below) plus, in this directory, the two `copy_from_upstream.yml` entries and any files in `patches/`. `verify` re-copies from upstream and fails if integrated code was hand-modified.

## The OQS API

The generated wrapper adapts the algorithm to liboqs's public API: it fills an `OQS_KEM`/`OQS_SIG` struct and implements the operations by calling the upstream symbols named in its `META.yml` (`signature_keypair`, `signature_enc`/`signature_dec` for KEMs; `signature_keypair`, `signature_signature`, `signature_verify` for signatures). The wrapper is not hand-written, but the upstream code must present callable functions and follow these contracts:

| Concern | Where |
|---|---|
| KEM API — `OQS_KEM` struct, keypair/encaps/decaps, length fields | [`../../src/kem/kem.h`](../../src/kem/kem.h) |
| Signature API — `OQS_SIG` struct, keypair/sign/verify (+ optional `*_with_ctx_str`) | [`../../src/sig/sig.h`](../../src/sig/sig.h) |
| Stateful signatures (XMSS/LMS) — separate `OQS_SIG_STFL` API | [`../../src/sig_stfl/sig_stfl.h`](../../src/sig_stfl/sig_stfl.h) |
| Return codes — `OQS_STATUS`, `OQS_SUCCESS` (0), `OQS_ERROR` (-1) | [`../../src/common/common.h`](../../src/common/common.h) |
| Memory — `OQS_MEM_malloc`/`_calloc`/`_aligned_alloc`, `OQS_MEM_secure_free`/`_insecure_free`/`_cleanse`/`_secure_bcmp` | [`../../src/common/common.h`](../../src/common/common.h) |
| Randomness — `OQS_randombytes` | [`../../src/common/rand/rand.h`](../../src/common/rand/rand.h) |
| Symmetric primitives — SHA-3/SHAKE, SHA-2, AES (for glue to reuse) | [`../../src/common/sha3/`](../../src/common/sha3), [`sha2/`](../../src/common/sha2), [`aes/`](../../src/common/aes) |
| Umbrella include (`<oqs/oqs.h>`) | [`../../src/oqs.h`](../../src/oqs.h) |

**Return-code convention.** Operation functions return `OQS_SUCCESS` (0) on success and `OQS_ERROR` (-1) on failure; the wrapper maps upstream's `int` return (0 = success) onto these. Externally exported functions carry the `OQS_API` visibility macro (defined in `common.h`).

## Integration Glue Code

Upstream code usually expects its own RNG, hash, and AES, and exports generic symbol names. liboqs supplies its own primitives and compiles every implementation variant into a single library, so a thin **glue layer** adapts the upstream onto liboqs. This glue lives **in the upstream repo** — often in a dedicated integration folder (e.g. ML-KEM's [`integration/liboqs/`](../../src/kem/ml_kem/mlkem-native_ml-kem-512_ref/integration/liboqs)) — and `copy` copies it verbatim into `src/`. The glue files are listed in the implementation's `sources` and wired in through its `compile_opts`.

Typical things the glue wires up:

| Upstream needs | Route to | Example |
|---|---|---|
| `randombytes` | `OQS_randombytes` | ML-KEM `config_c.h` defines `mlk_randombytes` → `OQS_randombytes` |
| Keccak / SHA-3 / SHAKE / SHA-2 | `OQS_SHA3_*` / `OQS_SHA2_*` | ML-KEM `fips202_glue.h` remaps `mlk_shake128*`/`mlk_sha3_256` onto OQS's FIPS202 |
| AES | `OQS_AES*` | MQOM `enc_liboqs.h` implements its block cipher via `<oqs/aes.h>` (`-DMQOM2_FOR_LIBOQS`) |
| Parameter-set selection | compile-time defines | set in `compile_opts` |

Reusing liboqs's primitives (rather than bundling the upstream's) is expected, it shares one constant-time implementation across the library. If a common primitive is missing (or an optimization thereof), a contribution to the liboqs common code is welcome.

**Symbol namespacing.** All implementations of a scheme (`ref`, `avx2`, `aarch64`, …) link into the same binary, so each must export **unique** symbol names or they collide. A good pattern is to use a common prefix for the implementation (e.g. mlkem-native's `PQCP_MLKEM_NATIVE_MLKEM512_C_*` vs. `..._X86_64_*`). The exported names must match the `signature_*` entrypoint fields in `META.yml` — those are exactly what the generated wrapper `extern`-declares and calls.

## Constant-Time Testing

`test_constant_time` runs each scheme under Valgrind with secret data marked uninitialized, flagging any branch or memory access that depends on a secret. A fully constant-time implementation needs no configuration. Audited-safe or known non-constant-time behaviour is documented with a Valgrind suppression file:

- Save the suppression file under `tests/constant_time/<kem|sig>/passes/` (audited, not a security threat) or `.../issues/` (a genuine or unverified concern — use this when unsure). Both suppress the error identically; the folder is only a label for auditors.
- Register it in the sibling `passes.json` / `issues.json`, which map each scheme's method name to its suppression files:
  ```json
  { "<some_algorithm>": ["mqom"], ... }
  ```

The header of [`tests/test_constant_time.py`](../../tests/test_constant_time.py) explains how to turn Valgrind's output into a suppression file.

## Adding a New Upstream Algorithm

1. Add the upstream repo to the `upstreams` section in `copy_from_upstream.yml`
2. Add the family and its schemes to `kems` or `sigs`, referencing the upstream
3. In the upstream repo, provide a per-scheme `META.yml` (lengths, NIST level, `nistkat-sha256`, and `implementations` with `sources`, `compile_opts`, and the `signature_*` entrypoint symbol names — see [The OQS API](#the-oqs-api))
4. In the upstream repo, add the glue that routes RNG/hash/AES onto liboqs and namespaces symbols (see [Integration Glue Code](#integration-glue-code)); list the glue files in `sources`
5. If the algorithm has shared code across implementations, set up common dependencies
6. Fill the algorithm datasheet `docs/algorithms/<kem|sig>/<family>.yml` — including the maintenance fields `upstream-maintenance`, `upstream-statement-url`, and `oqs-support-tier` (see [`ALGORITHMS.md`](../../ALGORITHMS.md)); `copy` renders these into the `README.md`/`ALGORITHMS.md` support tables. Submitters are invited to add a code-owner entry for their algorithm in [`.github/CODEOWNERS`](../../.github/CODEOWNERS) (e.g. `/src/<kem|sig>/<family> @your-handle`) to stay in the loop; if the algorithm is to be listed at Tier 1, such an entry is required
7. Run `python3 copy_from_upstream.py copy`, then `python3 copy_from_upstream.py verify`
8. Run `test_constant_time`; document any non-constant-time behaviour as described in [Constant-Time Testing](#constant-time-testing)
