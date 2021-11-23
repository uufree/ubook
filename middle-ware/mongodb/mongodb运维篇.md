# MongoDB运维篇

[TOC]

## 部署

详见deploy文件夹

## 配置

## 工具链

- **mongod**：Mongo Service

  - `--quiet`：隐藏所有的输出日志
  - `--port <port>`：指定port，默认27017
  - `--logpath arg`：指定log path。在k8s环境下，还是直接从STDOUT收集日志更方便些
  - `--logappend`：采用append方法写log，而不是over-write
  - `--logRotate [rename|reopen]`：循环写日志的方法
  - `--bind_ip <arg>`
  - `--bind_ip_all`
  - `--listenBacklog <size>`：listen backlog。默认128
  - `--maxConns <size>`：可以支持的最大连接数。默认1000000
  - `--keyFile <path>`：cluster authentication
  - `--noauth`：关闭验证
  - `--auth`：启用验证
  - `--clusterAuthMode <arg>`：可选：keyFile|sendKeyFile|sendX509|x509
  - `--fork`：以fork的方式启动mongodb
  - `--replSet arg`：副本集名称
  - `--configsvr`：以config service的方式启用服务
  - `--shardsvr`：以shard service的方式启用服务
  - `--storageEngine arg`：存储引擎。默认wiredTiger
  - `--dbpath arg`：数据文件的存放位置。默认/data/db
  - `--directoryperdb`：每个db使用独立的文件夹
  - `--noprealloc`：禁止文件空间预分配
  - `--syncdelay arg`：每多少秒写一次硬盘。默认60s
  - `--journal`：启用日志记录功能。such as mysql log
  - `--nojournal`：禁用日志记录功能

- **mongo**：Mongo Shell

  - `--quiet`：隐藏所有的输出日志
  - `--host <ip>`
  - `--port <port>`
  - `-u <username>`
  - `-p <password>`
  - `--authenticationDatabase <database-name>`：需要验证的DB Name
  - `--authenticationMechanism <mechanism>`：身份验证算法。一般不用填

  ```bash
  # 指定登录db和service host
  mongo tbb --host 127.0.0.1 --port 27017 -u tbbuser -p tbbuser
  ```

- **mongoc-stat**：这个工具没啥用。

- **mongodump**：导出备份工具

  - `--quiet`：隐藏所有的输出日志
  - `--host <ip>`
  - `--port <port>`
  - `-u <username>`
  - `-p <password>`
  - `--authenticationDatabase <database-name>`：需要验证的DB Name
  - `--authenticationMechanism <mechanism>`：身份验证算法。一般不用填
  - `-d <db-name>`：需要导出的DB
  - `-c <collection-name>`：需要导出的Collection
  - `--query=<{query-string}> `：根据query条件导出文档
  - `--queryFile=<query-file-path>`：根据query file导出文档
  - `-o <path>`：导出文件的存放位置
  - `--gzip`：导出gzip压缩的文件
  - `--dumpDbUsersAndRoles`：导出db中的user和roles
  - `--excludeCollection=<collection-name>`：在导出过程中，需要排除那些collection
  - `--excludeCollectionsWithPrefix=<collection-prefix>`：在导出过程中，需要排除的collection prefix

  ```bash
  # 根据一定的条件导出gzip
  mongodump -h 127.0.0.1:27017 -u tbbuser -p tbbuser --authenticationDatabase tbb -d tbb -c test --query='{name: "uuchen"}' -o tbb
  
  # 导出archive文件
  mongodump -h 127.0.0.1:27017 -u tbbuser -p tbbuser --authenticationDatabase tbb -d tbb --archive=tbb.archive --gzip
  ```

- **mongorestore**：导入备份文件

  - `--quiet`：隐藏所有的输出日志
  - `--host <ip>`
  - `--port <port>`
  - `-u <username>`
  - `-p <password>`
  - `--authenticationDatabase <database-name>`：需要验证的DB Name
  - `--authenticationMechanism <mechanism>`：身份验证算法。一般不用填
  - `-d <db-name>`：需要导出的DB
  - `-c <collection-name>`：需要导出的Collection
  - `--objcheck`：在导入前检查所有的文档对象
  - `--restoreDbUsersAndRoles`：导入user和roles
  - `--dir=<directory-name>`：指定导入的文件夹
  - `--gzip`：导入gzip类型的文件
  - `--drop`：在导入之前删除同名的collection
  - `--noIndexRestore`：不导入index
  - `--noOptionsRestore`：不导入options

  ```bash
  # 导入collection
  mongorestore -h 127.0.0.1:27017 -u tbbuser -p tbbuser --authenticationDatabase tbb --dir=test/ --gzip --objcheck
  
  
  # 导入db
  mongorestore -h 127.0.0.1:27017 -u tbbuser -p tbbuser --authenticationDatabase tbb --dir=tbb --gzip --objcheck --drop
  
  # 根据archive文件导入
  mongorestore -h 127.0.0.1:27017 -u tbbuser -p tbbuser --authenticationDatabase tbb --archive=tbb.archive --gzip --objcheck --drop
  ```

- **mongoexport**：导出文件（可以以非BSON格式导出）

- **mongoimport**：导入文件（接受非BSON格式的导入）

- **mongofiles**：GridFS操作工具。真的不需要用MongoDB做存储

- **mongos**：作为Mongo Cluster Router运行

  - `--quiet`：隐藏所有的输出日志
  - `--port <port>`：指定port，默认27017
  - `--logpath arg`：指定log path。在k8s环境下，还是直接从STDOUT收集日志更方便些
  - `--logappend`：采用append方法写log，而不是over-write
  - `--logRotate [rename|reopen]`：循环写日志的方法
  - `--bind_ip <arg>`
  - `--bind_ip_all`
  - `--listenBacklog <size>`：listen backlog。默认128
  - `--maxConns <size>`：可以支持的最大连接数。默认1000000
  - `--keyFile <path>`：cluster authentication
  - `--noauth`：关闭验证
  - `--auth`：启用验证
  - `--clusterAuthMode <arg>`：可选：keyFile|sendKeyFile|sendX509|x509
  - `--fork`：以fork的方式启动mongodb
  - `--configdb arg`：指定配置服务器列表。格式为：`config-replset-name/host1:port,host2:port`

- **mongostat**：观察整个Mongo服务的负载

  - `--quiet`：隐藏所有的输出日志
  - `-h <ip:port>`
  - `-u <username>`
  - `-p <password>`
  - `--authenticationDatabase <database-name>`：需要验证的DB Name
  - `--authenticationMechanism <mechanism>`：身份验证算法。一般不用填
  - `--json`：打印json格式的输出

  ```bash
  mongostat -h 0.0.0.0:27017 -u superadmin -p superadmin --authenticationDatabase admin
  
  # 输出字段释义
  insert/query/update/delete/getmore/command：处理命令的次数
  dirty：存储引擎中dirty数据占缓存百分比
  used：存储引擎中引擎使用缓存占百分比
  flushes：刷新到磁盘的次数
  vsize：使用的虚拟内存大小
  res：正在使用的内存大小
  qr|w：读写队列的大小，即有多少读写队列被阻塞
  ar|w：正在活动的客户端的数量，即正在进行的读写操作的客户端
  net_in：通过网络传入的字节数
  net_out：通过网络传出的字节数
  conn：打开的连接数。包括STDIN、STDOUT
  ```

- **mongotop**：观察每个Collection的负载

  - `--quiet`：隐藏所有的输出日志
  - `-h <ip:port>`
  - `-u <username>`
  - `-p <password>`
  - `--authenticationDatabase <database-name>`：需要验证的DB Name
  - `--authenticationMechanism <mechanism>`：身份验证算法。一般不用填
  - `--locks`：在打印信息中携带锁的使用信息
  - `--json`：打印json格式的输出

## 监控

## 备份

1. 使用`db.fsyncLock()`锁定数据库后，**手动拷贝数据库文件夹备份**。最后再使用`db.fsyncUnlock()`解锁数据库
2. 使用`mongodump`备份数据；使用`mongostore`导入数据

