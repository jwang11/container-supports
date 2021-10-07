# ID的概念

## UID(用户ID)，EUID(有效用户ID)和SUID(设置用户ID)
> UID 全称是 Real User ID，也可以写成RUID <br>
> EUID 全称是Effective User ID <br>
> SUID 全称是Saved set-user-ID 

- UID，就是我们当前以哪个用户登录的，我们运行的程序的UID就是这个用户的ID。
- EID就是当前进程是以哪个用户ID来运行的，一般情况下是EUID=UID，如果可执行文件具有了SUID权限，那么它的EUID=File Owner ID。
- SUID对可执行文件来说，是个设置位（S）。对于进程来说，SUID就是有效用户ID的一个副本。

为了避免混淆，只说了User，没有说Group。GID, EGID区别可以参照理解。

## SID，PGID
> SID全称Session ID，会话ID <br>
> PGID全称是Process Group ID，进程组ID

### 1. Session会话
session就是一组进程组的集合，session id就是这个session中leader的进程ID。

- session的特点
session的主要特点是当session的leader退出后，session中的所有其它进程将会收到SIGHUP信号，其默认行为是终止进程，即session的leader退出后，session中的其它进程也会退出。

如果session和tty关联的话，它们之间只能一一对应，一个tty只能属于一个session，一个session只能打开一个tty。当然session也可以不和任何tty关联。

- session的创建
session可以在任何时候创建，调用setsid函数即可，session中的第一个进程即为这个session的leader，leader是不能变的。常见的创建session的场景是：

    * 用户登录后，启动shell时将会创建新的session，shell会作为session的leader，随后shell里面运行的进程都将属于这个session，当shell退出后，所有该用户运行的进程将退出。
    * session一般都会和一个特定的tty关联，session的leader会成为tty的控制进程
    * 当session里的进程组发生变化时，控制进程负责更新tty上进程组
    * 当tty要关闭的时候，控制进程所在session的所有进程都会收到SIGHUP信号。
    * 启动deamon进程，这类进程需要和父进程划清界限，所以需要启动一个新的session(调用setsid)，而且不和tty关联。

### 2. PG 进程组
进程组（process group）是一组进程的集合，进程组id就是这个进程组中leader的进程ID。

* 进程组的特点
进程组的主要特点是可以以进程组为单位通过函数killpg发送信号

* 进程组的创建
进程组主要用在shell里面，shell负责进程组的管理，包括创建、销毁等。（这里shell就是session的leader）

对大部分进程来说，它自己就是进程组的leader，并且进程组里面就只有它自己一个进程

shell里面执行类似ls|more这样的以管道连接起来的命令时，两个进程就属于同一个进程组，ls是进程组的leader。

shell里面启动一个进程后，一般都会将该进程放到一个单独的进程组，然后该进程fork的所有进程都会属于该进程组，比如多进程的程序，
它的所有进程都会属于同一个进程组，当在shell里面按下CTRL+C时，该程序的所有进程都会收到SIGINT而退出。

* 后台进程组
shell中启动一个进程时，默认情况下，该进程就是一个前端进程组的leader，可以收到用户的输入，并且可以将输出打印到终端，只有当该进程组退出后，shell才可以再响应用户的输入。

但我们也可以将该进程组运行在后台，这样shell就可以继续相应用户的输入，常见的方法如下：

    - 启动程序时，在后面加&，如sleep 1000 &，进程将会进入后台继续运行
    - 程序启动后，可以按CTRL+Z让它进入后台，和后面加&不同的是，进程会被暂停执行

对于后台运行的进程组，在shell里面体现为job的概念，即一个后台进程组就是一个job，所有后台运行的进程组可以通过jobs命令查看到，也可以通过fg命令将后台进程组切换到前端，这样就可以继续接收用户的输入了

```console
$ sleep 100&
[1] 7405
$ jobs
[1]+  Running                 sleep 100 &

$ fg 1
sleep 100
^Z
[1]+  Stopped                 sleep 100
$ jobs
[1]+  Stopped                 sleep 100

```

### 3. Session和进程组关系

这里看一下shell作为session leader的情况，假设我们在shell里面执行了这些命令：
```console
$ sleep 100&
[1] 7486
$ cat |wc -l&
[2] 7488
$ jobs
[1]-  Running                 sleep 100 &
[2]+  Stopped                 cat | wc -l

+--------------------------------------------------------------+
|                                                              |
|      pg1             pg2             pg3            pg4      |
|    +------+       +-------+        +-----+        +------+   |
|    | bash |       | sleep |        | cat |        | jobs |   |
|    +------+       +-------+        +-----+        +------+   |
| session leader                     | wc  |                   |
|                                    +-----+                   |
|                                                              |
+--------------------------------------------------------------+
                            session
```
    * bash是session的leader，sleep、cat、wc和jobs这四个进程都由bash fork而来，所以他们也属于这个session
    * bash也是自己所在进程组的leader
    * bash会为自己启动的每个进程都创建一个新的进程组，所以这里sleep和jobs进程属于自己单独的进程组
    * 对于用管道符号“|”连接起来的命令，bash会将它们放到一个进程组中

## Nohup的背后

为了把进程放到后台运行（不受tty影响），我们经常用nohup命令。其实，nohup干了这么几件事：

* 将stdin重定向到/dev/null，于是程序读标准输入将会返回EOF
* stdout和stderr重定向到nohup.out或者用户通过参数指定的文件，程序所有输出到stdout和stderr的内容将会写入该文件
* 蔽掉SIGHUP信号
* 用exec启动指定的命令（nohup进程将会被新进程取代，但进程ID不变）

有几点需要明白：
* nohup程序不负责将进程放到后台，这也是为什么我们经常在nohup命令后面要加上符号“&”的原因
* 于stdin、stdout和stderr都被重定向了，nohup启动的程序不会读写tty
* 于stdin重定向到了/dev/null，程序读stdin的时候会收到EOF返回值
* session leader（如bash）退出后，该进程会收到SIGHUP信号，但由于nohup帮我们忽略了该信号，所以该进程不会退出，但依然属于该Session

## Daemon
---

通过nohup，就可以实现让进程在后台一直执行的功能，为什么我们还要写deamon进程呢？

从上面的nohup的介绍中可以看出来，虽然进程是在后台执行，但进程跟当前session还是有着千丝万缕的关系，至少其父进程还是被session管着的，所以我们还是需要一个跟任何session都没有关系的进程来实现deamon的功能。实现deamon进程的大概步骤如下：

* 调用fork生成一个新进程，然后原来的进程退出，这样新进程就变成了孤儿进程，于是被init进程接收，这样新进程就和调用进程没有父子关系了。
* 调用setsid，创建新的session，新进程将成为新session的leader，同时该新session不和任何tty关联。
* 切换当前工作目录到其它地方，一般是切换到根目录，这样就取消了对原工作目录的引用，如果原工作目录是某个挂载点下面的目录，这样就不会影响该挂载点的卸载。
* 关闭一些从父进程继承过来而自己不需要的fd，避免不小心读写这些fd。
* 重定向stdin、stdout和stderr，避免读写它们出现错误。

## setsid，setuid和setpgid
---

- setpgid
```code
NAME
       setpgid - set process group

SYNOPSIS
       #include <sys/types.h>
       #include <unistd.h>
       
       int setpgid(pid_t pid, pid_t pgid);
 
DESCRIPTION
       setpgid()  sets  the  PGID of the process specified by pid to pgid.  If
       pid is zero, then the process ID of the calling process  is  used.   If
       pgid is zero, then the PGID of the process specified by pid is made the
       same as its process ID.  If setpgid() is used to move  a  process  from
       one  process  group to another (as is done by some shells when creating
       pipelines), both process groups must be part of the same  session  (see
       setsid(2) and credentials(7)).  In this case, the pgid specifies an ex‐
       isting process group to be joined and the session ID of that group must
       match the session ID of the joining process.

```

- setuid
```diff
NAME
       setuid - set user identity

SYNOPSIS
       #include <sys/types.h>
       #include <unistd.h>

       int setuid(uid_t uid);

DESCRIPTION
       setuid()  sets  the  effective  user ID of the calling process.  If the
       calling process is privileged (more precisely: if the process  has  the
       CAP_SETUID  capability  in  its user namespace), the real UID and saved
       set-user-ID are also set.

+      // 在set-user-ID完成了工作后，通常setuid(getuid())设置使euid恢复到原来的uid
       Under Linux, setuid() is implemented like the POSIX version with
       the  _POSIX_SAVED_IDS feature.  This allows a set-user-ID (other
       than root) program to drop all of its user privileges,  do  some
       un-privileged  work,  and  then  reengage the original effective
       user ID in a secure manner.


       If  the  user  is root or the program is set-user-ID-root, special care
       must be taken: setuid() checks the effective user ID of the caller  and
       if  it  is the superuser, all process-related user ID's are set to uid.
       After this has occurred, it is impossible for  the  program  to  regain
       root privileges.

```

- setsid
```code
NAME
       setsid - run a program in a new session

SYNOPSIS
       setsid [options] program [arguments]

DESCRIPTION
       setsid  runs  a  program in a new session. The command calls fork(2) if
       already a process group leader.  Otherwise, it executes  a  program  in
       the  current process.  This default behavior is possible to override by
       the --fork option.
```
