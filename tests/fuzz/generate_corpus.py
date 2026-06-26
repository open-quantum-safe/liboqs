#!/usr/bin/env python3
# SPDX-License-Identifier: MIT

"""Generate deterministic seed corpora for the generic public-input fuzzers."""

import argparse
import re
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
CORPUS_ROOT = ROOT / "tests" / "fuzz" / "corpus"

REGISTRIES = {
    "kem": (ROOT / "src" / "kem" / "kem.c", "OQS_KEM_alg_"),
    "sig": (ROOT / "src" / "sig" / "sig.c", "OQS_SIG_alg_"),
    "sig_stfl": (ROOT / "src" / "sig_stfl" / "sig_stfl.c", "OQS_SIG_STFL_alg_"),
}

TARGETS = {
    "fuzz_test_kem_encaps": ("kem", (0, 1)),
    "fuzz_test_kem_decaps": ("kem", (0,)),
    "fuzz_test_sig_verify": ("sig", (0, 1, 2)),
    "fuzz_test_sig_stfl_verify": ("sig_stfl", (0,)),
    "fuzz_test_sig_stfl_deserialize": ("sig_stfl", (0,)),
}

GENERATED_NAME_RE = re.compile(r"^alg-[0-9]{3}-mode-[0-9]{2}$")


def registry_tokens(source: Path, prefix: str) -> list[str]:
    text = source.read_text(encoding="utf-8")
    array_marker = "const char *a["
    array_start = text.find(array_marker)
    if array_start < 0:
        raise ValueError(f"could not find algorithm array in {source}")

    body_start = text.find("{", array_start)
    body_end = text.find("};", body_start)
    if body_start < 0 or body_end < 0:
        raise ValueError(f"could not parse algorithm array in {source}")

    tokens = re.findall(rf"\b{re.escape(prefix)}[A-Za-z0-9_]+\b", text[body_start:body_end])
    if not tokens:
        raise ValueError(f"algorithm array in {source} is empty")
    if len(tokens) != len(set(tokens)):
        raise ValueError(f"algorithm array in {source} contains duplicate identifiers")
    if len(tokens) > 0x10000:
        raise ValueError(f"algorithm array in {source} exceeds the two-byte seed index")
    return tokens


def expected_corpus() -> dict[Path, bytes]:
    registries = {
        name: registry_tokens(source, prefix)
        for name, (source, prefix) in REGISTRIES.items()
    }
    expected = {}
    for target, (registry_name, modes) in TARGETS.items():
        for index, _token in enumerate(registries[registry_name]):
            for mode in modes:
                path = CORPUS_ROOT / target / f"alg-{index:03d}-mode-{mode:02d}"
                expected[path] = bytes((1, index & 0xFF, (index >> 8) & 0xFF, mode, 0))
    return expected


def write_corpus(expected: dict[Path, bytes]) -> int:
    expected_paths = set(expected)
    for target in TARGETS:
        target_dir = CORPUS_ROOT / target
        target_dir.mkdir(parents=True, exist_ok=True)
        for path in target_dir.iterdir():
            if path.is_file() and GENERATED_NAME_RE.match(path.name) and path not in expected_paths:
                path.unlink()

    changed = 0
    for path, contents in expected.items():
        path.parent.mkdir(parents=True, exist_ok=True)
        if not path.exists() or path.read_bytes() != contents:
            path.write_bytes(contents)
            changed += 1
    print(f"wrote {len(expected)} corpus seeds ({changed} changed)")
    return 0


def check_corpus(expected: dict[Path, bytes]) -> int:
    errors = []
    expected_paths = set(expected)
    for path, contents in expected.items():
        if not path.exists():
            errors.append(f"missing {path.relative_to(ROOT)}")
        elif path.read_bytes() != contents:
            errors.append(f"wrong contents in {path.relative_to(ROOT)}")

    for target in TARGETS:
        target_dir = CORPUS_ROOT / target
        if not target_dir.exists():
            continue
        for path in target_dir.iterdir():
            if path.is_file() and GENERATED_NAME_RE.match(path.name) and path not in expected_paths:
                errors.append(f"stale generated seed {path.relative_to(ROOT)}")

    if errors:
        print("\n".join(errors), file=sys.stderr)
        return 1
    print(f"validated {len(expected)} corpus seeds")
    return 0


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument("--write", action="store_true", help="write expected seed files")
    mode.add_argument("--check", action="store_true", help="verify committed seed files")
    args = parser.parse_args()

    try:
        expected = expected_corpus()
    except ValueError as exc:
        print(str(exc), file=sys.stderr)
        return 1

    if args.write:
        return write_corpus(expected)
    return check_corpus(expected)


if __name__ == "__main__":
    raise SystemExit(main())
