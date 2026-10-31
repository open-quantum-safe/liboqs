# SPDX-License-Identifier: MIT
from pathlib import Path

import pytest

Environment = pytest.importorskip("jinja2").Environment


TEMPLATES = (
    "sig",
    "kem",
)

ROOT = Path(__file__).resolve().parents[1] / "src"


def render(family, sources, include_sources=None):
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
