#include <sys/resource.h>
#include <string.h>

void init_core_dump()
{
    struct rlimit limit;
 
    memset(&limit, 0, sizeof(limit));
    // 软限制，16M
    limit.rlim_cur = 16 * 1024 * 1024;
    // 硬限制，这个参数表示对资源没有限制
    limit.rlim_max = RLIM_INFINITY; 
    // RLIMIT_CORE是内核转存文件的最大长度
    setrlimit(RLIMIT_CORE, &limit);
}
 
int main(void)
{
    init_core_dump();
    return 0;
}
