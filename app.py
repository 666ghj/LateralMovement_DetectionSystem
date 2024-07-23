from flask import Flask,session,request,redirect,render_template
import re
app = Flask(__name__)
app.secret_key = 'The author of this system is BaiFu.'

from view.index import index
app.register_blueprint(index.index_pb)


@app.route('/')
def begin():
    # 跳转到index页面
    return redirect('/index')

# 404错误
@app.errorhandler(404)
def page_not_found(e):
    return render_template('404.html'), 404

# 500错误
@app.errorhandler(500)
def server_error(e):
    return render_template('500.html'), 500

if __name__ == '__main__':
    app.run()
