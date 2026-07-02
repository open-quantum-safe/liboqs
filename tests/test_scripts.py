# SPDX-License-Identifier: MIT
"""Smoke and regression tests for scripts in scripts/."""

import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

import pytest

import helpers


LIBOQS_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
SCRIPTS = os.path.join(LIBOQS_ROOT, "scripts")


TESTED_SCRIPT_PATHS = {
    "build-android.sh",
    "copy_from_upstream/copy_from_slh_dsa_c.py",
    "copy_from_upstream/copy_from_upstream.py",
    "copy_from_upstream/update_upstream_alg_docs.py",
    "copy_from_xkcp/checkout.sh",
    "copy_from_xkcp/package.sh",
    "copy_from_xkcp/update_patches.sh",
    "doxyfy.py",
    "format_code.sh",
    "format_docs_yaml.py",
    "generate_algorithms_md.py",
    "genkatdict.py",
    "genkatsha256.sh",
    "git_commit.sh",
    "parse_liboqs_speed.py",
    "provider-test-trigger.sh",
    "run_doxygen.sh",
    "update_alg_support_table.py",
    "update_cbom.py",
    "update_docs_from_yaml.py",
    "validate_cbom.sh",
}


def _run(command, working_dir=LIBOQS_ROOT, expected_returncode=0, **kwargs):
    return helpers.run_subprocess(
        command,
        working_dir=str(working_dir),
        expected_returncode=expected_returncode,
        **kwargs,
    )


def test_all_scripts_are_covered_by_smoke_tests():
    actual = {
        path.relative_to(SCRIPTS).as_posix()
        for path in Path(SCRIPTS).rglob("*")
        if path.suffix in {".py", ".sh"}
        and "repos" not in path.relative_to(SCRIPTS).parts
    }
    assert actual == TESTED_SCRIPT_PATHS


def test_genkatdict_writes_kats_json(tmp_path):
    (tmp_path / "alg2.kat.sha256").write_text("222\n", encoding="utf-8")
    (tmp_path / "alg1.kat.sha256").write_text("111\n", encoding="utf-8")

    _run([sys.executable, os.path.join(SCRIPTS, "genkatdict.py")], working_dir=tmp_path)

    with open(tmp_path / "kats.json", encoding="utf-8") as f:
        assert json.load(f) == {"alg1": "111\n", "alg2": "222\n"}


def test_doxyfy_reformats_headings_and_links(tmp_path):
    source = tmp_path / "input.md"
    output = tmp_path / "output.md"
    source.write_text(
        "# Main Heading\n\n"
        "Legacy Heading\n"
        "==============\n\n"
        "[Go](#Main Heading)\n",
        encoding="utf-8",
    )

    _run([sys.executable, os.path.join(SCRIPTS, "doxyfy.py"), str(source), str(output)])

    reformatted = output.read_text(encoding="utf-8")
    assert "# Main Heading {#main-heading}\n" in reformatted
    assert "Legacy Heading {#legacy-heading}\n" in reformatted


def test_format_docs_yaml_formats_temp_tree(tmp_path):
    pytest.importorskip("yaml")
    kem_dir = tmp_path / "docs" / "algorithms" / "kem"
    sig_dir = tmp_path / "docs" / "algorithms" / "sig"
    kem_dir.mkdir(parents=True)
    sig_dir.mkdir(parents=True)
    (kem_dir / "sample.yml").write_text("name: Sample\nparameter-sets: []\n", encoding="utf-8")
    (sig_dir / "sample.yml").write_text("name: SampleSig\nparameter-sets: []\n", encoding="utf-8")

    _run([sys.executable, os.path.join(SCRIPTS, "format_docs_yaml.py"), "--liboqs-root", str(tmp_path)])

    assert "name: Sample\n" in (kem_dir / "sample.yml").read_text(encoding="utf-8")
    assert "name: SampleSig\n" in (sig_dir / "sample.yml").read_text(encoding="utf-8")


def test_update_docs_from_yaml_updates_empty_temp_tree(tmp_path):
    pytest.importorskip("yaml")
    pytest.importorskip("tabulate")
    for family in ["kem", "sig", "sig_stfl"]:
        (tmp_path / "docs" / "algorithms" / family).mkdir(parents=True)

    (tmp_path / "README.md").write_text(
        "<!-- OQS_TEMPLATE_FRAGMENT_ALG_SUPPORT_START -->\n"
        "old\n"
        "<!-- OQS_TEMPLATE_FRAGMENT_ALG_SUPPORT_END -->\n",
        encoding="utf-8",
    )
    (tmp_path / "ALGORITHMS.md").write_text(
        "\n".join(
            [
                "<!-- OQS_TEMPLATE_FRAGMENT_ALGORITHMS_KEM_START -->",
                "old",
                "<!-- OQS_TEMPLATE_FRAGMENT_ALGORITHMS_KEM_END -->",
                "<!-- OQS_TEMPLATE_FRAGMENT_ALGORITHMS_SIG_START -->",
                "old",
                "<!-- OQS_TEMPLATE_FRAGMENT_ALGORITHMS_SIG_END -->",
                "<!-- OQS_TEMPLATE_FRAGMENT_ALGORITHMS_SIG_STFL_START -->",
                "old",
                "<!-- OQS_TEMPLATE_FRAGMENT_ALGORITHMS_SIG_STFL_END -->",
            ]
        ),
        encoding="utf-8",
    )

    _run([sys.executable, os.path.join(SCRIPTS, "update_docs_from_yaml.py"), "--liboqs-root", str(tmp_path)])

    assert "Algorithm family" in (tmp_path / "README.md").read_text(encoding="utf-8")
    assert "old" not in (tmp_path / "ALGORITHMS.md").read_text(encoding="utf-8")


def test_update_cbom_builds_temp_cbom(tmp_path):
    pytest.importorskip("git")
    pytest.importorskip("yaml")
    kem_dir = tmp_path / "docs" / "algorithms" / "kem"
    sig_dir = tmp_path / "docs" / "algorithms" / "sig"
    docs_dir = tmp_path / "docs"
    kem_dir.mkdir(parents=True)
    sig_dir.mkdir(parents=True)
    (kem_dir / "samplekem.yml").write_text(
        """
name: SampleKEM
type: kem
parameter-sets:
  - name: SampleKEM-1
    claimed-nist-level: 1
    implementations:
      - upstream-id: clean
        supported-platforms: all
""".lstrip(),
        encoding="utf-8",
    )
    (sig_dir / "samplesig.yml").write_text(
        """
name: SampleSIG
type: signature
parameter-sets:
  - name: SampleSIG-1
    claimed-nist-level: 1
    implementations:
      - upstream-id: clean
        supported-platforms: all
""".lstrip(),
        encoding="utf-8",
    )

    _run(
        [
            sys.executable,
            os.path.join(SCRIPTS, "update_cbom.py"),
            "--liboqs-root",
            str(tmp_path),
            "--liboqs-version",
            "test-version",
        ]
    )

    with open(docs_dir / "cbom.json", encoding="utf-8") as f:
        cbom = json.load(f)
    assert cbom["bomFormat"] == "CycloneDX"
    assert cbom["components"][0]["version"] == "test-version"
    assert "alg:SampleKEM-1:clean:generic" in cbom["dependencies"][0]["provides"]
    assert "alg:SampleSIG-1:clean:generic" in cbom["dependencies"][0]["provides"]


def test_parse_liboqs_speed_writes_benchmark_json(tmp_path):
    log = tmp_path / "speed.log"
    log.write_text(
        "Configuration info\n"
        "------------------\n"
        "Compiler: test-cc\n"
        "-------\n"
        "ML-KEM-512 operations\n"
        "keygen | 2 | 1 | 0.5 | ignore | 123 |\n"
        "encaps | 2 | 1 | 0.5 | ignore | 456 |\n"
        "decaps | 2 | 1 | 0.5 | ignore | 789 |\n"
        "Ended\n",
        encoding="utf-8",
    )

    _run(
        [
            sys.executable,
            os.path.join(SCRIPTS, "parse_liboqs_speed.py"),
            str(log),
            "--algorithm",
            "ignored-by-parser",
        ],
        working_dir=tmp_path,
    )

    with open(tmp_path / "ML-KEM-512_formatted.json", encoding="utf-8") as f:
        data = json.load(f)
    assert [entry["name"] for entry in data] == [
        "ML-KEM-512 keygen",
        "ML-KEM-512 encaps",
        "ML-KEM-512 decaps",
    ]
    assert [entry["value"] for entry in data] == [123, 456, 789]
    assert data[0]["extra"] == "Compiler: test-cc | "


def test_copy_from_slh_dsa_fragment_bounds():
    pytest.importorskip("yaml")
    pytest.importorskip("jinja2")
    sys.path.insert(0, os.path.join(SCRIPTS, "copy_from_upstream"))
    import copy_from_slh_dsa_c  # noqa: E402

    text = "pre\n##### OQS_COPY_FROM_SLH_DSA_FRAGMENT_BODY_STARTpayload##### OQS_COPY_FROM_SLH_DSA_FRAGMENT_BODY_END\npost"
    start = copy_from_slh_dsa_c.section_bound("BODY", "#####", text, "START")
    end = copy_from_slh_dsa_c.section_bound("BODY", "#####", text, "END")

    assert text[start:end] == "payload"


def test_copy_from_upstream_requires_liboqs_dir():
    pytest.importorskip("yaml")
    pytest.importorskip("jinja2")
    env = os.environ.copy()
    env.pop("LIBOQS_DIR", None)
    result = subprocess.run(
        [
            sys.executable,
            os.path.join(SCRIPTS, "copy_from_upstream", "copy_from_upstream.py"),
            "verify",
        ],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        cwd=os.path.join(SCRIPTS, "copy_from_upstream"),
        env=env,
    )
    assert result.returncode == 1
    assert "Must set environment variable LIBOQS_DIR" in result.stdout.decode("utf-8")


def test_update_upstream_alg_docs_help_is_available():
    pytest.importorskip("yaml")
    out = _run(
        [
            sys.executable,
            os.path.join(SCRIPTS, "copy_from_upstream", "update_upstream_alg_docs.py"),
            "--help",
        ]
    )
    assert "--write-changes" in out


@pytest.mark.skipif(sys.platform.startswith("win"), reason="Shell script smoke tests require sh")
def test_genkatsha256_hashes_kat_files(tmp_path):
    kat = tmp_path / "sample.kat"
    kat.write_text("known answer\n", encoding="utf-8")

    _run([os.path.join(SCRIPTS, "genkatsha256.sh")], working_dir=tmp_path)

    expected = hashlib.sha256(b"known answer\n").hexdigest()
    assert (tmp_path / "sample.kat.sha256").read_text(encoding="utf-8") == expected


@pytest.mark.skipif(sys.platform.startswith("win"), reason="Shell script smoke tests require sh")
def test_build_android_help_is_available():
    out = _run([os.path.join(SCRIPTS, "build-android.sh"), "--help"])
    assert "Usage: ./build-android" in out


@pytest.mark.skipif(sys.platform.startswith("win"), reason="Shell script smoke tests require sh")
def test_git_commit_reports_a_revision_or_dirty_state():
    out = _run([os.path.join(SCRIPTS, "git_commit.sh")])
    assert out == "unknown" or len(out) >= 40


@pytest.mark.skipif(sys.platform.startswith("win"), reason="Shell script smoke tests require sh")
def test_format_code_requires_liboqs_dir():
    out = _run([os.path.join(SCRIPTS, "format_code.sh")], expected_returncode=1, env={"LIBOQS_DIR": ""})
    assert "Please set the environment variable LIBOQS_DIR" in out


@pytest.mark.skipif(sys.platform.startswith("win"), reason="Shell script smoke tests require sh")
def test_run_doxygen_rejects_missing_arguments():
    out = _run([os.path.join(SCRIPTS, "run_doxygen.sh")], expected_returncode=1)
    assert "Wrong number of arguments" in out


@pytest.mark.skipif(sys.platform.startswith("win"), reason="Shell script smoke tests require sh")
def test_provider_test_trigger_requires_access_token():
    out = _run([os.path.join(SCRIPTS, "provider-test-trigger.sh")], expected_returncode=1, env={"ACCESS_TOKEN": ""})
    assert "requires a GitHub Personal Access Token" in out


@pytest.mark.skipif(sys.platform.startswith("win"), reason="Shell script smoke tests require sh")
def test_validate_cbom_invokes_schema_validator_with_local_tools(tmp_path):
    bin_dir = tmp_path / "bin"
    bin_dir.mkdir()
    for command in ["wget", "ajv"]:
        tool = bin_dir / command
        tool.write_text("#!/bin/sh\nprintf '%s\\n' \"$0 $@\"\n", encoding="utf-8")
        tool.chmod(0o755)

    out = _run(
        [os.path.join(SCRIPTS, "validate_cbom.sh")],
        env={"PATH": str(bin_dir) + os.pathsep + os.environ["PATH"]},
    )

    assert "bom-1.6.schema.json" in out
    assert "../docs/cbom.json" in out


@pytest.mark.skipif(sys.platform.startswith("win"), reason="Shell script smoke tests require sh")
@pytest.mark.parametrize(
    "script_name",
    [
        "checkout.sh",
        "update_patches.sh",
        "package.sh",
    ],
)
def test_copy_from_xkcp_scripts_fail_before_network_or_packaging(tmp_path, script_name):
    work = tmp_path / "copy_from_xkcp"
    shutil.copytree(os.path.join(SCRIPTS, "copy_from_xkcp"), work)
    for name in ["upstream", "upstream-patched", "build"]:
        path = work / name
        if path.exists():
            if path.is_dir():
                shutil.rmtree(path)
            else:
                path.unlink()
    if script_name == "package.sh":
        version = (work / "VERSION").read_text(encoding="utf-8").strip()
        (work / f"{version}.zip").write_text("not a zip\n", encoding="utf-8")
    elif script_name == "checkout.sh":
        version = (work / "VERSION").read_text(encoding="utf-8").strip()
        (work / f"{version}.zip").write_text("not a zip\n", encoding="utf-8")

    out = _run([os.path.join(work, script_name)], working_dir=work, ignore_returncode=True)
    assert "not found" in out or "End-of-central-directory signature not found" in out or str(work) in out
