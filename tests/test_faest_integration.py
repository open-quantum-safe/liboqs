# SPDX-License-Identifier: MIT

import importlib.util
import os
import subprocess
from pathlib import Path

import pytest
import yaml

LIBOQS_ROOT = Path(__file__).resolve().parents[1]
COPY_FROM_UPSTREAM = LIBOQS_ROOT / "scripts" / "copy_from_upstream"
FAEST_GENERATE = COPY_FROM_UPSTREAM / "faest" / "generate.py"
FAEST_REF_REPO = COPY_FROM_UPSTREAM / "repos" / "faest-ref"
COPY_YML = COPY_FROM_UPSTREAM / "copy_from_upstream.yml"


def _load_faest_generate():
    spec = importlib.util.spec_from_file_location("faest_generate", FAEST_GENERATE)
    assert spec is not None and spec.loader is not None
    mod = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(mod)
    return mod


def _pinned_faest_ref_commit() -> str:
    data = yaml.safe_load(COPY_YML.read_text(encoding="utf-8"))
    for upstream in data["upstreams"]:
        if upstream["name"] == "faest-ref":
            return upstream["git_commit"]
    raise RuntimeError("faest-ref upstream not found in copy_from_upstream.yml")


def _ensure_faest_ref_checkout() -> Path:
    if FAEST_REF_REPO.is_dir() and (FAEST_REF_REPO / "meson.build").is_file():
        return FAEST_REF_REPO

    commit = _pinned_faest_ref_commit()
    upstream = yaml.safe_load(COPY_YML.read_text(encoding="utf-8"))
    faest_upstream = next(u for u in upstream["upstreams"] if u["name"] == "faest-ref")
    url = faest_upstream["git_url"]

    FAEST_REF_REPO.parent.mkdir(parents=True, exist_ok=True)
    if not FAEST_REF_REPO.exists():
        subprocess.run(
            ["git", "init", str(FAEST_REF_REPO)],
            check=True,
            capture_output=True,
        )
        subprocess.run(
            ["git", "-C", str(FAEST_REF_REPO), "remote", "add", "origin", url],
            check=True,
            capture_output=True,
        )
    subprocess.run(
        ["git", "-C", str(FAEST_REF_REPO), "fetch", "--depth=1", "origin", commit],
        check=True,
        capture_output=True,
    )
    subprocess.run(
        ["git", "-C", str(FAEST_REF_REPO), "reset", "--hard", commit],
        check=True,
        capture_output=True,
    )
    return FAEST_REF_REPO


@pytest.mark.skipif(
    not FAEST_GENERATE.is_file(),
    reason="FAEST integration generator not present",
)
def test_faest_integration_artifacts_current():
    faest_ref = _ensure_faest_ref_checkout()
    mod = _load_faest_generate()
    stale = mod.check_stale(faest_ref.resolve())
    assert stale == [], (
        "FAEST integration artifacts are stale. "
        f"Run {FAEST_GENERATE} --faest-ref {faest_ref} and commit the results. "
        f"First stale file: {stale[0] if stale else 'unknown'}"
    )
