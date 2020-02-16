import helpers
import pytest
import sys

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
def test_style():

    result = helpers.run_subprocess(
<<<<<<< HEAD
        ['tests/test_style.sh']
=======
        ['tests/test_style.sh'],
        env = {'CLANGFORMAT': found_clang_format},
>>>>>>> 1b0f08f6... Initial format with astyle
    )
    assert 'Formatted' not in result

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

