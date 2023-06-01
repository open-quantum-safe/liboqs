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

    assert(kats[sig_name] == h256.hexdigest())

@helpers.filtered_test
@pytest.mark.parametrize('sig_stfl_name', helpers.available_sig_stfls_by_name())
def test_sig_stfl(sig_stfl_name):
    kats = helpers.get_kats("sig_stfl")
    if not(helpers.is_sig_stfl_enabled_by_name(sig_stfl_name)): pytest.skip('Not enabled')
    katfile = helpers.get_katfile("sig_stfl", sig_stfl_name)
    if not katfile: pytest.skip("KATs file is missing")
    output = helpers.run_subprocess(
        [helpers.path_to_executable('kat_sig_stfl'), sig_stfl_name, katfile],
    )
    output = output.replace("\r\n", "\n")
    h256 = sha256()
    h256.update(output.encode())

    assert(kats[sig_stfl_name] == h256.hexdigest())

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)
