import csv
import json
import random
from datetime import datetime


# 将日志时间转换为 ISO 格式
def convert_time(log_time):
    try:
        dt = datetime.strptime(log_time, "%b %d %H:%M:%S")
        dt = dt.replace(year=datetime.now().year)  # 假设日志发生在当前年份
        return dt.isoformat() + '.000Z'
    except ValueError as e:
        print(f"Error parsing date: {e}")
        return None


# 根据指定概率随机选择颜色
def get_random_color():
    colors = ["#ff0000", "#0000ff", "#808080", "#00ff00"]  # 红色、蓝色、灰色、绿色
    probabilities = [0.1, 0.2, 0.2, 0.5]  # 对应颜色的概率
    return random.choices(colors, probabilities)[0]


# 读取 CSV 文件并转换为 JSON 格式的事件数据
def csv_to_json(csv_file_path, json_file_path, encoding='utf-8'):
    events = []
    with open(csv_file_path, mode='r', encoding=encoding) as csv_file:
        csv_reader = csv.DictReader(csv_file)
        for row in csv_reader:
            start_time = convert_time(row["log_time"])
            if not start_time:
                continue
            event = {
                "title": f"{row['source_host']}->{row['destination_host']}",
                "start": start_time,
                "color": get_random_color()
            }
            events.append(event)

    with open(json_file_path, mode='w', encoding='utf-8') as json_file:
        json.dump(events, json_file, indent=4)


# 使用示例
csv_file_path = './static/neo4j_data/ssh_logs.csv'
json_file_path = './static/echarts_data/log.json'
csv_to_json(csv_file_path, json_file_path,encoding='utf-8')
