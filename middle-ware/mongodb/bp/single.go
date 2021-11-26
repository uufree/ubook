package main

import (
	"context"
	"log"
	"time"

	"go.mongodb.org/mongo-driver/bson"
	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
	"go.mongodb.org/mongo-driver/mongo/readconcern"
	"go.mongodb.org/mongo-driver/mongo/readpref"
	"go.mongodb.org/mongo-driver/mongo/writeconcern"
)

// 常用的BSON类型：
// bson.D：一种有序的表达类型。没有特殊需求不会用
// 例如：bson.D{{"foo", "bar"}, {"hello", "world"}, {"pi", 3.14159}}
// bson.M：一种无序的表达类型。经常用，底层是map[string]interface{}，可以在查询条件中这么用：if age>0 {m[age] = 10}
// 例如：bson.M{"foo": "bar", "hello": "world", "pi": 3.14159}
// bson.A: bson数组
// 例如：bson.A{"bar", "world", 3.14159, bson.D{{"qux", 12345}}}

// TODO: mongodb使用的是UTC时间，我们使用的是CST时间，+8:00小时。建议使用timestamp进行规避

func focusStruct() {
	// FOCUS: 读取策略, 决定从那些节点读取数据
	readpref.Primary()            // 只从主节点读取，默认值
	readpref.PrimaryPreferred()   // 优先选择主节点，不可用时选择从节点
	readpref.Secondary()          // 只在从节点读取
	readpref.SecondaryPreferred() // 优先在从节点读取，从节点不可用时选择主节点
	readpref.Nearest()            // 选择附近节点

	// FOCUS: 读取策略，决定读取那些数据
	readconcern.Available()    // 读取所有可用的数据,不保证数据已写入大多数节点
	readconcern.Local()        // 读取访问实例中最新的数据
	readconcern.Majority()     // 读取在大多数节点已写入完成的数据
	readconcern.Linearizable() // 读取在所有节点上写入成功的数据
	readconcern.Snapshot()     // 读取最近快照中的数据，仅在多文档事务中使用。注：mongo会周期的生成snapshot

	// FOCUS: 写入策略
	// w: 数据成功写到n个节点，才向客户端返回确认。可选：0（发送即返回）、1（主节点写入成功即返回）、n（写入n个节点后返回）、majority（写入大多数节点后返回）
	// j: 写入journal日志持久化之后才向客户端确认。默认false
	// wTimeout: 写入超时时间
	_ = writeconcern.New(writeconcern.WMajority(), writeconcern.J(true), writeconcern.WTimeout(3*time.Second))

	// FOCUS: session。这个很少用
	so := options.SessionOptions{}
	so.SetSnapshot(true)                                     // 在只读事务中，可以设置这个选项
	so.SetCausalConsistency(false)                           // 因果一致性。因为rollback可能会丢数据，强保证
	so.SetDefaultReadConcern(readconcern.Snapshot())         // 设置读、写配置
	so.SetDefaultWriteConcern(writeconcern.New())            // 设置读、写配置
	so.SetDefaultReadPreference(readpref.PrimaryPreferred()) // 设置读、写配置
	so.SetDefaultMaxCommitTime(nil)                          // 设置事务超时时间

	// FOCUS: TransactionOptions
	to := options.TransactionOptions{}
	to.SetReadConcern(readconcern.Snapshot())         // 设置读、写配置
	to.SetWriteConcern(writeconcern.New())            // 设置读、写配置
	to.SetReadPreference(readpref.PrimaryPreferred()) // 设置读、写配置
	to.SetMaxCommitTime(nil)                          // 设置事务超时时间

	// FOCUS: aggregation options
	ao := options.AggregateOptions{}
	ao.SetAllowDiskUse(true) // 防止因集合中文档数量过大，导致内存不足

	// FOCUS: update options
	uo := options.UpdateOptions{}
	uo.SetUpsert(true) // 是否执行不存在即插入的语义

	// FOCUS: find options
	fo := options.FindOptions{}
	fo.SetAllowDiskUse(true) // 允许在内存不足时，使用临时文件
	fo.SetLimit(10)          // limit
	fo.SetSkip(10)           // skip
	fo.SetSort(bson.D{ // sort
		{"age", -1},
	})
	fo.SetProjection(bson.M{ // projection
		"name": 1,
		"age":  1,
		"_id":  0,
	})
}

func insert(collection *mongo.Collection) {
	// insert options 没啥特别的选项，无须关注
	// insert one
	result, err := collection.InsertOne(context.Background(), bson.M{
		"name": "uuchen",
		"age":  25,
		"job": bson.M{
			"company": "sensetime",
			"post":    "engineer",
		},
		"years":     bson.A{19, 20, 21},
		"timestamp": time.Now().Unix(),
	})
	if err != nil {
		log.Fatal(err)
	}
	log.Println(result.InsertedID)

	// insert many
	mresult, err := collection.InsertMany(context.Background(), []interface{}{
		bson.M{
			"name": "ppn",
			"age":  24,
			"job": bson.M{
				"company": "hnlg",
				"post":    "student",
			},
			"years":     bson.A{20, 21},
			"timestamp": time.Now().Unix(),
		},
		bson.M{
			"name": "pyw",
			"age":  25,
			"job": bson.M{
				"company": "sensetime",
				"post":    "engineer",
			},
			"years":     bson.A{19, 20, 21},
			"timestamp": time.Now().Unix(),
		},
	})
	if err != nil {
		log.Fatal(err)
	}
	log.Println(time.Now())
	log.Println(mresult.InsertedIDs)
}

func delete(collection *mongo.Collection) {
	// delete options里面没啥，无需关注
	result, err := collection.DeleteMany(context.Background(), bson.M{
		"name": bson.M{
			"$in": bson.A{"uuchen", "ppn", "pyw"},
		},
	})
	if err != nil {
		log.Fatal(err)
	}
	log.Println(result.DeletedCount)
}

func update(collection *mongo.Collection) {
	// FOCUS: 需要重点关注下options.UpdateOptions{}
	result, err := collection.UpdateOne(context.Background(),
		bson.M{"name": "uuchen"},
		bson.M{"$set": bson.M{
			"age": 26,
		}})
	if err != nil {
		log.Fatal(err)
	}
	log.Println(result)
}

func find(collection *mongo.Collection) {
	// FOCUS: find比较灵活，在使用时需要重点关注以下几个options:
	// options.FindOptions{}
	// options.FindOneOptions{}
	// options.FindOneAndDeleteOptions{}
	// options.FindOneAndReplaceOptions{}
	// options.FindOneAndUpdateOptions{}

	fo := options.FindOptions{}
	fo.SetSort(bson.D{
		{"age", -1},
	})
	fo.SetProjection(bson.M{
		"name": 1,
		"age":  1,
		"_id":  0,
	})
	cursor, err := collection.Find(context.Background(), bson.M{
		"name": bson.M{
			"$in": bson.A{"uuchen", "ppn"},
		},
	}, &fo)
	if err != nil {
		log.Fatal(err)
	}

	msg := make([]map[string]interface{}, 0)
	if err := cursor.All(context.Background(), &msg); err != nil {
		log.Fatal(err)
	}
	log.Println(msg)
}

func transaction(client *mongo.Client, collection *mongo.Collection) {
	client.UseSession(context.Background(), func(sctx mongo.SessionContext) error {
		err := sctx.StartTransaction(options.Transaction().
			SetWriteConcern(writeconcern.New(writeconcern.WMajority(),
				writeconcern.J(true),
				writeconcern.WTimeout(3*time.Second))).
			SetReadConcern(readconcern.Snapshot()))
		if err != nil {
			log.Fatal(err)
		}

		_, err = collection.UpdateMany(sctx,
			bson.M{"name": "uuchen"},
			bson.M{"$set": bson.M{"age": 26}})
		if err != nil {
			sctx.AbortTransaction(sctx)
			return err
		}

		err = sctx.CommitTransaction(sctx)
		if err != nil {
			log.Fatal(err)
		}

		return nil
	})
}

func aggregation(collection *mongo.Collection) {
	match := bson.M{
		"$match": bson.M{
			"name": bson.M{
				"$in": bson.A{"uuchen", "ppn"},
			},
		},
	}
	project := bson.M{
		"$project": bson.M{
			"name":  1,
			"age":   1,
			"years": 1,
			"_id":   0,
		},
	}

	unwind := bson.M{
		"$unwind": bson.M{
			"path":                       "$years",
			"preserveNullAndEmptyArrays": true,
		},
	}

	group := bson.M{
		"$group": bson.M{
			"_id": "$name",
			"age": bson.M{
				"$sum": "$age",
			},
			"year": bson.M{
				"$max": "$years",
			},
		},
	}

	// 注意：排序时，字段不能乱序
	sort := bson.D{
		{"$sort", bson.D{
			{"name", 1},
			{"age", -1},
			{"year", 1},
		}},
	}

	type Result struct {
		Name string `bson:"_id"`
		Age  int64  `bson:"age"`
		Year int64  `bson:"year"`
	}

	// 需要重点关注options.AggregateOptions选项
	cursor, err := collection.Aggregate(context.Background(), []interface{}{match, project, unwind, group, sort})
	if err != nil {
		log.Fatal(err)
	}
	defer cursor.Close(context.Background())

	results := make([]Result, 0)
	if err := cursor.All(context.Background(), &results); err != nil {
		log.Fatal(err)
	}
	log.Println(results)
}

func main() {
	// create client
	client, err := mongo.Connect(context.Background(),
		options.Client().ApplyURI("mongodb://tbbuser:tbbuser@10.152.208.13:27017/tbb"))
	if err != nil {
		log.Fatal(err)
	}
	defer client.Disconnect(context.Background())

	if err := client.Ping(context.Background(), nil); err != nil {
		log.Fatalf("ping failed, err: %v", err)
	}

	// FOCUS：这块需要关注下options.DatabaseOptions{}，主要是read concern和write concern
	collection := client.Database("tbb").Collection("test")

	insert(collection)
	update(collection)
	find(collection)
	transaction(client, collection)
	aggregation(collection)
	delete(collection)
}
