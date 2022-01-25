# Mysql运维篇

[TOC]

## 部署

### Single

详见deploy文件夹

### Replica

参考：https://www.cnblogs.com/haima/p/14341903.html

## 配置

- **query_cache_type**：决定是否使用查询缓存。建议设置为**DEMAND**，表示不使用查询缓存
- **innodb_flush_log_at_trx_commit**：决定Redo Log的同步策略。建议设置为1，即每次事务的Redo Log都持久化到磁盘
- **sync_binlog**：决定BinLog的同步策略。建议设置为1，即每次事务的BinLog都持久化到磁盘

## 工具链

### mysql

### mysqlcheck

### mysqldumpslow

### mysqloptimize

### mysqlreport

### mysqladmin

### mysql_config_editor

### mysql_embedded

### mysqlpump

### mysqlshow

### mysqlanalyze

### mysqldump

### mysqlimport

### mysqlrepair

### mysqlslap

## 监控

TODO...