# SPDX-License-Identifier: MIT
"""Checks that every build option a CMakeLists tests can actually be set.

A family's `CMakeLists.txt` selects sources with
`if(OQS_ENABLE_SIG_<scheme>_<impl>)`. If no option of that name is ever
declared in `.CMake/alg_support.cmake`, the condition is false in every
configuration, so those sources are shipped in the tarball, reviewed, and
never compiled anywhere. Nothing else in the tree notices: the build
succeeds, and the KATs pass on whichever implementation did get selected,
because the implementations of one scheme agree on their vectors.

Scope, so the name is not read as broader than the check: this compares the
options a CMakeLists *references* against the options `alg_support.cmake`
*declares*. Sources that ship under `src/` and are referenced by no
CMakeLists at all are a different problem and are not covered here. Libjade
uses `OQS_ENABLE_LIBJADE_*`, which is outside the pattern on both sides.

Whether an option can exist at all is decided by
`scripts/copy_from_upstream/.CMake/alg_support.cmake/add_enable_by_alg_conditional.fragment`,
which emits a per-implementation option only for an implementation that is
not the family default and that declares either a supported platform or
`memory_optimized`. So the set of implementations that cannot be enabled is
derivable from `docs/algorithms/*/*.yml` rather than listed here, and it
stays correct when parameter sets are added or renamed. What this module
does keep is a reason for each one, because shipping sources nothing can
build should be a recorded decision rather than an accident.

This reads metadata and CMake text only. It does not configure or build, so
it costs nothing and runs on every platform.
"""

import glob
import os
import re

import pytest

LIBOQS_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
ALG_SUPPORT = os.path.join(LIBOQS_ROOT, ".CMake", "alg_support.cmake")
OQSCONFIG = os.path.join(LIBOQS_ROOT, "src", "oqsconfig.h.cmake")

OPTION_DECLARATION = re.compile(
    r"(?:cmake_dependent_option|option)\s*\(\s*(OQS_ENABLE_(?:SIG|KEM)_[A-Za-z0-9_]+)"
)
OPTION_REFERENCE = re.compile(r"\b(OQS_ENABLE_(?:SIG|KEM)_[A-Za-z0-9_]+)\b")
CMAKEDEFINE = re.compile(
    r"^\s*#cmakedefine\s+(OQS_ENABLE_(?:SIG|KEM)_[A-Za-z0-9_]+)", re.M
)
# CMake bracket comments `#[[ ... ]]` and bracket arguments `[[ ... ]]`, with
# any number of `=` between the brackets.
BRACKET = re.compile(r"#?\[(=*)\[.*?\]\1\]", re.S)

# Why a shipped implementation is not selectable, keyed by (family,
# implementation). An implementation that the metadata says cannot be
# enabled, and that is not recorded here, fails: the point is that the
# decision is visible.
WITHHELD_REASONS = {
    ("qruov", "opt"): (
        "QR-UOV opt is withheld pending an ARM64 investigation: making it "
        "selectable produced reproducible SIGBUS crashes in 8 of the 30 "
        "variants during test_sig, reported in open-quantum-safe/liboqs#2582. "
        "ref is the default and avx2 is the optimised x86-64 path. Note that "
        "the generated wrapper selects with `#if _opt / #elif _avx2`, so "
        "whenever opt does become selectable that ordering has to change in "
        "the same commit, or enabling opt will silently drop AVX2."
    ),
}


def strip_cmake_comments(text):
    """Drop comments so commented-out code is not read as live, and vice versa.

    `src/sig_stfl/lms/CMakeLists.txt` has a commented-out guard on an option
    that does not exist, so without this the check would report it.

    Bracket forms are removed first, because handling only `#` to end of line
    gets both directions wrong: the body of a `#[[ ... ]]` block comment would
    be read as live code, and a `#` inside a `[[ ... ]]` bracket argument
    would truncate a line that is live. Newlines are preserved so that line
    numbering is unchanged.

    Remaining limitation, since this decides whether a real finding is
    reported: a quoted argument spanning several lines resets the quote state
    at each newline. Nothing in the files read here does that.
    """
    text = BRACKET.sub(lambda m: "\n" * m.group(0).count("\n"), text)

    lines = []
    for line in text.splitlines():
        quoted = False
        end = len(line)
        index = 0
        while index < len(line):
            char = line[index]
            if char == "\\" and quoted:
                index += 2
                continue
            if char == '"':
                quoted = not quoted
            elif char == "#" and not quoted:
                end = index
                break
            index += 1
        lines.append(line[:end])
    return "\n".join(lines)


def read_text(path):
    with open(path, encoding="utf-8", errors="replace") as f:
        return f.read()


def declared_options():
    return set(OPTION_DECLARATION.findall(strip_cmake_comments(read_text(ALG_SUPPORT))))


def referenced_options():
    """Every OQS_ENABLE_SIG/KEM option tested by a family's CMakeLists."""
    references = {}
    for dirpath, _, filenames in os.walk(os.path.join(LIBOQS_ROOT, "src")):
        if "CMakeLists.txt" not in filenames:
            continue
        path = os.path.join(dirpath, "CMakeLists.txt")
        relative = os.path.relpath(path, LIBOQS_ROOT).replace(os.sep, "/")
        for symbol in OPTION_REFERENCE.findall(strip_cmake_comments(read_text(path))):
            references.setdefault(symbol, set()).add(relative)
    return references


def configured_defines():
    """Every OQS_ENABLE_SIG/KEM macro src/oqsconfig.h.cmake promises to define."""
    return set(CMAKEDEFINE.findall(read_text(OQSCONFIG)))


def withheld_pairs():
    """(family, implementation) pairs no generated option can enable.

    Mirrors the filter in add_enable_by_alg_conditional.fragment.
    """
    yaml = pytest.importorskip("yaml")
    pairs = set()
    pattern = os.path.join(LIBOQS_ROOT, "docs", "algorithms", "*", "*.yml")
    for path in sorted(glob.glob(pattern)):
        family = os.path.splitext(os.path.basename(path))[0]
        data = yaml.safe_load(read_text(path)) or {}
        for parameter_set in data.get("parameter-sets") or []:
            for implementation in parameter_set.get("implementations") or []:
                if (
                    implementation.get("default")
                    or implementation.get("supported-platforms")
                    or implementation.get("memory_optimized")
                ):
                    continue
                pairs.add((family, implementation.get("upstream-id")))
    return pairs


def matches_pair(symbol, pairs):
    """Whether an option name belongs to one of the withheld (family, impl) pairs."""
    for prefix in ("OQS_ENABLE_SIG_", "OQS_ENABLE_KEM_"):
        if symbol.startswith(prefix):
            tail = symbol[len(prefix):]
            return any(
                tail.startswith(family + "_") and tail.endswith("_" + implementation)
                for family, implementation in pairs
            )
    return False


def test_a_line_comment_hides_a_declaration():
    text = '# option(OQS_ENABLE_SIG_ghost_1_neon "" ON)'
    assert not OPTION_DECLARATION.findall(strip_cmake_comments(text))


def test_a_bracket_comment_hides_a_declaration():
    """A `#[[ ]]` body must not be read as live, or the guard can be silenced.

    Wrapping a declaration in a bracket comment would otherwise make an
    unselectable implementation look selectable, and the staleness check
    would then invite someone to delete its recorded reason.
    """
    text = '#[[\noption(OQS_ENABLE_SIG_ghost_1_neon "" ON)\n]]'
    assert not OPTION_DECLARATION.findall(strip_cmake_comments(text))


def test_a_bracket_argument_does_not_hide_live_code():
    """The inverse: a `#` inside `[[ ]]` must not truncate a live line."""
    text = 'message([[note # see below]])\nif(OQS_ENABLE_SIG_ghost_1_neon)'
    assert "OQS_ENABLE_SIG_ghost_1_neon" in strip_cmake_comments(text)


def test_a_quoted_hash_does_not_hide_live_code():
    text = 'set(X "a # b")\nif(OQS_ENABLE_SIG_ghost_1_neon)'
    assert "OQS_ENABLE_SIG_ghost_1_neon" in strip_cmake_comments(text)


def test_comment_stripping_preserves_line_count():
    text = "a\n#[[\nb\n]]\nc"
    assert len(strip_cmake_comments(text).splitlines()) == len(text.splitlines())


def test_option_parsing_finds_known_options():
    """A regex regression that matched almost nothing would pass every check."""
    declared = declared_options()
    assert "OQS_ENABLE_SIG_mayo_1_avx2" in declared
    assert "OQS_ENABLE_KEM_ml_kem_768" in declared


def test_reference_parsing_finds_known_options():
    referenced = referenced_options()
    assert "OQS_ENABLE_SIG_mayo_1_avx2" in referenced
    assert "OQS_ENABLE_SIG_qruov_1q7L10aes" in referenced


def test_every_withheld_implementation_has_a_recorded_reason():
    undocumented = sorted(withheld_pairs() - set(WITHHELD_REASONS))
    assert not undocumented, (
        "these implementations ship but no generated option can enable them, "
        "and no reason is recorded:\n"
        + "\n".join(f"  {family}: {impl}" for family, impl in undocumented)
        + "\n\nGive the implementation a supported platform in "
        "docs/algorithms, or record why it is withheld in WITHHELD_REASONS."
    )

    stale = sorted(set(WITHHELD_REASONS) - withheld_pairs())
    assert not stale, (
        "these implementations are recorded as withheld but the metadata now "
        "allows an option for them, so the entry is out of date:\n"
        + "\n".join(f"  {family}: {impl}" for family, impl in stale)
    )


def test_every_referenced_option_can_be_enabled():
    declared = declared_options()
    references = referenced_options()
    pairs = withheld_pairs()

    undeclared = {
        symbol: sorted(files)
        for symbol, files in references.items()
        if symbol not in declared and not matches_pair(symbol, pairs)
    }

    assert not undeclared, (
        "these options are tested by a CMakeLists but declared nowhere in "
        ".CMake/alg_support.cmake, so the sources they guard can never be "
        "compiled in any configuration:\n"
        + "\n".join(f"  {symbol}  ({', '.join(undeclared[symbol])})"
                    for symbol in sorted(undeclared))
    )


def test_every_configured_define_can_be_enabled():
    """src/oqsconfig.h.cmake must not promise a macro no option can define.

    The same defect seen from a third surface: the generator emits a
    `#cmakedefine` for every implementation but an option only for some, so a
    withheld implementation leaves a macro that is silently never defined.
    """
    declared = declared_options()
    pairs = withheld_pairs()

    undefinable = sorted(
        symbol
        for symbol in configured_defines() - declared
        if not matches_pair(symbol, pairs)
    )

    assert not undefinable, (
        "src/oqsconfig.h.cmake declares these macros but no CMake option can "
        "ever define them:\n" + "\n".join(f"  {symbol}" for symbol in undefinable)
    )
