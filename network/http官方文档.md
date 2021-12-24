# HTTP

[TOC]

## Method

- **CONNECT**

  开启一个客户端与所请求资源之间的双向沟通的通道。

  ```
  CONNECT server.example.com:80 HTTP/1.1
  Host: server.example.com:80
  Proxy-Authorization: basic aGVsbG86d29ybGQ=
  ```

- **DELETE**

  删除指定的资源

  ```
  DELETE /file.html HTTP/1.1
  ```

- **GET**

  获取指定资源

  ```
  GET /index.html
  ```

- **HEAD**

  获取请求资源的metadata

  ```
  HEAD /index.html
  ```

- **OPTIONS**

  获取资源支持的请求方法

  ```
  OPTIONS /index.html HTTP/1.1
  ```

- **PATCH**

  对资源进行部分修改

  ```
  PATCH /file.txt HTTP/1.1
  ```

- **POST**

  发送数据给服务器，由`Content-Type`指定发送的数据类型

  ```
  # post 1
  POST / HTTP/1.1
  Host: foo.com
  Content-Type: application/x-www-form-urlencoded
  Content-Length: 13
  
  say=Hi&to=Mom
  
  
  # post 2
  POST /test.html HTTP/1.1
  Host: example.org
  Content-Type: multipart/form-data;boundary="boundary"
  
  --boundary
  Content-Disposition: form-data; name="field1"
  
  value1
  --boundary
  Content-Disposition: form-data; name="field2"; filename="example.txt"
  
  value2
  ```

- **PUT**

  创建或者替换目标资源

  ```
  PUT /new.html HTTP/1.1
  Host: example.com
  Content-type: text/html
  Content-length: 16
  
  <p>New File</p>
  ```

- **TRACE**

  获取数据发往服务器的过程中，通过的IP节点

  ```
  TRACE /index.html
  ```

## Header

### 通用

- **Cache-Control**

  通过指定指令来实现缓存机制。缓存指令是单向的，这意味着在请求中设置的指令，不一定被包含在响应中。

  ```
  # 缓存请求指令：
  Cache-Control: max-age=<seconds>
  Cache-Control: max-stale[=<seconds>]
  Cache-Control: min-fresh=<seconds>
  Cache-control: no-cache
  Cache-control: no-store
  Cache-control: no-transform
  Cache-control: only-if-cached
  
  # 缓存响应指令：
  Cache-control: must-revalidate
  Cache-control: no-cache
  Cache-control: no-store
  Cache-control: no-transform
  Cache-control: public
  Cache-control: private
  Cache-control: proxy-revalidate
  Cache-Control: max-age=<seconds>
  Cache-control: s-maxage=<seconds>
  
  # 缓存拓展指令：
  Cache-control: immutable
  Cache-control: stale-while-revalidate=<seconds>
  Cache-control: stale-if-error=<seconds>
  ```

  指令详解：

  - 可缓存性
    - **public**：表明响应可以被任意客户端缓存，包括代理、浏览器
    - **private**：表明响应只能被浏览器缓存，不能放在代理上与别人共享
    - **no-cache**：强制要求缓存把请求提交给原始服务器进行验证(协商缓存验证)
    - **no-store**：不使用任何缓存
  - 到期
    - **max-age=<seconds>**：设置缓存存储的最大周期，超过这个时间缓存被认为过期
    - **max-stale[=<seconds>]**：表明客户端愿意接收一个已经过期的资源。可以设置一个可选的秒数，表示响应不能已经过时超过该给定的时间。
    - **min-fresh=<seconds>**：表示客户端希望获取一个能在指定的秒数内保持其最新状态的响应
  - 其他
    - **no-transform**：禁止代理对数据做优化（例如图像格式转化等）
    - **must-revalidate**：浏览器缓存到期后，必须回源
    - **proxy-revalidate**：浏览器缓存到期后，验证到代理这一层即可

- **Connection**

  当前的事务完成后，是否会关闭网络连接

  ```
  Connection: keep-alive
  Connection: close
  ```

- **Date**

  报文创建的日期和时间

  ```
  Date: <day-name>, <day> <month> <year> <hour>:<minute>:<second> GMT
  ```

- **Transfer-Encoding**

  指明了将entity传递给用户采用的编码方式

  ```
  Transfer-Encoding: chunked
  Transfer-Encoding: compress
  Transfer-Encoding: deflate
  Transfer-Encoding: gzip
  Transfer-Encoding: identity
  
  // Several values can be listed, separated by a comma
  Transfer-Encoding: gzip, chunked
  ```

- **Upgrade**

  主要用于协议升级

  ```
  GET /index.html HTTP/1.1
  Host: www.example.com
  Connection: upgrade
  Upgrade: example/1, foo/2
  ```

- **Via**

  由代理服务器添加的，适用于正向和反向代理，在请求和响应首部中均可出现。这个消息首部可以用来追踪消息转发情况，防止循环请求，以及识别在请求或响应传递链中消息发送者对于协议的支持能力

  ```
  Via: 1.1 vegur
  Via: HTTP/1.1 GWA
  Via: 1.0 fred, 1.1 p.example.net
  ```

### 请求

- **Accept**

  将客户端可以处理的内容类型（MIME）通知给服务端

  ```
  Accept: text/html
  Accept: image/*
  Accept: text/html, application/xhtml+xml, application/xml;q=0.9, */*;q=0.8
  ```

- **Accept-Charset**

  将客户端可以处理的字符集类型通知给服务端

  ```
  Accept-Charset: iso-8859-1
  Accept-Charset: utf-8, iso-8859-1;q=0.5
  Accept-Charset: utf-8, iso-8859-1;q=0.5, *;q=0.1
  ```

- **Accept-Encoding**

  将客户端可以理解的编码方式（通常是某种压缩算法）通知给服务端

  ```
  Accept-Encoding: gzip
  Accept-Encoding: compress
  Accept-Encoding: deflate
  Accept-Encoding: br
  Accept-Encoding: identity
  Accept-Encoding: *
  ```

- **Accept-Language**

  将客户端支持的语种发送给服务端

  ```
  Accept-Language: de
  Accept-Language: de-CH
  Accept-Language: en-US,en;q=0.5
  Accept-Language: zh-CN,zh;q=0.8,zh-TW;q=0.7,zh-HK;q=0.5,en-US;q=0.3,en;q=0.2
  ```

- **Authorization**

  用于验证用户代理身份的凭证，通常会在服务器返回401（Unauthorized）的后续请求中发送此消息头

  ```
  Authorization: Basic YWxhZGRpbjpvcGVuc2VzYW1l
  ```

- **Host**

  请求将要发送到的服务器主机名和端口号

  ```
  Host: developer.mozilla.org
  ```

- **If-Match**

  在请求方法为**GET**和**HEAD**的情况下，服务器仅在请求资源满足Request Header中的**ETag**时才会返回资源

  ```
  If-Match: "bfc13a64729c4290ef5b2c2730249c88ca92d82d"
  If-Match: W/"67ab43", "54ed21", "7892dd"
  If-Match: *
  ```

- **If-None-Match**

  在请求方法为**GET**和**HEAD**的情况下，服务器仅在请求资源不满足Request Header中的**ETag**时才会返回资源

  ```
  If-None-Match: "bfc13a64729c4290ef5b2c2730249c88ca92d82d"
  If-None-Match: W/"67ab43", "54ed21", "7892dd"
  If-None-Match: *
  ```

- **If-Modified-Since**

  服务器只在所请求的资源在给定的日期时间之后，且对内容进行过修改的情况下才会将资源返回

  ```
  If-Modified-Since: Wed, 21 Oct 2015 07:28:00 GMT
  ```

- **If-Unmodified-Since**

  服务器只在所请求的资源在给定的日期时间之后，且没有对内容进行过修改的情况下才会将资源返回

  ```
  If-Unmodified-Since: Wed, 21 Oct 2015 07:28:00 GMT
  ```

- **Range**

  请求首部，告知服务器返回文件的哪一部分

  ```
  Range: <unit>=<range-start>-
  Range: <unit>=<range-start>-<range-end>
  Range: <unit>=<range-start>-<range-end>, <range-start>-<range-end>
  Range: <unit>=<range-start>-<range-end>, <range-start>-<range-end>, <range-start>-<range-end>
  ```

- **X-Forwarded-For**

  在客户端访问服务器的过程中如果需要经过HTTP代理或者负载均衡服务器，可以被用来获取最初发起请求的客户端的IP地址，这个消息首部成为事实上的标准

  ```
  X-Forwarded-For: 2001:db8:85a3:8d3:1319:8a2e:370:7348
  X-Forwarded-For: 203.0.113.195
  X-Forwarded-For: 203.0.113.195, 70.41.3.18, 150.172.238.178
  ```

- **X-Forwarded-Host**

  确定客户端发起的请求中使用**Host**指定的初始域名

  ```
  X-Forwarded-Host: id42.example-cdn.com
  ```

- **X-Forwarded-Proto**

  确定客户端与代理服务器或者负载均衡服务器之间的连接所采用的传输协议

  ```
  X-Forwarded-Proto: https
  ```

- **X-Real-IP**

  客户端真实IP

  ```
  X-Real-IP: 203.0.113.195
  ```
  
- **User-Agent**

  包含了一个特征字符串，用来让网络协议的对端来识别发起请求的用户代理软件的应用类型、操作系统、软件开发商以及版本号

  ```
  User-Agent: Mozilla/<version> (<system-information>) <platform> (<platform-details>) <extensions>
  ```

- **Cookie**

  请求首部，存放`Set-Cookie` Response Header中返回的Cookie

  ```
  Cookie: <cookie-list>
  Cookie: name=value
  Cookie: name=value; name2=value2; name3=value3
  ```

### 响应

- **Accept-Ranges**

  服务器标识自身支持范围请求

  ```
  Accept-Ranges: bytes
  Accept-Ranges: none
  ```

- **Age**

  对象在缓存代理中存贮的时长，以秒为单位。Age的值通常接近于0。表示此对象刚刚从原始服务器获取不久；其他的值则是表示代理服务器当前的系统时间与此应答中的**通用头Date**的值之差。

  ```
  Age: 24
  ```

- **ETag**

  资源的特定版本的标识符。如果内容没有改变，Web服务器不需要发送完整的响应。而如果内容发生了变化，使用ETag有助于防止资源的同时更新相互覆盖

  ```
  ETag: "33a64df551425fcc55e4d42a148795d9f25f89d4"
  ETag: W/"0815"
  ```

- **Location**

  首部指定的是需要将页面重新定向至的地址。一般在响应码为3xx的响应中才会有意义

  ```
  Location: /index.html
  ```

- **Proxy-Authenticate**

- **Proxy-Authentication**

  指定从代理服务器上获取资源而采用的身份认证方式

  ```
  Proxy-Authenticate: Basic
  Proxy-Authenticate: Basic realm="Access to the internal site"
  ```

- **WWW-Authenticate**

  定义了使用何种验证方式去获取对资源的连接

  ```
  WWW-Authenticate: Basic
  WWW-Authenticate: Basic realm="Access to the staging site"
  ```

- **Retry-After**

  表示用户需要等待多长时间之后才能继续发送请求

  ```
  Retry-After: <http-date>
  Retry-After: <delay-seconds>
  ```

- **Server**

  处理请求的源头服务器所用到的软件相关信息

  ```
  Server: Apache/2.4.1 (Unix)
  ```

- **Set-Cookie**

  由服务器端向客户端发送 cookie

  ```
  Set-Cookie: <cookie-name>=<cookie-value>
  Set-Cookie: <cookie-name>=<cookie-value>; Expires=<date>
  Set-Cookie: <cookie-name>=<cookie-value>; Max-Age=<non-zero-digit>
  Set-Cookie: <cookie-name>=<cookie-value>; Domain=<domain-value>
  Set-Cookie: <cookie-name>=<cookie-value>; Path=<path-value>
  Set-Cookie: <cookie-name>=<cookie-value>; Secure
  Set-Cookie: <cookie-name>=<cookie-value>; HttpOnly
  
  Set-Cookie: <cookie-name>=<cookie-value>; SameSite=Strict
  Set-Cookie: <cookie-name>=<cookie-value>; SameSite=Lax
  
  // Multiple directives are also possible, for example:
  Set-Cookie: <cookie-name>=<cookie-value>; Domain=<domain-value>; Secure; HttpOnly
  ```

- **Digest**

  提供了请求资源一个摘要

  ```
  Digest: <digest-algorithm>=<digest-value>
  Digest: <digest-algorithm>=<digest-value>,<digest-algorithm>=<digest-value>
  ```

- **Clear-Site-Data**

  清除当前请求网站有关的浏览器数据（cookie，存储，缓存）

  ```
  Clear-Site-Data: "cache"
  Clear-Site-Data: "cache", "cookies"
  Clear-Site-Data: "*"
  ```

### 实体

- **Allow**

  用于枚举资源所支持的 HTTP 方法的集合。如果服务器返回405（Method Not Allow），这个字段务必需要填充。

  ```
  Allow: GET, POST, HEAD
  ```

- **Content-Encoding**

  指示对特定媒体类型的数据进行那种压缩

  ```
  Content-Encoding: gzip
  Content-Encoding: compress
  Content-Encoding: deflate
  Content-Encoding: identity
  Content-Encoding: br
  ```

- **Content-Language**

  希望采用的语言或语言组合

  ```
  Content-Language: de-DE
  Content-Language: en-US
  Content-Language: de-DE, en-CA
  ```

- **Content-Length**

  指明发送给接收方的消息主体的大小

  ```
  Content-Length: <length>
  ```

- **Content-Location**

  指定的是要返回的数据的地址选项

  ```
  Content-Location: <url>
  ```

- **Content-Range**

  一个数据片段在整个文件中的位置

  ```
  Content-Range: <unit> <range-start>-<range-end>/<size>
  Content-Range: <unit> <range-start>-<range-end>/*
  Content-Range: <unit> */<size>
  ```

- **Content-Type**

  指示资源的MIME类型

- **Content-Disposition**

  指示回复的内容该以何种形式展示，是以**内联**的形式还是以**附件**的形式下载到本地。可选：

  - `inline`：默认值，表示Body会以页面的一部分或者整个页面展示
  - `attachment`：表示Body中的内容会被下载到本地

  ```
  Content-Disposition: inline
  Content-Disposition: attachment
  Content-Disposition: attachment; filename="filename.jpg"
  ```

- **Expires**

  指定资源相当于**Date**的过期时间

  ```
  Expires: Wed, 21 Oct 2015 07:28:00 GMT
  ```

- **Last-Modified**

  资源最后的修改时间

  ```
  Last-Modified: Wed, 21 Oct 2015 07:28:00 GMT
  ```

## Response Code

### 10X

服务器收到请求，需要请求者继续执行操作

- 100：表示目前为止一切正常, 客户端应该继续请求
- 101：表示服务器应客户端升级协议的请求，正在切换协议

### 20X

操作被成功接收并处理

- 200：表明请求已经成功
- 201：表示请求已经被成功处理，并且创建了新的资源
- 202：表示服务器端已经收到请求消息，但是尚未进行处理。
- 203：请求成功，但返回的meta信息不在原始的服务器，而是一个副本
- 204：服务器成功处理，但未返回内容
- 205：服务器处理成功，用户终端（例如：浏览器）应重置文档视图
- 206：表示请求已成功，并且主体包含所请求的数据区间，该数据区间是在请求的Range首部指定的

### 30X

重定向，需要进一步的操作以完成请求

- 300：表示重定向的响应状态码，表示该请求拥有多种可能的响应。如果服务器提供优先选择，就应该生成一个**Location**头部
- 301：表示请求的资源已经被**永久移动**到**Location**头部指定的URL上。在某些客户端，重定向方法会被修改为GET。
- 302：表示请求的资源已经被**临时移动**到**Location**头部指定的URL上。在某些客户端，重定向方法会被修改为GET。
- 303：在POST或PUT操作时返回，表示请求的URL已经存在别的资源。
- 304：表示服务端无须再次传输请求的内容，允许客户端使用缓存的内容
- 307：表示请求的资源已经被**临时移动**到**Location**头部指定的URL上，重定向方法不会改变。
- 308：表示请求的资源已经被**永久移动**到**Location**头部指定的URL上，重定向方法不会改变。

### 40X

客户端错误，请求包含语法错误或无法完成请求

- 400：表示由于语法无效，服务器无法理解该请求
- 401：表示缺乏目标资源要求的身份验证凭证
- 403：表示服务器端有能力处理该请求，但是拒绝授权访问
- 404：表示服务器端无法找到所请求的资源
- 405：表明服务器禁止了使用当前 HTTP 方法的请求
- 407：表示缺少位于浏览器和代理服务器之间要求的身份凭证
- 408：表示请求超时
- 409：表示请求与服务器端目标资源的当前状态相冲突
- 410：表明请求的目标资源在原服务器上不存在了，并且是永久性的丢失
- 411：缺少Content-Length字段，服务器拒绝客户端请求
- 412：Precondition Failed
- 413：Payload Too Large
- 414：URI Too Long
- 415：Unsupported Media Type
- 416：表示服务器无法处理所请求的数据区间
- 417：表示服务器无法满足请求头中Expect字段描述的信息
- 422：表示服务器理解请求实体的内容类型，并且请求实体的语法是正确的，但是服务器无法处理请求指令
- 425：Too Early
- 426：表示服务器拒绝处理客户端使用当前协议发送的请求，但是可以接受其使用升级后的协议发送的请求。
- 428：Precondition Required
- 429：表示在一定的时间内用户发送了太多的请求，可以用Retry-After提示用户需要等待多长时间才能继续发送请求
- 431：Request Header Fields Too Large

### 50X

服务器错误，服务器在处理请求的过程中发生了错误

- 500：Internal Server Error
- 501：Not Implemented
- 502：Bad Gateway
- 503：表示服务器尚未处于可以接受请求的状态
- 504：Gateway Timeout
- 505：HTTP Version Not Supported
- 506：Variant Also Negotiates
- 507：表示服务器不能存储相关内容
- 508：表示服务器中断一个操作，整个操作失败
- 510：Not Extended
- 511：Network Authentication Required

## MIME

| 类型        | 描述                         | 实例                                                         |
| ----------- | ---------------------------- | ------------------------------------------------------------ |
| text        | 表明文件是普通文本           | text/plain, text/html, text/css, text/javascript             |
| image       | 表明是某种图像               | image/gif, image/png, image/jpeg, image/bmp, image/webp, image/x-icon, image/vnd.microsoft.icon |
| audio       | 表明是某种音频文件           | audio/midi, audio/mpeg, audio/webm, audio/ogg, audio/wav     |
| video       | 表明是某种视频文件           | video/webm`, `video/ogg                                      |
| application | 表明是某种二进制数据         | application/octet-stream, application/pkcs12, application/vnd.mspowerpoint, application/xhtml+xml, application/xml, application/pdf, application/json |
| multipart   | 表示细分领域的文件类型的种类 | multipart/form-data, multipart/byteranges                    |

常用的的MIME类型解释如下：

- `application/octet-stream`：这是应用程序文件的默认值。表示未知的应用程序文件，浏览器不可以展示

- `text/plain`：文本文件默认值。表示未知的文本文件，浏览器可以展示

- `application/json`：表示json类型

- `multipart/form-data`：表单格式，常用于上传多文档

  ```html
  Content-Type: multipart/form-data; boundary=aBoundaryString
  (other headers associated with the multipart document as a whole)
  
  --aBoundaryString
  Content-Disposition: form-data; name="myFile"; filename="img.jpg"
  Content-Type: image/jpeg
  
  (data)
  --aBoundaryString
  Content-Disposition: form-data; name="myField"
  
  (data)
  --aBoundaryString
  (more subparts)
  --aBoundaryString--
  ```

- `multipart/byteranges`：文件分段

  ```
  HTTP/1.1 206 Partial Content
  Accept-Ranges: bytes
  Content-Type: multipart/byteranges; boundary=3d6b6a416f9b5
  Content-Length: 385
  
  --3d6b6a416f9b5
  Content-Type: text/html
  Content-Range: bytes 100-200/1270
  
  eta http-equiv="Content-type" content="text/html; charset=utf-8" />
      <meta name="vieport" content
  --3d6b6a416f9b5
  Content-Type: text/html
  Content-Range: bytes 300-400/1270
  
  -color: #f0f0f2;
          margin: 0;
          padding: 0;
          font-family: "Open Sans", "Helvetica
  --3d6b6a416f9b5--
  ```

- `application/x-www-form-urlencoded`：发送`GET`请求时，请求数据被编码成以`&`分割的键值对

## Reference

https://developer.mozilla.org/zh-CN/docs/Web/HTTP



