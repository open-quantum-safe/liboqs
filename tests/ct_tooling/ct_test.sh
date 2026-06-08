#!/bin/bash
# SPDX-License-Identifier: MIT

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
LIBOQS_DIR="$(realpath "$SCRIPT_DIR/../..")"

notify() {
    printf '[%s] %s\n' "$(date '+%H:%M:%S')" "$*"
}

build() {
    local TOOL=$1
    local COMP_V=$2
    local LIBOQS_BUILD=$3
    local OPT_FLAG=$4
    local BUILD_DIR=$5
    local INPUT=$6

    # Remove build dir if OQS_MINIMAL_BUILD or input changed
    if [ -f "$BUILD_DIR/CMakeCache.txt" ]; then
        grep -q "OQS_MINIMAL_BUILD" "$BUILD_DIR/CMakeCache.txt" && rm -rf "$BUILD_DIR"
    fi

    # Handle a minimal liboqs build only if a single algorithm is passed as input, otherwise build the complete library
    local MINIMAL_BUILD_ARG=()
    if [[ -n "$INPUT" && "$INPUT" != "all" && "$INPUT" != "kems" && "$INPUT" != "sigs" ]]; then
        MINIMAL_BUILD_ARG=(-DOQS_MINIMAL_BUILD="$INPUT")
    fi

    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    local CMAKE_ARGS=("-S .." "-G Ninja" "${MINIMAL_BUILD_ARG[@]}" "-DCMAKE_C_COMPILER=$COMP_V" "-DOQS_OPT_TARGET=$LIBOQS_BUILD" "-DCMAKE_BUILD_TYPE=Debug" "-DOQS_USE_OPENSSL=OFF" "-DOQS_DIST_BUILD=OFF")

    case "$TOOL" in
        valgrind-varlat)

            cmake "${CMAKE_ARGS[@]}" \
                -DCMAKE_C_FLAGS="$OPT_FLAG" \
                -DOQS_ENABLE_TEST_CONSTANT_TIME=ON > /dev/null 2>&1
            cmake --build . -j$(nproc) > /dev/null 2>&1
            ;;
        memsan)

            # Generate suppression flags for all suppression files containing false positives
            SUP_DIR="$SCRIPT_DIR/tools/memsan/false_positives"
            SUP_FLAGS=()
            for f in "$SUP_DIR"/*.txt; do
                [ -f "$f" ] || continue
                SUP_FLAGS+=( "-fsanitize-ignorelist=$f" )
            done

            cmake "${CMAKE_ARGS[@]}" \
                -DBUILD_SHARED_LIBS=ON \
                -DOQS_ENABLE_TEST_CONSTANT_TIME_MEMSAN=ON \
                -DCMAKE_C_FLAGS="-fsanitize=memory -fsanitize-recover=all ${SUP_FLAGS[*]} $OPT_FLAG -g" \
                -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=memory" \
                -DCMAKE_SHARED_LINKER_FLAGS="-fsanitize=memory" > /dev/null 2>&1
            cmake --build . -j$(nproc) > /dev/null 2>&1
            ;;
        *)
            echo "Unknown tool: $TOOL"; return 1
            ;;
    esac
}

test() {
    local TOOL=$1
    local BUILD_DIR=$2
    local TEST_TYPE=$3
    local COMP_V="${4//-/_}"
    local TARGET=$5
    local ALGORITHM=$6
    local SCRIPT_DIR=$7

    if [[ "$TEST_TYPE" == "kem" ]]; then
        TEST_BINARY="test_kem"
        UPPER_TYPE="KEM"
    elif [[ "$TEST_TYPE" == "sig" ]]; then
        TEST_BINARY="test_sig"
        UPPER_TYPE="SIG"
    fi

    # Skip SPHINCS and SLH-DSA for SIG tests
    if [[ "$ALGORITHM" == *SPHINCS* || "$ALGORITHM" == *SLH_DSA* ]]; then
        echo "Skipping ${UPPER_TYPE} ${ALGORITHM}"
        return 0
    fi

    LOG_DIR="${SCRIPT_DIR}/tools/${TOOL//-/_}/logs/${COMP_V}_${TARGET}"
    mkdir -p "$LOG_DIR"
    CURRENT_RUN_DIR="$LOG_DIR/$SANITIZED_OPT_FLAG"
    mkdir -p "$CURRENT_RUN_DIR"
    OUTPUT_DIR="$CURRENT_RUN_DIR/$TEST_TYPE"
    mkdir -p "$OUTPUT_DIR"
    SUMMARY_FILE="$OUTPUT_DIR/${TEST_TYPE}_summary.txt"

    COMPILER_PATH=$(grep -E '^CMAKE_C_COMPILER:.*=' "$BUILD_DIR"/CMakeCache.txt | head -n1 | cut -d'=' -f2- | tr -d '\r')
    COMPILER_VERSION=$("$COMPILER_PATH" --version 2>&1 | head -n1)
    ARCH="$(uname -m)"
    COMP_FLAGS=$(grep "CMAKE_C_FLAGS:" "$BUILD_DIR/CMakeCache.txt" | cut -d'=' -f2-)
    MAX_WARNINGS=100000

    # Check if this is first algorithm (header only once)
    if [[ ! -s "$SUMMARY_FILE" ]]; then
        {
            echo "========================================"
            echo "Compiled with: $COMP_FLAGS"
            echo "Compiler version: ${COMPILER_VERSION}"
            echo "Architecture: ${ARCH}"
            echo "========================================"
            echo ""
        } > "$SUMMARY_FILE"
    else
        echo "" >> "$SUMMARY_FILE"
    fi

    PASS_COUNT=0
    FAIL_COUNT=0
    LOG_FILE="$OUTPUT_DIR/${ALGORITHM}_${TIMESTAMP}.log"
    echo -n "Testing $ALGORITHM ... " | tee -a "$SUMMARY_FILE"

    cd "$LIBOQS_DIR"

    # Execute CT tests based on the tool selected
    case "$TOOL" in
        valgrind-varlat)
            # Generate suppression flags for all suppression files containing false positives
            SUP_DIR="$SCRIPT_DIR/tools/valgrind_varlat/false_positives"
            SUP_FLAGS=()
            for f in "$SUP_DIR"/*.txt; do
                [ -f "$f" ] || continue
                SUP_FLAGS+=( "--suppressions=$f" )
            done

            VALGRIND_OPTS=(
                valgrind_varlat
                --tool=memcheck
                --gen-suppressions=all
                "${SUP_FLAGS[@]}" # Include all suppression files
                --error-exitcode=123
                --max-stackframe=20480000
                --num-callers=20
                --variable-latency-errors=yes # Enable the KyberSlash patch
            )
    
            : > "$LOG_FILE" ; : > "$LOG_FILE.hashes"; : > "$LOG_FILE.count"

            "${VALGRIND_OPTS[@]}" "$BUILD_DIR"/tests/$TEST_BINARY "$ALGORITHM" 2>&1 | awk \
                -v log_file="$LOG_FILE" \
                -v tmp_file="$LOG_FILE.tmp" \
                -v hash_file="$LOG_FILE.hashes" \
                -v count_file="$LOG_FILE.count" \
                -v max_warnings="$MAX_WARNINGS" '
                    # Extract unique suppression blocks from Valgrind output
                    BEGIN {
                        unique_warnings_count = 0;
                        in_block = 0;         # Whether we are inside a { ... } block
                        block = "";           # Current block content (including braces)
                        suppress = 0;         # reached max_warnings

                        # Preload known hashes if present
                        while ((getline line < hash_file) > 0) {
                            gsub(/\r$/, "", line);
                            if (length(line) > 0) {
                                seen[line] = 1;
                            }
                        }
                        close(hash_file);
                    }

                    {
                        if (suppress) {
                            if (in_block) {
                                if ($0 ~ /^\}$/) { in_block = 0 }
                            } else if ($0 ~ /^\{$/) {
                                in_block = 1
                            }
                            next
                        }

                        if (in_block) {
                            block = block $0 "\n";
                                
                            # When } is encountered, it is the end of block: compute hash via tmp file
                            if ($0 ~ /^\}$/) {
                                print block > tmp_file; close(tmp_file);
                                cmd = "sha256sum \"" tmp_file "\"";
                                cmd | getline line; close(cmd);
                                hash = line; sub(/ .*/, "", hash);

                                # If the hash is new, store it in seen[] and increase the count
                                if (!(hash in seen)) {
                                    print block >> log_file; close(log_file);
                                    print "" >> log_file;      # spacer line between blocks
                                    print hash >> hash_file; close(hash_file);
                                    seen[hash] = 1;
                                    unique_warnings_count++;

                                    if (unique_warnings_count >= max_warnings) {
                                        suppress = 1;
                                    }
                                }

                                in_block = 0;
                                block = "";
                            }
                            next
                        }

                        # When { is detected, start a new block
                        if ($0 ~ /^\{$/) {
                            in_block = 1;
                            block = $0 "\n";
                        }
                    }

                    END {
                        print unique_warnings_count > count_file; close(count_file);
                    }
                    '
            EXIT_CODE=${PIPESTATUS[0]}
            ERROR_COUNT=$(cat "$LOG_FILE.count" 2>/dev/null); ERROR_COUNT=${ERROR_COUNT:-0}
            rm -f "$OUTPUT_DIR"/*.hashes "$OUTPUT_DIR"/*log.tmp
            ;;
        
        memsan)
            touch "$LOG_FILE"

            "$BUILD_DIR"/tests/$TEST_BINARY "$ALGORITHM" 2>&1 | awk \
                -v log_file="$LOG_FILE" \
                -v max_warnings="$MAX_WARNINGS" '
                /^SUMMARY: MemorySanitizer:/ {
                    # memcmp/bcmp warnings are known false positives but -fsanitize-ignorelist has no effect because they are not compiled by clang
                    # Skip their warnings here instead.
                    if ($0 ~ / in (memcmp|bcmp)$/) next

                    # Check if this exact SUMMARY was already logged and store it if not
                    cmd = "grep -Fxq \"" $0 "\" " log_file
                    if (system(cmd) != 0) {
                        warnings++
                        print >> log_file
                        fflush(log_file)
                    }

                    if (warnings >= max_warnings) {
                        print warnings > log_file ".count"
                        print "TERMINATED: Exceeded " max_warnings " warnings" >> log_file
                        fflush(log_file)
                        terminated = 1
                        exit 1
                    }
                }
                # Count and store the number of unique warnings obtained and store it
                END {
                    if (!terminated && warnings > 0) {
                        print warnings > log_file ".count"
                    } else if (!terminated) {
                        print 0 > log_file ".count"
                    }
                }
            '
            EXIT_CODE=$?
            ERROR_COUNT=$(cat "$LOG_FILE.count" 2>/dev/null); ERROR_COUNT=${ERROR_COUNT:-0}
            ;;
        *)
            echo "Unknown tool: $TOOL"; return 1
            ;;
    esac

    if [ "$ERROR_COUNT" -eq "$MAX_WARNINGS" ]; then
        echo "FAIL" | tee -a "$SUMMARY_FILE"
        echo "  → Found $ERROR_COUNT warnings (warning cap reached — further warnings suppressed)" \
            | tee -a "$SUMMARY_FILE"
        ((++FAIL_COUNT))

    elif [ "$ERROR_COUNT" -gt 0 ]; then
        echo "FAIL" | tee -a "$SUMMARY_FILE"
        echo "  → Found $ERROR_COUNT warnings" \
            | tee -a "$SUMMARY_FILE"
        ((++FAIL_COUNT))

    elif [ $EXIT_CODE -ne 0 ]; then
        echo "FAIL (Exit code: $EXIT_CODE)" | tee -a "$SUMMARY_FILE"
        ((++FAIL_COUNT))

    else
        echo "PASS" | tee -a "$SUMMARY_FILE"
        ((++PASS_COUNT))

    fi

    rm -f "$OUTPUT_DIR"/*.count
}

get_available_algs() {
    local ALG_TYPE="$1"
    local LIBOQS_DIR="$2"

    (cd "$LIBOQS_DIR" && python3 -c "import sys
sys.path.insert(0, 'tests')
import helpers
for alg in helpers.available_"$ALG_TYPE"s_by_name():
    print(alg)")
}

# Read inputs from arguments
if [ "$#" -lt 4 ]; then
    echo "Usage: $0 <compiler_version> <target> <opt_flags...> <input>"
    echo "Example: $0 clang-20 generic -O2 -fno-tree-vectorize all"
    exit 1
fi

TOOL="$1"
COMPILER="$2"
TARGET="$3"

# The last argument is the input (all/kems/sigs/<specific_variant>)
INPUT="${!#}"

# Collect all optimization flags between position 4 and the last-1 argument
if [ "$#" -gt 4 ]; then
    NUM_OPT_ARGS=$(( $# - 4 ))
    OPT_FLAG="${@:4:$NUM_OPT_ARGS}"
else
    OPT_FLAG=""
fi

SANITIZED_OPT_FLAG=$(printf "%s" "$OPT_FLAG" | tr ' ' '_' | tr -d '-' | tr -c '[:alnum:]_' '_')
if [ -z "$SANITIZED_OPT_FLAG" ]; then
    SANITIZED_OPT_FLAG=default
fi

BUILD_NAME="${TOOL//-/_}_${COMPILER//-/_}_${TARGET}_${SANITIZED_OPT_FLAG}"
BUILD_DIR="$LIBOQS_DIR/build_$BUILD_NAME"

# Export build dir for tests/helpers.py to find generated headers
export OQS_BUILD_DIR="$BUILD_DIR"
KEMS=$(get_available_algs kem "$LIBOQS_DIR")
SIGS=$(get_available_algs sig "$LIBOQS_DIR")

# Convert user-facing algorithm name to CMake OQS_MINIMAL_BUILD key
# Some OQS API names don't match the CMake variable suffix directly (e.g. sntrup761 maps to OQS_ENABLE_KEM_ntruprime_sntrup761, OV-Is to OQS_ENABLE_SIG_uov_ov_Is)
# Detect the type (KEM/SIG), then look up the correct suffix from alg_support.cmake
BUILD_INPUT="$INPUT"
ALG_TYPE=""
echo "$KEMS" | grep -Fxq "$INPUT" && ALG_TYPE="KEM"
echo "$SIGS" | grep -Fxq "$INPUT" && ALG_TYPE="SIG"

if [[ -n "$ALG_TYPE" ]]; then
    NORMALIZED_INPUT="$(printf '%s' "$INPUT" | tr '[:upper:]-' '[:lower:]_')"
    CMAKE_SUFFIX=$(grep -oE "OQS_ENABLE_${ALG_TYPE}_[a-zA-Z0-9_]+" "$LIBOQS_DIR/.CMake/alg_support.cmake" \
        | sed "s/OQS_ENABLE_${ALG_TYPE}_//" \
        | grep -vE "(_avx2|_avx|_aesni|_x86_64|_aarch64|_icicle_cuda|_cuda)$" \
        | grep -iE "_${NORMALIZED_INPUT}$|^${NORMALIZED_INPUT}$" | head -1)
    BUILD_INPUT="${ALG_TYPE}_${CMAKE_SUFFIX:-$NORMALIZED_INPUT}"
fi

# Build liboqs with the specified compilation parameters
notify "Preparing liboqs build (compiler=${COMPILER}, target=${TARGET}, flags=${OPT_FLAG})"
build "$TOOL" "$COMPILER" "$TARGET" "$OPT_FLAG" "$BUILD_DIR" "$BUILD_INPUT"

cd "$LIBOQS_DIR"

TIMESTAMP="$(date '+%Y%m%d_%H%M%S')"
notify "Setting up ${TOOL} CT testing"

RUN_KEMS=()
RUN_SIGS=()

case "$INPUT" in
    all)
        RUN_KEMS=($KEMS)
        RUN_SIGS=($SIGS)
        ;;
    kems)
        RUN_KEMS=($KEMS)
        ;;
    sigs)
        RUN_SIGS=($SIGS)
        ;;
    *)
        if echo "$KEMS" | grep -Fxq "$INPUT"; then
            RUN_KEMS=("$INPUT")
        elif echo "$SIGS" | grep -Fxq "$INPUT"; then
            RUN_SIGS=("$INPUT")
        else
            echo "Enter a valid input: all/kems/sigs/<specific_variant>"
            exit 1
        fi
        ;;
esac

for KEM in "${RUN_KEMS[@]}"; do
    test "$TOOL" "$BUILD_DIR" kem "$COMPILER" "$TARGET" "$KEM" "$SCRIPT_DIR"
done

for SIG in "${RUN_SIGS[@]}"; do
    test "$TOOL" "$BUILD_DIR" sig "$COMPILER" "$TARGET" "$SIG" "$SCRIPT_DIR"
done

notify "Finished ${TOOL} CT testing"
echo ""