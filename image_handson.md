# 实战

### OCI image格式
- Docker config文件
```
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

- 在看一个多层的config
```
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
### overlayfs实战
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
