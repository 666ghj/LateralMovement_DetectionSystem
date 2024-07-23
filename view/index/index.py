# flask路由控制，当路由为'/index'时，展示当前目录下的index.html文件

from flask import Flask, render_template, Blueprint

# 创建新的蓝图来处理 /index 路由
index_pb = Blueprint('index', __name__, template_folder='.')


@index_pb.route('/index')
def show_index():
    return render_template('index.html')
