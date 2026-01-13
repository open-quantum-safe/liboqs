"""Utilities unrelated to OQS"""

import inspect

import jinja2


def currentframe_funcname() -> str:
    """Retrieves the name of the caller's function using stack inspection.

    Iterates through the current call stack to find the frame object of the
    immediate caller. It accesses the code object associated with that frame
    to extract the function name.

    This function relies on CPython implementation details (stack frames).
    It may not behave identically in other Python implementations like PyPy.

    :return: The name of the function that invoked this utility.
    """
    stack = inspect.stack()
    if len(stack) < 2:
        raise RuntimeError("current frame is missing a caller")
    return stack[1].frame.f_code.co_name


def load_jinja_template(path: str) -> jinja2.Template:
    """Read from the input path, then instantiate a template object"""
    with open(path, "r", encoding="utf-8") as f:
        template_str = f.read()
    return jinja2.Template(template_str)
