## Capabilities


### 1. Capabilities是什么?

* 在内核2.2之前

    为了检查进程权限，将进程分为两类
    * 特权进程(euid=0)。root用户，或者sudo+cmd，或者直接带有suid标志的程序可以获取完整的root权限来对系统进行操作。
    * 非特权进程（euid!=0)。

* 从内核2.2开始

    Linux将传统上与root用户关联的特权划分为不同的功能组，这些功能组称为capabilites。
    
    Capabilites作为线程（进程）的属性存在，每个功能组可以独立启用和禁用。如此一来，权限检查的过程就变成了：<br>
在执行特权操作时，如果进程的有效身份不是root，就去检查是否具有该特权操作所对应的 capabilites，并以此决定是否可以进行该特权操作。比如要向进程发送信号(kill())，就得具有 capability CAP_KILL；如果设置系统时间，就得具有 capability CAP_SYS_TIME。

| Capability | 描述   |
| -------- | ------ |
| CAP_AUDIT_CONTROL   | 启用和禁用内核审计；改变审计过滤规则；检索审计状态和过滤规则 |
| CAP_AUDIT_WRITE   | 将记录写入内核审计日志  |
| CAP_CHOWN | 修改文件所有者的权限 |
| CAP_IPC_LOCK | 允许锁定共享内存片段  |
| CAP_KILL | 允许对不属于自己的进程发送信号  |
| CAP_MAC_ADMIN | 允许 MAC 配置或状态更改  |
| CAP_MKNOD | 允许使用 mknod() 系统调用  |
| CAP_NET_ADMIN | 允许执行网络管理任务  |
| CAP_NET_RAW | 允许使用原始套接字  |
| CAP_SETUID | 允许改变进程的 UID  |
| CAP_SYS_ADMIN | 允许执行系统管理任务，如加载或卸载文件系统、设置磁盘配额等  |
| CAP_SYS_CHROOT | 允许使用 chroot() 系统调用  |
| CAP_SYS_TIME | 允许改变系统时钟  |
| CAP_SYS_TTY_CONFIG | 允许配置 TTY 设备  |
| CAP_SYSLOG | 允许使用 syslog() 系统调用  |
| ... | ...  |

### 2. 如何使用capabilities?

- 命令行 - getcap命令和setcap 命令分别用来查看和设置程序文件的 capabilities属性。
```diff
$ sudo setcap cap_setgid,cap_setuid+ep /bin/bash
$ sudo getcap /bin/bash
/bin/bash = cap_setgid,cap_setuid+ep
$ exec bash
$ cat /proc/$$/status | egrep 'Cap(Inh|Prm|Eff)'
CapInh: 0000000000000000
CapPrm: 00000000000000c0
CapEff: 00000000000000c0

$ echo '0 1000 1' > /proc/16100/uid_map
$ echo '0 1000 1' > /proc/16100/gid_map

+ # 命令中的ep分别表示Effective和Permitted集合
+ # "+"号表示把指定的 capabilities添加到这些集合中，"-"号表示从集合中移除。
```
