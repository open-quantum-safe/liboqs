import ast
import time
import sys
import pytest
import requests
from pathlib import Path
from unittest.mock import patch, call
from requests.exceptions import RequestException, ConnectionError

sys.path.insert(0, str(Path("tests").resolve()))
import helpers

# Load the actual modified function without
# requiring the compiled cryptographic library.
path = Path("tests/test_wycheproof_vectors.py")
tree = ast.parse(path.read_text())

node = next(
    n for n in tree.body
    if isinstance(n, ast.FunctionDef)
    and n.name == "_cached_requests_get_with_retries"
)

namespace = {
    "helpers": helpers,
    "time": time,
    "pytest": pytest,
    "RequestException": RequestException
}

exec(compile(
    ast.Module(body=[node], type_ignores=[]),
    str(path), "exec"
), namespace)

retry = namespace["_cached_requests_get_with_retries"]
URL = "https://example.com/vectors.json"

def setup_function():
    helpers.cached_requests_get.cache_clear()

def response(status=200):
    r = requests.Response()
    r.status_code = status
    r.url = URL
    return r

def test_successful_download():
    with patch("requests.get", return_value=response()) as get:
        assert retry(URL).status_code == 200
        assert get.call_count == 1

def test_temporary_network_failure():
    with patch("requests.get", side_effect=[
        ConnectionError("Connection reset"),
        response()
    ]) as get, patch("time.sleep") as sleep:
        assert retry(URL).status_code == 200
        assert get.call_count == 2
        sleep.assert_called_once_with(1)

def test_persistent_network_failure():
    with patch("requests.get", side_effect=ConnectionError(
        "Network unavailable"
    )) as get, patch("time.sleep") as sleep:
        with pytest.raises(pytest.skip.Exception):
            retry(URL)
        assert get.call_count == 3
        assert sleep.call_args_list == [call(1), call(2)]

def test_http_error_recovery():
    with patch("requests.get", side_effect=[
        response(503),
        response()
    ]) as get, patch("time.sleep"):
        assert retry(URL).status_code == 200
        assert get.call_count == 2
