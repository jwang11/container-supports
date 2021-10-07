# container-supports知识准备


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

理解控制组（cpu, cpu-account）的[使用](cgroup)
