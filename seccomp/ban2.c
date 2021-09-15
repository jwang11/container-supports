#include<unistd.h>
#include<seccomp.h>
#include<sys/syscall.h>

int main()
{
  scmp_filter_ctx ctx;

  // 初始化过滤状态为允许所有系统调用
  ctx = seccomp_init(SCMP_ACT_ALLOW); 
  // 添加需要限制的系统调用
  seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(execve), 0); 
  seccomp_load(ctx);
  
  char * filename = "/bin/sh";
  char * argv[] = { "/bin/sh", NULL };
  char * envp[] = { NULL };
  syscall(__NR_execve, filename, argv, envp);
  
  return 0;
}
