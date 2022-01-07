package main

import (
	"context"
	"github.com/go-redis/redis/v8"
	log "github.com/sirupsen/logrus"
)

func main() {
	//bitmap()
	//hyperloglog()
	//bloom_filter()
	//cuckoo_filter()
	//cell()
	time_series()
}

// bitmap
func bitmap() {
	rdb := redis.NewClient(&redis.Options{
		Addr:     "0.0.0.0:6379",
		Password: "sensetime", // no password set
		DB:       0,           // use default DB
	})
	defer rdb.Close()

	// inter
	if err := rdb.Del(context.Background(), "bm", "new_bm").Err(); err != nil {
		log.Fatal(err)
	}

	if err := rdb.SetBit(context.Background(), "bm", 0, 1).Err(); err != nil {
		log.Fatal(err)
	}

	v, err := rdb.GetBit(context.Background(), "bm", 0).Result()
	if err != nil {
		log.Fatal(err)
	}
	log.Infof("result: %+v", v)

	count, err := rdb.BitCount(context.Background(), "bm", &redis.BitCount{
		Start: 0,
		End:   0,
	}).Result()
	if err != nil {
		log.Fatal(err)
	}
	log.Infof("count: %+v", count)

	pos, err := rdb.BitPos(context.Background(), "bm", 1).Result()
	if err != nil {
		log.Fatal(err)
	}
	log.Infof("pos: %+v", pos)

	// outer
	if err := rdb.BitOpAnd(context.Background(), "new_bm", "bm", "bm").Err(); err != nil {
		log.Fatal(err)
	}

	if err := rdb.BitOpOr(context.Background(), "new_bm", "bm", "bm").Err(); err != nil {
		log.Fatal(err)
	}

	if err := rdb.BitOpNot(context.Background(), "new_bm", "bm").Err(); err != nil {
		log.Fatal(err)
	}

	if err := rdb.BitOpXor(context.Background(), "new_bm", "bm", "bm").Err(); err != nil {
		log.Fatal(err)
	}
}

// hyperloglog
func hyperloglog() {
	rdb := redis.NewClient(&redis.Options{
		Addr:     "0.0.0.0:6379",
		Password: "sensetime", // no password set
		DB:       0,           // use default DB
	})
	defer rdb.Close()

	if err := rdb.Del(context.Background(), "pf", "pf1").Err(); err != nil {
		log.Fatal(err)
	}

	for i := 0; i < 10000; i++ {
		if err := rdb.PFAdd(context.Background(), "pf", i).Err(); err != nil {
			log.Fatal(err)
		}
		if err := rdb.PFAdd(context.Background(), "pf1", i).Err(); err != nil {
			log.Fatal(err)
		}
	}

	count, err := rdb.PFCount(context.Background(), "pf").Result()
	if err != nil {
		log.Fatal(err)
	}
	log.Infof("pf count: %+v", count)

	count1, err := rdb.PFCount(context.Background(), "pf1").Result()
	if err != nil {
		log.Fatal(err)
	}
	log.Infof("pf1 count: %+v", count1)

	if err := rdb.PFMerge(context.Background(), "new_pf", "pf", "pf1").Err(); err != nil {
		log.Fatal(err)
	}
	newCount, err := rdb.PFCount(context.Background(), "new_pf").Result()
	if err != nil {
		log.Fatal(err)
	}
	log.Infof("new pf count: %+v", newCount)
}

// bloom filter
func bloom_filter() {
	rdb := redis.NewClient(&redis.Options{
		Addr:     "0.0.0.0:6379",
		Password: "sensetime", // no password set
		DB:       0,           // use default DB
	})
	defer rdb.Close()

	if err := rdb.Del(context.Background(), "bf").Err(); err != nil {
		log.Fatal(err)
	}

	if err := rdb.Do(context.Background(), "bf.reserve", "bf", 0.01, 10000).Err(); err != nil {
		log.Fatal(err)
	}

	for i := 0; i < 10000; i++ {
		if err := rdb.Do(context.Background(), "bf.add", "bf", i).Err(); err != nil {
			log.Fatal(err)
		}
	}

	m := make(map[int64]int64)
	for i:=0; i<20000; i++ {
		e, err := rdb.Do(context.Background(), "bf.exists", "bf", i).Result()
		if err != nil {
			log.Fatal(err)
		}

		v, ok := e.(int64)
		if !ok {
			log.Fatal(ok)
		}

		m[v] = m[v] + 1
	}

	for v, c := range m {
		log.Info(v, c)
	}
}

// cuckoo filter
func cuckoo_filter() {
	rdb := redis.NewClient(&redis.Options{
		Addr:     "0.0.0.0:6379",
		Password: "sensetime", // no password set
		DB:       0,           // use default DB
	})
	defer rdb.Close()

	if err := rdb.Del(context.Background(), "cf").Err(); err != nil {
		log.Fatal(err)
	}

	if err := rdb.Do(context.Background(), "cf.reserve", "cf", 10000, "bucketsize", 2).Err(); err != nil {
		log.Fatal(err)
	}

	for i := 0; i < 10000; i++ {
		if err := rdb.Do(context.Background(), "cf.add", "cf", i).Err(); err != nil {
			log.Fatal(err)
		}
	}

	m := make(map[int64]int64)
	for i:=0; i<20000; i++ {
		e, err := rdb.Do(context.Background(), "cf.exists", "cf", i).Result()
		if err != nil {
			log.Fatal(err)
		}

		v, ok := e.(int64)
		if !ok {
			log.Fatal(ok)
		}

		m[v] = m[v] + 1
	}

	for v, c := range m {
		log.Info(v, c)
	}
}

// cell
func cell() {
	rdb := redis.NewClient(&redis.Options{
		Addr:     "0.0.0.0:6379",
		Password: "sensetime", // no password set
		DB:       0,           // use default DB
	})
	defer rdb.Close()

	if err := rdb.Del(context.Background(), "cell").Err(); err != nil {
		log.Fatal(err)
	}

	for i:=0; i<10000; i++ {
		results, err := rdb.Do(context.Background(), "cl.throttle", "cell", 15, 30, 60).Int64Slice()
		if err != nil {
			log.Fatal(err)
		}
		log.Infof("results: %+v", results)
	}
}

// time series
func time_series() {
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
