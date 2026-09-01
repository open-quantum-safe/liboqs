# SPDX-License-Identifier: MIT
"""Tests that CMake configuration options are honoured.

These tests only run the CMake configuration step (no compilation) against a
throwaway build directory and inspect the generated `compile_commands.json`.
A minimal algorithm set is selected to keep the configuration cheap.
"""

import json
import os
import platform
import shutil
import subprocess

import pytest

LIBOQS_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))

# `-fPIC` is a GCC/Clang spelling; MSVC has no equivalent flag and always
# generates position independent code.
pytestmark = pytest.mark.skipif(
    platform.system() == "Windows" or shutil.which("cmake") is None,
    reason="requires cmake and a GCC/Clang-style compiler driver",
)

# Configure a single KEM and no test targets: the position independent code
# setting is global, so one algorithm exercises it just as well as all of them.
COMMON_ARGS = [
    "-DOQS_BUILD_ONLY_LIB=ON",
    "-DOQS_MINIMAL_BUILD=KEM_ml_kem_512",
]


def configure(build_dir, *args):
    """Run `cmake` and return (stdout+stderr, compile_commands entries)."""
    result = subprocess.run(
        ["cmake", "-S", LIBOQS_ROOT, "-B", str(build_dir), *COMMON_ARGS, *args],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        universal_newlines=True,
    )
    assert result.returncode == 0, result.stdout
    with open(os.path.join(str(build_dir), "compile_commands.json")) as f:
        entries = json.load(f)
    assert entries, "no compile commands were generated"
    return result.stdout, entries


def pic_entries(entries):
    return [e for e in entries if "-fPIC" in e["command"]]


def test_position_independent_code_on_by_default(tmp_path):
    """Without an explicit setting, liboqs is built as position independent code."""
    _, entries = configure(tmp_path / "build", "-DBUILD_SHARED_LIBS=OFF")
    assert len(pic_entries(entries)) == len(entries)


def test_position_independent_code_can_be_disabled(tmp_path):
    """A static build honours CMAKE_POSITION_INDEPENDENT_CODE=OFF."""
    _, entries = configure(
        tmp_path / "build",
        "-DBUILD_SHARED_LIBS=OFF",
        "-DCMAKE_POSITION_INDEPENDENT_CODE=OFF",
    )
    assert pic_entries(entries) == []


def test_position_independent_code_can_be_disabled_in_toolchain_file(tmp_path):
    """A toolchain file setting CMAKE_POSITION_INDEPENDENT_CODE=OFF is honoured."""
    toolchain = tmp_path / "toolchain.cmake"
    toolchain.write_text("set(CMAKE_POSITION_INDEPENDENT_CODE OFF)\n")
    _, entries = configure(
        tmp_path / "build",
        "-DBUILD_SHARED_LIBS=OFF",
        "-DCMAKE_TOOLCHAIN_FILE=" + str(toolchain),
    )
    assert pic_entries(entries) == []


def test_position_independent_code_kept_for_shared_builds(tmp_path):
    """Disabling it for a shared build is refused, with a warning.

    liboqs is assembled from OBJECT libraries, which do not inherit the implicit
    position independent code setting of the shared `oqs` target, so honouring
    OFF here would produce a shared library from non-PIC objects.
    """
    output, entries = configure(
        tmp_path / "build",
        "-DBUILD_SHARED_LIBS=ON",
        "-DCMAKE_POSITION_INDEPENDENT_CODE=OFF",
    )
    assert len(pic_entries(entries)) == len(entries)
    # CMake hard-wraps warning text, so compare against whitespace-collapsed output.
    assert "not supported together with BUILD_SHARED_LIBS=ON" in " ".join(output.split())


if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)
