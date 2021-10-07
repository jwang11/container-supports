# 深入理解容器知识准备


## Capabilities

Linux将传统上与root用户关联的特权划分为不同的功能集，这些功能组称为capabilites。

See [here](capabilities)

## Seccomp

Seccomp是linux内核提供的沙箱机制，可以限制进程对系统调用的访问，从系统调用号，到系统调用的参数，都可以检查和限制

See [here](seccomp)

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


##  Pty伪终端

伪终端(pseudo terminal，有时也被称为pty)是指伪终端master和伪终端slave这一对字符设备。其中的slave对应/dev/pts/ 目录下的一个文件，而master则在内存中标识为一个文件描述符(fd)。伪终端由终端模拟器提供，终端模拟器是一个运行在用户态的应用程序。我们

/dev/ptmx 是一个字符设备文件，当进程打开 /dev/ptmx 文件时，进程会同时获得一个指向 pseudoterminal master(ptm)的文件描述符和一个在 /dev/pts 目录中创建的 pseudoterminal slave(pts) 设备。对这个文件描述符的读写都会被 ptmx 转发到对应的 pts。
一般情况下我们通过远程连接的方式执行命令时，进程的标准输入、标准输出和标准错误输出都会绑定到伪终端上。

如何[Pty编程](pty)
