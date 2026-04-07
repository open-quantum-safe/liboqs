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

## Adding a New Upstream Algorithm

1. Add the upstream repo to the `upstreams` section in `copy_from_upstream.yml`
2. Add the family and its schemes to `kems` or `sigs`, referencing the upstream
3. Ensure the upstream provides a `META.yml` per scheme with source lists and metadata
4. If the algorithm has shared code across implementations, set up common dependencies
5. Run `python3 copy_from_upstream.py copy` and verify the generated output
