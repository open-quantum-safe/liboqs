#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Generate liboqs integration artifacts for FAEST."""

from __future__ import annotations

import argparse
import re
import shutil
import subprocess
import tempfile
from pathlib import Path

COPY_FROM_UPSTREAM = Path(__file__).resolve().parents[1]
INTEGRATION = Path(__file__).resolve().parent
TEMPLATES = INTEGRATION / "templates"
GENERATED = INTEGRATION / "generated"
DEFAULT_FAEST_REF = COPY_FROM_UPSTREAM / "repos" / "faest-ref"
LIBOQS_PREFIX = "liboqs:scripts/copy_from_upstream/faest"

_MEMORY_CHECK_FUNCS = ("malloc", "calloc", "free", "realloc", "strdup")


def annotate_memory_check_ignores(text: str) -> str:
    """Append IGNORE memory-check to lines that call libc allocators."""
    out_lines: list[str] = []
    in_multiline_comment = False
    for line in text.splitlines():
        stripped = line.strip()
        if "/*" in line and not in_multiline_comment:
            in_multiline_comment = True
        if in_multiline_comment:
            out_lines.append(line)
            if "*/" in line:
                in_multiline_comment = False
            continue
        if stripped.startswith("//") or "IGNORE memory-check" in line:
            out_lines.append(line)
            continue
        if any(
            re.search(rf"\b{func}\(", line) and not re.search(rf"\b_{func}\(", line)
            for func in _MEMORY_CHECK_FUNCS
        ):
            out_lines.append(line.rstrip() + " // IGNORE memory-check")
        else:
            out_lines.append(line)
    if text.endswith("\n"):
        return "\n".join(out_lines) + "\n"
    return "\n".join(out_lines)


# liboqs-only names; numeric parameters are read from faest-ref/meson.build.
SCHEME_METADATA = {
    "128s": {"pretty_name": "FAEST-128S", "pqclean_scheme": "faest-128s", "nist_level": 1},
    "128f": {"pretty_name": "FAEST-128F", "pqclean_scheme": "faest-128f", "nist_level": 1},
    "em_128s": {"pretty_name": "FAEST-EM-128S", "pqclean_scheme": "faest-em-128s", "nist_level": 1},
    "em_128f": {"pretty_name": "FAEST-EM-128F", "pqclean_scheme": "faest-em-128f", "nist_level": 1},
    "192s": {"pretty_name": "FAEST-192S", "pqclean_scheme": "faest-192s", "nist_level": 3},
    "192f": {"pretty_name": "FAEST-192F", "pqclean_scheme": "faest-192f", "nist_level": 3},
    "em_192s": {"pretty_name": "FAEST-EM-192S", "pqclean_scheme": "faest-em-192s", "nist_level": 3},
    "em_192f": {"pretty_name": "FAEST-EM-192F", "pqclean_scheme": "faest-em-192f", "nist_level": 3},
    "256s": {"pretty_name": "FAEST-256S", "pqclean_scheme": "faest-256s", "nist_level": 5},
    "256f": {"pretty_name": "FAEST-256F", "pqclean_scheme": "faest-256f", "nist_level": 5},
    "em_256s": {"pretty_name": "FAEST-EM-256S", "pqclean_scheme": "faest-em-256s", "nist_level": 5},
    "em_256f": {"pretty_name": "FAEST-EM-256F", "pqclean_scheme": "faest-em-256f", "nist_level": 5},
}

_INT_PARAM_KEYS = (
    "lambda",
    "nst",
    "ske",
    "r",
    "beta",
    "ell",
    "lke",
    "lenc",
    "tau",
    "w_grind",
    "t_open",
    "sig_size",
    "pk_size",
    "sk_size",
    "owf_input_size",
    "owf_output_size",
    "n_mult",
    "nist_level",
)


def parse_meson_parameter_sets(meson_path: Path) -> list[dict[str, object]]:
    """Parse parameter-set definitions from faest-ref/meson.build."""
    current: dict[str, object] | None = None
    parameter_sets: list[dict[str, object]] = []
    set_re = re.compile(r"^\s*param_\w+\.set\('([^']+)',\s*(.+)\)\s*$")

    for line in meson_path.read_text(encoding="utf-8").splitlines():
        if re.match(r"^param_\w+ = configuration_data\(\)", line):
            current = {}
            continue
        if current is not None and (match := set_re.match(line)):
            key = match.group(1).lower()
            raw = match.group(2).strip()
            if raw.startswith("'") and raw.endswith("'"):
                current[key] = raw[1:-1]
            else:
                current[key] = int(raw)
            continue
        if current is not None and line.startswith("parameter_sets +="):
            parameter_sets.append(current)
            current = None

    if not parameter_sets:
        raise ValueError(f"No parameter sets found in {meson_path}")
    return parameter_sets


def load_parameter_sets(faest_ref: Path) -> list[dict]:
    """Merge faest-ref meson parameters with liboqs scheme metadata."""
    meson_sets = parse_meson_parameter_sets(faest_ref / "meson.build")
    parameter_sets: list[dict] = []
    for meson_set in meson_sets:
        param_l = str(meson_set["param_l"])
        if param_l not in SCHEME_METADATA:
            raise KeyError(f"No liboqs scheme metadata for param_l={param_l!r}")
        ps = {**meson_set, **SCHEME_METADATA[param_l]}
        for key in _INT_PARAM_KEYS:
            if key in ps:
                ps[key] = int(ps[key])
        parameter_sets.append(ps)
    return parameter_sets



SUBMITTERS = [
    "Carsten Baum",
    "Waard Beullens",
    "Lennart Braun",
    "Cyprien Delpech de Saint Guilhem",
    "Michael Klooß",
    "Christian Majenz",
    "Shibam Mukherjee",
    "Emmanuela Orsini",
    "Sebastian Ramacher",
    "Christian Rechberger",
    "Lawrence Roy",
    "Peter Scholl",
]

# SHA-256 of liboqs `kat_sig <scheme>` output (count = 0 vector).
NISTKAT_SHA256 = {
    "FAEST-128S": "ac36ff496db5b30a8c4df1768100c0e804c4b627c8df85be27ebd3a14001ca13",
    "FAEST-128F": "a8dc23f6d66282f529c1cf52288566cc0af8650910aa0b00a3c7acc9be380bc9",
    "FAEST-EM-128S": "ee087b31592cbd1eabc34882fbe22d3c5d270b92ada1e4aab5be5588dc9cbd2e",
    "FAEST-EM-128F": "585e5adc1a1edb610eeec10148e246db86758d0f77fc27137def3a2ab7238a2f",
    "FAEST-192S": "cdff1a6fc7d81eb03522b968a9af6e0db6295e00410c95ea6330c2bf1fa97148",
    "FAEST-192F": "0d8d585f17687dfad35159b02ebf9d7ed96983bcf4630fe1062f5a0690a3f632",
    "FAEST-EM-192S": "7855c8018fb3a60994722be77b44b0c14902303c649f2b80afa5c21911d0f676",
    "FAEST-EM-192F": "19d56fcb425264ee609c1da98622baa26bd349ab2ac6635a9c1a5338b691480f",
    "FAEST-256S": "5573247ebd897e38930942910772d58b7fbfe969793a8326180f173131492295",
    "FAEST-256F": "77db00b2d70791ed8f710bb226722357c5e155dc0ce596e9356aaf8d80f46f90",
    "FAEST-EM-256S": "3aaf2e891cade204aef3e92e3fc39dd2ee8e6eeaf4cf37ed8ec1888ae6903150",
    "FAEST-EM-256F": "c27139ced48f1bfecc0ced01e3ee03377623f562c86e98fcc3b47dafa7872799",
}

TABLE_PRESETS = (
    "128s",
    "128f",
    "192s",
    "192f",
    "em_192s",
    "em_192f",
    "256s",
    "256f",
)

COMMON_HEADERS = [
    "aes.h",
    "aesni.h",
    "bavc.h",
    "compat.h",
    "cpu.h",
    "endian_compat.h",
    "faest.h",
    "faest_aes.h",
    "faest_defines.h",
    "faest_impl.h",
    "fields.h",
    "fields_u64.h",
    "hash_shake.h",
    "instances.h",
    "macros.h",
    "owf.h",
    "random_oracle.h",
    "randomness.h",
    "universal_hashing.h",
    "utils.h",
    "vole.h",
]

COMMON_SOURCES = [
    "aes.c",
    "bavc.c",
    "compat_mempcpy.c",
    "faest_impl.c",
    "fields.c",
    "fields_u64.c",
    "instances.c",
    "owf.c",
    "random_oracle.c",
    "randomness.c",
    "universal_hashing.c",
    "utils.c",
    "vole.c",
    f"{LIBOQS_PREFIX}/generated/config.h",
    f"{LIBOQS_PREFIX}/generated/parameters.h",
    f"{LIBOQS_PREFIX}/generated/faest_aes_128.c",
    f"{LIBOQS_PREFIX}/generated/faest_aes_192.c",
    f"{LIBOQS_PREFIX}/generated/faest_aes_256.c",
    f"{LIBOQS_PREFIX}/generated/faest_namespace.h",
] + [f"{LIBOQS_PREFIX}/generated/tables_{preset}.h" for preset in TABLE_PRESETS] + COMMON_HEADERS

# Portable ref: faest-ref 3.0.3 can select SSE2/AVX2/NEON/vector at compile
# time. Pin U64 here so the single liboqs ref target stays portable (no
# -Wpsabi from GCC vector_size) without extra META implementations.
FAEST_COMPILE_OPTS = "-DOQS -DHAVE_CONFIG_H -DFAEST_STATIC -DFAEST_FIELDS=FAEST_FIELDS_USE_U64"


# Bare faest-ref globals seen by `nm -g` on liboqs.a. Renamed to oqs_faest_*
# via generated/faest_namespace.h so they match liboqs coding conventions.
# Keep in sync with any new exported faest-ref symbols.
FAEST_NAMESPACE_SYMBOLS = [
    "aes128_init_round_keys", "aes_128_prover", "aes_128_verifier", "aes192_init_round_keys",
    "aes_192_prover", "aes_192_verifier", "aes256_init_round_keys", "aes_256_prover",
    "aes_256_verifier", "aes_extend_witness_7", "alloc_pointer_array", "bavc_clear",
    "bavc_commit", "bavc_open", "bavc_reconstruct", "bf128_byte_combine",
    "bf128_byte_combine_bits", "bf128_byte_combine_sq", "bf128_mul", "bf128_mul_64_inplace",
    "bf128_mul_bit", "bf128_mul_bit_8", "bf128_mul_inplace", "bf128_sq_bit", "bf128_sq_bit_inplace",
    "bf128_square", "bf128_sum_poly_bits", "bf192_byte_combine", "bf192_byte_combine_bits",
    "bf192_byte_combine_sq", "bf192_mul", "bf192_mul_64_inplace", "bf192_mul_bit",
    "bf192_mul_bit_8", "bf192_mul_inplace", "bf192_sq_bit", "bf192_sq_bit_inplace", "bf192_square",
    "bf192_sum_poly_bits", "bf256_byte_combine", "bf256_byte_combine_bits", "bf256_byte_combine_sq",
    "bf256_mul", "bf256_mul_64_inplace", "bf256_mul_bit", "bf256_mul_bit_8", "bf256_mul_inplace",
    "bf256_sq_bit", "bf256_sq_bit_inplace", "bf256_square", "bf256_sum_poly_bits",
    "bf2_matrix_mul_tbl", "bf2_poly_mul", "bf2_poly_reduce", "bf384_mul_128_inplace",
    "bf576_mul_192_inplace", "bf768_mul_256_inplace", "bf8_inv", "bf8_mul",
    "bf8_mulX", "bf8_square", "decode_all_chall_3", "expand_key", "extract_bit_slice",
    "faest_128f_clear_private_key", "faest_128f_clear_unpacked_private_key", "faest_128f_keygen", "faest_128f_sign",
    "faest_128f_sign_with_randomness", "faest_128f_unpacked_sign", "faest_128f_unpacked_sign_with_randomness", "faest_128f_unpack_private_key",
    "faest_128f_validate_keypair", "faest_128f_verify", "faest_128s_clear_private_key", "faest_128s_clear_unpacked_private_key",
    "faest_128s_keygen", "faest_128s_sign", "faest_128s_sign_with_randomness", "faest_128s_unpacked_sign",
    "faest_128s_unpacked_sign_with_randomness", "faest_128s_unpack_private_key", "faest_128s_validate_keypair", "faest_128s_verify",
    "faest_192f_clear_private_key", "faest_192f_clear_unpacked_private_key", "faest_192f_keygen", "faest_192f_sign",
    "faest_192f_sign_with_randomness", "faest_192f_unpacked_sign", "faest_192f_unpacked_sign_with_randomness", "faest_192f_unpack_private_key",
    "faest_192f_validate_keypair", "faest_192f_verify", "faest_192s_clear_private_key", "faest_192s_clear_unpacked_private_key",
    "faest_192s_keygen", "faest_192s_sign", "faest_192s_sign_with_randomness", "faest_192s_unpacked_sign",
    "faest_192s_unpacked_sign_with_randomness", "faest_192s_unpack_private_key", "faest_192s_validate_keypair", "faest_192s_verify",
    "faest_256f_clear_private_key", "faest_256f_clear_unpacked_private_key", "faest_256f_keygen", "faest_256f_sign",
    "faest_256f_sign_with_randomness", "faest_256f_unpacked_sign", "faest_256f_unpacked_sign_with_randomness", "faest_256f_unpack_private_key",
    "faest_256f_validate_keypair", "faest_256f_verify", "faest_256s_clear_private_key", "faest_256s_clear_unpacked_private_key",
    "faest_256s_keygen", "faest_256s_sign", "faest_256s_sign_with_randomness", "faest_256s_unpacked_sign",
    "faest_256s_unpacked_sign_with_randomness", "faest_256s_unpack_private_key", "faest_256s_validate_keypair", "faest_256s_verify",
    "faest_em_128f_clear_private_key", "faest_em_128f_clear_unpacked_private_key", "faest_em_128f_keygen", "faest_em_128f_sign",
    "faest_em_128f_sign_with_randomness", "faest_em_128f_unpacked_sign", "faest_em_128f_unpacked_sign_with_randomness", "faest_em_128f_unpack_private_key",
    "faest_em_128f_validate_keypair", "faest_em_128f_verify", "faest_em_128s_clear_private_key", "faest_em_128s_clear_unpacked_private_key",
    "faest_em_128s_keygen", "faest_em_128s_sign", "faest_em_128s_sign_with_randomness", "faest_em_128s_unpacked_sign",
    "faest_em_128s_unpacked_sign_with_randomness", "faest_em_128s_unpack_private_key", "faest_em_128s_validate_keypair", "faest_em_128s_verify",
    "faest_em_192f_clear_private_key", "faest_em_192f_clear_unpacked_private_key", "faest_em_192f_keygen", "faest_em_192f_sign",
    "faest_em_192f_sign_with_randomness", "faest_em_192f_unpacked_sign", "faest_em_192f_unpacked_sign_with_randomness", "faest_em_192f_unpack_private_key",
    "faest_em_192f_validate_keypair", "faest_em_192f_verify", "faest_em_192s_clear_private_key", "faest_em_192s_clear_unpacked_private_key",
    "faest_em_192s_keygen", "faest_em_192s_sign", "faest_em_192s_sign_with_randomness", "faest_em_192s_unpacked_sign",
    "faest_em_192s_unpacked_sign_with_randomness", "faest_em_192s_unpack_private_key", "faest_em_192s_validate_keypair", "faest_em_192s_verify",
    "faest_em_256f_clear_private_key", "faest_em_256f_clear_unpacked_private_key", "faest_em_256f_keygen", "faest_em_256f_sign",
    "faest_em_256f_sign_with_randomness", "faest_em_256f_unpacked_sign", "faest_em_256f_unpacked_sign_with_randomness", "faest_em_256f_unpack_private_key",
    "faest_em_256f_validate_keypair", "faest_em_256f_verify", "faest_em_256s_clear_private_key", "faest_em_256s_clear_unpacked_private_key",
    "faest_em_256s_keygen", "faest_em_256s_sign", "faest_em_256s_sign_with_randomness", "faest_em_256s_unpacked_sign",
    "faest_em_256s_unpacked_sign_with_randomness", "faest_em_256s_unpack_private_key", "faest_em_256s_validate_keypair", "faest_em_256s_verify",
    "faest_get_paramset", "faest_mempcpy", "faest_sign", "faest_verify", "free_pointer_array",
    "generic_aes_ecb_encrypt", "generic_aes_ecb_free", "generic_aes_ecb_new", "H0_final",
    "H0_init", "H0_update", "H1_final", "H1_init",
    "H1_update", "H2_0_final", "H2_1_final", "H2_2_final",
    "H2_3_final", "H2_copy", "H2_init", "H2_update",
    "H2_update_u32_le", "H3_final", "H3_init", "H3_update",
    "H4", "H5", "leaf_hash_128", "leaf_hash_192",
    "leaf_hash_256", "masked_xor_u8_array", "owf_128", "owf_192",
    "owf_256", "owf_em_128", "owf_em_192", "owf_em_256",
    "prg", "prg_2_lambda", "prg_4_lambda", "rand_bytes",
    "rijndael192_encrypt_block", "rijndael192_init_round_keys", "rijndael256_encrypt_block", "rijndael256_init_round_keys",
    "transpose_matrix", "vole_commit", "vole_hash", "vole_hash_128",
    "vole_hash_192", "vole_hash_256", "vole_reconstruct", "xor_u8_array",
    "xor_u8_array_inplace", "zk_hash_128_7_finalize", "zk_hash_128_7_init", "zk_hash_128_7_raise_and_update",
    "zk_hash_128_7_update", "zk_hash_128_finalize", "zk_hash_128_init", "zk_hash_128_update",
    "zk_hash_192_7_finalize", "zk_hash_192_7_init", "zk_hash_192_7_raise_and_update", "zk_hash_192_7_update",
    "zk_hash_192_finalize", "zk_hash_192_init", "zk_hash_192_update", "zk_hash_256_7_finalize",
    "zk_hash_256_7_init", "zk_hash_256_7_raise_and_update", "zk_hash_256_7_update", "zk_hash_256_finalize",
    "zk_hash_256_init", "zk_hash_256_update",
]


def render_template(template_path: Path, mapping: dict[str, str]) -> str:
    content = template_path.read_text(encoding="utf-8")
    for key, value in mapping.items():
        content = content.replace(f"@{key}@", value)
    return content


def calc_k(ps: dict) -> int:
    return ((ps["lambda"] - ps["w_grind"]) // ps["tau"]) + 1


def calc_tau1(ps: dict) -> int:
    return (ps["lambda"] - ps["w_grind"]) % ps["tau"]


def calc_tau0(ps: dict) -> int:
    return ps["tau"] - calc_tau1(ps)


def calc_l(ps: dict) -> int:
    k = calc_k(ps)
    tau1 = calc_tau1(ps)
    tau0 = calc_tau0(ps)
    return tau1 * (1 << k) + tau0 * (1 << (k - 1))


def generate_parameters_h(parameter_sets: list[dict]) -> str:
    lines = [
        "/* SPDX-License-Identifier: MIT */",
        "/* Generated for liboqs integration. */",
        "#ifndef FAEST_PARAMETERS_H",
        "#define FAEST_PARAMETERS_H",
        "",
    ]
    for ps in parameter_sets:
        prefix = f"FAEST_{ps['param']}"
        lines.extend(
            [
                f"#define {prefix}_PARAM \"{ps['param']}\"",
                f"#define {prefix}_PARAM_L \"{ps['param_l']}\"",
                f"#define {prefix}_LAMBDA {ps['lambda']}",
                f"#define {prefix}_Nst {ps['nst']}",
                f"#define {prefix}_Ske {ps['ske']}",
                f"#define {prefix}_R {ps['r']}",
                f"#define {prefix}_BETA {ps['beta']}",
                f"#define {prefix}_ELL {ps['ell']}",
                f"#define {prefix}_Lke {ps['lke']}",
                f"#define {prefix}_Lenc {ps['lenc']}",
                f"#define {prefix}_TAU {ps['tau']}",
                f"#define {prefix}_W_GRIND {ps['w_grind']}",
                f"#define {prefix}_T_OPEN {ps['t_open']}",
                f"#define {prefix}_SIG_SIZE {ps['sig_size']}",
                f"#define {prefix}_PK_SIZE {ps['pk_size']}",
                f"#define {prefix}_SK_SIZE {ps['sk_size']}",
                f"#define {prefix}_OWF_INPUT_SIZE {ps['owf_input_size']}",
                f"#define {prefix}_OWF_OUTPUT_SIZE {ps['owf_output_size']}",
                f"#define {prefix}_N_MULT {ps['n_mult']}",
                "",
            ]
        )
    lines.extend(
        [
            "#define FAEST_128_LAMBDA 128",
            "#define FAEST_192_LAMBDA 192",
            "#define FAEST_256_LAMBDA 256",
            "",
            "#endif",
            "",
        ]
    )
    return "\n".join(lines)


def generate_config_h() -> str:
    return (TEMPLATES / "config.h.in").read_text(encoding="utf-8")


def generate_faest_namespace_h() -> str:
    """Prefix faest-ref globals with oqs_faest_ (baentsch / liboqs convention)."""
    lines = [
        "/* SPDX-License-Identifier: MIT */",
        "/* Generated for liboqs integration. Do not edit by hand. */",
        "#ifndef FAEST_NAMESPACE_H",
        "#define FAEST_NAMESPACE_H",
        "",
        "/* Same pattern as SDitH APPLY_NAMESPACE / MQOM_NAMESPACE: rename bare",
        " * faest-ref symbols so `nm -g liboqs.a` sees an oqs_ prefix. Upstream",
        " * faest-ref can replace this with -DFAEST_NAMESPACE=oqs_faest_ later. */",
        "",
    ]
    width = max(len(sym) for sym in FAEST_NAMESPACE_SYMBOLS)
    for sym in FAEST_NAMESPACE_SYMBOLS:
        lines.append(f"#define {sym:<{width}} oqs_faest_{sym}")
    lines.extend(["", "#endif", ""])
    return "\n".join(lines)


def cxx_compiler() -> str:
    for name in ("c++", "clang++", "g++"):
        path = shutil.which(name)
        if path:
            return path
    raise RuntimeError("A C++ compiler is required to generate FAEST VOLE tables")


def generate_vole_tables(faest_ref: Path, parameters_h: str) -> dict[Path, str]:
    """Build faest-ref's vole_mult_tables tool and emit C headers."""
    with tempfile.TemporaryDirectory(prefix="faest-tables-") as tmp:
        tmpdir = Path(tmp)
        (tmpdir / "tables").mkdir()
        (tmpdir / "parameters.h").write_text(parameters_h, encoding="utf-8")
        shutil.copy(faest_ref / "macros.h", tmpdir / "macros.h")
        shutil.copy(faest_ref / "faest_defines.h", tmpdir / "faest_defines.h")
        shutil.copy(
            faest_ref / "tables" / "vole_mult_tables.cpp",
            tmpdir / "tables" / "vole_mult_tables.cpp",
        )
        exe = tmpdir / "vole_mult_tables"
        subprocess.run(
            [
                cxx_compiler(),
                "-std=c++17",
                "-O2",
                "-o",
                str(exe),
                str(tmpdir / "tables" / "vole_mult_tables.cpp"),
            ],
            check=True,
        )
        outputs: dict[Path, str] = {}
        for preset in TABLE_PRESETS:
            dest = tmpdir / f"tables_{preset}.h"
            subprocess.run(
                [str(exe), "--preset", preset, "--emit-c", str(dest)],
                check=True,
            )
            outputs[GENERATED / f"tables_{preset}.h"] = (
                "/* SPDX-License-Identifier: MIT */\n" + dest.read_text(encoding="utf-8")
            )
        return outputs


def generate_faest_aes(faest_ref: Path, sss: str) -> str:
    return annotate_memory_check_ignores(
        render_template(faest_ref / "faest_aes.c.in", {"SSS": sss})
    )


def namespace(ps: dict) -> str:
    return f"PQCLEAN_FAEST_{ps['param']}_REF"


def generate_meta_yml(ps: dict) -> str:
    ns = namespace(ps)
    submitters = "\n".join(f"  - {name}" for name in SUBMITTERS)
    return f"""name: {ps['pretty_name']}
type: signature
principal-submitters:
{submitters}
crypto-assumption: Syndrome decoding in the random code model and AES.
website: https://faest.info/
spec-version: 3.0
claimed-nist-level: {ps['nist_level']}
claimed-security: EUF-CMA
length-public-key: {ps['pk_size']}
length-secret-key: {ps['sk_size']}
length-signature: {ps['sig_size']}
nistkat-sha256: {NISTKAT_SHA256[ps['pretty_name']]}
implementations:
  - name: ref
    version: 3.0.3
    folder_name: {LIBOQS_PREFIX}/generated/{ps['pqclean_scheme']}
    signature_keypair: {ns}_crypto_sign_keypair
    signature_signature: {ns}_crypto_sign_signature
    signature_verify: {ns}_crypto_sign_verify
    compile_opts: {FAEST_COMPILE_OPTS}
    sources: api.h crypto_sign.c faest_{ps['param_l']}.c faest_{ps['param_l']}.h
    common_dep: faest_common faest_common_headers
    no-secret-dependent-branching-claimed: true
    no-secret-dependent-branching-checked-by-valgrind: false
    large-stack-usage: true
    supported-platforms: all
"""


def generate_meta_common_yml() -> str:
    sources = " ".join(COMMON_SOURCES)
    # Compile the parameter-independent core once. Inlining it into every
    # variant (include_only: true) defines the same non-static symbols 12 times
    # and breaks shared-library links. A headers-only companion dep supplies
    # the include path for per-variant glue, matching SDitH.
    header_sources = " ".join(
        [
            f"{LIBOQS_PREFIX}/generated/config.h",
            f"{LIBOQS_PREFIX}/generated/faest_namespace.h",
            f"{LIBOQS_PREFIX}/generated/parameters.h",
        ]
        + [f"{LIBOQS_PREFIX}/generated/tables_{preset}.h" for preset in TABLE_PRESETS]
        + COMMON_HEADERS
    )
    return f"""commons:
  - name: faest_common
    folder_name: .
    sources: {sources}
    compile_opts: {FAEST_COMPILE_OPTS}
    include_only: false
  - name: faest_common_headers
    folder_name: .
    sources: {header_sources}
    include_only: true
"""


def generate_support_md() -> str:
    return """# FAEST liboqs integration

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
LIBOQS_DIR=/path/to/liboqs python3 scripts/copy_from_upstream/faest/generate.py \\
  --faest-ref /path/to/faest-ref
```

If `copy_from_upstream` has already fetched faest-ref:

```sh
python3 scripts/copy_from_upstream/faest/generate.py \\
  --faest-ref scripts/copy_from_upstream/repos/faest-ref
```

Verify committed artifacts match the pinned upstream commit:

```sh
python3 scripts/copy_from_upstream/faest/generate.py --check \\
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
"""


def build_outputs(faest_ref: Path, parameter_sets: list[dict]) -> dict[Path, str]:
    """Return expected integration file paths and contents."""
    outputs: dict[Path, str] = {}
    outputs[GENERATED / "config.h"] = generate_config_h()
    outputs[GENERATED / "faest_namespace.h"] = generate_faest_namespace_h()
    parameters_h = generate_parameters_h(parameter_sets)
    outputs[GENERATED / "parameters.h"] = parameters_h
    outputs.update(generate_vole_tables(faest_ref, parameters_h))

    for sss in ("128", "192", "256"):
        outputs[GENERATED / f"faest_aes_{sss}.c"] = generate_faest_aes(faest_ref, sss)

    for ps in parameter_sets:
        scheme_dir = GENERATED / ps["pqclean_scheme"]
        mapping = {
            "PARAM": ps["param"],
            "PARAM_L": ps["param_l"],
            "PK_SIZE": str(ps["pk_size"]),
            "SK_SIZE": str(ps["sk_size"]),
            "SIG_SIZE": str(ps["sig_size"]),
            "LAMBDA": str(ps["lambda"]),
            "Nst": str(ps["nst"]),
            "Ske": str(ps["ske"]),
            "R": str(ps["r"]),
            "BETA": str(ps["beta"]),
            "ELL": str(ps["ell"]),
            "Lke": str(ps["lke"]),
            "Lenc": str(ps["lenc"]),
            "TAU": str(ps["tau"]),
            "W_GRIND": str(ps["w_grind"]),
            "T_OPEN": str(ps["t_open"]),
            "OWF_INPUT_SIZE": str(ps["owf_input_size"]),
            "OWF_OUTPUT_SIZE": str(ps["owf_output_size"]),
            "N_MULT": str(ps["n_mult"]),
        }
        outputs[scheme_dir / f"faest_{ps['param_l']}.h"] = render_template(
            faest_ref / "faest_param.h.in", mapping
        )
        outputs[scheme_dir / f"faest_{ps['param_l']}.c"] = render_template(
            faest_ref / "faest_param.c.in", mapping
        )
        outputs[scheme_dir / "api.h"] = render_template(TEMPLATES / "api.h.in", mapping)
        outputs[scheme_dir / "crypto_sign.c"] = render_template(
            TEMPLATES / "crypto_sign.c.in", mapping
        )
        outputs[INTEGRATION / f"{ps['pretty_name']}_META.yml"] = generate_meta_yml(ps)

    outputs[INTEGRATION / "META_Common.yml"] = generate_meta_common_yml()
    outputs[INTEGRATION / "SUPPORT.md"] = generate_support_md()
    return outputs


def write_outputs(outputs: dict[Path, str]) -> None:
    for path, content in outputs.items():
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(content, encoding="utf-8")


def find_stale_outputs(outputs: dict[Path, str]) -> list[Path]:
    stale: list[Path] = []
    for path, content in outputs.items():
        if not path.is_file() or path.read_text(encoding="utf-8") != content:
            stale.append(path)
    return stale


def check_stale(faest_ref: Path) -> list[Path]:
    """Return integration files that differ from freshly generated content."""
    parameter_sets = load_parameter_sets(faest_ref)
    return find_stale_outputs(build_outputs(faest_ref, parameter_sets))


def generate_all(faest_ref: Path) -> None:
    parameter_sets = load_parameter_sets(faest_ref)
    write_outputs(build_outputs(faest_ref, parameter_sets))


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--faest-ref",
        type=Path,
        default=DEFAULT_FAEST_REF,
        help="Path to faest-ref checkout (default: copy_from_upstream/repos/faest-ref)",
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="Exit with status 1 if generated files are out of date.",
    )
    args = parser.parse_args()

    faest_ref = args.faest_ref.resolve()
    if not faest_ref.is_dir():
        raise SystemExit(f"faest-ref directory not found: {faest_ref}")

    if args.check:
        stale = check_stale(faest_ref)
        if stale:
            raise SystemExit(
                "FAEST integration artifacts are out of date. "
                f"Regenerate with generate.py and recommit. First stale file: {stale[0]}"
            )
        print("liboqs FAEST integration artifacts are up to date.")
        return

    generate_all(faest_ref)
    print(f"Generated liboqs integration artifacts under {INTEGRATION}")


if __name__ == "__main__":
    main()
