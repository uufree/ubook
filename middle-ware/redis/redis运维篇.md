# Redis运维篇

[TOC]

## 部署

详见deploy文件夹。

## 配置

### v6.2

- redis.conf

  ```
  # Redis configuration file example.
  #
  # Note that in order to read the configuration file, Redis must be
  # started with the file path as first argument:
  #
  # ./redis-server /path/to/redis.conf
  
  # Note on units: when memory size is needed, it is possible to specify
  # it in the usual form of 1k 5GB 4M and so forth:
  #
  # 1k => 1000 bytes
  # 1kb => 1024 bytes
  # 1m => 1000000 bytes
  # 1mb => 1024*1024 bytes
  # 1g => 1000000000 bytes
  # 1gb => 1024*1024*1024 bytes
  #
  # units are case insensitive so 1GB 1Gb 1gB are all the same.
  
  ################################## INCLUDES ###################################
  
  # Include one or more other config files here.  This is useful if you
  # have a standard template that goes to all Redis servers but also need
  # to customize a few per-server settings.  Include files can include
  # other files, so use this wisely.
  #
  # Note that option "include" won't be rewritten by command "CONFIG REWRITE"
  # from admin or Redis Sentinel. Since Redis always uses the last processed
  # line as value of a configuration directive, you'd better put includes
  # at the beginning of this file to avoid overwriting config change at runtime.
  #
  # If instead you are interested in using includes to override configuration
  # options, it is better to use include as the last line.
  #
  # include /path/to/local.conf
  # include /path/to/other.conf
  
  ################################## MODULES #####################################
  
  # Load modules at startup. If the server is not able to load modules
  # it will abort. It is possible to use multiple loadmodule directives.
  #
  # loadmodule /path/to/my_module.so
  # loadmodule /path/to/other_module.so
  
  ################################## NETWORK #####################################
  
  # bind
  bind 127.0.0.1 -::1
  
  # port
  port 6379
  
  # 默认启动
  # 当没有显示的绑定到一组地址，并且没有配置密码。此时，仅允许127.0.0.1 client访问。有效隔绝了来自互联网的攻击
  protected-mode yes
  
  # TCP监听队列长度。默认511。建议同步此配置与/proc/sys/net/core/somaxconn中的配置相同
  tcp-backlog 511
  
  # 几乎不使用。
  # unixsocket /run/redis.sock
  # unixsocketperm 700
  
  # 设置客户端idle timeout来限制连接的最大空闲时间。一旦客户端连接的idle时间超过了timeout，连接将被关闭。
  # 设置为0，则意味者server端不做任何处理
  timeout 0
  
  # 在TCP这一层进行keepalive检测，防止大量死连接占用系统资源。设置为0，意味着不进行keepalive检测
  tcp-keepalive 300
  
  ################################# GENERAL #####################################
  
  # 是否以守护进程启动服务
  daemonize no
  
  # 进程pid文件位置
  pidfile /var/run/redis_6379.pid
  
  # 日志级别，可选：debug | verbose | notice | warning
  loglevel notice
  
  # 不指定，则默认输出至终端
  logfile ""
  
  # 可用数据库
  databases 16
  
  # 是否每次启动时展示redis logo
  always-show-logo no
  
  # 是否需要redis修改top命令或者ps命令中的进程名称
  set-proc-title yes
  
  # 进程名称模板
  # {title}           Name of process as executed if parent, or type of child process.
  # {listen-addr}     Bind address or '*' followed by TCP or TLS port listening on, or
  #                   Unix socket if only that's available.
  # {server-mode}     Special mode, i.e. "[sentinel]" or "[cluster]".
  # {port}            TCP port listening on, or 0.
  # {tls-port}        TLS port listening on, or 0.
  # {unixsocket}      Unix domain socket listening on, or "".
  # {config-file}     Name of configuration file used.
  #
  proc-title-template "{title} {listen-addr} {server-mode}"
  
  # RDB配置项
  ################################ SNAPSHOTTING  ################################
  # RDB启动条件
  # a. 3600s内，至少一个1个key发生了变更
  # b. 300s内，至少500个key发生了变更
  # c. 60s内，至少10000个key发生了变更
  # save 3600 1
  # save 300 100
  # save 60 10000
  
  # 当出现写入失败的时候，停止写入工作
  stop-writes-on-bgsave-error yes
  
  # 是否使用LZF算法压缩RDB文件
  rdbcompression yes
  
  # 是否启用RDB文件校验
  rdbchecksum yes
  
  # 指定生成的RDB文件的名称
  dbfilename dump.rdb
  
  # 指定保存RDB、AOF文件夹的位置
  dir ./
  
  ################################# REPLICATION #################################
  
  # 以副本模式运行时，主节点的ip:port
  # 注：这个选项以命令行模式：--slaveof 启动的拓展性更强
  replicaof <masterip> <masterport>
  
  # 以副本模式运行时，master节点的密码
  masterauth <master-password>
  
  # 如果副本与主节点失联：
  # yes：照常处理读命令
  # no：提示与主节点同步中，拒绝所有请求
  replica-serve-stale-data yes
  
  # 副本数据仅可读，不可写
  replica-read-only yes
  
  # Replication SYNC strategy: disk or socket.
  #
  # New replicas and reconnecting replicas that are not able to continue the
  # replication process just receiving differences, need to do what is called a
  # "full synchronization". An RDB file is transmitted from the master to the
  # replicas.
  #
  # The transmission can happen in two different ways:
  #
  # 1) Disk-backed: The Redis master creates a new process that writes the RDB
  #                 file on disk. Later the file is transferred by the parent
  #                 process to the replicas incrementally.
  # 2) Diskless: The Redis master creates a new process that directly writes the
  #              RDB file to replica sockets, without touching the disk at all.
  #
  # With disk-backed replication, while the RDB file is generated, more replicas
  # can be queued and served with the RDB file as soon as the current child
  # producing the RDB file finishes its work. With diskless replication instead
  # once the transfer starts, new replicas arriving will be queued and a new
  # transfer will start when the current one terminates.
  #
  # When diskless replication is used, the master waits a configurable amount of
  # time (in seconds) before starting the transfer in the hope that multiple
  # replicas will arrive and the transfer can be parallelized.
  #
  # With slow disks and fast (large bandwidth) networks, diskless replication
  # works better.
  repl-diskless-sync no
  
  # When diskless replication is enabled, it is possible to configure the delay
  # the server waits in order to spawn the child that transfers the RDB via socket
  # to the replicas.
  #
  # This is important since once the transfer starts, it is not possible to serve
  # new replicas arriving, that will be queued for the next RDB transfer, so the
  # server waits a delay in order to let more replicas arrive.
  #
  # The delay is specified in seconds, and by default is 5 seconds. To disable
  # it entirely just set it to 0 seconds and the transfer will start ASAP.
  repl-diskless-sync-delay 5
  
  # -----------------------------------------------------------------------------
  # WARNING: RDB diskless load is experimental. Since in this setup the replica
  # does not immediately store an RDB on disk, it may cause data loss during
  # failovers. RDB diskless load + Redis modules not handling I/O reads may also
  # cause Redis to abort in case of I/O errors during the initial synchronization
  # stage with the master. Use only if you know what you are doing.
  # -----------------------------------------------------------------------------
  #
  # Replica can load the RDB it reads from the replication link directly from the
  # socket, or store the RDB to a file and read that file after it was completely
  # received from the master.
  #
  # In many cases the disk is slower than the network, and storing and loading
  # the RDB file may increase replication time (and even increase the master's
  # Copy on Write memory and salve buffers).
  # However, parsing the RDB file directly from the socket may mean that we have
  # to flush the contents of the current database before the full rdb was
  # received. For this reason we have the following options:
  #
  # "disabled"    - Don't use diskless load (store the rdb file to the disk first)
  # "on-empty-db" - Use diskless load only when it is completely safe.
  # "swapdb"      - Keep a copy of the current db contents in RAM while parsing
  #                 the data directly from the socket. note that this requires
  #                 sufficient memory, if you don't have it, you risk an OOM kill.
  repl-diskless-load disabled
  
  # Replicas send PINGs to server in a predefined interval. It's possible to
  # change this interval with the repl_ping_replica_period option. The default
  # value is 10 seconds.
  #
  # repl-ping-replica-period 10
  
  # 主节点发送RDB文件给从节点时的超时时间
  repl-timeout 60
  
  # 是否禁用TCP_NODELAY选项
  # yes: 禁用TCP_NODELAY选项。禁用后，数据无须合并，直接发送至对端
  # no: 启用TCP_NODELAY选项。启用后，TCP协议栈会自动合并较小的数据段，延迟上升。
  repl-disable-tcp-nodelay no
  
  # 主节点的复制积压缓冲区，默认1MB
  repl-backlog-size 1mb
  
  # After a master has no connected replicas for some time, the backlog will be
  # freed. The following option configures the amount of seconds that need to
  # elapse, starting from the time the last replica disconnected, for the backlog
  # buffer to be freed.
  #
  # Note that replicas never free the backlog for timeout, since they may be
  # promoted to masters later, and should be able to correctly "partially
  # resynchronize" with other replicas: hence they should always accumulate backlog.
  #
  # A value of 0 means to never release the backlog.
  #
  # repl-backlog-ttl 3600
  
  # The replica priority is an integer number published by Redis in the INFO
  # output. It is used by Redis Sentinel in order to select a replica to promote
  # into a master if the master is no longer working correctly.
  #
  # A replica with a low priority number is considered better for promotion, so
  # for instance if there are three replicas with priority 10, 100, 25 Sentinel
  # will pick the one with priority 10, that is the lowest.
  #
  # However a special priority of 0 marks the replica as not able to perform the
  # role of master, so a replica with priority of 0 will never be selected by
  # Redis Sentinel for promotion.
  #
  # By default the priority is 100.
  replica-priority 100
  
  # -----------------------------------------------------------------------------
  # By default, Redis Sentinel includes all replicas in its reports. A replica
  # can be excluded from Redis Sentinel's announcements. An unannounced replica
  # will be ignored by the 'sentinel replicas <master>' command and won't be
  # exposed to Redis Sentinel's clients.
  #
  # This option does not change the behavior of replica-priority. Even with
  # replica-announced set to 'no', the replica can be promoted to master. To
  # prevent this behavior, set replica-priority to 0.
  #
  # replica-announced yes
  
  # It is possible for a master to stop accepting writes if there are less than
  # N replicas connected, having a lag less or equal than M seconds.
  #
  # The N replicas need to be in "online" state.
  #
  # The lag in seconds, that must be <= the specified value, is calculated from
  # the last ping received from the replica, that is usually sent every second.
  #
  # This option does not GUARANTEE that N replicas will accept the write, but
  # will limit the window of exposure for lost writes in case not enough replicas
  # are available, to the specified number of seconds.
  #
  # For example to require at least 3 replicas with a lag <= 10 seconds use:
  #
  # min-replicas-to-write 3
  # min-replicas-max-lag 10
  #
  # Setting one or the other to 0 disables the feature.
  #
  # By default min-replicas-to-write is set to 0 (feature disabled) and
  # min-replicas-max-lag is set to 10.
  
  # A Redis master is able to list the address and port of the attached
  # replicas in different ways. For example the "INFO replication" section
  # offers this information, which is used, among other tools, by
  # Redis Sentinel in order to discover replica instances.
  # Another place where this info is available is in the output of the
  # "ROLE" command of a master.
  #
  # The listed IP address and port normally reported by a replica is
  # obtained in the following way:
  #
  #   IP: The address is auto detected by checking the peer address
  #   of the socket used by the replica to connect with the master.
  #
  #   Port: The port is communicated by the replica during the replication
  #   handshake, and is normally the port that the replica is using to
  #   listen for connections.
  #
  # However when port forwarding or Network Address Translation (NAT) is
  # used, the replica may actually be reachable via different IP and port
  # pairs. The following two options can be used by a replica in order to
  # report to its master a specific set of IP and port, so that both INFO
  # and ROLE will report those values.
  #
  # There is no need to use both the options if you need to override just
  # the port or the IP address.
  #
  # replica-announce-ip 5.5.5.5
  # replica-announce-port 1234
  
  ############################### KEYS TRACKING #################################
  
  # Redis implements server assisted support for client side caching of values.
  # This is implemented using an invalidation table that remembers, using
  # a radix key indexed by key name, what clients have which keys. In turn
  # this is used in order to send invalidation messages to clients. Please
  # check this page to understand more about the feature:
  #
  #   https://redis.io/topics/client-side-caching
  #
  # When tracking is enabled for a client, all the read only queries are assumed
  # to be cached: this will force Redis to store information in the invalidation
  # table. When keys are modified, such information is flushed away, and
  # invalidation messages are sent to the clients. However if the workload is
  # heavily dominated by reads, Redis could use more and more memory in order
  # to track the keys fetched by many clients.
  #
  # For this reason it is possible to configure a maximum fill value for the
  # invalidation table. By default it is set to 1M of keys, and once this limit
  # is reached, Redis will start to evict keys in the invalidation table
  # even if they were not modified, just to reclaim memory: this will in turn
  # force the clients to invalidate the cached values. Basically the table
  # maximum size is a trade off between the memory you want to spend server
  # side to track information about who cached what, and the ability of clients
  # to retain cached objects in memory.
  #
  # If you set the value to 0, it means there are no limits, and Redis will
  # retain as many keys as needed in the invalidation table.
  # In the "stats" INFO section, you can find information about the number of
  # keys in the invalidation table at every given moment.
  #
  # Note: when key tracking is used in broadcasting mode, no memory is used
  # in the server side so this setting is useless.
  #
  # tracking-table-max-keys 1000000
  
  ################################## SECURITY ###################################
  
  # Warning: since Redis is pretty fast, an outside user can try up to
  # 1 million passwords per second against a modern box. This means that you
  # should use very strong passwords, otherwise they will be very easy to break.
  # Note that because the password is really a shared secret between the client
  # and the server, and should not be memorized by any human, the password
  # can be easily a long string from /dev/urandom or whatever, so by using a
  # long and unguessable password no brute force attack will be possible.
  
  # Redis ACL users are defined in the following format:
  #
  #   user <username> ... acl rules ...
  #
  # For example:
  #
  #   user worker +@list +@connection ~jobs:* on >ffa9203c493aa99
  #
  # The special username "default" is used for new connections. If this user
  # has the "nopass" rule, then new connections will be immediately authenticated
  # as the "default" user without the need of any password provided via the
  # AUTH command. Otherwise if the "default" user is not flagged with "nopass"
  # the connections will start in not authenticated state, and will require
  # AUTH (or the HELLO command AUTH option) in order to be authenticated and
  # start to work.
  #
  # The ACL rules that describe what a user can do are the following:
  #
  #  on           Enable the user: it is possible to authenticate as this user.
  #  off          Disable the user: it's no longer possible to authenticate
  #               with this user, however the already authenticated connections
  #               will still work.
  #  skip-sanitize-payload    RESTORE dump-payload sanitation is skipped.
  #  sanitize-payload         RESTORE dump-payload is sanitized (default).
  #  +<command>   Allow the execution of that command
  #  -<command>   Disallow the execution of that command
  #  +@<category> Allow the execution of all the commands in such category
  #               with valid categories are like @admin, @set, @sortedset, ...
  #               and so forth, see the full list in the server.c file where
  #               the Redis command table is described and defined.
  #               The special category @all means all the commands, but currently
  #               present in the server, and that will be loaded in the future
  #               via modules.
  #  +<command>|subcommand    Allow a specific subcommand of an otherwise
  #                           disabled command. Note that this form is not
  #                           allowed as negative like -DEBUG|SEGFAULT, but
  #                           only additive starting with "+".
  #  allcommands  Alias for +@all. Note that it implies the ability to execute
  #               all the future commands loaded via the modules system.
  #  nocommands   Alias for -@all.
  #  ~<pattern>   Add a pattern of keys that can be mentioned as part of
  #               commands. For instance ~* allows all the keys. The pattern
  #               is a glob-style pattern like the one of KEYS.
  #               It is possible to specify multiple patterns.
  #  allkeys      Alias for ~*
  #  resetkeys    Flush the list of allowed keys patterns.
  #  &<pattern>   Add a glob-style pattern of Pub/Sub channels that can be
  #               accessed by the user. It is possible to specify multiple channel
  #               patterns.
  #  allchannels  Alias for &*
  #  resetchannels            Flush the list of allowed channel patterns.
  #  ><password>  Add this password to the list of valid password for the user.
  #               For example >mypass will add "mypass" to the list.
  #               This directive clears the "nopass" flag (see later).
  #  <<password>  Remove this password from the list of valid passwords.
  #  nopass       All the set passwords of the user are removed, and the user
  #               is flagged as requiring no password: it means that every
  #               password will work against this user. If this directive is
  #               used for the default user, every new connection will be
  #               immediately authenticated with the default user without
  #               any explicit AUTH command required. Note that the "resetpass"
  #               directive will clear this condition.
  #  resetpass    Flush the list of allowed passwords. Moreover removes the
  #               "nopass" status. After "resetpass" the user has no associated
  #               passwords and there is no way to authenticate without adding
  #               some password (or setting it as "nopass" later).
  #  reset        Performs the following actions: resetpass, resetkeys, off,
  #               -@all. The user returns to the same state it has immediately
  #               after its creation.
  #
  # ACL rules can be specified in any order: for instance you can start with
  # passwords, then flags, or key patterns. However note that the additive
  # and subtractive rules will CHANGE MEANING depending on the ordering.
  # For instance see the following example:
  #
  #   user alice on +@all -DEBUG ~* >somepassword
  #
  # This will allow "alice" to use all the commands with the exception of the
  # DEBUG command, since +@all added all the commands to the set of the commands
  # alice can use, and later DEBUG was removed. However if we invert the order
  # of two ACL rules the result will be different:
  #
  #   user alice on -DEBUG +@all ~* >somepassword
  #
  # Now DEBUG was removed when alice had yet no commands in the set of allowed
  # commands, later all the commands are added, so the user will be able to
  # execute everything.
  #
  # Basically ACL rules are processed left-to-right.
  #
  # For more information about ACL configuration please refer to
  # the Redis web site at https://redis.io/topics/acl
  
  # ACL LOG
  #
  # The ACL Log tracks failed commands and authentication events associated
  # with ACLs. The ACL Log is useful to troubleshoot failed commands blocked 
  # by ACLs. The ACL Log is stored in memory. You can reclaim memory with 
  # ACL LOG RESET. Define the maximum entry length of the ACL Log below.
  acllog-max-len 128
  
  # Using an external ACL file
  #
  # Instead of configuring users here in this file, it is possible to use
  # a stand-alone file just listing users. The two methods cannot be mixed:
  # if you configure users here and at the same time you activate the external
  # ACL file, the server will refuse to start.
  #
  # The format of the external ACL user file is exactly the same as the
  # format that is used inside redis.conf to describe users.
  #
  # aclfile /etc/redis/users.acl
  
  # 设置redis访问密码
  requirepass foobared
  
  # New users are initialized with restrictive permissions by default, via the
  # equivalent of this ACL rule 'off resetkeys -@all'. Starting with Redis 6.2, it
  # is possible to manage access to Pub/Sub channels with ACL rules as well. The
  # default Pub/Sub channels permission if new users is controlled by the 
  # acl-pubsub-default configuration directive, which accepts one of these values:
  #
  # allchannels: grants access to all Pub/Sub channels
  # resetchannels: revokes access to all Pub/Sub channels
  #
  # To ensure backward compatibility while upgrading Redis 6.0, acl-pubsub-default
  # defaults to the 'allchannels' permission.
  #
  # Future compatibility note: it is very likely that in a future version of Redis
  # the directive's default of 'allchannels' will be changed to 'resetchannels' in
  # order to provide better out-of-the-box Pub/Sub security. Therefore, it is
  # recommended that you explicitly define Pub/Sub permissions for all users
  # rather then rely on implicit default values. Once you've set explicit
  # Pub/Sub for all existing users, you should uncomment the following line.
  #
  # acl-pubsub-default resetchannels
  
  # Command renaming (DEPRECATED).
  #
  # ------------------------------------------------------------------------
  # WARNING: avoid using this option if possible. Instead use ACLs to remove
  # commands from the default user, and put them only in some admin user you
  # create for administrative purposes.
  # ------------------------------------------------------------------------
  #
  # It is possible to change the name of dangerous commands in a shared
  # environment. For instance the CONFIG command may be renamed into something
  # hard to guess so that it will still be available for internal-use tools
  # but not available for general clients.
  #
  # Example:
  #
  # rename-command CONFIG b840fc02d524045429941cc15f59e41cb7be6c52
  #
  # It is also possible to completely kill a command by renaming it into
  # an empty string:
  #
  # rename-command CONFIG ""
  #
  # Please note that changing the name of commands that are logged into the
  # AOF file or transmitted to replicas may cause problems.
  
  ################################### CLIENTS ####################################
  
  # redis允许的最大的clients连接数量。默认10000
  maxclients 10000
  
  ############################## MEMORY MANAGEMENT ################################
  # 限制最大可使用内存，对应：used_memory。由于内存碎片率的存在，实际使用内存可能会略大
  maxmemory <bytes>
  
  # 内存溢出控制策略
  # volatile-lru -> Evict using approximated LRU, only keys with an expire set.
  # allkeys-lru -> Evict any key using approximated LRU.
  # volatile-lfu -> Evict using approximated LFU, only keys with an expire set.
  # allkeys-lfu -> Evict any key using approximated LFU.
  # volatile-random -> Remove a random key having an expire set.
  # allkeys-random -> Remove a random key, any key.
  # volatile-ttl -> Remove the key with the nearest expire time (minor TTL)
  # noeviction -> Don't evict anything, just return an error on write operations.
  maxmemory-policy noeviction
  
  # LRU, LFU and minimal TTL algorithms are not precise algorithms but approximated
  # algorithms (in order to save memory), so you can tune it for speed or
  # accuracy. By default Redis will check five keys and pick the one that was
  # used least recently, you can change the sample size using the following
  # configuration directive.
  #
  # The default of 5 produces good enough results. 10 Approximates very closely
  # true LRU but costs more CPU. 3 is faster but not very accurate.
  #
  # maxmemory-samples 5
  
  # Eviction processing is designed to function well with the default setting.
  # If there is an unusually large amount of write traffic, this value may need to
  # be increased.  Decreasing this value may reduce latency at the risk of 
  # eviction processing effectiveness
  #   0 = minimum latency, 10 = default, 100 = process without regard to latency
  #
  # maxmemory-eviction-tenacity 10
  
  # Starting from Redis 5, by default a replica will ignore its maxmemory setting
  # (unless it is promoted to master after a failover or manually). It means
  # that the eviction of keys will be just handled by the master, sending the
  # DEL commands to the replica as keys evict in the master side.
  #
  # This behavior ensures that masters and replicas stay consistent, and is usually
  # what you want, however if your replica is writable, or you want the replica
  # to have a different memory setting, and you are sure all the writes performed
  # to the replica are idempotent, then you may change this default (but be sure
  # to understand what you are doing).
  #
  # Note that since the replica by default does not evict, it may end using more
  # memory than the one set via maxmemory (there are certain buffers that may
  # be larger on the replica, or data structures may sometimes take more memory
  # and so forth). So make sure you monitor your replicas and make sure they
  # have enough memory to never hit a real out-of-memory condition before the
  # master hits the configured maxmemory setting.
  #
  # replica-ignore-maxmemory yes
  
  # Redis reclaims expired keys in two ways: upon access when those keys are
  # found to be expired, and also in background, in what is called the
  # "active expire key". The key space is slowly and interactively scanned
  # looking for expired keys to reclaim, so that it is possible to free memory
  # of keys that are expired and will never be accessed again in a short time.
  #
  # The default effort of the expire cycle will try to avoid having more than
  # ten percent of expired keys still in memory, and will try to avoid consuming
  # more than 25% of total memory and to add latency to the system. However
  # it is possible to increase the expire "effort" that is normally set to
  # "1", to a greater value, up to the value "10". At its maximum value the
  # system will use more CPU, longer cycles (and technically may introduce
  # more latency), and will tolerate less already expired keys still present
  # in the system. It's a tradeoff between memory, CPU and latency.
  #
  # active-expire-effort 1
  
  ############################# LAZY FREEING ####################################
  
  # Redis has two primitives to delete keys. One is called DEL and is a blocking
  # deletion of the object. It means that the server stops processing new commands
  # in order to reclaim all the memory associated with an object in a synchronous
  # way. If the key deleted is associated with a small object, the time needed
  # in order to execute the DEL command is very small and comparable to most other
  # O(1) or O(log_N) commands in Redis. However if the key is associated with an
  # aggregated value containing millions of elements, the server can block for
  # a long time (even seconds) in order to complete the operation.
  #
  # For the above reasons Redis also offers non blocking deletion primitives
  # such as UNLINK (non blocking DEL) and the ASYNC option of FLUSHALL and
  # FLUSHDB commands, in order to reclaim memory in background. Those commands
  # are executed in constant time. Another thread will incrementally free the
  # object in the background as fast as possible.
  #
  # DEL, UNLINK and ASYNC option of FLUSHALL and FLUSHDB are user-controlled.
  # It's up to the design of the application to understand when it is a good
  # idea to use one or the other. However the Redis server sometimes has to
  # delete keys or flush the whole database as a side effect of other operations.
  # Specifically Redis deletes objects independently of a user call in the
  # following scenarios:
  #
  # 1) On eviction, because of the maxmemory and maxmemory policy configurations,
  #    in order to make room for new data, without going over the specified
  #    memory limit.
  # 2) Because of expire: when a key with an associated time to live (see the
  #    EXPIRE command) must be deleted from memory.
  # 3) Because of a side effect of a command that stores data on a key that may
  #    already exist. For example the RENAME command may delete the old key
  #    content when it is replaced with another one. Similarly SUNIONSTORE
  #    or SORT with STORE option may delete existing keys. The SET command
  #    itself removes any old content of the specified key in order to replace
  #    it with the specified string.
  # 4) During replication, when a replica performs a full resynchronization with
  #    its master, the content of the whole database is removed in order to
  #    load the RDB file just transferred.
  #
  # In all the above cases the default is to delete objects in a blocking way,
  # like if DEL was called. However you can configure each case specifically
  # in order to instead release memory in a non-blocking way like if UNLINK
  # was called, using the following configuration directives.
  
  lazyfree-lazy-eviction no
  lazyfree-lazy-expire no
  lazyfree-lazy-server-del no
  replica-lazy-flush no
  
  # It is also possible, for the case when to replace the user code DEL calls
  # with UNLINK calls is not easy, to modify the default behavior of the DEL
  # command to act exactly like UNLINK, using the following configuration
  # directive:
  
  lazyfree-lazy-user-del no
  
  # FLUSHDB, FLUSHALL, and SCRIPT FLUSH support both asynchronous and synchronous
  # deletion, which can be controlled by passing the [SYNC|ASYNC] flags into the
  # commands. When neither flag is passed, this directive will be used to determine
  # if the data should be deleted asynchronously.
  
  lazyfree-lazy-user-flush no
  
  ################################ THREADED I/O #################################
  
  # Redis is mostly single threaded, however there are certain threaded
  # operations such as UNLINK, slow I/O accesses and other things that are
  # performed on side threads.
  #
  # Now it is also possible to handle Redis clients socket reads and writes
  # in different I/O threads. Since especially writing is so slow, normally
  # Redis users use pipelining in order to speed up the Redis performances per
  # core, and spawn multiple instances in order to scale more. Using I/O
  # threads it is possible to easily speedup two times Redis without resorting
  # to pipelining nor sharding of the instance.
  #
  # By default threading is disabled, we suggest enabling it only in machines
  # that have at least 4 or more cores, leaving at least one spare core.
  # Using more than 8 threads is unlikely to help much. We also recommend using
  # threaded I/O only if you actually have performance problems, with Redis
  # instances being able to use a quite big percentage of CPU time, otherwise
  # there is no point in using this feature.
  #
  # So for instance if you have a four cores boxes, try to use 2 or 3 I/O
  # threads, if you have a 8 cores, try to use 6 threads. In order to
  # enable I/O threads use the following configuration directive:
  #
  # io-threads 4
  #
  # Setting io-threads to 1 will just use the main thread as usual.
  # When I/O threads are enabled, we only use threads for writes, that is
  # to thread the write(2) syscall and transfer the client buffers to the
  # socket. However it is also possible to enable threading of reads and
  # protocol parsing using the following configuration directive, by setting
  # it to yes:
  #
  # io-threads-do-reads no
  #
  # Usually threading reads doesn't help much.
  #
  # NOTE 1: This configuration directive cannot be changed at runtime via
  # CONFIG SET. Aso this feature currently does not work when SSL is
  # enabled.
  #
  # NOTE 2: If you want to test the Redis speedup using redis-benchmark, make
  # sure you also run the benchmark itself in threaded mode, using the
  # --threads option to match the number of Redis threads, otherwise you'll not
  # be able to notice the improvements.
  
  ############################ KERNEL OOM CONTROL ##############################
  
  # On Linux, it is possible to hint the kernel OOM killer on what processes
  # should be killed first when out of memory.
  #
  # Enabling this feature makes Redis actively control the oom_score_adj value
  # for all its processes, depending on their role. The default scores will
  # attempt to have background child processes killed before all others, and
  # replicas killed before masters.
  #
  # Redis supports three options:
  #
  # no:       Don't make changes to oom-score-adj (default).
  # yes:      Alias to "relative" see below.
  # absolute: Values in oom-score-adj-values are written as is to the kernel.
  # relative: Values are used relative to the initial value of oom_score_adj when
  #           the server starts and are then clamped to a range of -1000 to 1000.
  #           Because typically the initial value is 0, they will often match the
  #           absolute values.
  oom-score-adj no
  
  # When oom-score-adj is used, this directive controls the specific values used
  # for master, replica and background child processes. Values range -2000 to
  # 2000 (higher means more likely to be killed).
  #
  # Unprivileged processes (not root, and without CAP_SYS_RESOURCE capabilities)
  # can freely increase their value, but not decrease it below its initial
  # settings. This means that setting oom-score-adj to "relative" and setting the
  # oom-score-adj-values to positive values will always succeed.
  oom-score-adj-values 0 200 800
  
  
  #################### KERNEL transparent hugepage CONTROL ######################
  
  # Usually the kernel Transparent Huge Pages control is set to "madvise" or
  # or "never" by default (/sys/kernel/mm/transparent_hugepage/enabled), in which
  # case this config has no effect. On systems in which it is set to "always",
  # redis will attempt to disable it specifically for the redis process in order
  # to avoid latency problems specifically with fork(2) and CoW.
  # If for some reason you prefer to keep it enabled, you can set this config to
  # "no" and the kernel global to "always".
  
  disable-thp yes
  
  ############################## APPEND ONLY MODE ###############################
  # 是否启用AOF，默认关闭
  appendonly no
  
  # AOF文件名称
  appendfilename "appendonly.aof"
  
  # AOF文件同步策略，可选：always、everysec、no
  appendfsync everysec
  
  # 在AOF重写期间不使用fsync操作，防止消耗大量的硬盘资源
  no-appendfsync-on-rewrite no
  
  # 当前AOF文件体积与上一次重写后AOF文件空间的比例。超过此比例即可触发重写
  auto-aof-rewrite-percentage 100
  
  # 运行AOF重写时，最小为文件体积。默认64MB，超过此限制即可触发重写
  auto-aof-rewrite-min-size 64mb
  
  # An AOF file may be found to be truncated at the end during the Redis
  # startup process, when the AOF data gets loaded back into memory.
  # This may happen when the system where Redis is running
  # crashes, especially when an ext4 filesystem is mounted without the
  # data=ordered option (however this can't happen when Redis itself
  # crashes or aborts but the operating system still works correctly).
  #
  # Redis can either exit with an error when this happens, or load as much
  # data as possible (the default now) and start if the AOF file is found
  # to be truncated at the end. The following option controls this behavior.
  #
  # If aof-load-truncated is set to yes, a truncated AOF file is loaded and
  # the Redis server starts emitting a log to inform the user of the event.
  # Otherwise if the option is set to no, the server aborts with an error
  # and refuses to start. When the option is set to no, the user requires
  # to fix the AOF file using the "redis-check-aof" utility before to restart
  # the server.
  #
  # Note that if the AOF file will be found to be corrupted in the middle
  # the server will still exit with an error. This option only applies when
  # Redis will try to read more data from the AOF file but not enough bytes
  # will be found.
  aof-load-truncated yes
  
  # When rewriting the AOF file, Redis is able to use an RDB preamble in the
  # AOF file for faster rewrites and recoveries. When this option is turned
  # on the rewritten AOF file is composed of two different stanzas:
  #
  #   [RDB file][AOF tail]
  #
  # When loading, Redis recognizes that the AOF file starts with the "REDIS"
  # string and loads the prefixed RDB file, then continues loading the AOF
  # tail.
  aof-use-rdb-preamble yes
  
  ################################ LUA SCRIPTING  ###############################
  
  # 限制lua脚本的最大执行时间，默认为5s
  # 设置为0或者负数时，就意味着不限制lua脚本的执行时间
  lua-time-limit 5000
  
  ################################ REDIS CLUSTER  ###############################
  # 是否启用redis cluster模式
  # cluster-enabled yes
  
  # cluster配置文件，redis会根据集群信息，对这个配置文件进行更改
  # cluster-config-file nodes-6379.conf
  
  # 集群中某个节点的超时时间（15s）
  # 1. 判定一个节点的失联事件
  # 2. 决定故障转移时间
  # 3. 决定消息发送频率（cluster-node-timeout / 2）
  # cluster-node-timeout 15000
  
  # A replica of a failing master will avoid to start a failover if its data
  # looks too old.
  #
  # There is no simple way for a replica to actually have an exact measure of
  # its "data age", so the following two checks are performed:
  #
  # 1) If there are multiple replicas able to failover, they exchange messages
  #    in order to try to give an advantage to the replica with the best
  #    replication offset (more data from the master processed).
  #    Replicas will try to get their rank by offset, and apply to the start
  #    of the failover a delay proportional to their rank.
  #
  # 2) Every single replica computes the time of the last interaction with
  #    its master. This can be the last ping or command received (if the master
  #    is still in the "connected" state), or the time that elapsed since the
  #    disconnection with the master (if the replication link is currently down).
  #    If the last interaction is too old, the replica will not try to failover
  #    at all.
  #
  # The point "2" can be tuned by user. Specifically a replica will not perform
  # the failover if, since the last interaction with the master, the time
  # elapsed is greater than:
  #
  #   (node-timeout * cluster-replica-validity-factor) + repl-ping-replica-period
  #
  # So for example if node-timeout is 30 seconds, and the cluster-replica-validity-factor
  # is 10, and assuming a default repl-ping-replica-period of 10 seconds, the
  # replica will not try to failover if it was not able to talk with the master
  # for longer than 310 seconds.
  #
  # A large cluster-replica-validity-factor may allow replicas with too old data to failover
  # a master, while a too small value may prevent the cluster from being able to
  # elect a replica at all.
  #
  # For maximum availability, it is possible to set the cluster-replica-validity-factor
  # to a value of 0, which means, that replicas will always try to failover the
  # master regardless of the last time they interacted with the master.
  # (However they'll always try to apply a delay proportional to their
  # offset rank).
  #
  # Zero is the only value able to guarantee that when all the partitions heal
  # the cluster will always be able to continue.
  #
  # cluster-replica-validity-factor 10
  
  # Cluster replicas are able to migrate to orphaned masters, that are masters
  # that are left without working replicas. This improves the cluster ability
  # to resist to failures as otherwise an orphaned master can't be failed over
  # in case of failure if it has no working replicas.
  #
  # Replicas migrate to orphaned masters only if there are still at least a
  # given number of other working replicas for their old master. This number
  # is the "migration barrier". A migration barrier of 1 means that a replica
  # will migrate only if there is at least 1 other working replica for its master
  # and so forth. It usually reflects the number of replicas you want for every
  # master in your cluster.
  #
  # Default is 1 (replicas migrate only if their masters remain with at least
  # one replica). To disable migration just set it to a very large value or
  # set cluster-allow-replica-migration to 'no'.
  # A value of 0 can be set but is useful only for debugging and dangerous
  # in production.
  #
  # cluster-migration-barrier 1
  
  # Turning off this option allows to use less automatic cluster configuration.
  # It both disables migration to orphaned masters and migration from masters
  # that became empty.
  #
  # Default is 'yes' (allow automatic migrations).
  #
  # cluster-allow-replica-migration yes
  
  # 当发生故障转移时，如果存在slots不可用
  # yes：整个集群不可用
  # no：与转移槽相关的命令不可用
  # cluster-require-full-coverage yes
  
  # This option, when set to yes, prevents replicas from trying to failover its
  # master during master failures. However the replica can still perform a
  # manual failover, if forced to do so.
  #
  # This is useful in different scenarios, especially in the case of multiple
  # data center operations, where we want one side to never be promoted if not
  # in the case of a total DC failure.
  #
  # cluster-replica-no-failover no
  
  # This option, when set to yes, allows nodes to serve read traffic while the
  # the cluster is in a down state, as long as it believes it owns the slots. 
  #
  # This is useful for two cases.  The first case is for when an application 
  # doesn't require consistency of data during node failures or network partitions.
  # One example of this is a cache, where as long as the node has the data it
  # should be able to serve it. 
  #
  # The second use case is for configurations that don't meet the recommended  
  # three shards but want to enable cluster mode and scale later. A 
  # master outage in a 1 or 2 shard configuration causes a read/write outage to the
  # entire cluster without this option set, with it set there is only a write outage.
  # Without a quorum of masters, slot ownership will not change automatically. 
  #
  # cluster-allow-reads-when-down no
  
  
  ########################## CLUSTER DOCKER/NAT support  ########################
  
  # In certain deployments, Redis Cluster nodes address discovery fails, because
  # addresses are NAT-ted or because ports are forwarded (the typical case is
  # Docker and other containers).
  #
  # In order to make Redis Cluster working in such environments, a static
  # configuration where each node knows its public address is needed. The
  # following four options are used for this scope, and are:
  #
  # * cluster-announce-ip
  # * cluster-announce-port
  # * cluster-announce-tls-port
  # * cluster-announce-bus-port
  #
  # Each instructs the node about its address, client ports (for connections
  # without and with TLS) and cluster message bus port. The information is then
  # published in the header of the bus packets so that other nodes will be able to
  # correctly map the address of the node publishing the information.
  #
  # If cluster-tls is set to yes and cluster-announce-tls-port is omitted or set
  # to zero, then cluster-announce-port refers to the TLS port. Note also that
  # cluster-announce-tls-port has no effect if cluster-tls is set to no.
  #
  # If the above options are not used, the normal Redis Cluster auto-detection
  # will be used instead.
  #
  # Note that when remapped, the bus port may not be at the fixed offset of
  # clients port + 10000, so you can specify any port and bus-port depending
  # on how they get remapped. If the bus-port is not set, a fixed offset of
  # 10000 will be used as usual.
  #
  # Example:
  #
  # cluster-announce-ip 10.1.1.5
  # cluster-announce-tls-port 6379
  # cluster-announce-port 0
  # cluster-announce-bus-port 6380
  
  ################################## SLOW LOG ###################################
  # 慢查询配置项。如果一条消息的超过10ms，就会被加入慢查询日志中。慢查询日志队列的默认长度为128，若已达到默
  # 认长度限制，就会丢弃掉最早入队的消息。
  slowlog-log-slower-than 10000 #微秒，默认配置=10ms
  slowlog-max-len 128
  
  ################################ LATENCY MONITOR ##############################
  
  # The Redis latency monitoring subsystem samples different operations
  # at runtime in order to collect data related to possible sources of
  # latency of a Redis instance.
  #
  # Via the LATENCY command this information is available to the user that can
  # print graphs and obtain reports.
  #
  # The system only logs operations that were performed in a time equal or
  # greater than the amount of milliseconds specified via the
  # latency-monitor-threshold configuration directive. When its value is set
  # to zero, the latency monitor is turned off.
  #
  # By default latency monitoring is disabled since it is mostly not needed
  # if you don't have latency issues, and collecting data has a performance
  # impact, that while very small, can be measured under big load. Latency
  # monitoring can easily be enabled at runtime using the command
  # "CONFIG SET latency-monitor-threshold <milliseconds>" if needed.
  latency-monitor-threshold 0
  
  ############################# EVENT NOTIFICATION ##############################
  
  # Redis can notify Pub/Sub clients about events happening in the key space.
  # This feature is documented at https://redis.io/topics/notifications
  #
  # For instance if keyspace events notification is enabled, and a client
  # performs a DEL operation on key "foo" stored in the Database 0, two
  # messages will be published via Pub/Sub:
  #
  # PUBLISH __keyspace@0__:foo del
  # PUBLISH __keyevent@0__:del foo
  #
  # It is possible to select the events that Redis will notify among a set
  # of classes. Every class is identified by a single character:
  #
  #  K     Keyspace events, published with __keyspace@<db>__ prefix.
  #  E     Keyevent events, published with __keyevent@<db>__ prefix.
  #  g     Generic commands (non-type specific) like DEL, EXPIRE, RENAME, ...
  #  $     String commands
  #  l     List commands
  #  s     Set commands
  #  h     Hash commands
  #  z     Sorted set commands
  #  x     Expired events (events generated every time a key expires)
  #  e     Evicted events (events generated when a key is evicted for maxmemory)
  #  t     Stream commands
  #  d     Module key type events
  #  m     Key-miss events (Note: It is not included in the 'A' class)
  #  A     Alias for g$lshzxetd, so that the "AKE" string means all the events
  #        (Except key-miss events which are excluded from 'A' due to their
  #         unique nature).
  #
  #  The "notify-keyspace-events" takes as argument a string that is composed
  #  of zero or multiple characters. The empty string means that notifications
  #  are disabled.
  #
  #  Example: to enable list and generic events, from the point of view of the
  #           event name, use:
  #
  #  notify-keyspace-events Elg
  #
  #  Example 2: to get the stream of the expired keys subscribing to channel
  #             name __keyevent@0__:expired use:
  #
  #  notify-keyspace-events Ex
  #
  #  By default all notifications are disabled because most users don't need
  #  this feature and the feature has some overhead. Note that if you don't
  #  specify at least one of K or E, no events will be delivered.
  notify-keyspace-events ""
  
  ############################### GOPHER SERVER #################################
  
  # Redis contains an implementation of the Gopher protocol, as specified in
  # the RFC 1436 (https://www.ietf.org/rfc/rfc1436.txt).
  #
  # The Gopher protocol was very popular in the late '90s. It is an alternative
  # to the web, and the implementation both server and client side is so simple
  # that the Redis server has just 100 lines of code in order to implement this
  # support.
  #
  # What do you do with Gopher nowadays? Well Gopher never *really* died, and
  # lately there is a movement in order for the Gopher more hierarchical content
  # composed of just plain text documents to be resurrected. Some want a simpler
  # internet, others believe that the mainstream internet became too much
  # controlled, and it's cool to create an alternative space for people that
  # want a bit of fresh air.
  #
  # Anyway for the 10nth birthday of the Redis, we gave it the Gopher protocol
  # as a gift.
  #
  # --- HOW IT WORKS? ---
  #
  # The Redis Gopher support uses the inline protocol of Redis, and specifically
  # two kind of inline requests that were anyway illegal: an empty request
  # or any request that starts with "/" (there are no Redis commands starting
  # with such a slash). Normal RESP2/RESP3 requests are completely out of the
  # path of the Gopher protocol implementation and are served as usual as well.
  #
  # If you open a connection to Redis when Gopher is enabled and send it
  # a string like "/foo", if there is a key named "/foo" it is served via the
  # Gopher protocol.
  #
  # In order to create a real Gopher "hole" (the name of a Gopher site in Gopher
  # talking), you likely need a script like the following:
  #
  #   https://github.com/antirez/gopher2redis
  #
  # --- SECURITY WARNING ---
  #
  # If you plan to put Redis on the internet in a publicly accessible address
  # to server Gopher pages MAKE SURE TO SET A PASSWORD to the instance.
  # Once a password is set:
  #
  #   1. The Gopher server (when enabled, not by default) will still serve
  #      content via Gopher.
  #   2. However other commands cannot be called before the client will
  #      authenticate.
  #
  # So use the 'requirepass' option to protect your instance.
  #
  # Note that Gopher is not currently supported when 'io-threads-do-reads'
  # is enabled.
  #
  # To enable Gopher support, uncomment the following line and set the option
  # from no (the default) to yes.
  #
  # gopher-enabled no
  
  ############################### ADVANCED CONFIG ###############################
  # hash结构：当元素数量小于512，并且每个元素小于64字节时。否则使用hashtable
  hash-max-ziplist-entries 512
  hash-max-ziplist-value 64
  
  # Lists are also encoded in a special way to save a lot of space.
  # The number of entries allowed per internal list node can be specified
  # as a fixed maximum size or a maximum number of elements.
  # For a fixed maximum size, use -5 through -1, meaning:
  # -5: max size: 64 Kb  <-- not recommended for normal workloads
  # -4: max size: 32 Kb  <-- not recommended
  # -3: max size: 16 Kb  <-- probably not recommended
  # -2: max size: 8 Kb   <-- good
  # -1: max size: 4 Kb   <-- good
  # Positive numbers mean store up to _exactly_ that number of elements
  # per list node.
  # The highest performing option is usually -2 (8 Kb size) or -1 (4 Kb size),
  # but if your use case is unique, adjust the settings as necessary.
  list-max-ziplist-size -2
  
  # Lists may also be compressed.
  # Compress depth is the number of quicklist ziplist nodes from *each* side of
  # the list to *exclude* from compression.  The head and tail of the list
  # are always uncompressed for fast push/pop operations.  Settings are:
  # 0: disable all list compression
  # 1: depth 1 means "don't start compressing until after 1 node into the list,
  #    going from either the head or tail"
  #    So: [head]->node->node->...->node->[tail]
  #    [head], [tail] will always be uncompressed; inner nodes will compress.
  # 2: [head]->[next]->node->node->...->node->[prev]->[tail]
  #    2 here means: don't compress head or head->next or tail->prev or tail,
  #    but compress all nodes between them.
  # 3: [head]->[next]->[next]->node->node->...->node->[prev]->[prev]->[tail]
  # etc.
  list-compress-depth 0
  
  # 当集合中的元素都是整数，且元素个数小于512时，采用intset编码。否则使用hashtable
  set-max-intset-entries 512
  
  # 当集合元素个数小于128，并且每个元素小于64字节时，采用ziplist编码。否则使用skiplist
  zset-max-ziplist-entries 128
  zset-max-ziplist-value 64
  
  # HyperLogLog sparse representation bytes limit. The limit includes the
  # 16 bytes header. When an HyperLogLog using the sparse representation crosses
  # this limit, it is converted into the dense representation.
  #
  # A value greater than 16000 is totally useless, since at that point the
  # dense representation is more memory efficient.
  #
  # The suggested value is ~ 3000 in order to have the benefits of
  # the space efficient encoding without slowing down too much PFADD,
  # which is O(N) with the sparse encoding. The value can be raised to
  # ~ 10000 when CPU is not a concern, but space is, and the data set is
  # composed of many HyperLogLogs with cardinality in the 0 - 15000 range.
  hll-sparse-max-bytes 3000
  
  # Streams macro node max size / items. The stream data structure is a radix
  # tree of big nodes that encode multiple items inside. Using this configuration
  # it is possible to configure how big a single node can be in bytes, and the
  # maximum number of items it may contain before switching to a new node when
  # appending new stream entries. If any of the following settings are set to
  # zero, the limit is ignored, so for instance it is possible to set just a
  # max entries limit by setting max-bytes to 0 and max-entries to the desired
  # value.
  stream-node-max-bytes 4096
  stream-node-max-entries 100
  
  # Active rehashing uses 1 millisecond every 100 milliseconds of CPU time in
  # order to help rehashing the main Redis hash table (the one mapping top-level
  # keys to values). The hash table implementation Redis uses (see dict.c)
  # performs a lazy rehashing: the more operation you run into a hash table
  # that is rehashing, the more rehashing "steps" are performed, so if the
  # server is idle the rehashing is never complete and some more memory is used
  # by the hash table.
  #
  # The default is to use this millisecond 10 times every second in order to
  # actively rehash the main dictionaries, freeing memory when possible.
  #
  # If unsure:
  # use "activerehashing no" if you have hard latency requirements and it is
  # not a good thing in your environment that Redis can reply from time to time
  # to queries with 2 milliseconds delay.
  #
  # use "activerehashing yes" if you don't have such hard requirements but
  # want to free memory asap when possible.
  activerehashing yes
  
  # 普通客户端输出缓冲区。无限制。
  client-output-buffer-limit normal 0 0 0
  # 主从复制客户端输出缓冲区。如果在60s内，缓冲区消耗超过64MB，或者总大小超过超过256MB。溢出，主从同步失败
  client-output-buffer-limit replica 256mb 64mb 60
  # 发布订阅客户端输出缓冲区。如果在60s内，缓冲区消耗超过8MB，或者总大小超过超过32MB。溢出，订阅失败
  client-output-buffer-limit pubsub 32mb 8mb 60
  
  # Client query buffers accumulate new commands. They are limited to a fixed
  # amount by default in order to avoid that a protocol desynchronization (for
  # instance due to a bug in the client) will lead to unbound memory usage in
  # the query buffer. However you can configure it here if you have very special
  # needs, such us huge multi/exec requests or alike.
  #
  # client-query-buffer-limit 1gb
  
  # In the Redis protocol, bulk requests, that are, elements representing single
  # strings, are normally limited to 512 mb. However you can change this limit
  # here, but must be 1mb or greater
  #
  # proto-max-bulk-len 512mb
  
  # Redis calls an internal function to perform many background tasks, like
  # closing connections of clients in timeout, purging expired keys that are
  # never requested, and so forth.
  #
  # Not all tasks are performed with the same frequency, but Redis checks for
  # tasks to perform according to the specified "hz" value.
  #
  # By default "hz" is set to 10. Raising the value will use more CPU when
  # Redis is idle, but at the same time will make Redis more responsive when
  # there are many keys expiring at the same time, and timeouts may be
  # handled with more precision.
  #
  # The range is between 1 and 500, however a value over 100 is usually not
  # a good idea. Most users should use the default of 10 and raise this up to
  # 100 only in environments where very low latency is required.
  hz 10
  
  # Normally it is useful to have an HZ value which is proportional to the
  # number of clients connected. This is useful in order, for instance, to
  # avoid too many clients are processed for each background task invocation
  # in order to avoid latency spikes.
  #
  # Since the default HZ value by default is conservatively set to 10, Redis
  # offers, and enables by default, the ability to use an adaptive HZ value
  # which will temporarily raise when there are many connected clients.
  #
  # When dynamic HZ is enabled, the actual configured HZ will be used
  # as a baseline, but multiples of the configured HZ value will be actually
  # used as needed once more clients are connected. In this way an idle
  # instance will use very little CPU time while a busy instance will be
  # more responsive.
  dynamic-hz yes
  
  # When a child rewrites the AOF file, if the following option is enabled
  # the file will be fsync-ed every 32 MB of data generated. This is useful
  # in order to commit the file to the disk more incrementally and avoid
  # big latency spikes.
  aof-rewrite-incremental-fsync yes
  
  # When redis saves RDB file, if the following option is enabled
  # the file will be fsync-ed every 32 MB of data generated. This is useful
  # in order to commit the file to the disk more incrementally and avoid
  # big latency spikes.
  rdb-save-incremental-fsync yes
  
  # Redis LFU eviction (see maxmemory setting) can be tuned. However it is a good
  # idea to start with the default settings and only change them after investigating
  # how to improve the performances and how the keys LFU change over time, which
  # is possible to inspect via the OBJECT FREQ command.
  #
  # There are two tunable parameters in the Redis LFU implementation: the
  # counter logarithm factor and the counter decay time. It is important to
  # understand what the two parameters mean before changing them.
  #
  # The LFU counter is just 8 bits per key, it's maximum value is 255, so Redis
  # uses a probabilistic increment with logarithmic behavior. Given the value
  # of the old counter, when a key is accessed, the counter is incremented in
  # this way:
  #
  # 1. A random number R between 0 and 1 is extracted.
  # 2. A probability P is calculated as 1/(old_value*lfu_log_factor+1).
  # 3. The counter is incremented only if R < P.
  #
  # The default lfu-log-factor is 10. This is a table of how the frequency
  # counter changes with a different number of accesses with different
  # logarithmic factors:
  #
  # +--------+------------+------------+------------+------------+------------+
  # | factor | 100 hits   | 1000 hits  | 100K hits  | 1M hits    | 10M hits   |
  # +--------+------------+------------+------------+------------+------------+
  # | 0      | 104        | 255        | 255        | 255        | 255        |
  # +--------+------------+------------+------------+------------+------------+
  # | 1      | 18         | 49         | 255        | 255        | 255        |
  # +--------+------------+------------+------------+------------+------------+
  # | 10     | 10         | 18         | 142        | 255        | 255        |
  # +--------+------------+------------+------------+------------+------------+
  # | 100    | 8          | 11         | 49         | 143        | 255        |
  # +--------+------------+------------+------------+------------+------------+
  #
  # NOTE: The above table was obtained by running the following commands:
  #
  #   redis-benchmark -n 1000000 incr foo
  #   redis-cli object freq foo
  #
  # NOTE 2: The counter initial value is 5 in order to give new objects a chance
  # to accumulate hits.
  #
  # The counter decay time is the time, in minutes, that must elapse in order
  # for the key counter to be divided by two (or decremented if it has a value
  # less <= 10).
  #
  # The default value for the lfu-decay-time is 1. A special value of 0 means to
  # decay the counter every time it happens to be scanned.
  #
  # lfu-log-factor 10
  # lfu-decay-time 1
  
  ########################### ACTIVE DEFRAGMENTATION #######################
  #
  # What is active defragmentation?
  # -------------------------------
  #
  # Active (online) defragmentation allows a Redis server to compact the
  # spaces left between small allocations and deallocations of data in memory,
  # thus allowing to reclaim back memory.
  #
  # Fragmentation is a natural process that happens with every allocator (but
  # less so with Jemalloc, fortunately) and certain workloads. Normally a server
  # restart is needed in order to lower the fragmentation, or at least to flush
  # away all the data and create it again. However thanks to this feature
  # implemented by Oran Agra for Redis 4.0 this process can happen at runtime
  # in a "hot" way, while the server is running.
  #
  # Basically when the fragmentation is over a certain level (see the
  # configuration options below) Redis will start to create new copies of the
  # values in contiguous memory regions by exploiting certain specific Jemalloc
  # features (in order to understand if an allocation is causing fragmentation
  # and to allocate it in a better place), and at the same time, will release the
  # old copies of the data. This process, repeated incrementally for all the keys
  # will cause the fragmentation to drop back to normal values.
  #
  # Important things to understand:
  #
  # 1. This feature is disabled by default, and only works if you compiled Redis
  #    to use the copy of Jemalloc we ship with the source code of Redis.
  #    This is the default with Linux builds.
  #
  # 2. You never need to enable this feature if you don't have fragmentation
  #    issues.
  #
  # 3. Once you experience fragmentation, you can enable this feature when
  #    needed with the command "CONFIG SET activedefrag yes".
  #
  # The configuration parameters are able to fine tune the behavior of the
  # defragmentation process. If you are not sure about what they mean it is
  # a good idea to leave the defaults untouched.
  
  # Enabled active defragmentation
  # activedefrag no
  
  # Minimum amount of fragmentation waste to start active defrag
  # active-defrag-ignore-bytes 100mb
  
  # Minimum percentage of fragmentation to start active defrag
  # active-defrag-threshold-lower 10
  
  # Maximum percentage of fragmentation at which we use maximum effort
  # active-defrag-threshold-upper 100
  
  # Minimal effort for defrag in CPU percentage, to be used when the lower
  # threshold is reached
  # active-defrag-cycle-min 1
  
  # Maximal effort for defrag in CPU percentage, to be used when the upper
  # threshold is reached
  # active-defrag-cycle-max 25
  
  # Maximum number of set/hash/zset/list fields that will be processed from
  # the main dictionary scan
  # active-defrag-max-scan-fields 1000
  
  # Jemalloc background thread for purging will be enabled by default
  jemalloc-bg-thread yes
  
  # It is possible to pin different threads and processes of Redis to specific
  # CPUs in your system, in order to maximize the performances of the server.
  # This is useful both in order to pin different Redis threads in different
  # CPUs, but also in order to make sure that multiple Redis instances running
  # in the same host will be pinned to different CPUs.
  #
  # Normally you can do this using the "taskset" command, however it is also
  # possible to this via Redis configuration directly, both in Linux and FreeBSD.
  #
  # You can pin the server/IO threads, bio threads, aof rewrite child process, and
  # the bgsave child process. The syntax to specify the cpu list is the same as
  # the taskset command:
  #
  # Set redis server/io threads to cpu affinity 0,2,4,6:
  # server_cpulist 0-7:2
  #
  # Set bio threads to cpu affinity 1,3:
  # bio_cpulist 1,3
  #
  # Set aof rewrite child process to cpu affinity 8,9,10,11:
  # aof_rewrite_cpulist 8-11
  #
  # Set bgsave child process to cpu affinity 1,10,11
  # bgsave_cpulist 1,10-11
  
  # In some cases redis will emit warnings and even refuse to start if it detects
  # that the system is in bad state, it is possible to suppress these warnings
  # by setting the following config which takes a space delimited list of warnings
  # to suppress
  #
  # ignore-warnings ARM64-COW-BUG
  ```
  
- **sentine.conf**

  ```
  # Example sentinel.conf
  # 注：redis-server --sentinel会在运行期间往配置文件中添加其余slave和其他sentinel的信息
  
  # 端口，默认6379
  port 26379
  
  # 是否作为守护进程，默认为no
  daemonize no
  
  # 日志输出位置，""将输出值stdout
  logfile ""
  
  # 工作目录，可通过容器映射至外部
  dir /tmp
  
  # 监控127.0.0.1:6379上的主节点，别名为mymaster，随便起就行
  # 2意味着判定主节点最终不可达最少需要2个的sentinel节点
  sentinel monitor mymaster 127.0.0.1 6379 2
  
  # 连接到主节点上所需要的密码
  sentinel auth-pass mymaster sensetime
  
  # 判定主节点、从节点、sentinel节点是否有效的超时时间为30s
  sentinel down-after-milliseconds mymaster 30000
  
  # 设置sentinel的登录密码
  requirepass <password>
  
  # 故障转移成功并选举出新的主节点后，同时仅允许1个从节点向主节点发起复制请求
  sentinel parallel-syncs mymaster 1
  
  # 故障转移超时时间，默认为3min
  sentinel failover-timeout mymaster 180000
  
  # 故障转移期间，会通过以下脚本进行通知事件信息。默认无需配置
  # sentinel notification-script mymaster /var/redis/notify.sh
  
  # 故障转移结束后，会通过以下脚本进行通知转移结果。默认无需配置
  # sentinel client-reconfig-script mymaster /var/redis/reconfig.sh
  ```

## 工具链

### redis-cli

- `-h <hostname>`：Server hostname (default: 127.0.0.1).
- `-p <port>`：Server port (default: 6379).
- `-a <password>`：Password to use when connecting to the server.
- `-r <repeat>`：遇到失败时，尝试执行n次
- `-i <interval>`：使用`-r`选项重复执行时 ，每个间隔`<interval>`秒
- `-x`：从STDIN中读取参数的值，例如：`echo "world" | redis-cli -x set hello`
- `-c`：启用Cluster Mode
- `--raw`：要求返回原始的数据格式（二进制编码）
- `--no-raw`：返回编码后的数据格式
- `--csv`：返回csv类型的数据格式
- `--stat`：从Keys、Mem、Clients、Blocked、Requests、Connections几个方面打印Redis Server的状态
- `--latency`：测试Client <-> Redis Server之间的延迟，秒单位。
- `--latency-history`：以分时段的方式了解延迟信息。可以使用`-i`控制分段时间。
  - 例如：`redis-cli  -i 3 --latency-history`
- `--latency-dist`：以分段图表的方式打印延迟信息
- `--rdb <filename>`：请求Redis生成并发送RDB文件，保存至本地
- `--pipe`：将原始的protocol发送给Redis Server
- `--pipe-timeout <n>`：限制发送原始protocol的超时时间
- `--bigkeys`：使用scan命令对Redis的键进行采样,从中找到内存占用比较大的键值，这些键可能是系统的瓶颈
- `--memkeys`：使用scan命令对Redis的键进行采样,从中找到内存占用比较大的键值，这些键可能是系统的瓶颈
- `--memkeys-samples`：找出n个内存占用过大的keys
- `--hotkeys`：找出系统的hot keys。仅在LFU淘汰策略下工作。
- `--eval <file>`：运行指定的Lua Scripts
- `--ldb`：Used with --eval enable the Redis Lua debugger.
- `--slave`：把当前客户端模拟成当前Redis节点的从节点，可以用来获取当前Redis节点的更新操作。合理的利用这个选项可以记录当前连接Redis节点的一些更新操作
- `--cluster <command> [args...] [opts...]`：Cluster Manager command and arguments

### redis-cli --cluster

- `create host1:port1 ... hostN:portN`：创建集群

  - `--cluster-replicas <arg>`：从节点个数

  ```c
  # 创建单节点集群
  redis-cli --cluster create 0.0.0.0:6379 0.0.0.0:6380 0.0.0.0:6381
  
  # 创建主从节点集群
  redis-cli --cluster create 0.0.0.0:6379 0.0.0.0:6380 0.0.0.0:6381 0.0.0.0:6382 0.0.0.0:6383 0.0.0.0:6384 --cluster-replicas 1
  ```

- `check host:port`：检查集群

  - `--cluster-search-multiple-owners`：检查是否有槽被同时分配到了多个节点

  ```c
  # 检查集群状态
  redis-cli -a sensetime --cluster check 0.0.0.0:6379
  
  # 检查是否有重复分配的槽
  redis-cli -a sensetime --cluster check 0.0.0.0:6379 --cluster-search-multiple-owners
  ```

- `info host:port`：查看集群状态

  ```c
  redis-cli -a sensetime --cluster info 0.0.0.0:6379
  ```

- `fix host:port`：修复集群

  - `--cluster-search-multiple-owners`：修复槽的重复分配问题

  ```c
  # 修复集群
  redis-cli -a sensetime --cluster fix 0.0.0.0:6379
  
  # 修复槽的重复分配问题
  redis-cli -a sensetime --cluster fix 0.0.0.0:6379 --cluster-search-multiple-owners
  ```

- `reshard host:port`：指定集群的任意一节点进行迁移slot，重新分slots

  - `--cluster-from <arg>`：需要从哪些源节点上迁移slot，可从多个源节点完成迁移，以逗号隔开，传递的是节点的node id，还可以直接传递`--from all`
  - `--cluster-to <arg>`：slot需要迁移的目的节点的node id，目的节点只能填写一个，不传递该参数的话，则会在迁移过程中提示用户输入
  - `--cluster-slots <arg>`：需要迁移的slot数量，不传递该参数的话，则会在迁移过程中提示用户输入
  - `--cluster-yes`：指定迁移时的确认输入
  - `--cluster-timeout <arg>`：设置migrate命令的超时时间
  - `--cluster-pipeline <arg>`：定义cluster getkeysinslot命令一次取出的key数量，不传的话使用默认值为10
  - `--cluster-replace`：是否直接replace到目标节点

  ```c
  # 注：此处必须使用node id，不能使用ip:port
  redis-cli -a sensetime --cluster reshard 0.0.0.0:6379 --cluster-from 309145e418b4defd80011a1ad681f849d396fc9a --cluster-to ca8b01ea3629395f3bb1ff8e485bb6041990dba1 --cluster-slots 1000 --cluster-yes
  ```

- `rebalance host:port`：指定集群的任意一节点进行平衡集群节点slot数量

  - `--cluster-weight <node1=w1...nodeN=wN>`：指定集群节点的权重
  - `--cluster-use-empty-masters`：设置可以让没有分配slot的主节点参与，默认不允许
  - `--cluster-timeout <arg>`：设置migrate命令的超时时间
  - `--cluster-simulate`：拟rebalance操作，不会真正执行迁移操作
  - `--cluster-pipeline <arg>`：定义cluster getkeysinslot命令一次取出的key数量，默认值为10
  - `--cluster-threshold <arg>`：迁移的slot阈值超过threshold，执行rebalance操作
  - `--cluster-replace`：是否直接replace到目标节点

  ```c
  # 最简洁的reblance命令
  redis-cli -a sensetime --cluster rebalance 0.0.0.0:6379
  ```

- `add-node new_host:new_port existing_host:existing_port`：添加节点，把新节点加入到指定的集群，默认添加主节点

  - `--cluster-slave`：新节点作为从节点，默认随机一个主节点
  - `--cluster-master-id <arg>`：给新节点指定主节点

  ```c
  # 将6382加入6379已成型的集群
  # 注：仅添加，但是没有分配slots
  redis-cli -a sensetime --cluster add-node 0.0.0.0:6382 0.0.0.0:6379
  
  # 将6383作为6379的slave加入集群
  # 注：此处的cluster-master-id可以通过fix命令获取    
  redis-cli -a sensetime --cluster add-node 0.0.0.0:6383 0.0.0.0:6379 --cluster-slave --cluster-master-id 5ec64003ff4b3f681149cdc7c4a3ab2b835d2072
  ```

- `del-node host:port node_id`：删除给定的一个节点，成功后关闭该节点服务

  ```c
  # 删除从节点
  redis-cli -a sensetime --cluster del-node 0.0.0.0:6383 b82b092317b4f1dd1d89a652b62d340972be482e
  
  # 删除无slots的主节点
  redis-cli -a sensetime --cluster del-node 0.0.0.0:6382 ca8b01ea3629395f3bb1ff8e485bb6041990dba1
  
  # 删除有slots的主节点
  # 注：无法直接删除有slots的主节点    
  ```

- `call host:port command arg arg .. arg`：在集群的所有节点执行相关命令

- `set-timeout host:port milliseconds`：设置cluster-node-timeout

- `import host:port `：将外部redis数据导入集群

  - `--cluster-from <arg>`：将指定实例的数据导入到集群
  - `--cluster-copy`：migrate时指定copy
  - `--cluster-replace`：migrate时指定replace

- `help`

### redis-server

- `--test-memory <MBs>`：测试当前机器的内存是否够用
- `--sentinel`：启动哨兵模式
- `redis-server /etc/redis.conf`：读取配置文件并启动

### redis-benchmark

- `-h <hostname>`：Server hostname (default: 127.0.0.1).
- `-p <port>`：Server port (default: 6379).
- `-a <password>`：Password to use when connecting to the server.
- `-c <clients>`：代表客户端的并发数量。默认50
- `-n <requests>`：总的请求数量。默认100000
- `-d <size>`：GET/SET请求的数据量。默认3bytes
- `--dbnum <db>`：选择那个数据库进行测试。默认0
- `--threads <num>`：启用多少个线程发送数据
- `--cluster`：启用cluster模式
- `-k <boolean>`：客户端是否使用keepalive选项。1表示启用，0表示不启用。默认1
- `-r <keyspacelen>`：向Redis Server插入更多的随机键。
  - 例如`redis-benchmark -c 100 -n 20000 -r 10000`：Redis会在Key Name后加一个12位的后缀，`-r 10000`表示只对后四位做随机处理
- `-P <numreq>`：每个请求中pipeline的数量。默认为1
- `-e`：如果出错，会将错误日志打印至终端
- `-q`：仅打印requests per second信息
- `--csv`：输出csv格式的信息
- `-l`：Loop。永远不停下测试。
- `-t <tests>`：仅对指定命令进行基准测试。
  - 例`redis-benchmark -t get,set -q`，仅对GET、SET命令测试

### redis-check-aof

AOF持久化文件检测和修复工具。

### redis-check-rdb

RDB持久化文件检测和修复工具。

```shell
redis-check-rdb ./test.rdb
redis-check-aof ./test.aof
```

## 监控

- 命令耗时统计
- 慢查询
- 持久化阻塞
- 连接拒绝
- 内存/CPU/网络/磁盘过载