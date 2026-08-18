# SPDX-License-Identifier: MIT

import hashlib
import helpers
import pytest
import random
import sys

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not supported on Windows")
def test_aes():
    helpers.run_subprocess(
        [helpers.path_to_executable('test_aes')],
    )

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not supported on Windows")
def test_sha3():
    helpers.run_subprocess(
        [helpers.path_to_executable('test_sha3')],
    )

@helpers.filtered_test
@pytest.mark.parametrize('algname', ['sha256', 'sha384', 'sha512', 'sha3_256', 'sha3_384', 'sha3_512'])
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not supported on Windows")
def test_hash_sha2_random(algname):
    # hash every size from 0 to 1024, then every 11th size after that 
    # (why 11? it's coprime with powers of 2, so we should land in a 
    #  bunch of random-ish spots relative to block boundaries)
    for i in list(range(0, 1024)) + list(range(1025, 20000, 11)):
        msg = "".join("1" for j in range(i)).encode()
        hasher = hashlib.new(algname)
        hasher.update(msg)
        output = helpers.run_subprocess(
            [helpers.path_to_executable('test_hash'), algname],
            input = msg,
        )
        if output.rstrip() != hasher.hexdigest():
            print(msg.hex())
            assert False, algname + " hashes don't match for the above " + str(i) + "-byte hex string; liboqs output = " + output.rstrip() + "; Python output = " + hasher.hexdigest()
        if algname[0:4] == "sha3": continue
        output = helpers.run_subprocess(
            [helpers.path_to_executable('test_hash'), algname + 'inc'],
            input = msg,
        )
        if output.rstrip() != hasher.hexdigest():
            print(msg.hex())
            assert False, algname + " hashes (using liboqs incremental API) don't match for the above " + str(i) + "-byte hex string; liboqs output = " + output.rstrip() + "; Python output = " + hasher.hexdigest()

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

