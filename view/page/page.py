from flask import Flask, render_template, Blueprint

page_pb = Blueprint('page',__name__,template_folder='templates')

# 识别路径/index
@page_pb.route('/index')
def show_index():
    return render_template('index.html')

# 识别路径/nodeMap
@page_pb.route('/nodeMap')
def show_nodeMap():
    return render_template('nodeMap.html')


# 识别路径logCollect
@page_pb.route('/logCollect')
def show_logCollect():
    return render_template('logCollect.html')

# 识别路径/networkControl
@page_pb.route('/networkControl')
def show_networkControl():
    return render_template('networkControl.html')

# 识别路径/threatEnhancement
@page_pb.route('/threatEnhancement')
def show_threatEnhancement():
    return render_template('threatEnhancement.html')

# 识别路径/detection
@page_pb.route('/detection')
def show_detection():
    return render_template('detection.html')

# 识别路径/forecast
@page_pb.route('/forecast')
def show_forecast():
    return render_template('forecast.html')

# 识别路径/detectionResult
@page_pb.route('/detectionResult')
def show_detectionResult():
    return render_template('detectionResult.html')

# 识别路径/forecastResult
@page_pb.route('/forecastResult')
def show_forecastResult():
    return render_template('forecastResult.html')