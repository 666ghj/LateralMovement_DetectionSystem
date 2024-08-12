from flask import Flask, send_from_directory, request, jsonify
from flask_socketio import SocketIO
import threading
import webbrowser
import backup
import driver
import server


app = Flask('FileCrypt', static_url_path='/', static_folder='frontend/dist')
socketio = SocketIO(app)


@app.route('/', methods=['GET'])
def serve_static_index():
    return send_from_directory(app.static_folder, 'index.html')


@app.route('/api/server', methods=['GET', 'PUT'])
def handle_server():
    if request.method == 'GET':
        return jsonify({'server': backup.get_server()}), 200
    elif request.method == 'PUT':
        try:
            backup.update_server(request.json['server'])
            return jsonify({'server': backup.get_server()}), 201
        except Exception as e:
            return jsonify({'error': str(e)}), 400


@app.route('/api/status', methods=['GET'])
def handle_status():
    return jsonify(driver.get_status()), 200


@app.route('/api/load', methods=['PUT'])
def handle_load():
    try:
        driver.load(request.json['pathPrefix'], request.json['password'])
        return jsonify(None), 200
    except Exception as e:
        return jsonify({'error': str(e)}), 400


@app.route('/api/unload', methods=['GET'])
def handle_unload():
    try:
        driver.unload()
        return jsonify(None), 200
    except Exception as e:
        return jsonify({'error': str(e)}), 400


@app.route('/api/list', methods=['PUT'])
def handle_list():
    try:
        result = backup.list(request.json['path'])
        return jsonify(result), 200
    except Exception as e:
        return jsonify({'error': str(e)}), 400


def emit_log(content: str):
    socketio.emit('log', content)


def main():
    socketio.run(app, host='localhost', port=2057)


if __name__ == '__main__':
    driver.set_logger(emit_log)
    backup.set_logger(emit_log)
    backup_thread = threading.Thread(target=backup.main_loop)
    server_thread = threading.Thread(target=server.main)
    backend_thread = threading.Thread(target=main)
    threads = [server_thread, backend_thread, backup_thread]
    try:
        for thread in threads:
            thread.setDaemon(True)
            thread.start()
        webbrowser.open('http://localhost:2057')
        for thread in threads:
            thread.join()
    except KeyboardInterrupt:
        print("Received interrupt signal, exiting...")
        exit(0)
