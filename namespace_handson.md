## 实战


### 进程和Namespace
```
root@jwang-desktop:~# sudo ls -l /proc/$$/ns
total 0
lrwxrwxrwx 1 root root 0 9月  28 15:55 cgroup -> 'cgroup:[4026531835]'
lrwxrwxrwx 1 root root 0 9月  28 15:55 ipc -> 'ipc:[4026531839]'
lrwxrwxrwx 1 root root 0 9月  28 15:55 mnt -> 'mnt:[4026531840]'
lrwxrwxrwx 1 root root 0 9月  28 15:55 net -> 'net:[4026532008]'
lrwxrwxrwx 1 root root 0 9月  28 15:55 pid -> 'pid:[4026531836]'
lrwxrwxrwx 1 root root 0 9月  28 15:55 pid_for_children -> 'pid:[4026531836]'
lrwxrwxrwx 1 root root 0 9月  28 15:55 time -> 'time:[4026531834]'
lrwxrwxrwx 1 root root 0 9月  28 15:55 time_for_children -> 'time:[4026531834]'
lrwxrwxrwx 1 root root 0 9月  28 15:55 user -> 'user:[4026531837]'
lrwxrwxrwx 1 root root 0 9月  28 15:55 uts -> 'uts:[4026531838]'
```

### PID Namespace
- 终端1：
```diff
$ sudo unshare --fork --pid bash

- // $$表示当前进程
$ echo $$
1
```

- 终端2：
```
$ps -ef|tail -n 4

root       12660    5954  0 17:17 pts/3    00:00:00 sudo unshare --fork --pid bash
root       12661   12660  0 17:17 pts/3    00:00:00 unshare --fork --pid bash   
```
>> init=1的进程 <br>
>> 1. 孤儿进程默认挂在PID=1下面
>> 2. 其它进程声明reaper，孤儿挂在该进程下面
```diff
$ pstree -s
systemd(1)─┬─ModemManager(722)─┬─{ModemManager}(745)
           │                   └─{ModemManager}(751)
           ├─NetworkManager(620)─┬─{NetworkManager}(695)
           │                     └─{NetworkManager}(714)
           ├─accounts-daemon(609)─┬─{accounts-daemon}(618)
           │                      └─{accounts-daemon}(710)
           ├─acpid(610)
           ├─avahi-daemon(613)───avahi-daemon(658)
           ├─colord(1617)─┬─{colord}(1620)
           │              └─{colord}(1623)
           ├─containerd(762)─┬─{containerd}(777)
           │                 ├─{containerd}(778)
           │                 ├─{containerd}(779)
           │                 ├─{containerd}(780)
           │                 ├─{containerd}(781)
           │                 ├─{containerd}(783)
           │                 ├─{containerd}(784)
           │                 ├─{containerd}(785)
           │                 ├─{containerd}(786)
           │                 ├─{containerd}(787)
           │                 ├─{containerd}(788)
           │                 ├─{containerd}(789)
           │                 ├─{containerd}(807)
           │                 ├─{containerd}(808)
           │                 ├─{containerd}(809)
           │                 ├─{containerd}(2113)
           │                 ├─{containerd}(2152)
           │                 ├─{containerd}(2153)
           │                 ├─{containerd}(2154)
           │                 ├─{containerd}(2155)
           │                 ├─{containerd}(3919)
           │                 └─{containerd}(13865)
           ├─containerd-shim(12084)─┬─sh(12105)
           │                        ├─{containerd-shim}(12085)
           │                        ├─{containerd-shim}(12086)
           │                        ├─{containerd-shim}(12087)
           │                        ├─{containerd-shim}(12088)
           │                        ├─{containerd-shim}(12089)
           │                        ├─{containerd-shim}(12090)
           │                        ├─{containerd-shim}(12091)
           │                        ├─{containerd-shim}(12092)
           │                        ├─{containerd-shim}(12093)
           │                        ├─{containerd-shim}(12095)
           │                        ├─{containerd-shim}(12112)
           │                        ├─{containerd-shim}(12113)
           │                        └─{containerd-shim}(12114)

```

### Mount Namespace
```diff
- // --mount-proc含义是在新进程起来之前，把/proc在新的namespace下挂载一下，否则是parent的copy
$ sudo unshare --pid --fork --mount-proc bash
$ ps -ef
UID          PID    PPID  C STIME TTY          TIME CMD
root           1       0  0 18:28 pts/2    00:00:00 bash
root           8       1  0 18:28 pts/2    00:00:00 ps -ef
```

### Net Namespace
```diff
$ sudo unshare --fork --net bash
$ ifconfig

```

### User Namespace

- 终端1：
```diff
$ id
uid=1000(jwang) gid=1000(jwang) groups=1000(jwang),4(adm),24(cdrom),27(sudo),30(dip),46(plugdev),120(lpadmin),131(lxd),132(sambashare),133(docker)

$ readlink /proc/$$/ns/user
user:[4026531837]

$unshare --user /bin/bash
nobody@jwang-desktop:~$

nobody@jwang-desktop:~$ readlink /proc/$$/ns/user
user:[4026532349]

nobody@jwang-desktop:~$ id
uid=65534(nobody) gid=65534(nogroup) groups=65534(nogroup)

nobody@jwang-desktop:~$ echo $$
16100

nobody@jwang-desktop:~$ cat /proc/$$/uid_map
空的
```

- 终端2
```diff
$ cat /proc/$$/status | egrep 'Cap(Inh|Prm|Eff)'
CapInh: 0000000000000000
CapPrm: 0000000000000000
CapEff: 0000000000000000

$ sudo setcap cap_setgid,cap_setuid+ep /bin/bash
$ exec bash
$ cat /proc/$$/status | egrep 'Cap(Inh|Prm|Eff)'
CapInh: 0000000000000000
CapPrm: 00000000000000c0
CapEff: 00000000000000c0

$ echo '0 1000 1' > /proc/16100/uid_map

$ echo '0 1000 1' > /proc/16100/gid_map
```


- 终端1
```diff
nobody@jwang-desktop:~$ id
uid=0(root) gid=0(root) groups=0(root),65534(nogroup)

nobody@jwang-desktop:~$ exec bash
root@jwang-desktop:~#
```
