# SPDX-License-Identifier: MIT
"""Test suites for liboqs.

Every test under tests/ belongs to exactly one suite, listed in SUITES
below. This table is the single source of truth for suite membership: CI
workflows and build targets select a suite by name instead of listing (or
ignoring) test files.

Run from the repository root:

    python3 -m pytest tests --suite correctness
    python3 -m pytest tests --suite basic,correctness
    python3 -m pytest tests --suite correctness --algs ML-KEM-768,ML-DSA-65

"""

from pathlib import Path

import pytest

import helpers

TESTS_DIR = Path(__file__).resolve().parent

# Each entry is either:
# - a test module (eg. "test_cmdline.py")
# - a single function, written "module::function" (eg. test_hash.py::test_aes)
SUITES = {
    # source-code and repository-consistency checks. No build required
    "basic": [
        "test_code_conventions.py",
        "test_cmake_option_coverage.py",
        "test_kats_json.py",
        "test_alg_support_generators.py",
    ],
    # build-system checks that configure CMake but run no liboqs code
    "build": [
        "test_cmake_options.py",
    ],
    # assert correct behaviour of the built library on this configuration
    "correctness": [
        "test_cmdline.py",
        "test_kat.py",
        "test_binary.py",
        "test_alg_info.py",
        "test_hash.py::test_aes",
        "test_hash.py::test_sha3",
    ],
    # lengthier functional tests and benchmarks for greater assurance
    "extensive": [
        "test_hash.py::test_hash_sha2_random",
        "test_speed.py",
        "test_mem.py",
    ],
    # exhaustive known-answer and external test-vector suites
    "vectors": [
        "test_kat_all.py",
        "test_acvp_vectors.py",
        "test_wycheproof_vectors.py",
    ],
    # Valgrind memory-leak and memory-error checks
    "leaks": [
        "test_leaks.py",
    ],
    # run portable (OQS_DIST_BUILD) binaries on a minimal CPU under QEMU
    "distbuild": [
        "test_distbuild.py",
    ],
}

# Parametrize argument names that carry an algorithm name, for --algs.
ALG_PARAMS = ("kem_name", "sig_name", "sig_stfl_name")

# Read by pytest during collection; pytest_configure fills it in for --suite.
collect_ignore = []

# =========================================================================== #
#                                   Helpers                                   #
# =========================================================================== #

def _index_suites(suites):
    """Invert SUITES into a test -> suite lookup. Reject tests listed in more than one suite."""
    index = {}
    for suite, tests in suites.items():
        for test in tests:
            if test in index:
                raise ValueError(
                    f"{test} is listed in both '{index[test]}' and '{suite}' in tests/conftest.py"
                )
            index[test] = suite
    return index


SUITE_OF = _index_suites(SUITES)


def _requested_suites(config):
    """Return the set of suites named by --suite"""
    suites = {s.strip() for s in config.getoption("--suite").split(",") if s.strip()}
    unknown = suites - set(SUITES)
    if unknown:
        raise pytest.UsageError(
            f"unknown suite(s): {', '.join(sorted(unknown))}; "
            f"choose from: {', '.join(SUITES)}"
        )
    return suites


def _requested_algs(config):
    """Return the set of algorithms named by --algs"""
    algs = {a.strip() for a in config.getoption("--algs").split(",") if a.strip()}
    known = set(
        helpers.available_kems_by_name()
        + helpers.available_sigs_by_name()
        + helpers.available_sig_stfls_by_name()
    )
    unknown = algs - known
    if unknown:
        raise pytest.UsageError(
            f"unknown algorithm(s) in --algs: {', '.join(sorted(unknown))}"
        )
    return algs


def _path(item):
    """Return the test's file as a Path. Item.path is pytest 7+; Ubuntu 22.04 ships pytest 6.2."""
    return Path(str(getattr(item, "path", None) or item.fspath))


def _suite_for(item):
    """Return the test's suite from SUITE_OF, preferring a module::function entry; None if unassigned."""
    module = _path(item).name
    return SUITE_OF.get(f"{module}::{item.originalname}") or SUITE_OF.get(module)


def _alg_for(item):
    """Return the algorithm name the test is parametrized with, or None if it takes none."""
    callspec = getattr(item, "callspec", None)
    if callspec is None:
        return None
    return next((callspec.params[p] for p in ALG_PARAMS if p in callspec.params), None)


# =========================================================================== #
#                                Pytest Hooks                                 #
# =========================================================================== #

# hook: runs during command line option parsing to add liboqs options
def pytest_addoption(parser):
    group = parser.getgroup("liboqs suites")
    group.addoption(
        "--suite",
        default="",
        metavar="NAME[,NAME...]",
        help="run only tests in these suites. Suites: " + ", ".join(SUITES),
    )
    group.addoption(
        "--algs",
        default="",
        metavar="ALG[,ALG...]",
        help="run per-algorithm tests only for these algorithm names; tests "
             "that take no algorithm always run.",
    )

# hook: runs during pytest configuration to validate suite and algorithm options
def pytest_configure(config):
    config.oqs_suites = _requested_suites(config)
    config.oqs_algs = _requested_algs(config)
    if config.oqs_suites:
        # Don't import test modules with no tests in the requested suites: some
        # read the build directory at import time, and suites like basic run
        # without a build. Modules missing from SUITES are still collected, so
        # they fail in pytest_runtest_setup.
        requested = {test.split("::")[0] for suite in config.oqs_suites for test in SUITES[suite]}
        assigned = {test.split("::")[0] for test in SUITE_OF}
        collect_ignore.extend(sorted(assigned - requested))


# hook: runs during test collection to modify the list of collected items
def pytest_collection_modifyitems(config, items):
    suites, algs = config.oqs_suites, config.oqs_algs

    keep, drop = [], []
    for item in items:
        if _path(item).parent != TESTS_DIR:
            # Tests outside tests/ (e.g. scripts/) are not part of any suite.
            (drop if suites else keep).append(item)
            continue

        suite = _suite_for(item)
        if suite is None:
            # Keep unassigned tests around so it fails in every run (see
            # pytest_runtest_setup) rather than silently dropping out
            item.oqs_unassigned = True
            keep.append(item)
            continue

        alg = _alg_for(item)
        if (suites and suite not in suites) or (algs and alg is not None and alg not in algs):
            drop.append(item)
        else:
            keep.append(item)

    if drop:
        config.hook.pytest_deselected(items=drop)
        items[:] = keep

# hook: runs during each test setup to ensure every test is part of a suite
def pytest_runtest_setup(item):
    if getattr(item, "oqs_unassigned", False):
        pytest.fail(
            f"{_path(item).name} is not assigned to a suite: add it to SUITES in tests/conftest.py",
            pytrace=False,
        )
