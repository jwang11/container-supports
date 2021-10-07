# 深入理解容器知识准备


## Capabilities

Linux将传统上与root用户关联的特权划分为不同的功能集，这些功能组称为capabilites。

See [here](capabilities)

## Seccomp

Seccomp是linux内核提供的沙箱机制，可以限制进程对系统调用的访问，从系统调用号，到系统调用的参数，都可以检查和限制

See [here](seccomp)

## rlimits

利用getrlimit()/setrlimit()函数[编程](rlimit)

获取或设置资源使用限制，linux下每种资源都有相关的软硬限制，软限制是内核强加给相应资源的限制值，硬限制是软限制的最大值。非授权调用的进程只能将其软限制指定为0~硬限制范围中的某个值，同时能不可逆转地降低其硬限制。授权进程可以任意改变其软硬限制。RLIM_INFINITY:表示不对资源限制。
```
resource：可能的选择有
 
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
- Context
- Struct和接口的继承
- bolt数据库

[编程](golang)
