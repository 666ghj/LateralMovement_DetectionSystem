import os
import utils
import pathlib
import requests
import base64
import json
import time


_logger = lambda msg: print(msg)
backup_server = 'http://127.0.0.1:3000'


def update_server(addr: str):
    global backup_server
    addr = utils.normalize_url(addr)
    if test_server(addr):
        backup_server = addr
    else:
        raise Exception("Server error!")


def get_server() -> str:
    return backup_server


def test_server(addr):
    url = utils.build_url(addr, '/availability')
    response = requests.get(url)
    return response.status_code == 200


def _upload(path, data):
    payload = json.dumps({'path': path, 'data': data})
    url = utils.build_url(backup_server, '/upload')
    response = requests.put(url, data=payload, headers={'Content-Type': 'application/json'})
    _logger(f"备份 {path} 至 {backup_server}")


def _test_removed(file_basename: str):
    info_path = f'{file_basename}.bak.info'
    data_path = f'{file_basename}.bak'
    file_list = os.listdir('C:\\FsFilterDemotemp')
    if info_path in file_list or data_path in file_list:
        return False
    return True

def _handle_file(file_basename: str):
    info_path = f'C:\\FsFilterDemotemp\\{file_basename}.bak.info'
    data_path = f'C:\\FsFilterDemotemp\\{file_basename}.bak'
    with open(info_path, 'rb') as f:
        path_bytes = f.read()
    os.remove(info_path)
    with open(data_path, 'rb') as f:
        raw_data = f.read()
    os.remove(data_path)
    path = path_bytes.decode('utf-16')[8:]
    data = base64.b64encode(raw_data).decode()
    if not backup_server == '':
        _upload(path, data)


def list(path):
    payload = json.dumps({'path': path})
    url = utils.build_url(backup_server, '/list')
    response = requests.put(url, data=payload, headers={'Content-Type': 'application/json'})
    return response.json()


def main_loop():
    while True:
        file_list = os.listdir('C:\\FsFilterDemotemp')
        if len(file_list) == 0:
            continue
        time.sleep(0.1)
        for file in file_list:
            if file.endswith('.info'):
                _handle_file(file[:-9])


def set_logger(func):
    global _logger
    _logger = func
