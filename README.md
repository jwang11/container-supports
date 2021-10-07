# container-supports知识准备


## Capabilities

Linux将传统上与root用户关联的特权划分为不同的功能集，这些功能组称为capabilites。

See [here](capabilities)

## Seccomp

Seccomp是linux内核提供的沙箱机制，可以限制进程对系统调用的访问，从系统调用号，到系统调用的参数，都可以检查和限制

See [here](seccomp)
