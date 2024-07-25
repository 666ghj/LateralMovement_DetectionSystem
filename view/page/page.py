from flask import Flask, render_template, Blueprint

page_pb = Blueprint('page',__name__,template_folder='templates')


# 识别路径/neo4j_dashboard
@page_pb.route('/neo4j_dashboard')
def show_neo4j_dashboard():
    return render_template('neo4j_dashboard.html')
