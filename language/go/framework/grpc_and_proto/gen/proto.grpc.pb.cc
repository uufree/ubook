// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: proto.proto

#include "proto.pb.h"
#include "proto.grpc.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/channel_interface.h>
#include <grpc++/impl/codegen/client_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/rpc_service_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/sync_stream.h>
namespace samples {
namespace common {

static const char* SearchService_method_names[] = {
  "/samples.common.SearchService/PostSingleSearch",
  "/samples.common.SearchService/PutSingleSearch",
  "/samples.common.SearchService/GetSingleSearch",
  "/samples.common.SearchService/DeleteSingleSearch",
};

std::unique_ptr< SearchService::Stub> SearchService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< SearchService::Stub> stub(new SearchService::Stub(channel));
  return stub;
}

SearchService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_PostSingleSearch_(SearchService_method_names[0], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_PutSingleSearch_(SearchService_method_names[1], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_GetSingleSearch_(SearchService_method_names[2], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_DeleteSingleSearch_(SearchService_method_names[3], ::grpc::RpcMethod::NORMAL_RPC, channel)
  {}

::grpc::Status SearchService::Stub::PostSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::samples::common::SearchResponse* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_PostSingleSearch_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>* SearchService::Stub::AsyncPostSingleSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>::Create(channel_.get(), cq, rpcmethod_PostSingleSearch_, context, request);
}

::grpc::Status SearchService::Stub::PutSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::samples::common::SearchResponse* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_PutSingleSearch_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>* SearchService::Stub::AsyncPutSingleSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>::Create(channel_.get(), cq, rpcmethod_PutSingleSearch_, context, request);
}

::grpc::Status SearchService::Stub::GetSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::samples::common::SearchResponse* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_GetSingleSearch_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>* SearchService::Stub::AsyncGetSingleSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>::Create(channel_.get(), cq, rpcmethod_GetSingleSearch_, context, request);
}

::grpc::Status SearchService::Stub::DeleteSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::samples::common::SearchResponse* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_DeleteSingleSearch_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>* SearchService::Stub::AsyncDeleteSingleSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) {
  return ::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>::Create(channel_.get(), cq, rpcmethod_DeleteSingleSearch_, context, request);
}

SearchService::Service::Service() {
  AddMethod(new ::grpc::RpcServiceMethod(
      SearchService_method_names[0],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< SearchService::Service, ::samples::common::SearchRequest, ::samples::common::SearchResponse>(
          std::mem_fn(&SearchService::Service::PostSingleSearch), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      SearchService_method_names[1],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< SearchService::Service, ::samples::common::SearchRequest, ::samples::common::SearchResponse>(
          std::mem_fn(&SearchService::Service::PutSingleSearch), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      SearchService_method_names[2],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< SearchService::Service, ::samples::common::SearchRequest, ::samples::common::SearchResponse>(
          std::mem_fn(&SearchService::Service::GetSingleSearch), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      SearchService_method_names[3],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< SearchService::Service, ::samples::common::SearchRequest, ::samples::common::SearchResponse>(
          std::mem_fn(&SearchService::Service::DeleteSingleSearch), this)));
}

SearchService::Service::~Service() {
}

::grpc::Status SearchService::Service::PostSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status SearchService::Service::PutSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status SearchService::Service::GetSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status SearchService::Service::DeleteSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


static const char* StreamSearchService_method_names[] = {
  "/samples.common.StreamSearchService/FrontStreamSearch",
  "/samples.common.StreamSearchService/BackStreamSearch",
  "/samples.common.StreamSearchService/BatchStreamSearch",
};

std::unique_ptr< StreamSearchService::Stub> StreamSearchService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< StreamSearchService::Stub> stub(new StreamSearchService::Stub(channel));
  return stub;
}

StreamSearchService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_FrontStreamSearch_(StreamSearchService_method_names[0], ::grpc::RpcMethod::CLIENT_STREAMING, channel)
  , rpcmethod_BackStreamSearch_(StreamSearchService_method_names[1], ::grpc::RpcMethod::SERVER_STREAMING, channel)
  , rpcmethod_BatchStreamSearch_(StreamSearchService_method_names[2], ::grpc::RpcMethod::BIDI_STREAMING, channel)
  {}

::grpc::ClientWriter< ::samples::common::SearchRequest>* StreamSearchService::Stub::FrontStreamSearchRaw(::grpc::ClientContext* context, ::samples::common::SearchResponse* response) {
  return new ::grpc::ClientWriter< ::samples::common::SearchRequest>(channel_.get(), rpcmethod_FrontStreamSearch_, context, response);
}

::grpc::ClientAsyncWriter< ::samples::common::SearchRequest>* StreamSearchService::Stub::AsyncFrontStreamSearchRaw(::grpc::ClientContext* context, ::samples::common::SearchResponse* response, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::ClientAsyncWriter< ::samples::common::SearchRequest>::Create(channel_.get(), cq, rpcmethod_FrontStreamSearch_, context, response, tag);
}

::grpc::ClientReader< ::samples::common::SearchResponse>* StreamSearchService::Stub::BackStreamSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request) {
  return new ::grpc::ClientReader< ::samples::common::SearchResponse>(channel_.get(), rpcmethod_BackStreamSearch_, context, request);
}

::grpc::ClientAsyncReader< ::samples::common::SearchResponse>* StreamSearchService::Stub::AsyncBackStreamSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::ClientAsyncReader< ::samples::common::SearchResponse>::Create(channel_.get(), cq, rpcmethod_BackStreamSearch_, context, request, tag);
}

::grpc::ClientReaderWriter< ::samples::common::SearchRequest, ::samples::common::SearchResponse>* StreamSearchService::Stub::BatchStreamSearchRaw(::grpc::ClientContext* context) {
  return new ::grpc::ClientReaderWriter< ::samples::common::SearchRequest, ::samples::common::SearchResponse>(channel_.get(), rpcmethod_BatchStreamSearch_, context);
}

::grpc::ClientAsyncReaderWriter< ::samples::common::SearchRequest, ::samples::common::SearchResponse>* StreamSearchService::Stub::AsyncBatchStreamSearchRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
  return ::grpc::ClientAsyncReaderWriter< ::samples::common::SearchRequest, ::samples::common::SearchResponse>::Create(channel_.get(), cq, rpcmethod_BatchStreamSearch_, context, tag);
}

StreamSearchService::Service::Service() {
  AddMethod(new ::grpc::RpcServiceMethod(
      StreamSearchService_method_names[0],
      ::grpc::RpcMethod::CLIENT_STREAMING,
      new ::grpc::ClientStreamingHandler< StreamSearchService::Service, ::samples::common::SearchRequest, ::samples::common::SearchResponse>(
          std::mem_fn(&StreamSearchService::Service::FrontStreamSearch), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      StreamSearchService_method_names[1],
      ::grpc::RpcMethod::SERVER_STREAMING,
      new ::grpc::ServerStreamingHandler< StreamSearchService::Service, ::samples::common::SearchRequest, ::samples::common::SearchResponse>(
          std::mem_fn(&StreamSearchService::Service::BackStreamSearch), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      StreamSearchService_method_names[2],
      ::grpc::RpcMethod::BIDI_STREAMING,
      new ::grpc::BidiStreamingHandler< StreamSearchService::Service, ::samples::common::SearchRequest, ::samples::common::SearchResponse>(
          std::mem_fn(&StreamSearchService::Service::BatchStreamSearch), this)));
}

StreamSearchService::Service::~Service() {
}

::grpc::Status StreamSearchService::Service::FrontStreamSearch(::grpc::ServerContext* context, ::grpc::ServerReader< ::samples::common::SearchRequest>* reader, ::samples::common::SearchResponse* response) {
  (void) context;
  (void) reader;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status StreamSearchService::Service::BackStreamSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::grpc::ServerWriter< ::samples::common::SearchResponse>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status StreamSearchService::Service::BatchStreamSearch(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::samples::common::SearchResponse, ::samples::common::SearchRequest>* stream) {
  (void) context;
  (void) stream;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace samples
}  // namespace common

