"""Utilities unrelated to OQS"""

import inspect


def currentframe_funcname() -> str:
    """Print the name of the function"""
    stack = inspect.stack()
    if len(stack) < 2:
        raise RuntimeError("current frame is missing a caller")
    return stack[1].frame.f_code.co_name
