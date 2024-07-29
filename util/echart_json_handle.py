# 该程序用于随机X与Y值

import json
import random
import math

def get_limits(nodes):
    x_values = [node['x'] for node in nodes]
    y_values = [node['y'] for node in nodes]
    x_min, x_max = min(x_values), max(x_values)
    y_min, y_max = min(y_values), max(y_values)
    return x_min, x_max, y_min, y_max

def modify_node_positions(nodes, x_min, x_max, y_min, y_max):
    center_x = (x_min + x_max) / 2
    center_y = (y_min + y_max) / 2
    max_size = max(node['size'] for node in nodes)
    min_size = min(node['size'] for node in nodes)

    for node in nodes:
        size_factor = (node['size'] - min_size) / (max_size - min_size)  # 归一化节点大小
        max_distance = max(abs(x_max - center_x), abs(y_max - center_y))  # 最大距离

        # 计算新的半径，size 越大，半径越小，但加入更多随机性
        base_r = (1 - size_factor) * max_distance
        random_r = random.uniform(-0.5 * max_distance, 0.5 * max_distance)  # 加入更多随机性
        r = base_r + random_r
        
        # 随机角度
        theta = random.uniform(0, 2 * math.pi)
        
        # 极坐标转换为笛卡尔坐标
        node['x'] = center_x + r * math.cos(theta)
        node['y'] = center_y + r * math.sin(theta)

def modify_json_file(input_file_path, output_file_path):
    # 读取 JSON 文件
    with open(input_file_path, 'r') as file:
        data = json.load(file)
    
    # 获取原始X和Y的极限值
    x_min, x_max, y_min, y_max = get_limits(data['nodes'])
    
    # 修改节点的X和Y值
    modify_node_positions(data['nodes'], x_min, x_max, y_min, y_max)
    
    # 将修改后的数据保存回 JSON 文件
    with open(output_file_path, 'w') as file:
        json.dump(data, file, indent=4)

input_file_path = '../static/echarts_data/test.json' 
output_file_path = '../static/echarts_data/test2.json'

modify_json_file(input_file_path, output_file_path)
