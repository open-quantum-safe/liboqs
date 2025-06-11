window.BENCHMARK_DATA = {
  "lastUpdate": 1749649635182,
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
    ]
  }
}