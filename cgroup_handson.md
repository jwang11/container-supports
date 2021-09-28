### 实战

#### PID Namespace
- 终端1：
```
$ sudo unshare --fork --pid bash

$ echo $$
1
```

- 终端2：
```
$ps -ef|tail 4

root       12660    5954  0 17:17 pts/3    00:00:00 sudo unshare --fork --pid bash
root       12661   12660  0 17:17 pts/3    00:00:00 unshare --fork --pid bash   
```
