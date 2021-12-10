# OpenResty综合篇

[TOC]

## 配置项

这块的选项太多了，本次仅记录一些迫切需要的。以后需要的话，需要再详细记录一下。

```nginx
# 设置由set_by_lua，content_by_lua和其他指定的脚本使用的Lua模块搜索路径。可使用;;可用于代表原始搜索路径。
syntax: 	lua_package_path <lua-style-path-str>
default: 	The content of LUA_PATH environment variable or Lua's compiled-in defaults.
context: 	http

# 设置由set_by_lua，content_by_lua和其他指定的脚本使用的Lua-C模块搜索路径。可使用;;可用于代表原始搜索路径。
syntax: 	lua_package_cpath <lua-style-cpath-str>
default: 	The content of LUA_CPATH environment variable or Lua's compiled-in defaults.
context: 	http

# 预分配一块dict内存空间
syntax: 	lua_shared_dict <name> <size>
default: 	no
context: 	http
phase: 		depends on usage

# 设置异步任务timer pending的数量
syntax: 	lua_max_pending_timers <count>
default: 	lua_max_pending_timers 1024
context: 	http

# 设置异步任务timer running的数量
syntax: 	lua_max_running_timers <count>
default: 	lua_max_running_timers 256
context: 	http

# 初始化master，全局调用一次
syntax: 	init_by_lua_block { lua-script }
context: 	http
phase: 		loading-config

syntax: 	init_by_lua_file <path-to-lua-script-file>
context: 	http
phase: 		loading-config

# 初始化worker，每个worker调用一次
syntax: 	init_worker_by_lua_block { lua-script }
context: 	http
phase: 		starting-worker

syntax: 	init_worker_by_lua_file <lua-file-path>
context: 	http
phase: 		starting-worker

# 终止worker，每个worker调用一次
syntax: 	exit_worker_by_lua_block { lua-script }
context: 	http
phase: 		exiting-worker

syntax: 	exit_worker_by_lua_file <path-to-lua-script-file>
context: 	http
phase: 		exiting-worker

# 在location中嵌入一段lua code，用于处理请求
syntax: 	content_by_lua_block { lua-script }
context: 	location, location if
phase: 		content

syntax: 	content_by_lua_file <path-to-lua-script-file>
context: 	location, location if
phase: 		content

# 这个功能未知...
syntax: 	rewrite_by_lua_block { lua-script }
context: 	http, server, location, location if
phase: 		rewrite tail

syntax: 	rewrite_by_lua_file <path-to-lua-script-file>
context: 	http, server, location, location if
phase: 		rewrite tail

# 这个功能也未知...
syntax: 	access_by_lua_block { lua-script }
context: 	http, server, location, location if
phase: 		access tail

syntax: 	access_by_lua_file <path-to-lua-script-file>
context: 	http, server, location, location if
phase: 		access tail

# 自定义load balance
syntax: 	balancer_by_lua_block { lua-script }
context: 	upstream
phase: 		content

syntax: 	balancer_by_lua_file <path-to-lua-script-file>
context: 	upstream
phase: 		content
```

## 模块方法

> 仅关注Lua Module

### LuaNginxModule

#### Constants

- `core constants`

  ```nginx
  ngx.OK (0)
  ngx.ERROR (-1)
  ngx.AGAIN (-2)
  ngx.DONE (-4)
  ngx.DECLINED (-5)
  ```

- `http method constants`

  ```nginx
  ngx.HTTP_GET
  ngx.HTTP_HEAD
  ngx.HTTP_PUT
  ngx.HTTP_POST
  ngx.HTTP_DELETE
  ngx.HTTP_OPTIONS   (added in the v0.5.0rc24 release)
  ngx.HTTP_MKCOL     (added in the v0.8.2 release)
  ngx.HTTP_COPY      (added in the v0.8.2 release)
  ngx.HTTP_MOVE      (added in the v0.8.2 release)
  ngx.HTTP_PROPFIND  (added in the v0.8.2 release)
  ngx.HTTP_PROPPATCH (added in the v0.8.2 release)
  ngx.HTTP_LOCK      (added in the v0.8.2 release)
  ngx.HTTP_UNLOCK    (added in the v0.8.2 release)
  ngx.HTTP_PATCH     (added in the v0.8.2 release)
  ngx.HTTP_TRACE     (added in the v0.8.2 release)
  ```

- `http status constants`

  ```nginx
  value = ngx.HTTP_CONTINUE (100) (first added in the v0.9.20 release)
  value = ngx.HTTP_SWITCHING_PROTOCOLS (101) (first added in the v0.9.20 release)
  value = ngx.HTTP_OK (200)
  value = ngx.HTTP_CREATED (201)
  value = ngx.HTTP_ACCEPTED (202) (first added in the v0.9.20 release)
  value = ngx.HTTP_NO_CONTENT (204) (first added in the v0.9.20 release)
  value = ngx.HTTP_PARTIAL_CONTENT (206) (first added in the v0.9.20 release)
  value = ngx.HTTP_SPECIAL_RESPONSE (300)
  value = ngx.HTTP_MOVED_PERMANENTLY (301)
  value = ngx.HTTP_MOVED_TEMPORARILY (302)
  value = ngx.HTTP_SEE_OTHER (303)
  value = ngx.HTTP_NOT_MODIFIED (304)
  value = ngx.HTTP_TEMPORARY_REDIRECT (307) (first added in the v0.9.20 release)
  value = ngx.HTTP_PERMANENT_REDIRECT (308)
  value = ngx.HTTP_BAD_REQUEST (400)
  value = ngx.HTTP_UNAUTHORIZED (401)
  value = ngx.HTTP_PAYMENT_REQUIRED (402) (first added in the v0.9.20 release)
  value = ngx.HTTP_FORBIDDEN (403)
  value = ngx.HTTP_NOT_FOUND (404)
  value = ngx.HTTP_NOT_ALLOWED (405)
  value = ngx.HTTP_NOT_ACCEPTABLE (406) (first added in the v0.9.20 release)
  value = ngx.HTTP_REQUEST_TIMEOUT (408) (first added in the v0.9.20 release)
  value = ngx.HTTP_CONFLICT (409) (first added in the v0.9.20 release)
  value = ngx.HTTP_GONE (410)
  value = ngx.HTTP_UPGRADE_REQUIRED (426) (first added in the v0.9.20 release)
  value = ngx.HTTP_TOO_MANY_REQUESTS (429) (first added in the v0.9.20 release)
  value = ngx.HTTP_CLOSE (444) (first added in the v0.9.20 release)
  value = ngx.HTTP_ILLEGAL (451) (first added in the v0.9.20 release)
  value = ngx.HTTP_INTERNAL_SERVER_ERROR (500)
  value = ngx.HTTP_METHOD_NOT_IMPLEMENTED (501)
  value = ngx.HTTP_BAD_GATEWAY (502) (first added in the v0.9.20 release)
  value = ngx.HTTP_SERVICE_UNAVAILABLE (503)
  value = ngx.HTTP_GATEWAY_TIMEOUT (504) (first added in the v0.3.1rc38 release)
  value = ngx.HTTP_VERSION_NOT_SUPPORTED (505) (first added in the v0.9.20 release)
  value = ngx.HTTP_INSUFFICIENT_STORAGE (507) (first added in the v0.9.20 release)
  ```

#### Location

- `res = ngx.location.capture(uri, options?)`：内部重定向到某一个URI上

  - options
    - **method**: request method, like ngx.HTTP_POST
    - **args**: query arguments (both string value and Lua tables are accepted)
    - **body**: request body (string value only)
    - **ctx**: request ctx table
    - **vars**: request vars table
    - **copy_all_vars**: bool
    - **share_all_vars**: bool
    - **always_forward_body**: bool
  - response
    - **res.status**: response.status, such as ngx.HTTP_OK...
    - **res.header**: response.header
    - **res.body**: response.body, maybe truncated, you need to check res.truncated
    - **res.truncated**: bool flag

- `res1, res2, ... = ngx.location.capture_multi({ {uri, options?}, {uri, options?}, ... })`

  转发请求

  ```nginx
  # single
  res = ngx.location.capture(
  	'/foo/bar?a=1&b=2',
      { method = ngx.HTTP_POST, body = 'hello, world', copy_all_vars = true,
        vars = { dog = "hello", cat = 32 }, ctx = {}
  	}
  )
  
  # multi
  -- construct the requests table
  local reqs = {}
  table.insert(reqs, { "/mysql" })
  table.insert(reqs, { "/postgres" })
  table.insert(reqs, { "/redis" })
  table.insert(reqs, { "/memcached" })
  
  -- issue all the requests at once and wait until they all return
  local resps = { ngx.location.capture_multi(reqs) }
  
  -- loop over the responses table
  for i, resp in ipairs(resps) do
      -- process the response table "resp"
  end
  ```

- `ngx.exec(uri, args?)`：内部重定向到某一个URI上

  ```nginx
  ngx.exec("/foo", { a = 3, b = "hello world" })
  ```

- `ngx.redirect(uri, status?)`：返回重新向地址和状态，由浏览器负责跳转。status仅支持30X系列

  ```nginx
  return ngx.redirect("/foo", ngx.HTTP_MOVED_TEMPORARILY)
  return ngx.redirect("http://www.google.com")
  ```

#### Structure

- `ngx.var`：Read and write Nginx variable values.

- `ngx.ctx`：request ctx

  ```nginx
  location /test {
      rewrite_by_lua_block {
          ngx.ctx.foo = 76
      }
      access_by_lua_block {
          ngx.ctx.foo = ngx.ctx.foo + 3
      }
      content_by_lua_block {
          ngx.say(ngx.ctx.foo)
      }
  }
  ```

#### Request

- `is_internal = ngx.req.is_internal()`：判定请求是否是外部请求

- `secs = ngx.req.start_time()`

  ```nginx
  local request_time = ngx.now() - ngx.req.start_time()
  ```

- `num = ngx.req.http_version()`：maybe 0.9, 1.0, 1.1, 2.0

- `str = ngx.req.raw_header(no_request_line?)`

- `method_name = ngx.req.get_method()`：such as "GET"，"POST"

- `ngx.req.set_method(method_id)`：such as ngx.HTTP_POST、ngx.HTTP_GET

- `ngx.req.set_uri(uri, jump?, binary?)`

  ```nginx
  rewrite ^ /foo last; -->> ngx.req.set_uri("/foo", true)
  rewrite ^ /foo break; -->> ngx.req.set_uri("/foo", false)
  ```

- `ngx.req.set_uri_args(args)`

  ```nginx
  ngx.req.set_uri_args({ a = 3, b = "hello world" })
  ```

- `args, err = ngx.req.get_uri_args(max_args?)`

  ```nginx
   location = /test {
       content_by_lua_block {
           local args, err = ngx.req.get_uri_args()
  
           if err == "truncated" then
               -- one can choose to ignore or reject the current request here
           end
  
           for key, val in pairs(args) do
               if type(val) == "table" then
                   ngx.say(key, ": ", table.concat(val, ", "))
               else
                   ngx.say(key, ": ", val)
               end
           end
       }
   }
  ```

- `args, err = ngx.req.get_post_args(max_args?)`

  ```nginx
   location = /test {
       content_by_lua_block {
           ngx.req.read_body()
           local args, err = ngx.req.get_post_args()
  
           if err == "truncated" then
               -- one can choose to ignore or reject the current request here
           end
  
           if not args then
               ngx.say("failed to get post args: ", err)
               return
           end
           for key, val in pairs(args) do
               if type(val) == "table" then
                   ngx.say(key, ": ", table.concat(val, ", "))
               else
                   ngx.say(key, ": ", val)
               end
           end
       }
   }
  ```

- `headers, err = ngx.req.get_headers(max_headers?, raw?)`

  ```nginx
   local h, err = ngx.req.get_headers()
  
   if err == "truncated" then
       -- one can choose to ignore or reject the current request here
   end
  
   for k, v in pairs(h) do
       ...
   end
  ```

- `ngx.req.set_header(header_name, header_value)`

  ```nginx
  ngx.req.set_header("Content-Type", "text/css")
  ```

- `ngx.req.clear_header(header_name)`

  ```nginx
  ngx.req.clear_header("Content-Type")
  ```

- `ngx.req.read_body()`：预读request body

  ```nginx
   ngx.req.read_body()
   local args = ngx.req.get_post_args()
  ```

- `ngx.req.discard_body()`：丢弃request body

- `data = ngx.req.get_body_data()`：需要预先调用`read_body()`

- `file_name = ngx.req.get_body_file()`：需要预先调用`read_body()`

- `ngx.req.set_body_data(data)`

- `ngx.req.set_body_file(file_name, auto_clean?)`

- `ngx.req.init_body(buffer_size?)`

- `ngx.req.append_body(data_chunk)`

- `ngx.req.finish_body()`

  ```nginx
   ngx.req.init_body(128 * 1024)  -- buffer is 128KB
   for chunk in next_data_chunk() do
       ngx.req.append_body(chunk) -- each chunk can be 4KB
   end
   ngx.req.finish_body()
  ```

#### Response

- `headers, err = ngx.resp.get_headers(max_headers?, raw?)`：获取response headers

  ```nginx
  local h, err = ngx.resp.get_headers()
  
  if err == "truncated" then
  	-- one can choose to ignore or reject the current response here
  end
  
  for k, v in pairs(h) do
       ...
  end
  ```

#### Global

- `ok, err = ngx.print(...)`：将结果发送至Client，不会附加换行符

- `ok, err = ngx.say(...)`：将结果发送至Client，附加换行符

- `ngx.log(log_level, ...)`：打印log

- `ok, err = ngx.flush(wait?)`：将response发送至Client，默认false

- `ngx.exit(status)`

- `ok, err = ngx.eof()`：明确响应指定流的末尾，以应对Chunk模式

- `ngx.sleep(seconds)`：不会阻塞nginx

- `str = ngx.encode_args(table)`

- `table, err = ngx.decode_args(str, max_args?)`

  ```nginx
  local args = ngx.decode_args(str, 0)
  
  # output: foo=3&b%20r=hello%20world
  ngx.encode_args({foo = 3, ["b r"] = "hello world"})
  ```

- `ok, err = ngx.on_abort(callback)`

  注册cleanup清理函数

  ```nginx
   local function my_cleanup()
       -- custom cleanup work goes here, like cancelling a pending DB transaction
  
       -- now abort all the "light threads" running in the current request handler
       ngx.exit(499)
   end
  
   local ok, err = ngx.on_abort(my_cleanup)
   if not ok then
       ngx.log(ngx.ERR, "failed to register the on_abort callback: ", err)
       ngx.exit(500)
   end
  ```

#### Encode & Decode

- `newstr = ngx.encode_base64(str, no_padding?)`
- `newstr = ngx.decode_base64(str)`
- `intval = ngx.crc32_short(str)`
- `intval = ngx.crc32_long(str)`
- `digest = ngx.hmac_sha1(secret_key, str)`
- `digest = ngx.md5(str)`
- `digest = ngx.md5_bin(str)`
- `digest = ngx.sha1_bin(str)`

#### Time

- `str = ngx.today()`

- `secs = ngx.time()`

- `secs = ngx.now()`

- `ngx.update_time()`

- `str = ngx.localtime()`

- `str = ngx.utctime()`

- `str = ngx.cookie_time(sec)`

  ```nginx
   ngx.say(ngx.cookie_time(1290079655))
       -- yields "Thu, 18-Nov-10 11:27:35 GMT"
  ```

- `str = ngx.http_time(sec)`

  ```nginx
   ngx.say(ngx.http_time(1290079655))
       -- yields "Thu, 18 Nov 2010 11:27:35 GMT"
  ```

- `sec = ngx.parse_http_time(str)`

  ```nginx
   local time = ngx.parse_http_time("Thu, 18 Nov 2010 11:27:35 GMT")
   if time == nil then
       ...
   end
  ```

#### Timer

定时器，用于完成各种异步任务

- `hdl, err = ngx.timer.at(delay, callback, user_arg1, user_arg2, ...)`：delay/seconds

  ```nginx
   location / {
       ...
       log_by_lua_block {
           local function push_data(premature, uri, args, status)
               -- push the data uri, args, and status to the remote
               -- via ngx.socket.tcp or ngx.socket.udp
               -- (one may want to buffer the data in Lua a bit to
               -- save I/O operations)
           end
           local ok, err = ngx.timer.at(0, push_data,
                                        ngx.var.uri, ngx.var.args, ngx.header.status)
           if not ok then
               ngx.log(ngx.ERR, "failed to create timer: ", err)
               return
           end
  
           -- other job in log_by_lua_block
       }
   }
  ```

- `hdl, err = ngx.timer.every(delay, callback, user_arg1, user_arg2, ...)`

- `count = ngx.timer.running_count()`

- `count = ngx.timer.pending_count()`

#### Re

https://github.com/openresty/lua-nginx-module#ngxrematch

#### Shared

多进程内存模型，类似于Redis。不建议选用！！

```nginx
 http {
     lua_shared_dict dogs 10m;
     server {
         location /set {
             content_by_lua_block {
                 local dogs = ngx.shared.dogs
                 dogs:set("Jim", 8)
                 ngx.say("STORED")
             }
         }
         location /get {
             content_by_lua_block {
                 local dogs = ngx.shared.dogs
                 ngx.say(dogs:get("Jim"))
             }
         }
     }
 }
```

- `value, flags = ngx.shared.DICT:get(key)`：不存在返回nil

- `value, flags, stale = ngx.shared.DICT:get_stale(key)`：即使key过期，依旧返回value

  ```nginx
   local cats = ngx.shared.cats
   local value, flags = cats.get(cats, "Marry")
  ```

- `success, err, forcible = ngx.shared.DICT:set(key, value, exptime?, flags?)`：强制插入新的值，如果空间不足就删除旧的值

- `ok, err = ngx.shared.DICT:safe_set(key, value, exptime?, flags?)`：不删除未到期的值，而是返回no memory的错误

  ```
   local cats = ngx.shared.cats
   local succ, err, forcible = cats.set(cats, "Marry", "it is a nice cat!")
  ```

- `success, err, forcible = ngx.shared.DICT:add(key, value, exptime?, flags?)`：仅在Key不存在的情况下，才将kv存入

- `ok, err = ngx.shared.DICT:safe_add(key, value, exptime?, flags?)`

- `success, err, forcible = ngx.shared.DICT:replace(key, value, exptime?, flags?)`

- `ngx.shared.DICT:delete(key)`

- `newval, err, forcible? = ngx.shared.DICT:incr(key, value, init?, init_ttl?)`

- `length, err = ngx.shared.DICT:lpush(key, value)`

- `length, err = ngx.shared.DICT:rpush(key, value)`

- `val, err = ngx.shared.DICT:lpop(key)`

- `val, err = ngx.shared.DICT:rpop(key)`

- `len, err = ngx.shared.DICT:llen(key)`

- `ttl, err = ngx.shared.DICT:ttl(key)`

  ```nginx
   require "resty.core"
  
   local cats = ngx.shared.cats
   local succ, err = cats:set("Marry", "a nice cat", 0.5)
  
   ngx.sleep(0.2)
  
   local ttl, err = cats:ttl("Marry")
   ngx.say(ttl) -- 0.3
  ```

- `success, err = ngx.shared.DICT:expire(key, exptime)`
- `ngx.shared.DICT:flush_all()`
- `flushed = ngx.shared.DICT:flush_expired(max_count?)`
- `keys = ngx.shared.DICT:get_keys(max_count?)`
- `capacity_bytes = ngx.shared.DICT:capacity()`
- `free_page_bytes = ngx.shared.DICT:free_space()`

#### Thread

- `co = ngx.thread.spawn(func, arg1, arg2, ...)`

   a special kind of Lua coroutines that are scheduled by the ngx_lua module.

  ```nginx
   -- query mysql, memcached, and a remote http service at the same time,
   -- output the results in the order that they
   -- actually return the results.
  
   local mysql = require "resty.mysql"
   local memcached = require "resty.memcached"
  
   local function query_mysql()
       local db = mysql:new()
       db:connect{
                   host = "127.0.0.1",
                   port = 3306,
                   database = "test",
                   user = "monty",
                   password = "mypass"
                 }
       local res, err, errno, sqlstate =
               db:query("select * from cats order by id asc")
       db:set_keepalive(0, 100)
       ngx.say("mysql done: ", cjson.encode(res))
   end
  
   local function query_memcached()
       local memc = memcached:new()
       memc:connect("127.0.0.1", 11211)
       local res, err = memc:get("some_key")
       ngx.say("memcached done: ", res)
   end
  
   local function query_http()
       local res = ngx.location.capture("/my-http-proxy")
       ngx.say("http done: ", res.body)
   end
  
   ngx.thread.spawn(query_mysql)      -- create thread 1
   ngx.thread.spawn(query_memcached)  -- create thread 2
   ngx.thread.spawn(query_http)       -- create thread 3
  ```

- `ok, res1, res2, ... = ngx.thread.wait(thread1, thread2, ...)`

  ```nginx
   local capture = ngx.location.capture
   local spawn = ngx.thread.spawn
   local wait = ngx.thread.wait
   local say = ngx.say
  
   local function fetch(uri)
       return capture(uri)
   end
  
   local threads = {
       spawn(fetch, "/foo"),
       spawn(fetch, "/bar"),
       spawn(fetch, "/baz")
   }
  
   for i = 1, #threads do
       local ok, res = wait(threads[i])
       if not ok then
           say(i, ": failed to run: ", res)
       else
           say(i, ": status: ", res.status)
           say(i, ": body: ", res.body)
       end
   end
  ```

- `ok, err = ngx.thread.kill(thread)`

#### Coroutine

- `co = coroutine.create(f)`

- `ok, ... = coroutine.resume(co, ...)`

- `... = coroutine.yield(...)`

- `co = coroutine.wrap(f)`

- `co = coroutine.running()`

- `status = coroutine.status(co)`  

### LuaCjsonLibrary

[Reference](https://github.com/openresty/lua-cjson/)

- `json_text = cjson.encode(value)`
- `value = cjson.decode(json_text)`

- `cjson.encode_empty_table_as_object(true|false|"on"|"off")`

  将空的table编码成`{}`

- `cjson.empty_array`

  编码出一个`[]`而非nil

  ```lua
  local cjson = require "cjson"
  
  local json = cjson.encode({
      foo = "bar",
      some_object = {},
      some_array = cjson.empty_array
  })
  
  # result
  {
      "foo": "bar",
      "some_object": {},
      "some_array": []
  }
  ```

- `setmetatable({}, cjson.array_mt)`

  将meta table编码成array

  ```lua
  local t = {}
  t[1] = "one"
  t[2] = "two"
  t[4] = "three"
  t.foo = "bar"
  setmetatable(t, cjson.array_mt)
  cjson.encode(t) -- ["one","two",null,"three"]
  ```

- `setmetatable({}, cjson.empty_array_mt)`

  将meta table编码成empty array

  ```lua
  local function serialize(arr)
      setmetatable(arr, cjson.empty_array_mt)
  
      return cjson.encode({some_array = arr})
  end
  ```

### LuaRestyDNSLibrary

[Reference](https://github.com/openresty/lua-resty-dns)

### LuaRestyLockLibrary

[Reference](https://github.com/openresty/lua-resty-lock)

### LuaRestyLrucacheLibrary

[Reference](https://github.com/openresty/lua-resty-lrucache)

```lua
-- file myapp.lua: example "myapp" module

local _M = {}

-- alternatively: local lrucache = require "resty.lrucache.pureffi"
local lrucache = require "resty.lrucache"

-- we need to initialize the cache on the lua module level so that
-- it can be shared by all the requests served by each nginx worker process:
local c, err = lrucache.new(200)  -- allow up to 200 items in the cache
if not c then
    error("failed to create the cache: " .. (err or "unknown"))
end

function _M.go()
    c:set("dog", 32)
    c:set("cat", 56)
    ngx.say("dog: ", c:get("dog"))
    ngx.say("cat: ", c:get("cat"))

    c:set("dog", { age = 10 }, 0.1)  -- expire in 0.1 sec
    c:delete("dog")

    c:flush_all()  -- flush all the cached data
end

return _M
```

- `cache, err = lrucache.new(max_items [, load_factor])`
- `cache:set(key, value, ttl?, flags?)`
- `data, stale_data, flags = cache:get(key)`
- `cache:delete(key)`
- `count = cache:count()`
- `size = cache:capacity()`
- `keys = cache:get_keys(max_count?, res?)`
- `cache:flush_all()`

### LuaRestyMemcachedLibrary

[Reference](https://github.com/openresty/lua-resty-memcached)

### LuaRestyMysqlLibrary

[Reference](https://github.com/openresty/lua-resty-mysql)

```nginx
# you do not need the following line if you are using
# the ngx_openresty bundle:
lua_package_path "/path/to/lua-resty-mysql/lib/?.lua;;";

server {
	location /test {
        content_by_lua '
            local mysql = require "resty.mysql"
            local db, err = mysql:new()
            if not db then
                ngx.say("failed to instantiate mysql: ", err)
                return
            end

            db:set_timeout(1000) -- 1 sec

            -- or connect to a unix domain socket file listened
            -- by a mysql server:
            --     local ok, err, errcode, sqlstate =
            --           db:connect{
            --              path = "/path/to/mysql.sock",
            --              database = "ngx_test",
            --              user = "ngx_test",
            --              password = "ngx_test" }

            local ok, err, errcode, sqlstate = db:connect{
                    host = "127.0.0.1",
                    port = 3306,
                    database = "ngx_test",
                    user = "ngx_test",
                    password = "ngx_test",
                    charset = "utf8",
                    max_packet_size = 1024 * 1024,
            }

            if not ok then
                ngx.say("failed to connect: ", err, ": ", errcode, " ", sqlstate)
                return
            end

            ngx.say("connected to mysql.")

            local res, err, errcode, sqlstate =
                db:query("drop table if exists cats")
            if not res then
                ngx.say("bad result: ", err, ": ", errcode, ": ", sqlstate, ".")
                return
            end

            res, err, errcode, sqlstate =
                db:query("create table cats "
                         .. "(id serial primary key, "
                         .. "name varchar(5))")
            if not res then
                ngx.say("bad result: ", err, ": ", errcode, ": ", sqlstate, ".")
                return
            end

            ngx.say("table cats created.")

            res, err, errcode, sqlstate =
                db:query("insert into cats (name) "
                             .. "values (\'Bob\'),(\'\'),(null)")
            if not res then
                ngx.say("bad result: ", err, ": ", errcode, ": ", sqlstate, ".")
                return
            end

            ngx.say(res.affected_rows, " rows inserted into table cats ",
                    "(last insert id: ", res.insert_id, ")")

            -- run a select query, expected about 10 rows in
            -- the result set:
            res, err, errcode, sqlstate =
                db:query("select * from cats order by id asc", 10)
            if not res then
                ngx.say("bad result: ", err, ": ", errcode, ": ", sqlstate, ".")
                return
            end

            local cjson = require "cjson"
            ngx.say("result: ", cjson.encode(res))

            -- put it into the connection pool of size 100,
            -- with 10 seconds max idle timeout
            local ok, err = db:set_keepalive(10000, 100)
            if not ok then
                ngx.say("failed to set keepalive: ", err)
                return
            end

            -- or just close the connection right away:
            -- local ok, err = db:close()
            -- if not ok then
            --     ngx.say("failed to close: ", err)
            --     return
            -- end
        ';
    }
}
```

- `db, err = mysql:new()`

- `ok, err, errcode, sqlstate = db:connect(options)`

  - **host**
  - **port**
  - **path**: the path of the unix socket file listened by the MySQL server
  - **database**
  - **user**
  - **password**
  - **charset**: `big5`, `dec8`, `cp850`, `hp8`, `koi8r`, `latin1`, `latin2`, `swe7`, `ascii`, `ujis`, `sjis`, `hebrew`, `tis620`, `euckr`, `koi8u`, `gb2312`, `greek`, `cp1250`, `gbk`, `latin5`, `armscii8`, `utf8`, `ucs2`, `cp866`, `keybcs2`, `macce`, `macroman`, `cp852`, `latin7`, `utf8mb4`, `cp1251`, `utf16`, `utf16le`, `cp1256`, `cp1257`, `utf32`, `binary`, `geostd8`, `cp932`, `eucjpms`, `gb18030`
  - **max_packet_size**: the upper limit for the reply packets sent from the MySQL server (default to 1MB).
  - **ssl**
  - **ssl_verify**
  - **pool**: the name for the MySQL connection pool
  - **pool_size**: 连接池大小
  - **backlog**: 连接池等待队列
  - **compact_arrays**: 当此选项设置为 true 时，query 和 read_result 方法将返回结果集的数组数组结构，而不是默认的哈希数组结构。

- `db:set_timeout(time)`

  为后续所有的操作设置超时时间，包括connect，毫秒计数。

- `ok, err = db:set_keepalive(max_idle_timeout, pool_size)`

  设置connect idle timeout，毫秒计数。In case of success, returns `1`. In case of errors, returns `nil` with a string describing the error.

- `times, err = db:get_reused_times()`

  返回当前连接的重用次数。In case of errors, returns `nil` with a string describing the error.

- `ok, err = db:close()`

  In case of success, returns `1`. In case of errors, returns `nil` with a string describing the error.

- `res, err, errcode, sqlstate = db:query(query)`

- `res, err, errcode, sqlstate = db:query(query, nrows)`

  可以在query中执行insert、delete等其他操作

  You should always check if the `err` return value is `again` in case of success
  

### LuaRestyRedisLibrary

[Reference](https://github.com/openresty/lua-resty-redis)

```nginx
# you do not need the following line if you are using
# the OpenResty bundle:
lua_package_path "/path/to/lua-resty-redis/lib/?.lua;;";

server {
    location /test {
        content_by_lua_block {
            local redis = require "resty.redis"
            local red = redis:new()

            red:set_timeouts(1000, 1000, 1000) -- 1 sec

            -- or connect to a unix domain socket file listened
            -- by a redis server:
            --     local ok, err = red:connect("unix:/path/to/redis.sock")

            local ok, err = red:connect("127.0.0.1", 6379)
            if not ok then
                ngx.say("failed to connect: ", err)
                return
            end
                
            local res, err = red:auth("foobared")
    		if not res then
        		ngx.say("failed to authenticate: ", err)
        		return
    		end

            ok, err = red:set("dog", "an animal")
            if not ok then
                ngx.say("failed to set dog: ", err)
                return
            end

            ngx.say("set result: ", ok)

            local res, err = red:get("dog")
            if not res then
                ngx.say("failed to get dog: ", err)
                return
            end

            if res == ngx.null then
                ngx.say("dog not found.")
                return
            end

            ngx.say("dog: ", res)

            red:init_pipeline()
            red:set("cat", "Marry")
            red:set("horse", "Bob")
            red:get("cat")
            red:get("horse")
            local results, err = red:commit_pipeline()
            if not results then
                ngx.say("failed to commit the pipelined requests: ", err)
                return
            end

            for i, res in ipairs(results) do
                if type(res) == "table" then
                    if res[1] == false then
                        ngx.say("failed to run command ", i, ": ", res[2])
                    else
                        -- process the table value
                    end
                else
                    -- process the scalar value
                end
            end

            -- put it into the connection pool of size 100,
            -- with 10 seconds max idle time
            local ok, err = red:set_keepalive(10000, 100)
            if not ok then
                ngx.say("failed to set keepalive: ", err)
                return
            end

            -- or just close the connection right away:
            -- local ok, err = red:close()
            -- if not ok then
            --     ngx.say("failed to close: ", err)
            --     return
            -- end
        }
    }
}
```

- `red, err = redis:new()`

- `ok, err = red:connect(host, port, options_table?)`

- `ok, err = red:connect("unix:/path/to/unix.sock", options_table?)`

  - ssl
  - ssl_verify
  - server_name
  - pool
  - pool_size
  - backlog

- `red:set_timeout(time)`

- `red:set_timeouts(connect_timeout, send_timeout, read_timeout)`

  为后续所有的操作设置超时时间，包括connect，毫秒计数。

- `res, err = red:auth("foobared")`

- `ok, err = red:set_keepalive(max_idle_timeout, pool_size)`

- `times, err = red:get_reused_times()`

- `ok, err = red:close()`

- `red:hmset(myhash, field1, value1, field2, value2, ...)`

- `hash = red:array_to_hash(array)`

### LuaRestyStringLibrary

[Reference](https://github.com/openresty/lua-resty-string)

### LuaRestyUploadLibrary

[Reference](https://github.com/openresty/lua-resty-upload)

### LuaRestyUpstreamHealthcheckLibrary

[Reference](https://github.com/openresty/lua-resty-upstream-healthcheck)

### LuaRestyWebSocketLibrary

[Reference](https://github.com/openresty/lua-resty-websocket)

### LuaRestyLimitTrafficLibrary

[Reference](https://github.com/openresty/lua-resty-limit-traffic)

### LuaRestyCookie

[Reference](https://github.com/cloudflare/lua-resty-cookie)