## 实战

### systemd和cgroup

- 展示层级结构
```diff
$ systemd-cgls
Control group /:
-.slice
├─845 bpfilter_umh
├─user.slice
│ └─user-1000.slice
│   ├─user@1000.service …
│   │ ├─gsd-xsettings.service
│   │ │ └─1521 /usr/libexec/gsd-xsettings
│   │ ├─gvfs-goa-volume-monitor.service
│   │ │ └─974 /usr/libexec/gvfs-goa-volume-monitor
│   │ ├─gsd-power.service
│   │ │ └─1497 /usr/libexec/gsd-power
│   │ ├─xdg-permission-store.service
│   │ │ └─1412 /usr/libexec/xdg-permission-store
│   │ ├─gsd-sound.service
│   │ │ └─1511 /usr/libexec/gsd-sound
...
│     └─23630 pager
├─init.scope
│ └─1 /sbin/init splash
└─system.slice
  ├─irqbalance.service
  │ └─627 /usr/sbin/irqbalance --foreground
  ├─containerd.service …
  │ ├─  762 /usr/bin/containerd
  │ ├─23451 /usr/bin/containerd-shim-runc-v2 -namespace default -id mybox1 -address /run/containerd/containerd.sock
  │ └─23471 /bin/sh
```

- 展示资源配置
```diff
Control Group                                                                    Tasks   %CPU   Memory  Input/s Output/s
user.slice                                                                         276    1.2     1.0G        -        -
/                                                                                  582    1.0     2.2G        -        -
system.slice                                                                       173    0.2   946.6M        -        -
system.slice/containerd.service                                                     36    0.2    90.5M        -        -
system.slice/irqbalance.service                                                      2    0.1   744.0K        -        -
default                                                                              1      -     1.7M        -        -
default/mybox1                                                                       1      -   260.0K        -        -
system.slice/ModemManager.service                                                    3      -     7.0M        -        -
system.slice/NetworkManager.service                                                  3      -    12.5M        -        -
system.slice/accounts-daemon.service                                                 3      -     6.7M        -        -
system.slice/acpid.service                                                           1      -   352.0K        -        -
system.slice/avahi-daemon.service                                                    2      -     1.4M        -        -
system.slice/boot-efi.mount                                                          -      -    24.0K        -        -
system.slice/colord.service                                                          3      -    20.5M        -        -
system.slice/cron.service                                                            1      -   492.0K        -        -
system.slice/cups-browsed.service                                                    3      -     1.9M        -        -
system.slice/cups.service                                                            1      -     1.9M        -        -
system.slice/dbus.service                                                            1      -     4.5M        -        -
system.slice/dev-hugepages.mount                                                     -      -    64.0K        -        -
system.slice/dev-mqueue.mount                                                        -      -     4.0K        -        -
system.slice/docker.service                                                         18      -   134.4M        -        -
system.slice/docker.socket                                                           -      -     4.0K        -        -
system.slice/fwupd.service                                                           5      -     5.1M        -        -
system.slice/gdm.service                                                             3      -     6.0M        -        -
system.slice/kerneloops.service                                                      2      -     1.2M        -        -
system.slice/libvirtd.service                                                       19      -    30.6M        -        -
system.slice/libvirtd.socket                                                         -      -     4.0K        -        -
system.slice/networkd-dispatcher.service                                             1      -    19.1M        -        -
system.slice/nmbd.service                                                            1      -    11.2M        -        -
system.slice/polkit.service                                                          3      -     6.6M        -        -
system.slice/rsyslog.service                                                         4      -     2.9M        -        -
system.slice/rtkit-daemon.service                                                    3      -   588.0K        -        -
system.slice/smbd.service                                                            5      -    16.6M        -        -
system.slice/snap-bare-5.mount                                                       -      -   160.0K        -        -
system.slice/snap-core18-2074.mount                                                  -      -    12.0K        -        -
system.slice/snap-core18-2128.mount                                                  -      -   176.0K        -        -
system.slice/snap-gnome\x2d3\x2d34\x2d1804-66.mount                                  -      -     8.0K        -        -
system.slice/snap-gnome\x2d3\x2d34\x2d1804-72.mount                                  -      -     8.0K        -       
```

- 资源控制
```diff
- #创建控制群组
$ cgcreate -g cpu:/g1
说明：这个命令会创建/sys/fs/cgroup/cpu/g1目录出来，在这个目录下有各种cgroup cpu的配置文件;

- #设置CPU限制参数
$ cgset -r cpu.cfs_quota_us=20000 g1
#查看是否设置成功
$ cgget -r cpu.cfs_quota_us g1

- #启动进程，通过top可以看到cpu占用100%，同时拿到进程pid：2231
$cat t1.sh
$ /bin/bash
x=0
while [ True ];do
    x=$x+1
done;
$ sh  /tmp/t1.sh  &

- #将此进程加入控制群组g1
$ cgclassify -g cpu:/g1 2231
$ cat /sys/fs/cgroup/cpu/g1/tasks

- 通过top看，cpu消耗变成了20%
- 注意：最好将一个进程写在一个控制组内；将多个进程写在一个控制组会共享cpu限制；
- 如：在一个控制组内写入3个进程，cpu限制使用为20%，每个进程大概会占6%左右；
```
