from urllib.parse import urlparse, urlunparse
from urllib.parse import urlparse, urlunparse, urljoin


def normalize_url(url: str) -> str:
    parsed_url = urlparse(url, scheme='http')
    if not parsed_url.netloc:
        parsed_url = parsed_url._replace(netloc=parsed_url.path, path='')
    return urlunparse(parsed_url)


def build_url(base: str, path: str) -> str:
    base_url = normalize_url(base)
    full_url = urljoin(base_url, path)
    return full_url


def unescape_str(s: str) -> str:
    return bytes(s, 'utf-8').decode('unicode_escape')


def escape_str(s: str) -> str:
    result = s.encode('unicode_escape').decode('utf-8')
    result = result.replace('"', '\\"')
    return result
