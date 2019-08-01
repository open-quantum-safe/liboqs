import hashlib
import helpers
import os
import os.path
import pytest

@helpers.filtered_test
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_kem(kem_name):
    if kem_name.startswith('Sidh'): pytest.skip('KATs not available for SIDH')
    if not(helpers.is_kem_enabled_by_name(kem_name)): pytest.skip('Not enabled')
    output = helpers.run_subprocess(
        [os.path.join('tests', 'kat_kem'), kem_name],
    )
    kats = []
    for filename in os.listdir(os.path.join('tests', 'KATs', 'kem')):
        if filename.startswith(kem_name + '.') and filename.endswith('.kat'):
            with open(os.path.join('tests', 'KATs', 'kem', filename), 'r') as myfile:
                kats.append(myfile.read())
    assert(output in kats)

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

