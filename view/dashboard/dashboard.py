# flask路由控制，当路由为'/index'时，展示当前目录下的index.html文件

from flask import Flask, render_template, Blueprint

# 创建新的蓝图来处理 /index 路由
dashboard_pb = Blueprint('dashboard', __name__, template_folder='.', static_folder='')


# 识别路径/index
@dashboard_pb.route('/dashboard')
def show_dashboard():
    return render_template('dashboard.html')
