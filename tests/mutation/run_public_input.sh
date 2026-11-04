#!/usr/bin/env bash
# SPDX-License-Identifier: MIT

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
TEMPLATE="${ROOT}/tests/mutation/mull-public-input.yml"

usage() {
	printf 'Usage: %s prepare [build-dir]\n' "$0" >&2
	printf '       %s run [build-dir] [report-dir]\n' "$0" >&2
}

absolute_dir() {
	mkdir -p "$1"
	(cd "$1" && pwd)
}

render_config() {
	local output="$1"
	local build_dir="$2"
	shift 2
	python3 - "$TEMPLATE" "$output" "$build_dir" "$ROOT" "$@" <<'PY'
from pathlib import Path
import sys

template_path, output_path, build_dir, root, *include_paths = sys.argv[1:]
template = Path(template_path).read_text(encoding="utf-8")
include_lines = "".join(f'  - "{path}"\n' for path in include_paths)
rendered = (
    template.replace("@BUILD_DIR@", build_dir)
    .replace("@PROJECT_ROOT@", root)
    .replace("@INCLUDE_PATHS@", include_lines.rstrip("\n"))
)
Path(output_path).write_text(rendered, encoding="utf-8")
PY
}

find_runner() {
	if [[ -n "${MULL_RUNNER:-}" ]]; then
		printf '%s\n' "$MULL_RUNNER"
		return
	fi
	if command -v mull-runner-18 >/dev/null 2>&1; then
		command -v mull-runner-18
		return
	fi
	if command -v mull-runner >/dev/null 2>&1; then
		command -v mull-runner
		return
	fi
	printf 'mull-runner-18 was not found; set MULL_RUNNER to its path\n' >&2
	exit 1
}

prepare_build_config() {
	local build_dir
	build_dir="$(absolute_dir "${1:-"${ROOT}/build/mutation"}")"
	render_config \
		"${build_dir}/mull-build.yml" \
		"$build_dir" \
		'.*/src/kem/kem\\.c$' \
		'.*/src/sig/sig\\.c$' \
		'.*/src/sig_stfl/sig_stfl\\.c$'
	printf '%s\n' "${build_dir}/mull-build.yml"
}

run_campaign() {
	local build_dir report_dir runner
	build_dir="$(absolute_dir "${1:-"${ROOT}/build/mutation"}")"
	report_dir="$(absolute_dir "${2:-"${build_dir}/mull-reports"}")"
	runner="$(find_runner)"

	ulimit -n 1024 || true

	local report_names=(
		kem-api-bike
		kem-api-hqc
		kem-api-ntruprime
		kem-api-ntru
		sig-api-mayo
		sig-api-snova
		sig-api-cross
		sig-api-falcon
		sig-api-falcon-padded
		stfl-api-lms-w1
		stfl-api-lms-w8
		stfl-api-xmss
	)
	for report_name in "${report_names[@]}"; do
		rm -f "${report_dir}/${report_name}.json" "${report_dir}/${report_name}.html" "${report_dir}/${report_name}.sqlite"
	done

	render_config "${report_dir}/mull-kem.yml" "$build_dir" '.*/src/kem/kem\\.c$'
	render_config "${report_dir}/mull-sig.yml" "$build_dir" '.*/src/sig/sig\\.c$'
	render_config "${report_dir}/mull-sig-stfl.yml" "$build_dir" '.*/src/sig_stfl/sig_stfl\\.c$'

	run_case() {
		local name="$1"
		local config="$2"
		local binary="$3"
		shift 3
		printf 'Running %s\n' "$name"
		MULL_CONFIG="$config" "$runner" \
			--allow-surviving \
			--no-output \
			--timeout "${MULL_TIMEOUT_MS:-30000}" \
			--workers "${MULL_WORKERS:-4}" \
			--reporters Elements \
			--report-dir "$report_dir" \
			--report-name "$name" \
			"$binary" "$@"
	}

	run_case kem-api-bike "${report_dir}/mull-kem.yml" "${build_dir}/tests/test_kem" BIKE-L1
	run_case kem-api-hqc "${report_dir}/mull-kem.yml" "${build_dir}/tests/test_kem" HQC-1
	run_case kem-api-ntruprime "${report_dir}/mull-kem.yml" "${build_dir}/tests/test_kem" sntrup761
	run_case kem-api-ntru "${report_dir}/mull-kem.yml" "${build_dir}/tests/test_kem" NTRU-HPS-2048-509

	run_case sig-api-mayo "${report_dir}/mull-sig.yml" "${build_dir}/tests/test_sig" MAYO-1
	run_case sig-api-snova "${report_dir}/mull-sig.yml" "${build_dir}/tests/test_sig" SNOVA_25_8_3
	run_case sig-api-cross "${report_dir}/mull-sig.yml" "${build_dir}/tests/test_sig" cross-rsdp-128-small
	run_case sig-api-falcon "${report_dir}/mull-sig.yml" "${build_dir}/tests/test_sig" Falcon-512
	run_case sig-api-falcon-padded "${report_dir}/mull-sig.yml" "${build_dir}/tests/test_sig" Falcon-padded-512

	run_case stfl-api-lms-w1 "${report_dir}/mull-sig-stfl.yml" "${build_dir}/tests/test_sig_stfl" LMS_SHA256_H5_W1 "${ROOT}/tests/KATs/sig_stfl/lms/LMS_SHA256_H5_W1.rsp"
	run_case stfl-api-lms-w8 "${report_dir}/mull-sig-stfl.yml" "${build_dir}/tests/test_sig_stfl" LMS_SHA256_H5_W8 "${ROOT}/tests/KATs/sig_stfl/lms/LMS_SHA256_H5_W8.rsp"
	run_case stfl-api-xmss "${report_dir}/mull-sig-stfl.yml" "${build_dir}/tests/test_sig_stfl" XMSS-SHA2_10_256 "${ROOT}/tests/KATs/sig_stfl/xmss/XMSS-SHA2_10_256.rsp"
}

case "${1:-}" in
prepare)
	shift
	prepare_build_config "$@"
	;;
run)
	shift
	run_campaign "$@"
	;;
*)
	usage
	exit 1
	;;
esac
