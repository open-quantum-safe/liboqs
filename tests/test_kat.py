import helpers
import os
import os.path
import pytest

@helpers.filtered_test
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_kem(kem_name):
    if kem_name.startswith('SIDH'): pytest.skip('KATs not available for SIDH')
    if not(helpers.is_kem_enabled_by_name(kem_name)): pytest.skip('Not enabled')
    output = helpers.run_subprocess(
        [helpers.path_to_executable('kat_kem'), kem_name],
    )
    output = output.replace("\r\n", "\n")
    kats = []
    for filename in os.listdir(os.path.join('tests', 'KATs', 'kem')):
        if filename.startswith(kem_name + '.') and filename.endswith('.kat'):
            with open(os.path.join('tests', 'KATs', 'kem', filename), 'r') as myfile:
                kats.append(myfile.read())
    assert(output in kats)

@helpers.filtered_test
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
def test_sig(sig_name):
    if not(helpers.is_sig_enabled_by_name(sig_name)): pytest.skip('Not enabled')
    output = helpers.run_subprocess(
        [helpers.path_to_executable('kat_sig'), sig_name],
    )
    output = output.replace("\r\n", "\n")
    kats = []
    avx2_aes_enabled = helpers.is_use_option_enabled_by_name('AVX2_INSTRUCTIONS') and helpers.is_use_option_enabled_by_name('AES_INSTRUCTIONS')
    for filename in os.listdir(os.path.join('tests', 'KATs', 'sig')):
        if filename.startswith(sig_name + '.') and filename.endswith('.kat'):
            # qtesla's avx2 implementation uses an optimized sampling method that results in different KAT values; we use the correct one (if avx2/aes instructions are available)
            if not (sig_name.startswith('qTesla')) or any([avx2_aes_enabled and 'avx2' in filename, not avx2_aes_enabled and not 'avx2' in filename]):
                with open(os.path.join('tests', 'KATs', 'sig', filename), 'r') as myfile:
                    kats.append(myfile.read())
    assert(output in kats)

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

