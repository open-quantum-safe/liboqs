#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""
ci_family_report.py

Single-file CLI that generates a periodic report of GitHub Actions CI
compute time spent per liboqs algorithm family (BIKE, Classic McEliece,
ML-KEM, ML-DSA, SLH-DSA, XMSS, ...).

Subcommands (see `<subcommand> --help` for details):

  generate-definitions   Extract algorithm families from liboqs source
  build-matrix            Turn family definitions into a GitHub Actions matrix
  measure-family          Build+test one family in isolation, measure timing
  merge                   Merge per-family CSV fragments into one file
  fetch-ci-data           Fetch + classify real CI job durations via the API
  report                  Combine everything into the final report (MD+CSV)

Typical usage in a GitHub Actions workflow -- one call per pipeline stage,
see .github/workflows/ci-family-usage-report.yml for the full flow.

Typical local usage:

  git clone --depth 1 https://github.com/open-quantum-safe/liboqs.git
  python3 ci_family_report.py generate-definitions liboqs --out family_definitions.json

  mkdir -p results
  python3 ci_family_report.py measure-family --type KEM --name ml_kem \\
      --defs family_definitions.json --repo liboqs --out results/KEM_ml_kem.csv
  # ... repeat per family ...

  python3 ci_family_report.py merge results/ --out oqs_family_timing.csv

  export GH_TOKEN=<a token with no scopes at all is sufficient>
  python3 ci_family_report.py fetch-ci-data --repo open-quantum-safe/liboqs \\
      --workflow pr.yml --n-runs 5 --out ci_job_data.json

  python3 ci_family_report.py report oqs_family_timing.csv ci_job_data.json \\
      --out-md report.md --out-csv report.csv
"""
from __future__ import annotations

import argparse
import csv
import glob
import json
import os
import re
import resource
import subprocess
import sys
import time
import urllib.error
import urllib.request
from datetime import datetime, timezone
from pathlib import Path

API_BASE = "https://api.github.com"

# Job name substring -> family this job exclusively builds/tests.
# Verified against the actual liboqs workflow definitions (linux.yml,
# macos.yml, windows.yml): these jobs use -DOQS_MINIMAL_BUILD=SIG_slh_dsa.
# Intentionally editable -- extend if new dedicated-family CI jobs appear.
DEDICATED_FAMILY_PATTERNS = {
    "slhdsa": "slh_dsa",
}

NON_ALGO_KEYWORDS = [
    "basic-checks", "scorecard", "poutine", "workflow validation",
    "codeql", "docs-sync", "dependabot", "pages-build",
]

CSV_FIELDS = ["type", "family", "algo_count", "identifiers",
              "configure_s", "build_s", "test_s", "total_s", "peak_rss_kb"]


# ---------------------------------------------------------------------------
# generate-definitions
# ---------------------------------------------------------------------------

def _extract_pairs(header_path: Path, macro_prefix: str) -> dict:
    """#define <macro_prefix><ident> "<string>"  ->  {ident: string}"""
    pairs = {}
    content = header_path.read_text()
    pattern = rf'#define\s+{re.escape(macro_prefix)}([A-Za-z0-9_]+)\s+"([^"]+)"'
    for m in re.finditer(pattern, content):
        ident, s = m.groups()
        if ident in ("count", "identifier", "is_enabled"):
            continue
        pairs[ident] = s
    return pairs


def _family_dirs(src_dir: Path) -> list:
    return sorted([p.name for p in src_dir.iterdir() if p.is_dir() and p.name != "external"])


def _build_family_index(src_dir: Path, dirs: list) -> dict:
    """family -> concatenated text of all .c/.h files in that family
    (used for robust assignment via full-text search instead of prefix
    matching, which fails for cases like 'efrodokem_640_aes' living in
    the 'frodokem' directory, or 'xmssmt_...' living in 'xmss')."""
    index = {}
    for d in dirs:
        parts = []
        for ext in ("*.c", "*.h"):
            for f in (src_dir / d).rglob(ext):
                try:
                    parts.append(f.read_text(errors="ignore"))
                except OSError:
                    pass
        index[d] = "\n".join(parts)
    return index


def _assign_family(macro_full_name: str, family_index: dict):
    matches = [fam for fam, text in family_index.items() if macro_full_name in text]
    if len(matches) == 1:
        return matches[0]
    if len(matches) > 1:
        return max(matches, key=len)  # most specific (longest) name wins
    return None


def _build_group(repo: Path, src_subdir: str, header_name: str, macro_prefix: str, minimal_build_prefix: str):
    src_dir = repo / "src" / src_subdir
    header_path = src_dir / header_name
    if not header_path.exists():
        print(f"WARNING: {header_path} not found -- skipping {src_subdir}", file=sys.stderr)
        return {}

    pairs = _extract_pairs(header_path, macro_prefix)
    dirs = _family_dirs(src_dir)
    family_index = _build_family_index(src_dir, dirs)

    families = {}
    unmatched = []
    for ident, runtime_string in pairs.items():
        fam = _assign_family(f"{macro_prefix}{ident}", family_index)
        if fam is None:
            unmatched.append(ident)
            continue
        families.setdefault(fam, {"minimal_build_ids": [], "runtime_strings": []})
        families[fam]["minimal_build_ids"].append(f"{minimal_build_prefix}{ident}")
        families[fam]["runtime_strings"].append(runtime_string)

    if unmatched:
        print(f"WARNING: {len(unmatched)} identifiers in {src_subdir} could not be "
              f"assigned to a subdirectory family: {unmatched[:10]}"
              f"{'...' if len(unmatched) > 10 else ''}", file=sys.stderr)
    return families


def cmd_generate_definitions(args):
    repo = Path(args.repo_path)
    if not (repo / "src" / "kem" / "kem.h").exists():
        sys.exit(f"ERROR: {repo} does not look like a liboqs checkout (src/kem/kem.h missing).")

    kem_families = _build_group(repo, "kem", "kem.h", "OQS_KEM_alg_", "KEM_")
    sig_families = _build_group(repo, "sig", "sig.h", "OQS_SIG_alg_", "SIG_")
    stfl_families = _build_group(repo, "sig_stfl", "sig_stfl.h", "OQS_SIG_STFL_alg_", "SIG_STFL_")
    # SLH-DSA's ~150+ "prehash" combination variants are intentionally kept
    # as ONE family (slh_dsa), matching how liboqs's own CI treats it
    # (-DOQS_MINIMAL_BUILD=SIG_slh_dsa, never per individual parameter set).

    result = {"generated_from": str(repo), "kem": kem_families, "sig": sig_families, "stfl": stfl_families}

    n_fam = len(kem_families) + len(sig_families) + len(stfl_families)
    n_algo = sum(len(v["minimal_build_ids"]) for grp in (kem_families, sig_families, stfl_families) for v in grp.values())
    print(f"Detected: {len(kem_families)} KEM families, {len(sig_families)} SIG families, "
          f"{len(stfl_families)} STFL families ({n_fam} total, {n_algo} algorithms)", file=sys.stderr)

    out = json.dumps(result, indent=2)
    if args.out:
        Path(args.out).write_text(out)
        print(f"Written: {args.out}", file=sys.stderr)
    else:
        print(out)


# ---------------------------------------------------------------------------
# build-matrix
# ---------------------------------------------------------------------------

def cmd_build_matrix(args):
    data = json.load(open(args.definitions))
    matrix = []
    for grp_type, grp_key in (("KEM", "kem"), ("SIG", "sig"), ("SIG", "stfl")):
        for fam in sorted(data.get(grp_key, {}).keys()):
            entry = {"type": grp_type, "name": fam}
            if grp_key == "stfl":
                entry["stfl"] = True
            matrix.append(entry)

    out = json.dumps(matrix)
    if args.github_output:
        with open(os.environ["GITHUB_OUTPUT"], "a") as f:
            f.write(f"matrix={out}\n")
        print(f"Wrote 'matrix' to $GITHUB_OUTPUT ({len(matrix)} entries)", file=sys.stderr)
    else:
        print(out)


# ---------------------------------------------------------------------------
# measure-family
# ---------------------------------------------------------------------------

def _run_timed(cmd: list, cwd: Path, log_path: Path) -> tuple:
    """Run a subprocess, return (elapsed_seconds, peak_rss_kb, returncode).
    Peak RSS is read via resource.getrusage(RUSAGE_CHILDREN) instead of the
    external `/usr/bin/time` binary, avoiding an extra apt dependency."""
    rss_before = resource.getrusage(resource.RUSAGE_CHILDREN).ru_maxrss
    t0 = time.time()
    with open(log_path, "w") as log:
        proc = subprocess.run(cmd, cwd=cwd, stdout=log, stderr=subprocess.STDOUT)
    elapsed = time.time() - t0
    # ru_maxrss is cumulative across all children ever reaped by this
    # process; subtracting the "before" value isn't exact for concurrent
    # children, but for our strictly-sequential configure->build->test
    # flow it correctly isolates this step's peak.
    rss_after = resource.getrusage(resource.RUSAGE_CHILDREN).ru_maxrss
    peak_rss_kb = max(rss_after - rss_before, rss_after if rss_before == 0 else 0, 0)
    return elapsed, peak_rss_kb, proc.returncode


def cmd_measure_family(args):
    defs = json.load(open(args.defs))
    group_key = "kem" if args.type == "KEM" else "sig"
    is_stfl = args.name in defs.get("stfl", {})
    if is_stfl:
        group_key = "stfl"

    fam_def = defs[group_key][args.name]
    ids = ";".join(fam_def["minimal_build_ids"])
    runtime_strings = fam_def["runtime_strings"]
    algo_count = len(fam_def["minimal_build_ids"])

    skip_tests = args.skip_tests or is_stfl  # STFL needs KAT infra for real tests, see README

    repo = Path(args.repo).resolve()
    build_dir = Path(args.build_dir) if args.build_dir else Path(f"/tmp/liboqs_measure_{args.type}_{args.name}")
    build_dir.mkdir(parents=True, exist_ok=True)

    cmake_cmd = ["cmake", "-GNinja", f"-DOQS_MINIMAL_BUILD={ids}",
                 "-DOQS_DIST_BUILD=OFF", "-DBUILD_SHARED_LIBS=OFF", "-DCMAKE_BUILD_TYPE=Release"]
    if is_stfl:
        cmake_cmd.append("-DOQS_HAZARDOUS_EXPERIMENTAL_ENABLE_SIG_STFL_KEY_SIG_GEN=ON")
    cmake_cmd.append(str(repo))

    configure_s, _, rc = _run_timed(cmake_cmd, build_dir, build_dir / "configure.log")
    if rc != 0:
        sys.exit(f"ERROR: cmake configure failed for {args.type}/{args.name}, "
                 f"see {build_dir / 'configure.log'}")

    build_s, peak_rss_kb, rc = _run_timed(["ninja"], build_dir, build_dir / "build.log")
    if rc != 0:
        sys.exit(f"ERROR: ninja build failed for {args.type}/{args.name}, "
                 f"see {build_dir / 'build.log'}")

    test_s = 0.0
    if not skip_tests:
        bin_name = "test_kem" if args.type == "KEM" else "test_sig"
        test_bin = build_dir / "tests" / bin_name
        t0 = time.time()
        with open(build_dir / "test.log", "w") as log:
            for alg_string in runtime_strings:
                r = subprocess.run([str(test_bin), alg_string], stdout=log, stderr=subprocess.STDOUT)
                if r.returncode != 0:
                    print(f"Warning: test failed for {alg_string}", file=sys.stderr)
        test_s = time.time() - t0

    total_s = configure_s + build_s + test_s
    print(f"[{args.type}/{args.name}] configure={configure_s:.2f}s build={build_s:.2f}s "
          f"test={test_s:.2f}s total={total_s:.2f}s", file=sys.stderr)

    row = {
        "type": args.type, "family": args.name, "algo_count": algo_count, "identifiers": ids,
        "configure_s": f"{configure_s:.6f}", "build_s": f"{build_s:.6f}",
        "test_s": f"{test_s:.6f}", "total_s": f"{total_s:.6f}", "peak_rss_kb": peak_rss_kb,
    }

    out_path = Path(args.out)
    out_path.parent.mkdir(parents=True, exist_ok=True)
    with open(out_path, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=CSV_FIELDS)
        w.writerow(row)

    if not args.keep_build_dir:
        import shutil
        shutil.rmtree(build_dir, ignore_errors=True)


# ---------------------------------------------------------------------------
# merge
# ---------------------------------------------------------------------------

def cmd_merge(args):
    files = sorted(glob.glob(f"{args.results_dir}/*.csv"))
    if not files:
        sys.exit(f"ERROR: no CSV files found in {args.results_dir}.")

    out = open(args.out, "w", newline="") if args.out else sys.stdout
    writer = csv.writer(out)
    writer.writerow(CSV_FIELDS)
    for path in files:
        with open(path, newline="", encoding="utf-8") as f:
            for row in csv.reader(f):
                if len(row) == len(CSV_FIELDS):
                    writer.writerow(row)
                else:
                    print(f"WARNING: {path} has an unexpected number of columns ({len(row)}), skipped",
                          file=sys.stderr)
    if args.out:
        out.close()
        print(f"Written: {args.out} ({len(files)} families merged)", file=sys.stderr)


# ---------------------------------------------------------------------------
# fetch-ci-data
# ---------------------------------------------------------------------------

def _gh_api(path: str, token: str) -> dict:
    req = urllib.request.Request(f"{API_BASE}{path}")
    req.add_header("Authorization", f"Bearer {token}")
    req.add_header("Accept", "application/vnd.github+json")
    try:
        with urllib.request.urlopen(req) as resp:
            return json.load(resp)
    except urllib.error.HTTPError as e:
        print(f"ERROR on GET {path}: {e.code} {e.read().decode()}", file=sys.stderr)
        raise


def _parse_ts(t: str) -> datetime:
    return datetime.strptime(t, "%Y-%m-%dT%H:%M:%SZ")


def _classify_job(name: str) -> tuple:
    lname = name.lower()
    for pattern, family in DEDICATED_FAMILY_PATTERNS.items():
        if pattern in lname:
            return "dedicated_family", family
    for kw in NON_ALGO_KEYWORDS:
        if kw in lname:
            return "non_algo", None
    return "full_build", None


def _fetch_run_jobs(repo: str, run_id: int, token: str) -> list:
    jobs, page = [], 1
    while True:
        data = _gh_api(f"/repos/{repo}/actions/runs/{run_id}/jobs?per_page=100&page={page}", token)
        batch = data.get("jobs", [])
        jobs.extend(batch)
        if len(batch) < 100:
            break
        page += 1
    return jobs


def cmd_fetch_ci_data(args):
    token = os.environ.get("GH_TOKEN") or os.environ.get("GITHUB_TOKEN")
    if not token:
        sys.exit("ERROR: GH_TOKEN or GITHUB_TOKEN environment variable required.")

    wf_data = _gh_api(f"/repos/{args.repo}/actions/workflows/{args.workflow}", token)
    workflow_id = wf_data["id"]

    runs_data = _gh_api(
        f"/repos/{args.repo}/actions/workflows/{workflow_id}/runs"
        f"?per_page={args.n_runs * 3}&status=completed", token)
    candidate_runs = [r for r in runs_data.get("workflow_runs", []) if r.get("conclusion") == "success"]
    runs = candidate_runs[: args.n_runs]
    if len(runs) < args.n_runs:
        print(f"WARNING: only found {len(runs)} successful runs (requested: {args.n_runs})", file=sys.stderr)

    all_runs_summary = []
    for run in runs:
        jobs = _fetch_run_jobs(args.repo, run["id"], token)
        categorized = {"dedicated_family": {}, "full_build_s": 0.0, "non_algo_s": 0.0}
        job_details = []

        for j in jobs:
            st, en = j.get("started_at"), j.get("completed_at")
            if not st or not en:
                continue
            dur_s = (_parse_ts(en) - _parse_ts(st)).total_seconds()
            cat, family = _classify_job(j["name"])
            job_details.append({"name": j["name"], "duration_s": dur_s, "category": cat, "family": family})
            if cat == "dedicated_family":
                categorized["dedicated_family"].setdefault(family, 0.0)
                categorized["dedicated_family"][family] += dur_s
            elif cat == "non_algo":
                categorized["non_algo_s"] += dur_s
            else:
                categorized["full_build_s"] += dur_s

        if len(job_details) < 10:
            print(f"  -> WARNING: run {run['id']} only has {len(job_details)} jobs with timestamps -- "
                  f"skipping (likely incomplete/skipped)", file=sys.stderr)
            continue

        total_s = categorized["full_build_s"] + categorized["non_algo_s"] + sum(categorized["dedicated_family"].values())
        all_runs_summary.append({
            "run_id": run["id"], "run_created_at": run["created_at"], "total_jobs": len(job_details),
            "total_s": total_s, "full_build_s": categorized["full_build_s"],
            "non_algo_s": categorized["non_algo_s"], "dedicated_family_s": categorized["dedicated_family"],
            "jobs": job_details,
        })
        print(f"Run {run['id']} ({run['created_at']}): {len(job_details)} jobs, "
              f"{total_s/60:.1f} min total", file=sys.stderr)

    n = len(all_runs_summary)
    if n == 0:
        sys.exit("ERROR: no completed runs found.")

    avg_total_s = sum(r["total_s"] for r in all_runs_summary) / n
    avg_full_build_s = sum(r["full_build_s"] for r in all_runs_summary) / n
    avg_non_algo_s = sum(r["non_algo_s"] for r in all_runs_summary) / n
    avg_dedicated = {}
    for r in all_runs_summary:
        for fam, s in r["dedicated_family_s"].items():
            avg_dedicated.setdefault(fam, []).append(s)
    avg_dedicated_s = {fam: sum(vals) / len(vals) for fam, vals in avg_dedicated.items()}

    result = {
        "repo": args.repo, "workflow": args.workflow, "n_runs_analyzed": n,
        "avg_total_s": avg_total_s, "avg_full_build_s": avg_full_build_s,
        "avg_non_algo_s": avg_non_algo_s, "avg_dedicated_family_s": avg_dedicated_s,
        "runs": all_runs_summary,
    }
    with open(args.out, "w") as f:
        json.dump(result, f, indent=2)
    print(f"\nWritten: {args.out}", file=sys.stderr)
    print(f"Average over {n} run(s): {avg_total_s/60:.1f} min total, "
          f"of which {avg_full_build_s/60:.1f} min full-build pool, "
          f"{avg_non_algo_s/60:.1f} min non-algorithm-specific overhead, "
          f"dedicated families: {[(k, f'{v/60:.1f}min') for k, v in avg_dedicated_s.items()]}",
          file=sys.stderr)


# ---------------------------------------------------------------------------
# report
# ---------------------------------------------------------------------------

def cmd_report(args):
    family_rows = []
    with open(args.family_timing_csv, newline="", encoding="utf-8") as f:
        for r in csv.DictReader(f):
            r["build_s"] = float(r["build_s"])
            r["test_s"] = float(r["test_s"])
            r["configure_s"] = float(r["configure_s"])
            r["algo_relevant_s"] = r["build_s"] + r["test_s"]
            family_rows.append(r)

    ci_data = json.load(open(args.ci_job_data_json))
    total_s = ci_data["avg_total_s"]
    full_build_pool_s = ci_data["avg_full_build_s"]
    non_algo_s = ci_data["avg_non_algo_s"]
    dedicated_s = ci_data["avg_dedicated_family_s"]

    dedicated_families = set(dedicated_s.keys())
    model_rows = [r for r in family_rows if r["family"] not in dedicated_families]
    total_relevant = sum(r["algo_relevant_s"] for r in model_rows)
    for r in model_rows:
        r["weight"] = r["algo_relevant_s"] / total_relevant if total_relevant else 0
        r["est_ci_s"] = r["weight"] * full_build_pool_s
        r["source"] = "modeled (share of full-build pool)"

    dedicated_rows = []
    for fam, s in dedicated_s.items():
        match = next((r for r in family_rows if r["family"] == fam), None)
        dedicated_rows.append({
            "type": match["type"] if match else "SIG", "family": fam,
            "algo_count": match["algo_count"] if match else "?",
            "est_ci_s": s, "weight": None,
            "source": "DIRECTLY measured (dedicated CI jobs)",
        })

    all_rows = dedicated_rows + model_rows
    all_rows.sort(key=lambda r: r["est_ci_s"], reverse=True)
    generated_at = datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M UTC")

    with open(args.out_csv, "w", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        w.writerow(["family", "type", "algo_count", "est_ci_minutes_per_run",
                    "pct_of_total_run", "source", "generated_at"])
        for r in all_rows:
            w.writerow([r["family"], r["type"], r["algo_count"], f"{r['est_ci_s']/60:.2f}",
                        f"{r['est_ci_s']/total_s*100:.2f}", r["source"], generated_at])

    lines = [
        "# liboqs CI Resource Usage by Algorithm Family", "",
        f"*Automatically generated: {generated_at}*",
        f"*Based on {ci_data['n_runs_analyzed']} real, successfully completed "
        f"`{ci_data['workflow']}` run(s) of `{ci_data['repo']}`, "
        f"combined with locally measured build/test times per algorithm family.*", "",
        f"**Average total CI time per run: {total_s/60:.1f} minutes** ({total_s/60/60:.2f} hours)", "",
        "| Family | Type | Algorithms | Share of total run | ~Minutes/run | Source |",
        "|---|---|---:|---:|---:|---|",
    ]
    for r in all_rows:
        pct = r["est_ci_s"] / total_s * 100
        marker = "✅" if "DIRECTLY" in r["source"] else "≈"
        lines.append(f"| {r['family']} | {r['type']} | {r['algo_count']} | "
                     f"{pct:.2f}% | {r['est_ci_s']/60:.1f} | {marker} {r['source']} |")
    lines += [
        "",
        f"| *(non-algorithm-specific overhead)* | — | — | "
        f"{non_algo_s/total_s*100:.2f}% | {non_algo_s/60:.1f} | Formatting/docs/scorecard etc. |",
        "",
        "## Methodology & limitations", "",
        "- ✅ **Directly measured**: families with their own, isolated CI jobs "
        "(detected via name patterns, see `DEDICATED_FAMILY_PATTERNS` in this script) "
        "-- no modeling assumption needed.",
        "- ≈ **Modeled**: all other families are built/tested together in \"full-build\" "
        "jobs. Their share is estimated from the ratio of locally, isolated measured "
        "build+test times (`build_s + test_s`) applied to the observed full-build CI "
        "time pool.",
        "- This modeling **tends to underestimate** families with disproportionately "
        "expensive test suites (e.g. large KAT vectors), which would only show up in "
        "real, dedicated CI jobs -- compare the large gap between SLH-DSA's local "
        "build time (seconds) and its real dedicated CI time (tens of minutes).",
        f"- Data basis: {ci_data['n_runs_analyzed']} CI run(s). For more stable "
        "estimates, increase --n-runs in the fetch-ci-data step.",
    ]

    Path(args.out_md).write_text("\n".join(lines) + "\n")
    print(f"Written: {args.out_md}, {args.out_csv}")


# ---------------------------------------------------------------------------
# CLI wiring
# ---------------------------------------------------------------------------

def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="command", required=True)

    p = sub.add_parser("generate-definitions", help="Extract algorithm families from liboqs source")
    p.add_argument("repo_path", help="Path to a liboqs checkout")
    p.add_argument("--out", help="Output JSON path (default: stdout)")
    p.set_defaults(func=cmd_generate_definitions)

    p = sub.add_parser("build-matrix", help="Turn family definitions into a GitHub Actions matrix")
    p.add_argument("definitions", help="family_definitions.json path")
    p.add_argument("--github-output", action="store_true",
                    help="Append 'matrix=...' to $GITHUB_OUTPUT instead of printing to stdout")
    p.set_defaults(func=cmd_build_matrix)

    p = sub.add_parser("measure-family", help="Build+test one family in isolation, measure timing")
    p.add_argument("--type", required=True, choices=["KEM", "SIG"])
    p.add_argument("--name", required=True, help="Family name, e.g. 'bike', 'ml_dsa', 'xmss'")
    p.add_argument("--defs", required=True, help="family_definitions.json path")
    p.add_argument("--repo", required=True, help="Path to a liboqs checkout")
    p.add_argument("--out", required=True, help="Output CSV path for this family's single data row")
    p.add_argument("--skip-tests", action="store_true", help="Only measure configure+build, no test run")
    p.add_argument("--build-dir", help="Use this directory instead of a fresh temp dir (for debugging)")
    p.add_argument("--keep-build-dir", action="store_true", help="Don't delete the build dir afterwards")
    p.set_defaults(func=cmd_measure_family)

    p = sub.add_parser("merge", help="Merge per-family CSV fragments into one file")
    p.add_argument("results_dir", help="Directory containing <TYPE>_<FAMILY>.csv files")
    p.add_argument("--out", help="Output CSV path (default: stdout)")
    p.set_defaults(func=cmd_merge)

    p = sub.add_parser("fetch-ci-data", help="Fetch + classify real CI job durations via the GitHub API")
    p.add_argument("--repo", required=True, help="e.g. open-quantum-safe/liboqs")
    p.add_argument("--workflow", default="pr.yml")
    p.add_argument("--n-runs", type=int, default=5)
    p.add_argument("--out", required=True)
    p.set_defaults(func=cmd_fetch_ci_data)

    p = sub.add_parser("report", help="Combine measurements + CI data into the final report")
    p.add_argument("family_timing_csv")
    p.add_argument("ci_job_data_json")
    p.add_argument("--out-md", required=True)
    p.add_argument("--out-csv", required=True)
    p.set_defaults(func=cmd_report)

    args = ap.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
