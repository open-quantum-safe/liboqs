# SPDX-License-Identifier: MIT

import helpers
import os
import pytest
import re
import sys

@helpers.filtered_test
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_kem_leak(kem_name):
    if not(helpers.is_kem_enabled_by_name(kem_name)): pytest.skip('Not enabled')
    if sys.platform != "linux" or os.system("grep ubuntu /etc/os-release") != 0 or os.system("uname -a | grep x86_64") != 0: pytest.skip('Leak testing not supported on this platform')
    helpers.run_subprocess(
        ["valgrind", "-s", "--error-exitcode=1", "--leak-check=full", "--show-leak-kinds=all", "--vex-guest-max-insns=25", "--track-origins=yes", helpers.path_to_executable('test_kem'), kem_name],
    )

@helpers.filtered_test
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
def test_sig_leak(sig_name):
    if ("SLH_DSA" in sig_name): pytest.skip()
    if not(helpers.is_sig_enabled_by_name(sig_name)): pytest.skip('Not enabled')
    if sys.platform != "linux" or os.system("grep ubuntu /etc/os-release") != 0 or os.system("uname -a | grep x86_64") != 0: pytest.skip('Leak testing not supported on this platform')
    helpers.run_subprocess(
        ["valgrind", "-s", "--error-exitcode=1", "--leak-check=full", "--show-leak-kinds=all", helpers.path_to_executable('test_sig'), sig_name],
    )

@pytest.mark.skipif("SLH_DSA_LEAK_TEST" not in os.environ, reason="SLH DSA leak testing only performed in extended tests")
@helpers.filtered_test
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
def test_slhdsa_leak(sig_name):
    if (not ("SLH_DSA" in sig_name)): pytest.skip()
    if not(helpers.is_sig_enabled_by_name(sig_name)): pytest.skip('Not enabled')
    if sys.platform != "linux" or os.system("grep ubuntu /etc/os-release") != 0 or os.system("uname -a | grep x86_64") != 0: pytest.skip('Leak testing not supported on this platform')
    helpers.run_subprocess(
        ["valgrind", "-s", "--error-exitcode=1", "--leak-check=full", "--show-leak-kinds=all", helpers.path_to_executable('test_sig'), sig_name],
    )

@helpers.filtered_test
@pytest.mark.parametrize('sig_stfl_name', helpers.available_sig_stfls_by_name())
def test_sig_stfl_leak(sig_stfl_name):
    if not(helpers.is_sig_stfl_enabled_by_name(sig_stfl_name)): pytest.skip('Not enabled')
    if sys.platform != "linux" or os.system("grep ubuntu /etc/os-release") != 0 or os.system("uname -a | grep x86_64") != 0: pytest.skip('Leak testing not supported on this platform')
    if sig_stfl_name.startswith("XMSS"):
        katfile = helpers.get_katfile("sig_stfl", sig_stfl_name)
        if not katfile: pytest.skip("KATs file is missing")
        helpers.run_subprocess(
            ["valgrind", "-s", "--error-exitcode=1", "--leak-check=full", "--show-leak-kinds=all", helpers.path_to_executable('test_sig_stfl'), sig_stfl_name, katfile],
        )
    else:
        helpers.run_subprocess(
            ["valgrind", "-s", "--error-exitcode=1", "--leak-check=full", "--show-leak-kinds=all", helpers.path_to_executable('test_sig_stfl'), sig_stfl_name],
        )

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

