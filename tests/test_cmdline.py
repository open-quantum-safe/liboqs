# SPDX-License-Identifier: MIT

from cgi import test
import helpers
import os
import pytest
import sys

@helpers.filtered_test
@pytest.mark.parametrize('program', ['example_kem', 'example_sig'])
def test_examples(program):
    helpers.run_subprocess(
        [helpers.path_to_executable(program)],
    )

@helpers.filtered_test
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_kem(kem_name):
    if not(helpers.is_kem_enabled_by_name(kem_name)): pytest.skip('Not enabled')
    helpers.run_subprocess(
        [helpers.path_to_executable('test_kem'), kem_name],
    )

# Stateless signature schemes testing
@helpers.filtered_test
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
def test_sig(sig_name):
    if not(helpers.is_sig_enabled_by_name(sig_name)): pytest.skip('Not enabled')
    if sys.platform.startswith("win") and 'APPVEYOR' in os.environ:
        if 'SPHINCS' in sig_name and ('192f' in sig_name or '192s' in sig_name or '256f' in sig_name or '256s' in sig_name):
            pytest.skip('Skipping SPHINCS+ 192s and 256s tests on Windows AppVeyor builds')
    helpers.run_subprocess(
        [helpers.path_to_executable('test_sig'), sig_name],
    )

# Stateful signature schemes testing
# @helpers.filtered_test
# @pytest.mark.parametrize('sig_stfl_name', helpers.available_sig_stfls_by_name())
# def test_sig_stfl_new_key(sig_stfl_name):
#     if not(helpers.is_sig_stfl_enabled_by_name(sig_stfl_name)): pytest.skip('Not enabled')
#     helpers.run_subprocess(
#         [helpers.path_to_executable('test_sig_stfl'), sig_stfl_name],
#     )

# # Stateful signature schemes testing with key read
@helpers.filtered_test
@pytest.mark.parametrize('sig_stfl_name', helpers.available_sig_stfls_by_name())
def test_sig_stfl_read_key(sig_stfl_name):
    directory = "./tests/saved_keys/"
    
    # Extract parts of the scheme name to form the filename
    height_of_tree = sig_stfl_name[sig_stfl_name.find('_')+1:sig_stfl_name.rfind('_')].replace('/', '')
    hash_alg = sig_stfl_name[sig_stfl_name.find('-')+1:sig_stfl_name.find('_')].lower()
    if hash_alg[-1] == '2': hash_alg = hash_alg[:-1]
    sig_scheme = sig_stfl_name[0:sig_stfl_name.find('-')].lower()
    hash_alg_bits = sig_stfl_name[sig_stfl_name.rfind('_')+1:]

    filename = "tmp_" + sig_scheme + height_of_tree + "_" + hash_alg + hash_alg_bits
    if not(helpers.is_sig_stfl_enabled_by_name(sig_stfl_name)): pytest.skip('Not enabled')
    helpers.run_subprocess(
        [helpers.path_to_executable('test_sig_stfl'), sig_stfl_name, "--read_key_from="+directory+filename],
    )

# Stateful signature schemes testing with key save
# @helpers.filtered_test
# @pytest.mark.parametrize('sig_stfl_name', helpers.available_sig_stfls_by_name())
# def test_sig_stfl_save_key(sig_stfl_name):
#     directory = "../../tests/saved_keys/"
    
#     # Extract parts of the scheme name to form the filename
#     height_of_tree = sig_stfl_name[sig_stfl_name.find('_')+1:sig_stfl_name.rfind('_')].replace('/', '')
#     hash_alg = sig_stfl_name[sig_stfl_name.find('-')+1:sig_stfl_name.find('_')].lower()
#     if hash_alg[-1] == '2': hash_alg = hash_alg[:-1]
#     sig_scheme = sig_stfl_name[0:sig_stfl_name.find('-')].lower()
#     hash_alg_bits = sig_stfl_name[sig_stfl_name.rfind('_')+1:]

#     filename = "tmp_" + sig_scheme + height_of_tree + "_" + hash_alg + hash_alg_bits
#     if not(helpers.is_sig_stfl_enabled_by_name(sig_stfl_name)): pytest.skip('Not enabled')
#     helpers.run_subprocess(
#         [helpers.path_to_executable('test_sig_stfl'), sig_stfl_name, "--save_key_to="+directory+filename],
#     )

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

