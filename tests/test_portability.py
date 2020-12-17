# SPDX-License-Identifier: MIT

import helpers
import pytest
import platform
from pathlib import Path

MIN_CPUS = {}
# set other CPU types for other architectures; Westmere supports cpuid but not avx2
MIN_CPUS["x86_64"] = "Westmere"

@helpers.filtered_test
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
@pytest.mark.skipif(not "Ubuntu" in platform.platform(), reason="Only supported on Ubuntu")
def test_kem(kem_name):
    if not(helpers.is_build_portable()):
        pytest.skip("Portability not enabled")

    if not(helpers.is_kem_enabled_by_name(kem_name)):
        pytest.skip('Not enabled')

    helpers.run_subprocess(["qemu-"+platform.machine(), "-cpu", MIN_CPUS[platform.machine()],
                            helpers.path_to_executable('test_kem'), kem_name])

@helpers.filtered_test
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
@pytest.mark.skipif(not "Ubuntu" in platform.platform(), reason="Only supported on Ubuntu")
def test_sig(sig_name):
    if not(helpers.is_build_portable()):
        pytest.skip("Portability not enabled")

    if not(helpers.is_sig_enabled_by_name(sig_name)):
        pytest.skip('Not enabled')

    if (sig_name.startswith("picnic")):
        pytest.skip("Picnic portability known to not be given.")

    helpers.run_subprocess(["qemu-"+platform.machine(), "-cpu", MIN_CPUS[platform.machine()],
                             helpers.path_to_executable('test_sig'), sig_name])

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

