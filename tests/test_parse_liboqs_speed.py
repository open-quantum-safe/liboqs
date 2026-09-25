# SPDX-License-Identifier: MIT
"""Tests for the speed-benchmark log parser under scripts/."""

import collections
import json
import os
import subprocess
import sys

import pytest

LIBOQS_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
PARSER = os.path.join(LIBOQS_ROOT, "scripts", "parse_liboqs_speed.py")

# The log parser is driven by the exact layout printed by the speed programs:
# the "Configuration info" block comes from tests/system_info.c, and the column
# header and the per-operation rows from tests/ds_benchmark.h.
_COLUMN_ROW = "%-36s | %10s | %14s | %15s | %10s | %25s | %10s"
_SEPARATOR_ROW = "%-36s | %10s:| %14s:| %15s:| %10s:| %25s:| %10s:"
_OPERATION_ROW = "%-36s | %10d | %14.3f | %15.3f | %10.3f | %25.0f | %10.0f"

Operation = collections.namedtuple(
    "Operation", "name iterations total_time mean_time stdev cycles cycles_stdev")

# one KEM and one signature algorithm, with cycles chosen to differ from the
# other numeric columns so a mis-mapped column cannot go unnoticed
_BIKE_L1_OPERATIONS = [
    Operation("keygen", 1000, 1.000, 1000.000, 10.000, 123456, 1000),
    Operation("encaps", 2000, 2.000, 1000.000, 20.000, 234567, 2000),
    Operation("decaps", 3000, 3.000, 1000.000, 30.000, 345678, 3000),
]
_ML_DSA_44_OPERATIONS = [
    Operation("keypair", 400, 1.000, 2500.000, 10.000, 500000, 300),
    Operation("sign", 500, 2.000, 4000.000, 20.000, 600000, 400),
    Operation("verify", 600, 3.000, 5000.000, 30.000, 700000, 500),
]


def _write_speed_log(directory, filename, algorithm, operations):
    """Write a minimal log as produced by `speed_kem`/`speed_sig`."""
    lines = [
        "Configuration info",
        "==================",
        "Target platform:  x86_64",
        "OQS version:      0.15.0 (major: 0, minor: 15, patch: 0)",
        "",
        "Speed test",
        "==========",
        "Started at 2026-01-15 12:00:00",
        _COLUMN_ROW % ("Operation", "Iterations", "Total time (s)", "Time (us): mean",
                       "pop. stdev", "CPU cycles: mean", "pop. stdev"),
        _SEPARATOR_ROW % ("-" * 36, "-" * 10, "-" * 14, "-" * 15, "-" * 10, "-" * 25, "-" * 10),
        # the speed programs print the algorithm name on a row of its own
        _COLUMN_ROW % (algorithm, "", "", "", "", "", ""),
    ]
    lines += [_OPERATION_ROW % operation for operation in operations]
    lines.append("Ended at 2026-01-15 12:01:00")
    path = os.path.join(directory, filename)
    with open(path, "w") as fh:
        fh.write("\n".join(lines) + "\n")
    return path


def _run_parser(working_dir, log_name, algorithm):
    """Invoke the parser the way the benchmark workflows do, in `working_dir`."""
    return subprocess.run(
        [sys.executable, PARSER, log_name, "--algorithm", algorithm],
        cwd=working_dir,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
    )


@pytest.mark.parametrize(
    "algorithm,operations",
    [
        ("BIKE-L1", _BIKE_L1_OPERATIONS),
        ("ML-DSA-44", _ML_DSA_44_OPERATIONS),
    ],
    ids=["kem", "sig"],
)
def test_parses_speed_log_into_cycles_json(tmp_path, algorithm, operations):
    log_name = "{}_output.txt".format(algorithm)
    _write_speed_log(tmp_path, log_name, algorithm, operations)

    result = _run_parser(tmp_path, log_name, algorithm)

    assert result.returncode == 0, result.stderr

    # kem-bench.yml / sig-bench.yml read exactly this file name.
    entries = json.loads((tmp_path / "{}_formatted.json".format(algorithm)).read_text())

    assert [entry["name"] for entry in entries] == \
        ["{} {}".format(algorithm, operation.name) for operation in operations]
    # the "CPU cycles: mean" column, not the iteration or timing columns
    assert [entry["value"] for entry in entries] == [operation.cycles for operation in operations]
    assert [entry["unit"] for entry in entries] == ["cycles"] * len(operations)
    assert [set(entry) for entry in entries] == [{"name", "value", "unit", "extra"}] * len(operations)
    # the build configuration is carried along as extra metadata
    assert "Target platform: x86_64" in entries[0]["extra"]


def test_output_is_reproducible_across_runs(tmp_path):
    log_name = "BIKE-L1_output.txt"
    _write_speed_log(tmp_path, log_name, "BIKE-L1", _BIKE_L1_OPERATIONS)

    assert _run_parser(tmp_path, log_name, "BIKE-L1").returncode == 0
    first = (tmp_path / "BIKE-L1_formatted.json").read_text()

    assert _run_parser(tmp_path, log_name, "BIKE-L1").returncode == 0

    assert (tmp_path / "BIKE-L1_formatted.json").read_text() == first


def test_missing_log_file_fails_and_writes_no_output(tmp_path):
    result = _run_parser(tmp_path, "does-not-exist.txt", "BIKE-L1")

    assert result.returncode != 0
    assert list(tmp_path.glob("*_formatted.json")) == []
