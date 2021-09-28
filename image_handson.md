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
