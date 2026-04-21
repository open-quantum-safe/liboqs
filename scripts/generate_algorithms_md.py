#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Render per-family algorithm tables and splice them into ALGORITHMS.md.

ALGORITHMS.md sits between the per-family README table and the per-algorithm
pages under docs/algorithms/. Its purpose is to give a single-page view
that's richer than README (NIST levels supported, constant-time coverage,
formal-verification status, optimization targets) without the repetition
of a per-parameter-set table. Per-variant detail lives in the per-algorithm
pages.

The prose, vocabulary, tier definitions, and section headings in
ALGORITHMS.md are hand-maintained. This script only replaces the contents
between the three OQS_TEMPLATE_FRAGMENT markers.
"""

import glob
import os
from typing import List

import tabulate
import yaml

from update_alg_support_table import (
    format_oqs_tier,
    format_upstream_maintenance,
    format_upstream_source,
)

HEADER = [
    "Algorithm family",
    "Standardization",
    "Primary implementation",
    "Upstream maintenance",
    "OQS tier",
    "NIST levels",
    "Constant-time",
    "Formally verified",
    "Optimization targets",
]

FRAGMENTS = [
    ("kem", "KEM"),
    ("sig", "SIG"),
    ("sig_stfl", "SIG_STFL"),
]


def _nist_levels(algdata: dict) -> str:
    levels = sorted({
        ps.get("claimed-nist-level")
        for ps in algdata.get("parameter-sets", [])
        if ps.get("claimed-nist-level") is not None
    })
    return ", ".join(str(l) for l in levels) if levels else "—"


def _ct_rollup(algdata: dict) -> str:
    """Rolled-up constant-time status across default implementations of
    every parameter set: ✓ if all checked by valgrind; 'claimed' if all
    are at least claimed; 'partial' if mixed; '—' if none."""
    checked = []
    claimed = []
    for ps in algdata.get("parameter-sets", []):
        impls = ps.get("implementations", [])
        default = next(
            (i for i in impls if i.get("default")),
            impls[0] if impls else {},
        )
        checked.append(bool(default.get("no-secret-dependent-branching-checked-by-valgrind")))
        claimed.append(bool(default.get("no-secret-dependent-branching-claimed")))
    if not checked:
        return "—"
    if all(checked):
        return "✓"
    if any(checked):
        return "partial"
    if all(claimed):
        return "claimed"
    if any(claimed):
        return "partial"
    return "—"


def _fv_rollup(algdata: dict) -> str:
    """formally-verified-upstreams lists alternative upstreams (e.g. libjade)
    that carry a formal verification. By convention the primary upstream is
    not listed there, so its presence signals partial (optional) FV coverage."""
    fv = algdata.get("formally-verified-upstreams") or {}
    return "partial" if fv else "—"


def _opt_targets(algdata: dict) -> str:
    """Distinct architecture/flag-set labels across all implementations of
    all parameter sets. Keeps the cell short by deduping."""
    labels = set()
    any_portable = False
    for ps in algdata.get("parameter-sets", []):
        for impl in ps.get("implementations", []):
            platforms = impl.get("supported-platforms")
            if platforms == "all" or not platforms:
                any_portable = True
                continue
            if not isinstance(platforms, list):
                continue
            for p in platforms:
                arch = p.get("architecture", "")
                flags = sorted(p.get("required_flags") or [])
                labels.add(f"{arch} ({'+'.join(flags)})" if flags else arch)
    parts = []
    if any_portable:
        parts.append("portable")
    parts.extend(sorted(labels))
    return ", ".join(parts) if parts else "—"


def family_row(algdata: dict) -> List[str]:
    return [
        algdata["name"],
        algdata.get("standardization-status", ""),
        format_upstream_source(algdata["primary-upstream"]["source"]),
        format_upstream_maintenance(algdata),
        format_oqs_tier(algdata),
        _nist_levels(algdata),
        _ct_rollup(algdata),
        _fv_rollup(algdata),
        _opt_targets(algdata),
    ]


def render_section(doc_dir: str) -> str:
    yaml_paths = sorted(glob.glob(os.path.join(doc_dir, "*.yml")))
    rows = [HEADER]
    for p in yaml_paths:
        with open(p) as f:
            algdata = yaml.safe_load(f)
        rows.append(family_row(algdata))
    return tabulate.tabulate(rows, tablefmt="pipe", headers="firstrow")


def _splice(doc: str, tag: str, content: str) -> str:
    start_marker = f"<!-- OQS_TEMPLATE_FRAGMENT_ALGORITHMS_{tag}_START -->"
    end_marker = f"<!-- OQS_TEMPLATE_FRAGMENT_ALGORITHMS_{tag}_END -->"
    start = doc.find(start_marker)
    end = doc.find(end_marker)
    if start < 0 or end < 0:
        raise RuntimeError(
            f"ALGORITHMS.md missing {tag} fragment markers "
            f"({start_marker} / {end_marker})"
        )
    start += len(start_marker)
    return doc[:start] + "\n" + content + "\n" + doc[end:]


def write(liboqs_root: str) -> None:
    path = os.path.join(liboqs_root, "ALGORITHMS.md")
    with open(path) as f:
        doc = f.read()
    for cat, tag in FRAGMENTS:
        table = render_section(
            os.path.join(liboqs_root, "docs", "algorithms", cat)
        )
        doc = _splice(doc, tag, table)
    with open(path, "w") as f:
        f.write(doc)
