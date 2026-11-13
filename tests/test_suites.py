# SPDX-License-Identifier: MIT
"""Checks that the test-suite definitions in tests/conftest.py stay consistent."""

import re
from pathlib import Path

from conftest import SUITES

TESTS_DIR = Path(__file__).resolve().parent


def test_build_targets_match_suites():
    """tests/CMakeLists.txt defines one run_tests_<suite> target per suite in conftest.py."""
    cmakelists = (TESTS_DIR / "CMakeLists.txt").read_text()
    match = re.search(r"set\(OQS_TEST_SUITES ([^)]*)\)", cmakelists)
    assert match, "OQS_TEST_SUITES not found in tests/CMakeLists.txt"
    assert match.group(1).split() == list(SUITES), (
        "OQS_TEST_SUITES in tests/CMakeLists.txt must list the suites in tests/conftest.py, in order"
    )
