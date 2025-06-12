window.BENCHMARK_DATA = {
  "lastUpdate": 1749737926095,
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737281306,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Dilithium2 keypair",
            "value": 76807,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Dilithium2 sign",
            "value": 190527,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Dilithium2 verify",
            "value": 74097,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737316270,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Dilithium3 keypair",
            "value": 133453,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Dilithium3 sign",
            "value": 306922,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Dilithium3 verify",
            "value": 126175,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737290991,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "BIKE-L1 keygen",
            "value": 597992,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "BIKE-L1 encaps",
            "value": 105354,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "BIKE-L1 decaps",
            "value": 1643185,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737343571,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Dilithium5 keypair",
            "value": 213648,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Dilithium5 sign",
            "value": 387189,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Dilithium5 verify",
            "value": 204887,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737323141,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "BIKE-L3 keygen",
            "value": 1824509,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "BIKE-L3 encaps",
            "value": 249347,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "BIKE-L3 decaps",
            "value": 4877824,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737370986,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "ML-DSA-44 keypair",
            "value": 76509,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-DSA-44 sign",
            "value": 192410,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-DSA-44 verify",
            "value": 74132,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737353180,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "BIKE-L5 keygen",
            "value": 4432981,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "BIKE-L5 encaps",
            "value": 491732,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "BIKE-L5 decaps",
            "value": 11882468,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737440297,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "ML-DSA-65 keypair",
            "value": 132925,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-DSA-65 sign",
            "value": 310047,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-DSA-65 verify",
            "value": 126159,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737402332,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-348864 keygen",
            "value": 98613199,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-348864 encaps",
            "value": 35089,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-348864 decaps",
            "value": 93314,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737469792,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "ML-DSA-87 keypair",
            "value": 212908,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-DSA-87 sign",
            "value": 385363,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-DSA-87 verify",
            "value": 202762,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737495705,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Falcon-512 keypair",
            "value": 17032906,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Falcon-512 sign",
            "value": 565413,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Falcon-512 verify",
            "value": 104478,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737434829,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-348864f keygen",
            "value": 80242554,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-348864f encaps",
            "value": 37685,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-348864f decaps",
            "value": 93860,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737520718,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Falcon-1024 keypair",
            "value": 48121224,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Falcon-1024 sign",
            "value": 1161812,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Falcon-1024 verify",
            "value": 210184,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737464424,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-460896 keygen",
            "value": 315766024,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-460896 encaps",
            "value": 79093,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-460896 decaps",
            "value": 227407,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737545477,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Falcon-padded-512 keypair",
            "value": 17146683,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Falcon-padded-512 sign",
            "value": 586691,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Falcon-padded-512 verify",
            "value": 107953,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737494536,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-460896f keygen",
            "value": 233294417,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-460896f encaps",
            "value": 79192,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-460896f decaps",
            "value": 224643,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737570832,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Falcon-padded-1024 keypair",
            "value": 49209041,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Falcon-padded-1024 sign",
            "value": 1153031,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Falcon-padded-1024 verify",
            "value": 207448,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737533918,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-6688128 keygen",
            "value": 445856749,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-6688128 encaps",
            "value": 148921,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-6688128 decaps",
            "value": 256886,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737595085,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHA2-128f-simple keypair",
            "value": 921129,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-128f-simple sign",
            "value": 21293237,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-128f-simple verify",
            "value": 1602775,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737563199,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-6688128f keygen",
            "value": 301103742,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-6688128f encaps",
            "value": 146149,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-6688128f decaps",
            "value": 255191,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737631717,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHA2-128s-simple keypair",
            "value": 57818964,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-128s-simple sign",
            "value": 437712858,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-128s-simple verify",
            "value": 600487,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737598161,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-6960119 keygen",
            "value": 394256780,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-6960119 encaps",
            "value": 141873,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-6960119 decaps",
            "value": 246275,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737667828,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHA2-192f-simple keypair",
            "value": 1334180,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-192f-simple sign",
            "value": 35811185,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-192f-simple verify",
            "value": 2524020,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737627772,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-6960119f keygen",
            "value": 289226217,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-6960119f encaps",
            "value": 142433,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-6960119f decaps",
            "value": 245982,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737653216,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-8192128 keygen",
            "value": 443680295,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-8192128 encaps",
            "value": 145773,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-8192128 decaps",
            "value": 254726,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737694530,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHA2-192s-simple keypair",
            "value": 86593966,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-192s-simple sign",
            "value": 816450631,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-192s-simple verify",
            "value": 1034181,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737720919,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHA2-256f-simple keypair",
            "value": 3625281,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-256f-simple sign",
            "value": 74951789,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-256f-simple verify",
            "value": 2587344,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737687364,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Classic-McEliece-8192128f keygen",
            "value": 315198482,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-8192128f encaps",
            "value": 154106,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Classic-McEliece-8192128f decaps",
            "value": 261903,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737748632,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHA2-256s-simple keypair",
            "value": 57061678,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-256s-simple sign",
            "value": 729074641,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHA2-256s-simple verify",
            "value": 1387540,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737714485,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Kyber512 keygen",
            "value": 24712,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Kyber512 encaps",
            "value": 31024,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Kyber512 decaps",
            "value": 22083,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737774696,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHAKE-128f-simple keypair",
            "value": 1895982,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-128f-simple sign",
            "value": 44435700,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-128f-simple verify",
            "value": 3089162,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737739173,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Kyber768 keygen",
            "value": 38944,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Kyber768 encaps",
            "value": 46944,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Kyber768 decaps",
            "value": 35301,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737806992,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHAKE-128s-simple keypair",
            "value": 124788191,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-128s-simple sign",
            "value": 940540582,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-128s-simple verify",
            "value": 1148849,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737766527,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "Kyber1024 keygen",
            "value": 52847,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Kyber1024 encaps",
            "value": 63960,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "Kyber1024 decaps",
            "value": 49696,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737834869,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHAKE-192f-simple keypair",
            "value": 2817915,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-192f-simple sign",
            "value": 72490436,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-192f-simple verify",
            "value": 4503255,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
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
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737802676,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "ML-KEM-512 keygen",
            "value": 23068,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-KEM-512 encaps",
            "value": 25192,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-KEM-512 decaps",
            "value": 29278,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "SPHINCS+-SHAKE-192s-simple": [
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
        "date": 1749649892068,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHAKE-192s-simple keypair",
            "value": 180354749,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-192s-simple sign",
            "value": 1618689971,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-192s-simple verify",
            "value": 1594146,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737860966,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHAKE-192s-simple keypair",
            "value": 180555221,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-192s-simple sign",
            "value": 1617176301,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-192s-simple verify",
            "value": 1577509,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "ML-KEM-768": [
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
        "date": 1749649894418,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "ML-KEM-768 keygen",
            "value": 38705,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-KEM-768 encaps",
            "value": 39063,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-KEM-768 decaps",
            "value": 46358,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737832899,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "ML-KEM-768 keygen",
            "value": 38200,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-KEM-768 encaps",
            "value": 39341,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-KEM-768 decaps",
            "value": 46427,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "SPHINCS+-SHAKE-256f-simple": [
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
        "date": 1749649918498,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHAKE-256f-simple keypair",
            "value": 7185300,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-256f-simple sign",
            "value": 143983806,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-256f-simple verify",
            "value": 4327265,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737896324,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHAKE-256f-simple keypair",
            "value": 7258841,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-256f-simple sign",
            "value": 143760796,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-256f-simple verify",
            "value": 4361905,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "ML-KEM-1024": [
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
        "date": 1749649922745,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "ML-KEM-1024 keygen",
            "value": 52225,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-KEM-1024 encaps",
            "value": 53985,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-KEM-1024 decaps",
            "value": 64212,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737860793,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "ML-KEM-1024 keygen",
            "value": 53156,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-KEM-1024 encaps",
            "value": 54205,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "ML-KEM-1024 decaps",
            "value": 64520,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "SPHINCS+-SHAKE-256s-simple": [
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
        "date": 1749649946541,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHAKE-256s-simple keypair",
            "value": 114794682,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-256s-simple sign",
            "value": 1365745234,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-256s-simple verify",
            "value": 2200442,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737925459,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "SPHINCS+-SHAKE-256s-simple keypair",
            "value": 115015924,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-256s-simple sign",
            "value": 1360850470,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "SPHINCS+-SHAKE-256s-simple verify",
            "value": 2258636,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "sntrup761": [
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
        "date": 1749649953366,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "sntrup761 keygen",
            "value": 677566,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "sntrup761 encaps",
            "value": 29736,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "sntrup761 decaps",
            "value": 32610,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737887911,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "sntrup761 keygen",
            "value": 678796,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "sntrup761 encaps",
            "value": 29617,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "sntrup761 decaps",
            "value": 32462,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "MAYO-1": [
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
        "date": 1749649973407,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "MAYO-1 keypair",
            "value": 169797,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "MAYO-1 sign",
            "value": 507281,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "MAYO-1 verify",
            "value": 188420,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "FrodoKEM-640-AES": [
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
        "date": 1749649977091,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "FrodoKEM-640-AES keygen",
            "value": 728099,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "FrodoKEM-640-AES encaps",
            "value": 1019478,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "FrodoKEM-640-AES decaps",
            "value": 968029,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      },
      {
        "commit": {
          "author": {
            "email": "nathaniel.brough@gmail.com",
            "name": "Nathaniel Brough",
            "username": "nathaniel-brough"
          },
          "committer": {
            "email": "noreply@github.com",
            "name": "GitHub",
            "username": "web-flow"
          },
          "distinct": true,
          "id": "8d9cfd7f33080b2463e145218ec0db47048a2d81",
          "message": "test: Add basic kem fuzz testing (#2133)\n\nSigned-off-by: Nathaniel Brough <nathaniel.brough@gmail.com>",
          "timestamp": "2025-06-12T10:03:59-04:00",
          "tree_id": "54866eac9cd9fb1ce39a49f45e5926f2b1c6fb0c",
          "url": "https://github.com/open-quantum-safe/liboqs/commit/8d9cfd7f33080b2463e145218ec0db47048a2d81"
        },
        "date": 1749737912528,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "FrodoKEM-640-AES keygen",
            "value": 727371,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "FrodoKEM-640-AES encaps",
            "value": 1015847,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "FrodoKEM-640-AES decaps",
            "value": 962361,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: 8d9cfd7f33080b2463e145218ec0db47048a2d81 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "MAYO-2": [
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
        "date": 1749650001363,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "MAYO-2 keypair",
            "value": 93543,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "MAYO-2 sign",
            "value": 253528,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "MAYO-2 verify",
            "value": 49565,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "FrodoKEM-640-SHAKE": [
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
        "date": 1749650004074,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "FrodoKEM-640-SHAKE keygen",
            "value": 2546569,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "FrodoKEM-640-SHAKE encaps",
            "value": 2776416,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "FrodoKEM-640-SHAKE decaps",
            "value": 2710653,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "MAYO-3": [
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
        "date": 1749650028389,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "MAYO-3 keypair",
            "value": 376133,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "MAYO-3 sign",
            "value": 953595,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "MAYO-3 verify",
            "value": 404435,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "FrodoKEM-976-AES": [
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
        "date": 1749650030009,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "FrodoKEM-976-AES keygen",
            "value": 1410346,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "FrodoKEM-976-AES encaps",
            "value": 1918341,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "FrodoKEM-976-AES decaps",
            "value": 1806568,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "MAYO-5": [
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
        "date": 1749650055284,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "MAYO-5 keypair",
            "value": 851873,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "MAYO-5 sign",
            "value": 2388062,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "MAYO-5 verify",
            "value": 921696,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "FrodoKEM-976-SHAKE": [
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
        "date": 1749650055555,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "FrodoKEM-976-SHAKE keygen",
            "value": 5484674,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "FrodoKEM-976-SHAKE encaps",
            "value": 5832840,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "FrodoKEM-976-SHAKE decaps",
            "value": 5710626,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdp-128-balanced": [
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
        "date": 1749650078801,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdp-128-balanced keypair",
            "value": 46087,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-128-balanced sign",
            "value": 1949292,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-128-balanced verify",
            "value": 1314909,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "FrodoKEM-1344-AES": [
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
        "date": 1749650080047,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "FrodoKEM-1344-AES keygen",
            "value": 2391495,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "FrodoKEM-1344-AES encaps",
            "value": 3275410,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "FrodoKEM-1344-AES decaps",
            "value": 3114493,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdp-128-fast": [
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
        "date": 1749650103973,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdp-128-fast keypair",
            "value": 46131,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-128-fast sign",
            "value": 1103504,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-128-fast verify",
            "value": 665180,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "FrodoKEM-1344-SHAKE": [
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
        "date": 1749650105225,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "FrodoKEM-1344-SHAKE keygen",
            "value": 9749662,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "FrodoKEM-1344-SHAKE encaps",
            "value": 10284447,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "FrodoKEM-1344-SHAKE decaps",
            "value": 10192592,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdp-128-small": [
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
        "date": 1749650133603,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdp-128-small keypair",
            "value": 46158,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-128-small sign",
            "value": 3919580,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-128-small verify",
            "value": 2754421,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdp-192-balanced": [
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
        "date": 1749650157345,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdp-192-balanced keypair",
            "value": 103491,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-192-balanced sign",
            "value": 4181380,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-192-balanced verify",
            "value": 2871604,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdp-192-fast": [
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
        "date": 1749650183342,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdp-192-fast keypair",
            "value": 103817,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-192-fast sign",
            "value": 2517695,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-192-fast verify",
            "value": 1588966,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdp-192-small": [
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
        "date": 1749650210184,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdp-192-small keypair",
            "value": 103666,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-192-small sign",
            "value": 6233154,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-192-small verify",
            "value": 4355993,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdp-256-balanced": [
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
        "date": 1749650245406,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdp-256-balanced keypair",
            "value": 148014,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-256-balanced sign",
            "value": 7133680,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-256-balanced verify",
            "value": 4913680,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdp-256-fast": [
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
        "date": 1749650275132,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdp-256-fast keypair",
            "value": 150903,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-256-fast sign",
            "value": 4401576,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-256-fast verify",
            "value": 2829263,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdp-256-small": [
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
        "date": 1749650301972,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdp-256-small keypair",
            "value": 150783,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-256-small sign",
            "value": 11528581,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdp-256-small verify",
            "value": 8163868,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdpg-128-balanced": [
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
        "date": 1749650328637,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdpg-128-balanced keypair",
            "value": 23585,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-128-balanced sign",
            "value": 1528401,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-128-balanced verify",
            "value": 970480,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdpg-128-fast": [
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
        "date": 1749650354538,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdpg-128-fast keypair",
            "value": 23473,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-128-fast sign",
            "value": 780247,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-128-fast verify",
            "value": 477360,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdpg-128-small": [
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
        "date": 1749650377848,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdpg-128-small keypair",
            "value": 23351,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-128-small sign",
            "value": 2982918,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-128-small verify",
            "value": 1954664,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdpg-192-balanced": [
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
        "date": 1749650404454,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdpg-192-balanced keypair",
            "value": 45492,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-192-balanced sign",
            "value": 2278458,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-192-balanced verify",
            "value": 1481794,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdpg-192-fast": [
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
        "date": 1749650432748,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdpg-192-fast keypair",
            "value": 46388,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-192-fast sign",
            "value": 1777670,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-192-fast verify",
            "value": 1115344,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdpg-192-small": [
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
        "date": 1749650461253,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdpg-192-small keypair",
            "value": 45297,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-192-small sign",
            "value": 4298767,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-192-small verify",
            "value": 2922911,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdpg-256-balanced": [
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
        "date": 1749650489918,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdpg-256-balanced keypair",
            "value": 72982,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-256-balanced sign",
            "value": 3601245,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-256-balanced verify",
            "value": 2297231,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdpg-256-fast": [
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
        "date": 1749650515828,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdpg-256-fast keypair",
            "value": 72383,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-256-fast sign",
            "value": 2892713,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-256-fast verify",
            "value": 1843544,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "cross-rsdpg-256-small": [
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
        "date": 1749650541323,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "cross-rsdpg-256-small keypair",
            "value": 72629,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-256-small sign",
            "value": 6379268,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "cross-rsdpg-256-small verify",
            "value": 4184183,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "OV-Is": [
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
        "date": 1749650569533,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "OV-Is keypair",
            "value": 2527996,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-Is sign",
            "value": 64038,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-Is verify",
            "value": 39332,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "OV-Ip": [
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
        "date": 1749650594515,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "OV-Ip keypair",
            "value": 1570419,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-Ip sign",
            "value": 59074,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-Ip verify",
            "value": 57226,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "OV-III": [
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
        "date": 1749650619542,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "OV-III keypair",
            "value": 9645451,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-III sign",
            "value": 179398,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-III verify",
            "value": 158546,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "OV-V": [
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
        "date": 1749650643192,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "OV-V keypair",
            "value": 24744025,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-V sign",
            "value": 337605,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-V verify",
            "value": 337604,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "OV-Is-pkc": [
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
        "date": 1749650670327,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "OV-Is-pkc keypair",
            "value": 2517581,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-Is-pkc sign",
            "value": 64330,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-Is-pkc verify",
            "value": 218600,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "OV-Ip-pkc": [
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
        "date": 1749650698365,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "OV-Ip-pkc keypair",
            "value": 1572550,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-Ip-pkc sign",
            "value": 59276,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-Ip-pkc verify",
            "value": 174984,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "OV-III-pkc": [
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
        "date": 1749650723821,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "OV-III-pkc keypair",
            "value": 9677558,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-III-pkc sign",
            "value": 178743,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-III-pkc verify",
            "value": 723634,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "OV-V-pkc": [
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
        "date": 1749650752024,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "OV-V-pkc keypair",
            "value": 25008609,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-V-pkc sign",
            "value": 338799,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-V-pkc verify",
            "value": 1745381,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "OV-Is-pkc-skc": [
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
        "date": 1749650776685,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "OV-Is-pkc-skc keypair",
            "value": 2354388,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-Is-pkc-skc sign",
            "value": 1607865,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-Is-pkc-skc verify",
            "value": 215425,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "OV-Ip-pkc-skc": [
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
        "date": 1749650803889,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "OV-Ip-pkc-skc keypair",
            "value": 1606217,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-Ip-pkc-skc sign",
            "value": 1112877,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-Ip-pkc-skc verify",
            "value": 176690,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "OV-III-pkc-skc": [
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
        "date": 1749650831090,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "OV-III-pkc-skc keypair",
            "value": 9327955,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-III-pkc-skc sign",
            "value": 5932906,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-III-pkc-skc verify",
            "value": 724456,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ],
    "OV-V-pkc-skc": [
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
        "date": 1749650856015,
        "tool": "customSmallerIsBetter",
        "benches": [
          {
            "name": "OV-V-pkc-skc keypair",
            "value": 23932675,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-V-pkc-skc sign",
            "value": 13340953,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          },
          {
            "name": "OV-V-pkc-skc verify",
            "value": 1538277,
            "unit": "cycles",
            "extra": "Target platform: x86_64-Linux-6.11.0-1015-azure | Compiler: gcc (13.3.0) | Compile options: [-Wa,--noexecstack;-O3;-fomit-frame-pointer;-fdata-sections;-ffunction-sections;-Wl,--gc-sections;-Wbad-function-cast] | OQS version: 0.13.1-dev (major: 0, minor: 13, patch: 1, pre-release: -dev) | Git commit: d745d35938d17e666ffa88673523fd4c4399d326 | OpenSSL enabled: Yes (OpenSSL 3.0.13 30 Jan 2024) | AES: NI | SHA-2: OpenSSL | SHA-3: C | OQS build flags: OQS_DIST_BUILD OQS_LIBJADE_BUILD OQS_OPT_TARGET=generic CMAKE_BUILD_TYPE=Release | CPU exts active: ADX AES AVX AVX2 BMI1 BMI2 PCLMULQDQ POPCNT SSE SSE2 SSE3 | "
          }
        ]
      }
    ]
  }
}