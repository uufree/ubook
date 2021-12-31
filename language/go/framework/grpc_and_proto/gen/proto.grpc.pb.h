// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: proto.proto
// Original file comments:
//
// protobuf优缺点：
// 优点：
// 1. 性能好、效率高
// 2. 支持多语言
// 3. 前向兼容、后向兼容左右的都很好
// 缺点：
// 1. 编码后的消息可读性比较差
//
// 用于声明当前是用的proto版本
#ifndef GRPC_proto_2eproto__INCLUDED
#define GRPC_proto_2eproto__INCLUDED

#include "proto.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/proto_utils.h>
#include <grpc++/impl/codegen/rpc_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/status.h>
#include <grpc++/impl/codegen/stub_options.h>
#include <grpc++/impl/codegen/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class RpcService;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace samples {
namespace common {

// 服务类型
class SearchService final {
 public:
  static constexpr char const* service_full_name() {
    return "samples.common.SearchService";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    virtual ::grpc::Status PostSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::samples::common::SearchResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::samples::common::SearchResponse>> AsyncPostSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::samples::common::SearchResponse>>(AsyncPostSingleSearchRaw(context, request, cq));
    }
    virtual ::grpc::Status PutSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::samples::common::SearchResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::samples::common::SearchResponse>> AsyncPutSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::samples::common::SearchResponse>>(AsyncPutSingleSearchRaw(context, request, cq));
    }
    virtual ::grpc::Status GetSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::samples::common::SearchResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::samples::common::SearchResponse>> AsyncGetSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::samples::common::SearchResponse>>(AsyncGetSingleSearchRaw(context, request, cq));
    }
    virtual ::grpc::Status DeleteSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::samples::common::SearchResponse* response) = 0;
    std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::samples::common::SearchResponse>> AsyncDeleteSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReaderInterface< ::samples::common::SearchResponse>>(AsyncDeleteSingleSearchRaw(context, request, cq));
    }
  private:
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::samples::common::SearchResponse>* AsyncPostSingleSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::samples::common::SearchResponse>* AsyncPutSingleSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::samples::common::SearchResponse>* AsyncGetSingleSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) = 0;
    virtual ::grpc::ClientAsyncResponseReaderInterface< ::samples::common::SearchResponse>* AsyncDeleteSingleSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    ::grpc::Status PostSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::samples::common::SearchResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>> AsyncPostSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>>(AsyncPostSingleSearchRaw(context, request, cq));
    }
    ::grpc::Status PutSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::samples::common::SearchResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>> AsyncPutSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>>(AsyncPutSingleSearchRaw(context, request, cq));
    }
    ::grpc::Status GetSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::samples::common::SearchResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>> AsyncGetSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>>(AsyncGetSingleSearchRaw(context, request, cq));
    }
    ::grpc::Status DeleteSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::samples::common::SearchResponse* response) override;
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>> AsyncDeleteSingleSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>>(AsyncDeleteSingleSearchRaw(context, request, cq));
    }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    ::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>* AsyncPostSingleSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>* AsyncPutSingleSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>* AsyncGetSingleSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) override;
    ::grpc::ClientAsyncResponseReader< ::samples::common::SearchResponse>* AsyncDeleteSingleSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq) override;
    const ::grpc::RpcMethod rpcmethod_PostSingleSearch_;
    const ::grpc::RpcMethod rpcmethod_PutSingleSearch_;
    const ::grpc::RpcMethod rpcmethod_GetSingleSearch_;
    const ::grpc::RpcMethod rpcmethod_DeleteSingleSearch_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status PostSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response);
    virtual ::grpc::Status PutSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response);
    virtual ::grpc::Status GetSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response);
    virtual ::grpc::Status DeleteSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response);
  };
  template <class BaseClass>
  class WithAsyncMethod_PostSingleSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_PostSingleSearch() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_PostSingleSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status PostSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestPostSingleSearch(::grpc::ServerContext* context, ::samples::common::SearchRequest* request, ::grpc::ServerAsyncResponseWriter< ::samples::common::SearchResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(0, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_PutSingleSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_PutSingleSearch() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_PutSingleSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status PutSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestPutSingleSearch(::grpc::ServerContext* context, ::samples::common::SearchRequest* request, ::grpc::ServerAsyncResponseWriter< ::samples::common::SearchResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(1, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_GetSingleSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_GetSingleSearch() {
      ::grpc::Service::MarkMethodAsync(2);
    }
    ~WithAsyncMethod_GetSingleSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestGetSingleSearch(::grpc::ServerContext* context, ::samples::common::SearchRequest* request, ::grpc::ServerAsyncResponseWriter< ::samples::common::SearchResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(2, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_DeleteSingleSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_DeleteSingleSearch() {
      ::grpc::Service::MarkMethodAsync(3);
    }
    ~WithAsyncMethod_DeleteSingleSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DeleteSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestDeleteSingleSearch(::grpc::ServerContext* context, ::samples::common::SearchRequest* request, ::grpc::ServerAsyncResponseWriter< ::samples::common::SearchResponse>* response, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncUnary(3, context, request, response, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_PostSingleSearch<WithAsyncMethod_PutSingleSearch<WithAsyncMethod_GetSingleSearch<WithAsyncMethod_DeleteSingleSearch<Service > > > > AsyncService;
  template <class BaseClass>
  class WithGenericMethod_PostSingleSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_PostSingleSearch() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_PostSingleSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status PostSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_PutSingleSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_PutSingleSearch() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_PutSingleSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status PutSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_GetSingleSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_GetSingleSearch() {
      ::grpc::Service::MarkMethodGeneric(2);
    }
    ~WithGenericMethod_GetSingleSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status GetSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_DeleteSingleSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_DeleteSingleSearch() {
      ::grpc::Service::MarkMethodGeneric(3);
    }
    ~WithGenericMethod_DeleteSingleSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status DeleteSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_PostSingleSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_PostSingleSearch() {
      ::grpc::Service::MarkMethodStreamed(0,
        new ::grpc::StreamedUnaryHandler< ::samples::common::SearchRequest, ::samples::common::SearchResponse>(std::bind(&WithStreamedUnaryMethod_PostSingleSearch<BaseClass>::StreamedPostSingleSearch, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_PostSingleSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status PostSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedPostSingleSearch(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::samples::common::SearchRequest,::samples::common::SearchResponse>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_PutSingleSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_PutSingleSearch() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::StreamedUnaryHandler< ::samples::common::SearchRequest, ::samples::common::SearchResponse>(std::bind(&WithStreamedUnaryMethod_PutSingleSearch<BaseClass>::StreamedPutSingleSearch, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_PutSingleSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status PutSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedPutSingleSearch(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::samples::common::SearchRequest,::samples::common::SearchResponse>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_GetSingleSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_GetSingleSearch() {
      ::grpc::Service::MarkMethodStreamed(2,
        new ::grpc::StreamedUnaryHandler< ::samples::common::SearchRequest, ::samples::common::SearchResponse>(std::bind(&WithStreamedUnaryMethod_GetSingleSearch<BaseClass>::StreamedGetSingleSearch, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_GetSingleSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status GetSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedGetSingleSearch(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::samples::common::SearchRequest,::samples::common::SearchResponse>* server_unary_streamer) = 0;
  };
  template <class BaseClass>
  class WithStreamedUnaryMethod_DeleteSingleSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithStreamedUnaryMethod_DeleteSingleSearch() {
      ::grpc::Service::MarkMethodStreamed(3,
        new ::grpc::StreamedUnaryHandler< ::samples::common::SearchRequest, ::samples::common::SearchResponse>(std::bind(&WithStreamedUnaryMethod_DeleteSingleSearch<BaseClass>::StreamedDeleteSingleSearch, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithStreamedUnaryMethod_DeleteSingleSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status DeleteSingleSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::samples::common::SearchResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with streamed unary
    virtual ::grpc::Status StreamedDeleteSingleSearch(::grpc::ServerContext* context, ::grpc::ServerUnaryStreamer< ::samples::common::SearchRequest,::samples::common::SearchResponse>* server_unary_streamer) = 0;
  };
  typedef WithStreamedUnaryMethod_PostSingleSearch<WithStreamedUnaryMethod_PutSingleSearch<WithStreamedUnaryMethod_GetSingleSearch<WithStreamedUnaryMethod_DeleteSingleSearch<Service > > > > StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef WithStreamedUnaryMethod_PostSingleSearch<WithStreamedUnaryMethod_PutSingleSearch<WithStreamedUnaryMethod_GetSingleSearch<WithStreamedUnaryMethod_DeleteSingleSearch<Service > > > > StreamedService;
};

class StreamSearchService final {
 public:
  static constexpr char const* service_full_name() {
    return "samples.common.StreamSearchService";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    std::unique_ptr< ::grpc::ClientWriterInterface< ::samples::common::SearchRequest>> FrontStreamSearch(::grpc::ClientContext* context, ::samples::common::SearchResponse* response) {
      return std::unique_ptr< ::grpc::ClientWriterInterface< ::samples::common::SearchRequest>>(FrontStreamSearchRaw(context, response));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::samples::common::SearchRequest>> AsyncFrontStreamSearch(::grpc::ClientContext* context, ::samples::common::SearchResponse* response, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncWriterInterface< ::samples::common::SearchRequest>>(AsyncFrontStreamSearchRaw(context, response, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientReaderInterface< ::samples::common::SearchResponse>> BackStreamSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request) {
      return std::unique_ptr< ::grpc::ClientReaderInterface< ::samples::common::SearchResponse>>(BackStreamSearchRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::samples::common::SearchResponse>> AsyncBackStreamSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderInterface< ::samples::common::SearchResponse>>(AsyncBackStreamSearchRaw(context, request, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::samples::common::SearchRequest, ::samples::common::SearchResponse>> BatchStreamSearch(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::samples::common::SearchRequest, ::samples::common::SearchResponse>>(BatchStreamSearchRaw(context));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::samples::common::SearchRequest, ::samples::common::SearchResponse>> AsyncBatchStreamSearch(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::samples::common::SearchRequest, ::samples::common::SearchResponse>>(AsyncBatchStreamSearchRaw(context, cq, tag));
    }
  private:
    virtual ::grpc::ClientWriterInterface< ::samples::common::SearchRequest>* FrontStreamSearchRaw(::grpc::ClientContext* context, ::samples::common::SearchResponse* response) = 0;
    virtual ::grpc::ClientAsyncWriterInterface< ::samples::common::SearchRequest>* AsyncFrontStreamSearchRaw(::grpc::ClientContext* context, ::samples::common::SearchResponse* response, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientReaderInterface< ::samples::common::SearchResponse>* BackStreamSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request) = 0;
    virtual ::grpc::ClientAsyncReaderInterface< ::samples::common::SearchResponse>* AsyncBackStreamSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientReaderWriterInterface< ::samples::common::SearchRequest, ::samples::common::SearchResponse>* BatchStreamSearchRaw(::grpc::ClientContext* context) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::samples::common::SearchRequest, ::samples::common::SearchResponse>* AsyncBatchStreamSearchRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    std::unique_ptr< ::grpc::ClientWriter< ::samples::common::SearchRequest>> FrontStreamSearch(::grpc::ClientContext* context, ::samples::common::SearchResponse* response) {
      return std::unique_ptr< ::grpc::ClientWriter< ::samples::common::SearchRequest>>(FrontStreamSearchRaw(context, response));
    }
    std::unique_ptr< ::grpc::ClientAsyncWriter< ::samples::common::SearchRequest>> AsyncFrontStreamSearch(::grpc::ClientContext* context, ::samples::common::SearchResponse* response, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncWriter< ::samples::common::SearchRequest>>(AsyncFrontStreamSearchRaw(context, response, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientReader< ::samples::common::SearchResponse>> BackStreamSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request) {
      return std::unique_ptr< ::grpc::ClientReader< ::samples::common::SearchResponse>>(BackStreamSearchRaw(context, request));
    }
    std::unique_ptr< ::grpc::ClientAsyncReader< ::samples::common::SearchResponse>> AsyncBackStreamSearch(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReader< ::samples::common::SearchResponse>>(AsyncBackStreamSearchRaw(context, request, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientReaderWriter< ::samples::common::SearchRequest, ::samples::common::SearchResponse>> BatchStreamSearch(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriter< ::samples::common::SearchRequest, ::samples::common::SearchResponse>>(BatchStreamSearchRaw(context));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::samples::common::SearchRequest, ::samples::common::SearchResponse>> AsyncBatchStreamSearch(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::samples::common::SearchRequest, ::samples::common::SearchResponse>>(AsyncBatchStreamSearchRaw(context, cq, tag));
    }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    ::grpc::ClientWriter< ::samples::common::SearchRequest>* FrontStreamSearchRaw(::grpc::ClientContext* context, ::samples::common::SearchResponse* response) override;
    ::grpc::ClientAsyncWriter< ::samples::common::SearchRequest>* AsyncFrontStreamSearchRaw(::grpc::ClientContext* context, ::samples::common::SearchResponse* response, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientReader< ::samples::common::SearchResponse>* BackStreamSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request) override;
    ::grpc::ClientAsyncReader< ::samples::common::SearchResponse>* AsyncBackStreamSearchRaw(::grpc::ClientContext* context, const ::samples::common::SearchRequest& request, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientReaderWriter< ::samples::common::SearchRequest, ::samples::common::SearchResponse>* BatchStreamSearchRaw(::grpc::ClientContext* context) override;
    ::grpc::ClientAsyncReaderWriter< ::samples::common::SearchRequest, ::samples::common::SearchResponse>* AsyncBatchStreamSearchRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) override;
    const ::grpc::RpcMethod rpcmethod_FrontStreamSearch_;
    const ::grpc::RpcMethod rpcmethod_BackStreamSearch_;
    const ::grpc::RpcMethod rpcmethod_BatchStreamSearch_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status FrontStreamSearch(::grpc::ServerContext* context, ::grpc::ServerReader< ::samples::common::SearchRequest>* reader, ::samples::common::SearchResponse* response);
    virtual ::grpc::Status BackStreamSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::grpc::ServerWriter< ::samples::common::SearchResponse>* writer);
    virtual ::grpc::Status BatchStreamSearch(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::samples::common::SearchResponse, ::samples::common::SearchRequest>* stream);
  };
  template <class BaseClass>
  class WithAsyncMethod_FrontStreamSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_FrontStreamSearch() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_FrontStreamSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status FrontStreamSearch(::grpc::ServerContext* context, ::grpc::ServerReader< ::samples::common::SearchRequest>* reader, ::samples::common::SearchResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestFrontStreamSearch(::grpc::ServerContext* context, ::grpc::ServerAsyncReader< ::samples::common::SearchResponse, ::samples::common::SearchRequest>* reader, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncClientStreaming(0, context, reader, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_BackStreamSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_BackStreamSearch() {
      ::grpc::Service::MarkMethodAsync(1);
    }
    ~WithAsyncMethod_BackStreamSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status BackStreamSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::grpc::ServerWriter< ::samples::common::SearchResponse>* writer) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestBackStreamSearch(::grpc::ServerContext* context, ::samples::common::SearchRequest* request, ::grpc::ServerAsyncWriter< ::samples::common::SearchResponse>* writer, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncServerStreaming(1, context, request, writer, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class WithAsyncMethod_BatchStreamSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_BatchStreamSearch() {
      ::grpc::Service::MarkMethodAsync(2);
    }
    ~WithAsyncMethod_BatchStreamSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status BatchStreamSearch(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::samples::common::SearchResponse, ::samples::common::SearchRequest>* stream) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestBatchStreamSearch(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::samples::common::SearchResponse, ::samples::common::SearchRequest>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncBidiStreaming(2, context, stream, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_FrontStreamSearch<WithAsyncMethod_BackStreamSearch<WithAsyncMethod_BatchStreamSearch<Service > > > AsyncService;
  template <class BaseClass>
  class WithGenericMethod_FrontStreamSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_FrontStreamSearch() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_FrontStreamSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status FrontStreamSearch(::grpc::ServerContext* context, ::grpc::ServerReader< ::samples::common::SearchRequest>* reader, ::samples::common::SearchResponse* response) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_BackStreamSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_BackStreamSearch() {
      ::grpc::Service::MarkMethodGeneric(1);
    }
    ~WithGenericMethod_BackStreamSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status BackStreamSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::grpc::ServerWriter< ::samples::common::SearchResponse>* writer) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithGenericMethod_BatchStreamSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_BatchStreamSearch() {
      ::grpc::Service::MarkMethodGeneric(2);
    }
    ~WithGenericMethod_BatchStreamSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status BatchStreamSearch(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::samples::common::SearchResponse, ::samples::common::SearchRequest>* stream) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  typedef Service StreamedUnaryService;
  template <class BaseClass>
  class WithSplitStreamingMethod_BackStreamSearch : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithSplitStreamingMethod_BackStreamSearch() {
      ::grpc::Service::MarkMethodStreamed(1,
        new ::grpc::SplitServerStreamingHandler< ::samples::common::SearchRequest, ::samples::common::SearchResponse>(std::bind(&WithSplitStreamingMethod_BackStreamSearch<BaseClass>::StreamedBackStreamSearch, this, std::placeholders::_1, std::placeholders::_2)));
    }
    ~WithSplitStreamingMethod_BackStreamSearch() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable regular version of this method
    ::grpc::Status BackStreamSearch(::grpc::ServerContext* context, const ::samples::common::SearchRequest* request, ::grpc::ServerWriter< ::samples::common::SearchResponse>* writer) final override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    // replace default version of method with split streamed
    virtual ::grpc::Status StreamedBackStreamSearch(::grpc::ServerContext* context, ::grpc::ServerSplitStreamer< ::samples::common::SearchRequest,::samples::common::SearchResponse>* server_split_streamer) = 0;
  };
  typedef WithSplitStreamingMethod_BackStreamSearch<Service > SplitStreamedService;
  typedef WithSplitStreamingMethod_BackStreamSearch<Service > StreamedService;
};

}  // namespace common
}  // namespace samples


#endif  // GRPC_proto_2eproto__INCLUDED