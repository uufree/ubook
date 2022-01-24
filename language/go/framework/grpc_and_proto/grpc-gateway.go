package main

import (
	"context"
	"github.com/grpc-ecosystem/grpc-gateway/v2/runtime"
	"google.golang.org/grpc/codes"
	"google.golang.org/grpc/status"
	"grpc_and_proto/gen/test/api"
	"log"
	"net/http"
	"time"
)

type BBusSearchServiceServer struct {
	*api.UnimplementedSearchServiceServer
}

func (ss *BBusSearchServiceServer) PostSingleSearch(ctx context.Context, req *api.SearchRequest) (*api.SearchResponse, error) {
	return nil, status.Errorf(codes.OK, "success")
}
func (ss *BBusSearchServiceServer) PutSingleSearch(ctx context.Context, req *api.SearchRequest) (*api.SearchResponse, error) {
	return nil, status.Errorf(codes.OK, "success")
}
func (ss *BBusSearchServiceServer) GetSingleSearch(ctx context.Context, req *api.SearchRequest) (*api.SearchResponse, error) {
	return nil, status.Errorf(codes.OK, "success")
}
func (ss *BBusSearchServiceServer) DeleteSingleSearch(ctx context.Context, req *api.SearchRequest) (*api.SearchResponse, error) {
	return nil, status.Errorf(codes.OK, "success")
}

func main() {
	// 需要什么options就配置什么options
	opts := make([]runtime.ServeMuxOption, 0)
	mux := runtime.NewServeMux(opts...)
	bs := BBusSearchServiceServer{}

	if err := api.RegisterSearchServiceHandlerServer(context.Background(), mux, &bs); err != nil {
		log.Fatalln("registry failed")
	}

	httpServer := &http.Server{
		Addr:    "localhost:9997",
		Handler: mux,
	}

	go func() {
		time.Sleep(5 * time.Second)
		httpServer.Shutdown(context.Background())
	}()

	err := httpServer.ListenAndServe()
	if err == http.ErrServerClosed {
		log.Printf("err: %+v", err)
	}
	if err != nil {
		log.Printf("err: %+v", err)
	}
}
