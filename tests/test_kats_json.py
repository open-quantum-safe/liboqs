# SPDX-License-Identifier: MIT
"""Structural checks on the KAT digest files themselves.

`test_kat.py` and `test_kat_all.py` index `tests/KATs/{kem,sig}/kats.json`
directly, so an entry that is missing a digest surfaces as a `KeyError`
raised from inside an unrelated test rather than as a problem with the data.
When a new family is added and only one of the two digests is generated for
it, that reads as N confusing test failures rather than one clear one, and
it is easy to mistake for a build or dispatch problem.

Algorithms that have no KATs at all are simply absent from these files, and
that stays the way to express it: SLH-DSA uses ACVP vectors and the `-extmu`
variants have no KATs, and neither appears here. `SINGLE_DIGEST_ONLY` is for
the different case of an algorithm that is present but legitimately has no
`--all` digest. It is empty today, and the check is exact in both
directions, so an entry that stops being needed fails rather than lingering.

These checks read the data on its own. They need no build, so they run
everywhere and fail fast, and they name the algorithms at fault.
"""

import json
import os
import re

import pytest

LIBOQS_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
KATS_DIR = os.path.join(LIBOQS_ROOT, "tests", "KATs")
SRC_DIR = os.path.join(LIBOQS_ROOT, "src")

# `single` is compared by test_kat.py, `all` by test_kat_all.py. Both are a
# sha256 over subprocess output, so both are 64 lowercase hex characters.
REQUIRED_DIGESTS = ("all", "single")
DIGEST_PATTERN = re.compile(r"\A[0-9a-f]{64}\Z")

# Algorithms present in kats.json that legitimately carry only `single`,
# keyed by kind, each with the reason.
SINGLE_DIGEST_ONLY = {}


def load_kats(kind):
    with open(os.path.join(KATS_DIR, kind, "kats.json"), encoding="utf-8") as f:
        return json.load(f)


@pytest.mark.parametrize("kind", ["kem", "sig"])
def test_kats_file_is_not_empty(kind):
    assert load_kats(kind), f"tests/KATs/{kind}/kats.json has no entries"


@pytest.mark.parametrize("kind", ["kem", "sig"])
def test_every_algorithm_carries_every_digest(kind):
    kats = load_kats(kind)
    exempt = SINGLE_DIGEST_ONLY.get(kind, {})

    incomplete = {
        name: sorted(set(REQUIRED_DIGESTS) - set(entry))
        for name, entry in kats.items()
        if not set(REQUIRED_DIGESTS) <= set(entry) and name not in exempt
    }

    assert not incomplete, (
        f"tests/KATs/{kind}/kats.json is missing digests: "
        + "; ".join(f"{name} has no {', '.join(fields)}"
                    for name, fields in sorted(incomplete.items()))
        + ". test_kat_all.py reads ['all'] with no guard, so a missing one "
        "fails as a KeyError inside that test. Generate the digest, or, if "
        "the algorithm genuinely has none, record it in SINGLE_DIGEST_ONLY."
    )

    stale = sorted(name for name in exempt if name in kats
                   and set(REQUIRED_DIGESTS) <= set(kats[name]))
    assert not stale, (
        f"these {kind} algorithms are recorded in SINGLE_DIGEST_ONLY but now "
        f"carry every digest: {', '.join(stale)}"
    )


def declared_algorithms(kind):
    """Algorithm names from the public header, which is the universe of names.

    Mirrors `helpers.available_{kems,sigs}_by_name()`.
    """
    header = os.path.join(SRC_DIR, kind, f"{kind}.h")
    macro = f"#define OQS_{kind.upper()}_alg_"
    names = []
    with open(header, encoding="utf-8", errors="replace") as f:
        for line in f:
            if line.startswith(macro):
                names.append(line.split(" ")[2].strip()[1:-1])
    return names


def has_kats(name):
    """Whether an algorithm is expected to appear in kats.json at all.

    The exclusions match the skips in test_kat_all.py: SLH-DSA is covered by
    ACVP vectors instead, and the extmu variants have no KATs. Kept in step
    with that file rather than inferred from the data.
    """
    return "SLH_DSA" not in name and "-extmu" not in name


@pytest.mark.parametrize("kind", ["kem", "sig"])
def test_every_algorithm_with_kats_has_an_entry(kind):
    """A missing entry is the failure the other checks cannot see.

    Iterating the file only validates what is in it, so deleting an algorithm
    outright would pass every other check here and then reappear as the
    KeyError this module exists to pre-empt.
    """
    kats = load_kats(kind)
    expected = [name for name in declared_algorithms(kind) if has_kats(name)]

    missing = [name for name in expected if name not in kats]
    assert not missing, (
        f"these {kind} algorithms are declared in src/{kind}/{kind}.h but have "
        f"no entry in tests/KATs/{kind}/kats.json: {', '.join(sorted(missing))}"
    )

    unknown = [name for name in kats if name not in set(declared_algorithms(kind))]
    assert not unknown, (
        f"tests/KATs/{kind}/kats.json has entries for algorithms that "
        f"src/{kind}/{kind}.h does not declare: {', '.join(sorted(unknown))}"
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
