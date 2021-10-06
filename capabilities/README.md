
### 1. Capabilities是什么?
---

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
---
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
- 编程
```
CAP_CLEAR(3)               Linux Programmer's Manual              CAP_CLEAR(3)

NAME
       cap_clear,  cap_clear_flag,  cap_get_flag,  cap_set_flag, cap_compare -
       capability data object manipulation

SYNOPSIS
       #include <sys/capability.h>

       int cap_clear(cap_t cap_p);

       int cap_clear_flag(cap_t cap_p, cap_flag_t flag);

       int cap_get_flag(cap_t cap_p, cap_value_t cap,
                        cap_flag_t flag, cap_flag_value_t *value_p);

       int cap_set_flag(cap_t cap_p, cap_flag_t flag, int ncap,
                        const cap_value_t *caps, cap_flag_value_t value);

       int cap_compare(cap_t cap_a, cap_t cap_b);

DESCRIPTION
       These functions work on a capability state held in working storage.  A  cap_t
       holds  information  about the capabilities in each of the three sets, Permit‐
       ted, Inheritable, and Effective.  Each capability in a set may be clear (dis‐
       abled, 0) or set (enabled, 1).

       These functions work with the following data types:

       cap_value_t       identifies a capability, such as CAP_CHOWN.

       cap_flag_t        identifies  one  of the three flags associated with a capa‐
                         bility (i.e., it identifies one  of  the  three  capability
                         sets).   Valid  values  for  this  type  are CAP_EFFECTIVE,
                         CAP_INHERITABLE or CAP_PERMITTED.

       cap_flag_value_t  identifies the setting  of  a  particular  capability  flag
                         (i.e,  the  value  of a capability in a set).  Valid values
                         for this type are CAP_CLEAR (0) or CAP_SET (1).
       cap_clear() initializes the capability state in working storage identified by
       cap_p so that all capability flags are cleared.

       cap_clear_flag()  clears  all of the capabilities of the specified capability
       flag, flag.

       cap_get_flag() obtains the current value of the capability flag, flag, of the
       capability,  cap, from the capability state identified by cap_p and places it
       in the location pointed to by value_p.

       cap_set_flag() sets the flag, flag, of each capability in the array  caps  in
       the  capability  state  identified by cap_p to value.  The argument, ncap, is
       used to specify the number of capabilities in the array, caps.

       cap_compare() compares two full capability sets and, in the  spirit  of  mem‐
       cmp(),  returns zero if the two capability sets are identical. A positive re‐
       turn value, status, indicates there is a difference  between  them.  The  re‐
       turned   value  carries  further  information  about  which  of  three  sets,
       cap_flag_t flag, differ. Specifically, the macro CAP_DIFFERS  (status,  flag)
       evaluates to non-zero if the returned status differs in its flag components.

```

```
CAP_GET_PROC(3)            Linux Programmer's Manual           CAP_GET_PROC(3)

NAME
       cap_get_proc,  cap_set_proc  -  capability  manipulation  on
       processes
      
SYNOPSIS       
       #include <sys/capability.h>

       cap_t cap_get_proc(void);

       int cap_set_proc(cap_t cap_p);


DESCRIPTION
       cap_get_proc()  allocates  a  capability state in working storage, sets
       its state to that of the calling process, and returns a pointer to this
       newly  created capability state.  The caller should free any releasable
       memory, when the capability state in working storage is no  longer  re‐
       quired, by calling cap_free() with the cap_t as an argument.

       cap_set_proc()  sets  the values for all capability flags for all capa‐
       bilities to the capability state identified by cap_p.  The new capabil‐
       ity  state of the process will be completely determined by the contents
       of cap_p upon successful return from this function.   If  any  flag  in
       cap_p is set for any capability not currently permitted for the calling
       process, the function will  fail,  and  the  capability  state  of  the
       process will remain unchanged.
```

### 3. 可执行文件的Capabilities
---

可执行文件有三种capabilities（保存在文件属性里），它们分别是：

* Permitted
* Inheritable
* Effective

在进程执行时，
1. Permitted 集合中的 capabilites 自动被加入到进程的 Permitted 集合中。
2. Inheritable集合中的capabilites会与进程的Inheritable集合执行&操作，以确定进程在执行execve函数后哪些capabilites被继承。
3. Effective是一个bit。如果设置为开启，那么在执行execve函数后，Permitted集合中新增的capabilities会自动出现在进程的Effective集合中。

### 4. 进程的Capabilities
----

进程有五种capabilities类型，
* Permitted
* Inheritable
* Effective
* Bounding
* Ambient

相比文件apabilites，进程的capabilities多了两个集合，分别是Bounding和Ambient。
/proc/[pid]/status 文件中包含了进程的五个Capabilities集合的信息，我们可以通过下面的命名查看当前进程的Capabilities信息：
