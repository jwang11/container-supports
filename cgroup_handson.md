### 实战

#### PID Namespace
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
