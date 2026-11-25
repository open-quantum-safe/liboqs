# SPDX-License-Identifier: MIT

import requests
import pytest
from unittest.mock import Mock

import helpers


def _response(url, status_code=200, content=b"vectors"):
    response = requests.Response()
    response.status_code = status_code
    response.url = url
    response._content = content
    response._content_consumed = True
    return response


def test_cached_requests_get_reuses_on_disk_response(monkeypatch, tmp_path):
    url = "https://example.test/vectors.json"
    monkeypatch.setenv("OQS_TEST_CACHE_DIR", str(tmp_path))
    monkeypatch.setattr(helpers.requests, "get", lambda *args, **kwargs: _response(url))
    first = helpers.cached_requests_get(url)

    monkeypatch.setattr(
        helpers.requests,
        "get",
        lambda *args, **kwargs: pytest.fail("expected the on-disk cache to be used"),
    )
    second = helpers.cached_requests_get(url)

    assert first.content == second.content == b"vectors"
    assert second.url == url


def test_cached_requests_get_retries_transient_errors(monkeypatch, tmp_path):
    url = "https://example.test/vectors.json"
    monkeypatch.setenv("OQS_TEST_CACHE_DIR", str(tmp_path))
    failures = iter([requests.ConnectionError("reset"), _response(url)])
    monkeypatch.setattr(helpers.requests, "get", Mock(side_effect=list(failures)))
    sleeps = []
    monkeypatch.setattr(helpers.time, "sleep", sleeps.append)

    response = helpers.cached_requests_get(url)

    assert response.content == b"vectors"
    assert sleeps == [1]


def test_cached_requests_get_does_not_retry_client_errors(monkeypatch, tmp_path):
    url = "https://example.test/missing.json"
    monkeypatch.setenv("OQS_TEST_CACHE_DIR", str(tmp_path))
    calls = []

    def get(*args, **kwargs):
        calls.append(url)
        return _response(url, status_code=404)

    monkeypatch.setattr(helpers.requests, "get", get)

    with pytest.raises(requests.HTTPError):
        helpers.cached_requests_get(url)

    assert calls == [url]
