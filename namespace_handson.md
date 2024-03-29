# 实战


## 进程和Namespace

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

## PID Namespace

- 终端1：
```diff
$ sudo unshare --fork --pid bash

+ // $$表示当前进程
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

这时候，新的PID namespace里仍然可以看到原来namespace进程，如何完全隔离呢？/proc重新加载一下就好了，ps里的进程是从/proc里读出来的
```diff
+ // --mount-proc含义是在新进程起来之前，把/proc在新的mount namespace重新挂载一下（隐含建立一个新mount namespace），否则是parent的copy
$ sudo unshare --pid --fork --mount-proc bash
$ ps -ef
UID          PID    PPID  C STIME TTY          TIME CMD
root           1       0  0 18:28 pts/2    00:00:00 bash
root           8       1  0 18:28 pts/2    00:00:00 ps -ef
```
## Mount Namespace

每个挂载点都有一个propagation type标志, 由它来决定当一个挂载点的下面创建和移除挂载点的时候，是否会传播到属于相同peer group的其他挂载点下去，也即同一个peer group里的其他的挂载点下面是不是也会创建和移除相应的挂载点.现在有4种不同类型的propagation type：

   * MS_SHARED: 从名字就可以看出，挂载信息会在同一个peer group的不同挂载点之间共享传播. 当一个挂载点下面添加或者删除挂载点的时候，同一个peer group里的其他挂载点下面也会挂载和卸载同样的挂载点
   * MS_PRIVATE: 跟上面的刚好相反，挂载信息根本就不共享，也即private的挂载点不会属于任何peer group
   * MS_SLAVE: 跟名字一样，信息的传播是单向的，在同一个peer group里面，master的挂载点下面发生变化的时候，slave的挂载点下面也跟着变化，但反之则不然，slave下发生变化的时候不会通知master，master不会发生变化。
   * MS_UNBINDABLE: 这个和MS_PRIVATE相同，只是这种类型的挂载点不能作为bind mount的源，主要用来防止递归嵌套情况的出现。 

#### 挂载点的Peer group和Propagation Type
```diff
+ # 准备四个虚拟disk，并创建ext2文件系统，用于mount测试
$ mkdir disks
$ cd disks/
$ dd if=/dev/zero bs=1M count=32 of=./disk1.img
32+0 records in
32+0 records out
$ mkfs.ext2 ./disk1.img

+ # 重复上面步骤生成并格式化disk2.img, disk3.img, disk4.img

$ mkdir disk1 disk2
$ ls
disk1  disk1.img  disk2  disk2.img  disk3.img  disk4.img

# 显示的以shared 方式挂载disk1, private方式挂载disk2
$ mount --make-shared ./disk1.img ./disk1
$ mount --make-private ./disk2.img ./disk2

$ cat /proc/self/mountinfo |grep disks
500 29 7:11 / /home/jwang/disks/disk1 rw,relatime shared:435 - ext2 /dev/loop11 rw
1002 29 7:12 / /home/jwang/disks/disk2 rw,relatime - ext2 /dev/loop12 rw

$ mkdir ./disk1/disk3 ./disk2/disk4
$ sudo mount ./disk3.img ./disk1/disk3
$ sudo mount ./disk4.img ./disk2/disk4

$ cat /proc/self/mountinfo |grep disk
500 29 7:11 / /home/jwang/disks/disk1 rw,relatime shared:435 - ext2 /dev/loop11 rw
1002 29 7:12 / /home/jwang/disks/disk2 rw,relatime - ext2 /dev/loop12 rw
1148 500 7:13 / /home/jwang/disks/disk1/disk3 rw,relatime shared:639 - ext2 /dev/loop13 rw
1215 1002 7:14 / /home/jwang/disks/disk2/disk4 rw,relatime - ext2 /dev/loop14 rw

+ # mountinfo信息显示，第一，二列是挂载点ID和父挂载点ID，500和1148的类型都是shared，而1002和1215的类型都是private的，
+ # 在默认mount的情况下，子挂载点会继承父挂载点的propagation type,但是 peer group不同

$ umount disk1/disk3/
$ umount disk2/disk4/
```

#### bind-mount测试
```diff
$ mkdir bind1 bind2

$ cat /proc/self/mountinfo |grep disk
500 29 7:11 / /home/jwang/disks/disk1 rw,relatime shared:435 - ext2 /dev/loop11 rw
1002 29 7:12 / /home/jwang/disks/disk2 rw,relatime - ext2 /dev/loop12 rw
1216 29 7:11 / /home/jwang/disks/bind1 rw,relatime shared:435 - ext2 /dev/loop11 rw
1240 29 7:12 / /home/jwang/disks/bind2 rw,relatime shared:676 - ext2 /dev/loop12 rw

+ # 因为500和1216都是shared类型且是通过bind方式mount在一起的，所以他们属于同一个peer group 435。

+ # 重新挂载disk3和disk4
$ sudo mount ./disk3.img ./disk1/disk3
$ sudo mount ./disk4.img ./disk2/disk4

$ cat /proc/self/mountinfo |grep "disks"
500 29 7:11 / /home/jwang/disks/disk1 rw,relatime shared:435 - ext2 /dev/loop11 rw
1002 29 7:12 / /home/jwang/disks/disk2 rw,relatime - ext2 /dev/loop12 rw
1216 29 7:11 / /home/jwang/disks/bind1 rw,relatime shared:435 - ext2 /dev/loop11 rw
1240 29 7:12 / /home/jwang/disks/bind2 rw,relatime shared:676 - ext2 /dev/loop12 rw
1148 500 7:13 / /home/jwang/disks/disk1/disk3 rw,relatime shared:639 - ext2 /dev/loop13 rw
1192 1216 7:13 / /home/jwang/disks/bind1/disk3 rw,relatime shared:639 - ext2 /dev/loop13 rw
1384 1002 7:14 / /home/jwang/disks/disk2/disk4 rw,relatime - ext2 /dev/loop14 rw

+ #由于disk1/和bind1/属于同一个peer group，
+ #所以在挂载了disk3后，在两个目录下都能看到disk3下的内容
+ #而新生成的挂载点  disk1/disk3 和 bind1/disk3 继承了父挂载点disk1和bind1的 传播类型，由于父挂载点时一个peer group， 则disk1/disk3  和 bind1/disk3 也是一个peer group

$ cat /proc/self/mountinfo |grep disks
1241 1192 7:11 / /home/jwang/disks/disk1 rw,relatime shared:435 - ext2 /dev/loop11 rw
1242 1192 7:12 / /home/jwang/disks/disk2 rw,relatime - ext2 /dev/loop12 rw
1342 1241 7:13 / /home/jwang/disks/disk1/disk3 rw,relatime shared:639 - ext2 /dev/loop13 r
```

#### New mount namespace测试
终端1：
```diff

+ # 进程进入新的mount namespace，检查mountinfo
$ cat /proc/self/mountinfo |grep disks
1241 1192 7:11 / /home/jwang/disks/disk1 rw,relatime shared:435 - ext2 /dev/loop11 rw
1242 1192 7:12 / /home/jwang/disks/disk2 rw,relatime - ext2 /dev/loop12 rw

+ # 内容看上去差不多，但挂载点ID，父挂载点ID都变了，说明是Copy过来的，不是原来namespace的
+ # disk1挂载点的peer group 435，和原namespace里的disk1挂载点是一样的

- # disk3的挂载信息是shared:639
$ cat /proc/self/mountinfo |grep disks
1241 1192 7:11 / /home/jwang/disks/disk1 rw,relatime shared:435 - ext2 /dev/loop11 rw
1242 1192 7:12 / /home/jwang/disks/disk2 rw,relatime - ext2 /dev/loop12 rw
1342 1241 7:13 / /home/jwang/disks/disk1/disk3 rw,relatime shared:639 - ext2 /dev/loop13 rw

+ # 从新namespace 卸载disk3
$ umount disk1/disk3/

$ cat /proc/self/mountinfo |grep disks
1241 1192 7:11 / /home/jwang/disks/disk1 rw,relatime shared:435 - ext2 /dev/loop11 rw
1242 1192 7:12 / /home/jwang/disks/disk2 rw,relatime - ext2 /dev/loop12 rw

```

终端2：
```diff
+ #原挂载点的mountinfo
$ sudo cat /proc/self/mountinfo |grep disks
500 29 7:11 / /home/jwang/disks/disk1 rw,relatime shared:435 - ext2 /dev/loop11 rw
1002 29 7:12 / /home/jwang/disks/disk2 rw,relatime - ext2 /dev/loop12 rw

+ # disk1挂载点下mount新的挂载disk3
$ sudo mount ./disk3.img ./disk1/disk3

+ # disk3的挂载信息是shared:639
$ sudo cat /proc/self/mountinfo |grep disks
500 29 7:11 / /home/jwang/disks/disk1 rw,relatime shared:435 - ext2 /dev/loop11 rw
1002 29 7:12 / /home/jwang/disks/disk2 rw,relatime - ext2 /dev/loop12 rw
1341 500 7:13 / /home/jwang/disks/disk1/disk3 rw,relatime shared:639 - ext2 /dev/loop13 rw

+ # 等待终端1卸载disk3后，检查mountinfo发现，disk3在原mount空间也被卸载了。
$ sudo cat /proc/self/mountinfo |grep disks
500 29 7:11 / /home/jwang/disks/disk1 rw,relatime shared:435 - ext2 /dev/loop11 rw
1002 29 7:12 / /home/jwang/disks/disk2 rw,relatime - ext2 /dev/loop12 rw
```

## Net Namespace

```diff
$ sudo unshare --fork --net bash
$ ifconfig
```
***理解CNI网络接口***

* **`准备工作`** 
```diff
+ # 建立一个--net=none的容器 
$ contid=$(docker run -d --net=none --name mynginx nginx)
$ pid=$(docker inspect -f '{{ .State.Pid }}' $contid)
$ netnspath=/proc/$pid/ns/net

+ # 检查一下该进程namespace下网络
$ nsenter -t $pid -n ip a
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
```

* **`单功能CNI Config`**

建立一个config.json，配置CNI网络
```json
 {
    "cniVersion": "0.4.0",
    "name": "mynet",
    "type": "bridge",
    "bridge": "mynet0",
    "isDefaultGateway": true,
    "forceAddress": false,
    "ipMasq": true,
    "hairpinMode": true,
    "ipam": {
        "type": "host-local",
        "subnet": "10.10.0.0/16"
    }
}
```

运行CNI命令
```diff
$ CNI_COMMAND=ADD CNI_CONTAINERID=$contid CNI_NETNS=$netnspath CNI_IFNAME=eth0 CNI_PATH=~/cni/bin ~/cni/bin/bridge < bridge.json
{
    "cniVersion": "0.4.0",
    "interfaces": [
        {
            "name": "mynet0",
            "mac": "a2:a9:fd:96:07:42"
        },
        {
            "name": "vethb87c58d2",
            "mac": "22:b7:cd:1a:2c:9a"
        },
        {
            "name": "eth0",
            "mac": "3a:ce:9f:3d:8f:27",
            "sandbox": "/proc/54163/ns/net"
        }
    ],
    "ips": [
        {
            "version": "4",
            "interface": 2,
            "address": "10.10.0.2/16",
            "gateway": "10.10.0.1"
        }
    ],
    "routes": [
        {
            "dst": "0.0.0.0/0",
            "gw": "10.10.0.1"
        }
    ],
    "dns": {}
}

+ # 检查网络，可以看到eth0
$ nsenter -t $pid -n ip a
1: lo: <LOOPBACK,UP,LOWER_UP> mtu 65536 qdisc noqueue state UNKNOWN group default qlen 1000
    link/loopback 00:00:00:00:00:00 brd 00:00:00:00:00:00
    inet 127.0.0.1/8 scope host lo
       valid_lft forever preferred_lft forever
2: eth0@if7: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc noqueue state UP group default
    link/ether 3a:ce:9f:3d:8f:27 brd ff:ff:ff:ff:ff:ff link-netnsid 0
    inet 10.10.0.2/16 brd 10.10.255.255 scope global eth0
       valid_lft forever preferred_lft forever

+ # 添加路由
$ ip route add 10.10.0.0/16 dev mynet0 src 10.10.0.1
$ route
Kernel IP routing table
Destination     Gateway         Genmask         Flags Metric Ref    Use Iface
default         _gateway        0.0.0.0         UG    100    0        0 eno1
10.10.0.0       0.0.0.0         255.255.0.0     U     0      0        0 mynet0
link-local      0.0.0.0         255.255.0.0     U     1000   0        0 virbr0
172.17.0.0      0.0.0.0         255.255.0.0     U     0      0        0 docker0
192.168.1.0     0.0.0.0         255.255.255.0   U     100    0        0 eno1
192.168.122.0   0.0.0.0         255.255.255.0   U     0      0        0 virbr0

+ # 访问nginx       
$ curl -I 10.10.0.2
HTTP/1.1 200 OK
Server: nginx/1.21.1
Date: Tue, 05 Oct 2021 03:15:21 GMT
Content-Type: text/html
Content-Length: 612
Last-Modified: Tue, 06 Jul 2021 14:59:17 GMT
Connection: keep-alive
ETag: "60e46fc5-264"
Accept-Ranges: bytes

+ # 清理,注意命令是DEL
$ CNI_COMMAND=DEL CNI_CONTAINERID=$contid CNI_NETNS=$netnspath CNI_IFNAME=eth0 CNI_PATH=~/cni/bin ~/cni/bin/bridge < bridge.json
$ ip link del mynet0
$ docker stop mynginx
$ docker rm nginx
```
 
* **`链式多功能Config`**
```json
+ # 建立一个portmap.conflist配置文件，包含两个插件，bridge和portmap
{
  "cniVersion": "0.4.0",
  "name": "portmap",
  "plugins": [
    {
      "type": "bridge",
      "bridge": "mynet0",
      "isDefaultGateway": true, 
      "forceAddress": false, 
      "ipMasq": true, 
      "hairpinMode": true,
      "ipam": {
        "type": "host-local",
        "subnet": "10.10.0.0/16",
        "gateway": "10.10.0.1"
      }
    },
    {
      "type": "portmap",
      "runtimeConfig": {
        "portMappings": [
          {"hostPort": 8080, "containerPort": 80, "protocol": "tcp"}
        ]
      }
    }
  ]
}
```

执行CNI命令
```diff
$ CNI_PATH=~/cni/bin NETCONFPATH=.  cnitool add portmap $netnspath
{
    "cniVersion": "0.4.0",
    "interfaces": [
        {
            "name": "mynet0",
            "mac": "92:ad:e8:25:6e:a3"
        },
        {
            "name": "vethccea921c",
            "mac": "82:87:12:10:8b:76"
        },
        {
            "name": "eth0",
            "mac": "e6:4f:25:58:6a:ad",
            "sandbox": "/proc/55704/ns/net"
        }
    ],
    "ips": [
        {
            "version": "4",
            "interface": 2,
            "address": "10.10.0.2/16",
            "gateway": "10.10.0.1"
        }
    ],
    "routes": [
        {
            "dst": "0.0.0.0/0",
            "gw": "10.10.0.1"
        }
    ],
    "dns": {}
}

+ // 检查portmap是否成功
$ curl -I http://localhost:8080
HTTP/1.1 200 OK
Server: nginx/1.21.1
Date: Tue, 05 Oct 2021 07:21:30 GMT
Content-Type: text/html
Content-Length: 612
Last-Modified: Tue, 06 Jul 2021 14:59:17 GMT
Connection: keep-alive
ETag: "60e46fc5-264"
Accept-Ranges: bytes

```


## User Namespace

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
