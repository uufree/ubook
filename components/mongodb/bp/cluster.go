package main

import (
	"context"
	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
	"log"
	"time"
	"github.com/google/uuid"
)

func main() {
	client, err := mongo.Connect(context.Background(),
		options.Client().ApplyURI("mongodb://10.152.208.13:27017"))
	if err != nil {
		log.Fatal(err)
	}
	defer client.Disconnect(context.Background())
	log.Println("create db handler success")

	collection := client.Database("test").Collection("test")
	log.Println("get collection success")

	start := time.Now()
	for i := 0; i < 10000; i++ {
		_, err = collection.InsertOne(context.Background(), map[string]string{
			"name": uuid.New().String(),
		})
		if err != nil {
			log.Fatal(err)
		}
	}
	log.Println(time.Now().Sub(start))
}
