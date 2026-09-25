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
liboqs-only templates (`api.h.in`, `crypto_sign.c.in`, `config.h.in`) implement the
PQClean detached-signature API and OQS primitive routing.

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
| Memory helpers | `OQS_MEM_*` (`config.h`, `compat.h`) |
| CPU features | `OQS_CPU_has_extension` (`cpu.h`) |
| AES-128/256 ECB | `OQS_AES128/256_ECB_*` (`aes.c`) |
| AES-192 ECB | `OQS_AES192_ECB_*` (`aes.c`) |

## Known follow-ups

1. Add optimized implementations (`avx2`, `neon`) as additional entries in each
   scheme's `META.yml`.
2. Run liboqs constant-time tests and document results under
   `tests/constant_time/sig/`.
