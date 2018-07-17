#!/usr/bin/env bash
###############################################################################
#                               Fix-up Doctests
#
# Please don't just blindly call this to make failures go away,
# but review all changes.
###############################################################################
SAGE_ROOT=$(sage -c "import os; print os.environ['SAGE_ROOT']")
export SAGE_ROOT="$SAGE_ROOT"

# shellcheck source=/dev/null
source "$SAGE_ROOT/local/bin/sage-env"
PYTHONIOENCODING=UTF-8 PYTHONPATH=$(pwd) sage-fixdoctests "$@"
