from flask import Flask, send_from_directory, request, jsonify
import pathlib
import base64


app = Flask('FileCrypt', static_url_path='/get', static_folder='backup')


@app.route('/upload', methods=['PUT'])
def backup():
    try:
        path = request.json['path']
        data = request.json['data']
        file_path = pathlib.Path('backup') / pathlib.Path(path)
        file_path.parent.mkdir(parents=True, exist_ok=True)
        data = base64.b64decode(data)
        with file_path.open('wb') as f:
            f.write(data)
        return jsonify(None), 200
    except Exception as e:
        print(str(e))
        return jsonify({'error': str(e)}), 400


@app.route('/availability', methods=['GET'])
def test():
    return jsonify(True), 200


@app.route('/list', methods=['PUT'])
def list():
    try:
        path = request.json['path']
        dir = pathlib.Path('backup') / pathlib.Path(path)
        folder_result = []
        file_result = []
        for item in dir.iterdir():
            if item.is_file():
                file_result.append({
                    'name': item.name,
                    'isFile': True,
                })
            elif item.is_dir():
                folder_result.append({
                    'name': item.name,
                    'isFile': False,
                })
        return jsonify(folder_result + file_result), 200
    except Exception as e:
        print(str(e))
        return jsonify({'error': str(e)}), 400


def main():
    app.run('0.0.0.0', port=3000)


if __name__ == '__main__':
    main()
