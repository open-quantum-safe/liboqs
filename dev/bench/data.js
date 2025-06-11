window.BENCHMARK_DATA = {
  "lastUpdate": 1749649427435,
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
    ]
  }
}