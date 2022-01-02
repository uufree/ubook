package main

import (
	"context"
	"google.golang.org/grpc"
	"google.golang.org/grpc/codes"
	"google.golang.org/grpc/status"
	"grpc_and_proto/gen/test/api"
	"log"
	"net"
)

type BusSearchServiceServer struct{
	*api.UnimplementedSearchServiceServer
	*api.UnimplementedStreamSearchServiceServer
}

func (ss *BusSearchServiceServer) PostSingleSearch(ctx context.Context, req *api.SearchRequest) (*api.SearchResponse, error) {
	return nil, status.Errorf(codes.OK, "success")
}
func (ss *BusSearchServiceServer) PutSingleSearch(ctx context.Context, req *api.SearchRequest) (*api.SearchResponse, error) {
	return nil, status.Errorf(codes.OK, "success")
}
func (ss *BusSearchServiceServer) GetSingleSearch(ctx context.Context, req *api.SearchRequest) (*api.SearchResponse, error) {
	return &api.SearchResponse{
		Results: []*api.SearchResponse_Result{},
	}, nil
}
func (ss *BusSearchServiceServer) DeleteSingleSearch(ctx context.Context, req *api.SearchRequest) (*api.SearchResponse, error) {
	return nil, status.Errorf(codes.OK, "success")
}
func (ss *BusSearchServiceServer) FrontStreamSearch(fss api.StreamSearchService_FrontStreamSearchServer) error {
	// stream使用方式详见grpc-go demo
	return nil
}
func (ss *BusSearchServiceServer) BackStreamSearch(req *api.SearchRequest, bss api.StreamSearchService_BackStreamSearchServer) error {
	// stream使用方式详见grpc-go demo
	return nil
}
func (ss *BusSearchServiceServer) BatchStreamSearch(bss api.StreamSearchService_BatchStreamSearchServer) error {
	// stream使用方式详见grpc-go demo
	return nil
}

func main() {
	lis, err := net.Listen("tcp", "0.0.0.0:50051")
	if err != nil {
		log.Fatalf("failed to listen: %v", err)
	}
	defer lis.Close()

	// grpc支持一些列的options，可以根据需求进行选用。常见的有：
	// 1. https
	// 2. 消息大小限制
	// 3. 连接时长限制
	serverOpts := make([]grpc.ServerOption, 0)
	grpcServer := grpc.NewServer(serverOpts...)
	log.Println("options len: ", len(serverOpts))

	bs := BusSearchServiceServer{}
	api.RegisterSearchServiceServer(grpcServer, &bs)

	grpcServer.Serve(lis)
}
