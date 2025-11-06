# SPDX-License-Identifier: MIT

import helpers
import os
import os.path
import pytest
import platform

@helpers.filtered_test
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_kem(kem_name):
    kats = helpers.get_kats("kem")
    if not(helpers.is_kem_enabled_by_name(kem_name)): pytest.skip('Not enabled')
    helpers.run_subprocess( [helpers.path_to_executable('speed_kem'), kem_name, "-f"] )

@helpers.filtered_test
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
def test_sig(sig_name):
    kats = helpers.get_kats("sig")
    if not(helpers.is_sig_enabled_by_name(sig_name)): pytest.skip('Not enabled')
    helpers.run_subprocess( [helpers.path_to_executable('speed_sig'), sig_name, "-f"])

@helpers.filtered_test
@pytest.mark.parametrize('sig_stfl_name', helpers.available_sig_stfls_by_name())
def test_sig(sig_stfl_name):
    # Define the list of LMS varients to allow
    names_to_allow = ["LMS_SHA256_H5_W1", "LMS_SHA256_H5_W2", 
                        "LMS_SHA256_H5_W4",
                        "LMS_SHA256_H5_W8",
                        "LMS_SHA256_H10_W1",
                        "LMS_SHA256_H10_W2",
                        "LMS_SHA256_H10_W4",
                        "LMS_SHA256_H10_W8",
                        "LMS_SHA256_H15_W1",
                        "LMS_SHA256_H15_W2",
                        "LMS_SHA256_H15_W4",
                        "LMS_SHA256_H5_W8_H5_W8",
                        "LMS_SHA256_H10_W4_H5_W8",]
    kats = helpers.get_kats("sig_stfl")
    if not(helpers.is_sig_stfl_enabled_by_name(sig_stfl_name)):
        pytest.skip('Not enabled')
    elif sig_stfl_name.find("_10")==-1 and sig_stfl_name.find("H10")==-1:
        pytest.skip('Test skipped')
    elif sig_stfl_name not in names_to_allow:
        pytest.skip('Test skipped')
    helpers.run_subprocess( [helpers.path_to_executable('speed_sig_stfl'), sig_stfl_name, "-f"])

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)
