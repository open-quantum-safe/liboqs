#!/bin/bash

# build_liboqs.sh - Build script for liboqs with OS detection and dependency installation
# Supports runtime CMake configuration options from CONFIGURE.md

set -e  # Exit on error

# Color codes for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to display usage information
usage() {
    cat << EOF
Usage: $0 [OPTIONS]

Build liboqs with optional CMake configuration options.

DEFAULT SETTINGS (when no options specified):
    - Build type: Release
    - Library type: Static (.a)
    - Algorithm set: All (OQS_ALGS_ENABLED=All)
    - Distribution build: ON (OQS_DIST_BUILD=ON)
    - OpenSSL: ON (OQS_USE_OPENSSL=ON)
    - Optimization target: auto (OQS_OPT_TARGET=auto)
    - Tests and docs: Built (OQS_BUILD_ONLY_LIB=OFF)

OPTIONS:
    -h, --help                          Show this help message
    
    Build Configuration:
    --shared                            Build shared library (BUILD_SHARED_LIBS=ON)
    --build-type TYPE                   Set build type: Debug, Release, MinSizeRel, RelWithDebInfo (default: Release)
    --install-prefix PATH               Set installation prefix (CMAKE_INSTALL_PREFIX)
    
    Algorithm Selection:
    --algs-enabled SET                  Algorithm set: STD, NIST_R4, NIST_SIG_ONRAMP, All (default: All)
    --minimal-build "ALG1;ALG2;..."     Build ONLY specified algorithms (disables all others)
                                        Supports multiple semicolon-separated algorithms
                                        Works for KEM, SIG, and SIG_STFL families
                                        (e.g., "KEM_ml_kem_768;SIG_ml_dsa_44;SIG_falcon_512")
                                        IMPORTANT: Must be quoted to prevent shell from treating ; as command separator
    --enable-kem-ALG                    Enable specific KEM algorithm (additive with defaults)
    --enable-sig-ALG                    Enable specific signature algorithm (additive with defaults)
    --enable-sig-stfl-ALG               Enable specific stateful signature algorithm (additive with defaults)
    
    Build Options:
    --build-only-lib                    Build only library, exclude tests and docs (OQS_BUILD_ONLY_LIB=ON)
    --dist-build                        Build for distribution (OQS_DIST_BUILD=ON, default)
    --no-dist-build                     Build for single machine (OQS_DIST_BUILD=OFF)
    --opt-target TARGET                 Optimization target: auto, generic, or specific CPU (default: auto)
    
    OpenSSL Options:
    --use-openssl                       Use OpenSSL (OQS_USE_OPENSSL=ON, default)
    --no-openssl                        Don't use OpenSSL (OQS_USE_OPENSSL=OFF)
    --openssl-root PATH                 OpenSSL root directory (OPENSSL_ROOT_DIR)
    --dlopen-openssl                    Dynamically load OpenSSL (OQS_DLOPEN_OPENSSL=ON)
    
    GPU Acceleration:
    --use-cupqc                         Use NVIDIA cuPQC library (OQS_USE_CUPQC=ON)
    --use-icicle                        Use ICICLE GPU acceleration (OQS_USE_ICICLE=ON)
    
    CPU Features (for non-dist builds):
    --use-adx                           Use ADX instructions (OQS_USE_ADX_INSTRUCTIONS=ON)
    --use-aes                           Use AES instructions (OQS_USE_AES_INSTRUCTIONS=ON)
    --use-avx                           Use AVX instructions (OQS_USE_AVX_INSTRUCTIONS=ON)
    --use-avx2                          Use AVX2 instructions (OQS_USE_AVX2_INSTRUCTIONS=ON)
    --use-avx512                        Use AVX512 instructions (OQS_USE_AVX512_INSTRUCTIONS=ON)
    
    Advanced Options:
    --embedded-build                    Build for embedded systems (OQS_EMBEDDED_BUILD=ON)
    --memopt-build                      Use memory-optimized implementations (OQS_MEMOPT_BUILD=ON)
    --libjade-build                     Use Libjade implementations (OQS_LIBJADE_BUILD=ON)
    --permit-unsupported-arch           Permit compilation on unsupported architecture (OQS_PERMIT_UNSUPPORTED_ARCHITECTURE=ON)
    --strict-warnings                   Enable strict compiler warnings (OQS_STRICT_WARNINGS=ON)
    --enable-constant-time-test         Enable constant-time testing (OQS_ENABLE_TEST_CONSTANT_TIME=ON)
    --use-coverage                      Enable code coverage (USE_COVERAGE=ON)
    --use-sanitizer TYPE                Enable sanitizer: Address, Memory, Undefined, Thread, Leak
    
    Stateful Signatures:
    --enable-xmss                       Enable XMSS stateful signatures (OQS_ENABLE_SIG_STFL_XMSS=ON)
    --enable-lms                        Enable LMS stateful signatures (OQS_ENABLE_SIG_STFL_LMS=ON)
    
    Note: Stateful key/signature generation is HAZARDOUS and disabled by default.
          To enable (NOT RECOMMENDED), use: -DOQS_HAZARDOUS_EXPERIMENTAL_ENABLE_SIG_STFL_KEY_SIG_GEN=ON
          See CONFIGURE.md for security implications before enabling.
    
    Custom CMake Options:
    -D KEY=VALUE                        Pass custom CMake option directly

EXAMPLES:
    # Basic build with defaults
    $0
    
    # Build shared library with debug symbols
    $0 --shared --build-type Debug
    
    # Minimal build with only ML-KEM-768 and ML-DSA-44
    $0 --minimal-build "KEM_ml_kem_768;SIG_ml_dsa_44"
    
    # Build for distribution with OpenSSL
    $0 --dist-build --use-openssl
    
    # Build with GPU acceleration
    $0 --use-icicle
    
    # Build with custom options
    $0 --build-type Release --strict-warnings -DOQS_USE_AVX2_INSTRUCTIONS=ON

For more details, see CONFIGURE.md

EOF
    exit 0
}

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Function to check if script is outdated by validating CMake options
check_script_staleness() {
    local SCRIPT_FILE="${BASH_SOURCE[0]}"
    local CMAKE_FILE="${SCRIPT_DIR}/CMakeLists.txt"
    local ALG_SUPPORT_FILE="${SCRIPT_DIR}/.CMake/alg_support.cmake"
    local WARNED=0
    local MISSING_OPTIONS=()
    
    # Skip check if CMake files don't exist
    if [ ! -f "$CMAKE_FILE" ] && [ ! -f "$ALG_SUPPORT_FILE" ]; then
        return 0
    fi
    
    # Extract all option() declarations from CMake files
    local CMAKE_OPTIONS=()
    
    # Parse main CMakeLists.txt
    if [ -f "$CMAKE_FILE" ]; then
        while IFS= read -r line; do
            [ -n "$line" ] && CMAKE_OPTIONS+=("$line")
        done < <(grep -E '^option\(OQS_|^option\(USE_|^option\(BUILD_' "$CMAKE_FILE" 2>/dev/null | sed 's/option(//;s/ .*//' || true)
    fi
    
    # Parse .CMake/alg_support.cmake for algorithm options
    if [ -f "$ALG_SUPPORT_FILE" ]; then
        while IFS= read -r line; do
            [ -n "$line" ] && CMAKE_OPTIONS+=("$line")
        done < <(grep -E '^option\(OQS_' "$ALG_SUPPORT_FILE" 2>/dev/null | sed 's/option(//;s/ .*//' || true)
    fi
    
    # Remove duplicates (handle empty array safely)
    if [ ${#CMAKE_OPTIONS[@]} -gt 0 ]; then
        CMAKE_OPTIONS=($(printf '%s\n' "${CMAKE_OPTIONS[@]}" | sort -u || true))
    fi
    
    # Check each CMake option against the script
    for opt in "${CMAKE_OPTIONS[@]}"; do
        # Skip empty lines
        [ -z "$opt" ] && continue
        
        # Skip algorithm-specific options that are covered by generic patterns
        # (e.g., OQS_ENABLE_KEM_KYBER is covered by --enable-kem-* pattern)
        if [[ "$opt" =~ ^OQS_ENABLE_KEM_ ]] || [[ "$opt" =~ ^OQS_ENABLE_SIG_ ]] || [[ "$opt" =~ ^OQS_ENABLE_LIBJADE_ ]]; then
            continue
        fi
        
        # Convert option name to script format (e.g., OQS_USE_OPENSSL -> --use-openssl or -DOQS_USE_OPENSSL)
        # Check if option is referenced in the script (either as flag or -D option)
        if ! grep -q "$opt" "$SCRIPT_FILE" 2>/dev/null; then
            MISSING_OPTIONS+=("$opt")
        fi
    done
    
    # Report missing options
    if [ ${#MISSING_OPTIONS[@]} -gt 0 ]; then
        echo -e "${YELLOW}⚠️  Warning: Found CMake options not exposed in this build script:${NC}"
        
        # Group and display missing options (limit to first 10 to avoid clutter)
        local count=0
        for opt in "${MISSING_OPTIONS[@]}"; do
            if [ $count -lt 10 ]; then
                echo -e "${YELLOW}   - $opt${NC}"
                count=$((count+1))
            fi
        done
        
        if [ ${#MISSING_OPTIONS[@]} -gt 10 ]; then
            echo -e "${YELLOW}   ... and $((${#MISSING_OPTIONS[@]} - 10)) more${NC}"
        fi
        
        echo -e "${YELLOW}   You can use these options with: -D<OPTION>=ON/OFF${NC}"
        echo -e "${YELLOW}   Example: $0 -DOQS_SPEED_USE_ARM_PMU=ON${NC}"
        echo ""
        WARNED=1
    fi
    
    # Additional validation: Check for new algorithm families
    if [ -f "$ALG_SUPPORT_FILE" ]; then
        local ALG_FAMILIES=$(grep -E '^option\(OQS_ENABLE_(KEM|SIG)_[A-Z_]+' "$ALG_SUPPORT_FILE" 2>/dev/null | \
                            sed 's/option(OQS_ENABLE_[KS][EI][MG]_//;s/ .*//' | sort -u || true)
        
        local MISSING_ALG_FAMILIES=()
        for alg in $ALG_FAMILIES; do
            # Check if algorithm family is mentioned in help text or options
            if ! grep -qi "$alg" "$SCRIPT_FILE" 2>/dev/null; then
                MISSING_ALG_FAMILIES+=("$alg")
            fi
        done
        
        if [ ${#MISSING_ALG_FAMILIES[@]} -gt 0 ] && [ ${#MISSING_ALG_FAMILIES[@]} -le 5 ]; then
            if [ $WARNED -eq 0 ]; then
                echo -e "${YELLOW}⚠️  Warning: New algorithm families detected:${NC}"
            else
                echo -e "${YELLOW}   New algorithm families detected:${NC}"
            fi
            for alg in "${MISSING_ALG_FAMILIES[@]}"; do
                echo -e "${YELLOW}   - $alg${NC}"
            done
            echo -e "${YELLOW}   Use --enable-kem-<alg> or --enable-sig-<alg> to enable${NC}"
            echo ""
            WARNED=1
        fi
    fi
    
    if [ $WARNED -eq 1 ]; then
        echo -e "${YELLOW}   You can still proceed. Use cmake directly for full control.${NC}"
        echo ""
        
        # Check if staleness check is disabled (useful for CI)
        # Issue warning but allow CI to continue
        if [ "${SKIP_STALENESS_CHECK:-}" = "1" ]; then
            echo -e "${YELLOW}⚠️  Note: Staleness check is disabled (SKIP_STALENESS_CHECK=1)${NC}"
            echo -e "${YELLOW}   The above warnings are informational only and will not fail the build.${NC}"
            echo -e "${YELLOW}   Please review and update the build script if needed.${NC}"
            echo ""
        fi
    fi
}

# Array to store CMake options
CMAKE_OPTIONS=()

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            usage
            ;;
        --shared)
            CMAKE_OPTIONS+=("-DBUILD_SHARED_LIBS=ON")
            shift
            ;;
        --build-type)
            CMAKE_OPTIONS+=("-DCMAKE_BUILD_TYPE=$2")
            shift 2
            ;;
        --install-prefix)
            CMAKE_OPTIONS+=("-DCMAKE_INSTALL_PREFIX=$2")
            shift 2
            ;;
        --algs-enabled)
            CMAKE_OPTIONS+=("-DOQS_ALGS_ENABLED=$2")
            shift 2
            ;;
        --minimal-build)
            CMAKE_OPTIONS+=("-DOQS_MINIMAL_BUILD=$2")
            shift 2
            ;;
        --enable-kem-*)
            ALG="${1#--enable-kem-}"
            ALG_UPPER=$(echo "$ALG" | tr '[:lower:]' '[:upper:]')
            CMAKE_OPTIONS+=("-DOQS_ENABLE_KEM_${ALG_UPPER}=ON")
            shift
            ;;
        --enable-sig-*)
            ALG="${1#--enable-sig-}"
            ALG_UPPER=$(echo "$ALG" | tr '[:lower:]' '[:upper:]')
            CMAKE_OPTIONS+=("-DOQS_ENABLE_SIG_${ALG_UPPER}=ON")
            shift
            ;;
        --enable-sig-stfl-*)
            ALG="${1#--enable-sig-stfl-}"
            ALG_UPPER=$(echo "$ALG" | tr '[:lower:]' '[:upper:]')
            CMAKE_OPTIONS+=("-DOQS_ENABLE_SIG_STFL_${ALG_UPPER}=ON")
            shift
            ;;
        --build-only-lib)
            CMAKE_OPTIONS+=("-DOQS_BUILD_ONLY_LIB=ON")
            shift
            ;;
        --dist-build)
            CMAKE_OPTIONS+=("-DOQS_DIST_BUILD=ON")
            shift
            ;;
        --no-dist-build)
            CMAKE_OPTIONS+=("-DOQS_DIST_BUILD=OFF")
            shift
            ;;
        --opt-target)
            CMAKE_OPTIONS+=("-DOQS_OPT_TARGET=$2")
            shift 2
            ;;
        --use-openssl)
            CMAKE_OPTIONS+=("-DOQS_USE_OPENSSL=ON")
            shift
            ;;
        --no-openssl)
            CMAKE_OPTIONS+=("-DOQS_USE_OPENSSL=OFF")
            shift
            ;;
        --openssl-root)
            CMAKE_OPTIONS+=("-DOPENSSL_ROOT_DIR=$2")
            shift 2
            ;;
        --dlopen-openssl)
            CMAKE_OPTIONS+=("-DOQS_DLOPEN_OPENSSL=ON")
            shift
            ;;
        --use-cupqc)
            CMAKE_OPTIONS+=("-DOQS_USE_CUPQC=ON")
            shift
            ;;
        --use-icicle)
            CMAKE_OPTIONS+=("-DOQS_USE_ICICLE=ON")
            shift
            ;;
        --use-adx)
            CMAKE_OPTIONS+=("-DOQS_USE_ADX_INSTRUCTIONS=ON")
            shift
            ;;
        --use-aes)
            CMAKE_OPTIONS+=("-DOQS_USE_AES_INSTRUCTIONS=ON")
            shift
            ;;
        --use-avx)
            CMAKE_OPTIONS+=("-DOQS_USE_AVX_INSTRUCTIONS=ON")
            shift
            ;;
        --use-avx2)
            CMAKE_OPTIONS+=("-DOQS_USE_AVX2_INSTRUCTIONS=ON")
            shift
            ;;
        --use-avx512)
            CMAKE_OPTIONS+=("-DOQS_USE_AVX512_INSTRUCTIONS=ON")
            shift
            ;;
        --embedded-build)
            CMAKE_OPTIONS+=("-DOQS_EMBEDDED_BUILD=ON")
            shift
            ;;
        --memopt-build)
            CMAKE_OPTIONS+=("-DOQS_MEMOPT_BUILD=ON")
            shift
            ;;
        --libjade-build)
            CMAKE_OPTIONS+=("-DOQS_LIBJADE_BUILD=ON")
            shift
            ;;
        --permit-unsupported-arch)
            CMAKE_OPTIONS+=("-DOQS_PERMIT_UNSUPPORTED_ARCHITECTURE=ON")
            shift
            ;;
        --strict-warnings)
            CMAKE_OPTIONS+=("-DOQS_STRICT_WARNINGS=ON")
            shift
            ;;
        --enable-constant-time-test)
            CMAKE_OPTIONS+=("-DOQS_ENABLE_TEST_CONSTANT_TIME=ON")
            shift
            ;;
        --use-coverage)
            CMAKE_OPTIONS+=("-DUSE_COVERAGE=ON")
            shift
            ;;
        --use-sanitizer)
            CMAKE_OPTIONS+=("-DUSE_SANITIZER=$2")
            shift 2
            ;;
        --enable-xmss)
            CMAKE_OPTIONS+=("-DOQS_ENABLE_SIG_STFL_XMSS=ON")
            shift
            ;;
        --enable-lms)
            CMAKE_OPTIONS+=("-DOQS_ENABLE_SIG_STFL_LMS=ON")
            shift
            ;;
        -D*)
            CMAKE_OPTIONS+=("$1")
            shift
            ;;
        *)
            echo -e "${RED}Error: Unknown option: $1${NC}"
            echo "Use --help for usage information"
            exit 1
            ;;
    esac
done

echo "========================================"
echo "  liboqs Build Script"
echo "========================================"
echo ""

# Check for script staleness only for real build execution paths
check_script_staleness

# No helper functions needed - brew and apt handle idempotency

# Detect OS and install dependencies
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    echo -e "${GREEN}Detected OS: macOS${NC}"
    echo ""
    
    # Check if Homebrew is installed
    if ! command -v brew &> /dev/null; then
        echo -e "${RED}Error: Homebrew is not installed!${NC}"
        echo "Please install Homebrew first: https://brew.sh"
        exit 1
    fi
    
    echo "Installing dependencies..."
    brew install cmake ninja openssl@3 wget doxygen graphviz astyle python3
    
    # Install Python test dependencies using pip3
    echo ""
    echo "Installing Python test dependencies..."
    # Note: Requires pip to be configured to allow system-wide installations
    # If this fails, configure pip with: echo "break-system-packages = true" >> ~/.config/pip/pip.conf
    pip3 install pytest pytest-xdist pyyaml
    
elif [[ -f /etc/os-release ]]; then
    # Source the os-release file
    . /etc/os-release
    
    if [[ "$ID" == "ubuntu" ]] || [[ "$ID" == "debian" ]] || [[ "$ID_LIKE" == *"debian"* ]]; then
        # Ubuntu/Debian - but check if Nix is available first
        if command -v nix &> /dev/null && [ -f "${SCRIPT_DIR}/flake.nix" ] && [ -z "$IN_NIX_SHELL" ]; then
            # Nix is available and flake.nix exists - use Nix environment
            echo -e "${GREEN}Detected OS: $NAME (with Nix available)${NC}"
            echo -e "${YELLOW}Using Nix development environment for reproducible builds...${NC}"
            echo ""
            
            # Re-execute this script inside the Nix development environment
            exec nix develop "${SCRIPT_DIR}" -c "$0" "$@"
        fi
        
        # Standard Ubuntu/Debian path
        echo -e "${GREEN}Detected OS: $NAME${NC}"
        echo ""
        
        echo "Installing dependencies..."
        echo -e "${YELLOW}Note: This will use 'sudo' and may prompt for your password.${NC}"
        sudo apt update
        sudo apt install -y astyle cmake gcc ninja-build libssl-dev python3-pytest python3-pytest-xdist unzip xsltproc doxygen graphviz python3-yaml valgrind
        
    elif [[ "$ID" == "nixos" ]]; then
        # NixOS
        echo -e "${GREEN}Detected OS: NixOS${NC}"
        echo ""
        
        # Check if already in a Nix development environment
        if [ -n "$IN_NIX_SHELL" ]; then
            echo "✓ Already in Nix development environment"
        else
            # Check if flake.nix exists
            if [ ! -f "${SCRIPT_DIR}/flake.nix" ]; then
                echo -e "${RED}Error: flake.nix not found in ${SCRIPT_DIR}${NC}"
                echo "Cannot automatically enter Nix development environment."
                exit 1
            fi
            
            echo -e "${YELLOW}Not in Nix development environment. Re-executing with 'nix develop'...${NC}"
            echo ""
            
            # Re-execute this script inside the Nix development environment
            # Pass all original arguments to the re-executed script
            exec nix develop "${SCRIPT_DIR}" -c "$0" "$@"
        fi
        
    else
        echo -e "${YELLOW}Warning: Unsupported Linux distribution: $NAME${NC}"
        echo "Please install dependencies manually."
        exit 1
    fi
    
else
    echo -e "${RED}Error: Unable to detect operating system${NC}"
    echo "Supported OS: macOS, Ubuntu, Debian, NixOS"
    exit 1
fi

echo -e "${GREEN}Dependencies installed successfully!${NC}"
mkdir -p "${SCRIPT_DIR}/build"
cd "${SCRIPT_DIR}/build"

echo ""
echo "========================================"
echo "  Building liboqs"
echo "========================================"

# Display CMake options if any were provided
if [ ${#CMAKE_OPTIONS[@]} -gt 0 ]; then
    echo -e "${BLUE}CMake options:${NC}"
    for opt in "${CMAKE_OPTIONS[@]}"; do
        echo "  $opt"
    done
    echo ""
fi

# Run CMake with all options
echo "Running CMake configuration..."
cmake -GNinja "${CMAKE_OPTIONS[@]}" ..

echo ""
echo "Building with Ninja..."
ninja

echo ""
echo "========================================"
echo "  Build Complete!"
echo "========================================"
echo -e "${GREEN}✓${NC} Build completed successfully in: ${SCRIPT_DIR}/build"
echo ""
echo "To install liboqs, run:"
echo "  cd ${SCRIPT_DIR}/build && sudo ninja install"
echo ""
echo "To run tests, run:"
echo "  cd ${SCRIPT_DIR}/build && ninja run_tests"
