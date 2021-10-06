# Apparmor实战

## 读写测试

```diff
$ cd apparmor
$ gcc -o test test.c
$ aa-genprof test

+ # prof生成在/etc/apparmor.d/xxxxx.test，

#include <tunables/global>

/home/jwang/github/container-supports/apparmor/test {
  #include <abstractions/base>
}
```

试着写文件
```
$ ./test a.txt "yes"
Open file a.txt with write ERROR
```

加一条规则
```

#include <tunables/global>

/home/jwang/github/container-supports/apparmor/test {
  #include <abstractions/base>
    /home/jwang/github/container-supports/apparmor/a.txt w，
}
```
