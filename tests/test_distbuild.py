# SPDX-License-Identifier: MIT

import helpers
import pytest
import platform
from pathlib import Path

# TODO: We shouldn't use platform.machine() to select the qemu binary directly,
# since platform.machine() might return, say, AMD64 instead of x86_64.

if platform.machine() == 'x86_64':
    MINCPU = "Westmere"
elif platform.machine() == 'aarch64':
    MINCPU = "cortex-a53"
else:
    MINCPU = "max"

@helpers.filtered_test
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
@helpers.test_requires_build_options("OQS_DIST_BUILD")
@helpers.test_requires_qemu(platform.machine(), MINCPU)
def test_kem(kem_name):
    if not(helpers.is_kem_enabled_by_name(kem_name)):
        pytest.skip('Not enabled')

    helpers.run_subprocess(["qemu-"+platform.machine()+"-static", "-cpu", MINCPU,
                            helpers.path_to_executable('test_kem'), kem_name])

@helpers.filtered_test
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
@helpers.test_requires_build_options("OQS_DIST_BUILD")
@helpers.test_requires_qemu(platform.machine(), MINCPU)
def test_sig(sig_name):
    if not(helpers.is_sig_enabled_by_name(sig_name)):
        pytest.skip('Not enabled')

    helpers.run_subprocess(["qemu-"+platform.machine()+"-static", "-cpu", MINCPU,
                             helpers.path_to_executable('test_sig'), sig_name])

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

