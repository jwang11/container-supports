## Capabilities


### 1. Capabilities是什么

* 在内核2.2之前

    为了检查进程权限，将进程分为两类
    * 特权进程(euid=0)。root用户，或者sudo+cmd，或者直接带有suid标志的程序可以获取完整的root权限来对系统进行操作。
    * 非特权进程（euid!=0)。

* 从内核2.2开始

    Linux将传统上与root用户关联的特权划分为不同的功能组，这些功能组称为capabilites。<br>
    
Capabilites作为线程（进程）的属性存在，每个功能组可以独立启用和禁用。如此一来，权限检查的过程就变成了：<br>
在执行特权操作时，如果进程的有效身份不是root，就去检查是否具有该特权操作所对应的 capabilites，并以此决定是否可以进行该特权操作。比如要向进程发送信号(kill())，就得具有 capability CAP_KILL；如果设置系统时间，就得具有 capability CAP_SYS_TIME。<br>
