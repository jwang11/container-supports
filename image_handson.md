# 实战

## OCI image格式（用docker）
### oci config文件
```diff
$ docker image list
REPOSITORY    TAG       IMAGE ID       CREATED        SIZE
nginx         latest    4cdc5dd7eaad   2 months ago   133MB
busybox       latest    69593048aa3a   3 months ago   1.24MB
hello-world   latest    d1165f221234   6 months ago   13.3kB

+ IMAGE_ID 其实就是image的config的digest(12位）

$cd /var/lib/docker/image/overlay2/
$ cat imagedb/content/sha256/69593048aa3acfee0f75f20b77acb549de2472063053f6730c4091b53f2dfb02 |jq
{
  "architecture": "amd64",
  "config": {
    "Hostname": "",
    "Domainname": "",
    "User": "",
    "AttachStdin": false,
    "AttachStdout": false,
    "AttachStderr": false,
    "Tty": false,
    "OpenStdin": false,
    "StdinOnce": false,
    "Env": [
      "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
    ],
    "Cmd": [
      "sh"
    ],
    "Image": "sha256:34dd1172b5b26c557e72a400b1c9aaf26746efe34ce01023f605cdb4d5870ad3",
    "Volumes": null,
    "WorkingDir": "",
    "Entrypoint": null,
    "OnBuild": null,
    "Labels": null
  },
  "container": "6dd2b340ed3c81fb1f73ff1c7cfa6fde88cf6dd4b0d35f2e7045f2c93ca71481",
  "container_config": {
    "Hostname": "6dd2b340ed3c",
    "Domainname": "",
    "User": "",
    "AttachStdin": false,
    "AttachStdout": false,
    "AttachStderr": false,
    "Tty": false,
    "OpenStdin": false,
    "StdinOnce": false,
    "Env": [
      "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"
    ],
    "Cmd": [
      "/bin/sh",
      "-c",
      "#(nop) ",
      "CMD [\"sh\"]"
    ],
  "Image": "sha256:34dd1172b5b26c557e72a400b1c9aaf26746efe34ce01023f605cdb4d5870ad3",
    "Volumes": null,
    "WorkingDir": "",
    "Entrypoint": null,
    "OnBuild": null,
    "Labels": {}
  },
  "created": "2021-06-07T20:20:29.305375985Z",
  "docker_version": "19.03.12",
  "history": [
    {
      "created": "2021-06-07T20:20:29.115815185Z",
      "created_by": "/bin/sh -c #(nop) ADD file:ab1db978794665f043ca88deb54915fffcc85a30b87600f6a59abaac27055ef8 in / "
    },
    {
      "created": "2021-06-07T20:20:29.305375985Z",
      "created_by": "/bin/sh -c #(nop)  CMD [\"sh\"]",
      "empty_layer": true
    }
  ],
  "os": "linux",
  "rootfs": {
    "type": "layers",
    "diff_ids": [
      "sha256:5b8c72934dfc08c7d2bd707e93197550f06c0751023dabb3a045b723c5e7b373"
    ]
  }
}
```

### 再看一个多层的config (nginx)
```
$ cat imagedb/content/sha256/4cdc5dd7eaadff5080649e8d0014f2f8d36d4ddf2eff2fdf577dd13da85c5d2f |jq
{
  "architecture": "amd64",
  "config": {
    "Hostname": "",
    "Domainname": "",
    "User": "",
    "AttachStdin": false,
    "AttachStdout": false,
    "AttachStderr": false,
    "ExposedPorts": {
      "80/tcp": {}
    },
    "Tty": false,
    "OpenStdin": false,
    "StdinOnce": false,
    "Env": [
      "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin",
      "NGINX_VERSION=1.21.1",
      "NJS_VERSION=0.6.1",
      "PKG_RELEASE=1~buster"
    ],
    "Cmd": [
      "nginx",
      "-g",
      "daemon off;"
    ],
    "Image": "sha256:ade63a6b00b9f5be8d14b801fe6df6f72a34aa695ba01db7380d345ed19cad70",
    "Volumes": null,
    "WorkingDir": "",
    "Entrypoint": [
      "/docker-entrypoint.sh"
    ],
    "OnBuild": null,
    "Labels": {
      "maintainer": "NGINX Docker Maintainers <docker-maint@nginx.com>"
    },
    "StopSignal": "SIGQUIT"
  },
  "container": "c0f61378a01130e56a6072f9409a77921c5089cb2552be7ceab5e44678d46c2b",
  "container_config": {
    "Hostname": "c0f61378a011",
    "Domainname": "",
    "User": "",
    "AttachStdin": false,
    "AttachStdout": false,
    "AttachStderr": false,
    "ExposedPorts": {
      "80/tcp": {}
    },
    "Tty": false,
    "OpenStdin": false,
    "StdinOnce": false,
    "Env": [
      "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin",
      "NGINX_VERSION=1.21.1",
      "NJS_VERSION=0.6.1",
      "PKG_RELEASE=1~buster"
    ],
    "Cmd": [
      "/bin/sh",
      "-c",
      "#(nop) ",
      "CMD [\"nginx\" \"-g\" \"daemon off;\"]"
    ],
    "Image": "sha256:ade63a6b00b9f5be8d14b801fe6df6f72a34aa695ba01db7380d345ed19cad70",
    "Volumes": null,
    "WorkingDir": "",
    "Entrypoint": [
      "/docker-entrypoint.sh"
    ],
    "OnBuild": null,
    "Labels": {
      "maintainer": "NGINX Docker Maintainers <docker-maint@nginx.com>"
    },
    "StopSignal": "SIGQUIT"
  },
  "created": "2021-07-06T19:39:40.385800548Z",
  "docker_version": "20.10.7",
  "history": [
...
  ],
  "os": "linux",
  "rootfs": {
    "type": "layers",
    "diff_ids": [
-     // 每一层是diff_id，也就是该层内容的digest(sha256)    
      "sha256:764055ebc9a7a290b64d17cf9ea550f1099c202d83795aa967428ebdf335c9f7",
      "sha256:ace9ed9bcfafbc909bc3e9451490652f685959db02a4e01e0528a868ee8eab3e",
      "sha256:48b4a40de3597ec0a28c2d4508dec64ae685ed0da77b128d0fb5c69cada91882",
      "sha256:c553c6ba5f1354e1980871b413e057950e0c02d2d7a66b39de2e03836048fda9",
      "sha256:d97733c0a3b64c08bc0dd286926a8eff1b162b4d9fad229eab807c6dc516c172",
      "sha256:9d1af766c81806211d5453b711169103e4f5c3c2609e1dfb9ea4dee7e96a7968"
    ]
  }
}
```
### layerdb里的目录名不是diff_id，而是chain_id。diff_id是保护当前层，用chain_id是保护层与层之间联系
```diff
ls layerdb/sha256/
1a28317a62c8193a2cd9513ed55b4e9beddb850fe397249eea2ee5044192779f/
2c78bcd3187437a7a5d9d8dbf555b3574ba7d143c1852860f9df0a46d5df056a/
2e17f8770b4ad4ce56357edc8dea7329f376803e703201f45141d044634c20e3/
2f06529dbb5e44e14d5ab0d7ecd6d4926db08124dbb1b4cbc75710a3b540aa17/
33d9996f6196b7459c2fff2af3c499c293df089085845401bec2ebda281b86d8/
4311d44226850ccb1c676c541d5a8cbfaa8a8fbbcbce7470f63f73de3b095452/
435c6dad68b58885ad437e5f35f53e071213134eb9e4932b445eac7b39170700/
4e006334a6fdea37622f72b21eb75fe1484fc4f20ce8b8526187d6f7bd90a6fe/
5b8c72934dfc08c7d2bd707e93197550f06c0751023dabb3a045b723c5e7b373/
63d268dd303e176ba45c810247966ff8d1cb9a5bce4a404584087ec01c63de15/
6bb8e6547520704f53cd85d941447e9088a1c9929b2f7357374c4ca423f12a84/
- // 这是nginx第1层，diff_id刚好等于chain_id
764055ebc9a7a290b64d17cf9ea550f1099c202d83795aa967428ebdf335c9f7/
7eab6969712beb0bd29a9c67e028a9c0cb8c623860966d53354e2f2bcb534fdf/
b27eb5bbca70862681631b492735bac31d3c1c558c774aca9c0e36f1b50ba915/
bdf28aff423adfe7c6cb938eced2f19a32efa9fa3922a3c5ddce584b139dc864/
d8cb195252a723d5f1dfe5f168cc1268255f5d53d836a176636bdea1acb39b4b/
ecd3e268e3ec6c7018720438d572bad85c7254440164e0bd326841f70bccdade/
f22b99068db93900abe17f7f5e09ec775c2826ecfe9db961fea68293744144bd/
```

### 以nginx为例，看看diff_id和chain_id推算
>> ChainID(layerN) = SHA256hex(ChainID(layerN-1) + " " + DiffID(layerN))

- 第一层 diff_id = chain_id = sha256:764055ebc9a7a290b64d17cf9ea550f1099c202d83795aa967428ebdf335c9f7

- 第二层 diff_id = sha256:ace9ed9bcfafbc909bc3e9451490652f685959db02a4e01e0528a868ee8eab3e
```diff
$echo -n "sha256:764055ebc9a7a290b64d17cf9ea550f1099c202d83795aa967428ebdf335c9f7 sha256:ace9ed9bcfafbc909bc3e9451490652f685959db02a4e01e0528a868ee8eab3e" | sha256sum
- chain_id是
2c78bcd3187437a7a5d9d8dbf555b3574ba7d143c1852860f9df0a46d5df056a  -
```
chain_id = sha256:2c78bcd3187437a7a5d9d8dbf555b3574ba7d143c1852860f9df0a46d5df056a

- 第三层 diff_id = sha256:48b4a40de3597ec0a28c2d4508dec64ae685ed0da77b128d0fb5c69cada91882
```diff
$echo -n "sha256:2c78bcd3187437a7a5d9d8dbf555b3574ba7d143c1852860f9df0a46d5df056a sha256:48b4a40de3597ec0a28c2d4508dec64ae685ed0da77b128d0fb5c69cada91882" | sha256sum
- chain_id是
bdf28aff423adfe7c6cb938eced2f19a32efa9fa3922a3c5ddce584b139dc864  -
```

- 第四层 diff_id = sha256:c553c6ba5f1354e1980871b413e057950e0c02d2d7a66b39de2e03836048fda9
```diff
$echo -n "sha256:bdf28aff423adfe7c6cb938eced2f19a32efa9fa3922a3c5ddce584b139dc864 sha256:c553c6ba5f1354e1980871b413e057950e0c02d2d7a66b39de2e03836048fda9" | sha256sum
- chain_id 是
435c6dad68b58885ad437e5f35f53e071213134eb9e4932b445eac7b39170700  -
```
- 第五层 diff_id = sha256:d97733c0a3b64c08bc0dd286926a8eff1b162b4d9fad229eab807c6dc516c172
```diff
$echo -n "sha256:435c6dad68b58885ad437e5f35f53e071213134eb9e4932b445eac7b39170700 sha256:d97733c0a3b64c08bc0dd286926a8eff1b162b4d9fad229eab807c6dc516c172" | sha256sum
- chain_id 是
b27eb5bbca70862681631b492735bac31d3c1c558c774aca9c0e36f1b50ba915  -
```
- 第六层 diff_id = sha256:9d1af766c81806211d5453b711169103e4f5c3c2609e1dfb9ea4dee7e96a7968
```diff
$echo -n "sha256:b27eb5bbca70862681631b492735bac31d3c1c558c774aca9c0e36f1b50ba915 sha256:9d1af766c81806211d5453b711169103e4f5c3c2609e1dfb9ea4dee7e96a7968" | sha256sum
- chain_id 是
63d268dd303e176ba45c810247966ff8d1cb9a5bce4a404584087ec01c63de15  -
```

### 通过chain_id找到cache_id。Docker为了安全，没有直接使用chain_id作为层存储目录，而是动态生成一个cache_id，放在chain_id目录下
```diff
$ ls layerdb/sha256/63d268dd303e176ba45c810247966ff8d1cb9a5bce4a404584087ec01c63de15
cache-id  diff  parent  size  tar-split.json.gz

$ cat layerdb/sha256/63d268dd303e176ba45c810247966ff8d1cb9a5bce4a404584087ec01c63de15/cache_id
- 得到cache_id
7c5b0caa88da4897b0d4595a756e09c1c7bcef57531881b02423e9565d1f2ad2

+ 检查/var/lib/docker/overlay2/{$cache_id}
$ ls /var/lib/docker/oerlay2/7c5b0caa88da4897b0d4595a756e09c1c7bcef57531881b02423e9565d1f2ad2
diff  link  lower  work

$ cd /var/lib/docker/oerlay2/7c5b0caa88da4897b0d4595a756e09c1c7bcef57531881b02423e9565d1f2ad2
$ tree diff
diff
└── docker-entrypoint.d
    └── 30-tune-worker-processes.sh

```
## overlayfs实战
```
终端1：
$ docker info
...
 Images: 2
 Server Version: 20.10.2
 Storage Driver: overlay2
  Backing Filesystem: extfs
  Supports d_type: true
  Native Overlay Diff: true

$tree overlayfs-test/
overlayfs-test/
├── Fruits
│   ├── Apple
│   ├── Orange
│   └── Tomato
├── Meat
│   ├── Beef
│   ├── cccc
│   └── Pork
├── Vegetables
│   ├── Carrots
│   └── Tomato
└── work

 $mount -t overlay overlay -o lowerdir=Fruits:Vegetables,upperdir=Meat,workdir=work /mnt/tmp
```
