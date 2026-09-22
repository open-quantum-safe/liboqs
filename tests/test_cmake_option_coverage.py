# SPDX-License-Identifier: MIT
"""Checks that the CMake option surface can actually reach what ships.

A family's `CMakeLists.txt` selects sources with
`if(OQS_ENABLE_SIG_<scheme>_<impl>)`. If no option of that name is ever
declared in `.CMake/alg_support.cmake`, the condition is false in every
configuration, so those sources are shipped in the tarball, reviewed, and never
compiled anywhere. Nothing else in the tree notices: the build succeeds, the
KATs pass on whichever implementation did get selected, and the result looks
like full coverage.

This reads the CMake files only. It does not configure or build, so it costs
nothing and runs on every platform.

An implementation may legitimately be withheld, which is what
`KNOWN_UNSELECTABLE` records. The check is deliberately exact in both
directions: an unselectable implementation that is not listed fails, and a
listed one that has become selectable, or whose sources have gone, also fails.
A one-way allowlist would silently rot.
"""

import os
import re

import pytest

LIBOQS_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
ALG_SUPPORT = os.path.join(LIBOQS_ROOT, ".CMake", "alg_support.cmake")

OPTION_DECLARATION = re.compile(
    r"(?:cmake_dependent_option|option)\s*\(\s*(OQS_ENABLE_(?:SIG|KEM)_[A-Za-z0-9_]+)"
)
OPTION_REFERENCE = re.compile(r"\b(OQS_ENABLE_(?:SIG|KEM)_[A-Za-z0-9_]+)\b")
CMAKEDEFINE = re.compile(
    r"^\s*#cmakedefine\s+(OQS_ENABLE_(?:SIG|KEM)_[A-Za-z0-9_]+)", re.M
)

# Implementations that ship but that no option can enable, each with the reason
# it is withheld. Anything here is a deliberate decision, not an oversight.
#
# QR-UOV `opt`: making `opt` selectable produced reproducible SIGBUS crashes in
# 8 of the 30 variants on ARM64 (3q7L10, 5q7L10, 5q31L10 and 5q127L10, in both
# the AES and SHAKE builds), so it is withheld pending that investigation.
# `ref` is the default and `avx2` is the optimised x86-64 path. The sources are
# kept so the upstream import stays whole. Note that the wrapper selects with
# `#if _opt / #elif _avx2`, so whenever `opt` does become selectable that
# ordering has to be fixed in the same change, or enabling `opt` will silently
# drop AVX2 from every build that has both.
KNOWN_UNSELECTABLE = {
    f"OQS_ENABLE_SIG_qruov_{scheme}_opt": "QR-UOV opt withheld pending the ARM64 SIGBUS investigation"
    for scheme in (
        "1q7L10aes", "1q7L10shake",
        "1q31L3aes", "1q31L3shake",
        "1q31L10aes", "1q31L10shake",
        "1q127L3aes", "1q127L3shake",
        "1q127L10aes", "1q127L10shake",
        "3q7L10aes", "3q7L10shake",
        "3q31L3aes", "3q31L3shake",
        "3q31L10aes", "3q31L10shake",
        "3q127L3aes", "3q127L3shake",
        "3q127L10aes", "3q127L10shake",
        "5q7L10aes", "5q7L10shake",
        "5q31L3aes", "5q31L3shake",
        "5q31L10aes", "5q31L10shake",
        "5q127L3aes", "5q127L3shake",
        "5q127L10aes", "5q127L10shake",
    )
}


def strip_cmake_comments(text):
    """Drop `#` comments so a commented-out condition is not read as live."""
    lines = []
    for line in text.splitlines():
        quoted = False
        end = len(line)
        for index, char in enumerate(line):
            if char == '"':
                quoted = not quoted
            elif char == "#" and not quoted:
                end = index
                break
        lines.append(line[:end])
    return "\n".join(lines)


def read_cmake(path):
    with open(path, encoding="utf-8", errors="replace") as f:
        return strip_cmake_comments(f.read())


def declared_options():
    return set(OPTION_DECLARATION.findall(read_cmake(ALG_SUPPORT)))


def referenced_options():
    """Every OQS_ENABLE_SIG/KEM option tested by a family's CMakeLists."""
    references = {}
    src = os.path.join(LIBOQS_ROOT, "src")
    for dirpath, _, filenames in os.walk(src):
        if "CMakeLists.txt" not in filenames:
            continue
        path = os.path.join(dirpath, "CMakeLists.txt")
        relative = os.path.relpath(path, LIBOQS_ROOT).replace(os.sep, "/")
        for symbol in OPTION_REFERENCE.findall(read_cmake(path)):
            references.setdefault(symbol, set()).add(relative)
    return references


def configured_defines():
    """Every OQS_ENABLE_SIG/KEM macro src/oqsconfig.h.cmake promises to define."""
    path = os.path.join(LIBOQS_ROOT, "src", "oqsconfig.h.cmake")
    with open(path, encoding="utf-8", errors="replace") as f:
        return set(CMAKEDEFINE.findall(f.read()))


def test_alg_support_declares_options():
    """Guard the guard: a parse that silently matched nothing would pass everything."""
    assert len(declared_options()) > 100


def test_source_cmakelists_reference_options():
    assert len(referenced_options()) > 100


def test_every_selected_implementation_can_be_enabled():
    declared = declared_options()
    references = referenced_options()

    unselectable = {
        symbol: sorted(files)
        for symbol, files in references.items()
        if symbol not in declared
    }

    undeclared = sorted(set(unselectable) - set(KNOWN_UNSELECTABLE))
    assert not undeclared, (
        "these options are tested by a CMakeLists but declared nowhere in "
        ".CMake/alg_support.cmake, so the sources they guard can never be "
        "compiled in any configuration:\n"
        + "\n".join(f"  {symbol}  ({', '.join(unselectable[symbol])})"
                    for symbol in undeclared)
        + "\n\nEither declare the option, or record it in KNOWN_UNSELECTABLE "
        "with the reason it is withheld."
    )

    stale = sorted(set(KNOWN_UNSELECTABLE) - set(unselectable))
    assert not stale, (
        "these options are recorded in KNOWN_UNSELECTABLE but are no longer "
        "unselectable, so the entry is out of date:\n"
        + "\n".join(f"  {symbol}  ({KNOWN_UNSELECTABLE[symbol]})" for symbol in stale)
        + "\n\nRemove them from KNOWN_UNSELECTABLE."
    )


def test_every_configured_define_can_be_enabled():
    """src/oqsconfig.h.cmake must not promise a macro no option can define.

    This is the same defect seen from a third surface. The generator emits the
    `#cmakedefine` for every implementation but the option only for those with
    a supported platform, so a withheld implementation leaves a macro that is
    silently never defined. Code that reads it compiles and does nothing.
    """
    declared = declared_options()

    undefinable = sorted(configured_defines() - declared - set(KNOWN_UNSELECTABLE))

    assert not undefinable, (
        "src/oqsconfig.h.cmake declares these macros but no CMake option can "
        "ever define them:\n"
        + "\n".join(f"  {symbol}" for symbol in undefinable)
    )


if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)
