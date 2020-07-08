# SPDX-License-Identifier: MIT

import helpers
import os
import os.path
import pytest
import platform
from hashlib import sha256

@helpers.filtered_test
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_kem(kem_name):
    kats = helpers.get_kats("kem")
    if kem_name.startswith('SIDH'): pytest.skip('KATs not available for SIDH')
    if not(helpers.is_kem_enabled_by_name(kem_name)): pytest.skip('Not enabled')
    output = helpers.run_subprocess(
        [helpers.path_to_executable('kat_kem'), kem_name],
    )
    output = output.replace("\r\n", "\n")
    h256 = sha256()
    h256.update(output.encode())

    assert(kats[kem_name] == h256.hexdigest())

@helpers.filtered_test
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
def test_sig(sig_name):
    kats = helpers.get_kats("sig")
    if not(helpers.is_sig_enabled_by_name(sig_name)): pytest.skip('Not enabled')
    output = helpers.run_subprocess(
        [helpers.path_to_executable('kat_sig'), sig_name],
    )
    output = output.replace("\r\n", "\n")
    h256 = sha256()
    h256.update(output.encode())
    # qtesla's avx2 implementation uses an optimized sampling method that results in 
    # different KAT values; we use the correct one (if avx2/aes instructions are available)
    avx2_aes_enabled_on_linux = helpers.is_use_option_enabled_by_name('AVX2_INSTRUCTIONS') and helpers.is_use_option_enabled_by_name('AES_INSTRUCTIONS') and platform.system() == 'Linux'
    if sig_name.startswith('qTesla') and avx2_aes_enabled_on_linux and not helpers.is_build_portable():
       sig_name = sig_name+".avx2"

    assert(kats[sig_name] == h256.hexdigest())

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

