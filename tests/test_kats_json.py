# SPDX-License-Identifier: MIT
"""Structural checks on the KAT digest files themselves.

`test_kat.py` and `test_kat_all.py` index `tests/KATs/{kem,sig}/kats.json`
directly, so an entry that is missing a digest surfaces as a `KeyError` raised
from inside an unrelated test rather than as a problem with the data. When a
new family is added and only one of the two digests is generated for it, that
reads as N confusing test failures rather than one clear one, and it is easy to
mistake for a build or dispatch problem.

These checks read the data on its own. They need no build, so they run
everywhere and fail fast, and they name the algorithms at fault.
"""

import json
import os
import re

import pytest

KATS_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "KATs")

# `single` is compared by test_kat.py, `all` by test_kat_all.py. Both are a
# sha256 of subprocess output, so both are 64 lowercase hex characters.
REQUIRED_DIGESTS = ("all", "single")
DIGEST_PATTERN = re.compile(r"\A[0-9a-f]{64}\Z")


def load_kats(kind):
    with open(os.path.join(KATS_DIR, kind, "kats.json")) as f:
        return json.load(f)


@pytest.mark.parametrize("kind", ["kem", "sig"])
def test_kats_file_is_not_empty(kind):
    assert load_kats(kind), f"tests/KATs/{kind}/kats.json has no entries"


@pytest.mark.parametrize("kind", ["kem", "sig"])
def test_every_algorithm_carries_every_digest(kind):
    kats = load_kats(kind)

    incomplete = {
        name: sorted(set(REQUIRED_DIGESTS) - set(entry))
        for name, entry in kats.items()
        if not set(REQUIRED_DIGESTS) <= set(entry)
    }

    assert not incomplete, (
        f"tests/KATs/{kind}/kats.json is missing digests: "
        + "; ".join(f"{name} has no {', '.join(fields)}"
                    for name, fields in sorted(incomplete.items()))
        + ". Generate the missing digests rather than skipping the algorithm: "
        f"test_kat_all.py reads ['all'] with no guard, so a missing one fails "
        f"as a KeyError inside that test."
    )


@pytest.mark.parametrize("kind", ["kem", "sig"])
def test_every_digest_is_a_sha256_hex_string(kind):
    kats = load_kats(kind)

    malformed = [
        f"{name}[{field}]={value!r}"
        for name, entry in sorted(kats.items())
        for field, value in sorted(entry.items())
        if not (isinstance(value, str) and DIGEST_PATTERN.match(value))
    ]

    assert not malformed, (
        f"tests/KATs/{kind}/kats.json has values that are not lowercase "
        f"sha256 hex digests: {', '.join(malformed)}"
    )


if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)
