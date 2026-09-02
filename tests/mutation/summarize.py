#!/usr/bin/env python3
# SPDX-License-Identifier: MIT

"""Aggregate Mull Elements reports for the focused public API campaign."""

from __future__ import annotations

import argparse
import json
from collections import Counter, defaultdict
from pathlib import Path


STATUS_PRIORITY = ("Killed", "Survived", "Timeout", "Error", "NotCovered")
SCOPED_FILES = {
    "src/kem/kem.c",
    "src/sig/sig.c",
    "src/sig_stfl/sig_stfl.c",
}
EXPECTED_REPORT_STEMS = {
    "kem-api-bike",
    "kem-api-hqc",
    "kem-api-ntru",
    "kem-api-ntruprime",
    "sig-api-cross",
    "sig-api-falcon",
    "sig-api-falcon-padded",
    "sig-api-mayo",
    "sig-api-snova",
    "stfl-api-lms-w1",
    "stfl-api-lms-w8",
    "stfl-api-xmss",
}


def short_path(path: str, root: Path) -> str:
    try:
        return str(Path(path).resolve().relative_to(root.resolve()))
    except ValueError:
        return path


def aggregate_reports(report_dir: Path, root: Path) -> dict:
    reports = []
    mutants: dict[str, dict] = {}
    for report_path in sorted(report_dir.glob("*.json")):
        if report_path.stem not in EXPECTED_REPORT_STEMS:
            continue
        report = json.loads(report_path.read_text(encoding="utf-8"))
        if "files" not in report:
            continue
        reports.append(report_path.name)
        for file_path, file_data in report["files"].items():
            relative_path = short_path(file_path, root)
            if relative_path not in SCOPED_FILES:
                continue
            for mutant in file_data.get("mutants", []):
                record = mutants.setdefault(
                    mutant["id"],
                    {
                        "file": relative_path,
                        "statuses": set(),
                    },
                )
                record["statuses"].add(mutant["status"])

    missing_reports = sorted(EXPECTED_REPORT_STEMS - {Path(name).stem for name in reports})
    if missing_reports:
        raise ValueError(f"missing Mull Elements reports in {report_dir}: {', '.join(missing_reports)}")

    per_file: dict[str, Counter] = defaultdict(Counter)
    overall = Counter()
    for mutant in mutants.values():
        statuses = mutant["statuses"]
        status = next((candidate for candidate in STATUS_PRIORITY if candidate in statuses), sorted(statuses)[0])
        overall[status] += 1
        per_file[mutant["file"]][status] += 1

    total = len(mutants)
    killed = overall["Killed"]
    return {
        "report_files": reports,
        "report_count": len(reports),
        "unique_mutants": total,
        "overall_statuses": dict(sorted(overall.items())),
        "mutation_score": round(100.0 * killed / total, 2) if total else None,
        "per_file": {
            file_path: {
                "total": sum(counts.values()),
                "statuses": dict(sorted(counts.items())),
                "mutation_score": round(100.0 * counts["Killed"] / sum(counts.values()), 2)
                if counts
                else None,
            }
            for file_path, counts in sorted(per_file.items())
        },
    }


def render_markdown(summary: dict, baseline: dict | None) -> str:
    lines = [
        "# Public API Mutation Summary",
        "",
        f"- Reports: {summary['report_count']}",
        f"- Unique mutants: {summary['unique_mutants']}",
        f"- Mutation score: {summary['mutation_score']:.2f}%",
    ]
    if baseline is not None:
        minimum = baseline["minimum_score"]
        delta = summary["mutation_score"] - minimum
        lines.append(f"- Baseline floor: {minimum:.2f}% ({delta:+.2f} percentage points)")
    lines.extend(
        [
            "",
            "| File | Mutants | Killed | Survived | Timeout | Error | Score |",
            "| --- | ---: | ---: | ---: | ---: | ---: | ---: |",
        ]
    )
    for file_path, data in summary["per_file"].items():
        statuses = data["statuses"]
        lines.append(
            "| {file} | {total} | {killed} | {survived} | {timeout} | {error} | {score:.2f}% |".format(
                file=file_path,
                total=data["total"],
                killed=statuses.get("Killed", 0),
                survived=statuses.get("Survived", 0),
                timeout=statuses.get("Timeout", 0),
                error=statuses.get("Error", 0),
                score=data["mutation_score"],
            )
        )
    lines.extend(
        [
            "",
            "Status counts are deduplicated by mutant id across representative algorithms; a mutant is counted as killed if any campaign case kills it.",
            "",
        ]
    )
    return "\n".join(lines)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--reports", required=True, type=Path, help="directory containing Mull Elements JSON reports")
    parser.add_argument("--baseline", type=Path, help="baseline JSON with minimum_score")
    parser.add_argument("--json-out", required=True, type=Path, help="aggregated JSON output path")
    parser.add_argument("--markdown-out", required=True, type=Path, help="Markdown summary output path")
    parser.add_argument("--enforce-baseline", action="store_true", help="fail if score drops below baseline minimum_score")
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[2]
    try:
        summary = aggregate_reports(args.reports, root)
    except ValueError as exc:
        parser.error(str(exc))

    baseline = None
    if args.baseline is not None:
        baseline = json.loads(args.baseline.read_text(encoding="utf-8"))
        summary["baseline"] = baseline
        summary["baseline_passed"] = summary["mutation_score"] >= baseline["minimum_score"]

    args.json_out.parent.mkdir(parents=True, exist_ok=True)
    args.markdown_out.parent.mkdir(parents=True, exist_ok=True)
    args.json_out.write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
    args.markdown_out.write_text(render_markdown(summary, baseline), encoding="utf-8")

    print(json.dumps({
        "report_count": summary["report_count"],
        "unique_mutants": summary["unique_mutants"],
        "overall_statuses": summary["overall_statuses"],
        "mutation_score": summary["mutation_score"],
        "baseline_passed": summary.get("baseline_passed"),
    }, indent=2))

    if args.enforce_baseline and baseline is not None and not summary["baseline_passed"]:
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
