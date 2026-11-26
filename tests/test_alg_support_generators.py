# SPDX-License-Identifier: MIT
"""Tests for the algorithm-support generators under scripts/."""

import os
import sys

import pytest

LIBOQS_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
sys.path.insert(0, os.path.join(LIBOQS_ROOT, "scripts"))

pytest.importorskip("yaml")

import update_alg_support_table as uast  # noqa: E402


def test_format_upstream_source_with_commit():
    src = "https://github.com/pq-code-package/mlkem-native/commit/d2cae2be522a67bfae26100fdb520576f1b2ef90"
    out = uast.format_upstream_source(src)
    assert "pq-code-package/mlkem-native@d2cae2b" in out
    assert src in out


import textwrap
import tempfile


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


def test_render_alg_support_tbl_links_to_datasheets():
    pytest.importorskip("tabulate")
    with tempfile.TemporaryDirectory() as root:
        sig_dir = os.path.join(root, "sig")
        os.mkdir(sig_dir)
        _write_yaml(sig_dir, "foo_dsa.yml", textwrap.dedent("""
            name: Foo-DSA
            type: signature
            standardization-status: Standardized by NIST
            primary-upstream:
              source: https://github.com/example/foo
              spdx-license-identifier: MIT
            parameter-sets: []
        """).lstrip())
        linked = uast.render_alg_support_tbl(sig_dir, anchor_alg_name=True)
        plain = uast.render_alg_support_tbl(sig_dir)
    assert "[Foo-DSA](docs/algorithms/sig/foo_dsa.md)" in linked
    assert "docs/algorithms" not in plain


def test_readme_datasheet_links_point_at_existing_files():
    import re

    with open(os.path.join(LIBOQS_ROOT, "README.md")) as f:
        readme = f.read()
    links = re.findall(r"\]\((docs/algorithms[^)]*)\)", readme)
    # the index link plus one link per algorithm family
    assert "docs/algorithms" in links
    assert len(links) > 1
    for link in links:
        assert os.path.exists(os.path.join(LIBOQS_ROOT, link)), link


import doxyfy_datasheet  # noqa: E402


def test_doxyfy_datasheet_prefixes_labels_and_links():
    src = "\n".join([
        "# Foo",
        "- **Primary Source**<a name=\"primary-source\"></a>:",
        "  - **avx2**:<a name=\"avx2\"></a>",
        "| [Primary Source](#primary-source) | [avx2](#avx2) |",
        "See the [Explanation of Terms](#explanation-of-terms) section.",
        "[elsewhere](#not-in-this-file)",
        "## Explanation of Terms",
    ])
    out = doxyfy_datasheet.doxyfy_datasheet(src, "kem", "foo").split("\n")
    assert out[0] == "# Foo {#algorithms-kem-foo}"
    assert out[1] == "- **Primary Source**: \\anchor algorithms-kem-foo-primary-source"
    assert out[2] == "  - **avx2**: \\anchor algorithms-kem-foo-avx2"
    assert out[3] == (
        "| [Primary Source](#algorithms-kem-foo-primary-source) "
        "| [avx2](#algorithms-kem-foo-avx2) |"
    )
    assert "(#algorithms-kem-foo-explanation-of-terms)" in out[4]
    assert out[5] == "[elsewhere](#not-in-this-file)"
    assert out[6] == "## Explanation of Terms {#algorithms-kem-foo-explanation-of-terms}"
    assert len(out) == len(src.split("\n"))


def test_doxyfy_datasheet_plain_title():
    out = doxyfy_datasheet.doxyfy_datasheet("# **Stateful Signatures**\n", "sig_stfl", "sig_stfl")
    assert out.split("\n")[0] == "# Stateful Signatures {#algorithms-sig_stfl-sig_stfl}"


def test_doxyfy_datasheet_keeps_real_datasheets_line_count():
    for kind in ("kem", "sig", "sig_stfl"):
        directory = os.path.join(LIBOQS_ROOT, "docs", "algorithms", kind)
        for filename in os.listdir(directory):
            if not filename.endswith(".md"):
                continue
            with open(os.path.join(directory, filename), encoding="utf-8") as f:
                text = f.read()
            out = doxyfy_datasheet.doxyfy_datasheet(text, kind, filename[:-3])
            assert out.count("\n") == text.count("\n"), filename
            assert "<a name=" not in out, filename


def test_algorithms_index_lists_every_datasheet():
    import doxygen_algorithms_index

    index = doxygen_algorithms_index.render_index(LIBOQS_ROOT)
    assert index.startswith("# Algorithm datasheets {#algorithms}")
    for kind in ("kem", "sig", "sig_stfl"):
        directory = os.path.join(LIBOQS_ROOT, "docs", "algorithms", kind)
        for filename in os.listdir(directory):
            if filename.endswith(".md"):
                assert f"\\subpage algorithms-{kind}-{filename[:-3]}" in index


def test_doxyfy_rewrites_datasheet_links():
    import subprocess

    with tempfile.TemporaryDirectory() as d:
        src = os.path.join(d, "in.md")
        dst = os.path.join(d, "out.md")
        with open(src, "w") as f:
            f.write(
                "See [docs/algorithms](docs/algorithms) and "
                "[ML-KEM](docs/algorithms/kem/ml_kem.md).\n"
            )
        subprocess.run(
            [sys.executable, os.path.join(LIBOQS_ROOT, "scripts", "doxyfy.py"), src, dst],
            check=True,
        )
        with open(dst) as f:
            out = f.read()
    assert "[docs/algorithms](@ref algorithms)" in out
    assert "[ML-KEM](@ref algorithms-kem-ml_kem)" in out
