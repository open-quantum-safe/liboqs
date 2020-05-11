# SPDX-License-Identifier: MIT

import hashlib
import helpers
import pytest
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
@pytest.mark.parametrize('msg', ['', 'a', 'abc', '1234567890123456789012345678901678901567890'])
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not supported on Windows")
def test_sha256(msg):
    output = helpers.run_subprocess(
        [helpers.path_to_executable('test_hash'), 'sha256'],
        input = msg.encode(),
    )
    assert(output.rstrip() == hashlib.sha256(msg.encode()).hexdigest())
    output = helpers.run_subprocess(
        [helpers.path_to_executable('test_hash'), 'sha256inc'],
        input = msg.encode(),
    )
    assert(output.rstrip() == hashlib.sha256(msg.encode()).hexdigest())

@helpers.filtered_test
@pytest.mark.parametrize('msg', ['', 'a', 'abc', '1234567890123456789012345678901678901567890'])
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not supported on Windows")
def test_sha384(msg):
    output = helpers.run_subprocess(
        [helpers.path_to_executable('test_hash'), 'sha384'],
        input = msg.encode(),
    )
    assert(output.rstrip() == hashlib.sha384(msg.encode()).hexdigest())
    output = helpers.run_subprocess(
        [helpers.path_to_executable('test_hash'), 'sha384inc'],
        input = msg.encode(),
    )
    assert(output.rstrip() == hashlib.sha384(msg.encode()).hexdigest())

@helpers.filtered_test
@pytest.mark.parametrize('msg', ['', 'a', 'abc', '1234567890123456789012345678901678901567890'])
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not supported on Windows")
def test_sha512(msg):
    output = helpers.run_subprocess(
        [helpers.path_to_executable('test_hash'), 'sha512'],
        input = msg.encode(),
    )
    assert(output.rstrip() == hashlib.sha512(msg.encode()).hexdigest())
    output = helpers.run_subprocess(
        [helpers.path_to_executable('test_hash'), 'sha512inc'],
        input = msg.encode(),
    )
    assert(output.rstrip() == hashlib.sha512(msg.encode()).hexdigest())

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

