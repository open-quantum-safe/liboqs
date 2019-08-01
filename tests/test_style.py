import helpers
import pytest
import sys

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
def test_style():

    modified_files = helpers.run_subprocess(
        ['git', 'status', '-s']
    )
    if (modified_files != ""):
        assert False, "There are modified files present in the directory prior to prettyprint check. This may indicate that some files should be added to .gitignore or need to be committed."

    clang_formats = ['/usr/local/Cellar/clang-format/2016-06-27/bin/clang-format', 'clang-format-3.9', 'clang-format']
    found_clang_format = None
    for clang_format in clang_formats:
        try:
            helpers.run_subprocess(
                [clang_format, '/dev/null'],
            )
            found_clang_format = clang_format
            break
        except:
            pass
        finally:
            pass
    assert found_clang_format != None, 'No clang-format found'

    version = helpers.run_subprocess(
        [found_clang_format, '-version']
    )
    assert 'version 3.9' in version, 'Invalid clang-format version (' + version + ')'

    helpers.run_subprocess(
        ['make', 'prettyprint'],
        env = {'CLANGFORMAT': found_clang_format},
    )

    modified_files = helpers.run_subprocess(
        ['git', 'status', '-s']
    )
    if (modified_files != ""):
        print(modified_files)
        assert False, "Some files do not adhere to project style standards.  See the last list of files below `git status -s`"

if __name__ == "__main__":
    import sys
    pytest.main(sys.argv)

