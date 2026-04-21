# SPDX-License-Identifier: MIT
"""Tests for the algorithm-support generators under scripts/."""

import os
import sys

LIBOQS_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, os.path.join(LIBOQS_ROOT, "scripts"))

import update_alg_support_table as uast  # noqa: E402


def test_format_upstream_source_with_commit():
    src = "https://github.com/pq-code-package/mlkem-native/commit/d2cae2be522a67bfae26100fdb520576f1b2ef90"
    out = uast.format_upstream_source(src)
    assert "pq-code-package/mlkem-native@d2cae2b" in out
    assert src in out


import textwrap
import tempfile

import pytest


def _write_yaml(dirpath, name, body):
    path = os.path.join(dirpath, name)
    with open(path, "w") as f:
        f.write(body)
    return path


def test_render_alg_support_tbl_includes_new_columns():
    pytest.importorskip("tabulate")
    with tempfile.TemporaryDirectory() as d:
        _write_yaml(d, "foo.yml", textwrap.dedent("""
            name: Foo
            type: kem
            standardization-status: Standardized by NIST
            spec-url: https://example.com/foo
            primary-upstream:
              source: https://github.com/example/foo/commit/abcdef0123456789
              spdx-license-identifier: MIT
            upstream-maintenance: Actively maintained
            upstream-statement-url: https://example.com/foo-statement
            oqs-support-tier: 1
            parameter-sets: []
        """).lstrip())
        tbl = uast.render_alg_support_tbl(d)
    assert "Upstream maintenance" in tbl
    assert "OQS tier" in tbl
    assert "Actively maintained" in tbl
    assert "https://example.com/foo-statement" in tbl
    assert "Tier 1" in tbl


import importlib


def _load_algorithms_md_module():
    return importlib.import_module("generate_algorithms_md")


def test_algorithms_md_has_vocabulary_glossary():
    with open(os.path.join(LIBOQS_ROOT, "ALGORITHMS.md")) as f:
        out = f.read()
    for term in [
        "Actively maintained",
        "Best effort",
        "No active maintenance",
        "TBD",
    ]:
        assert term in out


def test_algorithms_md_family_row():
    mod = _load_algorithms_md_module()
    algdata = {
        "name": "Foo",
        "standardization-status": "Standardized",
        "primary-upstream": {
            "source": "https://github.com/example/foo/commit/abcdef0123456789"
        },
        "upstream-maintenance": "Actively maintained",
        "upstream-statement-url": None,
        "oqs-support-tier": 1,
        "parameter-sets": [
            {
                "name": "Foo-512",
                "claimed-nist-level": 1,
                "implementations": [
                    {
                        "upstream": "primary-upstream",
                        "upstream-id": "ref",
                        "supported-platforms": "all",
                        "default": True,
                        "no-secret-dependent-branching-checked-by-valgrind": True,
                    },
                    {
                        "upstream": "primary-upstream",
                        "upstream-id": "avx2",
                        "supported-platforms": [
                            {"architecture": "x86_64", "required_flags": ["avx2"]}
                        ],
                    },
                ],
            },
            {
                "name": "Foo-1024",
                "claimed-nist-level": 5,
                "implementations": [
                    {
                        "upstream": "primary-upstream",
                        "upstream-id": "ref",
                        "supported-platforms": "all",
                        "default": True,
                        "no-secret-dependent-branching-checked-by-valgrind": True,
                    }
                ],
            },
        ],
    }
    row = mod.family_row(algdata)
    cols = dict(zip(mod.HEADER, row))
    assert cols["Algorithm family"] == "Foo"
    assert cols["NIST levels"] == "1, 5"
    assert cols["Constant-time"] == "✓"
    assert "portable" in cols["Optimization targets"]
    assert "x86_64 (avx2)" in cols["Optimization targets"]
    assert cols["OQS tier"] == "Tier 1 (Core)"
