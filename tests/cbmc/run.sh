#!/usr/bin/env sh
# SPDX-License-Identifier: MIT

set -eu

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
cd "$ROOT_DIR"

RAW_DIR="${1:-tests/cbmc/raw-runs}"
MODEL_DIR="tests/cbmc/models"
HARNESS_DIR="tests/cbmc/harnesses"

mkdir -p "$RAW_DIR"

if ! command -v cbmc >/dev/null 2>&1; then
	echo "cbmc not found" >&2
	exit 1
fi

run_proof() {
	id=$1
	shift
	raw="$RAW_DIR/$id.txt"
	coverage="$RAW_DIR/$id.coverage.txt"

	echo "Running $id"
	if cbmc \
		-I "$MODEL_DIR" \
		--drop-unused-functions \
		--bounds-check \
		--pointer-check \
		--pointer-overflow-check \
		--div-by-zero-check \
		--signed-overflow-check \
		--conversion-check \
		--unwind 5 \
		--unwinding-assertions \
		--trace \
		"$@" >"$raw" 2>&1; then
		echo "$id: PROVED_BOUNDED"
	else
		echo "$id: FAILED"
		cat "$raw"
		return 1
	fi

	if cbmc \
		-I "$MODEL_DIR" \
		--drop-unused-functions \
		--cover assertion \
		--unwind 5 \
		"$@" >"$coverage" 2>&1; then
		echo "$id coverage: captured"
	else
		echo "$id coverage: FAILED"
		cat "$coverage"
		return 1
	fi
}

run_proof mem_secure_bcmp \
	"src/common/common.c" \
	"$HARNESS_DIR/mem_secure_bcmp_harness.c"

run_proof sig_status_normalization \
	"src/sig/sig.c" \
	"$MODEL_DIR/oqs_memory.c" \
	"$HARNESS_DIR/sig_status_normalization_harness.c"

run_proof sig_stfl_dispatch_contract \
	"src/sig_stfl/sig_stfl.c" \
	"$MODEL_DIR/oqs_memory.c" \
	"$HARNESS_DIR/sig_stfl_dispatch_contract_harness.c"

run_proof sig_stfl_lock_contract \
	"src/sig_stfl/sig_stfl.c" \
	"$MODEL_DIR/oqs_memory.c" \
	"$HARNESS_DIR/sig_stfl_lock_contract_harness.c"
