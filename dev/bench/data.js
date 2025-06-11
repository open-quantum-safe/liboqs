window.BENCHMARK_DATA = {
  "lastUpdate": 1749649868378,
  "repoUrl": "https://github.com/open-quantum-safe/liboqs",
  "entries": {
    "Dilithium2": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649377826,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Dilithium2 keypair",
            "value": 77372,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Dilithium2 sign",
            "value": 194340,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Dilithium2 verify",
            "value": 75571,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Dilithium3": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649400340,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Dilithium3 keypair",
            "value": 132926,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Dilithium3 sign",
            "value": 310073,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Dilithium3 verify",
            "value": 126634,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "BIKE-L1": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649417065,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "BIKE-L1 keygen",
            "value": 598083,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "BIKE-L1 encaps",
            "value": 105389,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "BIKE-L1 decaps",
            "value": 1646402,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Dilithium5": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649426239,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Dilithium5 keypair",
            "value": 210884,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Dilithium5 sign",
            "value": 385079,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Dilithium5 verify",
            "value": 202327,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "BIKE-L3": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649447554,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "BIKE-L3 keygen",
            "value": 1682613,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "BIKE-L3 encaps",
            "value": 237493,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "BIKE-L3 decaps",
            "value": 4891413,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "ML-DSA-44": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649452396,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "ML-DSA-44 keypair",
            "value": 76603,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-DSA-44 sign",
            "value": 191897,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-DSA-44 verify",
            "value": 73884,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "BIKE-L5": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649474732,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "BIKE-L5 keygen",
            "value": 4428906,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "BIKE-L5 encaps",
            "value": 488786,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "BIKE-L5 decaps",
            "value": 11913393,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "ML-DSA-65": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649480884,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "ML-DSA-65 keypair",
            "value": 133076,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-DSA-65 sign",
            "value": 307058,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-DSA-65 verify",
            "value": 126503,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Classic-McEliece-348864": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649502183,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-348864 keygen",
            "value": 99284756,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-348864 encaps",
            "value": 35137,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-348864 decaps",
            "value": 94564,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "ML-DSA-87": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649504327,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "ML-DSA-87 keypair",
            "value": 212289,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-DSA-87 sign",
            "value": 387276,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-DSA-87 verify",
            "value": 205182,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Falcon-512": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649527521,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Falcon-512 keypair",
            "value": 17167190,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Falcon-512 sign",
            "value": 566790,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Falcon-512 verify",
            "value": 104788,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Classic-McEliece-348864f": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649528912,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-348864f keygen",
            "value": 80939530,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-348864f encaps",
            "value": 36303,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-348864f decaps",
            "value": 94031,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Falcon-1024": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649554634,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Falcon-1024 keypair",
            "value": 49795438,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Falcon-1024 sign",
            "value": 1172782,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Falcon-1024 verify",
            "value": 210411,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Classic-McEliece-460896": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649554975,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-460896 keygen",
            "value": 282261811,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-460896 encaps",
            "value": 79307,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-460896 decaps",
            "value": 227783,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Falcon-padded-512": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649578082,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Falcon-padded-512 keypair",
            "value": 16687149,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Falcon-padded-512 sign",
            "value": 574097,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Falcon-padded-512 verify",
            "value": 110394,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Classic-McEliece-460896f": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649582363,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-460896f keygen",
            "value": 231099047,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-460896f encaps",
            "value": 79377,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-460896f decaps",
            "value": 224760,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Falcon-padded-1024": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649606300,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Falcon-padded-1024 keypair",
            "value": 50548805,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Falcon-padded-1024 sign",
            "value": 1160699,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Falcon-padded-1024 verify",
            "value": 205976,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Classic-McEliece-6688128": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649615937,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-6688128 keygen",
            "value": 438702474,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-6688128 encaps",
            "value": 140883,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-6688128 decaps",
            "value": 256859,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "SPHINCS+-SHA2-128f-simple": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649633777,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHA2-128f-simple keypair",
            "value": 906257,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-128f-simple sign",
            "value": 21354507,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-128f-simple verify",
            "value": 1621782,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Classic-McEliece-6688128f": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649651016,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-6688128f keygen",
            "value": 302257963,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-6688128f encaps",
            "value": 146166,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-6688128f decaps",
            "value": 255855,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "SPHINCS+-SHA2-128s-simple": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649670689,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHA2-128s-simple keypair",
            "value": 58058882,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-128s-simple sign",
            "value": 439345894,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-128s-simple verify",
            "value": 595694,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Classic-McEliece-6960119": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649676859,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-6960119 keygen",
            "value": 349632216,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-6960119 encaps",
            "value": 139564,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-6960119 decaps",
            "value": 247533,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "SPHINCS+-SHA2-192f-simple": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649700549,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHA2-192f-simple keypair",
            "value": 1342755,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-192f-simple sign",
            "value": 35852968,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-192f-simple verify",
            "value": 2473530,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Classic-McEliece-6960119f": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649703210,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-6960119f keygen",
            "value": 289130945,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-6960119f encaps",
            "value": 144092,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-6960119f decaps",
            "value": 245490,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Classic-McEliece-8192128": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649727237,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-8192128 keygen",
            "value": 534940002,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-8192128 encaps",
            "value": 150310,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-8192128 decaps",
            "value": 254419,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "SPHINCS+-SHA2-192s-simple": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649727331,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHA2-192s-simple keypair",
            "value": 85934563,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-192s-simple sign",
            "value": 812515140,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-192s-simple verify",
            "value": 999942,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "SPHINCS+-SHA2-256f-simple": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649752772,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHA2-256f-simple keypair",
            "value": 3620052,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-256f-simple sign",
            "value": 75164744,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-256f-simple verify",
            "value": 2612323,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Classic-McEliece-8192128f": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649761042,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-8192128f keygen",
            "value": 316158306,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-8192128f encaps",
            "value": 154095,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-8192128f decaps",
            "value": 257356,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "SPHINCS+-SHA2-256s-simple": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649777024,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHA2-256s-simple keypair",
            "value": 56724518,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-256s-simple sign",
            "value": 726137272,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-256s-simple verify",
            "value": 1392646,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Kyber512": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649789131,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Kyber512 keygen",
            "value": 24793,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Kyber512 encaps",
            "value": 31242,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Kyber512 decaps",
            "value": 22180,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "SPHINCS+-SHAKE-128f-simple": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649807219,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHAKE-128f-simple keypair",
            "value": 1898219,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-128f-simple sign",
            "value": 44429348,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-128f-simple verify",
            "value": 3037554,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Kyber768": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649814461,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Kyber768 keygen",
            "value": 39130,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Kyber768 encaps",
            "value": 46827,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Kyber768 decaps",
            "value": 35204,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "SPHINCS+-SHAKE-128s-simple": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649832194,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHAKE-128s-simple keypair",
            "value": 123519089,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-128s-simple sign",
            "value": 933011798,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-128s-simple verify",
            "value": 1125840,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "Kyber1024": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649841976,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Kyber1024 keygen",
            "value": 52654,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Kyber1024 encaps",
            "value": 64133,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Kyber1024 decaps",
            "value": 50010,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "SPHINCS+-SHAKE-192f-simple": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649861939,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHAKE-192f-simple keypair",
            "value": 2832512,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-192f-simple sign",
            "value": 72443570,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-192f-simple verify",
            "value": 4520921,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "ML-KEM-512": [
      {
        "commit": {
          "author": {
            "email": "pablogf@uma.es",
            "name": "Pablo Gutiérrez",
            "username": "pablo-gf"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "d745d35938d17e666ffa88673523fd4c4399d326",
          "message": "Continuous Benchmarking using Github Actions (#2134)\n\n* Added workflows and script for speed beanchmarking\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* changed branch push to main\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Added SPDX-License-Identifer\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\n\n* Fixed github security warnings\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixed github security warnings 2\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n* Fixes after commit-to-main tests\n\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>\n\n---------\n\nSigned-off-by: Pablo Gutiérrez Félix <pablogf@uma.es>\nSigned-off-by: Pablo Gutiérrez <pablogf@uma.es>",
          "timestamp": "2025-06-11T09:38:44-04:00",
          "tree_id": "ae802000b1122b27e2dab87b7426b0392e52ad76",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/d745d35938d17e666ffa88673523fd4c4399d326"
        },
        "date": 1749649867538,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "ML-KEM-512 keygen",
            "value": 23281,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-KEM-512 encaps",
            "value": 25365,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-KEM-512 decaps",
            "value": 29413,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ]
  }
}