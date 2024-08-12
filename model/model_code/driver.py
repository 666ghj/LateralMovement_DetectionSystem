import utils
import re
import subprocess
from pathlib import Path


_logger = lambda msg: print(msg)
_isLoaded = False
_password = ''
_pathPrefix = ''


def _parse_exported_code():
    with open('driver\\global_arg.c', 'r') as f:
        content = f.read()
    path_prefix_pattern = r'PWCHAR gPath = L"(.+?)";'
    rc4_key_pattern = r'PCHAR gRc4Key = "(.+?)";'
    path_prefix = re.search(path_prefix_pattern, content)
    rc4_key = re.search(rc4_key_pattern, content)
    if path_prefix and rc4_key:
        return utils.unescape_str(path_prefix.group(1)), utils.unescape_str(rc4_key.group(1))
    return '', ''


def _export_code():
    template = f"""#include "global.h"\n
PWCHAR gPath = L"{utils.escape_str(_pathPrefix)}";
PCHAR gRc4Key = "{utils.escape_str(_password)}";
"""
    with open('driver\\global_arg.c', 'w') as f:
        f.write(template)


def _vcx_build():
    proc = subprocess.Popen(["cmd", "/c", "scripts\\build.bat"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = proc.communicate()
    if proc.returncode:
        raise Exception(f'Failed to build: {stderr}')
    _logger(f"构建驱动成功…")


def _inf_install():
    file_path = Path('driver\\x64\\Debug\\FsFilterDemo\\FsFilterDemo.inf')
    absolute_path = file_path.resolve()
    proc = subprocess.Popen(["C:\\Windows\\System32\\InfDefaultInstall.exe", absolute_path], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = proc.communicate()
    if proc.returncode:
        raise Exception(f'Failed to install: {stderr}')
    _logger(f"安装驱动成功…")


def _flt_load():
    proc = subprocess.Popen(["fltmc", "load", "FsFilterDemo"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = proc.communicate()
    if proc.returncode:
        raise Exception(f'Failed to load: {stderr}')
    _logger(f"加载驱动成功！")


def _flt_unload():
    proc = subprocess.Popen(["fltmc", "unload", "FsFilterDemo"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = proc.communicate()
    if proc.returncode:
        raise Exception(f'Failed to unload: {stderr}')
    _logger(f"卸载驱动成功！")


def get_status() -> bool:
    return {'isLoaded': _isLoaded, 'password': _password, 'pathPrefix': _pathPrefix}


def load(path: str, key: str):
    global _isLoaded, _password, _pathPrefix
    _password = key
    _pathPrefix = path
    _logger(f"开始加载驱动，进程：{_pathPrefix}，密码：{_password}")
    _export_code()
    _vcx_build()
    _inf_install()
    _flt_load()
    _isLoaded = True


def unload():
    global _isLoaded
    _flt_unload()
    _isLoaded = False


def init():
    global _pathPrefix, _password, _isLoaded
    _pathPrefix, _password = _parse_exported_code()
    proc = subprocess.Popen(["fltmc", "filters"], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    stdout, stderr = proc.communicate()
    if proc.returncode:
        raise Exception(f'Failed to get filters: {stderr}')
    _isLoaded = b'FsFilterDemo' in stdout


def set_logger(func):
    global _logger
    _logger = func


init()


if __name__ == '__main__':
    _pathPrefix = '\\Device\\HarddiskVolume3\\FsFilterDemo\\'
    _password = 'password'
    _export_code()
    _vcx_build()
    _inf_install()
    _flt_load()
    _flt_unload()