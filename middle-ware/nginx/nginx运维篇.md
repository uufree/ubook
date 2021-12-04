# Nginx运维篇

[TOC]

## 安装

### Nginx

编译步骤：

```bash
./auto/configure
make -j10
make install
```

编译参数：

```bash
./configure -h
```

### OpenResty

> OpenResty是Nginx的一个Wrapper，在configure参数中准备了两组信息：
>
> - Nginx相关的预编译组件
> - OpenResty相关的预编译组件

编译步骤：

```bash
./configure
make -j10
make install
```

编译参数：

```bash
./configure -h
```

## 工具链

- nginx
  - `-h`：print help info
  - `-v`：print version
  - `-V`：print version and configure options
  - `-t`：测试配置项是否有效
  - `-T`：测试配置项是否有效
  - `-q`：在使用`-t`测试配置项时，仅输出error级别的错误
  - `-s single`：向master发送信号，可选：**stop**、**quit**、**reopen**、**reload**
    - stop：快速停止服务
    - quit：优雅停止服务，即处理完所有的请求再停止
    - reopen：重新打开日志文件，这样就可以old log转移到其他地方做备份
    - reload：使运行中的worker重新加载nginx.conf，优雅停止old worker
  - `-p prefix`：重新指定nginx的安装目录。
  - `-e filename`：指定error.log，默认采用{prefix}/error.log
  - `-c filename`：指定nginx.conf，默认采用{prefix}/nginx.conf
  - `-g directives`：临时指定一些全局配置项

## 配置项

### Unit

- Memory
  - `K`：1024 bytes
  - `M`：1024 MB
- Timestamp
  - `ms`
  - `s`
  - `m`
  - `h`
  - `d`
  - `w`
  - `y`

### Core Function

```nginx
# on意味着Nginx主动分发请求，收到一个请求，就唤醒一个worker
# off意味着worker主动争抢请求，收到一个请求，所有的worker都会醒来
# 高并发下，建议使用off
Syntax:		accept_mutex on | off;
Default:	accept_mutex off;
Context:	events

# 如果获取请求失败，将sleep 500ms
Syntax:		accept_mutex_delay time;
Default:	accept_mutex_delay 500ms;
Context:	events

# 是否以守护进程的方式启用服务
Syntax:		daemon on | off;
Default:	daemon on;
Context:	main

# 声明环境变量，以供后续使用
Syntax:		env variable[=value];
Default:	env TZ;
Context:	main

# 错误日志
Syntax:		error_log file [level];
Default:	error_log logs/error.log error;
Level: 		debug, info, notice, warn, error, crit, alert
Context:	main, http, mail, stream, server, location

# 事件处理模块
Syntax:		events { ... }
Default:	—
Context:	main

# http模块
Syntax:	http { ... }
Default:	—
Context:	main

# 引用其他conf子文件的
Syntax:		include file | mask;
Default:	—
Context:	any

# 加载一个第三方模块
Syntax:		load_module file;
Default:	—
Context:	main

# 进程锁文件，用于支持accept_mutex和进程共享内存访问
Syntax:		lock_file file;
Default:	lock_file logs/nginx.lock;
Context:	main

# 是否要以master/process的方式工作
Syntax:		master_process on | off;
Default:	master_process on;
Context:	main

# 一个请求到达时：off意味着只有一个process可以建立连接；on意味着所有的process都会对这个请求建立连接
Syntax:		multi_accept on | off;
Default:	multi_accept off;
Context:	events

# pid file
Syntax:		pid file;
Default:	pid logs/nginx.pid;
Context:	main

# 是否需要在worker中启用多线程进行处理。适用于多IO接口
Syntax:		thread_pool name threads=number [max_queue=number];
Default:	thread_pool default threads=32 max_queue=65536;
Context:	main

# 选择一个IO多路复用函数。一般无需设置，ngx会选择一个最优的
Syntax:		use method;
Default:	—
Method：	   select/poll/epoll/kqueue	
Context:	events

# 允许在单个进程中同时存在多少个异步IO操作
Syntax:		worker_aio_requests number;
Default:	worker_aio_requests 32;
Context:	events

# 设置一个worker进程可以同时打开的最大连接数
Syntax:		worker_connections number;
Default:	worker_connections 512;
Context:	events

# 设置worker的数量
Syntax:		worker_processes number | auto;
Default:	worker_processes 1;
Context:	main

# 设置worker进程的CPU亲和度
Syntax:		worker_cpu_affinity cpumask ...;
			worker_cpu_affinity auto [cpumask];
Default:	—
Context:	main
Samples:    worker_processes    4; worker_cpu_affinity 0001 0010 0100 1000;

# 设置进程的nice值，可选：-20~20，越小意味着优先级越高
Syntax:		worker_priority number;
Default:	worker_priority 0;
Context:	main

# 设置worker最大可以打开的文件数量
Syntax:		worker_rlimit_nofile number;
Default:	—
Context:	main
```

### Http Core Module

核心模块

```nginx
# 是否启用aio
Syntax:		aio on | off | threads[=pool];
Default:	aio off;
Context:	http, server, location

# 是否启用aio write
Syntax:		aio_write on | off;
Default:	aio_write off;
Context:	http, server, location

# 设置读取request body的buffer。如果request body超出限制，会使用临时文件保存超出部分
Syntax:		client_body_buffer_size size;
Default:	client_body_buffer_size 8k|16k;
Context:	http, server, location

# 定义读取request body的超时时间
Syntax:		client_body_timeout time;
Default:	client_body_timeout 60s;
Context:	http, server, location

# 设置request header的大小
Syntax:		client_header_buffer_size size;
Default:	client_header_buffer_size 1k;
Context:	http, server

# 设置读取request header的超时时间
Syntax:		client_header_timeout time;
Default:	client_header_timeout 60s;
Context:	http, server

# 设置request body的大小上限
Syntax:		client_max_body_size size;
Default:	client_max_body_size 1m;
Context:	http, server, location

# 设置请求默认的mime type
Syntax:		default_type mime-type;
Default:	default_type text/plain;
Context:	http, server, location

# 设置直接IO
Syntax:		directio size | off;
Default:	directio off;
Context:	http, server, location
Samples:	directio 4m;

# 定义错误页面
Syntax:		error_page code ... [=[response]] uri;
Default:	—
Context:	http, server, location, if in location
Samples:	error_page 500 502 503 504 /50x.html;

# 在一个长连接中最多可以支持多少个请求
Syntax:		keepalive_requests number;
Default:	keepalive_requests 1000;
Context:	http, server, location

# 长连接最长的有效时间（在此期间，一直有数据调用，但是未超过timeout）
Syntax:		keepalive_time time;
Default:	keepalive_time 1h;
Context:	http, server, location

# 空闲长连接的超时时间
# 0的话，意味着不支持长连接
Syntax:		keepalive_timeout timeout [header_timeout];
Default:	keepalive_timeout 75s;
Context:	http, server, location

# 控制nginx如何关闭连接
Syntax:		lingering_close off | on | always;
Default:	lingering_close on;
Context:	http, server, location

# listen选项
Syntax:		listen address[:port] [default_server] [ssl] [http2 | spdy] [proxy_protocol] [setfib=number] [fastopen=number] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [ipv6only=on|off] [reuseport] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
			listen port [default_server] [ssl] [http2 | spdy] [proxy_protocol] [setfib=number] [fastopen=number] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [ipv6only=on|off] [reuseport] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
			listen unix:path [default_server] [ssl] [http2 | spdy] [proxy_protocol] [backlog=number] [rcvbuf=size] [sndbuf=size] [accept_filter=filter] [deferred] [bind] [so_keepalive=on|off|[keepidle]:[keepintvl]:[keepcnt]];
Default:	listen *:80 | *:8000;
Context:	server
Samples: 	listen 127.0.0.1 default_server accept_filter=dataready backlog=1024;

# url请求块
Syntax:		location [ = | ~ | ~* | ^~ ] uri { ... }
			location @name { ... }
Default:	—
Context:	server, location

# 发送response body超时时间
Syntax:		send_timeout time;
Default:	send_timeout 60s;
Context:	http, server, location

# 是否启用sendfile系统调用
Syntax:		sendfile on | off;
Default:	sendfile off;
Context:	http, server, location, if in location

# 启用TCP_NODELAY选项
Syntax:		tcp_nodelay on | off;
Default:	tcp_nodelay on;
Context:	http, server, location

# 自定义DNS服务地址
Syntax:		resolver address ... [valid=time] [ipv6=on|off] [status_zone=zone];
Default:	—
Context:	http, server, location

# 自定义DNS解析超时时间
Syntax:		resolver_timeout time;
Default:	resolver_timeout 30s;
Context:	http, server, location

# 在localtion中设置针对一个resource server的root path
Syntax:		root path;
Default:	root html;
Context:	http, server, location, if in location

# 指定资源过期时间，默认不过期
Syntax:		expires time;
Default:	expires -1;
Context:	location

# 尝试寻找静态资源
Syntax:		try_files file ... uri;
			try_files file ... =code;
Default:	—
Context:	server, location

## vars
$args: arguments in the request line
$binary_remote_addr: client address in a binary form. IPv4 is 4bytes, IPv6 is 16bytes
$body_bytes_sent: number of bytes sent to a client, not counting the response header;
$bytes_sent: number of bytes sent to a client
$connection: connection serial number. 用于标识唯一的连接
$connection_requests: current number of requests made through a connection
$connection_time: connection time in seconds with a milliseconds resolution
$content_length: “Content-Length” request header field
$content_type: “Content-Type” request header field
$host: “Host” request header field
$hostname: host name
$https: “on” if connection operates in SSL mode, or an empty string otherwise
$is_args: “?” if a request line has arguments, or an empty string otherwise
$remote_addr: client address
$remote_port: client port
$request: full original request line
$request_body: request body
$request_body_file: name of a temporary file with the request body
$request_completion: “OK” if a request has completed, or an empty string otherwise
$request_id: unique request identifier generated from 16 random bytes
$request_length: request length (including request line, header, and request body)
$request_method: request method, usually “GET” or “POST”
$request_time: request processing time in seconds with a milliseconds resolution
$request_uri: full original request URI (with arguments)
$scheme: request scheme, “http” or “https”
$server_addr: an address of the server which accepted a request
$server_name: name of the server which accepted a request
$server_port: port of the server which accepted a request
$server_protocol: request protocol, usually “HTTP/1.0”, “HTTP/1.1”, or “HTTP/2.0”
$status: response status
$uri: current URI in request
```

### Http Access Module

IP规则过滤模块

```nginx
# 允许那些地址访问
Syntax:		allow address | CIDR | unix: | all;
Default:	—
Context:	http, server, location, limit_except

# 禁止那些地址访问
Syntax:		deny address | CIDR | unix: | all;
Default:	—
Context:	http, server, location, limit_except

# samples
location / {
    deny  192.168.1.1;
    allow 192.168.1.0/24;
    allow 10.1.1.0/16;
    allow 2001:0db8::/32;
    deny  all;
}
```

### Http Gunzip Module

为不支持`Content-Encoding: gzip`的浏览器提供。When send http response, decompress response body。

```nginx
# 开关
Syntax:		gunzip on | off;
Default:	gunzip off;
Context:	http, server, location

# buffer
Syntax:		gunzip_buffers number size;
Default:	gunzip_buffers 32 4k|16 8k;
Context:	http, server, location
```

### Http Gzip Module

When send http response, compress response body with *.gz。作为静态资源服务器时，可以启用此开关，用作压缩css、html等静态资源。

```nginx
# 开关
Syntax:		gzip on | off;
Default:	gzip off;
Context:	http, server, location, if in location

# buffer
Syntax:		gzip_buffers number size;
Default:	gzip_buffers 32 4k|16 8k;
Context:	http, server, location

# 设置可以压缩的最小的http version
Syntax:		gzip_http_version 1.0 | 1.1;
Default:	gzip_http_version 1.1;
Context:	http, server, location

# 设置可以压缩的最小长度（以Content-Length为准）
Syntax:		gzip_min_length length;
Default:	gzip_min_length 20;
Context:	http, server, location

# 设置可以压缩的Mime类型。
# '*'表示支持所有类型
Syntax:		gzip_types mime-type ...;
Default:	gzip_types text/html;
Context:	http, server, location
```

### Http HLS Module

将mp4和mov视频转为HLS播放。支持H.264和AAC、MP3编码。

```nginx
# 启用hls开关
Syntax:	hls;
Default:	—
Context:	location

# 设置读写video的data frame buffer
Syntax:		hls_buffers number size;
Default:	hls_buffers 8 2m;
Context:	http, server, location

# 支持额外的播放参数
Syntax:		hls_forward_args on | off;
Default:	hls_forward_args off;
Context:	http, server, location
Samples:	test.mp4.ts?start=0.000&end=9.333&a=1&b=2

# ts文件分段长度
Syntax:		hls_fragment time;
Default:	hls_fragment 5s;
Context:	http, server, location

# 设置用于处理视频文件的初始buffer
Syntax:		hls_mp4_buffer_size size;
Default:	hls_mp4_buffer_size 512k;
Context:	http, server, location

# 视频元数据（Moov Atom）大小最大不能超过10MB
Syntax:		hls_mp4_max_buffer_size size;
Default:	hls_mp4_max_buffer_size 10m;
Context:	http, server, location

# samples
location / {
    hls;
    hls_fragment            5s;
    hls_buffers             10 10m;
    hls_mp4_buffer_size     1m;
    hls_mp4_max_buffer_size 5m;
    root /var/video/;
}
```

### Http Image Filter Module

转换图片格式，大小、旋转等

```nginx
# 图片格式处理
Syntax:		image_filter off;
			image_filter test;
			image_filter size;
			image_filter rotate 90 | 180 | 270;
			image_filter resize width height;
			image_filter crop width height;
Default:	image_filter off;
Context:	location

# 设置图片格式处理buffer大小
Syntax:		image_filter_buffer size;
Default:	image_filter_buffer 1M;
Context:	http, server, location

# 设置jpeg图像质量，可选1-100。值越大，图像质量越好
Syntax:		image_filter_jpeg_quality quality;
Default:	image_filter_jpeg_quality 75;
Context:	http, server, location

# 设置webp图像质量，可选1-100。值越大，图像质量越好
Syntax:		image_filter_webp_quality quality;
Default:	image_filter_webp_quality 80;
Context:	http, server, location

# samples
location /img/ {
    proxy_pass   http://backend;
    image_filter resize 150 100;
    image_filter rotate 90;
    error_page   415 = /empty;
}
```

### Http Index Module

处理`/`请求

```nginx
Syntax:		index file ...;
Default:	index index.html;
Context:	http, server, location

# samples
location / {
    index index.$geo.html index.html;
}
```

### Http Limit Conn Module

根据预定义的Key，限制连接数

```nginx
# $binary_remote_addr是http core module中的一个变量
# 申请一块空间，存放key
# 以sample为例，$binary_remote_addr占用4字节，大约可存放10×1024×1024/4个conn。超过时会报limit_conn_status描述的错误
Syntax:		limit_conn_zone key zone=name:size;
Default:	—
Context:	http
Samples:	limit_conn_zone $binary_remote_addr zone=addr:10m;

# 配置连接限制
Syntax:		limit_conn zone number;
Default:	—
Context:	http, server, location

# 超出连接限制时，返回的http status code
Syntax:		limit_conn_status code;
Default:	limit_conn_status 503;
Context:	http, server, location

# samples
http {
    limit_conn_zone $binary_remote_addr zone=addr:10m;

    ...

    server {

        ...

        location /download/ {
            limit_conn addr 1;
        }
        
## Vars
$limit_conn_status: such as PASSED, REJECTED, or REJECTED_DRY_RUN
```

### Http Limit Req Module

根据预定义的Key，限制请求数量

```nginx
# 声明限制请求的zone
Syntax:		limit_req_zone key zone=name:size rate=rate [sync];
Default:	—
Context:	http
Samples: 	limit_req_zone $binary_remote_addr zone=one:10m rate=1r/s;

# 限制请求
# burst:   相当于漏桶缓冲区，缓存部分请求
# nodelay: 当请求数量超过burst时，将立刻返回
Syntax:		limit_req zone=name [burst=number] [nodelay | delay=number];
Default:	—
Context:	http, server, location

# 请求被拒绝时，返回的错误
Syntax:		limit_req_status code;
Default:	limit_req_status 503;
Context:	http, server, location

# samples
http {
    limit_req_zone $binary_remote_addr zone=one:10m rate=1r/s;

    ...

    server {

        ...

        location /search/ {
            limit_req zone=one burst=5 nodelay;
        }
        
$limit_req_status: such as PASSED, DELAYED, REJECTED, DELAYED_DRY_RUN, or REJECTED_DRY_RUN 
```

### Http Log Module

```nginx
# 需要了再补充
```

### Http Mp4 Module

```nginx
# 需要了再补充
```

### Http Proxy Module

```nginx
# 是否缓存backend server的response
Syntax:		proxy_buffering on | off;
Default:	proxy_buffering on;
Context:	http, server, location

# 设置从backend server中，为读取response header准备的buffer大小
Syntax:		proxy_buffer_size size;
Default:	proxy_buffer_size 4k|8k;
Context:	http, server, location

# 对单个链接，设置可以从backend server中读取response的缓冲大小
Syntax:		proxy_buffers number size;
Default:	proxy_buffers 8 4k|8k;
Context:	http, server, location

# 对单个链接，在客户端还没有读取response之前，限制缓存response的大小
Syntax:		proxy_busy_buffers_size size;
Default:	proxy_busy_buffers_size 8k|16k;
Context:	http, server, location

# 隐藏请求中的部分header
Syntax:		proxy_hide_header field;
Default:	—
Context:	http, server, location

# 忽略请求中的部分header
Syntax:		proxy_ignore_headers field ...;
Default:	—
Context:	http, server, location

# 如果启用了buffering，并且backend server response的大小超出限制，就会将数据写入临时文件
Syntax:		proxy_max_temp_file_size size;
Default:	proxy_max_temp_file_size 1024m;
Context:	http, server, location

# 将请求代理到那个backend server上
Syntax:		proxy_pass URL;
Default:	—
Context:	location, if in location, limit_except

# 定义连接到backend server的超时时间
Syntax:		proxy_connect_timeout time;
Default:	proxy_connect_timeout 60s;
Context:	http, server, location

# 定义从backend server中读取数据的超时时间
Syntax:		proxy_read_timeout time;
Default:	proxy_read_timeout 60s;
Context:	http, server, location

# 定义将请求发送给backend server的超时时间
Syntax:		proxy_send_timeout time;
Default:	proxy_send_timeout 60s;
Context:	http, server, location

# 请求转发给backend server时，可以增加一些header
Syntax:		proxy_set_header field value;
Default:	proxy_set_header Host $proxy_host;
			proxy_set_header Connection close;
Context:	http, server, location

# 设置http version。支持长连接时，需要设置成1.1
Syntax:		proxy_http_version 1.0 | 1.1;
Default:	proxy_http_version 1.0;
Context:	http, server, location

# cache...
# cookie...
# ssl...

# env
$proxy_host: name and port of a proxied server
$proxy_port: port of a proxied server
$proxy_add_x_forwarded_for

# samples
location / {
    proxy_pass       http://localhost:8000;
    proxy_set_header Host      $host;
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
}
```

### Http Rewrite Module

用PCRE正则修改请求的URI

```nginx
# return
Syntax:		return code [text];
			return code URL;
			return URL;
Default:	—
Context:	server, location, if

# set
Syntax:		set $variable value;
Default:	—
Context:	server, location, if

# 重写uri
# flag可选：
# 	last：停止处理当前的ngx_http_rewrite_module的指令集，并开始搜索与更改后的URI相匹配的location;
# 	break：停止处理当前的ngx_http_rewrite_module指令集
# last和break一样，它们都会终止此location中其他它rewrite模块指令的执行，但是last立即发起新一轮的 location匹配而break则不会
Syntax:		rewrite regex replacement [flag];
Default:	—
Context:	server, location, if

# samples
location ~ /api/v1/keyword {
	proxy_set_header Host $host;
    proxy_set_header X-Real-IP $remote_addr;
    proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;

    rewrite ^/api/v1/keyword/(.*)$ /v1/$1 break;
    proxy_pass  http://task-manager:8002;
}

location / {
    rewrite ^/test1 /test2;
    rewrite ^/test2 /test3 last;  # 此处发起新一轮location匹配 uri为/test3
    rewrite ^/test3 /test4;
    proxy_pass http://www.baidu.com;
}

location = /test2 {
    return 200 "/test2";
}  

location = /test3 {
    return 200 "/test3";
}
location = /test4 {
    return 200 "/test4";
}
# 发送如下请求
# curl 127.0.0.1:8080/test1
# /test3 
```

### Http SSL Module

支持HTTPS

```nginx
# 使用时再做笔记

# 是否启用https
Syntax:		ssl on | off;
Default:	ssl off;
Context:	http, server

# pem file
Syntax:		ssl_certificate file;
Default:	—
Context:	http, server

# key file
Syntax:		ssl_certificate_key file;
Default:	—
Context:	http, server

# 指定密码格式
Syntax:		ssl_ciphers ciphers;
Default:	ssl_ciphers HIGH:!aNULL:!MD5;
Context:	http, server

# Samples
http {

    ...

    server {
        listen              443 ssl;
        keepalive_timeout   70;

        ssl_protocols       TLSv1 TLSv1.1 TLSv1.2;
        ssl_ciphers         AES128-SHA:AES256-SHA:RC4-SHA:DES-CBC3-SHA:RC4-MD5;
        ssl_certificate     /usr/local/nginx/conf/cert.pem;
        ssl_certificate_key /usr/local/nginx/conf/cert.key;
        ssl_session_cache   shared:SSL:10m;
        ssl_session_timeout 10m;

        ...
    }

## vars    
# 需要了再补充    
```

### Http Upstream Module

负载均衡模块

```nginx
# 声明upstream module
Syntax:		upstream name { ... }
Default:	—
Context:	http

# 在upstream context中描述一个server
Syntax:		server address [parameters];
Default:	—
Context:	upstream
Parameters: 
	weight=number: 		设置服务的权重
	max_conns=number: 	限制最大的连接数
	max_fails=number: 	限制最大失败，如果超过阈值，将在fail_timeout内无法接受新的链接
	fail_timeout=time:	失败等待时间
	backup: 			将server标记为备选，如果主服务不可用，将使用它作为备选链接
	down:				将server标记为下线
	resolve:			采用自定义的DNS解析器
	slow_start=time: 	缓慢启动，防止超载

# 暂时将请求缓存在queue中
Syntax:		queue number [timeout=time];
Default:	—
Context:	upstream

# 负载均衡: hash
Syntax:		hash key [consistent];
Default:	—
Context:	upstream

# 负载均衡: ip hash
Syntax:		ip_hash;
Default:	—
Context:	upstream

# 负载均衡: 最少链接
Syntax:		least_conn;
Default:	—
Context:	upstream

# 负载均衡: 响应时间最快+活动链接最少
Syntax:		least_time header | last_byte | last_byte inflight;
Default:	—
Context:	upstream
Explain:
	header: Time to receive the first byte from the server
	last_byte: Time to receive the full response from the server
	last_byte inflight: Time to receive the full response from the server, taking into account incomplete requests

# 负载均衡: 随机选择
Syntax:		random [two [method]];
Default:	—
Context:	upstream

# 支持的keepalive链接数量。建议小于总的FD数量
Syntax:		keepalive connections;
Default:	—
Context:	upstream

# 一个长链接最多可以处理多少请求
Syntax:		keepalive_requests number;
Default:	keepalive_requests 1000;
Context:	upstream

# 一个长连接多长可以存在多久
Syntax:		keepalive_time time;
Default:	keepalive_time 1h;
Context:	upstream

# keepalive timeout
Syntax:		keepalive_timeout timeout;
Default:	keepalive_timeout 60s;
Context:	upstream

# sticky cookie...

# Samples
upstream backend {
    server backend1.example.com       weight=5;
    server backend2.example.com:8080;
    server unix:/tmp/backend3;

    server backup1.example.com:8080   backup;
    server backup2.example.com:8080   backup;
}

server {
    location / {
        proxy_pass http://backend;
    }
}

## vars
$upstream_addr: keeps the IP address and port of the upstream server
$upstream_bytes_received: number of bytes received from an upstream server
$upstream_bytes_sent: number of bytes sent to an upstream server
$upstream_cache_status: “MISS”, “BYPASS”, “EXPIRED”, “STALE”, “UPDATING”, “REVALIDATED”, or “HIT”
$upstream_connect_time: keeps time spent on establishing a connection with the upstream server 
$upstream_header_time: keeps time spent on receiving the response header from the upstream server
$upstream_response_length: keeps the length of the response obtained from the upstream server
$upstream_response_time: keeps time spent on receiving the response from the upstream server;
$upstream_status: keeps status code of the response obtained from the upstream server
```

### Http V2 Module

支持http/2协议

```nginx
# 为每个request body设置预读的buffer
Syntax:		http2_body_preread_size size;
Default:	http2_body_preread_size 64k;
Context:	http, server

# 设置response chunk的大小
Syntax:		http2_chunk_size size;
Default:	http2_chunk_size 8k;
Context:	http, server, location

# 设置空闲链接的超时时间
Syntax:		http2_idle_timeout time;
Default:	http2_idle_timeout 3m;
Context:	http, server

# 限制一个连接中的最大并发推请求数量
Syntax:		http2_max_concurrent_pushes number;
Default:	http2_max_concurrent_pushes 10;
Context:	http, server

# 限制一个连接中的最大并发流数量
Syntax:		http2_max_concurrent_streams number;
Default:	http2_max_concurrent_streams 128;
Context:	http, server

# 限制header中的最大字段大小
Syntax:		http2_max_field_size size;
Default:	http2_max_field_size 4k;
Context:	http, server

# 限制header的最大大小
Syntax:		http2_max_header_size size;
Default:	http2_max_header_size 16k;
Context:	http, server

# 限制一个连接中最多可以处理的请求数量
Syntax:		http2_max_requests number;
Default:	http2_max_requests 1000;
Context:	http, server

# 设置recv buffer size
Syntax:		http2_recv_buffer_size size;
Default:	http2_recv_buffer_size 256k;
Context:	http

# 设置recv timeout
Syntax:		http2_recv_timeout time;
Default:	http2_recv_timeout 30s;
Context:	http, server
```

## 配置文件

### Framework

```nginx
user nobody; # a directive in the 'main' context

events {
    # configuration of connection processing
}

http {
    # Configuration specific to HTTP and affecting all virtual servers  

    server {
        # configuration of HTTP virtual server 1       
        location /one {
            # configuration for processing URIs starting with '/one'
        }
        location /two {
            # configuration for processing URIs starting with '/two'
        }
    } 
    
    server {
        # configuration of HTTP virtual server 2
    }
}

# 无需关注
stream {
    # Configuration specific to TCP/UDP and affecting all virtual servers
    server {
        # configuration of TCP virtual server 1 
    }
}

mail {
    # Configuration mail
    server {
    }
}
```











