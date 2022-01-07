package main

import (
	"context"
	"github.com/go-redis/redis/v8"
	log "github.com/sirupsen/logrus"
)

func main() {
	rdb := redis.NewClient(&redis.Options{
		Addr:     "0.0.0.0:6379",
		Password: "sensetime", // no password set
		DB:       0,           // use default DB
	})
	defer rdb.Close()

	if err := rdb.Del(context.Background(), "ts").Err(); err != nil {
		log.Fatal(err)
	}

	if err := rdb.Do(context.Background(), "ts.create", "ts", "labels", "name", "uuchen", "labels", "age", 24).Err(); err != nil {
		log.Fatal(err)
	}

	for i:=0; i<100; i++ {
		if err := rdb.Do(context.Background(), "ts.add", "ts", i, i).Err(); err != nil {
			log.Fatal(err)
		}
	}

	result, err := rdb.Do(context.Background(), "ts.get", "ts").Int64Slice()
	if err != nil {
		log.Fatal(err)
	}
	log.Infof("result: %+v", result)

	mres, err := rdb.Do(context.Background(), "ts.mget", "filter", "name=uuchen").Result()
	if err != nil {
		log.Fatal(err)
	}
	log.Infof("mres: %+v", mres)

	rres, err := rdb.Do(context.Background(), "ts.range", "ts", 0, 10).Result()
	if err != nil {
		log.Fatal(err)
	}
	log.Infof("rres: %+v", rres)

	sum, err := rdb.Do(context.Background(), "ts.range", "ts", 1, 9, "aggregation", "sum", 3).Result()
	if err != nil {
		log.Fatal(err)
	}
	log.Infof("sum: %+v", sum)
}