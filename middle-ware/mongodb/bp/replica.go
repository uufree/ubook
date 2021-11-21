package main

import (
	"context"
	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
	"go.mongodb.org/mongo-driver/mongo/writeconcern"
	"log"
	"time"
)

// TODO: 初始化集群时，即调用rs.initiate(..)时，填的ip必须用实际的ip，要不然golang client链接不到，有点奇怪。
func main() {
	client, err := mongo.Connect(context.Background(),
		options.Client().ApplyURI("mongodb://10.152.208.13:27019,10.152.208.13:27018,10.152.208.13:27017/test/?replicaSet=mongos"))
	if err != nil {
		log.Fatal(err)
	}
	defer client.Disconnect(context.Background())

	options.Collection().SetWriteConcern(writeconcern.New(writeconcern.W(2), writeconcern.J(true), writeconcern.WTimeout(60*time.Second)))
	log.Println("create db handler success")

	collection := client.Database("test").Collection("test", options.Collection().SetWriteConcern(writeconcern.New(writeconcern.W(2), writeconcern.J(true), writeconcern.WTimeout(60*time.Second))))
	log.Println("get collection success")

	start := time.Now()
	for i := 0; i < 10000; i++ {
		_, err = collection.InsertOne(context.Background(), map[string]string{"name": "uuchen"})
		if err != nil {
			log.Fatal(err)
		}
	}
	log.Println(time.Now().Sub(start))
}
