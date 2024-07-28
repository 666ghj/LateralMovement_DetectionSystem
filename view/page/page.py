from flask import Flask, render_template, Blueprint

page_pb = Blueprint('page',__name__,template_folder='templates')


# 识别路径/neo4j_dashboard
@page_pb.route('/nodeMap')
def show_nodeMap():
    return render_template('nodeMap.html')


