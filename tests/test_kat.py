import hashlib
import helpers
import os
import os.path
import pytest

@pytest.mark.parametrize('kem_name', helpers.enabled_kems_by_name())
def test_kem(kem_name):
    if kem_name.startswith('Sidh'): pytest.skip('KATs not available for SIDH')
    output = helpers.run_subprocess(
        ['kat_kem', kem_name],
        working_dir = 'tests'
    )
    kats = []
    for file in os.scandir(os.path.join('tests', 'KATs', 'kem')):
        filename = file.name
        if filename.startswith(kem_name + '.') and filename.endswith('.kat'):
            with open(os.path.join('tests', 'KATs', 'kem', filename), 'r') as myfile:
                kats.append(myfile.read())
    assert(output in kats)

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

