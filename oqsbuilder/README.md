# OQS Builder
This is the replacement for `copy_from_upstream.py`

**Progress**:
- ✅ clone remote repository
- ✅ apply patches
- ⚠️ move source file from upstream into `liboqs/src`
- ⚠️ render `CMakeLists.txt` and other build files
- ⚠️ figure out how to check feature parity with `copy_from_upstream.py`

## Spot checks
```bash
# Should print version
python -m oqsbuilder --version

python -m oqsbuilder --file oqsbuilder/INTEGRATIONS.yml copy
```
