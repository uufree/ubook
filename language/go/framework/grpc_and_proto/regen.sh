#!/bin/bash

# gen cpp
/usr/local/bin/protoc --proto_path=./pb \
                      --proto_path=./3rd_party \
                      --proto_path=/usr/local/include \
                      --cpp_out=./gen \
                      --grpc_out=./gen \
                      --plugin=protoc-gen-grpc="$(command -v grpc_cpp_plugin)" \
                      pb/hello.proto


# gen go
# protoc默认不支持go语言，需要安装protoc-gen-go插件才能编译至go代码
/usr/local/bin/protoc --proto_path=./pb \
                      --proto_path=./3rd_party \
                      --proto_path=/usr/local/include \
                      --go_out=./gen \
                      --go-grpc_out=./gen \
                      pb/hello.proto

# gen gateway
/usr/local/bin/protoc --proto_path=./pb \
                      --proto_path=./3rd_party \
                      --proto_path=/usr/local/include \
                      --grpc-gateway_out=logtostderr=true:./gen \
                      pb/hello.proto

# gen swagger
/usr/local/bin/protoc --proto_path=./pb \
                      --proto_path=./3rd_party \
                      --proto_path=/usr/local/include \
                      --swagger_out=logtostderr=true:./doc \
                      pb/hello.proto