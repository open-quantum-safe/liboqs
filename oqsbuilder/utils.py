"""Utilities unrelated to OQS"""

import inspect

import jinja2


def currentframe_funcname() -> str:
    """Print the name of the function"""
    stack = inspect.stack()
    if len(stack) < 2:
        raise RuntimeError("current frame is missing a caller")
    return stack[1].frame.f_code.co_name


def load_jinja_template(path: str) -> jinja2.Template:
    """Read from the input path, then instantiate a template object"""
    with open(path, "r", encoding="utf-8") as f:
        template_str = f.read()
    return jinja2.Template(template_str)
