# Nginx实战篇

[TOC]

## 长短链接

1. 不要相信HTTP/1.x版本对keep-alive的支持。有需要的话，一定要手动添加Header

2. **无论长短链接，都是由Server关闭链接**。

   - 在Client->Nginx中，由Nginx关闭链接。

   - 在Nginx->Backend中，由Backend关闭链接。

3. 避免Backend Server上TIME_WAIT过多的方法

   - 在nginx.conf的proxy block和upstream block中做长连接相关的配置。

     ```nginx
     upstream backend {
         # ip_hash;
         server 10.152.208.13:38080;
         keepalive 65536;
         keepalive_timeout 5s;
     }
     
     location ~ {
         proxy_http_version 1.1;
         proxy_set_header Connection "keep-alive";
         proxy_pass http://backend;
     }
     ```

   - 让Backend Server支持处理长连接。

     ```go
     server := &http.Server{
     	Addr:         "0.0.0.0:38080",
     	Handler:      r,
     	IdleTimeout:  5 * time.Second,
     }
     ```

4. 避免Nginx上TIME_WAIT过多的方法

   - 将每个Worker进程默认管理的FD数量由1024调整至1048576

     - 获取每个进程默认管理的FD数量：`ulimit -n`
     - 获取每个进程最多可以管理的FD数量：`cat /proc/sys/fs/nr_open`

     ```nginx
     # main block
     worker_rlimit_nofile 1048576;
     
     # events block
     events {
         worker_connections 1048576;
     }
     ```

   - 修改系统配置

     ```bash
     # 增减SYN等待队列的长度
     net.ipv4.tcp_max_syn_backlog = 8192
     
     # 表示系统同时保持TIME_WAIT套接字的最大数量。此后新增加的time wait将被直接释放
     net.ipv4.tcp_max_tw_buckets = 10000
     
     # 开启TCP时间戳
     net.ipv4.tcp_timestamps = 1
     
     # 表示开启重用。允许将TIME-WAIT sockets重新用于新的TCP连接，默认为0，表示关闭；
     net.ipv4.tcp_tw_reuse = 1
     
     # 示开启TCP连接中TIME-WAIT sockets的快速回收，默认为0，表示关闭。
     net.ipv4.tcp_tw_recycle = 1
     ```

5. 如果Nginx和Backend上keepalive的配置不一致，以谁的为准呢？

   - Nginx的keep-alive timeout > Backend keep-alive timeout，由Backend主动关闭链接

   - Nginx的keep-alive timeout < Backend keep-alive timeout，由Nginx主动关闭链接

   所以在实际应用中，建议将Backend keep-alive设置的稍小一些，将TIME_WAIT留在Backend上。

6. 上述实验资料：

   - nginx配置

     详见deploy/configs/keep-alive.conf
     
   - golang backend server
   
     详见deploy/http-demo/server.go
   
   - wireshark
   
   - postman
   
   - linux tools
   
     ```bash
     # 查询某个进程pid的TCP Socket使用状况。仅能查询LISTEN、ESTABLISHED这两个粒度比较粗的状态
     sudo lsof -p 30614 | grep TCP
     ```

## 重定向

详见deploy/configs/redirect.conf

## 静态资源

详见deploy/configs/resource.conf

## 负载均衡

> With Reverse Proxy

详见deploy/configs/load-balancing.conf

## 大文件传输

**chunked**: 详见deploy/configs/load-balancing.conf、deploy/http-demo/server.go

**content-length**: 详见deploy/configs/bigfile.conf

## 断点续传

### 上传

详见deploy/http-demo/multipart-upload-client.go

### 下载

详见deploy/http-demo/multipart-download-client.go

## HTTPS

生成证书和私钥：

```bash
# RSA，签名算法
openssl req -newkey rsa:2048 -nodes -keyout rsa_private.key -x509 -days 365 -out rsa_cert.crt

# ECDSA，签名算法
openssl ecparam -name secp384r1 -genkey -out ecc_private.key
openssl req -new -key ecc_private.key -out ecc_cert.crt
```

详见deploy/configs/https.conf

## HTTP/2

详见deploy/configs/https.conf

## WebSocket

## Open Resty

### 认证

#### session + cookie

#### token

### 权限

#### 授权

#### 鉴权

### 限流

### 熔断



- gin, swagger：1

- grpc及protoc系列插件：2

- grpc-gateway：3

- jwt：4

- casbin：5

- oauth2：6

- websocket：7

  