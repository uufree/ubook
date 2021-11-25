# Lua

[TOC]

## 0. Crash Course

- 保留关键字

  ```lua
  and       break     do        else      elseif
  end       false     for       function  if
  in        local     nil       not       or
  repeat    return    then      true      until     while
  ```

- 运算符

  - 算术操作符：`+     -     *     /     %     ^`
  - 关系操作符：`==    ~=    <=    >=    <     >`
  - 逻辑操作符：`and    or    not`
  - 其他
    - `#`：获取字符串长度
    - `..`：拼接字符串

- 基础类型

  - **nil**
  - **boolean**
  - **number**
  - **string**
  - **function**
  - **userdata**: can hold arbitrary C data
  - **thread**: independent threads of execution used to implement coroutines
  - **table**

## 1. Variables 

```lua
-- Two dashes start a one-line comment.

--[[
     Adding two ['s and ]'s makes it a
     multi-line comment.
--]]

num = 42  						-- All numbers are doubles(64 bits). Global
local num = 42 					-- 临时变量
t = "double-quotes"   		    -- Immutable strings like Python.
u = [[ Double brackets
       start and end
       multi-line strings.]]	-- Format String. such as python ''' xxx '''
t = nil  						-- Undefines t; Lua has garbage collection.

-- print var type
print(type(num))

x, y = y, x 	-- 交换x和y的值

```

## 2. Flow control

```lua
-- if
if num > 10 then
  print(">10")
elseif num ~= 20 then  -- ~= is not equals
  print("!=20")
end

-- if
-- Only nil and false are falsy; 0 and '' are true!
aBoolValue = false
if not aBoolValue then 
  print('twas false') 
end

-- while
-- support 'break'
while num < 50 do
  num = num + 1  -- No ++ or += type operators.
  if num == 20 then
    break
   end
end

-- for
-- support 'break'
-- The range includes both ends. [1, 100]
for i = 1, 100, 1 do  
  print(i)
end

-- for range skip
-- support 'break'
for i = 100, 1, -2 do  
  print(i)
end

-- repeat
-- support 'break'
repeat
  print('the way of the future')
  num = num - 1
until num == 0
```

## 3. Functions

```lua
-- Normal
function fib(n)
  if n < 2 then return 1 end
  return fib(n - 2) + fib(n - 1)
end

-- Functions are first-class, may be local/global.
local function fib_local(n)
  if n < 2 then return 1 end
  return fib_local(n - 2) + fib_local(n - 1)
end


-- 支持闭包和匿名函数
function adder(x)
  -- The returned function is created when adder is
  -- called, and remembers the value of x:
  return function (y) return x + y end
end

-- 多返回值
function bar(a, b, c)
  print(a, b, c)
  return 4, 8, 15, 16, 23, 42
end

-- 变长参数
function ma(...)
    for k, v in pairs{...} do
        print(k, v)
    end
end

ma(1,2,3,4,5,6,7,8)


```

## 4. Tables

### 4.1 Table

```lua
-- Tables = Lua's only compound data structure;
-- Using tables as dictionaries / maps:
t = {key1 = 'value1', key2 = false}
print(t.key1)  -- Prints 'value1'.
t.newKey = {}  -- Adds a new key/value pair.
t.key2 = nil   -- Removes key2 from the table.

-- Table iteration.
for key, val in pairs(u) do  
  print(key, val)
end

-- _G is a special table of all globals.
print(_G['_G'] == _G)  -- Prints 'true'.

-- List literals implicitly set up int keys:
-- #操作符用于获取map长度
v = {'value1', 'value2', 1.21, 'gigawatts'}
for i = 1, #v do  -- #v is the size of v for lists.
  print(v[i])  -- Indices start at 1 !! SO CRAZY!
end

print(v[#v]) 	-- 打印列表v的最后一个值
v[#v] = nil		-- 删除列表v的最后一个值
v[#v+1] = num 	-- 将num添加到列表v的最后

-- 多维数组
ma = {}
for i=1,10 do
    tma = {}
    for j=1,10 do
        tma[j] = j
    end
    ma[i] = tma
end

```

### 4.2 Metatable

```lua
-- 元表中的函数用来描述行为。任何table都可以作为其他值的元表
-- A table can have a metatable that gives the table
-- operator-overloadish behavior.
f1 = {a = 1, b = 2}  
f2 = {a = 2, b = 3}
metafraction = {}
function metafraction.__add(f1, f2)
  sum = {}
  sum.b = f1.b * f2.b
  sum.a = f1.a * f2.b + f2.a * f1.b
  return sum
end

setmetatable(f1, metafraction)
setmetatable(f2, metafraction)
print(getmetatable(f1))
print(getmetatable(f2))

s = f1 + f2  -- call __add(f1, f2) on f1's metatable

-- Full list. Here a is a table with the metamethod.
-- __add(a, b)                     for a + b
-- __sub(a, b)                     for a - b
-- __mul(a, b)                     for a * b
-- __div(a, b)                     for a / b
-- __mod(a, b)                     for a % b
-- __pow(a, b)                     for a ^ b
-- __unm(a)                        for -a
-- __concat(a, b)                  for a .. b
-- __len(a)                        for #a
-- __eq(a, b)                      for a == b
-- __lt(a, b)                      for a < b
-- __le(a, b)                      for a <= b
-- __index(a, b)  <fn or a table>  for a.b
-- __newindex(a, b, c)             for a.b = c
-- __call(a, ...)                  for a(...)

-- __index详解。当访问table中一个不存在的字段或者函数时，就会去table __index进行查找。常用于继承体系
```

### 4.3 Class-like tables and inheritance

```lua
-- example
Dog = {}                                   

function Dog:new()                        
  newObj = {sound = 'woof'}               
  self.__index = self                      
  return setmetatable(newObj, self)       
end

function Dog:makeSound()                   
  print('I say ' .. self.sound)
end

mrDog = Dog:new()                         
mrDog:makeSound()  -- 'I say woof'        

-- inheritance example
LoudDog = Dog:new()                           

function LoudDog:makeSound()
  s = self.sound .. ' '                      
  print(s .. s .. s)
end

seymour = LoudDog:new()                       
seymour:makeSound()  -- 'woof woof woof'     
```

## 5. Modules

```lua
-- mod.lua
local M = {}

local function sayMyName()
  print('Hrunkner')
end

function M.sayHello()
  print('Why hello there')
  sayMyName()
end

return M

-- main
local mod = require('mod')  -- Run the file mod.lua.
mod.sayHello()  -- Says hello to Hrunkner.
mod.sayMyName()  -- error
```

## 6. Coroutine

```lua
-- Lua将所有协程相关的函数放在table coroutine中
-- 一个协程有以下状态：suspended, running, dead, normal
-- 创建一个协程时，他处于挂起状态
f = function()
    for i=1, 2 do
        coroutine.yield(i)
    end
end

co = coroutine.create(f)
print(coroutine.status(co))
print(coroutine.resume(co))
print(coroutine.resume(co))
print(coroutine.resume(co))
print(coroutine.status(co))
```

## 7. Thread

Lua不支持真正的多线程。

## 8. Library

### 8.1 math

```lua
local i = require("math")

for k, v in pairs(i) do
    print(k, v)
endLua
```

### 8.2  table

```lua
--[[
maxn    function: 0x420220
concat  function: 0x420320
pack    function: 0x420180
sort    function: 0x420840
unpack  function: 0x41ff90
remove  function: 0x41fea0
insert  function: 0x420090
]]--

-- 插入
t = {1,2,3}
table.insert(t, 1, 0)	// 0,1,2,3
table.insert(t, 0)		// 1,2,3,0

-- 删除
t = {1,2,3}
table.remove(t,1)		// 2,3
table.remove(t)			// 1,2

-- 排序
t = {5,4,8,9,0,1}
table.sort(t, function(a,b)return a<b end) 	// 0,1,4,5,8,9
table.sort(t, function(a,b)return a>b end)	// 9,8,5,4,1,0

-- 连接
t = {5,4,8,9,0,1}
table.concat(t,',')		// 5,4,8,9,0,1
print(table.concat(t))	// 548901

-- foreach
table.foreach(table, f)
```

### 8.3 string

```lua
-- byte
string.byte("ABCDE")	-- print 'A' ASCII code
string.byte("ABCDE", 3)	-- print 'C' ASCII code

-- char
string.char(65,66,67)	-- print ABC

-- find
string.find(s, pattern [, index [, plain]])

-- len
string.len(s)

-- lower
string.lower(s)

-- upper
string.upper(s)

-- rep(重复)
string.rep(s, n)
string.rep("1", 3)	--print: 111

-- reverse(反转)
string.reverse(s)

-- 子区间
string.sub(s, i [, j])
```

### 8.4 io

```lua
-- open
-- mode可选：r, w, a, r+, w+, a+
file = io.open (filename [, mode])

-- close
io.close ([file])

-- flush
io.flush ()

-- lines
io.lines ([filename])
for line in io.lines(filename) do ... end

-- output
io.output ([file])

-- input
io.input ([file])

-- read
io.read (format1, ...)

-- write
io.write (value1, ...)

-- seek
-- offset可选：set, cur, end
f:seek ([whence] [, offset])
```

### 8.5 os

```lua
-- clock: Return CPU time since Lua started in seconds.
os.clock()

-- difftime: Calculate the number of seconds between time t1 to time t2.
os.difftime(t2, t1)

-- execute: Execute an operating system shell command
os.execute([command])

-- exit
os.exit([code])

-- getenv
os.getenv(varname)

-- remove
os.remove(filename)

--rename
os.rename(oldname, newname)

-- time
os.time()
os.time()		-- print: 1637848310

-- date
os.date([format [, time]])
os.date("%d.%m.%Y")		-- 25.11.2021

tt= os.date('*t')		-- 打印非常详细的数据
for k, v in pairs(tt) do
    print(k, v)
end
```

## 9. Reference

http://tylerneylon.com/a/learn-lua/
https://www.coppeliarobotics.com/helpFiles/en/luaCrashCourse.htm