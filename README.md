# 深入理解容器知识准备


## Capabilities

Linux将传统上与root用户关联的特权划分为不同的功能集，这些功能组称为capabilites。

See [here](capabilities)

## Apparmor

如何使用Apparmor限制app的权限，see [here](apparmor_handson.md)

## Seccomp

Seccomp是linux内核提供的沙箱机制，可以限制进程对系统调用的访问，从系统调用号，到系统调用的参数，都可以检查和限制

See [here](seccomp)

## rlimits

利用getrlimit()/setrlimit()函数[编程](rlimit)

获取或设置资源使用限制，linux下每种资源都有相关的软硬限制，软限制是内核强加给相应资源的限制值，硬限制是软限制的最大值。非授权调用的进程只能将其软限制指定为0~硬限制范围中的某个值，同时能不可逆转地降低其硬限制。授权进程可以任意改变其软硬限制。RLIM_INFINITY:表示不对资源限制。resource有：
```
RLIMIT_AS //进程的最大虚内存空间，字节为单位。
RLIMIT_CORE //内核转存文件的最大长度。
RLIMIT_CPU //最大允许的CPU使用时间，秒为单位。当进程达到软限制，内核将给其发送SIGXCPU信号，这一信号的默认行为是终止进程的执行。然而，可以捕捉信号，处理句柄可将控制返回给主程序。如果进程继续耗费CPU时间，核心会以每秒一次的频率给其发送SIGXCPU信号，直到达到硬限制，那时将给进程发送 SIGKILL信号终止其执行。
RLIMIT_DATA //进程数据段的最大值。
RLIMIT_FSIZE //进程可建立的文件的最大长度。如果进程试图超出这一限制时，核心会给其发送SIGXFSZ信号，默认情况下将终止进程的执行。
RLIMIT_LOCKS //进程可建立的锁和租赁的最大值。
RLIMIT_MEMLOCK //进程可锁定在内存中的最大数据量，字节为单位。
RLIMIT_MSGQUEUE //进程可为POSIX消息队列分配的最大字节数。
RLIMIT_NICE //进程可通过setpriority() 或 nice()调用设置的最大完美值。
RLIMIT_NOFILE //指定比进程可打开的最大文件描述词大一的值，超出此值，将会产生EMFILE错误。
RLIMIT_NPROC //用户可拥有的最大进程数。
RLIMIT_RTPRIO //进程可通过sched_setscheduler 和 sched_setparam设置的最大实时优先级。
RLIMIT_SIGPENDING //用户可拥有的最大挂起信号数。
RLIMIT_STACK //最大的进程堆栈，以字节为单位。
```

## 各种ID（UID，EUID, PID，PGID）的概念

* UID 全称是 Real User ID，也可以写成RUID
* EUID 全称是Effective User ID
* SUID 全称是Saved set-user-ID 
* SID全称Session ID，会话ID
* PGID全称是Process Group ID，进程组ID

see [here](printid)

## Namespace命名空间

理解Linux各种命名空间（PID，Mount，User，Network等）的[使用](namespace_handson.md)和[编程](namespace)


## Cgroup控制组

理解控制组（cpu, cpu-account）和systemd关系，以及[使用](cgroup_handson.md)


## Pty伪终端

伪终端(pseudo terminal，有时也被称为pty)是指伪终端master和伪终端slave这一对字符设备。其中的slave对应/dev/pts/ 目录下的一个文件，而master则在内存中标识为一个文件描述符(fd)。伪终端由终端模拟器提供，终端模拟器是一个运行在用户态的应用程序。我们

/dev/ptmx 是一个字符设备文件，当进程打开 /dev/ptmx 文件时，进程会同时获得一个指向 pseudoterminal master(ptm)的文件描述符和一个在 /dev/pts 目录中创建的 pseudoterminal slave(pts) 设备。对这个文件描述符的读写都会被 ptmx 转发到对应的 pts。
一般情况下我们通过远程连接的方式执行命令时，进程的标准输入、标准输出和标准错误输出都会绑定到伪终端上。

如何[Pty编程](pty)

## Golang的高级编程

- go gRPC
    * gRPC全称是google Remote Procedure Call,是一个高性能、开源和通用的 RPC 框架，面向移动和 HTTP/2 设计。gRPC 基于 HTTP/2 标准设计，带来诸如双向流、流控、头部压缩、单 TCP 连接上的多复用请求等特性
    * 服务接口和传递的消息均以protobuf的IDL语言统一描述：分别以service和message对象表示。protoc支持插件，grpc就是通过grpc插件来实现用protoc生成client和server的代码
    * Server端的主要代码实际上是一个接口。用户需要自己提供一个类，实现该接口，启动server时把这个类注册成这个服务的实现类。
    * Protocol Buffers(简称Protobuf) ，是Google出品的序列化框架，与开发语言无关，和平台无关，具有良好的可扩展性。Protobuf和所有的序列化框架一样，都可以用于数据存储、通讯协议。Protobuf支持生成代码的语言包括Java、Python、C++、Go、JavaNano、Ruby、C#, Portobuf的序列化的结果体积要比XML、JSON小很多，XML和JSON的描述信息太多了，导致消息要大；此外Portobuf还使用了Varint 编码，减少数据对空间的占用。Portobuf序列化和反序列化速度比XML、JSON快很多，是直接把对象和字节数组做转换，而XML和JSON还需要构建成XML或者JSON对象结构。
 
 ```console
 protoc -I helloworld/ helloworld/helloworld.proto --go_out=plugins=grpc:helloworld
 ```
- Context
    * context包专门用来简化处理单个请求的多个goroutine之间与请求域的数据、取消信号、截止时间等相关操作。
    * 控制goroutine的生命周期
- WaitGroup   
- Struct和接口的继承
- bolt数据库
    * Bolt是一个用Go编写的键值数据库。给程序提供一个简单、快捷、稳定的数据库。
    * 支持事务。boltdb所有的修改操作都是在内存中完成，最终提交事务后进行落盘操作。

[编程](golang)

## systemd的socket activation

- Systemd是一套用来管理主机上各个Daemon运行的工具, 开发目标是提供更优秀的框架以表示系统服务间的依赖关系，并依此实现系统初始化时服务的并行启动.
- 如果Daemon B依赖Daemon A，那么常见的就是B启动时会向A发起连接。由于Daemon A和Daemon B通常在一台主机上, 因此它们之间的连接通常使用Unix Socket完成.
- Socket activation的思想就是: Daemon B启动时其实并不需要Daemon A真正运行起来,它只需要Daemon A建立的socket处于listen状态就OK了。 而这个socket不必由Daemon A建立, 而是由systemd在系统初始化时就建立， 当Daemon B发起启动时发起连接，systemd再将Daemon A启动。

## systemd的sd_notify

systemd有一个本机基于套接字的healthcheck方法，sd_notify。

sd_notify套接字机制在应用程序完全Ready时通知systemd系统，通过设置service配置里ype = notify以启用此功能.
通常程序里会检查NOTIFY_SOCKET环境变量以获取名称,并在应用程序Ready后，把READY = 1写入该套接字.
