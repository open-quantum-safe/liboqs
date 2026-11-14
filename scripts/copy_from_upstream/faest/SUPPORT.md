# FAEST liboqs integration

liboqs owns FAEST integration artifacts. [faest-ref](https://github.com/faest-sign/faest-ref)
provides the cryptographic implementation, parameter templates (`.in` files), and
`#ifdef OQS` routing for liboqs primitives (including AES-128/192/256 ECB).

## Layout

| Location | Contents |
|----------|----------|
| `faest-ref/` | Core sources, `faest_param*.in`, `faest_aes.c.in`, OQS hooks |
| `scripts/copy_from_upstream/faest/` | `generate.py`, liboqs templates, META YAML, generated outputs (including VOLE tables) |
| `src/sig/faest/` | Copied integration tree (do not edit by hand) |

Parameter numeric values are read from `faest-ref/meson.build` at generation time.
liboqs-only templates (`api.h.in`, `crypto_sign.c.in`, `config.h.in`) plus
generated `faest_namespace.h` implement the PQClean detached-signature API, OQS
primitive routing, and `oqs_faest_` symbol prefixes.

## Maintenance (liboqs)

After changing faest-ref parameter sets, upstream `.in` templates, or liboqs templates:

```sh
LIBOQS_DIR=/path/to/liboqs python3 scripts/copy_from_upstream/faest/generate.py \
  --faest-ref /path/to/faest-ref
```

If `copy_from_upstream` has already fetched faest-ref:

```sh
python3 scripts/copy_from_upstream/faest/generate.py \
  --faest-ref scripts/copy_from_upstream/repos/faest-ref
```

Verify committed artifacts match the pinned upstream commit:

```sh
python3 scripts/copy_from_upstream/faest/generate.py --check \
  --faest-ref scripts/copy_from_upstream/repos/faest-ref
```

Then run `copy_from_upstream.py copy` and liboqs tests.

## liboqs primitive routing

When built for liboqs (`-DOQS`), FAEST routes cryptographic primitives to liboqs:

| Primitive | Routing |
|-----------|---------|
| Random bytes | `OQS_randombytes` (`randomness.c`) |
| SHAKE | `OQS_SHA3_shake*` (`hash_shake.h`) |
| Memory helpers | `OQS_MEM_*` via `faest_malloc` / `faest_calloc` / `faest_free` (`compat.h`) |
| CPU features | `OQS_CPU_has_extension` (`cpu.h`) |
| AES-128/256 ECB | `OQS_AES128/256_ECB_*` (`aes.c`) |
| AES-192 ECB | `OQS_AES192_ECB_*` (`aes.c`) |

Bare faest-ref globals are renamed with `#define` in `generated/faest_namespace.h`
(included from `config.h`) to `oqs_faest_<symbol>`. Upstream can later replace
this with `-DFAEST_NAMESPACE=oqs_faest_`.

## Known follow-ups

1. faest-ref 3.0.3 ships compile-time SSE2/AVX2+PCLMUL/NEON field files
   (`fields_sse2.c`, `fields_avx2.c`, `fields_neon.c`). This integration keeps
   the portable `FAEST_FIELDS_USE_U64` ref implementation; extra META `avx2` /
   `neon` implementations are a follow-up.
2. Run liboqs constant-time tests and document results under
   `tests/constant_time/sig/`.
