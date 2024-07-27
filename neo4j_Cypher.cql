// 进行一次导入的全流程
// 提前将csv文件放入neo4j的import目录下
// 创建源节点
LOAD CSV WITH HEADERS FROM "file:///data.csv" AS row
MERGE (source:Host {host: row.source_host});
// 创建目标节点
LOAD CSV WITH HEADERS FROM "file:///data.csv" AS row
MERGE (destination:Host {host: row.destination_host});
// 创建连接事件
LOAD CSV WITH HEADERS FROM "file:///data.csv" AS row
MATCH (source:Host {host: row.source_host})
MATCH (destination:Host {host: row.destination_host})
CREATE (source)-[r:CONNECTION {
    id: toInteger(row.id),
    log_time: row.log_time,
    source_port: toInteger(row.source_port),
    dest_port: toInteger(row.dest_port),
    event: row.event,
    connection_method: row.connection_method,
    success: toInteger(row.success),
    source_user: row.source_user,
    destination_user: row.destination_user
}]->(destination);
// 创建成功连接的关系
MATCH (source:Host)-[r:CONNECTION]->(destination:Host)
WHERE r.success = 1
CREATE (source)-[new_r:SUCCESS_CONNECTION {
    id: r.id,
    log_time: r.log_time,
    source_port: r.source_port,
    dest_port: r.dest_port,
    event: r.event,
    connection_method: r.connection_method,
    color: 'red',
    source_user: r.source_user,
    destination_user: r.destination_user
}]->(destination);
// 创建失败连接的关系
MATCH (source:Host)-[r:CONNECTION]->(destination:Host)
WHERE r.success = 0
CREATE (source)-[new_r:FAILURE_CONNECTION {
    id: r.id,
    log_time: r.log_time,
    source_port: r.source_port,
    dest_port: r.dest_port,
    event: r.event,
    connection_method: r.connection_method,
    color: 'gray',
    source_user: r.source_user,
    destination_user: r.destination_user
}]->(destination);
// 删除CONNECTION关系
MATCH ()-[r:CONNECTION]->()
DELETE r;

// 清空数据库
// 删除所有关系
MATCH ()-[r]->()
DELETE r;
// 删除所有节点
MATCH (n)
DELETE n;