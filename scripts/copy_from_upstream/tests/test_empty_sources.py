# SPDX-License-Identifier: MIT
from pathlib import Path

import pytest

Environment = pytest.importorskip("jinja2").Environment


TEMPLATES = (
    "sig",
    "kem",
)

ROOT = Path(__file__).resolve().parents[1] / "src"


def render(family, sources, include_sources=None, has_impl_files=None):
    template_path = ROOT / family / "family" / "CMakeLists.txt"

    template = Environment().from_string(
        template_path.read_text(encoding="utf-8")
    )

    implementations = [
        {
            "name": "ref",
            "sources": ["reference.c"],
            "upstream": {"name": "upstream"},
            "compile_opts": "",
            "required_flags": [],
        },
        {
            "name": "avx2",
            "sources": sources,
            "upstream": {"name": "upstream"},
            "compile_opts": "",
            "required_flags": [],
            "common_dep": (
                ["shared"] if include_sources is not None else []
            ),
        },
    ]

    if has_impl_files is not None:
        implementations[1]["has_impl_files"] = has_impl_files

    scheme = {
        "scheme": "test",
        "scheme_c": "test",
        "pqclean_scheme": "test",
        "default_implementation": "ref",
        "metadata": {"implementations": implementations},
    }

    context = {
        "family": family,
        "schemes": [scheme],
        "upstream_location": "upstream",
        "common_deps_usedby": {},
    }

    if include_sources is not None:
        context["common_deps"] = [{
            "name": "shared",
            "include_only": True,
            "sources_addl": include_sources,
        }]

    return template.render(**context)


def test_nonempty_sources():
    for family in TEMPLATES:
        output = render(family, ["optimized.c"])

        assert f"add_library({family}_test_ref OBJECT" in output
        assert f"add_library({family}_test_avx2 OBJECT" in output
        assert f"$<TARGET_OBJECTS:{family}_test_ref>" in output
        assert f"$<TARGET_OBJECTS:{family}_test_avx2>" in output


def test_empty_sources():
    for family in TEMPLATES:
        output = render(family, [])

        assert f"add_library({family}_test_ref OBJECT" in output
        assert f"$<TARGET_OBJECTS:{family}_test_ref>" in output

        assert f"add_library({family}_test_avx2 OBJECT" not in output
        assert f"$<TARGET_OBJECTS:{family}_test_avx2>" not in output
        assert f"target_include_directories({family}_test_avx2" not in output


def test_include_only_sources():
    for family in TEMPLATES:
        output = render(family, [], ["shared.c"])

        assert f"add_library({family}_test_avx2 OBJECT" in output
        assert f"upstream_shared/shared.c" in output
        assert f"$<TARGET_OBJECTS:{family}_test_avx2>" in output

def test_shared_only_sig_has_no_obsolete_impl_include():
    output = render("sig", [], ["shared.c"])

    assert (
        "add_library(sig_test_avx2 OBJECT upstream_shared/shared.c)"
        in output
    )
    assert (
        "target_include_directories(sig_test_avx2 PRIVATE "
        "${CMAKE_CURRENT_LIST_DIR}/upstream_test_avx2)"
        not in output
    )
    assert (
        "target_include_directories(sig_test_avx2 PRIVATE "
        "${CMAKE_CURRENT_LIST_DIR}/upstream_shared)"
        in output
    )


def test_default_sig_shared_source_spacing():
    template_path = ROOT / "sig" / "family" / "CMakeLists.txt"
    template = Environment().from_string(
        template_path.read_text(encoding="utf-8")
    )

    for sources in ([], ["reference.c"]):
        scheme = {
            "scheme": "test",
            "scheme_c": "test",
            "pqclean_scheme": "test",
            "default_implementation": "ref",
            "metadata": {
                "implementations": [{
                    "name": "ref",
                    "sources": sources,
                    "upstream": {"name": "upstream"},
                    "compile_opts": "",
                    "required_flags": [],
                    "common_dep": ["shared"],
                }]
            },
        }

        output = template.render(
            family="sig",
            schemes=[scheme],
            upstream_location="upstream",
            common_deps=[{
                "name": "shared",
                "include_only": True,
                "sources_addl": ["shared.c"],
            }],
            common_deps_usedby={},
        )

        lines = [
            line.strip()
            for line in output.splitlines()
            if line.strip().startswith("add_library(")
        ]

        assert len(lines) == 1
        assert "  " not in lines[0]

        expected = (
            "add_library(sig_test_ref OBJECT sig_sig_test.c "
            + (
                "upstream_test_ref/reference.c "
                if sources else ""
            )
            + "upstream_shared/shared.c)"
        )

        assert lines[0] == expected


def test_header_only_sig_retains_include_directory():
    output = render("sig", [], ["shared.c"], has_impl_files=True)

    assert (
        "target_include_directories(sig_test_avx2 PRIVATE "
        "${CMAKE_CURRENT_LIST_DIR}/upstream_test_avx2)"
        in output
    )


def test_empty_sig_directory_omits_include_directory():
    output = render("sig", [], ["shared.c"], has_impl_files=False)

    assert (
        "target_include_directories(sig_test_avx2 PRIVATE "
        "${CMAKE_CURRENT_LIST_DIR}/upstream_test_avx2)"
        not in output
    )
