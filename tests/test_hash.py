import hashlib
import helpers
import pytest

@pytest.mark.parametrize('msg', ['', 'a', 'abc', '1234567890123456789012345678901678901567890'])
def test_sha256(msg):
    output = helpers.run_subprocess(
        ['tests/test_hash', 'sha256'],
        input = msg.encode(),
    )
    assert(output.rstrip() == hashlib.sha256(msg.encode()).hexdigest())

@pytest.mark.parametrize('msg', ['', 'a', 'abc', '1234567890123456789012345678901678901567890'])
def test_sha384(msg):
    output = helpers.run_subprocess(
        ['tests/test_hash', 'sha384'],
        input = msg.encode(),
    )
    assert(output.rstrip() == hashlib.sha384(msg.encode()).hexdigest())

@pytest.mark.parametrize('msg', ['', 'a', 'abc', '1234567890123456789012345678901678901567890'])
def test_sha512(msg):
    output = helpers.run_subprocess(
        ['tests/test_hash', 'sha512'],
        input = msg.encode(),
    )
    assert(output.rstrip() == hashlib.sha512(msg.encode()).hexdigest())

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

