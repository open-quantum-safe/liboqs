# SPDX-License-Identifier: MIT

import os
from pathlib import Path
import shutil
import subprocess

import pytest

import helpers


LIBOQS_ROOT = Path(__file__).resolve().parents[1]


def _run(command, cwd=None, env=None):
    result = subprocess.run(
        command,
        cwd=cwd,
        env=env,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        universal_newlines=True,
    )
    assert result.returncode == 0, result.stdout


def test_installed_package_can_be_consumed_after_relocation(tmp_path):
    build_dir = Path(helpers.get_current_build_dir_name())
    if not build_dir.is_absolute():
        build_dir = LIBOQS_ROOT / build_dir
    if not (build_dir / "cmake_install.cmake").is_file():
        pytest.skip("requires a configured liboqs build")
    if shutil.which("cmake") is None:
        pytest.skip("requires CMake")

    install_dir = tmp_path / "install"
    relocated_dir = tmp_path / "relocated"
    consumer_dir = tmp_path / "consumer"
    consumer_build_dir = tmp_path / "consumer-build"
    consumer_dir.mkdir()

    _run(["cmake", "--install", str(build_dir), "--prefix", str(install_dir)])
    shutil.move(str(install_dir), str(relocated_dir))

    (consumer_dir / "CMakeLists.txt").write_text(
        "cmake_minimum_required(VERSION 3.15)\n"
        "project(liboqs_installed_consumer C)\n"
        "find_package(liboqs CONFIG REQUIRED)\n"
        "add_executable(installed_consumer main.c)\n"
        "target_link_libraries(installed_consumer PRIVATE OQS::oqs)\n",
        encoding="utf-8",
    )
    (consumer_dir / "main.c").write_text(
        "#include <oqs/oqs.h>\n"
        "#include <oqs/sig.h>\n"
        "#include <oqs/kem.h>\n"
        "int main(void) {\n"
        "  OQS_init();\n"
        "  int found = 0;\n"
        "  for (size_t i = 0; i < OQS_SIG_algs_length; i++) {\n"
        "    const char *name = OQS_SIG_alg_identifier(i);\n"
        "    if (OQS_SIG_alg_is_enabled(name)) {\n"
        "      OQS_SIG *sig = OQS_SIG_new(name);\n"
        "      if (sig == NULL) { OQS_destroy(); return 1; }\n"
        "      OQS_SIG_free(sig);\n"
        "      found = 1;\n"
        "      break;\n"
        "    }\n"
        "  }\n"
        "  if (!found) {\n"
        "    for (size_t i = 0; i < OQS_KEM_algs_length; i++) {\n"
        "      const char *name = OQS_KEM_alg_identifier(i);\n"
        "      if (OQS_KEM_alg_is_enabled(name)) {\n"
        "        OQS_KEM *kem = OQS_KEM_new(name);\n"
        "        if (kem == NULL) { OQS_destroy(); return 1; }\n"
        "        OQS_KEM_free(kem);\n"
        "        found = 1;\n"
        "        break;\n"
        "      }\n"
        "    }\n"
        "  }\n"
        "  OQS_destroy();\n"
        "  return found ? 0 : 1;\n"
        "}\n",
        encoding="utf-8",
    )

    _run(
        [
            "cmake",
            "-S",
            str(consumer_dir),
            "-B",
            str(consumer_build_dir),
            "-DCMAKE_PREFIX_PATH=" + str(relocated_dir),
        ]
    )
    _run(["cmake", "--build", str(consumer_build_dir), "--config", "Release"])

    executable = consumer_build_dir / "installed_consumer"
    if os.name == "nt":
        executable = consumer_build_dir / "Release" / "installed_consumer.exe"
        if not executable.is_file():
            executable = consumer_build_dir / "installed_consumer.exe"

    environment = os.environ.copy()
    runtime_dirs = [
        str(relocated_dir / "bin"),
        str(relocated_dir / "lib"),
        str(relocated_dir / "lib64"),
    ]
    if os.name == "nt":
        environment["PATH"] = os.pathsep.join(runtime_dirs + [environment.get("PATH", "")])
    elif os.uname().sysname == "Darwin":
        environment["DYLD_LIBRARY_PATH"] = os.pathsep.join(
            runtime_dirs + [environment.get("DYLD_LIBRARY_PATH", "")]
        )
    else:
        environment["LD_LIBRARY_PATH"] = os.pathsep.join(
            runtime_dirs + [environment.get("LD_LIBRARY_PATH", "")]
        )

    _run([str(executable)], env=environment)
