package main

import (
	"context"
	"google.golang.org/grpc"
	"grpc_and_proto/gen/test/api"
	"log"
)

func main() {
	// 出现403错误码是因为address填充了0.0.0.0:50051，导致grpc dns无法解析
	// 可能是代理配置有问题
	conn, err := grpc.Dial("localhost:50051", grpc.WithInsecure())
	if err != nil {
		log.Fatalln("failed to create conn, err: ", err)
	}
	defer conn.Close()

	client := api.NewSearchServiceClient(conn)
	rsp, err := client.GetSingleSearch(context.Background(), &api.SearchRequest{Id: "1"})
	if err != nil {
		log.Fatal(err)
	}
	log.Println(*rsp)
}
