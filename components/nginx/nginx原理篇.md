# Nginx原理篇

[TOC]

1. Nginx有那些特点？
   - 快
   - 高拓展性。采用模块化设计，有庞大的第三方模块可供选择
   - 高可靠性
   - 低内存消耗
   - 高并发
   - 热部署。采用master-worker进程分离，在不间断服务的前提下，就可以做到更新配置项

2. 为什么Nginx Master和Nginx Worker都可以监听80端口呢？

   - Master监听80端口
   - 从Master Fork出n个Worker，Worker继承了Master的Socket资源

   根据以上步骤，就出现了Worker、Master共享80端口的情况。如果此时有请求到达，所有的Worker都会被唤醒，但仅有一个Worker可以成功调用`accept()`获取到链接。之后，这个获取到链接的Worker就可以正常的处理请求了。