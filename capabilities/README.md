## Capabilities


### Capabilities是什么
---
从内核2.2开始，Linux将传统上与root用户关联的特权划分为不同的功能组，这些功能组称为capabilites。<br>
Capabilites作为线程（进程）的属性存在，每个功能组可以独立启用和禁用。 <br>
如此一来，权限检查的过程就变成了：在执行特权操作时，如果进程的有效身份不是root，<br>
就去检查是否具有该特权操作所对应的 capabilites，并以此决定是否可以进行该特权操作。<br>
比如要向进程发送信号(kill())，就得具有 capability CAP_KILL；如果设置系统时间，就得具有 capability CAP_SYS_TIME。<br>
