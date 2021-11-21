# MongoDB命令篇

[TOC]

## 连接

标准的Mongo URI连接语法为：

```bash
mongodb://[username:password@]host1[:port1][,host2[:port2],...[,hostN[:portN]]][/[database][?options]]
```

- `mongodb://`：这是固定的格式，必须要指定。
- `username:password@`：可选项，如果设置，在连接数据库服务器之后，驱动都会尝试登录这个数据库
- `host1`：必须的指定至少一个host
- `portX`：可选的指定端口，如果不填，默认为27017
- `/database`：如果指定username:password@，连接并验证登录指定数据库。若不指定，默认打开 test 数据库
- `?options`：是连接选项。如果不使用/database，则前面需要加上/。

## 数据库

### 数据库方法

- 展示所有的数据库：`show dbs`
- 显示当前的数据库名称：`db`
- 切换到一个数据库：`use db-name`
- 创建一个新的数据库：`use db-name`
- 删除当前数据库：`db.dropDatabase()`

### 用户管理方案

- 验证用户是否具有当前数据库的权限：`db.auth(user, password)`

- 创建用户：`db.createUser({document})`

  ```json
  {
      "user": "root",
      "pwd": "password",
      "roles": [
          {"role": "userAdminAnyDatabase", "db": "admin"}
      ],
      "customData": {
          "name": "uuchen",
    		"age": 24,
          "email": "xxxxxx@163.com"
      }
  }
  ```

  需要注意的是，此处的**role**共有以下选项：

  - **read**：允许用户读取指定的数据库
  - **readWrite**：允许用户在指定的数据库中进行读写
  - **dbAdmin**：允许用户在指定数据库中执行管理函数，如索引创建、删除，查看统计
  - **userAdmin**：允许用户向system.users集合写入，可以在指定数据库里创建、删除和管理用户
  - **clusterAdmin**：只在admin数据库中可用，赋予用户所有分片和复制集相关函数的管理权限
  - **readAnyDatabase**：只在admin数据库中可用，赋予用户所有数据库的读权限
  - **readWriteAnyDatabase**：只在admin数据库中可用，赋予用户所有数据库的读写权限
  - **userAdminAnyDatabase**：只在admin数据库中可用，赋予用户所有数据库的userAdmin权限
  - **dbAdminAnyDatabase**：只在admin数据库中可用，赋予用户所有数据库的dbAdmin权限。
  - **root**：只在admin数据库中可用。超级账号，超级权限

  根据以上权限分类，可以将角色分为以下3种：

  - **Super Admin**
  - **业务数据库Admin**
  - **业务数据库User**

  ```javascript
  // >> 创建Super Admin
  // 注：此步骤必须在admin数据库中进行
  // 注：此步骤需要关闭mongo的权限校验
  mongo admin
  db.createUser(
      {
          user: "superadmin", 
          pwd: "superadmin", 
          roles: [
              {role: "clusterAdmin", db: "admin"}, 
              {role: "readWriteAnyDatabase", db: "admin"}, 
              {role: "userAdminAnyDatabase", db: "admin"}, 
              {role: "dbAdminAnyDatabase", db: "admin"}, 
              {role: "root", db: "admin"}
          ]
      }
  )
  // 创建成功后，退出mongo shell。加上--auth后，重启mongo，并尝试用superadmin登录
  // 注：--authenticationDatabase意思是说，在那个db中校验用户是否存在
  mongo admin -u superadmin -p superadmin --authenticationDatabase admin
  
  // >> 创建业务数据库tbb admin
  // 接上步骤，使用superadmin登录后，创建业务数据库admin
  use tbb
  db.createUser(
      {
          user: "tbbadmin", 
          pwd: "tbbadmin", 
          roles: [
              {role: "readWrite", db: "tbb"}, 
              {role: "dbAdmin", db: "tbb"}, 
              {role: "userAdmin", db: "tbb"}
          ]
      }
  )
  // 创建成功后，退出mongo shell。重新用tbbadmin登录
  mongo tbb -u tbbadmin -p tbbadmin --authenticationDatabase tbb
  
  // >> 创建业务数据库tbb user
  // 接上步骤，使用tbbadmin登录后，创建业务数据库user
  // 注：可以根据需求，确认是否需要拆分user功能：read、readWrite、readWriteAndCreateIndex
  use tbb
  db.createUser(
      {
          user: "tbbuser", 
          pwd: "tbbuser", 
          roles: [
              {role: "readWrite", db: "tbb"}, Jaeger
              {role: "dbAdmin", db: "tbb"}
          ]
      }
  )
  ```

- 展示当前数据库的用户：`show users`

- 修改用户密码：`db.changeUserPassword(root, newpassword)`

- 删除数据库用户：`db.dropUser(root)`

## 集合

- 展示当前DB的集合列表：`show collections`

- 创建集合：`db.createCollection(collection-name, {document})`

  ```json
  {
  	"capped": false, 	// 可选，是否为固定集合
  	"size": 6142100,	// 可选，固定集合的大小，单位是Bytes
  	"max": 100			// 可选，固定集合中的文档数量
  }
  ```

- 删除集合：`db.test.drop()`

- 查询集合的详细信息：`db.test.stat()`

## 文档

- 插入文档：

  - 单个：`db.test.insertOne({document})`
  - 批量：`db.test.insertMany([{document1}, {document2}])`

- 删除文档

  - 全部删除：`db.test.remove({})`
  - 针对性删除：`db.test.remove({"name": "ppn"})`
  - 仅删除1个：`db.test.remove({"name": "uuchen"}, {"justOne": true})`

- 更新操作

  > 在更新操作中，类似`$inc`这样的原地操作非常快；类似`$push`这种操作可能会导致文档位置发生变化，导致速度变慢

  - 更新范式：`db.test.update({query-document}, {update-document}, {optional-document})`

    ```json
    {
        "upsert": false, 	// 是否需要启用upsert
        "multi": false,		// 是否需要一次更新多个
  }
    ```

  - 替换更新：`db.test.update({name: "uuchen"}, {"name": "ppn", age: 24})`

  - 字段更新

    - `$set`：更新指定字段的值，若字段不存在就创建，也可以修改字段类型
    - `$unset`：删除指定类型字段

    - `$inc`：增加或者减小字段的值

    - `$push`：向已有的数组的末尾添加一个元素，如果不存在的话，就创建一个新的数组
    - `$addToSet`：将数组当做Set使用，不存在即创建

    - `$pop`：从数组中删除元素

    - `$pull`：从数组中删除指定值的元素

    ```bash
    # 在文档中新增字段
    db.test.update({"name": "uuchen"}, {"$set": {"age": 24}})
    
    # 在文档中修改字段
    db.test.update({"name": "uuchen"}, {"$set": {"age": 25}})
    
    # 在文档中修改字段类型
    db.test.update({"name": "uuchen"}, {"$set": {"age": [1,2,3]}})
    
    # 在文档中删除字段
    db.test.update({"name": "uuchen"}, {"$unset": {"age": true}})
    
    # 增加字段的值
    db.test.update({name: "uuchen"}, {$inc: {age: 1}})
    
    # 减小字段的值
    db.test.update({name: "uuchen"}, {$inc: {age: -1}})
    
    # 向文档中添加一个新的数组字段
    db.test.update({name: "uuchen"}, {$push: {gf: "ppn"}})
    
    # 往数组字段中添加子文档
    db.test.update({name: "uuchen"}, {$push: {gf: {"name": "ppn"}}})
    
    # 往数组中一次添加多个值
    db.test.update({name: "uuchen"}, {$push: {gf: {$each: ["pp1", "ppn2", "ppn3"]}}})
    
    # 限制数组大小，超过限制时将导致添加失败。当前示例中限制数组长度为10，仅保存**最先加入**的10个元素
    db.test.update({name: "uuchen"}, {$push: {gf: {$each: ["ppn11", "ppn12"], $slice: 10}}})
    
    # 限制数组大小，超过限制时将导致添加失败。当前示例中限制数组长度为10，仅保存**最后加入**的10个元素
    db.test.update({name: "uuchen"}, {$push: {gf: {$each: ["ppn11", "ppn12"], $slice: -10}}})
    
    # 向文档中添加一个新的集合字段
    db.test.update({name: "uuchen"}, {$addToSet: {gf: "ppn"}})
    
    # 向集合字段添加多个值
    db.test.update({name: "uuchen"}, {$addToSet: {gf: {$each: ["pp1", "ppn2", "ppn3"]}}})
    
    # 从数组头部删除一个元素
    db.test.update({name: "uuchen"}, {$pop: {gf: -1}})
    
    # 从数组尾部删除一个元素
    db.test.update({name: "uuchen"}, {$pop: {gf: 1}})
    
    # 从数组中删除指定值的元素
    db.test.update({name: "uuchen"}, {$pull: {gf: "ppn2"}})
    
    # 从数组中删除指定的子文档
    db.test.update({name: "uuchen"}, {$pull: {gf: {name: "ppn2"}}})
    ```

- 查询操作

  - 查询数组中所有文档：`db.test.find({})`

  - 查询数组中所有文档，并可视化打印：`db.test.find({}).pretty()`

  - 返回文档中指定的字段：`db.test.find({}, {name: 1, _id: 0, age: 1})`

  - **子文档查询**：`db.test.find({"obj.name": "obj"})`

  - 对返回结果限制条件：
    - 限制返回文档数量：`db.test.find({}).limit(1)`
    - 跳过指定数量的文档：`db.test.find({}).skip(1)`
    - 对返回结果进行升序排序：`db.test.find({}).sort({age: 1})`
    - 对返回结果进行降序排序：`db.test.find({}).sort({age: -1})`
    
  - 查询条件
    - 比较查询条件
      - `$lt`：**<**
      - `$lte`：**<=**
      - `$gt`：**>**
      - `$gte`：**>=**
      - `$eq`：**=**

      ```bash
      # 查询年龄>=18且<=35的文档
      db.test.find({age: {$gte: 18, $lte: 35}})
      ```

    - 逻辑查询条件
      - `$in`：条件匹配，in array

      - `$nin`：条件匹配，not in array

      - `$or`：**或**条件

      - `$not`：**非**条件

      - `$and`：**与**条件

      ```bash
      # 查询age为11,24,25的文档
      db.test.find({age: {$in: [11,24,25]}})
      
      # 查询age非11,24,25的文档
      db.test.find({age: {$nin: [11,24,25]}})
      
      # 查询age=25且name=uuchen的文档
      db.test.find({$and: [{age: 25}, {name: "uuchen"}]})
      
      # 查询age=24或name=uuchen的文档
      db.test.find({$or: [{age: 24}, {name: "uuchen"}]})
      
      # 查询age!=24的文档
      db.test.find({age: {$not: {$eq: 24}}})
      
      # 查询age=25且(name=uuchen或者name=ppn)的文档
      db.test.find({age: 25, $or: [{name: "uuchen"}, {name: "ppn"}]})
      ```

    - 数组查询条件
      - `$all`：数据中必须包含全部的值，才能算匹配成功
      - `$size`：查询特定长度的数组
      - `$slice`：作为find的第二个参数限制返回数组的内容
      - `$elemMatch`：遍历并匹配数组中的每一个元素

      ```bash
      # 查询数组中同时包含1,2,3
      db.test.find({bf: {$all: [1,2,3]}})
      
      # 返回指定长度的数组
      db.test.find({bf: {$size: 2}})
      
      # 查询并返回数组中的前3个值
      db.test.find({bf: {$size: 3}}, {_id: 0, bf: {$slice: 3}})
      
      # 查询并返回数组中的后3个值
      db.test.find({bf: {$size: 3}}, {_id: 0, bf: {$slice: -3}})
      
      # 查询并返回数组中第0-2个值
      db.test.find({bf: {$size: 3}}, {_id: 0, bf: {$slice: [0,2]}})
      
      # 数组子文档查询
      db.test.find({gf: {$elemMatch: {name: "ppn1"}}})
      ```

    - 特殊查询条件：

      - `$exists`：查询某个Key是否存在

      ```bash
      # 查询name这个key存在文档
      db.test.find({name: {$exists: true}})
      ```

- 常用操作

  - **日期相关**

    ```bash
    # 时间相关的操作
    # 注： Date有8个小时的时差，Timestamp没有时差
    # 注： 建议使用Date，方便展示和查询
    db.test.insert({"date": new Date()})
    db.test.insert({"date": new Timestamp()})
    db.test.find({date: {$gt: new Date(2021,01,01)}})
    ```

  - **范式化**

    > 范式化：将数据分散到多个集合中，多个集合中的数据可以互相引用
    >
    > 反范式化：内嵌子文档

    ```bash
    # 插入引用文档
    > db.test.insert({address: {$ref: "test1", $id: ObjectId("6195cdb759c0d3b79657c271")}})
    WriteResult({ "nInserted" : 1 })
    
    # 查询引用文档
    > db.test.find({address: DBRef("test1", ObjectId("6195cdb759c0d3b79657c271"))})
    { "_id" : ObjectId("6195ce0659c0d3b79657c272"), "address" : DBRef("test1", ObjectId("6195cdb759c0d3b79657c271")) }
    ```

  - ObjectId

    ```bash
    # 手动使用ObjectID创建新文档
    > db.test.insert({_id: ObjectId(), name: "uuchen"})
    WriteResult({ "nInserted" : 1 })
    ```

  - 正则表达式

    MongoDB使用**PCRE正则表达式**，使用时再做记录

  - **游标查询**
  
    详见bp

## 索引

- 获取所有的索引：`db.test.getIndexes()`
- 查看索引大小：`db.test.totalIndexSize()`
- 删除除了_id索引外的所有索引：`db.test.dropIndexes()`
- 删除指定索引：`db.test.dropIndex("name_-1")`

- 创建升序排列的单个字段索引：`db.test.createIndex({name: 1})`
- 创建降序排列的单个字段索引：`db.test.createIndex({name: -1})`
- 创建多个字段索引：`db.test.createIndex({name: 1, age: -1})`
- 创建**唯一索引**：`db.test.createIndex({name: -1}, {unique: true})`
- 创建**TTL索引**（update字段必须为Timestamp或者Date类型）：`db.test.createIndex({update: 1}, {expireAfterSeconds: 30})`
- **索引查询分析**
  - 展示查询的详细信息：`db.test.find({name: "uuchen"}).explain()`
  - 强制使用某个类型的索引：`db.test.find({name: "uuchen"}).hint({age: 1}).explain()`

## 事务

TODO...

## 聚合

聚合是一组**管道描述符**的组合使用的结果。从前往后，管道描述符的使用顺序为：

```bash
$unwind -> $match -> $project -> $group -> $sort -> $skip -> $limit
```

- `$unwind`：拆分文档中的数组内容。即将数组中的子文档拆分为文档

  ```bash
  > db.test.insert({name: "uuchen", objs: [{1: 1}, {2: 2}, {3: 3}]})
  WriteResult({ "nInserted" : 1 })
  
  > db.test.aggregate([{$unwind: "$objs"}])
  { "_id" : ObjectId("6194f903e8c86bef3b68c4be"), "name" : "uuchen", "objs" : { "1" : 1 } }
  { "_id" : ObjectId("6194f903e8c86bef3b68c4be"), "name" : "uuchen", "objs" : { "2" : 2 } }
  { "_id" : ObjectId("6194f903e8c86bef3b68c4be"), "name" : "uuchen", "objs" : { "3" : 3 } }
  ```

- `$match`：用于过滤数据，只输出符合条件的文档

  ```bash
  > db.test.aggregate([{$unwind: "$objs"}, {$match: {name: "uuchen"}}])
  { "_id" : ObjectId("6194f903e8c86bef3b68c4be"), "name" : "uuchen", "objs" : { "1" : 1 } }
  { "_id" : ObjectId("6194f903e8c86bef3b68c4be"), "name" : "uuchen", "objs" : { "2" : 2 } }
  { "_id" : ObjectId("6194f903e8c86bef3b68c4be"), "name" : "uuchen", "objs" : { "3" : 3 } }
  ```

- `$project`：修改输入文档的结构，从文档内着手，**构建`$group`步骤的输入**。可以用来重命名、增加或删除字段。除此之外，还有一些非常实用的功能。

  - 算术操作符
    - `$add [exp1, exp2, exp3...]`：exp1 + exp2 + exp3 + ...
    - `$subtract [exp1 exp2]`：exp1 - exp2
    - `$multiply [exp1, exp2, exp3...]`：exp1 * exp2 * exp3 * ...
    - `$divide [exp1, exp2]`：exp1 / exp2
    - `$mod [exp1, exp2]`：exp1 % exp2
  - 日期操作符
    - `$year`
    - `$month`
    - `$week`
    - `$dayOfMonth`
    - `$dayOfWeek`
    - `$dayofYear`
    - `$hour`
    - `$minute`
    - `$second`
  - 字符串操作符
    - `$substr [exp, startOffset, numToReturn]`
    - `$concat [exp1, exp2, exp3...]`
    - `$toLower`
    - `$toUpper`
  - 逻辑操作符：TODO，这块比较麻烦，有需要再看。详见《MongoDB权威指南》p136

  ```bash
  > db.test.aggregate([{$unwind: "$objs"}, {$match: {name: "uuchen"}}, {$project: {_id: 0, name: 1}}])
  { "name" : "uuchen" }
  { "name" : "uuchen" }
  { "name" : "uuchen" }
  
  # 文档内进行add计算
  db.test.aggregate([{$match: {name: "uuchen"}}, {$project: {name: 1, _id: 0, sum: {$add: [1,2]}}}])
  
  # 文档内进行sub计算
  db.test.aggregate([{$match: {name: "uuchen"}}, {$project: {name: 1, _id: 0, sum: {$subtract: [1,2]}}}])
  
  # 文档内进行mul计算
  db.test.aggregate([{$match: {name: "uuchen"}}, {$project: {name: 1, _id: 0, sum: {$multiply: ["$age", 2]}}}])
  
  # 文档内进行div计算
  db.test.aggregate([{$match: {name: "uuchen"}}, {$project: {name: 1, _id: 0, sum: {$divide: ["$age", 2]}}}])
  
  # 文档内进行mod计算
  db.test.aggregate([{$match: {name: "uuchen"}}, {$project: {name: 1, _id: 0, sum: {$mod: ["$age", 2]}}}])
  
  # 获取year字段
  db.test.aggregate([{$match: {name: "uuchen"}}, {$project: {name: 1, _id: 0, date: {$year: new Date()}}}])
  
  # 使用substr进行操作
  db.test.aggregate([{$match: {name: "uuchen"}}, {$project: {name: 1, _id: 0, str: {$substr: ["$name", 0, 2]}}}])
  
  # 使用concat进行操作
  db.test.aggregate([{$match: {name: "uuchen"}}, {$project: {name: 1, _id: 0, str: {$concat: ["$name", "1", "2"]}}}])
  
  # 字段小写
  db.test.aggregate([{$match: {name: "uuchen"}}, {$project: {name: 1, _id: 0, str: {$toLower: "$name"}}}])
  
  # 字段大写
  db.test.aggregate([{$match: {name: "uuchen"}}, {$project: {name: 1, _id: 0, str: {$toUpper: "$name"}}}])
  ```

- `$group`：将集合中的文档分组，从文档间着手，**重新生成文档**并统计结果。这个步骤存在很多有用的操作符，具体如下：

  - 算术操作符
    - `$sum`
    - `$avg`
  - 极值操作符
    - `$max`
    - `$min`
    - `$first`
    - `$last`
  - 数组操作符
    - `$addToSet`
    - `$push`

  ```bash
  # 使用$group重新生成文档时，必须要生成一个唯一的_id字段
  # sum：统计数量
  > db.test.aggregate([{$unwind: "$objs"}, {$match: {name: "uuchen"}}, {$project: {_id: 0, name: 1}}, {$group: {_id: "$name", count: {$sum: 1}}}])
  { "_id" : "uuchen", "count" : 3 }
  
  # sum: 计算age字段的和
  > db.test.aggregate([{$unwind: "$objs"}, {$match: {name: "uuchen"}}, {$project: {_id: 0, name: 1, age: 1}}, {$group: {_id: "$name", sum: {$sum: "$age"}}}])
  { "_id" : "uuchen", "sum" : 75 }
  
  # avg: 计算age字段的平均值
  > db.test.aggregate([{$unwind: "$objs"}, {$match: {name: "uuchen"}}, {$project: {_id: 0, name: 1, age: 1}}, {$group: {_id: "$name", avg: {$avg: "$age"}}}])
  { "_id" : "uuchen", "avg" : 25 }
  
  # max: 计算age字段的最大值
  > db.test.aggregate([{$unwind: "$objs"}, {$match: {name: "uuchen"}}, {$project: {_id: 0, name: 1, age: 1}}, {$group: {_id: "$name", max: {$max: "$age"}}}])
  { "_id" : "uuchen", "max" : 25 }
  
  # min: 计算age字段的最小值
  > db.test.aggregate([{$unwind: "$objs"}, {$match: {name: "uuchen"}}, {$project: {_id: 0, name: 1, age: 1}}, {$group: {_id: "$name", min: {$min: "$age"}}}])
  { "_id" : "uuchen", "min" : 25 }
  
  # first: 获取第一个字段的age字段
  > db.test.aggregate([{$unwind: "$objs"}, {$match: {name: "uuchen"}}, {$project: {_id: 0, name: 1, age: 1}}, {$group: {_id: "$name", first: {$first: "$age"}}}])
  { "_id" : "uuchen", "first" : 25 }
  
  # last: 获取最后一个age字段
  > db.test.aggregate([{$unwind: "$objs"}, {$match: {name: "uuchen"}}, {$project: {_id: 0, name: 1, age: 1}}, {$group: {_id: "$name", last: {$last: "$age"}}}])
  { "_id" : "uuchen", "last" : 25 }
  
  # addToSet: 将age字段聚合为集合
  > db.test.aggregate([{$unwind: "$objs"}, {$match: {name: "uuchen"}}, {$project: {_id: 0, name: 1, age: 1}}, {$group: {_id: "$name", ages: {$addToSet: "$age"}}}])
  { "_id" : "uuchen", "ages" : [ 25 ] }
  
  # push：将age字段集合为数组
  > db.test.aggregate([{$unwind: "$objs"}, {$match: {name: "uuchen"}}, {$project: {_id: 0, name: 1, age: 1}}, {$group: {_id: "$name", ages: {$push: "$age"}}}])
  { "_id" : "uuchen", "ages" : [ 25, 25, 25 ] }
  ```

- `$sort`：将输入文档排序后输出

  ```bash
  > db.test.aggregate([{$match: {name: {$in: ["uuchen", "ppn", "ppn1", "ppn2"]}}}, {$project: {name: 1, _id: 0}}, {$group: {_id: "$name", count: {$sum: 1}}}])
  { "_id" : "ppn2", "count" : 4 }
  { "_id" : "ppn", "count" : 2 }
  { "_id" : "ppn1", "count" : 2 }
  { "_id" : "uuchen", "count" : 4 }
  
  > db.test.aggregate([{$match: {name: {$in: ["uuchen", "ppn", "ppn1", "ppn2"]}}}, {$project: {name: 1, _id: 0}}, {$group: {_id: "$name", count: {$sum: 1}}}, {$sort: {_id: 1}}])
  { "_id" : "ppn", "count" : 2 }
  { "_id" : "ppn1", "count" : 2 }
  { "_id" : "ppn2", "count" : 4 }
  { "_id" : "uuchen", "count" : 4 }
  ```

- `$skip`：在聚合管道中跳过指定数量的文档，并返回余下的文档

  ```bash
  > db.test.aggregate([{$match: {name: {$in: ["uuchen", "ppn", "ppn1", "ppn2"]}}}, {$project: {name: 1, _id: 0}}, {$group: {_id: "$name", count: {$sum: 1}}}, {$sort: {_id: 1}}, {$skip: 2}])
  { "_id" : "ppn2", "count" : 4 }
  { "_id" : "uuchen", "count" : 4 }
  ```

- `$limit`：用来限制MongoDB聚合管道返回的文档数

  ```bash
  > db.test.aggregate([{$match: {name: {$in: ["uuchen", "ppn", "ppn1", "ppn2"]}}}, {$project: {name: 1, _id: 0}}, {$group: {_id: "$name", count: {$sum: 1}}}, {$sort: {_id: 1}}, {$skip: 2}, {$limit: 1}])
  { "_id" : "ppn2", "count" : 4 }
  ```

## MapReduce

比聚合更复杂的聚合，需要使用Js写Map和Reduce函数。较少使用。

## 副本集

- 展示当前的副本集状态：`rs.status()`

- 初始化副本集：`rs.initiate({_id: "mongos", members: [{_id: 0, host: "mongo-1:27017"}, {_id: 1, host: "mongo-2:27018"},{_id: 2, host: "mongo-3:27019"}]})`

- 往副本集中添加新的副本：`rs.add("mongo-4:27020")`

- 往副本集中添加**仲裁者副本**：`rs.addArb("mongo-4:27020")`

  > 仲裁者副本仅用于选举，不负责数据备份。

- 从副本集中移除副本：`rs.remove("mongo-4:27020")`

- Slave从Master中同步数据：`rs.syncFrom("mongo-2:27018")`

- 允许在Secondary上执行读操作，此步骤**必须在Secondary上执行才有效**：`rs.secondaryOk()`

  > 默认情况下，Secondary无读写权限，仅负责备份
  
- 获取集群配置：`rs.conf()`

- 更新集群配置：`rs.reconfig({document})`

  ```bash
  mongos:SECONDARY> rs.conf()
  {
  	"_id" : "mongos",
  	"version" : 1,
  	"protocolVersion" : NumberLong(1),
  	"writeConcernMajorityJournalDefault" : true,
  	"members" : [
  		{
  			"_id" : 0,
  			"host" : "mongo-1:27017",
  			"arbiterOnly" : false,	// 是否是仲裁者
  			"buildIndexes" : true,	// 是否允许build index
  			"hidden" : false,		// 是否是隐藏节点
  			// 选举优先级，默认1。范围0-1000。越大，选举优先级越高；0将被禁止参与选举
  			"priority" : 1,			
  			"tags" : {
  				
  			},
  			"slaveDelay" : NumberLong(0),	// 延迟复制时间。second
  			"votes" : 1
  		},
  		{
  			"_id" : 1,
  			"host" : "mongo-2:27018",
  			"arbiterOnly" : false,
  			"buildIndexes" : true,
  			"hidden" : false,
  			"priority" : 1,
  			"tags" : {
  				
  			},
  			"slaveDelay" : NumberLong(0),
  			"votes" : 1
  		},
  		{
  			"_id" : 2,
  			"host" : "mongo-3:27019",
  			"arbiterOnly" : false,
  			"buildIndexes" : true,
  			"hidden" : false,
  			"priority" : 1,
  			"tags" : {
  				
  			},
  			"slaveDelay" : NumberLong(0),
  			"votes" : 1
  		}
  	],
  	"settings" : {
  		"chainingAllowed" : true,
  		"heartbeatIntervalMillis" : 2000,
  		"heartbeatTimeoutSecs" : 10,
  		"electionTimeoutMillis" : 10000,
  		"catchUpTimeoutMillis" : -1,
  		"catchUpTakeoverDelayMillis" : 30000,
  		"getLastErrorModes" : {
  			
  		},
  		"getLastErrorDefaults" : {
  			"w" : 1,
  			"wtimeout" : 0
  		},
  		"replicaSetId" : ObjectId("6198d4097baed727508ddae5")
  	}
  }
  
  # 延迟复制demo
  > conf=rs.conf()
  > conf["members"][0]["priority"] = 0
  > conf["members"][0]["slaveDelay"] = 30
  > rs.reconfig(conf)
  ```

- 打印oplog的详细信息：`db.printReplicationInfo()`

- 打印复制源信息：`db.printSecondaryReplicationInfo()`

## 分片

- 查看分片集群状态：`sh.status()`

- 在集群中加入分片副本集（主+从+ARB）：`sh.addShard("cluster1/mongo-r1-1:27018,mongo-r1-2:27018,mongo-r1-3:27018")`

- 开启分片功能：

  **开启分片功能前，一定要先给在Collection上创建Index**

  - DB：`sh.enableSharding("test")`
  - Collection：`sh.shardCollection("test.test", {name: 1})`

- 查询分片路由：`db.test.find({}).explain()`

