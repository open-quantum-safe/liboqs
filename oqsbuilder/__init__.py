"""OQS Builder"""

import os

# TODO: consider using a logger instead of print for debugging

_liboqs_dir = os.environ.get("LIBOQS_DIR")
if not _liboqs_dir:
    raise KeyError("Missing environment variable LIBOQS_DIR")
LIBOQS_DIR = _liboqs_dir
LIBOQS_PATCH_DIR = os.path.join(LIBOQS_DIR, "scripts", "copy_from_upstream", "patches")

__version__ = "0.1"
