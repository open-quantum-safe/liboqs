# SPDX-License-Identifier: MIT
"""Tests for the upstream implementation importer."""

import os
import sys

import pytest

LIBOQS_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
COPY_SCRIPT_DIR = os.path.join(LIBOQS_ROOT, "scripts", "copy_from_upstream")
sys.path.insert(0, COPY_SCRIPT_DIR)

from upstream_platforms import normalize_platform  # noqa: E402


def test_normalize_arm_platform_flags():
    platform = {
        "architecture": "arm_8",
        "required_flags": ["asimd", "aes", "sha3"],
    }

    assert normalize_platform(platform) == {
        "architecture": "ARM64_V8",
        "required_flags": ["arm_neon", "arm_aes", "arm_sha3"],
    }


def _render_cmake_template(kind, required_flags, target):
    jinja2 = pytest.importorskip("jinja2")
    template_path = os.path.join(
        COPY_SCRIPT_DIR, "src", kind, "family", "CMakeLists.txt"
    )
    with open(template_path, encoding="utf-8") as template_file:
        template = jinja2.Template(template_file.read())

    if target == "implementation":
        implementation = {
            "name": "neon",
            "compile_opts": "",
            "required_flags": required_flags,
            "sources": [],
            "upstream": {"name": "upstream"},
        }
        schemes = [
            {
                "default_implementation": "default",
                "metadata": {"implementations": [implementation]},
                "pqclean_scheme": "test",
                "scheme": "test",
                "scheme_c": "test",
            }
        ]
        common_deps = []
        common_deps_usedby = {}
    else:
        schemes = []
        common_deps = [
            {
                "compile_opts": "",
                "include_only": False,
                "name": "common",
                "required_flags": required_flags,
                "sources_addl": [],
            }
        ]
        common_deps_usedby = {
            "common": [{"impl_name": "neon", "scheme_c": "test"}]
        }

    return template.render(
        common_deps=common_deps,
        common_deps_usedby=common_deps_usedby,
        default_implementation="default",
        family="test",
        schemes=schemes,
        upstream_location="upstream",
    )


@pytest.mark.parametrize("kind", ["kem", "sig"])
@pytest.mark.parametrize("target", ["common", "implementation"])
def test_arm_aes_and_sha3_compile_flags(kind, target):
    output = _render_cmake_template(
        kind, ["arm_neon", "arm_aes", "arm_sha3"], target
    )

    assert output.count("-march=armv8-a+crypto+sha3") == 1
    assert "-marm_aes" not in output
    assert "-marm_sha3" not in output


@pytest.mark.parametrize("kind", ["kem", "sig"])
@pytest.mark.parametrize("target", ["common", "implementation"])
def test_arm_aes_only_compile_flags(kind, target):
    output = _render_cmake_template(kind, ["arm_neon", "arm_aes"], target)

    assert output.count("-march=armv8-a+crypto") == 1
    assert "-marm_aes" not in output
