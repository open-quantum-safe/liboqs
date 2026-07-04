# SPDX-License-Identifier: MIT

import helpers
import os
import pytest
import re
import sys

# Ensure every key-exchange algorithm in the code
# is mentioned in the documentation.
@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize('kem_name', helpers.available_kems_by_name())
def test_datasheet_kem(kem_name):
    helpers.run_subprocess(
        ['grep', '-r', kem_name, 'docs/algorithms']
    )

# Ensure every signature algorithm in the code
# is mentioned in the documentation.
@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
@pytest.mark.parametrize('sig_name', helpers.available_sigs_by_name())
def test_datasheet_sig(sig_name):
    helpers.run_subprocess(
        ['grep', '-r', sig_name, 'docs/algorithms']
    )

# Ensure astyle agrees with the formatting.
@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
def test_style():

    result = helpers.run_subprocess(
        ['tests/run_astyle.sh']
    )
    assert 'Formatted' not in result

@helpers.filtered_test
@pytest.mark.skipif(sys.platform.startswith("win"), reason="Not needed on Windows")
def test_spdx():

    result = helpers.run_subprocess(
        ['tests/test_spdx.sh']
    )
    if len(result) != 0:
        print("The following files do not have proper SPDX-License-Identifier headers:")
        print(result)
        assert False

# Regular expression matching the parts of C source code that must be
# blanked out before searching for memory functions: comments (both styles)
# and string/character literals. Literals have to be recognized as well:
# a string such as "/*" would otherwise be mistaken for the start of a
# comment, and function names inside literals are not calls.
C_COMMENTS_AND_LITERALS = re.compile(
    r'/\*.*?\*/'              # multi-line comment
    r'|//[^\n]*'              # single-line comment
    r'|"(?:\\.|[^"\\\n])*"'   # string literal
    r"|'(?:\\.|[^'\\\n])*'",  # character literal
    re.DOTALL)

MEMORY_FUNCTIONS = re.compile(r'\b(free|malloc|calloc|realloc|strdup)\s*\(')

def blank_out(match):
    # Replace the matched text with spaces, keeping newlines so that line
    # numbers remain valid in the blanked source.
    return '\n'.join(' ' * len(part) for part in match.group(0).split('\n'))

# Returns a list of (line number, function name) tuples, one for each call
# to a standard memory function in the given C source code. Calls inside
# comments and string/character literals are excluded, as are lines
# annotated with 'IGNORE memory-check'.
def find_memory_functions(content):
    ignored_lines = {no for no, line in enumerate(content.splitlines(), 1)
                     if 'IGNORE memory-check' in line}
    code = C_COMMENTS_AND_LITERALS.sub(blank_out, content)
    findings = []
    for match in MEMORY_FUNCTIONS.finditer(code):
        no = code.count('\n', 0, match.start()) + 1
        if no not in ignored_lines:
            findings.append((no, match.group(1)))
    return findings

# Ensure the checker itself parses C constructs correctly.
@pytest.mark.parametrize('content,expected', [
    # A plain call is found; prefixed identifiers such as OQS_MEM_malloc are not.
    ('void *p = malloc(4);\nq = OQS_MEM_malloc(4);\n', [(1, 'malloc')]),
    # Whitespace or a comment between the name and the parenthesis.
    ('free (p);\nfree/* ! */(q);\n', [(1, 'free'), (2, 'free')]),
    # Calls inside comments are not findings.
    ('// free(p)\n/* malloc(4) */\n', []),
    # Code sharing a line with a comment is still checked.
    ('void *p = malloc(4); /* four bytes */\n', [(1, 'malloc')]),
    ('/* cleanup: */ free(p);\n', [(1, 'free')]),
    ('free(p); /* comment continues\nhere */ free(q);\n', [(1, 'free'), (2, 'free')]),
    # Comment markers inside string literals do not start a comment...
    ('const char *s = "/*";\nfree(p);\n', [(2, 'free')]),
    # ...and function names inside literals are not calls.
    ('puts("malloc(4) failed");\nchar c = \'(\';\n', []),
    # Escaped quotes do not terminate a literal early.
    ('puts("\\" free(p)");\n', []),
    # 'IGNORE memory-check' suppresses findings on its line only.
    ('free(p); // IGNORE memory-check\nfree(q);\n', [(2, 'free')]),
])
def test_memory_functions_checker(content, expected):
    assert find_memory_functions(content) == expected

def test_memory_functions():
    c_h_files = []
    for path, _, files in os.walk('src'):
        c_h_files += [os.path.join(path, f) for f in files if f.endswith(('.c', '.h', '.fragment'))]

    okay = True
    for fn in c_h_files:
        with open(fn) as f:
            content = f.read()
        lines = content.splitlines()
        for no, func in find_memory_functions(content):
            okay = False
            print(f"Suspicious `{func}` in {fn}:{no}:{lines[no - 1].strip()}")

    assert okay, ("Standard memory functions are used in some files. "
                  "These should be changed to OQS_MEM_* equivalents as appropriate. "
                  "If you are sure you want to use these functions in a particular spot, "
                  "add the comment '// IGNORE memory-check' on the line where the function occurs.")

if __name__ == "__main__":
    test_memory_functions()
    import sys
    pytest.main(sys.argv)
