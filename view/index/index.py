# flask路由控制，当路由为'/index'时，展示当前目录下的index.html文件

from flask import Flask, render_template, Blueprint

# 创建新的蓝图来处理 /index 路由
index_pb = Blueprint('index', __name__, template_folder='.', static_folder='')


# 识别路径/index
@index_pb.route('/index')
def show_index():
    return render_template('index.html')

# 识别路径/test
@index_pb.route('/test')
def show_test():
    return render_template('base_page.html')

# 识别路径/test2
@index_pb.route('/test2')
def show_test2():
    return render_template('test2.html')
